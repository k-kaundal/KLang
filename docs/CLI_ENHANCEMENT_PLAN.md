# KLang CLI Enhancement Plan

## 📋 Current State Analysis

### Existing Implementation (src/cli.c)
The current KLang CLI is **minimal** and only supports:
- `klang` - starts REPL (default)
- `klang repl` - starts REPL
- `klang run <file>` - runs a KLang file
- `klang build <file>` - compiles and executes via VM

**Issues with Current Implementation:**
1. ❌ No `--help` or `-h` flag
2. ❌ No `--version` or `-v` flag
3. ❌ No command suggestions for typos
4. ❌ Poor error messages (just "Unknown command")
5. ❌ No color output
6. ❌ No progress indicators for long operations
7. ❌ No config file support
8. ❌ No additional commands (check, format, test, etc.)
9. ❌ No argument parsing library (manual `strcmp`)
10. ❌ No interactive features in REPL (no history, no autocomplete)
11. ❌ No verbose/quiet modes
12. ❌ No file watching capabilities
13. ❌ No project scaffolding
14. ❌ No package management integration

---

## 🎯 Enhancement Goals

Transform KLang CLI into a **professional, feature-rich development tool** comparable to:
- `node` (Node.js)
- `python` (Python)
- `cargo` (Rust)
- `go` (Go)
- `deno` (Deno)

---

## 🚀 Proposed Features

### Phase 1: Core CLI Improvements (Essential)

#### 1.1 Help System
```bash
klang --help, -h        # Show general help
klang run --help        # Show command-specific help
klang help <command>    # Alternative help syntax
```

**Implementation:**
- Add comprehensive help text for all commands
- Include examples for each command
- Show available options and flags
- Format help output with colors and sections

#### 1.2 Version Information
```bash
klang --version, -v     # Show version
klang version           # Alternative syntax
```

**Output format:**
```
KLang v0.1.0
Runtime: Interpreter + VM
Build: <commit-hash> (<date>)
Platform: Linux x86_64
```

#### 1.3 Better Error Messages
- Show "Did you mean?" suggestions for typos
- Display available commands when unknown command is used
- Color-coded error messages (red for errors)
- Exit codes for different error types

Example:
```bash
$ klang rn test.kl
Error: Unknown command 'rn'
Did you mean 'run'?

Usage: klang <command> [options]
Run 'klang --help' for more information.
```

#### 1.4 Color Output Support
- Use ANSI escape codes for colors
- Detect TTY and disable colors for pipes
- Add `--no-color` flag to disable colors
- Color scheme:
  - Red: Errors
  - Yellow: Warnings
  - Green: Success messages
  - Blue: Info messages
  - Cyan: Prompts
  - Gray: Debug info

#### 1.5 Better Argument Parsing
Replace manual `strcmp` with a proper argument parser:
- Support long flags (`--flag`) and short flags (`-f`)
- Support flag values (`--output=file`, `-o file`)
- Support multiple flags (`-abc` = `-a -b -c`)
- Validate arguments before execution
- Generate help automatically from argument definitions

---

### Phase 2: Additional Commands (High Priority)

#### 2.1 Check Command (Syntax/Type Checking)
```bash
klang check <file>      # Check syntax without running
klang check --watch     # Watch mode for continuous checking
klang check --all       # Check all .kl files in directory
```

**Purpose:** Static analysis without execution
- Parse and validate syntax
- Type checking (if type system is expanded)
- Report errors without running code
- Faster feedback loop for development

#### 2.2 Format Command (Code Formatting)
```bash
klang fmt <file>        # Format a file
klang fmt <dir>         # Format all files in directory
klang fmt --check       # Check if formatted (CI mode)
klang fmt --write       # Write changes (default)
klang fmt --diff        # Show diff without writing
```

**Purpose:** Consistent code style (like `gofmt`, `rustfmt`)
- Automatic indentation
- Consistent spacing
- Line wrapping
- Configurable via `.klangfmt` file

