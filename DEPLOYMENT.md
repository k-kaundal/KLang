# KLang Deployment Guide

Complete guide for deploying, distributing, and installing KLang across all platforms.

## 📦 Distribution Methods

### 1. Docker Container

#### Quick Start
```bash
# Pull official image
docker pull ghcr.io/k-kaundal/klang:latest

# Run REPL
docker run -it ghcr.io/k-kaundal/klang:latest

# Run a script
docker run -v $(pwd):/workspace ghcr.io/k-kaundal/klang:latest klang run /workspace/script.kl
```

#### Docker Compose
```bash
# Start development environment
docker-compose up klang-dev

# Start playground server
docker-compose up klang-playground
```

#### Building Custom Images
```bash
# Production image
docker build -t klang:custom .

# Development image with all tools
docker build -f Dockerfile.dev -t klang:dev .
```

---

### 2. Homebrew (macOS/Linux)

#### Installation
```bash
# Add KLang tap
brew tap k-kaundal/klang

# Install KLang
brew install klang

# Verify
klang --version
```

#### Updating
```bash
brew upgrade klang
```

#### Uninstalling
```bash
brew uninstall klang
```

---

### 3. Package Managers

#### Debian/Ubuntu (APT)
```bash
# Add repository (coming soon)
echo "deb [trusted=yes] https://apt.klang.dev stable main" | sudo tee /etc/apt/sources.list.d/klang.list

# Update and install
sudo apt-get update
sudo apt-get install klang

# Verify
klang --version
```

#### CentOS/RHEL/Fedora (YUM/DNF)
```bash
# Add repository (coming soon)
sudo tee /etc/yum.repos.d/klang.repo <<EOF
[klang]
name=KLang Repository
baseurl=https://yum.klang.dev/stable
enabled=1
gpgcheck=0
EOF

# Install
sudo yum install klang
# or
sudo dnf install klang

# Verify
klang --version
```

#### Arch Linux (Pacman)
```bash
# Install from AUR (coming soon)
yay -S klang
# or
paru -S klang

# Verify
klang --version
```

---

### 4. Universal Installer (All Platforms)

#### Linux/macOS
```bash
# One-line install
curl -fsSL https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash

# Or with wget
wget -qO- https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash
```

#### Windows (PowerShell)
```powershell
# Run as Administrator
irm https://raw.githubusercontent.com/k-kaundal/KLang/main/install.ps1 | iex
```

---

### 5. Manual Build from Source

#### Prerequisites
- GCC or Clang compiler
- Make
- Git
- libreadline-dev
- LLVM 16+ (optional, for advanced features)

#### Build Steps
```bash
# Clone repository
git clone https://github.com/k-kaundal/KLang.git
cd KLang

# Build
make clean
make

# Install system-wide (requires sudo)
sudo make install

# Or install to user directory (no sudo)
make install-user

# Verify
klang --version
```

---

## 🔄 Auto-Update Mechanism

### Checking for Updates
```bash
# Check if update is available
klang-update --check

# Update with confirmation
klang-update

# Update without confirmation
klang-update --yes
```

### Automatic Updates (Optional)
```bash
# Enable auto-update cron job (Linux/macOS)
(crontab -l 2>/dev/null; echo "0 2 * * 0 $HOME/.klang/bin/klang-update --yes") | crontab -

# Or use systemd timer (Linux)
# See docs/systemd/klang-update.timer
```

---

## 🚀 CI/CD Integration

### GitHub Actions

#### Build and Test
```yaml
name: Build with KLang

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Install KLang
        run: |
          curl -fsSL https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash
          echo "$HOME/.klang/bin" >> $GITHUB_PATH
      
      - name: Build project
        run: klang build
      
      - name: Run tests
        run: klang test
```

#### Use Pre-built Docker Image
```yaml
name: Test with KLang Docker

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    container:
      image: ghcr.io/k-kaundal/klang:latest
    steps:
      - uses: actions/checkout@v4
      - run: klang test
```

### GitLab CI
```yaml
image: ghcr.io/k-kaundal/klang:latest

stages:
  - build
  - test

build:
  stage: build
  script:
    - klang build

test:
  stage: test
  script:
    - klang test
```

### Jenkins
```groovy
pipeline {
    agent {
        docker {
            image 'ghcr.io/k-kaundal/klang:latest'
        }
    }
    stages {
        stage('Build') {
            steps {
                sh 'klang build'
            }
        }
        stage('Test') {
            steps {
                sh 'klang test'
            }
        }
    }
}
```

