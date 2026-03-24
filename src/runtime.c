#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <regex.h>

/* Forward declarations for helper functions */
extern Value eval_block(Interpreter *interp, ASTNode *block, Env *env);
extern char *value_to_string(Value *v);

/* Helper function to deep copy a value for storage in containers */
static Value value_deep_copy(Value v) {
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

static Value builtin_print(Interpreter *interp, Value *args, int argc) {
    int i;
    (void)interp;
    for (i = 0; i < argc; i++) {
        if (i > 0) printf(" ");
        value_print(&args[i]);
    }
    return make_null();
}

static Value builtin_println(Interpreter *interp, Value *args, int argc) {
    int i;
    (void)interp;
    for (i = 0; i < argc; i++) {
        if (i > 0) printf(" ");
        value_print(&args[i]);
    }
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
    if (args[0].type == VAL_TUPLE) return make_int((long long)args[0].as.tuple_val.count);
    if (args[0].type == VAL_DICT) return make_int((long long)args[0].as.dict_val->count);
    if (args[0].type == VAL_SET) return make_int((long long)args[0].as.set_val->count);
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
        case VAL_TUPLE: return make_string("tuple");
        case VAL_DICT: return make_string("dict");
        case VAL_SET: return make_string("set");
        case VAL_OBJECT: return make_string("object");
        case VAL_CLASS: return make_string("class");
        case VAL_GENERATOR: return make_string("generator");
        case VAL_PROMISE: return make_string("promise");
        case VAL_FILE: return make_string("file");
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
    char *ptr = result;
    
    for (int i = 0; i < list->count; i++) {
        if (i > 0) {
            int delim_len = strlen(delim);
            memcpy(ptr, delim, delim_len);
            ptr += delim_len;
        }
        char *s = value_to_string(&list->items[i]);
        int s_len = strlen(s);
        memcpy(ptr, s, s_len);
        ptr += s_len;
        free(s);
    }
    *ptr = '\0';
    
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
    char *ptr = result;
    
    for (int i = 0; i < times; i++) {
        memcpy(ptr, str, str_len);
        ptr += str_len;
    }
    *ptr = '\0';
    
    Value v = make_string(result);
    free(result);
    return v;
}

/* ============================================================
 * MATH MODULE - Comprehensive mathematical functions
 * ============================================================ */

/* Basic arithmetic functions */
static Value builtin_math_abs(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    if (args[0].type == VAL_INT) {
        long long val = args[0].as.int_val;
        return make_int(val < 0 ? -val : val);
    }
    if (args[0].type == VAL_FLOAT) {
        return make_float(fabs(args[0].as.float_val));
    }
    return make_float(0.0);
}

static Value builtin_math_ceil(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(ceil(val));
}

static Value builtin_math_floor(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(floor(val));
}

static Value builtin_math_round(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(round(val));
}

static Value builtin_math_min(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double min_val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    for (int i = 1; i < argc; i++) {
        double val = (args[i].type == VAL_FLOAT) ? args[i].as.float_val : (double)args[i].as.int_val;
        if (val < min_val) min_val = val;
    }
    return make_float(min_val);
}

static Value builtin_math_max(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double max_val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    for (int i = 1; i < argc; i++) {
        double val = (args[i].type == VAL_FLOAT) ? args[i].as.float_val : (double)args[i].as.int_val;
        if (val > max_val) max_val = val;
    }
    return make_float(max_val);
}

/* Power and exponential functions */
static Value builtin_math_pow(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) return make_float(0.0);
    double base = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    double exp = (args[1].type == VAL_FLOAT) ? args[1].as.float_val : (double)args[1].as.int_val;
    return make_float(pow(base, exp));
}

static Value builtin_math_sqrt(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(sqrt(val));
}

