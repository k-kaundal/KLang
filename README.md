# 🚀 KLang

> A high-performance, statically-typed programming language with its own runtime — combining the power of C, the simplicity of Python, and the safety of TypeScript.

---

## 🧠 Overview

KLang is a modern programming language designed for:

* ⚡ High performance (near-native execution)
* 🔒 Strong type safety
* 🧩 Clean, minimal syntax
* 🛠️ Full control with a custom runtime

It bridges the gap between low-level systems programming and high-level developer productivity.

---

## ✨ Key Features

* Statically typed with type inference
* Python-like syntax for readability
* Custom runtime (Interpreter → VM → Native compilation)
* Memory safety (GC initially, advanced models later)
* REPL support for interactive execution
* Modular and scalable architecture

---

## 🧪 Example

```klang
fn main() {
    let x = 5 + 3 * 2
    print(x)
}
```

---

## 🏗️ Architecture

```
Source Code (.kl)
        ↓
     Lexer
        ↓
     Parser (AST)
        ↓
 Semantic Analyzer
        ↓
 Bytecode / Native Code
        ↓
   KLang Runtime (VM / GC)
```

---

## ⚙️ Current Status

### ✅ Implemented

* Lexer (tokenizer)
* Parser (AST generation)
* Expression evaluation
* **Variable Declaration Semantics (JavaScript-style)**
  * `let` - Block-scoped, mutable, prevents redeclaration
  * `var` - Block-scoped, mutable, allows redeclaration
  * `const` - Block-scoped, immutable, prevents redeclaration
  * Optional initialization for `let`/`var`
  * Enforced initialization for `const`
  * Immutability enforcement and redeclaration checks
* Functions with parameters and return values
* **Object-Oriented Programming (Classes, Objects, Inheritance)**
  * Class definitions with fields and methods
  * Object instantiation with `new` keyword
  * Constructors (`init` method)
  * `this` keyword for self-reference
  * Inheritance with `extends`
  * Method overriding
  * `super` keyword for parent access
  * **Static members** (static fields and methods)
  * **Access modifiers** (public, private, protected)
  * **Abstract classes** and abstract methods
* Control flow (if/else, while, for)
* Lists and indexing
* **String Features**
  * Single and double quote support (`'...'` and `"..."`)
  * Comprehensive string manipulation functions (uppercase, lowercase, trim, split, join, etc.)
  * Rich string operations (substring, contains, replace, repeat)
* **Math Module** 🆕
  * 20+ mathematical functions (sin, cos, sqrt, pow, log, etc.)
  * Constants: Math.PI, Math.E, Math.TAU
  * Full trigonometric, hyperbolic, and logarithmic support
* **Python Utilities** 🆕
  * `range()` function - Python-style range generation
  * Support for range(n), range(start, stop), range(start, stop, step)
  * Memory-efficient iteration patterns
* **Array Methods**
  * Functional: map, filter, reduce, forEach, find, some, every
  * Manipulation: push, pop, slice, concat, reverse, sort
  * Search: indexOf, includes
* **Enhanced Comments**
  * Multiple comment styles: `#`, `//`, `/* */`, `///` (doc comments)
* Interactive REPL with `exit()` function
* Built-in functions (print, println, input, len, type conversion, abs, min, max)
* **Standard Library** 🆕
  * 43+ utility functions across 5 modules
  * Math operations (factorial, gcd, lcm, power, sqrt)
  * String utilities (repeat, reverse, padding)
  * Array operations (sum, average, min, max, contains)
  * I/O helpers (prompt, formatted output)
  * Core utilities (sign, compare, logical operations)
* **File I/O Operations** 🆕
  * Complete file system access and control
  * 12 file I/O functions (fopen, fclose, fread, fwrite, etc.)
  * Convenience functions (readFile, writeFile, appendFile)
  * File management (exists, delete, rename, size)
  * Line-by-line reading support
* **File Extension Validation** 🆕
  * Validates `.kl`, `.k`, `.klang` extensions
  * Clear error messages for invalid files

### 🚧 In Progress

* Tuple type (90% complete - fixing bug)
* Dictionary type
* Set type
* Type checker
* Bytecode VM

### 📌 Planned

