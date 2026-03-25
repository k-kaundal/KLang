#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stddef.h>

/* HTTP methods */
typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_PATCH,
    HTTP_OPTIONS,
    HTTP_HEAD
} HttpMethod;

/* HTTP request structure */
typedef struct {
    HttpMethod method;
    char *path;
    char *query_string;
    char **headers;
    int header_count;
    char *body;
    size_t body_length;
} HttpRequest;

/* HTTP response structure */
typedef struct {
    int status_code;
    char **headers;
    int header_count;
    char *body;
    size_t body_length;
} HttpResponse;

/* Route handler callback */
typedef void (*RouteHandler)(HttpRequest *req, HttpResponse *res);

/* Middleware callback */
typedef int (*Middleware)(HttpRequest *req, HttpResponse *res);

/* HTTP server structure */
typedef struct HttpServer HttpServer;

/* Server lifecycle */
HttpServer* http_server_new(int port);
void http_server_free(HttpServer *server);
int http_server_start(HttpServer *server);
void http_server_stop(HttpServer *server);
int http_server_is_running(HttpServer *server);

/* Route management */
void http_server_get(HttpServer *server, const char *path, RouteHandler handler);
void http_server_post(HttpServer *server, const char *path, RouteHandler handler);
void http_server_put(HttpServer *server, const char *path, RouteHandler handler);
void http_server_delete(HttpServer *server, const char *path, RouteHandler handler);
void http_server_patch(HttpServer *server, const char *path, RouteHandler handler);
void http_server_route(HttpServer *server, HttpMethod method, const char *path, RouteHandler handler);

/* Middleware */
void http_server_use(HttpServer *server, Middleware middleware);

/* Static file serving */
void http_server_static(HttpServer *server, const char *mount_path, const char *directory);

/* Response helpers */
void http_response_set_status(HttpResponse *res, int status_code);
void http_response_set_header(HttpResponse *res, const char *name, const char *value);
void http_response_send(HttpResponse *res, const char *body);
void http_response_json(HttpResponse *res, const char *json);
void http_response_file(HttpResponse *res, const char *filepath);

/* Request helpers */
const char* http_request_get_header(HttpRequest *req, const char *name);
const char* http_request_get_query_param(HttpRequest *req, const char *name);

/* Hot reload support (dev mode) */
void http_server_enable_hot_reload(HttpServer *server, int enable);
void http_server_watch_directory(HttpServer *server, const char *directory);

/* WebSocket support */
void http_server_websocket(HttpServer *server, const char *path, 
                           void (*on_connect)(void *conn),
                           void (*on_message)(void *conn, const char *msg),
                           void (*on_disconnect)(void *conn));

#endif /* HTTP_SERVER_H */
