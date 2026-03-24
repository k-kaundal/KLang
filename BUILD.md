# KLang Build Instructions

## Quick Start

### Prerequisites

Install required dependencies:

```bash
sudo apt-get update
sudo apt-get install -y build-essential libreadline-dev llvm-16-dev
```

### Build

```bash
make clean
make
```

### Verify

```bash
./klang --version
./klang repl
```

## Troubleshooting Build Issues

### Issue: "readline/readline.h: No such file or directory"

**Solution:**
```bash
sudo apt-get install -y libreadline-dev
```

### Issue: "llvm-c/Core.h: file not found"

**Solution:**
```bash
sudo apt-get install -y llvm-16-dev
```

### Issue: "llvm-config-16: Command not found"

This is usually just a warning, not the actual error. Look for the real compilation error further down in the build output.

If LLVM 16 is genuinely missing:
```bash
sudo apt-get install -y llvm-16 llvm-16-dev
```

## Dependencies

- **GCC**: C compiler
- **Make**: Build system
- **libreadline-dev**: REPL interactive input
- **llvm-16-dev**: LLVM backend for code generation
- **libm**: Math library (usually included)

## Platform-Specific Notes

### Ubuntu/Debian
```bash
sudo apt-get install -y build-essential libreadline-dev llvm-16-dev
```

### Fedora/RHEL
```bash
sudo dnf install gcc make readline-devel llvm16-devel
```

### macOS (with Homebrew)
```bash
brew install readline llvm@16
```

You may need to set environment variables:
```bash
export LDFLAGS="-L/opt/homebrew/opt/readline/lib"
export CPPFLAGS="-I/opt/homebrew/opt/readline/include"
export PKG_CONFIG_PATH="/opt/homebrew/opt/llvm@16/lib/pkgconfig"
```

## Playground Setup

After building KLang, set up the playground:

```bash
# Install Python dependencies
pip install -r playground-requirements.txt

# Start playground server
./start-playground.sh

# Open playground.html in browser
```

## Common Build Errors

### Error: Missing LLVM libraries at link time

If you get undefined reference errors for LLVM functions:
```bash
# Check LLVM installation
llvm-config-16 --version
llvm-config-16 --libs

# Reinstall if needed
sudo apt-get install --reinstall llvm-16-dev
```

### Error: Conflicting LLVM versions

If you have multiple LLVM versions:
```bash
# Update Makefile to use specific version
# Edit line 2: LLVM_CONFIG = llvm-config-16
```

### Error: Permission denied when running klang

```bash
chmod +x klang
```

## Build Output

Successful build creates:
- `klang` - Main compiler binary (~579 KB)
- `src/*.o` - Object files (can be cleaned with `make clean`)

## Testing

Run the test suite:
```bash
make test
```

Run specific example:
```bash
./klang run examples/hello.kl
```

## Clean Build

Remove all build artifacts:
```bash
make clean
```

## Getting Help

If you encounter issues not covered here:

1. Check the full error message (not just the first error)
2. Verify all dependencies are installed
3. Try a clean build: `make clean && make`
4. Check GitHub issues: https://github.com/k-kaundal/KLang/issues

## Development

For development builds with debug symbols (already included by default):
```bash
make clean
make
```

For release builds (optimized, no debug symbols):
```bash
# Edit Makefile: Add -O3 to CFLAGS, remove -g
make clean
make
```
