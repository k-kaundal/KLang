/**
 * capability.c - Capability System Implementation
 * 
 * Implements the capability-based security system for KLang.
 * Provides fine-grained permission control with resource pattern matching.
 * 
 * Architecture:
 *   - CapabilitySet: Dynamic array of capabilities
 *   - CapabilityContext: Runtime capability tracking per interpreter
 *   - CapabilityManager: Global singleton for system-wide management
 *   - Pattern Matching: Glob-style pattern matching for resources
 * 
 * Pattern Syntax:
 *   - "*" matches any sequence (excluding path separator)
 *   - "**" matches any sequence including path separators (recursive)
 *   - "?" matches any single character
 *   - Examples: "/tmp/star", "star.json", "/app/starstar/star.kl"
 */

/* Need strdup from POSIX */
#define _POSIX_C_SOURCE 200809L

#include "capability.h"
#include "security.h"
#include "../../interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Initial capacity for dynamic arrays */
#define INITIAL_CAPACITY 8

/* Global capability manager singleton */
static CapabilityManager *global_manager = NULL;

/* ===================================================================
 * Pattern Matching Implementation
 * =================================================================== */

/**
 * Match a resource against a glob-style pattern
 * 
 * Implements efficient pattern matching with support for:
 *   - "*" wildcard (non-recursive)
 *   - "**" wildcard (recursive, matches path separators)
 *   - "?" single character wildcard
 * 
 * Algorithm: Recursive descent with backtracking
 * Complexity: O(n*m) worst case, O(n) average case
 */
int resource_matches_pattern(const char *resource, const char *pattern) {
    /* NULL or empty pattern matches everything */
    if (!pattern || !*pattern || strcmp(pattern, "*") == 0) {
        return 1;
    }
    
    /* NULL resource doesn't match any specific pattern */
    if (!resource) {
        return 0;
    }
    
    /* Exact match check (optimization) */
    if (strcmp(resource, pattern) == 0) {
        return 1;
    }
    
    /* Pattern matching with wildcards */
    const char *r = resource;
    const char *p = pattern;
    const char *star = NULL;  /* Last '*' position in pattern */
    const char *match = NULL; /* Corresponding position in resource */
    
    while (*r) {
        /* Check for recursive wildcard "**" */
        if (*p == '*' && *(p + 1) == '*') {
            /* Skip the ** */
            p += 2;
            
            /* Skip any following slashes */
            while (*p == '/' || *p == '\\') {
                p++;
            }
            
            /* If pattern ends with **, match everything */
            if (!*p) {
                return 1;
            }
            
            /* Try matching the rest of the pattern at every position */
            while (*r) {
                if (resource_matches_pattern(r, p)) {
                    return 1;
                }
                r++;
            }
            return 0;
        }
        
        /* Single character wildcard */
        if (*p == '?') {
            p++;
            r++;
            continue;
        }
        
        /* Non-recursive wildcard (doesn't match path separators) */
        if (*p == '*') {
            /* Remember wildcard position */
            star = p++;
            match = r;
            continue;
        }
        
        /* Exact character match */
        if (*p == *r) {
            p++;
            r++;
            continue;
        }
        
        /* No match - backtrack if we have a star */
        if (star) {
            /* Don't let * match path separators */
            if (*match == '/' || *match == '\\') {
                return 0;
            }
            p = star + 1;
            r = ++match;
            continue;
        }
        
        /* No match and no star - fail */
        return 0;
    }
    
    /* Consume trailing stars in pattern */
    while (*p == '*') {
        p++;
    }
    
    /* Success if both pattern and resource are exhausted */
    return !*p;
}

/* ===================================================================
 * CapabilitySet Operations
 * =================================================================== */

/**
 * Create a new, empty capability set
 */
CapabilitySet *capability_set_new(void) {
    CapabilitySet *set = (CapabilitySet *)calloc(1, sizeof(CapabilitySet));
    if (!set) {
        return NULL;
    }
    
    set->capabilities = (Capability *)malloc(INITIAL_CAPACITY * sizeof(Capability));
    if (!set->capabilities) {
        free(set);
        return NULL;
    }
    
    set->count = 0;
    set->capacity = INITIAL_CAPACITY;
    
    return set;
}

