# KLang Security Implementation Plan

**Version:** 1.0  
**Date:** 2025  
**Related:** SECURITY_ARCHITECTURE.md

## Overview

This document provides a detailed, step-by-step implementation plan for the three major security features:
1. Security-by-default (unsafe blocks)
2. Capability-based execution
3. Sandbox-native support

## Implementation Order

The features will be implemented in the following order to minimize dependencies:

1. **Phase 1:** Foundation & Capability System (Weeks 1-2)
2. **Phase 2:** Unsafe Blocks (Weeks 3-4)
3. **Phase 3:** Sandbox Engine (Weeks 5-6)
4. **Phase 4:** Testing & Documentation (Weeks 7-8)
5. **Phase 5:** Advanced Features & Optimization (Weeks 9-10)

---

## Phase 1: Foundation & Capability System

### Week 1: Core Infrastructure

#### 1.1 Create Security Headers

**File:** `include/security.h`
```c
#ifndef SECURITY_H
#define SECURITY_H

#include <stddef.h>
#include <stdio.h>

/* Forward declarations */
typedef struct Interpreter Interpreter;
typedef struct Capability Capability;
typedef struct CapabilitySet CapabilitySet;
typedef struct CapabilityContext CapabilityContext;
typedef struct SafetyContext SafetyContext;

/* Safety context for unsafe block tracking */
typedef struct SafetyContext {
    int in_unsafe_context;    /* Are we in an unsafe block? */
    int unsafe_depth;          /* Nesting level of unsafe blocks */
    int strict_mode;           /* Enforce strict security? */
    int warn_only;             /* Warn about unsafe ops but allow? */
} SafetyContext;

/* Security context management */
SafetyContext *safety_context_new(void);
void safety_context_free(SafetyContext *ctx);
void safety_context_enter_unsafe(SafetyContext *ctx);
void safety_context_exit_unsafe(SafetyContext *ctx);
int safety_context_is_unsafe(SafetyContext *ctx);

/* Unsafe operation checking */
void check_unsafe_context(Interpreter *interp, const char *operation);
void require_unsafe_context(Interpreter *interp, const char *operation);

#endif /* SECURITY_H */
```

**File:** `include/capability.h`
```c
#ifndef CAPABILITY_H
#define CAPABILITY_H

#include <stddef.h>

/* Capability types */
typedef enum {
    CAP_NONE = 0,
    CAP_FILE_READ,          /* Read files */
    CAP_FILE_WRITE,         /* Write files */
    CAP_FILE_DELETE,        /* Delete files */
    CAP_NETWORK_CONNECT,    /* Make network connections */
    CAP_NETWORK_LISTEN,     /* Listen on network ports */
    CAP_ENV_READ,           /* Read environment variables */
    CAP_ENV_WRITE,          /* Write environment variables */
    CAP_PROCESS_SPAWN,      /* Spawn child processes */
    CAP_FFI_CALL,           /* Call native functions */
    CAP_UNSAFE_MEM,         /* Manual memory management */
    CAP_ALL                 /* All capabilities */
} CapabilityType;

/* Capability structure */
typedef struct Capability {
    CapabilityType type;
    char *resource_pattern;   /* e.g., "/tmp/*", "*.json" */
    int revocable;            /* Can this capability be revoked? */
    void *metadata;           /* Additional data */
} Capability;

/* Set of capabilities */
typedef struct CapabilitySet {
    Capability *capabilities;
    int count;
    int capacity;
} CapabilitySet;

/* Capability context */
typedef struct CapabilityContext {
    CapabilitySet *granted;     /* Capabilities granted */
    CapabilitySet *requested;   /* Capabilities requested but not granted */
    int prompt_on_missing;      /* Prompt user for missing capabilities? */
    int audit_enabled;          /* Log capability checks? */
    FILE *audit_log;            /* Audit log file */
} CapabilityContext;

/* CapabilitySet operations */
CapabilitySet *capability_set_new(void);
void capability_set_free(CapabilitySet *set);
void capability_set_add(CapabilitySet *set, CapabilityType type, const char *pattern);
void capability_set_remove(CapabilitySet *set, int index);
Capability *capability_set_find(CapabilitySet *set, CapabilityType type, const char *resource);

/* CapabilityContext operations */
CapabilityContext *capability_context_new(void);
void capability_context_free(CapabilityContext *ctx);
void capability_grant(CapabilityContext *ctx, CapabilityType type, const char *pattern);
int capability_has(CapabilityContext *ctx, CapabilityType type, const char *resource);
void capability_require(Interpreter *interp, CapabilityType type, const char *resource);

/* Pattern matching for resources */
int resource_matches_pattern(const char *resource, const char *pattern);

/* Capability type names */
const char *capability_type_name(CapabilityType type);
const char *capability_flag_name(CapabilityType type);

#endif /* CAPABILITY_H */
```

