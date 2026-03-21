#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer *lexer;
    Token current;
    Token peek;
    int had_error;
} Parser;

void parser_init(Parser *parser, Lexer *lexer);
void parser_free(Parser *parser);
ASTNode **parse_program(Parser *parser, int *count);

#endif