#### 2.3 Test Command
```bash
klang test              # Run all tests
klang test <file>       # Run specific test file
klang test --watch      # Watch mode
klang test --coverage   # Generate coverage report
klang test --verbose    # Verbose output
```

**Purpose:** Built-in testing framework
- Discover test files (`*_test.kl`, `test_*.kl`)
- Run tests and report results
- Colorful output (✓ pass, ✗ fail)
- Test statistics

#### 2.4 Init Command (Project Scaffolding)
```bash
klang init              # Interactive project creation
klang init <name>       # Create project with name
klang init --lib        # Library template
klang init --bin        # Binary template
```

**Purpose:** Quick project setup
- Create directory structure
- Generate `klang.toml` config file
- Create sample files
- Initialize git repository

#### 2.5 Doc Command (Documentation)
```bash
klang doc <function>    # Show documentation
klang doc --generate    # Generate HTML docs
klang doc --open        # Open docs in browser
```

**Purpose:** Documentation tools
- Extract doc comments from code
- Generate documentation
- Show inline help

#### 2.6 Info Command (Debugging)
```bash
klang info              # Show system info
klang info --ast <file> # Show AST
klang info --tokens     # Show tokens
klang info --bytecode   # Show bytecode
```

**Purpose:** Debugging and introspection
- System information
- Show compilation stages
- Useful for development and learning

#### 2.7 Watch Command
```bash
klang watch <file>      # Watch and auto-run
klang watch --check     # Watch and check only
```

**Purpose:** Development mode with auto-reload
- Monitor file changes
- Automatically re-run on save
- Clear screen between runs
- Show timestamp of last run

---

### Phase 3: REPL Enhancements (High Priority)

#### 3.1 Readline Integration
- **Command history** (up/down arrows)
- **Autocomplete** (Tab key)
- **Multi-line input** (detect incomplete statements)
- **History file** (`~/.klang_history`)
- **Vi/Emacs keybindings**

**Library:** Use GNU Readline or linenoise (smaller alternative)

#### 3.2 REPL Special Commands
```
> .help         # Show REPL commands
> .exit/.quit   # Exit REPL
> .load <file>  # Load and execute file
> .save <file>  # Save session to file
> .clear        # Clear screen
> .env          # Show environment variables
> .reset        # Reset interpreter state
> .type <var>   # Show type of variable
> .ast <expr>   # Show AST of expression
```

#### 3.3 Better REPL Output
- Syntax highlighting for input
- Pretty-print output
- Show type information
- Auto-print expressions (no explicit print needed)
- Show execution time for long operations

#### 3.4 REPL History Search
- Ctrl+R for reverse search
- Persistent history across sessions
- History limit configuration

---

### Phase 4: Configuration & Customization

#### 4.1 Config File Support (`klang.toml`)
```toml
[project]
name = "my-app"
version = "0.1.0"
authors = ["Your Name"]

[build]
target = "release"
optimize = true

[format]
indent = 4
max_line_length = 100

[repl]
history_size = 1000
auto_print = true
colors = true

[paths]
stdlib = "/usr/local/lib/klang/stdlib"
```

#### 4.2 Environment Variables
```bash
KLANG_PATH=/path/to/stdlib
KLANG_CONFIG=/path/to/klang.toml
KLANG_NO_COLOR=1
KLANG_VERBOSE=1
```

#### 4.3 Global Config
- User config: `~/.klang/config.toml`
- System config: `/etc/klang/config.toml`
- Project config: `./klang.toml`
- Priority: Project > User > System

---

### Phase 5: Advanced Features

#### 5.1 Verbose/Quiet Modes
```bash
klang run --verbose file.kl    # Show detailed execution
klang run --quiet file.kl      # Suppress output
klang run --timing file.kl     # Show timing info
```

#### 5.2 Progress Indicators
- Spinner for long operations
- Progress bar for batch operations
- ETA for time-consuming tasks

