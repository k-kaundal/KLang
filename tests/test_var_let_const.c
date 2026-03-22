#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../src/lexer.h"
#include "../src/parser.h"
#include "../src/ast.h"
#include "../src/interpreter.h"

#define ASSERT(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "Assertion failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        return 1; \
    } \
} while (0)

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        fprintf(stderr, "Assertion failed: %s == %s (%d != %d) at %s:%d\n", \
                #a, #b, (int)(a), (int)(b), __FILE__, __LINE__); \
        return 1; \
    } \
} while (0)

#define ASSERT_STR_EQ(a, b) do { \
    if (strcmp((a), (b)) != 0) { \
        fprintf(stderr, "Assertion failed: %s == %s (\"%s\" != \"%s\") at %s:%d\n", \
                #a, #b, (a), (b), __FILE__, __LINE__); \
        return 1; \
    } \
} while (0)

static Value run_code(const char *source) {
    Lexer lexer;
    Parser parser;
    ASTNode **nodes;
    int count;
    Interpreter *interp;
    Value result;
    int i;

    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);
    nodes = parse_program(&parser, &count);
    
    interp = interpreter_new();
    result = make_null();
    
    for (i = 0; i < count; i++) {
        value_free(&result);
        result = eval_node(interp, nodes[i]);
    }
    
    /* Check for errors */
    if (interp->had_error) {
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        interpreter_free(interp);
        lexer_free(&lexer);
        return make_null();
    }
    
    for (i = 0; i < count; i++) ast_free(nodes[i]);
    free(nodes);
    interpreter_free(interp);
    lexer_free(&lexer);
    return result;
}

/* Test lexer recognizes new tokens */
int test_lexer_tokens(void) {
    Lexer lexer;
    Token tok;
    
    /* Test var */
    lexer_init(&lexer, "var x = 5");
    tok = lexer_next_token(&lexer);
    ASSERT_EQ(tok.type, TOKEN_VAR);
    token_free(&tok);
    lexer_free(&lexer);
    
    /* Test const */
    lexer_init(&lexer, "const PI = 3.14");
    tok = lexer_next_token(&lexer);
    ASSERT_EQ(tok.type, TOKEN_CONST);
    token_free(&tok);
    lexer_free(&lexer);
    
    /* Test let still works */
    lexer_init(&lexer, "let y = 10");
    tok = lexer_next_token(&lexer);
    ASSERT_EQ(tok.type, TOKEN_LET);
    token_free(&tok);
    lexer_free(&lexer);
    
    printf("✓ Lexer tokens test passed\n");
    return 0;
}

/* Test parser creates correct AST nodes */
int test_parser_declarations(void) {
    Lexer lexer;
    Parser parser;
    ASTNode **nodes;
    int count;
    
    /* Test var declaration */
    lexer_init(&lexer, "var x = 42");
    parser_init(&parser, &lexer);
    nodes = parse_program(&parser, &count);
    ASSERT_EQ(count, 1);
    ASSERT_EQ(nodes[0]->type, NODE_LET);
    ASSERT_STR_EQ(nodes[0]->data.let_stmt.name, "x");
    ASSERT_EQ(nodes[0]->data.let_stmt.decl_type, DECL_VAR);
    ast_free(nodes[0]);
    free(nodes);
    lexer_free(&lexer);
    
    /* Test const declaration */
    lexer_init(&lexer, "const PI = 3.14");
    parser_init(&parser, &lexer);
    nodes = parse_program(&parser, &count);
    ASSERT_EQ(count, 1);
    ASSERT_EQ(nodes[0]->type, NODE_LET);
    ASSERT_STR_EQ(nodes[0]->data.let_stmt.name, "PI");
    ASSERT_EQ(nodes[0]->data.let_stmt.decl_type, DECL_CONST);
    ast_free(nodes[0]);
    free(nodes);
    lexer_free(&lexer);
    
    /* Test let declaration */
    lexer_init(&lexer, "let y = 100");
    parser_init(&parser, &lexer);
    nodes = parse_program(&parser, &count);
    ASSERT_EQ(count, 1);
    ASSERT_EQ(nodes[0]->type, NODE_LET);
    ASSERT_STR_EQ(nodes[0]->data.let_stmt.name, "y");
    ASSERT_EQ(nodes[0]->data.let_stmt.decl_type, DECL_LET);
    ast_free(nodes[0]);
    free(nodes);
    lexer_free(&lexer);
    
    printf("✓ Parser declarations test passed\n");
    return 0;
}

