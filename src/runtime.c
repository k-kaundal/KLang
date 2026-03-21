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

/* String manipulation functions - Dart-like API */

static Value builtin_uppercase(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0 || args[0].type != VAL_STRING) return make_string("");
    char *result = strdup(args[0].as.str_val);
    for (int i = 0; result[i]; i++) {
        if (result[i] >= 'a' && result[i] <= 'z') {
            result[i] = result[i] - 'a' + 'A';
        }
    }
    Value v = make_string(result);
    free(result);
    return v;
}

static Value builtin_lowercase(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0 || args[0].type != VAL_STRING) return make_string("");
    char *result = strdup(args[0].as.str_val);
    for (int i = 0; result[i]; i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] - 'A' + 'a';
        }
    }
    Value v = make_string(result);
    free(result);
    return v;
}

static Value builtin_trim(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0 || args[0].type != VAL_STRING) return make_string("");
    const char *str = args[0].as.str_val;
    
    /* Find start */
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') str++;
    
    /* Find end */
    int len = strlen(str);
    while (len > 0 && (str[len-1] == ' ' || str[len-1] == '\t' || 
                       str[len-1] == '\n' || str[len-1] == '\r')) {
        len--;
    }
    
    char *result = malloc(len + 1);
    memcpy(result, str, len);
    result[len] = '\0';
    Value v = make_string(result);
    free(result);
    return v;
}

static Value builtin_split(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        Value empty_list;
        empty_list.type = VAL_LIST;
        empty_list.as.list_val.items = NULL;
        empty_list.as.list_val.count = 0;
        empty_list.as.list_val.capacity = 0;
        return empty_list;
    }
    
    const char *str = args[0].as.str_val;
    const char *delim = args[1].as.str_val;
    int delim_len = strlen(delim);
    
    /* Count tokens */
    int count = 1;
    const char *p = str;
    if (delim_len == 0) {
        /* Split into characters */
        count = strlen(str);
    } else {
        while ((p = strstr(p, delim)) != NULL) {
            count++;
            p += delim_len;
        }
    }
    
    /* Allocate list */
    Value result;
    result.type = VAL_LIST;
    result.as.list_val.items = malloc(count * sizeof(Value));
    result.as.list_val.count = count;
    result.as.list_val.capacity = count;
    
    /* Split string */
    int idx = 0;
    
    if (delim_len == 0) {
        /* Split into characters */
        for (int i = 0; str[i]; i++) {
            char buf[2] = {str[i], '\0'};
            result.as.list_val.items[idx++] = make_string(buf);
        }
    } else {
        p = str;
        const char *next;
        while ((next = strstr(p, delim)) != NULL) {
            int len = next - p;
            char *token = malloc(len + 1);
            memcpy(token, p, len);
            token[len] = '\0';
            result.as.list_val.items[idx++] = make_string(token);
            free(token);
            p = next + delim_len;
        }
        /* Last token */
        result.as.list_val.items[idx++] = make_string(p);
    }
    
    return result;
}

static Value builtin_join(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_LIST || args[1].type != VAL_STRING) {
        return make_string("");
    }
    
    ListVal *list = &args[0].as.list_val;
    const char *delim = args[1].as.str_val;
    
    if (list->count == 0) return make_string("");
    
    /* Calculate total length */
    int total_len = 0;
    for (int i = 0; i < list->count; i++) {
        char *s = value_to_string(&list->items[i]);
        total_len += strlen(s);
        free(s);
    }
    total_len += strlen(delim) * (list->count - 1);
    
    /* Build result */
    char *result = malloc(total_len + 1);
    result[0] = '\0';
    
    for (int i = 0; i < list->count; i++) {
        if (i > 0) strcat(result, delim);
        char *s = value_to_string(&list->items[i]);
        strcat(result, s);
        free(s);
    }
    
    Value v = make_string(result);
    free(result);
    return v;
}

static Value builtin_substring(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        return make_string("");
    }
    
    const char *str = args[0].as.str_val;
    int len = strlen(str);
    int start = (int)args[1].as.int_val;
    int end = len;
    
    if (argc >= 3 && args[2].type == VAL_INT) {
        end = (int)args[2].as.int_val;
    }
    
    /* Bounds checking */
    if (start < 0) start = 0;
    if (end > len) end = len;
    if (start >= end) return make_string("");
    
    int sub_len = end - start;
    char *result = malloc(sub_len + 1);
    memcpy(result, str + start, sub_len);
    result[sub_len] = '\0';
    
    Value v = make_string(result);
    free(result);
    return v;
}

static Value builtin_startswith(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        return make_bool(0);
    }
    
    const char *str = args[0].as.str_val;
    const char *prefix = args[1].as.str_val;
    int prefix_len = strlen(prefix);
    
    if (strlen(str) < (size_t)prefix_len) return make_bool(0);
    return make_bool(strncmp(str, prefix, prefix_len) == 0);
}

static Value builtin_endswith(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        return make_bool(0);
    }
    
    const char *str = args[0].as.str_val;
    const char *suffix = args[1].as.str_val;
    int str_len = strlen(str);
    int suffix_len = strlen(suffix);
    
    if (str_len < suffix_len) return make_bool(0);
    return make_bool(strcmp(str + str_len - suffix_len, suffix) == 0);
}

static Value builtin_contains(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        return make_bool(0);
    }
    
    const char *str = args[0].as.str_val;
    const char *substr = args[1].as.str_val;
    
    return make_bool(strstr(str, substr) != NULL);
}

static Value builtin_indexof(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        return make_int(-1);
    }
    
    const char *str = args[0].as.str_val;
    const char *substr = args[1].as.str_val;
    const char *pos = strstr(str, substr);
    
    if (pos == NULL) return make_int(-1);
    return make_int(pos - str);
}

static Value builtin_replace(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 3 || args[0].type != VAL_STRING || 
        args[1].type != VAL_STRING || args[2].type != VAL_STRING) {
        return make_string("");
    }
    
    const char *str = args[0].as.str_val;
    const char *old = args[1].as.str_val;
    const char *new = args[2].as.str_val;
    
    int old_len = strlen(old);
    int new_len = strlen(new);
    
    if (old_len == 0) return make_string(str);
    
    /* Count occurrences */
    int count = 0;
    const char *p = str;
    while ((p = strstr(p, old)) != NULL) {
        count++;
        p += old_len;
    }
    
    if (count == 0) return make_string(str);
    
    /* Allocate result */
    int result_len = strlen(str) + count * (new_len - old_len);
    char *result = malloc(result_len + 1);
    char *dst = result;
    
    /* Replace all occurrences */
    p = str;
    const char *next;
    while ((next = strstr(p, old)) != NULL) {
        int len = next - p;
        memcpy(dst, p, len);
        dst += len;
        memcpy(dst, new, new_len);
        dst += new_len;
        p = next + old_len;
    }
    strcpy(dst, p);
    
    Value v = make_string(result);
    free(result);
    return v;
}

static Value builtin_repeat(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        return make_string("");
    }
    
    const char *str = args[0].as.str_val;
    int times = (int)args[1].as.int_val;
    
    if (times <= 0) return make_string("");
    
    int str_len = strlen(str);
    int result_len = str_len * times;
    char *result = malloc(result_len + 1);
    result[0] = '\0';
    
    for (int i = 0; i < times; i++) {
        strcat(result, str);
    }
    
    Value v = make_string(result);
    free(result);
    return v;
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
