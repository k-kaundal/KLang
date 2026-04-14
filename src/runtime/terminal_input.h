/*
 * Terminal Input Handler Header
 */

#ifndef TERMINAL_INPUT_H
#define TERMINAL_INPUT_H

/* Input event types */
typedef enum {
    INPUT_KEY,
    INPUT_MOUSE,
    INPUT_RESIZE,
    INPUT_NONE
} InputEventType;

/* Input event structure */
typedef struct {
    InputEventType type;
    union {
        struct {
            char key[32];
            int special; /* 0 = normal char, 1 = special key */
        } key;
        struct {
            int button;  /* 0=left, 1=middle, 2=right */
            int x;
            int y;
            int action;  /* 0=press, 1=release, 2=motion */
        } mouse;
        struct {
            int rows;
            int cols;
        } resize;
    } data;
} InputEvent;

/* Terminal control functions */
int terminal_enable_raw_mode(void);
void terminal_disable_raw_mode(void);
void terminal_enable_mouse(void);
void terminal_disable_mouse(void);
int terminal_get_size(int *rows, int *cols);
int terminal_read_event(InputEvent *event, int timeout_ms);

/* Screen control */
void terminal_clear_screen(void);
void terminal_hide_cursor(void);
void terminal_show_cursor(void);
void terminal_move_cursor(int row, int col);
void terminal_use_alt_buffer(void);
void terminal_use_main_buffer(void);

/* Initialize and cleanup */
int terminal_init_ui(void);
void terminal_cleanup(void);

#endif /* TERMINAL_INPUT_H */
