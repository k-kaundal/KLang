# VS Code Extension Quick Start Guide

## ✅ What's Been Created

Your full VS Code extension for KLang is now complete with:

### Core Features
- ✅ **TypeScript Extension Source** (`src/extension.ts`)
  - Language Server Protocol client (ready for KLang LSP)
  - Command handlers (run, build, check, format, REPL)
  - Hover provider for keyword documentation
  - Document symbol provider (outline view)
  - File detection and validation

- ✅ **Comprehensive Syntax Highlighting** (`syntaxes/klang.tmLanguage.json`)
  - All KLang keywords (fn, class, async, await, etc.)
  - Template literals with interpolation
  - Comments (line and block)
  - Strings with escape sequences
  - Numbers (hex, binary, octal, float, integer)
  - Operators, punctuation, and built-in functions

- ✅ **40+ Code Snippets** (`snippets/klang.json`)
  - Functions, classes, control flow
  - Async/await, promises, generators
  - Array methods, destructuring
  - Import/export, and more

- ✅ **Language Configuration** (`language-configuration.json`)
  - Comment support
  - Auto-closing brackets
  - Surrounding pairs

- ✅ **Build System**
  - TypeScript compilation
  - ESLint configuration
  - VS Code tasks and launch configs

## 📦 Development & Testing

### Test the Extension (Development Mode)

1. **Open in VS Code:**
   ```bash
   code /Users/apple/development/KLang/vscode-klang
   ```

2. **Press F5** to launch Extension Development Host
   - This opens a new VS Code window with your extension loaded
   - Any `.kl`, `.k`, or `.klang` files will have syntax highlighting

3. **Test Features:**
   - Create a test file: `test.kl`
   - Type `fn` and press Tab to test snippets
   - Try the command palette: `Ctrl+Shift+P` → "KLang: Run Current File"
   - Test keyboard shortcuts: `Ctrl+Shift+R`

### Debug the Extension

- Set breakpoints in `src/extension.ts`
- Press F5 to debug
- Check Debug Console for output

## 📦 Package for Distribution

### Create VSIX Package

```bash
cd /Users/apple/development/KLang/vscode-klang
npm run package
```

This creates `klang-1.0.0.vsix` that you can distribute.

### Install VSIX Locally

```bash
code --install-extension klang-1.0.0.vsix
```

Or in VS Code:
1. Extensions view (Ctrl+Shift+X)
2. Click "..." menu
3. "Install from VSIX..."
4. Select the `.vsix` file

## 🚀 Commands Available

Once installed/running, these commands are available:

| Command | Shortcut | Description |
|---------|----------|-------------|
| KLang: Run Current File | Ctrl+Shift+R | Execute current .kl file |
| KLang: Build Current File | Ctrl+Shift+B | Build with compiler |
| KLang: Check Syntax | Ctrl+Shift+C | Check syntax only |
| KLang: Format Document | Ctrl+Shift+F | Format code (when implemented) |
| KLang: Open REPL | - | Launch interactive REPL |

## 🎨 Testing Syntax Highlighting

Create a test file `example.kl`:

```klang
# Test KLang syntax highlighting
import { Helper } from "./utils"

const PI = 3.14159

# Async function
async fn fetchData(url: string) {
    const response = await fetch(url)
    return response
}

# Class with inheritance
class Animal {
    let name: string = ""
    
    fn init(name: string) {
        this.name = name
    }
    
    fn speak() {
        println(`${this.name} makes a sound`)
    }
}

class Dog extends Animal {
    fn speak() {
        println(`${this.name} barks!`)
    }
}

# Arrow functions
const double = (x) => x * 2
const numbers = [1, 2, 3, 4, 5]
const doubled = numbers.map(x => x * 2)

# Template literals
let message = `Hello, the answer is ${21 * 2}`

# Try snippets: type 'fn' and press Tab
```

## 🎯 Testing Snippets

In a `.kl` file, type these prefixes and press Tab:

- `fn` → Function
- `afn` → Async function
- `class` → Class declaration
- `for` → For-of loop
- `promise` → Promise
- `map` → Array map
- `log` → println()

## 📝 Configuration

Users can configure the extension in VS Code settings:

```json
{
  "klang.languageServer.enabled": true,
  "klang.languageServer.path": "klang",
  "klang.format.enabled": true,
  "klang.lint.enabled": true
}
```

## 🔧 Making Changes

### Modify Extension Code
1. Edit `src/extension.ts`
2. Run `npm run compile`
3. Press F5 to test

### Update Syntax Highlighting
1. Edit `syntaxes/klang.tmLanguage.json`
2. Reload window (Ctrl+R in Extension Development Host)

### Add Snippets
1. Edit `snippets/klang.json`
2. Reload window

### Watch Mode (Auto-compile)
```bash
npm run watch
```

## 📤 Publishing to VS Code Marketplace

### Prerequisites
1. Create a [Visual Studio Marketplace account](https://marketplace.visualstudio.com/)
2. Get a Personal Access Token (PAT)
3. Install vsce: `npm install -g @vscode/vsce`

### Publish Steps

```bash
# Login (first time only)
vsce login klang-team

# Package and publish
npm run publish

# Or manually
vsce package
vsce publish
```

## 📋 Checklist Before Publishing

- [ ] Test all commands work
- [ ] Verify syntax highlighting
- [ ] Test all snippets
- [ ] Update version in package.json
- [ ] Update CHANGELOG.md
- [ ] Add screenshots to README (optional)
- [ ] Test on different themes
- [ ] Package and test VSIX locally

## 🐛 Troubleshooting

### Extension doesn't activate
- Check Output panel → "Extension Host"
- Look for errors in Debug Console (F5 mode)

### Syntax highlighting not working
- Verify file extension is .kl, .k, or .klang
- Check language mode in status bar
- Reload window (Ctrl+R)

### Commands not appearing
- Check command palette filter
- Verify activationEvents in package.json
- Check when clause (some commands only work with .kl files)

### TypeScript errors
```bash
npm run compile
# Check for errors in output
```

## 📚 Next Steps

1. **Language Server Protocol**
   - Implement LSP server in KLang compiler
   - The extension is already configured to connect

2. **Formatter**
   - Implement code formatter in KLang
   - Update `formatKLangFile()` in extension.ts

3. **Debugger**
   - Implement Debug Adapter Protocol (DAP)
   - Add debug configuration

4. **Testing**
   - Add extension tests
   - Set up CI/CD

## 🎉 You're Done!

Your VS Code extension is complete and ready to use. Press F5 to start testing!

For more information:
- [VS Code Extension API](https://code.visualstudio.com/api)
- [Publishing Extensions](https://code.visualstudio.com/api/working-with-extensions/publishing-extension)
- [Extension Guidelines](https://code.visualstudio.com/api/references/extension-guidelines)
