#!/bin/bash
# KLang Quick Installer
# Universal installation script for all platforms
# Usage: curl -sSL https://klang.dev/install | bash

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

KLANG_VERSION="0.1.0"
GITHUB_REPO="k-kaundal/KLang"
INSTALL_DIR="$HOME/.klang"

# Detect platform
detect_platform() {
    local os=$(uname -s)
    local arch=$(uname -m)
    
    case "$os" in
        Linux*)
            OS="linux"
            ;;
        Darwin*)
            OS="macos"
            ;;
        *)
            echo -e "${RED}Unsupported OS: $os${NC}"
            exit 1
            ;;
    esac
    
    case "$arch" in
        x86_64|amd64)
            ARCH="x64"
            ;;
        aarch64|arm64)
            ARCH="arm64"
            ;;
        *)
            echo -e "${RED}Unsupported architecture: $arch${NC}"
            exit 1
            ;;
    esac
    
    echo -e "${BLUE}Platform detected: $OS-$ARCH${NC}"
}

# Show banner
show_banner() {
    echo ""
    echo -e "${BLUE}╔══════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║                                                      ║${NC}"
    echo -e "${BLUE}║              KLang Quick Installer                   ║${NC}"
    echo -e "${BLUE}║                   v${KLANG_VERSION}                            ║${NC}"
    echo -e "${BLUE}║                                                      ║${NC}"
    echo -e "${BLUE}╚══════════════════════════════════════════════════════╝${NC}"
    echo ""
}

# Download and extract KLang
download_klang() {
    local package_name="klang-${KLANG_VERSION}-${OS}-${ARCH}"
    local download_url="https://github.com/${GITHUB_REPO}/releases/download/v${KLANG_VERSION}/${package_name}.tar.gz"
    local temp_dir=$(mktemp -d)
    
    echo -e "${BLUE}Downloading KLang ${KLANG_VERSION}...${NC}"
    
    # Try to download from GitHub releases (will fail for now since releases don't exist yet)
    if command -v curl &> /dev/null; then
        if ! curl -sL "$download_url" -o "$temp_dir/klang.tar.gz" 2>/dev/null; then
            echo -e "${YELLOW}Pre-built binaries not yet available.${NC}"
            echo -e "${BLUE}Building from source instead...${NC}"
            build_from_source
            return
        fi
    elif command -v wget &> /dev/null; then
        if ! wget -q "$download_url" -O "$temp_dir/klang.tar.gz" 2>/dev/null; then
            echo -e "${YELLOW}Pre-built binaries not yet available.${NC}"
            echo -e "${BLUE}Building from source instead...${NC}"
            build_from_source
            return
        fi
    else
        echo -e "${RED}Error: curl or wget required for download${NC}"
        exit 1
    fi
    
    echo -e "${BLUE}Extracting...${NC}"
    tar -xzf "$temp_dir/klang.tar.gz" -C "$temp_dir"
    
    # Install
    mkdir -p "$INSTALL_DIR/bin"
    cp "$temp_dir/${package_name}/klang" "$INSTALL_DIR/bin/"
    chmod +x "$INSTALL_DIR/bin/klang"
    
    # Copy extras
    if [ -d "$temp_dir/${package_name}/examples" ]; then
        cp -r "$temp_dir/${package_name}/examples" "$INSTALL_DIR/"
    fi
    if [ -d "$temp_dir/${package_name}/docs" ]; then
        cp -r "$temp_dir/${package_name}/docs" "$INSTALL_DIR/"
    fi
    
    # Cleanup
    rm -rf "$temp_dir"
    
    echo -e "${GREEN}✓ Downloaded and extracted${NC}"
}

