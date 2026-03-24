# Quick Start Guide

## Get Started in 5 Minutes! ⚡

This guide will get you writing and running KLang code in just a few minutes. If you haven't installed KLang yet, check the [Installation Guide](01-installation.md) first.

## Your First Line of Code

The simplest KLang program:

```klang
println("Hello, KLang!")
```

Save this as `hello.kl` and run:

```bash
klang run hello.kl
```

**Output:**
```
Hello, KLang!
```

Congratulations! You've run your first KLang program! 🎉

## Using the REPL

The REPL (Read-Eval-Print Loop) is perfect for experimenting:

```bash
klang repl
```

Try these commands interactively:

```klang
>>> let name = "World"
>>> println("Hello, " + name)
Hello, World

>>> let x = 10
>>> let y = 20
>>> println("Sum: " + str(x + y))
Sum: 30

>>> let numbers = [1, 2, 3, 4, 5]
>>> let doubled = numbers.map(n => n * 2)
>>> println(doubled)
[2, 4, 6, 8, 10]
```

Type `exit()` or press `Ctrl+D` to quit the REPL.

## Essential Syntax at a Glance

### Variables

```klang
# Immutable (cannot be changed)
const PI = 3.14159

# Mutable (can be changed)
let count = 0
count = count + 1

# Allows redeclaration
var x = 10
var x = 20  # This is OK with var
```

### Functions

```klang
# Function declaration
fn greet(name) {
    return "Hello, " + name + "!"
}

println(greet("Alice"))  # Output: Hello, Alice!

# Arrow function
let square = x => x * x
println(square(5))  # Output: 25

# Multiple parameters
let add = (a, b) => a + b
println(add(3, 7))  # Output: 10
```

### Arrays

```klang
# Create an array
let fruits = ["apple", "banana", "cherry"]

# Access elements
println(fruits[0])  # Output: apple

# Array methods
let numbers = [1, 2, 3, 4, 5]
let doubled = numbers.map(n => n * 2)
let evens = numbers.filter(n => n % 2 == 0)
let sum = numbers.reduce((acc, n) => acc + n, 0)

println(doubled)  # [2, 4, 6, 8, 10]
println(evens)    # [2, 4]
println(sum)      # 15
```

### Control Flow

```klang
# If-else
let age = 18
if age >= 18 {
    println("You're an adult")
} else {
    println("You're a minor")
}

# For loop
for i in 0..5 {
    println("Count: " + str(i))
}

# While loop
let count = 0
while count < 3 {
    println("Count: " + str(count))
    count = count + 1
}
```

### Objects

```klang
# Object literal
let person = {
    name: "Alice",
    age: 25,
    city: "New York"
}

# Access properties
println(person.name)  # Output: Alice
```

## Common Built-in Functions

```klang
# Output
println("Hello")         # Print with newline
print("Hello")           # Print without newline

# Type conversions
let num = int("42")      # String to integer
let text = str(123)      # Number to string
let decimal = float("3.14")  # String to float

# String operations
let upper = uppercase("hello")  # "HELLO"
let lower = lowercase("WORLD")  # "world"
let length = len("hello")       # 5

# Array/String length
let arr = [1, 2, 3]
println(len(arr))        # 3

# Type checking
println(type(42))        # int
println(type("hello"))   # string
println(type([1, 2]))    # array
```

## Practical Examples

### Example 1: Calculate Average

```klang
let scores = [85, 92, 78, 95, 88]
let sum = scores.reduce((acc, n) => acc + n, 0)
let average = sum / len(scores)

println("Scores: " + str(scores))
println("Average: " + str(average))
```

**Output:**
```
Scores: [85, 92, 78, 95, 88]
Average: 87.6
```

### Example 2: Filter and Transform

```klang
let products = [
    {name: "Laptop", price: 1000},
    {name: "Mouse", price: 25},
    {name: "Keyboard", price: 75},
    {name: "Monitor", price: 300}
]

# Get expensive products (price > 50)
let expensive = [1000, 75, 300]  # Simplified
let withTax = expensive.map(p => p * 1.1)

println("Expensive items with tax: " + str(withTax))
```

### Example 3: Fibonacci Numbers

```klang
fn fibonacci(n) {
    if n <= 1 {
        return n
    }
    return fibonacci(n - 1) + fibonacci(n - 2)
}

# Generate first 10 Fibonacci numbers
for i in 0..10 {
    println("fib(" + str(i) + ") = " + str(fibonacci(i)))
}
```

### Example 4: String Processing

```klang
let sentence = "hello world from klang"
let words = sentence.split(" ")
let capitalized = words.map(w => uppercase(w))
let result = capitalized.join(" ")

println(result)  # Output: HELLO WORLD FROM KLANG
```

### Example 5: Counter with Closures

```klang
fn makeCounter() {
    let count = 0
    fn increment() {
        count = count + 1
        return count
    }
    return increment
}

let counter = makeCounter()
println(counter())  # 1
println(counter())  # 2
println(counter())  # 3
```

## File I/O Basics

```klang
# Write to file
let content = "Hello from KLang!"
write_file("output.txt", content)
println("File written successfully")

# Read from file
let data = read_file("output.txt")
println("File content: " + data)

# Check if file exists
if file_exists("output.txt") {
    println("File exists!")
}
```

## Working with Multiple Files

