#include "lsp_server.h"
#include "lexer.h"
#include "parser.h"
#include "cli_colors.h"
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Forward declaration for asprintf (may not be in headers with strict C99) */
int asprintf(char **strp, const char *fmt, ...);

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
    } else if (strcmp(method, "textDocument/documentSymbol") == 0) {
        return LSP_TEXT_DOCUMENT_DOCUMENT_SYMBOL;
    } else if (strcmp(method, "textDocument/publishDiagnostics") == 0) {
        return LSP_TEXT_DOCUMENT_DIAGNOSTIC;
    }
    
    return LSP_UNKNOWN;
}

/* Handle initialize request */
void lsp_handle_initialize(LSPServer *server, int id, const char *params) {
    (void)params;  /* Reserved for future capability negotiation */
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
    (void)params;  /* Would parse document URI and content */
    lsp_log(server, "Document opened");
    /* In a full implementation, would parse document and store it */
}

/* Handle textDocument/didChange */
void lsp_handle_text_document_did_change(LSPServer *server, const char *params) {
    (void)params;  /* Would parse document changes */
    lsp_log(server, "Document changed");
    /* In a full implementation, would update stored document and provide diagnostics */
}

/* Handle textDocument/completion */
void lsp_handle_text_document_completion(LSPServer *server, int id, const char *params) {
    (void)params;  /* Would parse cursor position */
    lsp_log(server, "Completion requested");
    
    /* 
     * Note: Completion items are provided as a static JSON string for simplicity.
     * In a production implementation, these should be built programmatically from
     * a data structure to improve maintainability.
     */
    
    /* Enhanced completions with more keywords, types, and built-ins */
    const char *result = 
        "{"
        "\"isIncomplete\":false,"
        "\"items\":["
        /* Keywords */
        "{\"label\":\"fn\",\"kind\":14,\"detail\":\"Function declaration\",\"documentation\":\"Declare a new function\",\"insertText\":\"fn ${1:name}(${2:params}) {\\n\\t$0\\n}\",\"insertTextFormat\":2},"
        "{\"label\":\"async\",\"kind\":14,\"detail\":\"Async function\",\"documentation\":\"Declare an async function\",\"insertText\":\"async fn ${1:name}(${2:params}) {\\n\\t$0\\n}\",\"insertTextFormat\":2},"
        "{\"label\":\"let\",\"kind\":14,\"detail\":\"Variable declaration\",\"documentation\":\"Declare a variable\",\"insertText\":\"let ${1:name} = ${0:value}\",\"insertTextFormat\":2},"
        "{\"label\":\"const\",\"kind\":14,\"detail\":\"Constant declaration\",\"documentation\":\"Declare a constant\",\"insertText\":\"const ${1:name} = ${0:value}\",\"insertTextFormat\":2},"
        "{\"label\":\"var\",\"kind\":14,\"detail\":\"Variable (function-scoped)\",\"documentation\":\"Declare a function-scoped variable\",\"insertText\":\"var ${1:name} = ${0:value}\",\"insertTextFormat\":2},"
        "{\"label\":\"class\",\"kind\":14,\"detail\":\"Class declaration\",\"documentation\":\"Declare a class\",\"insertText\":\"class ${1:Name} {\\n\\t$0\\n}\",\"insertTextFormat\":2},"
        "{\"label\":\"interface\",\"kind\":14,\"detail\":\"Interface declaration\",\"documentation\":\"Declare an interface\",\"insertText\":\"interface ${1:Name} {\\n\\t$0\\n}\",\"insertTextFormat\":2},"
        "{\"label\":\"enum\",\"kind\":14,\"detail\":\"Enum declaration\",\"documentation\":\"Declare an enum\",\"insertText\":\"enum ${1:Name} {\\n\\t$0\\n}\",\"insertTextFormat\":2},"
        "{\"label\":\"if\",\"kind\":14,\"detail\":\"If statement\",\"documentation\":\"Conditional statement\",\"insertText\":\"if ${1:condition} {\\n\\t$0\\n}\",\"insertTextFormat\":2},"
        "{\"label\":\"else\",\"kind\":14,\"detail\":\"Else clause\",\"documentation\":\"Else clause\",\"insertText\":\"else {\\n\\t$0\\n}\",\"insertTextFormat\":2},"
        "{\"label\":\"for\",\"kind\":14,\"detail\":\"For loop\",\"documentation\":\"For loop\",\"insertText\":\"for ${1:item} in ${2:collection} {\\n\\t$0\\n}\",\"insertTextFormat\":2},"
        "{\"label\":\"while\",\"kind\":14,\"detail\":\"While loop\",\"documentation\":\"While loop\",\"insertText\":\"while ${1:condition} {\\n\\t$0\\n}\",\"insertTextFormat\":2},"
        "{\"label\":\"return\",\"kind\":14,\"detail\":\"Return statement\",\"documentation\":\"Return from function\",\"insertText\":\"return ${0:value}\",\"insertTextFormat\":2},"
        "{\"label\":\"await\",\"kind\":14,\"detail\":\"Await expression\",\"documentation\":\"Wait for promise\",\"insertText\":\"await ${0:promise}\",\"insertTextFormat\":2},"
        "{\"label\":\"yield\",\"kind\":14,\"detail\":\"Yield expression\",\"documentation\":\"Yield value\",\"insertText\":\"yield ${0:value}\",\"insertTextFormat\":2},"
        "{\"label\":\"import\",\"kind\":14,\"detail\":\"Import statement\",\"documentation\":\"Import from module\",\"insertText\":\"import { ${1:name} } from \\\"${0:module}\\\"\",\"insertTextFormat\":2},"
        "{\"label\":\"export\",\"kind\":14,\"detail\":\"Export statement\",\"documentation\":\"Export from module\",\"insertText\":\"export ${0}\",\"insertTextFormat\":2},"
        "{\"label\":\"new\",\"kind\":14,\"detail\":\"New instance\",\"documentation\":\"Create new instance\",\"insertText\":\"new ${1:ClassName}(${0})\",\"insertTextFormat\":2},"
        "{\"label\":\"this\",\"kind\":14,\"detail\":\"This reference\",\"documentation\":\"Reference to current object\"},"
        "{\"label\":\"static\",\"kind\":14,\"detail\":\"Static member\",\"documentation\":\"Static class member\"},"
        "{\"label\":\"public\",\"kind\":14,\"detail\":\"Public access\",\"documentation\":\"Public access modifier\"},"
        "{\"label\":\"private\",\"kind\":14,\"detail\":\"Private access\",\"documentation\":\"Private access modifier\"},"
        "{\"label\":\"protected\",\"kind\":14,\"detail\":\"Protected access\",\"documentation\":\"Protected access modifier\"},"
        "{\"label\":\"abstract\",\"kind\":14,\"detail\":\"Abstract declaration\",\"documentation\":\"Abstract class or method\"},"
        "{\"label\":\"extends\",\"kind\":14,\"detail\":\"Class inheritance\",\"documentation\":\"Inherit from parent\"},"
        "{\"label\":\"implements\",\"kind\":14,\"detail\":\"Interface implementation\",\"documentation\":\"Implement interface\"},"
        "{\"label\":\"break\",\"kind\":14,\"detail\":\"Break statement\",\"documentation\":\"Exit loop\"},"
        "{\"label\":\"continue\",\"kind\":14,\"detail\":\"Continue statement\",\"documentation\":\"Skip to next iteration\"},"
        "{\"label\":\"throw\",\"kind\":14,\"detail\":\"Throw exception\",\"documentation\":\"Throw exception\",\"insertText\":\"throw ${0:error}\",\"insertTextFormat\":2},"
        "{\"label\":\"try\",\"kind\":14,\"detail\":\"Try-catch block\",\"documentation\":\"Exception handling\",\"insertText\":\"try {\\n\\t${1}\\n} catch (${2:e}) {\\n\\t${0}\\n}\",\"insertTextFormat\":2},"
        /* Built-in functions */
        "{\"label\":\"println\",\"kind\":3,\"detail\":\"Print with newline\",\"documentation\":\"Print to console with newline\",\"insertText\":\"println(${0:message})\",\"insertTextFormat\":2},"
        "{\"label\":\"print\",\"kind\":3,\"detail\":\"Print without newline\",\"documentation\":\"Print to console\",\"insertText\":\"print(${0:message})\",\"insertTextFormat\":2},"
        "{\"label\":\"len\",\"kind\":3,\"detail\":\"Get length\",\"documentation\":\"Get length of collection\",\"insertText\":\"len(${0:collection})\",\"insertTextFormat\":2},"
        "{\"label\":\"typeof\",\"kind\":3,\"detail\":\"Get type\",\"documentation\":\"Get type of value\",\"insertText\":\"typeof(${0:value})\",\"insertTextFormat\":2},"
        "{\"label\":\"assert\",\"kind\":3,\"detail\":\"Assertion\",\"documentation\":\"Assert condition\",\"insertText\":\"assert(${1:condition}, ${0:message})\",\"insertTextFormat\":2},"
        /* Types */
        "{\"label\":\"number\",\"kind\":25,\"detail\":\"Number type\",\"documentation\":\"Numeric type\"},"
        "{\"label\":\"string\",\"kind\":25,\"detail\":\"String type\",\"documentation\":\"String type\"},"
        "{\"label\":\"bool\",\"kind\":25,\"detail\":\"Boolean type\",\"documentation\":\"Boolean type\"},"
        "{\"label\":\"void\",\"kind\":25,\"detail\":\"Void type\",\"documentation\":\"No return value\"},"
        "{\"label\":\"any\",\"kind\":25,\"detail\":\"Any type\",\"documentation\":\"Any type\"},"
        "{\"label\":\"array\",\"kind\":25,\"detail\":\"Array type\",\"documentation\":\"Array type\",\"insertText\":\"array<${0:Type}>\",\"insertTextFormat\":2}"
        "]"
        "}";
    
    lsp_send_response(id, result);
}

/* Handle textDocument/hover */
void lsp_handle_text_document_hover(LSPServer *server, int id, const char *params) {
    (void)params;  /* Would parse hover position */
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
    (void)params;  /* Would parse symbol position */
    lsp_log(server, "Definition requested");
    lsp_send_response(id, "null");
}

/* Handle textDocument/references */
void lsp_handle_text_document_references(LSPServer *server, int id, const char *params) {
    (void)params;  /* Would parse symbol position */
    lsp_log(server, "References requested");
    lsp_send_response(id, "[]");
}

/* Handle textDocument/formatting */
void lsp_handle_text_document_formatting(LSPServer *server, int id, const char *params) {
    (void)params;  /* Would parse formatting options */
    lsp_log(server, "Formatting requested");
    lsp_send_response(id, "[]");
}

/* Handle textDocument/documentSymbol */
void lsp_handle_text_document_document_symbol(LSPServer *server, int id, const char *params) {
    (void)params;  /* Would parse document URI and content */
    lsp_log(server, "Document symbol requested");
    
    /* Return empty array for now - would parse document and extract symbols */
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
            
        case LSP_TEXT_DOCUMENT_DOCUMENT_SYMBOL:
            lsp_handle_text_document_document_symbol(server, id, NULL);
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
