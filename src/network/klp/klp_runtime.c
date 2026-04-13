/*
 * KLP Runtime Bindings Implementation
 * Integrates KLP protocol into KLang runtime
 */

#include "klp_runtime.h"
#include "runtime/runtime.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Forward declaration */
extern Value eval_block(Interpreter *interp, ASTNode *block, Env *env);

/* Route handler callback wrapper */
static void klp_route_handler_wrapper(KLPStream *stream, void *user_data) {
    /* user_data contains: { interpreter, handler_function, connection } */
    void **data = (void**)user_data;
    Interpreter *interp = (Interpreter*)data[0];
    Value *handler_func = (Value*)data[1];
    KLPConnection *conn = (KLPConnection*)data[2];
    
    /* Create stream wrapper */
    KLPStreamWrapper *wrapper = (KLPStreamWrapper*)malloc(sizeof(KLPStreamWrapper));
    if (!wrapper) return;
    
    wrapper->stream = stream;
    wrapper->connection = conn;
    
    /* Create Value for stream object */
    Value stream_val;
    stream_val.type = VAL_POINTER;
    stream_val.as.pointer_val.ptr = wrapper;
    stream_val.as.pointer_val.type_name = "KLPStream";
    
    /* Call handler function with stream as argument */
    Value args[1] = { stream_val };
    
    /* Call the handler (simplified - proper error handling needed) */
    if (handler_func->type == VAL_FUNCTION) {
        /* In a real implementation, we'd properly call the function */
        printf("KLP: Handler called for stream %u\n", stream->id);
    } else if (handler_func->type == VAL_BUILTIN) {
        handler_func->as.builtin(interp, args, 1);
    }
}

/* Register KLP module with runtime */
void klp_runtime_init(Interpreter *interp) {
    if (!interp) return;
    
    /* In a full implementation, we would:
     * 1. Create a "klp" module/namespace
     * 2. Register all built-in functions in that namespace
     * 3. Make it available to KLang scripts via import or global
     */
    
    printf("KLP Runtime: Initialized\n");
}

/* klp.server(port) */
Value builtin_klp_server(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    
    if (argc < 1 || args[0].type != VAL_INT) {
        fprintf(stderr, "klp.server() requires port number\n");
        return make_null();
    }
    
    int port = (int)args[0].as.int_val;
    
    /* Create KLP server */
    KLPServer *server = klp_server_create(port);
    if (!server) {
        fprintf(stderr, "Failed to create KLP server\n");
        return make_null();
    }
    
    /* Wrap server in a struct */
    KLPServerWrapper *wrapper = (KLPServerWrapper*)malloc(sizeof(KLPServerWrapper));
    if (!wrapper) {
        klp_server_free(server);
        return make_null();
    }
    
    wrapper->server = server;
    wrapper->route_handlers.type = VAL_NULL;  /* Would be a dict in full impl */
    
    /* Return as pointer value (in full implementation, would be custom object) */
    Value result;
    result.type = VAL_POINTER;
    result.as.pointer_val.ptr = wrapper;
    result.as.pointer_val.type_name = "KLPServer";
    
    return result;
}

/* klp.connect(url) */
Value builtin_klp_connect(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    
    if (argc < 1 || args[0].type != VAL_STRING) {
        fprintf(stderr, "klp.connect() requires URL string\n");
        return make_null();
    }
    
    const char *url = args[0].as.str_val;
    
    /* Parse URL (simplified - assumes klp://host:port format) */
    char host[256] = "127.0.0.1";
    int port = 9000;
    
    if (strncmp(url, "klp://", 6) == 0) {
        const char *addr = url + 6;
        char *colon = strchr(addr, ':');
        if (colon) {
            size_t host_len = colon - addr;
            if (host_len < sizeof(host)) {
                strncpy(host, addr, host_len);
                host[host_len] = '\0';
            }
            port = atoi(colon + 1);
        }
    }
    
    /* Create client */
    KLPClient *client = klp_client_create(host, port);
    if (!client) {
        fprintf(stderr, "Failed to create KLP client\n");
        return make_null();
    }
    
    /* Connect */
    if (klp_client_connect(client) != 0) {
        fprintf(stderr, "Failed to connect to %s:%d\n", host, port);
        klp_client_free(client);
        return make_null();
    }
    
    /* Wrap client */
    KLPClientWrapper *wrapper = (KLPClientWrapper*)malloc(sizeof(KLPClientWrapper));
    if (!wrapper) {
        klp_client_free(client);
        return make_null();
    }
    
    wrapper->client = client;
    
    /* Return as pointer value */
    Value result;
    result.type = VAL_POINTER;
    result.as.pointer_val.ptr = wrapper;
    result.as.pointer_val.type_name = "KLPClient";
    
    return result;
}

