# KLang CLI Quick Reference

## 📚 Complete Command Reference

### Core Commands

#### `klang` (default)
Starts the REPL when no command is provided.
```bash
klang                    # Start REPL
```

#### `klang run`
Execute a KLang file.
```bash
klang run <file>         # Run a file
klang run -w <file>      # Watch and auto-run on changes
klang run -t <file>      # Show execution time
klang run --help         # Show help
```

**Options:**
- `-w, --watch` - Watch mode (auto-rerun on file changes)
- `-t, --timing` - Show execution timing
- `-d, --debug` - Enable debug output
- `-e, --env VAR=VAL` - Set environment variable

**Examples:**
```bash
klang run app.kl
klang run --watch app.kl
klang run --timing app.kl
klang run --env DEBUG=1 app.kl
```

---

#### `klang build`
Compile a KLang file (VM compilation).
```bash
klang build <file>       # Build file
klang build --help       # Show help
```

**Options:**
- `-o, --output <file>` - Output file name
- `-O, --optimize` - Enable optimizations
- `--emit-asm` - Emit assembly
- `--emit-bytecode` - Emit bytecode

**Examples:**
```bash
klang build app.kl
klang build -o app.out app.kl
klang build --optimize app.kl
```

---

#### `klang repl`
Start the interactive REPL (Read-Eval-Print Loop).
```bash
klang repl               # Start REPL
klang repl --help        # Show help
```

**REPL Commands:**
```
> .help          Show REPL help
> .exit          Exit REPL (or Ctrl+D)
> .quit          Exit REPL (alias)
> .load <file>   Load and execute file
> .save <file>   Save session to file
> .clear         Clear screen
> .env           Show environment variables
> .reset         Reset interpreter state
> .type <var>    Show type of variable
> .ast <expr>    Show AST of expression
```

**Examples:**
```bash
klang repl
```

In REPL:
```klang
> let x = 10
> x + 5
15 (Int)

> fn greet(name) { return "Hello, " + name }
> greet("World")
"Hello, World" (String)

> .type greet
Function (String) -> String

> .exit
```

---

### Development Commands (Proposed)

#### `klang check`
Check syntax and types without running.
```bash
klang check <file>       # Check a file
klang check --all .      # Check all files in directory
klang check -w <file>    # Watch mode
klang check --help       # Show help
```

**Options:**
- `-a, --all` - Check all .kl files in directory (recursive)
- `-w, --watch` - Watch mode (recheck on changes)
- `-V, --verbose` - Verbose output
- `--strict` - Strict mode (treat warnings as errors)

**Examples:**
```bash
klang check app.kl
klang check --all src/
klang check --watch app.kl
```

**Output:**
```
✓ app.kl - OK (parsed in 12ms)
✗ broken.kl - Error
  Line 5: Unexpected token '}'
  Line 8: Undefined variable 'x'
```

---

#### `klang fmt`
Format KLang code automatically.
```bash
klang fmt <file>         # Format a file
klang fmt <dir>          # Format directory
klang fmt --check .      # Check if formatted
klang fmt --help         # Show help
```

**Options:**
- `-c, --check` - Check if formatted (exit 1 if not)
- `-w, --write` - Write changes (default: true)
- `-d, --diff` - Show diff without writing
- `-i, --indent <n>` - Indentation size (default: 4)
- `--max-line <n>` - Max line length (default: 100)

**Examples:**
```bash
klang fmt app.kl
klang fmt src/
klang fmt --check .
klang fmt --diff app.kl
klang fmt --indent 2 app.kl
```

**Output:**
```
✓ Formatted src/main.kl
✓ Formatted src/utils.kl
Formatted 2 files.
```

---

#### `klang test`
Run tests using built-in test framework.
```bash
klang test               # Run all tests
klang test <file>        # Run specific test file
klang test --help        # Show help
```

**Options:**
- `-w, --watch` - Watch mode (rerun on changes)
- `-c, --coverage` - Generate coverage report
- `-V, --verbose` - Verbose output
- `-f, --filter <pattern>` - Filter tests by name

**Test File Naming:**
- `*_test.kl` or `test_*.kl`

**Examples:**
```bash
klang test
klang test tests/math_test.kl
klang test --watch
klang test --coverage
klang test --filter "addition"
```

**Output:**
```
Running tests...
✓ test_addition (2ms)
✓ test_subtraction (1ms)
✗ test_division (5ms)
  Expected: 2, Got: 0

2 passed, 1 failed (8ms total)
```

