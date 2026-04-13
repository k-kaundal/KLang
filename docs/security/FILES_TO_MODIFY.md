# Files to Create and Modify

This document provides a complete list of all files that need to be created or modified to implement the KLang security architecture.

## Summary

- **New files to create:** 27
- **Existing files to modify:** 16
- **Total files affected:** 43

---

## New Files to Create

### Headers (include/)

1. **`include/security.h`**
   - Purpose: Core security types, safety context
   - Size: ~100 lines
   - Dependencies: None
   - Priority: High

2. **`include/capability.h`**
   - Purpose: Capability system types and functions
   - Size: ~150 lines
   - Dependencies: security.h
   - Priority: High

3. **`include/sandbox.h`**
   - Purpose: Sandbox types and management
   - Size: ~150 lines
   - Dependencies: capability.h, interpreter.h
   - Priority: High

### Source Files (src/runtime/security/)

4. **`src/runtime/security/security_context.c`**
   - Purpose: Safety context implementation
   - Size: ~200 lines
   - Dependencies: security.h, interpreter.h
   - Priority: High

5. **`src/runtime/security/capability.c`**
   - Purpose: Capability system implementation
   - Size: ~400 lines
   - Dependencies: capability.h, interpreter.h
   - Priority: High

6. **`src/runtime/security/sandbox.c`**
   - Purpose: Sandbox implementation
   - Size: ~500 lines
   - Dependencies: sandbox.h, interpreter.h
   - Priority: High

7. **`src/runtime/security/manifest_parser.c`**
   - Purpose: Parse kl.toml security manifests
   - Size: ~300 lines
   - Dependencies: capability.h, sandbox.h
   - Priority: Medium

8. **`src/runtime/security/audit_log.c`**
   - Purpose: Security audit logging
   - Size: ~200 lines
   - Dependencies: capability.h, sandbox.h
   - Priority: Medium

9. **`src/core/parser/unsafe_parser.c`**
   - Purpose: Parse unsafe block syntax
   - Size: ~150 lines
   - Dependencies: parser.h, ast.h
   - Priority: High

### Test Files (tests/security/)

10. **`tests/security/test_unsafe_blocks.c`**
    - Purpose: Unit tests for unsafe blocks
    - Size: ~400 lines
    - Priority: High

11. **`tests/security/test_capabilities.c`**
    - Purpose: Unit tests for capability system
    - Size: ~500 lines
    - Priority: High

12. **`tests/security/test_sandbox.c`**
    - Purpose: Unit tests for sandboxes
    - Size: ~600 lines
    - Priority: High

13. **`tests/security/test_security_integration.c`**
    - Purpose: Integration tests
    - Size: ~500 lines
    - Priority: Medium

### Documentation (docs/security/)

14. **`docs/security/SECURITY_ARCHITECTURE.md`**
    - Purpose: Complete security architecture design
    - Size: ~1000 lines (already created)
    - Priority: High

15. **`docs/security/IMPLEMENTATION_PLAN.md`**
    - Purpose: Detailed implementation plan
    - Size: ~600 lines (already created)
    - Priority: High

16. **`docs/security/README.md`**
    - Purpose: Security features overview
    - Size: ~300 lines (already created)
    - Priority: High

17. **`docs/security/SECURITY_GUIDE.md`**
    - Purpose: User guide for security features
    - Size: ~500 lines
    - Priority: High

18. **`docs/security/CAPABILITY_GUIDE.md`**
    - Purpose: Guide to capability system
    - Size: ~400 lines
    - Priority: Medium

19. **`docs/security/SANDBOX_GUIDE.md`**
    - Purpose: Guide to sandboxes
    - Size: ~400 lines
    - Priority: Medium

20. **`docs/security/FILES_TO_MODIFY.md`**
    - Purpose: This file
    - Size: ~300 lines
    - Priority: High

### Examples (examples/security/)

21. **`examples/security/unsafe_example.kl`**
    - Purpose: Examples of unsafe blocks
    - Size: ~150 lines
    - Priority: High

