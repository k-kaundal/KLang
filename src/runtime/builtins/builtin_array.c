#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Forward declarations for helper functions */
extern Value eval_block(Interpreter *interp, ASTNode *block, Env *env);
extern char *value_to_string(Value *v);

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

Value builtin_range(Interpreter *interp, Value *args, int argc) {
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

/* array.map(callback) - Transform each element */
Value builtin_array_map(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_filter(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_reduce(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_forEach(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_find(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_some(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_every(Interpreter *interp, Value *args, int argc) {
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

/* array.indexOf(value) - Find index of value */
Value builtin_array_indexOf(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_includes(Interpreter *interp, Value *args, int argc) {
    Value result = builtin_array_indexOf(interp, args, argc);
    if (result.type == VAL_INT) {
        return make_bool(result.as.int_val >= 0);
    }
    return make_bool(0);
}

/* array.push(value) - Add to end (mutating) */
Value builtin_array_push(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_pop(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_slice(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_concat(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_join(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_reverse(Interpreter *interp, Value *args, int argc) {
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
Value builtin_array_sort(Interpreter *interp, Value *args, int argc) {
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
