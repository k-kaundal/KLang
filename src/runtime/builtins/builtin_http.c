#include "runtime.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* KLP Protocol Support */
#ifdef ENABLE_KLP
#include "klp_runtime.h"
#endif

/* HTTP Request Functions (using system commands for now - can be upgraded to libcurl later) */

Value builtin_http_get(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: http.get() requires a string argument (URL)\n");
        return make_null();
    }
    
    const char *url = args[0].as.str_val;
    
    // Build curl command
    char command[4096];
    snprintf(command, sizeof(command), "curl -s -X GET '%s'", url);
    
    // Execute curl and capture output
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Failed to execute HTTP GET request\n");
        return make_null();
    }
    
    // Read response into buffer
    char *response = NULL;
    size_t response_size = 0;
    size_t capacity = 8192;
    response = malloc(capacity);
    
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        if (response_size + len >= capacity) {
            capacity *= 2;
            response = realloc(response, capacity);
        }
        strcpy(response + response_size, buffer);
        response_size += len;
    }
    
    int status = pclose(fp);
    
    if (status != 0) {
        free(response);
        fprintf(stderr, "Error: HTTP GET request failed with status %d\n", status);
        return make_null();
    }
    
    Value result = make_string(response);
    free(response);
    return result;
}

Value builtin_http_post(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: http.post() requires two string arguments (URL, body)\n");
        return make_null();
    }
    
    const char *url = args[0].as.str_val;
    const char *body = args[1].as.str_val;
    
    // Optional headers as third argument
    const char *content_type = "application/json";
    if (argc >= 3 && args[2].type == VAL_STRING) {
        content_type = args[2].as.str_val;
    }
    
    // Build curl command
    char command[8192];
    snprintf(command, sizeof(command), 
             "curl -s -X POST '%s' -H 'Content-Type: %s' -d '%s'", 
             url, content_type, body);
    
    // Execute curl and capture output
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Failed to execute HTTP POST request\n");
        return make_null();
    }
    
    // Read response into buffer
    char *response = NULL;
    size_t response_size = 0;
    size_t capacity = 8192;
    response = malloc(capacity);
    
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        if (response_size + len >= capacity) {
            capacity *= 2;
            response = realloc(response, capacity);
        }
        strcpy(response + response_size, buffer);
        response_size += len;
    }
    
    int status = pclose(fp);
    
    if (status != 0) {
        free(response);
        fprintf(stderr, "Error: HTTP POST request failed with status %d\n", status);
        return make_null();
    }
    
    Value result = make_string(response);
    free(response);
    return result;
}

Value builtin_http_request(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1 || args[0].type != VAL_DICT) {
        fprintf(stderr, "Error: http.request() requires a dictionary argument (config)\n");
        return make_null();
    }
    
    DictVal *config = args[0].as.dict_val;
    
    // Extract URL (required)
    Value *url_val = NULL;
    for (int i = 0; i < config->count; i++) {
        if (config->keys[i].type == VAL_STRING && 
            strcmp(config->keys[i].as.str_val, "url") == 0) {
            url_val = &config->values[i];
            break;
        }
    }
    
    if (url_val == NULL || url_val->type != VAL_STRING) {
        fprintf(stderr, "Error: http.request() config must have a 'url' string property\n");
        return make_null();
    }
    
    const char *url = url_val->as.str_val;
    
    // Extract method (default: GET)
    const char *method = "GET";
    for (int i = 0; i < config->count; i++) {
        if (config->keys[i].type == VAL_STRING && 
            strcmp(config->keys[i].as.str_val, "method") == 0) {
            if (config->values[i].type == VAL_STRING) {
                method = config->values[i].as.str_val;
            }
            break;
        }
    }
    
    // Extract headers (optional)
    char headers_str[4096] = "";
    for (int i = 0; i < config->count; i++) {
        if (config->keys[i].type == VAL_STRING && 
            strcmp(config->keys[i].as.str_val, "headers") == 0) {
            if (config->values[i].type == VAL_DICT) {
                DictVal *headers = config->values[i].as.dict_val;
                for (int j = 0; j < headers->count; j++) {
                    if (headers->keys[j].type == VAL_STRING && 
                        headers->values[j].type == VAL_STRING) {
                        char header[512];
                        snprintf(header, sizeof(header), " -H '%s: %s'",
                                headers->keys[j].as.str_val,
                                headers->values[j].as.str_val);
                        strncat(headers_str, header, sizeof(headers_str) - strlen(headers_str) - 1);
                    }
                }
            }
            break;
        }
    }
    
    // Extract body (optional)
    const char *body = NULL;
    for (int i = 0; i < config->count; i++) {
        if (config->keys[i].type == VAL_STRING && 
            strcmp(config->keys[i].as.str_val, "body") == 0) {
            if (config->values[i].type == VAL_STRING) {
                body = config->values[i].as.str_val;
            }
            break;
        }
    }
    
    // Build curl command
    char command[8192];
    if (body != NULL) {
        snprintf(command, sizeof(command), 
                 "curl -s -X %s '%s'%s -d '%s'", 
                 method, url, headers_str, body);
    } else {
        snprintf(command, sizeof(command), 
                 "curl -s -X %s '%s'%s", 
                 method, url, headers_str);
    }
    
    // Execute curl and capture output
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Failed to execute HTTP request\n");
        return make_null();
    }
    
    // Read response into buffer
    char *response = NULL;
    size_t response_size = 0;
    size_t capacity = 8192;
    response = malloc(capacity);
    response[0] = '\0';
    
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        if (response_size + len >= capacity) {
            capacity *= 2;
            response = realloc(response, capacity);
        }
        strcpy(response + response_size, buffer);
        response_size += len;
    }
    
    int status = pclose(fp);
    
    if (status != 0) {
        free(response);
        fprintf(stderr, "Error: HTTP request failed with status %d\n", status);
        return make_null();
    }
    
    Value result = make_string(response);
    free(response);
    return result;
}
