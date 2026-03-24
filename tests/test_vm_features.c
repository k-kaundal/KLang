/**
 * @file test_vm_features.c
 * @brief Test new VM features: objects, arrays, exceptions
 */

#include "../src/vm_register.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) \
    printf("Testing: %s... ", name); \
    fflush(stdout);

#define PASS() \
    printf("PASS\n"); \
    tests_passed++;

#define FAIL(msg) \
    printf("FAIL: %s\n", msg); \
    tests_failed++;

void test_object_create() {
    TEST("Object creation and basic operations");
    
    Object *obj = object_new();
    assert(obj != NULL);
    assert(object_size(obj) == 0);
    
    object_set(obj, "name", value_make_string("KLang"));
    object_set(obj, "version", value_make_int(3));
    
    assert(object_size(obj) == 2);
    assert(object_has(obj, "name"));
    assert(object_has(obj, "version"));
    assert(!object_has(obj, "nonexistent"));
    
    bool found;
    Value name = object_get(obj, "name", &found);
    assert(found);
    assert(name.type == VALUE_TYPE_STRING);
    assert(strcmp(name.data.as_string, "KLang") == 0);
    
    object_free(obj);
    PASS();
}

void test_array_operations() {
    TEST("Array operations");
    
    Array *arr = array_new();
    assert(arr != NULL);
    assert(array_length(arr) == 0);
    
    array_push(arr, value_make_int(10));
    array_push(arr, value_make_int(20));
    array_push(arr, value_make_int(30));
    
    assert(array_length(arr) == 3);
    
    bool ok;
    Value val = array_get(arr, 1, &ok);
    assert(ok);
    assert(val.type == VALUE_TYPE_INT);
    assert(val.data.as_int == 20);
    
    val = array_pop(arr, &ok);
    assert(ok);
    assert(val.data.as_int == 30);
    assert(array_length(arr) == 2);
    
    array_free(arr);
    PASS();
}

void test_vm_object_opcodes() {
    TEST("VM object opcodes");
    
    VM *vm = vm_v3_new();
    ConstantPool *pool = constant_pool_new();
    
    // Add field name to constant pool
    constant_pool_add(pool, value_make_string("x"));
    constant_pool_add(pool, value_make_string("y"));
    
    Instruction code[] = {
        {OP_NEW_OBJECT, 0, 0, 0, 0},           // R0 = {}
        {OP_LOAD_I, 1, 0, 0, 42},               // R1 = 42
        {OP_SET_FIELD, 0, 1, 0, 0},             // R0.x = R1 (key from pool[0])
        {OP_GET_FIELD, 2, 0, 0, 0},             // R2 = R0.x
        {OP_HAS_FIELD, 3, 0, 0, 0},             // R3 = has(R0, x)
        {OP_TYPEOF, 4, 0, 0, 0},                // R4 = typeof(R0)
        {OP_HALT, 0, 0, 0, 0}
    };
    
    CallFrame *frame = &vm->frames[0];
    frame->constants = pool;
    vm->frame_count = 1;
    
    int result = vm_v3_execute(vm, code, 7);
    assert(result == 0);
    
    // Check R2 got the value
    assert(frame->registers[2].type == VALUE_TYPE_INT);
    assert(frame->registers[2].data.as_int == 42);
    
    // Check R3 (has field)
    assert(frame->registers[3].type == VALUE_TYPE_BOOL);
    assert(frame->registers[3].data.as_bool == true);
    
    // Check R4 (typeof)
    assert(frame->registers[4].type == VALUE_TYPE_STRING);
    assert(strcmp(frame->registers[4].data.as_string, "object") == 0);
    
    constant_pool_free(pool);
    vm_v3_free(vm);
    PASS();
}

