#include "runtime/memory/memory_pool.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Helper: Create a new pool block */
static PoolBlock *pool_block_new(size_t object_size) {
    PoolBlock *block = calloc(1, sizeof(PoolBlock));
    if (!block) return NULL;
    
    block->memory = calloc(POOL_BLOCK_COUNT, object_size);
    if (!block->memory) {
        free(block);
        return NULL;
    }
    
    /* Allocate bitmap (1 bit per object, round up to bytes) */
    size_t bitmap_size = (POOL_BLOCK_COUNT + 7) / 8;
    block->free_bitmap = calloc(1, bitmap_size);
    if (!block->free_bitmap) {
        free(block->memory);
        free(block);
        return NULL;
    }
    
    /* Initially all slots are free (bitmap all 1s) */
    memset(block->free_bitmap, 0xFF, bitmap_size);
    block->free_count = POOL_BLOCK_COUNT;
    block->next = NULL;
    
    return block;
}

/* Helper: Free a pool block */
static void pool_block_free(PoolBlock *block) {
    if (!block) return;
    free(block->memory);
    free(block->free_bitmap);
    free(block);
}

/* Helper: Check if a slot is free */
static int is_slot_free(PoolBlock *block, int index) {
    int byte_index = index / 8;
    int bit_index = index % 8;
    return (block->free_bitmap[byte_index] & (1 << bit_index)) != 0;
}

/* Helper: Mark a slot as used */
static void mark_slot_used(PoolBlock *block, int index) {
    int byte_index = index / 8;
    int bit_index = index % 8;
    block->free_bitmap[byte_index] &= ~(1 << bit_index);
    block->free_count--;
}

/* Helper: Mark a slot as free */
static void mark_slot_free(PoolBlock *block, int index) {
    int byte_index = index / 8;
    int bit_index = index % 8;
    block->free_bitmap[byte_index] |= (1 << bit_index);
    block->free_count++;
}

/* Create a new memory pool */
MemoryPool *pool_new(size_t object_size) {
    MemoryPool *pool = calloc(1, sizeof(MemoryPool));
    if (!pool) return NULL;
    
    pool->object_size = object_size;
    pool->blocks = NULL;
    pool->total_allocated = 0;
    pool->total_objects = 0;
    pool->free_objects = 0;
    
    return pool;
}

/* Free a memory pool */
void pool_free(MemoryPool *pool) {
    if (!pool) return;
    
    PoolBlock *block = pool->blocks;
    while (block) {
        PoolBlock *next = block->next;
        pool_block_free(block);
        block = next;
    }
    
    free(pool);
}

/* Allocate an object from the pool */
void *pool_alloc(MemoryPool *pool) {
    if (!pool) return NULL;
    
    /* Find a block with free space */
    PoolBlock *block = pool->blocks;
    while (block && block->free_count == 0) {
        block = block->next;
    }
    
    /* No free space, allocate new block */
    if (!block) {
        block = pool_block_new(pool->object_size);
        if (!block) return NULL;
        
        block->next = pool->blocks;
        pool->blocks = block;
        pool->total_allocated += POOL_BLOCK_COUNT * pool->object_size;
    }
    
    /* Find first free slot in the block */
    for (int i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (is_slot_free(block, i)) {
            mark_slot_used(block, i);
            pool->total_objects++;
            void *ptr = (char *)block->memory + (i * pool->object_size);
            memset(ptr, 0, pool->object_size);
            return ptr;
        }
    }
    
    /* Should never reach here */
    return NULL;
}

/* Deallocate an object back to the pool */
void pool_dealloc(MemoryPool *pool, void *ptr) {
    if (!pool || !ptr) return;
    
    /* Find which block contains this pointer */
    PoolBlock *block = pool->blocks;
    while (block) {
        char *block_start = (char *)block->memory;
        char *block_end = block_start + (POOL_BLOCK_COUNT * pool->object_size);
        
        if ((char *)ptr >= block_start && (char *)ptr < block_end) {
            /* Found the block, calculate slot index */
            size_t offset = (char *)ptr - block_start;
            int index = offset / pool->object_size;
            
            if (index >= 0 && index < POOL_BLOCK_COUNT) {
                mark_slot_free(block, index);
                pool->total_objects--;
                pool->free_objects++;
                return;
            }
        }
        
        block = block->next;
    }
    
    /* Pointer not found in any block - this is an error */
    fprintf(stderr, "Warning: pool_dealloc called with invalid pointer\n");
}

/* Get pool statistics */
void pool_get_stats(MemoryPool *pool, size_t *total_bytes, size_t *free_bytes) {
    if (!pool) return;
    
    if (total_bytes) {
        *total_bytes = pool->total_allocated;
    }
    
    if (free_bytes) {
        size_t free_objects = 0;
        PoolBlock *block = pool->blocks;
        while (block) {
            free_objects += block->free_count;
            block = block->next;
        }
        *free_bytes = free_objects * pool->object_size;
    }
}

/* Create a new pool manager */
MemoryPoolManager *pool_manager_new(void) {
    MemoryPoolManager *manager = calloc(1, sizeof(MemoryPoolManager));
    if (!manager) return NULL;
    
    manager->small_pool = pool_new(SMALL_POOL_SIZE);
    manager->medium_pool = pool_new(MEDIUM_POOL_SIZE);
    manager->large_pool = pool_new(LARGE_POOL_SIZE);
    
    if (!manager->small_pool || !manager->medium_pool || !manager->large_pool) {
        pool_manager_free(manager);
        return NULL;
    }
    
    return manager;
}

/* Free the pool manager */
void pool_manager_free(MemoryPoolManager *manager) {
    if (!manager) return;
    
    pool_free(manager->small_pool);
    pool_free(manager->medium_pool);
    pool_free(manager->large_pool);
    
    free(manager);
}
