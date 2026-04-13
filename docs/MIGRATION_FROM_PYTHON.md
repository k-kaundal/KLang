# Migration Guide: Python to KLang

This guide helps Python developers transition to KLang. KLang combines Python's simplicity with modern features and AI-native capabilities.

## Table of Contents
- [Quick Comparison](#quick-comparison)
- [Syntax Differences](#syntax-differences)
- [Common Patterns](#common-patterns)
- [Advanced Features](#advanced-features)
- [Best Practices](#best-practices)

## Quick Comparison

| Feature | Python | KLang |
|---------|--------|-------|
| **Print** | `print("Hello")` | `println("Hello")` |
| **Variables** | `x = 10` | `let x = 10` |
| **Functions** | `def func():` | `fn func() {` |
| **Lists** | `[1, 2, 3]` | `[1, 2, 3]` |
| **Dicts** | `{"key": "value"}` | `{key: "value"}` |
| **Classes** | `class MyClass:` | `class MyClass {` |
| **For Loop** | `for x in list:` | `for x in list {` |
| **If Statement** | `if condition:` | `if condition {` |

## Syntax Differences

### 1. Variable Declaration

**Python:**
```python
x = 10
name = "John"
PI = 3.14159
```

**KLang:**
```klang
let x = 10
let name = "John"
const PI = 3.14159
```

**Key Differences:**
- KLang requires `let` or `const` for variable declaration
- Use `const` for constants (similar to Python convention of UPPERCASE)
- KLang has explicit scoping with `let` (block-scoped)

### 2. Functions

**Python:**
```python
def greet(name):
    return f"Hello, {name}!"

def add(a, b):
    return a + b
```

**KLang:**
```klang
fn greet(name) {
    return "Hello, " + name + "!"
}

fn add(a, b) {
    return a + b
}
```

**Arrow Functions (Bonus!):**
```klang
let greet = name => "Hello, " + name + "!"
let add = (a, b) => a + b
```

### 3. Classes and OOP

**Python:**
```python
class Animal:
    def __init__(self, name):
        self.name = name
    
    def speak(self):
        print(f"{self.name} makes a sound")

class Dog(Animal):
    def speak(self):
        print(f"{self.name} barks!")

dog = Dog("Buddy")
dog.speak()
```

**KLang:**
```klang
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
dog.speak()
```

**Key Differences:**
- Use `constructor` instead of `__init__`
- Use `this` instead of `self`
- Use braces `{}` instead of colons and indentation
- Use `extends` instead of parentheses for inheritance

### 4. Lists and Dictionaries

**Python:**
```python
# Lists
numbers = [1, 2, 3, 4, 5]
numbers.append(6)
doubled = [x * 2 for x in numbers]

# Dictionaries
person = {
    "name": "John",
    "age": 30
}
person["city"] = "NYC"
```

**KLang:**
```klang
// Lists (Arrays)
let numbers = [1, 2, 3, 4, 5]
numbers.push(6)
let doubled = numbers.map(x => x * 2)

// Objects (similar to dicts)
let person = {
    name: "John",
    age: 30
}
person.city = "NYC"
```

### 5. String Formatting

**Python:**
```python
name = "John"
age = 30
message = f"My name is {name} and I'm {age} years old"
```

**KLang:**
```klang
let name = "John"
let age = 30
let message = `My name is ${name} and I'm ${age} years old`
```

**Note:** KLang uses template literals with backticks (like JavaScript)

### 6. Loops

**Python:**
```python
# For loop
for i in range(10):
    print(i)

# For each
for item in items:
    print(item)

# While loop
while condition:
    do_something()
```

**KLang:**
```klang
// For loop
for let i = 0; i < 10; i++ {
    println(i)
}

// For each
for item in items {
    println(item)
}

// While loop
while condition {
    do_something()
}
```

### 7. List Comprehensions vs Array Methods

**Python:**
```python
# List comprehension
squares = [x**2 for x in range(10)]
evens = [x for x in numbers if x % 2 == 0]

# Map and filter
doubled = map(lambda x: x * 2, numbers)
evens = filter(lambda x: x % 2 == 0, numbers)
```

**KLang:**
```klang
// Array methods
let squares = range(10).map(x => x * x)
let evens = numbers.filter(x => x % 2 == 0)

// Chain operations
let result = numbers
    .filter(x => x % 2 == 0)
    .map(x => x * 2)
```

## Common Patterns

### File I/O

**Python:**
```python
# Read file
with open('file.txt', 'r') as f:
    content = f.read()

# Write file
with open('file.txt', 'w') as f:
    f.write("Hello, World!")
```

**KLang:**
```klang
// Read file
let content = readFile('file.txt')

// Write file
writeFile('file.txt', "Hello, World!")
```

### HTTP Requests

**Python:**
```python
import requests

response = requests.get('https://api.example.com/data')
data = response.json()
```

**KLang:**
```klang
import http from "stdlib/net"

let response = http.get('https://api.example.com/data')
let data = JSON.parse(response)
```

### Exception Handling

**Python:**
```python
try:
    risky_operation()
except Exception as e:
    print(f"Error: {e}")
finally:
    cleanup()
```

**KLang:**
```klang
try {
    risky_operation()
} catch (e) {
    println("Error: " + str(e))
} finally {
    cleanup()
}
```

## Advanced Features

### Async/Await

**Python:**
```python
import asyncio

async def fetch_data():
    await asyncio.sleep(1)
    return "data"

async def main():
    data = await fetch_data()
    print(data)

asyncio.run(main())
```

**KLang:**
```klang
async fn fetchData() {
    await sleep(1000)
    return "data"
}

async fn main() {
    let data = await fetchData()
    println(data)
}

main()
```

### Decorators vs Function Wrapping

**Python:**
```python
def timing_decorator(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        end = time.time()
        print(f"Took {end - start}s")
        return result
    return wrapper

@timing_decorator
def slow_function():
    time.sleep(1)
```

**KLang:**
```klang
fn withTiming(func) {
    return (...args) => {
        let start = Date.now()
        let result = func(...args)
        let end = Date.now()
        println(`Took ${end - start}ms`)
        return result
    }
}

let slowFunction = withTiming(() => {
    sleep(1000)
})
```

### Generators

**Python:**
```python
def fibonacci(n):
    a, b = 0, 1
    for _ in range(n):
        yield a
        a, b = b, a + b

for num in fibonacci(10):
    print(num)
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

## AI-Native Features (Unique to KLang!)

Python requires external libraries, but KLang has built-in AI support:

**Python (with library):**
```python
from openai import OpenAI

client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))
response = client.chat.completions.create(
    model="gpt-4",
    messages=[{"role": "user", "content": "Hello!"}]
)
print(response.choices[0].message.content)
```

**KLang (built-in):**
```klang
let llm = LLMClient("openai", {apiKey: env("OPENAI_API_KEY")})
let response = llm.chat("Hello!")
println(response)

// Built-in RAG system
let rag = RAG()
rag.add_document("KLang is an AI-native language")
let answer = rag.query("What is KLang?")
println(answer)
```

## Best Practices

### 1. Use `let` and `const` Appropriately
```klang
const MAX_RETRIES = 3        // Constants
let currentAttempt = 0       // Variables
```

### 2. Prefer Arrow Functions for Callbacks
```klang
// Good
numbers.map(x => x * 2)

// Also fine for complex functions
numbers.map(x => {
    let result = x * 2
    return result + 1
})
```

### 3. Use Template Literals for String Interpolation
```klang
let message = `User ${name} has ${count} items`
```

### 4. Chain Array Operations
```klang
let result = numbers
    .filter(x => x > 0)
    .map(x => x * 2)
    .reduce((sum, x) => sum + x, 0)
```

### 5. Leverage Modern Features
```klang
// Destructuring
let [first, second, ...rest] = array
let {name, age} = person

// Spread operator
let combined = [...array1, ...array2]
let merged = {...obj1, ...obj2}

// Default parameters
fn greet(name = "Guest") {
    return `Hello, ${name}!`
}
```

## Migration Checklist

- [ ] Replace `def` with `fn` or arrow functions
- [ ] Add `let`/`const` to variable declarations
- [ ] Change `self` to `this` in classes
- [ ] Replace `__init__` with `constructor`
- [ ] Convert list comprehensions to `.map()` and `.filter()`
- [ ] Update string formatting to template literals
- [ ] Add braces `{}` to all code blocks
- [ ] Test async/await code (syntax similar but slightly different)
- [ ] Explore KLang's AI-native features
- [ ] Review and update imports

## Getting Help

- **Documentation**: [docs/USER_GUIDE.md](USER_GUIDE.md)
- **API Reference**: [docs/API_REFERENCE.md](API_REFERENCE.md)
- **Examples**: [examples/](../examples/)
- **Community**: [GitHub Discussions](https://github.com/k-kaundal/KLang/discussions)

## Common Pitfalls

1. **Forgetting braces**: Python uses indentation, KLang uses `{}`
2. **Variable declaration**: Always use `let` or `const`
3. **String concatenation**: Use `+` or template literals, not Python's automatic concatenation
4. **Import syntax**: Use `import X from "path"` not `from path import X`
5. **Method names**: Some built-in methods have different names (e.g., `append` → `push`)

## Performance Tips

1. KLang compiles to native code via LLVM - often faster than Python
2. Use JIT compilation for production: `klang compile --mode=production`
3. Benchmarks show KLang is typically 10-100x faster than Python
4. Take advantage of built-in optimizations and type hints

## Next Steps

1. Read the [User Guide](USER_GUIDE.md)
2. Try the [examples](../examples/)
3. Build a small project to get familiar
4. Explore KLang's unique AI features
5. Join the community and ask questions!

Welcome to KLang! 🚀
