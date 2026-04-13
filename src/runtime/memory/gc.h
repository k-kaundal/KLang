#ifndef GC_H
#define GC_H

#include <stddef.h>

typedef struct GCObject {
    int marked;
    struct GCObject *next;
    size_t size;
} GCObject;

typedef struct {
    GCObject *head;
    size_t total_allocated;
} GCHeap;

GCHeap *gc_heap_new(void);
void gc_heap_free(GCHeap *heap);
void *gc_alloc(GCHeap *heap, size_t size);
void gc_mark_object(GCObject *obj);
void gc_sweep(GCHeap *heap);
void gc_collect(GCHeap *heap);
void gc_free_all(GCHeap *heap);

#endif
