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

void run_ternary_tests(void) {
    printf("--- Ternary Operator Tests ---\n");

    // Test 1: Basic ternary with true condition
    {
        Value v = run_expr("let result = 10 > 5 ? \"big\" : \"small\"\nresult");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "big");
        value_free(&v);
    }

    // Test 2: Basic ternary with false condition
    {
        Value v = run_expr("let result = 3 > 5 ? \"big\" : \"small\"\nresult");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "small");
        value_free(&v);
    }

    // Test 3: Ternary with numbers
    {
        Value v = run_expr("let x = 10\nlet result = x > 5 ? 100 : 50\nresult");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 100);
        value_free(&v);
    }

    // Test 4: Ternary with expressions
    {
        Value v = run_expr("let a = 10\nlet b = 20\nlet max = a > b ? a : b\nmax");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 20);
        value_free(&v);
    }

    // Test 5: Nested ternary (right-associative)
    {
        Value v = run_expr("let score = 85\nlet grade = score >= 90 ? \"A\" : score >= 80 ? \"B\" : \"C\"\ngrade");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "B");
        value_free(&v);
    }

    // Test 6: Nested ternary with score = 95
    {
        Value v = run_expr("let score = 95\nlet grade = score >= 90 ? \"A\" : score >= 80 ? \"B\" : \"C\"\ngrade");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "A");
        value_free(&v);
    }

    // Test 7: Nested ternary with score = 70
    {
        Value v = run_expr("let score = 70\nlet grade = score >= 90 ? \"A\" : score >= 80 ? \"B\" : \"C\"\ngrade");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "C");
        value_free(&v);
    }

    // Test 8: Ternary in function return
    {
        Value v = run_expr("fn abs(x) { return x >= 0 ? x : -x }\nabs(-5)");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 5);
        value_free(&v);
    }

    // Test 9: Ternary in function return (positive)
    {
        Value v = run_expr("fn abs(x) { return x >= 0 ? x : -x }\nabs(7)");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 7);
        value_free(&v);
    }

    // Test 10: Ternary with boolean result
    {
        Value v = run_expr("let x = 5\nlet result = x > 3 ? true : false\nresult");
        ASSERT_EQ(v.type, VAL_BOOL);
        ASSERT_EQ(v.as.bool_val, 1);
        value_free(&v);
    }

    // Test 11: Ternary with complex condition
    {
        Value v = run_expr("let a = 5\nlet b = 10\nlet result = (a + b) > 10 ? \"yes\" : \"no\"\nresult");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "yes");
        value_free(&v);
    }

    // Test 12: Ternary inside arithmetic
    {
        Value v = run_expr("let x = 5\nlet result = 10 + (x > 3 ? 5 : 1)\nresult");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 15);
        value_free(&v);
    }

    // Test 13: Multiple independent ternaries
    {
        Value v = run_expr("let x = 5\nlet y = 3\nlet r1 = x > 3 ? 10 : 20\nlet r2 = y > 3 ? 30 : 40\nr1 + r2");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 50);
        value_free(&v);
    }

    // Test 14: Ternary with equality check
    {
        Value v = run_expr("let x = 5\nlet result = x == 5 ? \"equal\" : \"not equal\"\nresult");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "equal");
        value_free(&v);
    }

    // Test 15: Ternary with inequality check
    {
        Value v = run_expr("let x = 5\nlet result = x != 5 ? \"not equal\" : \"equal\"\nresult");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "equal");
        value_free(&v);
    }

    // Test 16: Ternary with null
    {
        Value v = run_expr("let x = 5\nlet result = x > 10 ? \"big\" : null\nresult == null");
        ASSERT_EQ(v.type, VAL_BOOL);
        ASSERT_EQ(v.as.bool_val, 1);
        value_free(&v);
    }

    // Test 17: Chain of ternaries (deeply nested)
    {
        Value v = run_expr("let n = 3\nlet r = n == 1 ? \"one\" : n == 2 ? \"two\" : n == 3 ? \"three\" : \"other\"\nr");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "three");
        value_free(&v);
    }

    // Test 18: Ternary with function call in condition
    {
        Value v = run_expr("fn isPositive(x) { return x > 0 }\nlet result = isPositive(5) ? \"yes\" : \"no\"\nresult");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "yes");
        value_free(&v);
    }

    // Test 19: Ternary with function calls in branches
    {
        Value v = run_expr("fn double(x) { return x * 2 }\nfn triple(x) { return x * 3 }\nlet x = 5\nlet result = x > 3 ? double(x) : triple(x)\nresult");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 10);
        value_free(&v);
    }

    // Test 20: Ternary with string concatenation
    {
        Value v = run_expr("let name = \"Alice\"\nlet greeting = name == \"Alice\" ? \"Hello, \" : \"Hi, \"\ngreeting");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "Hello, ");
        value_free(&v);
    }

    // Test 21: Ternary as function argument
    {
        Value v = run_expr("fn add(a, b) { return a + b }\nlet x = 5\nlet result = add(10, x > 3 ? 5 : 10)\nresult");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 15);
        value_free(&v);
    }

    // Test 22: Complex nested ternary with arithmetic
    {
        Value v = run_expr("let x = 15\nlet result = x > 20 ? x * 2 : x > 10 ? x + 5 : x - 5\nresult");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 20);
        value_free(&v);
    }

    // Test 23: Ternary with false condition returning expression
    {
        Value v = run_expr("let a = 10\nlet b = 5\nlet result = a < b ? a + b : a - b\nresult");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 5);
        value_free(&v);
    }

    // Test 24: Ternary in while loop
    {
        Value v = run_expr("let i = 0\nlet sum = 0\nwhile (i < 5) {\n  sum = sum + (i > 2 ? 10 : 1)\n  i = i + 1\n}\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 23);  // 1 + 1 + 1 + 10 + 10
        value_free(&v);
    }

    // Test 25: Ternary with list index
    {
        Value v = run_expr("let arr = [10, 20, 30]\nlet idx = 1\nlet result = idx > 0 ? arr[idx] : arr[0]\nresult");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 20);
        value_free(&v);
    }

    printf("Ternary tests completed!\n");
}
