# KLang Comprehensive Basic Features Test Documentation

## Overview

This document describes the comprehensive test suite for KLang's basic features. These tests thoroughly validate all fundamental language features including control flow, loops, functions, recursion, string operations, array operations, and object operations.

## Test Files

### 1. Control Flow Tests (`test_control_flow_comprehensive.kl`)

**Coverage:**
- Basic if statements (true/false conditions)
- If-else statements
- If-else if-else chains (multiple conditions)
- Nested if statements (5 levels deep)
- Logical operators (&&, ||, !)
- Short-circuit evaluation
- Ternary operator (? :)
- Truthy/falsy value evaluation
- Real-world scenarios (authentication, shipping calculator, date validator)
- Edge cases (empty blocks, deep nesting)

**Test Sections:** 10
**Test Cases:** 50+

### 2. Loops Tests (`test_loops_comprehensive.kl`)

**Coverage:**
- While loops (basic, conditional, countdown)
- For loops (C-style: basic, step, backward, calculations)
- For-of loops (array iteration, mixed types)
- Break statement (in while, for, for-of, with conditions)
- Continue statement (skip patterns, multiple conditions)
- Nested loops (2D, 3D, matrix operations)
- Loop patterns (min/max, count, reverse, average)
- Complex scenarios (Fibonacci, prime check, filters)
- Performance patterns (large iterations, early exit)
- Edge cases (zero iterations, single iteration, empty arrays)

**Test Sections:** 10
**Test Cases:** 60+

### 3. Functions & Recursion Tests (`test_functions_recursion_comprehensive.kl`)

**Coverage:**
- Basic function declarations (no params, with params, multiple params)
- Return values (all types: int, float, string, bool, array, object, null)
- Function parameters (single, multiple, shadowing, expressions)
- Default parameters (single, multiple, with calculations)
- Recursive functions (factorial, fibonacci, sum of digits, power, GCD)
- Mutual recursion (even/odd check)
- Higher-order functions (callbacks, function return, composition)
- Closures (counters, adders, capture multiple variables)
- Function scope (local, global access, global modification, nested)
- Complex patterns (recursive array sum, max finder, string reversal, palindrome, binary search)
- Edge cases (empty functions, multiple returns, deep recursion)

**Test Sections:** 11
**Test Cases:** 70+

### 4. String Operations Tests (`test_strings_comprehensive.kl`)

**Coverage:**
- String creation (literals, empty, numbers, spaces, special chars)
- String concatenation (simple, multiple, with numbers, expressions)
- String length (basic, empty, long, spaces)
- String indexing (single char, last char, iteration)
- String comparison (equality, inequality, in conditions)
- String case operations (toUpperCase, toLowerCase if available)
- String searching (character search, substring search, count occurrences)
- String manipulation (reverse, repeat, substring, replace, remove)
- String formatting (templates, CSV builder, padding, centering)
- String validation (digits check, alpha check, blank check, starts/ends with)
- String trimming (left, right, both)
- Complex operations (word count, title case, char array, join, palindrome)
- Edge cases (empty strings, single char, long strings, special chars)

**Test Sections:** 13
**Test Cases:** 80+

### 5. Array Operations Tests (`test_arrays_comprehensive.kl`)

**Coverage:**
- Array creation (empty, integers, strings, mixed types, nested)
- Array access (by index, last element, nested arrays)
- Array modification (modify element, append, modify nested)
- Array iteration (for-of, index-based, backward)
- Array algorithms (sum, min, max, count, average)
- Array transformation (map, filter, reduce)
- Array searching (indexOf, contains, find)
- Array manipulation (reverse, slice, concat, unique)
- Sorting (bubble sort)
- Complex operations (flatten, zip, partition)
- Edge cases (empty arrays, single element, large arrays)

**Test Sections:** 11
**Test Cases:** 70+

### 6. Object Operations Tests (`test_objects_comprehensive.kl`)

