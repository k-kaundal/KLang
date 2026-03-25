# Change Log

All notable changes to the KLang VS Code extension will be documented in this file.

## [1.0.0] - 2026-03-25

### Added
- Initial release of KLang VS Code extension
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
  - `KLang: Format Document` - Document formatting (prepared)
  - `KLang: Open REPL` - Launch interactive REPL
  
- **Keyboard Shortcuts**
  - Run: Ctrl+Shift+R (Cmd+Shift+R on Mac)
  - Build: Ctrl+Shift+B (Cmd+Shift+B on Mac)
  - Check: Ctrl+Shift+C (Cmd+Shift+C on Mac)
  - Format: Ctrl+Shift+F (Cmd+Shift+F on Mac)
  
- **Editor Features**
  - Document symbol provider (outline view)
  - Hover provider for keyword documentation
  - Context menu integration
  - Editor title run button
  - File association for .kl, .k, .klang files
  
- **Language Server Protocol**
  - LSP client implementation ready
  - Prepared for future KLang LSP server integration
  - Configuration options for LSP
  
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
  
- **Branding**
  - Custom KLang icon (SVG)
  - Extension metadata and descriptions

### Notes
- Language Server features require KLang LSP implementation
- Code formatter is prepared but needs KLang formatter implementation
- Debugger support planned for future releases

## [Unreleased]

### Planned Features
- Full Language Server Protocol support
- Debug Adapter Protocol (DAP) integration
- Code formatter implementation
- Linter integration
- Refactoring support
- Test explorer integration
- Problem matcher for build tasks
- IntelliSense improvements
- Code actions and quick fixes
- Go to definition/references
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

---

Format based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)
