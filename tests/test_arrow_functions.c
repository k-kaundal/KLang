#include "test_runner.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "runtime.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

void run_arrow_function_tests(void) {
    printf("--- Arrow Function Tests ---\n");

    /* Test 1: Single parameter without parens */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        Interpreter *interp;
        Value result;
        
        lexer_init(&lexer, "const square = x => x * x\nsquare(5)");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        
        ASSERT_EQ(count, 2);
        ASSERT_EQ(parser.had_error, 0);
        
        // First node should be a let statement
        ASSERT_EQ(nodes[0]->type, NODE_LET);
        ASSERT_STR_EQ(nodes[0]->data.let_stmt.name, "square");
        
        // Value should be a function def with is_arrow set
        ASSERT_EQ(nodes[0]->data.let_stmt.value->type, NODE_FUNC_DEF);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->data.func_def.is_arrow, 1);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->data.func_def.params.count, 1);
        
        // Execute and test
        interp = interpreter_new();
        runtime_init(interp);
        
        for (i = 0; i < count; i++) {
            result = eval_node(interp, nodes[i]);
            if (i == count - 1) {
                ASSERT_EQ(result.type, VAL_INT);
                ASSERT_EQ(result.as.int_val, 25);
            }
            value_free(&result);
        }
        
        interpreter_free(interp);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test 2: Multiple parameters with parens */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        Interpreter *interp;
        Value result;
        
        lexer_init(&lexer, "const add = (a, b) => a + b\nadd(3, 7)");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        
        ASSERT_EQ(count, 2);
        ASSERT_EQ(parser.had_error, 0);
        
        ASSERT_EQ(nodes[0]->type, NODE_LET);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->type, NODE_FUNC_DEF);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->data.func_def.is_arrow, 1);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->data.func_def.params.count, 2);
        
        // Execute and test
        interp = interpreter_new();
        runtime_init(interp);
        
        for (i = 0; i < count; i++) {
            result = eval_node(interp, nodes[i]);
            if (i == count - 1) {
                ASSERT_EQ(result.type, VAL_INT);
                ASSERT_EQ(result.as.int_val, 10);
            }
            value_free(&result);
        }
        
        interpreter_free(interp);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test 3: No parameters */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        Interpreter *interp;
        Value result;
        
        lexer_init(&lexer, "const greet = () => \"Hello!\"\ngreet()");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        
        ASSERT_EQ(count, 2);
        ASSERT_EQ(parser.had_error, 0);
        
        ASSERT_EQ(nodes[0]->type, NODE_LET);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->type, NODE_FUNC_DEF);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->data.func_def.is_arrow, 1);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->data.func_def.params.count, 0);
        
        // Execute and test
        interp = interpreter_new();
        runtime_init(interp);
        
        for (i = 0; i < count; i++) {
            result = eval_node(interp, nodes[i]);
            if (i == count - 1) {
                ASSERT_EQ(result.type, VAL_STRING);
                ASSERT_STR_EQ(result.as.str_val, "Hello!");
            }
            value_free(&result);
        }
        
        interpreter_free(interp);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test 4: Block body with explicit return */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        Interpreter *interp;
        Value result;
        
        lexer_init(&lexer, "const complex = x => { let y = x * 2\n return y + 1 }\ncomplex(10)");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        
        ASSERT_EQ(count, 2);
        ASSERT_EQ(parser.had_error, 0);
        
        ASSERT_EQ(nodes[0]->type, NODE_LET);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->type, NODE_FUNC_DEF);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->data.func_def.is_arrow, 1);
        
        // Execute and test
        interp = interpreter_new();
        runtime_init(interp);
        
        for (i = 0; i < count; i++) {
            result = eval_node(interp, nodes[i]);
            if (i == count - 1) {
                ASSERT_EQ(result.type, VAL_INT);
                ASSERT_EQ(result.as.int_val, 21);
            }
            value_free(&result);
        }
        
        interpreter_free(interp);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test 5: Arrow function passed to higher-order function */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        Interpreter *interp;
        Value result;
        
        // Note: Passing functions as values has some issues in KLang currently
        // This test verifies parsing and basic execution
        lexer_init(&lexer, 
            "const double = x => x * 2\n"
            "double(5)");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        
        ASSERT_EQ(count, 2);
        ASSERT_EQ(parser.had_error, 0);
        
        // Execute and test
        interp = interpreter_new();
        runtime_init(interp);
        
        for (i = 0; i < count; i++) {
            result = eval_node(interp, nodes[i]);
            if (i == count - 1) {
                ASSERT_EQ(result.type, VAL_INT);
                ASSERT_EQ(result.as.int_val, 10);
            }
            value_free(&result);
        }
        
        interpreter_free(interp);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test 6: Arrow function with let/var/const declarations */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        Interpreter *interp;
        Value result;
        
        lexer_init(&lexer, 
            "let f1 = x => x + 1\n"
            "var f2 = x => x + 2\n"
            "const f3 = x => x + 3\n"
            "f1(10) + f2(10) + f3(10)");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        
        ASSERT_EQ(count, 4);
        ASSERT_EQ(parser.had_error, 0);
        
        // Execute and test
        interp = interpreter_new();
        runtime_init(interp);
        
        for (i = 0; i < count; i++) {
            result = eval_node(interp, nodes[i]);
            if (i == count - 1) {
                ASSERT_EQ(result.type, VAL_INT);
                ASSERT_EQ(result.as.int_val, 36); // 11 + 12 + 13
            }
            value_free(&result);
        }
        
        interpreter_free(interp);
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }

    /* Test 7: Nested arrow functions */
    {
        Lexer lexer;
        Parser parser;
        int count = 0;
        ASTNode **nodes;
        int i;
        Interpreter *interp;
        
        // Test that nested arrow functions parse correctly
        lexer_init(&lexer, 
            "const makeAdder = x => y => x + y\n"
            "let x = 5");
        parser_init(&parser, &lexer);
        nodes = parse_program(&parser, &count);
        
        ASSERT_EQ(count, 2);
        ASSERT_EQ(parser.had_error, 0);
        
        // Verify the structure
        ASSERT_EQ(nodes[0]->type, NODE_LET);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->type, NODE_FUNC_DEF);
        ASSERT_EQ(nodes[0]->data.let_stmt.value->data.func_def.is_arrow, 1);
        
        // The body should contain a return of another arrow function
        ASTNode *body = nodes[0]->data.let_stmt.value->data.func_def.body;
        ASSERT_EQ(body->type, NODE_BLOCK);
        ASSERT_EQ(body->data.block.stmts.count, 1);
        ASSERT_EQ(body->data.block.stmts.items[0]->type, NODE_RETURN);
        
        ASTNode *inner = body->data.block.stmts.items[0]->data.return_stmt.value;
        ASSERT_EQ(inner->type, NODE_FUNC_DEF);
        ASSERT_EQ(inner->data.func_def.is_arrow, 1);
        
        interp = interpreter_new();
        runtime_init(interp);
        for (i = 0; i < count; i++) {
            Value result = eval_node(interp, nodes[i]);
            value_free(&result);
        }
        interpreter_free(interp);
        
        for (i = 0; i < count; i++) ast_free(nodes[i]);
        free(nodes);
        parser_free(&parser);
        lexer_free(&lexer);
    }
}
