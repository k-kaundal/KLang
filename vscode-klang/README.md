# KLang VS Code Extension

Official Visual Studio Code extension for the KLang programming language.

## Features

- **Syntax Highlighting**: Full syntax highlighting for KLang (.kl, .k, .klang files)
- **Code Snippets**: Quick snippets for common KLang patterns
- **Language Server Protocol**: Intelligent code completion, hover information, and diagnostics
- **Debugger Support**: Debug KLang programs with breakpoints and step execution
- **Commands**: Run, build, and check KLang files directly from VS Code

## Installation

### From Source

1. Clone the KLang repository
2. Navigate to the `vscode-klang` directory
3. Install the extension:

```bash
code --install-extension vscode-klang
```

Or manually copy the `vscode-klang` directory to your VS Code extensions folder:
- **Windows**: `%USERPROFILE%\.vscode\extensions`
- **macOS/Linux**: `~/.vscode/extensions`

## Usage

### Running KLang Files

1. Open a `.kl` file
2. Press `Ctrl+Shift+R` (or `Cmd+Shift+R` on Mac)
3. Or right-click and select "KLang: Run Current File"

### Language Server

The extension automatically starts the KLang Language Server when you open a KLang file, providing:

- Code completion
- Hover information
- Go to definition
- Find references
- Diagnostics (errors and warnings)

### Configuration

Configure the extension in VS Code settings:

```json
{
  "klang.languageServer.enabled": true,
  "klang.languageServer.path": "klang"
}
```

## Requirements

- Visual Studio Code 1.75.0 or higher
- KLang compiler installed and available in PATH

## Snippets

The extension provides several useful snippets:

- `fn` - Function declaration
- `afn` - Arrow function
- `class` - Class declaration
- `for` - For loop
- `if` - If statement
- `import` - Import statement

## Commands

- `KLang: Run Current File` - Execute the current KLang file
- `KLang: Build Current File` - Build the current file with the VM
- `KLang: Format Document` - Format the current KLang file
- `KLang: Check Syntax` - Check syntax without executing

## Contributing

Contributions are welcome! Please see the main [KLang repository](https://github.com/k-kaundal/KLang) for contribution guidelines.

## License

MIT License - See [LICENSE](../LICENSE) for details.
