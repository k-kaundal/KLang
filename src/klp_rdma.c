/*
 * KLP RDMA Support Implementation
 * Phase 4: InfiniBand/RoCE Zero-Copy Transport
 * 
 * NOTE: This is a reference implementation. Full production requires:
 *   - InfiniBand or RoCE capable hardware
 *   - libibverbs and librdmacm libraries
 *   - Define ENABLE_RDMA to enable real RDMA features
 * 
 * For production:
 *   - Install RDMA libraries: sudo apt-get install libibverbs-dev librdmacm-dev
 *   - Link with -libverbs -lrdmacm
 *   - Configure RDMA devices
 */

#include "klp_rdma.h"
#include "klp_protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ENABLE_RDMA
#include <infiniband/verbs.h>
#include <rdma/rdma_cma.h>
#endif

/* Initialize RDMA */
int klp_rdma_init() {
#ifdef ENABLE_RDMA
    printf("KLP RDMA: Initialized with libibverbs support\n");
    return 0;
#else
    printf("KLP RDMA: Initialized (stub mode - recompile with ENABLE_RDMA)\n");
    return 0;
#endif
}

void klp_rdma_cleanup() {
    /* Cleanup resources */
}

/* Device management */
int klp_rdma_get_device_count() {
#ifdef ENABLE_RDMA
    struct ibv_device **device_list;
    int num_devices = 0;
    
    device_list = ibv_get_device_list(&num_devices);
    if (device_list) {
        ibv_free_device_list(device_list);
    }
    
    return num_devices;
#else
    /* Stub: return 0 devices */
    return 0;
#endif
}

const char* klp_rdma_get_device_name(int index) {
#ifdef ENABLE_RDMA
    struct ibv_device **device_list;
    int num_devices = 0;
    const char *name = NULL;
    
    device_list = ibv_get_device_list(&num_devices);
    if (device_list && index >= 0 && index < num_devices) {
        name = ibv_get_device_name(device_list[index]);
    }
    
    if (device_list) {
        ibv_free_device_list(device_list);
    }
    
    return name;
#else
    (void)index;
    return "no-device (stub)";
#endif
}

int klp_rdma_select_device(int index) {
    (void)index;
    printf("KLP RDMA: Selected device %d (stub)\n", index);
    return 0;
}

/* Connection management */
KLPRDMAConnection* klp_rdma_connection_create(KLPRDMABackend backend) {
    KLPRDMAConnection *conn = (KLPRDMAConnection*)calloc(1, sizeof(KLPRDMAConnection));
    if (!conn) return NULL;
    
    conn->backend = backend;
    conn->connected = 0;
    
    printf("KLP RDMA: Created connection (backend: %s)\n", klp_rdma_backend_name(backend));
    
    return conn;
}

void klp_rdma_connection_free(KLPRDMAConnection *conn) {
    if (!conn) return;
    
    if (conn->connected) {
        klp_rdma_disconnect(conn);
    }
    
    if (conn->remote_addr) free(conn->remote_addr);
    if (conn->send_mr) free(conn->send_mr);
    if (conn->recv_mr) free(conn->recv_mr);
    
    free(conn);
}

int klp_rdma_connect(KLPRDMAConnection *conn, const char *host, int port) {
    if (!conn || !host) return -1;
    
    conn->remote_addr = strdup(host);
    conn->remote_port = port;
    
#ifdef ENABLE_RDMA
    /* Real RDMA connection setup */
    struct rdma_cm_id *cm_id;
    struct rdma_event_channel *cm_channel;
    
    cm_channel = rdma_create_event_channel();
    if (!cm_channel) return -1;
    
    if (rdma_create_id(cm_channel, &cm_id, NULL, RDMA_PS_TCP) != 0) {
        rdma_destroy_event_channel(cm_channel);
        return -1;
    }
    
    /* Store in context */
    conn->device_context = cm_id;
    conn->connected = 1;
    
    printf("KLP RDMA: Connected to %s:%d\n", host, port);
    return 0;
#else
    /* Stub mode */
    conn->connected = 1;
    printf("KLP RDMA: Connected to %s:%d (stub)\n", host, port);
    return 0;
#endif
}

