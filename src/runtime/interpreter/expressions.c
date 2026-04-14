#include "interpreter_internal.h"

Value eval_number(Interpreter *interp, ASTNode *node, Env *env) {
    (void)interp; (void)env;  /* Unused parameters */
    double v = node->data.num_val;
    if (fmod(v, 1.0) == 0.0 && v >= -9.2e18 && v <= 9.2e18)
        return make_int((long long)v);
    return make_float(v);
}

Value eval_string(Interpreter *interp, ASTNode *node, Env *env) {
    (void)interp; (void)env;  /* Unused parameters */
    return make_string(node->data.str_val);
}

Value eval_bool(Interpreter *interp, ASTNode *node, Env *env) {
    (void)interp; (void)env;  /* Unused parameters */
    return make_bool(node->data.bool_val);
}

Value eval_null(Interpreter *interp, ASTNode *node, Env *env) {
    (void)interp; (void)node; (void)env;  /* Unused parameters */
    return make_null();
}

Value eval_ident(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_binop(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_unop(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_list(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_tuple(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_object(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_index(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_call(Interpreter *interp, ASTNode *node, Env *env) {
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
    } else if (callee.type == VAL_CLASS) {
        // Direct class instantiation: Test() instead of new Test()
        // Check if class is abstract
        if (callee.as.class_val.is_abstract) {
            fprintf(stderr, "Error: cannot instantiate abstract class '%s'\n",
                    callee.as.class_val.name);
            interp->had_error = 1;
            value_free(&result);
            result = make_null();
        } else {
            // Create object
            Value obj = make_object(callee.as.class_val.name, callee.as.class_val.methods);
            
            // Initialize fields with default values from class
            EnvEntry *field_entry = callee.as.class_val.fields->entries;
            while (field_entry) {
                Value field_val = field_entry->value;
                // Don't duplicate here - env_set_local will handle it
                env_set_local(obj.as.object_val->fields, field_entry->name, field_val);
                field_entry = field_entry->next;
            }
            
            // Call constructor or init method if it exists
            Value *ctor_method = env_get(callee.as.class_val.methods, "constructor");
            if (!ctor_method || ctor_method->type != VAL_FUNCTION) {
                // Fallback to "init" for backward compatibility
                ctor_method = env_get(callee.as.class_val.methods, "init");
            }
            if (ctor_method && ctor_method->type == VAL_FUNCTION) {
                // Create call environment with 'this' bound
                Env *call_env = env_new(ctor_method->as.func_val.closure);
                env_set_local(call_env, "this", obj);
                
                /* Handle rest parameters */
                if (ctor_method->as.func_val.has_rest_param && ctor_method->as.func_val.param_count > 0) {
                    /* Bind regular params */
                    for (i = 0; i < ctor_method->as.func_val.param_count - 1 && i < argc; i++)
                        env_set_local(call_env, ctor_method->as.func_val.param_names[i], args[i]);
                    
                    /* Collect rest args into array */
                    Value rest_array;
                    int rest_count = argc - (ctor_method->as.func_val.param_count - 1);
                    if (rest_count < 0) rest_count = 0;
                    rest_array.type = VAL_LIST;
                    rest_array.as.list_val.count = rest_count;
                    rest_array.as.list_val.capacity = rest_count;
                    rest_array.as.list_val.items = malloc((rest_count > 0 ? rest_count : 1) * sizeof(Value));
                    for (i = 0; i < rest_count; i++) {
                        int arg_idx = ctor_method->as.func_val.param_count - 1 + i;
                        rest_array.as.list_val.items[i] = args[arg_idx];
                    }
                    env_set_local(call_env, ctor_method->as.func_val.param_names[ctor_method->as.func_val.param_count - 1], rest_array);
                } else {
                    for (i = 0; i < ctor_method->as.func_val.param_count && i < argc; i++)
                        env_set_local(call_env, ctor_method->as.func_val.param_names[i], args[i]);
                }
                
                Value init_result = eval_block(interp, ctor_method->as.func_val.body, call_env);
                value_free(&init_result);
                
                env_release(call_env);
                
                if (interp->last_result.is_return) {
                    value_free(&interp->last_result.return_value);
                    interp->last_result.is_return = 0;
                }
            }
            
            value_free(&result);
            result = obj;
        }
    }

    for (i = 0; i < argc; i++) value_free(&args[i]);
    free(args);
    value_free(&callee);
    return result;
}

Value eval_member_access(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_this(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_super(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_template_literal(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_ternary(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_await(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_yield(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_postfix(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_optional_chain(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_nullish_coalesce(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_address_of(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_dereference(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_pointer_member(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_struct_literal(Interpreter *interp, ASTNode *node, Env *env) {
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

Value eval_new(Interpreter *interp, ASTNode *node, Env *env) {
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
    
    // Call constructor or init method if it exists
    Value *ctor_method = env_get(class_val->as.class_val.methods, "constructor");
    if (!ctor_method || ctor_method->type != VAL_FUNCTION) {
        // Fallback to "init" for backward compatibility
        ctor_method = env_get(class_val->as.class_val.methods, "init");
    }
    if (ctor_method && ctor_method->type == VAL_FUNCTION) {
        int argc = node->data.new_expr.args.count;
        Value *args = malloc((argc > 0 ? argc : 1) * sizeof(Value));
        int i;
        for (i = 0; i < argc; i++)
            args[i] = eval_node_env(interp, node->data.new_expr.args.items[i], env);
        
        // Create call environment with 'this' bound
        Env *call_env = env_new(ctor_method->as.func_val.closure);
        env_set_local(call_env, "this", obj);
        for (i = 0; i < ctor_method->as.func_val.param_count && i < argc; i++)
            env_set_local(call_env, ctor_method->as.func_val.param_names[i], args[i]);
        
        Value ctor_result = eval_block(interp, ctor_method->as.func_val.body, call_env);
        value_free(&ctor_result);
        
        
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
