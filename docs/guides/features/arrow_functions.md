# Arrow Functions in KLang

KLang now supports JavaScript-style arrow function syntax, providing a concise way to write anonymous functions.

## Syntax

Arrow functions support multiple forms:

### Single Parameter (No Parentheses)
```javascript
const square = x => x * x
```

### Multiple Parameters (With Parentheses)
```javascript
const add = (a, b) => a + b
```

### No Parameters
```javascript
const greet = () => "Hello!"
```

### Expression Body (Implicit Return)
```javascript
const double = x => x * 2  // Automatically returns x * 2
```

### Block Body (Explicit Return)
```javascript
const complex = x => {
    let result = x * 2
    return result + 1
}
```

## Features

### First-Class Values
Arrow functions are first-class values and can be:
- Assigned to variables (let, var, const)
- Returned from functions
- Stored in data structures

```javascript
// Assign to different variable types
let f1 = x => x + 1
var f2 = x => x + 2
const f3 = x => x + 3
```

### Nested Arrow Functions
Arrow functions can be nested to create higher-order functions:

```javascript
const makeAdder = x => y => x + y
const add5 = makeAdder(5)
// add5 is now a function that adds 5 to its argument
```

### Lexical Scoping
Arrow functions capture variables from their enclosing scope (closure):

```javascript
fn makeCounter() {
    let count = 0
    return () => {
        count = count + 1
        return count
    }
}
```

## Comparison with Regular Functions

### Regular Function
```javascript
fn square(x: int) -> int {
    return x * x
}
```

### Arrow Function
```javascript
const square = x => x * x
```

## Implementation Details

- Token: `=>` (TOKEN_FAT_ARROW) for arrow functions
- Token: `->` (TOKEN_ARROW) for return type annotations  
- Arrow functions are implemented as `NODE_FUNC_DEF` nodes with `is_arrow` flag set
- Expression bodies are automatically wrapped in a return statement
- Arrow functions return function values that can be called immediately or stored

## Examples

See `examples/arrow_functions.klang` for comprehensive examples.

### Basic Usage
```javascript
const numbers = [1, 2, 3, 4, 5]

// Single parameter
const square = x => x * x

// Multiple parameters  
const add = (a, b) => a + b

// No parameters
const getAnswer = () => 42

// Block body
const factorial = n => {
    if n <= 1 {
        return 1
    }
    return n * factorial(n - 1)
}
```

### With Different Declaration Types
```javascript
let mutable = x => x + 1      // Can be reassigned
var legacyMutable = x => x + 2  // Can be redeclared
const immutable = x => x + 3   // Cannot be reassigned
```

## Limitations

Arrow functions in KLang:
- Do not support default parameters (same as regular functions)
- Do not support rest parameters/spread syntax
- Do not support destructuring in parameters
- Function passing as arguments may have limitations due to existing KLang runtime constraints

## Compatibility

Arrow functions are fully compatible with existing KLang syntax and can be used anywhere a function expression would be used. They work with all declaration types (let, var, const) and integrate seamlessly with the existing function call mechanism.