---

#### `klang init`
Create a new KLang project.
```bash
klang init               # Interactive mode
klang init <name>        # Create project
klang init --lib <name>  # Library template
klang init --help        # Show help
```

**Options:**
- `--lib` - Library template (default: binary)
- `--bin` - Binary template (explicit)
- `--git` - Initialize git repository (default: true)
- `--no-git` - Don't initialize git

**Examples:**
```bash
klang init               # Interactive
klang init my-app        # Create binary project
klang init --lib my-lib  # Create library project
```

**Output:**
```
Creating new KLang project 'my-app'...

✓ Created directory structure
✓ Generated klang.toml
✓ Created src/main.kl
✓ Created README.md
✓ Initialized git repository

Project created successfully!

Get started with:
  cd my-app
  klang run src/main.kl
```

**Generated Structure:**
```
my-app/
├── klang.toml
├── README.md
├── .gitignore
├── src/
│   └── main.kl
├── tests/
│   └── main_test.kl
└── docs/
```

---

#### `klang doc`
Documentation tools.
```bash
klang doc <symbol>       # Show documentation
klang doc --generate     # Generate HTML docs
klang doc --help         # Show help
```

**Options:**
- `-g, --generate` - Generate documentation
- `-o, --output <dir>` - Output directory (default: docs/)
- `--open` - Open in browser after generation

**Examples:**
```bash
klang doc print
klang doc --generate
klang doc --generate --open
```

---

#### `klang info`
Show system and debug information.
```bash
klang info               # Show system info
klang info --ast <file>  # Show AST
klang info --help        # Show help
```

**Options:**
- `--ast <file>` - Show AST of file
- `--tokens <file>` - Show tokens (lexer output)
- `--bytecode <file>` - Show bytecode (compiler output)
- `--system` - Show system information

**Examples:**
```bash
klang info
klang info --ast app.kl
klang info --tokens app.kl
klang info --bytecode app.kl
```

**Output (system):**
```
KLang System Information:
  Version: 0.1.0
  Platform: Linux x86_64
  Build: a1b2c3d (2024-03-22)
  Runtime: Interpreter + VM
  GC: Mark-and-Sweep
  Stdlib: /usr/local/lib/klang/stdlib
```

---

#### `klang watch`
Watch files and auto-run on changes.
```bash
klang watch <file>       # Watch and run
klang watch --check      # Watch and check only
klang watch --help       # Show help
```

