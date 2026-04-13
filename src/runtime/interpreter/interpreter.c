#include "interpreter_internal.h"
#include "security.h"

/* Helper function to increment reference count when copying dict/set values */
void value_retain(Value *v) {
    if (v->type == VAL_DICT && v->as.dict_val) {
        v->as.dict_val->ref_count++;
    } else if (v->type == VAL_SET && v->as.set_val) {
        v->as.set_val->ref_count++;
    } else if (v->type == VAL_STRUCT && v->as.struct_val) {
        v->as.struct_val->ref_count++;
    }
}

Env *env_new(Env *parent) {
    Env *e = calloc(1, sizeof(Env));
    e->parent = parent;
    e->ref_count = 1;  // Start with reference count of 1
    return e;
}

void env_free(Env *env) {
    EnvEntry *e = env->entries;
    while (e) {
        EnvEntry *next = e->next;
        REFCOUNT_LOG("env_free: freeing entry '%s'", e->name);
        value_free(&e->value);
        free(e->name);
        free(e);
        e = next;
    }
    free(env);
}

void env_retain(Env *env) {
    if (env) {
        env->ref_count++;
    }
}

void env_release(Env *env) {
    if (env) {
        env->ref_count--;
        if (env->ref_count <= 0) {
            env_free(env);
        }
    }
}

Value *env_get(Env *env, const char *name) {
    Env *cur = env;
    while (cur) {
        EnvEntry *e = cur->entries;
        while (e) {
            if (strcmp(e->name, name) == 0) return &e->value;
            e = e->next;
        }
        cur = cur->parent;
    }
    return NULL;
}

EnvEntry *env_get_entry(Env *env, const char *name) {
    Env *cur = env;
    while (cur) {
        EnvEntry *e = cur->entries;
        while (e) {
            if (strcmp(e->name, name) == 0) return e;
            e = e->next;
        }
        cur = cur->parent;
    }
    return NULL;
}

int can_access_member(AccessModifier access, int is_same_class, int is_subclass) {
    if (access == ACCESS_PUBLIC) return 1;
    if (access == ACCESS_PRIVATE) return is_same_class;
    if (access == ACCESS_PROTECTED) return is_same_class || is_subclass;
    return 0;
}

void env_set_local(Env *env, const char *name, Value val) {
    // Deep copy to avoid double-free issues
    Value val_copy = val;
    if (val.type == VAL_STRING && val.as.str_val) {
        val_copy.as.str_val = strdup(val.as.str_val);
    } else if (val.type == VAL_FUNCTION) {
        // Deep copy function parameter names if they exist
        if (val.as.func_val.param_names) {
            val_copy.as.func_val.param_names = malloc(val.as.func_val.param_count * sizeof(char *));
            int i;
            for (i = 0; i < val.as.func_val.param_count; i++) {
                val_copy.as.func_val.param_names[i] = strdup(val.as.func_val.param_names[i]);
            }
        }
        // Note: body and closure are shared pointers
        // Retain closure to keep it alive
        if (val_copy.as.func_val.closure) {
            env_retain(val_copy.as.func_val.closure);
        }
    }
    // Note: For dict/set/object, we do shallow copy (share the same dict/set/object)
    // Increment reference count when storing in environment
    if (val.type == VAL_DICT && val.as.dict_val) {
        val.as.dict_val->ref_count++;
    } else if (val.type == VAL_SET && val.as.set_val) {
        val.as.set_val->ref_count++;
    } else if (val.type == VAL_OBJECT && val.as.object_val) {
        val.as.object_val->ref_count++;
        REFCOUNT_LOG("env_set_local(%s): ptr=%p ref_count=%d (after increment)", 
                     name, (void*)val.as.object_val, val.as.object_val->ref_count);
    }
    
    EnvEntry *e = env->entries;
    while (e) {
        if (strcmp(e->name, name) == 0) {
            value_free(&e->value);
            e->value = val_copy;
            return;
        }
        e = e->next;
    }
    {
        EnvEntry *ne = malloc(sizeof(EnvEntry));
        ne->name = strdup(name);
        ne->value = val_copy;
        ne->access = ACCESS_PUBLIC;
        ne->decl_type = DECL_LET;
        ne->is_const = 0;
        ne->next = env->entries;
        env->entries = ne;
    }
}

void env_set_local_with_access(Env *env, const char *name, Value val, AccessModifier access) {
    // Deep copy strings to avoid double-free issues
    Value val_copy = val;
    if (val.type == VAL_STRING && val.as.str_val) {
        val_copy.as.str_val = strdup(val.as.str_val);
    } else if (val.type == VAL_FUNCTION) {
        // Deep copy function parameter names if they exist
        if (val.as.func_val.param_names) {
            val_copy.as.func_val.param_names = malloc(val.as.func_val.param_count * sizeof(char *));
            int i;
            for (i = 0; i < val.as.func_val.param_count; i++) {
                val_copy.as.func_val.param_names[i] = strdup(val.as.func_val.param_names[i]);
            }
        }
        // Retain closure to keep it alive
        if (val_copy.as.func_val.closure) {
            env_retain(val_copy.as.func_val.closure);
        }
    }
    // Note: For dict/set/object, we do shallow copy (share the same dict/set/object)
    // Increment reference count when storing in environment
    if (val.type == VAL_DICT && val.as.dict_val) {
        val.as.dict_val->ref_count++;
    } else if (val.type == VAL_SET && val.as.set_val) {
        val.as.set_val->ref_count++;
    } else if (val.type == VAL_OBJECT && val.as.object_val) {
        val.as.object_val->ref_count++;
        REFCOUNT_LOG("env_set_local_with_access(%s): ptr=%p ref_count=%d (after increment)", 
                     name, (void*)val.as.object_val, val.as.object_val->ref_count);
    }
    
    EnvEntry *e = env->entries;
    while (e) {
        if (strcmp(e->name, name) == 0) {
            value_free(&e->value);
            e->value = val_copy;
            e->access = access;
            return;
        }
        e = e->next;
    }
    {
        EnvEntry *ne = malloc(sizeof(EnvEntry));
        ne->name = strdup(name);
        ne->value = val_copy;
        ne->access = access;
        ne->decl_type = DECL_LET;
        ne->is_const = 0;
        ne->next = env->entries;
        env->entries = ne;
    }
}

