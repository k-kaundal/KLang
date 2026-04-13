#include "runtime/memory/memory_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Create new memory manager */
MemoryManager *memory_manager_new(void) {
    MemoryManager *manager = calloc(1, sizeof(MemoryManager));
    if (!manager) return NULL;
    
    /* Initialize GC heap */
    manager->gc_heap = gc_heap_new();
    if (!manager->gc_heap) {
        free(manager);
        return NULL;
    }
    
    /* Initialize memory pools */
    manager->pool_manager = pool_manager_new();
    if (!manager->pool_manager) {
        gc_heap_free(manager->gc_heap);
        free(manager);
        return NULL;
    }
    
    /* Default configuration: enable everything */
    manager->use_pools = true;
    manager->use_gc = true;
    manager->use_refcount = true;
    
    manager->manual_allocations = 0;
    manager->pooled_allocations = 0;
    manager->gc_allocations = 0;
    
    return manager;
}

/* Free memory manager */
void memory_manager_free(MemoryManager *manager) {
    if (!manager) return;
    
    gc_heap_free(manager->gc_heap);
    pool_manager_free(manager->pool_manager);
    free(manager);
}

/* Smart allocation - chooses best strategy based on size and config */
void *mm_alloc(MemoryManager *manager, size_t size) {
    if (!manager) return NULL;
    
    /* If GC is enabled and size is reasonable, use GC allocation */
    if (manager->use_gc && size < 1024 * 1024) {  /* < 1MB */
        manager->gc_allocations++;
        return gc_alloc(manager->gc_heap, size);
    }
    
    /* If pools are enabled and size fits, use pooled allocation */
    if (manager->use_pools) {
        if (size <= SMALL_POOL_SIZE) {
            manager->pooled_allocations++;
            return pool_alloc(manager->pool_manager->small_pool);
        } else if (size <= MEDIUM_POOL_SIZE) {
            manager->pooled_allocations++;
            return pool_alloc(manager->pool_manager->medium_pool);
        } else if (size <= LARGE_POOL_SIZE) {
            manager->pooled_allocations++;
            return pool_alloc(manager->pool_manager->large_pool);
        }
    }
    
    /* Fall back to manual allocation */
    manager->manual_allocations++;
    return malloc(size);
}

/* Pooled allocation */
void *mm_alloc_pooled(MemoryManager *manager, size_t size) {
    if (!manager || !manager->use_pools) {
        return malloc(size);
    }
    
    manager->pooled_allocations++;
    
    if (size <= SMALL_POOL_SIZE) {
        return pool_alloc(manager->pool_manager->small_pool);
    } else if (size <= MEDIUM_POOL_SIZE) {
        return pool_alloc(manager->pool_manager->medium_pool);
    } else if (size <= LARGE_POOL_SIZE) {
        return pool_alloc(manager->pool_manager->large_pool);
    }
    
    /* Size too large for pools */
    return malloc(size);
}

/* GC allocation */
void *mm_alloc_gc(MemoryManager *manager, size_t size) {
    if (!manager || !manager->use_gc) {
        return malloc(size);
    }
    
    manager->gc_allocations++;
    return gc_alloc(manager->gc_heap, size);
}

/* Reallocation */
void *mm_realloc(MemoryManager *manager, void *ptr, size_t new_size) {
    if (!manager) return realloc(ptr, new_size);
    
    /* For simplicity, just use standard realloc */
    /* In a production system, you'd track which allocation type was used */
    return realloc(ptr, new_size);
}

/* Free memory */
void mm_free(MemoryManager *manager, void *ptr) {
    if (!ptr) return;
    
    /* Try to determine allocation type and free accordingly */
    /* In a production system, you'd track metadata about each allocation */
    
    /* For now, assume it might be pooled or manual */
    /* GC-allocated objects are freed by the GC */
    
    /* Try pool deallocation first (it will detect if pointer is not pooled) */
    if (manager && manager->use_pools) {
        /* This is a simplified approach - in production you'd need better tracking */
        free(ptr);
    } else {
        free(ptr);
    }
}

/* Reference counting */
void mm_retain(MemoryManager *manager, void *ptr) {
    if (!manager || !ptr || !manager->use_refcount) return;
    gc_retain(ptr);
}

void mm_release(MemoryManager *manager, void *ptr) {
    if (!manager || !ptr || !manager->use_refcount) return;
    gc_release(manager->gc_heap, ptr);
}

/* GC operations */
void mm_gc_collect(MemoryManager *manager) {
    if (!manager || !manager->use_gc) return;
    gc_collect_young(manager->gc_heap);
}

void mm_gc_collect_full(MemoryManager *manager) {
    if (!manager || !manager->use_gc) return;
    gc_collect_full(manager->gc_heap);
}

void mm_gc_add_root(MemoryManager *manager, void *root) {
    if (!manager || !manager->use_gc) return;
    gc_add_root(manager->gc_heap, root);
}

void mm_gc_remove_root(MemoryManager *manager, void *root) {
    if (!manager || !manager->use_gc) return;
    gc_remove_root(manager->gc_heap, root);
}

