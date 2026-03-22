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

void run_async_await_tests(void) {
    printf("--- Async/Await Tests ---\n");

    // Test 1: Async function returns Promise
    {
        Value v = run_expr("async fn getValue() { return 42 }\ngetValue()");
        ASSERT_EQ(v.type, VAL_PROMISE);
        ASSERT_EQ(v.as.promise_val.state, PROMISE_FULFILLED);
        value_free(&v);
    }

    // Test 2: Await on Promise
    {
        Value v = run_expr(
            "async fn getValue() { return 42 }\n"
            "async fn test() { const value = await getValue()\nreturn value }\n"
            "test()");
        ASSERT_EQ(v.type, VAL_PROMISE);
        ASSERT_EQ(v.as.promise_val.state, PROMISE_FULFILLED);
        ASSERT_EQ(v.as.promise_val.result->type, VAL_INT);
        ASSERT_EQ(v.as.promise_val.result->as.int_val, 42);
        value_free(&v);
    }

    // Test 3: Await on non-Promise value
    {
        Value v = run_expr(
            "async fn test() { const value = await 100\nreturn value }\n"
            "test()");
        ASSERT_EQ(v.type, VAL_PROMISE);
        ASSERT_EQ(v.as.promise_val.state, PROMISE_FULFILLED);
        ASSERT_EQ(v.as.promise_val.result->type, VAL_INT);
        ASSERT_EQ(v.as.promise_val.result->as.int_val, 100);
        value_free(&v);
    }

    // Test 4: Async arrow function
    {
        Value v = run_expr("const f = async () => { return 99 }\nf()");
        ASSERT_EQ(v.type, VAL_PROMISE);
        ASSERT_EQ(v.as.promise_val.state, PROMISE_FULFILLED);
        ASSERT_EQ(v.as.promise_val.result->type, VAL_INT);
        ASSERT_EQ(v.as.promise_val.result->as.int_val, 99);
        value_free(&v);
    }

    // Test 5: Sequential awaits
    {
        Value v = run_expr(
            "async fn getNum(n) { return n }\n"
            "async fn sum() {\n"
            "    const a = await getNum(10)\n"
            "    const b = await getNum(20)\n"
            "    return a + b\n"
            "}\n"
            "sum()");
        ASSERT_EQ(v.type, VAL_PROMISE);
        ASSERT_EQ(v.as.promise_val.state, PROMISE_FULFILLED);
        ASSERT_EQ(v.as.promise_val.result->type, VAL_INT);
        ASSERT_EQ(v.as.promise_val.result->as.int_val, 30);
        value_free(&v);
    }

    // Test 6: Async with .then()
    {
        Value v = run_expr(
            "async fn getValue() { return 55 }\n"
            "let result = 0\n"
            "getValue().then(v => { result = v })\n"
            "result");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 55);
        value_free(&v);
    }

    // Test 7: Async function with string return
    {
        Value v = run_expr("async fn greet() { return \"hello\" }\ngreet()");
        ASSERT_EQ(v.type, VAL_PROMISE);
        ASSERT_EQ(v.as.promise_val.state, PROMISE_FULFILLED);
        ASSERT_EQ(v.as.promise_val.result->type, VAL_STRING);
        ASSERT_STR_EQ(v.as.promise_val.result->as.str_val, "hello");
        value_free(&v);
    }

    // Test 8: Async with Promise.resolve
    {
        Value v = run_expr(
            "async fn test() { const value = await Promise.resolve(777)\nreturn value }\n"
            "test()");
        ASSERT_EQ(v.type, VAL_PROMISE);
        ASSERT_EQ(v.as.promise_val.state, PROMISE_FULFILLED);
        ASSERT_EQ(v.as.promise_val.result->type, VAL_INT);
        ASSERT_EQ(v.as.promise_val.result->as.int_val, 777);
        value_free(&v);
    }

    printf("All async/await tests passed!\n\n");
}
