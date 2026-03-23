# KLang Critical Operators Implementation

This document describes the implementation of five critical operator features added to KLang.

## Overview

The following operators have been fully implemented:
1. **Logical Operators** (`&&`, `||`) with short-circuit evaluation
2. **Compound Assignments** (`+=`, `-=`, `*=`, `/=`, `%=`)
3. **Increment/Decrement** (`++`, `--`) with prefix and postfix support
4. **Optional Chaining** (`?.`)
5. **Nullish Coalescing** (`??`)

## 1. Logical Operators (`&&`, `||`)

### Syntax
```javascript
condition1 && condition2  // Logical AND
condition1 || condition2  // Logical OR
```

### Features
- **Short-circuit evaluation**: The right operand is only evaluated if necessary
  - For `&&`: right side is not evaluated if left side is false
  - For `||`: right side is not evaluated if left side is true
- Returns boolean (`true` or `false`)

### Examples
```javascript
// Basic usage
let canDrive = age >= 18 && hasLicense;
let isValid = isAdmin || hasPermission;

// Short-circuit evaluation
let x = false && expensiveFunction();  // expensiveFunction() is NOT called
let y = true || expensiveFunction();   // expensiveFunction() is NOT called

// In conditions
if (user != null && user.age >= 18) {
    print("Adult user");
}
```

### Implementation Details
- **Lexer**: Added `TOKEN_AND_AND` and `TOKEN_OR_OR`
- **Parser**: Implemented `parse_logical_and()` and `parse_logical_or()` functions
  - Precedence: `equality < logical_and < logical_or < nullish_coalesce < ternary`
- **Interpreter**: Special handling in `NODE_BINOP` case with early return for short-circuit

---

## 2. Compound Assignments (`+=`, `-=`, `*=`, `/=`, `%=`)

### Syntax
```javascript
x += value   // Equivalent to: x = x + value
x -= value   // Equivalent to: x = x - value
x *= value   // Equivalent to: x = x * value
x /= value   // Equivalent to: x = x / value
x %= value   // Equivalent to: x = x % value
```

### Features
- Works with variables, object properties, and array elements
- Supports both integer and floating-point operations
- Type conversion follows the same rules as binary operators

### Examples
```javascript
// With variables
let x = 10;
x += 5;   // x is now 15
x *= 2;   // x is now 30

// With object properties
let obj = { score: 100 };
obj.score += 50;   // obj.score is now 150
obj.score *= 2;    // obj.score is now 300

// With array elements
let arr = [10, 20, 30];
arr[0] += 5;   // arr[0] is now 15
arr[1] *= 2;   // arr[1] is now 40
```

### Implementation Details
- **Lexer**: Added tokens: `TOKEN_PLUS_ASSIGN`, `TOKEN_MINUS_ASSIGN`, `TOKEN_STAR_ASSIGN`, `TOKEN_SLASH_ASSIGN`, `TOKEN_PERCENT_ASSIGN`
- **Parser**: Modified assignment parsing in `parse_statement()` to:
  1. Detect compound assignment operators
  2. Transform `x += y` into `x = x + y` in the AST
  3. For arrays and objects, create deep copies of the left-hand side for the binary operation
- **Interpreter**: Uses existing binary operator handling after transformation

---

## 3. Increment/Decrement Operators (`++`, `--`)

### Syntax
```javascript
++x   // Prefix increment: increment x, then return new value
x++   // Postfix increment: return current value, then increment x
--x   // Prefix decrement: decrement x, then return new value
x--   // Postfix decrement: return current value, then decrement x
```

### Features
- **Prefix**: Increments/decrements first, returns new value
- **Postfix**: Returns current value first, then increments/decrements
- Works with variables, object properties, and array elements
- Supports both integer and floating-point values

### Examples
```javascript
// Prefix
let x = 5;
print(++x);  // Prints 6, x is now 6
print(--x);  // Prints 5, x is now 5

// Postfix
let y = 10;
print(y++);  // Prints 10, y is now 11
print(y--);  // Prints 11, y is now 10

// In expressions
let i = 5;
let j = 10;
let sum = ++i + j++;  // i becomes 6, j becomes 11, sum is 16

// With arrays
let arr = [1, 2, 3];
arr[0]++;     // arr[0] is now 2
++arr[1];     // arr[1] is now 3
print(arr[2]++);  // Prints 3, arr[2] becomes 4

// With objects
let obj = { count: 0 };
obj.count++;  // obj.count is now 1
```

### Implementation Details
- **Lexer**: Added `TOKEN_PLUS_PLUS` and `TOKEN_MINUS_MINUS`
- **AST**: Added `NODE_POSTFIX` node type with `is_postfix` flag
- **Parser**: 
  - Prefix: Handled in `parse_unary()`
  - Postfix: Handled in `parse_postfix()`
- **Interpreter**: Added `NODE_POSTFIX` case that:
  1. Evaluates current value
  2. Calculates new value
  3. Stores new value
  4. Returns old value (postfix) or new value (prefix)
  - Special handling for array indices to modify in-place

---

## 4. Optional Chaining (`?.`)

### Syntax
```javascript
obj?.property   // Access property if obj is not null
```

### Features
- Returns `null` if the left operand is `null`
- Otherwise, performs normal property access
- Prevents "null reference" errors
- Can be chained multiple times

### Examples
```javascript
// Basic usage
let user = { name: "Alice" };
print(user?.name);  // Prints "Alice"

let nullUser = null;
print(nullUser?.name);  // Prints null (no error)

// Chaining
let data = { user: { address: { city: "NYC" } } };
print(data?.user?.address?.city);  // Prints "NYC"

let data2 = { user: null };
print(data2?.user?.address?.city);  // Prints null (stops at first null)

// In conditions
if (config?.debug) {
    print("Debug mode enabled");
}
```

