#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli_colors.h"

static int color_support = 1;

int colors_enabled(void) {
    return color_support;
}

void enable_colors(int enable) {
    color_support = enable;
}

const char* get_color(const char* color_code) {
    if (!color_support) {
        return "";
    }
    return color_code;
}

void print_error(const char* message) {
    fprintf(stderr, "%sError:%s %s\n", 
            get_color(COLOR_BOLD_RED), 
            get_color(COLOR_RESET), 
            message);
}

void print_success(const char* message) {
    printf("%s✓%s %s\n", 
           get_color(COLOR_BOLD_GREEN), 
           get_color(COLOR_RESET), 
           message);
}

void print_warning(const char* message) {
    fprintf(stderr, "%sWarning:%s %s\n", 
            get_color(COLOR_BOLD_YELLOW), 
            get_color(COLOR_RESET), 
            message);
}

void print_info(const char* message) {
    printf("%sℹ%s %s\n", 
           get_color(COLOR_BOLD_CYAN), 
           get_color(COLOR_RESET), 
           message);
}

void print_header(const char* text) {
    printf("\n%s%s%s\n", 
           get_color(COLOR_BOLD_CYAN), 
           text, 
           get_color(COLOR_RESET));
}