static Value builtin_math_exp(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(1.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(exp(val));
}

static Value builtin_math_log(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(log(val));
}

static Value builtin_math_log10(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(log10(val));
}

/* Trigonometric functions */
static Value builtin_math_sin(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(sin(val));
}

static Value builtin_math_cos(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(1.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(cos(val));
}

static Value builtin_math_tan(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(tan(val));
}

static Value builtin_math_asin(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(asin(val));
}

static Value builtin_math_acos(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(acos(val));
}

static Value builtin_math_atan(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(atan(val));
}

static Value builtin_math_atan2(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) return make_float(0.0);
    double y = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    double x = (args[1].type == VAL_FLOAT) ? args[1].as.float_val : (double)args[1].as.int_val;
    return make_float(atan2(y, x));
}

/* Hyperbolic functions */
static Value builtin_math_sinh(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(sinh(val));
}

static Value builtin_math_cosh(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(1.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(cosh(val));
}

static Value builtin_math_tanh(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(tanh(val));
}

/* ============================================================
 * END MATH MODULE
 * ============================================================ */

/* ============================================================
 * RANGE FUNCTION - Python-style range iterator
 * ============================================================ */

static Value builtin_range(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    long long start = 0, stop = 0, step = 1;
    
    if (argc == 0) {
        // range() with no arguments returns empty list
        Value result;
        result.type = VAL_LIST;
        result.as.list_val.count = 0;
        result.as.list_val.capacity = 0;
        result.as.list_val.items = NULL;
        return result;
    } else if (argc == 1) {
        // range(stop) -> range(0, stop, 1)
        stop = (args[0].type == VAL_INT) ? args[0].as.int_val : (long long)args[0].as.float_val;
    } else if (argc == 2) {
        // range(start, stop) -> range(start, stop, 1)
        start = (args[0].type == VAL_INT) ? args[0].as.int_val : (long long)args[0].as.float_val;
        stop = (args[1].type == VAL_INT) ? args[1].as.int_val : (long long)args[1].as.float_val;
    } else {
        // range(start, stop, step)
        start = (args[0].type == VAL_INT) ? args[0].as.int_val : (long long)args[0].as.float_val;
        stop = (args[1].type == VAL_INT) ? args[1].as.int_val : (long long)args[1].as.float_val;
        step = (args[2].type == VAL_INT) ? args[2].as.int_val : (long long)args[2].as.float_val;
    }
    
    // Handle invalid step
    if (step == 0) {
        fprintf(stderr, "ValueError: range() step argument must not be zero\n");
        Value result;
        result.type = VAL_LIST;
        result.as.list_val.count = 0;
        result.as.list_val.capacity = 0;
        result.as.list_val.items = NULL;
        return result;
    }
    
    // Calculate count
    long long count = 0;
    if (step > 0) {
        if (stop > start) {
            count = (stop - start + step - 1) / step;
        }
    } else {
        if (stop < start) {
            count = (start - stop - step - 1) / (-step);
        }
    }
    
    // Create result list
    Value result;
    result.type = VAL_LIST;
    result.as.list_val.count = (int)count;
    result.as.list_val.capacity = (int)count;
    result.as.list_val.items = malloc((count > 0 ? count : 1) * sizeof(Value));
    
    // Fill the list
    long long current = start;
    for (int i = 0; i < count; i++) {
        result.as.list_val.items[i] = make_int(current);
        current += step;
    }
    
    return result;
}

/* ============================================================
 * END RANGE FUNCTION
 * ============================================================ */

/* Helper function to call a callback function with arguments */
static Value call_function(Interpreter *interp, Value *func, Value *args, int argc) {
    if (func->type != VAL_FUNCTION) {
        fprintf(stderr, "Error: callback is not a function\n");
        return make_null();
    }
    
    // Create call environment
    Env *call_env = env_new(func->as.func_val.closure);
    
    // Bind parameters
    for (int i = 0; i < func->as.func_val.param_count && i < argc; i++) {
        env_set_local(call_env, func->as.func_val.param_names[i], args[i]);
    }
    
    // Execute function body
    Value result = eval_block(interp, func->as.func_val.body, call_env);
    env_free(call_env);
    
    // Handle return value
    if (interp->last_result.is_return) {
        value_free(&result);
        result = interp->last_result.return_value;
        interp->last_result.is_return = 0;
        interp->last_result.return_value = make_null();
    }
    
    return result;
}

/* Array Methods - Priority 1 */

/* array.map(callback) - Transform each element */
static Value builtin_array_map(Interpreter *interp, Value *args, int argc) {
    if (argc < 2 || args[0].type != VAL_LIST || args[1].type != VAL_FUNCTION) {
        Value empty_list;
        empty_list.type = VAL_LIST;
        empty_list.as.list_val.items = NULL;
        empty_list.as.list_val.count = 0;
        empty_list.as.list_val.capacity = 0;
        return empty_list;
    }
    
    Value array = args[0];
    Value callback = args[1];
    int count = array.as.list_val.count;
    
    // Create result array
    Value result;
    result.type = VAL_LIST;
    result.as.list_val.count = count;
    result.as.list_val.capacity = count;
    result.as.list_val.items = malloc((count > 0 ? count : 1) * sizeof(Value));
    
    // Map each element
    for (int i = 0; i < count; i++) {
        Value callback_args[3];
        callback_args[0] = array.as.list_val.items[i];
        callback_args[1] = make_int(i);
        callback_args[2] = array;
        
        Value mapped = call_function(interp, &callback, callback_args, 3);
        result.as.list_val.items[i] = mapped;
        
        // Memory management for callback arguments:
        // - callback_args[0]: Reference to array element, don't free
        // - callback_args[1]: Created integer index, must free
        // - callback_args[2]: Reference to array, don't free
        value_free(&callback_args[1]);
    }
    
    return result;
}

/* array.filter(callback) - Filter elements by condition */
static Value builtin_array_filter(Interpreter *interp, Value *args, int argc) {
    if (argc < 2 || args[0].type != VAL_LIST || args[1].type != VAL_FUNCTION) {
        Value empty_list;
        empty_list.type = VAL_LIST;
        empty_list.as.list_val.items = NULL;
        empty_list.as.list_val.count = 0;
        empty_list.as.list_val.capacity = 0;
        return empty_list;
    }
    
    Value array = args[0];
    Value callback = args[1];
    int count = array.as.list_val.count;
    
    // First pass: count matching elements
    int match_count = 0;
    for (int i = 0; i < count; i++) {
        Value callback_args[3];
        callback_args[0] = array.as.list_val.items[i];
        callback_args[1] = make_int(i);
        callback_args[2] = array;
        
        Value test_result = call_function(interp, &callback, callback_args, 3);
        
        int truthy = 0;
        if (test_result.type == VAL_BOOL) truthy = test_result.as.bool_val;
        else if (test_result.type == VAL_INT) truthy = test_result.as.int_val != 0;
        else if (test_result.type == VAL_FLOAT) truthy = test_result.as.float_val != 0.0;
        
        if (truthy) match_count++;
        
        value_free(&test_result);
        value_free(&callback_args[1]);
    }
    
    // Create result array
    Value result;
    result.type = VAL_LIST;
    result.as.list_val.count = match_count;
    result.as.list_val.capacity = match_count;
    result.as.list_val.items = malloc((match_count > 0 ? match_count : 1) * sizeof(Value));
    
    // Second pass: collect matching elements
    int result_idx = 0;
    for (int i = 0; i < count; i++) {
        Value callback_args[3];
        callback_args[0] = array.as.list_val.items[i];
        callback_args[1] = make_int(i);
        callback_args[2] = array;
        
        Value test_result = call_function(interp, &callback, callback_args, 3);
        
        int truthy = 0;
        if (test_result.type == VAL_BOOL) truthy = test_result.as.bool_val;
        else if (test_result.type == VAL_INT) truthy = test_result.as.int_val != 0;
        else if (test_result.type == VAL_FLOAT) truthy = test_result.as.float_val != 0.0;
        
        if (truthy) {
            // Copy the element (need to handle strings specially)
            if (array.as.list_val.items[i].type == VAL_STRING) {
                result.as.list_val.items[result_idx] = make_string(array.as.list_val.items[i].as.str_val);
            } else {
                result.as.list_val.items[result_idx] = array.as.list_val.items[i];
            }
            result_idx++;
        }
        
        value_free(&test_result);
        value_free(&callback_args[1]);
    }
    
    return result;
}

/* array.reduce(callback, initial) - Reduce to single value */
static Value builtin_array_reduce(Interpreter *interp, Value *args, int argc) {
    if (argc < 2 || args[0].type != VAL_LIST || args[1].type != VAL_FUNCTION) {
        return make_null();
    }
    
    Value array = args[0];
    Value callback = args[1];
    int count = array.as.list_val.count;
    
    if (count == 0) {
        // If no initial value and empty array, return null
        if (argc < 3) return make_null();
        // Otherwise return initial value
        return argc >= 3 ? args[2] : make_null();
    }
    
    Value accumulator;
    int start_idx = 0;
    
    if (argc >= 3) {
        // Initial value provided
        if (args[2].type == VAL_STRING) {
            accumulator = make_string(args[2].as.str_val);
        } else {
            accumulator = args[2];
        }
    } else {
        // Use first element as initial
        if (array.as.list_val.items[0].type == VAL_STRING) {
            accumulator = make_string(array.as.list_val.items[0].as.str_val);
        } else {
            accumulator = array.as.list_val.items[0];
        }
        start_idx = 1;
    }
    
    // Reduce
    for (int i = start_idx; i < count; i++) {
        Value callback_args[4];
        callback_args[0] = accumulator;
        callback_args[1] = array.as.list_val.items[i];
        callback_args[2] = make_int(i);
        callback_args[3] = array;
        
        Value new_accumulator = call_function(interp, &callback, callback_args, 4);
        value_free(&accumulator);
        accumulator = new_accumulator;
        
        value_free(&callback_args[2]);
    }
    
    return accumulator;
}

/* array.forEach(callback) - Iterate over elements */
static Value builtin_array_forEach(Interpreter *interp, Value *args, int argc) {
    if (argc < 2 || args[0].type != VAL_LIST || args[1].type != VAL_FUNCTION) {
        return make_null();
    }
    
    Value array = args[0];
    Value callback = args[1];
    int count = array.as.list_val.count;
    
    // Execute callback for each element
    for (int i = 0; i < count; i++) {
        Value callback_args[3];
        callback_args[0] = array.as.list_val.items[i];
        callback_args[1] = make_int(i);
        callback_args[2] = array;
        
        Value result = call_function(interp, &callback, callback_args, 3);
        value_free(&result);
        value_free(&callback_args[1]);
    }
    
    return make_null();
}

/* array.find(callback) - Find first matching element */
static Value builtin_array_find(Interpreter *interp, Value *args, int argc) {
    if (argc < 2 || args[0].type != VAL_LIST || args[1].type != VAL_FUNCTION) {
        return make_null();
    }
    
    Value array = args[0];
    Value callback = args[1];
    int count = array.as.list_val.count;
    
    // Find first matching element
    for (int i = 0; i < count; i++) {
        Value callback_args[3];
        callback_args[0] = array.as.list_val.items[i];
        callback_args[1] = make_int(i);
        callback_args[2] = array;
        
        Value test_result = call_function(interp, &callback, callback_args, 3);
        
        int truthy = 0;
        if (test_result.type == VAL_BOOL) truthy = test_result.as.bool_val;
        else if (test_result.type == VAL_INT) truthy = test_result.as.int_val != 0;
        else if (test_result.type == VAL_FLOAT) truthy = test_result.as.float_val != 0.0;
        
        value_free(&test_result);
        value_free(&callback_args[1]);
        
        if (truthy) {
            // Return copy of found element
            if (array.as.list_val.items[i].type == VAL_STRING) {
                return make_string(array.as.list_val.items[i].as.str_val);
            } else {
                return array.as.list_val.items[i];
            }
        }
    }
    
    return make_null();
}

/* array.some(callback) - Check if any element matches */
static Value builtin_array_some(Interpreter *interp, Value *args, int argc) {
    if (argc < 2 || args[0].type != VAL_LIST || args[1].type != VAL_FUNCTION) {
        return make_bool(0);
    }
    
    Value array = args[0];
    Value callback = args[1];
    int count = array.as.list_val.count;
    
    // Check if any element matches
    for (int i = 0; i < count; i++) {
        Value callback_args[3];
        callback_args[0] = array.as.list_val.items[i];
        callback_args[1] = make_int(i);
        callback_args[2] = array;
        
        Value test_result = call_function(interp, &callback, callback_args, 3);
        
        int truthy = 0;
        if (test_result.type == VAL_BOOL) truthy = test_result.as.bool_val;
        else if (test_result.type == VAL_INT) truthy = test_result.as.int_val != 0;
        else if (test_result.type == VAL_FLOAT) truthy = test_result.as.float_val != 0.0;
        
        value_free(&test_result);
        value_free(&callback_args[1]);
        
        if (truthy) {
            return make_bool(1);
        }
    }
    
    return make_bool(0);
}

/* array.every(callback) - Check if all elements match */
static Value builtin_array_every(Interpreter *interp, Value *args, int argc) {
    if (argc < 2 || args[0].type != VAL_LIST || args[1].type != VAL_FUNCTION) {
        return make_bool(1); // Empty array returns true
    }
    
    Value array = args[0];
    Value callback = args[1];
    int count = array.as.list_val.count;
    
    // Check if all elements match
    for (int i = 0; i < count; i++) {
        Value callback_args[3];
        callback_args[0] = array.as.list_val.items[i];
        callback_args[1] = make_int(i);
        callback_args[2] = array;
        
        Value test_result = call_function(interp, &callback, callback_args, 3);
        
        int truthy = 0;
        if (test_result.type == VAL_BOOL) truthy = test_result.as.bool_val;
        else if (test_result.type == VAL_INT) truthy = test_result.as.int_val != 0;
        else if (test_result.type == VAL_FLOAT) truthy = test_result.as.float_val != 0.0;
        
        value_free(&test_result);
        value_free(&callback_args[1]);
        
        if (!truthy) {
            return make_bool(0);
        }
    }
    
    return make_bool(1);
}

/* Array Methods - Priority 2 */

/* array.indexOf(value) - Find index of value */
static Value builtin_array_indexOf(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_LIST) {
        return make_int(-1);
    }
    
    Value array = args[0];
    Value search_val = args[1];
    int count = array.as.list_val.count;
    
    for (int i = 0; i < count; i++) {
        Value *item = &array.as.list_val.items[i];
        int match = 0;
        
        if (item->type == search_val.type) {
            switch (item->type) {
                case VAL_INT:
                    match = (item->as.int_val == search_val.as.int_val);
                    break;
                case VAL_FLOAT:
                    match = (item->as.float_val == search_val.as.float_val);
                    break;
                case VAL_STRING:
                    match = (strcmp(item->as.str_val, search_val.as.str_val) == 0);
                    break;
                case VAL_BOOL:
                    match = (item->as.bool_val == search_val.as.bool_val);
                    break;
                case VAL_NULL:
                    match = 1;
                    break;
                default:
                    break;
            }
        }
        
        if (match) return make_int(i);
    }
    
    return make_int(-1);
}

/* array.includes(value) - Check if array contains value */
static Value builtin_array_includes(Interpreter *interp, Value *args, int argc) {
    Value result = builtin_array_indexOf(interp, args, argc);
    if (result.type == VAL_INT) {
        return make_bool(result.as.int_val >= 0);
    }
    return make_bool(0);
}

/* array.push(value) - Add to end (mutating) */
static Value builtin_array_push(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_LIST) {
        return make_null();
    }
    
    Value *array = &args[0];
    Value new_val = args[1];
    
    // Resize if needed
    if (array->as.list_val.count >= array->as.list_val.capacity) {
        int new_capacity = array->as.list_val.capacity * 2;
        if (new_capacity == 0) new_capacity = 4;
        array->as.list_val.items = realloc(array->as.list_val.items, new_capacity * sizeof(Value));
        array->as.list_val.capacity = new_capacity;
    }
    
    // Add element (copy if string)
    if (new_val.type == VAL_STRING) {
        array->as.list_val.items[array->as.list_val.count] = make_string(new_val.as.str_val);
    } else {
        array->as.list_val.items[array->as.list_val.count] = new_val;
    }
    array->as.list_val.count++;
    
    return make_int(array->as.list_val.count);
}

