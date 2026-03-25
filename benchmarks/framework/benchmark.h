/**
 * @file benchmark.h
 * @brief Benchmark framework utilities for KLang
 * 
 * Provides timing, statistics, and reporting utilities for performance benchmarking.
 */

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdint.h>
#include <math.h>

/* ANSI color codes for pretty output */
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

/* Benchmark configuration */
#define WARMUP_ITERATIONS 1000
#define MIN_BENCHMARK_TIME_MS 100  /* Run at least 100ms */

/**
 * @brief Timing information structure
 */
typedef struct {
    double min_ns;       /* Minimum time in nanoseconds */
    double max_ns;       /* Maximum time in nanoseconds */
    double mean_ns;      /* Mean time in nanoseconds */
    double median_ns;    /* Median time in nanoseconds */
    double stddev_ns;    /* Standard deviation in nanoseconds */
    double p95_ns;       /* 95th percentile */
    double p99_ns;       /* 99th percentile */
} TimingStats;

/**
 * @brief Memory usage statistics
 */
typedef struct {
    size_t peak_bytes;        /* Peak memory usage */
    size_t allocations;       /* Number of allocations */
    size_t deallocations;     /* Number of deallocations */
    size_t bytes_allocated;   /* Total bytes allocated */
} MemoryStats;

/**
 * @brief Complete benchmark result
 */
typedef struct {
    char name[256];           /* Benchmark name */
    uint64_t iterations;      /* Number of iterations */
    double total_time_ms;     /* Total time in milliseconds */
    double throughput_mops;   /* Throughput in millions of ops/sec */
    TimingStats timing;       /* Timing statistics */
    MemoryStats memory;       /* Memory statistics */
} BenchmarkResult;

/**
 * @brief Get current time in nanoseconds
 */
static inline uint64_t get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

/**
 * @brief Get current time in milliseconds
 */
static inline double get_time_ms() {
    return get_time_ns() / 1000000.0;
}

/**
 * @brief Get current memory usage in bytes
 */
static inline size_t get_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    #ifdef __APPLE__
        return (size_t)usage.ru_maxrss;  /* macOS: bytes */
    #else
        return (size_t)usage.ru_maxrss * 1024;  /* Linux: kilobytes */
    #endif
}

/**
 * @brief Compare function for qsort (for percentile calculation)
 */
static int compare_double(const void *a, const void *b) {
    double da = *(const double *)a;
    double db = *(const double *)b;
    return (da > db) - (da < db);
}

/**
 * @brief Calculate statistics from timing samples
 */
static TimingStats calculate_timing_stats(double *samples, size_t count) {
    TimingStats stats = {0};
    
    if (count == 0) return stats;
    
    /* Sort samples for percentiles */
    qsort(samples, count, sizeof(double), compare_double);
    
    /* Min and max */
    stats.min_ns = samples[0];
    stats.max_ns = samples[count - 1];
    
    /* Mean */
    double sum = 0.0;
    for (size_t i = 0; i < count; i++) {
        sum += samples[i];
    }
    stats.mean_ns = sum / count;
    
    /* Median */
    if (count % 2 == 0) {
        stats.median_ns = (samples[count/2 - 1] + samples[count/2]) / 2.0;
    } else {
        stats.median_ns = samples[count/2];
    }
    
    /* Standard deviation */
    double variance = 0.0;
    for (size_t i = 0; i < count; i++) {
        double diff = samples[i] - stats.mean_ns;
        variance += diff * diff;
    }
    stats.stddev_ns = sqrt(variance / count);
    
    /* Percentiles */
    stats.p95_ns = samples[(size_t)(count * 0.95)];
    stats.p99_ns = samples[(size_t)(count * 0.99)];
    
    return stats;
}

/**
 * @brief Format number with thousands separators
 */
static void format_number(char *buf, size_t bufsize, uint64_t num) {
    char temp[64];
    snprintf(temp, sizeof(temp), "%llu", (unsigned long long)num);
    
    int len = strlen(temp);
    int comma_count = (len - 1) / 3;
    int new_len = len + comma_count;
    
    if ((size_t)new_len >= bufsize) {
        strncpy(buf, temp, bufsize - 1);
        buf[bufsize - 1] = '\0';
        return;
    }
    
    buf[new_len] = '\0';
    int src = len - 1;
    int dst = new_len - 1;
    int count = 0;
    
    while (src >= 0) {
        if (count == 3) {
            buf[dst--] = ',';
            count = 0;
        }
        buf[dst--] = temp[src--];
        count++;
    }
}

/**
 * @brief Format bytes as human-readable string
 */
static void format_bytes(char *buf, size_t bufsize, size_t bytes) {
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit_idx = 0;
    double size = (double)bytes;
    
    while (size >= 1024.0 && unit_idx < 4) {
        size /= 1024.0;
        unit_idx++;
    }
    
    snprintf(buf, bufsize, "%.2f %s", size, units[unit_idx]);
}

/**
 * @brief Print benchmark result in formatted output
 */
