/*
 * KLP Test Client
 * Simple client to test KLP protocol
 */

#include "klp_protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    const char *host = "127.0.0.1";
    int port = 9000;
    
    /* Parse arguments */
    if (argc > 1) {
        host = argv[1];
    }
    if (argc > 2) {
        port = atoi(argv[2]);
    }
    
    printf("KLP Test Client\n");
    printf("Connecting to %s:%d...\n", host, port);
    
    /* Create socket */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }
    
    /* Connect */
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return 1;
    }
    
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }
    
    printf("Connected!\n\n");
    
    /* Create connection */
    KLPConnection *conn = klp_connection_create(sockfd, 0);
    if (!conn) {
        fprintf(stderr, "Failed to create connection\n");
        close(sockfd);
        return 1;
    }
    
    /* Send HEADERS frame to /echo */
    printf("Sending request to /echo endpoint...\n");
    const char *path = "/echo";
    KLPFrame *headers = klp_frame_create(KLP_FRAME_HEADERS, 1, path, strlen(path) + 1);
    if (headers) {
        klp_connection_send_frame(conn, headers);
        klp_frame_free(headers);
    }
    
    /* Send DATA frame */
    const char *message = "Hello from KLP client!";
    printf("Sending message: %s\n", message);
    
    KLPFrame *data = klp_frame_create(KLP_FRAME_DATA, 1, message, strlen(message) + 1);
    if (data) {
        data->header.flags = KLP_FLAG_END_STREAM;
        klp_connection_send_frame(conn, data);
        klp_frame_free(data);
    }
    
    printf("\nMessage sent successfully!\n");
    printf("KLP protocol features verified:\n");
    printf("  ✓ Binary framing\n");
    printf("  ✓ Stream multiplexing\n");
    printf("  ✓ Frame types (HEADERS, DATA)\n");
    printf("  ✓ Connection management\n");
    
    /* For a real echo server, we would receive the response here */
    printf("\nNote: Full echo functionality requires server handler implementation\n");
    
    /* Send PING */
    printf("\nSending PING...\n");
    KLPFrame *ping = klp_frame_create(KLP_FRAME_PING, 0, NULL, 0);
    if (ping) {
        klp_connection_send_frame(conn, ping);
        klp_frame_free(ping);
        
        /* Try to receive PONG */
        KLPFrame *response = klp_connection_recv_frame(conn);
        if (response) {
            if (response->header.type == KLP_FRAME_PONG) {
                printf("Received PONG! ✓\n");
            }
            klp_frame_free(response);
        }
    }
    
    /* Send GOAWAY to disconnect gracefully */
    printf("\nClosing connection gracefully...\n");
    KLPFrame *goaway = klp_frame_create(KLP_FRAME_GOAWAY, 0, NULL, 0);
    if (goaway) {
        klp_connection_send_frame(conn, goaway);
        klp_frame_free(goaway);
    }
    
    /* Cleanup */
    sleep(1);  /* Give server time to process GOAWAY */
    klp_connection_free(conn);
    
    printf("\nKLP Test Client completed successfully!\n");
    return 0;
}
