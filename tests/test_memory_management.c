/*
 * Comprehensive unit tests for KLang Enhanced Memory Management System
 * 
 * Tests:
 * - Memory pools (allocation, deallocation, stats)
 * - Enhanced GC (generational, incremental, reference counting)
 * - Memory manager (unified interface)
 * - Integration scenarios
 */

#include "../src/runtime/memory/memory_pool.h"
#include "../src/runtime/memory/gc_enhanced.h"
#include "../src/runtime/memory/memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "FAIL: %s\n", msg); \
        return 0; \
    } \
} while(0)

#define RUN_TEST(test) do { \
    printf("Running %s... ", #test); \
    fflush(stdout); \
    if (test()) { \
        printf("✅ PASS\n"); \
        passed++; \
    } else { \
        printf("❌ FAIL\n"); \
        failed++; \
    } \
    total++; \
} while(0)

/* ===== Memory Pool Tests ===== */

static int test_pool_create_destroy() {
    MemoryPool *pool = pool_new(64);
    TEST_ASSERT(pool != NULL, "Failed to create pool");
    TEST_ASSERT(pool->object_size == 64, "Incorrect object size");
    pool_free(pool);
    return 1;
}

static int test_pool_alloc_dealloc() {
    MemoryPool *pool = pool_new(64);
    
    void *ptr1 = pool_alloc(pool);
    TEST_ASSERT(ptr1 != NULL, "Pool allocation failed");
    
    void *ptr2 = pool_alloc(pool);
    TEST_ASSERT(ptr2 != NULL, "Second pool allocation failed");
    TEST_ASSERT(ptr1 != ptr2, "Allocations should be different");
    
    pool_dealloc(pool, ptr1);
    pool_dealloc(pool, ptr2);
    
    pool_free(pool);
    return 1;
}

static int test_pool_many_allocations() {
    MemoryPool *pool = pool_new(32);
    void *ptrs[1000];
    
    /* Allocate many objects */
    for (int i = 0; i < 1000; i++) {
        ptrs[i] = pool_alloc(pool);
        TEST_ASSERT(ptrs[i] != NULL, "Allocation failed");
    }
    
    /* Free them all */
    for (int i = 0; i < 1000; i++) {
        pool_dealloc(pool, ptrs[i]);
    }
    
    pool_free(pool);
    return 1;
}

static int test_pool_stats() {
    MemoryPool *pool = pool_new(64);
    
    size_t total, free_bytes;
    pool_get_stats(pool, &total, &free_bytes);
    
    /* Initially should have no blocks */
    TEST_ASSERT(total == 0, "Initial total should be 0");
    
    /* Allocate something */
    void *ptr = pool_alloc(pool);
    TEST_ASSERT(ptr != NULL, "Allocation failed");
    
    pool_get_stats(pool, &total, &free_bytes);
    TEST_ASSERT(total > 0, "Total should be > 0 after allocation");
    
    pool_dealloc(pool, ptr);
    pool_free(pool);
    return 1;
}

/* ===== Enhanced GC Tests ===== */

static int test_gc_create_destroy() {
    GCHeap *heap = gc_heap_new();
    TEST_ASSERT(heap != NULL, "Failed to create GC heap");
    gc_heap_free(heap);
    return 1;
}

static int test_gc_alloc_basic() {
    GCHeap *heap = gc_heap_new();
    
    void *ptr = gc_alloc(heap, 100);
    TEST_ASSERT(ptr != NULL, "GC allocation failed");
    
    GCStats stats = gc_get_stats(heap);
    TEST_ASSERT(stats.total_objects == 1, "Should have 1 object");
    TEST_ASSERT(stats.young_objects == 1, "Should be in young generation");
    
    gc_heap_free(heap);
    return 1;
}

static int test_gc_reference_counting() {
    GCHeap *heap = gc_heap_new();
    
    void *ptr = gc_alloc(heap, 100);
    TEST_ASSERT(gc_get_ref_count(ptr) == 1, "Initial ref count should be 1");
    
    gc_retain(ptr);
    TEST_ASSERT(gc_get_ref_count(ptr) == 2, "Ref count should be 2");
    
    gc_release(heap, ptr);
    TEST_ASSERT(gc_get_ref_count(ptr) == 1, "Ref count should be 1");
    
    /* Release final reference - object should be freed */
    gc_release(heap, ptr);
    
    GCStats stats = gc_get_stats(heap);
    TEST_ASSERT(stats.total_objects == 0, "Object should be freed");
    
    gc_heap_free(heap);
    return 1;
}