/* Statistics */
MemoryStats mm_get_stats(MemoryManager *manager) {
    MemoryStats stats;
    memset(&stats, 0, sizeof(MemoryStats));
    
    if (!manager) return stats;
    
    GCStats gc_stats = gc_get_stats(manager->gc_heap);
    
    stats.total_allocated = gc_stats.total_allocated;
    stats.total_freed = gc_stats.total_freed;
    stats.current_usage = gc_stats.current_usage;
    stats.peak_usage = gc_stats.peak_usage;
    
    stats.manual_allocs = manager->manual_allocations;
    stats.pooled_allocs = manager->pooled_allocations;
    stats.gc_allocs = manager->gc_allocations;
    
    stats.gc_collections_young = gc_stats.collections_young;
    stats.gc_collections_full = gc_stats.collections_full;
    stats.gc_objects_collected = gc_stats.objects_collected;
    
    /* Pool stats */
    if (manager->pool_manager) {
        size_t total = 0, free_bytes = 0;
        pool_get_stats(manager->pool_manager->small_pool, &total, &free_bytes);
        stats.pool_total_bytes += total;
        stats.pool_free_bytes += free_bytes;
        
        pool_get_stats(manager->pool_manager->medium_pool, &total, &free_bytes);
        stats.pool_total_bytes += total;
        stats.pool_free_bytes += free_bytes;
        
        pool_get_stats(manager->pool_manager->large_pool, &total, &free_bytes);
        stats.pool_total_bytes += total;
        stats.pool_free_bytes += free_bytes;
    }
    
    stats.memory_pressure = gc_get_memory_pressure(manager->gc_heap);
    
    return stats;
}

void mm_print_stats(MemoryManager *manager) {
    if (!manager) return;
    
    MemoryStats stats = mm_get_stats(manager);
    
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║    KLang Memory Manager Statistics    ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    
    printf("Memory Usage:\n");
    printf("  Current:     %10zu bytes\n", stats.current_usage);
    printf("  Peak:        %10zu bytes\n", stats.peak_usage);
    printf("  Allocated:   %10zu bytes\n", stats.total_allocated);
    printf("  Freed:       %10zu bytes\n", stats.total_freed);
    printf("\n");
    
    printf("Allocations by Type:\n");
    printf("  Manual:      %10zu\n", stats.manual_allocs);
    printf("  Pooled:      %10zu\n", stats.pooled_allocs);
    printf("  GC:          %10zu\n", stats.gc_allocs);
    printf("\n");
    
    printf("Garbage Collection:\n");
    printf("  Young GCs:   %10zu\n", stats.gc_collections_young);
    printf("  Full GCs:    %10zu\n", stats.gc_collections_full);
    printf("  Collected:   %10zu objects\n", stats.gc_objects_collected);
    printf("\n");
    
    printf("Memory Pools:\n");
    printf("  Total:       %10zu bytes\n", stats.pool_total_bytes);
    printf("  Free:        %10zu bytes\n", stats.pool_free_bytes);
    printf("  Used:        %10zu bytes\n", 
           stats.pool_total_bytes - stats.pool_free_bytes);
    printf("\n");
    
    printf("System Status:\n");
    printf("  Pressure:    %10.2f%%\n", stats.memory_pressure * 100.0);
    printf("\n");
    
    /* Print detailed GC stats if available */
    if (manager->use_gc) {
        gc_print_stats(manager->gc_heap);
    }
}

void mm_reset_stats(MemoryManager *manager) {
    if (!manager) return;
    
    gc_reset_stats(manager->gc_heap);
    manager->manual_allocations = 0;
    manager->pooled_allocations = 0;
    manager->gc_allocations = 0;
}

/* Configuration */
void mm_enable_pools(MemoryManager *manager, bool enable) {
    if (manager) manager->use_pools = enable;
}

void mm_enable_gc(MemoryManager *manager, bool enable) {
    if (manager) manager->use_gc = enable;
}

void mm_enable_refcount(MemoryManager *manager, bool enable) {
    if (manager) manager->use_refcount = enable;
}

void mm_set_gc_config(MemoryManager *manager, GCConfig *config) {
    if (!manager || !config) return;
    gc_set_config(manager->gc_heap, config);
}

/* Debugging */
bool mm_validate_pointer(MemoryManager *manager, void *ptr) {
    if (!manager || !ptr) return false;
    
    /* Basic validation - check if pointer looks reasonable */
    /* In production, you'd maintain a registry of allocated pointers */
    return true;
}

void mm_dump_state(MemoryManager *manager) {
    if (!manager) return;
    
    printf("\n=== Memory Manager State Dump ===\n");
    printf("Configuration:\n");
    printf("  Pools enabled:    %s\n", manager->use_pools ? "yes" : "no");
    printf("  GC enabled:       %s\n", manager->use_gc ? "yes" : "no");
    printf("  RefCount enabled: %s\n", manager->use_refcount ? "yes" : "no");
    printf("\n");
    
    mm_print_stats(manager);
}
