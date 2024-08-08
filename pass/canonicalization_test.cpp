#include "canonicalization.hpp"
#include "test_data.hpp"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IR/TypeFinder.h"
#include <fstream>
#include <string>

// Protoypes
bool nameStartsWith(Value &V, const std::string &prefix);
void test_globals(ShallowModule &shallow_mod);
void test_correctness_of_module(const std::string &module_string, LLVMContext &Context);
std::unique_ptr<Module> loadModule(const std::string &input, LLVMContext &Context);
void test_sorting(Module &M);

// Functions
void custom_assert(bool check, std::string fail_message)
{
    if (!check)
    {
        std::cerr << "[ERROR]\n";
        std::cerr << fail_message << "\n";
    }
}

// Simple wrapper to check whether a specific string Values name starts with a prefix
bool nameStartsWith(Value &V, const std::string &prefix)
{
    return V.getName().str().rfind(prefix, 0) != std::string::npos;
}

/*
Test Globals:
 - Checks whether all Values in the ShallowModule haven been renamed according to definition (e.g. instructions start with "v" etc.)
 - Positive test
*/
void test_globals(ShallowModule &shallow_mod)
{
    std::cout << "[TEST BEGIN] Checking for @gv and @gf etc.\n";

    for (auto mapEntry : shallow_mod.globals)
    {
        custom_assert(nameStartsWith(*mapEntry.second.first, global_prefix.str()), "[TEST FAILED] Checking for @gv and @gf etc.:\nA global was not correctly renamed");
    }

    if (!shallow_mod.mainFunction.second.first->isDeclaration())
    {
        custom_assert(nameStartsWith(*shallow_mod.mainFunction.second.first, main_function_prefix.str()), "[TEST FAILED] Checking for @gv and @gf etc.:\nThe main function was not correctly renamed");
    }

    for (auto mapEntry : shallow_mod.functions)
    {
        Function &F = *mapEntry.second.first;
        if (!F.isDeclaration())
        {
            custom_assert(nameStartsWith(*mapEntry.second.first, function_prefix.str()), "[TEST FAILED] Checking for @gv and @gf etc.:\nA function was not correctly renamed");
        }

        for (auto &Arg : F.args())
        {
            if (Arg.getName().str() != "")
            {
                custom_assert(nameStartsWith(Arg, argument_prefix.str()), "[TEST FAILED] Checking for @gv and @gf etc.:\nA function argument was not correctly renamed");
            }
        }

        for (auto &B : F)
        {
            custom_assert(nameStartsWith(B, block_prefix.str()), "[TEST FAILED] Checking for @gv and @gf etc.:\nA basic block was not correctly renamed");
            for (auto &I : B)
            {
                if (!I.getType()->isVoidTy())
                {
                    custom_assert(nameStartsWith(I, variable_prefix.str()), "[TEST FAILED] Checking for @gv and @gf etc.:\nAn instruction was not correctly renamed");
                }
            }
        }
    }
    std::cout << "[TEST PASSED] Checking for @gv and @gf etc.\n";
}

/*
Test correctness of module:
 - Tries to parse the output of the shallow module as a new module
 - Simply leverages the LLVM parseIR() function to check that from the string
 - Positive test
*/
void test_correctness_of_module(const std::string &module_string, LLVMContext &Context)
{
    std::cout << "[TEST BEGIN] Parsing the shallow module again\n";
    try
    {
        std::unique_ptr<Module> canonicalizedModuleObject = loadModule(module_string, Context);
        std::cout << "[TEST PASSED] Parsing the shallow module again\n";
    }
    catch (...)
    {
        custom_assert(false, "[TEST FAILED] Parsing the shallow module again:\nCouldn't parse the module\n");
    }
    return;
}

