#!/bin/bash
# Test ES6 module function/class exports

# Get the repo root directory (parent of tests directory)
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$REPO_ROOT/tests/examples/modules"

echo "=== Testing ES6 Module System ==="
echo ""

echo "Test 1: Simple constant export"
"$REPO_ROOT/klang" run test_simple.k
if [ $? -eq 0 ]; then
    echo "✓ Test 1 passed"
else
    echo "✗ Test 1 failed"
    exit 1
fi
echo ""

echo "Test 2: Function export"
"$REPO_ROOT/klang" run test_func.k
if [ $? -eq 0 ]; then
    echo "✓ Test 2 passed"
else
    echo "✗ Test 2 failed"
    exit 1
fi
echo ""

echo "Test 3: Default export"
"$REPO_ROOT/klang" run test_import_default.k
if [ $? -eq 0 ]; then
    echo "✓ Test 3 passed"
else
    echo "✗ Test 3 failed"
    exit 1
fi
echo ""

echo "Test 4: Multiple exports (math module)"
"$REPO_ROOT/klang" run test_math.k
if [ $? -eq 0 ]; then
    echo "✓ Test 4 passed"
else
    echo "✗ Test 4 failed"
    exit 1
fi
echo ""

echo "Test 5: Class export"
"$REPO_ROOT/klang" run test_class.k
if [ $? -eq 0 ]; then
    echo "✓ Test 5 passed"
else
    echo "✗ Test 5 failed"
    exit 1
fi
echo ""

echo "=== All Module Tests Passed! ==="
