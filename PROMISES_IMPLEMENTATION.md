# JavaScript-Style Promises Implementation for KLang

## Overview
This document describes the implementation of JavaScript-style Promises for KLang, providing asynchronous programming capabilities similar to JavaScript ES6 Promises.

## Features Implemented

### 1. Core Promise States
- **PROMISE_PENDING**: Initial state, neither fulfilled nor rejected
- **PROMISE_FULFILLED**: Operation completed successfully
- **PROMISE_REJECTED**: Operation failed

### 2. Promise Constructor
```javascript
const promise = new Promise((resolve, reject) => {
    if (success) {
        resolve(value)
    } else {
        reject(reason)
    }
})
```

### 3. Instance Methods

#### then(onFulfilled, onRejected)
Attaches callbacks for fulfillment and rejection cases.
```javascript
promise.then(
    value => { /* handle success */ },
    reason => { /* handle error */ }
)
```

#### catch(onRejected)
Attaches a callback for rejection cases only.
```javascript
promise.catch(error => {
    println(`Error: ${error}`)
})
```

#### finally(onFinally)
Attaches a callback that runs regardless of success or failure.
```javascript
promise.finally(() => {
    println("Cleanup")
})
```

### 4. Static Methods

#### Promise.resolve(value)
Returns a Promise that is resolved with the given value.
```javascript
const p = Promise.resolve(42)
```

#### Promise.reject(reason)
Returns a Promise that is rejected with the given reason.
```javascript
const p = Promise.reject("error")
```

### 5. Promise Chaining
Promises support chaining where each `then()` returns a new Promise:
```javascript
Promise.resolve(10)
    .then(v => v * 2)
    .then(v => v + 5)
    .then(v => println(v))  // Prints: 25
```

### 6. Error Propagation
Errors propagate through the chain until caught:
```javascript
Promise.reject("error")
    .then(v => v * 2)       // Skipped
    .then(v => v + 5)       // Skipped
    .catch(e => println(e))  // Catches error
```

## Implementation Details

### Data Structures

#### PromiseVal (interpreter.h)
```c
typedef struct {
    PromiseState state;
    Value *result;                  // Fulfilled value or rejection reason
    PromiseCallbackNode *callbacks; // Linked list of callbacks
} PromiseVal;
```

#### PromiseCallbackNode (interpreter.h)
```c
typedef struct PromiseCallbackNode {
    Value *on_fulfilled;        // Function to call on fulfillment
    Value *on_rejected;         // Function to call on rejection
    Value *promise_to_resolve;  // The promise returned by then/catch/finally
    struct PromiseCallbackNode *next;
} PromiseCallbackNode;
```

### Key Functions

#### promise_resolve() (runtime.c)
Transitions promise to PROMISE_FULFILLED state and executes all registered fulfillment callbacks.

#### promise_reject() (runtime.c)
Transitions promise to PROMISE_REJECTED state and executes all registered rejection callbacks.

#### builtin_Promise_constructor() (runtime.c)
Creates a new Promise and executes the executor function with resolve/reject functions.

#### builtin_Promise_then() (runtime.c)
Registers callbacks and returns a new Promise for chaining.

### Special Handling

#### Promise Constructor in NODE_CALL (interpreter.c)
The `new Promise(...)` syntax is handled specially in the NODE_CALL case:
```c
if (strcmp(class_name, "Promise") == 0) {
    // Call builtin_Promise_constructor
}
```

#### Promise Method Access (interpreter.c)
Promise methods (.then, .catch, .finally) are handled in member access:
```c
if (obj.type == VAL_PROMISE) {
    // Lookup __promise_then, __promise_catch, __promise_finally
}
```

#### Static Method Access (interpreter.c)
Promise.resolve() and Promise.reject() are registered as static methods on the Promise class.

## Files Modified

