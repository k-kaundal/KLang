# KLang QA Test Suite

Comprehensive test suite for KLang print functionality and memory management.

## Quick Start

```bash
# Run all tests
./tests/quick_test.sh

# Run specific test
./tests/quick_test.sh print
./tests/quick_test.sh memory
./tests/quick_test.sh intensive
./tests/quick_test.sh edge

# Run with valgrind
./tests/quick_test.sh valgrind
```

## Test Files

### Core Test Files
1. **test_print_klang.kl** (1.8K) - Print/println functionality tests
   - All data types (strings, numbers, booleans, null)
   - Unicode characters
   - Escape sequences
   - Large strings
   - Rapid print operations

2. **test_memory_klang.kl** (3.5K) - Memory stress tests
   - Variable creation/destruction (100 iterations)
   - Function calls (100+)
   - Recursion (factorial)
   - Deep recursion (100 levels)
   - Nested scopes
   - Boolean operations

3. **test_memory_intensive.kl** (4.0K) - Intensive stress tests
   - Very deep recursion (500 levels)
   - Large strings (1000 characters)
   - Many function calls (10,000)
   - Nested scopes (100 iterations)
   - String operations (500 iterations)
   - Mixed arithmetic (10,000 operations)
   - Performance testing

4. **test_edge_cases.kl** (3.1K) - Edge case tests
   - Empty strings
   - Very long prints (4000 characters)
   - Zero values
   - Negative numbers
   - Boolean edge cases
   - Special characters
   - Minimal recursion
   - Variable shadowing

### Test Runners
- **quick_test.sh** - Fast test runner with colored output
- **run_comprehensive_tests.sh** - Full test suite with detailed reporting

## Test Results

### Status: ✅ ALL FUNCTIONAL TESTS PASS

- Print functionality: ✅ 100% pass
- Memory operations: ✅ 100% pass (functional)
- Stability: ✅ Zero crashes
- Performance: ✅ Excellent

### Known Issues: ⚠️ Memory Leaks

- Memory leaks detected: 15-100KB per run
- **This is intentional** (prevents double-free)
- Source: `src/interpreter.c` (env_new, env_set_local)
- Impact: Acceptable for development, problematic for production

## Documentation

Detailed reports available in project root:

1. **QA_COMPLETE_REPORT.md** (10.6K)
   - Full analysis with source code investigation
   - Performance metrics
   - Detailed recommendations

2. **QA_TEST_REPORT.md** (10.2K)
   - Test-by-test breakdown
   - Valgrind analysis
   - Issue descriptions

3. **TESTING_SUMMARY.md** (2.8K)
   - Quick reference guide
   - Command examples

4. **tests/test_summary.txt** (3.3K)
   - Plain text summary

## Running Individual Tests

```bash
# Basic print test
./klang run tests/test_print_klang.kl

# Memory stress test
./klang run tests/test_memory_klang.kl

# Intensive stress test
./klang run tests/test_memory_intensive.kl

# Edge case test
./klang run tests/test_edge_cases.kl
```

## Running with Valgrind

```bash
# Basic memory check
valgrind --leak-check=full ./klang run tests/test_print_klang.kl

# Detailed memory analysis
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
  ./klang run tests/test_memory_intensive.kl
```

## Test Coverage

- **Total Test Lines:** 562
- **Test Scenarios:** 50+
- **Data Types:** All core types
- **Operations:** 25,000+ per intensive test
- **Recursion Depth:** Up to 500 levels
- **String Length:** Up to 4,000 characters
- **Function Calls:** 10,000+ tested

## Performance Benchmarks

- Basic tests: ~0.05 seconds
- Intensive tests: 0.076 seconds
- Operations/sec: ~328,000
- Memory overhead: ~0.004KB per operation

## Grades

**Overall: B+ (90/100)**

- Print/IO: A+ (100/100) ✅
- Stability: A+ (100/100) ✅
- Performance: A (95/100) ✅
- Memory Mgmt: B (85/100) ⚠️
- Error Handling: A (95/100) ✅

## Recommendations

### Critical (P0) - Production Blockers
1. Implement safe memory cleanup
2. Enable env_free() with proper lifecycle
3. Fix env_set_local memory leak
4. Fix env_declare memory leak

### High Priority (P1)
5. Add garbage collection
6. Integrate valgrind in CI/CD
7. Add memory monitoring

### Medium Priority (P2)
8. Add 'undefined' literal support
9. Optimize per-scope memory
10. Improve documentation

## Continuous Testing

Add to CI/CD pipeline:

```yaml
# Example GitHub Actions
- name: Run KLang Tests
  run: ./tests/quick_test.sh

- name: Valgrind Check
  run: |
    valgrind --leak-check=full --error-exitcode=1 \
      ./klang run tests/test_print_klang.kl
```

## Contributing

When adding new features:
1. Add tests to appropriate test file
2. Run `./tests/quick_test.sh` to verify
3. Update this README if needed
4. Check with valgrind for new leaks

## License

Same as KLang project license.

## Questions?

See detailed reports in project root:
- QA_COMPLETE_REPORT.md
- QA_TEST_REPORT.md
- TESTING_SUMMARY.md