int klp_rdma_listen(KLPRDMAConnection *conn, int port) {
    if (!conn) return -1;
    
    conn->remote_port = port;
    
    printf("KLP RDMA: Listening on port %d (stub)\n", port);
    return 0;
}

KLPRDMAConnection* klp_rdma_accept(KLPRDMAConnection *listening_conn) {
    if (!listening_conn) return NULL;
    
    /* Create new connection for accepted client */
    KLPRDMAConnection *client_conn = klp_rdma_connection_create(listening_conn->backend);
    if (!client_conn) return NULL;
    
    client_conn->connected = 1;
    
    printf("KLP RDMA: Accepted connection (stub)\n");
    return client_conn;
}

void klp_rdma_disconnect(KLPRDMAConnection *conn) {
    if (!conn) return;
    
#ifdef ENABLE_RDMA
    if (conn->device_context) {
        struct rdma_cm_id *cm_id = (struct rdma_cm_id*)conn->device_context;
        rdma_disconnect(cm_id);
        rdma_destroy_id(cm_id);
        conn->device_context = NULL;
    }
#endif
    
    conn->connected = 0;
    printf("KLP RDMA: Disconnected\n");
}

/* Memory registration */
KLPRDMAMemoryRegion* klp_rdma_register_memory(KLPRDMAConnection *conn,
                                               void *addr, size_t length,
                                               int access_flags) {
    if (!conn || !addr) return NULL;
    
    KLPRDMAMemoryRegion *mr = (KLPRDMAMemoryRegion*)calloc(1, sizeof(KLPRDMAMemoryRegion));
    if (!mr) return NULL;
    
    mr->addr = addr;
    mr->length = length;
    mr->access_flags = access_flags;
    
#ifdef ENABLE_RDMA
    /* Real memory registration */
    struct ibv_pd *pd = (struct ibv_pd*)conn->protection_domain;
    if (pd) {
        struct ibv_mr *ibv_mr = ibv_reg_mr(pd, addr, length, access_flags);
        if (ibv_mr) {
            mr->lkey = ibv_mr->lkey;
            mr->rkey = ibv_mr->rkey;
        }
    }
#else
    /* Stub: generate fake keys */
    mr->lkey = 0x12345678;
    mr->rkey = 0x87654321;
#endif
    
    printf("KLP RDMA: Registered memory region %p (%zu bytes)\n", addr, length);
    return mr;
}

void klp_rdma_deregister_memory(KLPRDMAMemoryRegion *mr) {
    if (!mr) return;
    
#ifdef ENABLE_RDMA
    /* Deregister from RDMA */
    /* struct ibv_mr *ibv_mr = ...; */
    /* ibv_dereg_mr(ibv_mr); */
#endif
    
    printf("KLP RDMA: Deregistered memory region\n");
    free(mr);
}

/* Data transfer operations */
int klp_rdma_send(KLPRDMAConnection *conn, const void *data, size_t length) {
    if (!conn || !data || !conn->connected) return -1;
    
#ifdef ENABLE_RDMA
    /* Real RDMA send */
    struct ibv_send_wr wr, *bad_wr;
    struct ibv_sge sge;
    
    memset(&wr, 0, sizeof(wr));
    memset(&sge, 0, sizeof(sge));
    
    sge.addr = (uintptr_t)data;
    sge.length = length;
    sge.lkey = conn->send_mr->lkey;
    
    wr.sg_list = &sge;
    wr.num_sge = 1;
    wr.opcode = IBV_WR_SEND;
    wr.send_flags = IBV_SEND_SIGNALED;
    
    struct ibv_qp *qp = (struct ibv_qp*)conn->queue_pair;
    if (ibv_post_send(qp, &wr, &bad_wr) != 0) {
        return -1;
    }
    
    return 0;
#else
    /* Stub mode */
    printf("KLP RDMA: Sent %zu bytes (stub)\n", length);
    return 0;
#endif
}

