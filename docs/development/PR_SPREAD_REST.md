# Pull Request: JavaScript Spread and Rest Operators

## Summary

This PR implements JavaScript ES6-style **spread** (`...`) and **rest** (`...`) operators in KLang, enabling modern array manipulation, flexible function parameters, and object composition.

## Features Implemented

### 1. Array Spread
Expand arrays inline:
```javascript
const combined = [...[1,2,3], ...[4,5,6]]  // [1, 2, 3, 4, 5, 6]
const mixed = [0, ...arr, 99]               // Mix with literals
const copy = [...original]                  // Create copy
```

### 2. Rest Parameters
Collect remaining function arguments into an array:
```javascript
fn sum(...numbers) {
    let total = 0
    let i = 0
    while (i < len(numbers)) {
        total = total + numbers[i]
        i = i + 1
    }
    return total
}

sum(1, 2, 3, 4, 5)  // 15
```

### 3. Object Spread
Merge and override object properties:
```javascript
const person = {name: "Alice", age: 30}
const location = {city: "NYC"}
const profile = {...person, ...location}
// {name: "Alice", age: 30, city: "NYC"}

const defaults = {theme: "light", size: 14}
const custom = {...defaults, theme: "dark"}
// {theme: "dark", size: 14}  // Override theme
```

## Technical Implementation

### Lexer Changes
- Added `TOKEN_SPREAD` for `...` (three dots)
- Distinguished from `TOKEN_DOTDOT` (`..`) used for ranges

### AST Changes
- New node type: `NODE_SPREAD` for spread expressions
- Added `has_rest_param` flag to function definitions
- Created `ast_new_spread()` constructor
- Updated `ast_free()` to handle spread nodes

### Parser Changes
- Array literal parsing detects and handles spread elements
- Function parameter parsing recognizes rest parameters
- Object literal parsing supports spread properties
- Validation: rest parameter must be last parameter

### Interpreter Changes
- Added `has_rest_param` to `FunctionVal` struct
- **Array spread**: Two-pass evaluation (count, then fill) with deep copying
- **Rest parameters**: Collect arguments into standard array
- **Object spread**: Copy properties with override support
- Proper memory management for nested structures

## Files Modified

- `src/lexer.h` & `src/lexer.c` - Token recognition
- `src/ast.h` & `src/ast.c` - AST nodes and constructors
- `src/parser.c` - Syntax parsing
- `src/interpreter.h` & `src/interpreter.c` - Evaluation logic

## Files Added

### Documentation
- `SPREAD_REST_OPERATORS.md` - User guide
- `SPREAD_REST_IMPLEMENTATION.md` - Implementation details

### Tests
- `test_spread_simple.k` - Array spread tests
- `test_rest_simple.k` - Rest parameter tests  
- `test_object_spread.k` - Object spread tests
- `test_spread_rest.sh` - Automated test runner

### Examples
- `examples/spread_rest_demo.k` - Comprehensive demonstration

## Test Results

All tests pass successfully:
```
Test 1: Array spread          ✓ PASSED
Test 2: Rest parameters        ✓ PASSED
Test 3: Object spread          ✓ PASSED
Test 4: Comprehensive demo     ✓ PASSED
```

## Usage Examples

### Variadic Math Functions
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

### Array Manipulation
```javascript
const base = [1, 2, 3]
const extended = [...base, 4, 5, 6]
const empty = []
const withEmpty = [0, ...empty, 1]  // [0, 1]
```

### Configuration Merging
```javascript
const defaultConfig = {debug: false, timeout: 5000}
const prodConfig = {...defaultConfig, timeout: 10000}
const devConfig = {...defaultConfig, debug: true}
```

## Compatibility

- ✅ Fully compatible with existing KLang code
- ✅ No breaking changes
- ✅ Works with async/await functions
- ✅ Works with generator functions
- ✅ Works with class methods
- ✅ Works with arrow functions

## Benefits

1. **Modern Syntax**: Aligns with JavaScript ES6+
2. **Code Clarity**: Cleaner array/object operations
3. **Flexibility**: Easy variadic functions
4. **Composability**: Simple merging and copying
5. **Developer Experience**: Familiar syntax for JS developers

## Performance

- Spread creates new collections (immutable pattern)
- Efficient two-pass array construction
- Deep copying prevents memory issues
- Minimal overhead for rest parameters

## Limitations

- Rest parameter must be last in function signature
- No spread for other iterables (only arrays/objects)
- Some memory management considerations with complex nesting

## How to Test

Run the test suite:
```bash
./test_spread_rest.sh
```

Run the demo:
```bash
./klang run examples/spread_rest_demo.k
```

Individual tests:
```bash
./klang run test_spread_simple.k
./klang run test_rest_simple.k
./klang run test_object_spread.k
```

## Future Enhancements

Potential improvements:
- Spread for strings and other iterables
- Object/array rest destructuring
- Performance optimizations for large collections

## Conclusion

This implementation successfully brings modern JavaScript spread/rest syntax to KLang, enhancing developer experience while maintaining backward compatibility and type safety.
