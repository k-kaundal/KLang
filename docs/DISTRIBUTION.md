# KLang Distribution Guide

## Self-Contained Runtime - No Dependencies Required!

KLang is designed as a **self-contained interpreter** that doesn't require users to have gcc, make, or any build tools installed to **run** KLang programs. Users only need the KLang binary itself.

---

## 📦 For End Users (Running KLang Programs)

### Quick Installation

#### Option 1: Quick Install Script (Recommended)
```bash
# Linux/macOS - One-line install
curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh | bash

# Or download and run
wget https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh
chmod +x quick-install.sh
./quick-install.sh
```

#### Option 2: Download Pre-built Binary
1. Go to [Releases](https://github.com/k-kaundal/KLang/releases)
2. Download the appropriate binary for your platform:
   - `klang-linux-x64.tar.gz` - Linux 64-bit
   - `klang-macos-x64.tar.gz` - macOS Intel
   - `klang-macos-arm64.tar.gz` - macOS Apple Silicon
   - `klang-windows-x64.zip` - Windows 64-bit

3. Extract and install:
```bash
# Linux/macOS
tar -xzf klang-*.tar.gz
cd klang-*/
./install.sh
```

#### Option 3: Build from Source
Only needed if pre-built binaries aren't available for your platform:

```bash
# Clone repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# Build (requires gcc/clang and make)
./build.sh

# Install
./install.sh
```

### Using KLang

Once installed, you can immediately run KLang programs:

```bash
# Interactive REPL
klang repl

# Run a program
klang run myprogram.kl

# No gcc or other compilers needed!
```

### System Requirements

**To RUN KLang programs:**
- ✅ Just the KLang binary
- ✅ Standard C library (comes with all OSes)
- ❌ NO compiler needed
- ❌ NO make needed
- ❌ NO build tools needed

**To BUILD KLang from source (developers only):**
- gcc or clang
- make
- Standard development tools

---

## 🏗️ For Developers (Building KLang)

### Cross-Platform Build System

KLang includes build scripts for all major platforms:

#### Linux/macOS: `build.sh`
```bash
./build.sh              # Basic build
./build.sh --with-tests # Build and test
./build.sh --package    # Build and create distribution package
```

#### Windows: `build.bat`
```bat
build.bat              # Build with MinGW or MSVC
```

### Building for Distribution

#### 1. Build for your platform
```bash
./build.sh --package
```

This creates a distribution package in `dist/`:
- Linux: `klang-{version}-linux-{arch}.tar.gz`
- macOS: `klang-{version}-macos-{arch}.tar.gz`
- Windows: `klang-{version}-windows-{arch}.zip`

#### 2. Package contents
Each package includes:
- `klang` binary (or `klang.exe` on Windows)
- `README.md`
- `examples/` directory
- `docs/` directory

#### 3. Installation scripts
- `install.sh` - Install KLang to system or user directory
- `quick-install.sh` - Universal installer for end users

---

## 🌍 Cross-Platform Support

### Supported Platforms

| Platform | Architecture | Binary Name | Status |
|----------|-------------|-------------|---------|
| Linux | x86_64 | klang | ✅ Tested |
| Linux | ARM64 | klang | ✅ Should work |
| macOS | x86_64 (Intel) | klang | ✅ Should work |
| macOS | ARM64 (Apple Silicon) | klang | ✅ Should work |
| Windows | x86_64 | klang.exe | ✅ With MinGW |

### Dependencies

KLang only requires standard system libraries that are present on all platforms:

**Linux:**
- `libc.so.6` (GNU C Library)
- `libm.so.6` (Math library)

**macOS:**
- System C library (built-in)
- Math library (built-in)

**Windows:**
- `msvcrt.dll` (C runtime)
- Or MinGW runtime libraries

These are **always** present on the operating system - no installation needed!

---

## 📝 Building Release Binaries

### For Maintainers

#### 1. Prepare Release
```bash
# Update version in build.sh
KLANG_VERSION="X.Y.Z"

# Tag release
git tag -a vX.Y.Z -m "Release vX.Y.Z"
git push origin vX.Y.Z
```

#### 2. Build for All Platforms

**Linux:**
```bash
./build.sh --package
# Creates: dist/klang-X.Y.Z-linux-x64.tar.gz
```

**macOS (Intel):**
```bash
./build.sh --package
# Creates: dist/klang-X.Y.Z-macos-x64.tar.gz
```

**macOS (Apple Silicon):**
```bash
./build.sh --package
# Creates: dist/klang-X.Y.Z-macos-arm64.tar.gz
```

**Windows (on MinGW/MSYS2):**
```bash
./build.sh --package
# Creates: dist/klang-X.Y.Z-windows-x64.zip
```

#### 3. Upload to GitHub Releases
1. Go to GitHub repository
2. Create new release from tag
3. Upload all distribution packages
4. Publish release

---

## 🚀 Deployment Scenarios

### Scenario 1: Developer Installing from Source
```bash
git clone https://github.com/k-kaundal/KLang.git
cd KLang
./build.sh
./install.sh
klang repl
```

### Scenario 2: End User with Pre-built Binary
```bash
curl -sSL https://install.klang.dev | bash
klang repl
```

### Scenario 3: Docker Deployment
```dockerfile
FROM alpine:latest
RUN apk add --no-cache libc6-compat
COPY klang /usr/local/bin/
ENTRYPOINT ["klang"]
```

### Scenario 4: CI/CD Environment
```yaml
- name: Install KLang
  run: |
    curl -sSL https://install.klang.dev | bash
    export PATH="$PATH:$HOME/.klang/bin"
    klang --version
```

---

## 🔧 Troubleshooting

### "command not found: klang"

**Solution:** Add KLang to your PATH:
```bash
# Add to ~/.bashrc or ~/.zshrc
export PATH="$PATH:$HOME/.klang/bin"

# Then reload
source ~/.bashrc
```

### "cannot execute binary file"

**Possible causes:**
1. Downloaded binary for wrong architecture
2. File permissions issue

**Solution:**
```bash
# Check your architecture
uname -m

# Make binary executable
chmod +x klang
```

### Windows: "The code execution cannot proceed because..."

**Solution:** Install Visual C++ Redistributable:
- Download from Microsoft website
- Or use MinGW-compiled version

---

## 📊 Platform-Specific Notes

### Linux
- Works on all major distributions
- Only requires glibc (present on all systems)
- Can be statically linked for maximum portability

### macOS
- Works on both Intel and Apple Silicon
- No additional dependencies needed
- May need to approve in Security & Privacy on first run

### Windows
- Best compatibility with MinGW build
- MSVC build requires Visual C++ runtime
- Can use WSL for Linux-like experience

---

## 🎯 Key Points

### What Users Need to Know:

1. **KLang is self-contained** - No compiler needed to run programs
2. **Small binary** - ~140KB, easy to distribute
3. **Cross-platform** - Works on Linux, macOS, Windows
4. **No dependencies** - Just download and run
5. **Easy installation** - One-line install script

### What Makes This Possible:

- KLang is an **interpreter**, not a compiler
- Programs are executed directly by the KLang runtime
- No need for gcc/clang at runtime
- Only standard system libraries required
- Self-contained execution model

---

## 📚 Additional Resources

- [Getting Started Guide](GETTING_STARTED.md)
- [Language Reference](LANGUAGE_REFERENCE.md)
- [Compilation Guide](COMPILATION_GUIDE.md)
- [Contributing Guide](development/CONTRIBUTING.md)

---

## 🎉 Summary

**For End Users:**
```bash
# Install once
curl -sSL https://install.klang.dev | bash

# Use forever
klang run anything.kl
```

**No gcc, no make, no build tools needed!** 🚀
