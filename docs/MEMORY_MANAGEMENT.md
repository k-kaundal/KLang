# KLang Memory Management System

## Overview

KLang features a sophisticated memory management system that combines multiple strategies for optimal performance and safety:

- **Reference Counting**: Immediate cleanup when objects are no longer referenced
- **Generational Garbage Collection**: Efficient collection by separating young and old objects
- **Incremental Collection**: Reduced pause times through step-by-step marking
- **Memory Pools**: Fast allocation for commonly-sized objects
- **Automatic Triggers**: Smart collection based on memory pressure

## Architecture

### Components

1. **Memory Pools (`memory_pool.h/c`)**
   - Pre-allocated blocks for small, medium, and large objects
   - Reduces fragmentation and malloc/free overhead
   - Bitmap-based free list for O(1) allocation

2. **Enhanced GC (`gc_enhanced.h/c`)**
   - Generational collection (young/old generations)
   - Reference counting for immediate cleanup
   - Incremental marking to reduce pause times
   - Configurable thresholds and behaviors

3. **Memory Manager (`memory_manager.h/c`)**
   - Unified interface to all memory subsystems
   - Smart allocation strategy selection
   - Comprehensive statistics and monitoring

### Memory Object Layout

```
+------------------+
| GCObject Header  |  <- Metadata (size, ref_count, generation, etc.)
+------------------+
| User Data        |  <- Your object
|                  |
+------------------+
```

## Usage

### Basic Allocation

```c
#include "runtime/memory/memory_manager.h"

/* Create memory manager */
MemoryManager *mm = memory_manager_new();

/* Allocate memory (automatic strategy selection) */
void *ptr = mm_alloc(mm, 100);

/* Use the memory */
/* ... */

/* Free when done */
mm_free(mm, ptr);

/* Cleanup */
memory_manager_free(mm);
```

### Reference Counting

```c
/* Allocate with GC */
void *obj = mm_alloc_gc(mm, sizeof(MyStruct));

/* Share the reference */
mm_retain(mm, obj);  /* Increment ref count */
some_function(obj);

/* Release when done */
mm_release(mm, obj);  /* Decrement ref count */

/* When ref count reaches 0, object is automatically freed */
```

### Root Set Management

```c
/* Add roots to keep objects alive during GC */
void *important_obj = mm_alloc_gc(mm, 1000);
mm_gc_add_root(mm, important_obj);

/* Do work that might trigger GC */
for (int i = 0; i < 10000; i++) {
    void *temp = mm_alloc_gc(mm, 100);
    /* temp objects can be collected */
}

/* Remove root when no longer needed */
mm_gc_remove_root(mm, important_obj);
```

### Manual Collection

```c
/* Trigger young generation collection */
mm_gc_collect(mm);

/* Trigger full heap collection */
mm_gc_collect_full(mm);
```

### Statistics and Monitoring

```c
/* Get statistics */
MemoryStats stats = mm_get_stats(mm);

printf("Memory usage: %zu bytes\n", stats.current_usage);
printf("Peak usage: %zu bytes\n", stats.peak_usage);
printf("GC collections: %zu\n", stats.gc_collections_young);
printf("Objects collected: %zu\n", stats.gc_objects_collected);

/* Print detailed report */
mm_print_stats(mm);
```

## Configuration

### GC Configuration

```c
/* Get default configuration */
GCConfig config = gc_get_default_config();

/* Customize thresholds */
config.young_threshold = 2 * 1024 * 1024;     /* 2MB */
config.heap_threshold = 20 * 1024 * 1024;     /* 20MB */
config.promotion_age = 5;                      /* Promote after 5 collections */

/* Enable/disable features */
config.incremental_enabled = true;
config.auto_gc_enabled = true;
config.gc_pressure_threshold = 0.80;          /* Trigger at 80% */

/* Apply configuration */
mm_set_gc_config(mm, &config);
```

### Memory Manager Configuration

```c
/* Enable/disable subsystems */
mm_enable_pools(mm, true);      /* Use memory pools */
mm_enable_gc(mm, true);         /* Use garbage collection */
mm_enable_refcount(mm, true);   /* Use reference counting */
```

## Allocation Strategies

The memory manager automatically selects the best allocation strategy:

1. **GC Allocation** (< 1MB, GC enabled)
   - Managed by garbage collector
   - Automatic cleanup when unreachable
   - Reference counted

2. **Pooled Allocation** (pools enabled, size matches pool)
   - Small objects: ≤ 32 bytes
   - Medium objects: ≤ 128 bytes
   - Large objects: ≤ 512 bytes
   - Fast allocation/deallocation
   - Reduced fragmentation

3. **Manual Allocation** (fallback)
   - Direct malloc/free
   - For very large objects
   - When pools/GC disabled

## Performance Characteristics

### Memory Pools
- **Allocation**: O(1) - bitmap scan
- **Deallocation**: O(1) - direct bitmap update
- **Overhead**: 1 bit per object + block header
- **Best for**: Frequent allocation/deallocation of similar-sized objects

### Reference Counting
- **Retain/Release**: O(1)
- **Immediate cleanup**: Objects freed as soon as ref count reaches 0
- **Overhead**: 4 bytes per object
- **Best for**: Deterministic cleanup, avoiding GC pauses

### Generational GC
- **Young collection**: Fast - only scans young generation
- **Full collection**: Slower - scans entire heap
- **Promotion**: O(1) - pointer updates
- **Best for**: Objects with different lifetimes

### Incremental GC
- **Step size**: Configurable (default: 100 objects)
- **Pause time**: Minimal - spread across multiple steps
- **Throughput**: Slightly lower than full collection
- **Best for**: Real-time or interactive applications

