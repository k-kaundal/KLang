/**
 * @file example_vm_v3.c
 * @brief Example programs demonstrating VM V3 usage
 */

#include "../src/vm_register.h"
#include <stdio.h>

/**
 * @brief Example 1: Simple arithmetic calculation
 * Calculates: result = (a + b) * c
 */
void example_arithmetic() {
    printf("\n=== Example 1: Arithmetic ===\n");
    printf("Calculating: (10 + 5) * 3\n\n");
    
    VM *vm = vm_v3_new();
    
    Instruction program[] = {
        {OP_LOAD_I, 10, 0, 0, 10},      // R10 = 10
        {OP_LOAD_I, 11, 0, 0, 5},       // R11 = 5
        {OP_ADD_I, 12, 10, 11, 0},      // R12 = R10 + R11 = 15
        {OP_LOAD_I, 13, 0, 0, 3},       // R13 = 3
        {OP_MUL_I, 14, 12, 13, 0},      // R14 = R12 * R13 = 45
        {OP_PRINT_REG, 14, 0, 0, 0},    // Print R14
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm_v3_execute(vm, program, 7);
    printf("Result stored in R14: %lld\n", 
           (long long)vm->frames[0].registers[14].data.as_int);
    
    vm_v3_free(vm);
}

/**
 * @brief Example 2: Conditional logic
 * Implements: if (x > y) result = 100 else result = 200
 */
void example_conditional() {
    printf("\n=== Example 2: Conditional Logic ===\n");
    printf("if (15 > 10) result = 100 else result = 200\n\n");
    
    VM *vm = vm_v3_new();
    
    Instruction program[] = {
        {OP_LOAD_I, 10, 0, 0, 15},      // R10 = 15
        {OP_LOAD_I, 11, 0, 0, 10},      // R11 = 10
        {OP_GT_I, 12, 10, 11, 0},       // R12 = (R10 > R11)
        {OP_JUMP_IF_FALSE, 0, 12, 0, 6}, // if !R12 jump to 6
        {OP_LOAD_I, 20, 0, 0, 100},     // R20 = 100 (true branch)
        {OP_JUMP, 0, 0, 0, 7},          // jump to end
        {OP_LOAD_I, 20, 0, 0, 200},     // R20 = 200 (false branch)
        {OP_PRINT_REG, 20, 0, 0, 0},    // Print R20
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm_v3_execute(vm, program, 9);
    printf("Result: %lld (condition was true)\n", 
           (long long)vm->frames[0].registers[20].data.as_int);
    
    vm_v3_free(vm);
}

/**
 * @brief Example 3: Using constants
 */
void example_constants() {
    printf("\n=== Example 3: Constant Pool ===\n");
    printf("Loading and using constants from pool\n\n");
    
    VM *vm = vm_v3_new();
    ConstantPool *pool = constant_pool_new();
    
    // Add constants to pool
    Value pi = value_make_float(3.14159);
    Value msg = value_make_string("Hello from VM V3!");
    
    int pi_idx = constant_pool_add(pool, pi);
    int msg_idx = constant_pool_add(pool, msg);
    
    printf("Added constant 'pi' at index %d\n", pi_idx);
    printf("Added constant 'msg' at index %d\n\n", msg_idx);
    
    Instruction program[] = {
        {OP_LOAD_CONST, 10, 0, 0, pi_idx},  // R10 = pi
        {OP_LOAD_CONST, 11, 0, 0, msg_idx}, // R11 = "Hello..."
        {OP_LOAD_I, 12, 0, 0, 2},           // R12 = 2
        {OP_INT_TO_FLOAT, 13, 12, 0, 0},    // R13 = 2.0
        {OP_MUL_F, 14, 10, 13, 0},          // R14 = pi * 2
        {OP_PRINT_REG, 10, 0, 0, 0},        // Print pi
        {OP_PRINT_REG, 11, 0, 0, 0},        // Print message
        {OP_PRINT_REG, 14, 0, 0, 0},        // Print pi * 2
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm->frame_count = 1;
    vm->frames[0].constants = pool;
    
    vm_v3_execute(vm, program, 9);
    
    printf("\nFinal value (pi * 2): %.5f\n", 
           vm->frames[0].registers[14].data.as_float);
    
    constant_pool_free(pool);
    vm_v3_free(vm);
}

/**
 * @brief Example 4: String operations
 */
void example_strings() {
    printf("\n=== Example 4: String Operations ===\n");
    printf("Concatenating strings and getting length\n\n");
    
    VM *vm = vm_v3_new();
    ConstantPool *pool = constant_pool_new();
    
    Value hello = value_make_string("Hello, ");
    Value world = value_make_string("World!");
    
    int hello_idx = constant_pool_add(pool, hello);
    int world_idx = constant_pool_add(pool, world);
    
    Instruction program[] = {
        {OP_LOAD_CONST, 10, 0, 0, hello_idx}, // R10 = "Hello, "
        {OP_LOAD_CONST, 11, 0, 0, world_idx}, // R11 = "World!"
        {OP_STR_CONCAT, 12, 10, 11, 0},       // R12 = R10 + R11
        {OP_STR_LEN, 13, 12, 0, 0},           // R13 = len(R12)
        {OP_PRINT_REG, 12, 0, 0, 0},          // Print concatenated
        {OP_PRINT_REG, 13, 0, 0, 0},          // Print length
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm->frame_count = 1;
    vm->frames[0].constants = pool;
    
    vm_v3_execute(vm, program, 7);
    
    printf("\nString: %s\n", vm->frames[0].registers[12].data.as_string);
    printf("Length: %lld\n", (long long)vm->frames[0].registers[13].data.as_int);
    
    // Clean up
    value_free(&vm->frames[0].registers[12]);
    constant_pool_free(pool);
    vm_v3_free(vm);
}

/**
 * @brief Example 5: Type conversions
 */
void example_conversions() {
    printf("\n=== Example 5: Type Conversions ===\n");
    printf("Converting between int, float, and string\n\n");
    
    VM *vm = vm_v3_new();
    
    Instruction program[] = {
        {OP_LOAD_I, 10, 0, 0, 42},          // R10 = 42 (int)
        {OP_INT_TO_FLOAT, 11, 10, 0, 0},    // R11 = 42.0 (float)
        {OP_INT_TO_STR, 12, 10, 0, 0},      // R12 = "42" (string)
        {OP_PRINT_REG, 10, 0, 0, 0},        // Print int
        {OP_PRINT_REG, 11, 0, 0, 0},        // Print float
        {OP_PRINT_REG, 12, 0, 0, 0},        // Print string
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm_v3_execute(vm, program, 7);
    
    printf("\nOriginal int: %lld\n", 
           (long long)vm->frames[0].registers[10].data.as_int);
    printf("As float: %.1f\n", 
           vm->frames[0].registers[11].data.as_float);
    printf("As string: %s\n", 
           vm->frames[0].registers[12].data.as_string);
    
    value_free(&vm->frames[0].registers[12]);
    vm_v3_free(vm);
}

/**
 * @brief Example 6: Register operations
 */
void example_registers() {
    printf("\n=== Example 6: Register Operations ===\n");
    printf("Moving and swapping register values\n\n");
    
    VM *vm = vm_v3_new();
    
    Instruction program[] = {
        {OP_LOAD_I, 10, 0, 0, 100},     // R10 = 100
        {OP_LOAD_I, 11, 0, 0, 200},     // R11 = 200
        {OP_MOVE, 12, 10, 0, 0},        // R12 = R10 (copy)
        {OP_PRINT_REG, 10, 0, 0, 0},    // Print R10
        {OP_PRINT_REG, 11, 0, 0, 0},    // Print R11
        {OP_PRINT_REG, 12, 0, 0, 0},    // Print R12
        {OP_SWAP, 10, 11, 0, 0},        // Swap R10 and R11
        {OP_PRINT_REG, 10, 0, 0, 0},    // Print R10 (now 200)
        {OP_PRINT_REG, 11, 0, 0, 0},    // Print R11 (now 100)
        {OP_HALT, 0, 0, 0, 0}
    };
    
    vm_v3_execute(vm, program, 10);
    
    printf("\nAfter swap:\n");
    printf("R10 = %lld (was 100)\n", 
           (long long)vm->frames[0].registers[10].data.as_int);
    printf("R11 = %lld (was 200)\n", 
           (long long)vm->frames[0].registers[11].data.as_int);
    printf("R12 = %lld (unchanged)\n", 
           (long long)vm->frames[0].registers[12].data.as_int);
    
    vm_v3_free(vm);
}

int main(void) {
    printf("=================================================\n");
    printf("  KLang VM V3 - Usage Examples\n");
    printf("=================================================\n");
    
    example_arithmetic();
    example_conditional();
    example_constants();
    example_strings();
    example_conversions();
    example_registers();
    
    printf("\n=================================================\n");
    printf("  All Examples Complete\n");
    printf("=================================================\n");
    printf("\nThese examples demonstrate:\n");
    printf("- Basic arithmetic and operations\n");
    printf("- Conditional logic with jumps\n");
    printf("- Constant pool usage\n");
    printf("- String operations\n");
    printf("- Type conversions\n");
    printf("- Register manipulation\n");
    printf("\nThe register-based architecture provides:\n");
    printf("- Direct register access (no stack operations)\n");
    printf("- Fixed 8-byte instructions\n");
    printf("- 256 registers per call frame\n");
    printf("- ~10x performance improvement\n");
    
    return 0;
}
