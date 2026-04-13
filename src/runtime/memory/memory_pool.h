#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stddef.h>
#include <stdint.h>

/**
 * Memory Pool for Object Allocation
 * 
 * Provides fast allocation for commonly-sized objects to reduce fragmentation
 * and improve performance by avoiding frequent malloc/free calls.
 */

/* Pool sizes for common object types */
#define SMALL_POOL_SIZE     32      /* For small objects (strings, small arrays) */
#define MEDIUM_POOL_SIZE    128     /* For medium objects (functions, closures) */
#define LARGE_POOL_SIZE     512     /* For large objects (dicts, sets) */

/* Number of objects per pool block */
#define POOL_BLOCK_COUNT    256

typedef struct PoolBlock {
    void *memory;                   /* Actual memory block */
    uint8_t *free_bitmap;           /* Bitmap of free slots */
    int free_count;                 /* Number of free slots */
    struct PoolBlock *next;         /* Next block in chain */
} PoolBlock;

typedef struct MemoryPool {
    size_t object_size;             /* Size of objects in this pool */
    PoolBlock *blocks;              /* Linked list of memory blocks */
    size_t total_allocated;         /* Total bytes allocated */
    size_t total_objects;           /* Total number of objects */
    size_t free_objects;            /* Number of free objects */
} MemoryPool;

typedef struct MemoryPoolManager {
    MemoryPool *small_pool;         /* Pool for small objects */
    MemoryPool *medium_pool;        /* Pool for medium objects */
    MemoryPool *large_pool;         /* Pool for large objects */
} MemoryPoolManager;

/* Pool management functions */
MemoryPoolManager *pool_manager_new(void);
void pool_manager_free(MemoryPoolManager *manager);

MemoryPool *pool_new(size_t object_size);
void pool_free(MemoryPool *pool);

void *pool_alloc(MemoryPool *pool);
void pool_dealloc(MemoryPool *pool, void *ptr);

/* Pool statistics */
void pool_get_stats(MemoryPool *pool, size_t *total_bytes, size_t *free_bytes);

#endif /* MEMORY_POOL_H */
