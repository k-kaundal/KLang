/**
 * @file string_bench.c
 * @brief String operation benchmarks
 */

#include "../framework/benchmark.h"
#include <string.h>

volatile char *g_str = NULL;

/**
 * Benchmark: String concatenation
 */
void bench_string_concat() {
    char buffer[1024];
    strcpy(buffer, "Hello");
    for (int i = 0; i < 100; i++) {
        strcat(buffer, " World");
    }
    g_str = buffer;
}

/**
 * Benchmark: String comparison
 */
void bench_string_compare() {
    const char *str1 = "Hello World this is a test string";
    const char *str2 = "Hello World this is a test string";
    
    for (int i = 0; i < 1000; i++) {
        volatile int result = strcmp(str1, str2);
    }
}

/**
 * Benchmark: String copy
 */
void bench_string_copy() {
    const char *source = "This is a test string for benchmarking";
    char dest[100];
    
    for (int i = 0; i < 1000; i++) {
        strcpy(dest, source);
    }
    g_str = dest;
}

/**
 * Benchmark: String length
 */
void bench_string_length() {
    const char *str = "This is a test string for benchmarking string length operation";
    
    for (int i = 0; i < 1000; i++) {
        volatile size_t len = strlen(str);
    }
}

/**
 * Benchmark: String search
 */
void bench_string_search() {
    const char *haystack = "The quick brown fox jumps over the lazy dog";
    const char *needle = "fox";
    
    for (int i = 0; i < 1000; i++) {
        volatile char *result = strstr(haystack, needle);
    }
}

/**
 * Benchmark: Character search
 */
void bench_char_search() {
    const char *str = "The quick brown fox jumps over the lazy dog";
    
    for (int i = 0; i < 1000; i++) {
        volatile char *result = strchr(str, 'x');
    }
}

int main() {
    printf("%s╔═══════════════════════════════════════════════╗%s\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    printf("%s║      KLANG STRING BENCHMARK SUITE            ║%s\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    printf("%s╚═══════════════════════════════════════════════╝%s\n\n", COLOR_CYAN COLOR_BOLD, COLOR_RESET);
    
    BenchmarkResult result;
    
    result = benchmark_run("String Concatenation (100 ops)", 10000, bench_string_concat);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/string_concat.json");
    
    result = benchmark_run("String Comparison (1000 ops)", 10000, bench_string_compare);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/string_compare.json");
    
    result = benchmark_run("String Copy (1000 ops)", 10000, bench_string_copy);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/string_copy.json");
    
    result = benchmark_run("String Length (1000 ops)", 10000, bench_string_length);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/string_length.json");
    
    result = benchmark_run("String Search (1000 ops)", 10000, bench_string_search);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/string_search.json");
    
    result = benchmark_run("Character Search (1000 ops)", 10000, bench_char_search);
    benchmark_print_result(&result);
    benchmark_save_json(&result, "../reports/string_char_search.json");
    
    printf("%s✓ All string benchmarks completed!%s\n\n", COLOR_GREEN COLOR_BOLD, COLOR_RESET);
    
    return 0;
}
