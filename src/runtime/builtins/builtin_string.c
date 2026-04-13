#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <regex.h>
#endif

/* Forward declaration from interpreter.c */
extern char *value_to_string(Value *v);

Value builtin_str(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0) return make_string("");
    char *str = value_to_string(&args[0]);
    Value v = make_string(str);
    free(str);
    return v;
}

Value builtin_uppercase(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0 || args[0].type != VAL_STRING) return make_string("");
    char *result = strdup(args[0].as.str_val);
    for (int i = 0; result[i]; i++) {
        if (result[i] >= 'a' && result[i] <= 'z') {
            result[i] = result[i] - 'a' + 'A';
        }
    }
    Value v = make_string(result);
    free(result);
    return v;
}

Value builtin_lowercase(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0 || args[0].type != VAL_STRING) return make_string("");
    char *result = strdup(args[0].as.str_val);
    for (int i = 0; result[i]; i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] - 'A' + 'a';
        }
    }
    Value v = make_string(result);
    free(result);
    return v;
}

Value builtin_trim(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc == 0 || args[0].type != VAL_STRING) return make_string("");
    const char *str = args[0].as.str_val;
    
    /* Find start */
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') str++;
    
    /* Find end */
    int len = strlen(str);
    while (len > 0 && (str[len-1] == ' ' || str[len-1] == '\t' || 
                       str[len-1] == '\n' || str[len-1] == '\r')) {
        len--;
    }
    
    char *result = malloc(len + 1);
    memcpy(result, str, len);
    result[len] = '\0';
    Value v = make_string(result);
    free(result);
    return v;
}

Value builtin_split(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        Value empty_list;
        empty_list.type = VAL_LIST;
        empty_list.as.list_val.items = NULL;
        empty_list.as.list_val.count = 0;
        empty_list.as.list_val.capacity = 0;
        return empty_list;
    }
    
    const char *str = args[0].as.str_val;
    const char *delim = args[1].as.str_val;
    int delim_len = strlen(delim);
    
    /* Count tokens */
    int count = 1;
    const char *p = str;
    if (delim_len == 0) {
        /* Split into characters */
        count = strlen(str);
    } else {
        while ((p = strstr(p, delim)) != NULL) {
            count++;
            p += delim_len;
        }
    }
    
    /* Allocate list */
    Value result;
    result.type = VAL_LIST;
    result.as.list_val.items = malloc(count * sizeof(Value));
    result.as.list_val.count = count;
    result.as.list_val.capacity = count;
    
    /* Split string */
    int idx = 0;
    
    if (delim_len == 0) {
        /* Split into characters */
        for (int i = 0; str[i]; i++) {
            char buf[2] = {str[i], '\0'};
            result.as.list_val.items[idx++] = make_string(buf);
        }
    } else {
        p = str;
        const char *next;
        while ((next = strstr(p, delim)) != NULL) {
            int len = next - p;
            char *token = malloc(len + 1);
            memcpy(token, p, len);
            token[len] = '\0';
            result.as.list_val.items[idx++] = make_string(token);
            free(token);
            p = next + delim_len;
        }
        /* Last token */
        result.as.list_val.items[idx++] = make_string(p);
    }
    
    return result;
}

Value builtin_join(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_LIST || args[1].type != VAL_STRING) {
        return make_string("");
    }
    
    ListVal *list = &args[0].as.list_val;
    const char *delim = args[1].as.str_val;
    
    if (list->count == 0) return make_string("");
    
    /* Calculate total length */
    int total_len = 0;
    for (int i = 0; i < list->count; i++) {
        char *s = value_to_string(&list->items[i]);
        total_len += strlen(s);
        free(s);
    }
    total_len += strlen(delim) * (list->count - 1);
    
    /* Build result */
    char *result = malloc(total_len + 1);
    char *ptr = result;
    
    for (int i = 0; i < list->count; i++) {
        if (i > 0) {
            int delim_len = strlen(delim);
            memcpy(ptr, delim, delim_len);
            ptr += delim_len;
        }
        char *s = value_to_string(&list->items[i]);
        int s_len = strlen(s);
        memcpy(ptr, s, s_len);
        ptr += s_len;
        free(s);
    }
    *ptr = '\0';
    
    Value v = make_string(result);
    free(result);
    return v;
}

Value builtin_substring(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        return make_string("");
    }
    
    const char *str = args[0].as.str_val;
    int len = strlen(str);
    int start = (int)args[1].as.int_val;
    int end = len;
    
    if (argc >= 3 && args[2].type == VAL_INT) {
        end = (int)args[2].as.int_val;
    }
    
    /* Bounds checking */
    if (start < 0) start = 0;
    if (end > len) end = len;
    if (start >= end) return make_string("");
    
    int sub_len = end - start;
    char *result = malloc(sub_len + 1);
    memcpy(result, str + start, sub_len);
    result[sub_len] = '\0';
    
    Value v = make_string(result);
    free(result);
    return v;
}

