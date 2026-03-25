/*
 * KLP Client Implementation
 * Client library for KLP protocol
 */

#ifndef KLP_CLIENT_H
#define KLP_CLIENT_H

#include "klp_protocol.h"

/* Client structure */
typedef struct {
    char *host;
    int port;
    KLPConnection *connection;
    int connected;
} KLPClient;

/* Client lifecycle */
KLPClient* klp_client_create(const char *host, int port);
void klp_client_free(KLPClient *client);
int klp_client_connect(KLPClient *client);
void klp_client_disconnect(KLPClient *client);
int klp_client_is_connected(KLPClient *client);

/* Request operations */
int klp_client_send(KLPClient *client, const char *path, const void *data, size_t size);
int klp_client_recv(KLPClient *client, void **data, size_t *size);

/* Stream operations */
KLPStream* klp_client_open_stream(KLPClient *client, const char *path);
int klp_client_close_stream(KLPClient *client, KLPStream *stream);

/* Stream I/O */
int klp_stream_write(KLPStream *stream, const void *data, size_t size);
int klp_stream_read(KLPStream *stream, void **data, size_t *size);

/* Tensor I/O */
int klp_stream_write_tensor(KLPStream *stream, const void *tensor_data, 
                             const KLPTensorMeta *meta);
int klp_stream_read_tensor(KLPStream *stream, void **tensor_data, 
                            KLPTensorMeta *meta);

/* Utility */
const char* klp_client_error(KLPClient *client);

#endif /* KLP_CLIENT_H */
