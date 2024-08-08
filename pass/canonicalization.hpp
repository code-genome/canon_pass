#ifndef CANONICALIZATION_PASS
#define CANONICALIZATION_PASS

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Twine.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/TypeFinder.h"

#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

// Preprocessor directive to enable or disable debugging
#define debugStream     \
    if (debug_disabled) \
    {                   \
    }                   \
    else                \
        outs() << "[DEBUG] "

using namespace llvm;

extern const Twine function_prefix;
extern const Twine main_function_prefix;
extern const Twine global_prefix;
extern const Twine struct_prefix;
extern const Twine block_prefix;
extern const Twine variable_prefix;
extern const Twine argument_prefix;

struct cmpByStringLength {
    bool operator()(const std::string& a, const std::string& b) const;
};

template <typename T>
using FunctionUsersMap = std::unordered_map<const Function *, std::set<T *>>;
template <typename T, typename Comp>
using OldNamesPointersMap = std::map<std::string, std::pair<T *, std::string>, Comp>;

inline bool debug_disabled;

// Class protoypes //

class ShallowModule
{
public:
    std::pair<std::string, std::pair<Function *, std::string>> mainFunction;
    OldNamesPointersMap<Function, cmpByStringLength> functions;
    OldNamesPointersMap<GlobalVariable, cmpByStringLength> globals;
    OldNamesPointersMap<StructType, cmpByStringLength> structs;

    std::unordered_map<Value *, std::string> temporary_value_renames;
    std::unordered_map<StructType *, std::string> temporary_struct_renames;

    ShallowModule() : functions{}, globals{}, structs{}, temporary_value_renames{}, temporary_struct_renames{} {}

    ShallowModule(Module &M, Function &original_F, FunctionUsersMap<Function> &referencedFunctions, FunctionUsersMap<GlobalVariable> &referencedGlobals, FunctionUsersMap<StructType> &referencedStructs);

    ~ShallowModule();

    void setMain(Function &f);
    void appendFunction(Function &f);
    void appendGlobal(GlobalVariable &g);
    void appendStruct(StructType &S);

    void print(raw_ostream &ostream) const;
    void printData(raw_ostream &ostream) const;
    friend raw_ostream &operator<<(raw_ostream &ostream, const ShallowModule &shallowModule);
    std::string str();
    std::string json_str();

private:
    void revertNaming();
};

/// TypeFinder - Walk over a module, identifying all of the types that are
/// used by the module.
class ShallowModuleTypeFinder
{
    // To avoid walking constant expressions multiple times and other IR
    // objects, we keep several helper maps.
    DenseSet<const Value *> VisitedConstants;
    DenseSet<Type *> VisitedTypes;

public:
    FunctionUsersMap<StructType> FunctionStructsMap;
    FunctionUsersMap<StructType> FunctionArgumentTypesMap;
    ShallowModuleTypeFinder() = default;

    void run(Module &M, FunctionUsersMap<Function> functionMap, FunctionUsersMap<GlobalVariable> globalMap);

protected:
    /// incorporateType - This method adds the type to the list of used
    /// structures if it's not in there already.
    void incorporateType(Type *Ty, const Function *F);

    /// incorporateValue - This method is used to walk operand lists finding types
    /// hiding in constant expressions and other operands that won't be walked in
    /// other ways.  GlobalValues, basic blocks, instructions, and inst operands
    /// are all explicitly enumerated.
    void incorporateValue(const Value *V, const Function *F);
};

template <typename ValueT, typename IteratorT>
class ShallowModuleFunctionUserFinder
{
public:
    FunctionUsersMap<ValueT> UserMap;
    std::function<bool(const User *, const Function *)> condition_func;

    ShallowModuleFunctionUserFinder(
        iterator_range<IteratorT> iterator, std::function<bool(const User *, const Function *)> condition_func = [](const User *u, const Function *) -> bool
        { return true; });

    void print();

private:
    void appendUsers(const Value &value, ValueT &reference_value, std::set<const User *> &KnownUsers);
};

// Function prototypes //

raw_ostream &operator<<(raw_ostream &ostream, const ShallowModule &shallowModule);

// Kickoff
void renameLocalValues(Module &M);
void removeAllAttributes(Module &M);
void cacheAllNames(Module &M);
void fillBlacklists(Module &M);

// Set names
void renameModuleValue(Value &value, Module *parent, const Twine prefix, int &counter, std::unordered_map<Value *, std::string> *temporary_value_renames, bool use_counter = true);
void renameFunctionValue(Value &value, Function *parent, const Twine prefix, int &counter);
void renameStruct(StructType &s, Module *parent, const Twine prefix, int &counter, std::unordered_map<StructType *, std::string> *temporary_struct_renames);

template <typename ParentT>
void setValueNameWithCache(Value &value, ParentT *parent, std::string newName);
void setStructNameWithCache(StructType &s, Module *parent, std::string newName);

// Get output
std::string getOutputStruct(StructType &value, bool getOriginal = false);
std::string getOutput(Value &value, bool getOriginal = false, bool onlyProtoype = false);
template <typename T>
std::string generateOutput(T &value, bool onlyProtoype = false);

// Helpers
std::string replaceText(std::string regex_str, std::string string, std::string replace, std::regex_constants::match_flag_type constant = std::regex_constants::format_first_only);
template <typename T>
void insertOrAppendToMap(FunctionUsersMap<T> &map, const Function *key, T *value);
std::string trim(const std::string &s);
std::string getFirstLineOfFunction(std::string f);
StructType *findUnderlyingStructType(Type *T);
void findGlobalUsersRecursively(const Value &value, const Value &ref_value, int max_depth);

// Debug
template <typename T>
void printFunctionUsersMap(FunctionUsersMap<T> &map);

// Cleanup
void cleanup();

#endif