/*
 * Terminal Input Handler for Interactive UI Server
 * Provides raw terminal mode, mouse support, and input parsing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "terminal_input.h"

#ifdef __linux__
#include <sys/time.h>
#endif

/* Global state */
static struct termios orig_termios;
static int raw_mode_enabled = 0;

/* Enable raw terminal mode */
int terminal_enable_raw_mode(void) {
    struct termios raw;
    
    if (raw_mode_enabled) {
        return 0; /* Already enabled */
    }
    
    /* Get current terminal attributes */
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        return -1;
    }
    
    raw = orig_termios;
    
    /* Disable:
     * - ECHO: Don't echo input
     * - ICANON: Disable canonical mode (line buffering)
     * - ISIG: Disable signal generation (Ctrl+C, Ctrl+Z)
     * - IEXTEN: Disable extended processing
     */
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    
    /* Disable:
     * - IXON: Disable software flow control (Ctrl+S, Ctrl+Q)
     * - ICRNL: Don't translate CR to NL
     */
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    
    /* Disable output processing */
    raw.c_oflag &= ~(OPOST);
    
    /* Set character size to 8 bits */
    raw.c_cflag |= (CS8);
    
    /* Minimum characters for read: 0 (non-blocking) */
    raw.c_cc[VMIN] = 0;
    /* Timeout for read: 1 decisecond (100ms) */
    raw.c_cc[VTIME] = 1;
    
    /* Apply terminal settings */
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        return -1;
    }
    
    raw_mode_enabled = 1;
    return 0;
}

/* Disable raw terminal mode */
void terminal_disable_raw_mode(void) {
    if (!raw_mode_enabled) {
        return;
    }
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    raw_mode_enabled = 0;
}

/* Enable mouse tracking */
void terminal_enable_mouse(void) {
    /* Enable mouse button tracking */
    printf("\033[?1000h");
    /* Enable mouse motion tracking */
    printf("\033[?1002h");
    /* Enable any mouse event tracking */
    printf("\033[?1003h");
    /* Enable SGR mouse mode (better coordinate support) */
    printf("\033[?1006h");
    fflush(stdout);
}

/* Disable mouse tracking */
void terminal_disable_mouse(void) {
    printf("\033[?1000l");
    printf("\033[?1002l");
    printf("\033[?1003l");
    printf("\033[?1006l");
    fflush(stdout);
}

/* Get terminal size */
int terminal_get_size(int *rows, int *cols) {
    struct winsize ws;
    
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    }
    
    *rows = ws.ws_row;
    *cols = ws.ws_col;
    return 0;
}

/* Parse escape sequences for special keys */
static int parse_escape_sequence(char *seq, int len, InputEvent *event) {
    event->type = INPUT_KEY;
    event->data.key.special = 1;
    
    /* Arrow keys: ESC[A, ESC[B, ESC[C, ESC[D */
    if (len == 3 && seq[1] == '[') {
        switch (seq[2]) {
            case 'A': strcpy(event->data.key.key, "ArrowUp"); return 1;
            case 'B': strcpy(event->data.key.key, "ArrowDown"); return 1;
            case 'C': strcpy(event->data.key.key, "ArrowRight"); return 1;
            case 'D': strcpy(event->data.key.key, "ArrowLeft"); return 1;
        }
    }
    
    /* Function keys and other special keys */
    if (len >= 4 && seq[1] == '[') {
        /* Home: ESC[H or ESC[1~ */
        if (seq[2] == 'H' || (seq[2] == '1' && seq[3] == '~')) {
            strcpy(event->data.key.key, "Home");
            return 1;
        }
        /* End: ESC[F or ESC[4~ */
        if (seq[2] == 'F' || (seq[2] == '4' && seq[3] == '~')) {
            strcpy(event->data.key.key, "End");
            return 1;
        }
        /* Delete: ESC[3~ */
        if (seq[2] == '3' && seq[3] == '~') {
            strcpy(event->data.key.key, "Delete");
            return 1;
        }
        /* Page Up: ESC[5~ */
        if (seq[2] == '5' && seq[3] == '~') {
            strcpy(event->data.key.key, "PageUp");
            return 1;
        }
        /* Page Down: ESC[6~ */
        if (seq[2] == '6' && seq[3] == '~') {
            strcpy(event->data.key.key, "PageDown");
            return 1;
        }
    }
    
    /* Mouse events: ESC[<...M or ESC[<...m */
    if (len >= 6 && seq[1] == '[' && seq[2] == '<') {
        int button, x, y;
        char action = seq[len - 1];
        
        /* Parse SGR mouse format: ESC[<B;X;YM or m */
        if (sscanf(&seq[3], "%d;%d;%d", &button, &x, &y) == 3) {
            event->type = INPUT_MOUSE;
            event->data.mouse.button = button & 0x3;
            event->data.mouse.x = x - 1; /* 0-indexed */
            event->data.mouse.y = y - 1;
            event->data.mouse.action = (action == 'M') ? 0 : 1; /* M=press, m=release */
            
            /* Check for motion */
            if (button & 0x20) {
                event->data.mouse.action = 2;
            }
            
            return 1;
        }
    }
    
    return 0;
}

