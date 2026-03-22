#!/bin/bash

# KLang Test Runner
# Comprehensive test suite execution

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
SKIPPED_TESTS=0

# Print header
echo -e "${BLUE}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║         KLang Comprehensive Test Suite Runner            ║${NC}"
echo -e "${BLUE}╚══════════════════════════════════════════════════════════╝${NC}"
echo ""

# Check if klang binary exists
if [ ! -f "./klang" ]; then
    echo -e "${RED}❌ Error: klang binary not found!${NC}"
    echo -e "${YELLOW}Please run 'make' to build KLang first.${NC}"
    exit 1
fi

# Function to run a test
run_test() {
    local test_file=$1
    local test_name=$(basename "$test_file")
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "  Testing $test_name... "
    
    # Run the test and capture output
    if ./klang run "$test_file" > /tmp/klang_test_output.txt 2>&1; then
        echo -e "${GREEN}✅ PASS${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ FAIL${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        echo -e "${YELLOW}Output:${NC}"
        cat /tmp/klang_test_output.txt | head -10
    fi
}

# Function to run C unit tests
run_c_test() {
    local test_exe=$1
    local test_name=$(basename "$test_exe")
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "  Testing $test_name... "
    
    if [ -f "$test_exe" ] && [ -x "$test_exe" ]; then
        if "$test_exe" > /tmp/klang_test_output.txt 2>&1; then
            echo -e "${GREEN}✅ PASS${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}❌ FAIL${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
            echo -e "${YELLOW}Output:${NC}"
            cat /tmp/klang_test_output.txt | head -10
        fi
    else
        echo -e "${YELLOW}⚠️  SKIP (not built)${NC}"
        SKIPPED_TESTS=$((SKIPPED_TESTS + 1))
    fi
}

# Run unit tests (C)
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}Running Unit Tests (C)${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

if [ -d "tests/unit" ]; then
    for test in tests/unit/test_*; do
        if [ -x "$test" ] && [ ! -f "$test.c" ]; then
            run_c_test "$test"
        fi
    done
else
    echo -e "${YELLOW}⚠️  No unit tests directory found${NC}"
fi

# Run feature tests (KLang)
echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}Running Feature Tests (KLang)${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

if [ -d "tests/features" ]; then
    for test in tests/features/test_*.k; do
        if [ -f "$test" ]; then
            run_test "$test"
        fi
    done
else
    echo -e "${YELLOW}⚠️  No features tests directory found${NC}"
fi

# Run example tests
echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}Running Example Programs${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

if [ -d "examples" ]; then
    # Test a few key examples
    for example in examples/hello.kl examples/factorial.kl examples/fibonacci.kl; do
        if [ -f "$example" ]; then
            run_test "$example"
        fi
    done
else
    echo -e "${YELLOW}⚠️  No examples directory found${NC}"
fi

# Print summary
echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}Test Summary${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""
echo -e "  Total Tests:   $TOTAL_TESTS"
echo -e "  ${GREEN}Passed:        $PASSED_TESTS${NC}"
echo -e "  ${RED}Failed:        $FAILED_TESTS${NC}"
echo -e "  ${YELLOW}Skipped:       $SKIPPED_TESTS${NC}"
echo ""

# Calculate percentage
if [ $TOTAL_TESTS -gt 0 ]; then
    PASS_PERCENT=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo -e "  Pass Rate:     ${GREEN}${PASS_PERCENT}%${NC}"
else
    echo -e "  ${YELLOW}No tests were run!${NC}"
fi

echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

# Exit with appropriate code
if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed!${NC}"
    exit 1
fi
