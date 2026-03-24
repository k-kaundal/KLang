<div align="center">

# 🚀 KLang

### The AI-Native Programming Language

*Combining the power of C, simplicity of Python, modern JavaScript features, and first-class AI integration*

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Build](https://img.shields.io/badge/build-passing-brightgreen)](.)
[![Tests](https://img.shields.io/badge/tests-100%25-brightgreen)](docs/reports/)
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
- HTTP/HTTPS client
- File I/O operations
- Environment variables
- Regular expressions
- JSON support
- Math & string utilities

</td>
<td>

### 🚀 Performance
- Multi-tier execution
- Interpreter for quick iterations
- VM for optimized bytecode
- LLVM JIT for native speed
- Memory-efficient GC

</td>
</tr>
</table>

---

## 🚀 Quick Start

### Installation

```bash
# Clone the repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install -y build-essential libreadline-dev llvm-16 llvm-16-dev

# Build KLang
make

# Verify installation
./klang --version
```

For other platforms, see [Installation Guide](docs/USER_GUIDE.md#installation).

### Hello World

```klang
# hello.kl
println("Hello, KLang! 🚀")

let greet = name => "Hello, " + name + "!"
println(greet("World"))
```

Run it:
```bash
./klang run hello.kl
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

More examples in [`examples/`](examples/) directory.

---

## 📚 Documentation

| Document | Description |
|----------|-------------|
| [User Guide](docs/USER_GUIDE.md) | **Start here!** Installation, basics, and tutorials |
| [API Reference](docs/API_REFERENCE.md) | Complete API with all functions and methods |
| [AI Native Guide](docs/AI_NATIVE_GUIDE.md) | Deep dive into AI features |
| [Architecture](docs/architecture/ARCHITECTURE.md) | Technical design and internals |
| [Contributing](CONTRIBUTING.md) | How to contribute to KLang |
| [Changelog](CHANGELOG.md) | Version history and updates |

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
- 📧 **Email**: [INSERT EMAIL]
- 🐦 **Twitter**: [INSERT TWITTER]

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
