#include "builtins.h"
#include <stdio.h>
#include <string.h>

Value builtin_print(Interpreter *interp, Value *args, int argc) {
    int i;
    (void)interp;
    for (i = 0; i < argc; i++) {
        if (i > 0) printf(" ");
        value_print(&args[i]);
    }
    return make_null();
}

Value builtin_println(Interpreter *interp, Value *args, int argc) {
    int i;
    (void)interp;
    for (i = 0; i < argc; i++) {
        if (i > 0) printf(" ");
        value_print(&args[i]);
    }
    printf("\n");
    return make_null();
}

Value builtin_input(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    char buf[1024];
    if (argc > 0) value_print(&args[0]);
    if (fgets(buf, sizeof(buf), stdin)) {
        int len = (int)strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
        return make_string(buf);
    }
    return make_string("");
}