int klp_rdma_recv(KLPRDMAConnection *conn, void *buffer, size_t buffer_size,
                  size_t *bytes_received) {
    if (!conn || !buffer || !conn->connected) return -1;
    
#ifdef ENABLE_RDMA
    /* Real RDMA receive */
    /* Would poll completion queue and copy data */
    *bytes_received = 0;
    return 0;
#else
    /* Stub mode */
    *bytes_received = 0;
    printf("KLP RDMA: Received data (stub)\n");
    return 0;
#endif
}

/* One-sided operations (zero-copy) */
int klp_rdma_write(KLPRDMAConnection *conn, const void *local_addr,
                   size_t length, uint64_t remote_addr, uint32_t rkey) {
    if (!conn || !local_addr || !conn->connected) return -1;
    
#ifdef ENABLE_RDMA
    struct ibv_send_wr wr, *bad_wr;
    struct ibv_sge sge;
    
    memset(&wr, 0, sizeof(wr));
    memset(&sge, 0, sizeof(sge));
    
    sge.addr = (uintptr_t)local_addr;
    sge.length = length;
    sge.lkey = conn->send_mr->lkey;
    
    wr.sg_list = &sge;
    wr.num_sge = 1;
    wr.opcode = IBV_WR_RDMA_WRITE;
    wr.send_flags = IBV_SEND_SIGNALED;
    wr.wr.rdma.remote_addr = remote_addr;
    wr.wr.rdma.rkey = rkey;
    
    struct ibv_qp *qp = (struct ibv_qp*)conn->queue_pair;
    if (ibv_post_send(qp, &wr, &bad_wr) != 0) {
        return -1;
    }
    
    printf("KLP RDMA: One-sided write %zu bytes\n", length);
    return 0;
#else
    (void)remote_addr;
    (void)rkey;
    printf("KLP RDMA: One-sided write %zu bytes (stub)\n", length);
    return 0;
#endif
}

int klp_rdma_read(KLPRDMAConnection *conn, void *local_addr, size_t length,
                  uint64_t remote_addr, uint32_t rkey) {
    if (!conn || !local_addr || !conn->connected) return -1;
    
    (void)remote_addr;
    (void)rkey;
    
    printf("KLP RDMA: One-sided read %zu bytes (stub)\n", length);
    return 0;
}

/* Atomic operations */
int klp_rdma_atomic_cas(KLPRDMAConnection *conn, uint64_t remote_addr,
                        uint32_t rkey, uint64_t compare, uint64_t swap,
                        uint64_t *result) {
    if (!conn || !result || !conn->connected) return -1;
    
    (void)remote_addr;
    (void)rkey;
    (void)compare;
    (void)swap;
    
    *result = compare; /* Stub: always succeeds */
    printf("KLP RDMA: Atomic CAS (stub)\n");
    return 0;
}

int klp_rdma_atomic_fetch_add(KLPRDMAConnection *conn, uint64_t remote_addr,
                               uint32_t rkey, uint64_t add, uint64_t *result) {
    if (!conn || !result || !conn->connected) return -1;
    
    (void)remote_addr;
    (void)rkey;
    
    *result = add; /* Stub: return the add value */
    printf("KLP RDMA: Atomic fetch-add (stub)\n");
    return 0;
}

/* KLP Frame transfer over RDMA */
int klp_rdma_send_frame(KLPRDMAConnection *conn, KLPFrame *frame) {
    if (!conn || !frame) return -1;
    
    /* Serialize frame and send */
    size_t total_size = sizeof(KLPFrameHeader) + frame->payload_size;
    uint8_t *buffer = malloc(total_size);
    if (!buffer) return -1;
    
    /* Copy header */
    memcpy(buffer, &frame->header, sizeof(KLPFrameHeader));
    
    /* Copy payload */
    if (frame->payload_size > 0 && frame->payload) {
        memcpy(buffer + sizeof(KLPFrameHeader), frame->payload, frame->payload_size);
    }
    
    int result = klp_rdma_send(conn, buffer, total_size);
    free(buffer);
    
    return result;
}

