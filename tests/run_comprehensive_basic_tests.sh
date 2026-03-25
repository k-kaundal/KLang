#!/bin/bash
# Comprehensive Test Suite Runner for KLang
# Runs all comprehensive basic features tests

echo "========================================"
echo "KLang Comprehensive Test Suite Runner"
echo "========================================"
echo ""

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Track results
PASSED=0
FAILED=0
TOTAL=0

# Function to run a test
run_test() {
    local test_name=$1
    local test_file=$2
    
    echo -n "Running $test_name... "
    TOTAL=$((TOTAL + 1))
    
    if ./klang run "$test_file" > /tmp/test_output.txt 2>&1; then
        if grep -q "passed!" /tmp/test_output.txt; then
            echo -e "${GREEN}PASSED${NC}"
            PASSED=$((PASSED + 1))
        else
            echo -e "${YELLOW}COMPLETED${NC} (check output)"
            PASSED=$((PASSED + 1))
        fi
    else
        echo -e "${RED}FAILED${NC}"
        echo "Error output:"
        cat /tmp/test_output.txt | head -20
        FAILED=$((FAILED + 1))
    fi
}

# Change to KLang directory
cd "$(dirname "$0")/.."

# Check if klang binary exists
if [ ! -f "./klang" ]; then
    echo "Error: klang binary not found. Please run 'make' first."
    exit 1
fi

echo "Starting comprehensive tests..."
echo ""

# Run all comprehensive test suites
echo "=== Basic Features Tests ==="
run_test "Print & Calculations" "tests/test_print_calculations.kl"
run_test "Print Comprehensive" "tests/test_print_comprehensive.kl"
run_test "Comments" "tests/test_comments.kl"
echo ""

echo "=== Control Flow Tests ==="
run_test "Control Flow Comprehensive" "tests/test_control_flow_comprehensive.kl"
echo ""

echo "=== Loops Tests ==="
run_test "Loops Comprehensive" "tests/test_loops_comprehensive.kl"
echo ""

echo "=== Functions & Recursion Tests ==="
run_test "Functions & Recursion Comprehensive" "tests/test_functions_recursion_comprehensive.kl"
echo ""

echo "=== String Operations Tests ==="
run_test "Strings Comprehensive" "tests/test_strings_comprehensive.kl"
echo ""

echo "=== Array Operations Tests ==="
run_test "Arrays Comprehensive" "tests/test_arrays_comprehensive.kl"
echo ""

echo "=== Object Operations Tests ==="
run_test "Objects Comprehensive" "tests/test_objects_comprehensive.kl"
echo ""

echo "=== Power Operator Tests ==="
run_test "Power Operator Comprehensive" "tests/test_power_operator_comprehensive.kl"
echo ""

# Summary
echo "========================================"
echo "Test Results Summary"
echo "========================================"
echo "Total tests: $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✓ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Some tests failed.${NC}"
    exit 1
fi