void env_set(Env *env, const char *name, Value val) {
    Env *cur = env;
    while (cur) {
        EnvEntry *e = cur->entries;
        while (e) {
            if (strcmp(e->name, name) == 0) {
                /* Check if trying to assign to const */
                if (e->is_const) {
                    fprintf(stderr, "Error: cannot assign to const variable '%s'\n", name);
                    return;
                }
                value_free(&e->value);
                e->value = val;
                return;
            }
            e = e->next;
        }
        cur = cur->parent;
    }
    env_set_local(env, name, val);
}

/* Check if a variable exists in the local scope only */
int env_has_local(Env *env, const char *name) {
    EnvEntry *e = env->entries;
    while (e) {
        if (strcmp(e->name, name) == 0) return 1;
        e = e->next;
    }
    return 0;
}

/* Declare a new variable with const/let/var semantics */
void env_declare(Env *env, const char *name, Value val, DeclType decl_type, int line, Interpreter *interp) {
    Value val_copy = val;
    if (val.type == VAL_STRING && val.as.str_val) {
        val_copy.as.str_val = strdup(val.as.str_val);
    } else if (val.type == VAL_FUNCTION && val.as.func_val.param_count > 0) {
        /* Deep copy function parameter names */
        int i;
        val_copy.as.func_val.param_names = malloc(val.as.func_val.param_count * sizeof(char *));
        for (i = 0; i < val.as.func_val.param_count; i++) {
            val_copy.as.func_val.param_names[i] = strdup(val.as.func_val.param_names[i]);
        }
    }
    
    /* Check for redeclaration in local scope */
    if (decl_type == DECL_LET || decl_type == DECL_CONST) {
        if (env_has_local(env, name)) {
            fprintf(stderr, "Error at line %d: identifier '%s' has already been declared\n", line, name);
            interp->had_error = 1;
            value_free(&val_copy);
            return;
        }
    }
    
    /* var allows redeclaration, so check and update if exists locally */
    if (decl_type == DECL_VAR && env_has_local(env, name)) {
        EnvEntry *e = env->entries;
        while (e) {
            if (strcmp(e->name, name) == 0) {
                value_free(&e->value);
                e->value = val_copy;
                return;
            }
            e = e->next;
        }
    }
    
    /* Create new entry */
    {
        EnvEntry *ne = malloc(sizeof(EnvEntry));
        ne->name = strdup(name);
        ne->value = val_copy;
        ne->access = ACCESS_PUBLIC;
        ne->decl_type = decl_type;
        ne->is_const = (decl_type == DECL_CONST);
        ne->next = env->entries;
        env->entries = ne;
    }
}

Value make_int(long long v) {
    Value val;
    val.type = VAL_INT;
    val.as.int_val = v;
    return val;
}

Value make_float(double v) {
    Value val;
    val.type = VAL_FLOAT;
    val.as.float_val = v;
    return val;
}

Value make_string(const char *s) {
    Value val;
    val.type = VAL_STRING;
    val.as.str_val = strdup(s);
    return val;
}

Value make_bool(int v) {
    Value val;
    val.type = VAL_BOOL;
    val.as.bool_val = v;
    return val;
}

Value make_null(void) {
    Value val;
    val.type = VAL_NULL;
    return val;
}

Value make_class(const char *name, const char *parent_name) {
    Value val;
    val.type = VAL_CLASS;
    val.as.class_val.name = strdup(name);
    val.as.class_val.parent_name = parent_name ? strdup(parent_name) : NULL;
    val.as.class_val.methods = NULL;
    val.as.class_val.fields = NULL;
    val.as.class_val.static_methods = env_new(NULL);  /* Initialize to prevent NULL pointer */
    val.as.class_val.static_fields = env_new(NULL);   /* Initialize to prevent NULL pointer */
    return val;
}

Value make_object(const char *class_name, Env *methods) {
    Value val;
    val.type = VAL_OBJECT;
    val.as.object_val = malloc(sizeof(ObjectVal));
    val.as.object_val->class_name = strdup(class_name);
    val.as.object_val->fields = env_new(NULL);
    val.as.object_val->methods = methods;
    val.as.object_val->ref_count = 1;
    REFCOUNT_LOG("make_object(%s): ptr=%p ref_count=1", class_name, (void*)val.as.object_val);
    return val;
}

