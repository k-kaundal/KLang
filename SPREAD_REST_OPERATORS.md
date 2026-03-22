# Spread and Rest Operators in KLang

## Overview

KLang now supports JavaScript-style spread (`...`) and rest (`...`) operators, providing powerful syntax for working with arrays, objects, and function parameters.

## Features Implemented

### 1. Array Spread (`[...arr]`)

Spread syntax expands array elements in place.

```javascript
const arr1 = [1, 2, 3]
const arr2 = [4, 5, 6]
const combined = [...arr1, ...arr2]  // [1, 2, 3, 4, 5, 6]

// Mix with literals
const mixed = [0, ...arr1, 99, ...arr2, 100]
// Result: [0, 1, 2, 3, 99, 4, 5, 6, 100]

// Copy an array
const copy = [...original]
```

### 2. Rest Parameters (`...args`)

Rest parameters collect remaining function arguments into an array.

```javascript
// Collect all arguments
fn sum(...numbers) {
    let total = 0
    let i = 0
    while (i < len(numbers)) {
        total = total + numbers[i]
        i = i + 1
    }
    return total
}

sum(1, 2, 3)        // 6
sum(10, 20, 30, 40) // 100

// Rest with regular parameters
fn greet(greeting, ...names) {
    println(greeting)
    let i = 0
    while (i < len(names)) {
        println(names[i])
        i = i + 1
    }
}

greet("Hello", "Alice", "Bob", "Charlie")
```

**Important**: Rest parameter must be the last parameter in the function signature.

### 3. Object Spread (`{...obj}`)

Spread object properties into a new object.

```javascript
const person = {name: "Alice", age: 30}
const location = {city: "NYC", country: "USA"}
const profile = {...person, ...location}
// Result: {name: "Alice", age: 30, city: "NYC", country: "USA"}

// Override properties
const defaults = {theme: "light", fontSize: 12}
const custom = {...defaults, theme: "dark"}
// Result: {theme: "dark", fontSize: 12}
```

## Syntax

### Array Spread
```
[...expression]
[literal, ...array1, ...array2]
```

### Rest Parameters
```
fn functionName(...paramName) { }
fn functionName(param1, param2, ...rest) { }
```

### Object Spread
```
{...expression}
{key: value, ...obj1, ...obj2}
```

## Implementation Details

### Lexer Changes
- Added `TOKEN_SPREAD` for `...` (three dots)
- Distinguishes from `TOKEN_DOTDOT` (`..`) used for ranges

### Parser Changes
- Array literals detect and parse spread elements
- Function parameter parsing recognizes rest parameters
- Object literals handle spread properties
- Rest parameter validation ensures it's the last parameter

### AST Changes
- New node type: `NODE_SPREAD`
- Added `has_rest_param` flag to function definitions
- Spread node contains the expression to be expanded

### Interpreter Changes
- **Array Spread**: Evaluates spread arguments and flattens them into the target array
- **Rest Parameters**: Collects excess arguments into an array
- **Object Spread**: Copies properties from spread objects to the target object
- Proper memory management with deep copying for strings and nested structures

## Examples

### Variadic Functions

```javascript
fn min(...numbers) {
    if (len(numbers) == 0) return null
    let minimum = numbers[0]
    let i = 1
    while (i < len(numbers)) {
        if (numbers[i] < minimum) {
            minimum = numbers[i]
        }
        i = i + 1
    }
    return minimum
}

min(5, 2, 9, 1, 7)  // 1
```

### Configuration Objects

```javascript
const defaultConfig = {
    debug: false,
    timeout: 5000,
    retries: 3
}

const prodConfig = {
    ...defaultConfig,
    timeout: 10000
}

const devConfig = {
    ...defaultConfig,
    debug: true,
    timeout: 1000
}
```

### Array Operations

```javascript
// Combine arrays
const numbers = [1, 2, 3]
const more = [4, 5, 6]
const all = [...numbers, ...more]

// Add elements to array
const extended = [0, ...numbers, 4, 5]

// Copy array
const backup = [...numbers]
```

## Limitations

- Rest parameters must be the last parameter in a function
- Spread only works with arrays and objects (not other iterables)
- Nested array spreading requires proper memory management

## Test Coverage

Run the comprehensive test suite:
```bash
./klang run test_spread_rest_complete.k
```

Or try the examples:
```bash
./klang run examples/spread_rest_operators.k
```

## Compatibility

This feature brings KLang closer to JavaScript ES6+ syntax, making it easier for developers familiar with modern JavaScript to work with KLang.

## Performance Notes

- Spread operations create new arrays/objects rather than modifying existing ones
- Deep copying is performed for strings to prevent memory issues
- Rest parameters are collected into a standard array accessible via `len()` and indexing
