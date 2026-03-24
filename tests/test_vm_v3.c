/**
 * @file test_vm_v3.c
 * @brief Test suite for VM V3 register-based VM
 */

#include "../src/vm_v3.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* Test counters */
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) \
    printf("\n--- Running test: %s ---\n", name); \
    if (1)

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) == (b)) { \
            printf("✓ PASS: %s == %s\n", #a, #b); \
            tests_passed++; \
        } else { \
            printf("✗ FAIL: %s (%lld) != %s (%lld)\n", #a, (long long)(a), #b, (long long)(b)); \
            tests_failed++; \
        } \
    } while (0)

#define ASSERT_TRUE(expr) \
    do { \
        if (expr) { \
            printf("✓ PASS: %s is true\n", #expr); \
            tests_passed++; \
        } else { \
            printf("✗ FAIL: %s is false\n", #expr); \
            tests_failed++; \
        } \
    } while (0)

#define ASSERT_FALSE(expr) \
    do { \
        if (!(expr)) { \
            printf("✓ PASS: %s is false\n", #expr); \
            tests_passed++; \
        } else { \
            printf("✗ FAIL: %s is true\n", #expr); \
            tests_failed++; \
        } \
    } while (0)

/* Test: Basic VM creation and cleanup */
void test_vm_creation() {
    TEST("VM Creation") {
        VM *vm = vm_v3_new();
        ASSERT_TRUE(vm != NULL);
        ASSERT_EQ(vm->frame_count, 0);
        ASSERT_FALSE(vm->running);
        vm_v3_free(vm);
    }
}

/* Test: Value operations */
void test_value_operations() {
    TEST("Value Operations") {
        // Integer
        Value v_int = value_make_int(42);
        ASSERT_EQ(v_int.type, VALUE_TYPE_INT);
        ASSERT_EQ(v_int.data.as_int, 42);
        
        // Float
        Value v_float = value_make_float(3.14);
        ASSERT_EQ(v_float.type, VALUE_TYPE_FLOAT);
        ASSERT_TRUE(v_float.data.as_float == 3.14);
        
        // Boolean
        Value v_bool = value_make_bool(true);
        ASSERT_EQ(v_bool.type, VALUE_TYPE_BOOL);
        ASSERT_TRUE(v_bool.data.as_bool);
        
        // Null
        Value v_null = value_make_null();
        ASSERT_EQ(v_null.type, VALUE_TYPE_NULL);
        
        // String
        Value v_str = value_make_string("hello");
        ASSERT_EQ(v_str.type, VALUE_TYPE_STRING);
        ASSERT_TRUE(strcmp(v_str.data.as_string, "hello") == 0);
        value_free(&v_str);
    }
}

/* Test: Value equality */
void test_value_equality() {
    TEST("Value Equality") {
        Value a = value_make_int(10);
        Value b = value_make_int(10);
        Value c = value_make_int(20);
        
        ASSERT_TRUE(value_equals(&a, &b));
        ASSERT_FALSE(value_equals(&a, &c));
        
        Value s1 = value_make_string("test");
        Value s2 = value_make_string("test");
        Value s3 = value_make_string("other");
        
        ASSERT_TRUE(value_equals(&s1, &s2));
        ASSERT_FALSE(value_equals(&s1, &s3));
        
        value_free(&s1);
        value_free(&s2);
        value_free(&s3);
    }
}

/* Test: Constant pool */
void test_constant_pool() {
    TEST("Constant Pool") {
        ConstantPool *pool = constant_pool_new();
        ASSERT_TRUE(pool != NULL);
        
        // Add constants
        Value v1 = value_make_int(100);
        Value v2 = value_make_float(2.5);
        Value v3 = value_make_string("constant");
        
        int idx1 = constant_pool_add(pool, v1);
        int idx2 = constant_pool_add(pool, v2);
        int idx3 = constant_pool_add(pool, v3);
        
        ASSERT_EQ(idx1, 0);
        ASSERT_EQ(idx2, 1);
        ASSERT_EQ(idx3, 2);
        ASSERT_EQ(pool->count, 3);
        
        // Retrieve constants
        Value r1 = constant_pool_get(pool, idx1);
        ASSERT_EQ(r1.data.as_int, 100);
        
        Value r2 = constant_pool_get(pool, idx2);
        ASSERT_TRUE(r2.data.as_float == 2.5);
        
        // Deduplication test
        Value v4 = value_make_int(100);
        int idx4 = constant_pool_add(pool, v4);
        ASSERT_EQ(idx4, idx1); // Should return existing index
        ASSERT_EQ(pool->count, 3); // Count shouldn't increase
        
        constant_pool_free(pool);
    }
}

/* Test: Global variables */
void test_global_variables() {
    TEST("Global Variables") {
        GlobalTable table;
        global_table_init(&table);
        
        Value v1 = value_make_int(42);
        Value v2 = value_make_string("test");
        
        (void)global_table_set(&table, "x", v1);
        (void)global_table_set(&table, "name", v2);
        
        ASSERT_EQ(table.count, 2);
        
        bool found;
        Value result = global_table_get(&table, "x", &found);
        ASSERT_TRUE(found);
        ASSERT_EQ(result.data.as_int, 42);
        
        result = global_table_get(&table, "name", &found);
        ASSERT_TRUE(found);
        ASSERT_TRUE(strcmp(result.data.as_string, "test") == 0);
        
        result = global_table_get(&table, "nonexistent", &found);
        ASSERT_FALSE(found);
        
        global_table_free(&table);
    }
}

/* Test: Simple arithmetic */
void test_arithmetic() {
    TEST("Arithmetic Operations") {
        VM *vm = vm_v3_new();
        
        // Test program: R10 = 5 + 3
        Instruction program[] = {
            {OP_LOAD_I, 10, 0, 0, 5},       // R10 = 5
            {OP_LOAD_I, 11, 0, 0, 3},       // R11 = 3
            {OP_ADD_I, 12, 10, 11, 0},      // R12 = R10 + R11
            {OP_HALT, 0, 0, 0, 0}
        };
        
        int result = vm_v3_execute(vm, program, 4);
        ASSERT_EQ(result, 0);
        ASSERT_EQ(vm->frames[0].registers[12].data.as_int, 8);
        
        vm_v3_free(vm);
    }
}

/* Test: Multiplication and subtraction */
void test_mul_sub() {
    TEST("Multiplication and Subtraction") {
        VM *vm = vm_v3_new();
        
        // Test: (10 * 5) - 3 = 47
        Instruction program[] = {
            {OP_LOAD_I, 10, 0, 0, 10},      // R10 = 10
            {OP_LOAD_I, 11, 0, 0, 5},       // R11 = 5
            {OP_MUL_I, 12, 10, 11, 0},      // R12 = R10 * R11 = 50
            {OP_LOAD_I, 13, 0, 0, 3},       // R13 = 3
            {OP_SUB_I, 14, 12, 13, 0},      // R14 = R12 - R13 = 47
            {OP_HALT, 0, 0, 0, 0}
        };
        
        int result = vm_v3_execute(vm, program, 6);
        ASSERT_EQ(result, 0);
        ASSERT_EQ(vm->frames[0].registers[14].data.as_int, 47);
        
        vm_v3_free(vm);
    }
}

/* Test: Immediate operations */
void test_immediate_operations() {
    TEST("Immediate Operations") {
        VM *vm = vm_v3_new();
        
        // Test: R10 = 10 + 5 (immediate)
        Instruction program[] = {
            {OP_LOAD_I, 10, 0, 0, 10},      // R10 = 10
            {OP_ADD_IMM, 11, 10, 0, 5},     // R11 = R10 + 5
            {OP_MUL_IMM, 12, 11, 0, 2},     // R12 = R11 * 2
            {OP_HALT, 0, 0, 0, 0}
        };
        
        int result = vm_v3_execute(vm, program, 4);
        ASSERT_EQ(result, 0);
        ASSERT_EQ(vm->frames[0].registers[11].data.as_int, 15);
        ASSERT_EQ(vm->frames[0].registers[12].data.as_int, 30);
        
        vm_v3_free(vm);
    }
}

/* Test: Comparison operations */
void test_comparisons() {
    TEST("Comparison Operations") {
        VM *vm = vm_v3_new();
        
        Instruction program[] = {
            {OP_LOAD_I, 10, 0, 0, 5},       // R10 = 5
            {OP_LOAD_I, 11, 0, 0, 3},       // R11 = 3
            {OP_GT_I, 12, 10, 11, 0},       // R12 = R10 > R11
            {OP_LT_I, 13, 10, 11, 0},       // R13 = R10 < R11
            {OP_EQ_I, 14, 10, 11, 0},       // R14 = R10 == R11
            {OP_HALT, 0, 0, 0, 0}
        };
        
        int result = vm_v3_execute(vm, program, 6);
        ASSERT_EQ(result, 0);
        ASSERT_TRUE(vm->frames[0].registers[12].data.as_bool);   // 5 > 3 = true
        ASSERT_FALSE(vm->frames[0].registers[13].data.as_bool);  // 5 < 3 = false
        ASSERT_FALSE(vm->frames[0].registers[14].data.as_bool);  // 5 == 3 = false
        
        vm_v3_free(vm);
    }
}

/* Test: Float operations */
void test_float_operations() {
    TEST("Float Operations") {
        VM *vm = vm_v3_new();
        
        // Manually create float values (LOAD_F with immediate is simplified)
        Instruction program[] = {
            {OP_LOAD_I, 10, 0, 0, 3},       // R10 = 3
            {OP_LOAD_I, 11, 0, 0, 2},       // R11 = 2
            {OP_INT_TO_FLOAT, 12, 10, 0, 0}, // R12 = float(R10)
            {OP_INT_TO_FLOAT, 13, 11, 0, 0}, // R13 = float(R11)
            {OP_DIV_F, 14, 12, 13, 0},      // R14 = R12 / R13 = 1.5
            {OP_HALT, 0, 0, 0, 0}
        };
        
        int result = vm_v3_execute(vm, program, 6);
        ASSERT_EQ(result, 0);
        ASSERT_TRUE(vm->frames[0].registers[14].data.as_float == 1.5);
        
        vm_v3_free(vm);
    }
}

/* Test: Logical operations */
void test_logical_operations() {
    TEST("Logical Operations") {
        VM *vm = vm_v3_new();
        
        Instruction program[] = {
            {OP_LOAD_TRUE, 10, 0, 0, 0},    // R10 = true
            {OP_LOAD_FALSE, 11, 0, 0, 0},   // R11 = false
            {OP_AND_B, 12, 10, 11, 0},      // R12 = R10 && R11
            {OP_OR_B, 13, 10, 11, 0},       // R13 = R10 || R11
            {OP_NOT_B, 14, 10, 0, 0},       // R14 = !R10
            {OP_HALT, 0, 0, 0, 0}
        };
        
        int result = vm_v3_execute(vm, program, 6);
        ASSERT_EQ(result, 0);
        ASSERT_FALSE(vm->frames[0].registers[12].data.as_bool); // true && false = false
        ASSERT_TRUE(vm->frames[0].registers[13].data.as_bool);  // true || false = true
        ASSERT_FALSE(vm->frames[0].registers[14].data.as_bool); // !true = false
        
        vm_v3_free(vm);
    }
}

/* Test: Jump operations */
void test_jumps() {
    TEST("Jump Operations") {
        VM *vm = vm_v3_new();
        
        // Test conditional jump
        Instruction program[] = {
            {OP_LOAD_TRUE, 10, 0, 0, 0},    // 0: R10 = true
            {OP_JUMP_IF_TRUE, 0, 10, 0, 4}, // 1: if R10 jump to 4
            {OP_LOAD_I, 11, 0, 0, 99},      // 2: R11 = 99 (skipped)
            {OP_JUMP, 0, 0, 0, 5},          // 3: jump to 5
            {OP_LOAD_I, 11, 0, 0, 42},      // 4: R11 = 42 (executed)
            {OP_HALT, 0, 0, 0, 0}           // 5: halt
        };
        
        int result = vm_v3_execute(vm, program, 6);
        ASSERT_EQ(result, 0);
        ASSERT_EQ(vm->frames[0].registers[11].data.as_int, 42);
        
        vm_v3_free(vm);
    }
}

/* Test: String operations */
void test_string_operations() {
    TEST("String Operations") {
        VM *vm = vm_v3_new();
        ConstantPool *pool = constant_pool_new();
        
        // Add strings to constant pool
        Value s1 = value_make_string("Hello");
        Value s2 = value_make_string(" World");
        int idx1 = constant_pool_add(pool, s1);
        int idx2 = constant_pool_add(pool, s2);
        
        Instruction program[] = {
            {OP_LOAD_CONST, 10, 0, 0, idx1}, // R10 = "Hello"
            {OP_LOAD_CONST, 11, 0, 0, idx2}, // R11 = " World"
            {OP_STR_CONCAT, 12, 10, 11, 0},  // R12 = R10 + R11
            {OP_STR_LEN, 13, 12, 0, 0},      // R13 = len(R12)
            {OP_HALT, 0, 0, 0, 0}
        };
        
        // Set up constant pool for execution
        vm->frame_count = 1;
        vm->frames[0].constants = pool;
        
        int result = vm_v3_execute(vm, program, 5);
        ASSERT_EQ(result, 0);
        
        CallFrame *frame = &vm->frames[0];
        ASSERT_TRUE(strcmp(frame->registers[12].data.as_string, "Hello World") == 0);
        ASSERT_EQ(frame->registers[13].data.as_int, 11);
        
        // Clean up
        value_free(&frame->registers[12]);
        constant_pool_free(pool);
        vm_v3_free(vm);
    }
}

/* Test: Type conversions */
void test_type_conversions() {
    TEST("Type Conversions") {
        VM *vm = vm_v3_new();
        
        Instruction program[] = {
            {OP_LOAD_I, 10, 0, 0, 42},      // R10 = 42
            {OP_INT_TO_FLOAT, 11, 10, 0, 0}, // R11 = float(42)
            {OP_FLOAT_TO_INT, 12, 11, 0, 0}, // R12 = int(R11)
            {OP_INT_TO_STR, 13, 10, 0, 0},  // R13 = "42"
            {OP_HALT, 0, 0, 0, 0}
        };
        
        int result = vm_v3_execute(vm, program, 5);
        ASSERT_EQ(result, 0);
        ASSERT_TRUE(vm->frames[0].registers[11].data.as_float == 42.0);
        ASSERT_EQ(vm->frames[0].registers[12].data.as_int, 42);
        ASSERT_TRUE(strcmp(vm->frames[0].registers[13].data.as_string, "42") == 0);
        
        value_free(&vm->frames[0].registers[13]);
        vm_v3_free(vm);
    }
}

/* Test: Move and swap operations */
void test_move_swap() {
    TEST("Move and Swap Operations") {
        VM *vm = vm_v3_new();
        
        Instruction program[] = {
            {OP_LOAD_I, 10, 0, 0, 100},     // R10 = 100
            {OP_LOAD_I, 11, 0, 0, 200},     // R11 = 200
            {OP_MOVE, 12, 10, 0, 0},        // R12 = R10
            {OP_SWAP, 10, 11, 0, 0},        // swap(R10, R11)
            {OP_HALT, 0, 0, 0, 0}
        };
        
        int result = vm_v3_execute(vm, program, 5);
        ASSERT_EQ(result, 0);
        ASSERT_EQ(vm->frames[0].registers[12].data.as_int, 100);
        ASSERT_EQ(vm->frames[0].registers[10].data.as_int, 200); // swapped
        ASSERT_EQ(vm->frames[0].registers[11].data.as_int, 100); // swapped
        
        vm_v3_free(vm);
    }
}

/* Main test runner */
int main(void) {
    printf("===========================================\n");
    printf("  KLang VM V3 Test Suite\n");
    printf("===========================================\n");
    
    test_vm_creation();
    test_value_operations();
    test_value_equality();
    test_constant_pool();
    test_global_variables();
    test_arithmetic();
    test_mul_sub();
    test_immediate_operations();
    test_comparisons();
    test_float_operations();
    test_logical_operations();
    test_jumps();
    test_string_operations();
    test_type_conversions();
    test_move_swap();
    
    printf("\n===========================================\n");
    printf("  Test Results\n");
    printf("===========================================\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    printf("Total tests:  %d\n", tests_passed + tests_failed);
    
    if (tests_failed == 0) {
        printf("\n✓ All tests passed!\n");
        return 0;
    } else {
        printf("\n✗ Some tests failed.\n");
        return 1;
    }
}