/**
 * Free a capability set and all its resources
 */
void capability_set_free(CapabilitySet *set) {
    int i;
    if (!set) {
        return;
    }
    
    /* Free each capability's resources */
    for (i = 0; i < set->count; i++) {
        if (set->capabilities[i].resource_pattern) {
            free(set->capabilities[i].resource_pattern);
        }
        /* Note: metadata is owned by the caller, we don't free it */
    }
    
    /* Free the array and set structure */
    if (set->capabilities) {
        free(set->capabilities);
    }
    free(set);
}

/**
 * Add a capability to the set
 * 
 * Dynamically grows the set if needed.
 * Makes a copy of the pattern string.
 */
void capability_set_add(CapabilitySet *set, CapabilityType type, const char *pattern) {
    Capability *new_caps;
    int new_capacity;
    
    if (!set) {
        return;
    }
    
    /* Grow array if needed */
    if (set->count >= set->capacity) {
        new_capacity = set->capacity * 2;
        new_caps = (Capability *)realloc(set->capabilities, 
                                         new_capacity * sizeof(Capability));
        if (!new_caps) {
            fprintf(stderr, "Error: Failed to grow capability set\n");
            return;
        }
        set->capabilities = new_caps;
        set->capacity = new_capacity;
    }
    
    /* Initialize new capability */
    set->capabilities[set->count].type = type;
    set->capabilities[set->count].resource_pattern = 
        (pattern && strcmp(pattern, "*") != 0) ? strdup(pattern) : NULL;
    set->capabilities[set->count].revocable = 1;  /* Default: revocable */
    set->capabilities[set->count].metadata = NULL;
    
    set->count++;
}

/**
 * Remove a capability from the set by index
 * 
 * Compacts the array after removal.
 */
void capability_set_remove(CapabilitySet *set, int index) {
    int i;
    if (!set || index < 0 || index >= set->count) {
        return;
    }
    
    /* Free the capability's resources */
    if (set->capabilities[index].resource_pattern) {
        free(set->capabilities[index].resource_pattern);
    }
    
    /* Shift remaining capabilities down */
    for (i = index; i < set->count - 1; i++) {
        set->capabilities[i] = set->capabilities[i + 1];
    }
    
    set->count--;
}

/**
 * Find a capability in the set that matches the given type and resource
 * 
 * Performs pattern matching to determine if any capability grants access.
 * Returns the first matching capability found.
 */
Capability *capability_set_find(CapabilitySet *set, CapabilityType type, 
                                const char *resource) {
    int i;
    if (!set) {
        return NULL;
    }
    
    /* Check for CAP_ALL which grants everything */
    for (i = 0; i < set->count; i++) {
        if (set->capabilities[i].type == CAP_ALL) {
            return &set->capabilities[i];
        }
    }
    
    /* Search for matching capability */
    for (i = 0; i < set->count; i++) {
        if (set->capabilities[i].type == type) {
            /* No pattern means match all resources */
            if (!set->capabilities[i].resource_pattern) {
                return &set->capabilities[i];
            }
            
            /* Match against pattern */
            if (resource && resource_matches_pattern(resource, 
                                                    set->capabilities[i].resource_pattern)) {
                return &set->capabilities[i];
            }
        }
    }
    
    return NULL;
}

/* ===================================================================
 * CapabilityContext Operations
 * =================================================================== */

/**
 * Create a new capability context with no granted capabilities
 */
CapabilityContext *capability_context_new(void) {
    CapabilityContext *ctx = (CapabilityContext *)calloc(1, sizeof(CapabilityContext));
    if (!ctx) {
        return NULL;
    }
    
    ctx->granted = capability_set_new();
    ctx->requested = capability_set_new();
    
    if (!ctx->granted || !ctx->requested) {
        capability_set_free(ctx->granted);
        capability_set_free(ctx->requested);
        free(ctx);
        return NULL;
    }
    
    ctx->prompt_on_missing = 0;
    ctx->audit_enabled = 0;
    ctx->audit_log = NULL;
    
    return ctx;
}

