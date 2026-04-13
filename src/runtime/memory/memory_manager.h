#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "gc_enhanced.h"
#include "memory_pool.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * Unified Memory Manager for KLang
 * 
 * Integrates:
 * - Enhanced GC (generational, incremental, reference counting)
 * - Memory pools for common allocations
 * - Statistics and monitoring
 * - Integration with runtime/interpreter/VM
 */

typedef struct MemoryManager {
    GCHeap *gc_heap;                    /* Enhanced GC heap */
    MemoryPoolManager *pool_manager;    /* Memory pools */
    
    /* Configuration */
    bool use_pools;                     /* Enable memory pools */
    bool use_gc;                        /* Enable GC */
    bool use_refcount;                  /* Enable reference counting */
    
    /* Statistics */
    size_t manual_allocations;          /* Manual malloc/free count */
    size_t pooled_allocations;          /* Pool allocations */
    size_t gc_allocations;              /* GC allocations */
} MemoryManager;

/* Memory manager lifecycle */
MemoryManager *memory_manager_new(void);
void memory_manager_free(MemoryManager *manager);

/* Allocation strategies */
void *mm_alloc(MemoryManager *manager, size_t size);
void *mm_alloc_pooled(MemoryManager *manager, size_t size);
void *mm_alloc_gc(MemoryManager *manager, size_t size);
void *mm_realloc(MemoryManager *manager, void *ptr, size_t new_size);
void mm_free(MemoryManager *manager, void *ptr);

/* Reference counting operations */
void mm_retain(MemoryManager *manager, void *ptr);
void mm_release(MemoryManager *manager, void *ptr);

/* GC operations */
void mm_gc_collect(MemoryManager *manager);
void mm_gc_collect_full(MemoryManager *manager);
void mm_gc_add_root(MemoryManager *manager, void *root);
void mm_gc_remove_root(MemoryManager *manager, void *root);

/* Statistics and monitoring */
typedef struct {
    /* Memory usage */
    size_t total_allocated;
    size_t total_freed;
    size_t current_usage;
    size_t peak_usage;
    
    /* Allocations by type */
    size_t manual_allocs;
    size_t pooled_allocs;
    size_t gc_allocs;
    
    /* GC stats */
    size_t gc_collections_young;
    size_t gc_collections_full;
    size_t gc_objects_collected;
    
    /* Pool stats */
    size_t pool_total_bytes;
    size_t pool_free_bytes;
    
    /* Memory pressure */
    double memory_pressure;
} MemoryStats;

MemoryStats mm_get_stats(MemoryManager *manager);
void mm_print_stats(MemoryManager *manager);
void mm_reset_stats(MemoryManager *manager);

/* Configuration */
void mm_enable_pools(MemoryManager *manager, bool enable);
void mm_enable_gc(MemoryManager *manager, bool enable);
void mm_enable_refcount(MemoryManager *manager, bool enable);
void mm_set_gc_config(MemoryManager *manager, GCConfig *config);

/* Debugging */
bool mm_validate_pointer(MemoryManager *manager, void *ptr);
void mm_dump_state(MemoryManager *manager);

#endif /* MEMORY_MANAGER_H */
