/*
 * KLP Server Implementation
 * High-performance server for KLP protocol
 */

#include "klp_server.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

/* Initial capacities */
#define INITIAL_ROUTE_CAPACITY 32
#define INITIAL_CONNECTION_CAPACITY 64

/* Forward declarations */
static void* server_thread_func(void *arg);
static void* client_handler_thread(void *arg);
static void handle_client(KLPServer *server, int client_fd);
static KLPRoute* find_route(KLPServer *server, const char *path);

/* Client handler thread argument */
typedef struct {
    KLPServer *server;
    int client_fd;
} ClientHandlerArg;

/* Server lifecycle */

KLPServer* klp_server_create(int port) {
    KLPServer *server = (KLPServer*)calloc(1, sizeof(KLPServer));
    if (!server) return NULL;
    
    server->port = port;
    server->socket_fd = -1;
    server->running = 0;
    
    /* Initialize routes */
    server->routes = (KLPRoute*)calloc(INITIAL_ROUTE_CAPACITY, sizeof(KLPRoute));
    if (!server->routes) {
        free(server);
        return NULL;
    }
    server->route_count = 0;
    server->route_capacity = INITIAL_ROUTE_CAPACITY;
    
    /* Initialize connections */
    server->connections = (KLPConnection**)calloc(INITIAL_CONNECTION_CAPACITY, 
                                                   sizeof(KLPConnection*));
    if (!server->connections) {
        free(server->routes);
        free(server);
        return NULL;
    }
    server->connection_count = 0;
    server->connection_capacity = INITIAL_CONNECTION_CAPACITY;
    pthread_mutex_init(&server->conn_mutex, NULL);
    
    /* Default configuration */
    server->max_frame_size = KLP_MAX_FRAME_SIZE;
    server->window_size = 65535;
    server->enable_compression = 1;
    server->enable_encryption = 0;
    server->enable_ai_optimization = 1;
    server->enable_quantum_features = 0;
    
    /* Callbacks */
    server->on_connect = NULL;
    server->on_disconnect = NULL;
    server->callback_user_data = NULL;
    
    return server;
}

void klp_server_free(KLPServer *server) {
    if (!server) return;
    
    /* Stop server if running */
    if (server->running) {
        klp_server_stop(server);
    }
    
    /* Free routes */
    for (int i = 0; i < server->route_count; i++) {
        free(server->routes[i].path);
    }
    free(server->routes);
    
    /* Free connections */
    pthread_mutex_lock(&server->conn_mutex);
    for (int i = 0; i < server->connection_count; i++) {
        klp_connection_free(server->connections[i]);
    }
    free(server->connections);
    pthread_mutex_unlock(&server->conn_mutex);
    
    pthread_mutex_destroy(&server->conn_mutex);
    free(server);
}

int klp_server_start(KLPServer *server) {
    if (!server || server->running) return -1;
    
    struct sockaddr_in addr;
    int opt = 1;
    
    /* Create socket */
    server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket_fd < 0) {
        perror("KLP: socket creation failed");
        return -1;
    }
    
    /* Set socket options */
    if (setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, 
                   &opt, sizeof(opt)) < 0) {
        perror("KLP: setsockopt failed");
        close(server->socket_fd);
        return -1;
    }
    
    /* Bind */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(server->port);
    
    if (bind(server->socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("KLP: bind failed");
        close(server->socket_fd);
        return -1;
    }
    
    /* Listen */
    if (listen(server->socket_fd, KLP_MAX_CONNECTIONS) < 0) {
        perror("KLP: listen failed");
        close(server->socket_fd);
        return -1;
    }
    
    /* Start server thread */
    server->running = 1;
    if (pthread_create(&server->server_thread, NULL, server_thread_func, server) != 0) {
        perror("KLP: pthread_create failed");
        close(server->socket_fd);
        server->running = 0;
        return -1;
    }
    
    printf("KLP Server started on port %d\n", server->port);
    printf("  - Compression: %s\n", server->enable_compression ? "enabled" : "disabled");
    printf("  - AI Optimization: %s\n", server->enable_ai_optimization ? "enabled" : "disabled");
    printf("  - Quantum Features: %s\n", server->enable_quantum_features ? "enabled" : "disabled");
    
    return 0;
}