/**
 * Free a capability context and all its resources
 */
void capability_context_free(CapabilityContext *ctx) {
    if (!ctx) {
        return;
    }
    
    capability_set_free(ctx->granted);
    capability_set_free(ctx->requested);
    
    if (ctx->audit_log && ctx->audit_log != stderr && ctx->audit_log != stdout) {
        fclose(ctx->audit_log);
    }
    
    free(ctx);
}

/**
 * Grant a capability to the context
 */
void capability_grant(CapabilityContext *ctx, CapabilityType type, const char *pattern) {
    if (!ctx || !ctx->granted) {
        return;
    }
    
    capability_set_add(ctx->granted, type, pattern);
}

/**
 * Check if the context has a capability for a specific resource
 * 
 * Returns 1 if capability is granted, 0 otherwise.
 */
int capability_has(CapabilityContext *ctx, CapabilityType type, const char *resource) {
    if (!ctx || !ctx->granted) {
        return 0;
    }
    
    return capability_set_find(ctx->granted, type, resource) != NULL;
}

/**
 * Require a capability for a resource, throwing an error if not granted
 * 
 * This is the primary enforcement function.
 * Logs to audit log if enabled, sets interpreter error state if denied.
 */
void capability_require(Interpreter *interp, CapabilityType type, const char *resource) {
    CapabilityContext *ctx;
    const char *type_name;
    
    if (!interp) {
        return;
    }
    
    /* Get capability context from interpreter's security context */
    /* Note: In Phase 2, this will be: interp->security_ctx->capability_ctx */
    /* For now, we just validate the parameters and prepare for integration */
    ctx = NULL; /* Will be: interp->security_ctx->capability_ctx; */
    
    /* If no security context yet, allow (permissive mode for v1.x) */
    if (!ctx) {
        return;
    }
    
    /* Check if capability is granted */
    if (capability_has(ctx, type, resource)) {
        /* Audit log success if enabled */
        if (ctx->audit_enabled && ctx->audit_log) {
            type_name = capability_type_name(type);
            fprintf(ctx->audit_log, "[GRANTED] %s: %s\n", 
                   type_name, resource ? resource : "(any)");
            fflush(ctx->audit_log);
        }
        return;
    }
    
    /* Capability denied - record the request */
    if (ctx->requested) {
        capability_set_add(ctx->requested, type, resource);
    }
    
    /* Audit log denial */
    if (ctx->audit_enabled && ctx->audit_log) {
        type_name = capability_type_name(type);
        fprintf(ctx->audit_log, "[DENIED] %s: %s\n", 
               type_name, resource ? resource : "(any)");
        fflush(ctx->audit_log);
    }
    
    /* Set error state */
    type_name = capability_type_name(type);
    fprintf(stderr, "Security Error: Missing capability '%s' for resource '%s'\n",
           type_name, resource ? resource : "(any)");
    fprintf(stderr, "Hint: Add --%s", capability_flag_name(type));
    if (resource) {
        fprintf(stderr, "=%s", resource);
    }
    fprintf(stderr, " to grant this permission\n");
    
    interp->had_error = 1;
}

/* ===================================================================
 * Capability Type Utilities
 * =================================================================== */

/**
 * Get human-readable name for a capability type
 */
const char *capability_type_name(CapabilityType type) {
    switch (type) {
        case CAP_NONE:            return "none";
        case CAP_FILE_READ:       return "file-read";
        case CAP_FILE_WRITE:      return "file-write";
        case CAP_FILE_DELETE:     return "file-delete";
        case CAP_NETWORK_CONNECT: return "network-connect";
        case CAP_NETWORK_LISTEN:  return "network-listen";
        case CAP_ENV_READ:        return "env-read";
        case CAP_ENV_WRITE:       return "env-write";
        case CAP_PROCESS_SPAWN:   return "process-spawn";
        case CAP_FFI_CALL:        return "ffi-call";
        case CAP_UNSAFE_MEM:      return "unsafe-mem";
        case CAP_ALL:             return "all";
        default:                  return "unknown";
    }
}

