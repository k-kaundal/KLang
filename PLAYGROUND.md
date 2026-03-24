# KLang Playground (Temporarily Unavailable)

**Note:** The KLang playground feature is currently undergoing maintenance and improvements. It is temporarily unavailable.

In the meantime, please use the KLang CLI to run and test your code locally.

---

## Using KLang Locally

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

### Hello World

Create a file `hello.kl`:

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

Start the interactive Read-Eval-Print Loop:

```bash
./klang repl
```

Try some code:

```klang
>>> let x = 10
>>> let y = 20
>>> println(x + y)
30
>>> let double = n => n * 2
>>> [1, 2, 3].map(double)
[2, 4, 6]
```

### Running Examples

Check out the [`examples/`](examples/) directory for comprehensive KLang code samples:

```bash
# Run any example
./klang run examples/hello.kl
./klang run examples/ai_native_working_demo.kl
./klang run examples/oop_demo.kl
```

**Example Categories:**
- **Basic Programming**: Variables, functions, loops, conditionals
- **Object-Oriented**: Classes, inheritance, polymorphism
- **Async/Await**: Promise-based asynchronous programming
- **AI Integration**: LLM, embeddings, RAG systems, AI agents
- **Data Structures**: Arrays, objects, advanced collections

---

## Features Available in CLI

When using KLang locally, you have access to all language features:

### ✅ Core Language
- Variables: `let`, `const`, `var`
- Functions: Regular, arrow functions, closures
- Classes: Full OOP with inheritance
- Modern syntax: Destructuring, spread/rest, async/await

### ✅ AI-Native Capabilities
- LLM Integration (OpenAI, Anthropic, Ollama)
- Vector Embeddings
- RAG Systems
- Autonomous AI Agents
- Chat Sessions

### ✅ Built-in Features
- HTTP/HTTPS Client
- File I/O Operations
- Environment Variables
- JSON Support
- Regular Expressions
- Math & String Utilities

---

## Getting Help

Need assistance? Check out these resources:

- **Documentation**: [docs/USER_GUIDE.md](docs/USER_GUIDE.md)
- **API Reference**: [docs/API_REFERENCE.md](docs/API_REFERENCE.md)
- **Examples**: [examples/](examples/)
- **GitHub Issues**: [Report bugs](https://github.com/k-kaundal/KLang/issues)
- **Discussions**: [Ask questions](https://github.com/k-kaundal/KLang/discussions)

---

## Future Plans

The playground feature will return in a future update with:
- ✨ Enhanced security and sandboxing
- ⚡ Better performance
- 📚 More example projects
- 🎨 Improved UI/UX
- 📱 Mobile support
- 🔍 Built-in debugger
- 💾 Code persistence

For now, enjoy the full power of KLang through the CLI! 🚀

---

## License

Same as KLang - MIT License
