# Change Log

All notable changes to the KLang VS Code extension will be documented in this file.

## [1.0.0] - 2026-03-25

### Added
- Initial release of KLang VS Code extension with comprehensive features
- **Syntax Highlighting**
  - Complete TextMate grammar for KLang
  - Support for all keywords: fn, class, async, await, let, const, var, etc.
  - Template literal highlighting with interpolation support
  - Comment highlighting (line and block)
  - String escape sequences
  - Number literals (hex, binary, octal, float, integer)
  - Operators and punctuation
  
- **Code Snippets** (40+ snippets)
  - Functions: fn, afn, arrow, aarrow, gen
  - Classes: class, classext, abstract, init, method, static
  - Control Flow: if, ifelse, for, forrange, while, switch, try
  - Async: promise, then, catch, await
  - Array Methods: map, filter, reduce, foreach
  - Destructuring: darray, dobj
  - Operators: spread, rest, ternary
  - Import/Export: import, importd, export, exportd
  - Utilities: log, template, obj, comment
  - AI/HTTP: ai, http
  
- **Commands**
  - `KLang: Run Current File` - Execute current KLang file
  - `KLang: Build Current File` - Build with KLang compiler
  - `KLang: Check Syntax` - Syntax checking
  - `KLang: Format Document` - Document formatting with KLang formatter
  - `KLang: Open REPL` - Launch interactive REPL
  
- **Keyboard Shortcuts**
  - Run: Ctrl+Shift+R (Cmd+Shift+R on Mac)
  - Build: Ctrl+Shift+B (Cmd+Shift+B on Mac)
  - Check: Ctrl+Shift+C (Cmd+Shift+C on Mac)
  - Format: Ctrl+Shift+F (Cmd+Shift+F on Mac)
  
- **Advanced Editor Features**
  - Enhanced document symbol provider with support for:
    - Functions (regular and async)
    - Classes
    - Interfaces
    - Enums
    - Variables (let, const, var)
  - Improved hover provider with rich markdown documentation
  - Definition provider for symbol navigation
  - Range formatting support
  - Context menu integration
  - Editor title run button
  - File association for .kl, .k, .klang files
  
- **Language Server Protocol (Full Integration)**
  - LSP client fully integrated with KLang LSP server
  - Smart code completion with:
    - 40+ keyword snippets with placeholders
    - Built-in function suggestions
    - Type completions
    - Trigger characters (`.`, `:`)
  - Real-time diagnostics and linting:
    - Syntax error detection
    - Warning messages
    - Debounced validation (500ms)
    - Inline error display
  - Document formatting integration
  - Document symbol support
  - Enhanced completion items with:
    - Detailed documentation
    - Snippet support (insertTextFormat: 2)
    - Kind icons
  
- **Linting and Diagnostics**
  - Automatic linting on:
    - Document open
    - Document save
    - Document change (debounced)
  - Parse error messages from KLang compiler
  - Inline error display with severity levels
  - Diagnostic collection management
  
- **Code Formatting**
  - Document formatting provider
  - Range formatting provider
  - Integration with `klang fmt` command
  - Configurable via settings
  
- **Configuration Settings**
  - `klang.languageServer.enabled` - Enable/disable LSP
  - `klang.languageServer.path` - KLang executable path
  - `klang.trace.server` - LSP trace level
  - `klang.format.enabled` - Enable/disable formatting
  - `klang.lint.enabled` - Enable/disable linting
  
- **Build System**
  - TypeScript compilation
  - ESLint configuration
  - Development environment setup
  - Launch configurations for debugging
  
- **Documentation**
  - Comprehensive README with examples
  - Language quick reference
  - Installation instructions
  - Usage guide
  - Updated with new features
  
- **Branding**
  - Custom KLang icon (SVG)
  - Extension metadata and descriptions

### Enhanced
- **LSP Server (C Implementation)**
  - Added comprehensive completion items with snippets
  - Enhanced hover documentation
  - Document symbol handler
  - Improved message type parsing
  - Support for more LSP methods

### Fixed
- Language Server client initialization
- Formatter command usage (`fmt` instead of `format`)
- Proper disposable pattern for LSP client

### Notes
- All features are fully functional with KLang compiler installed
- LSP server provides real-time language intelligence
- Linting and diagnostics work out of the box

## [Unreleased]

### Planned Features
- Debug Adapter Protocol (DAP) integration
- Advanced refactoring support
- Test explorer integration
- Problem matcher for build tasks
- Workspace symbol search
- Call hierarchy
- Type hierarchy
- Semantic token provider
- Color provider for color literals
- Folding range provider
- Selection range provider
- Code lens provider
- Signature help provider
- Rename provider
- Inline hints
- Code actions and quick fixes

---

Format based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)
