# KLang API Reference

Complete API reference for the KLang programming language.

## Table of Contents
- [CLI Commands](#cli-commands)
- [Built-in Functions](#built-in-functions)
- [Standard Library](#standard-library)
- [Configuration](#configuration)

---

## CLI Commands

### Execution Commands

#### `klang run <file>`
Execute a KLang source file using the interpreter.

**Usage:**
```bash
klang run script.kl
```

**Options:** None

---

#### `klang build <file>`
Compile to bytecode and execute via VM.

**Usage:**
```bash
klang build script.kl
```

**Options:** None

---

#### `klang compile <file>`
Compile to native executable using LLVM.

**Usage:**
```bash
klang compile [OPTIONS] <file>
```

**Options:**
- `--mode=<mode>` - Build mode: `debug`, `release`, `dev`, `prod` (default: release)
- `--opt=<level>` - Optimization level: `0`, `1`, `2`, `3` (default: 2)
- `-O<level>` - Short form for optimization level
- `--lto` - Enable Link-Time Optimization
- `--no-lto` - Disable Link-Time Optimization
- `--strip` - Strip debug symbols

**Examples:**
```bash
klang compile app.kl                    # Standard compilation
klang compile --mode=release app.kl     # Release mode
klang compile -O3 --lto app.kl          # Maximum optimization
klang compile --mode=debug app.kl       # Debug mode with symbols
```

---

### Development Commands

#### `klang check <file>`
Check syntax without executing.

**Usage:**
```bash
klang check script.kl
```

---

#### `klang typecheck <file>`
Perform static type checking.

**Usage:**
```bash
klang typecheck [OPTIONS] <file>
```

**Options:**
- `--strict` - Enable strict type checking

**Examples:**
```bash
klang typecheck app.kl          # Standard type check
klang typecheck --strict app.kl # Strict mode
```

---

#### `klang fmt <file>`
Format source code.

**Usage:**
```bash
klang fmt [OPTIONS] <file>
```

**Options:**
- `--check` - Check if formatting is needed (don't modify)
- `--indent=<N>` - Set indent size (1-8, default: 4)
- `--tabs` - Use tabs instead of spaces

**Examples:**
```bash
klang fmt script.kl              # Format with defaults
klang fmt --check script.kl      # Dry run
klang fmt --indent=2 script.kl   # 2-space indent
klang fmt --tabs script.kl       # Use tabs
```

---

#### `klang info <file>`
Display file information and statistics.

**Usage:**
```bash
klang info script.kl
```

**Output includes:**
- File size
- Line count
- Character count
- Function count
- Complexity metrics

---

#### `klang test <path>`
Run tests from file or directory.

**Usage:**
```bash
klang test <file|directory>
```

**Examples:**
```bash
klang test tests/              # Run all tests in directory
klang test tests/foo_test.kl   # Run specific test
```

---

### Project Commands

#### `klang init <name>`
Initialize a new project.

**Usage:**
```bash
klang init <project-name> [OPTIONS]
```

**Options:**
- `--git` - Initialize git repository

**Examples:**
```bash
klang init my-project           # Create project
klang init my-project --git     # Create with git
```

**Creates:**
```
my-project/
├── src/
│   └── main.kl
├── tests/
├── docs/
├── package.kl.json
└── README.md
```

---

#### `klang pkg <command>`
Package manager commands.

**Commands:**
- `install`, `add` - Install package
- `uninstall`, `remove` - Remove package
- `update` - Update package(s)
- `list`, `ls` - List installed packages
- `init` - Create package manifest

**Usage:**
```bash
klang pkg install <package[@version]>
klang pkg uninstall <package>
klang pkg update [package]
klang pkg list
klang pkg init <name>
```

**Examples:**
```bash
klang pkg install lodash          # Install latest
klang pkg install lodash@4.17.21  # Specific version
klang pkg update                  # Update all
klang pkg list                    # Show installed
```

---

### Tool Commands

#### `klang lsp`
Start Language Server Protocol server.

**Usage:**
```bash
klang lsp
```

**Features:**
- Syntax highlighting
- Code completion
- Go to definition
- Error diagnostics
- Code formatting
- Hover information

---

#### `klang serve`
Start HTTP development server.

**Usage:**
```bash
klang serve [OPTIONS]
```

**Options:**
- `--port=<PORT>`, `-p <PORT>` - Server port (default: 8080)
- `--dir=<DIR>` - Directory to serve (default: current)
- `--hot-reload`, `--watch` - Enable hot-reload
- `--mode=<MODE>` - Build mode (auto hot-reload in dev)

**Examples:**
```bash
klang serve                       # Serve on 8080
klang serve --port=3000           # Custom port
klang serve --hot-reload          # With hot-reload
klang serve --mode=dev --dir=dist # Dev mode, serve dist/
```

---

#### `klang repl`
Start interactive REPL.

**Usage:**
```bash
klang repl
```

**REPL Commands:**
- `exit`, `quit` - Exit REPL
- `exit(n)` - Exit with code n

---

### Information Commands

#### `klang version`
Show version information.

**Usage:**
```bash
klang version
klang -v
klang --version
```

---

#### `klang config`
Show current configuration.

**Usage:**
```bash
klang config
```

**Shows:**
- Build settings
- Environment variables
- Default paths
- Feature flags

---

#### `klang help [command]`
Show help information.

**Usage:**
```bash
klang help              # General help
klang help <command>    # Command-specific help
klang -h
klang --help
```

---

## Built-in Functions

### Output Functions

#### `println(...values)`
Print values to stdout with newline.
```klang
println("Hello")
println("x =", x)
```

#### `print(...values)`
Print values to stdout without newline.
```klang
print("Loading")
print("...")
```

---

### Type Conversion

#### `str(value)`
Convert value to string.
```klang
let x = 42
let s = str(x)  # "42"
```

#### `int(value)`
Convert value to integer.
```klang
let x = int("42")  # 42
```

#### `float(value)`
Convert value to float.
```klang
let x = float("3.14")  # 3.14
```

#### `bool(value)`
Convert value to boolean.
```klang
let x = bool(1)  # true
```

---

### Array Methods

#### `array.push(value)`
Add element to end.
```klang
let arr = [1, 2, 3]
arr.push(4)  # [1, 2, 3, 4]
```

#### `array.pop()`
Remove and return last element.
```klang
let arr = [1, 2, 3]
let x = arr.pop()  # x = 3, arr = [1, 2]
```

#### `array.map(fn)`
Transform each element.
```klang
let nums = [1, 2, 3]
let doubled = nums.map((n) => n * 2)  # [2, 4, 6]
```

#### `array.filter(fn)`
Filter elements by predicate.
```klang
let nums = [1, 2, 3, 4, 5]
let evens = nums.filter((n) => n % 2 == 0)  # [2, 4]
```

#### `array.reduce(fn, initial)`
Reduce to single value.
```klang
let nums = [1, 2, 3, 4]
let sum = nums.reduce((acc, n) => acc + n, 0)  # 10
```

---

### String Methods

#### `string.length`
Get string length.
```klang
let s = "hello"
let len = s.length  # 5
```

#### `string.toUpperCase()`
Convert to uppercase.
```klang
let s = "hello"
let upper = s.toUpperCase()  # "HELLO"
```

#### `string.toLowerCase()`
Convert to lowercase.
```klang
let s = "HELLO"
let lower = s.toLowerCase()  # "hello"
```

---

## Configuration

### Environment Variables

#### `KLANG_NO_COLOR`
Disable colored output.
```bash
export KLANG_NO_COLOR=1
```

#### `KLANG_DEBUG`
Enable debug mode.
```bash
export KLANG_DEBUG=1
```

#### `KLANG_INDENT_SIZE`
Set default indent size (1-8).
```bash
export KLANG_INDENT_SIZE=2
```

#### `KLANG_USE_TABS`
Use tabs instead of spaces.
```bash
export KLANG_USE_TABS=1
```

#### `KLANG_HISTORY_SIZE`
REPL history size (0-10000).
```bash
export KLANG_HISTORY_SIZE=1000
```

#### `KLANG_PATH`
Module search paths (colon-separated).
```bash
export KLANG_PATH="/usr/local/lib/klang:$HOME/.klang/modules"
```

#### `KLANG_MAX_LINE_LENGTH`
Maximum line length for formatter.
```bash
export KLANG_MAX_LINE_LENGTH=100
```

---

## File Extensions

KLang supports multiple file extensions:
- `.kl` - Standard KLang files (recommended)
- `.k` - Short form
- `.klang` - Explicit form

All extensions are treated identically by the compiler.

---

For more information, visit [GitHub](https://github.com/k-kaundal/KLang).
