#include "lsp_server.h"
#include "lexer.h"
#include "parser.h"
#include "cli_colors.h"
#include <stdarg.h>
#include <ctype.h>

/* Initialize LSP server */
LSPServer* lsp_server_init(void) {
    LSPServer *server = malloc(sizeof(LSPServer));
    
    if (!server) {
        return NULL;
    }
    
    server->initialized = 0;
    server->shutdown_requested = 0;
    server->root_uri = NULL;
    
    /* Open log file */
    server->log_file = fopen("/tmp/klang-lsp.log", "w");
    if (server->log_file) {
        setvbuf(server->log_file, NULL, _IONBF, 0); /* Unbuffered */
    }
    
    return server;
}

/* Free LSP server */
void lsp_server_free(LSPServer *server) {
    if (!server) return;
    
    free(server->root_uri);
    
    if (server->log_file) {
        fclose(server->log_file);
    }
    
    free(server);
}

/* Log message to file */
void lsp_log(LSPServer *server, const char *format, ...) {
    if (!server || !server->log_file) return;
    
    va_list args;
    va_start(args, format);
    vfprintf(server->log_file, format, args);
    fprintf(server->log_file, "\n");
    va_end(args);
}

/* Read LSP message from stdin */
char* lsp_read_message(void) {
    char header[256];
    int content_length = 0;
    
    /* Read headers */
    while (fgets(header, sizeof(header), stdin)) {
        /* Check for Content-Length header */
        if (strncmp(header, "Content-Length: ", 16) == 0) {
            content_length = atoi(header + 16);
        }
        
        /* Empty line marks end of headers */
        if (strcmp(header, "\r\n") == 0 || strcmp(header, "\n") == 0) {
            break;
        }
    }
    
    if (content_length <= 0) {
        return NULL;
    }
    
    /* Read content */
    char *content = malloc(content_length + 1);
    if (!content) {
        return NULL;
    }
    
    size_t bytes_read = fread(content, 1, content_length, stdin);
    content[bytes_read] = '\0';
    
    return content;
}

/* Send LSP response */
void lsp_send_response(int id, const char *result) {
    char *response;
    int length;
    
    /* Build JSON response */
    if (result) {
        length = asprintf(&response, 
            "{\"jsonrpc\":\"2.0\",\"id\":%d,\"result\":%s}",
            id, result);
    } else {
        length = asprintf(&response,
            "{\"jsonrpc\":\"2.0\",\"id\":%d,\"result\":null}",
            id);
    }
    
    if (length < 0) return;
    
    /* Send with Content-Length header */
    printf("Content-Length: %d\r\n\r\n%s", length, response);
    fflush(stdout);
    
    free(response);
}

/* Send LSP notification */
void lsp_send_notification(const char *method, const char *params) {
    char *notification;
    int length;
    
    if (params) {
        length = asprintf(&notification,
            "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":%s}",
            method, params);
    } else {
        length = asprintf(&notification,
            "{\"jsonrpc\":\"2.0\",\"method\":\"%s\"}",
            method);
    }
    
    if (length < 0) return;
    
    printf("Content-Length: %d\r\n\r\n%s", length, notification);
    fflush(stdout);
    
    free(notification);
}

/* Send LSP error */
void lsp_send_error(int id, int code, const char *message) {
    char *error;
    int length;
    
    length = asprintf(&error,
        "{\"jsonrpc\":\"2.0\",\"id\":%d,\"error\":{\"code\":%d,\"message\":\"%s\"}}",
        id, code, message);
    
    if (length < 0) return;
    
    printf("Content-Length: %d\r\n\r\n%s", length, error);
    fflush(stdout);
    
    free(error);
}

