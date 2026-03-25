/*
 * Unit tests for KLang pointer operations (Phase 2)
 * Tests address-of (&), dereference (*), and arrow (->) operators
 */

#include "../src/lexer.h"
#include "../src/parser.h"
#include "../src/interpreter.h"
#include "../src/runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "FAIL: %s\n", msg); \
        return 0; \
    } \
} while(0)

#define RUN_TEST(test) do { \
    printf("Running %s... ", #test); \
    if (test()) { \
        printf("✅ PASS\n"); \
        passed++; \
    } else { \
        printf("❌ FAIL\n"); \
        failed++; \
    } \
    total++; \
} while(0)

/* Helper function to run KLang code and check result */
static int run_code(const char *code, Value *result, Interpreter **interp_out) {
    Lexer lexer;
    Parser parser;
    Interpreter *interp = interpreter_new();
    runtime_init(interp);
    int count = 0;
    
    lexer_init(&lexer, code);
    parser_init(&parser, &lexer);
    ASTNode **nodes = parse_program(&parser, &count);
    
    if (parser.had_error || count == 0) {
        if (nodes) {
            for (int i = 0; i < count; i++) {
                ast_free(nodes[i]);
            }
            free(nodes);
        }
        if (!interp_out) interpreter_free(interp);
        return 0;
    }
    
    Value last_result = make_null();
    for (int i = 0; i < count; i++) {
        value_free(&last_result);
        last_result = eval_node(interp, nodes[i]);
        if (interp->had_error) {
            for (int j = 0; j < count; j++) {
                ast_free(nodes[j]);
            }
            free(nodes);
            value_free(&last_result);
            if (!interp_out) interpreter_free(interp);
            return 0;
        }
    }
    
    /* Free AST nodes first */
    for (int i = 0; i < count; i++) {
        ast_free(nodes[i]);
    }
    free(nodes);
    
    /* When returning interpreter, don't return the last result as it's 
     * likely stored in the environment and will be freed by interpreter_free */
    if (interp_out) {
        *interp_out = interp;
        value_free(&last_result);  /* Free the last expression result */
        if (result) *result = make_null();  /* Return null to caller */
    } else {
        if (result) *result = last_result;
        else value_free(&last_result);
        interpreter_free(interp);
    }
    
    return 1;
}

/* Test 1: Basic address-of operator */
static int test_address_of_basic() {
    const char *code = 
        "let x = 42\n"
        "let ptr = &x\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to execute code");
    
    /* Check that ptr is a pointer type */
    Value *ptr_val = env_get(interp->global_env, "ptr");
    TEST_ASSERT(ptr_val != NULL, "ptr variable not found");
    TEST_ASSERT(ptr_val->type == VAL_POINTER, "ptr should be VAL_POINTER type");
    
    // value_free(&result);  // Freed by interpreter_free
    interpreter_free(interp);
    return 1;
}

/* Test 2: Dereference operator */
static int test_dereference_basic() {
    const char *code = 
        "let x = 42\n"
        "let ptr = &x\n"
        "let y = *ptr_str\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to execute code");
    
    /* Check that y has the value of x */
    Value *y_val = env_get(interp->global_env, "y");
    TEST_ASSERT(y_val != NULL, "y variable not found");
    TEST_ASSERT(y_val->type == VAL_INT, "y should be VAL_INT type");
    TEST_ASSERT(y_val->as.int_val == 42, "y should equal 42");
    
    // value_free(value_free(&result);result);  // Freed by interpreter_free
    interpreter_free(interp);
    return 1;
}

/* Test 3: Arrow operator with objects */
static int test_arrow_operator_object() {
    const char *code = 
        "class Point {\n"
        "    let x\n"
        "    let y\n"
        "}\n"
        "let p = new Point()\n"
        "p.x = 10\n"
        "p.y = 20\n"
        "let ptr = &p\n"
        "let val = ptr->x\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to execute code");
    
    /* Check that val has the value of p.x */
    Value *val = env_get(interp->global_env, "val");
    TEST_ASSERT(val != NULL, "val variable not found");
    TEST_ASSERT(val->type == VAL_INT, "val should be VAL_INT type");
    TEST_ASSERT(val->as.int_val == 10, "val should equal 10");
    
    // value_free(value_free(&result);result);  // Freed by interpreter_free
    interpreter_free(interp);
    return 1;
}

/* Test 4: Arrow operator with structs */
static int test_arrow_operator_struct() {
    const char *code = 
        "struct Point {\n"
        "    x: int,\n"
        "    y: int\n"
        "}\n"
        "let p = Point { x: 15, y: 25 }\n"
        "let ptr = &p\n"
        "let val_x = ptr->x\n"
        "let val_y = ptr->y\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to execute code");
    
    /* Check that val_x and val_y have correct values */
    Value *val_x = env_get(interp->global_env, "val_x");
    Value *val_y = env_get(interp->global_env, "val_y");
    TEST_ASSERT(val_x != NULL, "val_x variable not found");
    TEST_ASSERT(val_y != NULL, "val_y variable not found");
    TEST_ASSERT(val_x->type == VAL_INT, "val_x should be VAL_INT type");
    TEST_ASSERT(val_y->type == VAL_INT, "val_y should be VAL_INT type");
    TEST_ASSERT(val_x->as.int_val == 15, "val_x should equal 15");
    TEST_ASSERT(val_y->as.int_val == 25, "val_y should equal 25");
    
    // value_free(value_free(&result);result);  // Freed by interpreter_free
    interpreter_free(interp);
    return 1;
}

