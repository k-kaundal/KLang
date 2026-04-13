#include "parallel.h"
#include <stdlib.h>
#include <string.h>
#include <sched.h>

/* Thread pool worker function */
static void* thread_pool_worker(void *arg) {
    ThreadPool *pool = (ThreadPool*)arg;
    
    while (1) {
        WorkItem *work = NULL;
        
        /* Get work from queue */
        pthread_mutex_lock(&pool->queue_mutex);
        
        while (pool->queue_size == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->queue_cond, &pool->queue_mutex);
        }
        
        if (pool->shutdown && pool->queue_size == 0) {
            pthread_mutex_unlock(&pool->queue_mutex);
            break;
        }
        
        /* Dequeue work */
        work = pool->queue_head;
        if (work) {
            pool->queue_head = work->next;
            if (pool->queue_head == NULL) {
                pool->queue_tail = NULL;
            }
            pool->queue_size--;
        }
        
        pthread_mutex_unlock(&pool->queue_mutex);
        
        /* Execute work */
        if (work) {
            work->function(work->arg);
            free(work);
            atomic_fetch_add(&pool->tasks_completed, 1);
        }
    }
    
    return NULL;
}

/* Create thread pool */
ThreadPool* thread_pool_create(int thread_count) {
    if (thread_count <= 0) thread_count = DEFAULT_THREAD_COUNT;
    if (thread_count > MAX_THREAD_COUNT) thread_count = MAX_THREAD_COUNT;
    
    ThreadPool *pool = malloc(sizeof(ThreadPool));
    pool->thread_count = thread_count;
    pool->shutdown = 0;
    pool->queue_head = NULL;
    pool->queue_tail = NULL;
    pool->queue_size = 0;
    
    atomic_init(&pool->tasks_completed, 0);
    atomic_init(&pool->tasks_submitted, 0);
    
    pthread_mutex_init(&pool->queue_mutex, NULL);
    pthread_cond_init(&pool->queue_cond, NULL);
    
    /* Create threads */
    pool->threads = malloc(thread_count * sizeof(pthread_t));
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&pool->threads[i], NULL, thread_pool_worker, pool);
    }
    
    return pool;
}

/* Destroy thread pool */
void thread_pool_destroy(ThreadPool *pool) {
    if (!pool) return;
    
    /* Signal shutdown */
    pthread_mutex_lock(&pool->queue_mutex);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->queue_cond);
    pthread_mutex_unlock(&pool->queue_mutex);
    
    /* Wait for threads */
    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    
    /* Cleanup */
    free(pool->threads);
    
    /* Free remaining work items */
    WorkItem *work = pool->queue_head;
    while (work) {
        WorkItem *next = work->next;
        free(work);
        work = next;
    }
    
    pthread_mutex_destroy(&pool->queue_mutex);
    pthread_cond_destroy(&pool->queue_cond);
    
    free(pool);
}

/* Submit work to thread pool */
int thread_pool_submit(ThreadPool *pool, void (*function)(void *), void *arg) {
    WorkItem *work = malloc(sizeof(WorkItem));
    work->function = function;
    work->arg = arg;
    work->next = NULL;
    
    pthread_mutex_lock(&pool->queue_mutex);
    
    if (pool->shutdown) {
        pthread_mutex_unlock(&pool->queue_mutex);
        free(work);
        return -1;
    }
    
    /* Add to queue */
    if (pool->queue_tail) {
        pool->queue_tail->next = work;
    } else {
        pool->queue_head = work;
    }
    pool->queue_tail = work;
    pool->queue_size++;
    
    atomic_fetch_add(&pool->tasks_submitted, 1);
    
    pthread_cond_signal(&pool->queue_cond);
    pthread_mutex_unlock(&pool->queue_mutex);
    
    return 0;
}

/* Wait for all tasks to complete */
void thread_pool_wait(ThreadPool *pool) {
    while (atomic_load(&pool->tasks_completed) < atomic_load(&pool->tasks_submitted)) {
        /* Busy wait - could be improved with condition variable */
        sched_yield();
    }
}

/* Create parallel context */
ParallelContext* parallel_context_create(Interpreter *interp, int thread_count) {
    ParallelContext *ctx = malloc(sizeof(ParallelContext));
    ctx->pool = thread_pool_create(thread_count);
    ctx->interp_template = interp;
    return ctx;
}

/* Destroy parallel context */
void parallel_context_destroy(ParallelContext *ctx) {
    if (!ctx) return;
    thread_pool_destroy(ctx->pool);
    free(ctx);
}

/* Atomic operations */
AtomicInt* atomic_int_create(int initial) {
    AtomicInt *a = malloc(sizeof(AtomicInt));
    atomic_init(&a->value, initial);
    return a;
}

int atomic_int_get(AtomicInt *a) {
    return atomic_load(&a->value);
}

void atomic_int_set(AtomicInt *a, int value) {
    atomic_store(&a->value, value);
}

int atomic_int_add(AtomicInt *a, int delta) {
    return atomic_fetch_add(&a->value, delta) + delta;
}

int atomic_int_sub(AtomicInt *a, int delta) {
    return atomic_fetch_sub(&a->value, delta) - delta;
}

int atomic_int_compare_exchange(AtomicInt *a, int expected, int desired) {
    return atomic_compare_exchange_strong(&a->value, &expected, desired);
}