#### 5.3 Benchmarking
```bash
klang bench <file>             # Benchmark execution
klang bench --iterations=1000  # Multiple runs
klang bench --compare <file2>  # Compare two files
```

#### 5.4 Package Manager Integration (Future)
```bash
klang install <package>        # Install package
klang update                   # Update packages
klang list                     # List installed packages
```

#### 5.5 LSP Support (Future)
```bash
klang lsp                      # Start Language Server
```

---

## 📐 Architecture Design

### File Structure
```
src/
├── cli/
│   ├── main.c              # Entry point
│   ├── args.c              # Argument parsing
│   ├── args.h
│   ├── commands.c          # Command dispatcher
│   ├── commands.h
│   ├── help.c              # Help system
│   ├── help.h
│   ├── colors.c            # Color output
│   ├── colors.h
│   ├── config.c            # Config file handling
│   ├── config.h
│   ├── utils.c             # CLI utilities
│   └── utils.h
├── cli_commands/
│   ├── cmd_run.c           # klang run
│   ├── cmd_build.c         # klang build
│   ├── cmd_repl.c          # klang repl
│   ├── cmd_check.c         # klang check
│   ├── cmd_fmt.c           # klang fmt
│   ├── cmd_test.c          # klang test
│   ├── cmd_init.c          # klang init
│   ├── cmd_doc.c           # klang doc
│   ├── cmd_info.c          # klang info
│   ├── cmd_watch.c         # klang watch
│   ├── cmd_version.c       # klang version
│   └── cmd_bench.c         # klang bench
├── repl/
│   ├── repl.c              # Enhanced REPL
│   ├── repl.h
│   ├── readline.c          # Readline integration
│   ├── readline.h
│   ├── completion.c        # Autocomplete
│   ├── completion.h
│   ├── highlight.c         # Syntax highlighting
│   └── highlight.h
└── cli.c (legacy - to be refactored)
```

### Data Structures

```c
// Argument definition
typedef struct {
    const char *long_name;   // --version
    char short_name;         // -v
    const char *description;
    bool requires_value;
    const char *value_name;  // <file>
} Argument;

// Command definition
typedef struct {
    const char *name;
    const char *description;
    const char *usage;
    Argument *args;
    int arg_count;
    int (*handler)(int argc, char **argv);
} Command;

// Config structure
typedef struct {
    char *project_name;
    char *version;
    int indent_size;
    int max_line_length;
    bool colors_enabled;
    int repl_history_size;
    char *stdlib_path;
} Config;

// CLI context
typedef struct {
    Config *config;
    bool verbose;
    bool quiet;
    bool no_color;
    bool timing;
} CLIContext;
```

---

## 🛠️ Implementation Steps

### Step 1: Refactor Current CLI
1. Extract commands into separate files
2. Create command registration system
3. Implement argument parser
4. Add colors module

**Files to create:**
- `src/cli/args.c` - Argument parsing
- `src/cli/commands.c` - Command dispatcher
- `src/cli/colors.c` - Color output

### Step 2: Implement Help & Version
1. Add `--help` flag handler
2. Add `--version` flag handler
3. Create help text for existing commands
4. Add command suggestions for typos

### Step 3: Add Core Commands
1. Implement `klang check`
2. Implement `klang fmt`
3. Implement `klang test`
4. Implement `klang info`

### Step 4: Enhance REPL
1. Integrate readline library
2. Add command history
3. Add autocomplete
4. Add special REPL commands (`.help`, `.load`, etc.)

### Step 5: Add Configuration
1. Implement config file parser (TOML)
2. Add environment variable support
3. Create default configs

### Step 6: Advanced Features
1. Add watch mode
2. Add benchmark command
3. Add init command
4. Add progress indicators

---

## 📦 Dependencies

### Required Libraries
1. **GNU Readline** (or linenoise)
   - Purpose: REPL history and autocomplete
   - License: GPL (readline) or BSD (linenoise)
   - Alternative: Use linenoise for simpler licensing

