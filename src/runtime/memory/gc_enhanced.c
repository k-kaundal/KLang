#include "runtime/memory/gc_enhanced.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

/* Get current time in microseconds */
static uint64_t get_time_us(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

/* Get GCObject from user pointer */
GCObject *gc_object_from_ptr(void *ptr) {
    if (!ptr) return NULL;
    return (GCObject *)((char *)ptr - sizeof(GCObject));
}

/* Get user pointer from GCObject */
void *gc_ptr_from_object(GCObject *obj) {
    if (!obj) return NULL;
    return (char *)obj + sizeof(GCObject);
}

/* Remove object from its generation list */
static void remove_from_list(GCObject *obj, GCObject **head) {
    if (obj->prev) {
        obj->prev->next = obj->next;
    } else {
        *head = obj->next;
    }
    
    if (obj->next) {
        obj->next->prev = obj->prev;
    }
    
    obj->prev = NULL;
    obj->next = NULL;
}

/* Add object to generation list */
static void add_to_list(GCObject *obj, GCObject **head) {
    obj->next = *head;
    obj->prev = NULL;
    
    if (*head) {
        (*head)->prev = obj;
    }
    
    *head = obj;
}

/* Promote object from young to old generation */
static void promote_to_old(GCHeap *heap, GCObject *obj) {
    if (obj->generation == GC_GEN_OLD) return;
    
    remove_from_list(obj, &heap->young_gen);
    obj->generation = GC_GEN_OLD;
    add_to_list(obj, &heap->old_gen);
    
    heap->stats.young_objects--;
    heap->stats.old_objects++;
}

/* Create default GC configuration */
GCConfig gc_get_default_config(void) {
    GCConfig config;
    
    config.young_threshold = 1024 * 1024;       /* 1MB */
    config.heap_threshold = 10 * 1024 * 1024;   /* 10MB */
    config.max_heap_size = 0;                   /* Unlimited */
    config.promotion_age = 3;                   /* Promote after 3 collections */
    config.incremental_enabled = true;
    config.incremental_step_size = 100;         /* Mark 100 objects per step */
    config.auto_gc_enabled = true;
    config.gc_pressure_threshold = 0.75;        /* 75% memory usage */
    
    return config;
}

/* Create new GC heap */
GCHeap *gc_heap_new(void) {
    GCHeap *heap = calloc(1, sizeof(GCHeap));
    if (!heap) return NULL;
    
    heap->young_gen = NULL;
    heap->old_gen = NULL;
    heap->gray_list = NULL;
    
    heap->config = gc_get_default_config();
    memset(&heap->stats, 0, sizeof(GCStats));
    
    heap->gc_in_progress = false;
    heap->gc_marking_phase = false;
    
    heap->roots = NULL;
    heap->root_count = 0;
    heap->root_capacity = 0;
    
    return heap;
}

/* Free entire GC heap */
void gc_heap_free(GCHeap *heap) {
    if (!heap) return;
    
    gc_free_all(heap);
    
    if (heap->roots) {
        free(heap->roots);
    }
    
    free(heap);
}

/* Allocate object with GC */
void *gc_alloc(GCHeap *heap, size_t size) {
    return gc_alloc_with_finalizer(heap, size, NULL);
}

/* Allocate object with custom mark function */
void *gc_alloc_with_finalizer(GCHeap *heap, size_t size, void (*mark_fn)(GCObject *, void *)) {
    if (!heap) return NULL;
    
    /* Check if we should trigger GC */
    if (heap->config.auto_gc_enabled && gc_should_collect(heap)) {
        gc_collect_young(heap);
    }
    
    /* Allocate object with header */
    GCObject *obj = calloc(1, sizeof(GCObject) + size);
    if (!obj) return NULL;
    
    /* Initialize header */
    obj->size = size;
    obj->ref_count = 1;
    obj->generation = GC_GEN_YOUNG;
    obj->color = GC_WHITE;
    obj->age = 0;
    obj->pinned = 0;
    obj->mark_fn = mark_fn;
    
    /* Add to young generation */
    add_to_list(obj, &heap->young_gen);
    
    /* Update statistics */
    heap->stats.total_allocated += size;
    heap->stats.current_usage += size + sizeof(GCObject);
    heap->stats.young_objects++;
    heap->stats.total_objects++;
    
    if (heap->stats.current_usage > heap->stats.peak_usage) {
        heap->stats.peak_usage = heap->stats.current_usage;
    }
    
    return gc_ptr_from_object(obj);
}

/* Reference counting - increment */
void gc_retain(void *ptr) {
    if (!ptr) return;
    
    GCObject *obj = gc_object_from_ptr(ptr);
    obj->ref_count++;
}

/* Reference counting - decrement and potentially free */
void gc_release(GCHeap *heap, void *ptr) {
    if (!ptr || !heap) return;
    
    GCObject *obj = gc_object_from_ptr(ptr);
    obj->ref_count--;
    
    /* If ref count reaches 0, immediately free */
    if (obj->ref_count <= 0) {
        /* Remove from generation list */
        if (obj->generation == GC_GEN_YOUNG) {
            remove_from_list(obj, &heap->young_gen);
            heap->stats.young_objects--;
        } else {
            remove_from_list(obj, &heap->old_gen);
            heap->stats.old_objects--;
        }
        
        /* Update statistics */
        heap->stats.total_freed += obj->size;
        heap->stats.current_usage -= obj->size + sizeof(GCObject);
        heap->stats.total_objects--;
        heap->stats.objects_collected++;
        heap->stats.bytes_collected += obj->size;
        
        free(obj);
    }
}

/* Get reference count */
int gc_get_ref_count(void *ptr) {
    if (!ptr) return 0;
    GCObject *obj = gc_object_from_ptr(ptr);
    return obj->ref_count;
}

/* Mark phase - mark object and its children */
static void mark_object(GCHeap *heap, GCObject *obj) {
    if (!obj || obj->color != GC_WHITE) return;
    
    /* Mark as gray (visited but not scanned) */
    obj->color = GC_GRAY;
    
    /* Add to gray list for scanning */
    obj->next = heap->gray_list;
    heap->gray_list = obj;
}

/* Scan gray objects and mark their children */
static void scan_gray_objects(GCHeap *heap, size_t max_objects) {
    size_t scanned = 0;
    
    while (heap->gray_list && scanned < max_objects) {
        GCObject *obj = heap->gray_list;
        heap->gray_list = obj->next;
        
        /* Mark as black (fully processed) */
        obj->color = GC_BLACK;
        
        /* If object has custom mark function, call it */
        if (obj->mark_fn) {
            obj->mark_fn(obj, heap);
        }
        
        scanned++;
    }
}

/* Sweep phase - collect white (unmarked) objects */
static void sweep_generation(GCHeap *heap, GCObject **gen_head, GCGeneration gen) {
    GCObject *obj = *gen_head;
    
    while (obj) {
        GCObject *next = obj->next;
        
        if (obj->color == GC_WHITE && !obj->pinned && obj->ref_count <= 0) {
            /* Object is unmarked and not referenced - collect it */
            remove_from_list(obj, gen_head);
            
            heap->stats.total_freed += obj->size;
            heap->stats.current_usage -= obj->size + sizeof(GCObject);
            heap->stats.total_objects--;
            heap->stats.objects_collected++;
            heap->stats.bytes_collected += obj->size;
            
            if (gen == GC_GEN_YOUNG) {
                heap->stats.young_objects--;
            } else {
                heap->stats.old_objects--;
            }
            
            free(obj);
        } else {
            /* Object survived - reset color and increment age */
            obj->color = GC_WHITE;
            obj->age++;
            
            /* Promote to old generation if old enough */
            if (gen == GC_GEN_YOUNG && obj->age >= heap->config.promotion_age) {
                GCObject *temp = obj;
                obj = next;  /* Move to next before promoting (modifies list) */
                promote_to_old(heap, temp);
                continue;
            }
        }
        
        obj = next;
    }
}

/* Mark roots */
static void mark_roots(GCHeap *heap) {
    for (size_t i = 0; i < heap->root_count; i++) {
        if (heap->roots[i]) {
            GCObject *obj = gc_object_from_ptr(heap->roots[i]);
            mark_object(heap, obj);
        }
    }
}

/* Young generation collection */
void gc_collect_young(GCHeap *heap) {
    if (!heap) return;
    
    uint64_t start_time = get_time_us();
    
    /* Mark phase */
    heap->gray_list = NULL;
    mark_roots(heap);
    scan_gray_objects(heap, (size_t)-1);  /* Scan all */
    
    /* Sweep young generation */
    sweep_generation(heap, &heap->young_gen, GC_GEN_YOUNG);
    
    /* Update statistics */
    heap->stats.collections_young++;
    uint64_t elapsed = get_time_us() - start_time;
    heap->stats.total_gc_time += elapsed;
    heap->stats.last_gc_time = elapsed;
}

/* Full heap collection */
void gc_collect_full(GCHeap *heap) {
    if (!heap) return;
    
    uint64_t start_time = get_time_us();
    
    /* Mark phase */
    heap->gray_list = NULL;
    mark_roots(heap);
    scan_gray_objects(heap, (size_t)-1);  /* Scan all */
    
    /* Sweep both generations */
    sweep_generation(heap, &heap->young_gen, GC_GEN_YOUNG);
    sweep_generation(heap, &heap->old_gen, GC_GEN_OLD);
    
    /* Update statistics */
    heap->stats.collections_full++;
    uint64_t elapsed = get_time_us() - start_time;
    heap->stats.total_gc_time += elapsed;
    heap->stats.last_gc_time = elapsed;
}

/* Incremental collection step */
void gc_collect_incremental_step(GCHeap *heap) {
    if (!heap || !heap->config.incremental_enabled) return;
    
    if (!heap->gc_in_progress) {
        /* Start new collection cycle */
        heap->gc_in_progress = true;
        heap->gc_marking_phase = true;
        heap->gray_list = NULL;
        mark_roots(heap);
        heap->stats.incremental_steps = 0;
    }
    
    if (heap->gc_marking_phase) {
        /* Mark phase - process some gray objects */
        scan_gray_objects(heap, heap->config.incremental_step_size);
        heap->stats.incremental_steps++;
        
        /* If no more gray objects, move to sweep phase */
        if (!heap->gray_list) {
            heap->gc_marking_phase = false;
        }
    } else {
        /* Sweep phase - collect some objects */
        /* For simplicity, do full sweep in one step */
        sweep_generation(heap, &heap->young_gen, GC_GEN_YOUNG);
        heap->gc_in_progress = false;
        heap->stats.collections_young++;
    }
}

/* Root set management */
void gc_add_root(GCHeap *heap, void *root) {
    if (!heap || !root) return;
    
    /* Expand root array if needed */
    if (heap->root_count >= heap->root_capacity) {
        size_t new_capacity = heap->root_capacity == 0 ? 16 : heap->root_capacity * 2;
        void **new_roots = realloc(heap->roots, new_capacity * sizeof(void *));
        if (!new_roots) return;
        
        heap->roots = new_roots;
        heap->root_capacity = new_capacity;
    }
    
    heap->roots[heap->root_count++] = root;
}

void gc_remove_root(GCHeap *heap, void *root) {
    if (!heap || !root) return;
    
    for (size_t i = 0; i < heap->root_count; i++) {
        if (heap->roots[i] == root) {
            /* Shift remaining roots */
            memmove(&heap->roots[i], &heap->roots[i + 1], 
                    (heap->root_count - i - 1) * sizeof(void *));
            heap->root_count--;
            return;
        }
    }
}

void gc_clear_roots(GCHeap *heap) {
    if (!heap) return;
    heap->root_count = 0;
}

/* Memory pressure detection */
double gc_get_memory_pressure(GCHeap *heap) {
    if (!heap) return 0.0;
    
    size_t threshold = heap->config.heap_threshold;
    if (threshold == 0) return 0.0;
    
    return (double)heap->stats.current_usage / (double)threshold;
}

bool gc_should_collect(GCHeap *heap) {
    if (!heap || !heap->config.auto_gc_enabled) return false;
    
    /* Check young generation threshold */
    size_t young_bytes = heap->stats.young_objects * 64;  /* Estimate */
    if (young_bytes >= heap->config.young_threshold) {
        return true;
    }
    
    /* Check memory pressure */
    double pressure = gc_get_memory_pressure(heap);
    if (pressure >= heap->config.gc_pressure_threshold) {
        return true;
    }
    
    return false;
}

/* Configuration */
void gc_set_config(GCHeap *heap, GCConfig *config) {
    if (!heap || !config) return;
    heap->config = *config;
}

/* Statistics */
GCStats gc_get_stats(GCHeap *heap) {
    if (!heap) {
        GCStats empty;
        memset(&empty, 0, sizeof(GCStats));
        return empty;
    }
    return heap->stats;
}

void gc_print_stats(GCHeap *heap) {
    if (!heap) return;
    
    GCStats *s = &heap->stats;
    
    printf("\n=== GC Statistics ===\n");
    printf("Memory:\n");
    printf("  Current usage:  %zu bytes\n", s->current_usage);
    printf("  Peak usage:     %zu bytes\n", s->peak_usage);
    printf("  Total allocated: %zu bytes\n", s->total_allocated);
    printf("  Total freed:    %zu bytes\n", s->total_freed);
    printf("\nObjects:\n");
    printf("  Young generation: %zu\n", s->young_objects);
    printf("  Old generation:   %zu\n", s->old_objects);
    printf("  Total objects:    %zu\n", s->total_objects);
    printf("\nCollections:\n");
    printf("  Young collections: %zu\n", s->collections_young);
    printf("  Full collections:  %zu\n", s->collections_full);
    printf("  Objects collected: %zu\n", s->objects_collected);
    printf("  Bytes collected:   %zu\n", s->bytes_collected);
    printf("\nTiming:\n");
    printf("  Total GC time:  %llu μs\n", (unsigned long long)s->total_gc_time);
    printf("  Last GC time:   %llu μs\n", (unsigned long long)s->last_gc_time);
    printf("  Incremental steps: %zu\n", s->incremental_steps);
    printf("====================\n\n");
}

void gc_reset_stats(GCHeap *heap) {
    if (!heap) return;
    memset(&heap->stats, 0, sizeof(GCStats));
}

/* Utilities */
void gc_pin_object(void *ptr) {
    if (!ptr) return;
    GCObject *obj = gc_object_from_ptr(ptr);
    obj->pinned = 1;
}

void gc_unpin_object(void *ptr) {
    if (!ptr) return;
    GCObject *obj = gc_object_from_ptr(ptr);
    obj->pinned = 0;
}

/* Free all objects */
void gc_free_all(GCHeap *heap) {
    if (!heap) return;
    
    /* Free young generation */
    GCObject *obj = heap->young_gen;
    while (obj) {
        GCObject *next = obj->next;
        free(obj);
        obj = next;
    }
    heap->young_gen = NULL;
    
    /* Free old generation */
    obj = heap->old_gen;
    while (obj) {
        GCObject *next = obj->next;
        free(obj);
        obj = next;
    }
    heap->old_gen = NULL;
    
    /* Reset statistics */
    heap->stats.current_usage = 0;
    heap->stats.young_objects = 0;
    heap->stats.old_objects = 0;
    heap->stats.total_objects = 0;
}
