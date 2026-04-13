#include "runtime/runtime.h"
#include "runtime/builtins/builtins.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* KLP Protocol Support */
#ifdef ENABLE_KLP
#include "klp_runtime.h"
#endif

/* Forward declarations for helper functions */
extern Value eval_block(Interpreter *interp, ASTNode *block, Env *env);
extern char *value_to_string(Value *v);

/* Helper function to deep copy a value for storage in containers */
Value value_deep_copy(Value v) {
    int i;
    if (v.type == VAL_STRING) {
        return make_string(v.as.str_val);
    } else if (v.type == VAL_TUPLE) {
        /* Deep copy tuple */
        Value tuple_copy;
        tuple_copy.type = VAL_TUPLE;
        tuple_copy.as.tuple_val.count = v.as.tuple_val.count;
        tuple_copy.as.tuple_val.elements = malloc(v.as.tuple_val.count * sizeof(Value));
        for (i = 0; i < v.as.tuple_val.count; i++) {
            tuple_copy.as.tuple_val.elements[i] = value_deep_copy(v.as.tuple_val.elements[i]);
        }
        return tuple_copy;
    } else if (v.type == VAL_LIST) {
        /* Deep copy list */
        Value list_copy;
        list_copy.type = VAL_LIST;
        list_copy.as.list_val.count = v.as.list_val.count;
        list_copy.as.list_val.capacity = v.as.list_val.capacity;
        list_copy.as.list_val.items = malloc(v.as.list_val.capacity * sizeof(Value));
        for (i = 0; i < v.as.list_val.count; i++) {
            list_copy.as.list_val.items[i] = value_deep_copy(v.as.list_val.items[i]);
        }
        return list_copy;
    } else if (v.type == VAL_DICT && v.as.dict_val) {
        /* Increment ref count for shared dict */
        v.as.dict_val->ref_count++;
        return v;
    } else if (v.type == VAL_SET && v.as.set_val) {
        /* Increment ref count for shared set */
        v.as.set_val->ref_count++;
        return v;
    }
    /* For other types, shallow copy is fine */
    return v;
}