/* array.pop() - Remove from end (mutating) */
static Value builtin_array_pop(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_LIST) {
        return make_null();
    }
    
    Value *array = &args[0];
    
    if (array->as.list_val.count == 0) {
        return make_null();
    }
    
    array->as.list_val.count--;
    Value result = array->as.list_val.items[array->as.list_val.count];
    
    // Copy if string to avoid double-free
    if (result.type == VAL_STRING) {
        result = make_string(result.as.str_val);
    }
    
    return result;
}

/* array.slice(start, end) - Extract subarray */
static Value builtin_array_slice(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_LIST) {
        Value empty_list;
        empty_list.type = VAL_LIST;
        empty_list.as.list_val.items = NULL;
        empty_list.as.list_val.count = 0;
        empty_list.as.list_val.capacity = 0;
        return empty_list;
    }
    
    Value array = args[0];
    int count = array.as.list_val.count;
    int start = 0;
    int end = count;
    
    if (argc >= 2 && args[1].type == VAL_INT) {
        start = (int)args[1].as.int_val;
        if (start < 0) start = count + start;
        if (start < 0) start = 0;
        if (start > count) start = count;
    }
    
    if (argc >= 3 && args[2].type == VAL_INT) {
        end = (int)args[2].as.int_val;
        if (end < 0) end = count + end;
        if (end < 0) end = 0;
        if (end > count) end = count;
    }
    
    if (start >= end) {
        Value empty_list;
        empty_list.type = VAL_LIST;
        empty_list.as.list_val.items = NULL;
        empty_list.as.list_val.count = 0;
        empty_list.as.list_val.capacity = 0;
        return empty_list;
    }
    
    int slice_len = end - start;
    Value result;
    result.type = VAL_LIST;
    result.as.list_val.count = slice_len;
    result.as.list_val.capacity = slice_len;
    result.as.list_val.items = malloc(slice_len * sizeof(Value));
    
    for (int i = 0; i < slice_len; i++) {
        if (array.as.list_val.items[start + i].type == VAL_STRING) {
            result.as.list_val.items[i] = make_string(array.as.list_val.items[start + i].as.str_val);
        } else {
            result.as.list_val.items[i] = array.as.list_val.items[start + i];
        }
    }
    
    return result;
}

/* array.concat(other) - Concatenate arrays */
static Value builtin_array_concat(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_LIST || args[1].type != VAL_LIST) {
        if (argc >= 1 && args[0].type == VAL_LIST) {
            // Return copy of first array
            return builtin_array_slice(interp, args, 1);
        }
        Value empty_list;
        empty_list.type = VAL_LIST;
        empty_list.as.list_val.items = NULL;
        empty_list.as.list_val.count = 0;
        empty_list.as.list_val.capacity = 0;
        return empty_list;
    }
    
    Value array1 = args[0];
    Value array2 = args[1];
    int count1 = array1.as.list_val.count;
    int count2 = array2.as.list_val.count;
    int total = count1 + count2;
    
    Value result;
    result.type = VAL_LIST;
    result.as.list_val.count = total;
    result.as.list_val.capacity = total;
    result.as.list_val.items = malloc((total > 0 ? total : 1) * sizeof(Value));
    
    // Copy first array
    for (int i = 0; i < count1; i++) {
        if (array1.as.list_val.items[i].type == VAL_STRING) {
            result.as.list_val.items[i] = make_string(array1.as.list_val.items[i].as.str_val);
        } else {
            result.as.list_val.items[i] = array1.as.list_val.items[i];
        }
    }
    
    // Copy second array
    for (int i = 0; i < count2; i++) {
        if (array2.as.list_val.items[i].type == VAL_STRING) {
            result.as.list_val.items[count1 + i] = make_string(array2.as.list_val.items[i].as.str_val);
        } else {
            result.as.list_val.items[count1 + i] = array2.as.list_val.items[i];
        }
    }
    
    return result;
}

/* array.join(separator) - Join to string */
static Value builtin_array_join(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_LIST) {
        return make_string("");
    }
    
    Value array = args[0];
    const char *separator = ",";
    
    if (argc >= 2 && args[1].type == VAL_STRING) {
        separator = args[1].as.str_val;
    }
    
    int count = array.as.list_val.count;
    if (count == 0) return make_string("");
    
    // Calculate total length
    int total_len = 0;
    char **str_vals = malloc(count * sizeof(char *));
    
    for (int i = 0; i < count; i++) {
        str_vals[i] = value_to_string(&array.as.list_val.items[i]);
        total_len += strlen(str_vals[i]);
        if (i > 0) total_len += strlen(separator);
    }
    
    // Build result string
    char *result = malloc(total_len + 1);
    result[0] = '\0';
    
    for (int i = 0; i < count; i++) {
        if (i > 0) strcat(result, separator);
        strcat(result, str_vals[i]);
        free(str_vals[i]);
    }
    
    free(str_vals);
    Value v = make_string(result);
    free(result);
    return v;
}

/* array.reverse() - Reverse array (mutating) */
static Value builtin_array_reverse(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_LIST) {
        return make_null();
    }
    
    Value *array = &args[0];
    int count = array->as.list_val.count;
    
    for (int i = 0; i < count / 2; i++) {
        Value temp = array->as.list_val.items[i];
        array->as.list_val.items[i] = array->as.list_val.items[count - 1 - i];
        array->as.list_val.items[count - 1 - i] = temp;
    }
    
    return *array;
}

/* array.sort(comparator) - Sort array (mutating) */
static Value builtin_array_sort(Interpreter *interp, Value *args, int argc) {
    if (argc < 1 || args[0].type != VAL_LIST) {
        return make_null();
    }
    
    Value *array = &args[0];
    int count = array->as.list_val.count;
    
    if (count <= 1) return *array;
    
    // Simple bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            Value *a = &array->as.list_val.items[j];
            Value *b = &array->as.list_val.items[j + 1];
            
            int should_swap = 0;
            
            if (argc >= 2 && args[1].type == VAL_FUNCTION) {
                // Use comparator function
                Value callback_args[2];
                callback_args[0] = *a;
                callback_args[1] = *b;
                
                Value cmp_result = call_function(interp, &args[1], callback_args, 2);
                
                if (cmp_result.type == VAL_INT) {
                    should_swap = (cmp_result.as.int_val > 0);
                } else if (cmp_result.type == VAL_FLOAT) {
                    should_swap = (cmp_result.as.float_val > 0.0);
                }
                
                value_free(&cmp_result);
            } else {
                // Default comparison
                if (a->type == VAL_INT && b->type == VAL_INT) {
                    should_swap = (a->as.int_val > b->as.int_val);
                } else if (a->type == VAL_FLOAT && b->type == VAL_FLOAT) {
                    should_swap = (a->as.float_val > b->as.float_val);
                } else if (a->type == VAL_INT && b->type == VAL_FLOAT) {
                    should_swap = ((double)a->as.int_val > b->as.float_val);
                } else if (a->type == VAL_FLOAT && b->type == VAL_INT) {
                    should_swap = (a->as.float_val > (double)b->as.int_val);
                } else if (a->type == VAL_STRING && b->type == VAL_STRING) {
                    should_swap = (strcmp(a->as.str_val, b->as.str_val) > 0);
                }
            }
            
            if (should_swap) {
                Value temp = *a;
                *a = *b;
                *b = temp;
            }
        }
    }
    
    return *array;
}

/* ============================================================================
 * Promise Implementation
 * ============================================================================ */

// Helper function to call a function value
static Value call_function_helper(Interpreter *interp, Value *func, Value *args, int argc) {
    if (func->type != VAL_FUNCTION) {
        fprintf(stderr, "Error: callback is not a function\n");
        return make_null();
    }
    
    Env *call_env = env_new(func->as.func_val.closure);
    
    // Bind parameters
    for (int i = 0; i < func->as.func_val.param_count && i < argc; i++) {
        env_set_local(call_env, func->as.func_val.param_names[i], args[i]);
    }
    
    // Execute function body
    Value result = eval_block(interp, func->as.func_val.body, call_env);
    
    // Handle return value
    if (interp->last_result.is_return) {
        value_free(&result);
        result = interp->last_result.return_value;
        interp->last_result.is_return = 0;
        interp->last_result.return_value = make_null();
    }
    
    env_free(call_env);
    return result;
}

// Helper to resolve a promise
static void promise_resolve(Interpreter *interp, Value *promise, Value value) {
    if (promise->type != VAL_PROMISE || promise->as.promise_val.state != PROMISE_PENDING) {
        return; // Already settled
    }
    
    promise->as.promise_val.state = PROMISE_FULFILLED;
    *promise->as.promise_val.result = value;
    
    // Process all callbacks
    PromiseCallbackNode *node = promise->as.promise_val.callbacks;
    while (node) {
        if (node->on_fulfilled && node->on_fulfilled->type == VAL_FUNCTION) {
            Value args[1] = { value };
            Value result = call_function_helper(interp, node->on_fulfilled, args, 1);
            
            // Resolve the chained promise
            if (node->promise_to_resolve) {
                promise_resolve(interp, node->promise_to_resolve, result);
            } else {
                value_free(&result);
            }
        } else if (node->promise_to_resolve) {
            // No handler, propagate value
            promise_resolve(interp, node->promise_to_resolve, value);
        }
        
        node = node->next;
    }
}