Value make_method(Value receiver, Value method) {
    Value val;
    val.type = VAL_METHOD;
    val.as.method_val.receiver = malloc(sizeof(Value));
    *val.as.method_val.receiver = receiver;
    /* Increment reference count when creating method with object receiver */
    if (receiver.type == VAL_OBJECT && receiver.as.object_val) {
        receiver.as.object_val->ref_count++;
        REFCOUNT_LOG("make_method: ptr=%p ref_count=%d (after increment)", 
                     (void*)receiver.as.object_val, receiver.as.object_val->ref_count);
    }
    
    val.as.method_val.method = malloc(sizeof(Value));
    *val.as.method_val.method = method;
    /* Deep copy function parameter names */
    if (method.type == VAL_FUNCTION && method.as.func_val.param_names) {
        val.as.method_val.method->as.func_val.param_names = 
            malloc(method.as.func_val.param_count * sizeof(char *));
        int i;
        for (i = 0; i < method.as.func_val.param_count; i++) {
            val.as.method_val.method->as.func_val.param_names[i] = 
                strdup(method.as.func_val.param_names[i]);
        }
    }
    
    return val;
}

Value make_promise(void) {
    Value val;
    val.type = VAL_PROMISE;
    val.as.promise_val.state = PROMISE_PENDING;
    val.as.promise_val.result = malloc(sizeof(Value));  /* Allocate result storage */
    *val.as.promise_val.result = make_null();           /* Initialize to null */
    val.as.promise_val.callbacks = NULL;
    return val;
}

Value make_module(const char *module_path, Env *exports, Env *module_env) {
    Value val;
    val.type = VAL_MODULE;
    val.as.module_val.module_path = strdup(module_path);
    val.as.module_val.exports = exports;
    val.as.module_val.module_env = module_env;
    return val;
}

Value make_dict(void) {
    Value val;
    val.type = VAL_DICT;
    val.as.dict_val = malloc(sizeof(DictVal));
    val.as.dict_val->keys = NULL;
    val.as.dict_val->values = NULL;
    val.as.dict_val->count = 0;
    val.as.dict_val->capacity = 0;
    val.as.dict_val->ref_count = 1;
    return val;
}

Value make_set(void) {
    Value val;
    val.type = VAL_SET;
    val.as.set_val = malloc(sizeof(SetVal));
    val.as.set_val->items = NULL;
    val.as.set_val->count = 0;
    val.as.set_val->capacity = 0;
    val.as.set_val->ref_count = 1;
    return val;
}

Value make_pointer(void *ptr, const char *type_name) {
    Value val;
    val.type = VAL_POINTER;
    val.as.pointer_val.ptr = ptr;
    val.as.pointer_val.type_name = type_name ? strdup(type_name) : NULL;
    return val;
}

Value make_struct(const char *struct_name, int is_union) {
    Value val;
    val.type = VAL_STRUCT;
    val.as.struct_val = malloc(sizeof(StructVal));
    val.as.struct_val->struct_name = struct_name ? strdup(struct_name) : NULL;
    val.as.struct_val->fields = env_new(NULL);
    val.as.struct_val->is_union = is_union;
    val.as.struct_val->ref_count = 1;
    return val;
}

Value make_generator(FunctionVal *func, Env *env) {
    Value val;
    val.type = VAL_GENERATOR;
    val.as.generator_val.state = GEN_SUSPENDED;
    val.as.generator_val.func = func;
    val.as.generator_val.saved_env = env;
    val.as.generator_val.yield_index = 0;
    val.as.generator_val.last_value = malloc(sizeof(Value));
    *val.as.generator_val.last_value = make_null();
    return val;
}


