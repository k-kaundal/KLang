#include "http_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_ROUTES 256
#define MAX_MIDDLEWARE 32
#define MAX_BUFFER 8192

/* Route entry */
typedef struct {
    HttpMethod method;
    char *path;
    RouteHandler handler;
} Route;

/* HTTP Server implementation */
struct HttpServer {
    int port;
    int socket_fd;
    int running;
    Route routes[MAX_ROUTES];
    int route_count;
    Middleware middleware[MAX_MIDDLEWARE];
    int middleware_count;
    char *static_mount;
    char *static_directory;
    int enable_hot_reload;
    pthread_t server_thread;
};

/* Forward declarations */
static void* server_thread_func(void *arg);
static void handle_client(HttpServer *server, int client_fd);
static HttpRequest* parse_request(const char *raw_request);
static void send_response(int client_fd, HttpResponse *res);
static void free_request(HttpRequest *req);
static void free_response(HttpResponse *res);
static Route* find_route(HttpServer *server, HttpMethod method, const char *path);
static const char* method_to_string(HttpMethod method);
static HttpMethod string_to_method(const char *str);

/* Server lifecycle */
HttpServer* http_server_new(int port) {
    HttpServer *server = (HttpServer*)calloc(1, sizeof(HttpServer));
    if (!server) return NULL;
    
    server->port = port;
    server->socket_fd = -1;
    server->running = 0;
    server->route_count = 0;
    server->middleware_count = 0;
    server->static_mount = NULL;
    server->static_directory = NULL;
    server->enable_hot_reload = 0;
    
    return server;
}

void http_server_free(HttpServer *server) {
    if (!server) return;
    
    if (server->running) {
        http_server_stop(server);
    }
    
    /* Free routes */
    for (int i = 0; i < server->route_count; i++) {
        free(server->routes[i].path);
    }
    
    /* Free static paths */
    if (server->static_mount) free(server->static_mount);
    if (server->static_directory) free(server->static_directory);
    
    free(server);
}

int http_server_start(HttpServer *server) {
    if (!server || server->running) return -1;
    
    struct sockaddr_in addr;
    int opt = 1;
    
    /* Create socket */
    server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket_fd < 0) {
        perror("socket creation failed");
        return -1;
    }
    
    /* Set socket options */
    if (setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server->socket_fd);
        return -1;
    }
    
    /* Bind socket */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(server->port);
    
    if (bind(server->socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        close(server->socket_fd);
        return -1;
    }
    
    /* Listen */
    if (listen(server->socket_fd, 10) < 0) {
        perror("listen failed");
        close(server->socket_fd);
        return -1;
    }
    
    server->running = 1;
    
    /* Start server thread */
    if (pthread_create(&server->server_thread, NULL, server_thread_func, server) != 0) {
        perror("thread creation failed");
        close(server->socket_fd);
        server->running = 0;
        return -1;
    }
    
    printf("HTTP Server listening on port %d\n", server->port);
    return 0;
}

void http_server_stop(HttpServer *server) {
    if (!server || !server->running) return;
    
    server->running = 0;
    
    if (server->socket_fd >= 0) {
        close(server->socket_fd);
        server->socket_fd = -1;
    }
    
    pthread_join(server->server_thread, NULL);
    printf("HTTP Server stopped\n");
}

int http_server_is_running(HttpServer *server) {
    return server ? server->running : 0;
}

/* Route management */
void http_server_get(HttpServer *server, const char *path, RouteHandler handler) {
    http_server_route(server, HTTP_GET, path, handler);
}

void http_server_post(HttpServer *server, const char *path, RouteHandler handler) {
    http_server_route(server, HTTP_POST, path, handler);
}

void http_server_put(HttpServer *server, const char *path, RouteHandler handler) {
    http_server_route(server, HTTP_PUT, path, handler);
}

void http_server_delete(HttpServer *server, const char *path, RouteHandler handler) {
    http_server_route(server, HTTP_DELETE, path, handler);
}

void http_server_patch(HttpServer *server, const char *path, RouteHandler handler) {
    http_server_route(server, HTTP_PATCH, path, handler);
}

void http_server_route(HttpServer *server, HttpMethod method, const char *path, RouteHandler handler) {
    if (!server || !path || !handler || server->route_count >= MAX_ROUTES) return;
    
    server->routes[server->route_count].method = method;
    server->routes[server->route_count].path = strdup(path);
    server->routes[server->route_count].handler = handler;
    server->route_count++;
}

/* Middleware */
void http_server_use(HttpServer *server, Middleware middleware) {
    if (!server || !middleware || server->middleware_count >= MAX_MIDDLEWARE) return;
    
    server->middleware[server->middleware_count++] = middleware;
}

/* Static file serving */
void http_server_static(HttpServer *server, const char *mount_path, const char *directory) {
    if (!server || !mount_path || !directory) return;
    
    if (server->static_mount) free(server->static_mount);
    if (server->static_directory) free(server->static_directory);
    
    server->static_mount = strdup(mount_path);
    server->static_directory = strdup(directory);
}

