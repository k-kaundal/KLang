# KLang Basic Features Tests

This directory contains comprehensive tests for KLang's basic features, with a focus on print statements and comments.

## Test Files

### 1. test_print_calculations.kl
Comprehensive tests for print statements with calculations:
- Basic arithmetic operations (+, -, *, /, %)
- Power operations using Math.pow()
- Complex expressions with parentheses
- Floating point calculations
- Negative numbers
- Mixed operations
- Large calculations
- Variables in calculations
- String concatenation with calculations
- Comparison operations
- Logical operations
- Combined operations

**Run with:**
```bash
./klang run tests/test_print_calculations.kl
```

### 2. test_comments.kl
Comprehensive tests for comment syntax:
- Single-line # comments
- Single-line // comments
- Multi-line /* */ comments
- Documentation /// comments
- Inline comments
- Empty comments
- Comments with special characters
- Comments with Unicode
- Comments with URLs
- Comments with code snippets
- Consecutive comments
- Long comments

**Run with:**
```bash
./klang run tests/test_comments.kl
```

### 3. test_print_basic.kl
Basic print functionality tests (pre-existing):
- Simple strings
- Numbers (integers, floats, negative)
- Booleans
- Null values
- Variables
- Expressions
- String concatenation
- Loops
- Functions

**Run with:**
```bash
./klang run tests/test_print_basic.kl
```

## Demo File

### examples/basic_features_demo.kl
A comprehensive, working demo that showcases all basic features:
- Print statements with multiple data types
- Arithmetic operations
- Power operations
- Complex expressions
- Variables in calculations
- Comparisons and logic
- String operations
- Real-world examples (shopping cart, temperature conversion, interest calculation)

**Run with:**
```bash
./klang run examples/basic_features_demo.kl
```

## Test Runner

### run_basic_features_tests.sh
Automated test runner script that executes all basic feature tests and provides a summary.

**Usage:**
```bash
./tests/run_basic_features_tests.sh
```

**Output:**
- Shows pass/fail status for each test
- Displays error messages for failed tests
- Provides summary with total passed/failed counts

## Test Coverage

### Print Statements ✅
- [x] print() - output without newline
- [x] println() - output with newline
- [x] Multiple data types (string, int, float, boolean, null)
- [x] Multiple arguments
- [x] Arithmetic expressions
- [x] Complex expressions with parentheses
- [x] Variables in print
- [x] String concatenation
- [x] Comparison operations
- [x] Logical operations

### Arithmetic Operations ✅
- [x] Addition (+)
- [x] Subtraction (-)
- [x] Multiplication (*)
- [x] Division (/)
- [x] Modulo (%)
- [x] Power (Math.pow - not **)
- [x] Order of operations
- [x] Parentheses grouping

### Comments ✅
- [x] Hash comments (#)
- [x] Double-slash comments (//)
- [x] Multi-line comments (/* */)
- [x] Documentation comments (///)
- [x] Inline comments
- [x] Empty comments
- [x] Comments with special characters
- [x] Comments with Unicode

## Known Limitations

### Power Operator
⚠️ The `**` exponentiation operator is NOT supported in KLang.

**Workaround:** Use `Math.pow(base, exponent)`

```klang
# ❌ Does not work
result = 2 ** 8

# ✅ Use this instead
result = Math.pow(2, 8)
```

## Test Results

All tests passing as of: **March 25, 2026**

```
✅ test_print_calculations.kl - PASSED
✅ test_comments.kl - PASSED
✅ test_print_basic.kl - PASSED
✅ basic_features_demo.kl - PASSED
✅ hello.kl - PASSED
```

## Documentation

For detailed information about these features, see:
- [BASIC_FEATURES_TEST_REPORT.md](../BASIC_FEATURES_TEST_REPORT.md) - Comprehensive test report
- [PRINT_AND_COMMENTS_QUICK_REFERENCE.md](../docs/PRINT_AND_COMMENTS_QUICK_REFERENCE.md) - Quick reference guide
- [USER_GUIDE.md](../docs/USER_GUIDE.md) - Complete user guide

## Contributing

When adding new tests:
1. Create a descriptive test file in this directory
2. Add test case to run_basic_features_tests.sh
3. Document the test in this README
4. Ensure all tests pass before committing

## Running All Tests

To run all basic feature tests at once:

```bash
cd /path/to/KLang
./tests/run_basic_features_tests.sh
```

This will execute all tests and provide a comprehensive summary.
