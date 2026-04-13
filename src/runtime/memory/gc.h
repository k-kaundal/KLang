#ifndef GC_H
#define GC_H

#include <stddef.h>

/* Legacy GC - Deprecated, use gc_enhanced.h for new code */

typedef struct GCObject {
    int marked;
    struct GCObject *next;
    size_t size;
} GCObject;

typedef struct {
    GCObject *head;
    size_t total_allocated;
} GCHeap;

/* Legacy GC functions - use gc_enhanced.h for new code */
GCHeap *gc_heap_new_legacy(void);
void gc_heap_free_legacy(GCHeap *heap);
void *gc_alloc_legacy(GCHeap *heap, size_t size);
void gc_mark_object_legacy(GCObject *obj);
void gc_sweep_legacy(GCHeap *heap);
void gc_collect_legacy(GCHeap *heap);
void gc_free_all_legacy(GCHeap *heap);

#endif
