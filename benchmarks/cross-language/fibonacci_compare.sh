#!/bin/bash
# Fibonacci benchmark comparison script

ITERATIONS=1000

echo "╔═══════════════════════════════════════════════╗"
echo "║   CROSS-LANGUAGE FIBONACCI BENCHMARK          ║"
echo "║   Computing Fibonacci(30) $ITERATIONS times   ║"
echo "╚═══════════════════════════════════════════════╝"
echo ""

# Create temp directory for benchmark code
TEMP_DIR=$(mktemp -d)
trap "rm -rf $TEMP_DIR" EXIT

# C version
cat > $TEMP_DIR/fib.c << 'EOF'
#include <stdio.h>
#include <time.h>

long long fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main() {
    clock_t start = clock();
    for (int i = 0; i < 1000; i++) {
        fib(30);
    }
    clock_t end = clock();
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    printf("C:          %.2f ms (%.2f ops/sec)\n", elapsed, 1000.0 / (elapsed / 1000.0));
    return 0;
}
EOF

# Python version
cat > $TEMP_DIR/fib.py << 'EOF'
import time

def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

start = time.time()
for i in range(1000):
    fib(30)
end = time.time()
elapsed = (end - start) * 1000.0
print(f"Python:     {elapsed:.2f} ms ({1000.0 / (elapsed / 1000.0):.2f} ops/sec)")
EOF

# JavaScript version
cat > $TEMP_DIR/fib.js << 'EOF'
function fib(n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

const start = Date.now();
for (let i = 0; i < 1000; i++) {
    fib(30);
}
const end = Date.now();
const elapsed = end - start;
console.log(`JavaScript: ${elapsed.toFixed(2)} ms (${(1000.0 / (elapsed / 1000.0)).toFixed(2)} ops/sec)`);
EOF

# KLang version (if klang is available)
cat > $TEMP_DIR/fib.k << 'EOF'
fn fib(n) {
    if n <= 1 {
        return n
    }
    return fib(n - 1) + fib(n - 2)
}

let start = time()
for i in range(1000) {
    fib(30)
}
let end = time()
let elapsed = (end - start) * 1000.0
println("KLang:      " + str(elapsed) + " ms (" + str(1000.0 / (elapsed / 1000.0)) + " ops/sec)")
EOF

# Run benchmarks
echo "Running benchmarks..."
echo ""

# C
if command -v gcc &> /dev/null; then
    gcc -O3 -o $TEMP_DIR/fib_c $TEMP_DIR/fib.c
    $TEMP_DIR/fib_c
else
    echo "C:          (gcc not found)"
fi

# Python
if command -v python3 &> /dev/null; then
    python3 $TEMP_DIR/fib.py
else
    echo "Python:     (python3 not found)"
fi

# JavaScript
if command -v node &> /dev/null; then
    node $TEMP_DIR/fib.js
else
    echo "JavaScript: (node not found)"
fi

# KLang
if command -v klang &> /dev/null; then
    klang run $TEMP_DIR/fib.k
else
    echo "KLang:      (klang not found - build and install first)"
fi

echo ""
echo "✓ Benchmark completed!"
