#ifndef SECURITY_H
#define SECURITY_H

#include <stddef.h>
#include <stdio.h>

/* Forward declarations */
typedef struct Interpreter Interpreter;
typedef struct Env Env;
typedef struct Capability Capability;
typedef struct CapabilitySet CapabilitySet;
typedef struct CapabilityContext CapabilityContext;

/* ===================================================================
 * Security Level Enumeration
 * =================================================================== */

/**
 * Security Level
 * 
 * Defines the security enforcement level for the interpreter.
 * Controls how strictly security policies are enforced.
 */
typedef enum {
    SECURITY_LEVEL_PERMISSIVE,     /* Permissive mode - no enforcement (default for v1.x) */
    SECURITY_LEVEL_WARN,           /* Warning mode - warn about violations but allow */
    SECURITY_LEVEL_STRICT,         /* Strict mode - enforce all security policies */
    SECURITY_LEVEL_PARANOID        /* Paranoid mode - strictest enforcement with extra checks */
} SecurityLevel;

/* ===================================================================
 * Unsafe Context Tracking
 * =================================================================== */

/**
 * Unsafe Context
 * 
 * Tracks whether we are currently executing within an unsafe block.
 * Supports nested unsafe blocks and maintains the depth of nesting.
 * 
 * This is the core structure for implementing security-by-default:
 * operations that are potentially dangerous require an unsafe context.
 */
typedef struct UnsafeContext {
    int in_unsafe_block;           /* Are we currently in an unsafe block? */
    int unsafe_depth;              /* Nesting depth of unsafe blocks */
    int line_number;               /* Source line where unsafe block started */
    const char *file_name;         /* Source file containing unsafe block */
} UnsafeContext;

/**
 * Create a new unsafe context
 * 
 * Returns: Newly allocated UnsafeContext or NULL on allocation failure
 */
UnsafeContext *unsafe_context_new(void);

/**
 * Free an unsafe context
 * 
 * Params:
 *   ctx: The unsafe context to free (can be NULL)
 */
void unsafe_context_free(UnsafeContext *ctx);

/**
 * Enter an unsafe block
 * 
 * Increments the unsafe depth and sets in_unsafe_block flag.
 * 
 * Params:
 *   ctx: The unsafe context to modify
 *   line: Source line number where unsafe block starts
 *   file: Source file name (can be NULL)
 */
void unsafe_context_enter(UnsafeContext *ctx, int line, const char *file);

/**
 * Exit an unsafe block
 * 
 * Decrements the unsafe depth. When depth reaches 0, clears in_unsafe_block flag.
 * 
 * Params:
 *   ctx: The unsafe context to modify
 */
void unsafe_context_exit(UnsafeContext *ctx);

/**
 * Check if currently in an unsafe context
 * 
 * Params:
 *   ctx: The unsafe context to check
 * 
 * Returns: 1 if in unsafe block, 0 otherwise
 */
int unsafe_context_is_unsafe(UnsafeContext *ctx);

/**
 * Get current unsafe depth
 * 
 * Params:
 *   ctx: The unsafe context to check
 * 
 * Returns: Current nesting depth of unsafe blocks
 */
int unsafe_context_depth(UnsafeContext *ctx);

/* ===================================================================
 * Security Context
 * =================================================================== */

/**
 * Security Context
 * 
 * Central security context for an interpreter instance.
 * Integrates unsafe block tracking, capability management, and security configuration.
 * 
 * This structure is embedded in the Interpreter and provides all security
 * enforcement mechanisms in one place.
 */
typedef struct SecurityContext {
    /* Unsafe block tracking */
    UnsafeContext *unsafe_ctx;
    
    /* Capability management */
    CapabilityContext *capability_ctx;
    
    /* Security configuration */
    SecurityLevel security_level;      /* Current security enforcement level */
    int strict_mode;                   /* Shorthand for SECURITY_LEVEL_STRICT */
    int warn_only;                     /* Shorthand for SECURITY_LEVEL_WARN */
    
    /* Audit and logging */
    int audit_enabled;                 /* Enable security audit logging */
    FILE *audit_log;                   /* Audit log file handle */
    
    /* Statistics */
    unsigned long unsafe_ops_count;    /* Count of unsafe operations executed */
    unsigned long cap_checks_count;    /* Count of capability checks performed */
    unsigned long cap_denials_count;   /* Count of capability denials */
    
    /* Error handling */
    int allow_permissive_fallback;     /* Fall back to permissive mode on errors */
} SecurityContext;