/* Parse message type from method name */
LSPMessageType lsp_parse_message_type(const char *method) {
    if (strcmp(method, "initialize") == 0) {
        return LSP_INITIALIZE;
    } else if (strcmp(method, "initialized") == 0) {
        return LSP_INITIALIZED;
    } else if (strcmp(method, "shutdown") == 0) {
        return LSP_SHUTDOWN;
    } else if (strcmp(method, "exit") == 0) {
        return LSP_EXIT;
    } else if (strcmp(method, "textDocument/didOpen") == 0) {
        return LSP_TEXT_DOCUMENT_DID_OPEN;
    } else if (strcmp(method, "textDocument/didChange") == 0) {
        return LSP_TEXT_DOCUMENT_DID_CHANGE;
    } else if (strcmp(method, "textDocument/didSave") == 0) {
        return LSP_TEXT_DOCUMENT_DID_SAVE;
    } else if (strcmp(method, "textDocument/didClose") == 0) {
        return LSP_TEXT_DOCUMENT_DID_CLOSE;
    } else if (strcmp(method, "textDocument/completion") == 0) {
        return LSP_TEXT_DOCUMENT_COMPLETION;
    } else if (strcmp(method, "textDocument/hover") == 0) {
        return LSP_TEXT_DOCUMENT_HOVER;
    } else if (strcmp(method, "textDocument/definition") == 0) {
        return LSP_TEXT_DOCUMENT_DEFINITION;
    } else if (strcmp(method, "textDocument/references") == 0) {
        return LSP_TEXT_DOCUMENT_REFERENCES;
    } else if (strcmp(method, "textDocument/formatting") == 0) {
        return LSP_TEXT_DOCUMENT_FORMATTING;
    }
    
    return LSP_UNKNOWN;
}

/* Handle initialize request */
void lsp_handle_initialize(LSPServer *server, int id, const char *params) {
    lsp_log(server, "Handling initialize request");
    
    server->initialized = 1;
    
    /* Send capabilities */
    const char *result = 
        "{"
        "\"capabilities\":{"
        "\"textDocumentSync\":1,"
        "\"completionProvider\":{\"triggerCharacters\":[\".\",\":\"]},"
        "\"hoverProvider\":true,"
        "\"definitionProvider\":true,"
        "\"referencesProvider\":true,"
        "\"documentFormattingProvider\":true,"
        "\"documentSymbolProvider\":true"
        "}"
        "}";
    
    lsp_send_response(id, result);
}

/* Handle textDocument/didOpen */
void lsp_handle_text_document_did_open(LSPServer *server, const char *params) {
    lsp_log(server, "Document opened");
    /* In a full implementation, would parse document and store it */
}

/* Handle textDocument/didChange */
void lsp_handle_text_document_did_change(LSPServer *server, const char *params) {
    lsp_log(server, "Document changed");
    /* In a full implementation, would update stored document and provide diagnostics */
}

/* Handle textDocument/completion */
void lsp_handle_text_document_completion(LSPServer *server, int id, const char *params) {
    lsp_log(server, "Completion requested");
    
    /* Provide basic completions */
    const char *result = 
        "{"
        "\"isIncomplete\":false,"
        "\"items\":["
        "{\"label\":\"fn\",\"kind\":14,\"detail\":\"Function declaration\"},"
        "{\"label\":\"let\",\"kind\":14,\"detail\":\"Variable declaration\"},"
        "{\"label\":\"const\",\"kind\":14,\"detail\":\"Constant declaration\"},"
        "{\"label\":\"if\",\"kind\":14,\"detail\":\"Conditional statement\"},"
        "{\"label\":\"for\",\"kind\":14,\"detail\":\"For loop\"},"
        "{\"label\":\"while\",\"kind\":14,\"detail\":\"While loop\"},"
        "{\"label\":\"return\",\"kind\":14,\"detail\":\"Return statement\"},"
        "{\"label\":\"class\",\"kind\":14,\"detail\":\"Class declaration\"},"
        "{\"label\":\"import\",\"kind\":14,\"detail\":\"Import statement\"},"
        "{\"label\":\"export\",\"kind\":14,\"detail\":\"Export statement\"},"
        "{\"label\":\"println\",\"kind\":3,\"detail\":\"Print line function\"}"
        "]"
        "}";
    
    lsp_send_response(id, result);
}

