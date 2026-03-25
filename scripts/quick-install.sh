#!/usr/bin/env bash
# KLang Quick Installer
# Universal installation script for all platforms
# 
# Usage: 
#   curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh | bash
#   or
#   wget -qO- https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh | bash
#
# Note: This script requires bash. If you get "command not found: #" error,
#       make sure you're piping to bash (not running directly in zsh/other shells)

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
    # Try to extract with error handling for different tar implementations
    local extract_error
    if ! extract_error=$(tar -xzf "$temp_dir/klang.tar.gz" -C "$temp_dir" 2>&1); then
        echo -e "${RED}Error: Failed to extract archive${NC}"
        echo -e "${YELLOW}Details: $extract_error${NC}"
        echo -e "${YELLOW}The archive may be corrupted or in an incompatible format${NC}"
        echo -e "${BLUE}Falling back to building from source...${NC}"
        build_from_source
        return
    fi
    
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
    
    # Check for LLVM
    if ! command -v llvm-config &> /dev/null && \
       ! command -v llvm-config-18 &> /dev/null && \
       ! command -v llvm-config-17 &> /dev/null && \
       ! command -v llvm-config-16 &> /dev/null && \
       [ ! -x "/opt/homebrew/opt/llvm/bin/llvm-config" ] && \
       [ ! -x "/usr/local/opt/llvm/bin/llvm-config" ]; then
        echo -e "${RED}Error: LLVM is required to build KLang${NC}"
        echo -e "Please install LLVM:"
        case "$OS" in
            linux)
                echo -e "  sudo apt-get install llvm-dev  # Debian/Ubuntu"
                echo -e "  sudo yum install llvm-devel     # CentOS/RHEL"
                echo -e "  sudo pacman -S llvm             # Arch"
                ;;
            macos)
                echo -e "  brew install llvm"
                echo -e "  Then add to PATH: export PATH=\"/opt/homebrew/opt/llvm/bin:\$PATH\""
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
    # Check if already in PATH
    if [[ ":$PATH:" == *":$INSTALL_DIR/bin:"* ]]; then
        echo -e "${GREEN}✓ KLang already in PATH${NC}"
        return
    fi
    
    # Detect user's actual shell (not the script interpreter)
    local user_shell=""
    if [ -n "$SHELL" ]; then
        user_shell=$(basename "$SHELL")
    else
        user_shell="bash"
    fi
    
    # Determine which RC files to update based on actual shell
    local rc_files=()
    local primary_rc=""
    
    case "$user_shell" in
        zsh)
            primary_rc="$HOME/.zshrc"
            rc_files=("$HOME/.zshrc" "$HOME/.zprofile")
            ;;
        bash)
            primary_rc="$HOME/.bashrc"
            rc_files=("$HOME/.bashrc" "$HOME/.bash_profile")
            ;;
        fish)
            primary_rc="$HOME/.config/fish/config.fish"
            rc_files=("$HOME/.config/fish/config.fish")
            ;;
        *)
            primary_rc="$HOME/.profile"
            rc_files=("$HOME/.profile")
            ;;
    esac
    
    local path_added=0
    local rc_file_updated=""
    
    # Try to add to shell RC files
    for rc_file in "${rc_files[@]}"; do
        # Skip if file doesn't exist and we can't create it
        if [ ! -f "$rc_file" ]; then
            # Try to create parent directory for fish
            if [[ "$rc_file" == *".config/fish"* ]]; then
                mkdir -p "$(dirname "$rc_file")" 2>/dev/null || continue
            fi
            # Create the file
            touch "$rc_file" 2>/dev/null || continue
        fi
        
        # Skip if not writable
        [ ! -w "$rc_file" ] && continue
        
        # Check if KLang PATH is already in this file
        if grep -iq "klang" "$rc_file" 2>/dev/null; then
            echo -e "${GREEN}✓ KLang PATH already exists in $rc_file${NC}"
            path_added=1
            rc_file_updated="$rc_file"
            continue
        fi
        
        # Add to PATH with fish-specific syntax
        local write_error
        if [[ "$rc_file" == *".config/fish"* ]]; then
            if ! write_error=$({
                echo ""
                echo "# KLang"
                echo "set -gx PATH \$PATH $INSTALL_DIR/bin"
            } >> "$rc_file" 2>&1); then
                continue
            fi
        else
            if ! write_error=$({
                echo ""
                echo "# KLang"
                echo "export PATH=\"\$PATH:$INSTALL_DIR/bin\""
            } >> "$rc_file" 2>&1); then
                continue
            fi
        fi
        
        echo -e "${GREEN}✓ Added KLang to PATH in $rc_file${NC}"
        path_added=1
        rc_file_updated="$rc_file"
        break
    done
    
    if [ $path_added -eq 0 ]; then
        echo -e "${YELLOW}Warning: Could not automatically add to PATH${NC}"
        echo -e "${YELLOW}Please manually add this line to your shell configuration:${NC}"
        echo -e "  ${BLUE}export PATH=\"\$PATH:$INSTALL_DIR/bin\"${NC}"
        echo ""
        echo -e "${YELLOW}For zsh, add to: ~/.zshrc${NC}"
        echo -e "${YELLOW}For bash, add to: ~/.bashrc${NC}"
        echo -e "${YELLOW}For fish, add to: ~/.config/fish/config.fish${NC}"
        return
    fi
    
    echo -e "${YELLOW}Important: Restart your terminal or run:${NC}"
    if [[ "$rc_file_updated" == *".config/fish"* ]]; then
        echo -e "  ${BLUE}source $rc_file_updated${NC}"
    elif [[ "$rc_file_updated" == *".zshrc"* ]]; then
        echo -e "  ${BLUE}source ~/.zshrc${NC}"
    elif [[ "$rc_file_updated" == *".bashrc"* ]]; then
        echo -e "  ${BLUE}source ~/.bashrc${NC}"
    else
        echo -e "  ${BLUE}source $rc_file_updated${NC}"
    fi
}

