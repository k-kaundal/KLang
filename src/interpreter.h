#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

typedef enum {
    VAL_INT, VAL_FLOAT, VAL_STRING, VAL_BOOL, VAL_NULL, VAL_FUNCTION, VAL_LIST, VAL_BUILTIN,
    VAL_CLASS, VAL_OBJECT, VAL_METHOD
} ValueType;

typedef struct Value Value;
typedef struct Env Env;
typedef struct Interpreter Interpreter;

typedef Value (*BuiltinFn)(Interpreter *interp, Value *args, int argc);

typedef struct {
    char **param_names;
    int param_count;
    ASTNode *body;
    Env *closure;
} FunctionVal;

typedef struct {
    Value *items;
    int count;
    int capacity;
} ListVal;

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

struct Value {
    ValueType type;
    union {
        long long int_val;
        double float_val;
        char *str_val;
        int bool_val;
        FunctionVal func_val;
        ListVal list_val;
        BuiltinFn builtin;
        ClassVal class_val;
        ObjectVal object_val;
        MethodVal method_val;
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

struct Interpreter {
    Env *global_env;
    EvalResult last_result;
    int had_error;
};

Interpreter *interpreter_new(void);
void interpreter_free(Interpreter *interp);
Value eval_node(Interpreter *interp, ASTNode *node);
Value make_int(long long v);
Value make_float(double v);
Value make_string(const char *s);
Value make_bool(int v);
Value make_null(void);
Value make_class(const char *name, const char *parent_name);
Value make_object(const char *class_name, Env *methods);
Value make_method(Value receiver, Value method);
void value_free(Value *v);
void value_print(Value *v);
char *value_to_string(Value *v);

Env *env_new(Env *parent);
void env_free(Env *env);
Value *env_get(Env *env, const char *name);
EnvEntry *env_get_entry(Env *env, const char *name);
void env_set(Env *env, const char *name, Value val);
void env_set_local(Env *env, const char *name, Value val);
void env_declare(Env *env, const char *name, Value val, DeclType decl_type, int line, Interpreter *interp);
int env_has_local(Env *env, const char *name);

#endif