void atomic_int_destroy(AtomicInt *a) {
    free(a);
}

/* Mutex operations */
Mutex* mutex_create(void) {
    Mutex *m = malloc(sizeof(Mutex));
    pthread_mutex_init(&m->mutex, NULL);
    return m;
}

void mutex_lock(Mutex *m) {
    pthread_mutex_lock(&m->mutex);
}

void mutex_unlock(Mutex *m) {
    pthread_mutex_unlock(&m->mutex);
}

int mutex_trylock(Mutex *m) {
    return pthread_mutex_trylock(&m->mutex) == 0;
}

void mutex_destroy(Mutex *m) {
    pthread_mutex_destroy(&m->mutex);
    free(m);
}

/* RWLock operations */
RWLock* rwlock_create(void) {
    RWLock *rw = malloc(sizeof(RWLock));
    pthread_rwlock_init(&rw->rwlock, NULL);
    return rw;
}

void rwlock_read_lock(RWLock *rw) {
    pthread_rwlock_rdlock(&rw->rwlock);
}

void rwlock_write_lock(RWLock *rw) {
    pthread_rwlock_wrlock(&rw->rwlock);
}

void rwlock_unlock(RWLock *rw) {
    pthread_rwlock_unlock(&rw->rwlock);
}

void rwlock_destroy(RWLock *rw) {
    pthread_rwlock_destroy(&rw->rwlock);
    free(rw);
}

/* Barrier operations */
Barrier* barrier_create(int count) {
    Barrier *b = malloc(sizeof(Barrier));
#ifdef __APPLE__
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cond, NULL);
    b->count = 0;
    b->threshold = count;
    b->generation = 0;
#else
    pthread_barrier_init(&b->barrier, NULL, count);
#endif
    return b;
}

void barrier_wait(Barrier *b) {
#ifdef __APPLE__
    pthread_mutex_lock(&b->mutex);
    int my_generation = b->generation;
    b->count++;
    
    if (b->count >= b->threshold) {
        b->generation++;
        b->count = 0;
        pthread_cond_broadcast(&b->cond);
        pthread_mutex_unlock(&b->mutex);
    } else {
        while (my_generation == b->generation) {
            pthread_cond_wait(&b->cond, &b->mutex);
        }
        pthread_mutex_unlock(&b->mutex);
    }
#else
    pthread_barrier_wait(&b->barrier);
#endif
}

void barrier_destroy(Barrier *b) {
#ifdef __APPLE__
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->cond);
#else
    pthread_barrier_destroy(&b->barrier);
#endif
    free(b);
}

/* Channel operations */
Channel* channel_create(int capacity) {
    Channel *ch = malloc(sizeof(Channel));
    ch->capacity = capacity;
    ch->buffer = malloc(capacity * sizeof(void*));
    ch->size = 0;
    ch->head = 0;
    ch->tail = 0;
    ch->closed = 0;
    
    pthread_mutex_init(&ch->mutex, NULL);
    pthread_cond_init(&ch->not_empty, NULL);
    pthread_cond_init(&ch->not_full, NULL);
    
    return ch;
}

int channel_send(Channel *ch, void *data) {
    pthread_mutex_lock(&ch->mutex);
    
    if (ch->closed) {
        pthread_mutex_unlock(&ch->mutex);
        return -1;
    }
    
    while (ch->size == ch->capacity) {
        pthread_cond_wait(&ch->not_full, &ch->mutex);
    }
    
    ch->buffer[ch->tail] = data;
    ch->tail = (ch->tail + 1) % ch->capacity;
    ch->size++;
    
    pthread_cond_signal(&ch->not_empty);
    pthread_mutex_unlock(&ch->mutex);
    
    return 0;
}

void* channel_receive(Channel *ch) {
    pthread_mutex_lock(&ch->mutex);
    
    while (ch->size == 0 && !ch->closed) {
        pthread_cond_wait(&ch->not_empty, &ch->mutex);
    }
    
    if (ch->size == 0 && ch->closed) {
        pthread_mutex_unlock(&ch->mutex);
        return NULL;
    }
    
    void *data = ch->buffer[ch->head];
    ch->head = (ch->head + 1) % ch->capacity;
    ch->size--;
    
    pthread_cond_signal(&ch->not_full);
    pthread_mutex_unlock(&ch->mutex);
    
    return data;
}

int channel_try_receive(Channel *ch, void **data) {
    pthread_mutex_lock(&ch->mutex);
    
    if (ch->size == 0) {
        pthread_mutex_unlock(&ch->mutex);
        return 0;
    }
    
    *data = ch->buffer[ch->head];
    ch->head = (ch->head + 1) % ch->capacity;
    ch->size--;
    
    pthread_cond_signal(&ch->not_full);
    pthread_mutex_unlock(&ch->mutex);
    
    return 1;
}

void channel_close(Channel *ch) {
    pthread_mutex_lock(&ch->mutex);
    ch->closed = 1;
    pthread_cond_broadcast(&ch->not_empty);
    pthread_cond_broadcast(&ch->not_full);
    pthread_mutex_unlock(&ch->mutex);
}

void channel_destroy(Channel *ch) {
    free(ch->buffer);
    pthread_mutex_destroy(&ch->mutex);
    pthread_cond_destroy(&ch->not_empty);
    pthread_cond_destroy(&ch->not_full);
    free(ch);
}
