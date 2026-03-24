#!/bin/bash

echo "Testing all example files..."
PASS=0
FAIL=0
SKIP=0

# Files to skip (require user interaction or are incomplete)
SKIP_FILES=("async_await_demo.k" "promises_demo.k" "test_llvm.kl" "simple_llvm.kl")

# Get all example files
for ext in k kl klang; do
    for file in examples/*.$ext; do
        [ -f "$file" ] || continue
        
        basename=$(basename "$file")
        
        # Check if file should be skipped
        skip=0
        for skip_file in "${SKIP_FILES[@]}"; do
            if [ "$basename" = "$skip_file" ]; then
                skip=1
                break
            fi
        done
        
        if [ $skip -eq 1 ]; then
            echo "  ⊘ SKIP: $basename (requires special handling)"
            ((SKIP++))
            continue
        fi
        
        # Run the test
        if timeout 5 ./klang run "$file" > /tmp/example_output.txt 2>&1; then
            echo "  ✅ PASS: $basename"
            ((PASS++))
        else
            exit_code=$?
            if [ $exit_code -eq 124 ]; then
                echo "  ⏱️  TIMEOUT: $basename"
            else
                echo "  ❌ FAIL: $basename"
                echo "     Exit code: $exit_code"
                head -5 /tmp/example_output.txt | sed 's/^/     /'
            fi
            ((FAIL++))
        fi
    done
done

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "Example Files Summary:"
echo "  Total:   $((PASS + FAIL + SKIP))"
echo "  Passed:  $PASS"
echo "  Failed:  $FAIL"
echo "  Skipped: $SKIP"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