22. **`examples/security/capability_example.kl`**
    - Purpose: Examples of capability system
    - Size: ~200 lines
    - Priority: High

23. **`examples/security/sandbox_example.kl`**
    - Purpose: Examples of sandboxes
    - Size: ~200 lines
    - Priority: High

24. **`examples/security/advanced_security.kl`**
    - Purpose: Advanced security combinations
    - Size: ~250 lines
    - Priority: Medium

### Benchmarks (benchmarks/security/)

25. **`benchmarks/security/security_bench.c`**
    - Purpose: Performance benchmarks
    - Size: ~300 lines
    - Priority: Low

26. **`benchmarks/security/capability_bench.c`**
    - Purpose: Capability system benchmarks
    - Size: ~200 lines
    - Priority: Low

27. **`benchmarks/security/sandbox_bench.c`**
    - Purpose: Sandbox benchmarks
    - Size: ~200 lines
    - Priority: Low

---

## Existing Files to Modify

### Core Interpreter

#### 1. `src/interpreter.h`
**Changes:**
- Add security context fields to `Interpreter` struct
- Add security-related forward declarations

**Lines to Add:** ~10
**Location:** After line 216 (end of struct Interpreter)

```c
struct Interpreter {
    // ... existing fields ...
    
    /* Security contexts */
    SafetyContext *safety_ctx;      /* Unsafe block tracking */
    CapabilityContext *cap_ctx;     /* Capability management */
    SandboxContext *sandbox_ctx;    /* Sandbox management */
};
```

**Priority:** High  
**Dependencies:** security.h, capability.h, sandbox.h

#### 2. `src/runtime/interpreter/interpreter.c`
**Changes:**
- Initialize security contexts in `interpreter_new()`
- Free security contexts in `interpreter_free()`
- Add unsafe block evaluation

**Lines to Add:** ~50
**Locations:** 
- In `interpreter_new()` after global_env initialization
- In `interpreter_free()` before freeing interpreter
- New function `eval_unsafe_block()`

**Priority:** High  
**Dependencies:** security.h

### Parser & AST

#### 3. `src/core/ast/ast.h`
**Changes:**
- Add new AST node types for security features

**Lines to Add:** ~15
**Location:** In `NodeType` enum (around line 18)

```c
typedef enum {
    // ... existing nodes ...
    NODE_UNSAFE_BLOCK,        /* unsafe { ... } */
    NODE_SANDBOX_DEF,         /* sandbox Name { ... } */
    NODE_RUN_IN_SANDBOX       /* run in SandboxName { ... } */
} NodeType;
```

And in struct ASTNode union:
```c
struct ASTNode {
    // ... existing fields ...
    struct { ASTNode *body; } unsafe_block;
    struct { char *name; /* sandbox definition fields */ } sandbox_def;
    struct { char *sandbox_name; ASTNode *body; } run_in_sandbox;
};
```

**Priority:** High  
**Dependencies:** None

#### 4. `src/core/parser/parser.c`
**Changes:**
- Add parsing for `unsafe { ... }` blocks
- Add parsing for `sandbox` definitions  
- Add parsing for `run in` statements
- Add new keywords to lexer

**Lines to Add:** ~200
**Locations:**
- New function `parse_unsafe_block()`
- New function `parse_sandbox_def()`
- New function `parse_run_in_sandbox()`
- Modify `parse_statement()` to recognize new keywords

**Priority:** High  
**Dependencies:** ast.h

### CLI

#### 5. `src/cli/cli.c`
**Changes:**
- Parse `--allow-*` flags
- Parse `--strict`, `--warn-unsafe`, `--unsafe-all` flags
- Pass security flags to interpreter

**Lines to Add:** ~100
**Location:** In argument parsing, before file execution

```c
/* New function */
void parse_security_flags(int argc, char **argv, Interpreter *interp) {
    // Parse all security-related flags
}
```

**Priority:** High  
**Dependencies:** capability.h

#### 6. `include/cli_commands.h`
**Changes:**
- Add security flag definitions
- Add help text for security flags

**Lines to Add:** ~30
**Location:** In command definitions

**Priority:** Medium  
**Dependencies:** None

