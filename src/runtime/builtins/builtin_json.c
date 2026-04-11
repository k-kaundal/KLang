#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* JSON Builtin Functions */

Value builtin_json_parse_stream(Interpreter *interp, Value *args, int argc) {
    (void)interp;
    if (argc < 2 || args[0].type != VAL_STRING) {
        fprintf(stderr, "Error: JSON.parseStream() requires a string (JSON text) and callback function\n");
        return make_null();
    }
    
    // This is a placeholder - full streaming would require a more complex implementation
    // For now, parse and call callback with the result
    return make_null();
}
