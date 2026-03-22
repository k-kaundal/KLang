# Changelog

All notable changes to KLang will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added - CLI Enhancements (v0.2.0) 🎉

#### Professional CLI Interface
- **Color Support** - Beautiful ANSI color output throughout the CLI
  - Cyan for headers and info
  - Green for success and commands
  - Red for errors
  - Yellow for warnings and tips
  - `--no-color` flag to disable colors
- **Comprehensive Help System**
  - `klang --help` or `klang -h` - Full help with examples
  - `klang help <command>` - Command-specific help
  - Beautiful ASCII art headers and formatting
  - Usage examples for every command
- **Version Information**
  - `klang --version` or `klang -v` - Show version, build date, and description

#### New Commands
- **Syntax Checker** - `klang check <file>`
  - Validates syntax without executing code
  - Reports errors with colored output
  - Shows statement count on success
  - Useful for CI/CD pipelines
- **File Information** - `klang info <file>`
  - Displays comprehensive file statistics
  - Line counts (total, code, comments, empty)
  - File size and path information
  - Comment ratio percentage
  - Extension validation

#### Enhanced REPL
- Professional welcome banner with ASCII art
- Colored prompt (`>`) in green
- Result indicator (`=>`) in cyan
- Helpful tips on startup
- Goodbye message on exit
- Improved user experience

#### Better Error Messages
- Colored error labels (red "Error:")
- Clear, descriptive error messages
- Helpful suggestions for next steps
- Examples: "Try 'klang --help' for available commands"
- Invalid extension errors with file name
- Missing argument errors with usage hints

#### Implementation Details
- New module: `cli_colors.c/h` - Color support system
- New module: `cli_help.c/h` - Help system and documentation
- New module: `cli_commands.c/h` - Check and info commands
- Enhanced: `cli.c` - Main CLI with new command handling
- Enhanced: `repl.c` - REPL with colors and improved UX
- Updated: `Makefile` - Include new source files

### Added - Previous Features
- Single quote support for strings - both `'...'` and `"..."` are now supported
- Enhanced escape sequence support including `\r` (carriage return)
- Comprehensive string manipulation functions (Dart-like API):
  - `uppercase(str)` - Convert string to uppercase
  - `lowercase(str)` - Convert string to lowercase  
  - `trim(str)` - Remove leading/trailing whitespace
  - `split(str, delimiter)` - Split string into list
  - `join(list, delimiter)` - Join list elements into string
  - `substring(str, start, end)` - Extract substring
  - `startswith(str, prefix)` - Check if string starts with prefix
  - `endswith(str, suffix)` - Check if string ends with suffix
  - `contains(str, substring)` - Check if string contains substring
  - `indexof(str, substring)` - Find index of substring
  - `replace(str, old, new)` - Replace all occurrences
  - `repeat(str, count)` - Repeat string n times
- Enhanced comment support:
  - `//` single-line comments
  - `///` documentation comments
  - `/* ... */` multi-line comments
  - `#` single-line comments (existing)
- REPL improvements:
  - `exit()` function for exiting REPL (in addition to `exit` keyword)
  - `quit()` function as alias for `exit()`
  - Optional exit code support: `exit(0)`, `exit(1)`, etc.

### Fixed
- Fixed double-free memory issue when passing lists to functions
- Added deep copy for list variables to prevent memory corruption
- Improved memory management for string operations in functions

### Changed
- Updated REPL welcome message to mention both `exit` and `exit()` commands
- Enhanced lexer to support multiple comment styles

## [0.1.0] - 2024-XX-XX

### Added
- Initial release of KLang
- Basic interpreter with REPL
- Core language features:
  - Variables (`let`, `const`)
  - Data types (int, float, string, bool, null)
  - Operators (arithmetic, comparison, logical)
  - Control flow (if/else, while, for)
  - Functions with parameters and return values
  - Lists and list operations
- Object-Oriented Programming:
  - Class definitions
  - Object instantiation with `new`
  - Methods and fields
  - Inheritance
  - `this` keyword for member access
- Built-in functions:
  - `print()`, `println()` - Output functions
  - `input()` - Input function
  - `len()` - Get length of strings and lists
  - `str()`, `int()`, `float()` - Type conversion
  - `type()` - Get type of value
- Command-line interface:
  - `klang repl` - Interactive mode
  - `klang run <file>` - Execute scripts
- Cross-platform support (Linux, macOS, Windows)
- Build system with Makefile
- Comprehensive test suite
- Documentation and examples

[Unreleased]: https://github.com/k-kaundal/KLang/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/k-kaundal/KLang/releases/tag/v0.1.0
