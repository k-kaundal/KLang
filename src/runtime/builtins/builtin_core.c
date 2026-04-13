#include "builtins.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Forward declarations */
extern Value eval_block(Interpreter *interp, ASTNode *block, Env *env);
extern char *value_to_string(Value *v);
extern Env *env_new(Env *parent);
extern void env_set_local(Env *env, const char *name, Value val);
extern void env_free(Env *env);

/* Forward declarations */
Value call_function_helper(Interpreter *interp, Value *func, Value *args, int argc);
void promise_resolve(Interpreter *interp, Value *promise, Value value);
void promise_reject(Interpreter *interp, Value *promise, Value value);

/* Helper function to compare two values for equality */
int values_equal(Value *a, Value *b) {
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
int dict_find_key(Value *dict, Value *key) {
    int i;
    for (i = 0; i < dict->as.dict_val->count; i++) {
        if (values_equal(&dict->as.dict_val->keys[i], key)) {
            return i;
        }
    }
    return -1;
}

/* Find index of value in set, returns -1 if not found */
int set_find_value(Value *set, Value *value) {
    int i;
    for (i = 0; i < set->as.set_val->count; i++) {
        if (values_equal(&set->as.set_val->items[i], value)) {
            return i;
        }
    }
    return -1;
}

/* Helper function to call a function value */
Value call_function_helper(Interpreter *interp, Value *func, Value *args, int argc) {
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

/* Helper to resolve a promise */
void promise_resolve(Interpreter *interp, Value *promise, Value value) {
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

/* Helper to reject a promise */
void promise_reject(Interpreter *interp, Value *promise, Value reason) {
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

/* Memory allocation tracker for debug mode */
typedef struct MemoryBlock {
    void *ptr;
    size_t size;
    const char *file;
    int line;
    struct MemoryBlock *next;
} MemoryBlock;

static MemoryBlock *memory_tracker_head = NULL;
static size_t total_allocated = 0;
static size_t total_deallocated = 0;

/* Track allocated memory in debug mode */
void track_allocation(void *ptr, size_t size) {
    Config *cfg = config_get();
    if (!cfg || !cfg->debug_mode) return;
    
    MemoryBlock *block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    if (!block) return;
    
    block->ptr = ptr;
    block->size = size;
    block->file = "runtime";
    block->line = 0;
    block->next = memory_tracker_head;
    memory_tracker_head = block;
    total_allocated += size;
}

/* Remove tracking for deallocated memory */
void track_deallocation(void *ptr) {
    Config *cfg = config_get();
    if (!cfg || !cfg->debug_mode) return;
    
    MemoryBlock *prev = NULL;
    MemoryBlock *curr = memory_tracker_head;
    
    while (curr) {
        if (curr->ptr == ptr) {
            if (prev) {
                prev->next = curr->next;
            } else {
                memory_tracker_head = curr->next;
            }
            total_deallocated += curr->size;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/* Core Utility Builtin Functions */

Value builtin_len(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_int(0);
    if (args[0].type == VAL_STRING) return make_int((long long)strlen(args[0].as.str_val));
    if (args[0].type == VAL_LIST) return make_int((long long)args[0].as.list_val.count);
    if (args[0].type == VAL_TUPLE) return make_int((long long)args[0].as.tuple_val.count);
    if (args[0].type == VAL_DICT) return make_int((long long)args[0].as.dict_val->count);
    if (args[0].type == VAL_SET) return make_int((long long)args[0].as.set_val->count);
    return make_int(0);
}

Value builtin_int_fn(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_int(0);
    if (args[0].type == VAL_INT) return make_int(args[0].as.int_val);
    if (args[0].type == VAL_FLOAT) return make_int((long long)args[0].as.float_val);
    if (args[0].type == VAL_STRING) return make_int(atoll(args[0].as.str_val));
    if (args[0].type == VAL_BOOL) return make_int(args[0].as.bool_val ? 1 : 0);
    return make_int(0);
}

Value builtin_float_fn(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_float(0.0);
    if (args[0].type == VAL_FLOAT) return make_float(args[0].as.float_val);
    if (args[0].type == VAL_INT) return make_float((double)args[0].as.int_val);
    if (args[0].type == VAL_STRING) return make_float(atof(args[0].as.str_val));
    return make_float(0.0);
}

Value builtin_type(Interpreter *interp, Value *args, int argc) {
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

/* Dictionary Functions */

Value builtin_dict(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    (void)args;
    (void)argc;
    return make_dict();
}

Value builtin_dict_set(Interpreter *interp, Value *args, int argc) {
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

Value builtin_dict_get(Interpreter *interp, Value *args, int argc) {
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

Value builtin_dict_has(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_DICT) {
        return make_bool(0);
    }
    
    Value *dict = &args[0];
    Value key = args[1];
    
    return make_bool(dict_find_key(dict, &key) >= 0);
}

Value builtin_dict_delete(Interpreter *interp, Value *args, int argc) {
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

Value builtin_dict_keys(Interpreter *interp, Value *args, int argc) {
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

Value builtin_dict_values(Interpreter *interp, Value *args, int argc) {
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

/* Set Functions */

Value builtin_set(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    (void)args;
    (void)argc;
    return make_set();
}

Value builtin_set_add(Interpreter *interp, Value *args, int argc) {
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

Value builtin_set_remove(Interpreter *interp, Value *args, int argc) {
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

Value builtin_set_has(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_SET) {
        return make_bool(0);
    }
    
    Value *set = &args[0];
    Value value = args[1];
    
    return make_bool(set_find_value(set, &value) >= 0);
}

Value builtin_set_size(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_SET) {
        return make_int(0);
    }
    
    return make_int(args[0].as.set_val->count);
}

Value builtin_set_clear(Interpreter *interp, Value *args, int argc) {
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

/* Promise Functions */

Value builtin_Promise_then(Interpreter *interp, Value *args, int argc) {
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

Value builtin_Promise_catch(Interpreter *interp, Value *args, int argc) {
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

Value builtin_Promise_finally(Interpreter *interp, Value *args, int argc) {
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

Value builtin_Promise_resolve(Interpreter *interp, Value *args, int argc) {
    Value promise = make_promise();
    Value value = (argc >= 1) ? args[0] : make_null();
    
    Value *promise_ptr = malloc(sizeof(Value));
    *promise_ptr = promise;
    
    promise_resolve(interp, promise_ptr, value);
    
    return *promise_ptr;
}

Value builtin_Promise_reject(Interpreter *interp, Value *args, int argc) {
    Value promise = make_promise();
    Value reason = (argc >= 1) ? args[0] : make_null();
    
    Value *promise_ptr = malloc(sizeof(Value));
    *promise_ptr = promise;
    
    promise_reject(interp, promise_ptr, reason);
    
    return *promise_ptr;
}

/* Generator Functions */

Value builtin_generator_next(Interpreter *interp, Value *args, int argc) {
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
        result.as.object_val = malloc(sizeof(ObjectVal));
        result.as.object_val->class_name = strdup("IteratorResult");
        result.as.object_val->fields = env_new(NULL);
        result.as.object_val->methods = env_new(NULL);
        result.as.object_val->ref_count = 1;
        env_set_local(result.as.object_val->fields, "value", make_null());
        env_set_local(result.as.object_val->fields, "done", make_bool(1));
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
    result.as.object_val = malloc(sizeof(ObjectVal));
    result.as.object_val->class_name = strdup("IteratorResult");
    result.as.object_val->fields = env_new(NULL);
    result.as.object_val->methods = env_new(NULL);
    result.as.object_val->ref_count = 1;
    
    env_set_local(result.as.object_val->fields, "value", yielded_value);
    env_set_local(result.as.object_val->fields, "done", make_bool(is_done));
    
    return result;
}

/* Memory Management Functions (C/C++ Compatibility) */

Value builtin_malloc(Interpreter *interp, Value *args, int argc) {
    Config *cfg = config_get();
    (void)interp;
    
    if (!cfg || !cfg->enable_manual_memory) {
        fprintf(stderr, "Error: Manual memory management is disabled. Set KLANG_ENABLE_MANUAL_MEMORY=1\n");
        return make_null();
    }
    
    if (argc < 1) {
        fprintf(stderr, "Error: malloc() requires size argument\n");
        return make_null();
    }
    
    if (args[0].type != VAL_INT) {
        fprintf(stderr, "Error: malloc() size must be an integer\n");
        return make_null();
    }
    
    size_t size = (size_t)args[0].as.int_val;
    if (size == 0) {
        fprintf(stderr, "Warning: malloc(0) returns NULL\n");
        return make_null();
    }
    
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Error: malloc(%zu) failed - out of memory\n", size);
        return make_null();
    }
    
    track_allocation(ptr, size);
    
    /* Return pointer as integer (address) */
    /* In a full implementation, we'd have VAL_POINTER type */
    Value result;
    result.type = VAL_INT;
    result.as.int_val = (long long)(uintptr_t)ptr;
    return result;
}

Value builtin_calloc(Interpreter *interp, Value *args, int argc) {
    Config *cfg = config_get();
    (void)interp;
    
    if (!cfg || !cfg->enable_manual_memory) {
        fprintf(stderr, "Error: Manual memory management is disabled. Set KLANG_ENABLE_MANUAL_MEMORY=1\n");
        return make_null();
    }
    
    if (argc < 2) {
        fprintf(stderr, "Error: calloc() requires nmemb and size arguments\n");
        return make_null();
    }
    
    if (args[0].type != VAL_INT || args[1].type != VAL_INT) {
        fprintf(stderr, "Error: calloc() arguments must be integers\n");
        return make_null();
    }
    
    size_t nmemb = (size_t)args[0].as.int_val;
    size_t size = (size_t)args[1].as.int_val;
    
    void *ptr = calloc(nmemb, size);
    if (!ptr && (nmemb * size) > 0) {
        fprintf(stderr, "Error: calloc(%zu, %zu) failed - out of memory\n", nmemb, size);
        return make_null();
    }
    
    if (ptr) {
        track_allocation(ptr, nmemb * size);
    }
    
    Value result;
    result.type = VAL_INT;
    result.as.int_val = (long long)(uintptr_t)ptr;
    return result;
}

Value builtin_realloc(Interpreter *interp, Value *args, int argc) {
    Config *cfg = config_get();
    Value result;
    (void)interp;
    
    if (!cfg || !cfg->enable_manual_memory) {
        fprintf(stderr, "Error: Manual memory management is disabled. Set KLANG_ENABLE_MANUAL_MEMORY=1\n");
        return make_null();
    }
    
    if (argc < 2) {
        fprintf(stderr, "Error: realloc() requires ptr and size arguments\n");
        return make_null();
    }
    
    if (args[0].type != VAL_INT || args[1].type != VAL_INT) {
        fprintf(stderr, "Error: realloc() arguments must be integers\n");
        return make_null();
    }
    
    void *old_ptr = (void*)(uintptr_t)args[0].as.int_val;
    size_t new_size = (size_t)args[1].as.int_val;
    
    track_deallocation(old_ptr);
    
    void *new_ptr = realloc(old_ptr, new_size);
    if (!new_ptr && new_size > 0) {
        fprintf(stderr, "Error: realloc(%p, %zu) failed - out of memory\n", old_ptr, new_size);
        /* Note: old_ptr is still valid if realloc fails */
        track_allocation(old_ptr, 0); /* Re-track old pointer */
        result.type = VAL_INT;
        result.as.int_val = (long long)(uintptr_t)old_ptr;
        return result;
    }
    
    if (new_ptr) {
        track_allocation(new_ptr, new_size);
    }
    
    result.type = VAL_INT;
    result.as.int_val = (long long)(uintptr_t)new_ptr;
    return result;
}

Value builtin_free(Interpreter *interp, Value *args, int argc) {
    Config *cfg = config_get();
    (void)interp;
    
    if (!cfg || !cfg->enable_manual_memory) {
        fprintf(stderr, "Error: Manual memory management is disabled. Set KLANG_ENABLE_MANUAL_MEMORY=1\n");
        return make_null();
    }
    
    if (argc < 1) {
        fprintf(stderr, "Error: free() requires ptr argument\n");
        return make_null();
    }
    
    if (args[0].type != VAL_INT) {
        fprintf(stderr, "Error: free() ptr must be an integer (pointer address)\n");
        return make_null();
    }
    
    void *ptr = (void*)(uintptr_t)args[0].as.int_val;
    
    if (!ptr) {
        /* free(NULL) is safe and does nothing */
        return make_null();
    }
    
    track_deallocation(ptr);
    free(ptr);
    
    return make_null();
}

Value builtin_sizeof(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    
    if (argc < 1) {
        fprintf(stderr, "Error: sizeof() requires an argument\n");
        return make_int(0);
    }
    
    /* For now, return sizes based on value type */
    /* In a full implementation, this would work with type names */
    switch (args[0].type) {
        case VAL_INT:
            return make_int(sizeof(long long));
        case VAL_FLOAT:
            return make_int(sizeof(double));
        case VAL_BOOL:
            return make_int(sizeof(int));
        case VAL_STRING:
            return make_int(args[0].as.str_val ? (long long)strlen(args[0].as.str_val) + 1 : 1);
        case VAL_LIST:
            return make_int(sizeof(ListVal) + args[0].as.list_val.capacity * sizeof(Value));
        default:
            return make_int(sizeof(Value));
    }
}

Value builtin_memstat(Interpreter *interp, Value *args, int argc) {
    Config *cfg = config_get();
    (void)interp;
    (void)args;
    (void)argc;
    
    if (!cfg || !cfg->debug_mode) {
        fprintf(stderr, "Error: Memory statistics only available in debug mode\n");
        return make_null();
    }
    
    printf("Memory Statistics:\n");
    printf("  Total allocated:   %zu bytes\n", total_allocated);
    printf("  Total deallocated: %zu bytes\n", total_deallocated);
    printf("  Currently in use:  %zu bytes\n", total_allocated - total_deallocated);
    
    if (memory_tracker_head) {
        printf("\n  Active allocations:\n");
        MemoryBlock *curr = memory_tracker_head;
        int count = 0;
        while (curr) {
            printf("    %d. %p (%zu bytes)\n", ++count, curr->ptr, curr->size);
            curr = curr->next;
        }
    } else {
        printf("  No active allocations\n");
    }
    
    return make_null();
}

/* =================================================================
 * PROMISE CONSTRUCTOR AND HELPERS
 * ================================================================= */

/* Global promise being constructed (for resolve/reject to access)  */
static Value *g_current_promise = NULL;

/* Internal resolve/reject functions for Promise constructor */
static Value builtin_promise_resolve_fn(Interpreter *interp, Value *args, int argc) {
    if (g_current_promise) {
        Value value = (argc >= 1) ? args[0] : make_null();
        promise_resolve(interp, g_current_promise, value);
    }
    return make_null();
}

static Value builtin_promise_reject_fn(Interpreter *interp, Value *args, int argc) {
    if (g_current_promise) {
        Value reason = (argc >= 1) ? args[0] : make_null();
        promise_reject(interp, g_current_promise, reason);
    }
    return make_null();
}

/* Promise constructor: new Promise((resolve, reject) => { ... }) */
Value builtin_Promise_constructor(Interpreter *interp, Value *args, int argc) {
    if (argc < 1 || args[0].type != VAL_FUNCTION) {
        fprintf(stderr, "Error: Promise executor must be a function\n");
        return make_null();
    }
    
    Value promise = make_promise();
    
    // Create a copy of the promise for the resolve/reject functions to capture
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
