//
// This code is part of the Code Genome Framework.
//
// (C) Copyright IBM 2023.
//
// This code is licensed under the Apache License, Version 2.0. You may
// obtain a copy of this license in the LICENSE.txt file in the root directory
// of this source tree or at http://www.apache.org/licenses/LICENSE-2.0.
//
// Any modifications or derivative works of this code must retain this
// copyright notice, and modified files need to carry a notice indicating
// that they have been altered from the originals.
//
#include "canonicalization.hpp"
#include "json.hpp"
#include <chrono>
#include <iostream>

using json = nlohmann::json;
// TODO: Add Smart pointers

// Explicit Instantiations (for the C++ linker)
template ShallowModuleFunctionUserFinder<Function, Module::iterator>::ShallowModuleFunctionUserFinder(iterator_range<Module::iterator>, std::function<bool(const User *, const Function *)>);
template void ShallowModuleFunctionUserFinder<Function, Module::iterator>::print();

template ShallowModuleFunctionUserFinder<GlobalVariable, Module::global_iterator>::ShallowModuleFunctionUserFinder(iterator_range<Module::global_iterator>, std::function<bool(const User *, const Function *)>);
template void ShallowModuleFunctionUserFinder<GlobalVariable, Module::global_iterator>::print();

template <typename T>
std::unordered_map<T *, std::string> original_output_cache;

std::unordered_map<const Function *, std::string> prototype_cache;
std::unordered_set<const Function *> prototype_cache_blacklist;

std::unordered_set<const StructType *> struct_cache_blacklist;
std::unordered_set<const GlobalVariable *> global_cache_blacklist;

template <typename T>
std::unordered_map<T *, std::unordered_map<std::string, Value *>> unavailableNamesInContext_cache;
std::unordered_map<Module *, std::unordered_map<std::string, StructType *>> unavailableNamesInContext_struct_cache;

const Twine function_prefix("gf");
const Twine main_function_prefix("_F");
const Twine global_prefix("gv");
const Twine struct_prefix("t");
const Twine block_prefix("b");
const Twine variable_prefix("v");
const Twine argument_prefix("a");

const int count_start = 1;
const std::string default_filename = "canonicalized.jsonl";

///////////////////////////////////////////////////
/////////////////// CLASSES ///////////////////////
///////////////////////////////////////////////////

/* ShallowModule
ShallowModule is the main class used for holding the information for a single function from the original module
It contains:
 - The main function
 - All of the referenced globals
 - All of the called functions
 - All the used, named struct types
*/

// Constructor from Module, Function and the pre-calculated maps which map all functions to the Values (e.g. globals, other functions) they use
ShallowModule::ShallowModule(Module &M, Function &original_F, FunctionUsersMap<Function> &referencedFunctions, FunctionUsersMap<GlobalVariable> &referencedGlobals, FunctionUsersMap<StructType> &referencedStructs)
{
    debugStream << "Generating a new shallow module" << original_F.getName() << "\n";
    ////// APPENDING MAIN FUNCTION TO SHALLOW MODULE ///////
    setMain(original_F);

    ////// APPENDING CALLED FUNCTION TO SHALLOW MODULE ///////
    if (referencedFunctions.find(mainFunction.second.first) != referencedFunctions.end())
    {
        for (auto const function_ptr : referencedFunctions[mainFunction.second.first])
        {
            appendFunction(*function_ptr);
        }
    }

    /////// PROCESSING THE GLOBALS /////////
    if (referencedGlobals.find(mainFunction.second.first) != referencedGlobals.end())
    {
        for (auto global_ptr : referencedGlobals[mainFunction.second.first])
        {
            appendGlobal(*global_ptr);
        }
    }

    ///// PROCESSING THE STRUCTS /////
    if (referencedStructs.find(mainFunction.second.first) != referencedStructs.end())
    {
        for (auto const struct_ptr : referencedStructs[mainFunction.second.first])
        {
            appendStruct(*struct_ptr);
        }
    }

    ////// RENAMING ////////
    int function_count = count_start;
    int global_count = count_start;
    int struct_count = count_start;
    int main_function_count = count_start;

    // Do not rename if it is a declaration
    if (!mainFunction.second.first->isDeclaration())
    {
        renameModuleValue(*(mainFunction.second.first), &M, main_function_prefix, main_function_count, &temporary_value_renames, false);
    }
    else
    {
        debugStream << "Not renaming '" << mainFunction.second.first->getName() << "' because it is a declaration! Continuing... \n";
    }

    for (auto mapEntry : functions)
    {
        // Do not rename if it is a declaration
        if (mapEntry.second.first->isDeclaration())
        {

            continue;
        }
        renameModuleValue(*(mapEntry.second.first), &M, function_prefix, function_count, &temporary_value_renames);
    }

    for (auto mapEntry : globals)
    {
        renameModuleValue(*(mapEntry.second.first), &M, global_prefix, global_count, &temporary_value_renames);
    }

    for (auto mapEntry : structs)
    {
        renameStruct(*(mapEntry.second.first), &M, struct_prefix, struct_count, &temporary_struct_renames);
    }
}

// Custom destructor to revert all renaming that was done before
ShallowModule::~ShallowModule()
{
    revertNaming();
}

// Setter for the main function
void ShallowModule::setMain(Function &f)
{
    // Used cached function body if possible
    std::string oldString = getOutput(f, true);
    mainFunction = std::pair<std::string, std::pair<Function *, std::string>>(oldString, std::pair<Function *, std::string>(&f, f.getName().str()));
}