### main.kl
```klang
# Import a module
import "utils"

let result = add(5, 10)
println("Result: " + str(result))
```

### utils.kl
```klang
fn add(a, b) {
    return a + b
}

fn multiply(a, b) {
    return a * b
}
```

Run the main file:
```bash
klang run main.kl
```

## AI Features Quick Preview

KLang has built-in AI capabilities! Here's a taste:

```klang
# Simple LLM call (requires API key)
let response = llm("What is 2 + 2?")
println(response)

# Create an LLM client
let client = LLMClient("openai", {
    apiKey: env("OPENAI_API_KEY"),
    model: "gpt-4"
})

let answer = client.chat("Explain recursion in simple terms")
println(answer)
```

**Note:** AI features require API keys. See the [AI Features Guide](20-ai-features.md) for setup.

## Common Patterns

### Pattern 1: Input Validation

```klang
fn validateAge(age) {
    if age < 0 {
        return "Age cannot be negative"
    } else if age > 150 {
        return "Age seems unrealistic"
    } else if age < 18 {
        return "Minor"
    } else {
        return "Adult"
    }
}

println(validateAge(25))  # Adult
println(validateAge(-5))  # Age cannot be negative
```

### Pattern 2: Array Transformation Pipeline

```klang
let data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

let result = data
    .filter(n => n % 2 == 0)    # Get evens: [2, 4, 6, 8, 10]
    .map(n => n * n)            # Square them: [4, 16, 36, 64, 100]
    .reduce((sum, n) => sum + n, 0)  # Sum: 220

println("Result: " + str(result))  # 220
```

### Pattern 3: Error Handling with Try-Catch

```klang
fn safeDivide(a, b) {
    if b == 0 {
        return "Error: Division by zero"
    }
    return a / b
}

println(safeDivide(10, 2))  # 5
println(safeDivide(10, 0))  # Error: Division by zero
```

### Pattern 4: Object Builder Pattern

```klang
fn createUser(name, age, city) {
    return {
        name: name,
        age: age,
        city: city,
        info: name + " (" + str(age) + ") from " + city
    }
}

let user = createUser("Bob", 30, "Seattle")
println(user.info)  # Bob (30) from Seattle
```

## CLI Commands Reference

```bash
# Run a file
klang run script.kl

# Start REPL
klang repl

# Check syntax without running
klang check script.kl

# Show version
klang --version

# Get help
klang --help

# Run with bytecode VM (faster)
klang run --vm script.kl

# Run with LLVM JIT (fastest)
klang run --jit script.kl

# Debug mode
klang run --debug script.kl
```

## Tips for Beginners

1. **Start with the REPL**: Experiment with syntax interactively
2. **Use `println()` liberally**: Debug by printing values
3. **Type checking**: Use `type()` to check variable types
4. **String conversion**: Use `str()` to convert numbers to strings for concatenation
5. **Array methods**: Master `map`, `filter`, and `reduce` - they're powerful!
6. **Read error messages**: KLang error messages are helpful and point to the issue
7. **Check examples**: The `examples/` directory has many working examples

## Debugging Tips

```klang
# Check types
let x = 42
println("Type of x: " + type(x))

# Print intermediate values
let numbers = [1, 2, 3]
println("Before: " + str(numbers))
let doubled = numbers.map(n => n * 2)
println("After: " + str(doubled))

# Debug function calls
fn factorial(n) {
    println("factorial called with: " + str(n))
    if n <= 1 {
        return 1
    }
    return n * factorial(n - 1)
}
```

## Common Mistakes to Avoid

### 1. Forgetting `str()` for String Concatenation

```klang
# ❌ Wrong
let num = 42
println("Number: " + num)  # May cause issues

# ✅ Correct
println("Number: " + str(num))
```

### 2. Mutating Constants

```klang
# ❌ Wrong
const PI = 3.14
PI = 3.14159  # Error!

# ✅ Correct
let pi = 3.14
pi = 3.14159  # OK
```

### 3. Array Indexing Out of Bounds

```klang
# ❌ Wrong
let arr = [1, 2, 3]
println(arr[5])  # Error or undefined

# ✅ Correct
if len(arr) > 5 {
    println(arr[5])
}
```

## Next Steps

Congratulations! You now know the basics of KLang. Here's what to explore next:

- **[Your First Program](03-your-first-program.md)** - Step-by-step tutorial building a complete program
- **[Variables & Types](10-variables-types.md)** - Deep dive into KLang's type system
- **[Functions](13-functions.md)** - Master function declarations, closures, and more
- **[Arrays](14-arrays.md)** - Complete guide to array methods and manipulation
- **[Control Flow](12-control-flow.md)** - Conditionals, loops, and flow control

## Quick Reference Card

```klang
# Variables
const X = 10        # Immutable
let y = 20          # Mutable
var z = 30          # Redeclarable

# Functions
fn func(x) { return x * 2 }
let arrow = x => x * 2

# Arrays
let arr = [1, 2, 3]
arr.map(x => x * 2)
arr.filter(x => x > 1)
arr.reduce((a, b) => a + b, 0)

# Control Flow
if condition { } else { }
for i in 0..10 { }
for item of array { }
while condition { }

# Objects
let obj = {key: "value"}

# Built-ins
println()  print()  str()  int()
len()  type()  uppercase()  lowercase()
```

---

**Ready to build something amazing?** Start with [Your First Program](03-your-first-program.md)! 🚀
