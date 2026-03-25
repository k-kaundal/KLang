# KLang Installation Guide

## Quick Install (Recommended)

### One-Line Install

The easiest way to install KLang is with our universal installer:

```bash
# Using curl
curl -fsSL https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash

# Or using wget
wget -qO- https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash
```

This will:
- ✅ Detect your OS and architecture
- ✅ Install dependencies automatically
- ✅ Build KLang from source
- ✅ Install to `~/.klang/bin`
- ✅ Automatically configure your PATH
- ✅ Work on Linux, macOS, and Windows (WSL)

---

## Installation Methods

### Method 1: Universal Installer (Easiest)

```bash
curl -fsSL https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash
```

**After installation:**
```bash
# Reload your shell
source ~/.bashrc  # For bash
source ~/.zshrc   # For zsh

# Verify
klang --version
```

---

### Method 2: Manual Build

```bash
# Clone repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# Build
make clean
make

# Test (optional)
./klang --version

# Install system-wide (requires sudo)
sudo make install

# Or install to user directory (no sudo)
make install-user
```

---

### Method 3: Using Makefile Only

```bash
# Clone and build
git clone https://github.com/k-kaundal/KLang.git
cd KLang
make

# Choose installation method:

# System-wide (requires sudo) - installs to /usr/local/bin
sudo make install

# User directory (no sudo) - installs to ~/.local/bin  
make install-user

# Custom location
make install PREFIX=/custom/path
```

**Make targets:**
- `make` - Build KLang
- `make install` - Install system-wide
- `make install-user` - Install to user directory
- `make uninstall` - Remove system installation
- `make uninstall-user` - Remove user installation
- `make test` - Run tests
- `make clean` - Clean build files

---

### Method 4: Docker (Coming Soon)

```bash
# Pull image
docker pull klang/klang:latest

# Run REPL
docker run -it klang/klang repl

# Run script
docker run -v $(pwd):/code klang/klang run /code/script.kl
```

---

### Method 5: Package Managers (Coming Soon)

```bash
# Homebrew (macOS/Linux)
brew install klang

# apt (Ubuntu/Debian)
sudo apt install klang

# yum (CentOS/RHEL)
sudo yum install klang

# pacman (Arch Linux)
sudo pacman -S klang
```

---

## Requirements

### Linux

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential git libreadline-dev llvm-dev
```

**CentOS/RHEL:**
```bash
sudo yum groupinstall -y 'Development Tools'
sudo yum install -y git readline-devel llvm-devel
```

**Arch Linux:**
```bash
sudo pacman -S base-devel git readline llvm
```

### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install required dependencies (includes LLVM)
brew install readline llvm

# Add LLVM to PATH (important!)
echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

### Windows

**Option 1: Use Windows Subsystem for Linux (WSL) - Recommended:**
```bash
# In PowerShell (as Administrator)
wsl --install

# Then follow Linux instructions
```

**Option 2: Native Windows (MSYS2):**
```bash
# Install MSYS2 from https://www.msys2.org/
# Then in MSYS2 terminal:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-llvm make git
```

**Option 3: Native Windows (Chocolatey):**
```powershell
# In PowerShell (as Administrator)
choco install git mingw llvm make
```

---

## Verify Installation

After installation, verify KLang is working:

```bash
# Check version
klang --version

# Try REPL
klang repl

# Run example
klang run examples/hello.kl

# Show help
klang help
```

---

## PATH Configuration

The installer automatically adds KLang to your PATH. If it doesn't work:

### Bash
```bash
echo 'export PATH="$PATH:$HOME/.klang/bin"' >> ~/.bashrc
source ~/.bashrc
```

### Zsh
```bash
echo 'export PATH="$PATH:$HOME/.klang/bin"' >> ~/.zshrc
source ~/.zshrc
```

### Fish
```bash
echo 'set -gx PATH $PATH $HOME/.klang/bin' >> ~/.config/fish/config.fish
source ~/.config/fish/config.fish
```

---

## Troubleshooting

### "klang: command not found"

**Solution 1:** Reload your shell configuration
```bash
source ~/.bashrc  # or ~/.zshrc
```

**Solution 2:** Add to PATH manually
```bash
export PATH="$PATH:$HOME/.klang/bin"
```

**Solution 3:** Use full path
```bash
$HOME/.klang/bin/klang --version
```

### Build errors

**Missing readline:**
```bash
# Ubuntu/Debian
sudo apt-get install libreadline-dev

# macOS
brew install readline

# CentOS/RHEL
sudo yum install readline-devel
```

**Missing LLVM (optional):**
```bash
# Ubuntu/Debian
sudo apt-get install llvm-16-dev

# macOS
brew install llvm@16

# CentOS/RHEL
sudo yum install llvm-devel
```

### Permission denied

If you get "Permission denied" during system install:
```bash
# Use sudo
sudo make install

# Or install to user directory (no sudo required)
make install-user
```

---

## Uninstallation

### Remove KLang

```bash
# If installed system-wide
sudo make uninstall

# If installed to user directory
make uninstall-user

# Or manually remove
rm -rf ~/.klang
```

### Remove from PATH

Edit your shell configuration file (~/.bashrc, ~/.zshrc, etc.) and remove the KLang PATH line.

---

## Installation Locations

### Universal Installer
- **Binary:** `~/.klang/bin/klang`
- **Examples:** `~/.klang/examples/`
- **Docs:** `~/.klang/docs/`
- **Stdlib:** `~/.klang/stdlib/`

### System Install (sudo make install)
- **Binary:** `/usr/local/bin/klang`
- **Data:** `/usr/local/share/klang/`

### User Install (make install-user)
- **Binary:** `~/.local/bin/klang`
- **Data:** `~/.local/share/klang/`

### Custom Install (make install PREFIX=/path)
- **Binary:** `/path/bin/klang`
- **Data:** `/path/share/klang/`

---

## Update KLang

```bash
# Using universal installer
curl -fsSL https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash

# Or manually
cd KLang
git pull origin main
make clean
make
sudo make install  # or make install-user
```

---

## Environment Variables

| Variable | Default | Description |
|----------|---------|-------------|
| `KLANG_VERSION` | `main` | Git branch/tag to install |
| `KLANG_INSTALL_DIR` | `~/.klang` | Installation directory |
| `PREFIX` | `/usr/local` | System install prefix |

**Examples:**
```bash
# Install specific version
KLANG_VERSION=v1.0.0 curl -fsSL ... | bash

# Install to custom directory
KLANG_INSTALL_DIR=~/tools/klang curl -fsSL ... | bash

# System install to /opt
make install PREFIX=/opt
```

---

## Next Steps

After installation:

1. **Try the REPL:** `klang repl`
2. **Run examples:** `klang run ~/.klang/examples/hello.kl`
3. **Read docs:** [GitHub Documentation](https://github.com/k-kaundal/KLang)
4. **Join community:** [Discord](https://discord.gg/klang)

---

## Getting Help

- **Documentation:** https://github.com/k-kaundal/KLang/docs
- **Issues:** https://github.com/k-kaundal/KLang/issues
- **Discussions:** https://github.com/k-kaundal/KLang/discussions
- **Discord:** https://discord.gg/klang

---

**Happy coding with KLang! 🚀**
