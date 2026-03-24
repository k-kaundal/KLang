# Variable Declaration Semantics: var, let, and const

## Overview

KLang now supports JavaScript-style variable declarations with three keywords:
- `let` - Block-scoped, mutable, prevents redeclaration
- `var` - Block-scoped, mutable, allows redeclaration  
- `const` - Block-scoped, immutable, prevents redeclaration

## Syntax

```javascript
let identifier [: type] [= value]
var identifier [: type] [= value]
const identifier [: type] = value
```

## Declaration Types

### `const` - Immutable Variables

Constants must be initialized at declaration and cannot be reassigned.

```javascript
const PI = 3.14159
const MAX_SIZE = 100
const NAME = "KLang"

// ✓ Valid: Reading const value
print(PI)

// ✗ Error: Cannot assign to const variable
PI = 3.14  // Error: cannot assign to const variable 'PI'

// ✗ Error: Must be initialized
const X  // Error: const declaration must be initialized
```

**Use cases:**
- Mathematical constants
- Configuration values
- Any value that shouldn't change

### `let` - Block-Scoped Variables

Block-scoped variables that can be reassigned but not redeclared in the same scope.

```javascript
let count = 0
let message = "Hello"

// ✓ Valid: Reassignment
count = 10
message = "World"

// ✗ Error: Cannot redeclare
let count = 20  // Error: identifier 'count' has already been declared

// ✓ Valid: Optional initialization
let x  // x is null
x = 5  // Now x is 5
```

**Use cases:**
- Loop counters
- Temporary variables
- Any mutable state

### `var` - Redeclarable Variables

Similar to `let` but allows redeclaration in the same scope.

```javascript
var x = 1
print(x)  // 1

// ✓ Valid: Redeclaration
var x = 2
print(x)  // 2

// ✓ Valid: Reassignment
x = 3
print(x)  // 3

// ✓ Valid: Optional initialization
var y  // y is null
```

**Use cases:**
- When you need flexibility to redeclare
- Legacy code patterns
- Scripts with dynamic variable definitions

## Block Scoping

All three declaration types are block-scoped. Variables declared in an inner scope shadow outer variables.

```javascript
let x = 10
const Y = 20

{
    let x = 30      // Inner x shadows outer x
    const Y = 40    // Inner Y shadows outer Y
    print(x)        // 30
    print(Y)        // 40
}

print(x)  // 10 (outer x restored)
print(Y)  // 20 (outer Y restored)
```

### Loop Scoping

Variables declared in loops are scoped to the loop body:

```javascript
for i in 0..3 {
    let temp = i * 2
    print(temp)
}
// print(temp)  // Error: temp is not defined
```

### Function Scoping

Variables declared in functions are scoped to the function:

```javascript
fn example() {
    let local = 42
    const LOCAL_CONST = 100
    var local_var = 200
    return local
}

print(example())  // 42
// print(local)   // Error: local is not defined
```

## Type Annotations

All declaration types support optional type annotations:

```javascript
let count: int = 0
var name: string = "John"
const PI: float = 3.14159
```

Note: Type annotations are currently for documentation only and not enforced at runtime.

## Comparison with JavaScript

KLang's implementation closely follows JavaScript's semantics with minor differences:

| Feature | JavaScript | KLang |
|---------|-----------|-------|
| `let` block scoping | ✓ | ✓ |
| `const` immutability | ✓ | ✓ |
| `var` redeclaration | ✓ | ✓ |
| `var` function hoisting | ✓ | ✗ (not yet implemented) |
| Temporal dead zone | ✓ | ✗ (not yet implemented) |

### Differences from JavaScript

1. **No function-level hoisting for `var`**: Currently, `var` is block-scoped like `let`. True function-scoping with hoisting may be added in the future.

2. **No temporal dead zone**: In JavaScript, referencing a `let`/`const` before declaration is an error. KLang currently treats undeclared variables as an immediate error.

## Error Messages

### Const Reassignment
```javascript
const X = 10
X = 20
// Error: cannot assign to const variable 'X'
```

### Redeclaration with let/const
```javascript
let x = 1
let x = 2
// Error at line 2: identifier 'x' has already been declared
```

### Uninitialized const
```javascript
const X
// Error at line 1: const declaration must be initialized
```

### Undefined Variable
```javascript
print(undeclared)
// Error at line 1: undefined variable 'undeclared'
```

## Best Practices

1. **Prefer `const` by default**: Use `const` for values that don't change. This makes code more predictable and easier to reason about.

2. **Use `let` for changing values**: When you need to modify a variable, use `let`.

3. **Avoid `var` in new code**: `var` is provided for compatibility and special cases. Prefer `let` or `const` for clarity.

4. **Keep scope tight**: Declare variables in the smallest scope necessary.

5. **Initialize when declaring**: Even though `let` and `var` allow uninitialized declarations, it's clearer to initialize them:
   ```javascript
   // Prefer
   let count = 0
   
   // Over
   let count
   count = 0
   ```

## Examples

### Counter Pattern
```javascript
fn createCounter() {
    let count = 0
    
    fn increment() {
        count = count + 1
        return count
    }
    
    return increment
}

let counter = createCounter()
print(counter())  // 1
print(counter())  // 2
print(counter())  // 3
```

### Configuration Object
```javascript
const CONFIG = {
    max_retries: 3,
    timeout: 5000,
    debug: true
}

// CONFIG itself is const, but we can modify its contents
// (if object mutation is supported)
```

### Loop with Block Scope
```javascript
for i in 0..5 {
    let squared = i * i
    const DOUBLE = i * 2
    print(squared)
    print(DOUBLE)
}
```

## Implementation Details

### AST Node Structure

All declarations use the `NODE_LET` type with a `decl_type` field:

```c
typedef enum {
    DECL_LET,
    DECL_VAR,
    DECL_CONST
} DeclType;

struct {
    char *name;
    char *type_annot;
    ASTNode *value;
    DeclType decl_type;
    // ... other fields
} let_stmt;
```

### Environment Entry

Each variable in the environment tracks its declaration type and constness:

```c
typedef struct EnvEntry {
    char *name;
    Value value;
    DeclType decl_type;
    int is_const;
    // ... other fields
} EnvEntry;
```

### Semantic Enforcement

- **Const assignment check**: `env_set()` checks `is_const` flag before allowing assignment
- **Redeclaration check**: `env_declare()` checks local scope for existing declaration
- **Initialization check**: Parser ensures const declarations have an initializer

## Future Enhancements

Potential improvements for future versions:

1. **Function-level hoisting for `var`**: Implement true JavaScript-like hoisting
2. **Temporal dead zone**: Detect references before declaration
3. **Destructuring**: `const {x, y} = point`
4. **Multiple declarations**: `let a = 1, b = 2, c = 3`
5. **Type enforcement**: Runtime type checking for type annotations

## Testing

Comprehensive tests are provided in `tests/test_var_let_const.c`. Run with:

```bash
make
gcc -Wall -Wextra -std=c99 -Isrc -Iinclude -g -o test_var_let_const \
    tests/test_var_let_const.c src/*.o -lm
./test_var_let_const
```

All tests should pass with no errors.

## See Also

- [JavaScript let documentation](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/let)
- [JavaScript const documentation](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/const)
- [JavaScript var documentation](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/var)