/* Handle textDocument/hover */
void lsp_handle_text_document_hover(LSPServer *server, int id, const char *params) {
    lsp_log(server, "Hover requested");
    
    /* Provide basic hover info */
    const char *result = 
        "{"
        "\"contents\":{"
        "\"kind\":\"markdown\","
        "\"value\":\"**KLang Symbol**\\n\\nHover over KLang code for information.\""
        "}"
        "}";
    
    lsp_send_response(id, result);
}

/* Handle textDocument/definition */
void lsp_handle_text_document_definition(LSPServer *server, int id, const char *params) {
    lsp_log(server, "Definition requested");
    lsp_send_response(id, "null");
}

/* Handle textDocument/references */
void lsp_handle_text_document_references(LSPServer *server, int id, const char *params) {
    lsp_log(server, "References requested");
    lsp_send_response(id, "[]");
}

/* Handle textDocument/formatting */
void lsp_handle_text_document_formatting(LSPServer *server, int id, const char *params) {
    lsp_log(server, "Formatting requested");
    lsp_send_response(id, "[]");
}

/* Handle LSP message */
void lsp_handle_message(LSPServer *server, const char *message) {
    lsp_log(server, "Received message: %s", message);
    
    /* Simple JSON parsing (in production would use a proper JSON parser) */
    const char *method_start = strstr(message, "\"method\":");
    const char *id_start = strstr(message, "\"id\":");
    
    int id = -1;
    if (id_start) {
        id = atoi(id_start + 5);
    }
    
    if (!method_start) {
        lsp_log(server, "No method found in message");
        return;
    }
    
    /* Extract method name */
    method_start += 10; /* Skip "method":" */
    if (*method_start == '"') method_start++;
    
    const char *method_end = strchr(method_start, '"');
    if (!method_end) return;
    
    int method_len = method_end - method_start;
    char *method = malloc(method_len + 1);
    strncpy(method, method_start, method_len);
    method[method_len] = '\0';
    
    lsp_log(server, "Method: %s, ID: %d", method, id);
    
    /* Handle different message types */
    LSPMessageType type = lsp_parse_message_type(method);
    
    switch (type) {
        case LSP_INITIALIZE:
            lsp_handle_initialize(server, id, NULL);
            break;
            
        case LSP_INITIALIZED:
            lsp_log(server, "Client initialized");
            break;
            
        case LSP_SHUTDOWN:
            lsp_log(server, "Shutdown requested");
            server->shutdown_requested = 1;
            lsp_send_response(id, "null");
            break;
            
        case LSP_EXIT:
            lsp_log(server, "Exit requested");
            exit(0);
            break;
            
        case LSP_TEXT_DOCUMENT_DID_OPEN:
            lsp_handle_text_document_did_open(server, NULL);
            break;
            
        case LSP_TEXT_DOCUMENT_DID_CHANGE:
            lsp_handle_text_document_did_change(server, NULL);
            break;
            
        case LSP_TEXT_DOCUMENT_COMPLETION:
            lsp_handle_text_document_completion(server, id, NULL);
            break;
            
        case LSP_TEXT_DOCUMENT_HOVER:
            lsp_handle_text_document_hover(server, id, NULL);
            break;
            
        case LSP_TEXT_DOCUMENT_DEFINITION:
            lsp_handle_text_document_definition(server, id, NULL);
            break;
            
        case LSP_TEXT_DOCUMENT_REFERENCES:
            lsp_handle_text_document_references(server, id, NULL);
            break;
            
        case LSP_TEXT_DOCUMENT_FORMATTING:
            lsp_handle_text_document_formatting(server, id, NULL);
            break;
            
        default:
            lsp_log(server, "Unknown method: %s", method);
            if (id >= 0) {
                lsp_send_error(id, -32601, "Method not found");
            }
            break;
    }
    
    free(method);
}

/* Main LSP server loop */
int lsp_server_run(LSPServer *server) {
    lsp_log(server, "LSP server starting");
    
    while (!server->shutdown_requested) {
        char *message = lsp_read_message();
        
        if (!message) {
            lsp_log(server, "No message received, exiting");
            break;
        }
        
        lsp_handle_message(server, message);
        free(message);
    }
    
    lsp_log(server, "LSP server stopped");
    return 0;
}