**File:** `include/sandbox.h`
```c
#ifndef SANDBOX_H
#define SANDBOX_H

#include <stddef.h>
#include <stdio.h>
#include "capability.h"
#include "interpreter.h"

/* Forward declarations */
typedef struct Sandbox Sandbox;
typedef struct SandboxContext SandboxContext;
typedef struct Env Env;

/* Sandbox profiles */
typedef enum {
    SANDBOX_PROFILE_NONE,        /* No restrictions */
    SANDBOX_PROFILE_STRICT,      /* Maximum security */
    SANDBOX_PROFILE_NETWORK,     /* Network-only access */
    SANDBOX_PROFILE_FILESYSTEM,  /* Filesystem-only access */
    SANDBOX_PROFILE_COMPUTE,     /* Pure computation, no I/O */
    SANDBOX_PROFILE_UNTRUSTED    /* For untrusted code */
} SandboxProfile;

/* Sandbox structure */
typedef struct Sandbox {
    char *name;                      /* Sandbox name */
    CapabilitySet *allowed_caps;     /* Allowed capabilities */
    
    /* Resource limits */
    size_t memory_limit_bytes;       /* Maximum memory */
    long cpu_time_limit_ms;          /* Maximum CPU time */
    int max_open_files;              /* Maximum open file descriptors */
    int max_network_connections;     /* Maximum network connections */
    int max_processes;               /* Maximum child processes */
    
    /* Security policies */
    char **deny_list;                /* Blacklist of resources */
    int deny_count;
    int deny_capacity;
    
    /* Runtime state */
    size_t memory_used;              /* Current memory usage */
    long cpu_time_used_ms;           /* CPU time consumed */
    long start_time_ms;              /* Sandbox start time */
    int open_files;                  /* Current open files */
    int active_connections;          /* Active network connections */
    
    /* Sandbox isolation */
    int isolated_env;                /* Use isolated environment? */
    Env *sandbox_env;                /* Sandboxed environment */
    
    /* Audit */
    int audit_enabled;               /* Enable audit logging? */
    FILE *audit_log;                 /* Audit log file */
} Sandbox;

/* Sandbox context */
typedef struct SandboxContext {
    Sandbox *current_sandbox;        /* Current active sandbox */
    Sandbox *parent_sandbox;         /* Parent sandbox (for nesting) */
    int sandbox_depth;               /* Nesting depth */
} SandboxContext;

/* Sandbox operations */
Sandbox *sandbox_new(const char *name);
void sandbox_free(Sandbox *sb);
Sandbox *sandbox_from_profile(SandboxProfile profile);

/* Sandbox enforcement */
void sandbox_check_memory(Interpreter *interp, size_t allocation_size);
void sandbox_check_cpu_time(Interpreter *interp);
void sandbox_check_access(Interpreter *interp, const char *resource);
void sandbox_track_file_open(Interpreter *interp);
void sandbox_track_file_close(Interpreter *interp);
void sandbox_track_connection_open(Interpreter *interp);
void sandbox_track_connection_close(Interpreter *interp);

/* SandboxContext operations */
SandboxContext *sandbox_context_new(void);
void sandbox_context_free(SandboxContext *ctx);
void sandbox_context_enter(SandboxContext *ctx, Sandbox *sb);
void sandbox_context_exit(SandboxContext *ctx);

/* Deny list operations */
void sandbox_add_deny_pattern(Sandbox *sb, const char *pattern);
int sandbox_is_denied(Sandbox *sb, const char *resource);

#endif /* SANDBOX_H */
```