// Helper to reject a promise
static void promise_reject(Interpreter *interp, Value *promise, Value reason) {
    if (promise->type != VAL_PROMISE || promise->as.promise_val.state != PROMISE_PENDING) {
        return; // Already settled
    }
    
    promise->as.promise_val.state = PROMISE_REJECTED;
    *promise->as.promise_val.result = reason;
    
    // Process all callbacks
    PromiseCallbackNode *node = promise->as.promise_val.callbacks;
    int handled = 0;
    while (node) {
        if (node->on_rejected && node->on_rejected->type == VAL_FUNCTION) {
            handled = 1;
            Value args[1] = { reason };
            Value result = call_function_helper(interp, node->on_rejected, args, 1);
            
            // Resolve the chained promise (catch transforms rejection to fulfillment)
            if (node->promise_to_resolve) {
                promise_resolve(interp, node->promise_to_resolve, result);
            } else {
                value_free(&result);
            }
        } else if (node->promise_to_resolve) {
            // No handler, propagate rejection
            promise_reject(interp, node->promise_to_resolve, reason);
        }
        
        node = node->next;
    }
    
    // If no handler caught the rejection, print error
    if (!handled) {
        fprintf(stderr, "Unhandled promise rejection: ");
        value_print(&reason);
        fprintf(stderr, "\n");
    }
}

// Promise constructor: new Promise((resolve, reject) => { ... })
// For now, we'll use a special approach where resolve/reject are special builtins
Value builtin_promise_resolve_fn(Interpreter *interp, Value *args, int argc);
Value builtin_promise_reject_fn(Interpreter *interp, Value *args, int argc);

// Global promise being constructed (for resolve/reject to access)
// NOTE: This is not thread-safe and is a known limitation.
// In a multi-threaded environment, this would need to be refactored
// to pass promise context through parameters or use thread-local storage.
static Value *g_current_promise = NULL;

Value builtin_promise_resolve_fn(Interpreter *interp, Value *args, int argc) {
    if (g_current_promise) {
        Value value = (argc >= 1) ? args[0] : make_null();
        promise_resolve(interp, g_current_promise, value);
    }
    return make_null();
}

Value builtin_promise_reject_fn(Interpreter *interp, Value *args, int argc) {
    if (g_current_promise) {
        Value reason = (argc >= 1) ? args[0] : make_null();
        promise_reject(interp, g_current_promise, reason);
    }
    return make_null();
}

Value builtin_Promise_constructor(Interpreter *interp, Value *args, int argc) {
    if (argc < 1 || args[0].type != VAL_FUNCTION) {
        fprintf(stderr, "Error: Promise executor must be a function\n");
        return make_null();
    }
    
    Value promise = make_promise();
    
    // Create a copy of the promise for the resolve/reject functions to capture
    // NOTE: Memory management - promise_ptr is allocated but not explicitly freed.
    // It's managed as part of the Value system when the promise is stored/copied.
    // This follows KLang's Value ownership model where heap-allocated sub-structures
    // are shared between Value copies.
    Value *promise_ptr = malloc(sizeof(Value));
    *promise_ptr = promise;
    
    // Set global promise
    g_current_promise = promise_ptr;
    
    // Create resolve and reject functions
    Value resolve_fn;
    resolve_fn.type = VAL_BUILTIN;
    resolve_fn.as.builtin = builtin_promise_resolve_fn;
    
    Value reject_fn;
    reject_fn.type = VAL_BUILTIN;
    reject_fn.as.builtin = builtin_promise_reject_fn;
    
    // Execute executor function with (resolve, reject)
    Value executor = args[0];
    Env *call_env = env_new(executor.as.func_val.closure);
    
    // Bind resolve and reject parameters
    if (executor.as.func_val.param_count >= 1) {
        env_set_local(call_env, executor.as.func_val.param_names[0], resolve_fn);
    }
    if (executor.as.func_val.param_count >= 2) {
        env_set_local(call_env, executor.as.func_val.param_names[1], reject_fn);
    }
    
    // Execute the executor
    Value result = eval_block(interp, executor.as.func_val.body, call_env);
    
    // Handle return value
    if (interp->last_result.is_return) {
        value_free(&result);
        result = interp->last_result.return_value;
        interp->last_result.is_return = 0;
        interp->last_result.return_value = make_null();
    }
    
    value_free(&result);
    env_free(call_env);
    
    // Clear global promise
    g_current_promise = NULL;
    
    return *promise_ptr;
}

// Promise.then(onFulfilled, onRejected)
static Value builtin_Promise_then(Interpreter *interp, Value *args, int argc) {
    if (argc < 1 || args[0].type != VAL_PROMISE) {
        fprintf(stderr, "Error: then() requires a promise as receiver\n");
        return make_null();
    }
    
    Value *promise = &args[0];
    Value on_fulfilled = (argc >= 2 && args[1].type == VAL_FUNCTION) ? args[1] : make_null();
    Value on_rejected = (argc >= 3 && args[2].type == VAL_FUNCTION) ? args[2] : make_null();
    
    // Create new promise for chaining
    Value *new_promise = malloc(sizeof(Value));
    *new_promise = make_promise();
    
    // Add callback to the promise
    PromiseCallbackNode *node = malloc(sizeof(PromiseCallbackNode));
    if (on_fulfilled.type == VAL_FUNCTION) {
        node->on_fulfilled = malloc(sizeof(Value));
        *node->on_fulfilled = on_fulfilled;
    } else {
        node->on_fulfilled = NULL;
    }
    if (on_rejected.type == VAL_FUNCTION) {
        node->on_rejected = malloc(sizeof(Value));
        *node->on_rejected = on_rejected;
    } else {
        node->on_rejected = NULL;
    }
    node->promise_to_resolve = new_promise;
    node->next = NULL;
    
    // Append to callback list
    if (promise->as.promise_val.callbacks == NULL) {
        promise->as.promise_val.callbacks = node;
    } else {
        PromiseCallbackNode *tail = promise->as.promise_val.callbacks;
        while (tail->next) tail = tail->next;
        tail->next = node;
    }
    
    // If promise is already settled, process callbacks immediately
    if (promise->as.promise_val.state == PROMISE_FULFILLED) {
        if (node->on_fulfilled) {
            Value args_arr[1] = { *promise->as.promise_val.result };
            Value result = call_function_helper(interp, node->on_fulfilled, args_arr, 1);
            promise_resolve(interp, new_promise, result);
        } else {
            promise_resolve(interp, new_promise, *promise->as.promise_val.result);
        }
    } else if (promise->as.promise_val.state == PROMISE_REJECTED) {
        if (node->on_rejected) {
            Value args_arr[1] = { *promise->as.promise_val.result };
            Value result = call_function_helper(interp, node->on_rejected, args_arr, 1);
            promise_resolve(interp, new_promise, result);
        } else {
            promise_reject(interp, new_promise, *promise->as.promise_val.result);
        }
    }
    
    return *new_promise;
}

// Promise.catch(onRejected)
static Value builtin_Promise_catch(Interpreter *interp, Value *args, int argc) {
    if (argc < 1 || args[0].type != VAL_PROMISE) {
        fprintf(stderr, "Error: catch() requires a promise as receiver\n");
        return make_null();
    }
    
    // catch(f) is equivalent to then(null, f)
    Value then_args[3];
    then_args[0] = args[0];
    then_args[1] = make_null();
    then_args[2] = (argc >= 2) ? args[1] : make_null();
    
    return builtin_Promise_then(interp, then_args, 3);
}

// Promise.finally(onFinally)
static Value builtin_Promise_finally(Interpreter *interp, Value *args, int argc) {
    if (argc < 1 || args[0].type != VAL_PROMISE) {
        fprintf(stderr, "Error: finally() requires a promise as receiver\n");
        return make_null();
    }
    
    Value *promise = &args[0];
    Value on_finally = (argc >= 2 && args[1].type == VAL_FUNCTION) ? args[1] : make_null();
    
    // Create new promise for chaining
    Value *new_promise = malloc(sizeof(Value));
    *new_promise = make_promise();
    
    // Add callback that runs on both fulfill and reject
    PromiseCallbackNode *node = malloc(sizeof(PromiseCallbackNode));
    if (on_finally.type == VAL_FUNCTION) {
        node->on_fulfilled = malloc(sizeof(Value));
        *node->on_fulfilled = on_finally;
        node->on_rejected = malloc(sizeof(Value));
        *node->on_rejected = on_finally;
    } else {
        node->on_fulfilled = NULL;
        node->on_rejected = NULL;
    }
    node->promise_to_resolve = new_promise;
    node->next = NULL;
    
    // Append to callback list
    if (promise->as.promise_val.callbacks == NULL) {
        promise->as.promise_val.callbacks = node;
    } else {
        PromiseCallbackNode *tail = promise->as.promise_val.callbacks;
        while (tail->next) tail = tail->next;
        tail->next = node;
    }
    
    // If promise is already settled, process callbacks immediately
    if (promise->as.promise_val.state != PROMISE_PENDING) {
        if (on_finally.type == VAL_FUNCTION) {
            Value result = call_function_helper(interp, &on_finally, NULL, 0);
            value_free(&result);
        }
        
        // Propagate original state and value
        if (promise->as.promise_val.state == PROMISE_FULFILLED) {
            promise_resolve(interp, new_promise, *promise->as.promise_val.result);
        } else {
            promise_reject(interp, new_promise, *promise->as.promise_val.result);
        }
    }
    
    return *new_promise;
}

// Promise.resolve(value)
static Value builtin_Promise_resolve(Interpreter *interp, Value *args, int argc) {
    Value promise = make_promise();
    Value value = (argc >= 1) ? args[0] : make_null();
    
    Value *promise_ptr = malloc(sizeof(Value));
    *promise_ptr = promise;
    
    promise_resolve(interp, promise_ptr, value);
    
    return *promise_ptr;
}

