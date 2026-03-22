# KLang Scripts Guide

**Complete guide to all build, install, and utility scripts** 📜

**Last Updated**: March 22, 2026  
**All Paths Verified**: ✅ Yes

---

## 📁 Script Organization

All scripts are located in the `scripts/` directory:

```
scripts/
├── build.sh                  # Unix/Linux/macOS build script
├── build.bat                 # Windows build script
├── install.sh                # Unix/Linux/macOS installer
├── install-windows.ps1       # Windows PowerShell installer
├── quick-install.sh          # One-line universal installer
├── setup.sh                  # Development environment setup
└── validate-docs-links.sh    # Documentation link validator
```

---

## 🔨 Build Scripts

### scripts/build.sh (Linux/macOS)

**Purpose**: Cross-platform build script for Unix-like systems

**Usage**:
```bash
cd /path/to/KLang
scripts/build.sh                # Basic build
scripts/build.sh --with-tests   # Build and run tests
scripts/build.sh --package      # Build and create distribution package
scripts/build.sh --help         # Show help
```

**Features**:
- ✅ Automatic platform detection (Linux/macOS, x64/ARM64)
- ✅ Checks for required tools (gcc/clang, make)
- ✅ Clean build process
- ✅ Optional testing
- ✅ Package creation with tar.gz
- ✅ Checksum generation

**Requirements**:
- gcc or clang
- make
- tar (for packaging)

**Output**:
- Binary: `./klang`
- Package: `dist/klang-{version}-{platform}-{arch}.tar.gz`

---

### scripts/build.bat (Windows)

**Purpose**: Build script for Windows using MinGW or MSVC

**Usage**:
```cmd
cd C:\path\to\KLang
scripts\build.bat
```

**Features**:
- ✅ Auto-detects MinGW (gcc) or MSVC (cl)
- ✅ Compiles all source files
- ✅ Links with math library
- ✅ Clean error messages

**Requirements**:
- MinGW-w64 OR Visual Studio Build Tools
- Windows CMD or PowerShell

**Output**:
- Binary: `klang.exe`

---

## 📦 Installation Scripts

### scripts/install.sh (Linux/macOS)

**Purpose**: Install KLang to system or user directory

**Usage**:
```bash
# Install to system (requires sudo)
scripts/install.sh
scripts/install.sh --system

# Install to user directory (~/.local/bin)
scripts/install.sh --user

# Uninstall
scripts/install.sh --uninstall

# Show help
scripts/install.sh --help
```

**Installation Locations**:
- **System**: `/usr/local/bin/klang`
- **User**: `~/.local/bin/klang`

**Features**:
- ✅ System-wide or user installation
- ✅ Automatic PATH detection
- ✅ Shell-specific PATH setup (bash, zsh, fish)
- ✅ Copies examples and docs to share directory
- ✅ Uninstall capability
- ✅ Installation verification

**Requirements**:
- KLang binary must be built first (`scripts/build.sh`)
- sudo (for system installation)

---

### scripts/install-windows.ps1 (Windows)

**Purpose**: PowerShell installer for Windows

**Usage**:
```powershell
# Install for current user (default, no admin required)
scripts\install-windows.ps1
scripts\install-windows.ps1 -User

# Install system-wide (requires admin)
scripts\install-windows.ps1 -System

# Show help
scripts\install-windows.ps1 -Help
```

**Installation Locations**:
- **User**: `%LOCALAPPDATA%\KLang\bin\klang.exe`
- **System**: `%ProgramFiles%\KLang\bin\klang.exe`

**Features**:
- ✅ User or system-wide installation
- ✅ Automatic PATH configuration
- ✅ Copies examples and documentation
- ✅ PATH verification
- ✅ Restart notification if needed

**Requirements**:
- KLang binary must be built first (`scripts\build.bat`)
- Administrator rights (for system installation only)

---

## 🚀 Quick Install Script

### scripts/quick-install.sh (Universal)

**Purpose**: One-line installer for end users (downloads or builds from source)

**Usage**:
```bash
# Standard install (run from anywhere)
curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/scripts/quick-install.sh | bash

# Or with wget
wget -qO- https://raw.githubusercontent.com/k-kaundal/KLang/main/scripts/quick-install.sh | bash

# Or download and review first
curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/scripts/quick-install.sh > /tmp/install.sh
less /tmp/install.sh
bash /tmp/install.sh
```

**Installation Location**:
- `~/.klang/bin/klang`

**Features**:
- ✅ Platform detection (Linux/macOS, x64/ARM64)
- ✅ Attempts to download pre-built binary
- ✅ Falls back to building from source if needed
- ✅ Automatic PATH setup for bash/zsh/fish
- ✅ Copies examples and documentation
- ✅ Shell-specific instructions

**Requirements**:
- curl or wget
- For build: git, gcc/clang, make

**Note**: This is designed for end users who want the simplest installation experience.

---

## 🛠️ Development Scripts

### scripts/setup.sh (Development)

**Purpose**: Quick setup for KLang development environment

**Usage**:
```bash
scripts/setup.sh
```

**What it does**:
1. ✅ Checks for required tools (gcc, make, git)
2. ✅ Builds KLang
3. ✅ Runs test suite
4. ✅ Verifies example programs
5. ✅ Shows quick start information

**Requirements**:
- gcc
- make
- git

**Use case**: First-time contributors setting up development environment

---

### scripts/validate-docs-links.sh (Quality)

**Purpose**: Validates all documentation links

**Usage**:
```bash
scripts/validate-docs-links.sh
```

