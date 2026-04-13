#include "runtime/runtime.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

/* KLP Protocol Support */
#ifdef ENABLE_KLP
#include "klp_runtime.h"
#endif

/* Environment variable functions */

Value builtin_env_get(Interpreter *interp, Value *args, int argc) {
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

Value builtin_env_set(Interpreter *interp, Value *args, int argc) {
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

Value builtin_env_has(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: env.has() requires a string argument (variable name)\n");
        return make_bool(0);
    }
    
    const char *var_name = args[0].as.str_val;
    const char *value = getenv(var_name);
    
    return make_bool(value != NULL);
}