void value_free(Value *v) {
    if (!v) return;
    REFCOUNT_LOG("value_free: type=%d", v->type);
    if (v->type == VAL_STRING && v->as.str_val) {
        free(v->as.str_val);
        v->as.str_val = NULL;
    }
    if (v->type == VAL_FUNCTION) {
        if (v->as.func_val.param_names) {
            int i;
            for (i = 0; i < v->as.func_val.param_count; i++)
                free(v->as.func_val.param_names[i]);
            free(v->as.func_val.param_names);
            v->as.func_val.param_names = NULL;
        }
        /* Release the closure environment (reference counted) */
        if (v->as.func_val.closure) {
            env_release(v->as.func_val.closure);
            v->as.func_val.closure = NULL;
        }
        /* Note: default_values array and its contents are owned by the AST, not freed here */
    }
    if (v->type == VAL_LIST) {
        int i;
        for (i = 0; i < v->as.list_val.count; i++)
            value_free(&v->as.list_val.items[i]);
        free(v->as.list_val.items);
        v->as.list_val.items = NULL;
    }
    if (v->type == VAL_TUPLE) {
        int i;
        for (i = 0; i < v->as.tuple_val.count; i++)
            value_free(&v->as.tuple_val.elements[i]);
        free(v->as.tuple_val.elements);
        v->as.tuple_val.elements = NULL;
    }
    if (v->type == VAL_CLASS) {
        // Don't free Env structures to avoid double-free issues
        // when class is copied/stored in multiple places.
        // NOTE: This is a known memory management limitation in KLang's Value system.
        // Classes share Env pointers across copies, so freeing them causes double-frees.
        // A proper fix would require reference counting or unique ownership semantics.
        if (v->as.class_val.name) {
            // Keep allocated - will leak but prevents crashes
            v->as.class_val.name = NULL;
        }
        if (v->as.class_val.parent_name) {
            // Keep allocated - will leak but prevents crashes
            v->as.class_val.parent_name = NULL;
        }
        // Don't free env structures - they're shared
        v->as.class_val.methods = NULL;
        v->as.class_val.fields = NULL;
        v->as.class_val.static_methods = NULL;
        v->as.class_val.static_fields = NULL;
    }
    if (v->type == VAL_OBJECT && v->as.object_val) {
        // Decrement reference count and only free when count reaches 0
        REFCOUNT_LOG("value_free: ptr=%p ref_count=%d (before decrement)", 
                     (void*)v->as.object_val, v->as.object_val->ref_count);
        v->as.object_val->ref_count--;
        if (v->as.object_val->ref_count <= 0) {
            REFCOUNT_LOG("value_free: ptr=%p FREEING (ref_count reached 0)", (void*)v->as.object_val);
            if (v->as.object_val->class_name) {
                free(v->as.object_val->class_name);
            }
            if (v->as.object_val->fields) {
                env_free(v->as.object_val->fields);
            }
            if (v->as.object_val->methods) {
                env_free(v->as.object_val->methods);
            }
            free(v->as.object_val);
            v->as.object_val = NULL;  // Only null out if we actually freed it
        } else {
            REFCOUNT_LOG("value_free: ptr=%p ref_count=%d (after decrement, not freeing)", 
                         (void*)v->as.object_val, v->as.object_val->ref_count);
            // Don't null out the pointer if we didn't free - it's still valid for other references
        }
    }
    if (v->type == VAL_METHOD) {
        if (v->as.method_val.receiver) {
            // Properly free the receiver value first (decrements ref_count)
            REFCOUNT_LOG("value_free(METHOD): freeing receiver");
            value_free(v->as.method_val.receiver);
            // The pointer is already freed by value_free if needed, just free the container
            free(v->as.method_val.receiver);
            v->as.method_val.receiver = NULL;
        }
        if (v->as.method_val.method) {
            // Free the method function value
            value_free(v->as.method_val.method);
            free(v->as.method_val.method);
            v->as.method_val.method = NULL;
        }
    }
    if (v->type == VAL_PROMISE) {
        // Free promise callbacks
        PromiseCallbackNode *node = v->as.promise_val.callbacks;
        while (node) {
            PromiseCallbackNode *next = node->next;
            if (node->on_fulfilled) {
                value_free(node->on_fulfilled);
                free(node->on_fulfilled);
            }
            if (node->on_rejected) {
                value_free(node->on_rejected);
                free(node->on_rejected);
            }
            if (node->promise_to_resolve) {
                value_free(node->promise_to_resolve);
                free(node->promise_to_resolve);
            }
            free(node);
            node = next;
        }
        if (v->as.promise_val.result) {
            value_free(v->as.promise_val.result);
            free(v->as.promise_val.result);
        }
    }
    if (v->type == VAL_MODULE) {
        if (v->as.module_val.module_path) {
            free(v->as.module_val.module_path);
            v->as.module_val.module_path = NULL;
        }
        // Note: exports and module_env are owned by the module cache in the interpreter
        // and will be freed when the interpreter is freed. Don't free them here to avoid double-free.
    }
    if (v->type == VAL_GENERATOR) {
        /* Don't free func as it contains pointers to AST nodes owned by parser */
        /* Don't free saved_env to avoid double-free with closure env */
        /* Don't free last_value to avoid double-free */
        /* TODO: Implement proper reference counting for generator lifecycle */
        /* This will leak memory but prevents crashes */
    }
    if (v->type == VAL_FILE) {
        /* Don't free FILE resources - files are reference types */
        /* User should explicitly call fclose() */
        /* This prevents double-free when file values are shared */
        v->as.file_val.fp = NULL;
        v->as.file_val.is_open = 0;
        v->as.file_val.path = NULL;
        v->as.file_val.mode = NULL;
    }
    if (v->type == VAL_DICT) {
        /* Decrement reference count and free only when it reaches 0 */
        if (v->as.dict_val != NULL) {
            v->as.dict_val->ref_count--;
            if (v->as.dict_val->ref_count <= 0) {
                /* Free all keys and values */
                int i;
                for (i = 0; i < v->as.dict_val->count; i++) {
                    value_free(&v->as.dict_val->keys[i]);
                    value_free(&v->as.dict_val->values[i]);
                }
                free(v->as.dict_val->keys);
                free(v->as.dict_val->values);
                free(v->as.dict_val);
            }
            v->as.dict_val = NULL;
        }
    }
    if (v->type == VAL_SET) {
        /* Decrement reference count and free only when it reaches 0 */
        if (v->as.set_val != NULL) {
            v->as.set_val->ref_count--;
            if (v->as.set_val->ref_count <= 0) {
                /* Free all items */
                int i;
                for (i = 0; i < v->as.set_val->count; i++) {
                    value_free(&v->as.set_val->items[i]);
                }
                free(v->as.set_val->items);
                free(v->as.set_val);
            }
            v->as.set_val = NULL;
        }
    }
    if (v->type == VAL_POINTER) {
        /* Free type name if allocated */
        if (v->as.pointer_val.type_name) {
            free(v->as.pointer_val.type_name);
            v->as.pointer_val.type_name = NULL;
        }
        /* Note: We don't free the pointer itself as we don't own it */
        v->as.pointer_val.ptr = NULL;
    }
    if (v->type == VAL_STRUCT && v->as.struct_val) {
        /* Decrement reference count and free only when it reaches 0 */
        v->as.struct_val->ref_count--;
        if (v->as.struct_val->ref_count <= 0) {
            if (v->as.struct_val->struct_name) {
                free(v->as.struct_val->struct_name);
            }
            if (v->as.struct_val->fields) {
                env_free(v->as.struct_val->fields);
            }
            free(v->as.struct_val);
        }
        v->as.struct_val = NULL;
    }
}

