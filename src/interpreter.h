#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

typedef enum {
    VAL_INT, VAL_FLOAT, VAL_STRING, VAL_BOOL, VAL_NULL, VAL_FUNCTION, VAL_LIST, VAL_BUILTIN,
    VAL_CLASS, VAL_OBJECT, VAL_METHOD, VAL_PROMISE, VAL_MODULE, VAL_GENERATOR, VAL_TUPLE
} ValueType;

typedef struct Value Value;
typedef struct Env Env;
typedef struct Interpreter Interpreter;

typedef Value (*BuiltinFn)(Interpreter *interp, Value *args, int argc);

typedef struct {
    char **param_names;
    int param_count;
    ASTNode **default_values;
    ASTNode *body;
    Env *closure;
    int is_async;
    int is_generator;
    int has_rest_param;
} FunctionVal;

typedef struct {
    Value *items;
    int count;
    int capacity;
} ListVal;

typedef struct {
    Value *elements;
    int count;
} TupleVal;

typedef struct {
    char *name;
    char *parent_name;
    Env *methods;         // Environment for instance methods
    Env *fields;          // Environment for instance default field values
    Env *static_methods;  // Environment for static methods
    Env *static_fields;   // Environment for static field values
    int is_abstract;      // Flag to track if class is abstract
} ClassVal;

typedef struct {
    char *class_name;
    Env *fields;       // Instance fields
    Env *methods;      // Reference to class methods (shared)
} ObjectVal;

typedef struct {
    Value *receiver;   // The 'this' object (pointer to avoid incomplete type)
    Value *method;     // The function to call (pointer to avoid incomplete type)
} MethodVal;

typedef enum {
    PROMISE_PENDING,
    PROMISE_FULFILLED,
    PROMISE_REJECTED
} PromiseState;

typedef struct PromiseCallbackNode {
    Value *on_fulfilled;       // Function to call on fulfillment (pointer to avoid incomplete type)
    Value *on_rejected;        // Function to call on rejection (pointer to avoid incomplete type)
    Value *promise_to_resolve; // The promise returned by then/catch/finally
    struct PromiseCallbackNode *next;
} PromiseCallbackNode;

typedef struct {
    PromiseState state;
    Value *result;             // Fulfilled value or rejection reason (pointer to avoid incomplete type)
    PromiseCallbackNode *callbacks;  // Linked list of callbacks
} PromiseVal;

typedef struct {
    char *module_path;         // Absolute path to the module
    Env *exports;              // Module's export namespace
    Env *module_env;           // Module's internal environment (kept alive for closures)
} ModuleVal;

typedef enum {
    GEN_RUNNING,
    GEN_SUSPENDED,
    GEN_COMPLETED
} GeneratorState;

typedef struct {
    GeneratorState state;
    FunctionVal *func;         // The generator function
    Env *saved_env;            // Saved environment state
    int yield_index;           // Which yield point we're at
    Value *last_value;         // Last yielded value (pointer to avoid incomplete type)
} GeneratorVal;

struct Value {
    ValueType type;
    union {
        long long int_val;
        double float_val;
        char *str_val;
        int bool_val;
        FunctionVal func_val;
        ListVal list_val;
        TupleVal tuple_val;
        BuiltinFn builtin;
        ClassVal class_val;
        ObjectVal object_val;
        MethodVal method_val;
        PromiseVal promise_val;
        ModuleVal module_val;
        GeneratorVal generator_val;
    } as;
};

typedef struct EnvEntry {
    char *name;
    Value value;
    AccessModifier access;
    DeclType decl_type;
    int is_const;
    struct EnvEntry *next;
} EnvEntry;

struct Env {
    EnvEntry *entries;
    Env *parent;
};

typedef struct {
    int is_return;
    Value return_value;
    int is_break;
    int is_continue;
} EvalResult;

typedef struct MicrotaskNode {
    Value callback;              // Function to execute
    Value *args;                 // Arguments for the function
    int argc;                    // Number of arguments
    struct MicrotaskNode *next;
} MicrotaskNode;

typedef struct LoadedModule {
    char *path;                  // Absolute path to module
    Env *exports;                // Module's export environment
    Env *module_env;             // Module's internal environment (for closures)
    ASTNode **ast_nodes;         // AST nodes (kept alive for function bodies)
    int ast_count;               // Number of AST nodes
    int is_loading;              // Flag to detect circular dependencies
} LoadedModule;

struct Interpreter {
    Env *global_env;
    EvalResult last_result;
    int had_error;
    MicrotaskNode *microtask_queue_head;
    MicrotaskNode *microtask_queue_tail;
    LoadedModule *loaded_modules;  // Array of loaded modules
    int module_count;
    int module_capacity;
    char *current_module_dir;      // Directory of currently executing module
};

Interpreter *interpreter_new(void);
void interpreter_free(Interpreter *interp);
Value eval_node(Interpreter *interp, ASTNode *node);
Value eval_block(Interpreter *interp, ASTNode *block, Env *env);
Value make_int(long long v);
Value make_float(double v);
Value make_string(const char *s);
Value make_bool(int v);
Value make_null(void);
Value make_class(const char *name, const char *parent_name);
Value make_object(const char *class_name, Env *methods);
Value make_method(Value receiver, Value method);
Value make_promise(void);
Value make_module(const char *module_path, Env *exports, Env *module_env);
void value_free(Value *v);
void value_print(Value *v);
char *value_to_string(Value *v);

void microtask_queue_push(Interpreter *interp, Value callback, Value *args, int argc);
void microtask_queue_process(Interpreter *interp);

/* Module system functions */
char *resolve_module_path(Interpreter *interp, const char *import_path);
Value load_module(Interpreter *interp, const char *module_path, Env *env);
LoadedModule *get_cached_module(Interpreter *interp, const char *module_path);
void cache_module(Interpreter *interp, const char *module_path, Env *exports, Env *module_env, ASTNode **ast_nodes, int ast_count);
void set_module_loading(Interpreter *interp, const char *module_path, int loading);
int is_module_loading(Interpreter *interp, const char *module_path);

Env *env_new(Env *parent);
void env_free(Env *env);
Value *env_get(Env *env, const char *name);
EnvEntry *env_get_entry(Env *env, const char *name);
void env_set(Env *env, const char *name, Value val);
void env_set_local(Env *env, const char *name, Value val);
void env_declare(Env *env, const char *name, Value val, DeclType decl_type, int line, Interpreter *interp);
int env_has_local(Env *env, const char *name);

#endif
