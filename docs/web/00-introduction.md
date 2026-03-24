# Welcome to KLang! 🚀

## The AI-Native Programming Language

KLang is a **modern, AI-native programming language** that seamlessly combines:
- The **power of C**
- The **simplicity of Python**
- **Modern JavaScript features**
- **First-class AI integration**

## What Makes KLang Unique?

### 🤖 AI-Native Core
KLang is the **first programming language** with native AI capabilities built into the core:
- **LLM Integration**: OpenAI, Anthropic, Ollama support
- **Vector Embeddings**: Built-in embedding generation
- **RAG Systems**: Retrieval-augmented generation
- **Autonomous Agents**: AI agents as first-class citizens
- **Chat Sessions**: Conversational AI support

### ⚡ High Performance
- **Multi-tier execution**: Interpreter → VM → LLVM JIT
- **Memory safe**: Reference counting + garbage collection
- **Fast iteration**: Quick REPL for development
- **Production ready**: Optimized bytecode and native compilation

### 🎯 Modern Syntax
```klang
# Clean, expressive syntax
let numbers = [1, 2, 3, 4, 5]
let doubled = numbers.map(x => x * 2)
println("Result: " + str(doubled))

# AI integration in one line!
let response = llm("What is the capital of France?")
println(response)
```

## Who Is KLang For?

- **AI/ML Developers**: Build AI applications with native support
- **Python Developers**: Familiar syntax with more power
- **JavaScript Developers**: Modern features you already know
- **Systems Programmers**: Performance when you need it

## Quick Example

```klang
# Traditional programming
fn fibonacci(n) {
    if n <= 1 { return n }
    return fibonacci(n-1) + fibonacci(n-2)
}

println("Fib(10) = " + str(fibonacci(10)))

# AI-powered programming
let llm = LLMClient("openai", {apiKey: env("OPENAI_API_KEY")})
let code_review = llm.chat("Review this fibonacci implementation")
println(code_review)
```

## Key Features at a Glance

| Feature | Description |
|---------|-------------|
| **Variables** | `let`, `const`, `var` with type inference |
| **Functions** | First-class functions, closures, arrow functions |
| **Classes** | Full OOP with inheritance and polymorphism |
| **Arrays** | Rich array methods (map, filter, reduce, etc.) |
| **Async/Await** | Promise-based asynchronous programming |
| **AI Features** | Native LLM, embeddings, RAG, and agents |
| **HTTP Client** | Built-in REST API support |
| **File I/O** | Easy file reading and writing |

## Why Choose KLang?

### 1. **Productivity** 
Write less code, achieve more with clean syntax and powerful features.

### 2. **AI Integration**
No external libraries needed - AI is built into the language.

### 3. **Performance**
From rapid prototyping to production-ready native code.

### 4. **Modern Features**
Destructuring, spread/rest, async/await, generators, and more.

### 5. **Easy to Learn**
If you know Python or JavaScript, you already know KLang!

## What's Next?

Ready to get started? Continue with:
- [Installation Guide](01-installation.md) - Set up KLang on your system
- [Quick Start](02-quick-start.md) - Write your first program in 5 minutes
- [Language Basics](10-variables-types.md) - Learn the fundamentals

## Community & Support

- 📖 [Full Documentation](docs/)
- 💬 [GitHub Discussions](https://github.com/k-kaundal/KLang/discussions)
- 🐛 [Report Issues](https://github.com/k-kaundal/KLang/issues)
- ⭐ [Star on GitHub](https://github.com/k-kaundal/KLang)

---

**Let's build the future of AI-native programming together!** 🚀