/* Response helpers */
void http_response_set_status(HttpResponse *res, int status_code) {
    if (res) res->status_code = status_code;
}

void http_response_set_header(HttpResponse *res, const char *name, const char *value) {
    if (!res || !name || !value) return;
    
    /* Allocate header array if needed */
    if (res->headers == NULL) {
        res->headers = (char**)malloc(sizeof(char*) * 32);
        res->header_count = 0;
    }
    
    /* Format: "Name: Value" */
    char *header = (char*)malloc(strlen(name) + strlen(value) + 3);
    sprintf(header, "%s: %s", name, value);
    res->headers[res->header_count++] = header;
}

void http_response_send(HttpResponse *res, const char *body) {
    if (!res || !body) return;
    
    res->body = strdup(body);
    res->body_length = strlen(body);
    http_response_set_header(res, "Content-Type", "text/plain");
}

void http_response_json(HttpResponse *res, const char *json) {
    if (!res || !json) return;
    
    res->body = strdup(json);
    res->body_length = strlen(json);
    http_response_set_header(res, "Content-Type", "application/json");
}

void http_response_file(HttpResponse *res, const char *filepath) {
    if (!res || !filepath) return;
    
    FILE *f = fopen(filepath, "rb");
    if (!f) {
        http_response_set_status(res, 404);
        http_response_send(res, "File not found");
        return;
    }
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    res->body = (char*)malloc(size);
    res->body_length = fread(res->body, 1, size, f);
    fclose(f);
    
    /* Determine content type based on extension */
    const char *ext = strrchr(filepath, '.');
    if (ext) {
        if (strcmp(ext, ".html") == 0) http_response_set_header(res, "Content-Type", "text/html");
        else if (strcmp(ext, ".css") == 0) http_response_set_header(res, "Content-Type", "text/css");
        else if (strcmp(ext, ".js") == 0) http_response_set_header(res, "Content-Type", "application/javascript");
        else if (strcmp(ext, ".json") == 0) http_response_set_header(res, "Content-Type", "application/json");
        else if (strcmp(ext, ".png") == 0) http_response_set_header(res, "Content-Type", "image/png");
        else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) http_response_set_header(res, "Content-Type", "image/jpeg");
        else http_response_set_header(res, "Content-Type", "application/octet-stream");
    }
}

/* Request helpers */
const char* http_request_get_header(HttpRequest *req, const char *name) {
    if (!req || !name) return NULL;
    
    for (int i = 0; i < req->header_count; i++) {
        if (strncmp(req->headers[i], name, strlen(name)) == 0) {
            return req->headers[i] + strlen(name) + 2; /* Skip ": " */
        }
    }
    
    return NULL;
}

const char* http_request_get_query_param(HttpRequest *req, const char *name) {
    if (!req || !name || !req->query_string) return NULL;
    
    /* Simple query string parsing */
    char *query = strdup(req->query_string);
    char *token = strtok(query, "&");
    const char *result = NULL;
    
    while (token) {
        char *eq = strchr(token, '=');
        if (eq) {
            *eq = '\0';
            if (strcmp(token, name) == 0) {
                result = strdup(eq + 1);
                break;
            }
        }
        token = strtok(NULL, "&");
    }
    
    free(query);
    return result;
}

/* Hot reload support */
void http_server_enable_hot_reload(HttpServer *server, int enable) {
    if (server) server->enable_hot_reload = enable;
}

void http_server_watch_directory(HttpServer *server, const char *directory) {
    /* TODO: Implement file watching with inotify/kqueue */
    (void)server;
    (void)directory;
}

/* WebSocket support */
void http_server_websocket(HttpServer *server, const char *path,
                           void (*on_connect)(void *conn),
                           void (*on_message)(void *conn, const char *msg),
                           void (*on_disconnect)(void *conn)) {
    /* TODO: Implement WebSocket support */
    (void)server;
    (void)path;
    (void)on_connect;
    (void)on_message;
    (void)on_disconnect;
}

/* Internal functions */
static void* server_thread_func(void *arg) {
    HttpServer *server = (HttpServer*)arg;
    
    while (server->running) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_fd = accept(server->socket_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            if (server->running) {
                perror("accept failed");
            }
            continue;
        }
        
        handle_client(server, client_fd);
        close(client_fd);
    }
    
    return NULL;
}

