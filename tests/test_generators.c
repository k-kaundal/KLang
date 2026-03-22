#include "test.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <string.h>

/* Test basic generator syntax parsing */
static void test_generator_syntax(void) {
    const char *source = "fn* counter() { yield 1 }";
    Lexer lexer;
    Parser parser;
    
    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);
    
    ASTNode *node = parse_statement(&parser);
    
    TEST_ASSERT(node != NULL, "Should parse generator function");
    TEST_ASSERT(node->type == NODE_FUNC_DEF, "Should be function definition");
    TEST_ASSERT(node->data.func_def.is_generator == 1, "Should have is_generator flag set");
    
    ast_free(node);
    parser_free(&parser);
    lexer_free(&lexer);
}

/* Test yield expression parsing */
static void test_yield_parsing(void) {
    const char *source = "fn* gen() { yield 42 }";
    Lexer lexer;
    Parser parser;
    
    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);
    
    ASTNode *node = parse_statement(&parser);
    
    TEST_ASSERT(node != NULL, "Should parse generator");
    TEST_ASSERT(node->type == NODE_FUNC_DEF, "Should be function");
    TEST_ASSERT(node->data.func_def.is_generator, "Should be generator");
    
    /* Check body contains yield */
    ASTNode *body = node->data.func_def.body;
    TEST_ASSERT(body != NULL, "Should have body");
    TEST_ASSERT(body->type == NODE_BLOCK, "Body should be block");
    TEST_ASSERT(body->data.block.stmts.count > 0, "Block should have statements");
    
    ASTNode *yield_stmt = body->data.block.stmts.items[0];
    TEST_ASSERT(yield_stmt->type == NODE_YIELD, "First statement should be yield");
    TEST_ASSERT(yield_stmt->data.yield_expr.value != NULL, "Yield should have value");
    
    ast_free(node);
    parser_free(&parser);
    lexer_free(&lexer);
}

/* Test generator object creation */
static void test_generator_creation(void) {
    const char *source = "fn* gen() { yield 42 }\nlet g = gen()";
    Lexer lexer;
    Parser parser;
    Interpreter *interp;
    
    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);
    interp = interpreter_new();
    
    while (!check(&parser, TOKEN_EOF)) {
        ASTNode *node = parse_statement(&parser);
        if (node) {
            Value result = eval_node(interp, node);
            value_free(&result);
            ast_free(node);
        }
    }
    
    /* Check that 'g' is a generator */
    Value *g = env_get(interp->global_env, "g");
    TEST_ASSERT(g != NULL, "Should have generator variable");
    TEST_ASSERT(g->type == VAL_GENERATOR, "Should be generator type");
    TEST_ASSERT(g->as.generator_val.state == GEN_SUSPENDED, "Should be suspended initially");
    
    parser_free(&parser);
    lexer_free(&lexer);
    interpreter_free(interp);
}

/* Test basic generator next() call */
static void test_generator_next(void) {
    const char *source = "fn* gen() { yield 42 }\nlet g = gen()\nlet r = g.next()";
    Lexer lexer;
    Parser parser;
    Interpreter *interp;
    
    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);
    interp = interpreter_new();
    
    while (!check(&parser, TOKEN_EOF)) {
        ASTNode *node = parse_statement(&parser);
        if (node) {
            Value result = eval_node(interp, node);
            value_free(&result);
            ast_free(node);
        }
    }
    
    /* Check result object */
    Value *r = env_get(interp->global_env, "r");
    TEST_ASSERT(r != NULL, "Should have result variable");
    TEST_ASSERT(r->type == VAL_OBJECT, "Result should be object");
    
    /* Check value field */
    Value *value_field = env_get(r->as.object_val.fields, "value");
    TEST_ASSERT(value_field != NULL, "Should have value field");
    TEST_ASSERT(value_field->type == VAL_INT, "Value should be int");
    TEST_ASSERT(value_field->as.int_val == 42, "Value should be 42");
    
    /* Check done field */
    Value *done_field = env_get(r->as.object_val.fields, "done");
    TEST_ASSERT(done_field != NULL, "Should have done field");
    TEST_ASSERT(done_field->type == VAL_BOOL, "Done should be bool");
    TEST_ASSERT(done_field->as.bool_val == 1, "Done should be true");
    
    parser_free(&parser);
    lexer_free(&lexer);
    interpreter_free(interp);
}

/* Test generator exhaustion */
static void test_generator_exhaustion(void) {
    const char *source = "fn* gen() { yield 1 }\nlet g = gen()\ng.next()\nlet r2 = g.next()";
    Lexer lexer;
    Parser parser;
    Interpreter *interp;
    
    lexer_init(&lexer, source);
    parser_init(&parser, &lexer);
    interp = interpreter_new();
    
    while (!check(&parser, TOKEN_EOF)) {
        ASTNode *node = parse_statement(&parser);
        if (node) {
            Value result = eval_node(interp, node);
            value_free(&result);
            ast_free(node);
        }
    }
    
    /* Check second result */
    Value *r2 = env_get(interp->global_env, "r2");
    TEST_ASSERT(r2 != NULL, "Should have second result");
    
    Value *value_field = env_get(r2->as.object_val.fields, "value");
    Value *done_field = env_get(r2->as.object_val.fields, "done");
    
    TEST_ASSERT(done_field->as.bool_val == 1, "Second call should be done");
    
    parser_free(&parser);
    lexer_free(&lexer);
    interpreter_free(interp);
}

void test_generators_run_all(void) {
    printf("Running generator tests...\n");
    test_generator_syntax();
    test_yield_parsing();
    test_generator_creation();
    test_generator_next();
    test_generator_exhaustion();
    printf("All generator tests passed!\n");
}
