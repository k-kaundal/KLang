/**
 * security.c - Security Context Implementation
 * 
 * Implements the security context and unsafe block tracking for KLang.
 * Provides the foundation for security-by-default execution model.
 * 
 * Architecture:
 *   - UnsafeContext: Tracks unsafe block nesting with line/file info
 *   - SecurityContext: Central security state integrating all security features
 *   - Security Levels: Configurable enforcement from permissive to paranoid
 *   - Audit Logging: Comprehensive logging of security events
 * 
 * Security Levels:
 *   - PERMISSIVE: No enforcement (backwards compatible, v1.x default)
 *   - WARN: Warn about violations but allow execution
 *   - STRICT: Enforce all security policies
 *   - PARANOID: Strictest enforcement with additional checks
 */

#include "security.h"
#include "capability.h"
#include "../../interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/* ===================================================================
 * UnsafeContext Operations
 * =================================================================== */

/**
 * Create a new unsafe context
 */
UnsafeContext *unsafe_context_new(void) {
    UnsafeContext *ctx = (UnsafeContext *)calloc(1, sizeof(UnsafeContext));
    if (!ctx) {
        return NULL;
    }
    
    ctx->in_unsafe_block = 0;
    ctx->unsafe_depth = 0;
    ctx->line_number = 0;
    ctx->file_name = NULL;
    
    return ctx;
}

/**
 * Free an unsafe context
 */
void unsafe_context_free(UnsafeContext *ctx) {
    if (!ctx) {
        return;
    }
    
    /* Note: file_name is not owned by us, don't free it */
    free(ctx);
}

/**
 * Enter an unsafe block
 * 
 * Increments the unsafe depth and sets in_unsafe_block flag.
 * Records the location for error reporting.
 */
void unsafe_context_enter(UnsafeContext *ctx, int line, const char *file) {
    if (!ctx) {
        return;
    }
    
    ctx->unsafe_depth++;
    ctx->in_unsafe_block = 1;
    
    /* Record location of first unsafe block in the chain */
    if (ctx->unsafe_depth == 1) {
        ctx->line_number = line;
        ctx->file_name = file;
    }
}

/**
 * Exit an unsafe block
 * 
 * Decrements the unsafe depth. When depth reaches 0, clears in_unsafe_block flag.
 */
void unsafe_context_exit(UnsafeContext *ctx) {
    if (!ctx) {
        return;
    }
    
    if (ctx->unsafe_depth > 0) {
        ctx->unsafe_depth--;
    }
    
    /* Clear flag when we exit all unsafe blocks */
    if (ctx->unsafe_depth == 0) {
        ctx->in_unsafe_block = 0;
        ctx->line_number = 0;
        ctx->file_name = NULL;
    }
}

/**
 * Check if currently in an unsafe context
 */
int unsafe_context_is_unsafe(UnsafeContext *ctx) {
    if (!ctx) {
        return 0;
    }
    
    return ctx->in_unsafe_block;
}

/**
 * Get current unsafe depth
 */
int unsafe_context_depth(UnsafeContext *ctx) {
    if (!ctx) {
        return 0;
    }
    
    return ctx->unsafe_depth;
}

/* ===================================================================
 * SecurityContext Operations
 * =================================================================== */

/**
 * Create a new security context
 * 
 * Initializes with default settings (permissive mode for backwards compatibility).
 */
SecurityContext *security_context_new(void) {
    SecurityContext *ctx = (SecurityContext *)calloc(1, sizeof(SecurityContext));
    if (!ctx) {
        return NULL;
    }
    
    /* Initialize unsafe context */
    ctx->unsafe_ctx = unsafe_context_new();
    if (!ctx->unsafe_ctx) {
        free(ctx);
        return NULL;
    }
    
    /* Initialize capability context */
    ctx->capability_ctx = capability_context_new();
    if (!ctx->capability_ctx) {
        unsafe_context_free(ctx->unsafe_ctx);
        free(ctx);
        return NULL;
    }
    
    /* Default configuration: permissive mode for v1.x compatibility */
    ctx->security_level = SECURITY_LEVEL_PERMISSIVE;
    ctx->strict_mode = 0;
    ctx->warn_only = 0;
    
    /* Audit logging disabled by default */
    ctx->audit_enabled = 0;
    ctx->audit_log = NULL;
    
    /* Initialize statistics */
    ctx->unsafe_ops_count = 0;
    ctx->cap_checks_count = 0;
    ctx->cap_denials_count = 0;
    
    /* Allow fallback to permissive mode on errors (for compatibility) */
    ctx->allow_permissive_fallback = 1;
    
    return ctx;
}

