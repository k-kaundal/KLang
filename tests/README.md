# KLang Test Suite

This directory contains the comprehensive test suite for KLang, covering all language features from basic syntax to advanced constructs.

## 📁 Test Organization

### `/unit` - Unit Tests (C)
Low-level tests for individual components:
- `test_lexer.c` - Tokenization tests
- `test_parser.c` - AST generation tests
- `test_interpreter.c` - Execution tests
- `test_runtime.c` - Runtime function tests
- Feature-specific tests (arrow functions, async/await, generators, etc.)

**Run**: `make test` or compile individually with gcc

### `/features` - Feature Tests (KLang)
High-level tests for language features:
- `test_math.k` - Math module functionality
- `test_range.k` - range() function
- `test_async_await.k` - Async/await patterns
- `test_generators.k` - Generator functions
- OOP tests, array methods, etc.

**Run**: `./klang run tests/features/test_<feature>.k`

### `/integration` - Integration Tests
End-to-end tests for complete programs:
- Multi-file programs
- Module imports
- Cross-feature interaction
- Real-world use cases

### `/examples` - Example Programs as Tests
Example programs that also serve as tests:
- From the `examples/` directory
- Verified to work correctly
- Serve as documentation

### `/fixtures` - Test Data
Test data and fixtures:
- Sample input files
- Expected output files
- Test configuration

## 🧪 Running Tests

### All Tests
```bash
# Run all tests
make test

# Or using test runner
cd tests && ./run_all_tests.sh
```

### Specific Test Categories
```bash
# Unit tests only
make test-unit

# Feature tests only
make test-features

# Integration tests only
make test-integration
```

### Individual Tests
```bash
# C unit test
gcc -o test_lexer tests/unit/test_lexer.c src/lexer.c -Isrc
./test_lexer

# KLang feature test
./klang run tests/features/test_math.k
```

## ✅ Test Coverage

### Core Language (100%)
- ✅ Lexer - Tokenization
- ✅ Parser - AST generation
- ✅ Interpreter - Execution
- ✅ Variables - let, var, const
- ✅ Functions - Regular and arrow functions
- ✅ Control flow - if/else, loops
- ✅ Operators - Arithmetic, logical, comparison

### Object-Oriented Programming (100%)
- ✅ Classes - Definition and instantiation
- ✅ Constructors - init method
- ✅ Methods - Instance and static
- ✅ Inheritance - extends keyword
- ✅ Access modifiers - public/private/protected
- ✅ Abstract classes
- ✅ this and super keywords

### Advanced Features (90%)
- ✅ Arrow functions - `() => {}`
- ✅ Async/await - Asynchronous programming
- ✅ Promises - Promise API
- ✅ Generators - function* and yield
- ✅ Destructuring - Arrays and objects
- ✅ Spread/Rest - ... operator
- ✅ Template literals - String interpolation
- ✅ For-of loops
- ✅ Ternary operator
- ⚠️ Tuples - 90% (known bug)

### Standard Library (70%)
- ✅ Math module - 20+ functions
- ✅ String operations - 12+ functions
- ✅ Array methods - 15+ methods
- ✅ range() - Python-style iteration
- ❌ File I/O - Planned
- ❌ OS operations - Planned
- ❌ JSON module - Planned

## 📊 Test Statistics

- **Total Tests**: 64 files
- **Unit Tests (C)**: 16 files
- **Feature Tests (KLang)**: 28 files
- **Integration Tests**: 0 (planned)
- **Test Coverage**: ~85%

## 🎯 Testing Strategy

### 1. Unit Tests
- Test individual components in isolation
- Written in C for low-level testing
- Fast execution
- High coverage

### 2. Feature Tests
- Test language features end-to-end
- Written in KLang itself
- Validate user-facing behavior
- Serve as documentation

### 3. Integration Tests
- Test multiple components together
- Real-world scenarios
- Cross-feature interaction
- Performance testing

### 4. Regression Tests
- Prevent reintroduction of bugs
- Automated on every commit
- CI/CD integration

## 🐛 Known Issues

### Critical
1. **Tuple double-free bug**
   - Status: Parser works, runtime has memory issue
   - Test: `tests/features/test_tuple.k` (currently fails)
   - Priority: CRITICAL

### Medium
2. **For-of with parentheses**
   - Parser doesn't handle `for ((x) of arr)`
   - Workaround: Use `for (x of arr)`

## 📝 Writing Tests

### Unit Test Template (C)
```c
#include "../test_runner.h"
#include "../../src/lexer.h"

void test_my_feature() {
    // Setup
    Lexer *lexer = lexer_new("let x = 5");
    
    // Execute
    Token *token = lexer_next_token(lexer);
    
    // Assert
    assert(token->type == TOK_LET);
    
    // Cleanup
    lexer_free(lexer);
}

int main() {
    RUN_TEST(test_my_feature);
    PRINT_TEST_RESULTS();
    return 0;
}
```

### Feature Test Template (KLang)
```javascript
// Test: Math module sqrt function
print("Testing Math.sqrt()...")

let result = Math.sqrt(16)
if (result == 4) {
    print("✅ Math.sqrt(16) = 4")
} else {
    print("❌ Math.sqrt(16) failed, got: " + str(result))
}

print("All tests passed!")
```

## 🔄 Continuous Integration

Tests are automatically run on:
- Every commit to main branch
- Every pull request
- Nightly builds
- Release tags

CI configuration: `.github/workflows/test.yml`

## 📈 Adding New Tests

When adding a new feature:
1. Write unit tests first (TDD)
2. Implement the feature
3. Write feature tests
4. Update this documentation
5. Ensure all tests pass

## 🏆 Test Quality Guidelines

- **Clear**: Test names describe what they test
- **Independent**: Tests don't depend on each other
- **Fast**: Tests run quickly
- **Deterministic**: Same input = same output
- **Complete**: Edge cases covered

## 🔧 Troubleshooting

### Tests Won't Compile
```bash
# Clean build
make clean
make

# Rebuild tests
cd tests/unit
gcc -Wall -o test_lexer test_lexer.c ../../src/lexer.c -I../../src
```

### Tests Fail Unexpectedly
1. Check that klang binary is up to date: `make clean && make`
2. Verify test file syntax
3. Check for known issues above
4. Run tests individually to isolate problem

### Adding New Test Files
1. Place in appropriate directory
2. Follow naming convention: `test_<feature>.c` or `test_<feature>.k`
3. Add to test runner if needed
4. Update this README

## 📚 Related Documentation

- [Testing Guide](../docs/guides/TESTING_GUIDE.md) - Comprehensive testing guide
- [Contributing](../docs/development/CONTRIBUTING.md) - How to contribute
- [Development Setup](../docs/development/) - Setting up dev environment

## 🆘 Need Help?

- [GitHub Issues](https://github.com/k-kaundal/KLang/issues)
- [Discussions](https://github.com/k-kaundal/KLang/discussions)
- Check existing tests for examples

---

**Last Updated**: March 2026  
**Test Framework Version**: 1.0  
**Coverage Target**: 90%+
