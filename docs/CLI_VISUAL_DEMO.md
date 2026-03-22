# KLang Enhanced CLI - Visual Demonstration

## 🎨 Overview

The KLang CLI has been transformed from a basic 3-command interface into a professional, feature-rich development tool with beautiful colored output, comprehensive help, and powerful new commands.

---

## ✨ Key Features

### 1. Professional Help System

```bash
$ klang --help
```

**Output:**
```
╔═══════════════════════════════════════════════════════════╗
║                   KLang CLI v0.2.0                     ║
║        High-Performance Programming Language            ║
╚═══════════════════════════════════════════════════════════╝

USAGE:
    klang [OPTIONS] <COMMAND> [ARGS]

COMMANDS:
  run <file>       Execute a KLang source file (.kl, .k, .klang)
  build <file>     Compile and run via bytecode VM
  repl             Start interactive REPL mode
  check <file>     Check syntax without executing
  info <file>      Display file information and statistics
  version          Show version information
  help [command]   Show help for a command

OPTIONS:
  -h, --help       Show this help message
  -v, --version    Show version information
  --no-color       Disable colored output

EXAMPLES:
  klang run program.kl           # Run a program
  klang repl                     # Start REPL
  klang check script.k           # Check syntax
  klang info myfile.klang        # Show file info
  klang help run                 # Help for 'run' command

FILE EXTENSIONS:
  Supported: .kl, .k, .klang

For more information, visit: https://github.com/k-kaundal/KLang
```

---

### 2. Version Information

```bash
$ klang --version
```

**Output:**
```
KLang version 0.2.0
Build date: Mar 22 2026
A high-performance, statically-typed programming language
```

---

### 3. Command-Specific Help

```bash
$ klang help run
```

**Output:**
```
KLang Run Command

USAGE:
    klang run <file>

DESCRIPTION:
    Execute a KLang source file using the interpreter.
    The file must have a valid extension (.kl, .k, .klang).

EXAMPLES:
    klang run script.kl
    klang run examples/hello.k
    klang run /path/to/program.klang
```

---

### 4. Syntax Checker

```bash
$ klang check examples/stdlib_demo.kl
```

**Output:**
```
Checking: examples/stdlib_demo.kl
→ Running lexical analysis...
→ Running syntax analysis...
✓ Syntax check passed

Summary: No syntax errors found in examples/stdlib_demo.kl
  • 64 statement(s) parsed successfully
```

**Colors:**
- 🔵 Cyan for "Checking:" and arrows
- ✅ Green checkmark and success message
- 🔴 Red for errors (when present)

---

### 5. File Information & Statistics

```bash
$ klang info examples/stdlib_demo.kl
```

**Output:**
```
File Information: examples/stdlib_demo.kl

General:
  Path:         examples/stdlib_demo.kl
  Size:         2732 bytes
  Extension:    .kl

Statistics:
  Total lines:       164
  Code lines:        120
  Comment lines:     15
  Empty lines:       29
  Comment ratio:     9%
```

**Features:**
- Detailed file analysis
- Line count breakdown
- Comment ratio calculation
- Clean, organized output

---

### 6. Enhanced REPL

```bash
$ klang repl
```

**Output:**
```
╔═══════════════════════════════════════════════════════════╗
║              KLang REPL v0.2.0                       ║
║        Interactive Programming Environment              ║
╚═══════════════════════════════════════════════════════════╝

Tip: Type exit or quit to leave the REPL
Tip: Use exit(n) to exit with a specific code

> 5 + 3
=> 8
> let x = 10
> x * 2
=> 20
> exit

Goodbye!
```

**Colors:**
- 🔵 Cyan banner and tips
- 🟢 Green prompt (`>`)
- 🔵 Cyan result indicator (`=>`)
- 🔵 Cyan goodbye message

---

### 7. Improved Error Messages

#### Invalid File Extension
```bash
$ klang run script.txt
```

**Output:**
```
Error: Invalid file extension. KLang files must have .kl, .k, or .klang extension
Given file: script.txt
```

#### Unknown Command
```bash
$ klang foo
```

**Output:**
```
Error: Unknown command
Command: foo
Try 'klang --help' for available commands.
```

#### Missing Argument
```bash
$ klang check
```

