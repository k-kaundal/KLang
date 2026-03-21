# KLang PATH Troubleshooting Guide

This guide helps you resolve "command not found: klang" errors after installation.

## Table of Contents

- [Quick Fixes](#quick-fixes)
- [Platform-Specific Guides](#platform-specific-guides)
  - [macOS](#macos)
  - [Linux](#linux)
  - [Windows](#windows)
- [Understanding the Issue](#understanding-the-issue)
- [Manual Installation](#manual-installation)
- [Verification](#verification)

---

## Quick Fixes

### 1. Restart Your Terminal

The most common fix - simply close and reopen your terminal application.

### 2. Reload Your Shell Configuration

Instead of restarting, you can reload your configuration:

**For Bash:**
```bash
source ~/.bashrc
```

**For Zsh (macOS default):**
```bash
source ~/.zshrc
```

**For Fish:**
```bash
source ~/.config/fish/config.fish
```

### 3. Use the Full Path

If PATH isn't set up yet, you can use the full path to klang:

```bash
~/.klang/bin/klang repl
```

---

## Platform-Specific Guides

### macOS

#### Issue: "command not found: klang"

**Step 1: Check if klang is installed**
```bash
ls -la ~/.klang/bin/klang
```

If this shows the file, klang is installed correctly.

**Step 2: Identify your shell**
```bash
echo $SHELL
```

Common outputs:
- `/bin/zsh` - You're using Zsh (macOS Catalina+ default)
- `/bin/bash` - You're using Bash

**Step 3: Add to PATH (Zsh)**

If you're using Zsh (most modern Macs):

```bash
# Add to your .zshrc
echo 'export PATH="$PATH:$HOME/.klang/bin"' >> ~/.zshrc

# Reload configuration
source ~/.zshrc

# Verify
which klang
```

**Step 4: Add to PATH (Bash)**

If you're using Bash:

```bash
# Add to your .bashrc
echo 'export PATH="$PATH:$HOME/.klang/bin"' >> ~/.bashrc

# Reload configuration
source ~/.bashrc

# Verify
which klang
```

**Step 5: Alternative - Add to .zprofile or .bash_profile**

Some terminals only read profile files on startup:

For Zsh:
```bash
echo 'export PATH="$PATH:$HOME/.klang/bin"' >> ~/.zprofile
```

For Bash:
```bash
echo 'export PATH="$PATH:$HOME/.klang/bin"' >> ~/.bash_profile
```

Then restart your terminal.

---

### Linux

#### Issue: "command not found: klang"

**Step 1: Check if klang is installed**
```bash
ls -la ~/.klang/bin/klang
```

**Step 2: Identify your shell**
```bash
echo $SHELL
```

**Step 3: Add to PATH (Bash - most common)**
```bash
# Add to your .bashrc
echo 'export PATH="$PATH:$HOME/.klang/bin"' >> ~/.bashrc

# Reload configuration
source ~/.bashrc

# Verify
which klang
```

**Step 4: Add to PATH (Zsh)**
```bash
# Add to your .zshrc
echo 'export PATH="$PATH:$HOME/.klang/bin"' >> ~/.zshrc

# Reload configuration
source ~/.zshrc

# Verify
which klang
```

**Step 5: System-wide installation alternative**

If you prefer system-wide installation (requires sudo):

```bash
cd /path/to/KLang/repo
./build.sh
sudo ./install.sh --system
```

This installs to `/usr/local/bin` which is typically already in PATH.

**Step 6: User-level installation alternative**

Install to `~/.local/bin` (no sudo required):

```bash
cd /path/to/KLang/repo
./build.sh
./install.sh --user
```

Then ensure `~/.local/bin` is in your PATH:

```bash
echo 'export PATH="$PATH:$HOME/.local/bin"' >> ~/.bashrc
source ~/.bashrc
```

---

### Windows

#### Issue: "klang is not recognized as an internal or external command"

**Step 1: Check if klang is installed**

Open PowerShell or Command Prompt:

```powershell
Get-Command klang -ErrorAction SilentlyContinue
```

Or check manually:
```powershell
Test-Path "$env:LOCALAPPDATA\KLang\bin\klang.exe"
```

**Step 2: Use the installation script**

From the KLang repository directory:

```powershell
.\install-windows.ps1 -User
```

This automatically adds KLang to your PATH.

**Step 3: Manual PATH setup (if script fails)**

1. Press `Win + X` and select "System"
2. Click "Advanced system settings"
3. Click "Environment Variables"
4. Under "User variables", select "Path" and click "Edit"
5. Click "New" and add: `%LOCALAPPDATA%\KLang\bin`
6. Click "OK" on all dialogs
7. **Restart your terminal/PowerShell**

**Step 4: Verify in new terminal**

Close all PowerShell/Command Prompt windows and open a new one:

```powershell
klang repl
```

**Step 5: Alternative - Use full path**

If PATH doesn't work, use the full path:

```powershell
& "$env:LOCALAPPDATA\KLang\bin\klang.exe" repl
```

---

## Understanding the Issue

### What is PATH?

`PATH` is an environment variable that tells your operating system where to look for executable programs. When you type `klang`, your shell searches all directories listed in `PATH` to find the `klang` executable.

### Why "command not found"?

This error means:
1. The `klang` executable doesn't exist, OR
2. The directory containing `klang` is not in your `PATH`

### Where does KLang install?

**Quick install (curl/wget):**
- Binary: `~/.klang/bin/klang`
- Examples: `~/.klang/examples/`
- Docs: `~/.klang/docs/`

**Manual install options:**
- System: `/usr/local/bin/klang` (requires sudo)
- User: `~/.local/bin/klang` (no sudo)
- Windows User: `%LOCALAPPDATA%\KLang\bin\klang.exe`
- Windows System: `%ProgramFiles%\KLang\bin\klang.exe`

---

## Manual Installation

If automatic installation fails, you can install manually:

### macOS/Linux

```bash
# 1. Build KLang
cd /path/to/KLang/repo
make clean
make

# 2. Create installation directory
mkdir -p ~/.klang/bin

# 3. Copy binary
cp klang ~/.klang/bin/

# 4. Make executable
chmod +x ~/.klang/bin/klang

# 5. Add to PATH (choose your shell)
# For Zsh:
echo 'export PATH="$PATH:$HOME/.klang/bin"' >> ~/.zshrc
source ~/.zshrc

# For Bash:
echo 'export PATH="$PATH:$HOME/.klang/bin"' >> ~/.bashrc
source ~/.bashrc
```

### Windows

```powershell
# 1. Build KLang
cd \path\to\KLang\repo
.\build.bat

# 2. Create installation directory
New-Item -ItemType Directory -Force -Path "$env:LOCALAPPDATA\KLang\bin"

# 3. Copy binary
Copy-Item klang.exe "$env:LOCALAPPDATA\KLang\bin\"

# 4. Add to PATH (PowerShell as Admin)
$userPath = [Environment]::GetEnvironmentVariable("Path", "User")
$klangPath = "$env:LOCALAPPDATA\KLang\bin"
[Environment]::SetEnvironmentVariable("Path", "$userPath;$klangPath", "User")

# 5. Restart terminal and verify
klang repl
```

---

## Verification

### Check if klang is installed

**macOS/Linux:**
```bash
# Check binary exists
ls -la ~/.klang/bin/klang

# Check if executable
file ~/.klang/bin/klang

# Try to run
~/.klang/bin/klang repl
```

**Windows:**
```powershell
# Check binary exists
Test-Path "$env:LOCALAPPDATA\KLang\bin\klang.exe"

# Try to run
& "$env:LOCALAPPDATA\KLang\bin\klang.exe" repl
```

### Check PATH configuration

**macOS/Linux:**
```bash
# View current PATH
echo $PATH

# Check if klang directory is in PATH
echo $PATH | grep -o '\.klang/bin'

# Find klang in PATH
which klang

# View shell configuration
cat ~/.zshrc | grep -i klang  # For Zsh
cat ~/.bashrc | grep -i klang  # For Bash
```

**Windows:**
```powershell
# View current PATH
$env:Path -split ';'

# Check if klang is in PATH
$env:Path -like '*KLang*'

# Find klang
Get-Command klang -ErrorAction SilentlyContinue
```

---

## Still Having Issues?

### Check for conflicting installations

```bash
# Find all klang executables
# macOS/Linux:
find ~ -name klang -type f 2>/dev/null

# See which one is being used
which klang
```

### Check file permissions

```bash
# macOS/Linux:
ls -la ~/.klang/bin/klang

# Should show: -rwxr-xr-x (executable)
# If not, fix with:
chmod +x ~/.klang/bin/klang
```

### Verify shell is reading configuration

```bash
# Test if .zshrc is loaded
echo "export TEST_VAR=hello" >> ~/.zshrc
source ~/.zshrc
echo $TEST_VAR  # Should print "hello"

# Clean up
sed -i '' '/TEST_VAR/d' ~/.zshrc  # macOS
sed -i '/TEST_VAR/d' ~/.zshrc     # Linux
```

### Fresh installation

If all else fails, start fresh:

```bash
# Remove old installation
rm -rf ~/.klang

# Re-run quick install
# Security Note: For maximum security, review the script before running:
# curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh > /tmp/install.sh
# less /tmp/install.sh  # Review the script
# bash /tmp/install.sh  # Run after review

# Quick install (one-line):
curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh | bash

# Follow the post-installation instructions carefully
```

---

## Getting Help

If you're still experiencing issues:

1. **Check the version of your shell:**
   ```bash
   echo $SHELL
   $SHELL --version
   ```

2. **Verify your OS:**
   ```bash
   uname -a
   ```

3. **Open an issue** on GitHub with:
   - Your OS and version
   - Your shell and version
   - Output of `echo $PATH`
   - Output of `ls -la ~/.klang/bin/`
   - The exact error message you're seeing

---

## Summary

Most PATH issues are resolved by:
1. ✅ Restarting your terminal
2. ✅ Running `source ~/.zshrc` (or `~/.bashrc`)
3. ✅ Ensuring the correct shell configuration file is updated
4. ✅ Verifying the klang binary exists and is executable

Remember: Changes to PATH only take effect in **new** terminal sessions or after reloading the configuration!