char *value_to_string(Value *v) {
    char buf[64];
    switch (v->type) {
        case VAL_INT:
            snprintf(buf, sizeof(buf), "%lld", v->as.int_val);
            return strdup(buf);
        case VAL_FLOAT:
            snprintf(buf, sizeof(buf), "%g", v->as.float_val);
            return strdup(buf);
        case VAL_STRING:
            return strdup(v->as.str_val);
        case VAL_BOOL:
            return strdup(v->as.bool_val ? "true" : "false");
        case VAL_NULL:
            return strdup("null");
        case VAL_FUNCTION:
            return strdup("<function>");
        case VAL_BUILTIN:
            return strdup("<builtin>");
        case VAL_LIST: {
            // Build array string representation [1, 2, 3]
            int count = v->as.list_val.count;
            if (count == 0) {
                return strdup("[]");
            }
            
            // Calculate total length needed
            int total_len = 2; // For [ and ]
            char **item_strs = malloc(count * sizeof(char *));
            for (int i = 0; i < count; i++) {
                item_strs[i] = value_to_string(&v->as.list_val.items[i]);
                total_len += strlen(item_strs[i]);
                if (i > 0) total_len += 2; // For ", "
            }
            
            // Build the string
            char *result = malloc(total_len + 1);
            strcpy(result, "[");
            for (int i = 0; i < count; i++) {
                if (i > 0) strcat(result, ", ");
                strcat(result, item_strs[i]);
                free(item_strs[i]);
            }
            strcat(result, "]");
            free(item_strs);
            return result;
        }
        case VAL_TUPLE: {
            // Build tuple string representation (1, 2, 3)
            int count = v->as.tuple_val.count;
            if (count == 0) {
                return strdup("()");
            }
            if (count == 1) {
                // Single element tuple needs trailing comma
                char *item_str = value_to_string(&v->as.tuple_val.elements[0]);
                int len = strlen(item_str) + 4; // (item,)
                char *result = malloc(len);
                snprintf(result, len, "(%s,)", item_str);
                free(item_str);
                return result;
            }
            
            // Calculate total length needed
            int total_len = 2; // For ( and )
            char **item_strs = malloc(count * sizeof(char *));
            for (int i = 0; i < count; i++) {
                item_strs[i] = value_to_string(&v->as.tuple_val.elements[i]);
                total_len += strlen(item_strs[i]);
                if (i > 0) total_len += 2; // For ", "
            }
            
            // Build the string
            char *result = malloc(total_len + 1);
            strcpy(result, "(");
            for (int i = 0; i < count; i++) {
                if (i > 0) strcat(result, ", ");
                strcat(result, item_strs[i]);
                free(item_strs[i]);
            }
            strcat(result, ")");
            free(item_strs);
            return result;
        }
        case VAL_CLASS:
            snprintf(buf, sizeof(buf), "<class %s>", v->as.class_val.name);
            return strdup(buf);
        case VAL_OBJECT:
            snprintf(buf, sizeof(buf), "<object %s>", v->as.object_val->class_name);
            return strdup(buf);
        case VAL_METHOD:
            return strdup("<method>");
        case VAL_PROMISE: {
            const char *state_str;
            switch (v->as.promise_val.state) {
                case PROMISE_PENDING: state_str = "pending"; break;
                case PROMISE_FULFILLED: state_str = "fulfilled"; break;
                case PROMISE_REJECTED: state_str = "rejected"; break;
                default: state_str = "unknown"; break;
            }
            snprintf(buf, sizeof(buf), "Promise { <%s> }", state_str);
            return strdup(buf);
        }
        case VAL_MODULE:
            snprintf(buf, sizeof(buf), "<module %s>", 
                v->as.module_val.module_path ? v->as.module_val.module_path : "unknown");
            return strdup(buf);
        case VAL_GENERATOR: {
            const char *state_str;
            switch (v->as.generator_val.state) {
                case GEN_RUNNING: state_str = "running"; break;
                case GEN_SUSPENDED: state_str = "suspended"; break;
                case GEN_COMPLETED: state_str = "completed"; break;
                default: state_str = "unknown"; break;
            }
            snprintf(buf, sizeof(buf), "<generator %s>", state_str);
            return strdup(buf);
        }
        case VAL_DICT: {
            // Build dictionary string representation {key: value, ...}
            if (!v->as.dict_val) {
                return strdup("{<freed>}");
            }
            int count = v->as.dict_val->count;
            if (count == 0) {
                return strdup("{}");
            }
            
            // Calculate total length needed
            int total_len = 2; // For { and }
            char **key_strs = malloc(count * sizeof(char *));
            char **val_strs = malloc(count * sizeof(char *));
            for (int i = 0; i < count; i++) {
                key_strs[i] = value_to_string(&v->as.dict_val->keys[i]);
                val_strs[i] = value_to_string(&v->as.dict_val->values[i]);
                total_len += strlen(key_strs[i]) + strlen(val_strs[i]) + 2; // +2 for ": "
                if (i > 0) total_len += 2; // For ", "
            }
            
            // Build the string
            char *result = malloc(total_len + 1);
            strcpy(result, "{");
            for (int i = 0; i < count; i++) {
                if (i > 0) strcat(result, ", ");
                strcat(result, key_strs[i]);
                strcat(result, ": ");
                strcat(result, val_strs[i]);
                free(key_strs[i]);
                free(val_strs[i]);
            }
            strcat(result, "}");
            free(key_strs);
            free(val_strs);
            return result;
        }
        case VAL_SET: {
            // Build set string representation {value, ...}
            if (!v->as.set_val) {
                return strdup("set(<freed>)");
            }
            int count = v->as.set_val->count;
            if (count == 0) {
                return strdup("set()");
            }
            
            // Calculate total length needed
            int total_len = 2; // For { and }
            char **item_strs = malloc(count * sizeof(char *));
            for (int i = 0; i < count; i++) {
                item_strs[i] = value_to_string(&v->as.set_val->items[i]);
                total_len += strlen(item_strs[i]);
                if (i > 0) total_len += 2; // For ", "
            }
            
            // Build the string
            char *result = malloc(total_len + 1);
            strcpy(result, "{");
            for (int i = 0; i < count; i++) {
                if (i > 0) strcat(result, ", ");
                strcat(result, item_strs[i]);
                free(item_strs[i]);
            }
            strcat(result, "}");
            free(item_strs);
            return result;
        }
        case VAL_POINTER: {
            // Print pointer with address and optional type
            if (v->as.pointer_val.type_name) {
                snprintf(buf, sizeof(buf), "*%s<%p>", 
                    v->as.pointer_val.type_name, 
                    v->as.pointer_val.ptr);
            } else {
                snprintf(buf, sizeof(buf), "<pointer %p>", v->as.pointer_val.ptr);
            }
            return strdup(buf);
        }
        case VAL_STRUCT: {
            // Print struct/union with type name and fields
            if (!v->as.struct_val) {
                return strdup("struct(<freed>)");
            }
            snprintf(buf, sizeof(buf), "%s %s { ... }", 
                v->as.struct_val->is_union ? "union" : "struct",
                v->as.struct_val->struct_name ? v->as.struct_val->struct_name : "?");
            return strdup(buf);
        }
        default:
            return strdup("<unknown>");
    }
}

