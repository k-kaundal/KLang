#include "builtins.h"
#include <math.h>

Value builtin_math_abs(Interpreter *interp, Value *args, int argc) {
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

Value builtin_math_ceil(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(ceil(val));
}

Value builtin_math_floor(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(floor(val));
}

Value builtin_math_round(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(round(val));
}

Value builtin_math_min(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double min_val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    for (int i = 1; i < argc; i++) {
        double val = (args[i].type == VAL_FLOAT) ? args[i].as.float_val : (double)args[i].as.int_val;
        if (val < min_val) min_val = val;
    }
    return make_float(min_val);
}

Value builtin_math_max(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double max_val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    for (int i = 1; i < argc; i++) {
        double val = (args[i].type == VAL_FLOAT) ? args[i].as.float_val : (double)args[i].as.int_val;
        if (val > max_val) max_val = val;
    }
    return make_float(max_val);
}

Value builtin_math_pow(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) return make_float(0.0);
    double base = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    double exp = (args[1].type == VAL_FLOAT) ? args[1].as.float_val : (double)args[1].as.int_val;
    return make_float(pow(base, exp));
}

Value builtin_math_sqrt(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(sqrt(val));
}

Value builtin_math_exp(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(1.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(exp(val));
}

Value builtin_math_log(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(log(val));
}

Value builtin_math_log10(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(log10(val));
}

Value builtin_math_sin(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(sin(val));
}

Value builtin_math_cos(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(1.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(cos(val));
}

Value builtin_math_tan(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(tan(val));
}

Value builtin_math_asin(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(asin(val));
}

Value builtin_math_acos(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(acos(val));
}

Value builtin_math_atan(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(atan(val));
}

Value builtin_math_atan2(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) return make_float(0.0);
    double y = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    double x = (args[1].type == VAL_FLOAT) ? args[1].as.float_val : (double)args[1].as.int_val;
    return make_float(atan2(y, x));
}

Value builtin_math_sinh(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(sinh(val));
}

Value builtin_math_cosh(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(1.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(cosh(val));
}

Value builtin_math_tanh(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    double val = (args[0].type == VAL_FLOAT) ? args[0].as.float_val : (double)args[0].as.int_val;
    return make_float(tanh(val));
}
