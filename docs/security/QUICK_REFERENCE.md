# Security Quick Reference

## Command-Line Flags

### Security Modes

```bash
# Strict mode (requires unsafe blocks)
klang run --strict script.kl

# Warn about unsafe operations but allow them
klang run --warn-unsafe script.kl

# Legacy/permissive mode (v1.x default)
klang run --legacy script.kl
klang run --unsafe-all script.kl
```

### Capability Flags

```bash
# File operations
klang run --allow-read script.kl                    # Read any file
klang run --allow-read=/tmp script.kl               # Read /tmp only
klang run --allow-read=/tmp,/home script.kl        # Multiple paths
klang run --allow-read=/tmp/*.json script.kl       # Pattern matching

klang run --allow-write script.kl                   # Write any file
klang run --allow-write=/tmp script.kl              # Write /tmp only

klang run --allow-delete script.kl                  # Delete any file
klang run --allow-delete=/tmp script.kl             # Delete /tmp only

# Network operations
klang run --allow-net script.kl                     # Any network access
klang run --allow-net=api.example.com script.kl    # Specific domain
klang run --allow-net=*.example.com script.kl      # Wildcard domain
klang run --allow-net=:8080 script.kl              # Specific port

klang run --allow-listen script.kl                  # Listen on any port
klang run --allow-listen=8080 script.kl            # Listen on port 8080

# Environment variables
klang run --allow-env script.kl                     # Read any env var
klang run --allow-env=PATH,HOME script.kl          # Specific vars

klang run --allow-env-write script.kl               # Write any env var
klang run --allow-env-write=DEBUG script.kl        # Specific vars

# Process operations  
klang run --allow-run script.kl                     # Spawn processes

# Manual memory management
klang run --allow-unsafe-mem script.kl              # malloc/free

# FFI (future)
klang run --allow-ffi script.kl                     # Native code calls

# All capabilities
klang run --allow-all script.kl                     # Grant everything
```

### Combined Flags

```bash
# Multiple capabilities
klang run --allow-read --allow-net --allow-env script.kl

# With patterns
klang run --allow-read=/tmp --allow-net=api.example.com script.kl

# Strict mode with capabilities
klang run --strict --allow-read=/data --allow-write=/output script.kl
```

---

## Syntax Quick Reference

### Unsafe Blocks

```klang
# Basic unsafe block
unsafe {
    let data = readFile("file.txt")
}

# Nested unsafe blocks
unsafe {
    let file1 = readFile("data1.txt")
    
    unsafe {
        let file2 = readFile("data2.txt")
    }
}

# Unsafe in function
function loadData(filename) {
    unsafe {
        return readFile(filename)
    }
}

# Multiple unsafe operations
unsafe {
    let data = readFile("input.txt")
    let response = http_get("https://api.example.com/data")
    writeFile("output.txt", data + response)
}
```

### Sandbox Syntax

```klang
# Define a sandbox
sandbox RestrictedEnv {
    allow: [
        "file-read:/tmp/*",
        "network-connect:api.example.com"
    ],
    limits: {
        memory: 100MB,
        cpu_time: 1000ms,
        max_files: 10,
        max_network_connections: 5
    },
    deny_list: [
        "/etc/*",
        "/root/*"
    ]
}

# Run code in sandbox
run in RestrictedEnv {
    unsafe {
        let data = readFile("/tmp/data.txt")
    }
}

# Inline sandbox
sandbox {
    allow: ["file-read:/tmp/*"],
    limits: { memory: 50MB }
    
    unsafe {
        processData()
    }
}

# Sandbox profiles
run in sandbox:strict {
    # Maximum security
}

run in sandbox:compute {
    # Pure computation, no I/O
}

run in sandbox:untrusted {
    # For untrusted code
}

# Nested sandboxes
run in OuterSandbox {
    # Outer permissions
    
    run in InnerSandbox {
        # More restrictive
    }
}
```

---

## Operations Requiring Unsafe Blocks

### File I/O
- `fopen(filename, mode)` - Open file
- `fclose(file)` - Close file
- `fread(file)` - Read from file
- `freadline(file)` - Read line from file
- `fwrite(file, data)` - Write to file
- `readFile(filename)` - Read entire file
- `writeFile(filename, data)` - Write entire file
- `appendFile(filename, data)` - Append to file
- `fdelete(filename)` - Delete file
- `frename(old, new)` - Rename file
- `fexists(filename)` - Check if file exists
- `fsize(filename)` - Get file size

### Network
- `http_get(url)` - HTTP GET request
- `http_post(url, data)` - HTTP POST request
- `http_request(options)` - Generic HTTP request

### Environment
- `env_get(name)` - Read environment variable
- `env_set(name, value)` - Write environment variable
- `env_has(name)` - Check if env var exists

### Memory (Manual)
- `malloc(size)` - Allocate memory
- `free(ptr)` - Free memory
- `realloc(ptr, size)` - Reallocate memory
- `calloc(count, size)` - Allocate and zero memory

### Pointers (C-style)
- `&variable` - Address-of operator
- `*pointer` - Dereference operator
- `pointer->field` - Pointer member access

### Process (Future)
- `exec(command)` - Execute command
- `spawn(program, args)` - Spawn process
- `system(command)` - Run system command

### FFI (Future)
- `dlopen(library)` - Load native library
- `dlsym(handle, symbol)` - Get symbol
- `call_native(func, args)` - Call native function

---

## Capability Types Reference