/**
 * Free a security context and all its resources
 */
void security_context_free(SecurityContext *ctx) {
    if (!ctx) {
        return;
    }
    
    /* Free sub-contexts */
    unsafe_context_free(ctx->unsafe_ctx);
    capability_context_free(ctx->capability_ctx);
    
    /* Close audit log if it's a file we opened */
    if (ctx->audit_log && ctx->audit_log != stderr && ctx->audit_log != stdout) {
        fclose(ctx->audit_log);
    }
    
    free(ctx);
}

/**
 * Set the security level
 */
void security_context_set_level(SecurityContext *ctx, SecurityLevel level) {
    if (!ctx) {
        return;
    }
    
    ctx->security_level = level;
    
    /* Update convenience flags */
    ctx->strict_mode = (level == SECURITY_LEVEL_STRICT || level == SECURITY_LEVEL_PARANOID);
    ctx->warn_only = (level == SECURITY_LEVEL_WARN);
}

/**
 * Get the current security level
 */
SecurityLevel security_context_get_level(SecurityContext *ctx) {
    if (!ctx) {
        return SECURITY_LEVEL_PERMISSIVE;
    }
    
    return ctx->security_level;
}

/**
 * Enable strict mode (shorthand for SECURITY_LEVEL_STRICT)
 */
void security_context_enable_strict_mode(SecurityContext *ctx) {
    security_context_set_level(ctx, SECURITY_LEVEL_STRICT);
}

/**
 * Enable warn-only mode (shorthand for SECURITY_LEVEL_WARN)
 */
void security_context_enable_warn_mode(SecurityContext *ctx) {
    security_context_set_level(ctx, SECURITY_LEVEL_WARN);
}

/**
 * Enable permissive mode (shorthand for SECURITY_LEVEL_PERMISSIVE)
 */
void security_context_enable_permissive_mode(SecurityContext *ctx) {
    security_context_set_level(ctx, SECURITY_LEVEL_PERMISSIVE);
}

/**
 * Enable audit logging
 */
void security_context_enable_audit(SecurityContext *ctx, const char *log_file) {
    if (!ctx) {
        return;
    }
    
    ctx->audit_enabled = 1;
    
    /* Open log file if specified */
    if (log_file) {
        ctx->audit_log = fopen(log_file, "a");
        if (!ctx->audit_log) {
            fprintf(stderr, "Warning: Failed to open audit log '%s', using stderr\n", 
                   log_file);
            ctx->audit_log = stderr;
        }
    } else {
        ctx->audit_log = stderr;
    }
    
    /* Also enable for capability context */
    if (ctx->capability_ctx) {
        ctx->capability_ctx->audit_enabled = 1;
        ctx->capability_ctx->audit_log = ctx->audit_log;
    }
}

/**
 * Disable audit logging
 */
void security_context_disable_audit(SecurityContext *ctx) {
    if (!ctx) {
        return;
    }
    
    /* Close log file if it's a file we opened */
    if (ctx->audit_log && ctx->audit_log != stderr && ctx->audit_log != stdout) {
        fclose(ctx->audit_log);
    }
    
    ctx->audit_enabled = 0;
    ctx->audit_log = NULL;
    
    /* Also disable for capability context */
    if (ctx->capability_ctx) {
        ctx->capability_ctx->audit_enabled = 0;
        ctx->capability_ctx->audit_log = NULL;
    }
}

/**
 * Get security statistics
 */
void security_context_get_stats(SecurityContext *ctx, 
                                unsigned long *unsafe_ops,
                                unsigned long *cap_checks,
                                unsigned long *cap_denials) {
    if (!ctx) {
        if (unsafe_ops) *unsafe_ops = 0;
        if (cap_checks) *cap_checks = 0;
        if (cap_denials) *cap_denials = 0;
        return;
    }
    
    if (unsafe_ops) {
        *unsafe_ops = ctx->unsafe_ops_count;
    }
    if (cap_checks) {
        *cap_checks = ctx->cap_checks_count;
    }
    if (cap_denials) {
        *cap_denials = ctx->cap_denials_count;
    }
}

/**
 * Reset security statistics
 */