**Output:**
```
Error: Missing file argument
Usage: klang check <file.kl>
Try 'klang help check' for more information.
```

**Colors:**
- 🔴 Bold red "Error:" label
- Clear error description
- Helpful suggestions in plain text

---

## 🎯 Command Summary

| Command | Description | Example |
|---------|-------------|---------|
| `klang run <file>` | Execute a KLang file | `klang run app.kl` |
| `klang build <file>` | Compile and run via VM | `klang build app.kl` |
| `klang repl` | Start interactive REPL | `klang repl` |
| `klang check <file>` | Check syntax only | `klang check app.kl` |
| `klang info <file>` | Show file statistics | `klang info app.kl` |
| `klang version` | Show version info | `klang version` |
| `klang help [cmd]` | Show help | `klang help run` |

---

## 🎨 Color Scheme

The CLI uses a professional color scheme:

- **Cyan** (`\033[36m`) - Headers, info, prompts
- **Green** (`\033[32m`) - Success messages, commands
- **Red** (`\033[31m`) - Errors
- **Yellow** (`\033[33m`) - Warnings, tips
- **Blue** (`\033[34m`) - Links
- **Bold** (`\033[1m`) - Emphasis

### Disable Colors

For scripts or environments that don't support colors:

```bash
$ klang --no-color --help
```

---

## 📊 Before vs After

### Before (v0.1.0)
```bash
$ klang
> 

# Basic REPL, no banner
# No help system
# No version command
# No syntax checker
# No file info
# Plain error messages
```

### After (v0.2.0)
```bash
$ klang
╔═══════════════════════════════════════════════════════════╗
║              KLang REPL v0.2.0                       ║
║        Interactive Programming Environment              ║
╚═══════════════════════════════════════════════════════════╝

# Professional banner
# Full help system
# Version info
# Syntax checker
# File statistics
# Colored, clear errors
```

---

## 🚀 Usage Examples

### Quick Start

```bash
# Get help
klang --help

# Check version
klang --version

# Start REPL
klang repl

# Run a program
klang run examples/hello.kl

# Check syntax before running
klang check examples/hello.kl

# Get file information
klang info examples/hello.kl
```

### Development Workflow

```bash
# 1. Check syntax during development
klang check myapp.kl

# 2. View statistics
klang info myapp.kl

# 3. Run the program
klang run myapp.kl

# 4. Or compile and run via VM for better performance
klang build myapp.kl
```

### Interactive Development

```bash
# Start REPL
klang repl

# Test expressions
> let fibonacci = fn(n) { 
    if n <= 1 { return n }
    return fibonacci(n-1) + fibonacci(n-2)
  }

> fibonacci(10)
=> 55

> exit
Goodbye!
```

---

## 💡 Tips & Tricks

### 1. Quick Syntax Check
```bash
# Check multiple files
for file in *.kl; do
  klang check "$file"
done
```

### 2. File Statistics
```bash
# Get stats for all files
for file in src/*.kl; do
  echo "=== $file ==="
  klang info "$file"
done
```

### 3. Get Command Help
```bash
# Help for any command
klang help run
klang help build
klang help check
klang help info
```

### 4. Disable Colors in Scripts
```bash
#!/bin/bash
# Use --no-color for scripting
klang --no-color check "$1" || exit 1
klang --no-color run "$1"
```

---

## 🎓 Learning Resources

After exploring the CLI, check out:

- **[Getting Started Guide](docs/GETTING_STARTED.md)** - Setup and basics
- **[Language Reference](docs/LANGUAGE_REFERENCE.md)** - Complete syntax
- **[Complete Documentation](docs/README.md)** - All documentation

---

## 🔜 Coming Soon

Phase 2 features in development:
- `klang fmt` - Code formatter
- `klang test` - Test runner
- `klang init` - Project scaffolding
- Command autocomplete
- REPL history
- Config file support

---

## 🤝 Contributing

Want to improve the CLI? See:
- [CLI Implementation Guide](docs/CLI_IMPLEMENTATION_GUIDE.md)
- [CLI Roadmap](docs/CLI_ROADMAP.md)
- [Contributing Guide](docs/development/CONTRIBUTING.md)

---

**The KLang CLI is now a professional, production-ready tool! 🎉**
