# KLang Security Architecture Design

**Version:** 1.0  
**Date:** 2025  
**Status:** Design Document  

## Executive Summary

This document outlines the comprehensive security architecture for KLang, implementing three major security features:

1. **Security-by-default** - Make the language secure by default with opt-out for unsafe operations
2. **Capability-based execution** - Implement a capability system for resource access control
3. **Sandbox-native** - Make sandboxing a core language feature, not just for plugins

---

## Table of Contents

1. [Current Security Posture](#1-current-security-posture)
2. [Security Architecture Overview](#2-security-architecture-overview)
3. [Feature 1: Security-by-Default](#3-feature-1-security-by-default)
4. [Feature 2: Capability-Based Execution](#4-feature-2-capability-based-execution)
5. [Feature 3: Sandbox-Native](#5-feature-3-sandbox-native)
6. [Implementation Plan](#6-implementation-plan)
7. [Backwards Compatibility](#7-backwards-compatibility)
8. [Performance Considerations](#8-performance-considerations)
9. [Security Examples](#9-security-examples)
10. [Conclusion](#10-conclusion)

---

## 1. Current Security Posture

### 1.1 Existing Security Features

**Memory Safety:**
- ✅ Garbage collection (mark-and-sweep) in `src/runtime/memory/gc.c`
- ✅ Reference counting for objects, environments, dicts, sets, structs
- ✅ No explicit pointer arithmetic in safe code
- ⚠️ Manual memory functions: `malloc()`, `free()`, `realloc()`, `calloc()` exposed as builtins

**Type Safety:**
- ✅ Dynamic typing with runtime type checking
- ✅ Type system in `src/core/types/type_system.c`
- ✅ Type checker in `src/core/types/type_checker.c`

**Plugin Sandboxing:**
- ✅ Basic `PluginSandbox` structure with:
  - `allow_file_access`
  - `allow_network_access`
  - `allow_process_spawn`
  - `memory_limit_mb`
  - `cpu_time_limit_ms`
- ⚠️ Sandbox enforcement not fully implemented

**Unsafe Operations:**
- ⚠️ File I/O always available (`fopen`, `readFile`, `writeFile`, etc.)
- ⚠️ HTTP operations always available (`http_get`, `http_post`)
- ⚠️ Environment variable access always available (`env_get`, `env_set`)
- ⚠️ No unsafe block syntax
- ⚠️ No capability checks

### 1.2 Security Gaps

1. **No opt-in for unsafe operations** - All dangerous operations are available by default
2. **No capability system** - Programs have full access to all resources
3. **Sandbox only for plugins** - Main programs run unrestricted
4. **No security context** - No runtime tracking of permissions
5. **No audit trail** - No logging of security-sensitive operations

---

## 2. Security Architecture Overview

### 2.1 Core Principles

1. **Least Privilege** - Programs start with minimal permissions
2. **Explicit Grants** - Capabilities must be explicitly granted
3. **Defense in Depth** - Multiple layers of security checks
4. **Fail-Safe Defaults** - Operations fail closed, not open
5. **Audit Everything** - All security decisions are logged

### 2.2 Security Layers

```
┌──────────────────────────────────────────────────────┐
│              Application Code                         │
├──────────────────────────────────────────────────────┤
│  Language Level Security (unsafe blocks, syntax)     │
├──────────────────────────────────────────────────────┤
│  Capability System (permissions, grants)             │
├──────────────────────────────────────────────────────┤
│  Sandbox Engine (resource limits, isolation)         │
├──────────────────────────────────────────────────────┤
│  Runtime Security Context (tracking, enforcement)    │
├──────────────────────────────────────────────────────┤
│  Memory Safety (GC, bounds checking)                 │
└──────────────────────────────────────────────────────┘
```

For complete design details, including:
- Full unsafe block syntax and semantics
- Complete capability system design  
- Comprehensive sandbox implementation
- Detailed implementation phases
- Code examples and patterns
- Performance optimizations
- Migration strategies

Please see:
- **[IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md)** - Detailed implementation guide
- **[FILES_TO_MODIFY.md](FILES_TO_MODIFY.md)** - All files to create/modify
- **[QUICK_REFERENCE.md](QUICK_REFERENCE.md)** - Quick syntax reference
- **[DESIGN_SUMMARY.md](DESIGN_SUMMARY.md)** - Executive summary

---

## 3. Feature 1: Security-by-Default

See [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md) Section "Phase 2: Unsafe Blocks" for complete details.

**Key Points:**
- Unsafe operations require explicit `unsafe { }` blocks
- Safe operations work without restrictions
- Backwards compatible (permissive mode in v1.x)
- Strict mode available via `--strict` flag

**Example:**
```klang
# Safe code
let x = 10
println(x * 2)

# Unsafe code requires explicit block
unsafe {
    let data = readFile("config.json")
}
```

---

## 4. Feature 2: Capability-Based Execution

See [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md) Section "Week 2: Capability System" for complete details.

**Key Points:**
- Fine-grained permission system
- Grant capabilities via CLI flags or manifest
- Pattern matching for resources
- Audit logging support

**Example:**
```bash
# Grant specific capabilities
klang run --allow-read=/tmp --allow-net=api.example.com script.kl
```

---

## 5. Feature 3: Sandbox-Native

See [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md) Section "Phase 3: Sandbox Engine" for complete details.

**Key Points:**
- First-class language construct
- Resource limits and isolation
- Pre-defined security profiles
- Sandbox nesting support

**Example:**
```klang
sandbox Restricted {
    allow: ["file-read:/tmp/*"],
    limits: { memory: 100MB, cpu_time: 1000ms }
}

run in Restricted {
    unsafe { processData() }
}
```

---

## 6. Implementation Plan

See [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md) for the complete 10-week implementation plan.

**Phases:**
1. **Weeks 1-2:** Foundation & Capability System
2. **Weeks 3-4:** Unsafe Blocks
3. **Weeks 5-6:** Sandbox Engine
4. **Weeks 7-8:** Testing & Documentation
5. **Weeks 9-10:** Advanced Features

**Files Affected:**
- 27 new files to create
- 16 existing files to modify
- See [FILES_TO_MODIFY.md](FILES_TO_MODIFY.md) for complete list

---

## 7. Backwards Compatibility

**Strategy:**
- v1.x: Permissive by default (no breaking changes)
- Security features opt-in via flags
- v2.0: Strict by default (with migration tools)

**Compatibility Flags:**
```bash
# Legacy mode
klang run --legacy script.kl
klang run --unsafe-all script.kl

# Strict mode
klang run --strict script.kl

# Warning mode
klang run --warn-unsafe script.kl
```

See [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md) Section "Backwards Compatibility" for details.

---

## 8. Performance Considerations

**Target Overhead:**
- Safe operations: 0%
- Unsafe operations: <5%
- Capability checks: <100ns
- Sandboxed code: <10%

**Optimization Strategies:**
- Capability caching
- Static analysis
- Lazy initialization
- Zero-cost abstractions

See [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md) Section "Performance" for details.

---

## 9. Security Examples

See [QUICK_REFERENCE.md](QUICK_REFERENCE.md) for complete syntax reference and examples.

**Basic Example:**
```klang
# Safe operations
let data = [1, 2, 3, 4, 5]
println(data.map(x => x * 2))

# Unsafe operations
unsafe {
    let config = readFile("config.json")
    let response = http_get("https://api.example.com")
}
```

**Sandbox Example:**
```klang
sandbox UntrustedCode {
    allow: [],
    limits: { memory: 10MB, cpu_time: 100ms }
}

run in UntrustedCode {
    eval(userProvidedCode)  # Safe
}
```

---

## 10. Conclusion

This security architecture provides:

1. **Security-by-Default** through unsafe blocks
2. **Capability-Based Execution** for fine-grained control
3. **Sandbox-Native** support for isolation

The implementation is:
- ✅ Practical and implementable
- ✅ Backwards compatible
- ✅ Performance-conscious
- ✅ Well-integrated with existing architecture

**Next Steps:**
1. Review and approve design
2. Begin Phase 1 implementation
3. Iterate based on feedback
4. Test extensively
5. Document thoroughly

---

## Related Documents

- **[IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md)** - Complete implementation guide
- **[FILES_TO_MODIFY.md](FILES_TO_MODIFY.md)** - All files to create/modify  
- **[QUICK_REFERENCE.md](QUICK_REFERENCE.md)** - Syntax quick reference
- **[DESIGN_SUMMARY.md](DESIGN_SUMMARY.md)** - Executive summary
- **[README.md](README.md)** - Security features overview

---

**Document Version:** 1.0  
**Last Updated:** 2025  
**Status:** Ready for Implementation  
**Estimated Timeline:** 10 weeks  
**Breaking Changes:** None (v1.x), Yes (v2.0)