// Promise.reject(reason)
static Value builtin_Promise_reject(Interpreter *interp, Value *args, int argc) {
    Value promise = make_promise();
    Value reason = (argc >= 1) ? args[0] : make_null();
    
    Value *promise_ptr = malloc(sizeof(Value));
    *promise_ptr = promise;
    
    promise_reject(interp, promise_ptr, reason);
    
    return *promise_ptr;
}

/* Generator next() method */
static Value builtin_generator_next(Interpreter *interp, Value *args, int argc) {
    /* args[0] is the generator object (receiver) */
    if (argc < 1 || args[0].type != VAL_GENERATOR) {
        fprintf(stderr, "Error: next() requires a generator\n");
        interp->had_error = 1;
        return make_null();
    }
    
    GeneratorVal *gen = &args[0].as.generator_val;
    
    /* Check if generator is already completed */
    if (gen->state == GEN_COMPLETED) {
        /* Return {value: null, done: true} */
        Value result;
        result.type = VAL_OBJECT;
        result.as.object_val.class_name = strdup("IteratorResult");
        result.as.object_val.fields = env_new(NULL);
        result.as.object_val.methods = env_new(NULL);
        env_set_local(result.as.object_val.fields, "value", make_null());
        env_set_local(result.as.object_val.fields, "done", make_bool(1));
        return result;
    }
    
    /* Execute generator function body */
    gen->state = GEN_RUNNING;
    
    /* Clear previous return state */
    interp->last_result.is_return = 0;
    value_free(&interp->last_result.return_value);
    interp->last_result.return_value = make_null();
    
    /* Execute the function body in the saved environment */
    Value body_result = eval_block(interp, gen->func->body, gen->saved_env);
    
    /* Check if we hit a yield or return */
    Value yielded_value;
    int is_done = 0;
    
    if (interp->last_result.is_return) {
        /* Got a yield (we reused return mechanism) or actual return */
        yielded_value = interp->last_result.return_value;
        interp->last_result.is_return = 0;
        interp->last_result.return_value = make_null();
        
        /* TODO: Distinguish between yield and return */
        /* For now, we'll mark as done after first yield */
        /* This is a simplified implementation */
        gen->state = GEN_COMPLETED; /* Mark as completed for now */
        is_done = 1;
    } else {
        /* Function completed without explicit return/yield */
        yielded_value = body_result;
        gen->state = GEN_COMPLETED;
        is_done = 1;
    }
    
    /* Create iterator result object {value: ..., done: ...} */
    Value result;
    result.type = VAL_OBJECT;
    result.as.object_val.class_name = strdup("IteratorResult");
    result.as.object_val.fields = env_new(NULL);
    result.as.object_val.methods = env_new(NULL);
    
    env_set_local(result.as.object_val.fields, "value", yielded_value);
    env_set_local(result.as.object_val.fields, "done", make_bool(is_done));
    
    return result;
}

/* ========================================
   File I/O Operations
   ======================================== */

/* Helper function to create a file value */
static Value make_file(const char *path, const char *mode, FILE *fp) {
    Value v;
    v.type = VAL_FILE;
    v.as.file_val.fp = fp;
    v.as.file_val.path = path ? strdup(path) : NULL;
    v.as.file_val.mode = mode ? strdup(mode) : NULL;
    v.as.file_val.is_open = (fp != NULL);
    return v;
}

/* fopen(path, mode) - Open a file for reading/writing */
static Value builtin_fopen(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) {
        fprintf(stderr, "Error: fopen requires 2 arguments (path, mode)\n");
        return make_null();
    }
    if (args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: fopen arguments must be strings\n");
        return make_null();
    }
    
    const char *path = args[0].as.str_val;
    const char *mode = args[1].as.str_val;
    
    FILE *fp = fopen(path, mode);
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file '%s' in mode '%s'\n", path, mode);
        return make_null();
    }
    
    return make_file(path, mode, fp);
}

/* fclose(file) - Close an open file */
static Value builtin_fclose(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: fclose requires 1 argument (file)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_FILE) {
        fprintf(stderr, "Error: fclose argument must be a file\n");
        return make_bool(0);
    }
    
    if (args[0].as.file_val.is_open && args[0].as.file_val.fp) {
        fclose(args[0].as.file_val.fp);
        args[0].as.file_val.fp = NULL;
        args[0].as.file_val.is_open = 0;
        return make_bool(1);
    }
    
    return make_bool(0);
}

/* fread(file) - Read entire file content */
static Value builtin_fread(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: fread requires 1 argument (file)\n");
        return make_string("");
    }
    if (args[0].type != VAL_FILE) {
        fprintf(stderr, "Error: fread argument must be a file\n");
        return make_string("");
    }
    
    if (!args[0].as.file_val.is_open || !args[0].as.file_val.fp) {
        fprintf(stderr, "Error: File is not open\n");
        return make_string("");
    }
    
    FILE *fp = args[0].as.file_val.fp;
    
    /* Get file size */
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    /* Allocate buffer and read */
    char *buffer = malloc(size + 1);
    size_t bytes_read = fread(buffer, 1, size, fp);
    buffer[bytes_read] = '\0';
    
    Value result = make_string(buffer);
    free(buffer);
    return result;
}

/* freadline(file) - Read single line from file */
static Value builtin_freadline(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: freadline requires 1 argument (file)\n");
        return make_string("");
    }
    if (args[0].type != VAL_FILE) {
        fprintf(stderr, "Error: freadline argument must be a file\n");
        return make_string("");
    }
    
    if (!args[0].as.file_val.is_open || !args[0].as.file_val.fp) {
        fprintf(stderr, "Error: File is not open\n");
        return make_string("");
    }
    
    char buffer[4096];
    if (fgets(buffer, sizeof(buffer), args[0].as.file_val.fp)) {
        /* Remove trailing newline if present */
        int len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        return make_string(buffer);
    }
    
    return make_string("");
}

/* fwrite(file, content) - Write to file */
static Value builtin_fwrite(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) {
        fprintf(stderr, "Error: fwrite requires 2 arguments (file, content)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_FILE) {
        fprintf(stderr, "Error: fwrite first argument must be a file\n");
        return make_bool(0);
    }
    if (args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: fwrite second argument must be a string\n");
        return make_bool(0);
    }
    
    if (!args[0].as.file_val.is_open || !args[0].as.file_val.fp) {
        fprintf(stderr, "Error: File is not open\n");
        return make_bool(0);
    }
    
    const char *content = args[1].as.str_val;
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, args[0].as.file_val.fp);
    fflush(args[0].as.file_val.fp);
    
    return make_bool(written == len);
}

/* fexists(path) - Check if file exists */
static Value builtin_fexists(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: fexists requires 1 argument (path)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: fexists argument must be a string\n");
        return make_bool(0);
    }
    
    FILE *fp = fopen(args[0].as.str_val, "r");
    if (fp) {
        fclose(fp);
        return make_bool(1);
    }
    return make_bool(0);
}

/* fdelete(path) - Delete a file */
static Value builtin_fdelete(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: fdelete requires 1 argument (path)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: fdelete argument must be a string\n");
        return make_bool(0);
    }
    
    int result = remove(args[0].as.str_val);
    return make_bool(result == 0);
}

/* frename(oldpath, newpath) - Rename/move a file */
static Value builtin_frename(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) {
        fprintf(stderr, "Error: frename requires 2 arguments (oldpath, newpath)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: frename arguments must be strings\n");
        return make_bool(0);
    }
    
    int result = rename(args[0].as.str_val, args[1].as.str_val);
    return make_bool(result == 0);
}

/* fsize(path) - Get file size in bytes */
static Value builtin_fsize(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: fsize requires 1 argument (path)\n");
        return make_int(-1);
    }
    if (args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: fsize argument must be a string\n");
        return make_int(-1);
    }
    
    FILE *fp = fopen(args[0].as.str_val, "r");
    if (!fp) {
        return make_int(-1);
    }
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    
    return make_int(size);
}

/* readFile(path) - Convenience: read entire file at once */
static Value builtin_readFile(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: readFile requires 1 argument (path)\n");
        return make_string("");
    }
    if (args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: readFile argument must be a string\n");
        return make_string("");
    }
    
    FILE *fp = fopen(args[0].as.str_val, "r");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", args[0].as.str_val);
        return make_string("");
    }
    
    /* Get file size */
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    /* Allocate buffer and read */
    char *buffer = malloc(size + 1);
    size_t bytes_read = fread(buffer, 1, size, fp);
    buffer[bytes_read] = '\0';
    fclose(fp);
    
    Value result = make_string(buffer);
    free(buffer);
    return result;
}

/* writeFile(path, content) - Convenience: write entire file at once */
static Value builtin_writeFile(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) {
        fprintf(stderr, "Error: writeFile requires 2 arguments (path, content)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: writeFile arguments must be strings\n");
        return make_bool(0);
    }
    
    FILE *fp = fopen(args[0].as.str_val, "w");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file '%s' for writing\n", args[0].as.str_val);
        return make_bool(0);
    }
    
    const char *content = args[1].as.str_val;
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, fp);
    fclose(fp);
    
    return make_bool(written == len);
}

/* appendFile(path, content) - Convenience: append to file */
static Value builtin_appendFile(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) {
        fprintf(stderr, "Error: appendFile requires 2 arguments (path, content)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: appendFile arguments must be strings\n");
        return make_bool(0);
    }
    
    FILE *fp = fopen(args[0].as.str_val, "a");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file '%s' for appending\n", args[0].as.str_val);
        return make_bool(0);
    }
    
    const char *content = args[1].as.str_val;
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, fp);
    fclose(fp);
    
    return make_bool(written == len);
}

/* ==================== Dictionary Functions ==================== */

