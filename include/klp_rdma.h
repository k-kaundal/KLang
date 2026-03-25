/*
 * KLP RDMA Support
 * Phase 4: InfiniBand/RoCE Zero-Copy Transport
 */

#ifndef KLP_RDMA_H
#define KLP_RDMA_H

#include "klp_protocol.h"
#include <stdint.h>
#include <stddef.h>

/* RDMA backend types */
typedef enum {
    KLP_RDMA_NONE = 0,
    KLP_RDMA_INFINIBAND = 1,
    KLP_RDMA_ROCE_V1 = 2,
    KLP_RDMA_ROCE_V2 = 3,
    KLP_RDMA_IWARP = 4
} KLPRDMABackend;

/* RDMA operations */
typedef enum {
    KLP_RDMA_SEND = 1,
    KLP_RDMA_RECV = 2,
    KLP_RDMA_WRITE = 3,      /* One-sided write */
    KLP_RDMA_READ = 4,       /* One-sided read */
    KLP_RDMA_ATOMIC_CAS = 5, /* Compare-and-swap */
    KLP_RDMA_ATOMIC_FA = 6   /* Fetch-and-add */
} KLPRDMAOperation;

/* RDMA memory region */
typedef struct {
    void *addr;              /* Virtual address */
    size_t length;           /* Length in bytes */
    uint32_t lkey;           /* Local key */
    uint32_t rkey;           /* Remote key */
    int access_flags;        /* Access permissions */
} KLPRDMAMemoryRegion;

/* RDMA connection */
typedef struct {
    KLPRDMABackend backend;
    void *device_context;    /* ibverbs context */
    void *protection_domain; /* Protection domain */
    void *queue_pair;        /* Queue pair */
    void *completion_queue;  /* Completion queue */
    
    /* Memory regions */
    KLPRDMAMemoryRegion *send_mr;
    KLPRDMAMemoryRegion *recv_mr;
    
    /* Connection state */
    char *remote_addr;
    int remote_port;
    int connected;
    
} KLPRDMAConnection;

/* RDMA transfer info */
typedef struct {
    KLPRDMAOperation op;
    void *local_addr;
    size_t length;
    uint64_t remote_addr;    /* For one-sided ops */
    uint32_t rkey;           /* For one-sided ops */
    uint64_t wr_id;          /* Work request ID */
} KLPRDMATransfer;

/* Initialize RDMA */
int klp_rdma_init();
void klp_rdma_cleanup();

/* Device management */
int klp_rdma_get_device_count();
const char* klp_rdma_get_device_name(int index);
int klp_rdma_select_device(int index);

/* Connection management */
KLPRDMAConnection* klp_rdma_connection_create(KLPRDMABackend backend);
void klp_rdma_connection_free(KLPRDMAConnection *conn);
int klp_rdma_connect(KLPRDMAConnection *conn, const char *host, int port);
int klp_rdma_listen(KLPRDMAConnection *conn, int port);
KLPRDMAConnection* klp_rdma_accept(KLPRDMAConnection *listening_conn);
void klp_rdma_disconnect(KLPRDMAConnection *conn);

/* Memory registration */
KLPRDMAMemoryRegion* klp_rdma_register_memory(KLPRDMAConnection *conn,
                                               void *addr, size_t length,
                                               int access_flags);
void klp_rdma_deregister_memory(KLPRDMAMemoryRegion *mr);

/* Data transfer operations */
int klp_rdma_send(KLPRDMAConnection *conn, const void *data, size_t length);
int klp_rdma_recv(KLPRDMAConnection *conn, void *buffer, size_t buffer_size,
                  size_t *bytes_received);

/* One-sided operations (zero-copy) */
int klp_rdma_write(KLPRDMAConnection *conn, const void *local_addr,
                   size_t length, uint64_t remote_addr, uint32_t rkey);
int klp_rdma_read(KLPRDMAConnection *conn, void *local_addr, size_t length,
                  uint64_t remote_addr, uint32_t rkey);

/* Atomic operations */
int klp_rdma_atomic_cas(KLPRDMAConnection *conn, uint64_t remote_addr,
                        uint32_t rkey, uint64_t compare, uint64_t swap,
                        uint64_t *result);
int klp_rdma_atomic_fetch_add(KLPRDMAConnection *conn, uint64_t remote_addr,
                               uint32_t rkey, uint64_t add, uint64_t *result);

/* KLP Frame transfer over RDMA */
int klp_rdma_send_frame(KLPRDMAConnection *conn, KLPFrame *frame);
KLPFrame* klp_rdma_recv_frame(KLPRDMAConnection *conn);

/* Batch operations (for performance) */
int klp_rdma_send_batch(KLPRDMAConnection *conn, KLPRDMATransfer *transfers,
                        int count);
int klp_rdma_poll_completions(KLPRDMAConnection *conn, int max_completions,
                               uint64_t *completed_wr_ids);

/* Performance tuning */
int klp_rdma_set_inline_threshold(KLPRDMAConnection *conn, size_t threshold);
int klp_rdma_set_max_send_wr(KLPRDMAConnection *conn, int max_wr);
int klp_rdma_set_max_recv_wr(KLPRDMAConnection *conn, int max_wr);

/* Utilities */
const char* klp_rdma_backend_name(KLPRDMABackend backend);
const char* klp_rdma_operation_name(KLPRDMAOperation op);
void klp_rdma_print_device_info(int device_index);
int klp_rdma_benchmark(KLPRDMAConnection *conn);

/* Access flags for memory registration */
#define KLP_RDMA_ACCESS_LOCAL_WRITE  (1 << 0)
#define KLP_RDMA_ACCESS_REMOTE_WRITE (1 << 1)
#define KLP_RDMA_ACCESS_REMOTE_READ  (1 << 2)
#define KLP_RDMA_ACCESS_REMOTE_ATOMIC (1 << 3)

#endif /* KLP_RDMA_H */
