# KLang Usage Guide

## How to Use KLang: Complete Reference

This guide covers all aspects of using KLang, from basic usage to advanced features.

---

## Table of Contents
- [Command-Line Interface](#command-line-interface)
- [Interactive REPL](#interactive-repl)
- [Running Scripts](#running-scripts)
- [Project Structure](#project-structure)
- [Standard Library](#standard-library)
- [Debugging](#debugging)
- [Best Practices](#best-practices)
- [Common Patterns](#common-patterns)

---

## Command-Line Interface

### Basic Syntax
```bash
klang [command] [options] [file]
```

### Available Commands

#### `repl` - Interactive Mode
Start the Read-Eval-Print-Loop for interactive coding:
```bash
./klang repl
```

**Features:**
- Instant code evaluation
- Variable persistence across statements
- Multi-line support (coming soon)
- Command history (coming soon)

#### `run <file>` - Execute Script
Run a KLang script:
```bash
./klang run myprogram.kl
./klang run examples/hello.kl
```

#### `build <file>` - Build Bytecode (Future)
Compile to bytecode:
```bash
./klang build myprogram.kl -o myprogram.bc
```

#### `compile <file>` - Native Compilation (Future)
Compile to native executable:
```bash
./klang compile myprogram.kl -o myprogram
```

### Options (Future Features)
```bash
--version          # Show version information
--help             # Show help message
--debug            # Enable debug output
--optimize         # Enable optimizations
--check            # Syntax check only
--output <file>    # Specify output file
--verbose          # Verbose output
```

---

## Interactive REPL

### Starting the REPL
```bash
./klang repl
```

You'll see:
```
KLang REPL v0.1.0
Type 'exit' to quit
>>
```

### Basic Usage

#### Simple Expressions
```klang
>> 2 + 2
4
>> 10 * 5
50
>> "Hello" + " " + "World"
"Hello World"
```

#### Variables
```klang
>> let x = 10
10
>> let y = 20
20
>> x + y
30
>> let name = "Alice"
"Alice"
>> println(name)
Alice
```

#### Functions
```klang
>> fn double(x: int) -> int { return x * 2 }
[function]
>> double(21)
42
```

#### Classes
```klang
>> class Point { let x: int = 0 let y: int = 0 }
[class]
>> let p = new Point()
[object]
>> p.x = 10
10
>> println(p.x)
10
```

### REPL Commands
```klang
exit             # Exit REPL
quit             # Exit REPL (alias)
clear            # Clear screen (future)
help             # Show help (future)
```

### Tips for REPL Usage

**1. Multi-Statement Entry (Current Workaround)**
```klang
# Put everything on one line (for now)
>> let x = 10 let y = 20 println(x + y)
```

**2. Testing Functions**
```klang
>> fn test() { println("Testing!") }
>> test()
Testing!
```

**3. Quick Calculations**
```klang
>> 100 * 1.08
108.0
>> 365 * 24 * 60 * 60
31536000
```

---

## Running Scripts

### Creating a Script

#### Step 1: Create a File
```bash
nano hello.kl
# or
vim hello.kl
# or
code hello.kl
```

#### Step 2: Write Code
```klang
# hello.kl
println("Hello, World!")
```

#### Step 3: Run
```bash
./klang run hello.kl
```

### Script Structure

#### Single-File Script
```klang
# myprogram.kl

# Variables
let config = "production"

# Functions
fn initialize() {
    println("Initializing...")
}

fn main() {
    initialize()
    println("Program started!")
}

# Execute
main()
```

#### Multi-Statement Script
```klang
# calculation.kl

let x = 10
let y = 20
let z = x + y

println("x = " + x)
println("y = " + y)
println("z = " + z)

fn calculate(a: int, b: int) -> int {
    return a * b
}

let result = calculate(x, y)
println("Result: " + result)
```

### Passing Arguments (Future Feature)
```bash
# Future syntax
./klang run script.kl --arg1 value1 --arg2 value2
```

```klang
# Future: Access arguments
let args = sys.args()
println(args[0])
```

---

## Project Structure

### Recommended Layout

#### Small Project
```
myproject/
├── main.kl           # Entry point
├── utils.kl          # Utility functions
└── README.md         # Project documentation
```

#### Medium Project
```
myproject/
├── src/
│   ├── main.kl       # Entry point
│   ├── core.kl       # Core logic
│   ├── utils.kl      # Utilities
│   └── models/
│       ├── user.kl   # User model
│       └── product.kl # Product model
├── tests/
│   ├── test_core.kl
│   └── test_models.kl
├── examples/
│   └── demo.kl
└── README.md
```

#### Large Project
```
myproject/
├── src/
│   ├── main.kl
│   ├── app/
│   │   ├── controllers/
│   │   ├── models/
│   │   └── views/
│   ├── lib/
│   │   ├── database.kl
│   │   ├── http.kl
│   │   └── utils.kl
│   └── config/
│       ├── development.kl
│       └── production.kl
├── tests/
│   ├── unit/
│   ├── integration/
│   └── fixtures/
├── docs/
├── examples/
├── scripts/
│   ├── build.sh
│   └── test.sh
├── Makefile
└── README.md
```

---

## Standard Library

### Built-in Functions

#### I/O Functions

**`println(value)`**
```klang
println("Hello")           # Print string
println(42)                # Print number
println(true)              # Print boolean
println([1, 2, 3])         # Print list
```

#### Collection Functions

**`len(collection)`**
```klang
let nums = [1, 2, 3, 4, 5]
let count = len(nums)      # 5

let text = "Hello"
# len(text)  # Future: string length
```

#### Type Functions

**`type(value)`** (if available)
```klang
type(42)                   # "int"
type("hello")              # "string"
type(true)                 # "bool"
type([1, 2, 3])            # "list"
```

### Future Standard Library

**String Functions** (planned)
```klang
str.length("hello")        # 5
str.upper("hello")         # "HELLO"
str.lower("HELLO")         # "hello"
str.split("a,b,c", ",")    # ["a", "b", "c"]
str.trim("  text  ")       # "text"
```

**Math Functions** (planned)
```klang
math.abs(-10)              # 10
math.max(5, 10)            # 10
math.min(5, 10)            # 5
math.sqrt(16)              # 4.0
math.pow(2, 3)             # 8
```

**List Functions** (planned)
```klang
list.append(nums, 6)       # Add element
list.remove(nums, 3)       # Remove element
list.contains(nums, 5)     # Check existence
list.sort(nums)            # Sort list
list.reverse(nums)         # Reverse list
```

---

## Debugging

### Basic Debugging

#### Print Debugging
```klang
fn calculate(x: int) -> int {
    println("calculate called with: " + x)
    let result = x * 2
    println("result: " + result)
    return result
}
```

#### Assertions (Manual)
```klang
fn assert(condition: bool, message: string) {
    if !condition {
        println("Assertion failed: " + message)
    }
}

let x = calculate(5)
assert(x == 10, "calculate(5) should return 10")
```

### Advanced Debugging (Future)

**Stack Traces**
```klang
# Future: Automatic stack traces on error
Error at line 15 in myfile.kl:
  calculate(x)
  main()
```

**Debugger Integration**
```bash
# Future: Interactive debugger
./klang debug myprogram.kl
(kldb) break 10
(kldb) run
(kldb) step
(kldb) print x
```

---

## Best Practices

### Code Organization

#### Separate Concerns
```klang
# Good: Separate functions
fn validate_input(value: int) -> bool {
    return value > 0
}

fn process_input(value: int) {
    if validate_input(value) {
        # Process
    }
}

# Bad: Everything in one function
fn do_everything(value: int) {
    if value > 0 {
        # Validate and process together
    }
}
```

#### Use Classes for Related Data
```klang
# Good: Use classes
class User {
    let name: string = ""
    let email: string = ""
    
    fn init(n: string, e: string) {
        this.name = n
        this.email = e
    }
    
    fn display() {
        println(this.name)
        println(this.email)
    }
}

# Bad: Scattered variables
let user_name = "Alice"
let user_email = "alice@example.com"
```

### Error Handling

#### Check Before Using
```klang
fn divide(a: int, b: int) -> float {
    if b == 0 {
        println("Error: Division by zero")
        return 0.0
    }
    return a / b
}
```

#### Validate Input
```klang
fn process_age(age: int) {
    if age < 0 || age > 150 {
        println("Invalid age")
        return
    }
    # Process valid age
}
```

### Performance

#### Avoid Unnecessary Loops
```klang
# Good: Single loop
let sum = 0
for i in 0 .. 100 {
    sum = sum + i
}

# Bad: Nested loops when not needed
for i in 0 .. 100 {
    for j in 0 .. 1 {  # Unnecessary
        sum = sum + i
    }
}
```

#### Cache Repeated Calculations
```klang
# Good: Calculate once
let count = len(items)
for i in 0 .. count {
    # Use count
}

# Bad: Calculate every iteration
for i in 0 .. len(items) {  # len() called repeatedly
    # ...
}
```

---

## Common Patterns

### Singleton Pattern
```klang
class Database {
    # No way to enforce singleton yet
    # Use convention
    
    fn connect() {
        println("Connected to database")
    }
}

let db = new Database()  # Create once, use everywhere
```

### Factory Pattern
```klang
class ShapeFactory {
    fn create_circle(radius: float) -> Circle {
        return new Circle(radius)
    }
    
    fn create_rectangle(width: float, height: float) -> Rectangle {
        return new Rectangle(width, height)
    }
}

let factory = new ShapeFactory()
let circle = factory.create_circle(5.0)
```

### Observer Pattern (Basic)
```klang
class Subject {
    let observers = []
    
    fn attach(observer) {
        # observers.append(observer)  # Future
    }
    
    fn notify() {
        # for observer in observers {
        #     observer.update()
        # }
    }
}
```

---

## Examples

### Calculator
```klang
# calculator.kl

class Calculator {
    fn add(a: float, b: float) -> float {
        return a + b
    }
    
    fn subtract(a: float, b: float) -> float {
        return a - b
    }
    
    fn multiply(a: float, b: float) -> float {
        return a * b
    }
    
    fn divide(a: float, b: float) -> float {
        if b == 0.0 {
            println("Error: Division by zero")
            return 0.0
        }
        return a / b
    }
}

let calc = new Calculator()
println(calc.add(10, 5))        # 15
println(calc.subtract(10, 5))   # 5
println(calc.multiply(10, 5))   # 50
println(calc.divide(10, 5))     # 2
```

### Contact List
```klang
# contacts.kl

class Contact {
    let name: string = ""
    let phone: string = ""
    let email: string = ""
    
    fn init(n: string, p: string, e: string) {
        this.name = n
        this.phone = p
        this.email = e
    }
    
    fn display() {
        println("Name: " + this.name)
        println("Phone: " + this.phone)
        println("Email: " + this.email)
    }
}

let alice = new Contact("Alice", "555-1234", "alice@example.com")
let bob = new Contact("Bob", "555-5678", "bob@example.com")

alice.display()
println("---")
bob.display()
```

---

## Resources

- 📖 [Getting Started](GETTING_STARTED.md)
- 📘 [Language Reference](LANGUAGE_REFERENCE.md)
- 🔨 [Compilation Guide](COMPILATION_GUIDE.md)
- 📚 [OOP Specification](OOP_LANGUAGE_SPEC.md)

---

**Questions?** Check the [FAQ](FAQ.md) or open an issue on [GitHub](https://github.com/k-kaundal/KLang/issues)
