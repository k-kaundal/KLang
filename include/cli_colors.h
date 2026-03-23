#ifndef CLI_COLORS_H
#define CLI_COLORS_H

/* ANSI Color codes for terminal output */
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_DIM     "\033[2m"

/* Text colors */
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

/* Bold colors */
#define COLOR_BOLD_RED     "\033[1;31m"
#define COLOR_BOLD_GREEN   "\033[1;32m"
#define COLOR_BOLD_YELLOW  "\033[1;33m"
#define COLOR_BOLD_BLUE    "\033[1;34m"
#define COLOR_BOLD_MAGENTA "\033[1;35m"
#define COLOR_BOLD_CYAN    "\033[1;36m"
#define COLOR_BOLD_WHITE   "\033[1;37m"

/* Helper functions */
int colors_enabled(void);
void enable_colors(int enable);
const char* get_color(const char* color_code);

/* Convenient printing functions */
void print_error(const char* message);
void print_success(const char* message);
void print_warning(const char* message);
void print_info(const char* message);
void print_header(const char* text);

#endif /* CLI_COLORS_H */