void runtime_init(Interpreter *interp) {
    Value v;
    v.type = VAL_BUILTIN;

    /* Core I/O */
    v.as.builtin = builtin_print;
    env_set_local(interp->global_env, "print", v);

    v.as.builtin = builtin_println;
    env_set_local(interp->global_env, "println", v);

    v.as.builtin = builtin_input;
    env_set_local(interp->global_env, "input", v);

    /* Core utilities */
    v.as.builtin = builtin_len;
    env_set_local(interp->global_env, "len", v);

    v.as.builtin = builtin_str;
    env_set_local(interp->global_env, "str", v);

    v.as.builtin = builtin_int_fn;
    env_set_local(interp->global_env, "int", v);

    v.as.builtin = builtin_float_fn;
    env_set_local(interp->global_env, "float", v);

    v.as.builtin = builtin_type;
    env_set_local(interp->global_env, "type", v);
    
    /* String manipulation functions */
    v.as.builtin = builtin_uppercase;
    env_set_local(interp->global_env, "uppercase", v);
    
    v.as.builtin = builtin_lowercase;
    env_set_local(interp->global_env, "lowercase", v);
    
    v.as.builtin = builtin_trim;
    env_set_local(interp->global_env, "trim", v);
    
    v.as.builtin = builtin_split;
    env_set_local(interp->global_env, "split", v);
    
    v.as.builtin = builtin_join;
    env_set_local(interp->global_env, "join", v);
    
    v.as.builtin = builtin_substring;
    env_set_local(interp->global_env, "substring", v);
    
    v.as.builtin = builtin_startswith;
    env_set_local(interp->global_env, "startswith", v);
    
    v.as.builtin = builtin_endswith;
    env_set_local(interp->global_env, "endswith", v);
    
    v.as.builtin = builtin_contains;
    env_set_local(interp->global_env, "contains", v);
    
    v.as.builtin = builtin_indexof;
    env_set_local(interp->global_env, "indexof", v);
    
    v.as.builtin = builtin_replace;
    env_set_local(interp->global_env, "replace", v);
    
    v.as.builtin = builtin_repeat;
    env_set_local(interp->global_env, "repeat", v);
    
    /* Math module functions */
    // Create Math object with constants and methods
    Value math_obj = make_object("Math", NULL);
    
    // Math constants
    Value pi_val = make_float(3.14159265358979323846);
    env_set_local(math_obj.as.object_val->fields, "PI", pi_val);
    
    Value e_val = make_float(2.71828182845904523536);
    env_set_local(math_obj.as.object_val->fields, "E", e_val);
    
    Value tau_val = make_float(6.28318530717958647692);
    env_set_local(math_obj.as.object_val->fields, "TAU", tau_val);
    
    // Math functions as global (can also access via Math.func)
    v.as.builtin = builtin_math_abs;
    env_set_local(interp->global_env, "abs", v);
    env_set_local(math_obj.as.object_val->fields, "abs", v);
    
    v.as.builtin = builtin_math_ceil;
    env_set_local(math_obj.as.object_val->fields, "ceil", v);
    
    v.as.builtin = builtin_math_floor;
    env_set_local(math_obj.as.object_val->fields, "floor", v);
    
    v.as.builtin = builtin_math_round;
    env_set_local(math_obj.as.object_val->fields, "round", v);
    
    v.as.builtin = builtin_math_min;
    env_set_local(interp->global_env, "min", v);
    env_set_local(math_obj.as.object_val->fields, "min", v);
    
    v.as.builtin = builtin_math_max;
    env_set_local(interp->global_env, "max", v);
    env_set_local(math_obj.as.object_val->fields, "max", v);
    
    v.as.builtin = builtin_math_pow;
    env_set_local(math_obj.as.object_val->fields, "pow", v);
    
    v.as.builtin = builtin_math_sqrt;
    env_set_local(math_obj.as.object_val->fields, "sqrt", v);
    
    v.as.builtin = builtin_math_exp;
    env_set_local(math_obj.as.object_val->fields, "exp", v);
    
    v.as.builtin = builtin_math_log;
    env_set_local(math_obj.as.object_val->fields, "log", v);
    
    v.as.builtin = builtin_math_log10;
    env_set_local(math_obj.as.object_val->fields, "log10", v);
    
    v.as.builtin = builtin_math_sin;
    env_set_local(math_obj.as.object_val->fields, "sin", v);
    
    v.as.builtin = builtin_math_cos;
    env_set_local(math_obj.as.object_val->fields, "cos", v);
    
    v.as.builtin = builtin_math_tan;
    env_set_local(math_obj.as.object_val->fields, "tan", v);
    
    v.as.builtin = builtin_math_asin;
    env_set_local(math_obj.as.object_val->fields, "asin", v);
    
    v.as.builtin = builtin_math_acos;
    env_set_local(math_obj.as.object_val->fields, "acos", v);
    
    v.as.builtin = builtin_math_atan;
    env_set_local(math_obj.as.object_val->fields, "atan", v);
    
    v.as.builtin = builtin_math_atan2;
    env_set_local(math_obj.as.object_val->fields, "atan2", v);
    
    v.as.builtin = builtin_math_sinh;
    env_set_local(math_obj.as.object_val->fields, "sinh", v);
    
    v.as.builtin = builtin_math_cosh;
    env_set_local(math_obj.as.object_val->fields, "cosh", v);
    
    v.as.builtin = builtin_math_tanh;
    env_set_local(math_obj.as.object_val->fields, "tanh", v);
    
    env_set_local(interp->global_env, "Math", math_obj);

    /* Array functions */
    v.as.builtin = builtin_range;
    env_set_local(interp->global_env, "range", v);
    
    v.as.builtin = builtin_array_includes;
    env_set_local(interp->global_env, "array_includes", v);
    
    v.as.builtin = builtin_array_push;
    env_set_local(interp->global_env, "array_push", v);
    
    v.as.builtin = builtin_array_pop;
    env_set_local(interp->global_env, "array_pop", v);
    
    v.as.builtin = builtin_array_slice;
    env_set_local(interp->global_env, "array_slice", v);
    
    v.as.builtin = builtin_array_concat;
    env_set_local(interp->global_env, "array_concat", v);
    
    v.as.builtin = builtin_array_join;
    env_set_local(interp->global_env, "array_join", v);
    
    v.as.builtin = builtin_array_reverse;
    env_set_local(interp->global_env, "array_reverse", v);
    
    v.as.builtin = builtin_array_sort;
    env_set_local(interp->global_env, "array_sort", v);
    
    v.as.builtin = builtin_array_map;
    env_set_local(interp->global_env, "array_map", v);
    
    v.as.builtin = builtin_array_filter;
    env_set_local(interp->global_env, "array_filter", v);
    
    v.as.builtin = builtin_array_reduce;
    env_set_local(interp->global_env, "array_reduce", v);
    
    v.as.builtin = builtin_array_forEach;
    env_set_local(interp->global_env, "array_forEach", v);
    
    v.as.builtin = builtin_array_find;
    env_set_local(interp->global_env, "array_find", v);
    
    v.as.builtin = builtin_array_some;
    env_set_local(interp->global_env, "array_some", v);
    
    v.as.builtin = builtin_array_every;
    env_set_local(interp->global_env, "array_every", v);
    
    v.as.builtin = builtin_array_indexOf;
    env_set_local(interp->global_env, "array_indexOf", v);

    /* Regex functions */
    v.as.builtin = builtin_regexTest;
    env_set_local(interp->global_env, "regexTest", v);
    
    v.as.builtin = builtin_regexMatch;
    env_set_local(interp->global_env, "regexMatch", v);
    
    v.as.builtin = builtin_regexReplace;
    env_set_local(interp->global_env, "regexReplace", v);
    
    v.as.builtin = builtin_regexSplit;
    env_set_local(interp->global_env, "regexSplit", v);

    /* File operations */
    v.as.builtin = builtin_fopen;
    env_set_local(interp->global_env, "fopen", v);
    
    v.as.builtin = builtin_fclose;
    env_set_local(interp->global_env, "fclose", v);
    
    v.as.builtin = builtin_fread;
    env_set_local(interp->global_env, "fread", v);
    
    v.as.builtin = builtin_freadline;
    env_set_local(interp->global_env, "freadline", v);
    
    v.as.builtin = builtin_fwrite;
    env_set_local(interp->global_env, "fwrite", v);
    
    v.as.builtin = builtin_fexists;
    env_set_local(interp->global_env, "fexists", v);
    
    v.as.builtin = builtin_fdelete;
    env_set_local(interp->global_env, "fdelete", v);
    
    v.as.builtin = builtin_frename;
    env_set_local(interp->global_env, "frename", v);
    
    v.as.builtin = builtin_fsize;
    env_set_local(interp->global_env, "fsize", v);
    
    v.as.builtin = builtin_readFile;
    env_set_local(interp->global_env, "readFile", v);
    
    v.as.builtin = builtin_writeFile;
    env_set_local(interp->global_env, "writeFile", v);
    
    v.as.builtin = builtin_appendFile;
    env_set_local(interp->global_env, "appendFile", v);

    /* Dictionary functions */
    v.as.builtin = builtin_dict;
    env_set_local(interp->global_env, "dict", v);
    
    v.as.builtin = builtin_dict_set;
    env_set_local(interp->global_env, "dict_set", v);
    
    v.as.builtin = builtin_dict_get;
    env_set_local(interp->global_env, "dict_get", v);
    
    v.as.builtin = builtin_dict_has;
    env_set_local(interp->global_env, "dict_has", v);
    
    v.as.builtin = builtin_dict_delete;
    env_set_local(interp->global_env, "dict_delete", v);
    
    v.as.builtin = builtin_dict_keys;
    env_set_local(interp->global_env, "dict_keys", v);
    
    v.as.builtin = builtin_dict_values;
    env_set_local(interp->global_env, "dict_values", v);

    /* Set functions */
    v.as.builtin = builtin_set;
    env_set_local(interp->global_env, "set", v);
    
    v.as.builtin = builtin_set_add;
    env_set_local(interp->global_env, "set_add", v);
    
    v.as.builtin = builtin_set_remove;
    env_set_local(interp->global_env, "set_remove", v);
    
    v.as.builtin = builtin_set_has;
    env_set_local(interp->global_env, "set_has", v);
    
    v.as.builtin = builtin_set_size;
    env_set_local(interp->global_env, "set_size", v);
    
    v.as.builtin = builtin_set_clear;
    env_set_local(interp->global_env, "set_clear", v);
    
    /* Environment variables */
    Value env_obj = make_object("Env", NULL);
    
    v.as.builtin = builtin_env_get;
    env_set_local(env_obj.as.object_val->fields, "get", v);
    
    v.as.builtin = builtin_env_set;
    env_set_local(env_obj.as.object_val->fields, "set", v);
    
    v.as.builtin = builtin_env_has;
    env_set_local(env_obj.as.object_val->fields, "has", v);
    
    env_set_local(interp->global_env, "Env", env_obj);
    
    /* AI-Native Functions: HTTP/Network */
    // Create http object
    Value http_obj = make_object("http", NULL);
    
    v.as.builtin = builtin_http_get;
    env_set_local(http_obj.as.object_val->fields, "get", v);
    
    v.as.builtin = builtin_http_post;
    env_set_local(http_obj.as.object_val->fields, "post", v);
    
    v.as.builtin = builtin_http_request;
    env_set_local(http_obj.as.object_val->fields, "request", v);
    
    env_set_local(interp->global_env, "http", http_obj);
    
    /* AI-Native Functions: JSON Streaming */
    v.as.builtin = builtin_json_parse_stream;
    env_set_local(interp->global_env, "__json_parse_stream", v);
    
    // Create Promise "class" with static methods
    Value promise_class = make_class("Promise", NULL);
    v.as.builtin = builtin_Promise_resolve;
    env_set_local(promise_class.as.class_val.static_methods, "resolve", v);
    v.as.builtin = builtin_Promise_reject;
    env_set_local(promise_class.as.class_val.static_methods, "reject", v);
    env_set_local(interp->global_env, "Promise", promise_class);
    
    /* Promise instance methods */
    v.as.builtin = builtin_Promise_then;
    env_set_local(interp->global_env, "__promise_then", v);
    
    v.as.builtin = builtin_Promise_catch;
    env_set_local(interp->global_env, "__promise_catch", v);
    
    v.as.builtin = builtin_Promise_finally;
    env_set_local(interp->global_env, "__promise_finally", v);
    
    /* Manual Memory Management Functions (C/C++ Compatibility) */
    v.as.builtin = builtin_malloc;
    env_set_local(interp->global_env, "malloc", v);
    
    v.as.builtin = builtin_calloc;
    env_set_local(interp->global_env, "calloc", v);
    
    v.as.builtin = builtin_realloc;
    env_set_local(interp->global_env, "realloc", v);
    
    v.as.builtin = builtin_free;
    env_set_local(interp->global_env, "free", v);
    
    v.as.builtin = builtin_sizeof;
    env_set_local(interp->global_env, "sizeof", v);
    
    v.as.builtin = builtin_memstat;
    env_set_local(interp->global_env, "memstat", v);
    
    /* KLP Protocol Module */
#ifdef ENABLE_KLP
    // Create klp module object
    Value klp_obj = make_object("KLP", NULL);
    
    // Server functions
    v.as.builtin = builtin_klp_server;
    env_set_local(klp_obj.as.object_val->fields, "server", v);
    
    // Client functions
    v.as.builtin = builtin_klp_connect;
    env_set_local(klp_obj.as.object_val->fields, "connect", v);
    
    // Add klp module to global namespace
    env_set_local(interp->global_env, "klp", klp_obj);
    
    // Initialize KLP runtime
    klp_runtime_init(interp);
#endif
}

void runtime_free(Interpreter *interp) {
    (void)interp;
}

void klang_error(const char *msg, int line) {
    fprintf(stderr, "Error at line %d: %s\n", line, msg);
    exit(1);
}

void klang_panic(const char *msg) {
    fprintf(stderr, "Panic: %s\n", msg);
    exit(1);
}