2. **TOML Parser** (tomlc99)
   - Purpose: Config file parsing
   - License: MIT
   - Alternative: Custom INI parser

3. **File watching** (inotify on Linux, kqueue on macOS)
   - Purpose: Watch mode
   - Built into OS, no external dependency

### Optional Libraries
1. **libgit2** - Git integration for init command
2. **libcurl** - Package management (future)

---

## 🎨 User Experience Examples

### Example 1: Professional Help Output
```bash
$ klang --help

KLang v0.1.0 - A modern programming language

Usage: klang [OPTIONS] <COMMAND>

Commands:
  run         Run a KLang file
  build       Compile a KLang file
  repl        Start interactive REPL
  check       Check syntax without running
  fmt         Format KLang code
  test        Run tests
  init        Create a new project
  info        Show system and debug info
  watch       Watch files and auto-run
  version     Show version information
  help        Show help information

Options:
  -h, --help       Show this help message
  -v, --version    Show version
  -V, --verbose    Verbose output
  -q, --quiet      Quiet mode
  --no-color       Disable colored output

Run 'klang <command> --help' for more information on a specific command.

Examples:
  klang run app.kl          Run a file
  klang repl                Start REPL
  klang fmt --check .       Check code formatting
  klang test                Run all tests

For more information, visit: https://github.com/yourusername/KLang
```

### Example 2: Command-Specific Help
```bash
$ klang run --help

klang-run - Run a KLang file

Usage: klang run [OPTIONS] <FILE>

Arguments:
  <FILE>    The KLang file to run (.kl, .k, .klang)

Options:
  -w, --watch           Watch file and auto-run on changes
  -t, --timing          Show execution time
  -d, --debug           Enable debug output
  -e, --env <VAR=VAL>   Set environment variable
  -h, --help            Show this help

Examples:
  klang run app.kl              Run app.kl
  klang run --watch app.kl      Watch and auto-run
  klang run --timing app.kl     Show execution time
```

### Example 3: Error with Suggestions
```bash
$ klang rnu app.kl

Error: Unknown command 'rnu'

Did you mean one of these?
  run    Run a KLang file
  repl   Start interactive REPL

Run 'klang --help' for all available commands.
```

### Example 4: Version Information
```bash
$ klang --version

KLang 0.1.0
Commit: a1b2c3d (2024-03-22)
Built: 2024-03-22 14:30:00 UTC
Platform: Linux x86_64 (GNU/Linux)
Features: GC, REPL, VM
```

### Example 5: Check Command
```bash
$ klang check app.kl

✓ app.kl - OK (parsed in 12ms)

$ klang check broken.kl

✗ broken.kl - Error
  Line 5: Unexpected token '}'
  Line 8: Undefined variable 'x'

Found 2 errors.
```

### Example 6: Format Command
```bash
$ klang fmt --check .

Checking formatting...
✗ src/main.kl - Not formatted
✓ src/utils.kl - OK
✗ src/lib.kl - Not formatted

2 files need formatting. Run 'klang fmt .' to format.

$ klang fmt .

Formatting...
✓ Formatted src/main.kl
✓ Formatted src/lib.kl

Formatted 2 files.
```

### Example 7: Enhanced REPL
```bash
$ klang repl

KLang REPL v0.1.0
Type .help for REPL commands, or exit to quit.

> let x = 10
> x + 5
15 (Int)

> fn add(a, b) { return a + b }
> add(3, 4)
7 (Int)

> .type add
Function (Int, Int) -> Int

> [Tab for autocomplete]
> pri[Tab]
print  println

> [Ctrl+R for search]
(reverse-i-search)`add': add(3, 4)

> .help

REPL Commands:
  .help         Show this help
  .exit, .quit  Exit REPL
  .load <file>  Load and execute file
  .save <file>  Save session to file
  .clear        Clear screen
  .env          Show variables
  .reset        Reset interpreter
  .type <var>   Show type of variable
  .ast <expr>   Show AST of expression