static void handle_client(HttpServer *server, int client_fd) {
    char buffer[MAX_BUFFER];
    ssize_t bytes_read = read(client_fd, buffer, MAX_BUFFER - 1);
    
    if (bytes_read <= 0) return;
    
    buffer[bytes_read] = '\0';
    
    HttpRequest *req = parse_request(buffer);
    if (!req) return;
    
    HttpResponse res = {0};
    res.status_code = 200;
    res.headers = NULL;
    res.header_count = 0;
    res.body = NULL;
    res.body_length = 0;
    
    /* Run middleware */
    int middleware_result = 1;
    for (int i = 0; i < server->middleware_count && middleware_result; i++) {
        middleware_result = server->middleware[i](req, &res);
    }
    
    if (middleware_result) {
        /* Find and execute route handler */
        Route *route = find_route(server, req->method, req->path);
        if (route) {
            route->handler(req, &res);
        } else {
            /* Check static files */
            if (server->static_mount && strncmp(req->path, server->static_mount, strlen(server->static_mount)) == 0) {
                char filepath[1024];
                snprintf(filepath, sizeof(filepath), "%s%s", server->static_directory, 
                        req->path + strlen(server->static_mount));
                http_response_file(&res, filepath);
            } else {
                res.status_code = 404;
                http_response_send(&res, "Not Found");
            }
        }
    }
    
    send_response(client_fd, &res);
    
    free_request(req);
    free_response(&res);
}

static HttpRequest* parse_request(const char *raw_request) {
    HttpRequest *req = (HttpRequest*)calloc(1, sizeof(HttpRequest));
    if (!req) return NULL;
    
    char *request_copy = strdup(raw_request);
    char *line = strtok(request_copy, "\r\n");
    
    if (line) {
        /* Parse request line */
        char method_str[16];
        char path_str[1024];
        sscanf(line, "%s %s", method_str, path_str);
        
        req->method = string_to_method(method_str);
        
        /* Parse path and query string */
        char *query = strchr(path_str, '?');
        if (query) {
            *query = '\0';
            req->query_string = strdup(query + 1);
        }
        req->path = strdup(path_str);
        
        /* Parse headers */
        req->headers = (char**)malloc(sizeof(char*) * 32);
        req->header_count = 0;
        
        while ((line = strtok(NULL, "\r\n")) && line[0] != '\0') {
            req->headers[req->header_count++] = strdup(line);
        }
    }
    
    free(request_copy);
    return req;
}

static void send_response(int client_fd, HttpResponse *res) {
    char buffer[MAX_BUFFER];
    int offset = 0;
    
    /* Status line */
    offset += snprintf(buffer + offset, MAX_BUFFER - offset, 
                      "HTTP/1.1 %d %s\r\n", res->status_code,
                      res->status_code == 200 ? "OK" : 
                      res->status_code == 404 ? "Not Found" : "Error");
    
    /* Headers */
    for (int i = 0; i < res->header_count; i++) {
        offset += snprintf(buffer + offset, MAX_BUFFER - offset, 
                          "%s\r\n", res->headers[i]);
    }
    
    /* Content-Length */
    if (res->body) {
        offset += snprintf(buffer + offset, MAX_BUFFER - offset, 
                          "Content-Length: %zu\r\n", res->body_length);
    }
    
    offset += snprintf(buffer + offset, MAX_BUFFER - offset, "\r\n");
    
    /* Send headers */
    write(client_fd, buffer, offset);
    
    /* Send body */
    if (res->body) {
        write(client_fd, res->body, res->body_length);
    }
}

static void free_request(HttpRequest *req) {
    if (!req) return;
    
    if (req->path) free(req->path);
    if (req->query_string) free(req->query_string);
    if (req->body) free(req->body);
    
    if (req->headers) {
        for (int i = 0; i < req->header_count; i++) {
            free(req->headers[i]);
        }
        free(req->headers);
    }
    
    free(req);
}

static void free_response(HttpResponse *res) {
    if (!res) return;
    
    if (res->body) free(res->body);
    
    if (res->headers) {
        for (int i = 0; i < res->header_count; i++) {
            free(res->headers[i]);
        }
        free(res->headers);
    }
}

static Route* find_route(HttpServer *server, HttpMethod method, const char *path) {
    for (int i = 0; i < server->route_count; i++) {
        if (server->routes[i].method == method && strcmp(server->routes[i].path, path) == 0) {
            return &server->routes[i];
        }
    }
    return NULL;
}

static const char* method_to_string(HttpMethod method) {
    switch (method) {
        case HTTP_GET: return "GET";
        case HTTP_POST: return "POST";
        case HTTP_PUT: return "PUT";
        case HTTP_DELETE: return "DELETE";
        case HTTP_PATCH: return "PATCH";
        case HTTP_OPTIONS: return "OPTIONS";
        case HTTP_HEAD: return "HEAD";
        default: return "UNKNOWN";
    }
}

static HttpMethod string_to_method(const char *str) {
    if (strcmp(str, "GET") == 0) return HTTP_GET;
    if (strcmp(str, "POST") == 0) return HTTP_POST;
    if (strcmp(str, "PUT") == 0) return HTTP_PUT;
    if (strcmp(str, "DELETE") == 0) return HTTP_DELETE;
    if (strcmp(str, "PATCH") == 0) return HTTP_PATCH;
    if (strcmp(str, "OPTIONS") == 0) return HTTP_OPTIONS;
    if (strcmp(str, "HEAD") == 0) return HTTP_HEAD;
    return HTTP_GET;
}
