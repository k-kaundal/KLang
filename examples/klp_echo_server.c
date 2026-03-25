/*
 * KLP Echo Server Example
 * Demonstrates basic KLP protocol usage
 */

#include "klp_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

static KLPServer *g_server = NULL;

/* Signal handler for graceful shutdown */
void signal_handler(int sig) {
    (void)sig;
    if (g_server) {
        printf("\nShutting down KLP server...\n");
        klp_server_stop(g_server);
    }
}

/* Echo handler - echoes back received data */
void echo_handler(KLPStream *stream, void *user_data) {
    (void)user_data;
    printf("KLP: Echo handler called for stream %u\n", stream->id);
    
    /* Note: In a real implementation, we'd read from the stream and echo back */
    /* This is a simplified example showing the structure */
}

/* Data handler - receives data and responds */
void data_handler(KLPStream *stream, void *user_data) {
    (void)user_data;
    printf("KLP: Data handler called for stream %u\n", stream->id);
}

/* Tensor handler - receives AI tensors */
void tensor_handler(KLPStream *stream, void *user_data) {
    (void)user_data;
    printf("KLP: Tensor handler called for stream %u (AI-optimized)\n", stream->id);
}

/* Connection callback */
void on_client_connect(KLPConnection *conn, void *user_data) {
    (void)user_data;
    printf("KLP: Client connected (socket=%d)\n", conn->socket_fd);
}

/* Disconnection callback */
void on_client_disconnect(KLPConnection *conn, void *user_data) {
    (void)user_data;
    printf("KLP: Client disconnected (socket=%d)\n", conn->socket_fd);
}

int main(int argc, char *argv[]) {
    int port = 9000;
    
    /* Parse command line arguments */
    if (argc > 1) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Invalid port number: %s\n", argv[1]);
            return 1;
        }
    }
    
    /* Set up signal handlers */
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    printf("═══════════════════════════════════════════════════\n");
    printf("  KLP (KLang Protocol) Echo Server\n");
    printf("  Advanced AI & Quantum-Ready Protocol\n");
    printf("═══════════════════════════════════════════════════\n\n");
    
    /* Create server */
    g_server = klp_server_create(port);
    if (!g_server) {
        fprintf(stderr, "Failed to create KLP server\n");
        return 1;
    }
    
    /* Configure server */
    klp_server_enable_compression(g_server, 1);
    klp_server_enable_ai_optimization(g_server, 1);
    klp_server_enable_quantum_features(g_server, 0); /* Not yet fully implemented */
    
    /* Register routes */
    klp_server_route(g_server, "/echo", echo_handler);
    klp_server_route(g_server, "/data", data_handler);
    klp_server_route(g_server, "/tensor", tensor_handler);
    
    /* Set callbacks */
    klp_server_set_connect_callback(g_server, on_client_connect);
    klp_server_set_disconnect_callback(g_server, on_client_disconnect);
    
    /* Start server */
    if (klp_server_start(g_server) < 0) {
        fprintf(stderr, "Failed to start KLP server\n");
        klp_server_free(g_server);
        return 1;
    }
    
    printf("\nKLP Server Features:\n");
    printf("  ✓ Binary framing protocol\n");
    printf("  ✓ Multiplexed streams\n");
    printf("  ✓ Adaptive chunking\n");
    printf("  ✓ AI tensor streaming\n");
    printf("  ✓ Zero-copy operations\n");
    printf("  ✓ Compression (zlib)\n");
    printf("  ⏳ Quantum-resistant crypto (coming soon)\n");
    printf("\nAvailable endpoints:\n");
    printf("  - /echo   : Echo server\n");
    printf("  - /data   : Data processing\n");
    printf("  - /tensor : AI tensor streaming\n");
    printf("\nPress Ctrl+C to stop the server...\n\n");
    
    /* Keep server running */
    while (klp_server_is_running(g_server)) {
        sleep(1);
    }
    
    /* Cleanup */
    klp_server_free(g_server);
    g_server = NULL;
    
    printf("\nKLP Server stopped successfully.\n");
    return 0;
}
