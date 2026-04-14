<div align="center">

# 🚀 KLang

### The AI-Native Programming Language

*Combining the power of C, simplicity of Python, modern JavaScript features, and first-class AI integration*

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Build](https://img.shields.io/badge/build-passing-brightgreen)](.)
[![Tests](https://img.shields.io/badge/tests-238/238_passing-brightgreen)](COMPLETION_REPORT.md)
[![Security](https://img.shields.io/badge/security-A+-brightgreen)](COMPLETION_REPORT.md)
[![Version](https://img.shields.io/badge/version-1.0.0--rc-blue)](CHANGELOG.md)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

[Features](#-features) • [Quick Start](#-quick-start) • [Documentation](#-documentation) • [Examples](#-examples) • [Contributing](#-contributing)

</div>

---

## 🌟 What is KLang?

KLang is a **modern, AI-native programming language** designed for the next generation of software development. It seamlessly integrates traditional programming with AI capabilities, making it the first language with **native LLM, RAG, and agent support** built into the core.

```klang
# Traditional + AI in one language!
let data = fetch("https://api.example.com/data")
let analysis = llm("Analyze this data: " + str(data))
println(analysis)
```

### Why KLang?

- 🤖 **AI-Native**: First language with built-in LLM, embeddings, RAG, and agents
- ⚡ **High Performance**: Interpreter → VM → LLVM JIT compilation
- 🎯 **Modern Syntax**: Clean Python-like syntax with JavaScript features
- 🛡️ **Memory Safe**: Reference counting + garbage collection
- 🔧 **Batteries Included**: HTTP client, file I/O, async/await, and more
- 📦 **Easy to Use**: Simple installation, intuitive API

---

## ✨ Features

<table>
<tr>
<td width="50%">

### 🤖 AI-Native Core
- **LLM Integration**: OpenAI, Anthropic, Ollama
- **Vector Embeddings**: Built-in embedding generation
- **RAG Systems**: Retrieval-augmented generation
- **Autonomous Agents**: AI agents as first-class citizens
- **Chat Sessions**: Conversational AI support

</td>
<td width="50%">

### ⚡ Modern Language
- **Arrow Functions**: `x => x * 2`
- **Destructuring**: `let [a, b] = [1, 2]`
- **Spread/Rest**: `...args`, `...obj`
- **Async/Await**: Promise-based async
- **Generators**: `yield` and iteration
- **Classes & OOP**: Full object-oriented support

</td>
</tr>
<tr>
<td>

### 🔧 Built-in Features
- **HTTP Server**: Built-in web server with routing
- **Hot Reload**: Development mode with auto-reload
- **Build Modes**: Debug, dev, release, production
- HTTP/HTTPS client
- File I/O operations
- Environment variables
- Regular expressions
- JSON support
- Math & string utilities

</td>
<td>

### 🚀 Performance
- **Multi-tier execution**: Interpreter → VM → LLVM
- **Optimization levels**: O0, O1, O2, O3
- **Link-time optimization** (LTO)
- **C/C++ compatibility**: Pointers, manual memory
- Interpreter for quick iterations
- VM for optimized bytecode
- LLVM JIT for native speed
- Memory-efficient GC

</td>
</tr>
</table>

---

## 🚀 Quick Start

### Prerequisites

Before installing KLang, ensure you have LLVM installed:

**macOS:**
```bash
brew install llvm
export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install llvm-dev
```

**Linux (Fedora/RHEL):**
```bash
sudo dnf install llvm-devel
```

**Windows (MSYS2):**
```bash
pacman -S mingw-w64-x86_64-llvm
```

### Installation

**One-Line Install (Easiest):**

```bash
# Using curl
curl -fsSL https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash

# Or using wget
wget -qO- https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash
```

**Or build from source:**

```bash
# Clone the repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# Build KLang
make

# Install system-wide (requires sudo)
sudo make install

# Or install to user directory (no sudo)
make install-user

# Verify installation
klang --version
```

For more installation methods, see [INSTALL.md](INSTALL.md).

### Hello World

```klang
# hello.kl
println("Hello, KLang! 🚀")

let greet = name => "Hello, " + name + "!"
println(greet("World"))
```

Run it:
```bash
klang run hello.kl
```

### Interactive REPL

```bash
./klang repl
```

```klang
>>> let x = 10
>>> let y = 20
>>> println(x + y)
30
>>> let double = n => n * 2
>>> [1, 2, 3].map(double)
[2, 4, 6]
```

---

## 🎯 Examples

### Basic Programming

```klang
# Variables and functions
let name = "KLang"
const version = "1.0"

fn factorial(n) {
    if n <= 1 { return 1 }
    return n * factorial(n - 1)
}

println(factorial(5))  # 120
```

### Modern JavaScript Features

```klang
# Arrow functions, destructuring, spread
let numbers = [1, 2, 3, 4, 5]

let doubled = numbers.map(x => x * 2)
let [first, second, ...rest] = numbers
let sum = numbers.reduce((acc, x) => acc + x, 0)

println("Doubled:", doubled)
println("Sum:", sum)
```

### Object-Oriented Programming

```klang
# Classes with inheritance
class Animal {
    constructor(name) {
        this.name = name
    }
    
    speak() {
        println(this.name + " makes a sound")
    }
}

class Dog extends Animal {
    speak() {
        println(this.name + " barks!")
    }
}

let dog = Dog("Buddy")
dog.speak()  # "Buddy barks!"
```

### Async/Await

```klang
# Promise-based async operations
async fn fetchData(url) {
    let response = await fetch(url)
    return response
}

async fn main() {
    let data = await fetchData("https://api.example.com")
    println("Data:", data)
}

main()
```

### 🤖 AI Features (Unique to KLang!)

```klang
# LLM Integration
let llm = LLMClient("openai", {apiKey: env("OPENAI_API_KEY")})
let response = llm.chat("What is the capital of France?")
println(response)

# Vector Embeddings
let embeddings = llm.embeddings("Hello, world!")
println("Embedding dimension:", len(embeddings))

# RAG System
let rag = RAG()
rag.add_document("KLang is an AI-native language")
let answer = rag.query("What is KLang?")
println(answer)
```

### 🚀 Server Creation (NestJS-like!)

```bash
# Create a new server project
klang new server blog-api

# Generate modules with controllers, services, and routes
klang generate module posts
klang generate module users
```

```klang
# Generated controller with CRUD operations
class PostsController {
    getAll(req, res) {
        res.json({ message: "Get all posts", data: [] })
    }
    
    getById(req, res) {
        let id = req.params.id
        res.json({ message: "Get post by id", id: id })
    }
    
    create(req, res) {
        res.status(201).json({ message: "Post created" })
    }
}
```

More examples in [`examples/`](examples/) directory.

---

## 🆕 New Features (v1.1)

### NestJS-like Server Creation & Code Generators

Create production-ready server applications with built-in code generators:

```bash
# Create a new server project
klang new server my-api

# Generate components instantly
klang generate module users
klang generate controller products
klang generate service auth
klang generate middleware cors
```

**Features:**
- 🏗️ Project scaffolding with best-practice structure
- 🎯 Component generators (controllers, services, middleware, routes)
- 📦 Module system for organized code
- ✅ Built-in validation library
- 🔒 Guards and interceptors support
- 🎨 Decorator patterns for advanced use cases

See [Server Generators Guide](docs/SERVER_GENERATORS.md) for details.

### Interactive Terminal UI Server 🆕

Build **browser-like interactive UIs** directly in the terminal with real buttons, inputs, and mouse/keyboard support:

```bash
# Run interactive UI application
klang run examples/interactive_ui_server.kl

# Start UI server with hot reload
klang serve-ui --hot-reload myapp.kl
```

Create interactive terminal applications:

```klang
import "stdlib/ui/server.kl" as server

# Create UI server
let app = server.createServer({
    title: "My Interactive App",
    width: 80,
    height: 24
})

# Add interactive button
let btn = server.createButton("Click Me!", {
    x: 10,
    y: 5,
    style: "primary"
})

btn.on("click", (e) => {
    println("Button clicked!")
})

app.addComponent(btn)
app.start()
```

**Features:**
- 🖱️ Full mouse support (click, hover, drag)
- ⌨️ Keyboard navigation (Tab, arrows, shortcuts)
- 🎨 Rich interactive components (buttons, inputs, checkboxes, selects)
- 🔄 Real-time state updates
- 📊 Event-driven architecture
- ⚡ 60 FPS rendering

See [Interactive UI Server Guide](docs/INTERACTIVE_UI_SERVER.md) for details.

### Built-in HTTP Server

Start a web server instantly:

```bash
# Serve current directory
klang serve

# Custom port and directory
klang serve --port=3000 --dir=./public

# Development mode with hot reload
klang serve --mode=dev --hot-reload
```

Create APIs in KLang:

```klang
import http from "stdlib/net"

let server = http.createServer(3000)

server.get("/api/hello", (req, res) => {
    res.json({message: "Hello from KLang!"})
})

server.start()
```

### Build Modes & Optimization

Optimize for different environments:

```bash
# Development (fast compile, hot reload)
klang compile --mode=dev app.kl

# Production (max performance)
klang compile --mode=production --lto app.kl

# Custom optimization
klang compile --opt=O3 --lto --strip app.kl

# Check configuration
klang config
```

### C/C++ Compatibility

Access low-level features when needed:

```bash
export KLANG_ENABLE_POINTERS=1
export KLANG_ENABLE_MANUAL_MEMORY=1
```

```klang
# Pointers and manual memory management
let ptr = malloc(sizeof(int) * 100)
ptr[0] = 42
free(ptr)

# Bitwise operations
let flags = 0b1010 & 0b1100  # AND
let shifted = flags << 2      # Left shift
```

See [Build Modes Guide](docs/BUILD_MODES.md), [HTTP Server Guide](docs/HTTP_SERVER.md), and [C/C++ Features](docs/C_CPP_FEATURES.md) for details.

---

## 📚 Documentation

| Document | Description |
|----------|-------------|
| [User Guide](docs/USER_GUIDE.md) | **Start here!** Installation, basics, and tutorials |
| [Migration from Python](docs/MIGRATION_FROM_PYTHON.md) | **NEW!** Complete guide for Python developers |
| [Migration from JavaScript](docs/MIGRATION_FROM_JAVASCRIPT.md) | **NEW!** Complete guide for JS/TS developers |
| [Error Handling Guide](docs/ERROR_HANDLING_GUIDE.md) | **NEW!** Comprehensive error handling patterns |
| [Interactive UI Server](docs/INTERACTIVE_UI_SERVER.md) | **NEW!** Browser-like interactive terminal UIs |
| [Server Generators](docs/SERVER_GENERATORS.md) | **NEW!** NestJS-like server creation and code generators |
| [Build Modes](docs/BUILD_MODES.md) | **NEW!** Build modes and optimization guide |
| [HTTP Server](docs/HTTP_SERVER.md) | **NEW!** Built-in web server documentation |
| [C/C++ Features](docs/C_CPP_FEATURES.md) | **NEW!** Low-level programming features |
| [API Reference](docs/API_REFERENCE.md) | Complete API with all functions and methods |
| [AI Native Guide](docs/AI_NATIVE_GUIDE.md) | Deep dive into AI features |
| [Architecture](docs/architecture/ARCHITECTURE.md) | Technical design and internals |
| [Contributing](CONTRIBUTING.md) | How to contribute to KLang |
| [Changelog](CHANGELOG.md) | Version history and updates |
| [Completion Report](COMPLETION_REPORT.md) | **Project completion status** ✅ |

---

## 🏗️ Architecture

KLang uses a **three-tier execution model**:

```
Source Code → Lexer → Parser → AST
                                 ↓
                          Interpreter (Fast iteration)
                                 ↓
                          VM (Bytecode execution)
                                 ↓
                          LLVM JIT (Native speed)
```

- **Interpreter**: Immediate execution for development
- **VM**: Optimized bytecode for production
- **LLVM**: Native compilation for maximum performance

---

## 🧪 Testing

KLang has **100% test pass rate** with comprehensive coverage:

```bash
# Run all tests
make test                    # C unit tests (238 tests)
./tests/run_all_tests.sh     # Feature tests (44 tests)

# Run specific test
./klang run tests/features/test_name.k
```

**Test Coverage:**
- ✅ 238 C unit tests (100%)
- ✅ 44 feature tests (100%)
- ✅ 20 AI integration tests (100%)
- ✅ 282 total core tests (100%)

---

## 🗺️ Roadmap

### Current (v1.0.0-rc) ✅
- [x] Core language features
- [x] AI-native capabilities (LLM, RAG, agents)
- [x] Modern JS features (arrow functions, async/await, etc.)
- [x] OOP with classes and inheritance
- [x] LLVM JIT compilation
- [x] 100% test coverage

### Near Term (v1.1-1.3)
- [x] Package manager
- [x] Standard library expansion
- [x] Language server protocol (LSP)
- [x] IDE integrations (VS Code, etc.)
- [x] Debugger
- [ ] Performance optimizations

### Future (v2.0+) ✅ COMPLETE
- [x] Parallel execution
- [x] Advanced type system
- [x] WebAssembly target
- [x] Plugin system
- [x] Cloud native features

See [CHANGELOG.md](CHANGELOG.md) for detailed version history.

---

## 🤝 Contributing

We love contributions! Here's how you can help:

- 🐛 **Report bugs** via [Issues](https://github.com/k-kaundal/KLang/issues)
- ✨ **Suggest features** via [Discussions](https://github.com/k-kaundal/KLang/discussions)
- 📝 **Improve docs** - Documentation PRs are always welcome
- 🧪 **Add tests** - More tests mean better quality
- 💻 **Submit code** - See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines

**Good First Issues**: Check out issues labeled [`good first issue`](https://github.com/k-kaundal/KLang/labels/good%20first%20issue)

---

## 📜 License

KLang is released under the [MIT License](LICENSE).

```
Copyright (c) 2026 Kartikey Kaundal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...
```

See [LICENSE](LICENSE) file for full license text.

---

## 🙏 Acknowledgments

KLang is built with inspiration from:
- **Python** - Clean syntax and philosophy
- **JavaScript** - Modern language features
- **Go** - Simplicity and concurrency
- **Rust** - Memory safety concepts

Special thanks to all [contributors](https://github.com/k-kaundal/KLang/graphs/contributors)!

---

## 📞 Community & Support

- 📖 **Documentation**: [docs/](docs/)
- 💬 **Discussions**: [GitHub Discussions](https://github.com/k-kaundal/KLang/discussions)
- 🐛 **Issues**: [GitHub Issues](https://github.com/k-kaundal/KLang/issues)
- 🐦 **X (Twitter)**: [@k_k_kaundal](https://x.com/k_k_kaundal)
- 💻 **GitHub**: [@k-kaundal](https://github.com/k-kaundal)

---

## ⭐ Show Your Support

If you find KLang useful, please consider:
- ⭐ **Star this repository** on GitHub
- 🐦 **Share** on social media
- 📝 **Write** a blog post or tutorial
- 🤝 **Contribute** to the project

---

## 📊 Project Stats

![GitHub stars](https://img.shields.io/github/stars/k-kaundal/KLang?style=social)
![GitHub forks](https://img.shields.io/github/forks/k-kaundal/KLang?style=social)
![GitHub issues](https://img.shields.io/github/issues/k-kaundal/KLang)
![GitHub pull requests](https://img.shields.io/github/issues-pr/k-kaundal/KLang)
![Lines of code](https://img.shields.io/tokei/lines/github/k-kaundal/KLang)

---

<div align="center">

**Made with ❤️ by the KLang Community**

[Website](#) • [Documentation](docs/) • [GitHub](https://github.com/k-kaundal/KLang)

</div>