void klp_server_stop(KLPServer *server) {
    if (!server || !server->running) return;
    
    server->running = 0;
    
    /* Close server socket to unblock accept() */
    if (server->socket_fd >= 0) {
        close(server->socket_fd);
        server->socket_fd = -1;
    }
    
    /* Wait for server thread */
    pthread_join(server->server_thread, NULL);
    
    /* Close all client connections */
    pthread_mutex_lock(&server->conn_mutex);
    for (int i = 0; i < server->connection_count; i++) {
        klp_connection_free(server->connections[i]);
        server->connections[i] = NULL;
    }
    server->connection_count = 0;
    pthread_mutex_unlock(&server->conn_mutex);
    
    printf("KLP Server stopped\n");
}

int klp_server_is_running(KLPServer *server) {
    return server ? server->running : 0;
}

/* Route management */

void klp_server_route(KLPServer *server, const char *path, KLPRouteHandler handler) {
    klp_server_route_with_data(server, path, handler, NULL);
}

void klp_server_route_with_data(KLPServer *server, const char *path,
                                KLPRouteHandler handler, void *user_data) {
    if (!server || !path || !handler) return;
    
    /* Expand routes array if needed */
    if (server->route_count >= server->route_capacity) {
        int new_capacity = server->route_capacity * 2;
        KLPRoute *new_routes = (KLPRoute*)realloc(server->routes, 
                                                   new_capacity * sizeof(KLPRoute));
        if (!new_routes) return;
        server->routes = new_routes;
        server->route_capacity = new_capacity;
    }
    
    /* Add route */
    KLPRoute *route = &server->routes[server->route_count];
    route->path = strdup(path);
    route->handler = handler;
    route->user_data = user_data;
    server->route_count++;
    
    printf("KLP: Registered route: %s\n", path);
}

/* Connection management */

void klp_server_set_connect_callback(KLPServer *server, KLPConnectionCallback callback) {
    if (server) {
        server->on_connect = callback;
    }
}

void klp_server_set_disconnect_callback(KLPServer *server, KLPConnectionCallback callback) {
    if (server) {
        server->on_disconnect = callback;
    }
}

void klp_server_broadcast(KLPServer *server, const void *data, size_t size) {
    if (!server || !data) return;
    
    KLPFrame *frame = klp_frame_create(KLP_FRAME_DATA, 0, data, size);
    if (!frame) return;
    
    pthread_mutex_lock(&server->conn_mutex);
    for (int i = 0; i < server->connection_count; i++) {
        if (server->connections[i]) {
            klp_connection_send_frame(server->connections[i], frame);
        }
    }
    pthread_mutex_unlock(&server->conn_mutex);
    
    klp_frame_free(frame);
}

/* Configuration */

void klp_server_set_max_frame_size(KLPServer *server, size_t size) {
    if (server) server->max_frame_size = size;
}

void klp_server_set_window_size(KLPServer *server, size_t size) {
    if (server) server->window_size = size;
}

void klp_server_enable_compression(KLPServer *server, int enable) {
    if (server) server->enable_compression = enable;
}

void klp_server_enable_encryption(KLPServer *server, int enable) {
    if (server) server->enable_encryption = enable;
}

void klp_server_enable_ai_optimization(KLPServer *server, int enable) {
    if (server) server->enable_ai_optimization = enable;
}

void klp_server_enable_quantum_features(KLPServer *server, int enable) {
    if (server) server->enable_quantum_features = enable;
}

/* Statistics */

void klp_server_get_stats(KLPServer *server, KLPServerStats *stats) {
    if (!server || !stats) return;
    
    memset(stats, 0, sizeof(KLPServerStats));
    
    pthread_mutex_lock(&server->conn_mutex);
    stats->active_connections = server->connection_count;
    pthread_mutex_unlock(&server->conn_mutex);
    
    /* TODO: Add more detailed statistics */
}

/* Internal functions */

