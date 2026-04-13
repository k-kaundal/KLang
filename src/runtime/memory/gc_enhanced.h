#ifndef GC_ENHANCED_H
#define GC_ENHANCED_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Enhanced Garbage Collector for KLang
 * 
 * Features:
 * - Generational garbage collection (young/old generations)
 * - Reference counting for immediate cleanup
 * - Incremental collection to reduce pause times
 * - Memory pressure detection
 * - Detailed statistics tracking
 */

/* GC Object Header */
typedef struct GCObject GCObject;

/* Object generation (for generational GC) */
typedef enum {
    GC_GEN_YOUNG = 0,       /* Young generation (newly allocated) */
    GC_GEN_OLD = 1          /* Old generation (survived collections) */
} GCGeneration;

/* Object color for tri-color marking */
typedef enum {
    GC_WHITE = 0,           /* Not visited (unmarked) */
    GC_GRAY = 1,            /* Visited but children not processed */
    GC_BLACK = 2            /* Visited and children processed */
} GCColor;

struct GCObject {
    /* Object metadata */
    size_t size;            /* Size of the object in bytes */
    int ref_count;          /* Reference count */
    uint8_t generation;     /* Generation (young or old) */
    uint8_t color;          /* Color for marking (white/gray/black) */
    uint8_t age;            /* Number of GC cycles survived */
    uint8_t pinned;         /* If 1, object cannot be collected */
    
    /* Linked list pointers */
    GCObject *next;         /* Next object in generation */
    GCObject *prev;         /* Previous object in generation */
    
    /* Mark function callback for custom types */
    void (*mark_fn)(GCObject *obj, void *context);
    
    /* User data follows this header */
};

/* GC Statistics */
typedef struct {
    /* Memory usage */
    size_t total_allocated;         /* Total bytes allocated */
    size_t total_freed;             /* Total bytes freed */
    size_t current_usage;           /* Current memory usage */
    size_t peak_usage;              /* Peak memory usage */
    
    /* Object counts */
    size_t young_objects;           /* Objects in young generation */
    size_t old_objects;             /* Objects in old generation */
    size_t total_objects;           /* Total objects */
    
    /* Collection statistics */
    size_t collections_young;       /* Number of young collections */
    size_t collections_full;        /* Number of full collections */
    size_t objects_collected;       /* Total objects collected */
    size_t bytes_collected;         /* Total bytes collected */
    
    /* Timing (in microseconds) */
    uint64_t total_gc_time;         /* Total time spent in GC */
    uint64_t last_gc_time;          /* Time of last GC */
    
    /* Incremental GC */
    size_t incremental_steps;       /* Number of incremental steps */
} GCStats;

/* GC Configuration */
typedef struct {
    /* Memory thresholds */
    size_t young_threshold;         /* Bytes before young collection */
    size_t heap_threshold;          /* Bytes before full collection */
    size_t max_heap_size;           /* Maximum heap size (0 = unlimited) */
    
    /* Generational settings */
    int promotion_age;              /* Age before promoting to old gen */
    
    /* Incremental settings */
    bool incremental_enabled;       /* Enable incremental collection */
    size_t incremental_step_size;   /* Objects to mark per step */
    
    /* Automatic GC triggers */
    bool auto_gc_enabled;           /* Enable automatic GC */
    double gc_pressure_threshold;   /* Pressure ratio (0.0-1.0) */
} GCConfig;

/* GC Heap */
typedef struct {
    /* Object lists by generation */
    GCObject *young_gen;            /* Young generation objects */
    GCObject *old_gen;              /* Old generation objects */
    
    /* Gray set for incremental marking */
    GCObject *gray_list;            /* Objects to be scanned */
    
    /* Configuration and statistics */
    GCConfig config;
    GCStats stats;
    
    /* Incremental GC state */
    bool gc_in_progress;            /* Is GC currently running? */
    bool gc_marking_phase;          /* In marking phase? */
    
    /* Root set tracking */
    void **roots;                   /* Array of root pointers */
    size_t root_count;
    size_t root_capacity;
    
} GCHeap;

/* Core GC functions */
GCHeap *gc_heap_new(void);
void gc_heap_free(GCHeap *heap);

/* Allocation */
void *gc_alloc(GCHeap *heap, size_t size);
void *gc_alloc_with_finalizer(GCHeap *heap, size_t size, void (*mark_fn)(GCObject *, void *));

/* Reference counting */
void gc_retain(void *ptr);
void gc_release(GCHeap *heap, void *ptr);
int gc_get_ref_count(void *ptr);

/* Manual collection triggers */
void gc_collect_young(GCHeap *heap);
void gc_collect_full(GCHeap *heap);
void gc_collect_incremental_step(GCHeap *heap);

/* Root set management */
void gc_add_root(GCHeap *heap, void *root);
void gc_remove_root(GCHeap *heap, void *root);
void gc_clear_roots(GCHeap *heap);

/* Memory pressure detection */
double gc_get_memory_pressure(GCHeap *heap);
bool gc_should_collect(GCHeap *heap);

/* Configuration */
void gc_set_config(GCHeap *heap, GCConfig *config);
GCConfig gc_get_default_config(void);

/* Statistics */
GCStats gc_get_stats(GCHeap *heap);
void gc_print_stats(GCHeap *heap);
void gc_reset_stats(GCHeap *heap);

/* Utilities */
GCObject *gc_object_from_ptr(void *ptr);
void *gc_ptr_from_object(GCObject *obj);
void gc_pin_object(void *ptr);
void gc_unpin_object(void *ptr);

/* Free all objects (for shutdown) */
void gc_free_all(GCHeap *heap);

#endif /* GC_ENHANCED_H */