void test_vm_array_opcodes() {
    TEST("VM array opcodes");
    
    VM *vm = vm_v3_new();
    
    Instruction code[] = {
        {OP_NEW_ARRAY, 0, 0, 0, 0},            // R0 = []
        {OP_LOAD_I, 1, 0, 0, 10},               // R1 = 10
        {OP_LOAD_I, 2, 0, 0, 20},               // R2 = 20
        {OP_ARRAY_PUSH, 0, 0, 1, 0},            // R0.push(R1)
        {OP_ARRAY_PUSH, 0, 0, 2, 0},            // R0.push(R2)
        {OP_ARRAY_LEN, 3, 0, 0, 0},             // R3 = R0.length
        {OP_LOAD_I, 4, 0, 0, 0},                // R4 = 0 (index)
        {OP_ARRAY_GET, 5, 0, 4, 0},             // R5 = R0[R4]
        {OP_ARRAY_POP, 6, 0, 0, 0},             // R6 = R0.pop()
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm->frame_count = 1;
    CallFrame *frame = &vm->frames[0];
    
    int result = vm_v3_execute(vm, code, 10);
    assert(result == 0);
    
    // Check length
    assert(frame->registers[3].type == VALUE_TYPE_INT);
    assert(frame->registers[3].data.as_int == 2);
    
    // Check get
    assert(frame->registers[5].type == VALUE_TYPE_INT);
    assert(frame->registers[5].data.as_int == 10);
    
    // Check pop
    assert(frame->registers[6].type == VALUE_TYPE_INT);
    assert(frame->registers[6].data.as_int == 20);
    
    vm_v3_free(vm);
    PASS();
}

void test_exception_handling() {
    TEST("Exception handling opcodes");
    
    VM *vm = vm_v3_new();
    
    Instruction code[] = {
        {OP_TRY, 0, 0, 0, 5},                  // Try, catch at ip=5
        {OP_LOAD_I, 0, 0, 0, 42},               // R0 = 42
        {OP_JUMP, 0, 0, 0, 7},                  // Jump over catch
        {OP_HALT, 0, 0, 0, 0},                  // Should not reach
        {OP_HALT, 0, 0, 0, 0},                  // Should not reach
        {OP_CATCH, 1, 0, 0, 0},                 // R1 = exception
        {OP_LOAD_I, 2, 0, 0, 99},               // R2 = 99 (error code)
        {OP_END_TRY, 0, 0, 0, 0},               // End try-catch
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm->frame_count = 1;
    int result = vm_v3_execute(vm, code, 9);
    assert(result == 0);
    
    // Should have executed normally
    CallFrame *frame = &vm->frames[0];
    assert(frame->registers[0].type == VALUE_TYPE_INT);
    assert(frame->registers[0].data.as_int == 42);
    
    vm_v3_free(vm);
    PASS();
}

void test_typeof_opcode() {
    TEST("TYPEOF opcode for all types");
    
    VM *vm = vm_v3_new();
    
    Instruction code[] = {
        {OP_LOAD_I, 0, 0, 0, 42},               // R0 = int
        {OP_TYPEOF, 1, 0, 0, 0},                // R1 = typeof(R0)
        
        {OP_LOAD_F, 2, 0, 0, 0},                // R2 = float
        {OP_TYPEOF, 3, 2, 0, 0},                // R3 = typeof(R2)
        
        {OP_LOAD_TRUE, 4, 0, 0, 0},             // R4 = bool
        {OP_TYPEOF, 5, 4, 0, 0},                // R5 = typeof(R4)
        
        {OP_NEW_ARRAY, 6, 0, 0, 0},             // R6 = array
        {OP_TYPEOF, 7, 6, 0, 0},                // R7 = typeof(R6)
        
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm->frame_count = 1;
    CallFrame *frame = &vm->frames[0];
    
    int result = vm_v3_execute(vm, code, 9);
    assert(result == 0);
    
    assert(strcmp(frame->registers[1].data.as_string, "int") == 0);
    assert(strcmp(frame->registers[3].data.as_string, "float") == 0);
    assert(strcmp(frame->registers[5].data.as_string, "bool") == 0);
    assert(strcmp(frame->registers[7].data.as_string, "array") == 0);
    
    vm_v3_free(vm);
    PASS();
}

void test_string_conversions() {
    TEST("String conversion opcodes");
    
    VM *vm = vm_v3_new();
    
    Instruction code[] = {
        {OP_LOAD_I, 0, 0, 0, 123},              // R0 = 123
        {OP_INT_TO_STR, 1, 0, 0, 0},            // R1 = "123"
        {OP_STR_TO_INT, 2, 1, 0, 0},            // R2 = 123
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm->frame_count = 1;
    CallFrame *frame = &vm->frames[0];
    
    int result = vm_v3_execute(vm, code, 4);
    assert(result == 0);
    
    assert(frame->registers[1].type == VALUE_TYPE_STRING);
    assert(strcmp(frame->registers[1].data.as_string, "123") == 0);
    
    assert(frame->registers[2].type == VALUE_TYPE_INT);
    assert(frame->registers[2].data.as_int == 123);
    
    vm_v3_free(vm);
    PASS();
}

int main() {
    printf("=== VM Features Test Suite ===\n\n");
    
    test_object_create();
    test_array_operations();
    test_vm_object_opcodes();
    test_vm_array_opcodes();
    test_exception_handling();
    test_typeof_opcode();
    test_string_conversions();
    
    printf("\n=== Test Results ===\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("Total:  %d\n", tests_passed + tests_failed);
    
    return tests_failed == 0 ? 0 : 1;
}