#### 1.2 Modify Interpreter Header

**File:** `src/interpreter.h` (add to struct Interpreter)
```c
struct Interpreter {
    Env *global_env;
    EvalResult last_result;
    int had_error;
    MicrotaskNode *microtask_queue_head;
    MicrotaskNode *microtask_queue_tail;
    LoadedModule *loaded_modules;
    int module_count;
    int module_capacity;
    char *current_module_dir;
    
    /* SECURITY CONTEXTS - NEW */
    SafetyContext *safety_ctx;      /* Unsafe block tracking */
    CapabilityContext *cap_ctx;     /* Capability management */
    SandboxContext *sandbox_ctx;    /* Sandbox management */
};
```

#### 1.3 Implement Security Context

**File:** `src/runtime/security/security_context.c`
```c
#include "security.h"
#include "interpreter.h"
#include "error_reporter.h"
#include <stdlib.h>
#include <string.h>

SafetyContext *safety_context_new(void) {
    SafetyContext *ctx = malloc(sizeof(SafetyContext));
    if (!ctx) return NULL;
    
    ctx->in_unsafe_context = 0;
    ctx->unsafe_depth = 0;
    ctx->strict_mode = 0;      /* Default: permissive for v1.x */
    ctx->warn_only = 0;
    
    return ctx;
}

void safety_context_free(SafetyContext *ctx) {
    if (ctx) {
        free(ctx);
    }
}

void safety_context_enter_unsafe(SafetyContext *ctx) {
    if (!ctx) return;
    ctx->unsafe_depth++;
    ctx->in_unsafe_context = 1;
}

void safety_context_exit_unsafe(SafetyContext *ctx) {
    if (!ctx) return;
    if (ctx->unsafe_depth > 0) {
        ctx->unsafe_depth--;
    }
    if (ctx->unsafe_depth == 0) {
        ctx->in_unsafe_context = 0;
    }
}

int safety_context_is_unsafe(SafetyContext *ctx) {
    return ctx && ctx->in_unsafe_context;
}

void check_unsafe_context(Interpreter *interp, const char *operation) {
    if (!interp || !interp->safety_ctx) return;
    
    /* Permissive mode for backwards compatibility */
    if (!interp->safety_ctx->strict_mode) {
        if (interp->safety_ctx->warn_only && !interp->safety_ctx->in_unsafe_context) {
            fprintf(stderr, "Warning: Operation '%s' should be in unsafe block\n", operation);
        }
        return;
    }
    
    /* Strict mode: enforce unsafe blocks */
    if (!interp->safety_ctx->in_unsafe_context) {
        error_report(interp,
            "Operation '%s' requires unsafe context. "
            "Wrap in 'unsafe { ... }' block or run with --unsafe-all",
            operation);
        interp->had_error = 1;
    }
}

void require_unsafe_context(Interpreter *interp, const char *operation) {
    check_unsafe_context(interp, operation);
}
```

### Week 2: Capability System

#### 2.1 Implement Capability Management