static void benchmark_print_result(const BenchmarkResult *result) {
    char iter_str[64], time_str[64], peak_mem_str[64], allocs_str[64];
    
    format_number(iter_str, sizeof(iter_str), result->iterations);
    format_bytes(peak_mem_str, sizeof(peak_mem_str), result->memory.peak_bytes);
    format_number(allocs_str, sizeof(allocs_str), result->memory.allocations);
    
    printf("\n%s=== Benchmark: %s ===%s\n", COLOR_BOLD COLOR_CYAN, result->name, COLOR_RESET);
    printf("%sIterations:%s    %s\n", COLOR_BLUE, COLOR_RESET, iter_str);
    printf("%sTime:%s          %.2f ms\n", COLOR_BLUE, COLOR_RESET, result->total_time_ms);
    printf("%sThroughput:%s    %.2f million ops/sec\n", COLOR_GREEN, COLOR_RESET, result->throughput_mops);
    
    printf("%sLatency:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  - Min:       %.0f ns\n", result->timing.min_ns);
    printf("  - Max:       %.0f ns\n", result->timing.max_ns);
    printf("  - Mean:      %.0f ns\n", result->timing.mean_ns);
    printf("  - Median:    %.0f ns\n", result->timing.median_ns);
    printf("  - P95:       %.0f ns\n", result->timing.p95_ns);
    printf("  - P99:       %.0f ns\n", result->timing.p99_ns);
    printf("  - StdDev:    %.0f ns\n", result->timing.stddev_ns);
    
    printf("%sMemory:%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("  - Peak:      %s\n", peak_mem_str);
    printf("  - Allocs:    %s\n", allocs_str);
    
    printf("\n");
}

/**
 * @brief Run a benchmark with the given function
 * 
 * @param name Benchmark name
 * @param min_iterations Minimum number of iterations to run
 * @param bench_fn Function to benchmark (returns void, no args)
 * @return BenchmarkResult structure with timing and statistics
 */
typedef void (*BenchmarkFunction)(void);

static BenchmarkResult benchmark_run(const char *name, uint64_t min_iterations, BenchmarkFunction bench_fn) {
    BenchmarkResult result = {0};
    strncpy(result.name, name, sizeof(result.name) - 1);
    
    /* Allocate sample buffer (max 10000 samples) */
    size_t max_samples = min_iterations < 10000 ? min_iterations : 10000;
    double *samples = malloc(max_samples * sizeof(double));
    if (!samples) {
        fprintf(stderr, "Failed to allocate sample buffer\n");
        return result;
    }
    
    /* Warmup */
    printf("%sWarming up...%s\n", COLOR_YELLOW, COLOR_RESET);
    for (int i = 0; i < WARMUP_ITERATIONS; i++) {
        bench_fn();
    }
    
    /* Measure memory before */
    size_t mem_before = get_memory_usage();
    
    /* Benchmark loop */
    printf("%sRunning benchmark...%s\n", COLOR_YELLOW, COLOR_RESET);
    uint64_t iterations = 0;
    double start_time = get_time_ms();
    double elapsed = 0.0;
    
    while (iterations < min_iterations || elapsed < MIN_BENCHMARK_TIME_MS) {
        uint64_t iter_start = get_time_ns();
        bench_fn();
        uint64_t iter_end = get_time_ns();
        
        /* Store sample (with decimation if needed) */
        if (iterations < max_samples) {
            samples[iterations] = (double)(iter_end - iter_start);
        } else {
            /* Reservoir sampling to maintain representative sample */
            size_t j = iterations % max_samples;
            samples[j] = (double)(iter_end - iter_start);
        }
        
        iterations++;
        elapsed = get_time_ms() - start_time;
    }
    
    /* Measure memory after */
    size_t mem_after = get_memory_usage();
    
    /* Calculate statistics */
    result.iterations = iterations;
    result.total_time_ms = elapsed;
    result.throughput_mops = (iterations / elapsed) / 1000.0;
    
    size_t sample_count = iterations < max_samples ? iterations : max_samples;
    result.timing = calculate_timing_stats(samples, sample_count);
    
    result.memory.peak_bytes = mem_after > mem_before ? mem_after - mem_before : 0;
    result.memory.allocations = 0;  /* Would need GC integration */
    result.memory.deallocations = 0;
    result.memory.bytes_allocated = result.memory.peak_bytes;
    
    free(samples);
    return result;
}

/**
 * @brief Save benchmark result to JSON file
 */
static void benchmark_save_json(const BenchmarkResult *result, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Failed to open %s for writing\n", filename);
        return;
    }
    
    fprintf(fp, "{\n");
    fprintf(fp, "  \"name\": \"%s\",\n", result->name);
    fprintf(fp, "  \"iterations\": %llu,\n", (unsigned long long)result->iterations);
    fprintf(fp, "  \"total_time_ms\": %.2f,\n", result->total_time_ms);
    fprintf(fp, "  \"throughput_mops\": %.2f,\n", result->throughput_mops);
    fprintf(fp, "  \"timing\": {\n");
    fprintf(fp, "    \"min_ns\": %.0f,\n", result->timing.min_ns);
    fprintf(fp, "    \"max_ns\": %.0f,\n", result->timing.max_ns);
    fprintf(fp, "    \"mean_ns\": %.0f,\n", result->timing.mean_ns);
    fprintf(fp, "    \"median_ns\": %.0f,\n", result->timing.median_ns);
    fprintf(fp, "    \"p95_ns\": %.0f,\n", result->timing.p95_ns);
    fprintf(fp, "    \"p99_ns\": %.0f,\n", result->timing.p99_ns);
    fprintf(fp, "    \"stddev_ns\": %.0f\n", result->timing.stddev_ns);
    fprintf(fp, "  },\n");
    fprintf(fp, "  \"memory\": {\n");
    fprintf(fp, "    \"peak_bytes\": %zu,\n", result->memory.peak_bytes);
    fprintf(fp, "    \"allocations\": %zu,\n", result->memory.allocations);
    fprintf(fp, "    \"deallocations\": %zu,\n", result->memory.deallocations);
    fprintf(fp, "    \"bytes_allocated\": %zu\n", result->memory.bytes_allocated);
    fprintf(fp, "  }\n");
    fprintf(fp, "}\n");
    
    fclose(fp);
}

#endif /* BENCHMARK_H */