Value builtin_startswith(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        return make_bool(0);
    }
    
    const char *str = args[0].as.str_val;
    const char *prefix = args[1].as.str_val;
    int prefix_len = strlen(prefix);
    
    if (strlen(str) < (size_t)prefix_len) return make_bool(0);
    return make_bool(strncmp(str, prefix, prefix_len) == 0);
}

Value builtin_endswith(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        return make_bool(0);
    }
    
    const char *str = args[0].as.str_val;
    const char *suffix = args[1].as.str_val;
    int str_len = strlen(str);
    int suffix_len = strlen(suffix);
    
    if (str_len < suffix_len) return make_bool(0);
    return make_bool(strcmp(str + str_len - suffix_len, suffix) == 0);
}

Value builtin_contains(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        return make_bool(0);
    }
    
    const char *str = args[0].as.str_val;
    const char *substr = args[1].as.str_val;
    
    return make_bool(strstr(str, substr) != NULL);
}

Value builtin_indexof(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        return make_int(-1);
    }
    
    const char *str = args[0].as.str_val;
    const char *substr = args[1].as.str_val;
    const char *pos = strstr(str, substr);
    
    if (pos == NULL) return make_int(-1);
    return make_int(pos - str);
}

Value builtin_replace(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 3 || args[0].type != VAL_STRING || 
        args[1].type != VAL_STRING || args[2].type != VAL_STRING) {
        return make_string("");
    }
    
    const char *str = args[0].as.str_val;
    const char *old = args[1].as.str_val;
    const char *new = args[2].as.str_val;
    
    int old_len = strlen(old);
    int new_len = strlen(new);
    
    if (old_len == 0) return make_string(str);
    
    /* Count occurrences */
    int count = 0;
    const char *p = str;
    while ((p = strstr(p, old)) != NULL) {
        count++;
        p += old_len;
    }
    
    if (count == 0) return make_string(str);
    
    /* Allocate result */
    int result_len = strlen(str) + count * (new_len - old_len);
    char *result = malloc(result_len + 1);
    char *dst = result;
    
    /* Replace all occurrences */
    p = str;
    const char *next;
    while ((next = strstr(p, old)) != NULL) {
        int len = next - p;
        memcpy(dst, p, len);
        dst += len;
        memcpy(dst, new, new_len);
        dst += new_len;
        p = next + old_len;
    }
    strcpy(dst, p);
    
    Value v = make_string(result);
    free(result);
    return v;
}

Value builtin_repeat(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        return make_string("");
    }
    
    const char *str = args[0].as.str_val;
    int times = (int)args[1].as.int_val;
    
    if (times <= 0) return make_string("");
    
    int str_len = strlen(str);
    int result_len = str_len * times;
    char *result = malloc(result_len + 1);
    char *ptr = result;
    
    for (int i = 0; i < times; i++) {
        memcpy(ptr, str, str_len);
        ptr += str_len;
    }
    *ptr = '\0';
    
    Value v = make_string(result);
    free(result);
    return v;
}

Value builtin_regexTest(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: regexTest requires two string arguments (pattern, text)\n");
        return make_bool(0);
    }
    
#ifdef _WIN32
    /* Regex not supported on Windows - return false */
    fprintf(stderr, "Warning: Regex not supported on Windows\n");
    return make_bool(0);
#else
    const char *pattern = args[0].as.str_val;
    const char *text = args[1].as.str_val;
    
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    
    if (result != 0) {
        fprintf(stderr, "Error: Invalid regex pattern: %s\n", pattern);
        regfree(&regex);
        return make_bool(0);
    }
    
    result = regexec(&regex, text, 0, NULL, 0);
    regfree(&regex);
    
    return make_bool(result == 0);
#endif
}

Value builtin_regexMatch(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: regexMatch requires two string arguments (pattern, text)\n");
        return make_null();
    }
    
#ifdef _WIN32
    /* Regex not supported on Windows - return null */
    fprintf(stderr, "Warning: Regex not supported on Windows\n");
    return make_null();
