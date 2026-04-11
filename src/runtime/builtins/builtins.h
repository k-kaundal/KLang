#ifndef BUILTINS_H
#define BUILTINS_H

#include "../../interpreter.h"

/* Forward declarations for all builtin functions */

/* Core I/O */
Value builtin_print(Interpreter *interp, Value *args, int argc);
Value builtin_println(Interpreter *interp, Value *args, int argc);
Value builtin_input(Interpreter *interp, Value *args, int argc);

/* Core utilities */
Value builtin_len(Interpreter *interp, Value *args, int argc);
Value builtin_str(Interpreter *interp, Value *args, int argc);
Value builtin_int_fn(Interpreter *interp, Value *args, int argc);
Value builtin_float_fn(Interpreter *interp, Value *args, int argc);
Value builtin_type(Interpreter *interp, Value *args, int argc);
Value builtin_dict(Interpreter *interp, Value *args, int argc);
Value builtin_dict_get(Interpreter *interp, Value *args, int argc);
Value builtin_dict_set(Interpreter *interp, Value *args, int argc);
Value builtin_dict_has(Interpreter *interp, Value *args, int argc);
Value builtin_dict_delete(Interpreter *interp, Value *args, int argc);
Value builtin_dict_keys(Interpreter *interp, Value *args, int argc);
Value builtin_dict_values(Interpreter *interp, Value *args, int argc);
Value builtin_set(Interpreter *interp, Value *args, int argc);
Value builtin_set_add(Interpreter *interp, Value *args, int argc);
Value builtin_set_remove(Interpreter *interp, Value *args, int argc);
Value builtin_set_has(Interpreter *interp, Value *args, int argc);
Value builtin_set_clear(Interpreter *interp, Value *args, int argc);
Value builtin_set_size(Interpreter *interp, Value *args, int argc);
Value builtin_Promise_resolve(Interpreter *interp, Value *args, int argc);
Value builtin_Promise_reject(Interpreter *interp, Value *args, int argc);
Value builtin_Promise_constructor(Interpreter *interp, Value *args, int argc);
Value builtin_Promise_then(Interpreter *interp, Value *args, int argc);
Value builtin_Promise_catch(Interpreter *interp, Value *args, int argc);
Value builtin_Promise_finally(Interpreter *interp, Value *args, int argc);
Value builtin_generator_next(Interpreter *interp, Value *args, int argc);
Value builtin_malloc(Interpreter *interp, Value *args, int argc);
Value builtin_calloc(Interpreter *interp, Value *args, int argc);
Value builtin_realloc(Interpreter *interp, Value *args, int argc);
Value builtin_free(Interpreter *interp, Value *args, int argc);
Value builtin_sizeof(Interpreter *interp, Value *args, int argc);
Value builtin_memstat(Interpreter *interp, Value *args, int argc);

/* String Operations */
Value builtin_uppercase(Interpreter *interp, Value *args, int argc);
Value builtin_lowercase(Interpreter *interp, Value *args, int argc);
Value builtin_trim(Interpreter *interp, Value *args, int argc);
Value builtin_split(Interpreter *interp, Value *args, int argc);
Value builtin_join(Interpreter *interp, Value *args, int argc);
Value builtin_substring(Interpreter *interp, Value *args, int argc);
Value builtin_startswith(Interpreter *interp, Value *args, int argc);
Value builtin_endswith(Interpreter *interp, Value *args, int argc);
Value builtin_contains(Interpreter *interp, Value *args, int argc);
Value builtin_indexof(Interpreter *interp, Value *args, int argc);
Value builtin_replace(Interpreter *interp, Value *args, int argc);
Value builtin_repeat(Interpreter *interp, Value *args, int argc);
Value builtin_regexMatch(Interpreter *interp, Value *args, int argc);
Value builtin_regexReplace(Interpreter *interp, Value *args, int argc);
Value builtin_regexSplit(Interpreter *interp, Value *args, int argc);
Value builtin_regexTest(Interpreter *interp, Value *args, int argc);

