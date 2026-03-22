#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

static Config global_config;
static int config_initialized = 0;

void config_init(void) {
    if (config_initialized) return;
    
    /* Initialize defaults */
    global_config.enable_colors = 1;
    global_config.indent_size = 4;
    global_config.use_tabs = 0;
    global_config.debug_mode = 0;
    global_config.history_size = 1000;
    global_config.max_line_length = 100;
    
    /* Module search paths */
    global_config.module_paths = NULL;
    global_config.module_path_count = 0;
    
    /* Load from environment variables */
    config_load_from_env();
    
    config_initialized = 1;
}

void config_load_from_env(void) {
    char *env_val;
    
    /* KLANG_NO_COLOR - Disable colors */
    env_val = getenv("KLANG_NO_COLOR");
    if (env_val && (strcmp(env_val, "1") == 0 || strcmp(env_val, "true") == 0)) {
        global_config.enable_colors = 0;
    }
    
    /* KLANG_DEBUG - Enable debug mode */
    env_val = getenv("KLANG_DEBUG");
    if (env_val && (strcmp(env_val, "1") == 0 || strcmp(env_val, "true") == 0)) {
        global_config.debug_mode = 1;
    }
    
    /* KLANG_INDENT_SIZE - Set indent size */
    env_val = getenv("KLANG_INDENT_SIZE");
    if (env_val) {
        int size = atoi(env_val);
        if (size >= 1 && size <= 8) {
            global_config.indent_size = size;
        }
    }
    
    /* KLANG_USE_TABS - Use tabs instead of spaces */
    env_val = getenv("KLANG_USE_TABS");
    if (env_val && (strcmp(env_val, "1") == 0 || strcmp(env_val, "true") == 0)) {
        global_config.use_tabs = 1;
    }
    
    /* KLANG_HISTORY_SIZE - REPL history size */
    env_val = getenv("KLANG_HISTORY_SIZE");
    if (env_val) {
        int size = atoi(env_val);
        if (size >= 0 && size <= 10000) {
            global_config.history_size = size;
        }
    }
    
    /* KLANG_PATH - Module search paths (colon-separated) */
    env_val = getenv("KLANG_PATH");
    if (env_val) {
        char *path_copy = strdup(env_val);
        char *token = strtok(path_copy, ":");
        int count = 0;
        
        /* Count paths */
        while (token != NULL) {
            count++;
            token = strtok(NULL, ":");
        }
        
        if (count > 0) {
            global_config.module_paths = malloc(sizeof(char*) * count);
            global_config.module_path_count = count;
            
            /* Parse again to store paths */
            free(path_copy);
            path_copy = strdup(env_val);
            token = strtok(path_copy, ":");
            count = 0;
            
            while (token != NULL) {
                global_config.module_paths[count++] = strdup(token);
                token = strtok(NULL, ":");
            }
        }
        
        free(path_copy);
    }
    
    /* KLANG_MAX_LINE_LENGTH - Maximum line length for formatter */
    env_val = getenv("KLANG_MAX_LINE_LENGTH");
    if (env_val) {
        int length = atoi(env_val);
        if (length >= 40 && length <= 200) {
            global_config.max_line_length = length;
        }
    }
}

Config* config_get(void) {
    if (!config_initialized) {
        config_init();
    }
    return &global_config;
}

void config_set_colors(int enable) {
    if (!config_initialized) config_init();
    global_config.enable_colors = enable;
}

void config_set_debug(int enable) {
    if (!config_initialized) config_init();
    global_config.debug_mode = enable;
}

void config_set_indent(int size, int use_tabs) {
    if (!config_initialized) config_init();
    global_config.indent_size = size;
    global_config.use_tabs = use_tabs;
}

void config_free(void) {
    int i;
    if (!config_initialized) return;
    
    /* Free module paths */
    if (global_config.module_paths) {
        for (i = 0; i < global_config.module_path_count; i++) {
            free(global_config.module_paths[i]);
        }
        free(global_config.module_paths);
        global_config.module_paths = NULL;
        global_config.module_path_count = 0;
    }
    
    config_initialized = 0;
}

void config_print(void) {
    int i;
    if (!config_initialized) config_init();
    
    printf("KLang Configuration:\n");
    printf("  Colors enabled:    %s\n", global_config.enable_colors ? "yes" : "no");
    printf("  Debug mode:        %s\n", global_config.debug_mode ? "yes" : "no");
    printf("  Indent size:       %d\n", global_config.indent_size);
    printf("  Use tabs:          %s\n", global_config.use_tabs ? "yes" : "no");
    printf("  History size:      %d\n", global_config.history_size);
    printf("  Max line length:   %d\n", global_config.max_line_length);
    
    if (global_config.module_path_count > 0) {
        printf("  Module paths:\n");
        for (i = 0; i < global_config.module_path_count; i++) {
            printf("    - %s\n", global_config.module_paths[i]);
        }
    }
}