/* Helper function to compare two values for equality */
static int values_equal(Value *a, Value *b) {
    if (a->type != b->type) return 0;
    
    switch (a->type) {
        case VAL_INT: return a->as.int_val == b->as.int_val;
        case VAL_FLOAT: return a->as.float_val == b->as.float_val;
        case VAL_STRING: return strcmp(a->as.str_val, b->as.str_val) == 0;
        case VAL_BOOL: return a->as.bool_val == b->as.bool_val;
        case VAL_NULL: return 1;
        default: return 0;  // Can't compare complex types
    }
}

/* Find index of key in dictionary, returns -1 if not found */
static int dict_find_key(Value *dict, Value *key) {
    int i;
    for (i = 0; i < dict->as.dict_val->count; i++) {
        if (values_equal(&dict->as.dict_val->keys[i], key)) {
            return i;
        }
    }
    return -1;
}

static Value builtin_dict(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    (void)args;
    (void)argc;
    return make_dict();
}

static Value builtin_dict_set(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 3 || args[0].type != VAL_DICT) {
        return make_null();
    }
    
    Value *dict = &args[0];
    Value key = args[1];
    Value value = args[2];
    
    /* Check if key already exists */
    int idx = dict_find_key(dict, &key);
    
    if (idx >= 0) {
        /* Update existing value */
        value_free(&dict->as.dict_val->values[idx]);
        dict->as.dict_val->values[idx] = value_deep_copy(value);
    } else {
        /* Add new key-value pair */
        if (dict->as.dict_val->count >= dict->as.dict_val->capacity) {
            int new_capacity = dict->as.dict_val->capacity * 2;
            dict->as.dict_val->keys = realloc(dict->as.dict_val->keys, new_capacity * sizeof(Value));
            dict->as.dict_val->values = realloc(dict->as.dict_val->values, new_capacity * sizeof(Value));
            dict->as.dict_val->capacity = new_capacity;
        }
        
        /* Copy key and value */
        dict->as.dict_val->keys[dict->as.dict_val->count] = value_deep_copy(key);
        dict->as.dict_val->values[dict->as.dict_val->count] = value_deep_copy(value);
        
        dict->as.dict_val->count++;
    }
    
    return make_null();
}

static Value builtin_dict_get(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_DICT) {
        return make_null();
    }
    
    Value *dict = &args[0];
    Value key = args[1];
    
    int idx = dict_find_key(dict, &key);
    
    if (idx >= 0) {
        Value result = dict->as.dict_val->values[idx];
        /* Return deep copy to maintain proper ownership */
        return value_deep_copy(result);
    }
    
    /* Return default value if provided */
    if (argc >= 3) {
        /* Return deep copy of default value */
        return value_deep_copy(args[2]);
    }
    
    return make_null();
}

static Value builtin_dict_has(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_DICT) {
        return make_bool(0);
    }
    
    Value *dict = &args[0];
    Value key = args[1];
    
    return make_bool(dict_find_key(dict, &key) >= 0);
}

static Value builtin_dict_delete(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_DICT) {
        return make_bool(0);
    }
    
    Value *dict = &args[0];
    Value key = args[1];
    
    int idx = dict_find_key(dict, &key);
    
    if (idx >= 0) {
        /* Free the key-value pair */
        value_free(&dict->as.dict_val->keys[idx]);
        value_free(&dict->as.dict_val->values[idx]);
        
        /* Shift remaining elements down */
        int i;
        for (i = idx; i < dict->as.dict_val->count - 1; i++) {
            dict->as.dict_val->keys[i] = dict->as.dict_val->keys[i + 1];
            dict->as.dict_val->values[i] = dict->as.dict_val->values[i + 1];
        }
        
        dict->as.dict_val->count--;
        return make_bool(1);
    }
    
    return make_bool(0);
}

static Value builtin_dict_keys(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_DICT) {
        Value list;
        list.type = VAL_LIST;
        list.as.list_val.count = 0;
        list.as.list_val.capacity = 4;
        list.as.list_val.items = malloc(4 * sizeof(Value));
        return list;
    }
    
    Value *dict = &args[0];
    Value list;
    int i;
    
    list.type = VAL_LIST;
    list.as.list_val.count = dict->as.dict_val->count;
    list.as.list_val.capacity = dict->as.dict_val->count > 0 ? dict->as.dict_val->count : 1;
    list.as.list_val.items = malloc(list.as.list_val.capacity * sizeof(Value));
    
    for (i = 0; i < dict->as.dict_val->count; i++) {
        if (dict->as.dict_val->keys[i].type == VAL_STRING) {
            list.as.list_val.items[i] = make_string(dict->as.dict_val->keys[i].as.str_val);
        } else {
            list.as.list_val.items[i] = dict->as.dict_val->keys[i];
        }
    }
    
    return list;
}

static Value builtin_dict_values(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_DICT) {
        Value list;
        list.type = VAL_LIST;
        list.as.list_val.count = 0;
        list.as.list_val.capacity = 4;
        list.as.list_val.items = malloc(4 * sizeof(Value));
        return list;
    }
    
    Value *dict = &args[0];
    Value list;
    int i;
    
    list.type = VAL_LIST;
    list.as.list_val.count = dict->as.dict_val->count;
    list.as.list_val.capacity = dict->as.dict_val->count > 0 ? dict->as.dict_val->count : 1;
    list.as.list_val.items = malloc(list.as.list_val.capacity * sizeof(Value));
    
    for (i = 0; i < dict->as.dict_val->count; i++) {
        if (dict->as.dict_val->values[i].type == VAL_STRING) {
            list.as.list_val.items[i] = make_string(dict->as.dict_val->values[i].as.str_val);
        } else {
            list.as.list_val.items[i] = dict->as.dict_val->values[i];
        }
    }
    
    return list;
}

/* ==================== Set Functions ==================== */

/* Find index of value in set, returns -1 if not found */
static int set_find_value(Value *set, Value *value) {
    int i;
    for (i = 0; i < set->as.set_val->count; i++) {
        if (values_equal(&set->as.set_val->items[i], value)) {
            return i;
        }
    }
    return -1;
}

static Value builtin_set(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    (void)args;
    (void)argc;
    return make_set();
}

static Value builtin_set_add(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_SET) {
        return make_null();
    }
    
    Value *set = &args[0];
    Value value = args[1];
    
    /* Check if value already exists */
    if (set_find_value(set, &value) >= 0) {
        return make_null();  /* Already in set, no-op */
    }
    
    /* Add new value */
    if (set->as.set_val->count >= set->as.set_val->capacity) {
        int new_capacity = set->as.set_val->capacity * 2;
        set->as.set_val->items = realloc(set->as.set_val->items, new_capacity * sizeof(Value));
        set->as.set_val->capacity = new_capacity;
    }
    
    /* Copy value */
    set->as.set_val->items[set->as.set_val->count] = value_deep_copy(value);
    
    set->as.set_val->count++;
    
    return make_null();
}

static Value builtin_set_remove(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_SET) {
        return make_bool(0);
    }
    
    Value *set = &args[0];
    Value value = args[1];
    
    int idx = set_find_value(set, &value);
    
    if (idx >= 0) {
        /* Free the value */
        value_free(&set->as.set_val->items[idx]);
        
        /* Shift remaining elements down */
        int i;
        for (i = idx; i < set->as.set_val->count - 1; i++) {
            set->as.set_val->items[i] = set->as.set_val->items[i + 1];
        }
        
        set->as.set_val->count--;
        return make_bool(1);
    }
    
    return make_bool(0);
}

static Value builtin_set_has(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_SET) {
        return make_bool(0);
    }
    
    Value *set = &args[0];
    Value value = args[1];
    
    return make_bool(set_find_value(set, &value) >= 0);
}

static Value builtin_set_size(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_SET) {
        return make_int(0);
    }
    
    return make_int(args[0].as.set_val->count);
}

static Value builtin_set_clear(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_SET) {
        return make_null();
    }
    
    SetVal *set_val = args[0].as.set_val;
    int i;
    
    /* Free all items */
    for (i = 0; i < set_val->count; i++) {
        value_free(&set_val->items[i]);
    }
    
    /* Free the items array and allocate a new one */
    free(set_val->items);
    set_val->count = 0;
    set_val->capacity = 8;
    set_val->items = malloc(8 * sizeof(Value));
    
    return make_null();
}

// ============================================================================
// Regex Functions using POSIX regex
// ============================================================================

static Value builtin_regexTest(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: regexTest requires two string arguments (pattern, text)\n");
        return make_bool(0);
    }
    
    const char *pattern = args[0].as.str_val;
    const char *text = args[1].as.str_val;
    
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    
    if (result != 0) {
        fprintf(stderr, "Error: Invalid regex pattern: %s\n", pattern);
        regfree(&regex);
        return make_bool(0);
    }
    
    result = regexec(&regex, text, 0, NULL, 0);
    regfree(&regex);
    
    return make_bool(result == 0);
}

static Value builtin_regexMatch(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: regexMatch requires two string arguments (pattern, text)\n");
        return make_null();
    }
    
    const char *pattern = args[0].as.str_val;
    const char *text = args[1].as.str_val;
    
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    
    if (result != 0) {
        fprintf(stderr, "Error: Invalid regex pattern: %s\n", pattern);
        regfree(&regex);
        return make_null();
    }
    
    regmatch_t matches[10];  // Support up to 10 capture groups
    result = regexec(&regex, text, 10, matches, 0);
    
    if (result != 0) {
        regfree(&regex);
        return make_null();
    }
    
    // Count actual matches
    int match_count = 0;
    for (int i = 0; i < 10 && matches[i].rm_so != -1; i++) {
        match_count++;
    }
    
    // Create array of matches
    Value match_list;
    match_list.type = VAL_LIST;
    match_list.as.list_val.items = malloc(match_count * sizeof(Value));
    match_list.as.list_val.count = match_count;
    match_list.as.list_val.capacity = match_count;
    
    for (int i = 0; i < match_count; i++) {
        int len = matches[i].rm_eo - matches[i].rm_so;
        char *match_str = malloc(len + 1);
        strncpy(match_str, text + matches[i].rm_so, len);
        match_str[len] = '\0';
        
        match_list.as.list_val.items[i] = make_string(match_str);
        free(match_str);
    }
    
    regfree(&regex);
    return match_list;
}