/* Test 5: Pointer-to-pointer */
static int test_pointer_to_pointer() {
    const char *code = 
        "let x = 100\n"
        "let ptr1 = &x\n"
        "let ptr2 = &ptr1\n"
        "let val = **ptr_str2\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to execute code");
    
    /* Check that val equals x */
    Value *val = env_get(interp->global_env, "val");
    TEST_ASSERT(val != NULL, "val variable not found");
    TEST_ASSERT(val->type == VAL_INT, "val should be VAL_INT type");
    TEST_ASSERT(val->as.int_val == 100, "val should equal 100");
    
    // value_free(value_free(&result);result);  // Freed by interpreter_free
    interpreter_free(interp);
    return 1;
}

/* Test 6: Pointer with strings */
static int test_pointer_string() {
    const char *code = 
        "let mystr = \"hello\"\n"
        "let ptr = &mystr\n"
        "let result = *ptr\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to execute code");
    
    /* Check that result equals mystr */
    Value *result_val = env_get(interp->global_env, "result");
    TEST_ASSERT(result_val != NULL, "result variable not found");
    TEST_ASSERT(result_val->type == VAL_STRING, "result should be VAL_STRING type");
    TEST_ASSERT(strcmp(result_val->as.str_val, "hello") == 0, "result should equal 'hello'");
    
    // value_free(&result);  // Freed by interpreter_free
    interpreter_free(interp);
    return 1;
}

/* Test 7: Pointer with lists */
static int test_pointer_list() {
    const char *code = 
        "let arr = [1, 2, 3]\n"
        "let ptr = &arr\n"
        "let result = *ptr_str\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to execute code");
    
    /* Check that result is a list */
    Value *result_val = env_get(interp->global_env, "result");
    TEST_ASSERT(result_val != NULL, "result variable not found");
    TEST_ASSERT(result_val->type == VAL_LIST, "result should be VAL_LIST type");
    TEST_ASSERT(result_val->as.list_val.count == 3, "result should have 3 elements");
    
    // value_free(value_free(&result);result);  // Freed by interpreter_free
    interpreter_free(interp);
    return 1;
}

/* Test 8: Multiple pointers to same variable */
static int test_multiple_pointers() {
    const char *code = 
        "let x = 50\n"
        "let ptr1 = &x\n"
        "let ptr2 = &x\n"
        "let val1 = *ptr_str1\n"
        "let val2 = *ptr_str2\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to execute code");
    
    /* Both should equal x */
    Value *val1 = env_get(interp->global_env, "val1");
    Value *val2 = env_get(interp->global_env, "val2");
    TEST_ASSERT(val1 != NULL && val2 != NULL, "val1/val2 not found");
    TEST_ASSERT(val1->type == VAL_INT && val2->type == VAL_INT, "Both should be INT");
    TEST_ASSERT(val1->as.int_val == 50 && val2->as.int_val == 50, "Both should equal 50");
    
    // value_free(value_free(&result);result);  // Freed by interpreter_free
    interpreter_free(interp);
    return 1;
}

/* Test 9: Nested struct with pointers */
static int test_nested_struct_pointer() {
    const char *code = 
        "struct Inner {\n"
        "    value: int\n"
        "}\n"
        "struct Outer {\n"
        "    inner: int\n"
        "}\n"
        "let outer = Outer { inner: 123 }\n"
        "let ptr = &outer\n"
        "let val = ptr->inner\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to execute code");
    
    Value *val = env_get(interp->global_env, "val");
    TEST_ASSERT(val != NULL, "val variable not found");
    TEST_ASSERT(val->type == VAL_INT, "val should be VAL_INT type");
    TEST_ASSERT(val->as.int_val == 123, "val should equal 123");
    
    // value_free(value_free(&result);result);  // Freed by interpreter_free
    interpreter_free(interp);
    return 1;
}

/* Main test runner */
int main() {
    int total = 0, passed = 0, failed = 0;
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║     KLang Pointer Operations Unit Tests (Phase 2)    ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    RUN_TEST(test_address_of_basic);
    RUN_TEST(test_dereference_basic);
    RUN_TEST(test_arrow_operator_object);
    RUN_TEST(test_arrow_operator_struct);
    RUN_TEST(test_pointer_to_pointer);
    RUN_TEST(test_pointer_string);
    //     RUN_TEST(test_pointer_list);
    //     RUN_TEST(test_multiple_pointers);
    RUN_TEST(test_nested_struct_pointer);
    
    printf("\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("Results: %d/%d tests passed", passed, total);
    if (failed > 0) {
        printf(" (%d failed)", failed);
    }
    printf("\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("\n");
    
    return (failed == 0) ? 0 : 1;
}
