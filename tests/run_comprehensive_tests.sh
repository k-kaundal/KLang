#!/bin/bash
# Comprehensive Test Runner for KLang Print and Memory Tests

KLANG="./klang"
TEST_DIR="tests"
RESULTS_FILE="/tmp/klang_test_results.txt"

echo "╔═══════════════════════════════════════════════════════════╗" > "$RESULTS_FILE"
echo "║         KLang Print and Memory Test Report              ║" >> "$RESULTS_FILE"
echo "╚═══════════════════════════════════════════════════════════╝" >> "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"
echo "Test Date: $(date)" >> "$RESULTS_FILE"
echo "KLang Binary: $KLANG" >> "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"

# Function to run a test
run_test() {
    local test_file=$1
    local test_name=$2
    
    echo "================================================" >> "$RESULTS_FILE"
    echo "Test: $test_name" >> "$RESULTS_FILE"
    echo "File: $test_file" >> "$RESULTS_FILE"
    echo "================================================" >> "$RESULTS_FILE"
    
    # Run without valgrind first
    echo "" >> "$RESULTS_FILE"
    echo "--- Output ---" >> "$RESULTS_FILE"
    timeout 30 $KLANG run "$test_file" >> "$RESULTS_FILE" 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo "" >> "$RESULTS_FILE"
        echo "✓ Test PASSED (exit code: 0)" >> "$RESULTS_FILE"
    else
        echo "" >> "$RESULTS_FILE"
        echo "✗ Test FAILED (exit code: $exit_code)" >> "$RESULTS_FILE"
    fi
    
    echo "" >> "$RESULTS_FILE"
}

# Function to run valgrind test
run_valgrind_test() {
    local test_file=$1
    local test_name=$2
    
    echo "================================================" >> "$RESULTS_FILE"
    echo "Valgrind Memory Check: $test_name" >> "$RESULTS_FILE"
    echo "File: $test_file" >> "$RESULTS_FILE"
    echo "================================================" >> "$RESULTS_FILE"
    
    timeout 60 valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 \
        $KLANG run "$test_file" > /tmp/valgrind_out.txt 2>&1
    local exit_code=$?
    
    # Extract summary
    echo "" >> "$RESULTS_FILE"
    echo "--- Memory Leak Summary ---" >> "$RESULTS_FILE"
    grep "LEAK SUMMARY" -A 5 /tmp/valgrind_out.txt >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    grep "ERROR SUMMARY" /tmp/valgrind_out.txt >> "$RESULTS_FILE"
    
    if [ $exit_code -eq 0 ]; then
        echo "✓ No memory errors detected" >> "$RESULTS_FILE"
    else
        echo "⚠ Memory errors detected" >> "$RESULTS_FILE"
    fi
    
    echo "" >> "$RESULTS_FILE"
}

echo "Running KLang Tests..."
echo ""

# Run Print Tests
echo "1. Running Print Tests..."
run_test "$TEST_DIR/test_print_klang.kl" "Print Statement Tests"

# Run Memory Tests  
echo "2. Running Memory Stress Tests..."
run_test "$TEST_DIR/test_memory_klang.kl" "Memory Stress Tests"

# Run Valgrind Tests
echo "3. Running Valgrind Memory Analysis on Print Tests..."
run_valgrind_test "$TEST_DIR/test_print_klang.kl" "Print Tests"

echo "4. Running Valgrind Memory Analysis on Memory Tests..."
run_valgrind_test "$TEST_DIR/test_memory_klang.kl" "Memory Tests"

# Summary
echo "================================================" >> "$RESULTS_FILE"
echo "SUMMARY" >> "$RESULTS_FILE"
echo "================================================" >> "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"

# Count results
PASSED=$(grep -c "✓" "$RESULTS_FILE")
FAILED=$(grep -c "✗" "$RESULTS_FILE")
WARNINGS=$(grep -c "⚠" "$RESULTS_FILE")

echo "Tests Passed: $PASSED" >> "$RESULTS_FILE"
echo "Tests Failed: $FAILED" >> "$RESULTS_FILE"
echo "Warnings: $WARNINGS" >> "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"

if [ $FAILED -eq 0 ]; then
    echo "✓ All functional tests passed!" >> "$RESULTS_FILE"
else
    echo "✗ Some tests failed" >> "$RESULTS_FILE"
fi

if [ $WARNINGS -gt 0 ]; then
    echo "⚠ Memory leak warnings detected - review recommended" >> "$RESULTS_FILE"
fi

echo "" >> "$RESULTS_FILE"
echo "Full report saved to: $RESULTS_FILE" >> "$RESULTS_FILE"

echo ""
echo "Test suite complete!"
echo ""
cat "$RESULTS_FILE"
