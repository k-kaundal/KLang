#!/bin/bash
# Quick Test Runner for KLang QA Suite
# Usage: ./quick_test.sh [test_name]

KLANG="./klang"
TEST_DIR="tests"

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "╔═══════════════════════════════════════╗"
echo "║     KLang QA Quick Test Runner       ║"
echo "╚═══════════════════════════════════════╝"
echo ""

# Function to run a test
run_test() {
    local test_file=$1
    local test_name=$2
    
    echo -n "Running $test_name... "
    
    if timeout 30 $KLANG run "$test_file" > /dev/null 2>&1; then
        echo -e "${GREEN}✓ PASSED${NC}"
        return 0
    else
        echo -e "${RED}✗ FAILED${NC}"
        return 1
    fi
}

# Function to run with valgrind
run_valgrind() {
    local test_file=$1
    local test_name=$2
    
    echo -n "Valgrind check for $test_name... "
    
    if timeout 60 valgrind --leak-check=full --error-exitcode=1 \
        $KLANG run "$test_file" > /dev/null 2>&1; then
        echo -e "${GREEN}✓ NO ERRORS${NC}"
        return 0
    else
        echo -e "${YELLOW}⚠ LEAKS DETECTED${NC}"
        return 1
    fi
}

# Test selection
if [ "$1" == "" ]; then
    # Run all tests
    echo "Running all tests..."
    echo ""
    
    PASSED=0
    FAILED=0
    
    # Functional tests
    echo "--- Functional Tests ---"
    run_test "$TEST_DIR/test_print_klang.kl" "Print Tests" && ((PASSED++)) || ((FAILED++))
    run_test "$TEST_DIR/test_memory_klang.kl" "Memory Tests" && ((PASSED++)) || ((FAILED++))
    run_test "$TEST_DIR/test_memory_intensive.kl" "Intensive Tests" && ((PASSED++)) || ((FAILED++))
    run_test "$TEST_DIR/test_edge_cases.kl" "Edge Cases" && ((PASSED++)) || ((FAILED++))
    
    echo ""
    echo "--- Memory Analysis (Valgrind) ---"
    echo "(Note: Memory leaks are expected - see QA report)"
    run_valgrind "$TEST_DIR/test_print_klang.kl" "Print Tests" || true
    run_valgrind "$TEST_DIR/test_memory_klang.kl" "Memory Tests" || true
    
    echo ""
    echo "═══════════════════════════════════════"
    echo "Summary: $PASSED passed, $FAILED failed"
    echo "═══════════════════════════════════════"
    echo ""
    
    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}✓ All functional tests passed!${NC}"
    else
        echo -e "${RED}✗ Some tests failed!${NC}"
    fi
    
elif [ "$1" == "print" ]; then
    run_test "$TEST_DIR/test_print_klang.kl" "Print Tests"
    
elif [ "$1" == "memory" ]; then
    run_test "$TEST_DIR/test_memory_klang.kl" "Memory Tests"
    
elif [ "$1" == "intensive" ]; then
    run_test "$TEST_DIR/test_memory_intensive.kl" "Intensive Tests"
    
elif [ "$1" == "edge" ]; then
    run_test "$TEST_DIR/test_edge_cases.kl" "Edge Case Tests"
    
elif [ "$1" == "valgrind" ]; then
    echo "Running valgrind on all tests..."
    echo "(This may take a few minutes)"
    echo ""
    run_valgrind "$TEST_DIR/test_print_klang.kl" "Print Tests"
    run_valgrind "$TEST_DIR/test_memory_klang.kl" "Memory Tests"
    run_valgrind "$TEST_DIR/test_memory_intensive.kl" "Intensive Tests"
    
elif [ "$1" == "help" ] || [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
    echo "Usage: $0 [test_name]"
    echo ""
    echo "Available tests:"
    echo "  print      - Run print functionality tests"
    echo "  memory     - Run basic memory tests"
    echo "  intensive  - Run intensive stress tests"
    echo "  edge       - Run edge case tests"
    echo "  valgrind   - Run valgrind on all tests"
    echo "  (no args)  - Run all tests"
    echo ""
    echo "Examples:"
    echo "  $0              # Run all tests"
    echo "  $0 print        # Run only print tests"
    echo "  $0 valgrind     # Run valgrind analysis"
    
else
    echo "Unknown test: $1"
    echo "Run '$0 help' for usage information"
    exit 1
fi

echo ""
echo "For detailed reports, see:"
echo "  - QA_COMPLETE_REPORT.md (full analysis)"
echo "  - QA_TEST_REPORT.md (detailed report)"
echo "  - tests/test_summary.txt (quick summary)"