| Type | CLI Flag | Pattern Example | Description |
|------|----------|-----------------|-------------|
| `CAP_FILE_READ` | `--allow-read` | `/tmp/*` | Read files |
| `CAP_FILE_WRITE` | `--allow-write` | `/tmp/*.txt` | Write files |
| `CAP_FILE_DELETE` | `--allow-delete` | `/tmp/*` | Delete files |
| `CAP_NETWORK_CONNECT` | `--allow-net` | `api.example.com` | Network connections |
| `CAP_NETWORK_LISTEN` | `--allow-listen` | `:8080` | Listen on ports |
| `CAP_ENV_READ` | `--allow-env` | `PATH,HOME` | Read env vars |
| `CAP_ENV_WRITE` | `--allow-env-write` | `DEBUG` | Write env vars |
| `CAP_PROCESS_SPAWN` | `--allow-run` | `*` | Spawn processes |
| `CAP_FFI_CALL` | `--allow-ffi` | `*.so` | Call native code |
| `CAP_UNSAFE_MEM` | `--allow-unsafe-mem` | `*` | Manual memory |
| `CAP_ALL` | `--allow-all` | `*` | All capabilities |

---

## Sandbox Profiles

| Profile | Memory | CPU Time | I/O | Network | Use Case |
|---------|--------|----------|-----|---------|----------|
| `sandbox:strict` | 10MB | 1s | None | None | Maximum security |
| `sandbox:compute` | 500MB | No limit | None | None | Pure computation |
| `sandbox:network` | 100MB | No limit | None | Yes | Network-only |
| `sandbox:filesystem` | 100MB | No limit | Yes | None | File-only |
| `sandbox:untrusted` | 5MB | 100ms | None | None | Untrusted code |

---

## Pattern Matching Examples

### File Paths

```bash
# Exact match
--allow-read=/tmp/data.txt

# Directory
--allow-read=/tmp

# Recursive directory  
--allow-read=/tmp/*

# File extension
--allow-read=*.json
--allow-read=/data/*.csv

# Multiple patterns (comma-separated)
--allow-read=/tmp/*,/data/*,*.json
```

### Network Domains

```bash
# Exact domain
--allow-net=api.example.com

# Subdomain wildcard
--allow-net=*.example.com

# Port
--allow-net=:8080
--allow-net=localhost:3000

# Domain with port
--allow-net=api.example.com:443

# Any domain
--allow-net=*
```

### Environment Variables

```bash
# Single variable
--allow-env=PATH

# Multiple variables
--allow-env=PATH,HOME,USER

# Pattern (future)
--allow-env=DEBUG_*

# All variables
--allow-env=*
```

---

## Error Messages

### Missing Unsafe Block

```
Error: Operation 'readFile' requires unsafe context.
Wrap in 'unsafe { ... }' block or run with --unsafe-all
```

**Solution:**
```klang
# Before (error)
let data = readFile("file.txt")

# After (fixed)
unsafe {
    let data = readFile("file.txt")
}
```

### Missing Capability

```
Error: Missing capability: file-read for resource '/data/file.txt'.
Grant with --allow-read or --allow-all
```

**Solution:**
```bash
# Run with capability
klang run --allow-read=/data script.kl
```

### Sandbox Limit Exceeded

```
Error: Sandbox memory limit exceeded: 105MB / 100MB
```

**Solution:**
```klang
# Increase limit
sandbox MyEnv {
    limits: { memory: 200MB }  # Increased
}
```

### Sandbox Access Denied

```
Error: Sandbox access denied: '/etc/passwd' matches deny pattern '/etc/*'
```

**Solution:**
```klang
# Remove from deny list or change file path
```

---

## Migration Guide

### From v1.x to v2.0

**Step 1: Analyze**
```bash
klang analyze --security script.kl
```

**Step 2: Auto-migrate**
```bash
klang migrate --add-unsafe script.kl
```

**Step 3: Test**
```bash
# Test in strict mode
klang run --strict script.kl
```

**Step 4: Update**
- Add `unsafe { }` blocks manually where needed
- Add capability requirements to documentation
- Update deployment scripts with `--allow-*` flags

---

## Best Practices

### 1. Minimize Unsafe Blocks

```klang
# Bad: Large unsafe block
unsafe {
    let data = readFile("input.txt")
    let processed = processData(data)  # Safe operation
    let result = transformData(processed)  # Safe operation
    writeFile("output.txt", result)
}

# Good: Small unsafe blocks
unsafe {
    let data = readFile("input.txt")
}

let processed = processData(data)
let result = transformData(processed)

unsafe {
    writeFile("output.txt", result)
}
```

### 2. Use Least Privilege

```bash
# Bad: Grant all capabilities
klang run --allow-all script.kl

# Good: Grant only what's needed
klang run --allow-read=/data --allow-write=/output script.kl
```

### 3. Use Sandboxes for Untrusted Code

```klang
# Bad: Run untrusted code directly
eval(userCode)

# Good: Run in sandbox
run in sandbox:untrusted {
    eval(userCode)
}
```

### 4. Document Required Capabilities

```klang
#!/usr/bin/env klang
# Required capabilities:
#   --allow-read=/data
#   --allow-net=api.example.com
#   --allow-write=/output

# Your code here
```

### 5. Use Manifest Files for Complex Projects

**kl.toml:**
```toml
[security]
capabilities = [
    { type = "file-read", pattern = "/data/*" },
    { type = "file-write", pattern = "/output/*" },
    { type = "network-connect", pattern = "api.example.com" }
]
```

---

## Debugging Security Issues

### Enable Audit Logging

```bash
klang run --audit-log=security.log script.kl
```

### Verbose Security Messages

```bash
klang run --security-verbose script.kl
```

### Check Granted Capabilities

```klang
# Print current capabilities
println(Security.capabilities())
```

### Test in Permissive Mode First

```bash
# Development: permissive
klang run --unsafe-all script.kl

# Testing: warnings
klang run --warn-unsafe script.kl

# Production: strict
klang run --strict script.kl
```

---

**Last Updated:** 2025  
**Status:** Reference Guide
