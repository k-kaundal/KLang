# Increment and Decrement Operators in KLang

## Overview

KLang supports both increment (`++`) and decrement (`--`) operators in both prefix and postfix forms, similar to C, C++, Java, and JavaScript.

## Syntax

### Postfix Form
```klang
variable++  // Increment after use
variable--  // Decrement after use
```

### Prefix Form
```klang
++variable  // Increment before use
--variable  // Decrement before use
```

## Behavior

### Postfix Operators (`i++`, `i--`)
- Returns the **current value** of the variable
- Then increments/decrements the variable
- Useful when you need the old value in an expression

### Prefix Operators (`++i`, `--i`)
- Increments/decrements the variable **first**
- Then returns the **new value**
- Useful when you need the updated value immediately

## Examples

### Basic Usage

```klang
let x = 5
println(x++)  // Prints: 5, x becomes 6
println(x)    // Prints: 6

let y = 5
println(++y)  // Prints: 6, y becomes 6
println(y)    // Prints: 6
```

### In Expressions

```klang
let a = 10
let b = a++ * 2  // b = 20 (10 * 2), a becomes 11

let c = 10
let d = ++c * 2  // d = 22 (11 * 2), c becomes 11
```

### With Arrays

```klang
let arr = [10, 20, 30]
let i = 0
println(arr[i++])  // Prints: 10, i becomes 1
println(arr[i++])  // Prints: 20, i becomes 2
println(arr[i++])  // Prints: 30, i becomes 3
```

### With Object Properties

```klang
let obj = {counter: 0}
obj.counter++       // Increment object property
println(obj.counter)  // Prints: 1
++obj.counter       // Prefix increment
println(obj.counter)  // Prints: 2
```

### In For Loops

You can use `++` and `--` operators in for loop increment clauses:

```klang
# Using postfix increment
for (let i = 0; i < 10; i++) {
    println(i)
}

# Using postfix decrement
for (let i = 10; i > 0; i--) {
    println(i)
}

# Using prefix increment
for (let i = 0; i < 10; ++i) {
    println(i)
}

# Traditional syntax also works
for (let i = 0; i < 10; i = i + 1) {
    println(i)
}
```

## Type Support

These operators work with:
- **Integers**: `let x = 5; x++`  → `x` becomes `6`
- **Floats**: `let y = 3.5; y++` → `y` becomes `4.5`

## Common Use Cases

### 1. Counter Variables
```klang
let count = 0
count++  // Increment counter
```

### 2. Array Iteration
```klang
let i = 0
while (i < len(arr)) {
    process(arr[i++])
}
```

### 3. Loop Counters
```klang
for (let i = 0; i < 10; i = i + 1) {
    println(i)
}
```

### 4. Complex Expressions
```klang
let x = 5
let result = x++ + ++x  // result = 5 + 7 = 12, x = 7
```

## Important Notes

1. **Side Effects**: These operators modify the variable, so use them carefully in complex expressions
2. **Operator Precedence**: Prefix operators have higher precedence than postfix
3. **Return Values**: 
   - Postfix returns the OLD value
   - Prefix returns the NEW value
4. **For Loop Support**: Both `i++` and `i = i + 1` syntax work in for loops

## Implementation Details

The increment/decrement operators are implemented at the language level:
- **Lexer**: Tokenizes `++` as `TOKEN_PLUS_PLUS` and `--` as `TOKEN_MINUS_MINUS`
- **Parser**: Handles both prefix (in `parse_unary()`) and postfix (in `parse_postfix()`) forms
- **AST**: Represented as `NODE_POSTFIX` with an `is_postfix` flag
- **Interpreter**: Evaluates in `eval_postfix()` with proper value handling

## See Also

- [Operators Documentation](./OPERATORS.md)
- [Expression Syntax](./EXPRESSIONS.md)
- [For Loop Documentation](./LOOPS.md)

## Example Program

See `examples/increment_decrement_demo.kl` for a comprehensive demonstration of all increment and decrement operator features.