// Wrapper to append a single function to the ShallowModule
void ShallowModule::appendFunction(Function &f)
{
    // Used cached function body if possible
    std::string oldString = getOutput(f, true, true);
    functions[oldString] = std::pair<Function *, std::string>(&f, f.getName().str());
}

// Wrapper to append a single global to the ShallowModule
// Firstly: extract all globals for this function with the following information:
// 1) Full String e.g. @global_var_400944 = constant [9 x i8] c"%d\0A%d\0A%d\00" -> SORTING ONLY
// 2) A Pointer to the Global -> This always points us to the global
// 3) The old name e.g. global_var_400944 -> We need this for the renaming
void ShallowModule::appendGlobal(GlobalVariable &g)
{
    // Used cached global body if possible
    std::string oldString = getOutput(g, true);
    globals[oldString] = std::pair<GlobalVariable *, std::string>(&g, g.getName().str());
}

// Wrapper to append a single struct to the ShallowModule
void ShallowModule::appendStruct(StructType &s)
{
    // Used cached struct body if possible
    std::string oldString = getOutputStruct(s, true);
    structs[oldString] = std::pair<StructType *, std::string>(&s, s.getName().str());
}

// Print the modules data (globals, types, prototypes) to a raw_ostream
void ShallowModule::printData(raw_ostream &ostream) const
{
    for (auto i = structs.begin(); i != structs.end(); i++)
    {
        ostream << getOutputStruct(*i->second.first);
        if (std::next(i) != structs.end() || !(globals.empty() && functions.empty()))
        {
            ostream << "\n";
        }
    }

    for (auto i = globals.begin(); i != globals.end(); i++)
    {
        ostream << getOutput(*i->second.first);
        if (std::next(i) != globals.end() || !functions.empty())
        {
            ostream << "\n";
        }
    }

    for (auto i = functions.begin(); i != functions.end(); i++)
    {
        ostream << getOutput(*i->second.first, false, true);
        if (std::next(i) != functions.end())
        {
            ostream << "\n";
        }
    }

    ostream.flush();
}

// Print the whole module to a raw_ostream
void ShallowModule::print(raw_ostream &ostream) const
{
    printData(ostream);
    ostream << "\n";
    ostream << *mainFunction.second.first;
    ostream.flush();
}

// Operator overload << to enable printing by e.g. errs() << shallowMod
raw_ostream &operator<<(raw_ostream &ostream, const ShallowModule &shallowModule)
{
    shallowModule.print(ostream);
    return ostream;
}

// Get a Shallow Module as a string (uses print)
std::string ShallowModule::str()
{
    std::string output;
    raw_string_ostream ostream(output);
    ostream << *this;
    return output;
}

// Get a Shallow Module as a JSON string
std::string ShallowModule::json_str()
{
    json canon;

    std::string data;
    raw_string_ostream data_ostream(data);
    printData(data_ostream);

    bool isExternal = mainFunction.second.first->isDeclaration();

    canon["name"] = mainFunction.second.second;
    canon["data"] = data;
    canon["code"] = getOutput(*mainFunction.second.first);
    canon["extern"] = isExternal;

    return canon.dump();
}

// Revert the naming of all referenced Values in the ShallowModule to the previous names
// This is automatically called by the destructor.
void ShallowModule::revertNaming()
{
    Module *M = mainFunction.second.first->getParent();
    // First, rename the Values which were changed due to name collisions
    for (auto mapEntry : temporary_value_renames)
    {
        setValueNameWithCache<Module>(*mapEntry.first, M, mapEntry.second);
    }

    for (auto mapEntry : temporary_struct_renames)
    {
        setStructNameWithCache(*mapEntry.first, M, mapEntry.second);
    }

    // Then, rename everything else
    setValueNameWithCache<Module>(*mainFunction.second.first, M, mainFunction.second.second);
    for (auto mapEntry : functions)
    {
        setValueNameWithCache<Module>(*mapEntry.second.first, M, mapEntry.second.second);
    }
    for (auto mapEntry : globals)
    {
        setValueNameWithCache<Module>(*mapEntry.second.first, M, mapEntry.second.second);
    }
    for (auto mapEntry : structs)
    {
        setStructNameWithCache(*mapEntry.second.first, M, mapEntry.second.second);
    }
}

/*  ShallowModuleTypeFinder
ShallowModuleTypeFinder is a class used for finding types in the for each function based on the previously generated referenced functions and referenced globals maps
-> The code is basically copied and modified from llvm/IR/Typefinder (which does a similar thing for whole modules)
*/