**File:** `src/runtime/security/capability.c`
```c
#include "capability.h"
#include "interpreter.h"
#include "error_reporter.h"
#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>

/* Capability set operations */

CapabilitySet *capability_set_new(void) {
    CapabilitySet *set = malloc(sizeof(CapabilitySet));
    if (!set) return NULL;
    
    set->capacity = 8;
    set->count = 0;
    set->capabilities = malloc(sizeof(Capability) * set->capacity);
    
    if (!set->capabilities) {
        free(set);
        return NULL;
    }
    
    return set;
}

void capability_set_free(CapabilitySet *set) {
    if (!set) return;
    
    for (int i = 0; i < set->count; i++) {
        if (set->capabilities[i].resource_pattern) {
            free(set->capabilities[i].resource_pattern);
        }
        if (set->capabilities[i].metadata) {
            free(set->capabilities[i].metadata);
        }
    }
    
    free(set->capabilities);
    free(set);
}

void capability_set_add(CapabilitySet *set, CapabilityType type, const char *pattern) {
    if (!set) return;
    
    /* Expand if needed */
    if (set->count >= set->capacity) {
        set->capacity *= 2;
        Capability *new_caps = realloc(set->capabilities, sizeof(Capability) * set->capacity);
        if (!new_caps) return;
        set->capabilities = new_caps;
    }
    
    /* Add capability */
    Capability *cap = &set->capabilities[set->count];
    cap->type = type;
    cap->resource_pattern = pattern ? strdup(pattern) : strdup("*");
    cap->revocable = 1;
    cap->metadata = NULL;
    
    set->count++;
}

Capability *capability_set_find(CapabilitySet *set, CapabilityType type, const char *resource) {
    if (!set || !resource) return NULL;
    
    for (int i = 0; i < set->count; i++) {
        Capability *cap = &set->capabilities[i];
        if (cap->type == type || cap->type == CAP_ALL) {
            if (resource_matches_pattern(resource, cap->resource_pattern)) {
                return cap;
            }
        }
    }
    
    return NULL;
}

/* Capability context operations */

CapabilityContext *capability_context_new(void) {
    CapabilityContext *ctx = malloc(sizeof(CapabilityContext));
    if (!ctx) return NULL;
    
    ctx->granted = capability_set_new();
    ctx->requested = capability_set_new();
    ctx->prompt_on_missing = 0;
    ctx->audit_enabled = 0;
    ctx->audit_log = NULL;
    
    return ctx;
}

void capability_context_free(CapabilityContext *ctx) {
    if (!ctx) return;
    
    capability_set_free(ctx->granted);
    capability_set_free(ctx->requested);
    
    if (ctx->audit_log) {
        fclose(ctx->audit_log);
    }
    
    free(ctx);
}

void capability_grant(CapabilityContext *ctx, CapabilityType type, const char *pattern) {
    if (!ctx) return;
    capability_set_add(ctx->granted, type, pattern);
}

int capability_has(CapabilityContext *ctx, CapabilityType type, const char *resource) {
    if (!ctx) return 0;
    return capability_set_find(ctx->granted, type, resource) != NULL;
}

void capability_require(Interpreter *interp, CapabilityType type, const char *resource) {
    if (!interp || !interp->cap_ctx) return;
    
    /* Check if we have the capability */
    if (capability_has(interp->cap_ctx, type, resource)) {
        /* Audit if enabled */
        if (interp->cap_ctx->audit_enabled && interp->cap_ctx->audit_log) {
            fprintf(interp->cap_ctx->audit_log, 
                "GRANTED: %s for %s\n",
                capability_type_name(type), resource);
        }
        return;
    }
    
    /* Missing capability */
    capability_set_add(interp->cap_ctx->requested, type, resource);
    
    /* Audit denied access */
    if (interp->cap_ctx->audit_enabled && interp->cap_ctx->audit_log) {
        fprintf(interp->cap_ctx->audit_log, 
            "DENIED: %s for %s\n",
            capability_type_name(type), resource);
    }
    
    error_report(interp,
        "Missing capability: %s for resource '%s'. "
        "Grant with --%s or --allow-all",
        capability_type_name(type), resource,
        capability_flag_name(type));
    interp->had_error = 1;
}

/* Pattern matching using fnmatch */
int resource_matches_pattern(const char *resource, const char *pattern) {
    if (!resource || !pattern) return 0;
    if (strcmp(pattern, "*") == 0) return 1;
    return fnmatch(pattern, resource, 0) == 0;
}

/* Capability type names */
const char *capability_type_name(CapabilityType type) {
    switch (type) {
        case CAP_FILE_READ: return "file-read";
        case CAP_FILE_WRITE: return "file-write";
        case CAP_FILE_DELETE: return "file-delete";
        case CAP_NETWORK_CONNECT: return "network-connect";
        case CAP_NETWORK_LISTEN: return "network-listen";
        case CAP_ENV_READ: return "env-read";
        case CAP_ENV_WRITE: return "env-write";
        case CAP_PROCESS_SPAWN: return "process-spawn";
        case CAP_FFI_CALL: return "ffi-call";
        case CAP_UNSAFE_MEM: return "unsafe-mem";
        case CAP_ALL: return "all";
        default: return "unknown";
    }
}

const char *capability_flag_name(CapabilityType type) {
    switch (type) {
        case CAP_FILE_READ: return "allow-read";
        case CAP_FILE_WRITE: return "allow-write";
        case CAP_FILE_DELETE: return "allow-delete";
        case CAP_NETWORK_CONNECT: return "allow-net";
        case CAP_NETWORK_LISTEN: return "allow-listen";
        case CAP_ENV_READ: return "allow-env";
        case CAP_ENV_WRITE: return "allow-env-write";
        case CAP_PROCESS_SPAWN: return "allow-run";
        case CAP_FFI_CALL: return "allow-ffi";
        case CAP_UNSAFE_MEM: return "allow-unsafe-mem";
        case CAP_ALL: return "allow-all";
        default: return "unknown";
    }
}
```