/* server.route(path, handler) */
Value builtin_klp_server_route(Interpreter *interp, Value *args, int argc) {
    if (argc < 3) {
        fprintf(stderr, "server.route() requires server, path, and handler\n");
        return make_null();
    }
    
    if (args[0].type != VAL_POINTER) {
        fprintf(stderr, "Invalid server object\n");
        return make_null();
    }
    
    if (args[1].type != VAL_STRING) {
        fprintf(stderr, "Route path must be a string\n");
        return make_null();
    }
    
    KLPServerWrapper *wrapper = (KLPServerWrapper*)args[0].as.pointer_val.ptr;
    const char *path = args[1].as.str_val;
    Value handler = args[2];
    
    /* Store handler (simplified - would use proper storage) */
    Value *handler_copy = (Value*)malloc(sizeof(Value));
    if (handler_copy) {
        *handler_copy = handler;
        
        /* Prepare callback data */
        void **callback_data = (void**)malloc(3 * sizeof(void*));
        callback_data[0] = interp;
        callback_data[1] = handler_copy;
        callback_data[2] = NULL;  /* Connection added at runtime */
        
        /* Register route */
        klp_server_route_with_data(wrapper->server, path, 
                                   klp_route_handler_wrapper, callback_data);
    }
    
    return make_null();
}

/* server.start() */
Value builtin_klp_server_start(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    
    if (argc < 1 || args[0].type != VAL_POINTER) {
        fprintf(stderr, "Invalid server object\n");
        return make_null();
    }
    
    KLPServerWrapper *wrapper = (KLPServerWrapper*)args[0].as.pointer_val.ptr;
    
    if (klp_server_start(wrapper->server) != 0) {
        fprintf(stderr, "Failed to start KLP server\n");
        return make_bool(0);
    }
    
    return make_bool(1);
}

/* server.stop() */
Value builtin_klp_server_stop(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    
    if (argc < 1 || args[0].type != VAL_POINTER) {
        fprintf(stderr, "Invalid server object\n");
        return make_null();
    }
    
    KLPServerWrapper *wrapper = (KLPServerWrapper*)args[0].as.pointer_val.ptr;
    
    klp_server_stop(wrapper->server);
    
    return make_null();
}

/* server.broadcast(data) */
Value builtin_klp_server_broadcast(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    
    if (argc < 2) {
        fprintf(stderr, "server.broadcast() requires server and data\n");
        return make_null();
    }
    
    if (args[0].type != VAL_POINTER) {
        fprintf(stderr, "Invalid server object\n");
        return make_null();
    }
    
    KLPServerWrapper *wrapper = (KLPServerWrapper*)args[0].as.pointer_val.ptr;
    
    /* Convert data to string for now */
    const char *data = NULL;
    size_t data_len = 0;
    
    if (args[1].type == VAL_STRING) {
        data = args[1].as.str_val;
        data_len = strlen(data);
    }
    
    if (data) {
        klp_server_broadcast(wrapper->server, data, data_len);
    }
    
    return make_null();
}

/* client.stream(path) */
Value builtin_klp_client_stream(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    
    if (argc < 2) {
        fprintf(stderr, "client.stream() requires client and path\n");
        return make_null();
    }
    
    if (args[0].type != VAL_POINTER) {
        fprintf(stderr, "Invalid client object\n");
        return make_null();
    }
    
    if (args[1].type != VAL_STRING) {
        fprintf(stderr, "Stream path must be a string\n");
        return make_null();
    }
    
    KLPClientWrapper *wrapper = (KLPClientWrapper*)args[0].as.pointer_val.ptr;
    const char *path = args[1].as.str_val;
    
    /* Open stream */
    KLPStream *stream = klp_client_open_stream(wrapper->client, path);
    if (!stream) {
        fprintf(stderr, "Failed to open stream\n");
        return make_null();
    }
    
    /* Wrap stream */
    KLPStreamWrapper *stream_wrapper = (KLPStreamWrapper*)malloc(sizeof(KLPStreamWrapper));
    if (!stream_wrapper) {
        return make_null();
    }
    
    stream_wrapper->stream = stream;
    stream_wrapper->connection = wrapper->client->connection;
    
    /* Return as pointer value */
    Value result;
    result.type = VAL_POINTER;
    result.as.pointer_val.ptr = stream_wrapper;
    result.as.pointer_val.type_name = "KLPStream";
    
    return result;
}