// Basically run the whole shallow module finder and find every important information
void ShallowModuleTypeFinder::run(Module &M, FunctionUsersMap<Function> functionMap, FunctionUsersMap<GlobalVariable> globalMap)
{
    for (const Function &FI : M.functions())
    {
        // Resetting the members for each function
        VisitedConstants = {};
        VisitedTypes = {};

        // Get types from global variables.
        if (globalMap.find(&FI) != globalMap.end())
        {
            for (const auto G_ptr : globalMap[&FI])
            {
                GlobalVariable &G = *G_ptr;
                incorporateType(G.getValueType(), &FI);
                if (G.hasInitializer())
                    incorporateValue(G.getInitializer(), &FI);
            }
        }

        incorporateType(FI.getFunctionType(), &FI);

        for (const Use &U : FI.operands())
            incorporateValue(U.get(), &FI);

        // First incorporate the arguments.
        for (const auto &A : FI.args())
            incorporateValue(&A, &FI);

        for (const BasicBlock &BB : FI)
            for (const Instruction &I : BB)
            {
                // Incorporate the type of the instruction.
                incorporateType(I.getType(), &FI);

                // Incorporate non-instruction operand types. (We are incorporating all
                // instructions with this loop.)
                for (const auto &O : I.operands())
                    if (&*O && !isa<Instruction>(&*O))
                        incorporateValue(&*O, &FI);
            }
    }

    // Now searching again for the arguments of each function
    for (const Function &FI : M.functions())
    {
        debugStream << "Currently looking for Prototype Blacklist members for: " << FI.getName() << "\n";
        if (StructType *STy = findUnderlyingStructType(FI.getReturnType()))
        {
            debugStream << "Added it!\n";
            insertOrAppendToMap(FunctionArgumentTypesMap, &FI, STy);
            continue;
        }
        for (const auto &A : FI.args())
        {
            if (StructType *STy = findUnderlyingStructType(A.getType()))
            {
                debugStream << "Added it!\n";
                insertOrAppendToMap(FunctionArgumentTypesMap, &FI, STy);
                break;
            }
        }
    }

    printFunctionUsersMap<StructType>(FunctionArgumentTypesMap);

    // Setting the prototype cache blacklist
    for (const auto &mapEntry : FunctionArgumentTypesMap)
    {
        prototype_cache_blacklist.insert(mapEntry.first);
    }

    // Identifying all structs that are in protoypes of called functions
    for (const auto &mapEntry : functionMap)
    {
        const Function *caller = mapEntry.first;
        for (const auto callee : mapEntry.second)
        {
            if (FunctionArgumentTypesMap.find(callee) != FunctionArgumentTypesMap.end())
            {
                FunctionStructsMap[caller].insert(FunctionArgumentTypesMap[callee].begin(), FunctionArgumentTypesMap[callee].end());
            }
        }
    }
}

StructType *findUnderlyingStructType(Type *T)
{
    debugStream << "Searching for an underlying struct type in: " << *T << "\n";
    if (StructType *STy = dyn_cast<StructType>(T))
    {
        return STy;
    }
    else if (PointerType *pointerType = dyn_cast<PointerType>(T))
    {
        return findUnderlyingStructType(pointerType->getPointerElementType());
    }
    else if (ArrayType *arrayType = dyn_cast<ArrayType>(T))
    {
        for (Type *subtype : arrayType->getPointerElementType()->subtypes())
        {
            if (StructType *type = dyn_cast_or_null<StructType>(findUnderlyingStructType(subtype)))
            {
                return type;
            }
        }
    }
    else if (VectorType *vectorType = dyn_cast<VectorType>(T))
    {
        for (Type *subtype : vectorType->getPointerElementType()->subtypes())
        {
            if (StructType *type = dyn_cast_or_null<StructType>(findUnderlyingStructType(subtype)))
            {
                return type;
            }
        }
    }
    return nullptr;
}

/// incorporateType - This method adds the type to the list of used structures
/// if it's not in there already.
void ShallowModuleTypeFinder::incorporateType(Type *Ty, const Function *F)
{
    // Check to see if we've already visited this type.
    if (!VisitedTypes.insert(Ty).second)
    {
        return;
    }

    SmallVector<Type *, 4> TypeWorklist;
    TypeWorklist.push_back(Ty);
    do
    {
        Ty = TypeWorklist.pop_back_val();

        // If this is a structure or opaque type, add a name for the type.
        if (StructType *STy = dyn_cast<StructType>(Ty))
        {
            insertOrAppendToMap(FunctionStructsMap, F, STy);
        }

        // Additionally, check for pointers
        if (StructType *STy = findUnderlyingStructType(Ty))
        {
            insertOrAppendToMap(FunctionStructsMap, F, STy);
        }

        // Add all unvisited subtypes to worklist for processing
        for (Type *SubTy : llvm::reverse(Ty->subtypes()))
            if (VisitedTypes.insert(SubTy).second)
                TypeWorklist.push_back(SubTy);
    } while (!TypeWorklist.empty());
}

/// incorporateValue - This method is used to walk operand lists finding types
/// hiding in constant expressions and other operands that won't be walked in
/// other ways.  GlobalValues, basic blocks, instructions, and inst operands are
/// all explicitly enumerated.
void ShallowModuleTypeFinder::incorporateValue(const Value *V, const Function *F)
{
    if (!isa<Constant>(V) || isa<GlobalValue>(V))
    {
        return;
    }

    // Already visited?
    if (!VisitedConstants.insert(V).second)
    {
        return;
    }

    // Check this type.
    incorporateType(V->getType(), F);

    // If this is an instruction, we incorporate it separately.
    if (isa<Instruction>(V))
    {
        return;
    }

    if (auto *GEP = dyn_cast<GEPOperator>(V))
        incorporateType(GEP->getSourceElementType(), F);

    if (auto *f = dyn_cast<Function>(V))
    {
        if (Type *T = f->getReturnType())
        {
            incorporateType(T, F);
        }
    }
    // Look in operands for types.

    const User *U = cast<User>(V);
    for (const auto &I : U->operands())
    {
        incorporateValue(&*I, F);
    }
}