static void* server_thread_func(void *arg) {
    KLPServer *server = (KLPServer*)arg;
    
    while (server->running) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        /* Accept connection */
        int client_fd = accept(server->socket_fd, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_fd < 0) {
            if (server->running) {
                perror("KLP: accept failed");
            }
            continue;
        }
        
        printf("KLP: New connection from client (fd=%d)\n", client_fd);
        
        /* Create client handler thread */
        ClientHandlerArg *arg = (ClientHandlerArg*)malloc(sizeof(ClientHandlerArg));
        if (arg) {
            arg->server = server;
            arg->client_fd = client_fd;
            
            pthread_t thread;
            if (pthread_create(&thread, NULL, client_handler_thread, arg) == 0) {
                pthread_detach(thread);
            } else {
                free(arg);
                close(client_fd);
            }
        } else {
            close(client_fd);
        }
    }
    
    return NULL;
}

static void* client_handler_thread(void *arg) {
    ClientHandlerArg *handler_arg = (ClientHandlerArg*)arg;
    KLPServer *server = handler_arg->server;
    int client_fd = handler_arg->client_fd;
    free(handler_arg);
    
    /* Create connection */
    KLPConnection *conn = klp_connection_create(client_fd, 1);
    if (!conn) {
        close(client_fd);
        return NULL;
    }
    
    /* Add to connections list */
    pthread_mutex_lock(&server->conn_mutex);
    if (server->connection_count < server->connection_capacity) {
        server->connections[server->connection_count++] = conn;
    }
    pthread_mutex_unlock(&server->conn_mutex);
    
    /* Call connect callback */
    if (server->on_connect) {
        server->on_connect(conn, server->callback_user_data);
    }
    
    /* Handle client */
    handle_client(server, client_fd);
    
    /* Call disconnect callback */
    if (server->on_disconnect) {
        server->on_disconnect(conn, server->callback_user_data);
    }
    
    /* Remove from connections list */
    pthread_mutex_lock(&server->conn_mutex);
    for (int i = 0; i < server->connection_count; i++) {
        if (server->connections[i] == conn) {
            server->connections[i] = server->connections[--server->connection_count];
            break;
        }
    }
    pthread_mutex_unlock(&server->conn_mutex);
    
    klp_connection_free(conn);
    printf("KLP: Client disconnected (fd=%d)\n", client_fd);
    
    return NULL;
}

static void handle_client(KLPServer *server, int client_fd) {
    KLPConnection *conn = NULL;
    
    /* Find connection */
    pthread_mutex_lock(&server->conn_mutex);
    for (int i = 0; i < server->connection_count; i++) {
        if (server->connections[i]->socket_fd == client_fd) {
            conn = server->connections[i];
            break;
        }
    }
    pthread_mutex_unlock(&server->conn_mutex);
    
    if (!conn) return;
    
    /* Main message loop */
    while (server->running) {
        KLPFrame *frame = klp_connection_recv_frame(conn);
        if (!frame) break;
        
        /* Handle frame based on type */
        switch (frame->header.type) {
            case KLP_FRAME_HEADERS: {
                /* Extract path from headers (simplified) */
                char *path = (char*)frame->payload;
                KLPRoute *route = find_route(server, path);
                
                if (route && route->handler) {
                    uint32_t stream_id = ntohl(frame->header.stream_id);
                    KLPStream *stream = klp_stream_get(conn, stream_id);
                    
                    if (!stream) {
                        stream = klp_stream_create(conn, stream_id);
                    }
                    
                    if (stream) {
                        stream->user_data = route->user_data;
                        route->handler(stream, route->user_data);
                    }
                }
                break;
            }
            
            case KLP_FRAME_DATA:
            case KLP_FRAME_TENSOR:
            case KLP_FRAME_QUANTUM:
                /* Forward to appropriate handler */
                /* TODO: Implement stream-based routing */
                break;
                
            case KLP_FRAME_PING: {
                /* Send pong */
                KLPFrame *pong = klp_frame_create(KLP_FRAME_PONG, 0, NULL, 0);
                if (pong) {
                    klp_connection_send_frame(conn, pong);
                    klp_frame_free(pong);
                }
                break;
            }
            
            case KLP_FRAME_GOAWAY:
                /* Client wants to disconnect */
                klp_frame_free(frame);
                return;
                
            default:
                break;
        }
        
        klp_frame_free(frame);
    }
}

static KLPRoute* find_route(KLPServer *server, const char *path) {
    if (!server || !path) return NULL;
    
    for (int i = 0; i < server->route_count; i++) {
        if (strcmp(server->routes[i].path, path) == 0) {
            return &server->routes[i];
        }
    }
    
    return NULL;
}
