#include "interpreter_internal.h"

Value eval_let(Interpreter *interp, ASTNode *node, Env *env) {
    Value val = make_null();
    /* Evaluate initial value if provided */
    if (node->data.let_stmt.value) {
        val = eval_node_env(interp, node->data.let_stmt.value, env);
    }
    /* Use env_declare to enforce const/let/var semantics */
    env_declare(env, node->data.let_stmt.name, val, node->data.let_stmt.decl_type, node->line, interp);
    return make_null();
}

Value eval_destructure_array(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_destructure_object(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_assign(Interpreter *interp, ASTNode *node, Env *env) {
    Value val = eval_node_env(interp, node->data.assign_stmt.value, env);
    env_set(env, node->data.assign_stmt.name, val);
    return make_null();
}

Value eval_block_stmt(Interpreter *interp, ASTNode *node, Env *env) {
    Env *block_env = env_new(env);
    Value result = eval_block(interp, node, block_env);
    env_release(block_env);
    return result;
}

Value eval_func_def(Interpreter *interp, ASTNode *node, Env *env) {
    (void)interp;  /* Unused parameter */
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

Value eval_return(Interpreter *interp, ASTNode *node, Env *env) {
    Value val = node->data.return_stmt.value ?
        eval_node_env(interp, node->data.return_stmt.value, env) : make_null();
    interp->last_result.is_return = 1;
    interp->last_result.return_value = val;
    return make_null();
}

Value eval_break(Interpreter *interp, ASTNode *node, Env *env) {
    (void)node; (void)env;  /* Unused parameters */
    interp->last_result.is_break = 1;
    return make_null();
}

Value eval_continue(Interpreter *interp, ASTNode *node, Env *env) {
    (void)node; (void)env;  /* Unused parameters */
    interp->last_result.is_continue = 1;
    return make_null();
}

Value eval_if(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_while(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_for(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_for_of(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_for_c_style(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_class_def(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_import_named(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_import_default(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_import_namespace(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_export(Interpreter *interp, ASTNode *node, Env *env) {
    /* Exports are handled during module loading, not during normal execution */
    /* If we encounter this during normal (non-module) execution, it's an error */
    if (node->data.export_stmt.declaration) {
        /* Execute the declaration in the current environment */
        return eval_node_env(interp, node->data.export_stmt.declaration, env);
    }
    return make_null();
}

Value eval_struct_def(Interpreter *interp, ASTNode *node, Env *env) {
    (void)interp;  /* Unused parameter */
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

/* Security: Evaluate unsafe block
 * Phase 2: Basic evaluation support for unsafe blocks
 * 
 * Unsafe blocks allow potentially dangerous operations that would normally
 * be restricted. In this phase, we simply evaluate the statements inside
 * the unsafe block. Future phases will add runtime security context tracking.
 * 
 * The unsafe block creates its own scope (like a regular block) and evaluates
 * all statements within it. This maintains proper scoping while marking the
 * code region as potentially unsafe.
 */
Value eval_unsafe_block(Interpreter *interp, ASTNode *node, Env *env) {
    /* Create a new environment for the unsafe block scope */
    Env *unsafe_env = env_new(env);
    Value result = make_null();
    int i;
    
    /* Evaluate each statement in the unsafe block */
    for (i = 0; i < node->data.unsafe_block.stmts.count; i++) {
        value_free(&result);
        result = eval_node_env(interp, node->data.unsafe_block.stmts.items[i], unsafe_env);
        
        /* Handle control flow: return, break, continue */
        if (interp->last_result.is_return || 
            interp->last_result.is_break || 
            interp->last_result.is_continue) {
            break;
        }
    }
    
    /* Release the unsafe block environment */
    env_release(unsafe_env);
    return result;
}
