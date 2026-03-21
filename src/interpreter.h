#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

typedef enum {
    VAL_INT, VAL_FLOAT, VAL_STRING, VAL_BOOL, VAL_NULL, VAL_FUNCTION, VAL_LIST, VAL_BUILTIN
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
    } as;
};

typedef struct EnvEntry {
    char *name;
    Value value;
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
void value_free(Value *v);
void value_print(Value *v);
char *value_to_string(Value *v);

Env *env_new(Env *parent);
void env_free(Env *env);
Value *env_get(Env *env, const char *name);
void env_set(Env *env, const char *name, Value val);
void env_set_local(Env *env, const char *name, Value val);

#endif
