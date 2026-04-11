#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

static Value eval_node_env(Interpreter *interp, ASTNode *node, Env *env);

// Forward declaration for Promise constructor
extern Value builtin_Promise_constructor(Interpreter *interp, Value *args, int argc);

// Debug flag for tracking object reference counting
#define DEBUG_REFCOUNT 0

#if DEBUG_REFCOUNT
#define REFCOUNT_LOG(fmt, ...) fprintf(stderr, "[REFCOUNT] " fmt "\n", ##__VA_ARGS__)
#else
#define REFCOUNT_LOG(fmt, ...) ((void)0)
#endif

/* Helper function to increment reference count when copying dict/set values */
static void value_retain(Value *v) {
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
    memset(&val.as, 0, sizeof(val.as));
    return val;
}

Value make_class(const char *name, const char *parent_name) {
    Value val;
    val.type = VAL_CLASS;
    val.as.class_val.name = strdup(name);
    val.as.class_val.parent_name = parent_name ? strdup(parent_name) : NULL;
    val.as.class_val.methods = env_new(NULL);
    val.as.class_val.fields = env_new(NULL);
    val.as.class_val.static_methods = env_new(NULL);
    val.as.class_val.static_fields = env_new(NULL);
    val.as.class_val.is_abstract = 0;
    return val;
}

Value make_object(const char *class_name, Env *methods) {
    Value val;
    val.type = VAL_OBJECT;
    val.as.object_val = malloc(sizeof(ObjectVal));
    val.as.object_val->class_name = strdup(class_name);
    val.as.object_val->fields = env_new(NULL);
    val.as.object_val->methods = methods;
    val.as.object_val->ref_count = 1;  // Initialize reference count
    REFCOUNT_LOG("make_object(%s) -> ptr=%p ref_count=1", class_name, (void*)val.as.object_val);
    return val;
}

Value make_method(Value receiver, Value method) {
    Value val;
    val.type = VAL_METHOD;
    val.as.method_val.receiver = malloc(sizeof(Value));
    val.as.method_val.method = malloc(sizeof(Value));
    *val.as.method_val.receiver = receiver;
    
    // Increment ref_count for object receiver to track shared ownership
    if (receiver.type == VAL_OBJECT && receiver.as.object_val) {
        receiver.as.object_val->ref_count++;
        REFCOUNT_LOG("make_method: ptr=%p ref_count=%d (after increment)", 
                     (void*)receiver.as.object_val, receiver.as.object_val->ref_count);
    }
    
    // Deep copy function if needed
    if (method.type == VAL_FUNCTION && method.as.func_val.param_names) {
        Value method_copy = method;
        int i;
        method_copy.as.func_val.param_names = malloc(method.as.func_val.param_count * sizeof(char *));
        for (i = 0; i < method.as.func_val.param_count; i++)
            method_copy.as.func_val.param_names[i] = strdup(method.as.func_val.param_names[i]);
        *val.as.method_val.method = method_copy;
    } else {
        *val.as.method_val.method = method;
    }
    
    return val;
}

Value make_promise(void) {
    Value val;
    val.type = VAL_PROMISE;
    val.as.promise_val.state = PROMISE_PENDING;
    val.as.promise_val.result = malloc(sizeof(Value));
    *val.as.promise_val.result = make_null();
    val.as.promise_val.callbacks = NULL;
    return val;
}

Value make_module(const char *module_path, Env *exports, Env *module_env) {
    Value val;
    val.type = VAL_MODULE;
    val.as.module_val.module_path = module_path ? strdup(module_path) : NULL;
    val.as.module_val.exports = exports;
    val.as.module_val.module_env = module_env;  // Keep module_env alive for closures
    return val;
}

Value make_dict(void) {
    Value val;
    DictVal *dict = malloc(sizeof(DictVal));
    val.type = VAL_DICT;
    dict->count = 0;
    dict->capacity = 8;
    dict->keys = malloc(8 * sizeof(Value));
    dict->values = malloc(8 * sizeof(Value));
    dict->ref_count = 1;  // Initialize reference count
    val.as.dict_val = dict;
    return val;
}

Value make_set(void) {
    Value val;
    SetVal *set = malloc(sizeof(SetVal));
    val.type = VAL_SET;
    set->count = 0;
    set->capacity = 8;
    set->items = malloc(8 * sizeof(Value));
    set->ref_count = 1;  // Initialize reference count
    val.as.set_val = set;
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
    StructVal *sv = malloc(sizeof(StructVal));
    val.type = VAL_STRUCT;
    sv->struct_name = struct_name ? strdup(struct_name) : NULL;
    sv->fields = env_new(NULL);
    sv->is_union = is_union;
    sv->ref_count = 1;
    val.as.struct_val = sv;
    return val;
}

Value make_generator(FunctionVal *func, Env *env) {
    Value v;
    v.type = VAL_GENERATOR;
    v.as.generator_val.state = GEN_SUSPENDED;
    v.as.generator_val.func = func;
    v.as.generator_val.saved_env = env;
    v.as.generator_val.yield_index = 0;
    v.as.generator_val.last_value = malloc(sizeof(Value));
    *v.as.generator_val.last_value = make_null();
    return v;
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
    // Only release global_env if it hasn't been freed already
    if (interp->global_env && interp->global_env->ref_count > 0) {
        env_release(interp->global_env);
    }
    free(interp);
}

Value eval_block(Interpreter *interp, ASTNode *block, Env *env) {
    Value result = make_null();
    int i;
    for (i = 0; i < block->data.block.stmts.count; i++) {
        value_free(&result);
        result = eval_node_env(interp, block->data.block.stmts.items[i], env);
        if (interp->last_result.is_return || interp->last_result.is_break || interp->last_result.is_continue)
            break;
    }
    return result;
}

