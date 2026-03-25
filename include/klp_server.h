/*
 * KLP Server (KLang Protocol Server)
 * High-performance server for KLP protocol
 */

#ifndef KLP_SERVER_H
#define KLP_SERVER_H

#include "klp_protocol.h"
#include <pthread.h>

/* Maximum concurrent connections */
#define KLP_MAX_CONNECTIONS 1024

/* Route handler callback */
typedef void (*KLPRouteHandler)(KLPStream *stream, void *user_data);

/* Connection callback */
typedef void (*KLPConnectionCallback)(KLPConnection *conn, void *user_data);

/* Message callback */
typedef void (*KLPMessageCallback)(KLPStream *stream, const void *data, 
                                   size_t size, void *user_data);

/* Route entry */
typedef struct {
    char *path;
    KLPRouteHandler handler;
    void *user_data;
} KLPRoute;

/* Server structure */
typedef struct {
    int port;
    int socket_fd;
    int running;
    pthread_t server_thread;
    
    /* Routes */
    KLPRoute *routes;
    int route_count;
    int route_capacity;
    
    /* Connections */
    KLPConnection **connections;
    int connection_count;
    int connection_capacity;
    pthread_mutex_t conn_mutex;
    
    /* Callbacks */
    KLPConnectionCallback on_connect;
    KLPConnectionCallback on_disconnect;
    void *callback_user_data;
    
    /* Configuration */
    size_t max_frame_size;
    size_t window_size;
    int enable_compression;
    int enable_encryption;
    int enable_ai_optimization;
    int enable_quantum_features;
} KLPServer;

/* Server lifecycle */
KLPServer* klp_server_create(int port);
void klp_server_free(KLPServer *server);
int klp_server_start(KLPServer *server);
void klp_server_stop(KLPServer *server);
int klp_server_is_running(KLPServer *server);

/* Route management */
void klp_server_route(KLPServer *server, const char *path, KLPRouteHandler handler);
void klp_server_route_with_data(KLPServer *server, const char *path, 
                                KLPRouteHandler handler, void *user_data);

/* Connection management */
void klp_server_set_connect_callback(KLPServer *server, KLPConnectionCallback callback);
void klp_server_set_disconnect_callback(KLPServer *server, KLPConnectionCallback callback);
void klp_server_broadcast(KLPServer *server, const void *data, size_t size);

/* Configuration */
void klp_server_set_max_frame_size(KLPServer *server, size_t size);
void klp_server_set_window_size(KLPServer *server, size_t size);
void klp_server_enable_compression(KLPServer *server, int enable);
void klp_server_enable_encryption(KLPServer *server, int enable);
void klp_server_enable_ai_optimization(KLPServer *server, int enable);
void klp_server_enable_quantum_features(KLPServer *server, int enable);

/* Statistics */
typedef struct {
    uint64_t total_connections;
    uint64_t active_connections;
    uint64_t bytes_sent;
    uint64_t bytes_received;
    uint64_t frames_sent;
    uint64_t frames_received;
    uint64_t tensors_sent;
    uint64_t tensors_received;
} KLPServerStats;

void klp_server_get_stats(KLPServer *server, KLPServerStats *stats);

#endif /* KLP_SERVER_H */