#### 2.2 Update CLI to Parse Capability Flags

**File:** `src/cli/cli.c` (add to argument parsing)
```c
/* Parse --allow-* flags */
void parse_security_flags(int argc, char **argv, Interpreter *interp) {
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--allow-", 8) == 0) {
            char *flag = argv[i] + 8;
            char *pattern = strchr(flag, '=');
            
            if (pattern) {
                *pattern = '\0';
                pattern++;
            } else {
                pattern = "*";
            }
            
            /* Parse flag and grant capability */
            if (strcmp(flag, "read") == 0) {
                capability_grant(interp->cap_ctx, CAP_FILE_READ, pattern);
            } else if (strcmp(flag, "write") == 0) {
                capability_grant(interp->cap_ctx, CAP_FILE_WRITE, pattern);
            } else if (strcmp(flag, "net") == 0) {
                capability_grant(interp->cap_ctx, CAP_NETWORK_CONNECT, pattern);
            } else if (strcmp(flag, "env") == 0) {
                capability_grant(interp->cap_ctx, CAP_ENV_READ, pattern);
            } else if (strcmp(flag, "all") == 0) {
                capability_grant(interp->cap_ctx, CAP_ALL, "*");
            }
        } else if (strcmp(argv[i], "--strict") == 0) {
            interp->safety_ctx->strict_mode = 1;
        } else if (strcmp(argv[i], "--warn-unsafe") == 0) {
            interp->safety_ctx->warn_only = 1;
        } else if (strcmp(argv[i], "--unsafe-all") == 0 || strcmp(argv[i], "--legacy") == 0) {
            /* Legacy permissive mode */
            interp->safety_ctx->strict_mode = 0;
            capability_grant(interp->cap_ctx, CAP_ALL, "*");
        }
    }
}
```

---

## Files to Create/Modify - Complete List

### New Files to Create

**Headers:**
1. `include/security.h` - Core security types and safety context
2. `include/capability.h` - Capability system types and functions
3. `include/sandbox.h` - Sandbox types and functions

**Source Files:**
4. `src/runtime/security/security_context.c` - Safety context implementation
5. `src/runtime/security/capability.c` - Capability system implementation
6. `src/runtime/security/sandbox.c` - Sandbox implementation
7. `src/runtime/security/manifest_parser.c` - Parse kl.toml manifests
8. `src/runtime/security/audit_log.c` - Audit logging

**Tests:**
9. `tests/security/test_unsafe_blocks.c` - Test unsafe blocks
10. `tests/security/test_capabilities.c` - Test capability system
11. `tests/security/test_sandbox.c` - Test sandboxes
12. `tests/security/test_security_integration.c` - Integration tests

**Documentation:**
13. `docs/security/SECURITY_ARCHITECTURE.md` - This document
14. `docs/security/SECURITY_GUIDE.md` - User guide
15. `docs/security/CAPABILITY_GUIDE.md` - Capability guide
16. `docs/security/SANDBOX_GUIDE.md` - Sandbox guide
17. `docs/security/IMPLEMENTATION_PLAN.md` - Implementation plan (this)

