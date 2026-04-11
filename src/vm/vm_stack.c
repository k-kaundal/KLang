#include "vm_stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Chunk *chunk_new(void) {
    Chunk *c = calloc(1, sizeof(Chunk));
    return c;
}

void chunk_free(Chunk *chunk) {
    int i;
    for (i = 0; i < chunk->count; i++) {
        if (chunk->code[i].op == OP_PUSH_STR || chunk->code[i].op == OP_LOAD || chunk->code[i].op == OP_STORE) {
            if (chunk->code[i].operand.sval) free(chunk->code[i].operand.sval);
        }
    }
    free(chunk->code);
    free(chunk);
}

static void chunk_grow(Chunk *chunk) {
    if (chunk->count >= chunk->capacity) {
        chunk->capacity = chunk->capacity == 0 ? 16 : chunk->capacity * 2;
        chunk->code = realloc(chunk->code, chunk->capacity * sizeof(Instruction));
    }
}

void chunk_emit_op(Chunk *chunk, Opcode op) {
    chunk_grow(chunk);
    chunk->code[chunk->count].op = op;
    chunk->code[chunk->count].operand.ival = 0;
    chunk->count++;
}

void chunk_emit_int(Chunk *chunk, Opcode op, long long val) {
    chunk_grow(chunk);
    chunk->code[chunk->count].op = op;
    chunk->code[chunk->count].operand.ival = val;
    chunk->count++;
}

void chunk_emit_float(Chunk *chunk, Opcode op, double val) {
    chunk_grow(chunk);
    chunk->code[chunk->count].op = op;
    chunk->code[chunk->count].operand.dval = val;
    chunk->count++;
}

void chunk_emit_str(Chunk *chunk, Opcode op, const char *val) {
    chunk_grow(chunk);
    chunk->code[chunk->count].op = op;
    chunk->code[chunk->count].operand.sval = strdup(val);
    chunk->count++;
}

VM *vm_new(void) {
    VM *vm = calloc(1, sizeof(VM));
    return vm;
}

void vm_free(VM *vm) {
    free(vm);
}

int vm_execute(VM *vm, Chunk *chunk) {
    vm->chunk = chunk;
    vm->ip = 0;
    vm->stack_top = 0;

    while (vm->ip < chunk->count) {
        Instruction *instr = &chunk->code[vm->ip++];
        switch (instr->op) {
            case OP_PUSH_INT:
                if (vm->stack_top < VM_STACK_MAX)
                    vm->stack[vm->stack_top++] = instr->operand.ival;
                break;
            case OP_POP:
                if (vm->stack_top > 0) vm->stack_top--;
                break;
            case OP_ADD:
                if (vm->stack_top >= 2) {
                    long long b = vm->stack[--vm->stack_top];
                    long long a = vm->stack[--vm->stack_top];
                    vm->stack[vm->stack_top++] = a + b;
                }
                break;
            case OP_SUB:
                if (vm->stack_top >= 2) {
                    long long b = vm->stack[--vm->stack_top];
                    long long a = vm->stack[--vm->stack_top];
                    vm->stack[vm->stack_top++] = a - b;
                }
                break;
            case OP_MUL:
                if (vm->stack_top >= 2) {
                    long long b = vm->stack[--vm->stack_top];
                    long long a = vm->stack[--vm->stack_top];
                    vm->stack[vm->stack_top++] = a * b;
                }
                break;
            case OP_DIV:
                if (vm->stack_top >= 2) {
                    long long b = vm->stack[--vm->stack_top];
                    long long a = vm->stack[--vm->stack_top];
                    vm->stack[vm->stack_top++] = b != 0 ? a / b : 0;
                }
                break;
            case OP_PRINT:
                if (vm->stack_top > 0)
                    printf("%lld\n", vm->stack[vm->stack_top - 1]);
                break;
            case OP_HALT:
                return 0;
            default:
                break;
        }
    }
    return 0;
}
