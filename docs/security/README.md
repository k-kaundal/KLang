# KLang Security Features

## Overview

KLang implements a comprehensive security architecture with three major features:

1. **Security-by-Default** - Safe operations by default, unsafe operations require explicit opt-in
2. **Capability-Based Execution** - Fine-grained permission system for resource access
3. **Sandbox-Native** - First-class language support for sandboxing and isolation

## Quick Start

### Running Secure Code

```bash
# Run with default security (permissive in v1.x)
klang run script.kl

# Run in strict mode (requires unsafe blocks)
klang run --strict script.kl

# Grant specific capabilities
klang run --allow-read=/tmp --allow-net=api.example.com script.kl

# Run in legacy mode (v1.x behavior, everything allowed)
klang run --unsafe-all script.kl
```

### Writing Secure Code

```klang
# Safe operations work without restrictions
let x = 10
let data = [1, 2, 3, 4, 5]
println(data.map(x => x * 2))

# Unsafe operations require explicit unsafe block
unsafe {
    let content = readFile("config.json")
    let response = http_get("https://api.example.com/data")
    println(content)
}
```

## Documentation

- **[SECURITY_ARCHITECTURE.md](SECURITY_ARCHITECTURE.md)** - Complete security architecture design
- **[IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md)** - Detailed implementation plan and file changes
- **[FILES_TO_MODIFY.md](FILES_TO_MODIFY.md)** - Summary of all files that need creation or modification

## Features

### 1. Security-by-Default (Unsafe Blocks)

**What it is:**
- Dangerous operations require explicit `unsafe { ... }` blocks
- Safe operations work without restrictions
- Compile-time and runtime enforcement

**Why it matters:**
- Prevents accidental security vulnerabilities
- Makes security explicit in code
- Easy to audit security-sensitive code

**Example:**
```klang
# This fails in strict mode
let data = readFile("secret.txt")  # ERROR: requires unsafe

# This works
unsafe {
    let data = readFile("secret.txt")  # OK
}
```

**Unsafe Operations:**
- File I/O (`fopen`, `readFile`, `writeFile`, etc.)
- Network (`http_get`, `http_post`)
- Environment variables (`env_get`, `env_set`)
- Manual memory (`malloc`, `free`)
- Raw pointers (`&`, `*`, `->`)

### 2. Capability-Based Execution

**What it is:**
- Permission system for resource access
- Capabilities granted via CLI flags or manifest
- Fine-grained control over what code can access

**Why it matters:**
- Least privilege principle
- Prevents unauthorized access
- Clear permission model

**Example:**
```bash
# Grant file read to /tmp only
klang run --allow-read=/tmp script.kl

# Grant network access to specific domain
klang run --allow-net=api.example.com script.kl

# Multiple capabilities
klang run --allow-read --allow-net --allow-env script.kl
```

**Capability Types:**
- `CAP_FILE_READ` - Read files
- `CAP_FILE_WRITE` - Write files  
- `CAP_FILE_DELETE` - Delete files
- `CAP_NETWORK_CONNECT` - Network connections
- `CAP_NETWORK_LISTEN` - Network listen
- `CAP_ENV_READ` - Read environment variables
- `CAP_ENV_WRITE` - Write environment variables
- `CAP_PROCESS_SPAWN` - Spawn processes
- `CAP_FFI_CALL` - Call native code
- `CAP_UNSAFE_MEM` - Manual memory management

### 3. Sandbox-Native

**What it is:**
- Built-in language construct for sandboxing
- Resource limits and isolation
- Pre-defined security profiles

**Why it matters:**
- Run untrusted code safely
- Enforce resource limits
- Prevent sandbox escapes

**Example:**
```klang
# Define a sandbox
sandbox RestrictedEnv {
    allow: ["file-read:/tmp/*"],
    limits: {
        memory: 100MB,
        cpu_time: 1000ms,
        max_files: 10
    },
    deny_list: ["/etc/*", "/root/*"]
}

# Run code in sandbox
run in RestrictedEnv {
    unsafe {
        let data = readFile("/tmp/data.txt")  # OK
        let secret = readFile("/etc/passwd")  # DENIED
    }
}
```