void value_print(Value *v) {
    char *s = value_to_string(v);
    printf("%s", s);
    free(s);
}

Interpreter *interpreter_new(void) {
    Interpreter *interp = calloc(1, sizeof(Interpreter));
    interp->global_env = env_new(NULL);
    interp->microtask_queue_head = NULL;
    interp->microtask_queue_tail = NULL;
    interp->loaded_modules = NULL;
    interp->module_count = 0;
    interp->module_capacity = 0;
    interp->current_module_dir = NULL;
    
    /* Initialize security context (Phase 2B) */
    interp->security = security_context_new();
    
    return interp;
}

void interpreter_free(Interpreter *interp) {
    int i;
    // Free microtask queue
    MicrotaskNode *node = interp->microtask_queue_head;
    while (node) {
        MicrotaskNode *next = node->next;
        value_free(&node->callback);
        for (i = 0; i < node->argc; i++) {
            value_free(&node->args[i]);
        }
        free(node->args);
        free(node);
        node = next;
    }
    // Free loaded modules
    for (i = 0; i < interp->module_count; i++) {
        int j;
        free(interp->loaded_modules[i].path);
        env_release(interp->loaded_modules[i].exports);
        env_release(interp->loaded_modules[i].module_env);
        // Free AST nodes
        for (j = 0; j < interp->loaded_modules[i].ast_count; j++) {
            ast_free(interp->loaded_modules[i].ast_nodes[j]);
        }
        free(interp->loaded_modules[i].ast_nodes);
    }
    free(interp->loaded_modules);
    if (interp->current_module_dir) free(interp->current_module_dir);
    
    /* Free security context (Phase 2B) */
    if (interp->security) {
        security_context_free(interp->security);
        interp->security = NULL;
    }
    
    // Only release global_env if it hasn't been freed already
    if (interp->global_env && interp->global_env->ref_count > 0) {
        env_release(interp->global_env);
    }
    free(interp);
}

void microtask_queue_push(Interpreter *interp, Value callback, Value *args, int argc) {
    MicrotaskNode *node = malloc(sizeof(MicrotaskNode));
    node->callback = callback;
    node->argc = argc;
    node->args = NULL;
    node->next = NULL;
    
    if (argc > 0) {
        node->args = malloc(argc * sizeof(Value));
        for (int i = 0; i < argc; i++) {
            node->args[i] = args[i];
        }
    }
    
    if (interp->microtask_queue_tail) {
        interp->microtask_queue_tail->next = node;
        interp->microtask_queue_tail = node;
    } else {
        interp->microtask_queue_head = node;
        interp->microtask_queue_tail = node;
    }
}

void microtask_queue_process(Interpreter *interp) {
    while (interp->microtask_queue_head) {
        MicrotaskNode *node = interp->microtask_queue_head;
        interp->microtask_queue_head = node->next;
        if (!interp->microtask_queue_head) {
            interp->microtask_queue_tail = NULL;
        }
        
        // Execute the callback
        if (node->callback.type == VAL_FUNCTION) {
            Env *call_env = env_new(node->callback.as.func_val.closure);
            for (int i = 0; i < node->callback.as.func_val.param_count && i < node->argc; i++) {
                env_set_local(call_env, node->callback.as.func_val.param_names[i], node->args[i]);
            }
            Value result = eval_block(interp, node->callback.as.func_val.body, call_env);
            
            // Handle return value
            if (interp->last_result.is_return) {
                value_free(&result);
                result = interp->last_result.return_value;
                interp->last_result.is_return = 0;
                interp->last_result.return_value = make_null();
            }
            
            value_free(&result);
            env_release(call_env);
        }
        
        // Free the node
        value_free(&node->callback);
        for (int i = 0; i < node->argc; i++) {
            value_free(&node->args[i]);
        }
        free(node->args);
        free(node);
    }
}

