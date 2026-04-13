/*
 * KLP Protocol Implementation
 * Core protocol operations for KLang Protocol
 */

#include "klp_protocol.h"
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <zlib.h>

/* Frame operations */

KLPFrame* klp_frame_create(KLPFrameType type, uint32_t stream_id,
                           const void *payload, size_t payload_size) {
    if (payload_size > KLP_MAX_FRAME_SIZE) {
        return NULL;
    }
    
    KLPFrame *frame = (KLPFrame*)calloc(1, sizeof(KLPFrame));
    if (!frame) return NULL;
    
    /* Initialize header */
    frame->header.magic = htonl(KLP_MAGIC);
    frame->header.version = KLP_VERSION;
    frame->header.type = type;
    frame->header.flags = KLP_FLAG_NONE;
    frame->header.reserved = 0;
    frame->header.stream_id = htonl(stream_id);
    frame->header.length = htonl((uint32_t)payload_size);
    
    /* Copy payload if provided */
    if (payload && payload_size > 0) {
        frame->payload = malloc(payload_size);
        if (!frame->payload) {
            free(frame);
            return NULL;
        }
        memcpy(frame->payload, payload, payload_size);
        frame->payload_size = payload_size;
    } else {
        frame->payload = NULL;
        frame->payload_size = 0;
    }
    
    return frame;
}

void klp_frame_free(KLPFrame *frame) {
    if (!frame) return;
    if (frame->payload) {
        free(frame->payload);
    }
    free(frame);
}

int klp_frame_encode(const KLPFrame *frame, uint8_t *buffer, size_t buffer_size) {
    if (!frame || !buffer) return KLP_ERROR_PROTOCOL_ERROR;
    
    size_t required_size = sizeof(KLPFrameHeader) + frame->payload_size;
    if (buffer_size < required_size) {
        return KLP_ERROR_FRAME_TOO_LARGE;
    }
    
    /* Copy header */
    memcpy(buffer, &frame->header, sizeof(KLPFrameHeader));
    
    /* Copy payload */
    if (frame->payload && frame->payload_size > 0) {
        memcpy(buffer + sizeof(KLPFrameHeader), frame->payload, frame->payload_size);
    }
    
    return (int)required_size;
}

KLPFrame* klp_frame_decode(const uint8_t *buffer, size_t buffer_size) {
    if (!buffer || buffer_size < sizeof(KLPFrameHeader)) {
        return NULL;
    }
    
    /* Parse header */
    KLPFrameHeader header;
    memcpy(&header, buffer, sizeof(KLPFrameHeader));
    
    /* Validate magic */
    uint32_t magic = ntohl(header.magic);
    if (magic != KLP_MAGIC) {
        return NULL;
    }
    
    /* Validate version */
    if (header.version != KLP_VERSION) {
        return NULL;
    }
    
    /* Extract fields */
    uint32_t stream_id = ntohl(header.stream_id);
    uint32_t length = ntohl(header.length);
    
    /* Validate frame size */
    if (length > KLP_MAX_FRAME_SIZE) {
        return NULL;
    }
    
    /* Check if we have enough data */
    if (buffer_size < sizeof(KLPFrameHeader) + length) {
        return NULL;
    }
    
    /* Extract payload */
    const uint8_t *payload_ptr = buffer + sizeof(KLPFrameHeader);
    
    /* Create frame */
    KLPFrame *frame = klp_frame_create((KLPFrameType)header.type, stream_id,
                                       payload_ptr, length);
    if (!frame) return NULL;
    
    /* Copy flags */
    frame->header.flags = header.flags;
    
    return frame;
}

/* Connection operations */

KLPConnection* klp_connection_create(int socket_fd, int is_server) {
    KLPConnection *conn = (KLPConnection*)calloc(1, sizeof(KLPConnection));
    if (!conn) return NULL;
    
    conn->socket_fd = socket_fd;
    conn->is_server = is_server;
    conn->streams = NULL;
    conn->stream_count = 0;
    conn->max_frame_size = KLP_MAX_FRAME_SIZE;
    conn->window_size = 65535;
    conn->enable_compression = 0;
    conn->enable_encryption = 0;
    conn->user_data = NULL;
    
    return conn;
}