---

## 🛠️ Building Packages

### Debian/Ubuntu Package
```bash
cd packaging/debian
dpkg-buildpackage -us -uc -b
```

### RPM Package
```bash
cd packaging/rpm
rpmbuild -ba klang.spec
```

### Homebrew Formula
```bash
# Test formula locally
brew install --build-from-source Formula/klang.rb

# Audit formula
brew audit --strict Formula/klang.rb
```

---

## 📋 Installation Verification

### Basic Verification
```bash
# Check version
klang --version

# Test REPL
klang repl

# Run example
klang run $HOME/.klang/examples/hello.kl

# Show help
klang help
```

### Comprehensive Testing
```bash
# Run test suite for installation
cd /path/to/KLang
./tests/test_install.sh

# Run all tests
make test

# Check dependencies
ldd $(which klang)  # Linux
otool -L $(which klang)  # macOS
```

---

## 🔧 Troubleshooting

### Common Issues

#### "klang: command not found"
```bash
# Check if klang is in PATH
which klang

# Add to PATH manually
export PATH="$PATH:$HOME/.klang/bin"

# Or for system install
export PATH="$PATH:/usr/local/bin"

# Make permanent by adding to shell rc file
echo 'export PATH="$PATH:$HOME/.klang/bin"' >> ~/.bashrc
source ~/.bashrc
```

#### Build Errors
```bash
# Install missing dependencies
# Ubuntu/Debian
sudo apt-get install build-essential libreadline-dev llvm-16-dev

# macOS
brew install readline llvm@16

# Fedora/RHEL
sudo dnf install gcc make readline-devel llvm-devel
```

#### Permission Issues
```bash
# Use user installation instead of system
make install-user

# Or fix permissions for system install
sudo chown -R $(whoami) /usr/local/bin/klang
```

#### Docker Issues
```bash
# Pull latest image
docker pull ghcr.io/k-kaundal/klang:latest

# Clean build
docker build --no-cache -t klang:local .

# Check logs
docker logs klang-container
```

---

## 🌐 Deployment to Production

### Server Deployment
```bash
# Install on server
curl -fsSL https://install.klang.dev | bash

# Set up as service (systemd)
sudo cp packaging/systemd/klang.service /etc/systemd/system/
sudo systemctl enable klang
sudo systemctl start klang
```

### Cloud Platforms

#### AWS EC2
```bash
# User data script
#!/bin/bash
curl -fsSL https://install.klang.dev | bash
```

#### Google Cloud Platform
```bash
# Startup script
curl -fsSL https://install.klang.dev | bash
```

#### Azure VM
```bash
# Custom script extension
curl -fsSL https://install.klang.dev | bash
```

#### Kubernetes
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: klang-app
spec:
  replicas: 3
  selector:
    matchLabels:
      app: klang
  template:
    metadata:
      labels:
        app: klang
    spec:
      containers:
      - name: klang
        image: ghcr.io/k-kaundal/klang:latest
        ports:
        - containerPort: 8080
```

---

## 📊 Monitoring and Metrics

### Health Checks
```bash
# Docker health check
docker inspect --format='{{.State.Health.Status}}' klang-container

# Manual check
klang --version && echo "Healthy" || echo "Unhealthy"
```

### Usage Statistics
```bash
# Check installation stats
klang stats

# Monitor performance
klang benchmark
```

---

## 🔐 Security

### Verification
```bash
# Verify checksums
sha256sum klang

# Check GPG signature (when available)
gpg --verify klang.sig klang
```

### Sandboxing
```bash
# Run in Docker with limited permissions
docker run --rm -it --read-only ghcr.io/k-kaundal/klang:latest

# Use firejail (Linux)
firejail klang run untrusted-script.kl
```

---

## 📖 Additional Resources

- [Installation Guide](INSTALL.md)
- [Build Guide](BUILD.md)
- [Contributing Guide](CONTRIBUTING.md)
- [Documentation](docs/)
- [Examples](examples/)
- [GitHub Repository](https://github.com/k-kaundal/KLang)
- [Issue Tracker](https://github.com/k-kaundal/KLang/issues)
- [Discussions](https://github.com/k-kaundal/KLang/discussions)

---

**Happy deploying with KLang! 🚀**
