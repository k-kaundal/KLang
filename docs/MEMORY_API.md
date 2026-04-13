# KLang Memory Management API Reference

## Overview

This document provides a complete API reference for the KLang memory management system.

## Table of Contents

1. [Memory Pools](#memory-pools)
2. [Enhanced Garbage Collector](#enhanced-garbage-collector)
3. [Memory Manager](#memory-manager)
4. [Data Structures](#data-structures)
5. [Constants](#constants)

---

## Memory Pools

### Types

#### `MemoryPool`
```c
typedef struct MemoryPool {
    size_t object_size;
    PoolBlock *blocks;
    size_t total_allocated;
    size_t total_objects;
    size_t free_objects;
} MemoryPool;
```

#### `MemoryPoolManager`
```c
typedef struct MemoryPoolManager {
    MemoryPool *small_pool;   // 32-byte objects
    MemoryPool *medium_pool;  // 128-byte objects
    MemoryPool *large_pool;   // 512-byte objects
} MemoryPoolManager;
```

### Functions

#### `pool_manager_new()`
```c
MemoryPoolManager *pool_manager_new(void);
```
Creates a new pool manager with small, medium, and large pools.

**Returns**: Pointer to new `MemoryPoolManager`, or `NULL` on failure.

**Example**:
```c
MemoryPoolManager *manager = pool_manager_new();
if (!manager) {
    fprintf(stderr, "Failed to create pool manager\n");
    return;
}
```

#### `pool_manager_free()`
```c
void pool_manager_free(MemoryPoolManager *manager);
```
Frees the pool manager and all its pools.

**Parameters**:
- `manager`: Pool manager to free

#### `pool_new()`
```c
MemoryPool *pool_new(size_t object_size);
```
Creates a new memory pool for objects of a specific size.

**Parameters**:
- `object_size`: Size of objects in bytes

**Returns**: Pointer to new `MemoryPool`, or `NULL` on failure.

#### `pool_free()`
```c
void pool_free(MemoryPool *pool);
```
Frees a memory pool and all its blocks.

**Parameters**:
- `pool`: Pool to free

#### `pool_alloc()`
```c
void *pool_alloc(MemoryPool *pool);
```
Allocates an object from the pool.

**Parameters**:
- `pool`: Pool to allocate from

**Returns**: Pointer to allocated memory, or `NULL` on failure.

**Complexity**: O(1) average case

**Example**:
```c
void *obj = pool_alloc(manager->small_pool);
if (obj) {
    // Use object
    memset(obj, 0, SMALL_POOL_SIZE);
}
```

#### `pool_dealloc()`
```c
void pool_dealloc(MemoryPool *pool, void *ptr);
```
Deallocates an object back to the pool.

**Parameters**:
- `pool`: Pool to return object to
- `ptr`: Pointer to object

**Complexity**: O(n) where n is number of blocks

#### `pool_get_stats()`
```c
void pool_get_stats(MemoryPool *pool, size_t *total_bytes, size_t *free_bytes);
```
Gets statistics about pool usage.

**Parameters**:
- `pool`: Pool to query
- `total_bytes`: Output - total allocated bytes
- `free_bytes`: Output - free bytes

---

## Enhanced Garbage Collector

### Types

#### `GCObject`
```c
struct GCObject {
    size_t size;
    int ref_count;
    uint8_t generation;  // GC_GEN_YOUNG or GC_GEN_OLD
    uint8_t color;       // GC_WHITE, GC_GRAY, or GC_BLACK
    uint8_t age;
    uint8_t pinned;
    GCObject *next;
    GCObject *prev;
    void (*mark_fn)(GCObject *obj, void *context);
};
```

#### `GCHeap`
```c
typedef struct {
    GCObject *young_gen;
    GCObject *old_gen;
    GCObject *gray_list;
    GCConfig config;
    GCStats stats;
    bool gc_in_progress;
    bool gc_marking_phase;
    void **roots;
    size_t root_count;
    size_t root_capacity;
} GCHeap;
```

#### `GCConfig`
```c
typedef struct {
    size_t young_threshold;         // Bytes before young collection
    size_t heap_threshold;          // Bytes before full collection
    size_t max_heap_size;           // Maximum heap size (0 = unlimited)
    int promotion_age;              // Age before promoting to old gen
    bool incremental_enabled;       // Enable incremental collection
    size_t incremental_step_size;   // Objects to mark per step
    bool auto_gc_enabled;           // Enable automatic GC
    double gc_pressure_threshold;   // Pressure ratio (0.0-1.0)
} GCConfig;
```

#### `GCStats`
```c
typedef struct {
    size_t total_allocated;
    size_t total_freed;
    size_t current_usage;
    size_t peak_usage;
    size_t young_objects;
    size_t old_objects;
    size_t total_objects;
    size_t collections_young;
    size_t collections_full;
    size_t objects_collected;
    size_t bytes_collected;
    uint64_t total_gc_time;
    uint64_t last_gc_time;
    size_t incremental_steps;
} GCStats;
```

### Functions

#### `gc_heap_new()`
```c
GCHeap *gc_heap_new(void);
```
Creates a new GC heap with default configuration.

**Returns**: Pointer to new `GCHeap`, or `NULL` on failure.

#### `gc_heap_free()`
```c
void gc_heap_free(GCHeap *heap);
```
Frees the GC heap and all managed objects.

**Parameters**:
- `heap`: Heap to free

#### `gc_alloc()`
```c
void *gc_alloc(GCHeap *heap, size_t size);
```
Allocates memory managed by the garbage collector.

**Parameters**:
- `heap`: GC heap
- `size`: Size in bytes

**Returns**: Pointer to allocated memory, or `NULL` on failure.

**Example**:
```c
void *obj = gc_alloc(heap, sizeof(MyStruct));
if (obj) {
    MyStruct *s = (MyStruct *)obj;
    s->value = 42;
}
```

#### `gc_alloc_with_finalizer()`
```c
void *gc_alloc_with_finalizer(GCHeap *heap, size_t size, 
                              void (*mark_fn)(GCObject *, void *));
```
Allocates memory with a custom mark function for complex objects.

**Parameters**:
- `heap`: GC heap
- `size`: Size in bytes
- `mark_fn`: Function to mark internal references

**Returns**: Pointer to allocated memory, or `NULL` on failure.

#### `gc_retain()`
```c
void gc_retain(void *ptr);
```
Increments the reference count of an object.

**Parameters**:
- `ptr`: Pointer to object

**Example**:
```c
void share_object(void *obj) {
    gc_retain(obj);
    global_array[count++] = obj;
}
```

#### `gc_release()`
```c
void gc_release(GCHeap *heap, void *ptr);
```
Decrements the reference count. Frees object if count reaches 0.

**Parameters**:
- `heap`: GC heap
- `ptr`: Pointer to object

#### `gc_get_ref_count()`
```c
int gc_get_ref_count(void *ptr);
```
Gets the current reference count of an object.

**Parameters**:
- `ptr`: Pointer to object

**Returns**: Reference count

#### `gc_collect_young()`
```c
void gc_collect_young(GCHeap *heap);
```
Performs a young generation garbage collection.

**Parameters**:
- `heap`: GC heap

**Complexity**: O(n) where n is young generation size

#### `gc_collect_full()`
```c
void gc_collect_full(GCHeap *heap);
```
Performs a full heap garbage collection.

**Parameters**:
- `heap`: GC heap

**Complexity**: O(n) where n is total heap size

#### `gc_collect_incremental_step()`
```c
void gc_collect_incremental_step(GCHeap *heap);
```
Performs one step of incremental garbage collection.

**Parameters**:
- `heap`: GC heap

#### `gc_add_root()`
```c
void gc_add_root(GCHeap *heap, void *root);
```
Adds an object to the root set.

**Parameters**:
- `heap`: GC heap
- `root`: Pointer to root object

**Example**:
```c
void *important = gc_alloc(heap, 1000);
gc_add_root(heap, important);  // Protect from collection
```

#### `gc_remove_root()`
```c
void gc_remove_root(GCHeap *heap, void *root);
```
Removes an object from the root set.

**Parameters**:
- `heap`: GC heap
- `root`: Pointer to root object

#### `gc_clear_roots()`
```c
void gc_clear_roots(GCHeap *heap);
```
Removes all roots.

**Parameters**:
- `heap`: GC heap

#### `gc_get_memory_pressure()`
```c
double gc_get_memory_pressure(GCHeap *heap);
```
Calculates current memory pressure (0.0 to 1.0).

**Parameters**:
- `heap`: GC heap

**Returns**: Memory pressure ratio

#### `gc_should_collect()`
```c
bool gc_should_collect(GCHeap *heap);
```
Checks if garbage collection should be triggered.

**Parameters**:
- `heap`: GC heap

**Returns**: `true` if GC recommended

#### `gc_set_config()`
```c
void gc_set_config(GCHeap *heap, GCConfig *config);
```
Sets GC configuration.

**Parameters**:
- `heap`: GC heap
- `config`: New configuration

#### `gc_get_default_config()`
```c
GCConfig gc_get_default_config(void);
```
Gets the default GC configuration.

**Returns**: Default configuration

#### `gc_get_stats()`
```c
GCStats gc_get_stats(GCHeap *heap);
```
Gets GC statistics.

**Parameters**:
- `heap`: GC heap

**Returns**: Statistics structure

#### `gc_print_stats()`
```c
void gc_print_stats(GCHeap *heap);
```
Prints formatted GC statistics.

**Parameters**:
- `heap`: GC heap

#### `gc_reset_stats()`
```c
void gc_reset_stats(GCHeap *heap);
```
Resets GC statistics to zero.

**Parameters**:
- `heap`: GC heap

#### `gc_pin_object()`
```c
void gc_pin_object(void *ptr);
```
Pins an object, preventing collection.

**Parameters**:
- `ptr`: Pointer to object

#### `gc_unpin_object()`
```c
void gc_unpin_object(void *ptr);
```
Unpins an object.

**Parameters**:
- `ptr`: Pointer to object

#### `gc_free_all()`
```c
void gc_free_all(GCHeap *heap);
```
Frees all objects in the heap.

**Parameters**:
- `heap`: GC heap

---

## Memory Manager

### Types

#### `MemoryManager`
```c
typedef struct MemoryManager {
    GCHeap *gc_heap;
    MemoryPoolManager *pool_manager;
    bool use_pools;
    bool use_gc;
    bool use_refcount;
    size_t manual_allocations;
    size_t pooled_allocations;
    size_t gc_allocations;
} MemoryManager;
```

#### `MemoryStats`
```c
typedef struct {
    size_t total_allocated;
    size_t total_freed;
    size_t current_usage;
    size_t peak_usage;
    size_t manual_allocs;
    size_t pooled_allocs;
    size_t gc_allocs;
    size_t gc_collections_young;
    size_t gc_collections_full;
    size_t gc_objects_collected;
    size_t pool_total_bytes;
    size_t pool_free_bytes;
    double memory_pressure;
} MemoryStats;
```

### Functions

#### `memory_manager_new()`
```c
MemoryManager *memory_manager_new(void);
```
Creates a new unified memory manager.

**Returns**: Pointer to new `MemoryManager`, or `NULL` on failure.

#### `memory_manager_free()`
```c
void memory_manager_free(MemoryManager *manager);
```
Frees the memory manager and all managed memory.

**Parameters**:
- `manager`: Memory manager to free

#### `mm_alloc()`
```c
void *mm_alloc(MemoryManager *manager, size_t size);
```
Allocates memory using the best strategy for the size.

**Parameters**:
- `manager`: Memory manager
- `size`: Size in bytes

**Returns**: Pointer to allocated memory, or `NULL` on failure.

**Strategy**:
- < 1MB: GC allocation
- Fits pool: Pooled allocation
- Otherwise: Manual allocation

#### `mm_alloc_pooled()`
```c
void *mm_alloc_pooled(MemoryManager *manager, size_t size);
```
Allocates from memory pools.

**Parameters**:
- `manager`: Memory manager
- `size`: Size in bytes

**Returns**: Pointer to allocated memory

#### `mm_alloc_gc()`
```c
void *mm_alloc_gc(MemoryManager *manager, size_t size);
```
Allocates using garbage collector.

**Parameters**:
- `manager`: Memory manager
- `size`: Size in bytes

**Returns**: Pointer to allocated memory

#### `mm_realloc()`
```c
void *mm_realloc(MemoryManager *manager, void *ptr, size_t new_size);
```
Reallocates memory.

**Parameters**:
- `manager`: Memory manager
- `ptr`: Existing pointer
- `new_size`: New size in bytes

**Returns**: Pointer to reallocated memory

#### `mm_free()`
```c
void mm_free(MemoryManager *manager, void *ptr);
```
Frees memory.

**Parameters**:
- `manager`: Memory manager
- `ptr`: Pointer to free

#### `mm_retain()`
```c
void mm_retain(MemoryManager *manager, void *ptr);
```
Increments reference count.

**Parameters**:
- `manager`: Memory manager
- `ptr`: Pointer to object

#### `mm_release()`
```c
void mm_release(MemoryManager *manager, void *ptr);
```
Decrements reference count.

**Parameters**:
- `manager`: Memory manager
- `ptr`: Pointer to object

#### `mm_gc_collect()`
```c
void mm_gc_collect(MemoryManager *manager);
```
Triggers young generation collection.

**Parameters**:
- `manager`: Memory manager

#### `mm_gc_collect_full()`
```c
void mm_gc_collect_full(MemoryManager *manager);
```
Triggers full heap collection.

**Parameters**:
- `manager`: Memory manager

#### `mm_gc_add_root()`
```c
void mm_gc_add_root(MemoryManager *manager, void *root);
```
Adds GC root.

**Parameters**:
- `manager`: Memory manager
- `root`: Root pointer

#### `mm_gc_remove_root()`
```c
void mm_gc_remove_root(MemoryManager *manager, void *root);
```
Removes GC root.

**Parameters**:
- `manager`: Memory manager
- `root`: Root pointer

#### `mm_get_stats()`
```c
MemoryStats mm_get_stats(MemoryManager *manager);
```
Gets unified memory statistics.

**Parameters**:
- `manager`: Memory manager

**Returns**: Statistics structure

#### `mm_print_stats()`
```c
void mm_print_stats(MemoryManager *manager);
```
Prints formatted statistics.

**Parameters**:
- `manager`: Memory manager

#### `mm_reset_stats()`
```c
void mm_reset_stats(MemoryManager *manager);
```
Resets statistics.

**Parameters**:
- `manager`: Memory manager

#### `mm_enable_pools()`
```c
void mm_enable_pools(MemoryManager *manager, bool enable);
```
Enables/disables memory pools.

**Parameters**:
- `manager`: Memory manager
- `enable`: Enable flag

#### `mm_enable_gc()`
```c
void mm_enable_gc(MemoryManager *manager, bool enable);
```
Enables/disables garbage collection.

**Parameters**:
- `manager`: Memory manager
- `enable`: Enable flag

#### `mm_enable_refcount()`
```c
void mm_enable_refcount(MemoryManager *manager, bool enable);
```
Enables/disables reference counting.

**Parameters**:
- `manager`: Memory manager
- `enable`: Enable flag

#### `mm_set_gc_config()`
```c
void mm_set_gc_config(MemoryManager *manager, GCConfig *config);
```
Sets GC configuration.

**Parameters**:
- `manager`: Memory manager
- `config`: GC configuration

#### `mm_validate_pointer()`
```c
bool mm_validate_pointer(MemoryManager *manager, void *ptr);
```
Validates a pointer.

**Parameters**:
- `manager`: Memory manager
- `ptr`: Pointer to validate

**Returns**: `true` if valid

#### `mm_dump_state()`
```c
void mm_dump_state(MemoryManager *manager);
```
Dumps complete memory manager state.

**Parameters**:
- `manager`: Memory manager

---

## Constants

### Pool Sizes
```c
#define SMALL_POOL_SIZE     32      // 32 bytes
#define MEDIUM_POOL_SIZE    128     // 128 bytes
#define LARGE_POOL_SIZE     512     // 512 bytes
#define POOL_BLOCK_COUNT    256     // Objects per block
```

### GC Generations
```c
typedef enum {
    GC_GEN_YOUNG = 0,
    GC_GEN_OLD = 1
} GCGeneration;
```

### GC Colors
```c
typedef enum {
    GC_WHITE = 0,    // Unmarked
    GC_GRAY = 1,     // Marked but not scanned
    GC_BLACK = 2     // Fully scanned
} GCColor;
```

---

## Error Handling

All allocation functions return `NULL` on failure. Always check return values:

```c
void *ptr = mm_alloc(manager, size);
if (!ptr) {
    fprintf(stderr, "Allocation failed\n");
    return ERROR;
}
```

---

## Thread Safety

**Current Status**: The memory management system is **not thread-safe**. Use external synchronization if accessing from multiple threads.

**Future**: Thread-safe version planned for future release.

---

## See Also

- [Memory Management Guide](MEMORY_MANAGEMENT.md)
- [KLang Runtime](../runtime/README.md)
- [Performance Tuning](PERFORMANCE.md)
