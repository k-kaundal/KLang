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
    printf("Testing C-style for loops...\n");
    
    // Test 1: Basic C-style for loop with let
    printf("Test 1: Basic C-style for loop with let\n");
    {
        Value v = run_expr("let sum = 0\nfor (let i = 0; i < 5; i = i + 1) { sum = sum + i }\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 10);  // 0+1+2+3+4 = 10
        value_free(&v);
    }
    
    // Test 2: C-style for loop with var
    printf("Test 2: C-style for loop with var\n");
    {
        Value v = run_expr("var prod = 1\nfor (var i = 1; i <= 4; i = i + 1) { prod = prod * i }\nprod");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 24);  // 1*2*3*4 = 24
        value_free(&v);
    }
    
    // Test 3: C-style for loop counting down
    printf("Test 3: C-style for loop counting down\n");
    {
        Value v = run_expr("let count = 0\nfor (let i = 10; i > 5; i = i - 1) { count = count + 1 }\ncount");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 5);  // iterations: 10,9,8,7,6
        value_free(&v);
    }
    
    // Test 4: C-style for loop with empty init
    printf("Test 4: C-style for loop with empty init\n");
    {
        Value v = run_expr("let j = 0\nlet sum = 0\nfor (; j < 3; j = j + 1) { sum = sum + j }\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 3);  // 0+1+2 = 3
        value_free(&v);
    }
    
    // Test 5: C-style for loop with break
    printf("Test 5: C-style for loop with break\n");
    {
        Value v = run_expr("let sum = 0\nfor (let i = 0; i < 10; i = i + 1) { if (i == 3) { break } sum = sum + i }\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 3);  // 0+1+2 = 3 (breaks at i==3)
        value_free(&v);
    }
    
    // Test 6: C-style for loop with continue
    printf("Test 6: C-style for loop with continue\n");
    {
        Value v = run_expr("let sum = 0\nfor (let i = 0; i < 5; i = i + 1) { if (i == 2) { continue } sum = sum + i }\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 8);  // 0+1+3+4 = 8 (skips i==2)
        value_free(&v);
    }
    
    // Test 7: Nested C-style for loops
    printf("Test 7: Nested C-style for loops\n");
    {
        Value v = run_expr("let count = 0\nfor (let i = 0; i < 3; i = i + 1) { for (let j = 0; j < 2; j = j + 1) { count = count + 1 } }\ncount");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 6);  // 3*2 = 6
        value_free(&v);
    }
    
    // Test 8: C-style for loop with empty update
    printf("Test 8: C-style for loop with empty update\n");
    {
        Value v = run_expr("let sum = 0\nfor (let i = 0; i < 3; ) { sum = sum + i\ni = i + 1 }\nsum");
        ASSERT_EQ(v.type, VAL_INT);
        ASSERT_EQ(v.as.int_val, 3);  // 0+1+2 = 3
        value_free(&v);
    }
    
    printf("\n✓ All C-style for loop tests passed!\n");
    return 0;
}