/*  ShallowModuleFunctionUserFinder
ShallowModuleFunctionUserFinder is a class to find functions which use specific values
Set "condition_func" to further specify when a specific users should be included
*/

template <typename ValueT, typename IteratorT>
ShallowModuleFunctionUserFinder<ValueT, IteratorT>::ShallowModuleFunctionUserFinder(
    iterator_range<IteratorT> iterator, std::function<bool(const User *, const Function *)> condition_func)
{
    this->condition_func = condition_func;
    for (ValueT &value : iterator)
    {
        std::set<const User *> KnownUsers;
        appendUsers(value, value, KnownUsers);
    }
    return;
}

// Find all Function Users of a templated type
// e.g. "Find all functions that use this particular value e.g. this GlobalVariable"
template <typename ValueT, typename IteratorT>
void ShallowModuleFunctionUserFinder<ValueT, IteratorT>::appendUsers(const Value &value, ValueT &reference_value, std::set<const User *> &KnownUsers)
{
    for (const User *user : value.users())
    {
        if (KnownUsers.find(user) != KnownUsers.end())
        {
            continue;
        }
        else
        {
            KnownUsers.insert(user);
        }

        if (auto *instruction = dyn_cast<Instruction>(user))
        {
            // An instruction uses V
            const Function *function_ptr = instruction->getParent()->getParent();

            if (!condition_func(user, function_ptr))
            {
                continue;
            }

            debugStream << "Used by Instruction: " << instruction->getName() << "\n";
            debugStream << "Parent: " << function_ptr->getName() << "\n";

            // Do not add the function for itself
            if (Function *f = dyn_cast<Function>(&reference_value))
            {
                if (function_ptr == f)
                {
                    continue;
                }
            }
            insertOrAppendToMap<ValueT>(UserMap, function_ptr, &reference_value);
        }
        else
        {
            appendUsers(*user, reference_value, KnownUsers); // recursive
        }
    }
    return;
}

template <typename ValueT, typename IteratorT>
void ShallowModuleFunctionUserFinder<ValueT, IteratorT>::print()
{
    printFunctionUsersMap<ValueT>(UserMap);
}

///////////////////////////////////////////////////
/////////////////// FUNCTIONS /////////////////////
///////////////////////////////////////////////////

/////// KICKOFF FUNCTIONS /////////
// Function to rename all Values in the module that do not need to be changed back after any functino has been processed (e.g. variables, blocks, argument)
// Reason: These values are local to each function
// Additionally removes metadata
void renameLocalValues(Module &M)
{
    /////// RENAMING THE MODULE-WIDE ELEMENTS /////////

    // Rename Variables, Blocks Arguments
    for (auto &F : M.functions())
    {
        int block_count = count_start;
        int var_count = count_start;
        int arg_count = count_start;

        for (auto &Arg : F.args())
        {
            renameFunctionValue(Arg, &F, argument_prefix, arg_count);
        }

        for (auto &B : F)
        {
            renameFunctionValue(B, &F, block_prefix, block_count);

            for (auto &I : B)
            {
                if (!I.getType()->isVoidTy())
                {
                    renameFunctionValue(I, &F, variable_prefix, var_count);
                }
                // Removing metadata
                if (I.hasMetadata())
                {
                    SmallVector<std::pair<unsigned, MDNode *>, 4> MDs;
                    I.getAllMetadata(MDs);
                    for (auto &mapEntry : MDs)
                    {
                        I.setMetadata(mapEntry.first, nullptr);
                    }
                }
            }
        }
    }
}

// Remove all attributes in this module
void removeAllAttributes(Module &M)
{
    for (auto &F : M.functions())
    {
        F.setAttributes(AttributeList());
    }
    for (auto &G : M.globals())
    {
        G.setAttributes(AttributeSet());
    }
}

// Cache all the names in the module, this should be run in the beginning
void cacheAllNames(Module &M)
{
    // Struct Names
    unavailableNamesInContext_struct_cache[&M] = std::unordered_map<std::string, StructType *>{};
    for (auto &S : M.getIdentifiedStructTypes())
    {
        unavailableNamesInContext_struct_cache[&M][S->getName().str()] = S;
    }

    // Value Names
    unavailableNamesInContext_cache<Module>[&M] = std::unordered_map<std::string, Value *>{};
    for (auto &G : M.globals())
    {
        unavailableNamesInContext_cache<Module>[&M][G.getName().str()] = &G;
    }
    for (auto &F : M.functions())
    {
        unavailableNamesInContext_cache<Module>[&M][F.getName().str()] = &F;

        unavailableNamesInContext_cache<Function>[&F] = std::unordered_map<std::string, Value *>{};
        for (auto &B : F)
        {
            unavailableNamesInContext_cache<Function>[&F][B.getName().str()] = &B;
            for (auto &Arg : F.args())
            {
                unavailableNamesInContext_cache<Function>[&F][Arg.getName().str()] = &Arg;
            }

            for (auto &I : B)
            {
                if (!I.getType()->isVoidTy())
                {
                    unavailableNamesInContext_cache<Function>[&F][I.getName().str()] = &I;
                }
            }
        }
    }
}