**Sandbox Profiles:**
- `sandbox:strict` - Maximum security, minimal resources
- `sandbox:compute` - Pure computation, no I/O
- `sandbox:network` - Network access only
- `sandbox:filesystem` - File access only
- `sandbox:untrusted` - For untrusted code

## Security Layers

KLang implements defense in depth with multiple security layers:

```
┌──────────────────────────────────────────────┐
│         Application Code                     │
├──────────────────────────────────────────────┤
│  Unsafe Blocks (syntax-level security)       │
├──────────────────────────────────────────────┤
│  Capability System (permission checks)       │
├──────────────────────────────────────────────┤
│  Sandbox Engine (resource limits)            │
├──────────────────────────────────────────────┤
│  Runtime Security Context (enforcement)      │
├──────────────────────────────────────────────┤
│  Memory Safety (GC, bounds checking)         │
└──────────────────────────────────────────────┘
```

## Current Status

| Feature | Status | Phase |
|---------|--------|-------|
| Architecture Design | ✅ Complete | - |
| Implementation Plan | ✅ Complete | - |
| Safety Context | 🚧 In Progress | Phase 1 |
| Capability System | 🚧 In Progress | Phase 1 |
| Unsafe Block Parsing | ⏳ Planned | Phase 2 |
| Unsafe Block Runtime | ⏳ Planned | Phase 2 |
| Sandbox Engine | ⏳ Planned | Phase 3 |
| Sandbox Syntax | ⏳ Planned | Phase 3 |
| Testing | ⏳ Planned | Phase 4 |
| Documentation | ⏳ Planned | Phase 4 |
| VM Integration | ⏳ Planned | Phase 5 |
| LLVM Integration | ⏳ Planned | Phase 5 |

## Backwards Compatibility

### Version Strategy

KLang maintains backwards compatibility through versioned behavior:

**v1.x (Current):**
- Permissive by default (all operations allowed)
- Security features opt-in
- No breaking changes

**v2.0 (Future):**
- Strict by default (unsafe blocks required)
- Capabilities required for dangerous operations
- Migration tools provided

### Compatibility Flags

```bash
# Legacy mode (v1.x behavior)
klang run --legacy script.kl
klang run --unsafe-all script.kl

# Strict mode (v2.0 behavior)
klang run --strict script.kl

# Warning mode (transition)
klang run --warn-unsafe script.kl
```

### Migration Path

1. **v1.6** - Add security features, opt-in only
2. **v1.7** - Warn about unsafe operations
3. **v1.8** - Provide migration tools
4. **v2.0** - Strict by default

## Performance

Target performance overhead:

| Operation | Overhead | Target |
|-----------|----------|--------|
| Safe operations | 0% | No overhead |
| Unsafe operations | <5% | Minimal overhead |
| Capability checks | <100ns | Fast checks |
| Sandboxed code | <10% | Acceptable overhead |

Optimization strategies:
- Capability caching
- Static analysis
- Lazy initialization
- Zero-cost abstractions in release builds

## Examples

See the `examples/security/` directory for complete examples:

- `unsafe_example.kl` - Unsafe block examples
- `capability_example.kl` - Capability system examples
- `sandbox_example.kl` - Sandbox examples
- `advanced_security.kl` - Combining all features

## Contributing

To contribute to KLang security features:

1. Read the architecture and implementation plan
2. Follow the coding standards
3. Write tests for all security features
4. Update documentation
5. Submit PR with security review

## Security Contact

To report security vulnerabilities:
- **Do NOT** open a public issue
- Email: [INSERT SECURITY EMAIL]
- Include detailed description and reproduction steps

## References

- [SECURITY_ARCHITECTURE.md](SECURITY_ARCHITECTURE.md) - Complete architecture
- [IMPLEMENTATION_PLAN.md](IMPLEMENTATION_PLAN.md) - Implementation details
- [../../SECURITY.md](../../SECURITY.md) - General security policy
- [Deno Security](https://deno.land/manual/getting_started/permissions) - Inspiration for capability model
- [Rust Unsafe](https://doc.rust-lang.org/book/ch19-01-unsafe-rust.html) - Inspiration for unsafe blocks

---

**Last Updated:** 2025  
**Status:** Design Phase  
**Version:** 1.0
