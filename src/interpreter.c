#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

static Value eval_node_env(Interpreter *interp, ASTNode *node, Env *env);

// Forward declaration for Promise constructor
extern Value builtin_Promise_constructor(Interpreter *interp, Value *args, int argc);

Env *env_new(Env *parent) {
    Env *e = calloc(1, sizeof(Env));
    e->parent = parent;
    return e;
}

void env_free(Env *env) {
    EnvEntry *e = env->entries;
    while (e) {
        EnvEntry *next = e->next;
        free(e->name);
        value_free(&e->value);
        free(e);
        e = next;
    }
    free(env);
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
    // Deep copy strings to avoid double-free issues
    Value val_copy = val;
    if (val.type == VAL_STRING && val.as.str_val) {
        val_copy.as.str_val = strdup(val.as.str_val);
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
    val.as.object_val.class_name = strdup(class_name);
    val.as.object_val.fields = env_new(NULL);
    val.as.object_val.methods = methods;
    return val;
}

Value make_method(Value receiver, Value method) {
    Value val;
    val.type = VAL_METHOD;
    val.as.method_val.receiver = malloc(sizeof(Value));
    val.as.method_val.method = malloc(sizeof(Value));
    *val.as.method_val.receiver = receiver;
    
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

void value_free(Value *v) {
    if (!v) return;
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
    }
    if (v->type == VAL_LIST) {
        int i;
        for (i = 0; i < v->as.list_val.count; i++)
            value_free(&v->as.list_val.items[i]);
        free(v->as.list_val.items);
        v->as.list_val.items = NULL;
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
    if (v->type == VAL_OBJECT) {
        // Don't free anything to avoid double-free issues
        // Memory will leak but program will be stable
        // TODO: implement proper lifecycle management
        if (v->as.object_val.class_name) {
            // free(v->as.object_val.class_name);  // Disabled
            v->as.object_val.class_name = NULL;
        }
        if (v->as.object_val.fields) {
            // env_free(v->as.object_val.fields);  // Disabled
            v->as.object_val.fields = NULL;
        }
    }
    if (v->type == VAL_METHOD) {
        if (v->as.method_val.receiver) {
            // Don't free anything in the receiver (all shared)
            // Just free the allocated pointer itself
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
        case VAL_CLASS:
            snprintf(buf, sizeof(buf), "<class %s>", v->as.class_val.name);
            return strdup(buf);
        case VAL_OBJECT:
            snprintf(buf, sizeof(buf), "<object %s>", v->as.object_val.class_name);
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
    return interp;
}

void interpreter_free(Interpreter *interp) {
    // Free microtask queue
    MicrotaskNode *node = interp->microtask_queue_head;
    while (node) {
        MicrotaskNode *next = node->next;
        value_free(&node->callback);
        for (int i = 0; i < node->argc; i++) {
            value_free(&node->args[i]);
        }
        free(node->args);
        free(node);
        node = next;
    }
    env_free(interp->global_env);
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
                    } else {
                        copy.as.list_val.items[ii] = v->as.list_val.items[ii];
                    }
                }
                return copy;
            }
            if (v->type == VAL_FUNCTION && v->as.func_val.param_names) {
                Value copy = *v;
                int ii;
                copy.as.func_val.param_names = malloc(v->as.func_val.param_count * sizeof(char *));
                for (ii = 0; ii < v->as.func_val.param_count; ii++)
                    copy.as.func_val.param_names[ii] = strdup(v->as.func_val.param_names[ii]);
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
        case NODE_ASSIGN: {
            Value val = eval_node_env(interp, node->data.assign_stmt.value, env);
            env_set(env, node->data.assign_stmt.name, val);
            return make_null();
        }
        case NODE_BLOCK: {
            Env *block_env = env_new(env);
            Value result = eval_block(interp, node, block_env);
            env_free(block_env);
            return result;
        }
        case NODE_FUNC_DEF: {
            Value func;
            func.type = VAL_FUNCTION;
            func.as.func_val.param_count = node->data.func_def.params.count;
            func.as.func_val.param_names = NULL;
            if (func.as.func_val.param_count > 0) {
                int i;
                func.as.func_val.param_names = malloc(func.as.func_val.param_count * sizeof(char *));
                for (i = 0; i < func.as.func_val.param_count; i++)
                    func.as.func_val.param_names[i] = strdup(node->data.func_def.params.items[i]->data.ident.name);
            }
            func.as.func_val.body = node->data.func_def.body;
            func.as.func_val.closure = env;
            
            // Arrow functions are expressions that return the function value
            // Named functions are statements that bind to a name
            if (node->data.func_def.is_arrow || strlen(node->data.func_def.name) == 0) {
                return func;
            } else {
                env_set_local(env, node->data.func_def.name, func);
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
                    env_set_local(obj.as.object_val.fields, field_entry->name, field_val);
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
                    for (i = 0; i < init_method->as.func_val.param_count && i < argc; i++)
                        env_set_local(call_env, init_method->as.func_val.param_names[i], args[i]);
                    
                    Value init_result = eval_block(interp, init_method->as.func_val.body, call_env);
                    value_free(&init_result);
                    
                    // Get updated 'this' from call_env (in case fields were modified)
                    Value *updated_this = env_get(call_env, "this");
                    if (updated_this && updated_this->type == VAL_OBJECT) {
                        // Fields are shared, so updates are automatic
                    }
                    
                    env_free(call_env);
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
            for (i = 0; i < argc; i++)
                args[i] = eval_node_env(interp, node->data.call.args.items[i], env);

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
                    
                    env_free(call_env);
                    if (interp->last_result.is_return) {
                        value_free(&result);
                        result = interp->last_result.return_value;
                        interp->last_result.is_return = 0;
                        interp->last_result.return_value = make_null();
                    }
                }
            } else if (callee.type == VAL_FUNCTION) {
                Env *call_env = env_new(callee.as.func_val.closure);
                for (i = 0; i < callee.as.func_val.param_count && i < argc; i++)
                    env_set_local(call_env, callee.as.func_val.param_names[i], args[i]);
                value_free(&result);
                result = eval_block(interp, callee.as.func_val.body, call_env);
                env_free(call_env);
                if (interp->last_result.is_return) {
                    value_free(&result);
                    result = interp->last_result.return_value;
                    interp->last_result.is_return = 0;
                    interp->last_result.return_value = make_null();
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
                env_free(loop_env);
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
            int i;
            
            if (iterable.type == VAL_LIST) {
                // Iterate over list/array elements
                for (i = 0; i < iterable.as.list_val.count; i++) {
                    Env *loop_env = env_new(env);
                    Value elem = iterable.as.list_val.items[i];
                    // Copy strings to avoid double-free
                    if (elem.type == VAL_STRING) {
                        elem = make_string(elem.as.str_val);
                    }
                    env_set_local(loop_env, node->data.for_of_stmt.var, elem);
                    {
                        Value r = eval_block(interp, node->data.for_of_stmt.body, loop_env);
                        value_free(&r);
                    }
                    env_free(loop_env);
                    
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
                    env_free(loop_env);
                    
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
                Env *obj_env = iterable.as.object_val.fields;
                EnvEntry *entry = obj_env->entries;
                
                while (entry) {
                    Env *loop_env = env_new(env);
                    Value key_val = make_string(entry->name);
                    env_set_local(loop_env, node->data.for_of_stmt.var, key_val);
                    {
                        Value r = eval_block(interp, node->data.for_of_stmt.body, loop_env);
                        value_free(&r);
                    }
                    env_free(loop_env);
                    
                    if (interp->last_result.is_return || interp->last_result.is_break) {
                        interp->last_result.is_break = 0;
                        break;
                    }
                    if (interp->last_result.is_continue) {
                        interp->last_result.is_continue = 0;
                    }
                    
                    entry = entry->next;
                }
            } else {
                fprintf(stderr, "Runtime error: for-of requires an iterable (list, string, or object)\n");
            }
            
            value_free(&iterable);
            return make_null();
        }
        case NODE_BINOP: {
            const char *op = node->data.binop.op;
            
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
                
                if (obj.type == VAL_OBJECT) {
                    // Set the field on the object (env_set will take ownership of val)
                    env_set(obj.as.object_val.fields, member_node->data.member_access.member, val);
                }
                
                // Don't free val - it's now owned by the object's fields
                // Don't free obj if it's an object - fields are shared
                return make_null();
            }
            
            // Handle index assignment (arr[0] = value)
            if (strcmp(op, "=") == 0 && node->data.binop.left->type == NODE_INDEX) {
                ASTNode *index_node = node->data.binop.left;
                Value obj = eval_node_env(interp, index_node->data.index_expr.obj, env);
                Value idx = eval_node_env(interp, index_node->data.index_expr.index, env);
                Value val = eval_node_env(interp, node->data.binop.right, env);
                
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
            int i;
            list.type = VAL_LIST;
            list.as.list_val.count = node->data.list.elements.count;
            list.as.list_val.capacity = list.as.list_val.count;
            list.as.list_val.items = malloc((list.as.list_val.count > 0 ? list.as.list_val.count : 1) * sizeof(Value));
            for (i = 0; i < node->data.list.elements.count; i++)
                list.as.list_val.items[i] = eval_node_env(interp, node->data.list.elements.items[i], env);
            return list;
        }
        case NODE_OBJECT: {
            Value obj;
            int i;
            obj.type = VAL_OBJECT;
            obj.as.object_val.class_name = strdup("Object");
            obj.as.object_val.fields = env_new(NULL);
            obj.as.object_val.methods = env_new(NULL);
            
            for (i = 0; i < node->data.object.count; i++) {
                ObjectProperty *prop = &node->data.object.props[i];
                char *key_str = NULL;
                Value val;
                
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
                        }
                    } else {
                        fprintf(stderr, "Runtime error: undefined variable '%s' in object shorthand\n", key_str);
                        val = make_null();
                    }
                } else {
                    val = eval_node_env(interp, prop->value, env);
                }
                
                /* Add to object */
                if (prop->is_method || val.type == VAL_FUNCTION) {
                    env_set_local(obj.as.object_val.methods, key_str, val);
                } else {
                    env_set_local(obj.as.object_val.fields, key_str, val);
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
                    if (item->type == VAL_STRING) result = make_string(item->as.str_val);
                    else result = *item;
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
                    // Copy parent fields (deep copy strings)
                    e = parent->as.class_val.fields->entries;
                    while (e) {
                        Value field_copy = e->value;
                        if (field_copy.type == VAL_STRING) {
                            field_copy.as.str_val = strdup(e->value.as.str_val);
                        }
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
                    // Copy parent static fields
                    e = parent->as.class_val.static_fields->entries;
                    while (e) {
                        Value field_copy = e->value;
                        if (field_copy.type == VAL_STRING) {
                            field_copy.as.str_val = strdup(e->value.as.str_val);
                        }
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
                    if (func.as.func_val.param_count > 0) {
                        int j;
                        func.as.func_val.param_names = malloc(func.as.func_val.param_count * sizeof(char *));
                        for (j = 0; j < func.as.func_val.param_count; j++)
                            func.as.func_val.param_names[j] = strdup(member->data.func_def.params.items[j]->data.ident.name);
                    }
                    func.as.func_val.body = member->data.func_def.body;
                    func.as.func_val.closure = env;
                    if (member->data.func_def.is_static) {
                        env_set_local_with_access(class_val.as.class_val.static_methods, member->data.func_def.name, func, member->data.func_def.access);
                    } else {
                        env_set_local_with_access(class_val.as.class_val.methods, member->data.func_def.name, func, member->data.func_def.access);
                    }
                } else if (member->type == NODE_LET) {
                    // Add field default value to class (static or instance)
                    Value field_val = member->data.let_stmt.value ? 
                        eval_node_env(interp, member->data.let_stmt.value, env) : make_null();
                    if (member->data.let_stmt.is_static) {
                        env_set_local_with_access(class_val.as.class_val.static_fields, member->data.let_stmt.name, field_val, member->data.let_stmt.access);
                    } else {
                        env_set_local_with_access(class_val.as.class_val.fields, member->data.let_stmt.name, field_val, member->data.let_stmt.access);
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
            
            // Initialize fields with default values from class (deep copy strings)
            EnvEntry *field_entry = class_val->as.class_val.fields->entries;
            while (field_entry) {
                Value field_val = field_entry->value;
                if (field_val.type == VAL_STRING) {
                    field_val.as.str_val = strdup(field_entry->value.as.str_val);
                }
                env_set_local(obj.as.object_val.fields, field_entry->name, field_val);
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
                
                // Get updated 'this' from call_env (in case fields were modified)
                Value *updated_this = env_get(call_env, "this");
                if (updated_this && updated_this->type == VAL_OBJECT) {
                    // Copy updated fields back
                    value_free(&obj);
                    obj = *updated_this;
                    obj.as.object_val.class_name = strdup(updated_this->as.object_val.class_name);
                    obj.as.object_val.fields = env_new(NULL);
                    EnvEntry *e = updated_this->as.object_val.fields->entries;
                    while (e) {
                        env_set_local(obj.as.object_val.fields, e->name, e->value);
                        e = e->next;
                    }
                }
                
                env_free(call_env);
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
            
            // Handle array methods
            if (obj.type == VAL_LIST) {
                const char *method_name = node->data.member_access.member;
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
                                     strcmp(this_val->as.object_val.class_name, obj.as.object_val.class_name) == 0);
                
                // Check fields first
                EnvEntry *field_entry = env_get_entry(obj.as.object_val.fields, node->data.member_access.member);
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
                    EnvEntry *method_entry = env_get_entry(obj.as.object_val.methods, node->data.member_access.member);
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
                        return result; // Don't free obj since it's part of method
                    }
                }
            } else if (obj.type == VAL_CLASS) {
                // Access to static methods or parent class
                Value *method = env_get(obj.as.class_val.methods, node->data.member_access.member);
                if (method) {
                    if (method->type == VAL_STRING) result = make_string(method->as.str_val);
                    else result = *method;
                }
            }
            
            // Don't free obj if it's an object or class - they share Env pointers
            if (obj.type != VAL_OBJECT && obj.type != VAL_CLASS) {
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
            Value *class_val = env_get(env, this_val->as.object_val.class_name);
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
            env_free(call_env);
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