static Value builtin_regexReplace(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 3 || args[0].type != VAL_STRING || 
        args[1].type != VAL_STRING || args[2].type != VAL_STRING) {
        fprintf(stderr, "Error: regexReplace requires three string arguments (pattern, text, replacement)\n");
        return make_null();
    }
    
    const char *pattern = args[0].as.str_val;
    const char *text = args[1].as.str_val;
    const char *replacement = args[2].as.str_val;
    
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    
    if (result != 0) {
        fprintf(stderr, "Error: Invalid regex pattern: %s\n", pattern);
        regfree(&regex);
        return make_string((char*)text);
    }
    
    regmatch_t match;
    result = regexec(&regex, text, 1, &match, 0);
    
    if (result != 0) {
        regfree(&regex);
        return make_string((char*)text);
    }
    
    // Build result string with replacement
    int text_len = strlen(text);
    int repl_len = strlen(replacement);
    int new_len = text_len - (match.rm_eo - match.rm_so) + repl_len;
    char *new_text = malloc(new_len + 1);
    
    // Copy before match
    strncpy(new_text, text, match.rm_so);
    // Copy replacement
    strcpy(new_text + match.rm_so, replacement);
    // Copy after match
    strcpy(new_text + match.rm_so + repl_len, text + match.rm_eo);
    
    Value result_val = make_string(new_text);
    free(new_text);
    regfree(&regex);
    return result_val;
}

static Value builtin_regexSplit(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: regexSplit requires two string arguments (pattern, text)\n");
        return make_null();
    }
    
    const char *pattern = args[0].as.str_val;
    const char *text = args[1].as.str_val;
    
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    
    if (result != 0) {
        fprintf(stderr, "Error: Invalid regex pattern: %s\n", pattern);
        regfree(&regex);
        // Return array with original text
        Value list;
        list.type = VAL_LIST;
        list.as.list_val.items = malloc(sizeof(Value));
        list.as.list_val.count = 1;
        list.as.list_val.capacity = 1;
        list.as.list_val.items[0] = make_string((char*)text);
        return list;
    }
    
    // First, count how many splits we'll have
    int split_count = 1;  // At least one part
    const char *current = text;
    regmatch_t match;
    
    while (regexec(&regex, current, 1, &match, 0) == 0) {
        split_count++;
        current += match.rm_eo;
        if (match.rm_eo == 0) break;  // Prevent infinite loop
    }
    
    // Create result list
    Value list;
    list.type = VAL_LIST;
    list.as.list_val.items = malloc(split_count * sizeof(Value));
    list.as.list_val.capacity = split_count;
    list.as.list_val.count = 0;
    
    // Now do the actual splitting
    current = text;
    while (regexec(&regex, current, 1, &match, 0) == 0) {
        // Add part before match
        if (match.rm_so >= 0) {
            char *part = malloc(match.rm_so + 1);
            strncpy(part, current, match.rm_so);
            part[match.rm_so] = '\0';
            list.as.list_val.items[list.as.list_val.count++] = make_string(part);
            free(part);
        }
        
        // Move past the match
        current += match.rm_eo;
        
        // Prevent infinite loop on empty matches
        if (match.rm_eo == 0) break;
    }
    
    // Add remaining text
    if (*current) {
        list.as.list_val.items[list.as.list_val.count++] = make_string((char*)current);
    }
    
    regfree(&regex);
    return list;
}

/* ============================================
 * AI-NATIVE BUILT-IN FUNCTIONS
 * ============================================ */

/* Environment Variables */
static Value builtin_env_get(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: env.get() requires a string argument (variable name)\n");
        return make_null();
    }
    
    const char *var_name = args[0].as.str_val;
    const char *value = getenv(var_name);
    
    if (value == NULL) {
        return make_null();
    }
    
    return make_string((char*)value);
}

static Value builtin_env_set(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: env.set() requires two string arguments (name, value)\n");
        return make_bool(0);
    }
    
    const char *var_name = args[0].as.str_val;
    const char *value = args[1].as.str_val;
    
    #ifdef _WIN32
    _putenv_s(var_name, value);
    #else
    setenv(var_name, value, 1);
    #endif
    
    return make_bool(1);
}

static Value builtin_env_has(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: env.has() requires a string argument (variable name)\n");
        return make_bool(0);
    }
    
    const char *var_name = args[0].as.str_val;
    const char *value = getenv(var_name);
    
    return make_bool(value != NULL);
}

/* HTTP Request Functions (using system commands for now - can be upgraded to libcurl later) */
static Value builtin_http_get(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: http.get() requires a string argument (URL)\n");
        return make_null();
    }
    
    const char *url = args[0].as.str_val;
    
    // Build curl command
    char command[4096];
    snprintf(command, sizeof(command), "curl -s -X GET '%s'", url);
    
    // Execute curl and capture output
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Failed to execute HTTP GET request\n");
        return make_null();
    }
    
    // Read response into buffer
    char *response = NULL;
    size_t response_size = 0;
    size_t capacity = 8192;
    response = malloc(capacity);
    
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        if (response_size + len >= capacity) {
            capacity *= 2;
            response = realloc(response, capacity);
        }
        strcpy(response + response_size, buffer);
        response_size += len;
    }
    
    int status = pclose(fp);
    
    if (status != 0) {
        free(response);
        fprintf(stderr, "Error: HTTP GET request failed with status %d\n", status);
        return make_null();
    }
    
    Value result = make_string(response);
    free(response);
    return result;
}

static Value builtin_http_post(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: http.post() requires two string arguments (URL, body)\n");
        return make_null();
    }
    
    const char *url = args[0].as.str_val;
    const char *body = args[1].as.str_val;
    
    // Optional headers as third argument
    const char *content_type = "application/json";
    if (argc >= 3 && args[2].type == VAL_STRING) {
        content_type = args[2].as.str_val;
    }
    
    // Build curl command
    char command[8192];
    snprintf(command, sizeof(command), 
             "curl -s -X POST '%s' -H 'Content-Type: %s' -d '%s'", 
             url, content_type, body);
    
    // Execute curl and capture output
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Failed to execute HTTP POST request\n");
        return make_null();
    }
    
    // Read response into buffer
    char *response = NULL;
    size_t response_size = 0;
    size_t capacity = 8192;
    response = malloc(capacity);
    
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        if (response_size + len >= capacity) {
            capacity *= 2;
            response = realloc(response, capacity);
        }
        strcpy(response + response_size, buffer);
        response_size += len;
    }
    
    int status = pclose(fp);
    
    if (status != 0) {
        free(response);
        fprintf(stderr, "Error: HTTP POST request failed with status %d\n", status);
        return make_null();
    }
    
    Value result = make_string(response);
    free(response);
    return result;
}

static Value builtin_http_request(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_DICT) {
        fprintf(stderr, "Error: http.request() requires a dictionary argument (config)\n");
        return make_null();
    }
    
    DictVal *config = args[0].as.dict_val;
    
    // Extract URL (required)
    Value *url_val = NULL;
    for (int i = 0; i < config->count; i++) {
        if (config->keys[i].type == VAL_STRING && 
            strcmp(config->keys[i].as.str_val, "url") == 0) {
            url_val = &config->values[i];
            break;
        }
    }
    
    if (url_val == NULL || url_val->type != VAL_STRING) {
        fprintf(stderr, "Error: http.request() config must have a 'url' string property\n");
        return make_null();
    }
    
    const char *url = url_val->as.str_val;
    
    // Extract method (default: GET)
    const char *method = "GET";
    for (int i = 0; i < config->count; i++) {
        if (config->keys[i].type == VAL_STRING && 
            strcmp(config->keys[i].as.str_val, "method") == 0) {
            if (config->values[i].type == VAL_STRING) {
                method = config->values[i].as.str_val;
            }
            break;
        }
    }
    
    // Extract headers (optional)
    char headers_str[4096] = "";
    for (int i = 0; i < config->count; i++) {
        if (config->keys[i].type == VAL_STRING && 
            strcmp(config->keys[i].as.str_val, "headers") == 0) {
            if (config->values[i].type == VAL_DICT) {
                DictVal *headers = config->values[i].as.dict_val;
                for (int j = 0; j < headers->count; j++) {
                    if (headers->keys[j].type == VAL_STRING && 
                        headers->values[j].type == VAL_STRING) {
                        char header[512];
                        snprintf(header, sizeof(header), " -H '%s: %s'",
                                headers->keys[j].as.str_val,
                                headers->values[j].as.str_val);
                        strncat(headers_str, header, sizeof(headers_str) - strlen(headers_str) - 1);
                    }
                }
            }
            break;
        }
    }
    
    // Extract body (optional)
    const char *body = NULL;
    for (int i = 0; i < config->count; i++) {
        if (config->keys[i].type == VAL_STRING && 
            strcmp(config->keys[i].as.str_val, "body") == 0) {
            if (config->values[i].type == VAL_STRING) {
                body = config->values[i].as.str_val;
            }
            break;
        }
    }
    
    // Build curl command
    char command[8192];
    if (body != NULL) {
        snprintf(command, sizeof(command), 
                 "curl -s -X %s '%s'%s -d '%s'", 
                 method, url, headers_str, body);
    } else {
        snprintf(command, sizeof(command), 
                 "curl -s -X %s '%s'%s", 
                 method, url, headers_str);
    }
    
    // Execute curl and capture output
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Failed to execute HTTP request\n");
        return make_null();
    }
    
    // Read response into buffer
    char *response = NULL;
    size_t response_size = 0;
    size_t capacity = 8192;
    response = malloc(capacity);
    response[0] = '\0';
    
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        if (response_size + len >= capacity) {
            capacity *= 2;
            response = realloc(response, capacity);
        }
        strcpy(response + response_size, buffer);
        response_size += len;
    }
    
    int status = pclose(fp);
    
    if (status != 0) {
        free(response);
        fprintf(stderr, "Error: HTTP request failed with status %d\n", status);
        return make_null();
    }
    
    Value result = make_string(response);
    free(response);
    return result;
}