void security_context_reset_stats(SecurityContext *ctx) {
    if (!ctx) {
        return;
    }
    
    ctx->unsafe_ops_count = 0;
    ctx->cap_checks_count = 0;
    ctx->cap_denials_count = 0;
}

/* ===================================================================
 * Unsafe Operation Checking
 * =================================================================== */

/**
 * Get security context from interpreter
 * 
 * Helper function to safely extract security context.
 * Returns NULL if not available (permissive mode).
 */
static SecurityContext *get_security_context(Interpreter *interp) {
    /* Note: In Phase 2, this will be: return interp->security_ctx; */
    /* For now, return NULL to indicate permissive mode */
    (void)interp; /* Suppress unused parameter warning */
    return NULL;
}

/**
 * Check if an operation requires unsafe context
 * 
 * Behavior depends on security level:
 *   - PERMISSIVE: Always allows, no warnings
 *   - WARN: Allows but prints warning if not in unsafe block
 *   - STRICT: Requires unsafe block, errors if not present
 *   - PARANOID: Like strict, plus additional validation
 */
void check_unsafe_operation(Interpreter *interp, const char *operation) {
    SecurityContext *sec_ctx;
    
    if (!interp || !operation) {
        return;
    }
    
    sec_ctx = get_security_context(interp);
    
    /* No security context = permissive mode */
    if (!sec_ctx) {
        return;
    }
    
    /* Increment counter */
    sec_ctx->unsafe_ops_count++;
    
    /* Check security level */
    switch (sec_ctx->security_level) {
        case SECURITY_LEVEL_PERMISSIVE:
            /* Allow everything, no warnings */
            return;
            
        case SECURITY_LEVEL_WARN:
            /* Allow but warn if not in unsafe block */
            if (!unsafe_context_is_unsafe(sec_ctx->unsafe_ctx)) {
                fprintf(stderr, "Warning: Unsafe operation '%s' outside unsafe block\n", 
                       operation);
                fprintf(stderr, "Hint: Wrap this code in an 'unsafe { ... }' block\n");
                
                /* Log to audit if enabled */
                if (sec_ctx->audit_enabled && sec_ctx->audit_log) {
                    fprintf(sec_ctx->audit_log, "[WARN] Unsafe operation outside block: %s\n", 
                           operation);
                    fflush(sec_ctx->audit_log);
                }
            }
            return;
            
        case SECURITY_LEVEL_STRICT:
        case SECURITY_LEVEL_PARANOID:
            /* Require unsafe block */
            if (!unsafe_context_is_unsafe(sec_ctx->unsafe_ctx)) {
                fprintf(stderr, "Security Error: Unsafe operation '%s' requires unsafe block\n", 
                       operation);
                fprintf(stderr, "Hint: Wrap this code in an 'unsafe { ... }' block\n");
                
                /* Log to audit if enabled */
                if (sec_ctx->audit_enabled && sec_ctx->audit_log) {
                    fprintf(sec_ctx->audit_log, "[DENIED] Unsafe operation outside block: %s\n", 
                           operation);
                    fflush(sec_ctx->audit_log);
                }
                
                interp->had_error = 1;
                return;
            }
            
            /* Log successful check in paranoid mode */
            if (sec_ctx->security_level == SECURITY_LEVEL_PARANOID && 
                sec_ctx->audit_enabled && sec_ctx->audit_log) {
                fprintf(sec_ctx->audit_log, "[ALLOWED] Unsafe operation in block: %s (depth=%d)\n", 
                       operation, unsafe_context_depth(sec_ctx->unsafe_ctx));
                fflush(sec_ctx->audit_log);
            }
            return;
    }
}

/**
 * Require unsafe context for an operation
 * 
 * Stricter version that always enforces unsafe context requirement
 * regardless of security level (except PERMISSIVE mode).
 */