/*
Sorting test case
*/
void test_sorting(Module &M)
{
    cacheAllNames(M);
    removeAllAttributes(M);

    ShallowModuleFunctionUserFinder<Function, Module::iterator> userFinderFunctions(M.functions());
    FunctionUsersMap<Function> referencedFunctions = userFinderFunctions.UserMap;

    ShallowModule shallow_mod{};
    for (auto &F : M.functions())
    {
        shallow_mod.appendFunction(F);
        if (referencedFunctions.find(&F) != referencedFunctions.end())
        {
            for (auto F : referencedFunctions[&F])
            {
                shallow_mod.appendFunction(*F);
            }
        }
    }
    for (auto &G : M.globals())
    {
        shallow_mod.appendGlobal(G);
    }
    TypeFinder type_finder{};
    type_finder.run(M, true);
    for (auto &S : type_finder)
    {
        shallow_mod.appendStruct(*S);
    }

    // This has no meaning and is just here so that the ShallowModule destructor does not segfault
    shallow_mod.setMain(*shallow_mod.functions.begin()->second.first);

    int i = 0;

    std::cout << "[TEST BEGIN] Checking for correct sorting...\n";
    // Now check for matching with the ground truth
    for (auto mapEntry : shallow_mod.functions)
    {
        bool check = mapEntry.first == ir8_functions_sorted[i];
        custom_assert(check, "Function was not correctly sorted: \n" + mapEntry.first + "\n!=\n" + ir8_functions_sorted[i] + "\n");
        i++;
    }
    i = 0;
    for (auto mapEntry : shallow_mod.globals)
    {
        bool check = mapEntry.first == ir8_globals_sorted[i];
        custom_assert(check, "Global was not correctly sorted");
        i++;
    }
    i = 0;
    for (auto mapEntry : shallow_mod.structs)
    {
        bool check = mapEntry.first == ir8_structs_sorted[i];
        custom_assert(check, "Struct was not correctly sorted");
        i++;
    }
    std::cout << "[TEST PASSED] Checking for correct sorting...\n";
}

// Helper function to load a Module object from a string
std::unique_ptr<Module> loadModule(const std::string &input, LLVMContext &Context)
{

    // Parse the input LLVM IR file into a module.
    SMDiagnostic Err;
    MemoryBufferRef buffer(StringRef(input), StringRef("temp_buffer"));
    std::unique_ptr<Module> Mod(parseIR(buffer, Err, Context));

    if (!Mod)
    {
        Err.print(input.c_str(), errs());
        throw std::invalid_argument("[ERROR] Could not parse LLVM Module from file \n");
    }
    return Mod;
}

int main(int argc, char *argv[])
{
    debug_disabled = true;

    std::vector<std::string> irs = {test_ir1, test_ir2, test_ir3, test_ir4, test_ir5, test_ir6, test_ir7, test_ir8};

    if (argc >= 2)
    {
        for (int i = 1; i < argc; i++)
        {
            std::ifstream file(argv[i]);
            std::stringstream buffer;
            buffer << file.rdbuf();
            irs.push_back(buffer.str());
        }
    }

    {
        LLVMContext SourceContext;
        std::cout << "----[NEW TESTFILE]---- IR_SORT"
                  << "\n";
        std::unique_ptr<Module> M(loadModule(test_ir8, SourceContext));
        test_sorting(*M);
        cleanup();
    }

    int counter = 1;
    for (auto ir : irs)
    {
        LLVMContext SourceContext;
        std::cout << "----[NEW TESTFILE]---- IR_" << counter << "\n";
        std::unique_ptr<Module> M(loadModule(ir, SourceContext));

        cacheAllNames(*M);
        renameLocalValues(*M);
        removeAllAttributes(*M);
        fillBlacklists(*M);

        /////// MAPPING FUNCTION <-> OTHER FUNCTIONS //////
        ShallowModuleFunctionUserFinder<Function, Module::iterator> userFinderFunctions(M->functions());
        FunctionUsersMap<Function> referencedFunctions = userFinderFunctions.UserMap;
        userFinderFunctions.print();

        /////// MAPPING FUNCTION <-> GLOBALS /////////
        // Get all the relevant globals for the current function and write them to a map
        ShallowModuleFunctionUserFinder<GlobalVariable, Module::global_iterator> userFinderGlobals(M->globals());
        FunctionUsersMap<GlobalVariable> referencedGlobals = userFinderGlobals.UserMap;
        userFinderGlobals.print();

        //////// GETTING THE STRUCTS //////////
        ShallowModuleTypeFinder typeFinder;
        typeFinder.run(*M, referencedFunctions, referencedGlobals);
        printFunctionUsersMap<StructType>(typeFinder.FunctionStructsMap);

        /// RUNNING TESTS /////
        for (auto &F : *M)
        {
            LLVMContext TargetContext;
            ShallowModule shallowTempModule(*M, F, referencedFunctions, referencedGlobals, typeFinder.FunctionStructsMap);

            test_globals(shallowTempModule);
            test_correctness_of_module(shallowTempModule.str(), TargetContext);
        }
        counter++;
        cleanup();
    }

    return 0;
}