### Builtins (Add Security Checks)

#### 7. `src/runtime/builtins/builtin_file.c`
**Changes:**
- Add unsafe context checks to all file operations
- Add capability checks (CAP_FILE_READ, CAP_FILE_WRITE, CAP_FILE_DELETE)

**Functions to Modify:**
- `builtin_fopen()` - check CAP_FILE_READ or CAP_FILE_WRITE
- `builtin_fread()` - check unsafe context
- `builtin_fwrite()` - check unsafe context
- `builtin_readFile()` - check CAP_FILE_READ + unsafe
- `builtin_writeFile()` - check CAP_FILE_WRITE + unsafe
- `builtin_appendFile()` - check CAP_FILE_WRITE + unsafe
- `builtin_fdelete()` - check CAP_FILE_DELETE + unsafe
- `builtin_frename()` - check CAP_FILE_WRITE + unsafe

**Lines to Add:** ~50 (2-3 lines per function)
**Priority:** High  
**Dependencies:** security.h, capability.h

#### 8. `src/runtime/builtins/builtin_http.c`
**Changes:**
- Add unsafe context checks
- Add capability checks (CAP_NETWORK_CONNECT)

**Functions to Modify:**
- `builtin_http_get()` - check CAP_NETWORK_CONNECT + unsafe
- `builtin_http_post()` - check CAP_NETWORK_CONNECT + unsafe
- `builtin_http_request()` - check CAP_NETWORK_CONNECT + unsafe

**Lines to Add:** ~15
**Priority:** High  
**Dependencies:** security.h, capability.h

#### 9. `src/runtime/builtins/builtin_env.c`
**Changes:**
- Add unsafe context checks
- Add capability checks (CAP_ENV_READ, CAP_ENV_WRITE)

**Functions to Modify:**
- `builtin_env_get()` - check CAP_ENV_READ + unsafe
- `builtin_env_set()` - check CAP_ENV_WRITE + unsafe
- `builtin_env_has()` - check CAP_ENV_READ + unsafe

**Lines to Add:** ~10
**Priority:** High  
**Dependencies:** security.h, capability.h

#### 10. `src/runtime/builtins/builtin_core.c`
**Changes:**
- Add unsafe context checks to memory functions
- Add capability checks (CAP_UNSAFE_MEM)

**Functions to Modify:**
- `builtin_malloc()` - check CAP_UNSAFE_MEM + unsafe
- `builtin_free()` - check CAP_UNSAFE_MEM + unsafe
- `builtin_realloc()` - check CAP_UNSAFE_MEM + unsafe
- `builtin_calloc()` - check CAP_UNSAFE_MEM + unsafe

**Lines to Add:** ~20
**Priority:** High  
**Dependencies:** security.h, capability.h

### VM Backends (Phase 5)

#### 11. `src/vm/vm_register.c`
**Changes:**
- Add security checks to VM operations
- Integrate with capability system

**Lines to Add:** ~100
**Priority:** Low (Phase 5)  
**Dependencies:** security.h, capability.h

#### 12. `src/vm/vm_stack.c`
**Changes:**
- Add security checks to VM operations
- Integrate with capability system

**Lines to Add:** ~100
**Priority:** Low (Phase 5)  
**Dependencies:** security.h, capability.h

#### 13. `src/compiler/llvm/llvm_backend.c`
**Changes:**
- Generate security checks in compiled code
- Integrate with capability system

**Lines to Add:** ~150
**Priority:** Low (Phase 5)  
**Dependencies:** security.h, capability.h

### Documentation

#### 14. `README.md`
**Changes:**
- Add security features section
- Add security examples
- Update feature list

**Lines to Add:** ~50
**Location:** After "Features" section

**Priority:** Medium  
**Dependencies:** None

#### 15. `SECURITY.md`
**Changes:**
- Update with new security features
- Add security architecture overview
- Link to detailed security docs

**Lines to Add:** ~100
**Location:** Expand "Security Features" section

**Priority:** Medium  
**Dependencies:** None

### Build System

#### 16. `Makefile`
**Changes:**
- Add security module compilation
- Add security tests
- Add security benchmarks