## Integration with KLang Runtime

### Interpreter Integration

The memory manager is integrated with the KLang interpreter for automatic management of Values:

```c
/* In interpreter.c */
Value eval_node(Interpreter *interp, ASTNode *node) {
    /* Use MM for allocations */
    if (node->type == AST_LIST) {
        Value v;
        v.type = VAL_LIST;
        v.as.list_val.items = mm_alloc(interp->mm, 
                                       sizeof(Value) * capacity);
        /* ... */
    }
    /* ... */
}
```

### VM Integration

The VM uses the memory manager for bytecode and runtime structures:

```c
/* In vm.c */
VM *vm_new() {
    VM *vm = malloc(sizeof(VM));
    vm->mm = memory_manager_new();
    
    /* Allocate stack with GC */
    vm->stack = mm_alloc_gc(vm->mm, sizeof(Value) * STACK_SIZE);
    mm_gc_add_root(vm->mm, vm->stack);
    
    return vm;
}
```

## Best Practices

### 1. Use Reference Counting for Shared Objects

```c
/* Good: explicit reference management */
void share_object(void *obj, MemoryManager *mm) {
    mm_retain(mm, obj);
    global_shared_objects[count++] = obj;
}

void unshare_object(void *obj, MemoryManager *mm) {
    mm_release(mm, obj);
}
```

### 2. Add Long-Lived Objects as Roots

```c
/* Good: protect important objects from collection */
void setup_globals(MemoryManager *mm) {
    global_config = mm_alloc_gc(mm, sizeof(Config));
    mm_gc_add_root(mm, global_config);
}
```

### 3. Use Pooled Allocation for Hot Paths

```c
/* Good: use pools for frequently allocated objects */
typedef struct {
    int x, y;
} Point;  /* 8 bytes - fits in small pool */

Point *create_point(MemoryManager *mm) {
    return mm_alloc_pooled(mm, sizeof(Point));
}
```

### 4. Trigger Collection at Safe Points

```c
/* Good: collect between operations */
void process_batch(MemoryManager *mm, Item *items, int count) {
    for (int i = 0; i < count; i++) {
        process_item(&items[i]);
        
        /* Collect every 100 items */
        if (i % 100 == 0) {
            mm_gc_collect(mm);
        }
    }
}
```

### 5. Monitor Memory Usage

```c
/* Good: track memory pressure */
void check_memory_health(MemoryManager *mm) {
    MemoryStats stats = mm_get_stats(mm);
    
    if (stats.memory_pressure > 0.90) {
        fprintf(stderr, "Warning: High memory pressure!\n");
        mm_gc_collect_full(mm);
    }
}
```

## Debugging

### Memory Leaks

Use statistics to detect leaks:

```c
/* Before operation */
MemoryStats before = mm_get_stats(mm);

/* Do operation */
run_test();

/* After operation */
MemoryStats after = mm_get_stats(mm);

/* Check for leaks */
if (after.current_usage > before.current_usage) {
    printf("Potential leak: %zu bytes not freed\n",
           after.current_usage - before.current_usage);
}
```

### Heap Dump

```c
/* Dump complete memory state */
mm_dump_state(mm);

/* Prints:
 * - Configuration
 * - Memory usage
 * - Object counts
 * - GC statistics
 * - Pool statistics
 */
```

### Validation

```c
/* Validate pointer before use */
if (!mm_validate_pointer(mm, ptr)) {
    fprintf(stderr, "Invalid pointer: %p\n", ptr);
}
```

## Advanced Features

### Custom Mark Functions

For complex objects with internal references:

```c
void my_object_mark(GCObject *obj, void *context) {
    GCHeap *heap = (GCHeap *)context;
    MyObject *my_obj = (MyObject *)gc_ptr_from_object(obj);
    
    /* Mark internal references */
    if (my_obj->child) {
        GCObject *child_obj = gc_object_from_ptr(my_obj->child);
        mark_object(heap, child_obj);
    }
}

/* Allocate with custom mark function */
MyObject *obj = gc_alloc_with_finalizer(heap, sizeof(MyObject), 
                                        my_object_mark);
```

### Pinning Objects

Prevent objects from being collected:

```c
/* Pin object (won't be collected even if unreachable) */
gc_pin_object(ptr);

/* Do critical work */
critical_operation(ptr);

/* Unpin when done */
gc_unpin_object(ptr);
```

## Performance Tuning

### For Throughput

```c
GCConfig config = gc_get_default_config();
config.young_threshold = 10 * 1024 * 1024;    /* Larger threshold */
config.auto_gc_enabled = false;               /* Manual collection */
mm_set_gc_config(mm, &config);

/* Collect at controlled points */
process_large_batch();
mm_gc_collect_full(mm);
```

### For Low Latency

```c
GCConfig config = gc_get_default_config();
config.incremental_enabled = true;
config.incremental_step_size = 50;            /* Smaller steps */
config.auto_gc_enabled = true;
mm_set_gc_config(mm, &config);

/* Collections happen automatically in small increments */
```

### For Memory Efficiency

```c
GCConfig config = gc_get_default_config();
config.young_threshold = 512 * 1024;          /* Smaller threshold */
config.gc_pressure_threshold = 0.70;          /* Collect earlier */
config.promotion_age = 2;                     /* Promote faster */
mm_set_gc_config(mm, &config);
```

## See Also

- [KLang Runtime Documentation](../runtime/README.md)
- [KLang Interpreter Guide](../interpreter/README.md)
- [Memory Safety in KLang](MEMORY_SAFETY.md)
- [Performance Optimization](PERFORMANCE.md)