> .exit
Goodbye!
```

### Example 8: Init Command
```bash
$ klang init my-app

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

---

## 📊 Priority Matrix

### High Priority (Phase 1 - MVP)
1. ✅ Help system (`--help`, `-h`)
2. ✅ Version info (`--version`, `-v`)
3. ✅ Better error messages with suggestions
4. ✅ Color output support
5. ✅ Proper argument parsing

### Medium Priority (Phase 2 - Usability)
6. ⭐ `check` command
7. ⭐ `fmt` command
8. ⭐ Enhanced REPL with readline
9. ⭐ Config file support
10. ⭐ `info` command

### Lower Priority (Phase 3 - Advanced)
11. 📦 `test` command
12. 📦 `init` command
13. 📦 `watch` command
14. 📦 `bench` command
15. 📦 Progress indicators

---

## 🧪 Testing Plan

### Unit Tests
- Test argument parsing with various inputs
- Test command dispatching
- Test color output with/without TTY
- Test config file parsing

### Integration Tests
- Test each command end-to-end
- Test flag combinations
- Test error handling
- Test REPL features

### Manual Testing
- Test on different terminals
- Test with different shells (bash, zsh, fish)
- Test on different platforms (Linux, macOS, Windows)

---

## 📝 Migration Path

### Backward Compatibility
- Keep existing command syntax working
- Add deprecation warnings for old usage
- Provide migration guide

### Documentation Updates
- Update README.md
- Create CLI_GUIDE.md
- Update GETTING_STARTED.md
- Add man pages

---

## 🎯 Success Criteria

A successful CLI enhancement means:
1. ✅ Users can discover features through `--help`
2. ✅ Error messages are clear and actionable
3. ✅ REPL has history and autocomplete
4. ✅ Common workflows are streamlined
5. ✅ Output is beautiful and informative
6. ✅ CLI feels professional and polished
7. ✅ No breaking changes for existing users
8. ✅ Documentation is comprehensive

---

## 🔄 Future Enhancements

### Long-term Vision
1. **Language Server Protocol (LSP)**
   - IDE integration
   - Real-time error checking
   - Code navigation

2. **Package Manager**
   - Install third-party libraries
   - Version management
   - Dependency resolution

3. **Build System**
   - Multi-file compilation
   - Incremental builds
   - Cross-compilation

4. **Debugger Integration**
   - Breakpoints
   - Step-through execution
   - Variable inspection

5. **Profiling Tools**
   - CPU profiling
   - Memory profiling
   - Flame graphs

---

## 📚 References & Inspiration

### CLI Design Inspiration
- **Rust (cargo)** - Excellent UX, clear error messages
- **Go (go)** - Simple but powerful commands
- **Deno (deno)** - Modern CLI with great defaults
- **Node.js (node)** - Ubiquitous REPL
- **Python (python)** - Simple and accessible

### Libraries to Study
- `clap` (Rust) - Argument parsing inspiration
- GNU Readline - REPL history/editing
- Rich (Python) - Beautiful terminal output
- Cobra (Go) - Command structure

---

## 💡 Key Design Principles

1. **Discoverability** - Help should be easy to find
2. **Consistency** - Similar flags across commands
3. **Feedback** - Always tell users what's happening
4. **Safety** - Confirm destructive operations
5. **Performance** - CLI should feel fast
6. **Beauty** - Pretty output improves UX
7. **Simplicity** - Common tasks should be easy
8. **Power** - Advanced users can do complex things

---

## 📞 Next Steps

1. **Review this plan** - Get feedback from team/community
2. **Prioritize features** - Decide what to build first
3. **Create issues** - Break down into implementable tasks
4. **Start with Phase 1** - Help and version first
5. **Iterate** - Build, test, get feedback, improve

---

**Status:** 📝 Planning Phase
**Version:** 1.0
**Last Updated:** 2024-03-22

