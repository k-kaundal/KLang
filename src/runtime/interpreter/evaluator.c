#include "interpreter_internal.h"

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

Value eval_node_env(Interpreter *interp, ASTNode *node, Env *env) {
    if (!node) return make_null();

    switch (node->type) {
        /* Expression nodes - delegated to expressions.c */
        case NODE_NUMBER:
            return eval_number(interp, node, env);
        case NODE_STRING:
            return eval_string(interp, node, env);
        case NODE_BOOL:
            return eval_bool(interp, node, env);
        case NODE_NULL:
            return eval_null(interp, node, env);
        case NODE_IDENT:
            return eval_ident(interp, node, env);
        case NODE_BINOP:
            return eval_binop(interp, node, env);
        case NODE_UNOP:
            return eval_unop(interp, node, env);
        case NODE_LIST:
            return eval_list(interp, node, env);
        case NODE_TUPLE:
            return eval_tuple(interp, node, env);
        case NODE_OBJECT:
            return eval_object(interp, node, env);
        case NODE_INDEX:
            return eval_index(interp, node, env);
        case NODE_CALL:
            return eval_call(interp, node, env);
        case NODE_MEMBER_ACCESS:
            return eval_member_access(interp, node, env);
        case NODE_THIS:
            return eval_this(interp, node, env);
        case NODE_SUPER:
            return eval_super(interp, node, env);
        case NODE_TEMPLATE_LITERAL:
            return eval_template_literal(interp, node, env);
        case NODE_TERNARY:
            return eval_ternary(interp, node, env);
        case NODE_AWAIT:
            return eval_await(interp, node, env);
        case NODE_YIELD:
            return eval_yield(interp, node, env);
        case NODE_POSTFIX:
            return eval_postfix(interp, node, env);
        case NODE_OPTIONAL_CHAIN:
            return eval_optional_chain(interp, node, env);
        case NODE_NULLISH_COALESCE:
            return eval_nullish_coalesce(interp, node, env);
        case NODE_ADDRESS_OF:
            return eval_address_of(interp, node, env);
        case NODE_DEREFERENCE:
            return eval_dereference(interp, node, env);
        case NODE_POINTER_MEMBER:
            return eval_pointer_member(interp, node, env);
        case NODE_STRUCT_LITERAL:
            return eval_struct_literal(interp, node, env);
        case NODE_NEW:
            return eval_new(interp, node, env);

        /* Statement nodes - delegated to statements.c */
        case NODE_LET:
            return eval_let(interp, node, env);
        case NODE_DESTRUCTURE_ARRAY:
            return eval_destructure_array(interp, node, env);
        case NODE_DESTRUCTURE_OBJECT:
            return eval_destructure_object(interp, node, env);
        case NODE_ASSIGN:
            return eval_assign(interp, node, env);
        case NODE_BLOCK:
            return eval_block_stmt(interp, node, env);
        case NODE_FUNC_DEF:
            return eval_func_def(interp, node, env);
        case NODE_RETURN:
            return eval_return(interp, node, env);
        case NODE_BREAK:
            return eval_break(interp, node, env);
        case NODE_CONTINUE:
            return eval_continue(interp, node, env);
        case NODE_IF:
            return eval_if(interp, node, env);
        case NODE_WHILE:
            return eval_while(interp, node, env);
        case NODE_FOR:
            return eval_for(interp, node, env);
        case NODE_FOR_OF:
            return eval_for_of(interp, node, env);
        case NODE_FOR_C_STYLE:
            return eval_for_c_style(interp, node, env);
        case NODE_CLASS_DEF:
            return eval_class_def(interp, node, env);
        case NODE_IMPORT_NAMED:
            return eval_import_named(interp, node, env);
        case NODE_IMPORT_DEFAULT:
            return eval_import_default(interp, node, env);
        case NODE_IMPORT_NAMESPACE:
            return eval_import_namespace(interp, node, env);
        case NODE_EXPORT:
            return eval_export(interp, node, env);
        case NODE_STRUCT_DEF:
        case NODE_UNION_DEF:
            return eval_struct_def(interp, node, env);
        
        /* Security: Unsafe block evaluation */
        case NODE_UNSAFE_BLOCK:
            return eval_unsafe_block(interp, node, env);

        default:
            fprintf(stderr, "Error: unknown node type %d\n", node->type);
            interp->had_error = 1;
            return make_null();
    }
}

Value eval_node(Interpreter *interp, ASTNode *node) {
    return eval_node_env(interp, node, interp->global_env);
}
