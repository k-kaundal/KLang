/**
 * @file memory_bench.c
 * @brief Memory allocation and GC performance benchmarks
 */

#include "../framework/benchmark.h"
#include <stdlib.h>
#include <string.h>

/* Global to prevent optimization */
volatile void *g_ptr = NULL;

/**
 * Benchmark: Simple allocations
 */
void bench_simple_allocation() {
    void *ptr = malloc(1024);
    g_ptr = ptr;
    free(ptr);
}

/**
 * Benchmark: Many small allocations
 */
void bench_many_small_allocs() {
    void *ptrs[100];
    for (int i = 0; i < 100; i++) {
        ptrs[i] = malloc(64);
    }
    for (int i = 0; i < 100; i++) {
        free(ptrs[i]);
    }
}

/**
 * Benchmark: Large allocations
 */
void bench_large_allocation() {
    void *ptr = malloc(1024 * 1024);  // 1MB
    g_ptr = ptr;
    memset(ptr, 0, 1024 * 1024);
    free(ptr);
}

/**
 * Benchmark: Allocation churn (many alloc/free cycles)
 */
void bench_allocation_churn() {
    for (int i = 0; i < 100; i++) {
        void *ptr = malloc(128);
        g_ptr = ptr;
        free(ptr);
    }
}

/**
 * Benchmark: Fragmentation pattern
 */
void bench_fragmentation() {
    void *ptrs[50];
    
    // Allocate interleaved sizes
    for (int i = 0; i < 50; i++) {
        size_t size = (i % 2 == 0) ? 64 : 1024;
        ptrs[i] = malloc(size);
    }
    
    // Free every other allocation
    for (int i = 0; i < 50; i += 2) {
        free(ptrs[i]);
    }
    
    // Free remaining
    for (int i = 1; i < 50; i += 2) {
        free(ptrs[i]);
    }
}

/**
 * Benchmark: Reallocation
 */
void bench_reallocation() {
    void *ptr = malloc(128);
    for (int i = 1; i <= 10; i++) {
        ptr = realloc(ptr, 128 * i);
        g_ptr = ptr;
    }
    free(ptr);
}

int main() {
    printf("%s╔═══════════════════════════════════════════════╗%s\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    printf("%s║      KLANG MEMORY BENCHMARK SUITE            ║%s\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    printf("%s╚═══════════════════════════════════════════════╝%s\n\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    
    BenchmarkResult result;
    
    result = benchmark_run("Simple Allocation (1KB)", 100000, bench_simple_allocation);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/memory_simple_alloc.json");
    
    result = benchmark_run("Many Small Allocations (100x64B)", 10000, bench_many_small_allocs);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/memory_many_small.json");
    
    result = benchmark_run("Large Allocation (1MB)", 10000, bench_large_allocation);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/memory_large_alloc.json");
    
    result = benchmark_run("Allocation Churn (100 cycles)", 10000, bench_allocation_churn);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/memory_churn.json");
    
    result = benchmark_run("Fragmentation Pattern", 10000, bench_fragmentation);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/memory_fragmentation.json");
    
    result = benchmark_run("Reallocation (10x growth)", 10000, bench_reallocation);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/memory_reallocation.json");
    
    printf("%s✓ All memory benchmarks completed!%s\n\n", COLOR_GREEN COLOR_BOLD, COLOR_RESET);
    
    return 0;
}
