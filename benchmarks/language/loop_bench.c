/**
 * @file loop_bench.c
 * @brief Benchmark for loop constructs
 */

#include "../framework/benchmark.h"

volatile int64_t g_sum = 0;

/* Simple for loop */
void bench_for_loop_simple() {
    int64_t sum = 0;
    for (int i = 0; i < 10000; i++) {
        sum += i;
    }
    g_sum = sum;
}

/* Nested for loops */
void bench_for_loop_nested() {
    int64_t sum = 0;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            sum += i + j;
        }
    }
    g_sum = sum;
}

/* While loop */
void bench_while_loop() {
    int64_t sum = 0;
    int i = 0;
    while (i < 10000) {
        sum += i;
        i++;
    }
    g_sum = sum;
}

/* Loop with break */
void bench_loop_with_break() {
    int64_t sum = 0;
    for (int i = 0; i < 1000000; i++) {
        sum += i;
        if (sum > 50000000) {
            break;
        }
    }
    g_sum = sum;
}

/* Loop with continue */
void bench_loop_with_continue() {
    int64_t sum = 0;
    for (int i = 0; i < 10000; i++) {
        if (i % 2 == 0) {
            continue;
        }
        sum += i;
    }
    g_sum = sum;
}

int main() {
    printf("%s╔═══════════════════════════════════════════════╗%s\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    printf("%s║       KLANG LOOP BENCHMARK SUITE             ║%s\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    printf("%s╚═══════════════════════════════════════════════╝%s\n\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    
    BenchmarkResult result;
    
    result = benchmark_run("Simple For Loop (10000 iterations)", 10000, bench_for_loop_simple);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/loop_for_simple.json");
    
    result = benchmark_run("Nested For Loops (100x100)", 1000, bench_for_loop_nested);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/loop_for_nested.json");
    
    result = benchmark_run("While Loop (10000 iterations)", 10000, bench_while_loop);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/loop_while.json");
    
    result = benchmark_run("Loop with Break", 10000, bench_loop_with_break);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/loop_break.json");
    
    result = benchmark_run("Loop with Continue", 10000, bench_loop_with_continue);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/loop_continue.json");
    
    printf("%s✓ All loop benchmarks completed!%s\n\n", COLOR_GREEN COLOR_BOLD, COLOR_RESET);
    
    return 0;
}