**Lines to Add:** ~30
**Location:** In source file lists and test targets

```makefile
# Security module
SECURITY_SRC = \
    src/runtime/security/security_context.c \
    src/runtime/security/capability.c \
    src/runtime/security/sandbox.c

# Security tests
SECURITY_TESTS = \
    tests/security/test_unsafe_blocks \
    tests/security/test_capabilities \
    tests/security/test_sandbox
```

**Priority:** High  
**Dependencies:** None

---

## Implementation Order

### Phase 1: Foundation (Weeks 1-2)
**Files to Create:**
1. include/security.h
2. include/capability.h
3. src/runtime/security/security_context.c
4. src/runtime/security/capability.c

**Files to Modify:**
1. src/interpreter.h
2. src/runtime/interpreter/interpreter.c
3. src/cli/cli.c
4. Makefile

### Phase 2: Unsafe Blocks (Weeks 3-4)
**Files to Create:**
1. src/core/parser/unsafe_parser.c

**Files to Modify:**
1. src/core/ast/ast.h
2. src/core/parser/parser.c
3. src/runtime/builtins/builtin_file.c
4. src/runtime/builtins/builtin_http.c
5. src/runtime/builtins/builtin_env.c
6. src/runtime/builtins/builtin_core.c

### Phase 3: Sandbox Engine (Weeks 5-6)
**Files to Create:**
1. include/sandbox.h
2. src/runtime/security/sandbox.c

**Files to Modify:**
1. src/core/ast/ast.h (add sandbox nodes)
2. src/core/parser/parser.c (parse sandbox)

### Phase 4: Testing & Docs (Weeks 7-8)
**Files to Create:**
1. tests/security/test_unsafe_blocks.c
2. tests/security/test_capabilities.c
3. tests/security/test_sandbox.c
4. tests/security/test_security_integration.c
5. docs/security/SECURITY_GUIDE.md
6. docs/security/CAPABILITY_GUIDE.md
7. docs/security/SANDBOX_GUIDE.md
8. examples/security/*.kl (all 4 files)

**Files to Modify:**
1. README.md
2. SECURITY.md

### Phase 5: Advanced Features (Weeks 9-10)
**Files to Create:**
1. src/runtime/security/manifest_parser.c
2. src/runtime/security/audit_log.c
3. benchmarks/security/*.c (all 3 files)

**Files to Modify:**
1. src/vm/vm_register.c
2. src/vm/vm_stack.c
3. src/compiler/llvm/llvm_backend.c

---

## Dependency Graph

```
security.h
└── capability.h
    └── sandbox.h
        └── All implementation files

interpreter.h
├── security.h
├── capability.h
└── sandbox.h

ast.h
└── parser.c
    └── unsafe_parser.c

All builtins
├── security.h
└── capability.h
```

---

## Testing Checklist

- [ ] Unit tests for safety context
- [ ] Unit tests for capability system
- [ ] Unit tests for sandbox engine
- [ ] Integration tests for all features combined
- [ ] Security tests (attempt to bypass)
- [ ] Performance benchmarks
- [ ] Backwards compatibility tests

---

## Documentation Checklist

- [x] SECURITY_ARCHITECTURE.md
- [x] IMPLEMENTATION_PLAN.md
- [x] README.md (this file)
- [x] FILES_TO_MODIFY.md (this file)
- [ ] SECURITY_GUIDE.md
- [ ] CAPABILITY_GUIDE.md
- [ ] SANDBOX_GUIDE.md
- [ ] Update main README.md
- [ ] Update SECURITY.md
- [ ] API documentation

---

## Code Review Checklist

Before merging security changes:

- [ ] All new files created
- [ ] All existing files modified
- [ ] No breaking changes in v1.x
- [ ] Backwards compatibility maintained
- [ ] All tests passing
- [ ] Documentation complete
- [ ] Security audit performed
- [ ] Performance benchmarks meet targets
- [ ] Code reviewed by security expert

---

**Last Updated:** 2025  
**Status:** Planning Complete, Ready for Implementation
