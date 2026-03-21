# Getting Started with KLang

## 📚 Table of Contents
- [Self-Contained Runtime](#self-contained-runtime)
- [For End Users](#for-end-users)
- [For Developers](#for-developers)
- [Quick Start](#quick-start)
- [Your First Program](#your-first-program)
- [Next Steps](#next-steps)

---

## Self-Contained Runtime

**Important:** KLang is a **self-contained interpreter** that runs `.kl` programs directly. Once you have the `klang` binary:

✅ **You CAN** run KLang programs immediately  
✅ **No gcc, clang, or other compilers needed**  
✅ **No build tools required at runtime**  
✅ **Just the KLang binary is enough**

🔧 **You only need build tools (gcc, make) to BUILD KLang itself, not to RUN it!**

---

## For End Users

If you just want to **run** KLang programs, you don't need any build tools.

### Quick Installation

#### Option 1: One-Line Install (Recommended)
```bash
# Linux/macOS
curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh | bash
```

This will:
- Download or build KLang
- Install to `~/.klang/bin/`
- Add to your PATH
- No sudo required!

#### Option 2: Download Pre-built Binary (Coming Soon)
```bash
# Download from releases
wget https://github.com/k-kaundal/KLang/releases/latest/download/klang-linux-x64.tar.gz
tar -xzf klang-linux-x64.tar.gz
./install.sh
```

#### Option 3: Build from Source
If pre-built binaries aren't available:
```bash
git clone https://github.com/k-kaundal/KLang.git
cd KLang
./build.sh
./install.sh
```

### System Requirements (Runtime)

**To RUN KLang programs:**
- Operating System: Linux, macOS, or Windows
- Memory: 50 MB RAM
- Disk: 5 MB for binary
- Dependencies: None! (Only standard system libraries)

**NOT REQUIRED:**
- ❌ gcc or clang
- ❌ make or build tools
- ❌ Python, Node.js, or other runtimes
- ❌ Any external dependencies

---

## For Developers

If you want to **modify** or **build** KLang from source:

### Prerequisites

### System Requirements
- **Operating System**: Linux, macOS, or Windows (with MinGW/WSL)
- **Compiler**: GCC 4.9+ or Clang 3.5+
- **Make**: GNU Make 3.81+
- **Memory**: At least 512 MB RAM
- **Disk Space**: 50 MB for source and binaries

### Required Tools
```bash
# On Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential gcc make git

# On macOS (using Homebrew)
brew install gcc make git

# On Windows (using WSL)
# Install WSL first, then follow Ubuntu instructions
```

---

## Installation

### Option 1: Clone from GitHub (Recommended)
```bash
# Clone the repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# Build the compiler
make clean && make

# Verify installation
./klang --version
```

### Option 2: Download Release (Coming Soon)
```bash
# Download the latest release
wget https://github.com/k-kaundal/KLang/releases/latest/klang.tar.gz
tar -xzf klang.tar.gz
cd klang
./klang --version
```

---

## Building from Source

### Step 1: Clone the Repository
```bash
git clone https://github.com/k-kaundal/KLang.git
cd KLang
```

### Step 2: Build the Compiler
```bash
# Clean previous builds
make clean

# Build the compiler
make

# This will create the 'klang' executable
```

### Step 3: Run Tests (Optional)
```bash
# Run the test suite
make test

# You should see:
# === Results: XXX passed, 0 failed ===
```

### Step 4: Install (Optional)
```bash
# Install to /usr/local/bin (requires sudo)
sudo make install

# Or add to PATH manually
export PATH=$PATH:$(pwd)
```

---

## Quick Start

### Using the REPL (Interactive Mode)
The REPL (Read-Eval-Print-Loop) is perfect for trying out KLang:

```bash
./klang repl
```

Try some code:
```klang
>> let x = 10
10
>> x + 5
15
>> let name = "KLang"
"KLang"
>> println("Hello from " + name)
Hello from KLang
```

Exit the REPL with Ctrl+D or type `exit`.

### Running a Script
Create a file `hello.kl`:
```klang
println("Hello, World!")
```

Run it:
```bash
./klang run hello.kl
```

### Compilation and Execution
```bash
# Interpret and run (fastest for development)
./klang run myprogram.kl

# Build to bytecode (future feature)
./klang build myprogram.kl -o myprogram.bc

# Compile to native (future feature)
./klang compile myprogram.kl -o myprogram
```

---

## Your First Program

### Hello World
Create `hello.kl`:
```klang
# This is a comment
println("Hello, World!")
```

Run it:
```bash
./klang run hello.kl
```

### Variables and Types
Create `variables.kl`:
```klang
# Variable declarations
let x: int = 10
let y = 20          # Type inferred
let name: string = "Alice"
let pi: float = 3.14
let isActive: bool = true

# Print values
println(x)
println(name)
println(pi)
```

### Functions
Create `functions.kl`:
```klang
# Function with parameters and return type
fn add(a: int, b: int) -> int {
    return a + b
}

fn greet(name: string) {
    println("Hello, " + name + "!")
}

# Call functions
let sum = add(5, 3)
println(sum)
greet("World")
```

### Classes and Objects
Create `classes.kl`:
```klang
# Define a class
class Person {
    let name: string = ""
    let age: int = 0
    
    # Constructor
    fn init(n: string, a: int) {
        this.name = n
        this.age = a
    }
    
    # Method
    fn introduce() {
        println("I am " + this.name)
        println("I am " + this.age + " years old")
    }
}

# Create an object
let person = new Person("Alice", 30)
person.introduce()
```

---

## Project Structure

### Typical KLang Project Layout
```
myproject/
├── src/
│   ├── main.kl          # Entry point
│   ├── utils.kl         # Utility functions
│   └── models/
│       ├── user.kl      # User class
│       └── product.kl   # Product class
├── tests/
│   ├── test_utils.kl
│   └── test_models.kl
├── examples/
│   └── demo.kl
└── README.md
```

### Running Multi-File Projects
```bash
# Currently, run each file separately
./klang run src/main.kl

# Module system (coming soon)
```

---

## Command-Line Options

### Basic Usage
```bash
klang [command] [options] [file]
```

### Commands
- `repl` - Start interactive REPL
- `run <file>` - Interpret and run a KLang file
- `build <file>` - Build to bytecode (future)
- `compile <file>` - Compile to native (future)
- `--version` - Show version information
- `--help` - Show help message

### Examples
```bash
# Interactive mode
./klang repl

# Run a script
./klang run examples/hello.kl

# Run with debug output (future)
./klang run --debug myprogram.kl

# Check syntax only (future)
./klang check myprogram.kl
```

---

## Environment Setup

### Editor Configuration

#### VS Code
Create `.vscode/settings.json`:
```json
{
  "files.associations": {
    "*.kl": "python"
  }
}
```

#### Vim
Add to `.vimrc`:
```vim
au BufRead,BufNewFile *.kl set filetype=python
```

#### Sublime Text
Add to settings:
```json
{
  "extensions": ["kl"],
  "syntax": "Python"
}
```

---

## Troubleshooting

### Build Errors

**Error: `gcc: command not found`**
```bash
# Install GCC
sudo apt-get install gcc
```

**Error: `make: command not found`**
```bash
# Install Make
sudo apt-get install make
```

### Runtime Errors

**Error: `./klang: No such file or directory`**
```bash
# Build first
make clean && make
```

**Error: `Segmentation fault`**
```bash
# This is a bug. Please report it with:
# 1. Your code that caused the error
# 2. Your operating system
# 3. Output of: ./klang --version
```

### Getting Help

1. **Documentation**: Read the docs in `docs/` directory
2. **Examples**: Check `examples/` for working code
3. **Issues**: Report bugs on GitHub Issues
4. **Community**: Join discussions on GitHub Discussions

---

## Next Steps

### Learn More
1. 📖 Read the [Language Reference](LANGUAGE_REFERENCE.md)
2. 🎓 Follow the [Tutorial](TUTORIAL.md) (coming soon)
3. 💻 Explore [Examples](../examples/)
4. 🔧 Check [API Reference](API_REFERENCE.md)

### Advanced Topics
- [Object-Oriented Programming](OOP_LANGUAGE_SPEC.md)
- [Type System](TYPE_SYSTEM.md) (coming soon)
- [Memory Management](MEMORY_MANAGEMENT.md) (coming soon)
- [Concurrency](CONCURRENCY.md) (future)

### Contributing
Want to contribute? See [CONTRIBUTING.md](../CONTRIBUTING.md)

---

## Quick Reference Card

### Basic Syntax
```klang
# Variables
let x: int = 10
let name = "Alice"

# Functions
fn add(a: int, b: int) -> int {
    return a + b
}

# Classes
class Point {
    let x: int = 0
    let y: int = 0
    
    fn init(x: int, y: int) {
        this.x = x
        this.y = y
    }
}

# Control Flow
if x > 10 {
    println("Big")
} else {
    println("Small")
}

while x < 100 {
    x = x + 1
}

for i in 0 .. 10 {
    println(i)
}
```

---

## Resources

### Official
- 📦 [GitHub Repository](https://github.com/k-kaundal/KLang)
- 📖 [Documentation](../docs/)
- 📝 [Examples](../examples/)

### Community
- 💬 [GitHub Discussions](https://github.com/k-kaundal/KLang/discussions)
- 🐛 [Issue Tracker](https://github.com/k-kaundal/KLang/issues)
- 📢 [Release Notes](https://github.com/k-kaundal/KLang/releases)

---

**Ready to code?** Start with the REPL: `./klang repl`

**Need help?** Check the [docs](../docs/) or open an [issue](https://github.com/k-kaundal/KLang/issues)

Happy coding! 🚀
