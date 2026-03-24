#ifndef PARALLEL_H
#define PARALLEL_H

#include <pthread.h>
#include <stdatomic.h>
#include "interpreter.h"

/* Thread pool configuration */
#define DEFAULT_THREAD_COUNT 4
#define MAX_THREAD_COUNT 64
#define WORK_QUEUE_SIZE 1024

/* Work item */
typedef struct WorkItem {
    void (*function)(void *arg);
    void *arg;
    struct WorkItem *next;
} WorkItem;

/* Thread pool */
typedef struct {
    pthread_t *threads;
    int thread_count;
    int shutdown;
    
    /* Work queue */
    WorkItem *queue_head;
    WorkItem *queue_tail;
    int queue_size;
    
    /* Synchronization */
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_cond;
    
    /* Statistics */
    atomic_int tasks_completed;
    atomic_int tasks_submitted;
} ThreadPool;

/* Parallel task result */
typedef struct {
    Value result;
    int completed;
    int error;
    char *error_message;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} ParallelTaskResult;

/* Parallel context */
typedef struct {
    ThreadPool *pool;
    Interpreter *interp_template;  /* Template for creating worker interpreters */
} ParallelContext;

/* Thread pool operations */
ThreadPool* thread_pool_create(int thread_count);
void thread_pool_destroy(ThreadPool *pool);
int thread_pool_submit(ThreadPool *pool, void (*function)(void *), void *arg);
void thread_pool_wait(ThreadPool *pool);

/* Parallel execution */
ParallelContext* parallel_context_create(Interpreter *interp, int thread_count);
void parallel_context_destroy(ParallelContext *ctx);

/* Parallel primitives */
Value parallel_for(ParallelContext *ctx, int start, int end, Value (*body)(int, void*), void *user_data);
Value parallel_map(ParallelContext *ctx, Value *array, int count, Value (*mapper)(Value, void*), void *user_data);
Value parallel_reduce(ParallelContext *ctx, Value *array, int count, Value (*reducer)(Value, Value, void*), Value init, void *user_data);
Value parallel_filter(ParallelContext *ctx, Value *array, int count, int (*predicate)(Value, void*), void *user_data);

/* Async task execution */
ParallelTaskResult* parallel_async(ParallelContext *ctx, Value (*task)(void*), void *arg);
Value parallel_await(ParallelTaskResult *result);
int parallel_is_ready(ParallelTaskResult *result);

/* Atomic operations */
typedef struct {
    atomic_int value;
} AtomicInt;

typedef struct {
    atomic_llong value;
} AtomicLong;

AtomicInt* atomic_int_create(int initial);
int atomic_int_get(AtomicInt *a);
void atomic_int_set(AtomicInt *a, int value);
int atomic_int_add(AtomicInt *a, int delta);
int atomic_int_sub(AtomicInt *a, int delta);
int atomic_int_compare_exchange(AtomicInt *a, int expected, int desired);
void atomic_int_destroy(AtomicInt *a);

/* Synchronization primitives */
typedef struct {
    pthread_mutex_t mutex;
} Mutex;

typedef struct {
    pthread_rwlock_t rwlock;
} RWLock;

typedef struct {
    pthread_barrier_t barrier;
} Barrier;

Mutex* mutex_create(void);
void mutex_lock(Mutex *m);
void mutex_unlock(Mutex *m);
int mutex_trylock(Mutex *m);
void mutex_destroy(Mutex *m);

RWLock* rwlock_create(void);
void rwlock_read_lock(RWLock *rw);
void rwlock_write_lock(RWLock *rw);
void rwlock_unlock(RWLock *rw);
void rwlock_destroy(RWLock *rw);

Barrier* barrier_create(int count);
void barrier_wait(Barrier *b);
void barrier_destroy(Barrier *b);

/* Channel for communication */
typedef struct {
    void **buffer;
    int capacity;
    int size;
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    int closed;
} Channel;

Channel* channel_create(int capacity);
int channel_send(Channel *ch, void *data);
void* channel_receive(Channel *ch);
int channel_try_receive(Channel *ch, void **data);
void channel_close(Channel *ch);
void channel_destroy(Channel *ch);

#endif /* PARALLEL_H */
