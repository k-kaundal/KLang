/**
 * @file test_function_calls.c
 * @brief Test function call mechanism
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

void test_simple_function_call() {
    TEST("Simple function call");
    
    VM *vm = vm_v3_new();
    
    // Create a simple function that returns 42
    Instruction func_code[] = {
        {OP_LOAD_I, 10, 0, 0, 42},              // R10 = 42
        {OP_RETURN_VALUE, 10, 0, 0, 0}          // return R10
    };
    
    Function *func = function_new("test_func", func_code, 2);
    func->param_count = 0;
    
    // Call the function directly via vm_v3_call
    Value func_val = value_make_function(func);
    int result = vm_v3_call(vm, &func_val, NULL, 0);
    
    assert(result == 0);
    assert(vm->return_value.type == VALUE_TYPE_INT);
    assert(vm->return_value.data.as_int == 42);
    
    function_free(func);
    vm_v3_free(vm);
    PASS();
}

void test_function_with_arguments() {
    TEST("Function with arguments");
    
    VM *vm = vm_v3_new();
    
    // Function that adds two arguments: R4 + R5 -> R10
    Instruction func_code[] = {
        {OP_ADD_I, 10, 4, 5, 0},                // R10 = R4 + R5
        {OP_RETURN_VALUE, 10, 0, 0, 0}          // return R10
    };
    
    Function *func = function_new("add", func_code, 2);
    func->param_count = 2;
    
    // Set up arguments
    Value args[2];
    args[0] = value_make_int(10);
    args[1] = value_make_int(32);
    
    // Call the function
    Value func_val = value_make_function(func);
    int result = vm_v3_call(vm, &func_val, args, 2);
    
    assert(result == 0);
    assert(vm->return_value.type == VALUE_TYPE_INT);
    assert(vm->return_value.data.as_int == 42);
    
    function_free(func);
    vm_v3_free(vm);
    PASS();
}

void test_nested_function_calls() {
    TEST("Nested function calls");
    
    VM *vm = vm_v3_new();
    
    // Inner function: returns argument * 2
    Instruction inner_code[] = {
        {OP_LOAD_I, 10, 0, 0, 2},               // R10 = 2
        {OP_MUL_I, 11, 4, 10, 0},               // R11 = R4 * R10
        {OP_RETURN_VALUE, 11, 0, 0, 0}          // return R11
    };
    
    Function *inner_func = function_new("double", inner_code, 3);
    inner_func->param_count = 1;
    
    // Call inner function with argument 21
    Value args[1];
    args[0] = value_make_int(21);
    
    Value func_val = value_make_function(inner_func);
    int result = vm_v3_call(vm, &func_val, args, 1);
    
    assert(result == 0);
    assert(vm->return_value.type == VALUE_TYPE_INT);
    assert(vm->return_value.data.as_int == 42);
    
    function_free(inner_func);
    vm_v3_free(vm);
    PASS();
}

void test_builtin_print() {
    TEST("Builtin print function");
    
    VM *vm = vm_v3_new();
    
    Instruction code[] = {
        {OP_LOAD_I, 4, 0, 0, 42},               // R4 = 42
        {OP_LOAD_I, 5, 0, 0, 100},              // R5 = 100
        {OP_CALL_BUILTIN, 0, 0, 2, 4},          // print(R4, R5) -> builtin 0
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm->frame_count = 1;
    
    printf("\n    Expected output: 42 100\n    Actual output:   ");
    int result = vm_v3_execute(vm, code, 4);
    
    assert(result == 0);
    
    vm_v3_free(vm);
    PASS();
}

void test_builtin_len() {
    TEST("Array length via OP_ARRAY_LEN");
    
    VM *vm = vm_v3_new();
    
    Instruction code[] = {
        {OP_NEW_ARRAY, 4, 0, 0, 0},             // R4 = []
        {OP_LOAD_I, 5, 0, 0, 10},               // R5 = 10
        {OP_ARRAY_PUSH, 0, 4, 5, 0},            // R4.push(R5) - src1=4 (array), src2=5 (value)
        {OP_ARRAY_PUSH, 0, 4, 5, 0},            // R4.push(R5)
        {OP_ARRAY_PUSH, 0, 4, 5, 0},            // R4.push(R5)
        {OP_ARRAY_LEN, 6, 4, 0, 0},             // R6 = R4.length - dest=6, src1=4
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm->frame_count = 1;
    CallFrame *frame = &vm->frames[0];
    
    int result = vm_v3_execute(vm, code, 7);
    
    if (result != 0 || vm->has_error) {
        printf("FAIL: execution failed\n");
        tests_failed++;
        vm_v3_free(vm);
        return;
    }
    
    // Get array directly to check it
    Array *arr = (Array*)frame->registers[4].data.as_array;
    int direct_len = array_length(arr);
    
    printf("\n    Direct array length: %d\n", direct_len);
    printf("    R6 value: %ld\n", frame->registers[6].data.as_int);
    
    if (frame->registers[6].type != VALUE_TYPE_INT) {
        printf("FAIL: R6 is not int, type=%d\n", frame->registers[6].type);
        tests_failed++;
        vm_v3_free(vm);
        return;
    }
    
    if (frame->registers[6].data.as_int != 3) {
        printf("FAIL: expected 3, got %ld\n", frame->registers[6].data.as_int);
        tests_failed++;
        vm_v3_free(vm);
        return;
    }
    
    vm_v3_free(vm);
    PASS();
}

void test_builtin_type() {
    TEST("Builtin type function");
    
    VM *vm = vm_v3_new();
    
    Instruction code[] = {
        {OP_LOAD_I, 4, 0, 0, 42},               // R4 = 42
        {OP_CALL_BUILTIN, 5, 2, 1, 4},          // type(R4) -> R5, builtin 2
        {OP_NEW_ARRAY, 6, 0, 0, 0},             // R6 = []
        {OP_CALL_BUILTIN, 7, 2, 1, 6},          // type(R6) -> R7, builtin 2
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm->frame_count = 1;
    CallFrame *frame = &vm->frames[0];
    
    int result = vm_v3_execute(vm, code, 5);
    assert(result == 0);
    
    assert(frame->registers[5].type == VALUE_TYPE_STRING);
    assert(strcmp(frame->registers[5].data.as_string, "int") == 0);
    
    assert(frame->registers[7].type == VALUE_TYPE_STRING);
    assert(strcmp(frame->registers[7].data.as_string, "array") == 0);
    
    vm_v3_free(vm);
    PASS();
}

void test_closure_with_upvalues() {
    TEST("Closure with upvalues");
    
    VM *vm = vm_v3_new();
    
    // Function that uses upvalue[0] and adds argument
    Instruction func_code[] = {
        {OP_LOAD_UPVALUE, 10, 0, 0, 0},         // R10 = upvalue[0]
        {OP_ADD_I, 11, 10, 4, 0},               // R11 = R10 + R4
        {OP_RETURN_VALUE, 11, 0, 0, 0}          // return R11
    };
    
    Function *func = function_new("add_upval", func_code, 3);
    func->param_count = 1;
    
    // Create closure with upvalue
    Closure *closure = closure_new(func);
    Value upval = value_make_int(30);
    closure_set_upvalue(closure, 0, &upval);
    
    // Call closure with argument 12
    Value args[1];
    args[0] = value_make_int(12);
    
    Value closure_val = value_make_closure(closure);
    int result = vm_v3_call(vm, &closure_val, args, 1);
    
    assert(result == 0);
    assert(vm->return_value.type == VALUE_TYPE_INT);
    assert(vm->return_value.data.as_int == 42);
    
    closure_free(closure);
    vm_v3_free(vm);
    PASS();
}

void test_stack_overflow_protection() {
    TEST("Stack overflow protection");
    
    VM *vm = vm_v3_new();
    
    // Try to exceed MAX_FRAMES
    Function *func = function_new("recursive", NULL, 0);
    func->bytecode = NULL;  // Will cause error
    func->param_count = 0;
    
    // Manually fill up frames
    for (int i = 0; i < MAX_FRAMES - 1; i++) {
        vm->frame_count++;
    }
    
    Value func_val = value_make_function(func);
    int result = vm_v3_call(vm, &func_val, NULL, 0);
    
    // Should fail with stack overflow
    assert(result == -1);
    assert(vm->has_error);
    
    function_free(func);
    vm_v3_free(vm);
    PASS();
}

int main() {
    printf("=== Function Call Test Suite ===\n\n");
    
    test_simple_function_call();
    test_function_with_arguments();
    test_nested_function_calls();
    test_builtin_print();
    test_builtin_len();
    test_builtin_type();
    test_closure_with_upvalues();
    test_stack_overflow_protection();
    
    printf("\n=== Test Results ===\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("Total:  %d\n", tests_passed + tests_failed);
    
    return tests_failed == 0 ? 0 : 1;
}