void require_unsafe_operation(Interpreter *interp, const char *operation) {
    SecurityContext *sec_ctx;
    
    if (!interp || !operation) {
        return;
    }
    
    sec_ctx = get_security_context(interp);
    
    /* No security context = permissive mode */
    if (!sec_ctx) {
        return;
    }
    
    /* Permissive mode allows everything */
    if (sec_ctx->security_level == SECURITY_LEVEL_PERMISSIVE) {
        return;
    }
    
    /* Increment counter */
    sec_ctx->unsafe_ops_count++;
    
    /* Require unsafe block */
    if (!unsafe_context_is_unsafe(sec_ctx->unsafe_ctx)) {
        fprintf(stderr, "Security Error: Operation '%s' requires unsafe block\n", 
               operation);
        fprintf(stderr, "Hint: Wrap this code in an 'unsafe { ... }' block\n");
        
        /* Log to audit if enabled */
        if (sec_ctx->audit_enabled && sec_ctx->audit_log) {
            fprintf(sec_ctx->audit_log, "[DENIED] Required unsafe operation outside block: %s\n", 
                   operation);
            fflush(sec_ctx->audit_log);
        }
        
        interp->had_error = 1;
    }
}

/**
 * Check if currently in unsafe context
 */
int is_in_unsafe_context(Interpreter *interp) {
    SecurityContext *sec_ctx;
    
    if (!interp) {
        return 0;
    }
    
    sec_ctx = get_security_context(interp);
    
    if (!sec_ctx || !sec_ctx->unsafe_ctx) {
        return 0;
    }
    
    return unsafe_context_is_unsafe(sec_ctx->unsafe_ctx);
}

/* ===================================================================
 * Integration Points for Interpreter
 * =================================================================== */

/**
 * Initialize security for an interpreter
 * 
 * Creates and attaches a SecurityContext to the interpreter.
 * Should be called during interpreter initialization.
 */
int security_init(Interpreter *interp) {
    SecurityContext *sec_ctx;
    
    if (!interp) {
        return -1;
    }
    
    /* Create security context */
    sec_ctx = security_context_new();
    if (!sec_ctx) {
        fprintf(stderr, "Error: Failed to initialize security context\n");
        return -1;
    }
    
    /* Note: In Phase 2, this will be: interp->security_ctx = sec_ctx; */
    /* For now, we just create it and free it to test allocation */
    security_context_free(sec_ctx);
    
    return 0;
}

/**
 * Shutdown security for an interpreter
 * 
 * Frees the SecurityContext and all associated resources.
 * Should be called during interpreter cleanup.
 */
void security_shutdown(Interpreter *interp) {
    SecurityContext *sec_ctx;
    
    if (!interp) {
        return;
    }
    
    sec_ctx = get_security_context(interp);
    
    if (sec_ctx) {
        /* Unregister from global manager if it exists */
        if (sec_ctx->capability_ctx && capability_manager_get()) {
            capability_manager_unregister(sec_ctx->capability_ctx);
        }
        
        /* Free the security context */
        security_context_free(sec_ctx);
        
        /* Note: In Phase 2, this will be: interp->security_ctx = NULL; */
    }
}

/**
 * Configure security from command-line flags
 * 
 * Parses and applies security configuration from CLI arguments.
 */
void security_configure_from_cli(Interpreter *interp,
                                 int strict,
                                 int warn,
                                 int unsafe_all,
                                 const char *audit_file) {
    SecurityContext *sec_ctx;
    
    if (!interp) {
        return;
    }
    
    sec_ctx = get_security_context(interp);
    
    if (!sec_ctx) {
        /* No security context yet - create one if any security flags are set */
        if (strict || warn || !unsafe_all || audit_file) {
            if (security_init(interp) != 0) {
                return;
            }
            sec_ctx = get_security_context(interp);
            if (!sec_ctx) {
                return;
            }
        } else {
            /* No security flags, stay in permissive mode */
            return;
        }
    }
    
    /* Apply security level */
    if (unsafe_all) {
        /* --unsafe-all disables all security checks */
        security_context_enable_permissive_mode(sec_ctx);
        
        /* Grant all capabilities */
        if (sec_ctx->capability_ctx) {
            capability_grant(sec_ctx->capability_ctx, CAP_ALL, NULL);
        }
    } else if (strict) {
        security_context_enable_strict_mode(sec_ctx);
    } else if (warn) {
        security_context_enable_warn_mode(sec_ctx);
    }
    
    /* Enable audit logging if requested */
    if (audit_file) {
        security_context_enable_audit(sec_ctx, audit_file);
    }
}

/* ===================================================================
 * Unsafe Operation Registry & Audit Logging
 * =================================================================== */

/**
 * Get current timestamp for audit log
 */