/* JSON Streaming Functions */
static Value builtin_json_parse_stream(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: JSON.parseStream() requires a string (JSON text) and callback function\n");
        return make_null();
    }
    
    // This is a placeholder - full streaming would require a more complex implementation
    // For now, parse and call callback with the result
    return make_null();
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
    
    /* Math module functions */
    // Create Math object with constants and methods
    Value math_obj = make_object("Math", NULL);
    
    // Math constants
    Value pi_val = make_float(3.14159265358979323846);
    env_set_local(math_obj.as.object_val.fields, "PI", pi_val);
    
    Value e_val = make_float(2.71828182845904523536);
    env_set_local(math_obj.as.object_val.fields, "E", e_val);
    
    Value tau_val = make_float(6.28318530717958647692);
    env_set_local(math_obj.as.object_val.fields, "TAU", tau_val);
    
    // Math functions as global (can also access via Math.func)
    v.as.builtin = builtin_math_abs;
    env_set_local(interp->global_env, "abs", v);
    env_set_local(math_obj.as.object_val.fields, "abs", v);
    
    v.as.builtin = builtin_math_ceil;
    env_set_local(math_obj.as.object_val.fields, "ceil", v);
    
    v.as.builtin = builtin_math_floor;
    env_set_local(math_obj.as.object_val.fields, "floor", v);
    
    v.as.builtin = builtin_math_round;
    env_set_local(math_obj.as.object_val.fields, "round", v);
    
    v.as.builtin = builtin_math_min;
    env_set_local(interp->global_env, "min", v);
    env_set_local(math_obj.as.object_val.fields, "min", v);
    
    v.as.builtin = builtin_math_max;
    env_set_local(interp->global_env, "max", v);
    env_set_local(math_obj.as.object_val.fields, "max", v);
    
    v.as.builtin = builtin_math_pow;
    env_set_local(math_obj.as.object_val.fields, "pow", v);
    
    v.as.builtin = builtin_math_sqrt;
    env_set_local(math_obj.as.object_val.fields, "sqrt", v);
    
    v.as.builtin = builtin_math_exp;
    env_set_local(math_obj.as.object_val.fields, "exp", v);
    
    v.as.builtin = builtin_math_log;
    env_set_local(math_obj.as.object_val.fields, "log", v);
    
    v.as.builtin = builtin_math_log10;
    env_set_local(math_obj.as.object_val.fields, "log10", v);
    
    v.as.builtin = builtin_math_sin;
    env_set_local(math_obj.as.object_val.fields, "sin", v);
    
    v.as.builtin = builtin_math_cos;
    env_set_local(math_obj.as.object_val.fields, "cos", v);
    
    v.as.builtin = builtin_math_tan;
    env_set_local(math_obj.as.object_val.fields, "tan", v);
    
    v.as.builtin = builtin_math_asin;
    env_set_local(math_obj.as.object_val.fields, "asin", v);
    
    v.as.builtin = builtin_math_acos;
    env_set_local(math_obj.as.object_val.fields, "acos", v);
    
    v.as.builtin = builtin_math_atan;
    env_set_local(math_obj.as.object_val.fields, "atan", v);
    
    v.as.builtin = builtin_math_atan2;
    env_set_local(math_obj.as.object_val.fields, "atan2", v);
    
    v.as.builtin = builtin_math_sinh;
    env_set_local(math_obj.as.object_val.fields, "sinh", v);
    
    v.as.builtin = builtin_math_cosh;
    env_set_local(math_obj.as.object_val.fields, "cosh", v);
    
    v.as.builtin = builtin_math_tanh;
    env_set_local(math_obj.as.object_val.fields, "tanh", v);
    
    // Register Math object globally
    env_set_local(interp->global_env, "Math", math_obj);
    
    /* Python-style utility functions */
    v.as.builtin = builtin_range;
    env_set_local(interp->global_env, "range", v);
    
    /* Array methods - Priority 1 */
    v.as.builtin = builtin_array_map;
    env_set_local(interp->global_env, "__array_map", v);
    
    v.as.builtin = builtin_array_filter;
    env_set_local(interp->global_env, "__array_filter", v);
    
    v.as.builtin = builtin_array_reduce;
    env_set_local(interp->global_env, "__array_reduce", v);
    
    v.as.builtin = builtin_array_forEach;
    env_set_local(interp->global_env, "__array_forEach", v);
    
    v.as.builtin = builtin_array_find;
    env_set_local(interp->global_env, "__array_find", v);
    
    v.as.builtin = builtin_array_some;
    env_set_local(interp->global_env, "__array_some", v);
    
    v.as.builtin = builtin_array_every;
    env_set_local(interp->global_env, "__array_every", v);
    
    /* Array methods - Priority 2 */
    v.as.builtin = builtin_array_indexOf;
    env_set_local(interp->global_env, "__array_indexOf", v);
    
    v.as.builtin = builtin_array_includes;
    env_set_local(interp->global_env, "__array_includes", v);
    
    v.as.builtin = builtin_array_push;
    env_set_local(interp->global_env, "__array_push", v);
    
    v.as.builtin = builtin_array_pop;
    env_set_local(interp->global_env, "__array_pop", v);
    
    v.as.builtin = builtin_array_slice;
    env_set_local(interp->global_env, "__array_slice", v);
    
    v.as.builtin = builtin_array_concat;
    env_set_local(interp->global_env, "__array_concat", v);
    
    v.as.builtin = builtin_array_join;
    env_set_local(interp->global_env, "__array_join", v);
    
    v.as.builtin = builtin_array_reverse;
    env_set_local(interp->global_env, "__array_reverse", v);
    
    v.as.builtin = builtin_array_sort;
    env_set_local(interp->global_env, "__array_sort", v);
    
    /* Promise methods */
    v.as.builtin = builtin_Promise_then;
    env_set_local(interp->global_env, "__promise_then", v);
    
    v.as.builtin = builtin_Promise_catch;
    env_set_local(interp->global_env, "__promise_catch", v);
    
    v.as.builtin = builtin_Promise_finally;
    env_set_local(interp->global_env, "__promise_finally", v);
    
    /* Promise static methods */
    v.as.builtin = builtin_Promise_resolve;
    env_set_local(interp->global_env, "__Promise_resolve", v);
    
    v.as.builtin = builtin_Promise_reject;
    env_set_local(interp->global_env, "__Promise_reject", v);
    
    /* Generator methods */
    v.as.builtin = builtin_generator_next;
    env_set_local(interp->global_env, "__generator_next", v);
    
    /* File I/O operations */
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
    env_set_local(interp->global_env, "__dict_set", v);
    
    v.as.builtin = builtin_dict_get;
    env_set_local(interp->global_env, "__dict_get", v);
    
    v.as.builtin = builtin_dict_has;
    env_set_local(interp->global_env, "__dict_has", v);
    
    v.as.builtin = builtin_dict_delete;
    env_set_local(interp->global_env, "__dict_delete", v);
    
    v.as.builtin = builtin_dict_keys;
    env_set_local(interp->global_env, "__dict_keys", v);
    
    v.as.builtin = builtin_dict_values;
    env_set_local(interp->global_env, "__dict_values", v);
    
    /* Set functions */
    v.as.builtin = builtin_set;
    env_set_local(interp->global_env, "set", v);
    
    v.as.builtin = builtin_set_add;
    env_set_local(interp->global_env, "__set_add", v);
    
    v.as.builtin = builtin_set_remove;
    env_set_local(interp->global_env, "__set_remove", v);
    
    v.as.builtin = builtin_set_has;
    env_set_local(interp->global_env, "__set_has", v);
    
    v.as.builtin = builtin_set_size;
    env_set_local(interp->global_env, "__set_size", v);
    
    v.as.builtin = builtin_set_clear;
    env_set_local(interp->global_env, "__set_clear", v);
    
    /* Regex functions */
    v.as.builtin = builtin_regexTest;
    env_set_local(interp->global_env, "regexTest", v);
    
    v.as.builtin = builtin_regexMatch;
    env_set_local(interp->global_env, "regexMatch", v);
    
    v.as.builtin = builtin_regexReplace;
    env_set_local(interp->global_env, "regexReplace", v);
    
    v.as.builtin = builtin_regexSplit;
    env_set_local(interp->global_env, "regexSplit", v);
    
    /* AI-Native Functions: Environment Variables */
    // Create env object
    Value env_obj = make_object("env", NULL);
    
    v.as.builtin = builtin_env_get;
    env_set_local(env_obj.as.object_val.fields, "get", v);
    
    v.as.builtin = builtin_env_set;
    env_set_local(env_obj.as.object_val.fields, "set", v);
    
    v.as.builtin = builtin_env_has;
    env_set_local(env_obj.as.object_val.fields, "has", v);
    
    env_set_local(interp->global_env, "env", env_obj);
    
    /* AI-Native Functions: HTTP/Network */
    // Create http object
    Value http_obj = make_object("http", NULL);
    
    v.as.builtin = builtin_http_get;
    env_set_local(http_obj.as.object_val.fields, "get", v);
    
    v.as.builtin = builtin_http_post;
    env_set_local(http_obj.as.object_val.fields, "post", v);
    
    v.as.builtin = builtin_http_request;
    env_set_local(http_obj.as.object_val.fields, "request", v);
    
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
