#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../src/lexer.h"
#include "../src/parser.h"
#include "../src/interpreter.h"

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        fprintf(stderr, "FAIL at line %d: expected %lld, got %lld\n", __LINE__, (long long)(b), (long long)(a)); \
        return 1; \
    } \
} while (0)

#define ASSERT_STR_EQ(a, b) do { \
    if (strcmp((a), (b)) != 0) { \
        fprintf(stderr, "FAIL at line %d: expected \"%s\", got \"%s\"\n", __LINE__, (b), (a)); \
        return 1; \
    } \
} while (0)

#define ASSERT_TRUE(a) ASSERT_EQ((a), 1)
#define ASSERT_FALSE(a) ASSERT_EQ((a), 0)

// Helper to run KLang code and return the result
Value run_expr(const char *code) {
    Lexer lexer;
    Parser parser;
    Interpreter *interp;
    ASTNode **statements;
    int count;
    Value result = make_null();
    
    lexer_init(&lexer, code);
    parser_init(&parser, &lexer);
    statements = parse_program(&parser, &count);
    interp = interpreter_new();
    
    for (int i = 0; i < count; i++) {
        value_free(&result);
        result = eval_node(interp, statements[i]);
    }
    
    for (int i = 0; i < count; i++) {
        ast_free(statements[i]);
    }
    free(statements);
    interpreter_free(interp);
    parser_free(&parser);
    lexer_free(&lexer);
    
    return result;
}

