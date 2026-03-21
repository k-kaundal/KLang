#include "test_runner.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

void run_parser_tests(void) {
    printf("--- Parser Tests ---\n");

    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        lexer_init(&lexer, "let x = 42");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        ASSERT_EQ(count, 1);
        ASSERT_EQ(nodes[0]->type, NODE_LET);
        ASSERT_STR_EQ(nodes[0]->data.let_stmt.name, "x");
        ASSERT_EQ(nodes[0]->data.let_stmt.value->type, NODE_NUMBER);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        lexer_init(&lexer, "fn add(a: int, b: int) -> int { return a }");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        ASSERT_EQ(count, 1);
        ASSERT_EQ(nodes[0]->type, NODE_FUNC_DEF);
        ASSERT_STR_EQ(nodes[0]->data.func_def.name, "add");
        ASSERT_EQ(nodes[0]->data.func_def.params.count, 2);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        lexer_init(&lexer, "1 + 2 * 3");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        ASSERT_EQ(count, 1);
        ASSERT_EQ(nodes[0]->type, NODE_BINOP);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        lexer_init(&lexer, "if x > 0 { let y = 1 }");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        ASSERT_EQ(count, 1);
        ASSERT_EQ(nodes[0]->type, NODE_IF);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    printf("Parser tests done.\n");
}