void klp_connection_free(KLPConnection *conn) {
    if (!conn) return;
    
    /* Close all streams */
    KLPStream *stream = conn->streams;
    while (stream) {
        KLPStream *next = stream->next;
        free(stream);
        stream = next;
    }
    
    /* Close socket */
    if (conn->socket_fd >= 0) {
        close(conn->socket_fd);
    }
    
    free(conn);
}

int klp_connection_send_frame(KLPConnection *conn, const KLPFrame *frame) {
    if (!conn || !frame) return KLP_ERROR_PROTOCOL_ERROR;
    
    /* Encode frame */
    size_t buffer_size = sizeof(KLPFrameHeader) + frame->payload_size;
    uint8_t *buffer = (uint8_t*)malloc(buffer_size);
    if (!buffer) return KLP_ERROR_NO_MEMORY;
    
    int encoded_size = klp_frame_encode(frame, buffer, buffer_size);
    if (encoded_size < 0) {
        free(buffer);
        return encoded_size;
    }
    
    /* Send to socket */
    ssize_t sent = send(conn->socket_fd, buffer, encoded_size, 0);
    free(buffer);
    
    if (sent < 0) {
        return KLP_ERROR_IO_ERROR;
    }
    
    return KLP_OK;
}

KLPFrame* klp_connection_recv_frame(KLPConnection *conn) {
    if (!conn) return NULL;
    
    /* Read header first */
    uint8_t header_buf[sizeof(KLPFrameHeader)];
    ssize_t received = recv(conn->socket_fd, header_buf, sizeof(KLPFrameHeader), MSG_WAITALL);
    
    if (received != sizeof(KLPFrameHeader)) {
        return NULL;
    }
    
    /* Parse header to get payload length */
    KLPFrameHeader header;
    memcpy(&header, header_buf, sizeof(KLPFrameHeader));
    uint32_t payload_length = ntohl(header.length);
    
    /* Validate payload length */
    if (payload_length > KLP_MAX_FRAME_SIZE) {
        return NULL;
    }
    
    /* Allocate buffer for full frame */
    size_t frame_size = sizeof(KLPFrameHeader) + payload_length;
    uint8_t *frame_buf = (uint8_t*)malloc(frame_size);
    if (!frame_buf) return NULL;
    
    /* Copy header */
    memcpy(frame_buf, header_buf, sizeof(KLPFrameHeader));
    
    /* Read payload */
    if (payload_length > 0) {
        received = recv(conn->socket_fd, frame_buf + sizeof(KLPFrameHeader),
                       payload_length, MSG_WAITALL);
        if (received != (ssize_t)payload_length) {
            free(frame_buf);
            return NULL;
        }
    }
    
    /* Decode frame */
    KLPFrame *frame = klp_frame_decode(frame_buf, frame_size);
    free(frame_buf);
    
    return frame;
}

/* Stream operations */

KLPStream* klp_stream_create(KLPConnection *conn, uint32_t stream_id) {
    if (!conn) return NULL;
    
    KLPStream *stream = (KLPStream*)calloc(1, sizeof(KLPStream));
    if (!stream) return NULL;
    
    stream->id = stream_id;
    stream->state = KLP_STREAM_OPEN;
    stream->priority = 0;
    stream->window_size = conn->window_size;
    stream->user_data = NULL;
    stream->next = conn->streams;
    
    conn->streams = stream;
    conn->stream_count++;
    
    return stream;
}

void klp_stream_close(KLPStream *stream) {
    if (!stream) return;
    stream->state = KLP_STREAM_CLOSED;
}