/* ========== Module System Implementation ========== */

/* Resolve module path to absolute path */
char *resolve_module_path(Interpreter *interp, const char *import_path) {
    char *resolved = malloc(PATH_MAX);
    char *with_ext = malloc(PATH_MAX);
    FILE *test_file;
    
    /* If it starts with ./ or ../ it's relative */
    if (import_path[0] == '.' && (import_path[1] == '/' || 
        (import_path[1] == '.' && import_path[2] == '/'))) {
        
        /* Get the directory of the current module */
        char *base_dir = interp->current_module_dir ? interp->current_module_dir : ".";
        snprintf(resolved, PATH_MAX, "%s/%s", base_dir, import_path);
        
        /* Try with .kl extension if file doesn't exist */
        test_file = fopen(resolved, "r");
        if (!test_file && !strstr(import_path, ".kl")) {
            snprintf(with_ext, PATH_MAX, "%s.kl", resolved);
            test_file = fopen(with_ext, "r");
            if (test_file) {
                strcpy(resolved, with_ext);
                fclose(test_file);
            }
        } else if (test_file) {
            fclose(test_file);
        }
    } 
    /* If it starts with stdlib/, resolve to the stdlib directory */
    else if (strncmp(import_path, "stdlib/", 7) == 0) {
        const char *module_name = import_path + 7;  /* Skip "stdlib/" */
        /* Check if KLANG_STDLIB_PATH is set */
        const char *stdlib_path = getenv("KLANG_STDLIB_PATH");
        if (stdlib_path) {
            snprintf(resolved, PATH_MAX, "%s/%s", stdlib_path, module_name);
        } else {
            /* Try multiple locations for stdlib */
            const char *search_paths[] = {
                "./stdlib/%s",
                "../stdlib/%s", 
                "../../stdlib/%s",
                "/usr/local/lib/klang/stdlib/%s",
                "/usr/lib/klang/stdlib/%s",
                NULL
            };
            int found = 0;
            int i;
            for (i = 0; search_paths[i] != NULL; i++) {
                snprintf(resolved, PATH_MAX, search_paths[i], module_name);
                test_file = fopen(resolved, "r");
                if (!test_file && !strstr(module_name, ".kl")) {
                    snprintf(with_ext, PATH_MAX, "%s.kl", resolved);
                    test_file = fopen(with_ext, "r");
                    if (test_file) {
                        strcpy(resolved, with_ext);
                        found = 1;
                    }
                }
                if (test_file) {
                    fclose(test_file);
                    found = 1;
                    break;
                }
            }
            /* If still not found and no .kl extension, add it */
            if (!found && !strstr(resolved, ".kl")) {
                snprintf(with_ext, PATH_MAX, "%s.kl", resolved);
                strcpy(resolved, with_ext);
            }
        }
    }
    else {
        /* Absolute path or bare import - for now treat as relative to cwd */
        if (import_path[0] == '/') {
            strncpy(resolved, import_path, PATH_MAX);
        } else {
            /* Try current directory */
            snprintf(resolved, PATH_MAX, "./%s", import_path);
        }
        
        /* Try with .kl extension if file doesn't exist */
        test_file = fopen(resolved, "r");
        if (!test_file && !strstr(import_path, ".kl")) {
            snprintf(with_ext, PATH_MAX, "%s.kl", resolved);
            test_file = fopen(with_ext, "r");
            if (test_file) {
                strcpy(resolved, with_ext);
                fclose(test_file);
            }
        } else if (test_file) {
            fclose(test_file);
        }
    }
    
    free(with_ext);
    return resolved;
}

/* Get cached module */
LoadedModule *get_cached_module(Interpreter *interp, const char *module_path) {
    int i;
    for (i = 0; i < interp->module_count; i++) {
        if (strcmp(interp->loaded_modules[i].path, module_path) == 0) {
            return &interp->loaded_modules[i];
        }
    }
    return NULL;
}

/* Cache module exports */
void cache_module(Interpreter *interp, const char *module_path, Env *exports, Env *module_env, ASTNode **ast_nodes, int ast_count) {
    if (interp->module_count >= interp->module_capacity) {
        interp->module_capacity = interp->module_capacity == 0 ? 4 : interp->module_capacity * 2;
        interp->loaded_modules = realloc(interp->loaded_modules, 
            interp->module_capacity * sizeof(LoadedModule));
    }
    
    interp->loaded_modules[interp->module_count].path = strdup(module_path);
    interp->loaded_modules[interp->module_count].exports = exports;
    interp->loaded_modules[interp->module_count].module_env = module_env;
    interp->loaded_modules[interp->module_count].ast_nodes = ast_nodes;
    interp->loaded_modules[interp->module_count].ast_count = ast_count;
    interp->loaded_modules[interp->module_count].is_loading = 0;
    interp->module_count++;
}

/* Check if module is currently being loaded (for circular dependency detection) */
int is_module_loading(Interpreter *interp, const char *module_path) {
    int i;
    for (i = 0; i < interp->module_count; i++) {
        if (strcmp(interp->loaded_modules[i].path, module_path) == 0) {
            return interp->loaded_modules[i].is_loading;
        }
    }
    return 0;
}

/* Set module loading flag */
void set_module_loading(Interpreter *interp, const char *module_path, int loading) {
    int i;
    for (i = 0; i < interp->module_count; i++) {
        if (strcmp(interp->loaded_modules[i].path, module_path) == 0) {
            interp->loaded_modules[i].is_loading = loading;
            return;
        }
    }
}