static Value eval_node_env(Interpreter *interp, ASTNode *node, Env *env) {
    if (!node) return make_null();

    switch (node->type) {
        case NODE_NUMBER: {
            double v = node->data.num_val;
            if (fmod(v, 1.0) == 0.0 && v >= -9.2e18 && v <= 9.2e18)
                return make_int((long long)v);
            return make_float(v);
        }
        case NODE_STRING:
            return make_string(node->data.str_val);
        case NODE_BOOL:
            return make_bool(node->data.bool_val);
        case NODE_NULL:
            return make_null();
        case NODE_IDENT: {
            Value *v = env_get(env, node->data.ident.name);
            if (!v) {
                fprintf(stderr, "Error at line %d: undefined variable '%s'\n", node->line, node->data.ident.name);
                interp->had_error = 1;
                return make_null();
            }
            if (v->type == VAL_STRING) return make_string(v->as.str_val);
            if (v->type == VAL_LIST) {
                /* Deep copy list to avoid double-free issues */
                Value copy;
                int ii;
                copy.type = VAL_LIST;
                copy.as.list_val.count = v->as.list_val.count;
                copy.as.list_val.capacity = v->as.list_val.capacity;
                copy.as.list_val.items = malloc(v->as.list_val.capacity * sizeof(Value));
                for (ii = 0; ii < v->as.list_val.count; ii++) {
                    /* Recursively copy each item */
                    if (v->as.list_val.items[ii].type == VAL_STRING) {
                        copy.as.list_val.items[ii] = make_string(v->as.list_val.items[ii].as.str_val);
                    } else if (v->as.list_val.items[ii].type == VAL_LIST) {
                        /* Deep copy nested arrays */
                        Value nested_copy;
                        int jj;
                        nested_copy.type = VAL_LIST;
                        nested_copy.as.list_val.count = v->as.list_val.items[ii].as.list_val.count;
                        nested_copy.as.list_val.capacity = v->as.list_val.items[ii].as.list_val.count;
                        nested_copy.as.list_val.items = malloc((nested_copy.as.list_val.count > 0 ? nested_copy.as.list_val.count : 1) * sizeof(Value));
                        for (jj = 0; jj < nested_copy.as.list_val.count; jj++) {
                            if (v->as.list_val.items[ii].as.list_val.items[jj].type == VAL_STRING) {
                                nested_copy.as.list_val.items[jj] = make_string(v->as.list_val.items[ii].as.list_val.items[jj].as.str_val);
                            } else {
                                nested_copy.as.list_val.items[jj] = v->as.list_val.items[ii].as.list_val.items[jj];
                                value_retain(&nested_copy.as.list_val.items[jj]);
                            }
                        }
                        copy.as.list_val.items[ii] = nested_copy;
                    } else {
                        copy.as.list_val.items[ii] = v->as.list_val.items[ii];
                        /* Retain dict/set values when copying arrays */
                        value_retain(&copy.as.list_val.items[ii]);
                    }
                }
                return copy;
            }
            if (v->type == VAL_TUPLE) {
                /* Deep copy tuple to avoid double-free issues */
                Value copy;
                int ii;
                copy.type = VAL_TUPLE;
                copy.as.tuple_val.count = v->as.tuple_val.count;
                copy.as.tuple_val.elements = malloc((v->as.tuple_val.count > 0 ? v->as.tuple_val.count : 1) * sizeof(Value));
                for (ii = 0; ii < v->as.tuple_val.count; ii++) {
                    /* Recursively copy each item */
                    if (v->as.tuple_val.elements[ii].type == VAL_STRING) {
                        copy.as.tuple_val.elements[ii] = make_string(v->as.tuple_val.elements[ii].as.str_val);
                    } else {
                        copy.as.tuple_val.elements[ii] = v->as.tuple_val.elements[ii];
                        /* Retain dict/set/struct values when copying tuples */
                        value_retain(&copy.as.tuple_val.elements[ii]);
                    }
                }
                return copy;
            }
            /* Dicts and sets are mutable reference types like objects - don't deep copy */
            /* They need to be modified in-place by methods */
            /* Increment reference count when returning */
            if (v->type == VAL_DICT && v->as.dict_val) {
                v->as.dict_val->ref_count++;
            }
            if (v->type == VAL_SET && v->as.set_val) {
                v->as.set_val->ref_count++;
            }
            if (v->type == VAL_OBJECT && v->as.object_val) {
                v->as.object_val->ref_count++;
                REFCOUNT_LOG("NODE_IDENT: ptr=%p ref_count=%d (after increment)", 
                             (void*)v->as.object_val, v->as.object_val->ref_count);
            }
            if (v->type == VAL_STRUCT && v->as.struct_val) {
                v->as.struct_val->ref_count++;
            }
            if (v->type == VAL_FUNCTION) {
                Value copy = *v;
                /* Deep copy parameter names if they exist */
                if (v->as.func_val.param_names) {
                    int ii;
                    copy.as.func_val.param_names = malloc(v->as.func_val.param_count * sizeof(char *));
                    for (ii = 0; ii < v->as.func_val.param_count; ii++)
                        copy.as.func_val.param_names[ii] = strdup(v->as.func_val.param_names[ii]);
                }
                /* Retain the closure when copying a function */
                if (copy.as.func_val.closure) {
                    env_retain(copy.as.func_val.closure);
                }
                return copy;
            }
            return *v;
        }
        case NODE_LET: {
            Value val = make_null();
            /* Evaluate initial value if provided */
            if (node->data.let_stmt.value) {
                val = eval_node_env(interp, node->data.let_stmt.value, env);
            }
            /* Use env_declare to enforce const/let/var semantics */
            env_declare(env, node->data.let_stmt.name, val, node->data.let_stmt.decl_type, node->line, interp);
            return make_null();
        }
        case NODE_DESTRUCTURE_ARRAY: {
            Value source = eval_node_env(interp, node->data.destructure_array.source, env);
            DeclType decl_type = node->data.destructure_array.decl_type;
            
            if (source.type != VAL_LIST && source.type != VAL_TUPLE) {
                fprintf(stderr, "Runtime error at line %d: cannot destructure non-array/tuple\n", node->line);
                value_free(&source);
                return make_null();
            }
            
            int source_count = (source.type == VAL_LIST) ? source.as.list_val.count : source.as.tuple_val.count;
            Value *source_items = (source.type == VAL_LIST) ? source.as.list_val.items : source.as.tuple_val.elements;
            
            int i;
            int source_idx = 0;
            for (i = 0; i < node->data.destructure_array.elements.count; i++) {
                ASTNode *elem = node->data.destructure_array.elements.items[i];
                
                if (elem->data.destructure_element.is_hole) {
                    source_idx++;
                    continue;
                }
                
                if (elem->data.destructure_element.is_rest) {
                    /* Collect remaining elements into array */
                    Value rest_array;
                    int rest_count = source_count - source_idx;
                    if (rest_count < 0) rest_count = 0;
                    rest_array.type = VAL_LIST;
                    rest_array.as.list_val.count = rest_count;
                    rest_array.as.list_val.capacity = rest_count;
                    rest_array.as.list_val.items = malloc((rest_count > 0 ? rest_count : 1) * sizeof(Value));
                    int rest_idx = 0;
                    while (source_idx < source_count) {
                        rest_array.as.list_val.items[rest_idx++] = source_items[source_idx];
                        source_idx++;
                    }
                    env_declare(env, elem->data.destructure_element.name, rest_array, decl_type, node->line, interp);
                    break;
                }
                
                Value val = make_null();
                if (source_idx < source_count) {
                    val = source_items[source_idx];
                } else if (elem->data.destructure_element.default_value) {
                    val = eval_node_env(interp, elem->data.destructure_element.default_value, env);
                }
                
                if (elem->data.destructure_element.nested) {
                    /* Handle nested destructuring */
                    elem->data.destructure_element.nested->data.destructure_array.source = ast_new_list(node->line);
                    if (val.type == VAL_LIST) {
                        /* Copy list items to nested source */
                        int j;
                        for (j = 0; j < (int)val.as.list_val.count; j++) {
                            nodelist_push(&elem->data.destructure_element.nested->data.destructure_array.source->data.list.elements, 
                                         ast_new_number(0, node->line));
                        }
                    }
                    eval_node_env(interp, elem->data.destructure_element.nested, env);
                } else if (elem->data.destructure_element.name) {
                    env_declare(env, elem->data.destructure_element.name, val, decl_type, node->line, interp);
                }
                
                source_idx++;
            }
            
            value_free(&source);
            return make_null();
        }
        case NODE_DESTRUCTURE_OBJECT: {
            Value source = eval_node_env(interp, node->data.destructure_object.source, env);
            DeclType decl_type = node->data.destructure_object.decl_type;
            
            if (source.type != VAL_OBJECT) {
                fprintf(stderr, "Runtime error at line %d: cannot destructure non-object\n", node->line);
                value_free(&source);
                return make_null();
            }
            
            int i;
            for (i = 0; i < node->data.destructure_object.properties.count; i++) {
                ASTNode *elem = node->data.destructure_object.properties.items[i];
                
                if (elem->data.destructure_element.is_rest) {
                    /* Rest properties - create new object with all fields */
                    Value rest_obj = make_object("Object", NULL);
                    env_declare(env, elem->data.destructure_element.name, rest_obj, decl_type, node->line, interp);
                    continue;
                }
                
                const char *key = elem->data.destructure_element.key ? 
                                 elem->data.destructure_element.key : 
                                 elem->data.destructure_element.name;
                
                Value val = make_null();
                /* Look up value from object fields */
                Value *found = env_get(source.as.object_val->fields, key);
                if (found && found->type != VAL_NULL) {
                    val = *found;
                } else if (elem->data.destructure_element.default_value) {
                    val = eval_node_env(interp, elem->data.destructure_element.default_value, env);
                }
                
                if (elem->data.destructure_element.nested) {
                    /* Handle nested object destructuring */
                    eval_node_env(interp, elem->data.destructure_element.nested, env);
                } else if (elem->data.destructure_element.name) {
                    env_declare(env, elem->data.destructure_element.name, val, decl_type, node->line, interp);
                }
            }
            
            value_free(&source);
            return make_null();
        }
        case NODE_ASSIGN: {
            Value val = eval_node_env(interp, node->data.assign_stmt.value, env);
            env_set(env, node->data.assign_stmt.name, val);
            return make_null();
        }
        case NODE_BLOCK: {
            Env *block_env = env_new(env);
            Value result = eval_block(interp, node, block_env);
            env_release(block_env);
            return result;
        }
        case NODE_FUNC_DEF: {
            Value func;
            func.type = VAL_FUNCTION;
            func.as.func_val.param_count = node->data.func_def.params.count;
            func.as.func_val.param_names = NULL;
            func.as.func_val.default_values = node->data.func_def.default_values; /* Point to AST-owned array */
            if (func.as.func_val.param_count > 0) {
                int i;
                func.as.func_val.param_names = malloc(func.as.func_val.param_count * sizeof(char *));
                for (i = 0; i < func.as.func_val.param_count; i++)
                    func.as.func_val.param_names[i] = strdup(node->data.func_def.params.items[i]->data.ident.name);
            }
            func.as.func_val.body = node->data.func_def.body;
            func.as.func_val.closure = env;
            env_retain(env);  /* Retain closure environment for nested functions */
            func.as.func_val.is_async = node->data.func_def.is_async;
            func.as.func_val.is_generator = node->data.func_def.is_generator;
            func.as.func_val.has_rest_param = node->data.func_def.has_rest_param;
            
            // Arrow functions are expressions that return the function value
            // Named functions are statements that bind to a name
            if (node->data.func_def.is_arrow || strlen(node->data.func_def.name) == 0) {
                return func;
            } else {
                env_set_local(env, node->data.func_def.name, func);
                value_free(&func);  /* Free the local copy after storing */
                return make_null();
            }
        }
        case NODE_CALL: {
            // Special handling for new expressions: new Point(args)
            if (node->data.call.callee->type == NODE_NEW) {
                ASTNode *new_node = node->data.call.callee;
                char *class_name = new_node->data.new_expr.class_name;
                
                // Special handling for Promise constructor
                if (strcmp(class_name, "Promise") == 0) {
                    // Evaluate executor argument
                    if (node->data.call.args.count < 1) {
                        fprintf(stderr, "Error at line %d: Promise constructor requires an executor function\n", node->line);
                        interp->had_error = 1;
                        return make_null();
                    }
                    
                    Value executor = eval_node_env(interp, node->data.call.args.items[0], env);
                    Value args[1] = { executor };
                    Value promise = builtin_Promise_constructor(interp, args, 1);
                    value_free(&executor);
                    return promise;
                }
                
                // Get the class
                Value *class_val = env_get(env, class_name);
                if (!class_val || class_val->type != VAL_CLASS) {
                    fprintf(stderr, "Error at line %d: '%s' is not a class\n", 
                        node->line, class_name);
                    interp->had_error = 1;
                    return make_null();
                }
                
                // Create object
                Value obj = make_object(class_name, class_val->as.class_val.methods);
                
                // Initialize fields with default values from class (deep copy strings)
                EnvEntry *field_entry = class_val->as.class_val.fields->entries;
                while (field_entry) {
                    Value field_val = field_entry->value;
                    if (field_val.type == VAL_STRING) {
                        field_val.as.str_val = strdup(field_entry->value.as.str_val);
                    }
                    env_set_local(obj.as.object_val->fields, field_entry->name, field_val);
                    field_entry = field_entry->next;
                }
                
                // Call init method if it exists
                Value *init_method = env_get(class_val->as.class_val.methods, "init");
                if (init_method && init_method->type == VAL_FUNCTION) {
                    int argc = node->data.call.args.count;
                    Value *args = malloc((argc > 0 ? argc : 1) * sizeof(Value));
                    int i;
                    for (i = 0; i < argc; i++)
                        args[i] = eval_node_env(interp, node->data.call.args.items[i], env);
                    
                    // Create call environment with 'this' bound
                    Env *call_env = env_new(init_method->as.func_val.closure);
                    env_set_local(call_env, "this", obj);
                    
                    /* Handle rest parameters */
                    if (init_method->as.func_val.has_rest_param && init_method->as.func_val.param_count > 0) {
                        /* Bind regular params */
                        for (i = 0; i < init_method->as.func_val.param_count - 1 && i < argc; i++)
                            env_set_local(call_env, init_method->as.func_val.param_names[i], args[i]);
                        
                        /* Collect rest args into array */
                        Value rest_array;
                        int rest_count = argc - (init_method->as.func_val.param_count - 1);
                        if (rest_count < 0) rest_count = 0;
                        rest_array.type = VAL_LIST;
                        rest_array.as.list_val.count = rest_count;
                        rest_array.as.list_val.capacity = rest_count;
                        rest_array.as.list_val.items = malloc((rest_count > 0 ? rest_count : 1) * sizeof(Value));
                        for (i = 0; i < rest_count; i++) {
                            int arg_idx = init_method->as.func_val.param_count - 1 + i;
                            rest_array.as.list_val.items[i] = args[arg_idx];
                        }
                        env_set_local(call_env, init_method->as.func_val.param_names[init_method->as.func_val.param_count - 1], rest_array);
                    } else {
                        for (i = 0; i < init_method->as.func_val.param_count && i < argc; i++)
                            env_set_local(call_env, init_method->as.func_val.param_names[i], args[i]);
                    }
                    
                    Value init_result = eval_block(interp, init_method->as.func_val.body, call_env);
                    value_free(&init_result);
                    
                    // Get updated 'this' from call_env (in case fields were modified)
                    Value *updated_this = env_get(call_env, "this");
                    if (updated_this && updated_this->type == VAL_OBJECT) {
                        // Fields are shared, so updates are automatic
                    }
                    
                    env_release(call_env);
                    for (i = 0; i < argc; i++) value_free(&args[i]);
                    free(args);
                    
                    if (interp->last_result.is_return) {
                        value_free(&interp->last_result.return_value);
                        interp->last_result.is_return = 0;
                    }
                }
                
                return obj;
            }
            
            Value callee = eval_node_env(interp, node->data.call.callee, env);
            int argc = node->data.call.args.count;
            Value *args = malloc((argc > 0 ? argc : 1) * sizeof(Value));
            Value result = make_null();
            int i;
            for (i = 0; i < argc; i++) {
                args[i] = eval_node_env(interp, node->data.call.args.items[i], env);
                /* Increment ref count for dict/set values passed as arguments */
                value_retain(&args[i]);
            }

            if (callee.type == VAL_BUILTIN) {
                result = callee.as.builtin(interp, args, argc);
            } else if (callee.type == VAL_METHOD) {
                // Handle bound method call
                Value method = *callee.as.method_val.method;
                Value receiver = *callee.as.method_val.receiver;
                
                if (method.type == VAL_BUILTIN) {
                    // Builtin method (e.g., array methods)
                    // Prepend receiver to args
                    Value *method_args = malloc((argc + 1) * sizeof(Value));
                    method_args[0] = receiver;
                    for (i = 0; i < argc; i++) {
                        method_args[i + 1] = args[i];
                    }
                    value_free(&result);
                    result = method.as.builtin(interp, method_args, argc + 1);
                    free(method_args);
                } else if (method.type == VAL_FUNCTION) {
                    Env *call_env = env_new(method.as.func_val.closure);
                    // Bind 'this' to the receiver object
                    env_set_local(call_env, "this", receiver);
                    for (i = 0; i < method.as.func_val.param_count && i < argc; i++)
                        env_set_local(call_env, method.as.func_val.param_names[i], args[i]);
                    value_free(&result);
                    result = eval_block(interp, method.as.func_val.body, call_env);
                    
                    // Update receiver fields if they were modified
                    if (receiver.type == VAL_OBJECT) {
                        Value *updated_this = env_get(call_env, "this");
                        if (updated_this && updated_this->type == VAL_OBJECT) {
                            // The receiver in the callee is already updated through shared env
                        }
                    }
                    
                    env_release(call_env);
                    if (interp->last_result.is_return) {
                        value_free(&result);
                        result = interp->last_result.return_value;
                        interp->last_result.is_return = 0;
                        interp->last_result.return_value = make_null();
                    }
                }
            } else if (callee.type == VAL_FUNCTION) {
                /* Check if function is a generator */
                if (callee.as.func_val.is_generator) {
                    /* Generator functions return a generator object when called */
                    FunctionVal *func_copy = malloc(sizeof(FunctionVal));
                    func_copy->param_count = callee.as.func_val.param_count;
                    func_copy->param_names = malloc(func_copy->param_count * sizeof(char *));
                    for (i = 0; i < func_copy->param_count; i++) {
                        func_copy->param_names[i] = strdup(callee.as.func_val.param_names[i]);
                    }
                    /* Copy default values pointer (owned by AST) */
                    func_copy->default_values = callee.as.func_val.default_values;
                    func_copy->body = callee.as.func_val.body;
                    func_copy->closure = callee.as.func_val.closure;
                    env_retain(func_copy->closure);  /* Retain closure for generator */
                    func_copy->is_async = callee.as.func_val.is_async;
                    func_copy->is_generator = 1;
                    func_copy->has_rest_param = callee.as.func_val.has_rest_param;
                    
                    /* Create generator environment with bound parameters */
                    Env *gen_env = env_new(callee.as.func_val.closure);
                    
                    /* Handle rest parameters */
                    if (callee.as.func_val.has_rest_param && callee.as.func_val.param_count > 0) {
                        /* Bind regular params */
                        for (i = 0; i < callee.as.func_val.param_count - 1 && i < argc; i++)
                            env_set_local(gen_env, callee.as.func_val.param_names[i], args[i]);
                        
                        /* Handle missing args with default values */
                        for (i = argc; i < callee.as.func_val.param_count - 1; i++) {
                            if (callee.as.func_val.default_values && callee.as.func_val.default_values[i]) {
                                Value default_val = eval_node_env(interp, callee.as.func_val.default_values[i], gen_env);
                                env_set_local(gen_env, callee.as.func_val.param_names[i], default_val);
                            }
                        }
                        
                        /* Collect rest args into array */
                        Value rest_array;
                        int rest_count = argc - (callee.as.func_val.param_count - 1);
                        if (rest_count < 0) rest_count = 0;
                        rest_array.type = VAL_LIST;
                        rest_array.as.list_val.count = rest_count;
                        rest_array.as.list_val.capacity = rest_count;
                        rest_array.as.list_val.items = malloc((rest_count > 0 ? rest_count : 1) * sizeof(Value));
                        for (i = 0; i < rest_count; i++) {
                            int arg_idx = callee.as.func_val.param_count - 1 + i;
                            rest_array.as.list_val.items[i] = args[arg_idx];
                        }
                        env_set_local(gen_env, callee.as.func_val.param_names[callee.as.func_val.param_count - 1], rest_array);
                    } else {
                        /* Bind provided arguments */
                        for (i = 0; i < callee.as.func_val.param_count && i < argc; i++)
                            env_set_local(gen_env, callee.as.func_val.param_names[i], args[i]);
                        
                        /* Handle missing args with default values */
                        for (i = argc; i < callee.as.func_val.param_count; i++) {
                            if (callee.as.func_val.default_values && callee.as.func_val.default_values[i]) {
                                Value default_val = eval_node_env(interp, callee.as.func_val.default_values[i], gen_env);
                                env_set_local(gen_env, callee.as.func_val.param_names[i], default_val);
                            }
                        }
                    }
                    
                    value_free(&result);
                    result = make_generator(func_copy, gen_env);
                }
                /* Check if function is async */
                else if (callee.as.func_val.is_async) {
                    /* Async functions always return a Promise */
                    /* Create a Promise and execute function body */
                    Value promise_result;
                    Env *call_env = env_new(callee.as.func_val.closure);
                    Value body_result;
                    
                    /* Bind parameters */
                    if (callee.as.func_val.has_rest_param && callee.as.func_val.param_count > 0) {
                        /* Bind regular params */
                        for (i = 0; i < callee.as.func_val.param_count - 1 && i < argc; i++)
                            env_set_local(call_env, callee.as.func_val.param_names[i], args[i]);
                        
                        /* Handle missing args with default values */
                        for (i = argc; i < callee.as.func_val.param_count - 1; i++) {
                            if (callee.as.func_val.default_values && callee.as.func_val.default_values[i]) {
                                Value default_val = eval_node_env(interp, callee.as.func_val.default_values[i], call_env);
                                env_set_local(call_env, callee.as.func_val.param_names[i], default_val);
                            }
                        }
                        
                        /* Collect rest args into array */
                        Value rest_array;
                        int rest_count = argc - (callee.as.func_val.param_count - 1);
                        if (rest_count < 0) rest_count = 0;
                        rest_array.type = VAL_LIST;
                        rest_array.as.list_val.count = rest_count;
                        rest_array.as.list_val.capacity = rest_count;
                        rest_array.as.list_val.items = malloc((rest_count > 0 ? rest_count : 1) * sizeof(Value));
                        for (i = 0; i < rest_count; i++) {
                            int arg_idx = callee.as.func_val.param_count - 1 + i;
                            rest_array.as.list_val.items[i] = args[arg_idx];
                        }
                        env_set_local(call_env, callee.as.func_val.param_names[callee.as.func_val.param_count - 1], rest_array);
                    } else {
                        /* Bind provided arguments */
                        for (i = 0; i < callee.as.func_val.param_count && i < argc; i++)
                            env_set_local(call_env, callee.as.func_val.param_names[i], args[i]);
                        
                        /* Handle missing args with default values */
                        for (i = argc; i < callee.as.func_val.param_count; i++) {
                            if (callee.as.func_val.default_values && callee.as.func_val.default_values[i]) {
                                Value default_val = eval_node_env(interp, callee.as.func_val.default_values[i], call_env);
                                env_set_local(call_env, callee.as.func_val.param_names[i], default_val);
                            }
                        }
                    }
                    
                    /* Execute function body */
                    value_free(&result);
                    body_result = eval_block(interp, callee.as.func_val.body, call_env);
                    env_release(call_env);
                    
                    /* Handle return value */
                    if (interp->last_result.is_return) {
                        value_free(&body_result);
                        body_result = interp->last_result.return_value;
                        interp->last_result.is_return = 0;
                        interp->last_result.return_value = make_null();
                    }
                    
                    /* Wrap result in Promise.resolve() */
                    promise_result.type = VAL_PROMISE;
                    promise_result.as.promise_val.state = PROMISE_FULFILLED;
                    promise_result.as.promise_val.result = malloc(sizeof(Value));
                    *promise_result.as.promise_val.result = body_result;
                    promise_result.as.promise_val.callbacks = NULL;
                    
                    result = promise_result;
                } else {
                    /* Regular synchronous function */
                    Env *call_env = env_new(callee.as.func_val.closure);
                    
                    /* Handle rest parameters */
                    if (callee.as.func_val.has_rest_param && callee.as.func_val.param_count > 0) {
                        /* Bind regular params */
                        for (i = 0; i < callee.as.func_val.param_count - 1 && i < argc; i++)
                            env_set_local(call_env, callee.as.func_val.param_names[i], args[i]);
                        
                        /* Handle missing args with default values */
                        for (i = argc; i < callee.as.func_val.param_count - 1; i++) {
                            if (callee.as.func_val.default_values && callee.as.func_val.default_values[i]) {
                                Value default_val = eval_node_env(interp, callee.as.func_val.default_values[i], call_env);
                                env_set_local(call_env, callee.as.func_val.param_names[i], default_val);
                            }
                        }
                        
                        /* Collect rest args into array */
                        Value rest_array;
                        int rest_count = argc - (callee.as.func_val.param_count - 1);
                        if (rest_count < 0) rest_count = 0;
                        rest_array.type = VAL_LIST;
                        rest_array.as.list_val.count = rest_count;
                        rest_array.as.list_val.capacity = rest_count;
                        rest_array.as.list_val.items = malloc((rest_count > 0 ? rest_count : 1) * sizeof(Value));
                        for (i = 0; i < rest_count; i++) {
                            int arg_idx = callee.as.func_val.param_count - 1 + i;
                            rest_array.as.list_val.items[i] = args[arg_idx];
                        }
                        env_set_local(call_env, callee.as.func_val.param_names[callee.as.func_val.param_count - 1], rest_array);
                    } else {
                        /* Bind provided arguments */
                        for (i = 0; i < callee.as.func_val.param_count && i < argc; i++)
                            env_set_local(call_env, callee.as.func_val.param_names[i], args[i]);
                        
                        /* Handle missing args with default values */
                        for (i = argc; i < callee.as.func_val.param_count; i++) {
                            if (callee.as.func_val.default_values && callee.as.func_val.default_values[i]) {
                                Value default_val = eval_node_env(interp, callee.as.func_val.default_values[i], call_env);
                                env_set_local(call_env, callee.as.func_val.param_names[i], default_val);
                            }
                        }
                    }
                    
                    value_free(&result);
                    result = eval_block(interp, callee.as.func_val.body, call_env);
                    env_release(call_env);
                    if (interp->last_result.is_return) {
                        value_free(&result);
                        result = interp->last_result.return_value;
                        interp->last_result.is_return = 0;
                        interp->last_result.return_value = make_null();
                    }
                }
            } else {
                fprintf(stderr, "Error: not a function\n");
                interp->had_error = 1;
            }

            for (i = 0; i < argc; i++) value_free(&args[i]);
            free(args);
            value_free(&callee);
            return result;
        }
        case NODE_RETURN: {
            Value val = node->data.return_stmt.value ?
                eval_node_env(interp, node->data.return_stmt.value, env) : make_null();
            interp->last_result.is_return = 1;
            interp->last_result.return_value = val;
            return make_null();
        }
        case NODE_BREAK: {
            interp->last_result.is_break = 1;
            return make_null();
        }
        case NODE_CONTINUE: {
            interp->last_result.is_continue = 1;
            return make_null();
        }
        case NODE_IF: {
            Value cond = eval_node_env(interp, node->data.if_stmt.cond, env);
            int truthy = 0;
            if (cond.type == VAL_BOOL) truthy = cond.as.bool_val;
            else if (cond.type == VAL_INT) truthy = cond.as.int_val != 0;
            else if (cond.type == VAL_FLOAT) truthy = cond.as.float_val != 0.0;
            else if (cond.type == VAL_STRING) truthy = cond.as.str_val && cond.as.str_val[0] != '\0';
            else if (cond.type == VAL_NULL) truthy = 0;
            else truthy = 1;
            value_free(&cond);
            if (truthy)
                return eval_node_env(interp, node->data.if_stmt.then_block, env);
            else if (node->data.if_stmt.else_block)
                return eval_node_env(interp, node->data.if_stmt.else_block, env);
            return make_null();
        }
        case NODE_WHILE: {
            while (1) {
                Value cond = eval_node_env(interp, node->data.while_stmt.cond, env);
                int truthy = 0;
                if (cond.type == VAL_BOOL) truthy = cond.as.bool_val;
                else if (cond.type == VAL_INT) truthy = cond.as.int_val != 0;
                else if (cond.type == VAL_FLOAT) truthy = cond.as.float_val != 0.0;
                value_free(&cond);
                if (!truthy) break;
                {
                    Value r = eval_node_env(interp, node->data.while_stmt.body, env);
                    value_free(&r);
                }
                if (interp->last_result.is_return || interp->last_result.is_break) {
                    interp->last_result.is_break = 0;
                    break;
                }
                if (interp->last_result.is_continue) {
                    interp->last_result.is_continue = 0;
                    continue;
                }
            }
            return make_null();
        }
        case NODE_FOR: {
            Value start_v = eval_node_env(interp, node->data.for_stmt.start, env);
            Value end_v = eval_node_env(interp, node->data.for_stmt.end, env);
            long long start = start_v.type == VAL_INT ? start_v.as.int_val : (long long)start_v.as.float_val;
            long long end = end_v.type == VAL_INT ? end_v.as.int_val : (long long)end_v.as.float_val;
            long long i;
            value_free(&start_v);
            value_free(&end_v);
            for (i = start; i < end; i++) {
                Env *loop_env = env_new(env);
                env_set_local(loop_env, node->data.for_stmt.var, make_int(i));
                {
                    Value r = eval_block(interp, node->data.for_stmt.body, loop_env);
                    value_free(&r);
                }
                env_release(loop_env);
                if (interp->last_result.is_return || interp->last_result.is_break) {
                    interp->last_result.is_break = 0;
                    break;
                }
                if (interp->last_result.is_continue) {
                    interp->last_result.is_continue = 0;
                    continue;
                }
            }
            return make_null();
        }
        case NODE_FOR_OF: {
            // Evaluate the iterable expression
            Value iterable = eval_node_env(interp, node->data.for_of_stmt.iterable, env);
            /* Increment ref count for dict/set iterables */
            value_retain(&iterable);
            int i;
            
            if (iterable.type == VAL_LIST) {
                // Iterate over list/array elements
                for (i = 0; i < iterable.as.list_val.count; i++) {
                    Env *loop_env = env_new(env);
                    Value elem = iterable.as.list_val.items[i];
                    // Copy strings to avoid double-free
                    if (elem.type == VAL_STRING) {
                        elem = make_string(elem.as.str_val);
                    } else if (elem.type == VAL_LIST) {
                        /* Deep copy nested arrays to avoid double-free */
                        Value list_copy;
                        int j;
                        list_copy.type = VAL_LIST;
                        list_copy.as.list_val.count = elem.as.list_val.count;
                        list_copy.as.list_val.capacity = elem.as.list_val.count;
                        list_copy.as.list_val.items = malloc((elem.as.list_val.count > 0 ? elem.as.list_val.count : 1) * sizeof(Value));
                        for (j = 0; j < elem.as.list_val.count; j++) {
                            if (elem.as.list_val.items[j].type == VAL_STRING) {
                                list_copy.as.list_val.items[j] = make_string(elem.as.list_val.items[j].as.str_val);
                            } else {
                                list_copy.as.list_val.items[j] = elem.as.list_val.items[j];
                                value_retain(&list_copy.as.list_val.items[j]);
                            }
                        }
                        elem = list_copy;
                    } else {
                        /* Increment ref count for dict/set elements */
                        value_retain(&elem);
                    }
                    env_set_local(loop_env, node->data.for_of_stmt.var, elem);
                    {
                        Value r = eval_block(interp, node->data.for_of_stmt.body, loop_env);
                        value_free(&r);
                    }
                    env_release(loop_env);
                    
                    if (interp->last_result.is_return || interp->last_result.is_break) {
                        interp->last_result.is_break = 0;
                        break;
                    }
                    if (interp->last_result.is_continue) {
                        interp->last_result.is_continue = 0;
                        continue;
                    }
                }
            } else if (iterable.type == VAL_TUPLE) {
                // Iterate over tuple elements
                for (i = 0; i < iterable.as.tuple_val.count; i++) {
                    Env *loop_env = env_new(env);
                    Value elem = iterable.as.tuple_val.elements[i];
                    // Copy strings to avoid double-free
                    if (elem.type == VAL_STRING) {
                        elem = make_string(elem.as.str_val);
                    } else if (elem.type == VAL_LIST) {
                        /* Deep copy nested arrays to avoid double-free */
                        Value list_copy;
                        int j;
                        list_copy.type = VAL_LIST;
                        list_copy.as.list_val.count = elem.as.list_val.count;
                        list_copy.as.list_val.capacity = elem.as.list_val.count;
                        list_copy.as.list_val.items = malloc((elem.as.list_val.count > 0 ? elem.as.list_val.count : 1) * sizeof(Value));
                        for (j = 0; j < elem.as.list_val.count; j++) {
                            if (elem.as.list_val.items[j].type == VAL_STRING) {
                                list_copy.as.list_val.items[j] = make_string(elem.as.list_val.items[j].as.str_val);
                            } else {
                                list_copy.as.list_val.items[j] = elem.as.list_val.items[j];
                                value_retain(&list_copy.as.list_val.items[j]);
                            }
                        }
                        elem = list_copy;
                    } else {
                        /* Increment ref count for dict/set elements */
                        value_retain(&elem);
                    }
                    env_set_local(loop_env, node->data.for_of_stmt.var, elem);
                    {
                        Value r = eval_block(interp, node->data.for_of_stmt.body, loop_env);
                        value_free(&r);
                    }
                    env_release(loop_env);
                    
                    if (interp->last_result.is_return || interp->last_result.is_break) {
                        interp->last_result.is_break = 0;
                        break;
                    }
                    if (interp->last_result.is_continue) {
                        interp->last_result.is_continue = 0;
                        continue;
                    }
                }
            } else if (iterable.type == VAL_STRING) {
                // Iterate over string characters
                const char *str = iterable.as.str_val;
                int len = strlen(str);
                for (i = 0; i < len; i++) {
                    Env *loop_env = env_new(env);
                    char char_str[2] = {str[i], '\0'};
                    Value char_val = make_string(char_str);
                    env_set_local(loop_env, node->data.for_of_stmt.var, char_val);
                    {
                        Value r = eval_block(interp, node->data.for_of_stmt.body, loop_env);
                        value_free(&r);
                    }
                    env_release(loop_env);
                    
                    if (interp->last_result.is_return || interp->last_result.is_break) {
                        interp->last_result.is_break = 0;
                        break;
                    }
                    if (interp->last_result.is_continue) {
                        interp->last_result.is_continue = 0;
                        continue;
                    }
                }
            } else if (iterable.type == VAL_OBJECT) {
                // Iterate over object keys
                Env *obj_env = iterable.as.object_val->fields;
                EnvEntry *entry = obj_env->entries;
                
                while (entry) {
                    Env *loop_env = env_new(env);
                    Value key_val = make_string(entry->name);
                    env_set_local(loop_env, node->data.for_of_stmt.var, key_val);
                    {
                        Value r = eval_block(interp, node->data.for_of_stmt.body, loop_env);
                        value_free(&r);
                    }
                    env_release(loop_env);
                    
                    if (interp->last_result.is_return || interp->last_result.is_break) {
                        interp->last_result.is_break = 0;
                        break;
                    }
                    if (interp->last_result.is_continue) {
                        interp->last_result.is_continue = 0;
                    }
                    
                    entry = entry->next;
                }
            } else if (iterable.type == VAL_DICT) {
                // Iterate over dictionary keys
                for (i = 0; i < iterable.as.dict_val->count; i++) {
                    Env *loop_env = env_new(env);
                    Value key = iterable.as.dict_val->keys[i];
                    // Copy strings to avoid double-free
                    if (key.type == VAL_STRING) {
                        key = make_string(key.as.str_val);
                    } else {
                        /* Increment ref count for dict/set keys */
                        value_retain(&key);
                    }
                    env_set_local(loop_env, node->data.for_of_stmt.var, key);
                    {
                        Value r = eval_block(interp, node->data.for_of_stmt.body, loop_env);
                        value_free(&r);
                    }
                    env_release(loop_env);
                    
                    if (interp->last_result.is_return || interp->last_result.is_break) {
                        interp->last_result.is_break = 0;
                        break;
                    }
                    if (interp->last_result.is_continue) {
                        interp->last_result.is_continue = 0;
                        continue;
                    }
                }
            } else if (iterable.type == VAL_SET) {
                // Iterate over set elements
                for (i = 0; i < iterable.as.set_val->count; i++) {
                    Env *loop_env = env_new(env);
                    Value elem = iterable.as.set_val->items[i];
                    // Copy strings to avoid double-free
                    if (elem.type == VAL_STRING) {
                        elem = make_string(elem.as.str_val);
                    } else {
                        /* Increment ref count for dict/set elements */
                        value_retain(&elem);
                    }
                    env_set_local(loop_env, node->data.for_of_stmt.var, elem);
                    {
                        Value r = eval_block(interp, node->data.for_of_stmt.body, loop_env);
                        value_free(&r);
                    }
                    env_release(loop_env);
                    
                    if (interp->last_result.is_return || interp->last_result.is_break) {
                        interp->last_result.is_break = 0;
                        break;
                    }
                    if (interp->last_result.is_continue) {
                        interp->last_result.is_continue = 0;
                        continue;
                    }
                }
            } else {
                fprintf(stderr, "Runtime error: for-of requires an iterable (list, tuple, string, dict, set, or object)\n");
            }
            
            value_free(&iterable);
            return make_null();
        }
        case NODE_FOR_C_STYLE: {
            // Create a new environment for the for loop
            Env *for_env = env_new(env);
            
            // Execute the initialization statement
            if (node->data.for_c_style_stmt.init) {
                Value init_val = eval_node_env(interp, node->data.for_c_style_stmt.init, for_env);
                value_free(&init_val);
            }
            
            // Loop while condition is true
            while (1) {
                // Evaluate the condition (if present, otherwise infinite loop)
                if (node->data.for_c_style_stmt.cond) {
                    Value cond_val = eval_node_env(interp, node->data.for_c_style_stmt.cond, for_env);
                    int cond_result = 0;
                    if (cond_val.type == VAL_BOOL) cond_result = cond_val.as.bool_val;
                    else if (cond_val.type == VAL_INT) cond_result = cond_val.as.int_val != 0;
                    else if (cond_val.type == VAL_FLOAT) cond_result = cond_val.as.float_val != 0.0;
                    else if (cond_val.type == VAL_STRING) cond_result = cond_val.as.str_val && cond_val.as.str_val[0] != '\0';
                    else if (cond_val.type == VAL_NULL) cond_result = 0;
                    else cond_result = 1;
                    value_free(&cond_val);
                    
                    if (!cond_result) {
                        break;
                    }
                }
                
                // Execute the body
                {
                    Value r = eval_block(interp, node->data.for_c_style_stmt.body, for_env);
                    value_free(&r);
                }
                
                // Check for break/continue/return
                if (interp->last_result.is_return || interp->last_result.is_break) {
                    interp->last_result.is_break = 0;
                    break;
                }
                if (interp->last_result.is_continue) {
                    interp->last_result.is_continue = 0;
                }
                
                // Execute the update expression
                if (node->data.for_c_style_stmt.update) {
                    Value update_val = eval_node_env(interp, node->data.for_c_style_stmt.update, for_env);
                    value_free(&update_val);
                }
            }
            
            env_release(for_env);
            return make_null();
        }
        case NODE_BINOP: {
            const char *op = node->data.binop.op;
            
            // Handle logical operators with short-circuit evaluation
            if (strcmp(op, "&&") == 0) {
                Value left = eval_node_env(interp, node->data.binop.left, env);
                int left_truthy = 0;
                if (left.type == VAL_BOOL) left_truthy = left.as.bool_val;
                else if (left.type == VAL_INT) left_truthy = left.as.int_val != 0;
                else if (left.type == VAL_FLOAT) left_truthy = left.as.float_val != 0.0;
                else if (left.type == VAL_STRING) left_truthy = strlen(left.as.str_val) > 0;
                else if (left.type == VAL_NULL) left_truthy = 0;
                
                if (!left_truthy) {
                    // Short-circuit: return false without evaluating right
                    value_free(&left);
                    return make_bool(0);
                }
                value_free(&left);
                
                // Evaluate right side
                Value right = eval_node_env(interp, node->data.binop.right, env);
                int right_truthy = 0;
                if (right.type == VAL_BOOL) right_truthy = right.as.bool_val;
                else if (right.type == VAL_INT) right_truthy = right.as.int_val != 0;
                else if (right.type == VAL_FLOAT) right_truthy = right.as.float_val != 0.0;
                else if (right.type == VAL_STRING) right_truthy = strlen(right.as.str_val) > 0;
                else if (right.type == VAL_NULL) right_truthy = 0;
                
                value_free(&right);
                return make_bool(right_truthy);
            }
            
            if (strcmp(op, "||") == 0) {
                Value left = eval_node_env(interp, node->data.binop.left, env);
                int left_truthy = 0;
                if (left.type == VAL_BOOL) left_truthy = left.as.bool_val;
                else if (left.type == VAL_INT) left_truthy = left.as.int_val != 0;
                else if (left.type == VAL_FLOAT) left_truthy = left.as.float_val != 0.0;
                else if (left.type == VAL_STRING) left_truthy = strlen(left.as.str_val) > 0;
                else if (left.type == VAL_NULL) left_truthy = 0;
                
                if (left_truthy) {
                    // Short-circuit: return true without evaluating right
                    value_free(&left);
                    return make_bool(1);
                }
                value_free(&left);
                
                // Evaluate right side
                Value right = eval_node_env(interp, node->data.binop.right, env);
                int right_truthy = 0;
                if (right.type == VAL_BOOL) right_truthy = right.as.bool_val;
                else if (right.type == VAL_INT) right_truthy = right.as.int_val != 0;
                else if (right.type == VAL_FLOAT) right_truthy = right.as.float_val != 0.0;
                else if (right.type == VAL_STRING) right_truthy = strlen(right.as.str_val) > 0;
                else if (right.type == VAL_NULL) right_truthy = 0;
                
                value_free(&right);
                return make_bool(right_truthy);
            }
            
            // Handle member assignment (this.x = value or ClassName.staticField = value)
            if (strcmp(op, "=") == 0 && node->data.binop.left->type == NODE_MEMBER_ACCESS) {
                ASTNode *member_node = node->data.binop.left;
                
                // Check if this is a static field assignment (ClassName.field = value)
                if (member_node->data.member_access.obj->type == NODE_IDENT) {
                    Value *maybe_class = env_get(env, member_node->data.member_access.obj->data.ident.name);
                    if (maybe_class && maybe_class->type == VAL_CLASS) {
                        // This is a static field assignment
                        Value val = eval_node_env(interp, node->data.binop.right, env);
                        env_set(maybe_class->as.class_val.static_fields, member_node->data.member_access.member, val);
                        return make_null();
                    }
                }
                
                // Regular instance field assignment
                Value obj = eval_node_env(interp, member_node->data.member_access.obj, env);
                Value val = eval_node_env(interp, node->data.binop.right, env);
                
                if (obj.type == VAL_OBJECT && obj.as.object_val && obj.as.object_val->fields) {
                    // Set the field on the object (env_set will take ownership of val)
                    env_set(obj.as.object_val->fields, member_node->data.member_access.member, val);
                }
                
                // Don't free val - it's now owned by the object's fields
                // Don't free obj - it's a view of 'this' from the environment, no ownership transfer
                return make_null();
            }
            
            // Handle index assignment (arr[0] = value)
            if (strcmp(op, "=") == 0 && node->data.binop.left->type == NODE_INDEX) {
                ASTNode *index_node = node->data.binop.left;
                Value val = eval_node_env(interp, node->data.binop.right, env);
                
                // Special case: if the object is an identifier, we need to modify it in-place
                if (index_node->data.index_expr.obj->type == NODE_IDENT) {
                    const char *arr_name = index_node->data.index_expr.obj->data.ident.name;
                    Value *arr_ptr = env_get(env, arr_name);
                    Value idx = eval_node_env(interp, index_node->data.index_expr.index, env);
                    
                    if (arr_ptr && arr_ptr->type == VAL_LIST && idx.type == VAL_INT) {
                        long long i = idx.as.int_val;
                        if (i >= 0 && i < (long long)arr_ptr->as.list_val.count) {
                            value_free(&arr_ptr->as.list_val.items[i]);
                            arr_ptr->as.list_val.items[i] = val;
                        }
                    }
                    value_free(&idx);
                    return make_null();
                } else {
                    // General case: evaluate the object (might be an expression)
                    Value obj = eval_node_env(interp, index_node->data.index_expr.obj, env);
                    Value idx = eval_node_env(interp, index_node->data.index_expr.index, env);
                    
                    if (obj.type == VAL_LIST && idx.type == VAL_INT) {
                        long long i = idx.as.int_val;
                        if (i >= 0 && i < (long long)obj.as.list_val.count) {
                            value_free(&obj.as.list_val.items[i]);
                            obj.as.list_val.items[i] = val;
                        }
                    }
                    
                    value_free(&obj);
                    value_free(&idx);
                    return make_null();
                }
            }
            
            Value left = eval_node_env(interp, node->data.binop.left, env);
            Value right = eval_node_env(interp, node->data.binop.right, env);
            Value result = make_null();

            if (strcmp(op, "+") == 0 && (left.type == VAL_STRING || right.type == VAL_STRING)) {
                char *ls = value_to_string(&left);
                char *rs = value_to_string(&right);
                size_t len = strlen(ls) + strlen(rs);
                char *combined = malloc(len + 1);
                strcpy(combined, ls);
                strcat(combined, rs);
                result.type = VAL_STRING;
                result.as.str_val = combined;
                free(ls);
                free(rs);
            } else if (strcmp(op, "==") == 0) {
                if (left.type == VAL_STRING && right.type == VAL_STRING)
                    result = make_bool(strcmp(left.as.str_val, right.as.str_val) == 0);
                else if (left.type == VAL_INT && right.type == VAL_INT)
                    result = make_bool(left.as.int_val == right.as.int_val);
                else if (left.type == VAL_FLOAT && right.type == VAL_FLOAT)
                    result = make_bool(left.as.float_val == right.as.float_val);
                else if (left.type == VAL_INT && right.type == VAL_FLOAT)
                    result = make_bool((double)left.as.int_val == right.as.float_val);
                else if (left.type == VAL_FLOAT && right.type == VAL_INT)
                    result = make_bool(left.as.float_val == (double)right.as.int_val);
                else if (left.type == VAL_BOOL && right.type == VAL_BOOL)
                    result = make_bool(left.as.bool_val == right.as.bool_val);
                else if (left.type == VAL_NULL && right.type == VAL_NULL)
                    result = make_bool(1);
                else
                    result = make_bool(0);
            } else if (strcmp(op, "!=") == 0) {
                int eq_val = 0;
                if (left.type == VAL_STRING && right.type == VAL_STRING)
                    eq_val = strcmp(left.as.str_val, right.as.str_val) == 0;
                else if (left.type == VAL_INT && right.type == VAL_INT)
                    eq_val = left.as.int_val == right.as.int_val;
                else if (left.type == VAL_BOOL && right.type == VAL_BOOL)
                    eq_val = left.as.bool_val == right.as.bool_val;
                result = make_bool(!eq_val);
            } else {
                double lv = 0, rv = 0;
                int is_int = 0;
                long long lvi = 0, rvi = 0;
                if (left.type == VAL_INT && right.type == VAL_INT) {
                    is_int = 1;
                    lvi = left.as.int_val;
                    rvi = right.as.int_val;
                    lv = (double)lvi;
                    rv = (double)rvi;
                } else {
                    if (left.type == VAL_INT) lv = (double)left.as.int_val;
                    else if (left.type == VAL_FLOAT) lv = left.as.float_val;
                    if (right.type == VAL_INT) rv = (double)right.as.int_val;
                    else if (right.type == VAL_FLOAT) rv = right.as.float_val;
                }

                if (strcmp(op, "+") == 0) result = is_int ? make_int(lvi + rvi) : make_float(lv + rv);
                else if (strcmp(op, "-") == 0) result = is_int ? make_int(lvi - rvi) : make_float(lv - rv);
                else if (strcmp(op, "*") == 0) result = is_int ? make_int(lvi * rvi) : make_float(lv * rv);
                else if (strcmp(op, "/") == 0) {
                    if (is_int && rvi != 0) result = make_int(lvi / rvi);
                    else result = make_float(rv != 0.0 ? lv / rv : 0.0);
                }
                else if (strcmp(op, "%") == 0) result = is_int ? make_int(lvi % rvi) : make_float(fmod(lv, rv));
                else if (strcmp(op, "**") == 0) result = make_float(pow(lv, rv));
                else if (strcmp(op, "<") == 0) result = make_bool(lv < rv);
                else if (strcmp(op, ">") == 0) result = make_bool(lv > rv);
                else if (strcmp(op, "<=") == 0) result = make_bool(lv <= rv);
                else if (strcmp(op, ">=") == 0) result = make_bool(lv >= rv);
            }
            value_free(&left);
            value_free(&right);
            return result;
        }
        case NODE_UNOP: {
            Value operand = eval_node_env(interp, node->data.unop.operand, env);
            const char *op = node->data.unop.op;
            Value result = make_null();
            if (strcmp(op, "-") == 0) {
                if (operand.type == VAL_INT) result = make_int(-operand.as.int_val);
                else if (operand.type == VAL_FLOAT) result = make_float(-operand.as.float_val);
            } else if (strcmp(op, "!") == 0) {
                int truthy = 0;
                if (operand.type == VAL_BOOL) truthy = operand.as.bool_val;
                else if (operand.type == VAL_INT) truthy = operand.as.int_val != 0;
                result = make_bool(!truthy);
            }
            value_free(&operand);
            return result;
        }
        case NODE_LIST: {
            Value list;
            int i, j;
            int total_count = 0;
            
            /* First pass: count total elements including spread arrays */
            for (i = 0; i < node->data.list.elements.count; i++) {
                ASTNode *elem = node->data.list.elements.items[i];
                if (elem->type == NODE_SPREAD) {
                    /* Evaluate spread element to count its items */
                    Value spread_val = eval_node_env(interp, elem->data.spread.argument, env);
                    if (spread_val.type == VAL_LIST) {
                        total_count += spread_val.as.list_val.count;
                    }
                    value_free(&spread_val);
                } else {
                    total_count++;
                }
            }
            
            list.type = VAL_LIST;
            list.as.list_val.count = total_count;
            list.as.list_val.capacity = total_count;
            list.as.list_val.items = malloc((total_count > 0 ? total_count : 1) * sizeof(Value));
            
            /* Second pass: fill the array */
            int idx = 0;
            for (i = 0; i < node->data.list.elements.count; i++) {
                ASTNode *elem = node->data.list.elements.items[i];
                if (elem->type == NODE_SPREAD) {
                    /* Spread array elements */
                    Value spread_val = eval_node_env(interp, elem->data.spread.argument, env);
                    if (spread_val.type == VAL_LIST) {
                        for (j = 0; j < spread_val.as.list_val.count; j++) {
                            Value item = spread_val.as.list_val.items[j];
                            /* Deep copy values */
                            if (item.type == VAL_STRING) {
                                list.as.list_val.items[idx++] = make_string(item.as.str_val);
                            } else if (item.type == VAL_LIST) {
                                /* Deep copy list */
                                Value list_copy;
                                int k;
                                list_copy.type = VAL_LIST;
                                list_copy.as.list_val.count = item.as.list_val.count;
                                list_copy.as.list_val.capacity = item.as.list_val.count;
                                list_copy.as.list_val.items = malloc((item.as.list_val.count > 0 ? item.as.list_val.count : 1) * sizeof(Value));
                                for (k = 0; k < item.as.list_val.count; k++) {
                                    if (item.as.list_val.items[k].type == VAL_STRING) {
                                        list_copy.as.list_val.items[k] = make_string(item.as.list_val.items[k].as.str_val);
                                    } else {
                                        list_copy.as.list_val.items[k] = item.as.list_val.items[k];
                                        value_retain(&list_copy.as.list_val.items[k]);
                                    }
                                }
                                list.as.list_val.items[idx++] = list_copy;
                            } else {
                                list.as.list_val.items[idx++] = item;
                                value_retain(&list.as.list_val.items[idx-1]);
                            }
                        }
                    }
                    /* Free the spread value properly */
                    value_free(&spread_val);
                } else {
                    /* Evaluate non-spread elements */
                    list.as.list_val.items[idx++] = eval_node_env(interp, elem, env);
                }
            }
            return list;
        }
        case NODE_TUPLE: {
            Value tuple;
            int i;
            int count = node->data.tuple.elements.count;
            
            tuple.type = VAL_TUPLE;
            tuple.as.tuple_val.count = count;
            tuple.as.tuple_val.elements = malloc((count > 0 ? count : 1) * sizeof(Value));
            
            /* Evaluate each element */
            for (i = 0; i < count; i++) {
                tuple.as.tuple_val.elements[i] = eval_node_env(interp, node->data.tuple.elements.items[i], env);
                value_retain(&tuple.as.tuple_val.elements[i]);
            }
            
            return tuple;
        }
        case NODE_OBJECT: {
            Value obj;
            int i;
            obj.type = VAL_OBJECT;
            obj.as.object_val = malloc(sizeof(ObjectVal));
            obj.as.object_val->class_name = strdup("Object");
            obj.as.object_val->fields = env_new(NULL);
            obj.as.object_val->methods = env_new(NULL);
            obj.as.object_val->ref_count = 1;
            
            for (i = 0; i < node->data.object.count; i++) {
                ObjectProperty *prop = &node->data.object.props[i];
                char *key_str = NULL;
                Value val;
                
                /* Check if this is a spread property */
                if (prop->value && prop->value->type == NODE_SPREAD) {
                    /* Spread object properties */
                    Value spread_val = eval_node_env(interp, prop->value->data.spread.argument, env);
                    if (spread_val.type == VAL_OBJECT) {
                        /* Copy all fields from spread object */
                        EnvEntry *entry = spread_val.as.object_val->fields->entries;
                        while (entry) {
                            Value field_val = entry->value;
                            /* Don't duplicate strings here - env_set_local will handle it */
                            if (field_val.type != VAL_STRING) {
                                /* Retain dict/set values */
                                value_retain(&field_val);
                            }
                            env_set_local(obj.as.object_val->fields, entry->name, field_val);
                            entry = entry->next;
                        }
                        /* Copy all methods from spread object */
                        entry = spread_val.as.object_val->methods->entries;
                        while (entry) {
                            env_set_local(obj.as.object_val->methods, entry->name, entry->value);
                            entry = entry->next;
                        }
                    }
                    /* Don't free spread_val to avoid double-free issues */
                    /* The GC will handle cleanup */
                    continue;
                }
                
                /* Compute the property key */
                if (prop->key) {
                    key_str = strdup(prop->key);
                } else if (prop->key_expr) {
                    /* Computed property name */
                    Value key_val = eval_node_env(interp, prop->key_expr, env);
                    key_str = value_to_string(&key_val);
                    value_free(&key_val);
                } else {
                    continue;
                }
                
                /* Evaluate the property value */
                if (prop->is_shorthand) {
                    /* Shorthand: {x} means {x: x} */
                    Value *var_val = env_get(env, key_str);
                    if (var_val) {
                        if (var_val->type == VAL_STRING) {
                            val = make_string(var_val->as.str_val);
                        } else {
                            val = *var_val;
                            /* Retain dict/set values when copying to object */
                            value_retain(&val);
                        }
                    } else {
                        fprintf(stderr, "Runtime error: undefined variable '%s' in object shorthand\n", key_str);
                        val = make_null();
                    }
                } else {
                    val = eval_node_env(interp, prop->value, env);
                    /* Retain dict/set values when adding to object */
                    value_retain(&val);
                }
                
                /* Add to object */
                if (prop->is_method || val.type == VAL_FUNCTION) {
                    env_set_local(obj.as.object_val->methods, key_str, val);
                } else {
                    env_set_local(obj.as.object_val->fields, key_str, val);
                }
                
                free(key_str);
            }
            return obj;
        }
        case NODE_INDEX: {
            Value obj = eval_node_env(interp, node->data.index_expr.obj, env);
            Value idx = eval_node_env(interp, node->data.index_expr.index, env);
            Value result = make_null();
            if (obj.type == VAL_LIST && idx.type == VAL_INT) {
                long long i = idx.as.int_val;
                if (i >= 0 && i < (long long)obj.as.list_val.count) {
                    Value *item = &obj.as.list_val.items[i];
                    if (item->type == VAL_STRING) {
                        result = make_string(item->as.str_val);
                    } else if (item->type == VAL_LIST) {
                        /* Deep copy nested arrays to avoid double-free */
                        Value list_copy;
                        int j;
                        list_copy.type = VAL_LIST;
                        list_copy.as.list_val.count = item->as.list_val.count;
                        list_copy.as.list_val.capacity = item->as.list_val.count;
                        list_copy.as.list_val.items = malloc((item->as.list_val.count > 0 ? item->as.list_val.count : 1) * sizeof(Value));
                        for (j = 0; j < item->as.list_val.count; j++) {
                            if (item->as.list_val.items[j].type == VAL_STRING) {
                                list_copy.as.list_val.items[j] = make_string(item->as.list_val.items[j].as.str_val);
                            } else {
                                list_copy.as.list_val.items[j] = item->as.list_val.items[j];
                                value_retain(&list_copy.as.list_val.items[j]);
                            }
                        }
                        result = list_copy;
                    } else {
                        result = *item;
                        /* Retain dict/set values when accessing from array */
                        value_retain(&result);
                    }
                }
            } else if (obj.type == VAL_TUPLE && idx.type == VAL_INT) {
                long long i = idx.as.int_val;
                if (i >= 0 && i < (long long)obj.as.tuple_val.count) {
                    Value *item = &obj.as.tuple_val.elements[i];
                    if (item->type == VAL_STRING) {
                        result = make_string(item->as.str_val);
                    } else if (item->type == VAL_LIST) {
                        /* Deep copy nested arrays to avoid double-free */
                        Value list_copy;
                        int j;
                        list_copy.type = VAL_LIST;
                        list_copy.as.list_val.count = item->as.list_val.count;
                        list_copy.as.list_val.capacity = item->as.list_val.count;
                        list_copy.as.list_val.items = malloc((item->as.list_val.count > 0 ? item->as.list_val.count : 1) * sizeof(Value));
                        for (j = 0; j < item->as.list_val.count; j++) {
                            if (item->as.list_val.items[j].type == VAL_STRING) {
                                list_copy.as.list_val.items[j] = make_string(item->as.list_val.items[j].as.str_val);
                            } else {
                                list_copy.as.list_val.items[j] = item->as.list_val.items[j];
                                value_retain(&list_copy.as.list_val.items[j]);
                            }
                        }
                        result = list_copy;
                    } else {
                        result = *item;
                        /* Retain dict/set values when accessing from tuple */
                        value_retain(&result);
                    }
                }
            } else if (obj.type == VAL_STRING && idx.type == VAL_INT) {
                long long i = idx.as.int_val;
                if (i >= 0 && (size_t)i < strlen(obj.as.str_val)) {
                    char buf[2] = {obj.as.str_val[i], 0};
                    result = make_string(buf);
                }
            }
            value_free(&obj);
            value_free(&idx);
            return result;
        }
        case NODE_CLASS_DEF: {
            // Create class value
            Value class_val = make_class(node->data.class_def.name, node->data.class_def.parent_name);
            class_val.as.class_val.is_abstract = node->data.class_def.is_abstract;
            
            // If there's a parent, copy parent methods and static members
            if (node->data.class_def.parent_name) {
                Value *parent = env_get(env, node->data.class_def.parent_name);
                if (parent && parent->type == VAL_CLASS) {
                    // Copy parent methods to child (deep copy functions)
                    EnvEntry *e = parent->as.class_val.methods->entries;
                    while (e) {
                        Value method_copy = e->value;
                        // Deep copy function param_names
                        if (method_copy.type == VAL_FUNCTION && method_copy.as.func_val.param_names) {
                            int j;
                            method_copy.as.func_val.param_names = malloc(method_copy.as.func_val.param_count * sizeof(char *));
                            for (j = 0; j < method_copy.as.func_val.param_count; j++)
                                method_copy.as.func_val.param_names[j] = strdup(e->value.as.func_val.param_names[j]);
                        }
                        env_set_local(class_val.as.class_val.methods, e->name, method_copy);
                        e = e->next;
                    }
                    // Copy parent fields (env_set_local will duplicate strings)
                    e = parent->as.class_val.fields->entries;
                    while (e) {
                        Value field_copy = e->value;
                        // Don't duplicate strings here - env_set_local will handle it
                        env_set_local(class_val.as.class_val.fields, e->name, field_copy);
                        e = e->next;
                    }
                    // Copy parent static methods
                    e = parent->as.class_val.static_methods->entries;
                    while (e) {
                        Value method_copy = e->value;
                        if (method_copy.type == VAL_FUNCTION && method_copy.as.func_val.param_names) {
                            int j;
                            method_copy.as.func_val.param_names = malloc(method_copy.as.func_val.param_count * sizeof(char *));
                            for (j = 0; j < method_copy.as.func_val.param_count; j++)
                                method_copy.as.func_val.param_names[j] = strdup(e->value.as.func_val.param_names[j]);
                        }
                        env_set_local(class_val.as.class_val.static_methods, e->name, method_copy);
                        e = e->next;
                    }
                    // Copy parent static fields (env_set_local will duplicate strings)
                    e = parent->as.class_val.static_fields->entries;
                    while (e) {
                        Value field_copy = e->value;
                        // Don't duplicate strings here - env_set_local will handle it
                        env_set_local(class_val.as.class_val.static_fields, e->name, field_copy);
                        e = e->next;
                    }
                }
            }
            
            // Process members
            int i;
            for (i = 0; i < node->data.class_def.members.count; i++) {
                ASTNode *member = node->data.class_def.members.items[i];
                if (member->type == NODE_FUNC_DEF) {
                    // Add method to class (static or instance)
                    Value func;
                    func.type = VAL_FUNCTION;
                    func.as.func_val.param_count = member->data.func_def.params.count;
                    func.as.func_val.param_names = NULL;
                    func.as.func_val.default_values = member->data.func_def.default_values;
                    func.as.func_val.is_async = member->data.func_def.is_async;
                    func.as.func_val.is_generator = member->data.func_def.is_generator;
                    func.as.func_val.has_rest_param = member->data.func_def.has_rest_param;
                    if (func.as.func_val.param_count > 0) {
                        int j;
                        func.as.func_val.param_names = malloc(func.as.func_val.param_count * sizeof(char *));
                        for (j = 0; j < func.as.func_val.param_count; j++)
                            func.as.func_val.param_names[j] = strdup(member->data.func_def.params.items[j]->data.ident.name);
                    }
                    func.as.func_val.body = member->data.func_def.body;
                    func.as.func_val.closure = env;
                    env_retain(env);  /* Retain closure environment */
                    if (member->data.func_def.is_static) {
                        env_set_local_with_access(class_val.as.class_val.static_methods, member->data.func_def.name, func, member->data.func_def.access);
                    } else {
                        env_set_local_with_access(class_val.as.class_val.methods, member->data.func_def.name, func, member->data.func_def.access);
                    }
                    value_free(&func);  /* Free the local copy after storing */
                } else if (member->type == NODE_LET) {
                    // Add field default value to class (static or instance)
                    Value field_val = member->data.let_stmt.value ? 
                        eval_node_env(interp, member->data.let_stmt.value, env) : make_null();
                    if (member->data.let_stmt.is_static) {
                        env_set_local_with_access(class_val.as.class_val.static_fields, member->data.let_stmt.name, field_val, member->data.let_stmt.access);
                    } else {
                        env_set_local_with_access(class_val.as.class_val.fields, member->data.let_stmt.name, field_val, member->data.let_stmt.access);
                    }
                    // Free the local copy after storing (env_set_local_with_access makes its own copy)
                    if (field_val.type == VAL_STRING && field_val.as.str_val) {
                        free(field_val.as.str_val);
                    }
                }
            }
            
            // Store class in environment
            env_set_local(env, node->data.class_def.name, class_val);
            return make_null();
        }
        case NODE_NEW: {
            // Instantiate object
            Value *class_val = env_get(env, node->data.new_expr.class_name);
            if (!class_val || class_val->type != VAL_CLASS) {
                fprintf(stderr, "Error at line %d: '%s' is not a class\n", 
                    node->line, node->data.new_expr.class_name);
                interp->had_error = 1;
                return make_null();
            }
            
            // Check if class is abstract
            if (class_val->as.class_val.is_abstract) {
                fprintf(stderr, "Error at line %d: cannot instantiate abstract class '%s'\n",
                    node->line, node->data.new_expr.class_name);
                interp->had_error = 1;
                return make_null();
            }
            
            // Create object
            Value obj = make_object(node->data.new_expr.class_name, class_val->as.class_val.methods);
            
            // Initialize fields with default values from class (env_set_local will duplicate strings)
            EnvEntry *field_entry = class_val->as.class_val.fields->entries;
            while (field_entry) {
                Value field_val = field_entry->value;
                // Don't duplicate here - env_set_local will handle it
                env_set_local(obj.as.object_val->fields, field_entry->name, field_val);
                field_entry = field_entry->next;
            }
            
            // Call init method if it exists
            Value *init_method = env_get(class_val->as.class_val.methods, "init");
            if (init_method && init_method->type == VAL_FUNCTION) {
                int argc = node->data.new_expr.args.count;
                Value *args = malloc((argc > 0 ? argc : 1) * sizeof(Value));
                int i;
                for (i = 0; i < argc; i++)
                    args[i] = eval_node_env(interp, node->data.new_expr.args.items[i], env);
                
                // Create call environment with 'this' bound
                Env *call_env = env_new(init_method->as.func_val.closure);
                env_set_local(call_env, "this", obj);
                for (i = 0; i < init_method->as.func_val.param_count && i < argc; i++)
                    env_set_local(call_env, init_method->as.func_val.param_names[i], args[i]);
                
                Value init_result = eval_block(interp, init_method->as.func_val.body, call_env);
                value_free(&init_result);
                
                
                env_release(call_env);
                for (i = 0; i < argc; i++) value_free(&args[i]);
                free(args);
                
                if (interp->last_result.is_return) {
                    value_free(&interp->last_result.return_value);
                    interp->last_result.is_return = 0;
                }
            }
            
            return obj;
        }
        case NODE_MEMBER_ACCESS: {
            // Check if we're accessing from within a method (have 'this')
            Value *this_val = env_get(env, "this");
            int is_inside_class = (this_val && this_val->type == VAL_OBJECT);
            
            // Check if this is a static member access (ClassName.member)
            if (node->data.member_access.obj->type == NODE_IDENT) {
                Value *maybe_class = env_get(env, node->data.member_access.obj->data.ident.name);
                if (maybe_class && maybe_class->type == VAL_CLASS) {
                    // This is a static member access
                    Value result = make_null();
                    
                    // Check static fields first
                    EnvEntry *field_entry = env_get_entry(maybe_class->as.class_val.static_fields, node->data.member_access.member);
                    if (field_entry) {
                        // Check access
                        if (field_entry->access == ACCESS_PRIVATE && !is_inside_class) {
                            fprintf(stderr, "Error at line %d: cannot access private static field '%s' of class '%s'\n",
                                    node->line, node->data.member_access.member, node->data.member_access.obj->data.ident.name);
                            interp->had_error = 1;
                            return make_null();
                        }
                        if (field_entry->value.type == VAL_STRING) result = make_string(field_entry->value.as.str_val);
                        else result = field_entry->value;
                        return result;
                    }
                    
                    // Check static methods
                    EnvEntry *method_entry = env_get_entry(maybe_class->as.class_val.static_methods, node->data.member_access.member);
                    if (method_entry && (method_entry->value.type == VAL_FUNCTION || method_entry->value.type == VAL_BUILTIN)) {
                        // Check access
                        if (method_entry->access == ACCESS_PRIVATE && !is_inside_class) {
                            fprintf(stderr, "Error at line %d: cannot access private static method '%s' of class '%s'\n",
                                    node->line, node->data.member_access.member, node->data.member_access.obj->data.ident.name);
                            interp->had_error = 1;
                            return make_null();
                        }
                        // For static methods, just return the function (no 'this' binding)
                        return method_entry->value;
                    }
                    
                    fprintf(stderr, "Error at line %d: class '%s' has no static member '%s'\n",
                            node->line, node->data.member_access.obj->data.ident.name, node->data.member_access.member);
                    interp->had_error = 1;
                    return make_null();
                }
            }
            
            // Regular instance member access
            Value obj = eval_node_env(interp, node->data.member_access.obj, env);
            Value result = make_null();
            
            // Handle array methods and properties
            if (obj.type == VAL_LIST) {
                const char *method_name = node->data.member_access.member;
                
                // Handle .length property
                if (strcmp(method_name, "length") == 0) {
                    result = make_int(obj.as.list_val.count);
                    value_free(&obj);
                    return result;
                }
                
                Value *builtin = NULL;
                
                // Map method names to builtin functions
                if (strcmp(method_name, "map") == 0) {
                    builtin = env_get(interp->global_env, "__array_map");
                } else if (strcmp(method_name, "filter") == 0) {
                    builtin = env_get(interp->global_env, "__array_filter");
                } else if (strcmp(method_name, "reduce") == 0) {
                    builtin = env_get(interp->global_env, "__array_reduce");
                } else if (strcmp(method_name, "forEach") == 0) {
                    builtin = env_get(interp->global_env, "__array_forEach");
                } else if (strcmp(method_name, "find") == 0) {
                    builtin = env_get(interp->global_env, "__array_find");
                } else if (strcmp(method_name, "some") == 0) {
                    builtin = env_get(interp->global_env, "__array_some");
                } else if (strcmp(method_name, "every") == 0) {
                    builtin = env_get(interp->global_env, "__array_every");
                } else if (strcmp(method_name, "indexOf") == 0) {
                    builtin = env_get(interp->global_env, "__array_indexOf");
                } else if (strcmp(method_name, "includes") == 0) {
                    builtin = env_get(interp->global_env, "__array_includes");
                } else if (strcmp(method_name, "push") == 0) {
                    builtin = env_get(interp->global_env, "__array_push");
                } else if (strcmp(method_name, "pop") == 0) {
                    builtin = env_get(interp->global_env, "__array_pop");
                } else if (strcmp(method_name, "slice") == 0) {
                    builtin = env_get(interp->global_env, "__array_slice");
                } else if (strcmp(method_name, "concat") == 0) {
                    builtin = env_get(interp->global_env, "__array_concat");
                } else if (strcmp(method_name, "join") == 0) {
                    builtin = env_get(interp->global_env, "__array_join");
                } else if (strcmp(method_name, "reverse") == 0) {
                    builtin = env_get(interp->global_env, "__array_reverse");
                } else if (strcmp(method_name, "sort") == 0) {
                    builtin = env_get(interp->global_env, "__array_sort");
                }
                
                if (builtin && builtin->type == VAL_BUILTIN) {
                    // Create a bound method with the array as receiver
                    result = make_method(obj, *builtin);
                    return result;
                }
                
                fprintf(stderr, "Error at line %d: array has no method '%s'\n",
                        node->line, method_name);
                interp->had_error = 1;
                value_free(&obj);
                return make_null();
            }
            
            // Handle dict methods
            if (obj.type == VAL_DICT) {
                const char *method_name = node->data.member_access.member;
                
                // Add null check for dict_val
                if (!obj.as.dict_val) {
                    fprintf(stderr, "Error: Attempting to access method on freed dict\n");
                    value_free(&obj);
                    return make_null();
                }
                
                // Handle .size property
                if (strcmp(method_name, "size") == 0) {
                    result = make_int(obj.as.dict_val->count);
                    value_free(&obj);
                    return result;
                }
                
                Value *builtin = NULL;
                
                // Map method names to builtin functions
                if (strcmp(method_name, "set") == 0) {
                    builtin = env_get(interp->global_env, "__dict_set");
                } else if (strcmp(method_name, "get") == 0) {
                    builtin = env_get(interp->global_env, "__dict_get");
                } else if (strcmp(method_name, "has") == 0) {
                    builtin = env_get(interp->global_env, "__dict_has");
                } else if (strcmp(method_name, "delete") == 0) {
                    builtin = env_get(interp->global_env, "__dict_delete");
                } else if (strcmp(method_name, "keys") == 0) {
                    builtin = env_get(interp->global_env, "__dict_keys");
                } else if (strcmp(method_name, "values") == 0) {
                    builtin = env_get(interp->global_env, "__dict_values");
                }
                
                if (builtin && builtin->type == VAL_BUILTIN) {
                    // Create a bound method with the dict as receiver
                    result = make_method(obj, *builtin);
                    // make_method takes ownership, balance NODE_IDENT's increment
                    if (obj.type == VAL_DICT && obj.as.dict_val) {
                        obj.as.dict_val->ref_count--;
                    }
                    return result;
                }
                
                fprintf(stderr, "Error at line %d: dict has no method '%s'\n",
                        node->line, method_name);
                interp->had_error = 1;
                value_free(&obj);
                return make_null();
            }
            
            // Handle set methods
            if (obj.type == VAL_SET) {
                const char *method_name = node->data.member_access.member;
                
                // Add null check for set_val
                if (!obj.as.set_val) {
                    fprintf(stderr, "Error: Attempting to access method on freed set\n");
                    value_free(&obj);
                    return make_null();
                }
                
                // Handle .size property
                if (strcmp(method_name, "size") == 0) {
                    result = make_int(obj.as.set_val->count);
                    value_free(&obj);
                    return result;
                }
                
                Value *builtin = NULL;
                
                // Map method names to builtin functions
                if (strcmp(method_name, "add") == 0) {
                    builtin = env_get(interp->global_env, "__set_add");
                } else if (strcmp(method_name, "remove") == 0) {
                    builtin = env_get(interp->global_env, "__set_remove");
                } else if (strcmp(method_name, "has") == 0) {
                    builtin = env_get(interp->global_env, "__set_has");
                } else if (strcmp(method_name, "clear") == 0) {
                    builtin = env_get(interp->global_env, "__set_clear");
                }
                
                if (builtin && builtin->type == VAL_BUILTIN) {
                    // Create a bound method with the set as receiver
                    result = make_method(obj, *builtin);
                    // make_method takes ownership, balance NODE_IDENT's increment
                    if (obj.type == VAL_SET && obj.as.set_val) {
                        obj.as.set_val->ref_count--;
                    }
                    return result;
                }
                
                fprintf(stderr, "Error at line %d: set has no method '%s'\n",
                        node->line, method_name);
                interp->had_error = 1;
                value_free(&obj);
                return make_null();
            }
            
            // Handle generator methods
            if (obj.type == VAL_GENERATOR) {
                const char *method_name = node->data.member_access.member;
                Value *builtin = NULL;
                
                if (strcmp(method_name, "next") == 0) {
                    builtin = env_get(interp->global_env, "__generator_next");
                }
                
                if (builtin && builtin->type == VAL_BUILTIN) {
                    // Create a bound method with the generator as receiver
                    result = make_method(obj, *builtin);
                    return result;
                }
                
                fprintf(stderr, "Error at line %d: generator has no method '%s'\n",
                        node->line, method_name);
                interp->had_error = 1;
                value_free(&obj);
                return make_null();
            }
            
            // Handle promise methods
            if (obj.type == VAL_PROMISE) {
                const char *method_name = node->data.member_access.member;
                Value *builtin = NULL;
                
                if (strcmp(method_name, "then") == 0) {
                    builtin = env_get(interp->global_env, "__promise_then");
                } else if (strcmp(method_name, "catch") == 0) {
                    builtin = env_get(interp->global_env, "__promise_catch");
                } else if (strcmp(method_name, "finally") == 0) {
                    builtin = env_get(interp->global_env, "__promise_finally");
                }
                
                if (builtin && builtin->type == VAL_BUILTIN) {
                    // Create a bound method with the promise as receiver
                    result = make_method(obj, *builtin);
                    return result;
                }
                
                fprintf(stderr, "Error at line %d: promise has no method '%s'\n",
                        node->line, method_name);
                interp->had_error = 1;
                value_free(&obj);
                return make_null();
            }
            
            if (obj.type == VAL_OBJECT) {
                // Determine if we're accessing our own object
                int is_self_access = (is_inside_class && this_val && 
                                     this_val->type == VAL_OBJECT &&
                                     strcmp(this_val->as.object_val->class_name, obj.as.object_val->class_name) == 0);
                
                // Check fields first
                EnvEntry *field_entry = env_get_entry(obj.as.object_val->fields, node->data.member_access.member);
                if (field_entry) {
                    // Check access
                    if (field_entry->access == ACCESS_PRIVATE && !is_self_access) {
                        fprintf(stderr, "Error at line %d: cannot access private field '%s'\n",
                                node->line, node->data.member_access.member);
                        interp->had_error = 1;
                        return make_null();
                    }
                    if (field_entry->value.type == VAL_STRING) result = make_string(field_entry->value.as.str_val);
                    else result = field_entry->value;
                } else {
                    // Check methods
                    EnvEntry *method_entry = env_get_entry(obj.as.object_val->methods, node->data.member_access.member);
                    if (method_entry && method_entry->value.type == VAL_FUNCTION) {
                        // Check access
                        if (method_entry->access == ACCESS_PRIVATE && !is_self_access) {
                            fprintf(stderr, "Error at line %d: cannot access private method '%s'\n",
                                    node->line, node->data.member_access.member);
                            interp->had_error = 1;
                            return make_null();
                        }
                        // Return bound method
                        result = make_method(obj, method_entry->value);
                        // Don't decrement here - we'll call value_free(&obj) at the end
                        // to properly release the temporary reference from NODE_IDENT
                    }
                }
            } else if (obj.type == VAL_CLASS) {
                // Access to static methods or parent class
                Value *method = env_get(obj.as.class_val.methods, node->data.member_access.member);
                if (method) {
                    if (method->type == VAL_STRING) result = make_string(method->as.str_val);
                    else result = *method;
                }
            } else if (obj.type == VAL_MODULE) {
                // Access to module exports (e.g., Math.PI)
                Value *exported = env_get(obj.as.module_val.exports, node->data.member_access.member);
                if (exported) {
                    if (exported->type == VAL_STRING) result = make_string(exported->as.str_val);
                    else result = *exported;
                } else {
                    fprintf(stderr, "Error at line %d: module does not export '%s'\n",
                            node->line, node->data.member_access.member);
                    interp->had_error = 1;
                }
            } else if (obj.type == VAL_STRUCT) {
                // Access to struct/union fields
                if (!obj.as.struct_val || !obj.as.struct_val->fields) {
                    fprintf(stderr, "Error at line %d: attempting to access field on invalid struct\n", node->line);
                    interp->had_error = 1;
                    value_free(&obj);
                    return make_null();
                }
                
                Value *field = env_get(obj.as.struct_val->fields, node->data.member_access.member);
                if (field) {
                    if (field->type == VAL_STRING) {
                        result = make_string(field->as.str_val);
                    } else {
                        result = *field;
                        value_retain(&result);
                    }
                } else {
                    fprintf(stderr, "Error at line %d: struct '%s' has no field '%s'\n",
                            node->line, 
                            obj.as.struct_val->struct_name ? obj.as.struct_val->struct_name : "?",
                            node->data.member_access.member);
                    interp->had_error = 1;
                }
            }
            
            // Free obj to properly decrement reference counts
            // For objects with reference counting, value_free will only decrement ref_count
            // For class/module, we still skip to avoid double-free of shared Env pointers
            // For NODE_THIS, don't decrement because it didn't increment (it's already owned by env)
            if (obj.type != VAL_CLASS && obj.type != VAL_MODULE && 
                node->data.member_access.obj->type != NODE_THIS) {
                value_free(&obj);
            }
            return result;
        }
        case NODE_THIS: {
            Value *this_val = env_get(env, "this");
            if (!this_val) {
                fprintf(stderr, "Error at line %d: 'this' used outside of class method\n", node->line);
                interp->had_error = 1;
                return make_null();
            }
            // Return the value directly (share Env pointers, don't deep copy)
            // Don't increment ref_count - 'this' is already owned by the environment
            return *this_val;
        }
        case NODE_SUPER: {
            // Get 'this' to find current object
            Value *this_val = env_get(env, "this");
            if (!this_val || this_val->type != VAL_OBJECT) {
                fprintf(stderr, "Error at line %d: 'super' used outside of class method\n", node->line);
                interp->had_error = 1;
                return make_null();
            }
            
            // Get the class definition
            Value *class_val = env_get(env, this_val->as.object_val->class_name);
            if (!class_val || class_val->type != VAL_CLASS) {
                fprintf(stderr, "Error at line %d: cannot find class definition\n", node->line);
                interp->had_error = 1;
                return make_null();
            }
            
            // Get parent class
            if (!class_val->as.class_val.parent_name) {
                fprintf(stderr, "Error at line %d: class has no parent\n", node->line);
                interp->had_error = 1;
                return make_null();
            }
            
            Value *parent_class = env_get(env, class_val->as.class_val.parent_name);
            if (!parent_class || parent_class->type != VAL_CLASS) {
                fprintf(stderr, "Error at line %d: cannot find parent class\n", node->line);
                interp->had_error = 1;
                return make_null();
            }
            
            // Access parent method
            Value *method = env_get(parent_class->as.class_val.methods, node->data.super_expr.member);
            if (!method || method->type != VAL_FUNCTION) {
                fprintf(stderr, "Error at line %d: parent class has no method '%s'\n", 
                    node->line, node->data.super_expr.member);
                interp->had_error = 1;
                return make_null();
            }
            
            // Return bound method
            return make_method(*this_val, *method);
        }
        case NODE_TEMPLATE_LITERAL: {
            /* Evaluate template literal by concatenating parts and evaluated expressions */
            int result_cap = 256;
            int result_len = 0;
            char *result = malloc(result_cap);
            int i;
            
            /* Iterate through parts and expressions */
            for (i = 0; i < node->data.template_literal.count; i++) {
                /* Add the string part */
                const char *part = node->data.template_literal.parts[i];
                int part_len = strlen(part);
                
                while (result_len + part_len + 1 >= result_cap) {
                    result_cap *= 2;
                    result = realloc(result, result_cap);
                }
                strcpy(result + result_len, part);
                result_len += part_len;
                
                /* Add the evaluated expression (if not the last part) */
                if (i < node->data.template_literal.count - 1) {
                    Value expr_val = eval_node_env(interp, node->data.template_literal.exprs[i], env);
                    char *expr_str = value_to_string(&expr_val);
                    int expr_len = strlen(expr_str);
                    
                    while (result_len + expr_len + 1 >= result_cap) {
                        result_cap *= 2;
                        result = realloc(result, result_cap);
                    }
                    strcpy(result + result_len, expr_str);
                    result_len += expr_len;
                    
                    free(expr_str);
                    value_free(&expr_val);
                }
            }
            
            result[result_len] = '\0';
            Value v = make_string(result);
            free(result);
            return v;
        }
        case NODE_TERNARY: {
            Value cond = eval_node_env(interp, node->data.ternary.cond, env);
            int is_true = 0;
            if (cond.type == VAL_BOOL) is_true = cond.as.bool_val;
            else if (cond.type == VAL_INT) is_true = cond.as.int_val != 0;
            else if (cond.type == VAL_FLOAT) is_true = cond.as.float_val != 0.0;
            else if (cond.type == VAL_STRING) is_true = cond.as.str_val && cond.as.str_val[0] != '\0';
            else if (cond.type == VAL_NULL) is_true = 0;
            else is_true = 1;
            value_free(&cond);
            
            if (is_true) {
                return eval_node_env(interp, node->data.ternary.true_expr, env);
            } else {
                return eval_node_env(interp, node->data.ternary.false_expr, env);
            }
        }
        case NODE_AWAIT: {
            /* Evaluate the expression being awaited */
            Value awaited = eval_node_env(interp, node->data.await_expr.expr, env);
            
            /* If it's a Promise, extract the resolved value */
            if (awaited.type == VAL_PROMISE) {
                PromiseVal promise = awaited.as.promise_val;
                
                /* If promise is fulfilled, return the result */
                if (promise.state == PROMISE_FULFILLED) {
                    Value result = *promise.result;
                    /* Don't free the original promise value - just extract result */
                    return result;
                }
                /* If promise is rejected, throw the error */
                else if (promise.state == PROMISE_REJECTED) {
                    fprintf(stderr, "Uncaught (in promise): ");
                    if (promise.result) {
                        if (promise.result->type == VAL_STRING) {
                            fprintf(stderr, "%s\n", promise.result->as.str_val);
                        } else {
                            fprintf(stderr, "Error\n");
                        }
                    } else {
                        fprintf(stderr, "Error\n");
                    }
                    interp->had_error = 1;
                    value_free(&awaited);
                    return make_null();
                }
                /* If promise is pending, for now return null (in real async this would suspend) */
                else {
                    fprintf(stderr, "Warning: awaiting pending promise - returning null\n");
                    value_free(&awaited);
                    return make_null();
                }
            }
            
            /* If not a promise, return the value as-is (await on non-promise resolves immediately) */
            return awaited;
        }
        case NODE_YIELD: {
            /* Yield is handled specially by generator execution */
            /* When encountered, we should save state and return */
            /* This is a marker that will be processed by the generator runner */
            Value result;
            if (node->data.yield_expr.value) {
                result = eval_node_env(interp, node->data.yield_expr.value, env);
            } else {
                result = make_null();
            }
            /* Set a special flag to indicate yield */
            interp->last_result.is_return = 1; /* Reuse return mechanism for now */
            interp->last_result.return_value = result;
            return result;
        }
        
        /* Module system - Import statements */
        case NODE_IMPORT_NAMED: {
            /* import {name1, name2 as alias} from "module" */
            char *resolved_path = resolve_module_path(interp, node->data.import_named.module_path);
            Value module = load_module(interp, resolved_path, env);
            int i;
            
            if (module.type == VAL_MODULE) {
                Env *exports = module.as.module_val.exports;
                for (i = 0; i < node->data.import_named.count; i++) {
                    const char *name = node->data.import_named.names[i];
                    const char *alias = node->data.import_named.aliases[i];
                    const char *local_name = alias ? alias : name;
                    
                    Value *exported = env_get(exports, name);
                    if (exported) {
                        /* Deep copy for certain types */
                        Value imported;
                        if (exported->type == VAL_FUNCTION && exported->as.func_val.param_names) {
                            imported = *exported;
                            imported.as.func_val.param_names = malloc(exported->as.func_val.param_count * sizeof(char *));
                            for (int j = 0; j < exported->as.func_val.param_count; j++)
                                imported.as.func_val.param_names[j] = strdup(exported->as.func_val.param_names[j]);
                        } else if (exported->type == VAL_STRING) {
                            imported = make_string(exported->as.str_val);
                        } else {
                            imported = *exported;
                        }
                        env_set_local(env, local_name, imported);
                    } else {
                        fprintf(stderr, "Error at line %d: Module '%s' does not export '%s'\n",
                            node->line, node->data.import_named.module_path, name);
                        interp->had_error = 1;
                    }
                }
            }
            
            free(resolved_path);
            value_free(&module);
            return make_null();
        }
        
        case NODE_IMPORT_DEFAULT: {
            /* import name from "module" */
            char *resolved_path = resolve_module_path(interp, node->data.import_default.module_path);
            Value module = load_module(interp, resolved_path, env);
            
            if (module.type == VAL_MODULE) {
                Env *exports = module.as.module_val.exports;
                Value *default_export = env_get(exports, "default");
                if (default_export) {
                    /* Deep copy for certain types */
                    Value imported;
                    if (default_export->type == VAL_FUNCTION && default_export->as.func_val.param_names) {
                        imported = *default_export;
                        imported.as.func_val.param_names = malloc(default_export->as.func_val.param_count * sizeof(char *));
                        for (int j = 0; j < default_export->as.func_val.param_count; j++)
                            imported.as.func_val.param_names[j] = strdup(default_export->as.func_val.param_names[j]);
                    } else if (default_export->type == VAL_STRING) {
                        imported = make_string(default_export->as.str_val);
                    } else {
                        imported = *default_export;
                    }
                    env_set_local(env, node->data.import_default.name, imported);
                } else {
                    fprintf(stderr, "Error at line %d: Module '%s' does not have a default export\n",
                        node->line, node->data.import_default.module_path);
                    interp->had_error = 1;
                }
            }
            
            free(resolved_path);
            value_free(&module);
            return make_null();
        }
        
        case NODE_IMPORT_NAMESPACE: {
            /* import * as name from "module" */
            char *resolved_path = resolve_module_path(interp, node->data.import_namespace.module_path);
            Value module = load_module(interp, resolved_path, env);
            
            if (module.type == VAL_MODULE) {
                /* Store the module value itself so user can access module.name */
                env_set_local(env, node->data.import_namespace.namespace, module);
            } else {
                value_free(&module);
            }
            
            free(resolved_path);
            return make_null();
        }
        
        case NODE_EXPORT:
            /* Exports are handled during module loading, not during normal execution */
            /* If we encounter this during normal (non-module) execution, it's an error */
            if (node->data.export_stmt.declaration) {
                /* Execute the declaration in the current environment */
                return eval_node_env(interp, node->data.export_stmt.declaration, env);
            }
            return make_null();
        
        case NODE_POSTFIX: {
            /* Handle prefix/postfix increment and decrement */
            const char *op = node->data.postfix.op;
            ASTNode *operand_node = node->data.postfix.operand;
            int is_postfix = node->data.postfix.is_postfix;
            
            /* Get current value */
            Value old_val = eval_node_env(interp, operand_node, env);
            Value new_val = make_null();
            
            /* Calculate new value */
            if (strcmp(op, "++") == 0) {
                if (old_val.type == VAL_INT) {
                    new_val = make_int(old_val.as.int_val + 1);
                } else if (old_val.type == VAL_FLOAT) {
                    new_val = make_float(old_val.as.float_val + 1.0);
                } else {
                    value_free(&old_val);
                    return make_null();
                }
            } else if (strcmp(op, "--") == 0) {
                if (old_val.type == VAL_INT) {
                    new_val = make_int(old_val.as.int_val - 1);
                } else if (old_val.type == VAL_FLOAT) {
                    new_val = make_float(old_val.as.float_val - 1.0);
                } else {
                    value_free(&old_val);
                    return make_null();
                }
            }
            
            /* Store the new value */
            if (operand_node->type == NODE_IDENT) {
                env_set(env, operand_node->data.ident.name, new_val);
            } else if (operand_node->type == NODE_MEMBER_ACCESS) {
                Value obj = eval_node_env(interp, operand_node->data.member_access.obj, env);
                if (obj.type == VAL_OBJECT) {
                    env_set(obj.as.object_val->fields, operand_node->data.member_access.member, new_val);
                }
                value_free(&obj);
            } else if (operand_node->type == NODE_INDEX) {
                // Special case: if the object is an identifier, modify it in-place
                if (operand_node->data.index_expr.obj->type == NODE_IDENT) {
                    const char *arr_name = operand_node->data.index_expr.obj->data.ident.name;
                    Value *arr_ptr = env_get(env, arr_name);
                    Value idx = eval_node_env(interp, operand_node->data.index_expr.index, env);
                    
                    if (arr_ptr && arr_ptr->type == VAL_LIST && idx.type == VAL_INT) {
                        long long i = idx.as.int_val;
                        if (i >= 0 && i < (long long)arr_ptr->as.list_val.count) {
                            value_free(&arr_ptr->as.list_val.items[i]);
                            arr_ptr->as.list_val.items[i] = new_val;
                        }
                    }
                    value_free(&idx);
                } else {
                    Value obj = eval_node_env(interp, operand_node->data.index_expr.obj, env);
                    Value idx = eval_node_env(interp, operand_node->data.index_expr.index, env);
                    if (obj.type == VAL_LIST && idx.type == VAL_INT) {
                        long long i = idx.as.int_val;
                        if (i >= 0 && i < (long long)obj.as.list_val.count) {
                            value_free(&obj.as.list_val.items[i]);
                            obj.as.list_val.items[i] = new_val;
                        }
                    }
                    value_free(&obj);
                    value_free(&idx);
                }
            }
            
            /* Return old value for postfix, new value for prefix */
            if (is_postfix) {
                return old_val;
            } else {
                value_free(&old_val);
                return new_val;
            }
        }
        
        case NODE_OPTIONAL_CHAIN: {
            /* Handle optional chaining (?.) */
            Value obj = eval_node_env(interp, node->data.optional_chain.obj, env);
            
            /* If obj is null or undefined, return null */
            if (obj.type == VAL_NULL) {
                value_free(&obj);
                return make_null();
            }
            
            /* Otherwise, perform normal member access */
            const char *member = node->data.optional_chain.member;
            Value result = make_null();
            
            if (obj.type == VAL_OBJECT) {
                Value *val = env_get(obj.as.object_val->fields, member);
                if (val) {
                    if (val->type == VAL_STRING) {
                        result = make_string(val->as.str_val);
                    } else {
                        result = *val;
                    }
                }
            }
            
            value_free(&obj);
            return result;
        }
        
        case NODE_NULLISH_COALESCE: {
            /* Handle nullish coalescing (??) */
            Value left = eval_node_env(interp, node->data.nullish_coalesce.left, env);
            
            /* If left is null or undefined, evaluate and return right */
            if (left.type == VAL_NULL) {
                value_free(&left);
                return eval_node_env(interp, node->data.nullish_coalesce.right, env);
            }
            
            /* Otherwise return left */
            return left;
        }
        
        case NODE_ADDRESS_OF: {
            /* Handle address-of operator &variable */
            ASTNode *operand = node->data.address_of.operand;
            
            /* For now, we can only take the address of identifiers */
            if (operand->type == NODE_IDENT) {
                Value *var_ref = env_get(env, operand->data.ident.name);
                if (!var_ref) {
                    fprintf(stderr, "Error at line %d: undefined variable '%s'\n", 
                        node->line, operand->data.ident.name);
                    interp->had_error = 1;
                    return make_null();
                }
                /* Return a pointer to the variable's value */
                return make_pointer((void*)var_ref, NULL);
            } else {
                fprintf(stderr, "Error at line %d: address-of operator requires a variable\n", node->line);
                interp->had_error = 1;
                return make_null();
            }
        }
        
        case NODE_DEREFERENCE: {
            /* Handle dereference operator *pointer */
            Value ptr_val = eval_node_env(interp, node->data.dereference.operand, env);
            
            if (ptr_val.type == VAL_POINTER) {
                /* Dereference the pointer to get the value */
                Value *pointed_value = (Value*)ptr_val.as.pointer_val.ptr;
                
                /* Make a copy of the pointed-to value */
                Value result;
                if (pointed_value->type == VAL_STRING) {
                    result = make_string(pointed_value->as.str_val);
                } else {
                    result = *pointed_value;
                    value_retain(&result);
                }
                value_free(&ptr_val);
                return result;
            } else if (ptr_val.type == VAL_INT) {
                /* Handle pointers stored as integers (from malloc) */
                void *ptr = (void*)(uintptr_t)ptr_val.as.int_val;
                if (!ptr) {
                    fprintf(stderr, "Error at line %d: attempt to dereference null pointer\n", node->line);
                    interp->had_error = 1;
                    value_free(&ptr_val);
                    return make_null();
                }
                /* For raw pointers, we can't safely dereference without type info */
                fprintf(stderr, "Error at line %d: cannot dereference raw pointer without type information\n", node->line);
                interp->had_error = 1;
                value_free(&ptr_val);
                return make_null();
            } else {
                fprintf(stderr, "Error at line %d: dereference operator requires a pointer\n", node->line);
                interp->had_error = 1;
                value_free(&ptr_val);
                return make_null();
            }
        }
        
        case NODE_POINTER_MEMBER: {
            /* Handle ptr->member (equivalent to (*ptr).member) */
            Value ptr_val = eval_node_env(interp, node->data.pointer_member.ptr, env);
            
            if (ptr_val.type == VAL_POINTER) {
                /* Dereference the pointer */
                Value *pointed_value = (Value*)ptr_val.as.pointer_val.ptr;
                
                /* Access the member */
                if (pointed_value->type == VAL_OBJECT) {
                    const char *member = node->data.pointer_member.member;
                    Value *field = env_get(pointed_value->as.object_val->fields, member);
                    if (field) {
                        Value result = *field;
                        value_retain(&result);
                        value_free(&ptr_val);
                        return result;
                    } else {
                        fprintf(stderr, "Error at line %d: object has no member '%s'\n", 
                            node->line, member);
                        interp->had_error = 1;
                        value_free(&ptr_val);
                        return make_null();
                    }
                } else if (pointed_value->type == VAL_STRUCT) {
                    /* Access struct member through pointer */
                    const char *member = node->data.pointer_member.member;
                    if (!pointed_value->as.struct_val || !pointed_value->as.struct_val->fields) {
                        fprintf(stderr, "Error at line %d: invalid struct pointer\n", node->line);
                        interp->had_error = 1;
                        value_free(&ptr_val);
                        return make_null();
                    }
                    Value *field = env_get(pointed_value->as.struct_val->fields, member);
                    if (field) {
                        Value result = *field;
                        value_retain(&result);
                        value_free(&ptr_val);
                        return result;
                    } else {
                        fprintf(stderr, "Error at line %d: struct '%s' has no field '%s'\n", 
                            node->line,
                            pointed_value->as.struct_val->struct_name ? pointed_value->as.struct_val->struct_name : "?",
                            member);
                        interp->had_error = 1;
                        value_free(&ptr_val);
                        return make_null();
                    }
                } else {
                    fprintf(stderr, "Error at line %d: pointer member access requires object or struct pointer\n", node->line);
                    interp->had_error = 1;
                    value_free(&ptr_val);
                    return make_null();
                }
            } else {
                fprintf(stderr, "Error at line %d: arrow operator requires a pointer\n", node->line);
                interp->had_error = 1;
                value_free(&ptr_val);
                return make_null();
            }
        }
        
        case NODE_STRUCT_DEF:
        case NODE_UNION_DEF: {
            /* Store struct/union definition in environment as a type */
            /* For now, we'll store the AST node itself as metadata */
            /* In a full implementation, we'd have a type system */
            char *struct_name = node->data.struct_def.name;
            if (struct_name) {
                /* Register the struct type - store type info in a special way */
                /* For simplicity, just store as a marker in env for now */
                Value type_marker = make_null();  /* Placeholder for type info */
                env_set(env, struct_name, type_marker);
            }
            return make_null();
        }
        
        case NODE_STRUCT_LITERAL: {
            /* Create struct instance from literal */
            char *struct_name = node->data.struct_literal.struct_name;
            int is_union = 0;  /* TODO: Determine if it's a union based on type info */
            Value struct_val = make_struct(struct_name, is_union);
            
            /* Initialize fields */
            for (int i = 0; i < node->data.struct_literal.field_count; i++) {
                char *field_name = node->data.struct_literal.fields[i].key;
                ASTNode *field_value_node = node->data.struct_literal.fields[i].value;
                
                if (field_name && field_value_node) {
                    Value field_value = eval_node_env(interp, field_value_node, env);
                    env_set(struct_val.as.struct_val->fields, field_name, field_value);
                }
            }
            
            return struct_val;
        }
        
        default:
            return make_null();
    }
}

Value eval_node(Interpreter *interp, ASTNode *node) {
    return eval_node_env(interp, node, interp->global_env);
}

// Microtask queue implementation
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

#include <libgen.h>
#include <unistd.h>
#include <limits.h>

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
