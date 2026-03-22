# Ternary Conditional Operator Implementation

## Overview
Implemented the ternary conditional operator (`condition ? trueValue : falseValue`) for KLang, providing a concise way to write conditional expressions.

## Syntax
```javascript
condition ? trueValue : falseValue
```

## Features Implemented

### 1. Lexer Changes (`src/lexer.h`, `src/lexer.c`)
- Added `TOKEN_QUESTION` token type to represent the `?` operator
- Updated lexer to recognize `?` as a single-character token
- Added token name mapping for `TOKEN_QUESTION`

### 2. AST Changes (`src/ast.h`, `src/ast.c`)
- Added `NODE_TERNARY` node type to represent ternary expressions
- Added ternary structure to AST union with three fields:
  - `cond`: The condition expression
  - `true_expr`: Expression evaluated when condition is truthy
  - `false_expr`: Expression evaluated when condition is falsy
- Implemented `ast_new_ternary()` constructor
- Added proper memory cleanup in `ast_free()` for ternary nodes

### 3. Parser Changes (`src/parser.c`)
- Added `parse_ternary()` function with right-associative parsing
- Positioned ternary operator between equality and assignment in precedence hierarchy
- Supports nested ternary expressions through recursive calls
- Updated `parse_expression()` to call `parse_ternary()`

### 4. Interpreter Changes (`src/interpreter.c`)
- Added `NODE_TERNARY` case in `eval_node_env()`
- Implements truthiness evaluation for conditions:
  - `VAL_BOOL`: True if bool_val is true
  - `VAL_INT`: True if non-zero
  - `VAL_FLOAT`: True if non-zero
  - `VAL_STRING`: True if non-empty
  - `VAL_NULL`: Always false
  - Other types: Always true
- Only evaluates one branch (short-circuit evaluation)

### 5. Tests (`tests/test_ternary.c`)
Comprehensive test suite with 25 tests covering:
- Basic true/false conditions
- Number and string values
- Nested ternary expressions
- Ternary in function returns
- Complex conditions
- Ternary with arithmetic operations
- Multiple independent ternaries
- Ternary with equality/inequality checks
- Deep nesting (chain of ternaries)
- Ternary with function calls
- Ternary as function arguments
- Ternary in loops
- Ternary with list indexing

## Precedence
The ternary operator has lower precedence than equality operators but higher than assignment, following JavaScript/C conventions:
```
parse_expression
  └─> parse_ternary
        └─> parse_equality
              └─> parse_comparison
                    └─> parse_additive
                          └─> parse_multiplicative
                                └─> parse_unary
```

## Associativity
The ternary operator is **right-associative**, meaning:
```javascript
a ? b : c ? d : e
```
is parsed as:
```javascript
a ? b : (c ? d : e)
```

## Examples

### Basic Usage
```javascript
let x = 10
let result = x > 5 ? "big" : "small"
println(result)  // "big"
```

### Nested Ternary
```javascript
let score = 85
let grade = score >= 90 ? "A" : score >= 80 ? "B" : "C"
println(grade)  // "B"
```

### With Expressions
```javascript
let a = 10
let b = 20
let max = a > b ? a : b
println(max)  // 20
```

### In Functions
```javascript
fn abs(x) {
    return x >= 0 ? x : -x
}
println(abs(-15))  // 15
```

### Complex Conditions
```javascript
let num = 15
let category = num > 20 ? "very large" : num > 10 ? "large" : "small"
println(category)  // "large"
```

## Test Results
All 25 tests pass successfully, verifying:
- ✅ Basic true/false evaluation
- ✅ Different value types (numbers, strings, booleans)
- ✅ Nested ternary expressions
- ✅ Integration with functions
- ✅ Complex condition evaluation
- ✅ Proper short-circuit evaluation
- ✅ Edge cases (null, zero, empty strings)

## Files Modified
1. `src/lexer.h` - Added TOKEN_QUESTION
2. `src/lexer.c` - Implemented ? token recognition
3. `src/ast.h` - Added NODE_TERNARY and ternary structure
4. `src/ast.c` - Implemented ast_new_ternary and cleanup
5. `src/parser.c` - Added parse_ternary with right-associativity
6. `src/interpreter.c` - Implemented ternary evaluation with truthiness
7. `tests/test_ternary.c` - 25 comprehensive tests
8. `tests/test_runner.h` - Added test declaration
9. `tests/test_runner.c` - Added test invocation
10. `Makefile` - Added test_ternary.c to build
11. `examples/ternary_operator.k` - Example usage

## Compatibility
The implementation follows JavaScript/C conventions for:
- Syntax: `condition ? trueValue : falseValue`
- Precedence: Between assignment and equality
- Associativity: Right-associative
- Truthiness: Follows common dynamic language conventions

## Performance
- Short-circuit evaluation: Only the selected branch is evaluated
- No unnecessary allocations: Direct evaluation without intermediate values
- Proper memory management: All values are properly freed

## Future Enhancements
Potential improvements:
- Optional chaining with ternary: `obj?.prop ? a : b`
- Nullish coalescing integration: `a ?? (b ? c : d)`
- Type inference for ternary expressions in a future type system