// Fill the blacklists with everything that cannot be cached.
void fillBlacklists(Module &M)
{
    // Now searching for the structs which cannot be cached
    TypeFinder typeFinder;
    typeFinder.run(M, true);
    for (const StructType *STy : typeFinder)
    {
        debugStream << "Currently looking for Struct Blacklist members for: " << STy->getName() << "\n";
        for (Type *T : STy->elements())
        {
            if (findUnderlyingStructType(T))
            {
                debugStream << "Added it!\n";
                struct_cache_blacklist.insert(STy);
                break;
            }
        }
        debugStream << "Struct Blacklist members for: " << STy->getName() << " DONE.\n";
    }



    // Globals
    for (const GlobalVariable &globalVar : M.globals())
    {
        // Check if this global variable is used in any other global variable
        findGlobalUsersRecursively(globalVar, globalVar, 50);

        // Skip, if we already know that that one is blacklisted
        if (global_cache_blacklist.count(&globalVar)) {
            continue;
        }
        debugStream << "Currently looking for Global Blacklist members for: " << globalVar.getName() << "\n";
        if (findUnderlyingStructType(globalVar.getType()))
        {
            global_cache_blacklist.insert(&globalVar);
            debugStream << "Added it!\n";
            continue;
        }
        for (const Use &U : globalVar.operands())
        {
            Value &V = *U.getUser();
            if (findUnderlyingStructType(V.getType()))
            {
                global_cache_blacklist.insert(&globalVar);
                debugStream << "Added it!\n";
                continue;
            }
        }
        debugStream << "Global Blacklist members for: " << globalVar.getName() << " DONE.\n";
    }
}

void findGlobalUsersRecursively(const Value &value, const Value &ref_value, int max_depth)
{
    if( max_depth<=0) return;
    for (const User *user : value.users())
    {
        if (auto *user_gv = dyn_cast<GlobalVariable>(user))
        {
            debugStream << "Value " << ref_value.getName() << " is used by another Global: " << user_gv->getName() << "\n";
            debugStream << "Added that one: " << user_gv->getName() << "\n";
            global_cache_blacklist.insert(user_gv);
        }
        else
        {
            findGlobalUsersRecursively(*user, ref_value, --max_depth); // recursive
        }
    }
}

/////// RENAMING FUNCTIONS /////////
// Rename values which are named in Module scope (globals, functions) with standard prefix and increasing numbers
// Checks for name collisions and resolves them
void renameModuleValue(Value &value, Module *parent, const Twine prefix, int &counter, std::unordered_map<Value *, std::string> *temporary_value_renames, bool use_counter)
{
    std::string name_str;
    if (use_counter)
    {
        name_str = prefix.concat(Twine(counter)).str();
        counter++;
    }
    else
    {
        name_str = prefix.str();
    }

    Twine name(name_str);

    debugStream << "Now renaming '" << value.getName() << "' -> '" << name.str() << "'\n";
    if (value.getName().str() == name.str())
    {
        return;
    }

    if (unavailableNamesInContext_cache<Module>[parent].find(name.str()) != unavailableNamesInContext_cache<Module>[parent].end())
    {
        Value *collider = unavailableNamesInContext_cache<Module>[parent][name.str()];
        debugStream << "Module Value Collision!\n";
        debugStream << "Temporarily renaming '" << *collider << "'\n";
        // Collisions here are worse because a lot of sorting depends on these -> Rename and save for later
        setValueNameWithCache<Module>(*collider, parent, "temp");
        temporary_value_renames->insert(std::pair<Value *, std::string>(collider, collider->getName().str()));
    }
    setValueNameWithCache<Module>(value, parent, name.str());
    return;
}

// Rename values which are named in Function scope (arguments, instructions, blocks) with standard prefix and increasing numbers
// Checks for name collisions and resolves them
void renameFunctionValue(Value &value, Function *parent, const Twine prefix, int &counter)
{
    Twine name = prefix.concat(Twine(counter));
    if (unavailableNamesInContext_cache<Function>[parent].find(name.str()) != unavailableNamesInContext_cache<Function>[parent].end())
    {
        Value *collider = unavailableNamesInContext_cache<Function>[parent][name.str()];
        if (collider != &value)
        {
            debugStream << "Function Value Collision!\n";
            // Collisions are not that bad here because no sorting depends on these values. Just set the temporary variable for the collider and move on.
            setValueNameWithCache<Function>(*collider, parent, "temp");
        }
    }
    setValueNameWithCache<Function>(value, parent, name.str());
    counter++;
    return;
}

// Rename the structs with standard prefix and increasing numbers
// Checks for name collisions and resolves them
void renameStruct(StructType &s, Module *parent, const Twine prefix, int &counter, std::unordered_map<StructType *, std::string> *temporary_struct_renames)
{
    Twine name = prefix.concat(Twine(counter));
    if (unavailableNamesInContext_struct_cache[parent].find(name.str()) != unavailableNamesInContext_struct_cache[parent].end())
    {
        StructType *collider = unavailableNamesInContext_struct_cache[parent][name.str()];
        if (collider != &s)
        {
            debugStream << "Struct Type Name Collision!\n";
            setStructNameWithCache(*collider, parent, "temp");
            temporary_struct_renames->insert(std::pair<StructType *, std::string>(collider, collider->getName().str()));
        }
    }
    setStructNameWithCache(s, parent, name.str());
    counter++;
    return;
}

// Set a Values names while keeping the cache valid
template <typename ParentT>
void setValueNameWithCache(Value &value, ParentT *parent, std::string newName)
{
    unavailableNamesInContext_cache<ParentT>[parent].erase(value.getName().str());
    value.setName(Twine(newName));
    unavailableNamesInContext_cache<ParentT>[parent][newName] = &value;
}