**Coverage:**
- Object creation (empty, with properties, mixed types, with arrays)
- Property access (dot notation, nested properties, arrays in objects)
- Property modification (modify existing, add new, modify nested)
- Objects with methods (calculator, counter, custom methods)
- Object patterns (configuration, point/distance, rectangle)
- Object factory functions (person factory, counter factory)
- Object composition (nested objects, arrays of objects)
- Object utilities (property check, clone, merge)
- Real-world patterns (user profile, shopping cart, database record)
- Edge cases (empty objects, single property, deep nesting, many properties)

**Test Sections:** 10
**Test Cases:** 60+

## Running Tests

### Run All Comprehensive Tests
```bash
./tests/run_comprehensive_basic_tests.sh
```

### Run Individual Tests
```bash
./klang run tests/test_control_flow_comprehensive.kl
./klang run tests/test_loops_comprehensive.kl
./klang run tests/test_functions_recursion_comprehensive.kl
./klang run tests/test_strings_comprehensive.kl
./klang run tests/test_arrays_comprehensive.kl
./klang run tests/test_objects_comprehensive.kl
```

## Test Format

All tests follow a consistent format:

```klang
# Section header
println("Section X: Test Category")
println("-" * 40)

# Individual test with clear description
# Test X.Y: Description
let test_data = ...
if (condition) {
    println("✓ Test X.Y: PASS - description")
} else {
    println("✗ Test X.Y: FAIL - description")
}
```

## Test Results Indicators

- `✓` - Test passed
- `✗` - Test failed  
- `PASS` - Explicit pass marker
- Section summaries at end of each test file

## Coverage Summary

| Feature | Test File | Sections | Test Cases |
|---------|-----------|----------|------------|
| Control Flow | test_control_flow_comprehensive.kl | 10 | 50+ |
| Loops | test_loops_comprehensive.kl | 10 | 60+ |
| Functions & Recursion | test_functions_recursion_comprehensive.kl | 11 | 70+ |
| Strings | test_strings_comprehensive.kl | 13 | 80+ |
| Arrays | test_arrays_comprehensive.kl | 11 | 70+ |
| Objects | test_objects_comprehensive.kl | 10 | 60+ |
| **Total** | **6 test files** | **65 sections** | **390+ tests** |

## Test Philosophy

These tests follow these principles:

1. **Comprehensive** - Cover all basic features thoroughly
2. **Clear** - Each test has a clear description and expected result
3. **Incremental** - Tests go from simple to complex
4. **Real-world** - Include practical usage patterns
5. **Edge cases** - Test boundary conditions and special cases
6. **Self-documenting** - Tests serve as examples of correct usage

## Adding New Tests

To add new tests to the suite:

1. Create a new test file following the naming pattern: `test_<feature>_comprehensive.kl`
2. Use the standard section format with headers and separators
3. Include clear test descriptions with ✓/✗ indicators
4. Add edge cases and real-world scenarios
5. Include a summary section at the end
6. Add to `run_comprehensive_basic_tests.sh`
7. Update this documentation

## Test Maintenance

- Tests should be updated when language features change
- New language features should get comprehensive test coverage
- Failed tests should be investigated and fixed promptly
- Test output should remain clean and easy to read

## Known Limitations

- Some tests may need adjustment for specific language features
- Complex object operations may require additional runtime support
- Performance tests are not included in this suite
- Memory tests are in separate test files

## Future Test Additions

Planned comprehensive test suites:

- [ ] Variable Scoping (let, var, const)
- [ ] Type Operations (typeof, conversions)
- [ ] Advanced Features (async/await, promises, generators)
- [ ] Error Handling (try/catch/finally, throw)
- [ ] Classes and OOP (class, extends, super, static)
- [ ] Modules (import/export)
- [ ] Regular Expressions
- [ ] Date/Time Operations
- [ ] Math Operations

## Contact

For questions or issues with tests, please open an issue on the KLang repository.