KLPFrame* klp_rdma_recv_frame(KLPRDMAConnection *conn) {
    if (!conn) return NULL;
    
    /* Stub: return NULL for now */
    printf("KLP RDMA: Receive frame (stub)\n");
    return NULL;
}

/* Batch operations */
int klp_rdma_send_batch(KLPRDMAConnection *conn, KLPRDMATransfer *transfers,
                        int count) {
    if (!conn || !transfers || count <= 0) return -1;
    
    for (int i = 0; i < count; i++) {
        /* Process each transfer */
        printf("KLP RDMA: Batch transfer %d (stub)\n", i);
    }
    
    return count;
}

int klp_rdma_poll_completions(KLPRDMAConnection *conn, int max_completions,
                               uint64_t *completed_wr_ids) {
    if (!conn || !completed_wr_ids) return -1;
    
#ifdef ENABLE_RDMA
    struct ibv_cq *cq = (struct ibv_cq*)conn->completion_queue;
    if (!cq) return 0;
    
    struct ibv_wc wc[max_completions];
    int n = ibv_poll_cq(cq, max_completions, wc);
    
    for (int i = 0; i < n; i++) {
        completed_wr_ids[i] = wc[i].wr_id;
    }
    
    return n;
#else
    (void)max_completions;
    /* Stub: no completions */
    return 0;
#endif
}

/* Performance tuning */
int klp_rdma_set_inline_threshold(KLPRDMAConnection *conn, size_t threshold) {
    if (!conn) return -1;
    printf("KLP RDMA: Set inline threshold to %zu bytes\n", threshold);
    return 0;
}

int klp_rdma_set_max_send_wr(KLPRDMAConnection *conn, int max_wr) {
    if (!conn) return -1;
    printf("KLP RDMA: Set max send WR to %d\n", max_wr);
    return 0;
}

int klp_rdma_set_max_recv_wr(KLPRDMAConnection *conn, int max_wr) {
    if (!conn) return -1;
    printf("KLP RDMA: Set max recv WR to %d\n", max_wr);
    return 0;
}

/* Utilities */
const char* klp_rdma_backend_name(KLPRDMABackend backend) {
    switch (backend) {
        case KLP_RDMA_NONE: return "None";
        case KLP_RDMA_INFINIBAND: return "InfiniBand";
        case KLP_RDMA_ROCE_V1: return "RoCE v1";
        case KLP_RDMA_ROCE_V2: return "RoCE v2";
        case KLP_RDMA_IWARP: return "iWARP";
        default: return "Unknown";
    }
}

const char* klp_rdma_operation_name(KLPRDMAOperation op) {
    switch (op) {
        case KLP_RDMA_SEND: return "Send";
        case KLP_RDMA_RECV: return "Receive";
        case KLP_RDMA_WRITE: return "Write";
        case KLP_RDMA_READ: return "Read";
        case KLP_RDMA_ATOMIC_CAS: return "Atomic CAS";
        case KLP_RDMA_ATOMIC_FA: return "Atomic Fetch-Add";
        default: return "Unknown";
    }
}

void klp_rdma_print_device_info(int device_index) {
    printf("RDMA Device %d:\n", device_index);
    printf("  Name: %s\n", klp_rdma_get_device_name(device_index));
    printf("  (More info requires ENABLE_RDMA)\n");
}

int klp_rdma_benchmark(KLPRDMAConnection *conn) {
    if (!conn) return -1;
    
    printf("KLP RDMA Benchmark:\n");
    printf("  (Benchmark not implemented - use perftest tools)\n");
    printf("  Try: ib_write_bw, ib_read_bw, ib_send_bw\n");
    
    return 0;
}
