# KLang Quick Start Guide

Get started with KLang in minutes!

## Installation

### One-Line Install (Recommended)

**Linux/macOS:**
```bash
curl -fsSL https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash
```

Or with wget:
```bash
wget -qO- https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash
```

**Windows (PowerShell):**
```powershell
iex ((New-Object Net.WebClient).DownloadString('https://raw.githubusercontent.com/k-kaundal/KLang/main/install.ps1'))
```

### Manual Installation

1. **Install Dependencies:**
   - **Ubuntu/Debian:**
     ```bash
     sudo apt-get update
     sudo apt-get install -y build-essential git libreadline-dev llvm-dev
     ```
   - **macOS:**
     ```bash
     brew install llvm readline
     ```

2. **Build from Source:**
   ```bash
   git clone https://github.com/k-kaundal/KLang.git
   cd KLang
   make
   sudo make install
   ```

3. **Verify Installation:**
   ```bash
   klang --version
   ```

## Your First KLang Program

### 1. Hello World

Create a file `hello.kl`:
```klang
println("Hello, KLang!")
```

Run it:
```bash
klang run hello.kl
```

### 2. Interactive REPL

Start the REPL:
```bash
klang repl
```

Try some code:
```klang
> let x = 42
> let y = x * 2
> println(y)
84
> exit
```

### 3. Variables and Types

Create `variables.kl`:
```klang
# Variables
let name = "KLang"
let version = 1.0
let isAwesome = true

# Arrays
let numbers = [1, 2, 3, 4, 5]
let mixed = [1, "two", true, 4.5]

# Objects
let user = {
  name: "Alice",
  age: 30,
  active: true
}

println("Language: " + name)
println("Numbers: " + str(numbers))
println("User: " + user.name)
```

### 4. Functions

Create `functions.kl`:
```klang
# Function declaration
fn greet(name) {
  return "Hello, " + name + "!"
}

# Arrow functions
let square = (x) => x * x

# Higher-order functions
let numbers = [1, 2, 3, 4, 5]
let squared = numbers.map((n) => n * n)

println(greet("World"))
println("5 squared = " + str(square(5)))
println("Squared: " + str(squared))
```

### 5. Async/Await

Create `async.kl`:
```klang
# Async function
async fn fetchData(url) {
  # Simulate API call
  await sleep(1000)
  return { status: "success", data: url }
}

# Use async/await
async fn main() {
  let result = await fetchData("https://api.example.com")
  println(result.status)
}

main()
```

## Essential Commands

### Run Code
```bash
klang run script.kl       # Interpret and run
klang build script.kl     # Compile to bytecode and run
klang compile script.kl   # Compile to native executable
```

### Development Tools
```bash
klang check script.kl     # Check syntax
klang typecheck script.kl # Type checking
klang fmt script.kl       # Format code
klang info script.kl      # File statistics
```

### Project Management
```bash
klang init my-project     # Create new project
klang test tests/         # Run tests
klang serve --port=3000   # Start dev server
```

### Package Management
```bash
klang pkg install lodash  # Install package
klang pkg list            # List packages
klang pkg update          # Update packages
```

## Project Structure

Initialize a new project:
```bash
klang init my-app --git
cd my-app
```

Standard structure:
```
my-app/
├── src/           # Source files
│   └── main.kl
├── tests/         # Test files
├── docs/          # Documentation
├── package.kl.json # Package manifest
└── README.md
```

## Next Steps

- 📖 Read the [full documentation](https://github.com/k-kaundal/KLang/tree/main/docs)
- 💡 Check out [examples](https://github.com/k-kaundal/KLang/tree/main/examples)
- 🐛 Report issues on [GitHub](https://github.com/k-kaundal/KLang/issues)
- 💬 Join the community discussions

## Common Issues

### Command not found
Add KLang to your PATH:
```bash
export PATH="$PATH:$HOME/.klang/bin"
```

### LLVM not found
Install LLVM development libraries:
```bash
# Ubuntu/Debian
sudo apt-get install llvm-dev

# macOS
brew install llvm
```

### Build fails
Make sure you have all dependencies:
```bash
# Check installed tools
which gcc make git llvm-config
```

## Getting Help

```bash
klang --help              # General help
klang help <command>      # Command-specific help
klang version             # Version info
klang config              # Show configuration
```

Happy coding with KLang! 🚀