KLPStream* klp_stream_get(KLPConnection *conn, uint32_t stream_id) {
    if (!conn) return NULL;
    
    KLPStream *stream = conn->streams;
    while (stream) {
        if (stream->id == stream_id) {
            return stream;
        }
        stream = stream->next;
    }
    
    return NULL;
}

/* Chunking operations */

int klp_chunk_data(const void *data, size_t data_size,
                   KLPPayloadType type, KLPChunkConfig *config,
                   void (*callback)(const void *chunk, size_t size, void *ctx),
                   void *ctx) {
    if (!data || !callback || !config) return KLP_ERROR_PROTOCOL_ERROR;
    
    size_t chunk_size = config->chunk_size;
    if (chunk_size == 0) {
        chunk_size = KLP_DEFAULT_CHUNK_SIZE;
    }
    
    /* Adaptive chunking based on payload type */
    switch (type) {
        case KLP_PAYLOAD_TENSOR:
            /* For tensors, chunk along dimensions */
            chunk_size = KLP_DEFAULT_CHUNK_SIZE * 4; /* Larger chunks for tensors */
            break;
        case KLP_PAYLOAD_TEXT:
        case KLP_PAYLOAD_JSON:
            /* Text data - smaller chunks */
            chunk_size = KLP_DEFAULT_CHUNK_SIZE / 2;
            break;
        default:
            break;
    }
    
    /* Send data in chunks */
    size_t offset = 0;
    while (offset < data_size) {
        size_t current_chunk_size = (data_size - offset > chunk_size) 
                                    ? chunk_size 
                                    : (data_size - offset);
        
        callback((const uint8_t*)data + offset, current_chunk_size, ctx);
        offset += current_chunk_size;
    }
    
    return KLP_OK;
}

/* Tensor operations */

int klp_send_tensor(KLPConnection *conn, uint32_t stream_id,
                    const void *data, const KLPTensorMeta *meta) {
    if (!conn || !data || !meta) return KLP_ERROR_PROTOCOL_ERROR;
    
    /* Send tensor metadata first */
    KLPFrame *meta_frame = klp_frame_create(KLP_FRAME_TENSOR_META, stream_id,
                                            meta, sizeof(KLPTensorMeta));
    if (!meta_frame) return KLP_ERROR_NO_MEMORY;
    
    int result = klp_connection_send_frame(conn, meta_frame);
    klp_frame_free(meta_frame);
    
    if (result != KLP_OK) return result;
    
    /* Send tensor data */
    KLPFrame *data_frame = klp_frame_create(KLP_FRAME_TENSOR, stream_id,
                                            data, meta->total_size);
    if (!data_frame) return KLP_ERROR_NO_MEMORY;
    
    data_frame->header.flags = KLP_FLAG_END_STREAM;
    
    result = klp_connection_send_frame(conn, data_frame);
    klp_frame_free(data_frame);
    
    return result;
}

int klp_recv_tensor(KLPConnection *conn, uint32_t stream_id,
                    void **data, KLPTensorMeta *meta) {
    (void)stream_id;  /* Unused parameter */
    if (!conn || !data || !meta) return KLP_ERROR_PROTOCOL_ERROR;
    
    /* Receive tensor metadata */
    KLPFrame *meta_frame = klp_connection_recv_frame(conn);
    if (!meta_frame) return KLP_ERROR_IO_ERROR;
    
    if (meta_frame->header.type != KLP_FRAME_TENSOR_META) {
        klp_frame_free(meta_frame);
        return KLP_ERROR_PROTOCOL_ERROR;
    }
    
    memcpy(meta, meta_frame->payload, sizeof(KLPTensorMeta));
    klp_frame_free(meta_frame);
    
    /* Receive tensor data */
    KLPFrame *data_frame = klp_connection_recv_frame(conn);
    if (!data_frame) return KLP_ERROR_IO_ERROR;
    
    if (data_frame->header.type != KLP_FRAME_TENSOR) {
        klp_frame_free(data_frame);
        return KLP_ERROR_PROTOCOL_ERROR;
    }
    
    /* Allocate and copy data */
    *data = malloc(data_frame->payload_size);
    if (!*data) {
        klp_frame_free(data_frame);
        return KLP_ERROR_NO_MEMORY;
    }
    
    memcpy(*data, data_frame->payload, data_frame->payload_size);
    klp_frame_free(data_frame);
    
    return KLP_OK;
}

