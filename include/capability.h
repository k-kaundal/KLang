#ifndef CAPABILITY_H
#define CAPABILITY_H

#include <stddef.h>
#include <stdio.h>

/* Forward declarations */
typedef struct Interpreter Interpreter;

/**
 * Capability Types
 * 
 * Defines the 11 core capability types that control access to system resources.
 * Each capability represents a specific permission that must be granted before
 * an operation can proceed.
 */
typedef enum {
    CAP_NONE = 0,              /* No capabilities */
    CAP_FILE_READ,             /* Read files from filesystem */
    CAP_FILE_WRITE,            /* Write files to filesystem */
    CAP_FILE_DELETE,           /* Delete files from filesystem */
    CAP_NETWORK_CONNECT,       /* Make outbound network connections */
    CAP_NETWORK_LISTEN,        /* Listen on network ports */
    CAP_ENV_READ,              /* Read environment variables */
    CAP_ENV_WRITE,             /* Write/modify environment variables */
    CAP_PROCESS_SPAWN,         /* Spawn child processes */
    CAP_FFI_CALL,              /* Call foreign functions (native code) */
    CAP_UNSAFE_MEM,            /* Manual memory management (malloc, free, etc.) */
    CAP_ALL                    /* All capabilities (legacy/permissive mode) */
} CapabilityType;

/**
 * Capability Structure
 * 
 * Represents a single granted permission with optional resource pattern matching.
 * 
 * Fields:
 *   type: The type of capability (e.g., CAP_FILE_READ)
 *   resource_pattern: Glob pattern for resource matching (e.g., "/tmp/ *", "*.json")
 *                     NULL or "*" means match all resources
 *   revocable: Whether this capability can be revoked at runtime
 *   metadata: Additional capability-specific data (implementation-defined)
 */
typedef struct Capability {
    CapabilityType type;
    char *resource_pattern;
    int revocable;
    void *metadata;
} Capability;

/**
 * Capability Set
 * 
 * Dynamic array of capabilities granted to a security context.
 * Supports efficient lookup and pattern matching for resource access checks.
 */
typedef struct CapabilitySet {
    Capability *capabilities;
    int count;
    int capacity;
} CapabilitySet;

/**
 * Capability Context
 * 
 * Runtime context for managing capabilities within an interpreter instance.
 * Tracks granted capabilities, requested (but denied) capabilities, and
 * provides audit logging support.
 * 
 * Fields:
 *   granted: Set of capabilities that have been granted
 *   requested: Set of capabilities that were requested but not granted
 *   prompt_on_missing: If true, prompt user when a capability is missing (interactive mode)
 *   audit_enabled: If true, log all capability checks to audit_log
 *   audit_log: File handle for audit logging (NULL if disabled)
 */
typedef struct CapabilityContext {
    CapabilitySet *granted;
    CapabilitySet *requested;
    int prompt_on_missing;
    int audit_enabled;
    FILE *audit_log;
} CapabilityContext;

/* ===================================================================
 * CapabilitySet Operations
 * =================================================================== */

/**
 * Create a new, empty capability set
 * 
 * Returns: Newly allocated CapabilitySet or NULL on allocation failure
 */
CapabilitySet *capability_set_new(void);

/**
 * Free a capability set and all its resources
 * 
 * Params:
 *   set: The capability set to free (can be NULL)
 */
void capability_set_free(CapabilitySet *set);

/**
 * Add a capability to the set
 * 
 * Params:
 *   set: The capability set to modify
 *   type: The type of capability to add
 *   pattern: Resource pattern for this capability (NULL or "*" for all resources)
 */
void capability_set_add(CapabilitySet *set, CapabilityType type, const char *pattern);

/**
 * Remove a capability from the set by index
 * 
 * Params:
 *   set: The capability set to modify
 *   index: Index of the capability to remove
 */
void capability_set_remove(CapabilitySet *set, int index);

/**
 * Find a capability in the set that matches the given type and resource
 * 
 * Performs pattern matching to determine if any capability in the set
 * grants access to the specified resource.
 * 
 * Params:
 *   set: The capability set to search
 *   type: The type of capability to find
 *   resource: The specific resource path to match (e.g., "/tmp/file.txt")
 * 
 * Returns: Pointer to matching Capability or NULL if not found
 */
Capability *capability_set_find(CapabilitySet *set, CapabilityType type, const char *resource);

/* ===================================================================
 * CapabilityContext Operations
 * =================================================================== */

/**
 * Create a new capability context with no granted capabilities
 * 
 * Returns: Newly allocated CapabilityContext or NULL on allocation failure
 */
CapabilityContext *capability_context_new(void);

/**
 * Free a capability context and all its resources
 * 
 * Params:
 *   ctx: The capability context to free (can be NULL)
 */
