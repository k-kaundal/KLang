# KLang VS Code Extension

Official Visual Studio Code extension for the KLang programming language - The AI-Native Programming Language.

## Features

### 🎨 Syntax Highlighting
- Complete syntax highlighting for KLang (.kl, .k, .klang files)
- Support for all language features:
  - Functions (regular and async)
  - Classes with inheritance
  - Template literals
  - Arrow functions
  - Async/await
  - Generators
  - Import/Export
  - Comments (line and block)
  - All operators and keywords

### 📝 Code Snippets
Over 40 comprehensive code snippets for:
- Functions (regular, async, arrow, generators)
- Classes (basic, inheritance, abstract)
- Control flow (if/else, loops, try/catch)
- Async operations (promises, async/await)
- Array methods (map, filter, reduce, forEach)
- Destructuring and spread operators
- Import/Export statements
- AI and HTTP operations

### ⚡ Commands
- **Run Current File** (`Ctrl+Shift+R` / `Cmd+Shift+R`)
  - Execute the currently open KLang file
  
- **Build Current File** (`Ctrl+Shift+B` / `Cmd+Shift+B`)
  - Build the current file with the KLang compiler
  
- **Check Syntax** (`Ctrl+Shift+C` / `Cmd+Shift+C`)
  - Check syntax without executing
  
- **Format Document** (`Ctrl+Shift+F` / `Cmd+Shift+F`)
  - Format the current KLang file (when formatter is available)
  
- **Open REPL**
  - Launch an interactive KLang REPL session

### 🔍 Editor Features
- Document symbol outline (functions, classes, variables)
- Hover documentation for keywords
- Context menu integration
- Editor title run button

### 🎯 Language Server Protocol (LSP)
Ready for Language Server integration when KLang LSP is implemented:
- Code completion
- Go to definition
- Find references
- Hover information
- Diagnostics (errors and warnings)

## Installation

### From VSIX
1. Download the `.vsix` file
2. Open VS Code
3. Go to Extensions (Ctrl+Shift+X / Cmd+Shift+X)
4. Click "..." menu → "Install from VSIX..."
5. Select the downloaded `.vsix` file

### From Source
1. Clone the KLang repository
2. Navigate to the `vscode-klang` directory
3. Run:
```bash
npm install
npm run compile
```
4. Press F5 to open Extension Development Host

## Requirements

- **Visual Studio Code** 1.75.0 or higher
- **KLang compiler** installed and available in PATH
  - Download from: https://github.com/k-kaundal/KLang
  - Installation instructions in the main repository

## Usage

### Running KLang Files

**Method 1: Command Palette**
1. Open a `.kl`, `.k`, or `.klang` file
2. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on Mac)
3. Type "KLang: Run Current File"
4. Press Enter

**Method 2: Keyboard Shortcut**
1. Open a KLang file
2. Press `Ctrl+Shift+R` (or `Cmd+Shift+R` on Mac)

**Method 3: Context Menu**
1. Right-click in the editor
2. Select "KLang: Run Current File"

**Method 4: Run Button**
1. Click the run button in the editor title bar

### Using Code Snippets

Type a snippet prefix and press Tab:
- `fn` → Function declaration
- `afn` → Async function
- `class` → Class declaration
- `if` → If statement
- `for` → For-of loop
- `promise` → Promise creation
- `map` → Array map
- `log` → Print to console
- ...and many more!

### Configuration

Configure the extension in VS Code settings:

```json
{
  // Enable/disable Language Server
  "klang.languageServer.enabled": true,
  
  // Path to KLang executable (default: searches PATH)
  "klang.languageServer.path": "klang",
  
  // Language Server trace level
  "klang.trace.server": "off",
  
  // Enable/disable formatting
  "klang.format.enabled": true,
  
  // Enable/disable linting
  "klang.lint.enabled": true
}
```

## KLang Language Quick Reference

### Variables
```klang
let x = 10           # Mutable variable
const y = 20         # Constant
var z = 30           # Function-scoped
```

### Functions
```klang
# Regular function
fn add(a: int, b: int) -> int {
    return a + b
}

# Arrow function
const multiply = (a, b) => a * b

# Async function
async fn fetchData() {
    const data = await fetch("https://api.example.com")
    return data
}
```

### Classes
```klang
class Person {
    let name: string = ""
    let age: int = 0
    
    fn init(name: string, age: int) {
        this.name = name
        this.age = age
    }
    
    fn greet() {
        println(`Hello, I'm ${this.name}`)
    }
}

let person = new Person("Alice", 30)
person.greet()
```

### Array Methods
```klang
let numbers = [1, 2, 3, 4, 5]
let doubled = numbers.map(x => x * 2)
let evens = numbers.filter(x => x % 2 == 0)
let sum = numbers.reduce((acc, x) => acc + x, 0)
```

## Keyboard Shortcuts

| Command | Windows/Linux | macOS |
|---------|---------------|-------|
| Run Current File | `Ctrl+Shift+R` | `Cmd+Shift+R` |
| Build Current File | `Ctrl+Shift+B` | `Cmd+Shift+B` |
| Check Syntax | `Ctrl+Shift+C` | `Cmd+Shift+C` |
| Format Document | `Ctrl+Shift+F` | `Cmd+Shift+F` |

## Known Issues

- Language Server Protocol (LSP) integration is prepared but requires KLang LSP implementation
- Code formatter is a placeholder and needs implementation
- Debugger support is planned for future releases

## Contributing

Contributions are welcome! Please see the main [KLang repository](https://github.com/k-kaundal/KLang) for contribution guidelines.

### Development Setup
1. Clone the repository
2. `cd vscode-klang`
3. `npm install`
4. Press F5 to start debugging

### Building VSIX
```bash
npm install
npm run compile
npm run package
```

This will create a `.vsix` file you can install.

## Release Notes

See [CHANGELOG.md](CHANGELOG.md) for detailed release notes.

### 1.0.0
- Initial release
- Full syntax highlighting
- 40+ code snippets
- Commands for run, build, check
- Document symbols and hover support
- LSP client ready for future integration

## License

MIT License - See [LICENSE](../LICENSE) for details.

## Links

- [GitHub Repository](https://github.com/k-kaundal/KLang)
- [Issues](https://github.com/k-kaundal/KLang/issues)
- [Documentation](https://github.com/k-kaundal/KLang/tree/main/docs)

---

**Enjoy coding with KLang!** 🚀