// Set a Structs names while keeping the cache valid
void setStructNameWithCache(StructType &s, Module *parent, std::string newName)
{
    unavailableNamesInContext_struct_cache[parent].erase(s.getName().str());
    s.setName(StringRef(newName));
    unavailableNamesInContext_struct_cache[parent][newName] = &s;
}

/////// GETTING OUTPUT FUNCTIONS /////////
// Get the original output of something (structs, value) via cache or generated
std::string getOutputStruct(StructType &value, bool getOriginal)
{
    if (getOriginal && (original_output_cache<StructType>.find(&value) != original_output_cache<StructType>.end()))
    {
        return original_output_cache<StructType>[&value];
    }
    else if (!getOriginal && !struct_cache_blacklist.count(&value) && (original_output_cache<StructType>.find(&value) != original_output_cache<StructType>.end()))
    {
        std::string output = original_output_cache<StructType>[&value];
        debugStream << "Found cached struct! -> " << output << "\n";
        auto name_start = output.find("%") + 1;
        auto name_end = output.find("=", name_start) - 1;
        if (name_start > name_end)
        {
            errs() << "Struct cache renaming failed. Exiting...";
            exit(1);
        }
        output = output.substr(0, name_start) + value.getName().str() + output.substr(name_end);
        debugStream << "Exchanged the name, new struct: " << output << "\n";
        return output;
    }
    else
    {
        std::string string = generateOutput<StructType>(value);
        if (getOriginal)
        {
            original_output_cache<StructType>[&value] = string;
        }
        return string;
    }
}

std::string getOutput(Value &value, bool getOriginal, bool onlyProtoype)
{
    if (getOriginal && !onlyProtoype && (original_output_cache<Value>.find(&value) != original_output_cache<Value>.end()))
    {
        return original_output_cache<Value>[&value];
    }
    else if (onlyProtoype)
    {
        if (Function *f = dyn_cast<Function>(&value))
        {
            std::string currentOutput;
            debugStream << "Currently stripping function: " << f->getName() << " | Pointer: " << f << "\n";

            // Do we have this prototype cached?
            if (prototype_cache.find(f) != prototype_cache.end() && !prototype_cache_blacklist.count(f))
            {
                currentOutput = prototype_cache[f];
                debugStream << "Found cached prototype! -> " << currentOutput << "\n";

                // Do not change the name if it is a declaration, otherwise replace the name for the current one
                if (!f->isDeclaration())
                {
                    auto name_start = currentOutput.find("@") + 1;
                    auto name_end = currentOutput.find("(", name_start);
                    currentOutput = currentOutput.substr(0, name_start) + f->getName().str() + currentOutput.substr(name_end);

                    debugStream << "Exchanged the name, new prototype: " << currentOutput << "\n";
                }
            }
            else
            {
                // Generate the prototype of a LLVM Value
                currentOutput = generateOutput<Function>(*f, true);
                debugStream << "No cached prototype! Generated new prototype: " << currentOutput << "\n";

                // Check whether this prototype is blacklisted to be on the cache -> Only add that prototype to the cache if it is not
                if (!prototype_cache_blacklist.count(f))
                {
                    prototype_cache[f] = currentOutput;

                    // Debugging output
                    debugStream << "Current state of the prototype cache: \n";
                    for (auto mapEntry : prototype_cache)
                    {
                        debugStream << "Pointer: " << mapEntry.first << " | Prototype: " << mapEntry.second << "\n";
                    }
                }
            }
            debugStream << "Returned after stripping: " << currentOutput << "\n";
            return currentOutput;
        }
        else
        {
            errs() << "You cannot get a prototype for a Value which is not a function!\n";
            exit(1);
        }
    }
    else if (GlobalVariable *gv = dyn_cast<GlobalVariable>(&value))
    {
        if (original_output_cache<Value>.find(gv) != original_output_cache<Value>.end() && !global_cache_blacklist.count(gv))
        {
            std::string output = original_output_cache<Value>[gv];
            debugStream << "Found cached global! -> " << output << "\n";
            auto name_start = output.find("@") + 1;
            auto name_end = output.find("=", name_start) - 1;
            if (name_start > name_end)
            {
                errs() << "Global cache renaming failed. Exiting...";
                exit(1);
            }
            output = output.substr(0, name_start) + value.getName().str() + output.substr(name_end);
            debugStream << "Exchanged the name, new global: " << output << "\n";
            return output;
        }
    }
    std::string string = generateOutput<Value>(value);
    if (getOriginal)
    {
        original_output_cache<Value>[&value] = string;
    }
    return string;
}

// Wrapper to get the full output of any type like it would be printed (must support .print())
// onlyProtoype defaults to false
template <typename T>
std::string generateOutput(T &value, bool onlyProtoype)
{
    std::string output;
    raw_string_ostream ostream(output);
    value.print(ostream);

    debugStream << "Running generateOutput(): got raw output: " << output << "\n";

    if (onlyProtoype)
    {
        output = getFirstLineOfFunction(output);
        auto fun_begin = output.find(" ");
        if (output.find("{") != std::string::npos) {
            output.pop_back();
        }
        output = "declare" + output.substr(fun_begin);
        // TODO: Improve this regex
        output = replaceText("\\ %" + argument_prefix.str() + "[0-9]+", output, "", std::regex_constants::match_any);
        debugStream << "Running generateOutput(): protoype of output: " << output << "\n";
    }
    output = trim(output);
    debugStream << "Running generateOutput(): returning output: " << output << "\n";
    return output;
}