static int test_gc_young_collection() {
    GCHeap *heap = gc_heap_new();
    
    /* Allocate some objects */
    void *ptr1 = gc_alloc(heap, 100);
    void *ptr2 = gc_alloc(heap, 100);
    void *ptr3 = gc_alloc(heap, 100);
    
    /* Add one as root (keep alive) */
    gc_add_root(heap, ptr1);
    
    /* Release references to ptr2 and ptr3 */
    gc_release(heap, ptr2);
    gc_release(heap, ptr3);
    
    GCStats before = gc_get_stats(heap);
    
    /* Collect young generation */
    gc_collect_young(heap);
    
    GCStats after = gc_get_stats(heap);
    
    /* ptr1 should survive (it's a root) */
    /* ptr2 and ptr3 should be collected (ref_count == 0) */
    TEST_ASSERT(after.objects_collected >= 2, "Should collect unreferenced objects");
    
    gc_remove_root(heap, ptr1);
    gc_heap_free(heap);
    return 1;
}

static int test_gc_generational() {
    GCHeap *heap = gc_heap_new();
    
    void *ptr = gc_alloc(heap, 100);
    gc_add_root(heap, ptr);
    
    GCObject *obj = gc_object_from_ptr(ptr);
    TEST_ASSERT(obj->generation == GC_GEN_YOUNG, "Should start in young gen");
    TEST_ASSERT(obj->age == 0, "Age should be 0");
    
    /* Collect multiple times to promote to old generation */
    for (int i = 0; i < 5; i++) {
        gc_collect_young(heap);
    }
    
    obj = gc_object_from_ptr(ptr);
    TEST_ASSERT(obj->generation == GC_GEN_OLD, "Should be promoted to old gen");
    
    gc_remove_root(heap, ptr);
    gc_heap_free(heap);
    return 1;
}

static int test_gc_incremental() {
    GCHeap *heap = gc_heap_new();
    
    /* Allocate many objects */
    for (int i = 0; i < 100; i++) {
        void *ptr = gc_alloc(heap, 50);
        if (i % 2 == 0) {
            /* Keep some as roots */
            gc_add_root(heap, ptr);
        } else {
            /* Release others */
            gc_release(heap, ptr);
        }
    }
    
    /* Do incremental collection steps */
    for (int i = 0; i < 10; i++) {
        gc_collect_incremental_step(heap);
    }
    
    GCStats stats = gc_get_stats(heap);
    TEST_ASSERT(stats.incremental_steps > 0, "Should have incremental steps");
    
    gc_clear_roots(heap);
    gc_heap_free(heap);
    return 1;
}

static int test_gc_memory_pressure() {
    GCHeap *heap = gc_heap_new();
    
    double pressure1 = gc_get_memory_pressure(heap);
    
    /* Allocate objects */
    for (int i = 0; i < 100; i++) {
        gc_alloc(heap, 1000);
    }
    
    double pressure2 = gc_get_memory_pressure(heap);
    TEST_ASSERT(pressure2 > pressure1, "Pressure should increase");
    
    gc_heap_free(heap);
    return 1;
}

static int test_gc_pin_object() {
    GCHeap *heap = gc_heap_new();
    
    void *ptr = gc_alloc(heap, 100);
    
    /* Add as root to keep it alive */
    gc_add_root(heap, ptr);
    
    /* Pin the object */
    gc_pin_object(ptr);
    
    GCObject *obj = gc_object_from_ptr(ptr);
    TEST_ASSERT(obj->pinned == 1, "Object should be pinned");
    
    /* Unpin */
    gc_unpin_object(ptr);
    TEST_ASSERT(obj->pinned == 0, "Object should be unpinned");
    
    gc_remove_root(heap, ptr);
    gc_heap_free(heap);
    return 1;
}

/* ===== Memory Manager Tests ===== */

static int test_mm_create_destroy() {
    MemoryManager *mm = memory_manager_new();
    TEST_ASSERT(mm != NULL, "Failed to create memory manager");
    memory_manager_free(mm);
    return 1;
}

static int test_mm_alloc_strategies() {
    MemoryManager *mm = memory_manager_new();
    
    /* Test manual allocation */
    void *ptr1 = mm_alloc(mm, 10000000);  /* Large - should use malloc */
    TEST_ASSERT(ptr1 != NULL, "Large allocation failed");
    mm_free(mm, ptr1);
    
    /* Test pooled allocation */
    void *ptr2 = mm_alloc_pooled(mm, 32);
    TEST_ASSERT(ptr2 != NULL, "Pooled allocation failed");
    
    /* Test GC allocation */
    void *ptr3 = mm_alloc_gc(mm, 100);
    TEST_ASSERT(ptr3 != NULL, "GC allocation failed");
    
    memory_manager_free(mm);
    return 1;
}

static int test_mm_statistics() {
    MemoryManager *mm = memory_manager_new();
    
    /* Do some allocations */
    mm_alloc_gc(mm, 100);
    mm_alloc_pooled(mm, 32);
    mm_alloc(mm, 1000);
    
    MemoryStats stats = mm_get_stats(mm);
    
    TEST_ASSERT(stats.gc_allocs > 0, "Should have GC allocations");
    TEST_ASSERT(stats.pooled_allocs > 0, "Should have pooled allocations");
    
    memory_manager_free(mm);
    return 1;
}

