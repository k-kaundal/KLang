#ifndef COMPILER_H
#define COMPILER_H

#include "core/ast/ast.h"
#include "vm_stack.h"

typedef struct {
    Chunk *chunk;
    int had_error;
} Compiler;

Compiler *compiler_new(void);
void compiler_free(Compiler *comp);
Chunk *compile_program(Compiler *comp, ASTNode **nodes, int count);
void compile_node(Compiler *comp, ASTNode *node);

#endif
