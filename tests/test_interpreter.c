#include "test_runner.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "runtime.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static Value run_expr(const char *src) {
    Lexer lexer;
    Parser parser;
    int count = 0;
    ASTNode **nodes;
    Interpreter *interp;
    Value result = make_null();
    int i;

    lexer_init(&lexer, src);
    parser_init(&parser, &lexer);
    nodes = parse_program(&parser, &count);
    interp = interpreter_new();
    runtime_init(interp);

    for (i = 0; i < count; i++) {
        value_free(&result);
        result = eval_node(interp, nodes[i]);
    }
    interpreter_free(interp);
    for (i = 0; i < count; i++) ast_free(nodes[i]);
    free(nodes);
    parser_free(&parser);
    lexer_free(&lexer);
    return result;
}

void run_interpreter_tests(void) {
    printf("--- Interpreter Tests ---\n");

    {
        Value v = run_expr("2 + 3");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 5);
        value_free(&v);
    }

    {
        Value v = run_expr("10 - 4");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 6);
        value_free(&v);
    }

    {
        Value v = run_expr("3 * 4");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 12);
        value_free(&v);
    }

    {
        Value v = run_expr("10 / 2");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 5);
        value_free(&v);
    }

    {
        Value v = run_expr("5 > 3");
        ASSERT_EQ(v.type, VAL_BOOL);
        ASSERT_EQ(v.as.bool_val, 1);
        value_free(&v);
    }

    {
        Value v = run_expr("2 == 2");
        ASSERT_EQ(v.type, VAL_BOOL);
        ASSERT_EQ(v.as.bool_val, 1);
        value_free(&v);
    }

    {
        Value v = run_expr("let x = 42\nx");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 42);
        value_free(&v);
    }

    {
        Value v = run_expr("\"hello\" + \" world\"");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "hello world");
        value_free(&v);
    }

    {
        Value v = run_expr("fn add(a: int, b: int) -> int { return a + b }\nadd(3, 4)");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 7);
        value_free(&v);
    }

    {
        Value v = run_expr("true");
        ASSERT_EQ(v.type, VAL_BOOL);
        ASSERT_EQ(v.as.bool_val, 1);
        value_free(&v);
    }

    {
        Value v = run_expr("let x = 0\nwhile x < 3 { x = x + 1 }\nx");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 3);
        value_free(&v);
    }

    {
        Value v = run_expr("let sum = 0\nfor i in 0 .. 4 { sum = sum + i }\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        /* upper bound is exclusive, so 0 + 1 + 2 + 3 */
        ASSERT_EQ(v.as.int_val, 6);
        value_free(&v);
    }

    {
        Value v = run_expr("let x = 5 # comment ignored by lexer\nx");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 5);
        value_free(&v);
    }

    {
        Value v = run_expr("type(1)");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "int");
        value_free(&v);
    }

    printf("Interpreter tests done.\n");
}
