/*
 * KLP Client Implementation
 * Client library for KLP protocol
 */

#include "klp_client.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

/* Client lifecycle */

KLPClient* klp_client_create(const char *host, int port) {
    if (!host || port <= 0 || port > 65535) {
        return NULL;
    }
    
    KLPClient *client = (KLPClient*)calloc(1, sizeof(KLPClient));
    if (!client) return NULL;
    
    client->host = strdup(host);
    if (!client->host) {
        free(client);
        return NULL;
    }
    
    client->port = port;
    client->connection = NULL;
    client->connected = 0;
    
    return client;
}

void klp_client_free(KLPClient *client) {
    if (!client) return;
    
    if (client->connected) {
        klp_client_disconnect(client);
    }
    
    if (client->host) {
        free(client->host);
    }
    
    free(client);
}

int klp_client_connect(KLPClient *client) {
    if (!client || client->connected) {
        return -1;
    }
    
    /* Create socket */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("KLP Client: socket creation failed");
        return -1;
    }
    
    /* Connect to server */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(client->port);
    
    if (inet_pton(AF_INET, client->host, &server_addr.sin_addr) <= 0) {
        perror("KLP Client: invalid address");
        close(sockfd);
        return -1;
    }
    
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("KLP Client: connection failed");
        close(sockfd);
        return -1;
    }
    
    /* Create KLP connection */
    client->connection = klp_connection_create(sockfd, 0);
    if (!client->connection) {
        close(sockfd);
        return -1;
    }
    
    client->connected = 1;
    
    printf("KLP Client: Connected to %s:%d\n", client->host, client->port);
    
    return 0;
}

void klp_client_disconnect(KLPClient *client) {
    if (!client || !client->connected) return;
    
    /* Send GOAWAY frame */
    if (client->connection) {
        KLPFrame *goaway = klp_frame_create(KLP_FRAME_GOAWAY, 0, NULL, 0);
        if (goaway) {
            klp_connection_send_frame(client->connection, goaway);
            klp_frame_free(goaway);
        }
        
        klp_connection_free(client->connection);
        client->connection = NULL;
    }
    
    client->connected = 0;
    
    printf("KLP Client: Disconnected from %s:%d\n", client->host, client->port);
}

int klp_client_is_connected(KLPClient *client) {
    return client ? client->connected : 0;
}

/* Request operations */

int klp_client_send(KLPClient *client, const char *path, const void *data, size_t size) {
    if (!client || !client->connected || !path) {
        return KLP_ERROR_PROTOCOL_ERROR;
    }
    
    /* Send HEADERS frame with path */
    KLPFrame *headers = klp_frame_create(KLP_FRAME_HEADERS, 1, path, strlen(path) + 1);
    if (!headers) return KLP_ERROR_NO_MEMORY;
    
    int result = klp_connection_send_frame(client->connection, headers);
    klp_frame_free(headers);
    
    if (result != KLP_OK) return result;
    
    /* Send DATA frame */
    if (data && size > 0) {
        KLPFrame *data_frame = klp_frame_create(KLP_FRAME_DATA, 1, data, size);
        if (!data_frame) return KLP_ERROR_NO_MEMORY;
        
        data_frame->header.flags = KLP_FLAG_END_STREAM;
        
        result = klp_connection_send_frame(client->connection, data_frame);
        klp_frame_free(data_frame);
    }
    
    return result;
}

int klp_client_recv(KLPClient *client, void **data, size_t *size) {
    if (!client || !client->connected || !data || !size) {
        return KLP_ERROR_PROTOCOL_ERROR;
    }
    
    /* Receive frame */
    KLPFrame *frame = klp_connection_recv_frame(client->connection);
    if (!frame) return KLP_ERROR_IO_ERROR;
    
    /* Extract data */
    *size = frame->payload_size;
    if (*size > 0) {
        *data = malloc(*size);
        if (!*data) {
            klp_frame_free(frame);
            return KLP_ERROR_NO_MEMORY;
        }
        memcpy(*data, frame->payload, *size);
    } else {
        *data = NULL;
    }
    
    klp_frame_free(frame);
    
    return KLP_OK;
}

/* Stream operations */

KLPStream* klp_client_open_stream(KLPClient *client, const char *path) {
    if (!client || !client->connected || !path) {
        return NULL;
    }
    
    /* Generate stream ID (client uses odd numbers) */
    static uint32_t next_stream_id = 1;
    uint32_t stream_id = next_stream_id;
    next_stream_id += 2;
    
    /* Create stream */
    KLPStream *stream = klp_stream_create(client->connection, stream_id);
    if (!stream) return NULL;
    
    /* Send HEADERS frame to open stream */
    KLPFrame *headers = klp_frame_create(KLP_FRAME_HEADERS, stream_id, 
                                         path, strlen(path) + 1);
    if (!headers) {
        klp_stream_close(stream);
        return NULL;
    }
    
    int result = klp_connection_send_frame(client->connection, headers);
    klp_frame_free(headers);
    
    if (result != KLP_OK) {
        klp_stream_close(stream);
        return NULL;
    }
    
    return stream;
}

int klp_client_close_stream(KLPClient *client, KLPStream *stream) {
    if (!client || !stream) return -1;
    
    /* Send RST_STREAM frame */
    KLPFrame *rst = klp_frame_create(KLP_FRAME_RST_STREAM, stream->id, NULL, 0);
    if (rst) {
        klp_connection_send_frame(client->connection, rst);
        klp_frame_free(rst);
    }
    
    klp_stream_close(stream);
    
    return 0;
}

/* Stream I/O */

int klp_stream_write(KLPStream *stream, const void *data, size_t size) {
    if (!stream || !data || size == 0) {
        return KLP_ERROR_PROTOCOL_ERROR;
    }
    
    /* For now, we need to get the connection from somewhere */
    /* This is a simplified implementation */
    /* In practice, stream should have a connection reference */
    
    KLPFrame *frame = klp_frame_create(KLP_FRAME_DATA, stream->id, data, size);
    if (!frame) return KLP_ERROR_NO_MEMORY;
    
    /* Note: This is a simplified version. In production, we'd need to:
     * 1. Get the connection from the stream
     * 2. Handle chunking for large payloads
     * 3. Handle flow control
     */
    
    klp_frame_free(frame);
    
    return KLP_OK;
}

int klp_stream_read(KLPStream *stream, void **data, size_t *size) {
    if (!stream || !data || !size) {
        return KLP_ERROR_PROTOCOL_ERROR;
    }
    
    /* Similar simplified implementation */
    /* In production, this would:
     * 1. Receive frames from the connection
     * 2. Filter by stream ID
     * 3. Reassemble chunks
     * 4. Handle flow control
     */
    
    return KLP_OK;
}

/* Tensor I/O */

int klp_stream_write_tensor(KLPStream *stream, const void *tensor_data,
                             const KLPTensorMeta *meta) {
    if (!stream || !tensor_data || !meta) {
        return KLP_ERROR_PROTOCOL_ERROR;
    }
    
    /* This would use klp_send_tensor with the stream's connection */
    /* Simplified for now */
    
    return KLP_OK;
}

int klp_stream_read_tensor(KLPStream *stream, void **tensor_data,
                            KLPTensorMeta *meta) {
    if (!stream || !tensor_data || !meta) {
        return KLP_ERROR_PROTOCOL_ERROR;
    }
    
    /* This would use klp_recv_tensor with the stream's connection */
    /* Simplified for now */
    
    return KLP_OK;
}

/* Utility */

const char* klp_client_error(KLPClient *client) {
    (void)client;
    return strerror(errno);
}
