#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

static Value eval_block(Interpreter *interp, ASTNode *block, Env *env);
static Value eval_node_env(Interpreter *interp, ASTNode *node, Env *env);

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

void env_set_local(Env *env, const char *name, Value val) {
    EnvEntry *e = env->entries;
    while (e) {
        if (strcmp(e->name, name) == 0) {
            value_free(&e->value);
            e->value = val;
            return;
        }
        e = e->next;
    }
    {
        EnvEntry *ne = malloc(sizeof(EnvEntry));
        ne->name = strdup(name);
        ne->value = val;
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
        case VAL_LIST:
            return strdup("<list>");
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
    return interp;
}

void interpreter_free(Interpreter *interp) {
    env_free(interp->global_env);
    free(interp);
}

static Value eval_block(Interpreter *interp, ASTNode *block, Env *env) {
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
            Value val = eval_node_env(interp, node->data.let_stmt.value, env);
            env_set_local(env, node->data.let_stmt.name, val);
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
            env_set_local(env, node->data.func_def.name, func);
            return make_null();
        }
        case NODE_CALL: {
            Value callee = eval_node_env(interp, node->data.call.callee, env);
            int argc = node->data.call.args.count;
            Value *args = malloc((argc > 0 ? argc : 1) * sizeof(Value));
            Value result = make_null();
            int i;
            for (i = 0; i < argc; i++)
                args[i] = eval_node_env(interp, node->data.call.args.items[i], env);

            if (callee.type == VAL_BUILTIN) {
                result = callee.as.builtin(interp, args, argc);
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
        case NODE_BINOP: {
            const char *op = node->data.binop.op;
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
        default:
            return make_null();
    }
}

Value eval_node(Interpreter *interp, ASTNode *node) {
    return eval_node_env(interp, node, interp->global_env);
}
