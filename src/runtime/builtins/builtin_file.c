#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Helper function to create a file value */
static Value make_file(const char *path, const char *mode, FILE *fp) {
    Value v;
    v.type = VAL_FILE;
    v.as.file_val.fp = fp;
    v.as.file_val.path = path ? strdup(path) : NULL;
    v.as.file_val.mode = mode ? strdup(mode) : NULL;
    v.as.file_val.is_open = (fp != NULL);
    return v;
}

/* fopen(path, mode) - Open a file */
Value builtin_fopen(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) {
        fprintf(stderr, "Error: fopen requires 2 arguments (path, mode)\n");
        return make_null();
    }
    if (args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: fopen arguments must be strings\n");
        return make_null();
    }
    
    const char *path = args[0].as.str_val;
    const char *mode = args[1].as.str_val;
    
    FILE *fp = fopen(path, mode);
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file '%s' in mode '%s'\n", path, mode);
        return make_null();
    }
    
    return make_file(path, mode, fp);
}

/* fclose(file) - Close an open file */
Value builtin_fclose(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: fclose requires 1 argument (file)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_FILE) {
        fprintf(stderr, "Error: fclose argument must be a file\n");
        return make_bool(0);
    }
    
    if (args[0].as.file_val.is_open && args[0].as.file_val.fp) {
        fclose(args[0].as.file_val.fp);
        args[0].as.file_val.fp = NULL;
        args[0].as.file_val.is_open = 0;
        return make_bool(1);
    }
    
    return make_bool(0);
}

/* fread(file) - Read entire file content */
Value builtin_fread(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: fread requires 1 argument (file)\n");
        return make_string("");
    }
    if (args[0].type != VAL_FILE) {
        fprintf(stderr, "Error: fread argument must be a file\n");
        return make_string("");
    }
    
    if (!args[0].as.file_val.is_open || !args[0].as.file_val.fp) {
        fprintf(stderr, "Error: File is not open\n");
        return make_string("");
    }
    
    FILE *fp = args[0].as.file_val.fp;
    
    /* Get file size */
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    /* Allocate buffer and read */
    char *buffer = malloc(size + 1);
    size_t bytes_read = fread(buffer, 1, size, fp);
    buffer[bytes_read] = '\0';
    
    Value result = make_string(buffer);
    free(buffer);
    return result;
}

/* freadline(file) - Read single line from file */
Value builtin_freadline(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: freadline requires 1 argument (file)\n");
        return make_string("");
    }
    if (args[0].type != VAL_FILE) {
        fprintf(stderr, "Error: freadline argument must be a file\n");
        return make_string("");
    }
    
    if (!args[0].as.file_val.is_open || !args[0].as.file_val.fp) {
        fprintf(stderr, "Error: File is not open\n");
        return make_string("");
    }
    
    char buffer[4096];
    if (fgets(buffer, sizeof(buffer), args[0].as.file_val.fp)) {
        /* Remove trailing newline if present */
        int len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        return make_string(buffer);
    }
    
    return make_string("");
}

/* fwrite(file, content) - Write to file */
Value builtin_fwrite(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) {
        fprintf(stderr, "Error: fwrite requires 2 arguments (file, content)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_FILE) {
        fprintf(stderr, "Error: fwrite first argument must be a file\n");
        return make_bool(0);
    }
    if (args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: fwrite second argument must be a string\n");
        return make_bool(0);
    }
    
    if (!args[0].as.file_val.is_open || !args[0].as.file_val.fp) {
        fprintf(stderr, "Error: File is not open\n");
        return make_bool(0);
    }
    
    const char *content = args[1].as.str_val;
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, args[0].as.file_val.fp);
    fflush(args[0].as.file_val.fp);
    
    return make_bool(written == len);
}

/* fexists(path) - Check if file exists */
Value builtin_fexists(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: fexists requires 1 argument (path)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: fexists argument must be a string\n");
        return make_bool(0);
    }
    
    FILE *fp = fopen(args[0].as.str_val, "r");
    if (fp) {
        fclose(fp);
        return make_bool(1);
    }
    return make_bool(0);
}

/* fdelete(path) - Delete a file */
Value builtin_fdelete(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: fdelete requires 1 argument (path)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: fdelete argument must be a string\n");
        return make_bool(0);
    }
    
    int result = remove(args[0].as.str_val);
    return make_bool(result == 0);
}

/* frename(oldpath, newpath) - Rename/move a file */
Value builtin_frename(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) {
        fprintf(stderr, "Error: frename requires 2 arguments (oldpath, newpath)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: frename arguments must be strings\n");
        return make_bool(0);
    }
    
    int result = rename(args[0].as.str_val, args[1].as.str_val);
    return make_bool(result == 0);
}

/* fsize(path) - Get file size in bytes */
Value builtin_fsize(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: fsize requires 1 argument (path)\n");
        return make_int(-1);
    }
    if (args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: fsize argument must be a string\n");
        return make_int(-1);
    }
    
    FILE *fp = fopen(args[0].as.str_val, "r");
    if (!fp) {
        return make_int(-1);
    }
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    
    return make_int(size);
}

/* readFile(path) - Convenience: read entire file at once */
Value builtin_readFile(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 1) {
        fprintf(stderr, "Error: readFile requires 1 argument (path)\n");
        return make_string("");
    }
    if (args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: readFile argument must be a string\n");
        return make_string("");
    }
    
    FILE *fp = fopen(args[0].as.str_val, "r");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", args[0].as.str_val);
        return make_string("");
    }
    
    /* Get file size */
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    /* Allocate buffer and read */
    char *buffer = malloc(size + 1);
    size_t bytes_read = fread(buffer, 1, size, fp);
    buffer[bytes_read] = '\0';
    fclose(fp);
    
    Value result = make_string(buffer);
    free(buffer);
    return result;
}

/* writeFile(path, content) - Convenience: write entire file at once */
Value builtin_writeFile(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) {
        fprintf(stderr, "Error: writeFile requires 2 arguments (path, content)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: writeFile arguments must be strings\n");
        return make_bool(0);
    }
    
    FILE *fp = fopen(args[0].as.str_val, "w");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file '%s' for writing\n", args[0].as.str_val);
        return make_bool(0);
    }
    
    const char *content = args[1].as.str_val;
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, fp);
    fclose(fp);
    
    return make_bool(written == len);
}

/* appendFile(path, content) - Convenience: append to file */
Value builtin_appendFile(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2) {
        fprintf(stderr, "Error: appendFile requires 2 arguments (path, content)\n");
        return make_bool(0);
    }
    if (args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: appendFile arguments must be strings\n");
        return make_bool(0);
    }
    
    FILE *fp = fopen(args[0].as.str_val, "a");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file '%s' for appending\n", args[0].as.str_val);
        return make_bool(0);
    }
    
    const char *content = args[1].as.str_val;
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, fp);
    fclose(fp);
    
    return make_bool(written == len);
}
