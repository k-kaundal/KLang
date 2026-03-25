/*
 * Unit tests for KLang struct/union operations (Phase 2)
 * Tests struct declaration, initialization, and field access
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

/* Test 1: Basic struct declaration */
static int test_struct_declaration() {
    const char *code = 
        "struct Point {\n"
        "    x: int,\n"
        "    y: int\n"
        "}\n";
    
    Value result;
    TEST_ASSERT(run_code(code, &result, NULL), "Failed to declare struct");
    value_free(&result);
    return 1;
}

/* Test 2: Struct initialization with values */
static int test_struct_initialization() {
    const char *code = 
        "struct Point {\n"
        "    x: int,\n"
        "    y: int\n"
        "}\n"
        "let p = Point { x: 10, y: 20 }\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to initialize struct");
    
    /* Check that p exists and is a struct */
    Value *p = env_get(interp->global_env, "p");
    TEST_ASSERT(p != NULL, "p variable not found");
    TEST_ASSERT(p->type == VAL_STRUCT, "p should be VAL_STRUCT type");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 3: Struct field access with dot operator */
static int test_struct_field_access() {
    const char *code = 
        "struct Point {\n"
        "    x: int,\n"
        "    y: int\n"
        "}\n"
        "let p = Point { x: 10, y: 20 }\n"
        "let x_val = p.x\n"
        "let y_val = p.y\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed to access struct fields");
    
    /* Check field values */
    Value *x_val = env_get(interp->global_env, "x_val");
    Value *y_val = env_get(interp->global_env, "y_val");
    TEST_ASSERT(x_val != NULL && y_val != NULL, "Field values not found");
    TEST_ASSERT(x_val->type == VAL_INT && y_val->type == VAL_INT, "Fields should be INT");
    TEST_ASSERT(x_val->as.int_val == 10, "x should equal 10");
    TEST_ASSERT(y_val->as.int_val == 20, "y should equal 20");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 4: Struct with different types */
static int test_struct_mixed_types() {
    const char *code = 
        "struct Person {\n"
        "    name: string,\n"
        "    age: int,\n"
        "    height: float\n"
        "}\n"
        "let p = Person { name: \"Alice\", age: 30, height: 5.6 }\n"
        "let name = p.name\n"
        "let age = p.age\n"
        "let height = p.height\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed with mixed types");
    
    Value *name = env_get(interp->global_env, "name");
    Value *age = env_get(interp->global_env, "age");
    Value *height = env_get(interp->global_env, "height");
    
    TEST_ASSERT(name != NULL && name->type == VAL_STRING, "name should be string");
    TEST_ASSERT(age != NULL && age->type == VAL_INT, "age should be int");
    TEST_ASSERT(height != NULL && height->type == VAL_FLOAT, "height should be float");
    TEST_ASSERT(strcmp(name->as.str_val, "Alice") == 0, "name should be 'Alice'");
    TEST_ASSERT(age->as.int_val == 30, "age should be 30");
    TEST_ASSERT(height->as.float_val > 5.5 && height->as.float_val < 5.7, "height should be ~5.6");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 5: Multiple struct instances */
static int test_multiple_struct_instances() {
    const char *code = 
        "struct Point {\n"
        "    x: int,\n"
        "    y: int\n"
        "}\n"
        "let p1 = Point { x: 1, y: 2 }\n"
        "let p2 = Point { x: 3, y: 4 }\n"
        "let p3 = Point { x: 5, y: 6 }\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed with multiple instances");
    
    Value *p1 = env_get(interp->global_env, "p1");
    Value *p2 = env_get(interp->global_env, "p2");
    Value *p3 = env_get(interp->global_env, "p3");
    
    TEST_ASSERT(p1 != NULL && p1->type == VAL_STRUCT, "p1 should be struct");
    TEST_ASSERT(p2 != NULL && p2->type == VAL_STRUCT, "p2 should be struct");
    TEST_ASSERT(p3 != NULL && p3->type == VAL_STRUCT, "p3 should be struct");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 6: Union declaration and usage */
static int test_union_basic() {
    const char *code = 
        "union Data {\n"
        "    i: int,\n"
        "    f: float\n"
        "}\n"
        "let d = Data { i: 42 }\n"
        "let val = d.i\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed with union");
    
    Value *val = env_get(interp->global_env, "val");
    TEST_ASSERT(val != NULL, "val not found");
    TEST_ASSERT(val->type == VAL_INT, "val should be INT");
    TEST_ASSERT(val->as.int_val == 42, "val should equal 42");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 7: Struct with pointer fields */
static int test_struct_with_pointers() {
    const char *code = 
        "struct Node {\n"
        "    value: int\n"
        "}\n"
        "let node = Node { value: 100 }\n"
        "let ptr = &node\n"
        "let val = ptr->value\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed with struct pointers");
    
    Value *val = env_get(interp->global_env, "val");
    TEST_ASSERT(val != NULL, "val not found");
    TEST_ASSERT(val->type == VAL_INT, "val should be INT");
    TEST_ASSERT(val->as.int_val == 100, "val should equal 100");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 8: Struct assignment */
static int test_struct_assignment() {
    const char *code = 
        "struct Point {\n"
        "    x: int,\n"
        "    y: int\n"
        "}\n"
        "let p1 = Point { x: 10, y: 20 }\n"
        "let p2 = p1\n"
        "let x = p2.x\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed with struct assignment");
    
    Value *x = env_get(interp->global_env, "x");
    TEST_ASSERT(x != NULL && x->type == VAL_INT, "x should be INT");
    TEST_ASSERT(x->as.int_val == 10, "x should equal 10");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 9: Empty struct */
static int test_empty_struct() {
    const char *code = 
        "struct Empty {}\n"
        "let e = Empty {}\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed with empty struct");
    
    Value *e = env_get(interp->global_env, "e");
    TEST_ASSERT(e != NULL && e->type == VAL_STRUCT, "e should be struct");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Test 10: Struct with many fields */
static int test_struct_many_fields() {
    const char *code = 
        "struct Complex {\n"
        "    a: int,\n"
        "    b: int,\n"
        "    c: int,\n"
        "    d: int,\n"
        "    e: int\n"
        "}\n"
        "let obj = Complex { a: 1, b: 2, c: 3, d: 4, e: 5 }\n"
        "let sum = obj.a\n";
    
    Interpreter *interp = NULL;
    Value result;
    TEST_ASSERT(run_code(code, &result, &interp), "Failed with many fields");
    
    Value *sum = env_get(interp->global_env, "sum");
    TEST_ASSERT(sum != NULL && sum->type == VAL_INT, "sum should be INT");
    TEST_ASSERT(sum->as.int_val == 1, "sum should equal 1");
    
    value_free(&result);
    interpreter_free(interp);
    return 1;
}

/* Main test runner */
int main() {
    int total = 0, passed = 0, failed = 0;
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║     KLang Struct/Union Unit Tests (Phase 2)          ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    RUN_TEST(test_struct_declaration);
    RUN_TEST(test_struct_initialization);
    RUN_TEST(test_struct_field_access);
    RUN_TEST(test_struct_mixed_types);
    RUN_TEST(test_multiple_struct_instances);
    RUN_TEST(test_union_basic);
    RUN_TEST(test_struct_with_pointers);
    RUN_TEST(test_struct_assignment);
    RUN_TEST(test_empty_struct);
    RUN_TEST(test_struct_many_fields);
    
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
