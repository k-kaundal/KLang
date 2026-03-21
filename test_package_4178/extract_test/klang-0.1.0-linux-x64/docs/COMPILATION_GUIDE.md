# KLang Compilation Guide

## Building KLang from Source

This guide covers everything you need to know about building, compiling, and distributing KLang.

---

## Table of Contents
- [Build System Overview](#build-system-overview)
- [Build Requirements](#build-requirements)
- [Building on Different Platforms](#building-on-different-platforms)
- [Build Options](#build-options)
- [Development Builds](#development-builds)
- [Release Builds](#release-builds)
- [Troubleshooting](#troubleshooting)
- [Cross-Compilation](#cross-compilation)

---

## Build System Overview

KLang uses **GNU Make** as its build system. The build process:

1. **Lexer** → Tokenizes source code
2. **Parser** → Generates Abstract Syntax Tree (AST)
3. **Interpreter** → Executes the AST
4. **VM** → Bytecode virtual machine (in progress)
5. **Compiler** → Native code generation (planned)

### Architecture
```
┌──────────────┐
│ Source (.kl) │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│    Lexer     │  → Tokenization
└──────┬───────┘
       │
       ▼
┌──────────────┐
│    Parser    │  → AST Generation
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ Interpreter  │  → Execution
└──────────────┘
```

---

## Build Requirements

### Minimum Requirements
- **GCC**: 4.9 or later
- **Clang**: 3.5 or later (alternative to GCC)
- **Make**: GNU Make 3.81 or later
- **C Standard**: C99 or later

### Recommended
- **GCC**: 7.0+
- **Memory**: 1 GB RAM
- **Disk**: 100 MB free space

### Checking Your Tools
```bash
# Check GCC version
gcc --version

# Check Make version
make --version

# Check if POSIX compliant
echo $SHELL
```

---

## Building on Different Platforms

### Linux (Ubuntu/Debian)

#### Install Build Tools
```bash
sudo apt-get update
sudo apt-get install build-essential gcc make git
```

#### Build KLang
```bash
# Clone repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# Build
make clean
make

# Test
make test

# Install system-wide (optional)
sudo make install
```

### Linux (Fedora/RHEL)

```bash
# Install tools
sudo dnf groupinstall "Development Tools"
sudo dnf install gcc make git

# Build
make clean && make
```

### macOS

#### Using Xcode Command Line Tools
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Build
make clean && make
```

#### Using Homebrew
```bash
# Install GCC via Homebrew
brew install gcc make git

# Build with Homebrew GCC
make CC=gcc-13 clean
make CC=gcc-13
```

### Windows

#### Option 1: WSL (Recommended)
```bash
# Install WSL and Ubuntu
wsl --install

# Inside WSL, follow Linux instructions
sudo apt-get install build-essential
make clean && make
```

#### Option 2: MinGW
```bash
# Install MinGW-w64
# Download from https://www.mingw-w64.org/

# Add MinGW to PATH
export PATH=/c/mingw64/bin:$PATH

# Build
mingw32-make clean
mingw32-make
```

#### Option 3: Cygwin
```bash
# Install Cygwin with gcc and make packages
# Then build normally
make clean && make
```

---

## Build Options

### Standard Build
```bash
# Clean and build
make clean
make
```

### Debug Build
```bash
# Build with debug symbols
make clean
make DEBUG=1
```

### Optimized Build
```bash
# Build with optimization (-O3)
make clean
make OPTIMIZE=1
```

### Verbose Build
```bash
# See all compiler commands
make clean
make VERBOSE=1
```

### Build Specific Components
```bash
# Build only lexer
make src/lexer.o

# Build only parser
make src/parser.o

# Build only interpreter
make src/interpreter.o
```

---

## Makefile Explained

### Current Makefile Structure
```makefile
# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -Isrc -Iinclude -g

# Source files
SRC = src/lexer.c src/ast.c src/parser.c src/interpreter.c \
      src/vm.c src/compiler.c src/gc.c src/runtime.c \
      src/repl.c src/cli.c

# Object files
OBJ = $(SRC:.c=.o)

# Target executable
TARGET = klang

# Build target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Pattern rule for object files
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Test target
test: $(TEST_SRC)
	$(CC) $(CFLAGS) -Itests -o test_runner $(TEST_SRC) -lm
	./test_runner

# Clean target
clean:
	rm -f src/*.o $(TARGET) test_runner

.PHONY: all test clean
```

### Compiler Flags Explained
- `-Wall` - Enable all warnings
- `-Wextra` - Enable extra warnings
- `-std=c99` - Use C99 standard
- `-D_POSIX_C_SOURCE=200809L` - POSIX compliance
- `-Isrc` - Include src directory
- `-Iinclude` - Include include directory
- `-g` - Include debug symbols
- `-lm` - Link math library

---

## Development Builds

### Quick Development Cycle
```bash
# Edit code...
# Then:
make clean && make && ./klang run test.kl
```

### Watch Mode (Using entr)
```bash
# Install entr
sudo apt-get install entr

# Auto-rebuild on changes
find src -name '*.c' | entr -c make
```

### Development with Debugging
```bash
# Build with debug symbols
make clean
make DEBUG=1

# Run with gdb
gdb ./klang
(gdb) run run examples/hello.kl
```

---

## Release Builds

### Creating a Release Build

#### Step 1: Clean Build
```bash
make clean
```

#### Step 2: Build with Optimizations
```bash
make OPTIMIZE=1
```

#### Step 3: Strip Debug Symbols
```bash
strip klang
```

#### Step 4: Test
```bash
make test
./klang run examples/factorial.kl
./klang run examples/oop_demo.kl
```

#### Step 5: Package
```bash
# Create tarball
tar -czf klang-v0.1.0-linux-x64.tar.gz klang README.md LICENSE examples/

# Or create directory structure
mkdir -p klang-v0.1.0/{bin,docs,examples}
cp klang klang-v0.1.0/bin/
cp README.md LICENSE klang-v0.1.0/
cp -r docs/ klang-v0.1.0/
cp examples/*.kl klang-v0.1.0/examples/
tar -czf klang-v0.1.0.tar.gz klang-v0.1.0/
```

---

## Troubleshooting

### Common Build Errors

#### Error: `lexer.c:XX: error: unknown type name 'Token'`
**Solution**: Missing include. Check that `lexer.h` is included.

#### Error: `undefined reference to 'sqrt'`
**Solution**: Missing math library. Add `-lm` to linker flags.
```bash
make LDFLAGS="-lm"
```

#### Error: `fatal error: stdio.h: No such file or directory`
**Solution**: Missing C standard library headers.
```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# macOS
xcode-select --install
```

#### Error: `make: *** No rule to make target 'klang'`
**Solution**: Corrupted Makefile or wrong directory.
```bash
# Re-clone repository
cd ..
rm -rf KLang
git clone https://github.com/k-kaundal/KLang.git
cd KLang
make
```

### Performance Issues

#### Slow Compilation
```bash
# Use parallel builds
make -j4  # Use 4 cores

# Or auto-detect cores
make -j$(nproc)
```

#### Large Binary Size
```bash
# Strip debug symbols
strip klang

# Before: ~2 MB
# After: ~500 KB
```

### Memory Issues

#### Out of Memory During Build
```bash
# Check available memory
free -h

# Build with less parallelism
make -j1

# Or increase swap
sudo fallocate -l 2G /swapfile
sudo chmod 600 /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile
```

---

## Cross-Compilation

### Linux → Windows
```bash
# Install MinGW cross-compiler
sudo apt-get install mingw-w64

# Cross-compile
make CC=x86_64-w64-mingw32-gcc clean
make CC=x86_64-w64-mingw32-gcc
```

### Linux → macOS
```bash
# Install osxcross
# Follow instructions at https://github.com/tpoechtrager/osxcross

# Cross-compile
make CC=o64-clang clean
make CC=o64-clang
```

### 32-bit → 64-bit
```bash
# On 32-bit system, build for 64-bit
make CFLAGS="-m64" clean
make CFLAGS="-m64"
```

---

## Build Customization

### Custom Compiler
```bash
# Use Clang instead of GCC
make CC=clang clean
make CC=clang

# Use specific GCC version
make CC=gcc-11 clean
make CC=gcc-11
```

### Custom Flags
```bash
# Add custom flags
make CFLAGS="-Wall -Wextra -O3 -march=native" clean
make CFLAGS="-Wall -Wextra -O3 -march=native"
```

### Custom Installation Directory
```bash
# Install to /opt/klang
sudo make PREFIX=/opt/klang install

# Add to PATH
export PATH=/opt/klang/bin:$PATH
```

---

## Continuous Integration

### GitHub Actions Build
```yaml
name: Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Build
        run: make clean && make
      - name: Test
        run: make test
```

### Docker Build
```dockerfile
FROM gcc:latest
WORKDIR /app
COPY . .
RUN make clean && make
CMD ["./klang", "repl"]
```

Build and run:
```bash
docker build -t klang .
docker run -it klang
```

---

## Optimization Levels

### Level 0: Debug (Default)
```bash
make CFLAGS="-O0 -g"
# Best for debugging
# Slowest execution
# Largest binary
```

### Level 1: Basic Optimization
```bash
make CFLAGS="-O1"
# Some optimizations
# Faster than -O0
# Good for development
```

### Level 2: Moderate Optimization
```bash
make CFLAGS="-O2"
# Most optimizations
# Good balance
# Recommended for releases
```

### Level 3: Maximum Optimization
```bash
make CFLAGS="-O3"
# All optimizations
# Fastest execution
# May increase binary size
```

---

## Static Analysis

### Compiler Warnings
```bash
# Enable all warnings
make CFLAGS="-Wall -Wextra -Wpedantic -Werror"
```

### Using Clang Static Analyzer
```bash
# Install clang-tools
sudo apt-get install clang-tools

# Run analyzer
scan-build make
```

### Using Valgrind
```bash
# Install valgrind
sudo apt-get install valgrind

# Check for memory leaks
valgrind --leak-check=full ./klang run test.kl
```

---

## Build Performance

### Typical Build Times
| System | CPU | Time |
|--------|-----|------|
| Linux | Intel i5 | ~2s |
| Linux | Intel i7 | ~1s |
| macOS | M1 | ~1s |
| WSL | Intel i5 | ~3s |

### Speeding Up Builds
```bash
# Use ccache
sudo apt-get install ccache
export CC="ccache gcc"
make

# Parallel builds
make -j$(nproc)

# Incremental builds
# (don't use 'make clean' unless necessary)
make
```

---

## Next Steps

- 📖 [Getting Started Guide](GETTING_STARTED.md)
- 💻 [Language Reference](LANGUAGE_REFERENCE.md)
- 🔧 [Contributing Guide](../CONTRIBUTING.md)

---

**Need Help?** Open an issue on [GitHub](https://github.com/k-kaundal/KLang/issues)
