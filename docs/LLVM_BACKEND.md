# KLang LLVM Backend Documentation

## Overview

The LLVM backend compiles KLang source code directly to native machine code using the LLVM compiler infrastructure. This provides maximum performance and generates standalone executables.

## Architecture

### Components

1. **LLVM Compiler Context** (`LLVMCompilerContext`)
   - Manages LLVM module, builder, and context
   - Maintains symbol table for variables
   - Tracks current function and loop targets
   - Caches common types

2. **Symbol Table**
   - Hash-table based implementation
   - Supports nested scopes
   - O(1) lookup/insert operations

3. **AST to LLVM IR Translation**
   - Expression compilation (`llvm_compile_expr`)
   - Statement compilation (`llvm_compile_stmt`)
   - Specialized handlers for each node type

4. **Optimization Pipeline**
   - Memory-to-register promotion
   - Constant folding and propagation
   - Dead code elimination
   - Control flow simplification

## Supported Features

### Data Types
- **Numbers**: Double-precision floating point
- **Booleans**: i1 type
- **Strings**: i8* pointers
- **Arrays**: Fixed-size array allocations

### Operations
- **Arithmetic**: +, -, *, /, %
- **Comparison**: ==, !=, <, >, <=, >=
- **Logical**: &&, ||, !
- **Unary**: -, !

### Control Flow
- **If/Else**: Conditional branching
- **While Loops**: Condition-controlled iteration
- **For Loops**: Range-based and C-style loops
- **Break/Continue**: Loop control statements

### Functions
- **Definition**: Parameter passing, return values
- **Calls**: Direct function calls
- **Built-ins**: print, println

## API Reference

### Initialization

```c
void llvm_backend_init(void);
```
Initializes LLVM native target and ASM printers. Must be called before compilation.

### Context Management

```c
LLVMCompilerContext *llvm_context_new(const char *module_name);
void llvm_context_free(LLVMCompilerContext *ctx);
```
Create and destroy compilation contexts.

### Compilation

```c
int llvm_compile_ast(ASTNode **nodes, int count, const char *output_path);
```
Compiles AST to native executable. Returns 0 on success.

```c
int llvm_compile_to_object(ASTNode **nodes, int count, const char *output_path);
```
Compiles AST to object file (.o).

### Optimization

```c
void llvm_apply_optimizations(LLVMCompilerContext *ctx);
```
Applies optimization passes to the LLVM module.

### Symbol Table

```c
SymbolTable *symbol_table_new(SymbolTable *parent);
void symbol_table_free(SymbolTable *table);
void symbol_table_insert(SymbolTable *table, const char *name, LLVMValueRef value);
LLVMValueRef symbol_table_lookup(SymbolTable *table, const char *name);
```

## Usage

### Command Line

```bash
# Compile KLang source to native executable
klang compile program.kl

# The executable is generated in the current directory
./program
```

### Example

**Input** (`hello.kl`):
```javascript
let x = 10;
let y = 20;
let sum = x + y;
println("Sum:", sum);
```

**Command**:
```bash
klang compile hello.kl
```

**Output**:
- Generates `hello` executable
- Can be run directly: `./hello`
- No KLang runtime required

## Implementation Details

### Two-Pass Compilation

1. **First Pass**: Compile all function definitions
   - Creates function prototypes
   - Allows forward references

2. **Second Pass**: Compile top-level statements
   - Creates main function wrapper
   - Executes statements in order

### Type Handling

- KLang uses dynamic typing
- LLVM backend uses `double` for all numbers
- Strings are `i8*` pointers (C-style)
- Booleans are `i1` for conditions

### Memory Management

- Local variables: Stack-allocated (`alloca`)
- Function parameters: Copied to stack slots
- Strings: Global constants

### Control Flow

- If/else: Conditional branches with merge blocks
- While loops: Condition, body, and continuation blocks
- For loops: Init, condition, body, update, and exit blocks
- Break/continue: Jump to appropriate basic blocks

## Optimization Passes

1. **mem2reg**: Promotes memory to registers
2. **InstCombine**: Combines redundant instructions
3. **Reassociate**: Reassociates expressions for optimization
4. **GVN**: Global Value Numbering for common subexpression elimination
5. **CFGSimplify**: Simplifies control flow graph
6. **DSE**: Dead store elimination
7. **AggressiveDCE**: Aggressive dead code elimination
8. **CVP**: Correlated value propagation

## Performance

- Native code generation
- Comparable to C/C++ performance
- No interpreter overhead
- Full LLVM optimization benefits
- Standalone executables

## Limitations

### Current Limitations
- Limited string operations
- No dynamic arrays
- No garbage collection
- No exception handling
- No module system support

### Known Issues
- Segmentation fault in printf generation (being fixed)
- Limited type inference
- No type checking

## Future Enhancements

1. **Type System**
   - Static type analysis
   - Type inference
   - Generic types

2. **Memory Management**
   - Automatic memory management
   - Reference counting
   - Garbage collection integration

3. **Advanced Features**
   - Closures and lambdas
   - Async/await
   - Exception handling
   - Module system

4. **Optimizations**
   - Inlining
   - Loop optimizations
   - Vectorization

## Building

The LLVM backend requires LLVM 16:

```bash
# Install LLVM 16 (Ubuntu/Debian)
sudo apt-get install llvm-16-dev

# Build KLang with LLVM support
make clean
make
```

## Debugging

### Generate LLVM IR

```c
llvm_write_ir(ctx, "output.ll");
```

### Verify Module

```c
char *error = NULL;
LLVMVerifyModule(ctx->module, LLVMPrintMessageAction, &error);
```

### View LLVM IR

```bash
# After generating .ll file
llvm-dis-16 output.ll
cat output.ll
```

## Contributing

When contributing to the LLVM backend:

1. Follow the existing code structure
2. Add error handling for edge cases
3. Update tests for new features
4. Document API changes
5. Ensure optimization passes are beneficial

## References

- [LLVM-C API Documentation](https://llvm.org/doxygen/)
- [LLVM Language Reference](https://llvm.org/docs/LangRef.html)
- [LLVM Programmer's Manual](https://llvm.org/docs/ProgrammersManual.html)
