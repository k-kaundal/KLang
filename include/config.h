#ifndef CONFIG_H
#define CONFIG_H

/* Build modes */
typedef enum {
    BUILD_MODE_DEBUG,      /* Debug mode: -O0, debug symbols, assertions */
    BUILD_MODE_DEV,        /* Development mode: -O1, fast compile, hot reload */
    BUILD_MODE_RELEASE,    /* Release mode: -O2, balanced optimization */
    BUILD_MODE_PRODUCTION  /* Production mode: -O3, maximum optimization */
} BuildMode;

/* Optimization levels */
typedef enum {
    OPT_LEVEL_NONE = 0,    /* -O0: No optimization */
    OPT_LEVEL_BASIC = 1,   /* -O1: Basic optimization */
    OPT_LEVEL_DEFAULT = 2, /* -O2: Default optimization */
    OPT_LEVEL_AGGRESSIVE = 3 /* -O3: Aggressive optimization */
} OptLevel;

typedef struct {
    /* Formatting options */
    int indent_size;
    int use_tabs;
    int max_line_length;
    
    /* UI options */
    int enable_colors;
    
    /* Debug options */
    int debug_mode;
    
    /* REPL options */
    int history_size;
    
    /* Module system */
    char **module_paths;
    int module_path_count;
    
    /* Build configuration */
    BuildMode build_mode;
    OptLevel opt_level;
    int enable_lto;           /* Link-time optimization */
    int strip_debug;          /* Strip debug symbols */
    int enable_assertions;    /* Enable runtime assertions */
    int enable_bounds_check;  /* Enable array bounds checking */
    int enable_hot_reload;    /* Enable hot reload (dev mode) */
    
    /* C/C++ compatibility options */
    int enable_pointers;      /* Enable pointer operations */
    int enable_manual_memory; /* Enable manual memory management */
    int enable_inline_asm;    /* Enable inline assembly */
} Config;

/* Initialize configuration with defaults and environment variables */
void config_init(void);

/* Load configuration from environment variables */
void config_load_from_env(void);

/* Get the global configuration */
Config* config_get(void);

/* Setters for specific config options */
void config_set_colors(int enable);
void config_set_debug(int enable);
void config_set_indent(int size, int use_tabs);
void config_set_build_mode(BuildMode mode);
void config_set_opt_level(OptLevel level);

/* Build mode helpers */
const char* config_get_build_mode_name(BuildMode mode);
const char* config_get_opt_level_name(OptLevel level);
BuildMode config_parse_build_mode(const char *str);
OptLevel config_parse_opt_level(const char *str);

/* Free configuration resources */
void config_free(void);

/* Print current configuration */
void config_print(void);

#endif /* CONFIG_H */
