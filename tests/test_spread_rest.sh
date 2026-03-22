#!/bin/bash
# Test script for spread and rest operators

# Get the repo root directory (parent of tests directory)
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$REPO_ROOT"

echo "Testing Spread and Rest Operators..."
echo "======================================"
echo ""

# Test 1: Array spread
echo "Test 1: Array spread"
./klang run test_spread_simple.k
if [ $? -eq 0 ]; then
    echo "✓ PASSED"
else
    echo "✗ FAILED"
    exit 1
fi
echo ""

# Test 2: Rest parameters
echo "Test 2: Rest parameters"
./klang run test_rest_simple.k
if [ $? -eq 0 ]; then
    echo "✓ PASSED"
else
    echo "✗ FAILED"
    exit 1
fi
echo ""

# Test 3: Object spread
echo "Test 3: Object spread"
./klang run test_object_spread.k
if [ $? -eq 0 ]; then
    echo "✓ PASSED"
else
    echo "✗ FAILED"
    exit 1
fi
echo ""

# Test 4: Comprehensive demo
echo "Test 4: Comprehensive demo"
./klang run examples/spread_rest_demo.k
if [ $? -eq 0 ]; then
    echo "✓ PASSED"
else
    echo "✗ FAILED"
    exit 1
fi
echo ""

echo "======================================"
echo "All spread/rest operator tests passed!"