/////// CLEANUP FUNCTIONS ////////

// Cleanup, run at the end
// This resets globals and it specifically important when dealing with multiple files for some reason (e.g. tests)
void cleanup()
{
    original_output_cache<Value> = {};
    original_output_cache<StructType> = {};
    prototype_cache = {};
    prototype_cache_blacklist = {};
    struct_cache_blacklist = {};
    global_cache_blacklist = {};
    unavailableNamesInContext_cache<Module> = {};
    unavailableNamesInContext_cache<Function> = {};
    unavailableNamesInContext_struct_cache = {};
}

/////// HELPER FUNCTIONS ////////

// Helper function to calculate the average of a double vector
float average(std::vector<double> &v)
{
    if (v.empty())
    {
        return 0;
    }

    auto const count = static_cast<float>(v.size());
    double sum = 0.0;
    for (auto value : v)
    {
        sum += value;
    }
    return sum / count;
}

// Helper function to calculate the median of a double vector
float median(std::vector<double> &v)
{
    if (v.empty())
    {
        return 0;
    }

    size_t n = v.size() / 2;
    nth_element(v.begin(), v.begin() + n, v.end());
    return v[n];
}

// Helper function to replace text via regex
std::string replaceText(std::string regex_str, std::string string, std::string replace, std::regex_constants::match_flag_type constant)
{
    std::regex regex(regex_str, std::regex::optimize);
    return std::regex_replace(string, regex, replace, constant);
}

template <typename IteratorT>
int count(iterator_range<IteratorT> iterator)
{
    int counter = 0;
    for (auto &i : iterator)
    {
        counter++;
    }
    return counter;
}

// Helper function to insert a new key with a new set as a value to the map OR append
// another value to set if the key already exists in the map
template <typename T>
void insertOrAppendToMap(FunctionUsersMap<T> &map, const Function *key, T *value)
{
    if (value == nullptr)
    {
        return;
    }

    if (map.find(key) != map.end())
    {
        map.at(key).insert(value);
    }
    else
    {
        map.insert({key, std::set<T *>({value})});
    }
    return;
}

std::string ltrim(const std::string &s, const std::string WHITESPACE)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s, const std::string WHITESPACE)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s)
{
    const std::string WHITESPACE = " \n\r\t\f\v";
    return rtrim(ltrim(s, WHITESPACE), WHITESPACE);
}

bool cmpByStringLength::operator()(const std::string &a, const std::string &b) const
{
    size_t start_a = a.find_first_of("=");
    size_t start_b = b.find_first_of("=");
    std::string new_a;
    std::string new_b;

    if (start_a == std::string::npos)
    {
        new_a = a;
    }
    else
    {
        new_a = a.substr(start_a + 1);
    }

    if (start_b == std::string::npos)
    {
        new_b = b;
    }
    else
    {
        new_b = b.substr(start_b + 1);
    }

    if (new_a.length() != new_b.length())
    {
        return new_a.length() < new_b.length();
    }
    else if (new_a != new_b)
    {
        return new_a < new_b;
    }
    else
    {
        return a < b;
    }
}

std::string getFirstLineOfFunction(std::string f)
{
    f = trim(f);
    if (f.rfind("define", 0) != std::string::npos)
    {
        f = f.substr();
        std::stringstream string_stream(f);
        std::getline(string_stream, f, '\n');
    }
    return f;
}

/////// DEBUG FUNCTIONS ////////

// Debugging function to print contents of FunctionUsersMaps
template <typename T>
void printFunctionUsersMap(FunctionUsersMap<T> &map)
{
    debugStream << "Printing Function Users Map...\n";
    for (const auto &mapEntry : map)
    {
        debugStream << "-> Function: " << mapEntry.first->getName() << "\n";
        for (const auto U : mapEntry.second)
        {
            debugStream << "---> Name: " << U->getName() << " | Pointer: " << U << "\n";
        }
    }
}

///////////////////////////////////////////////////
/////////////////// MAIN //////////////////////////
///////////////////////////////////////////////////

// This struct is neccessary following the typical LLVM Pass structure
namespace
{
    // This add the CLI option canon-out to LLVM opt to set the output file manually
    static cl::opt<std::string> CanonFilename("canon-out", cl::desc("Canonicalized output filename"), cl::value_desc("filename"));
    static cl::opt<std::string> LogFilename("log-out", cl::desc("Log output filename"), cl::value_desc("logfilename"));
    static cl::opt<bool> CanonDebug("canondebug", cl::desc("Enable Debug Mode for CanonPass"), cl::value_desc("canondebug"));

    struct CanonicalizationPass : public ModulePass
    {
        // These IDs are mostly for LLVM internal processing - not important for us
        static char ID;
        CanonicalizationPass() : ModulePass(ID) {}