**Features**:
- ✅ Scans all markdown files
- ✅ Extracts and validates links
- ✅ Checks file existence
- ✅ Resolves relative paths
- ✅ Skips external URLs
- ✅ Colored output
- ✅ Summary statistics

**Use case**: Pre-commit validation, CI/CD integration

---

## 🧪 Test Scripts

Located in `tests/` directory:

### tests/run_all_tests.sh

**Purpose**: Comprehensive test suite runner

**Usage**:
```bash
tests/run_all_tests.sh
```

**What it tests**:
- Unit tests (C)
- Feature tests (KLang)
- Example programs

---

### tests/run_phase1_tests.sh

**Purpose**: JavaScript Phase 1 feature tests

**Usage**:
```bash
tests/run_phase1_tests.sh
```

**Tests**:
- var/let/const
- Arrow functions
- Template literals
- Enhanced objects

---

### tests/test_modules.sh

**Purpose**: ES6 module system tests

**Usage**:
```bash
tests/test_modules.sh
```

---

### tests/test_spread_rest.sh

**Purpose**: Spread and rest operator tests

**Usage**:
```bash
tests/test_spread_rest.sh
```

---

### tests/test-archive.sh

**Purpose**: Cross-platform tar archive compatibility test

**Usage**:
```bash
tests/test-archive.sh
```

**Purpose**: Ensures GNU tar (Linux) and BSD tar (macOS) compatibility

---

## 📋 Path Conventions

### When Running Scripts

**From repository root**:
```bash
scripts/build.sh          # Correct
scripts/install.sh        # Correct
tests/run_all_tests.sh    # Correct
```

**Don't use** (unless at root):
```bash
./build.sh               # Wrong - script not at root
./install.sh             # Wrong - script not at root
```

### In Documentation

Always reference with `scripts/` prefix:
```markdown
Run `scripts/build.sh` to build KLang.
```

### In Scripts

**Test scripts** that need the klang binary:
```bash
# Method 1: Detect repo root
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
"$REPO_ROOT/klang" run test.k

# Method 2: Change to repo root first
cd "$(dirname "$0")/.."
./klang run test.k
```

**Install scripts** assume run from repo root:
```bash
# This works because user runs: scripts/install.sh
if [ ! -f "./klang" ]; then
    echo "Error: klang binary not found!"
    exit 1
fi
```

---

## 🔧 Common Workflows

### For End Users

**Install KLang (easiest)**:
```bash
curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/scripts/quick-install.sh | bash
source ~/.bashrc  # or ~/.zshrc
klang repl
```

---

### For Contributors

**Setup development environment**:
```bash
git clone https://github.com/k-kaundal/KLang.git
cd KLang
scripts/setup.sh
```

**Build and test**:
```bash
scripts/build.sh --with-tests
```

**Install locally for testing**:
```bash
scripts/install.sh --user
```

---

### For Maintainers

**Create release package**:
```bash
scripts/build.sh --package
# Package created in: dist/klang-{version}-{platform}-{arch}.tar.gz
```

**Validate documentation**:
```bash
scripts/validate-docs-links.sh
```

**Run all tests**:
```bash
tests/run_all_tests.sh
```

---

## ⚠️ Important Notes

### Script Locations

**All scripts are in `scripts/` directory** except:
- Test scripts in `tests/`
- Build configuration in `Makefile`

### Running Scripts

**Always run from repository root**:
```bash
cd /path/to/KLang
scripts/build.sh       # ✅ Correct
```

**Don't cd into scripts directory**:
```bash
cd /path/to/KLang/scripts
./build.sh            # ❌ Wrong - won't find source files
```

### Windows Path Separators

Use backslash `\` for Windows:
```cmd
scripts\build.bat
```

Use forward slash `/` for PowerShell:
```powershell
scripts/install-windows.ps1
```

Or use backslash (PowerShell accepts both):
```powershell
scripts\install-windows.ps1
```

---

## 🐛 Troubleshooting

### "klang binary not found"

**Solution**: Build first
```bash
scripts/build.sh
```

### "Permission denied"

**Solution**: Make script executable
```bash
chmod +x scripts/build.sh
chmod +x scripts/install.sh
```

### "No C compiler found"

**Solution**: Install build tools

**Linux (Debian/Ubuntu)**:
```bash
sudo apt-get install build-essential
```

**Linux (CentOS/RHEL)**:
```bash
sudo yum groupinstall 'Development Tools'
```

**macOS**:
```bash
xcode-select --install
```

**Windows**:
- Install MinGW-w64 or MSYS2
- OR Visual Studio Build Tools

### "Command not found: klang" after install

**Solution**: Reload shell configuration
```bash
source ~/.bashrc    # or ~/.zshrc
```

Or restart terminal

See: [PATH Troubleshooting Guide](docs/PATH_TROUBLESHOOTING.md)

---

## 📊 Script Summary

| Script | Purpose | Platform | Requires Build |
|--------|---------|----------|----------------|
| build.sh | Build KLang | Unix | No |
| build.bat | Build KLang | Windows | No |
| install.sh | Install binary | Unix | Yes |
| install-windows.ps1 | Install binary | Windows | Yes |
| quick-install.sh | Download/build+install | Unix | No |
| setup.sh | Dev environment | Unix | No |
| validate-docs-links.sh | Check docs | All | No |

---

## ✅ Verification

All scripts have been verified with correct paths as of March 22, 2026.

**Test any script**:
```bash
cd /path/to/KLang
scripts/build.sh --help
scripts/install.sh --help
```

---

**Status**: ✅ All paths correct and verified  
**Last Review**: March 22, 2026  
**Maintained By**: KLang Core Team
