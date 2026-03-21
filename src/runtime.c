#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Value builtin_print(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc > 0) value_print(&args[0]);
    return make_null();
}

static Value builtin_println(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc > 0) value_print(&args[0]);
    printf("\n");
    return make_null();
}

static Value builtin_input(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    char buf[1024];
    if (argc > 0) value_print(&args[0]);
    if (fgets(buf, sizeof(buf), stdin)) {
        int len = (int)strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
        return make_string(buf);
    }
    return make_string("");
}

static Value builtin_len(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_int(0);
    if (args[0].type == VAL_STRING) return make_int((long long)strlen(args[0].as.str_val));
    if (args[0].type == VAL_LIST) return make_int((long long)args[0].as.list_val.count);
    return make_int(0);
}

static Value builtin_str(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    Value result;
    char *s;
    if (argc == 0) return make_string("");
    s = value_to_string(&args[0]);
    result = make_string(s);
    free(s);
    return result;
}

static Value builtin_int_fn(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_int(0);
    if (args[0].type == VAL_INT) return make_int(args[0].as.int_val);
    if (args[0].type == VAL_FLOAT) return make_int((long long)args[0].as.float_val);
    if (args[0].type == VAL_STRING) return make_int(atoll(args[0].as.str_val));
    if (args[0].type == VAL_BOOL) return make_int(args[0].as.bool_val ? 1 : 0);
    return make_int(0);
}

static Value builtin_float_fn(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    if (args[0].type == VAL_FLOAT) return make_float(args[0].as.float_val);
    if (args[0].type == VAL_INT) return make_float((double)args[0].as.int_val);
    if (args[0].type == VAL_STRING) return make_float(atof(args[0].as.str_val));
    return make_float(0.0);
}

static Value builtin_type(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_string("null");
    switch (args[0].type) {
        case VAL_INT: return make_string("int");
        case VAL_FLOAT: return make_string("float");
        case VAL_STRING: return make_string("string");
        case VAL_BOOL: return make_string("bool");
        case VAL_NULL: return make_string("null");
        case VAL_FUNCTION: return make_string("function");
        case VAL_BUILTIN: return make_string("builtin");
        case VAL_LIST: return make_string("list");
        default: return make_string("unknown");
    }
}

void runtime_init(Interpreter *interp) {
    Value v;
    v.type = VAL_BUILTIN;

    v.as.builtin = builtin_print;
    env_set_local(interp->global_env, "print", v);

    v.as.builtin = builtin_println;
    env_set_local(interp->global_env, "println", v);

    v.as.builtin = builtin_input;
    env_set_local(interp->global_env, "input", v);

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
