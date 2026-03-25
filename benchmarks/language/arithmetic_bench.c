/**
 * @file arithmetic_bench.c
 * @brief Benchmark for arithmetic operations
 */

#include "../framework/benchmark.h"

/* Global variables to prevent compiler optimization */
volatile int64_t g_result_int = 0;
volatile double g_result_float = 0.0;

/* Integer arithmetic */
void bench_int_add() {
    int64_t sum = 0;
    for (int i = 0; i < 1000; i++) {
        sum += i;
    }
    g_result_int = sum;
}

void bench_int_mul() {
    int64_t product = 1;
    for (int i = 1; i < 100; i++) {
        product *= (i % 7 + 1);
    }
    g_result_int = product;
}

void bench_int_div() {
    int64_t result = 1000000;
    for (int i = 1; i < 100; i++) {
        result /= (i % 5 + 1);
        result += 1000;
    }
    g_result_int = result;
}

/* Floating-point arithmetic */
void bench_float_add() {
    double sum = 0.0;
    for (int i = 0; i < 1000; i++) {
        sum += (double)i * 0.5;
    }
    g_result_float = sum;
}

void bench_float_mul() {
    double product = 1.0;
    for (int i = 1; i < 100; i++) {
        product *= 1.01;
    }
    g_result_float = product;
}

void bench_float_div() {
    double result = 1000000.0;
    for (int i = 1; i < 100; i++) {
        result /= 1.01;
    }
    g_result_float = result;
}

/* Mixed operations */
void bench_mixed_ops() {
    int64_t a = 10, b = 20, c = 30;
    double x = 1.5, y = 2.5, z = 3.5;
    
    for (int i = 0; i < 1000; i++) {
        a = (a + b) * c - b;
        x = (x + y) * z - y;
        b = a / 10;
        y = x / 2.0;
    }
    
    g_result_int = a;
    g_result_float = x;
}

int main() {
    printf("%s╔═══════════════════════════════════════════════╗%s\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    printf("%s║    KLANG ARITHMETIC BENCHMARK SUITE          ║%s\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    printf("%s╚═══════════════════════════════════════════════╝%s\n\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    
    BenchmarkResult result;
    
    result = benchmark_run("Integer Addition (1000 ops)", 100000, bench_int_add);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "reports/arithmetic_int_add.json");
    
    result = benchmark_run("Integer Multiplication (100 ops)", 100000, bench_int_mul);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "reports/arithmetic_int_mul.json");
    
    result = benchmark_run("Integer Division (100 ops)", 100000, bench_int_div);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "reports/arithmetic_int_div.json");
    
    result = benchmark_run("Float Addition (1000 ops)", 100000, bench_float_add);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "reports/arithmetic_float_add.json");
    
    result = benchmark_run("Float Multiplication (100 ops)", 100000, bench_float_mul);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "reports/arithmetic_float_mul.json");
    
    result = benchmark_run("Float Division (100 ops)", 100000, bench_float_div);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "reports/arithmetic_float_div.json");
    
    result = benchmark_run("Mixed Operations (1000 ops)", 100000, bench_mixed_ops);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "reports/arithmetic_mixed.json");
    
    printf("%s✓ All arithmetic benchmarks completed!%s\n\n", COLOR_GREEN COLOR_BOLD, COLOR_RESET);
    
    return 0;
}
