/**
 * @file function_bench.c
 * @brief Benchmark for function calls and recursion
 */

#include "../framework/benchmark.h"

volatile int64_t g_result = 0;

/* Simple function call */
static int64_t add_two_numbers(int64_t a, int64_t b) {
    return a + b;
}

void bench_function_call() {
    int64_t sum = 0;
    for (int i = 0; i < 1000; i++) {
        sum = add_two_numbers(sum, i);
    }
    g_result = sum;
}

/* Recursive fibonacci */
static int64_t fib_recursive(int n) {
    if (n <= 1) return n;
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

void bench_recursion_fib() {
    g_result = fib_recursive(20);
}

/* Iterative fibonacci (for comparison) */
static int64_t fib_iterative(int n) {
    if (n <= 1) return n;
    int64_t a = 0, b = 1, temp;
    for (int i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

void bench_iteration_fib() {
    g_result = fib_iterative(20);
}

/* Tail recursion (factorial) */
static int64_t factorial_tail_helper(int n, int64_t acc) {
    if (n <= 1) return acc;
    return factorial_tail_helper(n - 1, n * acc);
}

static int64_t factorial_tail(int n) {
    return factorial_tail_helper(n, 1);
}

void bench_tail_recursion() {
    g_result = factorial_tail(20);
}

/* Deep call stack */
static int64_t deep_call(int depth) {
    if (depth <= 0) return 42;
    return deep_call(depth - 1) + 1;
}

void bench_deep_call_stack() {
    g_result = deep_call(100);
}

int main() {
    printf("%s╔═══════════════════════════════════════════════╗%s\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    printf("%s║     KLANG FUNCTION BENCHMARK SUITE           ║%s\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    printf("%s╚═══════════════════════════════════════════════╝%s\n\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    
    BenchmarkResult result;
    
    result = benchmark_run("Simple Function Call (1000 calls)", 10000, bench_function_call);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/function_call.json");
    
    result = benchmark_run("Recursive Fibonacci(20)", 1000, bench_recursion_fib);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/function_fib_recursive.json");
    
    result = benchmark_run("Iterative Fibonacci(20)", 100000, bench_iteration_fib);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/function_fib_iterative.json");
    
    result = benchmark_run("Tail Recursion Factorial(20)", 10000, bench_tail_recursion);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/function_tail_recursion.json");
    
    result = benchmark_run("Deep Call Stack (100 frames)", 10000, bench_deep_call_stack);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/function_deep_stack.json");
    
    printf("%s✓ All function benchmarks completed!%s\n\n", COLOR_GREEN COLOR_BOLD, COLOR_RESET);
    
    return 0;
}
