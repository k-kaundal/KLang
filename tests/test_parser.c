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

    /* Test class definition parsing */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        lexer_init(&lexer, "class Point { let x: int = 0 }");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        ASSERT_EQ(count, 1);
        ASSERT_EQ(nodes[0]->type, NODE_CLASS_DEF);
        ASSERT_STR_EQ(nodes[0]->data.class_def.name, "Point");
        ASSERT_EQ(nodes[0]->data.class_def.members.count, 1);
        ASSERT_EQ(nodes[0]->data.class_def.members.items[0]->type, NODE_LET);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test class inheritance parsing */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        lexer_init(&lexer, "class Point3D extends Point { }");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        ASSERT_EQ(count, 1);
        ASSERT_EQ(nodes[0]->type, NODE_CLASS_DEF);
        ASSERT_STR_EQ(nodes[0]->data.class_def.name, "Point3D");
        ASSERT_STR_EQ(nodes[0]->data.class_def.parent_name, "Point");
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test new expression parsing */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        lexer_init(&lexer, "let p = new Point(3, 4)");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        ASSERT_EQ(count, 1);
        ASSERT_EQ(nodes[0]->type, NODE_LET);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->type, NODE_CALL);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->data.call.callee->type, NODE_NEW);
        ASSERT_STR_EQ(nodes[0]->data.let_stmt.value->data.call.callee->data.new_expr.class_name, "Point");
        ASSERT_EQ(nodes[0]->data.let_stmt.value->data.call.args.count, 2);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test this keyword parsing */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        lexer_init(&lexer, "this");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        ASSERT_EQ(count, 1);
        ASSERT_EQ(nodes[0]->type, NODE_THIS);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test member access parsing */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        lexer_init(&lexer, "this.x");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        ASSERT_EQ(count, 1);
        ASSERT_EQ(nodes[0]->type, NODE_MEMBER_ACCESS);
        ASSERT_STR_EQ(nodes[0]->data.member_access.member, "x");
        ASSERT_EQ(nodes[0]->data.member_access.obj->type, NODE_THIS);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test super keyword parsing */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        lexer_init(&lexer, "super.method()");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        ASSERT_EQ(count, 1);
        ASSERT_EQ(nodes[0]->type, NODE_CALL);
        ASSERT_EQ(nodes[0]->data.call.callee->type, NODE_SUPER);
        ASSERT_STR_EQ(nodes[0]->data.call.callee->data.super_expr.member, "method");
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test member assignment parsing */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        lexer_init(&lexer, "this.x = 42");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        ASSERT_EQ(count, 1);
        ASSERT_EQ(nodes[0]->type, NODE_BINOP);
        ASSERT_STR_EQ(nodes[0]->data.binop.op, "=");
        ASSERT_EQ(nodes[0]->data.binop.left->type, NODE_MEMBER_ACCESS);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    printf("Parser tests done.\n");
}