/**
 * Create a new security context
 * 
 * Initializes with default settings (permissive mode for backwards compatibility).
 * 
 * Returns: Newly allocated SecurityContext or NULL on allocation failure
 */
SecurityContext *security_context_new(void);

/**
 * Free a security context and all its resources
 * 
 * Params:
 *   ctx: The security context to free (can be NULL)
 */
void security_context_free(SecurityContext *ctx);

/**
 * Set the security level
 * 
 * Params:
 *   ctx: The security context to modify
 *   level: The security level to set
 */
void security_context_set_level(SecurityContext *ctx, SecurityLevel level);

/**
 * Get the current security level
 * 
 * Params:
 *   ctx: The security context to query
 * 
 * Returns: Current security level
 */
SecurityLevel security_context_get_level(SecurityContext *ctx);

/**
 * Enable strict mode (shorthand for SECURITY_LEVEL_STRICT)
 * 
 * Params:
 *   ctx: The security context to modify
 */
void security_context_enable_strict_mode(SecurityContext *ctx);

/**
 * Enable warn-only mode (shorthand for SECURITY_LEVEL_WARN)
 * 
 * Params:
 *   ctx: The security context to modify
 */
void security_context_enable_warn_mode(SecurityContext *ctx);

/**
 * Enable permissive mode (shorthand for SECURITY_LEVEL_PERMISSIVE)
 * 
 * Params:
 *   ctx: The security context to modify
 */
void security_context_enable_permissive_mode(SecurityContext *ctx);

/**
 * Enable audit logging
 * 
 * Params:
 *   ctx: The security context to modify
 *   log_file: Path to audit log file (NULL for stderr)
 */
void security_context_enable_audit(SecurityContext *ctx, const char *log_file);

/**
 * Disable audit logging
 * 
 * Params:
 *   ctx: The security context to modify
 */
void security_context_disable_audit(SecurityContext *ctx);

/**
 * Get security statistics
 * 
 * Params:
 *   ctx: The security context to query
 *   unsafe_ops: Output parameter for unsafe operations count (can be NULL)
 *   cap_checks: Output parameter for capability checks count (can be NULL)
 *   cap_denials: Output parameter for capability denials count (can be NULL)
 */
void security_context_get_stats(SecurityContext *ctx, 
                                unsigned long *unsafe_ops,
                                unsigned long *cap_checks,
                                unsigned long *cap_denials);

/**
 * Reset security statistics
 * 
 * Params:
 *   ctx: The security context to modify
 */
void security_context_reset_stats(SecurityContext *ctx);

/* ===================================================================
 * Unsafe Operation Checking
 * =================================================================== */

/**
 * Check if an operation requires unsafe context
 * 
 * This is the primary enforcement function for unsafe operations.
 * Behavior depends on security level:
 *   - PERMISSIVE: Always allows, no warnings
 *   - WARN: Allows but prints warning if not in unsafe block
 *   - STRICT: Requires unsafe block, errors if not present
 *   - PARANOID: Like strict, plus additional validation
 * 
 * Params:
 *   interp: The interpreter instance
 *   operation: Name of the operation (for error messages)
 */
void check_unsafe_operation(Interpreter *interp, const char *operation);

/**
 * Require unsafe context for an operation
 * 
 * Stricter version that always enforces unsafe context requirement
 * regardless of security level (except PERMISSIVE mode).
 * 
 * Params:
 *   interp: The interpreter instance
 *   operation: Name of the operation (for error messages)
 */
void require_unsafe_operation(Interpreter *interp, const char *operation);

/**
 * Check if currently in unsafe context
 * 
 * Params:
 *   interp: The interpreter instance
 * 
 * Returns: 1 if in unsafe block, 0 otherwise
 */
int is_in_unsafe_context(Interpreter *interp);

/* ===================================================================
 * Integration Points for Interpreter
 * =================================================================== */

/**
 * Initialize security for an interpreter
 * 
 * Creates and attaches a SecurityContext to the interpreter.
 * Should be called during interpreter initialization.
 * 
 * Params:
 *   interp: The interpreter to initialize
 * 
 * Returns: 0 on success, -1 on failure
 */
int security_init(Interpreter *interp);

/**
 * Shutdown security for an interpreter
 * 
 * Frees the SecurityContext and all associated resources.
 * Should be called during interpreter cleanup.
 * 
 * Params:
 *   interp: The interpreter to clean up
 */
void security_shutdown(Interpreter *interp);