# Verify installation
verify_install() {
    echo ""
    echo -e "${BLUE}Verifying installation...${NC}"
    
    if [ -x "$INSTALL_DIR/bin/klang" ]; then
        echo -e "${GREEN}✓ KLang installed successfully!${NC}"
        echo -e "  Location: $INSTALL_DIR/bin/klang"
        echo ""
        
        # Detect user's shell for better instructions
        local user_shell=""
        if [ -n "$SHELL" ]; then
            user_shell=$(basename "$SHELL")
        fi
        
        echo -e "${GREEN}Next steps:${NC}"
        echo ""
        echo -e "${YELLOW}1. Reload your shell configuration:${NC}"
        case "$user_shell" in
            zsh)
                echo -e "   ${BLUE}source ~/.zshrc${NC}"
                echo -e "   ${YELLOW}Or simply close and reopen your terminal${NC}"
                ;;
            bash)
                echo -e "   ${BLUE}source ~/.bashrc${NC}"
                echo -e "   ${YELLOW}Or simply close and reopen your terminal${NC}"
                ;;
            fish)
                echo -e "   ${BLUE}source ~/.config/fish/config.fish${NC}"
                echo -e "   ${YELLOW}Or simply close and reopen your terminal${NC}"
                ;;
            *)
                echo -e "   ${BLUE}source ~/.profile${NC}"
                echo -e "   ${YELLOW}Or simply close and reopen your terminal${NC}"
                ;;
        esac
        echo ""
        echo -e "${YELLOW}2. Verify KLang is in your PATH:${NC}"
        echo -e "   ${BLUE}which klang${NC}"
        echo -e "   ${YELLOW}(Should show: $INSTALL_DIR/bin/klang)${NC}"
        echo ""
        echo -e "${YELLOW}3. Try KLang:${NC}"
        echo -e "   ${BLUE}klang repl${NC}                    ${GREEN}# Interactive mode${NC}"
        echo -e "   ${BLUE}klang run $INSTALL_DIR/examples/hello.kl${NC}"
        echo ""
        echo -e "${YELLOW}Troubleshooting:${NC}"
        echo -e "  If 'klang' command is still not found after reloading:"
        echo -e "  • Use the full path: ${BLUE}$INSTALL_DIR/bin/klang repl${NC}"
        echo -e "  • Manually add to PATH: ${BLUE}export PATH=\"\$PATH:$INSTALL_DIR/bin\"${NC}"
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