static int test_mm_gc_operations() {
    MemoryManager *mm = memory_manager_new();
    
    void *ptr = mm_alloc_gc(mm, 100);
    mm_gc_add_root(mm, ptr);
    
    /* Trigger collection */
    mm_gc_collect(mm);
    
    mm_gc_remove_root(mm, ptr);
    mm_gc_collect_full(mm);
    
    memory_manager_free(mm);
    return 1;
}

static int test_mm_config() {
    MemoryManager *mm = memory_manager_new();
    
    /* Test enabling/disabling features */
    mm_enable_pools(mm, false);
    mm_enable_gc(mm, true);
    mm_enable_refcount(mm, true);
    
    /* Allocations should still work (will use GC) */
    void *ptr = mm_alloc(mm, 100);
    TEST_ASSERT(ptr != NULL, "Allocation should work");
    /* Don't free GC-allocated pointers manually */
    
    memory_manager_free(mm);
    return 1;
}

static int test_mm_refcount() {
    MemoryManager *mm = memory_manager_new();
    
    void *ptr = mm_alloc_gc(mm, 100);
    
    mm_retain(mm, ptr);
    mm_retain(mm, ptr);
    
    TEST_ASSERT(gc_get_ref_count(ptr) == 3, "Ref count should be 3");
    
    mm_release(mm, ptr);
    TEST_ASSERT(gc_get_ref_count(ptr) == 2, "Ref count should be 2");
    
    memory_manager_free(mm);
    return 1;
}

/* ===== Integration Tests ===== */

static int test_integration_pools_and_gc() {
    MemoryManager *mm = memory_manager_new();
    
    /* Mix pooled and GC allocations */
    void *gc_ptrs[50];
    void *pool_ptrs[50];
    
    for (int i = 0; i < 50; i++) {
        gc_ptrs[i] = mm_alloc_gc(mm, 100);
        pool_ptrs[i] = mm_alloc_pooled(mm, 64);
    }
    
    /* Add some GC roots */
    for (int i = 0; i < 25; i++) {
        mm_gc_add_root(mm, gc_ptrs[i]);
    }
    
    /* Trigger GC */
    mm_gc_collect(mm);
    
    MemoryStats stats = mm_get_stats(mm);
    TEST_ASSERT(stats.gc_allocs == 50, "Should have 50 GC allocations");
    TEST_ASSERT(stats.pooled_allocs == 50, "Should have 50 pooled allocations");
    
    memory_manager_free(mm);
    return 1;
}

static int test_integration_stress() {
    MemoryManager *mm = memory_manager_new();
    
    void *roots[20];  /* Track roots to clean up */
    int root_count = 0;
    
    /* Stress test with many allocations */
    for (int round = 0; round < 10; round++) {
        for (int i = 0; i < 100; i++) {
            void *ptr = mm_alloc(mm, (i % 3 + 1) * 64);
            if (i % 5 == 0 && root_count < 20) {
                roots[root_count++] = ptr;
                mm_gc_add_root(mm, ptr);
            }
        }
        
        mm_gc_collect(mm);
    }
    
    /* Clean up roots */
    for (int i = 0; i < root_count; i++) {
        mm_gc_remove_root(mm, roots[i]);
    }
    
    MemoryStats stats = mm_get_stats(mm);
    TEST_ASSERT(stats.gc_collections_young > 0, "Should have collections");
    
    memory_manager_free(mm);
    return 1;
}

/* Main test runner */
int main() {
    int total = 0, passed = 0, failed = 0;
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║   KLang Enhanced Memory Management System - Unit Tests       ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("Memory Pool Tests:\n");
    RUN_TEST(test_pool_create_destroy);
    RUN_TEST(test_pool_alloc_dealloc);
    RUN_TEST(test_pool_many_allocations);
    RUN_TEST(test_pool_stats);
    printf("\n");
    
    printf("Enhanced GC Tests:\n");
    RUN_TEST(test_gc_create_destroy);
    RUN_TEST(test_gc_alloc_basic);
    RUN_TEST(test_gc_reference_counting);
    RUN_TEST(test_gc_young_collection);
    RUN_TEST(test_gc_generational);
    RUN_TEST(test_gc_incremental);
    RUN_TEST(test_gc_memory_pressure);
    RUN_TEST(test_gc_pin_object);
    printf("\n");
    
    printf("Memory Manager Tests:\n");
    RUN_TEST(test_mm_create_destroy);
    RUN_TEST(test_mm_alloc_strategies);
    RUN_TEST(test_mm_statistics);
    RUN_TEST(test_mm_gc_operations);
    RUN_TEST(test_mm_config);
    RUN_TEST(test_mm_refcount);
    printf("\n");
    
    printf("Integration Tests:\n");
    RUN_TEST(test_integration_pools_and_gc);
    RUN_TEST(test_integration_stress);
    printf("\n");
    
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("Results: %d/%d tests passed", passed, total);
    if (failed > 0) {
        printf(" (%d failed)", failed);
    }
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("\n");
    
    return (failed == 0) ? 0 : 1;
}