**Examples:**
18. `examples/security/unsafe_example.kl` - Unsafe block examples
19. `examples/security/capability_example.kl` - Capability examples
20. `examples/security/sandbox_example.kl` - Sandbox examples

### Files to Modify

**Core Interpreter:**
1. `src/interpreter.h` - Add security contexts to Interpreter struct
2. `src/runtime/interpreter/interpreter.c` - Initialize/cleanup security contexts

**CLI:**
3. `src/cli/cli.c` - Parse --allow-* flags
4. `include/cli_commands.h` - Add security flag definitions

**Parser & AST:**
5. `src/core/ast/ast.h` - Add NODE_UNSAFE_BLOCK, NODE_SANDBOX_DEF, NODE_RUN_IN_SANDBOX
6. `src/core/parser/parser.c` - Parse unsafe and sandbox syntax

**Builtins (add capability/unsafe checks):**
7. `src/runtime/builtins/builtin_file.c` - Check CAP_FILE_READ/WRITE
8. `src/runtime/builtins/builtin_http.c` - Check CAP_NETWORK_CONNECT
9. `src/runtime/builtins/builtin_env.c` - Check CAP_ENV_READ/WRITE
10. `src/runtime/builtins/builtin_core.c` - Check unsafe context for malloc/free

**VM Backends (Phase 5):**
11. `src/vm/vm_register.c` - Add security checks
12. `src/vm/vm_stack.c` - Add security checks
13. `src/compiler/llvm/llvm_backend.c` - Add security checks

**Documentation:**
14. `README.md` - Add security features section
15. `SECURITY.md` - Update with new features

**Build System:**
16. `Makefile` - Add security module compilation

---

## Implementation Priority

### High Priority (Must Have for MVP)
1. ✅ Safety context (unsafe blocks)
2. ✅ Capability context
3. ✅ Basic capability types (file, network, env)
4. ✅ Command-line capability flags
5. ✅ Unsafe block parsing and evaluation
6. ✅ Capability checks in builtins

### Medium Priority (Should Have)
7. Sandbox engine
8. Sandbox profiles
9. Manifest file support
10. Audit logging
11. Comprehensive tests

### Low Priority (Nice to Have)
12. Runtime capability requests with prompts
13. Capability delegation
14. Advanced constraint capabilities
15. VM/LLVM backend integration
16. Performance optimizations

---

## Testing Strategy

### Unit Tests
- Test safety context enter/exit
- Test capability grant/check
- Test pattern matching
- Test sandbox limit enforcement

### Integration Tests
- Test unsafe blocks with file I/O
- Test capability enforcement
- Test sandbox nesting
- Test backwards compatibility modes

### Security Tests
- Test that denied operations fail
- Test sandbox escapes (should fail)
- Test capability bypass attempts
- Fuzz testing for edge cases

---

## Backwards Compatibility Checklist

- [ ] Default to permissive mode (v1.x behavior)
- [ ] Add --strict flag for new behavior
- [ ] Add --legacy/--unsafe-all flag for old behavior
- [ ] Warn in --warn-unsafe mode
- [ ] Document migration path
- [ ] Provide migration tools
- [ ] Set deprecation timeline

---

## Success Criteria

### Functional
- [ ] Unsafe blocks work correctly
- [ ] Capability system enforces permissions
- [ ] Sandboxes limit resources
- [ ] CLI flags parse correctly
- [ ] All tests pass

### Performance
- [ ] < 1% overhead for safe code
- [ ] < 5% overhead for unsafe code
- [ ] < 10% overhead for sandboxed code

### Documentation
- [ ] Architecture documented
- [ ] User guides written
- [ ] Examples provided
- [ ] API reference complete

### Quality
- [ ] Code reviewed
- [ ] Security audited
- [ ] Tests comprehensive
- [ ] No regressions

---

**Next Steps:**
1. Review and approve this plan
2. Start Phase 1 Week 1 implementation
3. Regular progress reviews
4. Iterate based on feedback

**Document Version:** 1.0  
**Status:** Ready for Implementation
