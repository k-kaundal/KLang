#!/bin/bash
# Comprehensive Test Script for KLang Basic Features
# This script validates print statements and comments

echo "======================================"
echo "KLang Basic Features Test Suite"
echo "======================================"
echo ""

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0

# Function to run a test
run_test() {
    local test_name=$1
    local test_file=$2
    
    echo -n "Running $test_name... "
    
    if ./klang run "$test_file" > /tmp/test_output.txt 2>&1; then
        if grep -q "Error\|Parse error" /tmp/test_output.txt; then
            echo -e "${RED}FAILED${NC}"
            echo "  Error found in output:"
            grep "Error\|Parse error" /tmp/test_output.txt | head -3
            ((FAILED++))
        else
            echo -e "${GREEN}PASSED${NC}"
            ((PASSED++))
        fi
    else
        echo -e "${RED}FAILED${NC}"
        echo "  Exit code: $?"
        ((FAILED++))
    fi
}

# Check if klang binary exists
if [ ! -f "./klang" ]; then
    echo -e "${RED}Error: klang binary not found!${NC}"
    echo "Please build KLang first with: make"
    exit 1
fi

echo "Starting tests..."
echo ""

# Test 1: Print Calculations
run_test "Print Calculations Test" "tests/test_print_calculations.kl"

# Test 2: Comments
run_test "Comments Test" "tests/test_comments.kl"

# Test 3: Basic Features Demo
run_test "Basic Features Demo" "examples/basic_features_demo.kl"

# Test 4: Hello World
run_test "Hello World" "examples/hello.kl"

# Test 5: Basic Print Test
run_test "Basic Print Test" "tests/test_print_basic.kl"

echo ""
echo "======================================"
echo "Test Results Summary"
echo "======================================"
echo -e "Passed: ${GREEN}$PASSED${NC}"
echo -e "Failed: ${RED}$FAILED${NC}"
echo "Total: $((PASSED + FAILED))"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed!${NC}"
    exit 1
fi
