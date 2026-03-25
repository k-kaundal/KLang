/*
 * Unit tests for KLang memory management functions (Phase 2)
 * Tests malloc, calloc, realloc, free, sizeof, and memstat
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

/* Helper function to run KLang code */
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
    
    if (result) *result = last_result;
    else value_free(&last_result);
    
    for (int i = 0; i < count; i++) {
        ast_free(nodes[i]);
    }
    free(nodes);
    
    if (interp_out) {
        *interp_out = interp;
    } else {
        interpreter_free(interp);
    }
    
    return 1;
}

/* Test 1: Basic malloc */
static int test_malloc_basic() {
    const char *code = 
        "let ptr = malloc(100)\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to malloc");
    
    Value *ptr = env_get(interp->global_env, "ptr");
    TEST_ASSERT(ptr != NULL, "ptr not found");
    TEST_ASSERT(ptr->type == VAL_POINTER, "ptr should be VAL_POINTER");
    TEST_ASSERT(ptr->as.pointer_val.ptr != NULL, "malloc should return non-null");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 2: sizeof with different types */
static int test_sizeof_types() {
    const char *code = 
        "let int_size = sizeof(42)\n"
        "let str_size = sizeof(\"hello\")\n"
        "let arr_size = sizeof([1, 2, 3])\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed sizeof operations");
    
    Value *int_size = env_get(interp->global_env, "int_size");
    Value *str_size = env_get(interp->global_env, "str_size");
    Value *arr_size = env_get(interp->global_env, "arr_size");
    
    TEST_ASSERT(int_size != NULL && int_size->type == VAL_INT, "int_size should be INT");
    TEST_ASSERT(str_size != NULL && str_size->type == VAL_INT, "str_size should be INT");
    TEST_ASSERT(arr_size != NULL && arr_size->type == VAL_INT, "arr_size should be INT");
    
    /* Check that sizes are reasonable */
    TEST_ASSERT(int_size->as.int_val > 0, "int size should be positive");
    TEST_ASSERT(str_size->as.int_val > 0, "string size should be positive");
    TEST_ASSERT(arr_size->as.int_val > 0, "array size should be positive");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 3: malloc and free */
static int test_malloc_free() {
    const char *code = 
        "let ptr = malloc(200)\n"
        "free(ptr)\n";
    
    Value result;
    TEST_ASSERT(run_code(code, &result, NULL), "Failed malloc/free cycle");
    value_free(&result);
    return 1;
}

/* Test 4: calloc (allocate and zero) */
static int test_calloc_basic() {
    const char *code = 
        "let ptr = calloc(10, 8)\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed calloc");
    
    Value *ptr = env_get(interp->global_env, "ptr");
    TEST_ASSERT(ptr != NULL, "ptr not found");
    TEST_ASSERT(ptr->type == VAL_POINTER, "ptr should be VAL_POINTER");
    TEST_ASSERT(ptr->as.pointer_val.ptr != NULL, "calloc should return non-null");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 5: realloc */
static int test_realloc_basic() {
    const char *code = 
        "let ptr1 = malloc(100)\n"
        "let ptr2 = realloc(ptr1, 200)\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed realloc");
    
    Value *ptr2 = env_get(interp->global_env, "ptr2");
    TEST_ASSERT(ptr2 != NULL, "ptr2 not found");
    TEST_ASSERT(ptr2->type == VAL_POINTER, "ptr2 should be VAL_POINTER");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 6: malloc with zero size */
static int test_malloc_zero() {
    const char *code = 
        "let ptr = malloc(0)\n";
    
    Value result;
    /* This should either succeed or fail gracefully */
    run_code(code, &result, NULL);
    value_free(&result);
    return 1;
}

/* Test 7: free null pointer (should be safe) */
static int test_free_null() {
    const char *code = 
        "let ptr = null\n"
        "free(ptr)\n";
    
    Value result;
    /* Should handle null gracefully */
    TEST_ASSERT(run_code(code, &result, NULL), "free(null) should be safe");
    value_free(&result);
    return 1;
}

/* Test 8: Multiple allocations */
static int test_multiple_allocations() {
    const char *code = 
        "let ptr1 = malloc(100)\n"
        "let ptr2 = malloc(200)\n"
        "let ptr3 = malloc(300)\n"
        "free(ptr1)\n"
        "free(ptr2)\n"
        "free(ptr3)\n";
    
    Value result;
    TEST_ASSERT(run_code(code, &result, NULL), "Multiple allocations failed");
    value_free(&result);
    return 1;
}

/* Test 9: sizeof with struct */
static int test_sizeof_struct() {
    const char *code = 
        "struct Point {\n"
        "    x: int,\n"
        "    y: int\n"
        "}\n"
        "let p = Point { x: 10, y: 20 }\n"
        "let size = sizeof(p)\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed sizeof with struct");
    
    Value *size = env_get(interp->global_env, "size");
    TEST_ASSERT(size != NULL && size->type == VAL_INT, "size should be INT");
    TEST_ASSERT(size->as.int_val > 0, "struct size should be positive");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 10: Large allocation */
static int test_large_allocation() {
    const char *code = 
        "let ptr = malloc(10000)\n"
        "free(ptr)\n";
    
    Value result;
    TEST_ASSERT(run_code(code, &result, NULL), "Large allocation failed");
    value_free(&result);
    return 1;
}

/* Test 11: Sequential realloc */
static int test_sequential_realloc() {
    const char *code = 
        "let ptr = malloc(10)\n"
        "ptr = realloc(ptr, 20)\n"
        "ptr = realloc(ptr, 40)\n"
        "ptr = realloc(ptr, 80)\n"
        "free(ptr)\n";
    
    Value result;
    TEST_ASSERT(run_code(code, &result, NULL), "Sequential realloc failed");
    value_free(&result);
    return 1;
}

/* Test 12: sizeof with different value types */
static int test_sizeof_various() {
    const char *code = 
        "let null_size = sizeof(null)\n"
        "let bool_size = sizeof(true)\n"
        "let float_size = sizeof(3.14)\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "sizeof with various types failed");
    
    Value *null_size = env_get(interp->global_env, "null_size");
    Value *bool_size = env_get(interp->global_env, "bool_size");
    Value *float_size = env_get(interp->global_env, "float_size");
    
    TEST_ASSERT(null_size != NULL && null_size->type == VAL_INT, "null_size should be INT");
    TEST_ASSERT(bool_size != NULL && bool_size->type == VAL_INT, "bool_size should be INT");
    TEST_ASSERT(float_size != NULL && float_size->type == VAL_INT, "float_size should be INT");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Main test runner */
int main() {
    int total = 0, passed = 0, failed = 0;
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║   KLang Memory Management Unit Tests (Phase 2)       ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    RUN_TEST(test_malloc_basic);
    RUN_TEST(test_sizeof_types);
    RUN_TEST(test_malloc_free);
    RUN_TEST(test_calloc_basic);
    RUN_TEST(test_realloc_basic);
    RUN_TEST(test_malloc_zero);
    RUN_TEST(test_free_null);
    RUN_TEST(test_multiple_allocations);
    RUN_TEST(test_sizeof_struct);
    RUN_TEST(test_large_allocation);
    RUN_TEST(test_sequential_realloc);
    RUN_TEST(test_sizeof_various);
    
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