        // This function is run whenever LLVM calls this pass
        bool runOnModule(Module &M)
        {
            // Benchmarking
            using std::chrono::duration;
            using std::chrono::duration_cast;
            using std::chrono::high_resolution_clock;
            using std::chrono::milliseconds;

            // Set or unset this to enable/disable the debugging output
            debug_disabled = !CanonDebug;

            /////// CONSTANTS /////////
            auto t1 = high_resolution_clock::now();

            // If the user did not set a filename for output, use the default one
            if (CanonFilename.empty())
            {
                CanonFilename = default_filename;
            }

            StringRef filename(CanonFilename);
            std::error_code ec(errno, std::generic_category());
            raw_fd_ostream file(filename, ec);

            // Functions to kick off the module processing and make things easier
            debugStream << "Initializing\n";
            cacheAllNames(M);
            renameLocalValues(M);
            removeAllAttributes(M);
            fillBlacklists(M);

            debugStream << "Mapping functions - OTHER FUNCTIONS\n";
            /////// MAPPING FUNCTION <-> OTHER FUNCTIONS //////
            auto t2 = high_resolution_clock::now();
            ShallowModuleFunctionUserFinder<Function, Module::iterator> userFinderFunctions(M.functions());
            FunctionUsersMap<Function> referencedFunctions = userFinderFunctions.UserMap;
            userFinderFunctions.print();

            /////// MAPPING FUNCTION <-> GLOBALS /////////
            /////// MAPPING FUNCTION <-> GLOBALS /////////
            debugStream << "Mapping functions - \n";
            auto t3 = high_resolution_clock::now();
            // Get all the relevant globals for the current function and write them to a map
            ShallowModuleFunctionUserFinder<GlobalVariable, Module::global_iterator> userFinderGlobals(M.globals());
            FunctionUsersMap<GlobalVariable> referencedGlobals = userFinderGlobals.UserMap;
            userFinderGlobals.print();

            debugStream << "Getting the structs\n";
            //////// GETTING THE STRUCTS //////////
            auto t4 = high_resolution_clock::now();
            ShallowModuleTypeFinder typeFinder;
            typeFinder.run(M, referencedFunctions, referencedGlobals);
            printFunctionUsersMap<StructType>(typeFinder.FunctionStructsMap);

            debugStream << "Creating single modules\n";
            //////// CREATE SINGLE MODULES //////////
            auto t5 = high_resolution_clock::now();

            std::vector<double> shallowModuleTimes{};
            std::vector<double> printTimes{};

            debugStream << "Preparation is done. Starting to generate the Shallow Modules.\n";

            for (auto &original_F : M.functions())
            {
                auto t5a = high_resolution_clock::now();
                ShallowModule shallowTempModule(M, original_F, referencedFunctions, referencedGlobals, typeFinder.FunctionStructsMap);
                auto t5b = high_resolution_clock::now();

                duration<double, std::milli> ms_double = t5b - t5a;
                shallowModuleTimes.push_back(ms_double.count());

                auto t5c = high_resolution_clock::now();
                file << shallowTempModule.json_str() + "\n";
                file.flush();

                auto t5d = high_resolution_clock::now();

                ms_double = t5d - t5c;
                printTimes.push_back(ms_double.count());

                // Revert the renaming of the globals (happens by destructor of ShallowModule)
            }

            debugStream << "Shallow Module generation is done. Cleanup...\n";

            file.close();

            auto t6 = high_resolution_clock::now();

            //////// BENCHMARKING OUTPUT //////////
            duration<double, std::milli> ms_double1 = t2 - t1;
            duration<double, std::milli> ms_double2 = t3 - t2;
            duration<double, std::milli> ms_double3 = t4 - t3;
            duration<double, std::milli> ms_double4 = t5 - t4;
            duration<double, std::milli> ms_double5 = t6 - t5;

            std::fprintf(stderr, "[BENCH] No of Functions in the Module: %zu \n\n", shallowModuleTimes.size());

            std::fprintf(stderr, "[BENCH] Setting Constants, renaming Module-wide variables:  %.3f ms\n", ms_double1.count());
            std::fprintf(stderr, "[BENCH] Finding called functions: %.3f ms\n", ms_double2.count());
            std::fprintf(stderr, "[BENCH] Finding globals recursively: %.3f ms\n", ms_double3.count());
            std::fprintf(stderr, "[BENCH] Finding structs recursively: %.3f ms\n", ms_double4.count());

            std::fprintf(stderr, "[BENCH] Shallow Modules and Printing: %.3f ms\n\n", ms_double5.count());

            std::fprintf(stderr, "[BENCH] Average Shallow Module Time: %.3f ms\n", average(shallowModuleTimes));
            std::fprintf(stderr, "[BENCH] Median Shallow Module Time: %.3f ms\n", median(shallowModuleTimes));
            std::fprintf(stderr, "[BENCH] Average Print Time: %.3f ms\n", average(printTimes));
            std::fprintf(stderr, "[BENCH] Median Print Time: %.3f ms\n", median(shallowModuleTimes));

            if (!LogFilename.empty())
            {
                StringRef logfilename(LogFilename);
                std::error_code ec(errno, std::generic_category());
                raw_fd_ostream logfile(logfilename, ec);

                json log;

                log["bin_id"] = M.getSourceFileName();
                auto prep_time = ms_double1 + ms_double2 + ms_double3 + ms_double4;
                auto dump_time = ms_double5.count() / 1000;
                log["prep_time_secs"] = prep_time.count() / 1000;
                log["dump_time_secs"] = dump_time;
                log["no_of_functions"] = shallowModuleTimes.size();

                logfile << log.dump();
                logfile.close();
            }

            //////// CLEANUP //////////
            cleanup();

            debugStream << "Finished.\n";
            // Ensure to LLVM that this pass does not actually change the read module
            return false;
        }
    };
}

char CanonicalizationPass::ID = 0;

// Change this to change how this pass looks and is used in opt
static RegisterPass<CanonicalizationPass> X("canonicalization", "Canonicalization Pass", false, false);