**Options:**
- `-c, --check` - Check only (don't run)
- `--clear` - Clear screen between runs (default: true)
- `--debounce <ms>` - Debounce time (default: 300ms)

**Examples:**
```bash
klang watch app.kl
klang watch --check app.kl
```

**Output:**
```
Watching app.kl for changes...

[12:34:56] Change detected, running...
[12:34:56] Output:
Hello, World!
[12:34:56] Completed in 15ms

Waiting for changes... (Ctrl+C to stop)
```

---

#### `klang bench`
Benchmark execution performance.
```bash
klang bench <file>       # Benchmark file
klang bench --help       # Show help
```

**Options:**
- `-n, --iterations <n>` - Number of iterations (default: 100)
- `-w, --warmup <n>` - Warmup iterations (default: 10)
- `--compare <file>` - Compare with another file

**Examples:**
```bash
klang bench app.kl
klang bench --iterations 1000 app.kl
klang bench --compare old.kl new.kl
```

**Output:**
```
Benchmarking app.kl (100 iterations)...

Warmup: 10 iterations (150ms)
Running: 100 iterations...

Results:
  Mean:   15.2ms
  Median: 15.0ms
  Min:    14.5ms
  Max:    17.8ms
  StdDev: 0.8ms

Throughput: 65.79 runs/sec
```

---

### Utility Commands

#### `klang version`
Show version information.
```bash
klang version            # Show version
klang -v                 # Short flag
klang --version          # Long flag
```

**Output:**
```
KLang 0.1.0
Commit: a1b2c3d (2024-03-22)
Built: 2024-03-22 14:30:00 UTC
Platform: Linux x86_64 (GNU/Linux)
Features: GC, REPL, VM
```

---

#### `klang help`
Show help information.
```bash
klang help               # General help
klang help <command>     # Command help
klang -h                 # Short flag
klang --help             # Long flag
```

**Examples:**
```bash
klang help
klang help run
klang run --help
```

---

## 🎨 Global Options

These options work with any command:

```bash
-h, --help               Show help
-v, --version            Show version
-V, --verbose            Verbose output
-q, --quiet              Quiet mode (minimal output)
--no-color               Disable colored output
--config <file>          Use specific config file
```

**Examples:**
```bash
klang run --verbose app.kl
klang check --quiet --all .
klang fmt --no-color app.kl
klang run --config custom.toml app.kl
```

---

## ⚙️ Configuration

### Config File (`klang.toml`)

Located at:
1. `./klang.toml` (project config)
2. `~/.klang/config.toml` (user config)
3. `/etc/klang/config.toml` (system config)

**Example:**
```toml
[project]
name = "my-app"
version = "0.1.0"
authors = ["Your Name <you@example.com>"]

[build]
target = "release"
optimize = true
warnings_as_errors = false

[format]
indent = 4
max_line_length = 100
trailing_newline = true

[repl]
history_size = 1000
auto_print = true
colors = true

[paths]
stdlib = "/usr/local/lib/klang/stdlib"
```

---

### Environment Variables

```bash
KLANG_PATH              # Search path for modules
KLANG_CONFIG            # Config file location
KLANG_NO_COLOR          # Disable colors (1 to enable)
KLANG_VERBOSE           # Verbose mode (1 to enable)
KLANG_STDLIB            # Standard library path
```

**Examples:**
```bash
export KLANG_NO_COLOR=1
export KLANG_STDLIB=/custom/path/stdlib
export KLANG_VERBOSE=1
klang run app.kl
```

---

## 🔍 Exit Codes

```
0   Success
1   General error
2   Command not found
3   Invalid arguments
4   Syntax error in source
5   Runtime error
6   File not found
7   Permission denied
```

---

## 💡 Tips & Tricks

### 1. Quick REPL Start
```bash
alias kr='klang repl'
```

### 2. Watch and Run
```bash
klang watch app.kl
# Or
klang run --watch app.kl
```

### 3. Check All Files
```bash
klang check --all src/
```

### 4. Format on Save (VS Code)
```json
{
  "emeraldwalk.runonsave": {
    "commands": [
      {
        "match": "\\.kl$",
        "cmd": "klang fmt ${file}"
      }
    ]
  }
}
```

### 5. Shell Completion
```bash
# Add to ~/.bashrc or ~/.zshrc
eval "$(klang completion bash)"  # or zsh, fish
```

### 6. Debug Mode
```bash
klang run --debug --verbose app.kl
```

### 7. Timing Everything
```bash
alias klangt='klang run --timing'
klangt app.kl
```

---

## 📝 Common Workflows

### Development Workflow
```bash
# 1. Create project
klang init my-app
cd my-app

# 2. Edit code
vim src/main.kl

# 3. Check syntax
klang check src/main.kl

# 4. Run with watch
klang watch src/main.kl

# 5. Format code
klang fmt src/

# 6. Run tests
klang test

# 7. Build release
klang build --optimize src/main.kl
```

### Testing Workflow
```bash
# Run all tests
klang test

# Run specific test
klang test tests/math_test.kl

# Watch mode
klang test --watch

# With coverage
klang test --coverage
```

### CI/CD Workflow
```bash
# Check formatting
klang fmt --check .

# Check syntax
klang check --all .

# Run tests
klang test

# Build
klang build --optimize src/main.kl
```

---

## 🐛 Troubleshooting

### Problem: Command not found
```bash
$ klang: command not found
```
**Solution:**
```bash
# Add to PATH
export PATH=$PATH:/usr/local/bin

# Or install globally
sudo make install
```

### Problem: Colors not working
```bash
# Check if TTY
tty

# Force enable colors
unset NO_COLOR
unset KLANG_NO_COLOR
```

### Problem: REPL history not saving
```bash
# Check permissions
ls -la ~/.klang_history

# Fix permissions
chmod 644 ~/.klang_history
```

### Problem: Slow check/format on large projects
```bash
# Use parallel processing (future feature)
klang check --parallel --all .
```

---

## 📚 Further Reading

- [CLI Enhancement Plan](./CLI_ENHANCEMENT_PLAN.md) - Full feature plan
- [CLI Implementation Guide](./CLI_IMPLEMENTATION_GUIDE.md) - Technical details
- [Getting Started](./GETTING_STARTED.md) - Beginner guide
- [Language Reference](./LANGUAGE_REFERENCE.md) - Language documentation

---

**Last Updated:** 2024-03-22  
**Version:** 1.0