### Implementation Details
- **Lexer**: Added `TOKEN_OPTIONAL_CHAIN` to recognize `?.`
- **AST**: Added `NODE_OPTIONAL_CHAIN` node type
- **Parser**: Modified `parse_postfix()` to handle `?.` similarly to `.`
- **Interpreter**: Added `NODE_OPTIONAL_CHAIN` case that:
  1. Evaluates the object
  2. Returns `null` if object is `null`
  3. Otherwise performs normal property access

---

## 5. Nullish Coalescing Operator (`??`)

### Syntax
```javascript
value1 ?? value2   // Returns value1 if not null, otherwise value2
```

### Features
- Returns the right operand if the left operand is `null`
- Unlike `||`, does NOT treat falsy values (0, false, "") as null
- Only treats actual `null` values as nullish
- Useful for providing default values

### Examples
```javascript
// Basic usage
let x = null;
print(x ?? "default");  // Prints "default"

let y = "value";
print(y ?? "default");  // Prints "value"

// Preserves falsy values (unlike ||)
let zero = 0;
print(zero ?? 42);  // Prints 0 (not 42)

let empty = "";
print(empty ?? "default");  // Prints "" (not "default")

let falseBool = false;
print(falseBool ?? true);  // Prints false (not true)

// Common use case: default parameters
fn greet(name) {
    let displayName = name ?? "Guest";
    print("Hello, " + displayName);
}

greet("Alice");  // Hello, Alice
greet(null);     // Hello, Guest

// With optional chaining
let timeout = config?.timeout ?? 3000;
```

### Implementation Details
- **Lexer**: Added `TOKEN_NULLISH_COALESCE` to recognize `??`
- **AST**: Added `NODE_NULLISH_COALESCE` node type
- **Parser**: Implemented `parse_nullish_coalesce()` function
  - Precedence: `logical_or < nullish_coalesce < ternary`
- **Interpreter**: Added `NODE_NULLISH_COALESCE` case that:
  1. Evaluates left operand
  2. Returns left if not `null`
  3. Otherwise evaluates and returns right operand

---

## Operator Precedence

The precedence from highest to lowest:

1. Primary expressions (literals, identifiers, parentheses)
2. Postfix operators (`++`, `--`, function calls, array indexing, `.`, `?.`)
3. Prefix operators (`++`, `--`, `-`, `!`, `await`, `yield`)
4. Multiplicative (`*`, `/`, `%`)
5. Additive (`+`, `-`)
6. Comparison (`<`, `>`, `<=`, `>=`)
7. Equality (`==`, `!=`)
8. Logical AND (`&&`)
9. Logical OR (`||`)
10. Nullish coalescing (`??`)
11. Ternary (`? :`)
12. Assignment (`=`, `+=`, `-=`, `*=`, `/=`, `%=`)

---

## Testing

Comprehensive tests have been created to verify all operators:

### Test Files
- `test_operators_simple.k` - Basic functionality tests
- `test_operators_advanced.k` - Advanced features including short-circuit evaluation
- `test_comprehensive.k` - Complex scenarios combining multiple operators

### Running Tests
```bash
./klang run test_operators_simple.k
./klang run test_operators_advanced.k
./klang run test_comprehensive.k
```

### Test Coverage
- ✅ Logical operators with short-circuit evaluation
- ✅ Compound assignments with variables, objects, and arrays
- ✅ Prefix and postfix increment/decrement
- ✅ Optional chaining with nested objects
- ✅ Nullish coalescing with various types
- ✅ Operator precedence
- ✅ Complex expressions combining multiple operators

---

## Edge Cases and Special Behavior

### Arrays and Objects
- Compound assignments and increment/decrement operators work correctly with array elements
- Special handling ensures modifications are made in-place to the original array
- Object property modifications also work correctly

### Type Handling
- Logical operators evaluate truthiness:
  - `false`, `0`, `null`, `""` are falsy
  - Everything else is truthy
- Increment/decrement works with both integers and floats
- Nullish coalescing only treats `null` as nullish, not other falsy values

### Short-Circuit Evaluation
- `&&` and `||` implement proper short-circuit evaluation
- Side effects in the right operand won't occur if short-circuited
- This is critical for conditional execution and null checks

---

## Implementation Summary

### Files Modified
1. **src/lexer.h** - Added 11 new token types
2. **src/lexer.c** - Added token scanning logic for all new operators
3. **src/ast.h** - Added 3 new AST node types
4. **src/parser.c** - Added parsing logic for all operators
5. **src/interpreter.c** - Added evaluation logic for all operators

### Lines of Code Added
- Lexer: ~80 lines
- Parser: ~150 lines  
- Interpreter: ~200 lines
- Total: ~430 lines of production code

### Backward Compatibility
- All changes are backward compatible
- Existing code continues to work without modification
- New operators are additive features

---

## Future Enhancements

Possible future additions:
- Bitwise operators (`&`, `|`, `^`, `~`, `<<`, `>>`)
- Logical assignment operators (`&&=`, `||=`, `??=`)
- Exponentiation operator (`**`)
- Pipeline operator (`|>`)
- Pattern matching operators

---

## Conclusion

All five critical operator features have been successfully implemented and tested. The implementation follows best practices:
- Clean separation of concerns (lexer/parser/interpreter)
- Proper operator precedence
- Short-circuit evaluation for efficiency
- Comprehensive test coverage
- Backward compatibility maintained