/* client.close() */
Value builtin_klp_client_close(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    
    if (argc < 1 || args[0].type != VAL_POINTER) {
        fprintf(stderr, "Invalid client object\n");
        return make_null();
    }
    
    KLPClientWrapper *wrapper = (KLPClientWrapper*)args[0].as.pointer_val.ptr;
    
    klp_client_disconnect(wrapper->client);
    klp_client_free(wrapper->client);
    free(wrapper);
    
    return make_null();
}

/* stream.write(data) */
Value builtin_klp_stream_write(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    
    if (argc < 2) {
        fprintf(stderr, "stream.write() requires stream and data\n");
        return make_null();
    }
    
    if (args[0].type != VAL_POINTER) {
        fprintf(stderr, "Invalid stream object\n");
        return make_null();
    }
    
    KLPStreamWrapper *wrapper = (KLPStreamWrapper*)args[0].as.pointer_val.ptr;
    
    /* Convert data to string/bytes */
    const char *data = NULL;
    size_t data_len = 0;
    
    if (args[1].type == VAL_STRING) {
        data = args[1].as.str_val;
        data_len = strlen(data);
    }
    
    if (data && wrapper->connection) {
        /* Send data frame */
        KLPFrame *frame = klp_frame_create(KLP_FRAME_DATA, wrapper->stream->id, 
                                          data, data_len);
        if (frame) {
            klp_connection_send_frame(wrapper->connection, frame);
            klp_frame_free(frame);
        }
    }
    
    return make_null();
}

/* stream.read() */
Value builtin_klp_stream_read(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    
    if (argc < 1 || args[0].type != VAL_POINTER) {
        fprintf(stderr, "Invalid stream object\n");
        return make_null();
    }
    
    KLPStreamWrapper *wrapper = (KLPStreamWrapper*)args[0].as.pointer_val.ptr;
    
    if (!wrapper->connection) {
        return make_null();
    }
    
    /* Receive frame */
    KLPFrame *frame = klp_connection_recv_frame(wrapper->connection);
    if (!frame) {
        return make_null();
    }
    
    /* Convert payload to string */
    Value result;
    if (frame->payload && frame->payload_size > 0) {
        char *str = (char*)malloc(frame->payload_size + 1);
        if (str) {
            memcpy(str, frame->payload, frame->payload_size);
            str[frame->payload_size] = '\0';
            result = make_string(str);
            free(str);
        } else {
            result = make_null();
        }
    } else {
        result = make_null();
    }
    
    klp_frame_free(frame);
    
    return result;
}

/* stream.writeTensor(tensor_data) */
Value builtin_klp_stream_write_tensor(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    (void)args;  /* Unused parameter */
    
    if (argc < 2) {
        fprintf(stderr, "stream.writeTensor() requires stream and tensor\n");
        return make_null();
    }
    
    /* Tensor support would require:
     * 1. Tensor type in KLang (VAL_TENSOR)
     * 2. Metadata extraction (shape, dtype)
     * 3. Data conversion
     */
    
    printf("KLP: Tensor write not yet fully implemented\n");
    
    return make_null();
}

/* stream.readTensor() */
Value builtin_klp_stream_read_tensor(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    (void)args;  /* Unused parameter */
    
    if (argc < 1) {
        fprintf(stderr, "stream.readTensor() requires stream\n");
        return make_null();
    }
    
    /* Tensor support would require:
     * 1. Receiving tensor metadata
     * 2. Receiving tensor data
     * 3. Creating KLang tensor object
     */
    
    printf("KLP: Tensor read not yet fully implemented\n");
    
    return make_null();
}
