/*
 * KLP Protocol (KLang Protocol)
 * Advanced protocol for AI and quantum computing
 * 
 * Features:
 * - Binary framing with multiplexing
 * - Direct streaming with zero-copy
 * - Adaptive chunking for different payload types
 * - AI tensor streaming
 * - Quantum-resistant cryptography
 * - Bidirectional streaming with backpressure
 */

#ifndef KLP_PROTOCOL_H
#define KLP_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>

/* Protocol version */
#define KLP_VERSION 1

/* Magic number: 'KLP ' */
#define KLP_MAGIC 0x4B4C5020

/* Maximum frame size (16MB) */
#define KLP_MAX_FRAME_SIZE (16 * 1024 * 1024)

/* Maximum streams per connection */
#define KLP_MAX_STREAMS 256

/* Default chunk size */
#define KLP_DEFAULT_CHUNK_SIZE (64 * 1024)

/* Frame types */
typedef enum {
    KLP_FRAME_DATA          = 0x00,  /* Standard data frame */
    KLP_FRAME_HEADERS       = 0x01,  /* Metadata/headers */
    KLP_FRAME_SETTINGS      = 0x02,  /* Connection settings */
    KLP_FRAME_PING          = 0x03,  /* Keepalive ping */
    KLP_FRAME_PONG          = 0x04,  /* Keepalive pong */
    KLP_FRAME_GOAWAY        = 0x05,  /* Graceful shutdown */
    KLP_FRAME_RST_STREAM    = 0x06,  /* Reset stream */
    KLP_FRAME_WINDOW_UPDATE = 0x07,  /* Flow control */
    
    /* AI-specific frames */
    KLP_FRAME_TENSOR        = 0x10,  /* AI tensor data */
    KLP_FRAME_TENSOR_META   = 0x11,  /* Tensor metadata (shape, dtype) */
    KLP_FRAME_MODEL_CHUNK   = 0x12,  /* Model parameter chunk */
    
    /* Quantum-specific frames */
    KLP_FRAME_QUANTUM       = 0x20,  /* Quantum state data */
    KLP_FRAME_QUANTUM_KEY   = 0x21,  /* Quantum key distribution */
    KLP_FRAME_ENTANGLEMENT  = 0x22   /* Quantum entanglement metadata */
} KLPFrameType;

/* Frame flags */
typedef enum {
    KLP_FLAG_NONE          = 0x00,
    KLP_FLAG_END_STREAM    = 0x01,  /* Last frame in stream */
    KLP_FLAG_END_HEADERS   = 0x02,  /* Last header frame */
    KLP_FLAG_COMPRESSED    = 0x04,  /* Payload is compressed */
    KLP_FLAG_PRIORITY      = 0x08,  /* High priority frame */
    KLP_FLAG_ENCRYPTED     = 0x10,  /* Payload is encrypted */
    KLP_FLAG_QUANTUM_SAFE  = 0x20   /* Uses quantum-resistant crypto */
} KLPFrameFlags;

/* Payload types for adaptive chunking */
typedef enum {
    KLP_PAYLOAD_TEXT       = 0,  /* Text data */
    KLP_PAYLOAD_BINARY     = 1,  /* Binary data */
    KLP_PAYLOAD_JSON       = 2,  /* JSON data */
    KLP_PAYLOAD_TENSOR     = 3,  /* AI tensor */
    KLP_PAYLOAD_MODEL      = 4,  /* Model weights */
    KLP_PAYLOAD_QUANTUM    = 5   /* Quantum state */
} KLPPayloadType;

/* Tensor data types */
typedef enum {
    KLP_DTYPE_INT8    = 0,
    KLP_DTYPE_INT16   = 1,
    KLP_DTYPE_INT32   = 2,
    KLP_DTYPE_INT64   = 3,
    KLP_DTYPE_FLOAT16 = 4,
    KLP_DTYPE_FLOAT32 = 5,
    KLP_DTYPE_FLOAT64 = 6,
    KLP_DTYPE_COMPLEX = 7
} KLPTensorDType;

/* Error codes */
typedef enum {
    KLP_OK                  = 0,
    KLP_ERROR_INVALID_MAGIC = -1,
    KLP_ERROR_INVALID_VERSION = -2,
    KLP_ERROR_FRAME_TOO_LARGE = -3,
    KLP_ERROR_INVALID_STREAM = -4,
    KLP_ERROR_STREAM_CLOSED = -5,
    KLP_ERROR_PROTOCOL_ERROR = -6,
    KLP_ERROR_COMPRESSION_ERROR = -7,
    KLP_ERROR_CRYPTO_ERROR = -8,
    KLP_ERROR_NO_MEMORY = -9,
    KLP_ERROR_IO_ERROR = -10
} KLPError;