#else
    const char *pattern = args[0].as.str_val;
    const char *text = args[1].as.str_val;
    
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    
    if (result != 0) {
        fprintf(stderr, "Error: Invalid regex pattern: %s\n", pattern);
        regfree(&regex);
        return make_null();
    }
    
    regmatch_t matches[10];  // Support up to 10 capture groups
    result = regexec(&regex, text, 10, matches, 0);
    
    if (result != 0) {
        regfree(&regex);
        return make_null();
    }
    
    // Count actual matches
    int match_count = 0;
    for (int i = 0; i < 10 && matches[i].rm_so != -1; i++) {
        match_count++;
    }
    
    // Create array of matches
    Value match_list;
    match_list.type = VAL_LIST;
    match_list.as.list_val.items = malloc(match_count * sizeof(Value));
    match_list.as.list_val.count = match_count;
    match_list.as.list_val.capacity = match_count;
    
    for (int i = 0; i < match_count; i++) {
        int len = matches[i].rm_eo - matches[i].rm_so;
        char *match_str = malloc(len + 1);
        strncpy(match_str, text + matches[i].rm_so, len);
        match_str[len] = '\0';
        
        match_list.as.list_val.items[i] = make_string(match_str);
        free(match_str);
    }
    
    regfree(&regex);
    return match_list;
#endif
}

Value builtin_regexReplace(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 3 || args[0].type != VAL_STRING || 
        args[1].type != VAL_STRING || args[2].type != VAL_STRING) {
        fprintf(stderr, "Error: regexReplace requires three string arguments (pattern, text, replacement)\n");
        return make_null();
    }
    
#ifdef _WIN32
    /* Regex not supported on Windows - return original text */
    fprintf(stderr, "Warning: Regex not supported on Windows\n");
    return make_string(args[1].as.str_val);
#else
    const char *pattern = args[0].as.str_val;
    const char *text = args[1].as.str_val;
    const char *replacement = args[2].as.str_val;
    
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    
    if (result != 0) {
        fprintf(stderr, "Error: Invalid regex pattern: %s\n", pattern);
        regfree(&regex);
        return make_string((char*)text);
    }
    
    regmatch_t match;
    result = regexec(&regex, text, 1, &match, 0);
    
    if (result != 0) {
        regfree(&regex);
        return make_string((char*)text);
    }
    
    // Build result string with replacement
    int text_len = strlen(text);
    int repl_len = strlen(replacement);
    int new_len = text_len - (match.rm_eo - match.rm_so) + repl_len;
    char *new_text = malloc(new_len + 1);
    
    // Copy before match
    strncpy(new_text, text, match.rm_so);
    // Copy replacement
    strcpy(new_text + match.rm_so, replacement);
    // Copy after match
    strcpy(new_text + match.rm_so + repl_len, text + match.rm_eo);
    
    Value result_val = make_string(new_text);
    free(new_text);
    regfree(&regex);
    return result_val;
#endif
}

Value builtin_regexSplit(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        fprintf(stderr, "Error: regexSplit requires two string arguments (pattern, text)\n");
        return make_null();
    }
    
#ifdef _WIN32
    /* Regex not supported on Windows - return array with original text */
    fprintf(stderr, "Warning: Regex not supported on Windows\n");
    Value list;
    list.type = VAL_LIST;
    list.as.list_val.items = malloc(sizeof(Value));
    list.as.list_val.count = 1;
    list.as.list_val.capacity = 1;
    list.as.list_val.items[0] = make_string(args[1].as.str_val);
    return list;
#else
    const char *pattern = args[0].as.str_val;
    const char *text = args[1].as.str_val;
    
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    
    if (result != 0) {
        fprintf(stderr, "Error: Invalid regex pattern: %s\n", pattern);
        regfree(&regex);
        // Return array with original text
        Value list;
        list.type = VAL_LIST;
        list.as.list_val.items = malloc(sizeof(Value));
        list.as.list_val.count = 1;
        list.as.list_val.capacity = 1;
        list.as.list_val.items[0] = make_string((char*)text);
        return list;
    }
    
    // First, count how many splits we'll have
    int split_count = 1;  // At least one part
    const char *current = text;
    regmatch_t match;
    
    while (regexec(&regex, current, 1, &match, 0) == 0) {
        split_count++;
        current += match.rm_eo;
        if (match.rm_eo == 0) break;  // Prevent infinite loop
    }
    
    // Create result list
    Value list;
    list.type = VAL_LIST;
    list.as.list_val.items = malloc(split_count * sizeof(Value));
    list.as.list_val.capacity = split_count;
    list.as.list_val.count = 0;
    
    // Now do the actual splitting
    current = text;
    while (regexec(&regex, current, 1, &match, 0) == 0) {
        // Add part before match
        if (match.rm_so >= 0) {
            char *part = malloc(match.rm_so + 1);
            strncpy(part, current, match.rm_so);
            part[match.rm_so] = '\0';
            list.as.list_val.items[list.as.list_val.count++] = make_string(part);
            free(part);
        }
        
        // Move past the match
        current += match.rm_eo;
        
        // Prevent infinite loop on empty matches
        if (match.rm_eo == 0) break;
    }
    
    // Add remaining text
    if (*current) {
        list.as.list_val.items[list.as.list_val.count++] = make_string((char*)current);
    }
    
    regfree(&regex);
    return list;
#endif
}
