# KLang Basic Features Test Report

## Test Date: 2026-03-25

This document provides a comprehensive report on the basic features of KLang, specifically focusing on print statements and comments.

---

## 1. Print Statement Features ✅

### 1.1 Basic Print Functions

KLang provides two primary print functions:

- **`print()`** - Prints output without a newline
- **`println()`** - Prints output with a newline

**Example:**
```klang
print("Hello ")
print("World")  # Output: Hello World
println("!")    # Output: !↵
```

### 1.2 Printing Data Types

KLang supports printing all basic data types:

| Data Type | Example | Output |
|-----------|---------|--------|
| String | `println("Hello")` | Hello |
| Integer | `println(42)` | 42 |
| Float | `println(3.14)` | 3.14 |
| Boolean | `println(true)` | true |
| Null | `println(null)` | null |

### 1.3 Arithmetic Operations in Print ✅

All basic arithmetic operations work directly in print statements:

**Operators Tested:**
- ✅ Addition: `println(2 + 3)` → 5
- ✅ Subtraction: `println(10 - 4)` → 6
- ✅ Multiplication: `println(5 * 6)` → 30
- ✅ Division: `println(20 / 4)` → 5
- ✅ Modulo: `println(17 % 5)` → 2

**Example:**
```klang
println("Result: ", 2 + 3)          # Output: Result: 5
println("Complex: ", (2 + 3) * 4)   # Output: Complex: 20
```

### 1.4 Power Operations ⚠️

**Important Note:** KLang currently does NOT support the `**` operator for exponentiation.

❌ **NOT SUPPORTED:**
```klang
println(2 ** 4)  # ERROR: This syntax does not work
```

✅ **USE INSTEAD:**
```klang
println(Math.pow(2, 4))  # Output: 16
```

The `Math.pow(base, exponent)` function is available for power operations:
- `Math.pow(2, 4)` → 16
- `Math.pow(3, 3)` → 27
- `Math.pow(2.5, 2)` → 6.25
- `Math.pow(-2, 3)` → -8

### 1.5 Complex Expressions ✅

KLang correctly handles operator precedence and parentheses:

```klang
println(2 + 3 * 4)           # Output: 14 (multiplication first)
println((2 + 3) * 4)         # Output: 20 (parentheses first)
println(Math.pow(10 - 5, 2)) # Output: 25
```

### 1.6 Comparison Operations ✅

All comparison operators work in print:

```klang
println(5 > 3)    # Output: true
println(2 < 10)   # Output: true
println(5 == 5)   # Output: true
println(3 != 4)   # Output: true
println(7 >= 7)   # Output: true
println(4 <= 9)   # Output: true
```

### 1.7 Logical Operations ✅

Boolean logic operators are supported:

```klang
println(true && true)    # Output: true
println(true && false)   # Output: false
println(false || true)   # Output: true
println(!true)           # Output: false
```

### 1.8 String Concatenation ✅

String concatenation with the `+` operator:

```klang
println("Hello " + "World")           # Output: Hello World
println("Result: " + str(2 + 2))      # Output: Result: 4
```

### 1.9 Multiple Arguments ✅

Print functions accept multiple arguments:

```klang
println("x = ", 5, ", y = ", 3)  # Output: x = 5, y = 3
```

---

## 2. Comment Features ✅

KLang supports three types of comments:

### 2.1 Single-Line Comments with `#`

Python-style hash comments:

```klang
# This is a comment
println("Hello")  # Inline comment
```

### 2.2 Single-Line Comments with `//`

C-style double-slash comments:

```klang
// This is a comment
println("Hello")  // Inline comment
```

### 2.3 Multi-Line Comments with `/* */`

C-style block comments:

```klang
/*
This is a multi-line comment
It can span multiple lines
*/
println("After comment")
```

### 2.4 Documentation Comments `///`

Triple-slash comments for documentation:

```klang
/// This is a documentation comment
/// It describes the following code
let value = 42
```

### 2.5 Comment Features Tested ✅

All of the following work correctly:

- ✅ Single-line `#` comments
- ✅ Single-line `//` comments
- ✅ Multi-line `/* */` comments
- ✅ Documentation `///` comments
- ✅ Inline comments
- ✅ Empty comments
- ✅ Comments with special characters
- ✅ Comments with Unicode (世界, 🌍, etc.)
- ✅ Comments with URLs
- ✅ Comments with code snippets
- ✅ Consecutive comments
- ✅ Long comments

---

## 3. Test Results Summary

### Tests Created:
1. **test_print_calculations.kl** - 12 comprehensive test cases
2. **test_comments.kl** - 15 comprehensive test cases

### Test Execution:
```bash
./klang run tests/test_print_calculations.kl  # ✅ PASSED
./klang run tests/test_comments.kl            # ✅ PASSED
```

### Coverage:
- ✅ Basic arithmetic operations
- ✅ Floating point calculations
- ✅ Negative numbers
- ✅ Mixed operations
- ✅ Variables in calculations
- ✅ Comparison operations
- ✅ Logical operations
- ✅ String concatenation
- ✅ All comment styles
- ⚠️ Power operator (use Math.pow instead of **)

---

## 4. Known Limitations

### 4.1 Power Operator `**`
The `**` exponentiation operator is not currently supported in KLang. Use `Math.pow()` instead.

**Workaround:**
```klang
# Instead of: result = 2 ** 8
result = Math.pow(2, 8)  # Use this
```

---

## 5. Best Practices

### 5.1 Print Statements
- Use `println()` for output with newlines
- Use `print()` for output without newlines
- Use `str()` to convert numbers to strings for concatenation
- Multiple arguments are comma-separated

### 5.2 Comments
- Use `#` for quick inline comments (Python style)
- Use `//` for C-style comments
- Use `/* */` for multi-line explanations
- Use `///` for documentation

### 5.3 Calculations
- Always use parentheses to make precedence clear
- Use `Math.pow()` for exponentiation
- Be aware that integer division may truncate

---

## 6. Next Steps

Based on this testing, the following features have been validated:

1. ✅ Print statements work correctly
2. ✅ All arithmetic operations (except ** operator)
3. ✅ Comments work with all three syntaxes
4. ✅ Complex expressions with proper precedence
5. ✅ Comparison and logical operations

**Recommended Enhancements:**
1. Add support for `**` power operator in lexer and parser
2. Add more comprehensive test coverage for edge cases
3. Document the Math.pow() requirement more prominently

---

## 7. Conclusion

KLang successfully supports:
- ✅ Comprehensive print functionality
- ✅ Multiple comment styles (#, //, /* */)
- ✅ Arithmetic operations in print statements
- ✅ Complex expressions and calculations
- ⚠️ Power operations via Math.pow() (not ** operator)

All tests passed successfully, and the language is ready for basic use with these features.

---

**Test Files Location:**
- `/tests/test_print_calculations.kl`
- `/tests/test_comments.kl`

**Test Date:** March 25, 2026
**KLang Version:** 1.0.0-rc