/* Compression (using zlib) */

int klp_compress(const void *input, size_t input_size,
                 void **output, size_t *output_size) {
    if (!input || !output || !output_size) return KLP_ERROR_PROTOCOL_ERROR;
    
    /* Allocate output buffer */
    *output_size = compressBound(input_size);
    *output = malloc(*output_size);
    if (!*output) return KLP_ERROR_NO_MEMORY;
    
    /* Compress */
    int result = compress((Bytef*)*output, (uLongf*)output_size,
                         (const Bytef*)input, input_size);
    
    if (result != Z_OK) {
        free(*output);
        *output = NULL;
        return KLP_ERROR_COMPRESSION_ERROR;
    }
    
    return KLP_OK;
}

int klp_decompress(const void *input, size_t input_size,
                   void **output, size_t *output_size) {
    if (!input || !output || !output_size) return KLP_ERROR_PROTOCOL_ERROR;
    
    /* For now, assume max decompressed size is 10x compressed */
    *output_size = input_size * 10;
    *output = malloc(*output_size);
    if (!*output) return KLP_ERROR_NO_MEMORY;
    
    /* Decompress */
    int result = uncompress((Bytef*)*output, (uLongf*)output_size,
                           (const Bytef*)input, input_size);
    
    if (result != Z_OK) {
        free(*output);
        *output = NULL;
        return KLP_ERROR_COMPRESSION_ERROR;
    }
    
    return KLP_OK;
}

/* Utility functions */

const char* klp_error_string(KLPError error) {
    switch (error) {
        case KLP_OK: return "Success";
        case KLP_ERROR_INVALID_MAGIC: return "Invalid magic number";
        case KLP_ERROR_INVALID_VERSION: return "Invalid protocol version";
        case KLP_ERROR_FRAME_TOO_LARGE: return "Frame too large";
        case KLP_ERROR_INVALID_STREAM: return "Invalid stream ID";
        case KLP_ERROR_STREAM_CLOSED: return "Stream closed";
        case KLP_ERROR_PROTOCOL_ERROR: return "Protocol error";
        case KLP_ERROR_COMPRESSION_ERROR: return "Compression error";
        case KLP_ERROR_CRYPTO_ERROR: return "Cryptography error";
        case KLP_ERROR_NO_MEMORY: return "Out of memory";
        case KLP_ERROR_IO_ERROR: return "I/O error";
        default: return "Unknown error";
    }
}

const char* klp_frame_type_string(KLPFrameType type) {
    switch (type) {
        case KLP_FRAME_DATA: return "DATA";
        case KLP_FRAME_HEADERS: return "HEADERS";
        case KLP_FRAME_SETTINGS: return "SETTINGS";
        case KLP_FRAME_PING: return "PING";
        case KLP_FRAME_PONG: return "PONG";
        case KLP_FRAME_GOAWAY: return "GOAWAY";
        case KLP_FRAME_RST_STREAM: return "RST_STREAM";
        case KLP_FRAME_WINDOW_UPDATE: return "WINDOW_UPDATE";
        case KLP_FRAME_TENSOR: return "TENSOR";
        case KLP_FRAME_TENSOR_META: return "TENSOR_META";
        case KLP_FRAME_MODEL_CHUNK: return "MODEL_CHUNK";
        case KLP_FRAME_QUANTUM: return "QUANTUM";
        case KLP_FRAME_QUANTUM_KEY: return "QUANTUM_KEY";
        case KLP_FRAME_ENTANGLEMENT: return "ENTANGLEMENT";
        default: return "UNKNOWN";
    }
}
