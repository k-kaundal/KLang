#include "gc.h"
#include <stdlib.h>
#include <string.h>

GCHeap *gc_heap_new(void) {
    GCHeap *heap = calloc(1, sizeof(GCHeap));
    return heap;
}

void gc_heap_free(GCHeap *heap) {
    gc_free_all(heap);
    free(heap);
}

void *gc_alloc(GCHeap *heap, size_t size) {
    GCObject *obj = calloc(1, sizeof(GCObject) + size);
    obj->marked = 0;
    obj->size = size;
    obj->next = heap->head;
    heap->head = obj;
    heap->total_allocated += size;
    return (char *)obj + sizeof(GCObject);
}

void gc_mark_object(GCObject *obj) {
    if (obj) obj->marked = 1;
}

void gc_sweep(GCHeap *heap) {
    GCObject **cur = &heap->head;
    while (*cur) {
        if (!(*cur)->marked) {
            GCObject *unreached = *cur;
            *cur = unreached->next;
            heap->total_allocated -= unreached->size;
            free(unreached);
        } else {
            (*cur)->marked = 0;
            cur = &(*cur)->next;
        }
    }
}

void gc_collect(GCHeap *heap) {
    gc_sweep(heap);
}

void gc_free_all(GCHeap *heap) {
    GCObject *cur = heap->head;
    while (cur) {
        GCObject *next = cur->next;
        heap->total_allocated -= cur->size;
        free(cur);
        cur = next;
    }
    heap->head = NULL;
}