/* Read input event (non-blocking) */
int terminal_read_event(InputEvent *event, int timeout_ms) {
    unsigned char buf[64];
    int nread;
    fd_set readfds;
    struct timeval tv;
    int ret;
    
    /* Setup for select() */
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    
    /* Wait for input or timeout */
    ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, timeout_ms >= 0 ? &tv : NULL);
    
    if (ret == -1) {
        return -1; /* Error */
    }
    if (ret == 0) {
        event->type = INPUT_NONE;
        return 0; /* Timeout */
    }
    
    /* Read available input */
    nread = read(STDIN_FILENO, buf, sizeof(buf) - 1);
    
    if (nread <= 0) {
        event->type = INPUT_NONE;
        return 0;
    }
    
    buf[nread] = '\0';
    
    /* Parse input */
    if (nread == 1) {
        /* Single character */
        event->type = INPUT_KEY;
        event->data.key.special = 0;
        
        /* Handle special single characters */
        if (buf[0] == 127 || buf[0] == 8) {
            /* Backspace */
            event->data.key.special = 1;
            strcpy(event->data.key.key, "Backspace");
        } else if (buf[0] == '\r' || buf[0] == '\n') {
            /* Enter */
            event->data.key.special = 1;
            strcpy(event->data.key.key, "Enter");
        } else if (buf[0] == '\t') {
            /* Tab */
            event->data.key.special = 1;
            strcpy(event->data.key.key, "Tab");
        } else if (buf[0] == 27) {
            /* Escape */
            event->data.key.special = 1;
            strcpy(event->data.key.key, "Escape");
        } else if (buf[0] == 3) {
            /* Ctrl+C */
            event->data.key.special = 1;
            strcpy(event->data.key.key, "CtrlC");
        } else if (buf[0] == 17) {
            /* Ctrl+Q */
            event->data.key.special = 1;
            strcpy(event->data.key.key, "CtrlQ");
        } else {
            /* Regular character */
            event->data.key.key[0] = buf[0];
            event->data.key.key[1] = '\0';
        }
        return 1;
    } else if (buf[0] == 27) {
        /* Escape sequence */
        return parse_escape_sequence((char*)buf, nread, event);
    }
    
    /* Unknown input */
    event->type = INPUT_NONE;
    return 0;
}

/* Clear screen */
void terminal_clear_screen(void) {
    printf("\033[2J");  /* Clear entire screen */
    printf("\033[H");   /* Move cursor to home */
    fflush(stdout);
}

/* Hide cursor */
void terminal_hide_cursor(void) {
    printf("\033[?25l");
    fflush(stdout);
}

/* Show cursor */
void terminal_show_cursor(void) {
    printf("\033[?25h");
    fflush(stdout);
}

/* Move cursor to position */
void terminal_move_cursor(int row, int col) {
    printf("\033[%d;%dH", row + 1, col + 1);
    fflush(stdout);
}

/* Alternative screen buffer */
void terminal_use_alt_buffer(void) {
    printf("\033[?1049h"); /* Switch to alternate screen */
    fflush(stdout);
}

void terminal_use_main_buffer(void) {
    printf("\033[?1049l"); /* Switch back to main screen */
    fflush(stdout);
}

/* Cleanup on exit */
void terminal_cleanup(void) {
    terminal_disable_mouse();
    terminal_disable_raw_mode();
    terminal_show_cursor();
    terminal_use_main_buffer();
}

/* Initialize terminal for UI */
int terminal_init_ui(void) {
    /* Register cleanup handler */
    atexit(terminal_cleanup);
    
    /* Enable raw mode */
    if (terminal_enable_raw_mode() == -1) {
        return -1;
    }
    
    /* Enable mouse */
    terminal_enable_mouse();
    
    /* Use alternate screen buffer */
    terminal_use_alt_buffer();
    
    /* Hide cursor */
    terminal_hide_cursor();
    
    /* Clear screen */
    terminal_clear_screen();
    
    return 0;
}