/**
 * Configure security from command-line flags
 * 
 * Parses and applies security configuration from CLI arguments.
 * 
 * Params:
 *   interp: The interpreter to configure
 *   strict: Enable strict mode (--strict)
 *   warn: Enable warn-only mode (--warn-unsafe)
 *   unsafe_all: Disable all security checks (--unsafe-all)
 *   audit_file: Path to audit log file (--audit-log=<file>)
 */
void security_configure_from_cli(Interpreter *interp,
                                 int strict,
                                 int warn,
                                 int unsafe_all,
                                 const char *audit_file);

/* ===================================================================
 * Unsafe Operation Registry
 * =================================================================== */

/**
 * Unsafe Operation Type
 * 
 * Categorizes unsafe operations for finer-grained control and reporting.
 */
typedef enum {
    UNSAFE_OP_FILE_IO,            /* File I/O operations */
    UNSAFE_OP_NETWORK,            /* Network operations */
    UNSAFE_OP_ENV,                /* Environment variable access */
    UNSAFE_OP_PROCESS,            /* Process spawning/control */
    UNSAFE_OP_MEMORY,             /* Manual memory management */
    UNSAFE_OP_FFI,                /* Foreign function calls */
    UNSAFE_OP_POINTER,            /* Raw pointer operations */
    UNSAFE_OP_EVAL,               /* Dynamic code evaluation */
    UNSAFE_OP_REFLECTION          /* Reflection/introspection */
} UnsafeOperationType;

/**
 * Register an unsafe operation in the audit log
 * 
 * Records that an unsafe operation was performed. Used for audit trails.
 * 
 * Params:
 *   interp: The interpreter instance
 *   op_type: The type of unsafe operation
 *   operation: Specific operation name (e.g., "readFile", "http_get")
 *   resource: Resource being accessed (can be NULL)
 */
void security_log_unsafe_operation(Interpreter *interp,
                                   UnsafeOperationType op_type,
                                   const char *operation,
                                   const char *resource);

/**
 * Register a capability check in the audit log
 * 
 * Records a capability check (whether granted or denied).
 * 
 * Params:
 *   interp: The interpreter instance
 *   cap_type: The capability type being checked
 *   resource: Resource being accessed
 *   granted: Whether access was granted (1) or denied (0)
 */
void security_log_capability_check(Interpreter *interp,
                                   int cap_type,  /* CapabilityType */
                                   const char *resource,
                                   int granted);

/* ===================================================================
 * Security Utilities
 * =================================================================== */

/**
 * Get string name for security level
 * 
 * Params:
 *   level: The security level
 * 
 * Returns: String name (e.g., "permissive", "strict")
 */
const char *security_level_name(SecurityLevel level);

/**
 * Parse security level from string
 * 
 * Params:
 *   name: String name (e.g., "strict", "warn")
 * 
 * Returns: Security level or SECURITY_LEVEL_PERMISSIVE if not recognized
 */
SecurityLevel security_level_from_name(const char *name);

/**
 * Get string name for unsafe operation type
 * 
 * Params:
 *   op_type: The operation type
 * 
 * Returns: String name (e.g., "file-io", "network")
 */
const char *unsafe_operation_type_name(UnsafeOperationType op_type);

/**
 * Check if an operation name is considered unsafe
 * 
 * Params:
 *   operation: The operation name to check (e.g., "readFile")
 * 
 * Returns: UnsafeOperationType if unsafe, -1 if safe
 */
int is_unsafe_operation(const char *operation);

/* ===================================================================
 * Backwards Compatibility Support
 * =================================================================== */

/**
 * Legacy safety context (for backwards compatibility)
 * 
 * This is a simpler version of UnsafeContext for Phase 1 implementation.
 * Will be merged with UnsafeContext in later phases.
 */
typedef struct SafetyContext {
    int in_unsafe_context;
    int unsafe_depth;
    int strict_mode;
    int warn_only;
} SafetyContext;

/**
 * Legacy safety context functions
 * These are kept for backwards compatibility during the transition period.
 */
SafetyContext *safety_context_new(void);
void safety_context_free(SafetyContext *ctx);
void safety_context_enter_unsafe(SafetyContext *ctx);
void safety_context_exit_unsafe(SafetyContext *ctx);
int safety_context_is_unsafe(SafetyContext *ctx);

/**
 * Check unsafe context (legacy interface)
 */
void check_unsafe_context(Interpreter *interp, const char *operation);

/**
 * Require unsafe context (legacy interface)
 */
void require_unsafe_context(Interpreter *interp, const char *operation);

#endif /* SECURITY_H */
