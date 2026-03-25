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
    
    /* Build configuration defaults (development mode) */
    global_config.build_mode = BUILD_MODE_DEV;
    global_config.opt_level = OPT_LEVEL_BASIC;
    global_config.enable_lto = 0;
    global_config.strip_debug = 0;
    global_config.enable_assertions = 1;
    global_config.enable_bounds_check = 1;
    global_config.enable_hot_reload = 1;
    
    /* C/C++ compatibility defaults (disabled for safety) */
    global_config.enable_pointers = 0;
    global_config.enable_manual_memory = 0;
    global_config.enable_inline_asm = 0;
    
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
    
    /* KLANG_BUILD_MODE - Set build mode (debug, dev, release, production) */
    env_val = getenv("KLANG_BUILD_MODE");
    if (env_val) {
        config_set_build_mode(config_parse_build_mode(env_val));
    }
    
    /* KLANG_OPT_LEVEL - Set optimization level (0, 1, 2, 3) */
    env_val = getenv("KLANG_OPT_LEVEL");
    if (env_val) {
        config_set_opt_level(config_parse_opt_level(env_val));
    }
    
    /* KLANG_ENABLE_LTO - Enable link-time optimization */
    env_val = getenv("KLANG_ENABLE_LTO");
    if (env_val && (strcmp(env_val, "1") == 0 || strcmp(env_val, "true") == 0)) {
        global_config.enable_lto = 1;
    }
    
    /* KLANG_STRIP_DEBUG - Strip debug symbols */
    env_val = getenv("KLANG_STRIP_DEBUG");
    if (env_val && (strcmp(env_val, "1") == 0 || strcmp(env_val, "true") == 0)) {
        global_config.strip_debug = 1;
    }
    
    /* KLANG_ENABLE_POINTERS - Enable pointer operations */
    env_val = getenv("KLANG_ENABLE_POINTERS");
    if (env_val && (strcmp(env_val, "1") == 0 || strcmp(env_val, "true") == 0)) {
        global_config.enable_pointers = 1;
    }
    
    /* KLANG_ENABLE_MANUAL_MEMORY - Enable manual memory management */
    env_val = getenv("KLANG_ENABLE_MANUAL_MEMORY");
    if (env_val && (strcmp(env_val, "1") == 0 || strcmp(env_val, "true") == 0)) {
        global_config.enable_manual_memory = 1;
    }
    
    /* KLANG_ENABLE_INLINE_ASM - Enable inline assembly */
    env_val = getenv("KLANG_ENABLE_INLINE_ASM");
    if (env_val && (strcmp(env_val, "1") == 0 || strcmp(env_val, "true") == 0)) {
        global_config.enable_inline_asm = 1;
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

void config_set_build_mode(BuildMode mode) {
    if (!config_initialized) config_init();
    global_config.build_mode = mode;
    
    /* Auto-adjust settings based on build mode */
    switch (mode) {
        case BUILD_MODE_DEBUG:
            global_config.opt_level = OPT_LEVEL_NONE;
            global_config.strip_debug = 0;
            global_config.enable_assertions = 1;
            global_config.enable_bounds_check = 1;
            global_config.enable_hot_reload = 0;
            global_config.enable_lto = 0;
            break;
        case BUILD_MODE_DEV:
            global_config.opt_level = OPT_LEVEL_BASIC;
            global_config.strip_debug = 0;
            global_config.enable_assertions = 1;
            global_config.enable_bounds_check = 1;
            global_config.enable_hot_reload = 1;
            global_config.enable_lto = 0;
            break;
        case BUILD_MODE_RELEASE:
            global_config.opt_level = OPT_LEVEL_DEFAULT;
            global_config.strip_debug = 0;
            global_config.enable_assertions = 1;
            global_config.enable_bounds_check = 1;
            global_config.enable_hot_reload = 0;
            global_config.enable_lto = 1;
            break;
        case BUILD_MODE_PRODUCTION:
            global_config.opt_level = OPT_LEVEL_AGGRESSIVE;
            global_config.strip_debug = 1;
            global_config.enable_assertions = 0;
            global_config.enable_bounds_check = 0;
            global_config.enable_hot_reload = 0;
            global_config.enable_lto = 1;
            break;
    }
}

void config_set_opt_level(OptLevel level) {
    if (!config_initialized) config_init();
    global_config.opt_level = level;
}

const char* config_get_build_mode_name(BuildMode mode) {
    switch (mode) {
        case BUILD_MODE_DEBUG: return "debug";
        case BUILD_MODE_DEV: return "development";
        case BUILD_MODE_RELEASE: return "release";
        case BUILD_MODE_PRODUCTION: return "production";
        default: return "unknown";
    }
}

const char* config_get_opt_level_name(OptLevel level) {
    switch (level) {
        case OPT_LEVEL_NONE: return "O0";
        case OPT_LEVEL_BASIC: return "O1";
        case OPT_LEVEL_DEFAULT: return "O2";
        case OPT_LEVEL_AGGRESSIVE: return "O3";
        default: return "O0";
    }
}

BuildMode config_parse_build_mode(const char *str) {
    if (!str) return BUILD_MODE_DEV;  /* Return default instead of -1 */
    if (strcmp(str, "debug") == 0) return BUILD_MODE_DEBUG;
    if (strcmp(str, "dev") == 0 || strcmp(str, "development") == 0) return BUILD_MODE_DEV;
    if (strcmp(str, "release") == 0) return BUILD_MODE_RELEASE;
    if (strcmp(str, "production") == 0 || strcmp(str, "prod") == 0) return BUILD_MODE_PRODUCTION;
    return BUILD_MODE_DEV;  /* Return default for invalid input */
}

OptLevel config_parse_opt_level(const char *str) {
    if (!str) return OPT_LEVEL_BASIC;  /* Return default instead of -1 */
    if (strcmp(str, "0") == 0 || strcmp(str, "O0") == 0) return OPT_LEVEL_NONE;
    if (strcmp(str, "1") == 0 || strcmp(str, "O1") == 0) return OPT_LEVEL_BASIC;
    if (strcmp(str, "2") == 0 || strcmp(str, "O2") == 0) return OPT_LEVEL_DEFAULT;
    if (strcmp(str, "3") == 0 || strcmp(str, "O3") == 0) return OPT_LEVEL_AGGRESSIVE;
    return OPT_LEVEL_BASIC;  /* Return default for invalid input */
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
    
    printf("\nBuild Configuration:\n");
    printf("  Build mode:        %s\n", config_get_build_mode_name(global_config.build_mode));
    printf("  Optimization:      %s\n", config_get_opt_level_name(global_config.opt_level));
    printf("  Link-time opt:     %s\n", global_config.enable_lto ? "yes" : "no");
    printf("  Strip debug:       %s\n", global_config.strip_debug ? "yes" : "no");
    printf("  Assertions:        %s\n", global_config.enable_assertions ? "yes" : "no");
    printf("  Bounds checking:   %s\n", global_config.enable_bounds_check ? "yes" : "no");
    printf("  Hot reload:        %s\n", global_config.enable_hot_reload ? "yes" : "no");
    
    printf("\nC/C++ Compatibility:\n");
    printf("  Pointers:          %s\n", global_config.enable_pointers ? "yes" : "no");
    printf("  Manual memory:     %s\n", global_config.enable_manual_memory ? "yes" : "no");
    printf("  Inline assembly:   %s\n", global_config.enable_inline_asm ? "yes" : "no");
    
    if (global_config.module_path_count > 0) {
        printf("\nModule paths:\n");
        for (i = 0; i < global_config.module_path_count; i++) {
            printf("    - %s\n", global_config.module_paths[i]);
        }
    }
}
