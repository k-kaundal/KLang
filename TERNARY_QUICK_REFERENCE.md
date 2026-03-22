# Ternary Conditional Operator - Quick Reference

## Syntax
```javascript
condition ? valueIfTrue : valueIfFalse
```

## Examples

### Basic Usage
```javascript
let age = 18
let status = age >= 18 ? "adult" : "minor"
```

### Nested (Right-Associative)
```javascript
let score = 85
let grade = score >= 90 ? "A" : score >= 80 ? "B" : "C"
// Equivalent to: score >= 90 ? "A" : (score >= 80 ? "B" : "C")
```

### In Functions
```javascript
fn abs(x) {
    return x >= 0 ? x : -x
}

fn max(a, b) {
    return a > b ? a : b
}
```

### With Expressions
```javascript
let result = (a + b) > 10 ? a * 2 : b * 2
```

### Multiple Ternaries
```javascript
let r1 = x > 5 ? 10 : 20
let r2 = y > 5 ? 30 : 40
```

## Truthiness Rules
- **true/false**: Boolean values
- **Numbers**: 0 is falsy, all others are truthy
- **Strings**: Empty string "" is falsy, all others are truthy
- **null**: Always falsy
- **Objects/Lists**: Always truthy

## Precedence
Lower precedence than equality (`==`, `!=`) but higher than assignment (`=`):
```javascript
// These are equivalent:
x == 5 ? a : b
(x == 5) ? a : b

// Assignment has lowest precedence:
let result = x > 5 ? a : b
// Same as: let result = (x > 5 ? a : b)
```

## Best Practices

### ✅ Good Uses
- Simple conditional assignment
- Inline conditional returns
- Selecting between two expressions

```javascript
// Clear and concise
let max = a > b ? a : b
return x >= 0 ? x : -x
let msg = isValid ? "success" : "error"
```

### ⚠️ Use with Caution
- Deeply nested ternaries (can be hard to read)
- Complex conditions

```javascript
// Hard to read:
let x = a ? b ? c : d : e ? f : g

// Better with if-else:
let x
if (a) {
    x = b ? c : d
} else {
    x = e ? f : g
}
```

## Testing
Run the comprehensive test suite:
```bash
make test
```

All 210 tests pass, including 25 ternary-specific tests.

## See Also
- `examples/ternary_operator.k` - More examples
- `TERNARY_OPERATOR_IMPLEMENTATION.md` - Implementation details
- `tests/test_ternary.c` - Test cases