* List comprehensions
* JSON module
* Regex module
* Garbage collector
* Module system enhancements
* Native compilation
* Package manager

---

## 🚀 Getting Started

### ⚡ Quick Install (For End Users)

**KLang is self-contained - No gcc or build tools needed to run programs!**

```bash
# One-line install (Linux/macOS)
curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh | bash

# For security-conscious users, review the script first:
# curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh > /tmp/install.sh
# less /tmp/install.sh
# bash /tmp/install.sh

# Then immediately use KLang
klang repl
klang run myprogram.kl
```

**Troubleshooting:** If you get "command not found: klang" after installation, see the [PATH Troubleshooting Guide](docs/PATH_TROUBLESHOOTING.md).

### 🔧 Build from Source (For Developers)

Only needed if you want to modify KLang or pre-built binaries aren't available:

```bash
# Clone and build
git clone https://github.com/k-kaundal/KLang.git
cd KLang
scripts/build.sh        # Cross-platform build script

# Install
scripts/install.sh      # Install to system

# Use
klang repl              # Interactive shell
klang run script.kl     # Run a script
```

### Platform Support

| Platform | Status | Installation |
|----------|--------|--------------|
| 🐧 Linux (x64, ARM64) | ✅ Tested | `scripts/build.sh` |
| 🍎 macOS (Intel, Apple Silicon) | ✅ Supported | `scripts/build.sh` |
| 🪟 Windows (x64) | ✅ Supported | `scripts/build.bat` + `scripts/install-windows.ps1` |

**Key Point:** Once you have the `klang` binary, you can run KLang programs **without any dependencies** - no gcc, no make, no compilers needed!

### 📖 Documentation

**Complete documentation is now organized in the `/docs` directory!**

**Essential Guides:**
- 🚀 [**Getting Started Guide**](docs/GETTING_STARTED.md) - Setup and installation
- 📘 [**Language Reference**](docs/LANGUAGE_REFERENCE.md) - Complete syntax reference
- 🔧 [**Usage Guide**](docs/USAGE_GUIDE.md) - How to use KLang
- 📚 [**Complete Documentation Index**](docs/README.md) - Browse all docs

**Features:**
- 🎓 [**OOP Specification**](docs/OOP_LANGUAGE_SPEC.md) - Object-oriented features
- ⚡ [**Async/Await**](docs/features/ASYNC_AWAIT_IMPLEMENTATION.md) - Asynchronous programming
- 🔄 [**Generators**](docs/features/GENERATOR_IMPLEMENTATION.md) - Lazy evaluation
- 📦 [**Modules**](docs/features/ES6_MODULES_IMPLEMENTATION.md) - Code organization