/* Array Operations */
Value builtin_array_push(Interpreter *interp, Value *args, int argc);
Value builtin_array_pop(Interpreter *interp, Value *args, int argc);
Value builtin_array_concat(Interpreter *interp, Value *args, int argc);
Value builtin_array_slice(Interpreter *interp, Value *args, int argc);
Value builtin_array_reverse(Interpreter *interp, Value *args, int argc);
Value builtin_array_sort(Interpreter *interp, Value *args, int argc);
Value builtin_array_map(Interpreter *interp, Value *args, int argc);
Value builtin_array_filter(Interpreter *interp, Value *args, int argc);
Value builtin_array_reduce(Interpreter *interp, Value *args, int argc);
Value builtin_array_find(Interpreter *interp, Value *args, int argc);
Value builtin_array_forEach(Interpreter *interp, Value *args, int argc);
Value builtin_array_some(Interpreter *interp, Value *args, int argc);
Value builtin_array_every(Interpreter *interp, Value *args, int argc);
Value builtin_array_includes(Interpreter *interp, Value *args, int argc);
Value builtin_array_indexOf(Interpreter *interp, Value *args, int argc);
Value builtin_array_join(Interpreter *interp, Value *args, int argc);
Value builtin_range(Interpreter *interp, Value *args, int argc);

/* Math Operations */
Value builtin_math_abs(Interpreter *interp, Value *args, int argc);
Value builtin_math_ceil(Interpreter *interp, Value *args, int argc);
Value builtin_math_floor(Interpreter *interp, Value *args, int argc);
Value builtin_math_round(Interpreter *interp, Value *args, int argc);
Value builtin_math_min(Interpreter *interp, Value *args, int argc);
Value builtin_math_max(Interpreter *interp, Value *args, int argc);
Value builtin_math_pow(Interpreter *interp, Value *args, int argc);
Value builtin_math_sqrt(Interpreter *interp, Value *args, int argc);
Value builtin_math_exp(Interpreter *interp, Value *args, int argc);
Value builtin_math_log(Interpreter *interp, Value *args, int argc);
Value builtin_math_log10(Interpreter *interp, Value *args, int argc);
Value builtin_math_sin(Interpreter *interp, Value *args, int argc);
Value builtin_math_cos(Interpreter *interp, Value *args, int argc);
Value builtin_math_tan(Interpreter *interp, Value *args, int argc);
Value builtin_math_asin(Interpreter *interp, Value *args, int argc);
Value builtin_math_acos(Interpreter *interp, Value *args, int argc);
Value builtin_math_atan(Interpreter *interp, Value *args, int argc);
Value builtin_math_atan2(Interpreter *interp, Value *args, int argc);
Value builtin_math_sinh(Interpreter *interp, Value *args, int argc);
Value builtin_math_cosh(Interpreter *interp, Value *args, int argc);
Value builtin_math_tanh(Interpreter *interp, Value *args, int argc);

/* File Operations */
Value builtin_fopen(Interpreter *interp, Value *args, int argc);
Value builtin_fclose(Interpreter *interp, Value *args, int argc);
Value builtin_fread(Interpreter *interp, Value *args, int argc);
Value builtin_freadline(Interpreter *interp, Value *args, int argc);
Value builtin_fwrite(Interpreter *interp, Value *args, int argc);
Value builtin_readFile(Interpreter *interp, Value *args, int argc);
Value builtin_writeFile(Interpreter *interp, Value *args, int argc);
Value builtin_appendFile(Interpreter *interp, Value *args, int argc);
Value builtin_fexists(Interpreter *interp, Value *args, int argc);
Value builtin_fdelete(Interpreter *interp, Value *args, int argc);
Value builtin_frename(Interpreter *interp, Value *args, int argc);
Value builtin_fsize(Interpreter *interp, Value *args, int argc);

/* Environment */
Value builtin_env_get(Interpreter *interp, Value *args, int argc);
Value builtin_env_set(Interpreter *interp, Value *args, int argc);
Value builtin_env_has(Interpreter *interp, Value *args, int argc);

/* HTTP */
Value builtin_http_get(Interpreter *interp, Value *args, int argc);
Value builtin_http_post(Interpreter *interp, Value *args, int argc);
Value builtin_http_request(Interpreter *interp, Value *args, int argc);

/* JSON */
Value builtin_json_parse_stream(Interpreter *interp, Value *args, int argc);

/* Helper function for deep copying values */
Value value_deep_copy(Value v);

#endif