# Build from source
build_from_source() {
    echo -e "${BLUE}Building KLang from source...${NC}"
    
    local temp_dir=$(mktemp -d)
    cd "$temp_dir"
    
    # Check for git
    if ! command -v git &> /dev/null; then
        echo -e "${RED}Error: git is required to build from source${NC}"
        echo -e "Install git first, then try again"
        exit 1
    fi
    
    # Clone repository
    echo -e "${BLUE}Cloning repository...${NC}"
    git clone --depth 1 https://github.com/${GITHUB_REPO}.git klang-src
    cd klang-src
    
    # Check for build tools
    if ! command -v gcc &> /dev/null && ! command -v clang &> /dev/null; then
        echo -e "${RED}Error: C compiler (gcc or clang) required${NC}"
        echo -e "Please install:"
        case "$OS" in
            linux)
                echo -e "  sudo apt-get install build-essential  # Debian/Ubuntu"
                echo -e "  sudo yum groupinstall 'Development Tools'  # CentOS/RHEL"
                ;;
            macos)
                echo -e "  xcode-select --install"
                ;;
        esac
        exit 1
    fi
    
    # Build
    echo -e "${BLUE}Compiling...${NC}"
    make clean > /dev/null 2>&1 || true
    if ! make > /dev/null 2>&1; then
        echo -e "${RED}Build failed${NC}"
        exit 1
    fi
    
    # Install
    mkdir -p "$INSTALL_DIR/bin"
    cp klang "$INSTALL_DIR/bin/"
    chmod +x "$INSTALL_DIR/bin/klang"
    
    # Copy extras
    cp -r examples "$INSTALL_DIR/" 2>/dev/null || true
    cp -r docs "$INSTALL_DIR/" 2>/dev/null || true
    
    cd ..
    rm -rf "$temp_dir"
    
    echo -e "${GREEN}✓ Built and installed from source${NC}"
}

# Setup PATH
setup_path() {
    local shell_rc=""
    
    # Detect shell
    if [ -n "$BASH_VERSION" ]; then
        shell_rc="$HOME/.bashrc"
    elif [ -n "$ZSH_VERSION" ]; then
        shell_rc="$HOME/.zshrc"
    else
        shell_rc="$HOME/.profile"
    fi
    
    # Check if already in PATH
    if [[ ":$PATH:" == *":$INSTALL_DIR/bin:"* ]]; then
        echo -e "${GREEN}✓ KLang already in PATH${NC}"
        return
    fi
    
    # Add to PATH
    echo "" >> "$shell_rc"
    echo "# KLang" >> "$shell_rc"
    echo "export PATH=\"\$PATH:$INSTALL_DIR/bin\"" >> "$shell_rc"
    
    echo -e "${GREEN}✓ Added KLang to PATH in $shell_rc${NC}"
    echo -e "${YELLOW}Note: Restart your terminal or run:${NC}"
    echo -e "  ${BLUE}source $shell_rc${NC}"
}

# Verify installation
verify_install() {
    echo ""
    echo -e "${BLUE}Verifying installation...${NC}"
    
    if [ -x "$INSTALL_DIR/bin/klang" ]; then
        echo -e "${GREEN}✓ KLang installed successfully!${NC}"
        echo -e "  Location: $INSTALL_DIR/bin/klang"
        echo ""
        echo -e "${GREEN}Next steps:${NC}"
        echo -e "  1. Restart your terminal or run: ${BLUE}source ~/.bashrc${NC}"
        echo -e "  2. Try: ${BLUE}klang repl${NC}"
        echo -e "  3. Or: ${BLUE}klang run $INSTALL_DIR/examples/hello.kl${NC}"
        echo ""
    else
        echo -e "${RED}Installation failed${NC}"
        exit 1
    fi
}

# Show completion message
show_completion() {
    echo -e "${GREEN}╔══════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║                                                      ║${NC}"
    echo -e "${GREEN}║         KLang Installation Complete! ✓              ║${NC}"
    echo -e "${GREEN}║                                                      ║${NC}"
    echo -e "${GREEN}╚══════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${BLUE}Documentation: $INSTALL_DIR/docs${NC}"
    echo -e "${BLUE}Examples: $INSTALL_DIR/examples${NC}"
    echo ""
    echo -e "Happy coding with KLang! 🚀"
    echo ""
}

# Main execution
main() {
    show_banner
    detect_platform
    download_klang
    setup_path
    verify_install
    show_completion
}

main "$@"
