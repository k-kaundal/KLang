#ifndef LSP_SERVER_H
#define LSP_SERVER_H

#define _GNU_SOURCE  /* For asprintf() */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* LSP Message Types */
typedef enum {
    LSP_INITIALIZE,
    LSP_INITIALIZED,
    LSP_SHUTDOWN,
    LSP_EXIT,
    LSP_TEXT_DOCUMENT_DID_OPEN,
    LSP_TEXT_DOCUMENT_DID_CHANGE,
    LSP_TEXT_DOCUMENT_DID_SAVE,
    LSP_TEXT_DOCUMENT_DID_CLOSE,
    LSP_TEXT_DOCUMENT_COMPLETION,
    LSP_TEXT_DOCUMENT_HOVER,
    LSP_TEXT_DOCUMENT_DEFINITION,
    LSP_TEXT_DOCUMENT_REFERENCES,
    LSP_TEXT_DOCUMENT_DOCUMENT_SYMBOL,
    LSP_WORKSPACE_SYMBOL,
    LSP_TEXT_DOCUMENT_FORMATTING,
    LSP_TEXT_DOCUMENT_DIAGNOSTIC,
    LSP_UNKNOWN
} LSPMessageType;

/* Position in document */
typedef struct {
    int line;
    int character;
} LSPPosition;

/* Range in document */
typedef struct {
    LSPPosition start;
    LSPPosition end;
} LSPRange;

/* Text document identifier */
typedef struct {
    char *uri;
    int version;
} LSPTextDocumentIdentifier;

/* Diagnostic severity */
typedef enum {
    LSP_DIAGNOSTIC_ERROR = 1,
    LSP_DIAGNOSTIC_WARNING = 2,
    LSP_DIAGNOSTIC_INFO = 3,
    LSP_DIAGNOSTIC_HINT = 4
} LSPDiagnosticSeverity;

/* Diagnostic */
typedef struct {
    LSPRange range;
    LSPDiagnosticSeverity severity;
    char *message;
    char *source;
} LSPDiagnostic;

/* Completion item kind */
typedef enum {
    LSP_COMPLETION_TEXT = 1,
    LSP_COMPLETION_METHOD = 2,
    LSP_COMPLETION_FUNCTION = 3,
    LSP_COMPLETION_CONSTRUCTOR = 4,
    LSP_COMPLETION_FIELD = 5,
    LSP_COMPLETION_VARIABLE = 6,
    LSP_COMPLETION_CLASS = 7,
    LSP_COMPLETION_INTERFACE = 8,
    LSP_COMPLETION_MODULE = 9,
    LSP_COMPLETION_PROPERTY = 10,
    LSP_COMPLETION_KEYWORD = 14,
    LSP_COMPLETION_SNIPPET = 15
} LSPCompletionItemKind;

/* Completion item */
typedef struct {
    char *label;
    LSPCompletionItemKind kind;
    char *detail;
    char *documentation;
    char *insertText;
} LSPCompletionItem;

/* Symbol kind */
typedef enum {
    LSP_SYMBOL_FILE = 1,
    LSP_SYMBOL_MODULE = 2,
    LSP_SYMBOL_NAMESPACE = 3,
    LSP_SYMBOL_CLASS = 5,
    LSP_SYMBOL_METHOD = 6,
    LSP_SYMBOL_FUNCTION = 12,
    LSP_SYMBOL_VARIABLE = 13,
    LSP_SYMBOL_CONSTANT = 14
} LSPSymbolKind;

/* Document symbol */
typedef struct {
    char *name;
    LSPSymbolKind kind;
    LSPRange range;
    LSPRange selectionRange;
} LSPDocumentSymbol;

/* LSP Server context */
typedef struct {
    int initialized;
    int shutdown_requested;
    char *root_uri;
    FILE *log_file;
} LSPServer;

/* Initialize LSP server */
LSPServer* lsp_server_init(void);

/* Free LSP server */
void lsp_server_free(LSPServer *server);

/* Start LSP server main loop */
int lsp_server_run(LSPServer *server);

/* Handle LSP message */
void lsp_handle_message(LSPServer *server, const char *message);

/* Send LSP response */
void lsp_send_response(int id, const char *result);

/* Send LSP notification */
void lsp_send_notification(const char *method, const char *params);

/* Send LSP error */
void lsp_send_error(int id, int code, const char *message);

/* Parse LSP message type */
LSPMessageType lsp_parse_message_type(const char *method);

/* Handle specific LSP requests */
void lsp_handle_initialize(LSPServer *server, int id, const char *params);
void lsp_handle_text_document_did_open(LSPServer *server, const char *params);
void lsp_handle_text_document_did_change(LSPServer *server, const char *params);
void lsp_handle_text_document_completion(LSPServer *server, int id, const char *params);
void lsp_handle_text_document_hover(LSPServer *server, int id, const char *params);
void lsp_handle_text_document_definition(LSPServer *server, int id, const char *params);
void lsp_handle_text_document_references(LSPServer *server, int id, const char *params);
void lsp_handle_text_document_formatting(LSPServer *server, int id, const char *params);

/* Utility functions */
char* lsp_read_message(void);
void lsp_log(LSPServer *server, const char *format, ...);

#endif /* LSP_SERVER_H */
