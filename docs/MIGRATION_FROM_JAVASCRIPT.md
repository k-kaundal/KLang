# Migration Guide: JavaScript to KLang

This guide helps JavaScript/TypeScript developers transition to KLang. KLang builds on JavaScript's modern syntax and adds AI-native capabilities plus performance optimization.

## Table of Contents
- [Quick Comparison](#quick-comparison)
- [What's Familiar](#whats-familiar)
- [Key Differences](#key-differences)
- [Common Patterns](#common-patterns)
- [Advanced Features](#advanced-features)
- [Best Practices](#best-practices)

## Quick Comparison

| Feature | JavaScript | KLang |
|---------|-----------|-------|
| **Variables** | `let x = 10` | `let x = 10` ✅ |
| **Constants** | `const PI = 3.14` | `const PI = 3.14` ✅ |
| **Functions** | `function fn() {}` | `fn fn() {}` |
| **Arrow Fns** | `x => x * 2` | `x => x * 2` ✅ |
| **Classes** | `class X {}` | `class X {}` ✅ |
| **Async/Await** | `async/await` | `async/await` ✅ |
| **Template Literals** | `` `Hello ${name}` `` | `` `Hello ${name}` `` ✅ |
| **Destructuring** | `[a, b] = [1, 2]` | `[a, b] = [1, 2]` ✅ |
| **Spread** | `...args` | `...args` ✅ |

✅ = Identical or very similar syntax

## What's Familiar

If you know modern JavaScript (ES6+), you already know most of KLang! These features work identically:

### 1. Modern Syntax
```klang
// Arrow functions
let double = x => x * 2
let add = (a, b) => a + b

// Template literals
let message = `Hello, ${name}!`

// Destructuring
let [first, second, ...rest] = array
let {name, age, ...others} = person

// Spread operator
let combined = [...arr1, ...arr2]
let merged = {...obj1, ...obj2}

// Default parameters
fn greet(name = "Guest") {
    return `Hello, ${name}!`
}
```

### 2. Array Methods
```klang
// All familiar array methods work
let doubled = numbers.map(x => x * 2)
let evens = numbers.filter(x => x % 2 == 0)
let sum = numbers.reduce((acc, x) => acc + x, 0)
let found = numbers.find(x => x > 10)
let hasEven = numbers.some(x => x % 2 == 0)
let allPositive = numbers.every(x => x > 0)

// Method chaining
let result = numbers
    .filter(x => x > 0)
    .map(x => x * 2)
    .reduce((sum, x) => sum + x, 0)
```

### 3. Object-Oriented Programming
```klang
class Animal {
    constructor(name) {
        this.name = name
    }
    
    speak() {
        println(`${this.name} makes a sound`)
    }
}

class Dog extends Animal {
    speak() {
        println(`${this.name} barks!`)
    }
}

let dog = Dog("Buddy")
dog.speak()
```

### 4. Async/Await
```klang
async fn fetchData(url) {
    let response = await fetch(url)
    return response
}

async fn main() {
    let data = await fetchData("https://api.example.com")
    println(data)
}

main()
```

## Key Differences

### 1. Function Declaration

**JavaScript:**
```javascript
function greet(name) {
    return `Hello, ${name}!`
}

// OR
const greet = function(name) {
    return `Hello, ${name}!`
}

// OR
const greet = (name) => `Hello, ${name}!`
```

**KLang:**
```klang
fn greet(name) {
    return `Hello, ${name}!`
}

// OR
let greet = name => `Hello, ${name}!`
```

**Key Difference:** Use `fn` keyword instead of `function`. Arrow functions work the same.

### 2. Import/Export

**JavaScript:**
```javascript
// Export
export const PI = 3.14159
export function add(a, b) { return a + b }
export default MyClass

// Import
import { PI, add } from './math'
import MyClass from './MyClass'
```

**KLang:**
```klang
// Export
export const PI = 3.14159
export fn add(a, b) { return a + b }
export default MyClass

// Import
import { PI, add } from "math"
import MyClass from "MyClass"
```

**Key Difference:** Mostly the same! Just use `fn` for functions.

### 3. Console Methods

**JavaScript:**
```javascript
console.log("Hello")
console.error("Error")
console.warn("Warning")
```

**KLang:**
```klang
println("Hello")         // Like console.log
print("No newline")      // Print without newline
// Use stderr for errors (built-in error handling)
```

### 4. Promises

**JavaScript:**
```javascript
new Promise((resolve, reject) => {
    if (success) resolve(value)
    else reject(error)
})

promise
    .then(value => console.log(value))
    .catch(error => console.error(error))
```

**KLang:**
```klang
Promise.new((resolve, reject) => {
    if success { resolve(value) }
    else { reject(error) }
})

promise
    .then(value => println(value))
    .catch(error => println(error))
```

### 5. Built-in Functions

**JavaScript:**
```javascript
parseInt("10")
parseFloat("3.14")
JSON.parse('{"key": "value"}')
JSON.stringify({key: "value"})
Math.abs(-5)
Math.floor(3.7)
```

**KLang:**
```klang
int("10")
float("3.14")
JSON.parse('{"key": "value"}')
JSON.stringify({key: "value"})
abs(-5)           // Math functions are global
floor(3.7)
```

### 6. Array Creation

**JavaScript:**
```javascript
// Range
const range = Array.from({length: 10}, (_, i) => i)

// Or with a library
const range = _.range(10)
```

**KLang:**
```klang
// Built-in range function
let range = range(10)         // [0, 1, 2, ..., 9]
let range = range(5, 15)      // [5, 6, 7, ..., 14]
let range = range(0, 10, 2)   // [0, 2, 4, 6, 8]
```

## Common Patterns

### HTTP Requests

**JavaScript (Node.js):**
```javascript
const fetch = require('node-fetch')

async function getData() {
    const response = await fetch('https://api.example.com/data')
    const data = await response.json()
    return data
}
```

**JavaScript (Browser):**
```javascript
async function getData() {
    const response = await fetch('https://api.example.com/data')
    const data = await response.json()
    return data
}
```

**KLang:**
```klang
import http from "stdlib/net"

async fn getData() {
    let response = await http.get('https://api.example.com/data')
    let data = JSON.parse(response)
    return data
}
```

### File I/O

**JavaScript (Node.js):**
```javascript
const fs = require('fs').promises

// Read file
const content = await fs.readFile('file.txt', 'utf8')

// Write file
await fs.writeFile('file.txt', 'Hello, World!')
```

**KLang:**
```klang
// Read file
let content = readFile('file.txt')

// Write file
writeFile('file.txt', 'Hello, World!')

// Append to file
appendFile('file.txt', 'More content')
```

### Environment Variables

**JavaScript:**
```javascript
const apiKey = process.env.API_KEY
process.env.NEW_VAR = 'value'
```

**KLang:**
```klang
let apiKey = env("API_KEY")
env("NEW_VAR", "value")  // Set environment variable
```

### Timers

**JavaScript:**
```javascript
setTimeout(() => {
    console.log("Delayed")
}, 1000)

setInterval(() => {
    console.log("Repeating")
}, 1000)
```

**KLang:**
```klang
setTimeout(() => {
    println("Delayed")
}, 1000)

setInterval(() => {
    println("Repeating")
}, 1000)
```

## Advanced Features

### Generators

**JavaScript:**
```javascript
function* fibonacci(n) {
    let a = 0, b = 1
    for (let i = 0; i < n; i++) {
        yield a
        ;[a, b] = [b, a + b]
    }
}

for (const num of fibonacci(10)) {
    console.log(num)
}
```

**KLang:**
```klang
fn* fibonacci(n) {
    let a = 0, b = 1
    for let i = 0; i < n; i++ {
        yield a
        let temp = a
        a = b
        b = temp + b
    }
}

for num of fibonacci(10) {
    println(num)
}
```

### Proxy/Metaprogramming

**JavaScript:**
```javascript
const handler = {
    get(target, prop) {
        return prop in target ? target[prop] : 'default'
    }
}

const proxy = new Proxy({}, handler)
```

**KLang:**
```klang
// KLang focuses on clarity over metaprogramming
// Use standard object patterns instead
let obj = {
    get: (key) => {
        return this[key] ?? 'default'
    }
}
```

### Classes with Private Fields

**JavaScript:**
```javascript
class Counter {
    #count = 0
    
    increment() {
        this.#count++
    }
    
    getCount() {
        return this.#count
    }
}
```

**KLang:**
```klang
class Counter {
    constructor() {
        this._count = 0  // Convention: _ for private
    }
    
    increment() {
        this._count++
    }
    
    getCount() {
        return this._count
    }
}

// Or use access modifiers
class Counter {
    private _count = 0
    
    public increment() {
        this._count++
    }
    
    public getCount() {
        return this._count
    }
}
```

## AI-Native Features (Unique to KLang!)

JavaScript requires external libraries, but KLang has built-in AI:

**JavaScript (with library):**
```javascript
import OpenAI from 'openai'

const openai = new OpenAI({
    apiKey: process.env.OPENAI_API_KEY
})

const response = await openai.chat.completions.create({
    model: "gpt-4",
    messages: [{ role: "user", content: "Hello!" }]
})

console.log(response.choices[0].message.content)
```

**KLang (built-in):**
```klang
let llm = LLMClient("openai", {apiKey: env("OPENAI_API_KEY")})
let response = llm.chat("Hello!")
println(response)

// Vector embeddings
let embeddings = llm.embeddings("Hello, world!")
println(`Embedding dimension: ${len(embeddings)}`)

// RAG system
let rag = RAG()
rag.add_document("KLang is an AI-native language")
let answer = rag.query("What is KLang?")
println(answer)

// Autonomous agents
let agent = Agent({
    name: "Assistant",
    instructions: "You are a helpful assistant",
    model: "gpt-4"
})
let result = agent.run("Help me with this task")
```

## Server Creation (NestJS-like!)

**JavaScript (Express):**
```javascript
const express = require('express')
const app = express()

app.get('/api/hello', (req, res) => {
    res.json({ message: 'Hello from Express!' })
})

app.listen(3000, () => {
    console.log('Server running on port 3000')
})
```

**KLang (built-in):**
```klang
import http from "stdlib/net"

let server = http.createServer(3000)

server.get("/api/hello", (req, res) => {
    res.json({message: "Hello from KLang!"})
})

server.start()

// Or use CLI generators (like NestJS)
// klang new server my-api
// klang generate module users
// klang generate controller products
```

## Performance Comparison

### Execution Speed

KLang typically runs **10-100x faster** than Node.js for compute-heavy tasks:

**Benchmark Results:**
- Integer operations: 10M ops/sec (vs ~1M in Node.js)
- Array operations: Comparable to V8
- LLVM compilation: Near-native C performance

### Optimization Modes

**JavaScript:**
```javascript
// Node.js optimization flags
node --optimize-for-size
node --max-old-space-size=4096
```

**KLang:**
```bash
# Development mode (fast compilation)
klang compile --mode=dev app.kl

# Production mode (max performance)
klang compile --mode=production --lto app.kl

# Custom optimization
klang compile --opt=O3 --lto --strip app.kl
```

## Best Practices

### 1. Use Modern Syntax
```klang
// Good: Arrow functions
let doubled = numbers.map(x => x * 2)

// Good: Template literals
let message = `User ${name} has ${count} items`

// Good: Destructuring
let [first, ...rest] = array
let {name, age} = person
```

### 2. Leverage Built-in Functions
```klang
// KLang has many built-ins that don't need imports
let rangeArray = range(10)
let upperCase = uppercase("hello")
let fileContent = readFile("data.txt")
```

### 3. Use Type Hints (Optional)
```klang
fn add(a: int, b: int): int {
    return a + b
}
```

### 4. Prefer `const` for Immutable Values
```klang
const MAX_RETRIES = 3
const API_URL = "https://api.example.com"
```

### 5. Chain Array Operations
```klang
let result = data
    .filter(x => x.active)
    .map(x => x.value)
    .reduce((sum, x) => sum + x, 0)
```

## Migration Checklist

- [ ] Replace `function` keyword with `fn` (or keep arrow functions)
- [ ] Change `console.log` to `println`
- [ ] Update imports (minor syntax differences)
- [ ] Replace `parseInt`/`parseFloat` with `int`/`float`
- [ ] Use built-in file I/O instead of `fs` module
- [ ] Use built-in HTTP client instead of `fetch` or `axios`
- [ ] Test async/await code (should work identically)
- [ ] Explore KLang's AI-native features
- [ ] Consider using optimization modes for production

## What You'll Love About KLang

1. **Familiar Syntax**: If you know modern JavaScript, you know KLang
2. **Better Performance**: LLVM compilation gives near-native speed
3. **Built-in Features**: No need for hundreds of npm packages
4. **AI-Native**: LLM, RAG, and agents built into the language
5. **Simple Deployment**: Single binary, no node_modules
6. **Clean Tooling**: No webpack, babel, or complex build configs

## Common Pitfalls

1. **Function declarations**: Use `fn` not `function`
2. **Console methods**: Use `println` not `console.log`
3. **Built-in functions**: Many are global (no need to import)
4. **Node.js APIs**: Most have equivalent built-ins in KLang
5. **npm packages**: Check if KLang has it built-in first

## Getting Help

- **Documentation**: [docs/USER_GUIDE.md](USER_GUIDE.md)
- **API Reference**: [docs/API_REFERENCE.md](API_REFERENCE.md)
- **Examples**: [examples/](../examples/)
- **Community**: [GitHub Discussions](https://github.com/k-kaundal/KLang/discussions)

## Example: Express App → KLang

**JavaScript (Express):**
```javascript
const express = require('express')
const app = express()

app.use(express.json())

app.get('/api/users', async (req, res) => {
    const users = await db.getUsers()
    res.json(users)
})

app.post('/api/users', async (req, res) => {
    const user = await db.createUser(req.body)
    res.status(201).json(user)
})

app.listen(3000)
```

**KLang:**
```klang
import http from "stdlib/net"

let server = http.createServer(3000)

server.get("/api/users", async (req, res) => {
    let users = await db.getUsers()
    res.json(users)
})

server.post("/api/users", async (req, res) => {
    let user = await db.createUser(req.body)
    res.status(201).json(user)
})

server.start()
```

## Next Steps

1. Read the [User Guide](USER_GUIDE.md)
2. Try the [examples](../examples/)
3. Build a small API to get familiar
4. Explore KLang's AI features
5. Share your experience with the community!

Welcome to KLang! 🚀