/* Load and execute a module */
Value load_module(Interpreter *interp, const char *module_path, Env *env) {
    FILE *file;
    long file_size;
    char *source;
    Lexer lexer;
    Parser parser;
    ASTNode **nodes;
    int count, i;
    Env *module_env, *export_env;
    (void)env; /* Parameter reserved for future use */
    char *old_module_dir;
    char *module_dir;
    Value result = make_null();
    
    /* Check if already loaded */
    LoadedModule *cached = get_cached_module(interp, module_path);
    if (cached != NULL) {
        return make_module(module_path, cached->exports, cached->module_env);
    }
    
    /* Check for circular dependency */
    if (is_module_loading(interp, module_path)) {
        fprintf(stderr, "Error: Circular dependency detected for module '%s'\n", module_path);
        interp->had_error = 1;
        return make_null();
    }
    
    /* Read the module file */
    file = fopen(module_path, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open module file '%s'\n", module_path);
        interp->had_error = 1;
        return make_null();
    }
    
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    source = malloc(file_size + 1);
    fread(source, 1, file_size, file);
    source[file_size] = '\0';
    fclose(file);
    
    /* Parse the module */
    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);
    nodes = parse_program(&parser, &count);
    
    if (parser.had_error) {
        interp->had_error = 1;
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
        free(source);
        return make_null();
    }
    
    /* Create module environment - isolated from global */
    module_env = env_new(NULL);  /* No parent - isolated scope */
    export_env = env_new(NULL);  /* Export namespace */
    
    /* Save current module directory and set new one */
    old_module_dir = interp->current_module_dir;
    module_dir = strdup(module_path);
    {
        char *dir_result = dirname(module_dir);
        interp->current_module_dir = strdup(dir_result);
    }
    free(module_dir);
    
    /* Mark module as loading before executing */
    cache_module(interp, module_path, export_env, module_env, nodes, count);
    set_module_loading(interp, module_path, 1);
    
    /* Execute module statements */
    for (i = 0; i < count; i++) {
        ASTNode *node = nodes[i];
        
        /* Handle exports */
        if (node->type == NODE_EXPORT) {
            if (node->data.export_stmt.is_default) {
                /* export default expr */
                Value val = eval_node_env(interp, node->data.export_stmt.declaration, module_env);
                
                /* For named function/class declarations, they return null but bind to module_env */
                /* We need to retrieve them from the environment */
                if (val.type == VAL_NULL && node->data.export_stmt.declaration) {
                    const char *name = NULL;
                    if (node->data.export_stmt.declaration->type == NODE_FUNC_DEF) {
                        name = node->data.export_stmt.declaration->data.func_def.name;
                    } else if (node->data.export_stmt.declaration->type == NODE_CLASS_DEF) {
                        name = node->data.export_stmt.declaration->data.class_def.name;
                    }
                    
                    if (name && strlen(name) > 0) {
                        Value *func_val = env_get(module_env, name);
                        if (func_val) {
                            val = *func_val;
                        }
                    }
                }
                
                env_set_local(export_env, "default", val);
            } else if (node->data.export_stmt.declaration) {
                /* export const/let/var/fn/class */
                Value val = eval_node_env(interp, node->data.export_stmt.declaration, module_env);
                
                /* Extract the name from the declaration and export it */
                const char *name = NULL;
                if (node->data.export_stmt.declaration->type == NODE_LET) {
                    name = node->data.export_stmt.declaration->data.let_stmt.name;
                } else if (node->data.export_stmt.declaration->type == NODE_FUNC_DEF) {
                    name = node->data.export_stmt.declaration->data.func_def.name;
                } else if (node->data.export_stmt.declaration->type == NODE_CLASS_DEF) {
                    name = node->data.export_stmt.declaration->data.class_def.name;
                }
                
                if (name) {
                    Value *exported_val = env_get(module_env, name);
                    if (exported_val) {
                        env_set_local(export_env, name, *exported_val);
                    } else {
                        fprintf(stderr, "Warning: Export failed - '%s' not found in module environment\n", name);
                    }
                }
                value_free(&val);
            } else if (node->data.export_stmt.names) {
                /* export { name1, name2 } */
                int j;
                for (j = 0; j < node->data.export_stmt.count; j++) {
                    const char *name = node->data.export_stmt.names[j];
                    Value *val = env_get(module_env, name);
                    if (val) {
                        env_set_local(export_env, name, *val);
                    } else {
                        fprintf(stderr, "Warning: Exported name '%s' not found in module\n", name);
                    }
                }
            }
        } else {
            /* Regular statement - execute in module environment */
            Value val = eval_node_env(interp, node, module_env);
            value_free(&val);
        }
        
        if (interp->had_error) break;
    }
    
    /* Mark module as loaded */
    set_module_loading(interp, module_path, 0);
    
    /* Restore old module directory */
    free(interp->current_module_dir);
    interp->current_module_dir = old_module_dir;
    
    /* Cleanup parser resources */
    parser_free(&parser);
    lexer_free(&lexer);
    free(source);
    /* NOTE: We do NOT free module_env or AST nodes here because:
       - exported functions reference module_env through their closures
       - function bodies point to AST nodes
       Both are stored in the module cache (loaded_modules array) and will be 
       freed in interpreter_free() at lines 543-556. */
    
    if (!interp->had_error) {
        result = make_module(module_path, export_env, module_env);
    } else {
        /* On error, clean up everything */
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        env_release(module_env);
        env_release(export_env);
    }
    
    return result;
}