/**
 * Get CLI flag name for a capability type
 */
const char *capability_flag_name(CapabilityType type) {
    switch (type) {
        case CAP_NONE:            return "--allow-none";
        case CAP_FILE_READ:       return "--allow-read";
        case CAP_FILE_WRITE:      return "--allow-write";
        case CAP_FILE_DELETE:     return "--allow-delete";
        case CAP_NETWORK_CONNECT: return "--allow-net";
        case CAP_NETWORK_LISTEN:  return "--allow-listen";
        case CAP_ENV_READ:        return "--allow-env-read";
        case CAP_ENV_WRITE:       return "--allow-env-write";
        case CAP_PROCESS_SPAWN:   return "--allow-run";
        case CAP_FFI_CALL:        return "--allow-ffi";
        case CAP_UNSAFE_MEM:      return "--allow-unsafe-mem";
        case CAP_ALL:             return "--allow-all";
        default:                  return "--allow-unknown";
    }
}

/**
 * Parse a capability type from a string name
 */
CapabilityType capability_type_from_name(const char *name) {
    if (!name) {
        return CAP_NONE;
    }
    
    /* Normalize common variations */
    if (strcmp(name, "file-read") == 0 || strcmp(name, "read") == 0) {
        return CAP_FILE_READ;
    }
    if (strcmp(name, "file-write") == 0 || strcmp(name, "write") == 0) {
        return CAP_FILE_WRITE;
    }
    if (strcmp(name, "file-delete") == 0 || strcmp(name, "delete") == 0) {
        return CAP_FILE_DELETE;
    }
    if (strcmp(name, "network-connect") == 0 || strcmp(name, "net") == 0) {
        return CAP_NETWORK_CONNECT;
    }
    if (strcmp(name, "network-listen") == 0 || strcmp(name, "listen") == 0) {
        return CAP_NETWORK_LISTEN;
    }
    if (strcmp(name, "env-read") == 0 || strcmp(name, "env") == 0) {
        return CAP_ENV_READ;
    }
    if (strcmp(name, "env-write") == 0) {
        return CAP_ENV_WRITE;
    }
    if (strcmp(name, "process-spawn") == 0 || strcmp(name, "run") == 0) {
        return CAP_PROCESS_SPAWN;
    }
    if (strcmp(name, "ffi-call") == 0 || strcmp(name, "ffi") == 0) {
        return CAP_FFI_CALL;
    }
    if (strcmp(name, "unsafe-mem") == 0 || strcmp(name, "mem") == 0) {
        return CAP_UNSAFE_MEM;
    }
    if (strcmp(name, "all") == 0) {
        return CAP_ALL;
    }
    
    return CAP_NONE;
}

/* ===================================================================
 * Capability Manager (Global Singleton)
 * =================================================================== */

/**
 * Get the global capability manager instance
 */
CapabilityManager *capability_manager_get(void) {
    return global_manager;
}

/**
 * Initialize the global capability manager
 */
void capability_manager_init(void) {
    if (global_manager) {
        return; /* Already initialized */
    }
    
    global_manager = (CapabilityManager *)calloc(1, sizeof(CapabilityManager));
    if (!global_manager) {
        fprintf(stderr, "Fatal: Failed to initialize capability manager\n");
        return;
    }
    
    global_manager->contexts = (CapabilityContext **)malloc(
        INITIAL_CAPACITY * sizeof(CapabilityContext *));
    if (!global_manager->contexts) {
        free(global_manager);
        global_manager = NULL;
        fprintf(stderr, "Fatal: Failed to allocate capability manager contexts\n");
        return;
    }
    
    global_manager->context_count = 0;
    global_manager->context_capacity = INITIAL_CAPACITY;
    global_manager->global_audit_enabled = 0;
    global_manager->global_audit_log = NULL;
    global_manager->strict_mode_default = 0;
}

