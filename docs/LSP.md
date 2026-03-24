# KLang Language Server Protocol (LSP)

The KLang Language Server provides intelligent code editing features for KLang through the Language Server Protocol.

## Overview

The LSP server provides:

- **Code Completion**: Intelligent autocomplete for keywords, functions, and variables
- **Hover Information**: Documentation and type information on hover
- **Go to Definition**: Jump to symbol definitions
- **Find References**: Find all references to a symbol
- **Diagnostics**: Real-time error and warning messages
- **Document Formatting**: Automatic code formatting
- **Document Symbols**: Outline view of code structure

## Usage

### Starting the LSP Server

The LSP server is typically started automatically by your IDE/editor. To start it manually:

```bash
klang lsp
```

The server communicates via stdin/stdout using the Language Server Protocol.

### VS Code Integration

The KLang VS Code extension automatically starts and manages the LSP server. See [VS Code Extension](../vscode-klang/README.md) for details.

### Other Editors

#### Vim/Neovim

With [coc.nvim](https://github.com/neoclide/coc.nvim):

```json
{
  "languageserver": {
    "klang": {
      "command": "klang",
      "args": ["lsp"],
      "filetypes": ["klang"],
      "rootPatterns": ["package.kl.json", ".git"]
    }
  }
}
```

#### Emacs

With [lsp-mode](https://github.com/emacs-lsp/lsp-mode):

```elisp
(add-to-list 'lsp-language-id-configuration '(klang-mode . "klang"))

(lsp-register-client
 (make-lsp-client :new-connection (lsp-stdio-connection '("klang" "lsp"))
                  :major-modes '(klang-mode)
                  :server-id 'klang-lsp))
```

#### Sublime Text

With [LSP](https://packagecontrol.io/packages/LSP) package:

```json
{
  "clients": {
    "klang": {
      "enabled": true,
      "command": ["klang", "lsp"],
      "selector": "source.klang"
    }
  }
}
```

## Features

### Code Completion

Provides context-aware completions for:

- Keywords (`fn`, `let`, `const`, `if`, `for`, `while`, etc.)
- Built-in functions (`println`, `print`, `len`, `str`, etc.)
- User-defined functions and variables
- Module exports
- Class methods and properties

Trigger: Type any character or press `Ctrl+Space`

### Hover Information

Shows documentation and type information when hovering over:

- Function names
- Variable names
- Keywords
- Built-in functions

### Diagnostics

Real-time feedback for:

- Syntax errors
- Type errors
- Undefined variables
- Unused variables
- Deprecated APIs

Diagnostics appear as:
- **Errors** (red): Must be fixed
- **Warnings** (yellow): Should be addressed
- **Information** (blue): FYI
- **Hints** (gray): Suggestions

### Go to Definition

Jump to the definition of:

- Functions
- Variables
- Classes
- Imports

Shortcut: `F12` or `Ctrl+Click`

### Find References

Find all usages of:

- Functions
- Variables
- Classes

Shortcut: `Shift+F12`

### Document Formatting

Automatically format your KLang code according to style guidelines.

Features:
- Consistent indentation
- Proper spacing
- Line wrapping

Shortcut: `Shift+Alt+F`

## Configuration

Configure the LSP server behavior:

```json
{
  "klang.languageServer.enabled": true,
  "klang.languageServer.path": "klang",
  "klang.languageServer.trace": "verbose",
  "klang.format.indentSize": 4,
  "klang.format.useTabs": false
}
```

## Protocol Support

KLang LSP implements the following LSP methods:

### Lifecycle
- `initialize`
- `initialized`
- `shutdown`
- `exit`

### Document Synchronization
- `textDocument/didOpen`
- `textDocument/didChange`
- `textDocument/didSave`
- `textDocument/didClose`

### Language Features
- `textDocument/completion`
- `textDocument/hover`
- `textDocument/signatureHelp`
- `textDocument/definition`
- `textDocument/references`
- `textDocument/documentHighlight`
- `textDocument/documentSymbol`
- `textDocument/formatting`
- `textDocument/rangeFormatting`

### Workspace
- `workspace/symbol`
- `workspace/didChangeConfiguration`

## Performance

The LSP server is designed for performance:

- Incremental parsing for fast updates
- Caching of parsed ASTs
- Lazy evaluation of expensive operations
- Background processing for non-blocking operations

## Troubleshooting

### LSP Not Starting

1. Check that `klang` is in your PATH:
   ```bash
   which klang
   ```

2. Test the LSP manually:
   ```bash
   klang lsp
   ```

3. Check the LSP log file:
   ```bash
   tail -f /tmp/klang-lsp.log
   ```

### No Completions

- Ensure the document is saved
- Check for syntax errors
- Restart the LSP server

### Slow Performance

- Close unused files
- Reduce project size
- Disable unused LSP features
- Increase LSP timeout settings

## Development

### Building with LSP

```bash
make clean
make
```

### Testing LSP

Test the LSP server with a simple client:

```bash
echo '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{}}' | klang lsp
```

### Debugging

Enable verbose logging:

```bash
export KLANG_LSP_LOG=verbose
klang lsp
```

## Contributing

Contributions to the LSP server are welcome! Areas for improvement:

- Enhanced type inference
- Better error messages
- More code actions
- Refactoring support
- Code lens
- Inlay hints

See [CONTRIBUTING.md](../CONTRIBUTING.md) for guidelines.