static void get_timestamp(char *buffer, size_t size) {
    time_t now;
    struct tm *tm_info;
    
    time(&now);
    tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

/**
 * Register an unsafe operation in the audit log
 */
void security_log_unsafe_operation(Interpreter *interp,
                                   UnsafeOperationType op_type,
                                   const char *operation,
                                   const char *resource) {
    SecurityContext *sec_ctx;
    char timestamp[32];
    
    if (!interp || !operation) {
        return;
    }
    
    sec_ctx = get_security_context(interp);
    
    if (!sec_ctx || !sec_ctx->audit_enabled || !sec_ctx->audit_log) {
        return;
    }
    
    get_timestamp(timestamp, sizeof(timestamp));
    
    fprintf(sec_ctx->audit_log, "[%s] UNSAFE_OP type=%s operation=%s", 
           timestamp, unsafe_operation_type_name(op_type), operation);
    
    if (resource) {
        fprintf(sec_ctx->audit_log, " resource=%s", resource);
    }
    
    fprintf(sec_ctx->audit_log, " in_unsafe=%d depth=%d\n",
           unsafe_context_is_unsafe(sec_ctx->unsafe_ctx),
           unsafe_context_depth(sec_ctx->unsafe_ctx));
    
    fflush(sec_ctx->audit_log);
}

/**
 * Register a capability check in the audit log
 */
void security_log_capability_check(Interpreter *interp,
                                   int cap_type,
                                   const char *resource,
                                   int granted) {
    SecurityContext *sec_ctx;
    char timestamp[32];
    
    if (!interp) {
        return;
    }
    
    sec_ctx = get_security_context(interp);
    
    if (!sec_ctx) {
        return;
    }
    
    /* Update statistics */
    sec_ctx->cap_checks_count++;
    if (!granted) {
        sec_ctx->cap_denials_count++;
    }
    
    /* Log to audit if enabled */
    if (!sec_ctx->audit_enabled || !sec_ctx->audit_log) {
        return;
    }
    
    get_timestamp(timestamp, sizeof(timestamp));
    
    fprintf(sec_ctx->audit_log, "[%s] CAP_CHECK capability=%s resource=%s result=%s\n",
           timestamp, 
           capability_type_name((CapabilityType)cap_type),
           resource ? resource : "(any)",
           granted ? "GRANTED" : "DENIED");
    
    fflush(sec_ctx->audit_log);
}

/* ===================================================================
 * Security Utilities
 * =================================================================== */

/**
 * Get string name for security level
 */
const char *security_level_name(SecurityLevel level) {
    switch (level) {
        case SECURITY_LEVEL_PERMISSIVE: return "permissive";
        case SECURITY_LEVEL_WARN:       return "warn";
        case SECURITY_LEVEL_STRICT:     return "strict";
        case SECURITY_LEVEL_PARANOID:   return "paranoid";
        default:                        return "unknown";
    }
}

/**
 * Parse security level from string
 */
SecurityLevel security_level_from_name(const char *name) {
    if (!name) {
        return SECURITY_LEVEL_PERMISSIVE;
    }
    
    if (strcmp(name, "permissive") == 0) {
        return SECURITY_LEVEL_PERMISSIVE;
    }
    if (strcmp(name, "warn") == 0) {
        return SECURITY_LEVEL_WARN;
    }
    if (strcmp(name, "strict") == 0) {
        return SECURITY_LEVEL_STRICT;
    }
    if (strcmp(name, "paranoid") == 0) {
        return SECURITY_LEVEL_PARANOID;
    }
    
    return SECURITY_LEVEL_PERMISSIVE;
}

/**
 * Get string name for unsafe operation type
 */
const char *unsafe_operation_type_name(UnsafeOperationType op_type) {
    switch (op_type) {
        case UNSAFE_OP_FILE_IO:    return "file-io";
        case UNSAFE_OP_NETWORK:    return "network";
        case UNSAFE_OP_ENV:        return "env";
        case UNSAFE_OP_PROCESS:    return "process";
        case UNSAFE_OP_MEMORY:     return "memory";
        case UNSAFE_OP_FFI:        return "ffi";
        case UNSAFE_OP_POINTER:    return "pointer";
        case UNSAFE_OP_EVAL:       return "eval";
        case UNSAFE_OP_REFLECTION: return "reflection";
        default:                   return "unknown";
    }
}

/**
 * Check if an operation name is considered unsafe
 * 
 * Returns the UnsafeOperationType if unsafe, -1 if safe.
 */
int is_unsafe_operation(const char *operation) {
    if (!operation) {
        return -1;
    }
    
    /* File I/O operations */
    if (strstr(operation, "File") || strstr(operation, "file") ||
        strcmp(operation, "readFile") == 0 ||
        strcmp(operation, "writeFile") == 0 ||
        strcmp(operation, "deleteFile") == 0) {
        return UNSAFE_OP_FILE_IO;
    }
    
    /* Network operations */
    if (strstr(operation, "http") || strstr(operation, "HTTP") ||
        strstr(operation, "net") || strstr(operation, "socket") ||
        strcmp(operation, "fetch") == 0) {
        return UNSAFE_OP_NETWORK;
    }
    
    /* Environment operations */
    if (strstr(operation, "env") || strstr(operation, "ENV") ||
        strcmp(operation, "getenv") == 0 ||
        strcmp(operation, "setenv") == 0) {
        return UNSAFE_OP_ENV;
    }
    
    /* Process operations */
    if (strstr(operation, "exec") || strstr(operation, "spawn") ||
        strcmp(operation, "system") == 0 ||
        strcmp(operation, "run") == 0) {
        return UNSAFE_OP_PROCESS;
    }
    
    /* Memory operations */
    if (strcmp(operation, "malloc") == 0 ||
        strcmp(operation, "calloc") == 0 ||
        strcmp(operation, "realloc") == 0 ||
        strcmp(operation, "free") == 0) {
        return UNSAFE_OP_MEMORY;
    }
    
    /* FFI operations */
    if (strstr(operation, "ffi") || strstr(operation, "FFI") ||
        strcmp(operation, "dlopen") == 0 ||
        strcmp(operation, "dlsym") == 0) {
        return UNSAFE_OP_FFI;
    }
    
    /* Pointer operations */
    if (strstr(operation, "pointer") || strstr(operation, "Pointer") ||
        strcmp(operation, "addressOf") == 0 ||
        strcmp(operation, "deref") == 0) {
        return UNSAFE_OP_POINTER;
    }
    
    /* Eval operations */
    if (strcmp(operation, "eval") == 0 ||
        strcmp(operation, "compile") == 0) {
        return UNSAFE_OP_EVAL;
    }
    
    /* Reflection operations */
    if (strstr(operation, "reflect") || strstr(operation, "Reflect")) {
        return UNSAFE_OP_REFLECTION;
    }
    
    /* Not recognized as unsafe */
    return -1;
}

/* ===================================================================
 * Backwards Compatibility Support (Legacy SafetyContext)
 * =================================================================== */

/**
 * Create a new legacy safety context
 */
SafetyContext *safety_context_new(void) {
    SafetyContext *ctx = (SafetyContext *)calloc(1, sizeof(SafetyContext));
    if (!ctx) {
        return NULL;
    }
    
    ctx->in_unsafe_context = 0;
    ctx->unsafe_depth = 0;
    ctx->strict_mode = 0;
    ctx->warn_only = 0;
    
    return ctx;
}

/**
 * Free a legacy safety context
 */
void safety_context_free(SafetyContext *ctx) {
    if (ctx) {
        free(ctx);
    }
}

/**
 * Enter unsafe block (legacy interface)
 */
void safety_context_enter_unsafe(SafetyContext *ctx) {
    if (!ctx) {
        return;
    }
    
    ctx->unsafe_depth++;
    ctx->in_unsafe_context = 1;
}

/**
 * Exit unsafe block (legacy interface)
 */
void safety_context_exit_unsafe(SafetyContext *ctx) {
    if (!ctx) {
        return;
    }
    
    if (ctx->unsafe_depth > 0) {
        ctx->unsafe_depth--;
    }
    
    if (ctx->unsafe_depth == 0) {
        ctx->in_unsafe_context = 0;
    }
}

/**
 * Check if in unsafe context (legacy interface)
 */
int safety_context_is_unsafe(SafetyContext *ctx) {
    if (!ctx) {
        return 0;
    }
    
    return ctx->in_unsafe_context;
}

/**
 * Check unsafe context (legacy interface)
 * 
 * Maps to the new check_unsafe_operation function.
 */
void check_unsafe_context(Interpreter *interp, const char *operation) {
    check_unsafe_operation(interp, operation);
}

/**
 * Require unsafe context (legacy interface)
 * 
 * Maps to the new require_unsafe_operation function.
 */
void require_unsafe_context(Interpreter *interp, const char *operation) {
    require_unsafe_operation(interp, operation);
}