int main(void) {
    printf("Testing for-of loops...\n");
    
    // Test 1: Basic for-of with array
    printf("Test 1: Basic for-of with array\n");
    {
        Value v = run_expr("let sum = 0\nlet arr = [1, 2, 3, 4, 5]\nfor (item of arr) { sum = sum + item }\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 15);
        value_free(&v);
    }
    
    // Test 2: for-of with const declaration
    printf("Test 2: for-of with const declaration\n");
    {
        Value v = run_expr("let sum = 0\nconst arr = [10, 20, 30]\nfor (const num of arr) { sum = sum + num }\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 60);
        value_free(&v);
    }
    
    // Test 3: for-of with let declaration
    printf("Test 3: for-of with let declaration\n");
    {
        Value v = run_expr("let product = 1\nlet arr = [2, 3, 4]\nfor (let x of arr) { product = product * x }\nproduct");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 24);
        value_free(&v);
    }
    
    // Test 4: for-of with var declaration
    printf("Test 4: for-of with var declaration\n");
    {
        Value v = run_expr("var sum = 0\nvar arr = [5, 10, 15]\nfor (var n of arr) { sum = sum + n }\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 30);
        value_free(&v);
    }
    
    // Test 5: for-of with string (character iteration)
    printf("Test 5: for-of with string (character iteration)\n");
    {
        Value v = run_expr("let result = \"\"\nfor (const char of \"hello\") { result = result + char }\nresult");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "hello");
        value_free(&v);
    }
    
    // Test 6: for-of with string counting specific character
    printf("Test 6: for-of with string counting specific character\n");
    {
        Value v = run_expr("let count = 0\nfor (const c of \"hello\") { if (c == \"l\") { count = count + 1 } }\ncount");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 2);
        value_free(&v);
    }
    
    // Test 7: for-of with break
    printf("Test 7: for-of with break\n");
    {
        Value v = run_expr("let sum = 0\nlet arr = [1, 2, 3, 4, 5]\nfor (const n of arr) { if (n == 4) { break } sum = sum + n }\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 6);  // 1 + 2 + 3
        value_free(&v);
    }
    
    // Test 8: for-of with continue
    printf("Test 8: for-of with continue\n");
    {
        Value v = run_expr("let sum = 0\nlet arr = [1, 2, 3, 4, 5]\nfor (const n of arr) { if (n % 2 == 0) { continue } sum = sum + n }\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 9);  // 1 + 3 + 5
        value_free(&v);
    }
    
    // Test 9: Nested for-of loops
    printf("Test 9: Nested for-of loops\n");
    {
        Value v = run_expr(
            "let sum = 0\n"
            "let matrix = [[1, 2], [3, 4], [5, 6]]\n"
            "for (const row of matrix) {\n"
            "    for (const item of row) {\n"
            "        sum = sum + item\n"
            "    }\n"
            "}\n"
            "sum"
        );
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 21);  // 1+2+3+4+5+6
        value_free(&v);
    }
    
    // Test 10: for-of with empty array
    printf("Test 10: for-of with empty array\n");
    {
        Value v = run_expr("let count = 0\nlet arr = []\nfor (const item of arr) { count = count + 1 }\ncount");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 0);
        value_free(&v);
    }
    
    // Test 11: for-of with empty string
    printf("Test 11: for-of with empty string\n");
    {
        Value v = run_expr("let count = 0\nfor (const c of \"\") { count = count + 1 }\ncount");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 0);
        value_free(&v);
    }
    
    // Test 12: for-of with single element array
    printf("Test 12: for-of with single element array\n");
    {
        Value v = run_expr("let result = 0\nfor (const x of [42]) { result = x }\nresult");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 42);
        value_free(&v);
    }
    
    // Test 13: for-of iterating over array of strings
    printf("Test 13: for-of iterating over array of strings\n");
    {
        Value v = run_expr("let result = \"\"\nlet words = [\"hello\", \" \", \"world\"]\nfor (const word of words) { result = result + word }\nresult");
        ASSERT_EQ(v.type, VAL_STRING);
        ASSERT_STR_EQ(v.as.str_val, "hello world");
        value_free(&v);
    }
    
    // Test 14: for-of with mixed type array
    printf("Test 14: for-of with mixed type array\n");
    {
        Value v = run_expr("let count = 0\nlet arr = [1, \"hello\", true, 3.14]\nfor (const item of arr) { count = count + 1 }\ncount");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 4);
        value_free(&v);
    }
    
    // Test 15: for-of break in nested loop (inner loop)
    printf("Test 15: for-of break in nested loop (inner loop)\n");
    {
        Value v = run_expr(
            "let sum = 0\n"
            "let matrix = [[1, 2, 3], [4, 5, 6]]\n"
            "for (const row of matrix) {\n"
            "    for (const item of row) {\n"
            "        if (item == 3) { break }\n"
            "        sum = sum + item\n"
            "    }\n"
            "}\n"
            "sum"
        );
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 13);  // 1+2 from first row, 4+5 from second row (3 breaks inner loop)
        value_free(&v);
    }
    
    // Test 16: for-of with object (iterating keys)
    printf("Test 16: for-of with object (iterating keys)\n");
    {
        Value v = run_expr("let count = 0\nlet obj = {a: 1, b: 2, c: 3}\nfor (const key of obj) { count = count + 1 }\ncount");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 3);
        value_free(&v);
    }
    
    // Test 17: for-of accumulating values from array
    printf("Test 17: for-of accumulating values from array\n");
    {
        Value v = run_expr("let max = 0\nlet arr = [3, 7, 2, 9, 1]\nfor (const n of arr) { if (n > max) { max = n } }\nmax");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 9);
        value_free(&v);
    }
    
    // Test 18: for-of with array of boolean values
    printf("Test 18: for-of with array of boolean values\n");
    {
        Value v = run_expr("let count = 0\nlet arr = [true, false, true, true]\nfor (const b of arr) { if (b) { count = count + 1 } }\ncount");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 3);
        value_free(&v);
    }
    
    // Test 19: for-of modifying external variable
    printf("Test 19: for-of modifying external variable\n");
    {
        Value v = run_expr("let result = []\nlet arr = [1, 2, 3]\nfor (const x of arr) { result = [x * 2] }\nresult");
        ASSERT_EQ(v.type, VAL_LIST);
        ASSERT_EQ(v.as.list_val.count, 1);
        ASSERT_EQ(v.as.list_val.items[0].type, VAL_INT);
        ASSERT_EQ(v.as.list_val.items[0].as.int_val, 6);  // Last iteration: 3 * 2
        value_free(&v);
    }
    
    // Test 20: for-of with return in function
    printf("Test 20: for-of with return in function\n");
    {
        Value v = run_expr(
            "fn findFirst(arr, target) {\n"
            "    for (const item of arr) {\n"
            "        if (item == target) { return item }\n"
            "    }\n"
            "    return null\n"
            "}\n"
            "findFirst([10, 20, 30, 40], 30)"
        );
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 30);
        value_free(&v);
    }
    
    printf("\nAll for-of tests passed!\n");
    return 0;
}