**API Reference:**
- 🧮 [**Math Module**](docs/api/MATH_MODULE_DOCS.md) - Mathematical operations
- 📊 [**Array Methods**](docs/ARRAY_METHODS.md) - Array operations
- 🔤 [**String Operations**](docs/COMPLETE_FEATURE_GUIDE.md#strings) - String manipulation

**For Developers:**
- ⚖️ [**JavaScript vs KLang**](docs/JS_KLANG_FEATURE_COMPARISON.md) - Feature comparison
- 🔄 [**JS Migration Guide**](docs/JS_TO_KLANG_MIGRATION.md) - Migrating from JavaScript
- 🤝 [**Contributing Guide**](docs/development/CONTRIBUTING.md) - How to contribute

**Quick Links:**
- [Documentation Index](docs/README.md) - Complete navigation
- [All Documentation Files](ALL_DOCUMENTATION_INDEX.md) - Comprehensive A-Z index (112 files)
- [Documentation Status](DOCUMENTATION_COMPLETENESS.md) - What's complete and what's not
- [Scripts Guide](SCRIPTS_GUIDE.md) - Complete guide to all build & install scripts
- [Examples](examples/) - Working code examples
- [Tests](tests/) - Test suite ([Test Guide](tests/README.md))
- [Project Structure](PROJECT_STRUCTURE.md) - Repository organization
- [Path Fix Summary](PATH_FIX_SUMMARY.md) - All documentation paths are now correct
- [Issues](https://github.com/k-kaundal/KLang/issues) - Bug reports & features
- [Discussions](https://github.com/k-kaundal/KLang/discussions) - Q&A & ideas

---

## 💻 REPL Example

```
>> let x = 10;
10
>> x + 5;
15
>> x * 2;
20
```

---

## 🔤 Language Basics

### Variables & Types

```klang
let x: int = 10   # explicit type
let y = 20        # type inferred
type(x)           # -> "int"
```

### Math Operations (NEW!)

```klang
// Mathematical calculations with full precision
let radius = 5.0
let area = Math.PI * Math.pow(radius, 2)
println(area)  // 78.5398

// Trigonometry
let angle = Math.PI / 4  // 45 degrees
println(Math.sin(angle))  // 0.707...

// Distance calculation
let distance = Math.sqrt(Math.pow(x2-x1, 2) + Math.pow(y2-y1, 2))
```

### Python-style range() (NEW!)

```klang
// Generate sequences easily
let numbers = range(10)         // [0, 1, 2, ..., 9]
let evens = range(0, 20, 2)     // [0, 2, 4, ..., 18]
let countdown = range(10, 0, -1) // [10, 9, 8, ..., 1]

// Use with array methods
let squares = range(1, 11).map(x => x * x)
// squares = [1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
```

### Functions & Parameters

```klang
fn add(a: int, b: int) -> int {
    return a + b
}

add(3, 4)         # 7
```

### Control Flow & Loops

```klang
if x > 10 {
    println("big")
} else {
    println("small")
}

while x < 5 {
    x = x + 1
}

for i in 0 .. 3 {
    println(i)    # 0,1,2
}

# loop controls
while true {
    if x == 10 { break }
    x = x + 1
}
```

### Collections & Indexing

```klang
let nums = [1, 2, 3]
println(nums[0])  # 1
println(len(nums))# 3
```

### Comments & Printing

```klang
let name = "KLang" # trailing comments use '#'
# full line comment
println("Hello " + name)
```

### Object-Oriented Programming

```klang
# Define a class with access modifiers
class Point {
    private let x: int = 0
    private let y: int = 0
    
    # Constructor
    fn init(x_val: int, y_val: int) {
        this.x = x_val
        this.y = y_val
    }
    
    # Public method
    public fn display() {
        println(this.x)
        println(this.y)
    }
    
    # Static utility method
    public static fn distance(p1: Point, p2: Point) -> float {
        # Calculate distance between two points
        return 0.0  # Simplified
    }
}

# Create object
let p = new Point(3, 4)
p.display()  # 3, 4

# Call static method
Point.distance(p, p)

# Inheritance with abstract class
abstract class Shape {
    public abstract fn area() -> float;
    
    public fn display() {
        println("Shape with area: ")
        println(this.area())
    }
}

class Circle extends Shape {
    private let radius: float = 0.0
    
    fn init(r: float) {
        this.radius = r
    }
    
    # Implement abstract method
    public fn area() -> float {
        return 3.14159 * this.radius * this.radius
    }
}

let c = new Circle(5.0)
c.display()  # Shape with area: 78.53975
```

### File I/O Operations

```klang
# Write to file
writeFile("/tmp/data.txt", "Hello World!")

# Read from file
let content = readFile("/tmp/data.txt")
println(content)  # Hello World!

# Append to file
appendFile("/tmp/data.txt", "\nNew line")

# Line-by-line reading
let file = fopen("/tmp/data.txt", "r")
let line1 = freadline(file)
let line2 = freadline(file)
fclose(file)

# File management
if fexists("/tmp/data.txt") {
    let size = fsize("/tmp/data.txt")
    println("Size: " + str(size) + " bytes")
    
    frename("/tmp/data.txt", "/tmp/renamed.txt")
    fdelete("/tmp/renamed.txt")
}
```

---

## 🧠 Design Principles

* Performance-first
* Type safety by default
* Minimal runtime overhead
* Readable and expressive syntax
* Low-level control when needed

---

## 🔒 Safety Model

* No undefined behavior (planned)
* Memory safety via garbage collection
* Optional `unsafe` blocks (future)

---

## ⚡ Roadmap

### v0.1

* Lexer, Parser, Interpreter
* REPL

### v0.2

* Static type checker
* Better error handling

### v0.3

* Bytecode Virtual Machine
* Performance improvements

### v0.4

* Garbage Collector
* Module system

### v1.0

* Native compilation (LLVM optional)
* Package manager
* Async runtime

---

## 🧩 Future Features

* ~~Object-oriented constructs~~ ✅ **IMPLEMENTED** with static members, access modifiers, and abstract classes
* Method overloading
* Interfaces (separate from classes)
* Generics
* Advanced closures
* Async / await
* JIT compiler
* WebAssembly (WASM)
* Multithreading

---

## 🛠️ CLI

**New in v0.2.0!** The KLang CLI has been completely enhanced with professional features:

### Core Commands
```bash
klang repl              # Interactive REPL with colored output
klang run file.kl       # Interpret a source file
klang build file.kl     # Compile & execute via VM
klang check file.kl     # Check syntax without executing
klang info file.kl      # Show file statistics and info
```

### Help & Information
```bash
klang --help            # Show comprehensive help
klang --version         # Show version information
klang help <command>    # Get help for specific command
```

### Features
- ✨ **Beautiful colored output** - Professional ANSI color support
- 📚 **Comprehensive help system** - Detailed help for every command
- 🔍 **Syntax checker** - Validate code without running
- 📊 **File statistics** - Analyze code metrics (lines, comments, etc.)
- 🎯 **Smart error messages** - Clear errors with helpful suggestions
- 🎨 **Enhanced REPL** - Interactive environment with colored prompts
- ⚙️ **Flexible options** - `--no-color` flag for scripts

### Examples
```bash
# Check syntax before running
klang check myapp.kl

# View file statistics
klang info myapp.kl

# Run with beautiful output
klang run myapp.kl

# Get command-specific help
klang help run
```

**See the complete visual demo:** [CLI Visual Demo](docs/CLI_VISUAL_DEMO.md)

---

## 🧭 File Structure

* `src/` — lexer, parser, interpreter, VM, compiler, runtime, CLI
* `include/` — public headers
* `stdlib/` — **standard library** (43 utility functions across 5 modules)
* `examples/` — sample `.kl` programs
* `tests/` — unit tests (lexer, parser, interpreter)
* `docs/` — architecture notes

## 📚 Standard Library

KLang includes a **basic standard library** with 43+ utility functions:

* **math.kl** - Mathematical functions (abs, pow, sqrt, factorial, gcd, lcm, etc.)
* **string.kl** - String utilities (repeat, reverse, padding, etc.)
* **array.kl** - Array operations (sum, average, min, max, etc.)
* **io.kl** - I/O utilities (prompt, headers, separators, etc.)
* **core.kl** - Core utilities (sign, compare, logical operations, etc.)

See [`stdlib/README.md`](stdlib/README.md) for complete documentation and [`examples/stdlib_simple.kl`](examples/stdlib_simple.kl) for usage examples.

## ✅ File Extension Validation

KLang validates file extensions to ensure proper usage:

* **Accepted extensions**: `.kl`, `.k`, `.klang`
* **Invalid extensions** will show a clear error message
* **REPL mode** doesn't require file extensions

```bash
# Valid
klang run program.kl
klang run script.k

# Invalid - shows error
klang run script.txt
# Error: Invalid file extension. KLang files must have .kl, .k, or .klang extension
```

## 🤝 Contributing

Contributions are welcome. Please:

1) Build & test locally: `make && make test`  
2) Keep changes minimal and well-scoped  
3) Update docs when adding/changing language features  
4) Open a PR describing the feature or fix

---

## 📜 License

MIT License

---

## 💡 Inspiration

* C (performance)
* Python (simplicity)
* TypeScript (type safety)
* Rust (memory safety)
* Go (developer experience)

---

## 🚀 Vision

A language that is:

* As fast as C
* As simple as Python
* As safe as Rust
* As productive as JavaScript

---

## 👨‍💻 Author

Built with focus and intent.

---

## ⭐ Support

If you like this project:

* Star the repo
* Contribute
* Share it

---

Now this is a **proper Markdown file content**—just paste it into `README.md` and you're done.

If you want next level, I can generate:

* `ARCHITECTURE.md` (deep compiler internals)
* `LANGUAGE_SPEC.md` (formal grammar + rules)
* `ROADMAP.md` (engineering milestones like real projects)