/* Frame header (12 bytes) */
typedef struct {
    uint32_t magic;      /* Magic number: 0x4B4C5020 */
    uint8_t version;     /* Protocol version */
    uint8_t type;        /* Frame type */
    uint8_t flags;       /* Frame flags */
    uint8_t reserved;    /* Reserved for future use */
    uint32_t stream_id;  /* Stream identifier */
    uint32_t length;     /* Payload length */
} __attribute__((packed)) KLPFrameHeader;

/* Tensor metadata */
typedef struct {
    uint8_t dtype;       /* Data type */
    uint8_t ndim;        /* Number of dimensions */
    uint16_t reserved;   /* Reserved */
    uint32_t total_size; /* Total tensor size in bytes */
    uint32_t shape[8];   /* Dimension sizes (max 8 dims) */
} __attribute__((packed)) KLPTensorMeta;

/* Frame structure */
typedef struct {
    KLPFrameHeader header;
    void *payload;       /* Frame payload */
    size_t payload_size; /* Actual payload size */
} KLPFrame;

/* Stream state */
typedef enum {
    KLP_STREAM_IDLE = 0,
    KLP_STREAM_OPEN,
    KLP_STREAM_HALF_CLOSED_LOCAL,
    KLP_STREAM_HALF_CLOSED_REMOTE,
    KLP_STREAM_CLOSED
} KLPStreamState;

/* Stream structure */
typedef struct KLPStream {
    uint32_t id;
    KLPStreamState state;
    int priority;
    size_t window_size;
    void *user_data;
    struct KLPStream *next;
} KLPStream;

/* Connection structure */
typedef struct {
    int socket_fd;
    int is_server;
    KLPStream *streams;
    int stream_count;
    size_t max_frame_size;
    size_t window_size;
    int enable_compression;
    int enable_encryption;
    void *user_data;
} KLPConnection;

/* Chunking configuration */
typedef struct {
    size_t chunk_size;          /* Base chunk size */
    int enable_compression;     /* Enable compression */
    int enable_adaptive;        /* Adaptive chunk sizing */
    float compression_threshold;/* Compress if ratio > threshold */
} KLPChunkConfig;

/* Protocol functions */

/* Frame operations */
KLPFrame* klp_frame_create(KLPFrameType type, uint32_t stream_id, 
                           const void *payload, size_t payload_size);
void klp_frame_free(KLPFrame *frame);
int klp_frame_encode(const KLPFrame *frame, uint8_t *buffer, size_t buffer_size);
KLPFrame* klp_frame_decode(const uint8_t *buffer, size_t buffer_size);

/* Connection operations */
KLPConnection* klp_connection_create(int socket_fd, int is_server);
void klp_connection_free(KLPConnection *conn);
int klp_connection_send_frame(KLPConnection *conn, const KLPFrame *frame);
KLPFrame* klp_connection_recv_frame(KLPConnection *conn);

/* Stream operations */
KLPStream* klp_stream_create(KLPConnection *conn, uint32_t stream_id);
void klp_stream_close(KLPStream *stream);
KLPStream* klp_stream_get(KLPConnection *conn, uint32_t stream_id);

/* Chunking operations */
int klp_chunk_data(const void *data, size_t data_size, 
                   KLPPayloadType type, KLPChunkConfig *config,
                   void (*callback)(const void *chunk, size_t size, void *ctx),
                   void *ctx);

/* Tensor operations */
int klp_send_tensor(KLPConnection *conn, uint32_t stream_id,
                    const void *data, const KLPTensorMeta *meta);
int klp_recv_tensor(KLPConnection *conn, uint32_t stream_id,
                    void **data, KLPTensorMeta *meta);

/* Compression */
int klp_compress(const void *input, size_t input_size,
                 void **output, size_t *output_size);
int klp_decompress(const void *input, size_t input_size,
                   void **output, size_t *output_size);

/* Utility functions */
const char* klp_error_string(KLPError error);
const char* klp_frame_type_string(KLPFrameType type);

#endif /* KLP_PROTOCOL_H */
