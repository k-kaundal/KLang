/*
 * KLP Runtime Bindings for KLang
 * Integrates KLP protocol into KLang runtime
 */

#ifndef KLP_RUNTIME_H
#define KLP_RUNTIME_H

#include "interpreter.h"
#include "klp_server.h"
#include "klp_client.h"

/* Register KLP module with runtime */
void klp_runtime_init(Interpreter *interp);

/* KLP Server value type wrapper */
typedef struct {
    KLPServer *server;
    Value route_handlers;  /* Dictionary of route -> handler function */
} KLPServerWrapper;

/* KLP Client value type wrapper */
typedef struct {
    KLPClient *client;
} KLPClientWrapper;

/* KLP Stream value type wrapper */
typedef struct {
    KLPStream *stream;
    KLPConnection *connection;  /* Reference to connection for I/O */
} KLPStreamWrapper;

/* Built-in functions */

/* klp.server(port) -> server object */
Value builtin_klp_server(Interpreter *interp, Value *args, int argc);

/* klp.connect(url) -> client object */
Value builtin_klp_connect(Interpreter *interp, Value *args, int argc);

/* server.route(path, handler) */
Value builtin_klp_server_route(Interpreter *interp, Value *args, int argc);

/* server.start() */
Value builtin_klp_server_start(Interpreter *interp, Value *args, int argc);

/* server.stop() */
Value builtin_klp_server_stop(Interpreter *interp, Value *args, int argc);

/* server.broadcast(data) */
Value builtin_klp_server_broadcast(Interpreter *interp, Value *args, int argc);

/* client.stream(path) -> stream object */
Value builtin_klp_client_stream(Interpreter *interp, Value *args, int argc);

/* client.close() */
Value builtin_klp_client_close(Interpreter *interp, Value *args, int argc);

/* stream.write(data) */
Value builtin_klp_stream_write(Interpreter *interp, Value *args, int argc);

/* stream.read() -> data */
Value builtin_klp_stream_read(Interpreter *interp, Value *args, int argc);

/* stream.writeTensor(tensor_data) */
Value builtin_klp_stream_write_tensor(Interpreter *interp, Value *args, int argc);

/* stream.readTensor() -> tensor_data */
Value builtin_klp_stream_read_tensor(Interpreter *interp, Value *args, int argc);

#endif /* KLP_RUNTIME_H */