void capability_context_free(CapabilityContext *ctx);

/**
 * Grant a capability to the context
 * 
 * Params:
 *   ctx: The capability context to modify
 *   type: The type of capability to grant
 *   pattern: Resource pattern for this capability (NULL or "*" for all resources)
 */
void capability_grant(CapabilityContext *ctx, CapabilityType type, const char *pattern);

/**
 * Check if the context has a capability for a specific resource
 * 
 * Params:
 *   ctx: The capability context to check
 *   type: The type of capability required
 *   resource: The specific resource to check (can be NULL for wildcard check)
 * 
 * Returns: 1 if capability is granted, 0 otherwise
 */
int capability_has(CapabilityContext *ctx, CapabilityType type, const char *resource);

/**
 * Require a capability for a resource, throwing an error if not granted
 * 
 * This is the primary enforcement function. It checks if the interpreter's
 * capability context has the required permission. If not, it logs the denied
 * request (if auditing is enabled) and reports an error.
 * 
 * Params:
 *   interp: The interpreter instance (must have capability context)
 *   type: The type of capability required
 *   resource: The specific resource being accessed (e.g., file path, URL)
 * 
 * Side Effects:
 *   - Sets interp->had_error = 1 if capability is denied
 *   - Logs to audit log if enabled
 *   - May prompt user if prompt_on_missing is enabled
 */
void capability_require(Interpreter *interp, CapabilityType type, const char *resource);

/* ===================================================================
 * Resource Pattern Matching
 * =================================================================== */

/**
 * Match a resource against a glob-style pattern
 * 
 * Supports standard glob patterns:
 *   - "*" matches any sequence of characters (excluding path separator)
 *   - "**" matches any sequence including path separators (recursive)
 *   - "?" matches any single character
 *   - "*.json" matches all JSON files
 *   - "/tmp/ *" matches all files in /tmp (non-recursive)
 *   - "/tmp/ **" matches all files in /tmp (recursive)
 * 
 * Params:
 *   resource: The resource path to test (e.g., "/tmp/file.txt")
 *   pattern: The glob pattern to match against (e.g., "/tmp/ *")
 * 
 * Returns: 1 if resource matches pattern, 0 otherwise
 */
int resource_matches_pattern(const char *resource, const char *pattern);

/* ===================================================================
 * Capability Type Utilities
 * =================================================================== */

/**
 * Get human-readable name for a capability type
 * 
 * Params:
 *   type: The capability type
 * 
 * Returns: String name (e.g., "file-read", "network-connect")
 */
const char *capability_type_name(CapabilityType type);

/**
 * Get CLI flag name for a capability type
 * 
 * Params:
 *   type: The capability type
 * 
 * Returns: Flag name (e.g., "--allow-read", "--allow-net")
 */
const char *capability_flag_name(CapabilityType type);

/**
 * Parse a capability type from a string name
 * 
 * Params:
 *   name: String name (e.g., "file-read", "net")
 * 
 * Returns: Capability type or CAP_NONE if not recognized
 */
CapabilityType capability_type_from_name(const char *name);

/* ===================================================================
 * Capability Manager (for runtime tracking)
 * =================================================================== */

/**
 * Capability Manager
 * 
 * Global manager for tracking all capability contexts and providing
 * centralized capability management. This allows for system-wide
 * capability policies and auditing.
 */
typedef struct CapabilityManager {
    CapabilityContext **contexts;      /* Array of all contexts */
    int context_count;
    int context_capacity;
    
    int global_audit_enabled;          /* Enable auditing for all contexts */
    FILE *global_audit_log;            /* Global audit log */
    
    int strict_mode_default;           /* Default strict mode for new contexts */
} CapabilityManager;

/**
 * Get the global capability manager instance (singleton)
 * 
 * Returns: Pointer to global CapabilityManager
 */
CapabilityManager *capability_manager_get(void);

/**
 * Initialize the global capability manager
 * 
 * Should be called once at program startup.
 */
void capability_manager_init(void);

/**
 * Shutdown the global capability manager
 * 
 * Frees all resources. Should be called once at program shutdown.
 */
void capability_manager_shutdown(void);

/**
 * Register a capability context with the manager
 * 
 * Params:
 *   ctx: The context to register
 */
void capability_manager_register(CapabilityContext *ctx);

/**
 * Unregister a capability context from the manager
 * 
 * Params:
 *   ctx: The context to unregister
 */
void capability_manager_unregister(CapabilityContext *ctx);

/**
 * Enable global audit logging
 * 
 * Params:
 *   log_file: File path for audit log (NULL for stderr)
 */
void capability_manager_enable_audit(const char *log_file);

/**
 * Disable global audit logging
 */
void capability_manager_disable_audit(void);

#endif /* CAPABILITY_H */
