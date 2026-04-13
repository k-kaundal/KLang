#include "compiler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Compiler *compiler_new(void) {
    Compiler *c = calloc(1, sizeof(Compiler));
    c->chunk = chunk_new();
    return c;
}

void compiler_free(Compiler *comp) {
    if (comp->chunk) chunk_free(comp->chunk);
    free(comp);
}

void compile_node(Compiler *comp, ASTNode *node) {
    if (!node) return;
    switch (node->type) {
        case NODE_NUMBER:
            chunk_emit_int(comp->chunk, OP_PUSH_INT, (long long)node->data.num_val);
            break;
        case NODE_BINOP: {
            const char *op = node->data.binop.op;
            compile_node(comp, node->data.binop.left);
            compile_node(comp, node->data.binop.right);
            if (strcmp(op, "+") == 0) chunk_emit_op(comp->chunk, OP_ADD);
            else if (strcmp(op, "-") == 0) chunk_emit_op(comp->chunk, OP_SUB);
            else if (strcmp(op, "*") == 0) chunk_emit_op(comp->chunk, OP_MUL);
            else if (strcmp(op, "/") == 0) chunk_emit_op(comp->chunk, OP_DIV);
            break;
        }
        case NODE_CALL: {
            if (node->data.call.callee->type == NODE_IDENT &&
                strcmp(node->data.call.callee->data.ident.name, "print") == 0 &&
                node->data.call.args.count > 0) {
                compile_node(comp, node->data.call.args.items[0]);
                chunk_emit_op(comp->chunk, OP_PRINT);
            }
            break;
        }
        default:
            break;
    }
}

Chunk *compile_program(Compiler *comp, ASTNode **nodes, int count) {
    Chunk *result;
    int i;
    for (i = 0; i < count; i++)
        compile_node(comp, nodes[i]);
    chunk_emit_op(comp->chunk, OP_HALT);
    result = comp->chunk;
    comp->chunk = chunk_new();
    return result;
}
