# Closure Implementation Fix for KLang

## Problem
Nested function declarations (closures) were causing segmentation faults when the inner function tried to access variables from the outer function's scope. The issue occurred because the environment was being freed while still being referenced by the closure.

## Root Cause
The interpreter was freeing the call environment immediately after a function returned, even if nested functions had captured that environment as their closure. This resulted in dangling pointers when the nested function was later called.

## Solution
Implemented reference counting for environment (Env) structures to properly manage closure lifetimes:

### Changes Made

#### 1. Enhanced Environment Structure (src/interpreter.h)
- Added `ref_count` field to `struct Env` to track how many references exist to an environment

#### 2. Reference Counting Functions (src/interpreter.c)
- **`env_new()`**: Initializes environments with `ref_count = 1`
- **`env_retain()`**: Increments reference count when an environment is captured
- **`env_release()`**: Decrements reference count and frees only when it reaches zero

#### 3. Function Value Management
- **NODE_FUNC_DEF**: When a function is created, it retains its closure environment
- **env_set_local()** and **env_set_local_with_access()**: Retain closure when copying function values
- **NODE_IDENT**: Retain closure when accessing/copying function variables
- **value_free()**: Release closure environment when function values are freed
- **Generator functions**: Properly retain closure for generator function copies

#### 4. Environment Cleanup
- Replaced all `env_free()` calls for temporary scopes (call_env, block_env, loop_env, for_env) with `env_release()`
- Module and global environments also use reference-counted release

## Key Behavioral Changes

### Before
```javascript
fn outer() {
    let x = 10
    fn inner() {
        return x + 5  // CRASH: x's environment was already freed
    }
    return inner
}
let myFunc = outer()
print(myFunc())  // Segmentation fault
```

### After
```javascript
fn outer() {
    let x = 10
    fn inner() {
        return x + 5  // Works: closure environment is kept alive
    }
    return inner
}
let myFunc = outer()
print(myFunc())  // Output: 15
```

## Supported Closure Patterns

### 1. Basic Variable Capture
```javascript
fn makeGreeter(name) {
    fn greet() {
        return "Hello, " + name
    }
    return greet
}
let greet = makeGreeter("Alice")
print(greet())  // "Hello, Alice"
```

### 2. Multiple Nested Levels
```javascript
fn level1() {
    let a = 1
    fn level2() {
        let b = 2
        fn level3() {
            return a + b + 3
        }
        return level3
    }
    return level2()
}
```

### 3. Mutable State (Counter Pattern)
```javascript
fn makeCounter() {
    let count = 0
    fn increment() {
        count = count + 1
        return count
    }
    return increment
}
```

### 4. Parameterized Closures
```javascript
fn makeAdder(x) {
    fn adder(y) {
        return x + y
    }
    return adder
}
```

## Testing
Created `examples/closures.k` demonstrating all supported closure patterns.

## Impact on Existing Code
- No breaking changes to existing functionality
- All regular functions continue to work as before
- Memory management is now more robust with reference counting
- No performance impact for functions that don't use closures

## Technical Notes
- Reference counting is only used for environments, not for all values
- Parent environment relationships are NOT reference counted (to avoid cycles)
- Zero-parameter functions now properly handle closure retention (fixed condition check)
- Class methods also properly retain their closure environment
