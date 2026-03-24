# KLang User Guide

## Welcome to KLang! 🚀

KLang is a modern, AI-native programming language that combines the best features of JavaScript, Python, and C with built-in AI capabilities.

## Quick Start

### Installation

```bash
# Clone the repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# Build KLang
make

# Run the REPL
./klang repl
```

### Your First Program

Create a file `hello.kl`:

```klang
println("Hello, KLang!")

let name = "World"
println("Hello, " + name + "!")
```

Run it:

```bash
./klang run hello.kl
```

## Core Features

### Variables and Types

```klang
# Variables
let x = 10
let name = "Alice"
let active = true
let data = null

# Constants
const PI = 3.14159

# Arrays
let numbers = [1, 2, 3, 4, 5]
let mixed = [1, "two", true, [3, 4]]

# Object Literals
let person = {
    name: "Alice",
    age: 30,
    city: "NYC"
}

# Tuples
let point = (10, 20)
let rgb = (255, 128, 0)
```

### Functions

```klang
# Function declaration
fn add(a, b) {
    return a + b
}

# Arrow functions
let multiply = (a, b) => a * b

# Higher-order functions
let numbers = [1, 2, 3, 4, 5]
let doubled = numbers.map(x => x * 2)
let evens = numbers.filter(x => x % 2 == 0)
```

### Control Flow

```klang
# If-else
if x > 10 {
    println("big")
} else if x > 5 {
    println("medium")
} else {
    println("small")
}

# Ternary operator
let status = age >= 18 ? "adult" : "minor"

# While loop
while x < 10 {
    println(x)
    x = x + 1
}

# For loop
for i in 0..10 {
    println(i)
}

# For-of loop
for item in items {
    println(item)
}
```

### Classes and Objects

```klang
class Person {
    fn constructor(name, age) {
        this.name = name
        this.age = age
    }
    
    fn greet() {
        println("Hello, I'm " + this.name)
    }
    
    fn birthday() {
        this.age = this.age + 1
    }
}

let alice = new Person("Alice", 30)
alice.greet()  # "Hello, I'm Alice"
alice.birthday()
```

### Collections

```klang
# Arrays
let arr = [1, 2, 3]
arr.push(4)
arr.pop()
let len = arr.length

# Dictionaries
let dict = dict()
dict.set("name", "Alice")
dict.set("age", 30)
let name = dict.get("name")
let hasAge = dict.has("age")

# Sets
let set = set()
set.add(1)
set.add(2)
set.add(1)  # Duplicate ignored
let hasOne = set.has(1)
```

## AI Features 🤖

### Simple LLM Calls

```klang
import "stdlib/ai.kl"
import "stdlib/llm.kl"

# Simple question
let apiKey = env.get("OPENAI_API_KEY")
let answer = llm("What is 2+2?", apiKey)
println(answer)
```

### Chat Sessions

```klang
import "stdlib/ai.kl"
import "stdlib/llm.kl"

let config = new LLMConfig("openai", apiKey)
let client = new LLMClient(config)
let session = new ChatSession(client, "helpful")

# Multi-turn conversation
let response1 = session.addMessage("user", "Hi!")
println(response1.content)

let response2 = session.addMessage("user", "What's the weather?")
println(response2.content)
```

### RAG (Retrieval Augmented Generation)

```klang
import "stdlib/ai.kl"
import "stdlib/llm.kl"
import "stdlib/rag.kl"

let client = new LLMClient(new LLMConfig("openai", apiKey))
let rag = new RAGSystem(client, 500, 3)

# Ingest documents
rag.ingestDocument("doc1", "KLang is a programming language...")
rag.ingestDocument("doc2", "KLang has AI features...")

# Query with context
let answer = rag.query("What is KLang?")
println(answer)
```

### Semantic Search

```klang
import "stdlib/embedding.kl"

let store = new EmbeddingStore()

# Add embeddings
store.add("1", embedding1, {title: "Doc 1"})
store.add("2", embedding2, {title: "Doc 2"})

# Search
let results = store.search(queryEmbedding, 5)
for result in results {
    println("ID: " + result.id + ", Score: " + str(result.score))
}
```

### Autonomous Agents

```klang
import "stdlib/agents.kl"

let agent = new Agent("Assistant", "helpful", llmClient)

# Agent thinks and acts
let reasoning = agent.think("User needs help with code")
println("Agent thinks: " + reasoning)

let actionResult = agent.act("search", {query: "KLang docs"})
println("Agent did: " + str(actionResult))
```

## Command Line Interface

```bash
# Run a program
./klang run program.kl

# Start REPL
./klang repl

# Check syntax
./klang check program.kl

# Format code
./klang fmt program.kl

# Run tests
./klang test

# Show version
./klang --version

# Get help
./klang --help
```

## Best Practices

### 1. Use Meaningful Names

```klang
# Good
let userAge = 25
let isActive = true
let calculateTotal = (items) => { /* ... */ }

# Avoid
let x = 25
let flag = true
let calc = (i) => { /* ... */ }
```

### 2. Handle Errors

```klang
fn divide(a, b) {
    if b == 0 {
        println("Error: Division by zero")
        return null
    }
    return a / b
}
```

### 3. Use Constants for Magic Numbers

```klang
const MAX_RETRIES = 3
const TIMEOUT_MS = 5000
const API_VERSION = "v1"
```

### 4. Comment Complex Logic

```klang
# Calculate Fibonacci using dynamic programming
# to avoid exponential time complexity
fn fibonacci(n) {
    let dp = [0, 1]
    for i in 2..n+1 {
        dp.push(dp[i-1] + dp[i-2])
    }
    return dp[n]
}
```

## Next Steps

- **[API Reference](API_REFERENCE.md)** - Complete API documentation
- **[AI Native Guide](AI_NATIVE_GUIDE.md)** - Deep dive into AI features
- **[Examples](../examples/)** - Sample programs
- **[Contributing](developer/CONTRIBUTING.md)** - How to contribute

## Getting Help

- 📚 [Documentation](README.md)
- 🐛 [Report Issues](https://github.com/k-kaundal/KLang/issues)
- 💬 [Discussions](https://github.com/k-kaundal/KLang/discussions)

---

**Happy coding with KLang!** 🚀