### src/interpreter.h
- Added `VAL_PROMISE` to `ValueType` enum
- Added `PromiseState` enum
- Added `PromiseCallbackNode` struct
- Added `PromiseVal` struct
- Added `promise_val` to Value union
- Added `make_promise()` declaration

### src/interpreter.c
- Implemented `make_promise()` function
- Updated `value_free()` to handle Promise cleanup
- Updated `value_to_string()` to display Promise state
- Added Promise method access in NODE_MEMBER_ACCESS
- Added special handling for `new Promise()` in NODE_CALL
- Updated static method lookup to accept VAL_BUILTIN

### src/runtime.c
- Implemented `promise_resolve()` helper
- Implemented `promise_reject()` helper
- Implemented `call_function_helper()` for executing callbacks
- Implemented `builtin_promise_resolve_fn()` and `builtin_promise_reject_fn()`
- Implemented `builtin_Promise_constructor()`
- Implemented `builtin_Promise_then()`
- Implemented `builtin_Promise_catch()`
- Implemented `builtin_Promise_finally()`
- Implemented `builtin_Promise_resolve()` static method
- Implemented `builtin_Promise_reject()` static method
- Registered Promise class and methods in `runtime_init()`

## Usage Examples

### Basic Usage
```javascript
// Create resolved promise
const p1 = Promise.resolve(42)

// Create rejected promise
const p2 = Promise.reject("error")

// Promise constructor
const p3 = new Promise((resolve, reject) => {
    const success = true
    if (success) {
        resolve("Success!")
    } else {
        reject("Failed!")
    }
})
```

### Chaining
```javascript
Promise.resolve(10)
    .then(value => {
        println(`Got: ${value}`)
        return value * 2
    })
    .then(value => {
        println(`Doubled: ${value}`)
    })
    .catch(error => {
        println(`Error: ${error}`)
    })
    .finally(() => {
        println("Done!")
    })
```

### Error Handling
```javascript
Promise.reject("Something went wrong")
    .catch(error => {
        println(`Caught: ${error}`)
        return "Recovered"
    })
    .then(value => {
        println(`After recovery: ${value}`)
    })
```

## Testing

### Test Files
- **tests/test_promises.c**: C test suite
- **examples/promises_demo.k**: Comprehensive examples
- **test_promise_basic.k**: Basic functionality tests

### Running Tests
```bash
# Run examples
./klang run examples/promises_demo.k

# Build and run C tests
gcc tests/test_promises.c -o test_promises
./test_promises
```

## Known Limitations

1. **Synchronous Execution**: Promises execute synchronously, not asynchronously. The executor and callbacks run immediately rather than being scheduled on a microtask queue.

2. **Memory Management**: Current implementation has memory management challenges:
   - Promise structs are heap-allocated and shared across copies
   - Some memory leaks exist but are acceptable given KLang's Value system design
   - This is consistent with how classes/objects are handled in KLang
   - A full fix would require reference counting throughout the Value system

3. **Thread Safety**: The global `g_current_promise` variable used during construction is not thread-safe. KLang is currently single-threaded, so this isn't an issue in practice.

4. **String Values**: String values in promises may cause memory corruption due to shared pointer issues in the Value system. Use numeric values or objects instead.

5. **Promise.all() and Promise.race()**: These static methods are not yet implemented.

6. **Async/Await**: Syntactic sugar for Promises (async/await) is not implemented.

## Future Enhancements

1. Implement true async execution with event loop
2. Add Promise.all() for parallel promise execution
3. Add Promise.race() for competitive promise execution
4. Add async/await syntax support
5. Improve memory management for promise cleanup
6. Add Promise.allSettled() and Promise.any()

## Conclusion

This implementation provides the core functionality of JavaScript Promises in KLang, enabling developers to write cleaner asynchronous code with proper error handling and chaining. While the execution model is currently synchronous, the API is compatible with JavaScript Promises, making it easy for developers familiar with JavaScript to use Promises in KLang.