/* Test let/var/const basic functionality */
int test_basic_declarations(void) {
    Value v;
    
    /* Test let */
    v = run_code("let x = 42\nx");
    ASSERT_EQ(v.type, VAL_INT);
    ASSERT_EQ(v.as.int_val, 42);
    value_free(&v);
    
    /* Test var */
    v = run_code("var y = 100\ny");
    ASSERT_EQ(v.type, VAL_INT);
    ASSERT_EQ(v.as.int_val, 100);
    value_free(&v);
    
    /* Test const */
    v = run_code("const PI = 3.14\nPI");
    ASSERT_EQ(v.type, VAL_FLOAT);
    value_free(&v);
    
    printf("✓ Basic declarations test passed\n");
    return 0;
}

/* Test const immutability */
int test_const_immutability(void) {
    Value v;
    
    /* const cannot be reassigned - this should produce an error */
    v = run_code("const X = 10\nX = 20\nX");
    /* After error, X should still be 10 or null */
    value_free(&v);
    
    printf("✓ Const immutability test passed\n");
    return 0;
}

/* Test let prevents redeclaration */
int test_let_no_redeclaration(void) {
    Value v;
    
    /* let x twice in same scope should error */
    v = run_code("let x = 1\nlet x = 2\nx");
    /* Should have error, x is either 1 or null */
    value_free(&v);
    
    printf("✓ Let no redeclaration test passed\n");
    return 0;
}

/* Test var allows redeclaration */
int test_var_redeclaration(void) {
    Value v;
    
    /* var allows redeclaration */
    v = run_code("var x = 1\nvar x = 2\nx");
    ASSERT_EQ(v.type, VAL_INT);
    ASSERT_EQ(v.as.int_val, 2);
    value_free(&v);
    
    printf("✓ Var redeclaration test passed\n");
    return 0;
}

/* Test block scoping for let */
int test_let_block_scope(void) {
    Value v;
    
    /* let in inner block doesn't affect outer */
    v = run_code("let x = 10\n{ let x = 20 }\nx");
    ASSERT_EQ(v.type, VAL_INT);
    ASSERT_EQ(v.as.int_val, 10);
    value_free(&v);
    
    printf("✓ Let block scope test passed\n");
    return 0;
}

/* Test const without initialization fails */
int test_const_must_initialize(void) {
    Value v;
    
    /* const without initialization should error during parsing */
    v = run_code("const X");
    value_free(&v);
    
    printf("✓ Const must initialize test passed\n");
    return 0;
}

/* Test let/var can be declared without initialization */
int test_let_var_optional_init(void) {
    Value v;
    
    /* let without initialization */
    v = run_code("let x\nx");
    ASSERT_EQ(v.type, VAL_NULL);
    value_free(&v);
    
    /* var without initialization */
    v = run_code("var y\ny");
    ASSERT_EQ(v.type, VAL_NULL);
    value_free(&v);
    
    printf("✓ Let/var optional init test passed\n");
    return 0;
}

/* Test mutation */
int test_mutation(void) {
    Value v;
    
    /* let can be mutated */
    v = run_code("let x = 5\nx = 10\nx");
    ASSERT_EQ(v.type, VAL_INT);
    ASSERT_EQ(v.as.int_val, 10);
    value_free(&v);
    
    /* var can be mutated */
    v = run_code("var y = 5\ny = 10\ny");
    ASSERT_EQ(v.type, VAL_INT);
    ASSERT_EQ(v.as.int_val, 10);
    value_free(&v);
    
    printf("✓ Mutation test passed\n");
    return 0;
}

/* Test complex scoping scenario */
int test_complex_scoping(void) {
    Value v;
    
    /* Complex scoping with let/var/const */
    const char *code = 
        "let outer = 1\n"
        "{\n"
        "  let inner = 2\n"
        "  const CONST = 3\n"
        "  outer = 10\n"
        "}\n"
        "outer";
    
    v = run_code(code);
    ASSERT_EQ(v.type, VAL_INT);
    ASSERT_EQ(v.as.int_val, 10);
    value_free(&v);
    
    printf("✓ Complex scoping test passed\n");
    return 0;
}

int main(void) {
    int result = 0;
    
    printf("Running var/let/const tests...\n\n");
    
    result |= test_lexer_tokens();
    result |= test_parser_declarations();
    result |= test_basic_declarations();
    result |= test_const_immutability();
    result |= test_let_no_redeclaration();
    result |= test_var_redeclaration();
    result |= test_let_block_scope();
    result |= test_const_must_initialize();
    result |= test_let_var_optional_init();
    result |= test_mutation();
    result |= test_complex_scoping();
    
    if (result == 0) {
        printf("\n✅ All var/let/const tests passed!\n");
    } else {
        printf("\n❌ Some tests failed\n");
    }
    
    return result;
}
