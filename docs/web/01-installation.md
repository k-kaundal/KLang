# Installation Guide

## System Requirements

### Minimum Requirements
- **OS**: Linux (Ubuntu 20.04+), macOS (10.15+), or Windows (WSL2)
- **RAM**: 2GB minimum, 4GB recommended
- **Disk**: 500MB for KLang + dependencies
- **Compiler**: GCC 9+ or Clang 10+

### Dependencies
- **libreadline-dev**: For REPL support
- **LLVM 16+**: For JIT compilation (optional but recommended)
- **Make**: Build tool

## Installation Methods

### Option 1: Quick Install (Recommended)

```bash
# Clone the repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# Quick install script
./quick-install.sh
```

The quick install script will:
1. Check system dependencies
2. Install missing dependencies (with sudo)
3. Build KLang
4. Add to PATH (optional)

### Option 2: Manual Installation

#### On Ubuntu/Debian

```bash
# 1. Clone repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# 2. Install dependencies
sudo apt-get update
sudo apt-get install -y build-essential libreadline-dev llvm-16 llvm-16-dev

# 3. Build KLang
make

# 4. Test installation
./klang --version
```

#### On macOS

```bash
# 1. Install Homebrew (if not installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 2. Clone repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# 3. Install dependencies
brew install readline llvm@16

# 4. Build KLang
make

# 5. Test installation
./klang --version
```

#### On Windows (WSL2)

```bash
# 1. Install WSL2 (if not installed)
# Follow: https://docs.microsoft.com/en-us/windows/wsl/install

# 2. Inside WSL2, follow Ubuntu instructions
git clone https://github.com/k-kaundal/KLang.git
cd KLang

sudo apt-get update
sudo apt-get install -y build-essential libreadline-dev llvm-16 llvm-16-dev

make
./klang --version
```

## Verify Installation

After installation, verify KLang is working:

```bash
# Check version
./klang --version

# Run REPL
./klang repl

# Run a test file
echo 'println("Hello, KLang!")' > test.kl
./klang run test.kl
```

Expected output:
```
KLang version 1.0.0-rc
Hello, KLang!
```

## Adding to PATH

### Linux/macOS

Add to your shell configuration (~/.bashrc, ~/.zshrc):

```bash
export PATH="/path/to/KLang:$PATH"
```

Then reload:
```bash
source ~/.bashrc  # or ~/.zshrc
```

Now you can use `klang` from anywhere:
```bash
klang --version
klang repl
```

### Create Symlink (Alternative)

```bash
sudo ln -s /path/to/KLang/klang /usr/local/bin/klang
```

## Build Options

### Standard Build
```bash
make
```

### Clean Build
```bash
make clean
make
```

### Debug Build
```bash
make debug
```

### Run Tests
```bash
make test
./tests/run_all_tests.sh
```

## Troubleshooting

### Error: "readline/readline.h: No such file or directory"

**Solution**: Install libreadline-dev
```bash
sudo apt-get install libreadline-dev
```

### Error: "llvm not found"

**Solution**: Install LLVM
```bash
# Ubuntu/Debian
sudo apt-get install llvm-16 llvm-16-dev

# macOS
brew install llvm@16
```

### Build Fails with "Permission Denied"

**Solution**: Check file permissions
```bash
chmod +x klang
chmod +x scripts/*.sh
```

### REPL Not Working

**Solution**: Ensure readline is installed and rebuild
```bash
sudo apt-get install libreadline-dev
make clean && make
```

## Optional: AI Features Setup

To use AI features, you'll need API keys:

### OpenAI
1. Get API key from: https://platform.openai.com
2. Set environment variable:
```bash
export OPENAI_API_KEY="your-key-here"
```

### Anthropic (Claude)
1. Get API key from: https://console.anthropic.com
2. Set environment variable:
```bash
export ANTHROPIC_API_KEY="your-key-here"
```

### Ollama (Local LLMs)
1. Install Ollama: https://ollama.ai
2. Pull a model:
```bash
ollama pull llama2
```

## Next Steps

Now that KLang is installed:
- [Quick Start Guide](02-quick-start.md) - Write your first program
- [Your First Program](03-your-first-program.md) - Detailed tutorial
- [Language Basics](10-variables-types.md) - Learn the syntax

## Update KLang

To update to the latest version:

```bash
cd /path/to/KLang
git pull origin main
make clean
make
```

---

**Installation complete! Let's start coding!** 🎉
