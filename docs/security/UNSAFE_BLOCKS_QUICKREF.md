# KLang Unsafe Blocks - Quick Reference

## Syntax

```klang
unsafe {
    // Statements that perform potentially unsafe operations
    // Example: direct memory access, FFI calls, etc.
}
```

## Features

### Basic Unsafe Block

```klang
fn example() {
    let x = 10;
    
    unsafe {
        // Code inside this block is marked as unsafe
        let y = 20;
        println("x = " + str(x) + ", y = " + str(y));
    }
    
    // Back to safe code
}
```

### Nested Unsafe Blocks

```klang
unsafe {
    println("Outer unsafe block");
    
    unsafe {
        println("Inner unsafe block");
        
        unsafe {
            println("Deeply nested unsafe block");
        }
    }
}
```

### Unsafe Blocks with Control Flow

```klang
// Return from unsafe block
fn get_value() {
    unsafe {
        return 42;
    }
}

// Break/continue in loops
unsafe {
    for (let i = 0; i < 10; i++) {
        if (i == 5) break;
        println(str(i));
    }
}
```

## Scoping Rules

Variables declared inside an unsafe block are scoped to that block:

```klang
let outer = "outer";

unsafe {
    let inner = "inner";  // Only visible inside this unsafe block
    println(outer);        // Can access outer variables
    println(inner);        // Can access inner variables
}

// println(inner);         // ERROR: inner is not visible here
println(outer);            // OK: outer is still visible
```

## When to Use Unsafe Blocks

In future phases, unsafe blocks will be required for:

1. **Direct Memory Management**
   - Manual allocation/deallocation
   - Pointer arithmetic
   - Raw memory access

2. **FFI (Foreign Function Interface)**
   - Calling C/C++ functions
   - Interfacing with system libraries
   - Native code integration

3. **Low-level Operations**
   - Direct hardware access
   - System calls
   - Performance-critical code

4. **Capability-Protected Operations** (Phase 4+)
   - File system access (when strict mode enabled)
   - Network operations (when strict mode enabled)
   - Environment variable access (when strict mode enabled)

## Current Status (Phase 2)

✅ **Implemented:**
- Parsing and syntax recognition
- AST generation
- Evaluation with proper scoping
- Nested blocks
- Control flow support

⏳ **Coming in Future Phases:**
- Runtime security context tracking (Phase 3)
- Capability enforcement (Phase 4)
- Strict/permissive modes via CLI flags
- Automatic safety analysis
- Unsafe operation detection

## Implementation Notes

### For Compiler Developers

The unsafe block implementation consists of:

1. **Lexer** (`src/core/lexer/`):
   - `TOKEN_UNSAFE` token type
   - Keyword recognition for "unsafe"

2. **Parser** (`src/core/parser/`):
   - `parse_unsafe_block()` function
   - Handles `unsafe { statements }` syntax

3. **AST** (`src/core/ast/`):
   - `NODE_UNSAFE_BLOCK` node type
   - `unsafe_block` structure with `NodeList stmts`

4. **Evaluator** (`src/runtime/interpreter/`):
   - `eval_unsafe_block()` function
   - Creates new scope for block
   - Evaluates statements sequentially

### Adding Safety Checks (Future)

To add safety checks for specific operations:

```c
// In builtin function implementation:
void check_file_access(Interpreter *interp) {
    if (interp->safety_ctx && !interp->safety_ctx->in_unsafe_context) {
        if (interp->safety_ctx->strict_mode) {
            error_report(interp, "File access requires unsafe block");
            return;
        }
    }
}
```

## Examples

See the following files for complete examples:
- `examples/security/unsafe_example.kl` - Basic usage
- `examples/security/unsafe_comprehensive_test.kl` - Advanced features

## Testing

Run the examples:
```bash
./klang run examples/security/unsafe_example.kl
./klang run examples/security/unsafe_comprehensive_test.kl
```

## References

- Full implementation details: `docs/security/PHASE2_IMPLEMENTATION.md`
- Security architecture: `docs/security/SECURITY_ARCHITECTURE.md`
- Implementation plan: `docs/security/IMPLEMENTATION_PLAN.md`