/**
 * Shutdown the global capability manager
 */
void capability_manager_shutdown(void) {
    if (!global_manager) {
        return;
    }
    
    /* Close global audit log if open */
    if (global_manager->global_audit_log && 
        global_manager->global_audit_log != stderr && 
        global_manager->global_audit_log != stdout) {
        fclose(global_manager->global_audit_log);
    }
    
    /* Free contexts array (don't free the contexts themselves - 
       they're owned by their interpreters) */
    if (global_manager->contexts) {
        free(global_manager->contexts);
    }
    
    free(global_manager);
    global_manager = NULL;
}

/**
 * Register a capability context with the manager
 */
void capability_manager_register(CapabilityContext *ctx) {
    CapabilityContext **new_contexts;
    int new_capacity;
    
    if (!global_manager || !ctx) {
        return;
    }
    
    /* Grow array if needed */
    if (global_manager->context_count >= global_manager->context_capacity) {
        new_capacity = global_manager->context_capacity * 2;
        new_contexts = (CapabilityContext **)realloc(
            global_manager->contexts,
            new_capacity * sizeof(CapabilityContext *));
        if (!new_contexts) {
            fprintf(stderr, "Error: Failed to grow capability manager contexts\n");
            return;
        }
        global_manager->contexts = new_contexts;
        global_manager->context_capacity = new_capacity;
    }
    
    global_manager->contexts[global_manager->context_count++] = ctx;
    
    /* Apply global audit settings if enabled */
    if (global_manager->global_audit_enabled) {
        ctx->audit_enabled = 1;
        if (global_manager->global_audit_log) {
            ctx->audit_log = global_manager->global_audit_log;
        }
    }
}

/**
 * Unregister a capability context from the manager
 */
void capability_manager_unregister(CapabilityContext *ctx) {
    int i, j;
    if (!global_manager || !ctx) {
        return;
    }
    
    /* Find and remove the context */
    for (i = 0; i < global_manager->context_count; i++) {
        if (global_manager->contexts[i] == ctx) {
            /* Shift remaining contexts down */
            for (j = i; j < global_manager->context_count - 1; j++) {
                global_manager->contexts[j] = global_manager->contexts[j + 1];
            }
            global_manager->context_count--;
            return;
        }
    }
}

/**
 * Enable global audit logging
 */
void capability_manager_enable_audit(const char *log_file) {
    int i;
    if (!global_manager) {
        return;
    }
    
    global_manager->global_audit_enabled = 1;
    
    /* Open log file if specified */
    if (log_file) {
        global_manager->global_audit_log = fopen(log_file, "a");
        if (!global_manager->global_audit_log) {
            fprintf(stderr, "Warning: Failed to open audit log '%s', using stderr\n", 
                   log_file);
            global_manager->global_audit_log = stderr;
        }
    } else {
        global_manager->global_audit_log = stderr;
    }
    
    /* Apply to all registered contexts */
    for (i = 0; i < global_manager->context_count; i++) {
        global_manager->contexts[i]->audit_enabled = 1;
        global_manager->contexts[i]->audit_log = global_manager->global_audit_log;
    }
}

/**
 * Disable global audit logging
 */
void capability_manager_disable_audit(void) {
    int i;
    if (!global_manager) {
        return;
    }
    
    /* Close log file if open */
    if (global_manager->global_audit_log && 
        global_manager->global_audit_log != stderr && 
        global_manager->global_audit_log != stdout) {
        fclose(global_manager->global_audit_log);
    }
    
    global_manager->global_audit_enabled = 0;
    global_manager->global_audit_log = NULL;
    
    /* Disable for all registered contexts */
    for (i = 0; i < global_manager->context_count; i++) {
        global_manager->contexts[i]->audit_enabled = 0;
        global_manager->contexts[i]->audit_log = NULL;
    }
}
