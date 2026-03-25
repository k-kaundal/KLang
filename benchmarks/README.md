# KLang Benchmark Suite

Comprehensive performance benchmarking for the KLang programming language.

## 📁 Directory Structure

```
benchmarks/
├── language/           # Language feature benchmarks (arithmetic, loops, functions, etc.)
├── cross-language/     # Comparative benchmarks against Python, JavaScript, C
├── memory/            # Memory allocation and GC benchmarks
├── framework/         # Benchmark utilities and helpers
├── reports/           # Benchmark results and reports
└── README.md          # This file
```

## 🚀 Quick Start

### Build Benchmarks

```bash
make benchmarks
```

### Run All Benchmarks

```bash
./run_benchmarks.sh
```

### Run Specific Benchmark

```bash
./benchmarks/language/arithmetic_bench
./benchmarks/language/loop_bench
```

### Generate Report

```bash
./benchmarks/generate_report.sh > benchmarks/reports/latest.md
```

## 📊 Benchmark Categories

### 1. Language Features (`language/`)

Tests core language performance:
- **Arithmetic**: Integer and float operations
- **Loops**: for, while, for-of iterations
- **Functions**: Call overhead, recursion, closures
- **Objects**: Object creation, property access, methods
- **Arrays**: Creation, indexing, iteration, methods
- **Strings**: Concatenation, substring, regex
- **Async**: Promises, async/await performance

### 2. Cross-Language Comparison (`cross-language/`)

Compares KLang against:
- **Python**: Similar syntax, interpreted
- **JavaScript (Node.js)**: Dynamic typing, JIT compiled
- **C**: Native performance baseline

Benchmarks include:
- Fibonacci (recursion)
- Matrix multiplication
- File I/O
- JSON parsing
- HTTP requests
- String processing

### 3. Memory Benchmarks (`memory/`)

Memory usage and GC performance:
- Allocation patterns
- GC pause times
- Memory leaks detection
- Peak memory usage
- Object lifetime tracking

### 4. Framework (`framework/`)

Reusable benchmark utilities:
- Timing functions
- Statistics (mean, median, stddev)
- Result formatting
- Cross-language runners

## 📈 Benchmark Metrics

Each benchmark reports:
- **Throughput**: Operations per second
- **Latency**: Time per operation (min/max/avg/p50/p95/p99)
- **Memory**: Peak usage, allocations
- **Comparison**: Relative performance vs baseline

## 🎯 Performance Goals

| Operation | Target | Current |
|-----------|--------|---------|
| Integer arithmetic | 100M ops/sec | TBD |
| Function call | 10M calls/sec | TBD |
| Object creation | 1M objects/sec | TBD |
| Array iteration | 50M items/sec | TBD |
| JSON parse | 10MB/sec | TBD |

## 🔄 Continuous Benchmarking

Benchmarks run automatically on:
- Every PR (regression detection)
- Nightly builds (trend tracking)
- Release candidates (performance validation)

Results are tracked in `reports/history/` for trend analysis.

## 🛠️ Adding New Benchmarks

1. Create benchmark in appropriate directory
2. Follow naming convention: `<feature>_bench.c` or `<feature>_bench.k`
3. Use framework utilities for consistent reporting
4. Add to `run_benchmarks.sh`
5. Document expected performance

## 📝 Benchmark Template (C)

```c
#include "../framework/benchmark.h"

void benchmark_my_feature() {
    BenchmarkResult result = benchmark_run("My Feature", 1000000, ^{
        // Code to benchmark
        int sum = 0;
        for (int i = 0; i < 100; i++) {
            sum += i;
        }
        return sum;
    });
    
    benchmark_print_result(&result);
}

int main() {
    benchmark_my_feature();
    return 0;
}
```

## 📝 Benchmark Template (KLang)

```klang
import "benchmark" from "../framework/benchmark.k"

let result = benchmark("My Feature", 1000000, () => {
    let sum = 0
    for i in range(100) {
        sum += i
    }
    return sum
})

println(result)
```

## 🎨 Output Format

```
=== Benchmark: Integer Arithmetic ===
Iterations:    1,000,000
Time:          45.23 ms
Throughput:    22.11 million ops/sec
Latency:
  - Min:       42 ns
  - Max:       1,203 ns
  - Mean:      45 ns
  - Median:    44 ns
  - P95:       52 ns
  - P99:       89 ns
Memory:
  - Peak:      2.4 MB
  - Allocs:    0
```

## 📊 Historical Tracking

Performance trends are tracked in `reports/history/`:
- `YYYY-MM-DD_benchmark.json` - Raw data
- `YYYY-MM-DD_report.md` - Human-readable report
- `trends.csv` - CSV for analysis

## 🔍 Profiling Integration

Benchmarks can generate profiling data:
- CPU flame graphs (`--flamegraph`)
- Memory allocation traces (`--memtrace`)
- Call graphs (`--callgraph`)

## 📚 Further Reading

- [Performance Tuning Guide](../docs/performance.md)
- [VM Architecture](../docs/VM_REGISTER_README.md)
- [LLVM Backend](../docs/llvm_backend.md)
