#ifndef CONFIG_H
#define CONFIG_H

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

/* Free configuration resources */
void config_free(void);

/* Print current configuration */
void config_print(void);

#endif /* CONFIG_H */
