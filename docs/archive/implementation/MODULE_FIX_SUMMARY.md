# ES6 Module Function Export Memory Management Fix

## Problem Summary

The KLang ES6 module system had critical memory management issues causing segmentation faults when exporting and importing functions. While constant exports worked correctly, function exports would crash the interpreter.

## Root Causes Identified

### 1. Premature Environment Cleanup
The module's internal environment (`module_env`) was being freed immediately after module loading, but exported functions had closures that referenced this environment, causing dangling pointers.

### 2. Premature AST Node Cleanup  
Function bodies in the FunctionVal struct pointed directly to AST nodes. These nodes were being freed after module loading, making function bodies invalid when the functions were later called.

### 3. Shallow Copying of Function Values
When functions were copied between environments (e.g., from `module_env` to `export_env`), only shallow copies were made. This caused:
- Multiple environments pointing to the same `param_names` array
- Double-free errors during cleanup
- Shared closure pointers without proper lifetime management

### 4. Default Export Handling
Named function declarations (e.g., `export default fn square(x)`) return `null` after binding to the environment, but the default export handler wasn't retrieving the actual function value.

## Solution Implemented

### 1. Extended Module Lifetime Management

**Modified Structures:**
```c
// src/interpreter.h
typedef struct {
    char *module_path;
    Env *exports;
    Env *module_env;    // NEW: Keep alive for closures
} ModuleVal;

typedef struct LoadedModule {
    char *path;
    Env *exports;
    Env *module_env;    // NEW: Store for lifetime management
    ASTNode **ast_nodes; // NEW: Keep function bodies alive
    int ast_count;
    int is_loading;
} LoadedModule;
```

**Changes:**
- Module environments are now kept alive as long as the module is cached
- Freed only when the interpreter shuts down
- Module cache stores both `exports` and `module_env`

### 2. AST Node Lifetime Management

**Before:**
```c
// Module loader freed AST nodes immediately
for (i = 0; i < count; i++) ast_free(nodes[i]);
free(nodes);
```

**After:**
```c
// AST nodes are passed to module cache and kept alive
cache_module(interp, module_path, export_env, module_env, nodes, count);

// Freed only during interpreter cleanup
for (j = 0; j < interp->loaded_modules[i].ast_count; j++) {
    ast_free(interp->loaded_modules[i].ast_nodes[j]);
}
```

### 3. Deep Copying of Function Values

**Modified `env_set_local()`:**
```c
void env_set_local(Env *env, const char *name, Value val) {
    Value val_copy = val;
    if (val.type == VAL_STRING && val.as.str_val) {
        val_copy.as.str_val = strdup(val.as.str_val);
    } else if (val.type == VAL_FUNCTION && val.as.func_val.param_names) {
        // NEW: Deep copy function parameter names
        val_copy.as.func_val.param_names = malloc(val.as.func_val.param_count * sizeof(char *));
        for (i = 0; i < val.as.func_val.param_count; i++) {
            val_copy.as.func_val.param_names[i] = strdup(val.as.func_val.param_names[i]);
        }
        // Note: body and closure are shared pointers - don't copy
    }
    // ... rest of function
}
```

**Benefits:**
- Each environment owns its own copy of parameter names
- No double-free errors during cleanup
- Shared closures and AST body pointers (intentionally - managed separately)

### 4. Default Export Fix

**Added Logic to Handle Named Functions:**
```c
if (node->data.export_stmt.is_default) {
    Value val = eval_node_env(interp, node->data.export_stmt.declaration, module_env);
    
    // NEW: Named functions/classes return null but bind to module_env
    if (val.type == VAL_NULL && node->data.export_stmt.declaration) {
        const char *name = NULL;
        if (node->data.export_stmt.declaration->type == NODE_FUNC_DEF) {
            name = node->data.export_stmt.declaration->data.func_def.name;
        } else if (node->data.export_stmt.declaration->type == NODE_CLASS_DEF) {
            name = node->data.export_stmt.declaration->data.class_def.name;
        }
        
        if (name && strlen(name) > 0) {
            Value *func_val = env_get(module_env, name);
            if (func_val) {
                val = *func_val;
            }
        }
    }
    
    env_set_local(export_env, "default", val);
}
```

## Testing

Created comprehensive test suite covering:

1. ✓ Constant exports
2. ✓ Function exports (single and multiple)
3. ✓ Default exports (functions and values)
4. ✓ Class exports
5. ✓ Mixed exports (constants + functions)

All tests pass without crashes or memory leaks.

## Files Modified

- `src/interpreter.h` - Updated structures
- `src/interpreter.c` - Implemented memory management fixes
- `examples/modules/` - Added test files
- `test_modules.sh` - Test suite
- `ES6_MODULES_IMPLEMENTATION.md` - Updated documentation

## Performance Implications

**Memory Usage:**
- Slight increase due to keeping module environments and AST nodes alive
- Trade-off for correctness and stability
- Memory is properly freed during interpreter shutdown

**Runtime:**
- No performance impact on module loading or function execution
- Deep copying param_names is negligible (small arrays)

## Conclusion

The module system is now fully functional with proper memory management. Functions, classes, and constants can all be exported and imported without crashes. The fix maintains the simplicity of the original design while ensuring proper lifetime management of all resources.
