#!/usr/bin/env bash
# KLang Universal Installer
# One-line install: curl -fsSL https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash
# Or: wget -qO- https://raw.githubusercontent.com/k-kaundal/KLang/main/install.sh | bash

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
BOLD='\033[1m'
NC='\033[0m'

# Configuration
# Auto-detect latest version from GitHub API, fallback to VERSION file, then v1.0.0-rc
get_latest_version() {
    # Try GitHub API first
    if command -v curl &> /dev/null; then
        local latest=$(curl -sSL --max-time 5 https://api.github.com/repos/$GITHUB_REPO/releases/latest 2>/dev/null | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/' 2>/dev/null)
        # Check if we got a valid tag (not null, not empty, starts with v, no colons or HTML)
        if [ -n "$latest" ] && [ "$latest" != "null" ] && [[ "$latest" == v* ]] && [[ ! "$latest" =~ [:] ]] && [[ ! "$latest" =~ \< ]]; then
            echo "$latest"
            return
        fi
    fi
    
    # Try VERSION file from repo
    if command -v curl &> /dev/null; then
        local version=$(curl -sSL --max-time 5 https://raw.githubusercontent.com/$GITHUB_REPO/main/VERSION 2>/dev/null | head -1 | tr -d '[:space:]')
        # Check if we got a valid version (not empty, no HTML tags, no HTTP errors)
        if [ -n "$version" ] && [[ ! "$version" =~ \<.*\> ]] && [[ ! "$version" =~ ^[0-9]+: ]] && [[ ! "$version" =~ [Ee]rror ]]; then
            echo "v$version"
            return
        fi
    fi
    
    # Fallback to hardcoded version
    echo "v1.0.0-rc"
}

KLANG_VERSION="${KLANG_VERSION:-$(get_latest_version)}"
GITHUB_REPO="k-kaundal/KLang"
INSTALL_DIR="${KLANG_INSTALL_DIR:-$HOME/.klang}"
BIN_DIR="$INSTALL_DIR/bin"

# Banner
show_banner() {
    echo ""
    echo -e "${BLUE}${BOLD}╔═══════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}${BOLD}║                                                       ║${NC}"
    echo -e "${BLUE}${BOLD}║              ${GREEN}KLang Universal Installer${BLUE}                ║${NC}"
    echo -e "${BLUE}${BOLD}║                                                       ║${NC}"
    echo -e "${BLUE}${BOLD}║          ${YELLOW}The AI-Native Programming Language${BLUE}          ║${NC}"
    echo -e "${BLUE}${BOLD}║                                                       ║${NC}"
    echo -e "${BLUE}${BOLD}╚═══════════════════════════════════════════════════════╝${NC}"
    echo ""
}

# Detect OS and architecture
detect_platform() {
    local os=$(uname -s | tr '[:upper:]' '[:lower:]')
    local arch=$(uname -m)
    
    case "$os" in
        linux*)   OS="linux" ;;
        darwin*)  OS="macos" ;;
        mingw*|msys*|cygwin*) OS="windows" ;;
        *)
            echo -e "${RED}Error: Unsupported OS: $os${NC}"
            exit 1
            ;;
    esac
    
    case "$arch" in
        x86_64|amd64)   ARCH="x64" ;;
        aarch64|arm64)  ARCH="arm64" ;;
        armv7l)         ARCH="armv7" ;;
        *)
            echo -e "${RED}Error: Unsupported architecture: $arch${NC}"
            exit 1
            ;;
    esac
    
    echo -e "${BLUE}Platform: ${BOLD}$OS-$ARCH${NC}"
}

# Check dependencies
check_dependencies() {
    local missing_deps=()
    
    # Check for required tools
    if ! command -v git &> /dev/null; then
        missing_deps+=("git")
    fi
    
    if ! command -v gcc &> /dev/null && ! command -v clang &> /dev/null; then
        missing_deps+=("gcc or clang")
    fi
    
    if ! command -v make &> /dev/null; then
        missing_deps+=("make")
    fi
    
    # Check for LLVM (required for KLang compilation)
    local llvm_found=false
    if command -v llvm-config &> /dev/null || \
       command -v llvm-config-18 &> /dev/null || \
       command -v llvm-config-17 &> /dev/null || \
       command -v llvm-config-16 &> /dev/null || \
       [ -x "/opt/homebrew/opt/llvm/bin/llvm-config" ] || \
       [ -x "/usr/local/opt/llvm/bin/llvm-config" ]; then
        llvm_found=true
    fi
    
    if [ "$llvm_found" = false ]; then
        missing_deps+=("llvm")
    fi
    
    if [ ${#missing_deps[@]} -gt 0 ]; then
        echo -e "${YELLOW}Missing dependencies: ${missing_deps[*]}${NC}"
        echo ""
        echo -e "${BLUE}Install them first:${NC}"
        
        case "$OS" in
            linux)
                if command -v apt-get &> /dev/null; then
                    echo -e "  ${GREEN}sudo apt-get update && sudo apt-get install -y build-essential git libreadline-dev llvm-dev${NC}"
                elif command -v yum &> /dev/null; then
                    echo -e "  ${GREEN}sudo yum groupinstall 'Development Tools' && sudo yum install -y git readline-devel llvm-devel${NC}"
                elif command -v pacman &> /dev/null; then
                    echo -e "  ${GREEN}sudo pacman -S base-devel git readline llvm${NC}"
                fi
                ;;
            macos)
                echo -e "  ${GREEN}xcode-select --install${NC} (for basic build tools)"
                echo -e "  ${GREEN}brew install llvm readline${NC} (required)"
                ;;
        esac
        echo ""
        read -p "Install dependencies now? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            install_dependencies
        else
            echo -e "${RED}Cannot proceed without dependencies${NC}"
            exit 1
        fi
    fi
}

# Install dependencies automatically
install_dependencies() {
    echo -e "${BLUE}Installing dependencies...${NC}"
    
    case "$OS" in
        linux)
            if command -v apt-get &> /dev/null; then
                sudo apt-get update
                sudo apt-get install -y build-essential git libreadline-dev llvm-dev
            elif command -v yum &> /dev/null; then
                sudo yum groupinstall -y 'Development Tools'
                sudo yum install -y git readline-devel llvm-devel
            elif command -v pacman &> /dev/null; then
                sudo pacman -Sy --noconfirm base-devel git readline llvm
            fi
            ;;
        macos)
            if ! command -v brew &> /dev/null; then
                echo -e "${YELLOW}Homebrew not found. Installing Homebrew...${NC}"
                /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
            fi
            echo -e "${BLUE}Installing LLVM and readline...${NC}"
            brew install llvm readline
            
            # Add LLVM to PATH for current session
            if [ -d "/opt/homebrew/opt/llvm/bin" ]; then
                export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
                echo -e "${GREEN}✓ Added LLVM to PATH for this session${NC}"
            elif [ -d "/usr/local/opt/llvm/bin" ]; then
                export PATH="/usr/local/opt/llvm/bin:$PATH"
                echo -e "${GREEN}✓ Added LLVM to PATH for this session${NC}"
            fi
            ;;
    esac
    
    echo -e "${GREEN}✓ Dependencies installed${NC}"
}

# Download and build KLang
install_klang() {
    local temp_dir=$(mktemp -d)
    
    echo -e "${BLUE}Downloading KLang from GitHub...${NC}"
    
    # Try cloning with version tag first, fallback to main branch
    if ! git clone --depth 1 --branch "$KLANG_VERSION" "https://github.com/$GITHUB_REPO.git" "$temp_dir/klang" 2>/dev/null; then
        echo -e "${YELLOW}Version $KLANG_VERSION not found, using main branch...${NC}"
        if ! git clone --depth 1 "https://github.com/$GITHUB_REPO.git" "$temp_dir/klang" 2>&1; then
            echo -e "${RED}Failed to clone repository${NC}"
            rm -rf "$temp_dir"
            exit 1
        fi
    fi
    
    cd "$temp_dir/klang"
    
    echo -e "${BLUE}Building KLang (this may take a minute)...${NC}"
    
    if ! make clean > /dev/null 2>&1; then
        echo -e "${YELLOW}Clean failed (might be first build)${NC}"
    fi
    
    if ! make > /dev/null 2>&1; then
        echo -e "${RED}Build failed. Showing build output:${NC}"
        make
        rm -rf "$temp_dir"
        exit 1
    fi
    
    echo -e "${GREEN}✓ Build successful${NC}"
    
    # Install binary
    echo -e "${BLUE}Installing to $INSTALL_DIR...${NC}"
    mkdir -p "$BIN_DIR"
    cp klang "$BIN_DIR/"
    chmod +x "$BIN_DIR/klang"
    
    # Copy examples and docs
    [ -d examples ] && cp -r examples "$INSTALL_DIR/" 2>/dev/null || true
    [ -d docs ] && cp -r docs "$INSTALL_DIR/" 2>/dev/null || true
    [ -d stdlib ] && cp -r stdlib "$INSTALL_DIR/" 2>/dev/null || true
    
    cd - > /dev/null
    rm -rf "$temp_dir"
    
    echo -e "${GREEN}✓ KLang installed to $BIN_DIR/klang${NC}"
}

# Detect user's shell
detect_shell() {
    if [ -n "$SHELL" ]; then
        basename "$SHELL"
    elif [ -n "$BASH" ]; then
        echo "bash"
    else
        echo "sh"
    fi
}

# Setup PATH automatically
setup_path() {
    local shell_type=$(detect_shell)
    local rc_files=()
    local path_line="export PATH=\"\$PATH:$BIN_DIR\""
    
    echo -e "${BLUE}Configuring PATH for $shell_type...${NC}"
    
    # Determine RC files based on shell
    case "$shell_type" in
        zsh)
            rc_files=("$HOME/.zshrc" "$HOME/.zprofile")
            ;;
        bash)
            rc_files=("$HOME/.bashrc" "$HOME/.bash_profile")
            ;;
        fish)
            rc_files=("$HOME/.config/fish/config.fish")
            path_line="set -gx PATH \$PATH $BIN_DIR"
            ;;
        *)
            rc_files=("$HOME/.profile")
            ;;
    esac
    
    # Check if already in PATH
    if [[ ":$PATH:" == *":$BIN_DIR:"* ]]; then
        echo -e "${GREEN}✓ KLang already in PATH${NC}"
        return 0
    fi
    
    local added=false
    
    # Try to add to first writable RC file
    for rc_file in "${rc_files[@]}"; do
        # Create parent directory if needed
        if [[ ! -f "$rc_file" ]]; then
            mkdir -p "$(dirname "$rc_file")" 2>/dev/null || continue
            touch "$rc_file" 2>/dev/null || continue
        fi
        
        # Check if already added
        if grep -q "klang" "$rc_file" 2>/dev/null; then
            echo -e "${GREEN}✓ PATH already configured in $rc_file${NC}"
            added=true
            break
        fi
        
        # Add to PATH
        if [ -w "$rc_file" ]; then
            {
                echo ""
                echo "# KLang"
                echo "$path_line"
            } >> "$rc_file"
            echo -e "${GREEN}✓ Added KLang to PATH in $rc_file${NC}"
            added=true
            break
        fi
    done
    
    if [ "$added" = false ]; then
        echo -e "${YELLOW}Warning: Could not automatically add to PATH${NC}"
        echo -e "${YELLOW}Add this line to your shell config manually:${NC}"
        echo -e "  ${BLUE}$path_line${NC}"
    fi
    
    # Add to current session
    export PATH="$PATH:$BIN_DIR"
}

# Verify installation
verify_installation() {
    echo ""
    echo -e "${BLUE}Verifying installation...${NC}"
    
    if [ -x "$BIN_DIR/klang" ]; then
        local version=$("$BIN_DIR/klang" --version 2>&1 | head -1)
        echo -e "${GREEN}✓ KLang installed successfully!${NC}"
        echo -e "${GREEN}  Version: $version${NC}"
        echo -e "${GREEN}  Location: $BIN_DIR/klang${NC}"
        
        # Validate version matches expected release (only if we have a valid expected version)
        local expected_version=$(echo "$KLANG_VERSION" | sed 's/^v//')
        # Only show warning if expected version looks valid (no errors, not the fallback)
        if [[ "$expected_version" =~ ^[0-9]+\.[0-9]+\.[0-9]+ ]] && [[ ! "$expected_version" =~ [Ee]rror ]]; then
            if echo "$version" | grep -q "$expected_version"; then
                echo -e "${GREEN}✓ Version validation passed: $expected_version${NC}"
            else
                # Only warn if versions are actually different and not installing from main
                if [[ "$expected_version" != "1.0.0-rc" ]]; then
                    echo -e "${YELLOW}⚠ Warning: Installed version doesn't match expected version $expected_version${NC}"
                    echo -e "${YELLOW}  This might be expected if you're installing from a specific branch${NC}"
                fi
            fi
        fi
    else
        echo -e "${RED}Installation verification failed${NC}"
        exit 1
    fi
}

# Show next steps
show_next_steps() {
    local shell_type=$(detect_shell)
    
    echo ""
    echo -e "${GREEN}${BOLD}╔═══════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}${BOLD}║                                                       ║${NC}"
    echo -e "${GREEN}${BOLD}║            Installation Complete! ✓                   ║${NC}"
    echo -e "${GREEN}${BOLD}║                                                       ║${NC}"
    echo -e "${GREEN}${BOLD}╚═══════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    echo -e "${YELLOW}${BOLD}Next Steps:${NC}"
    echo ""
    echo -e "${BLUE}1. Reload your shell configuration:${NC}"
    
    case "$shell_type" in
        zsh)
            echo -e "   ${GREEN}source ~/.zshrc${NC}"
            ;;
        bash)
            echo -e "   ${GREEN}source ~/.bashrc${NC}"
            ;;
        fish)
            echo -e "   ${GREEN}source ~/.config/fish/config.fish${NC}"
            ;;
        *)
            echo -e "   ${GREEN}source ~/.profile${NC}"
            ;;
    esac
    
    echo -e "   ${YELLOW}Or simply restart your terminal${NC}"
    echo ""
    echo -e "${BLUE}2. Verify KLang is in your PATH:${NC}"
    echo -e "   ${GREEN}which klang${NC}"
    echo -e "   ${YELLOW}(Should show: $BIN_DIR/klang)${NC}"
    echo ""
    echo -e "${BLUE}3. Try KLang:${NC}"
    echo -e "   ${GREEN}klang --version${NC}              # Check version"
    echo -e "   ${GREEN}klang repl${NC}                   # Interactive REPL"
    echo -e "   ${GREEN}klang run hello.kl${NC}           # Run a script"
    echo -e "   ${GREEN}klang help${NC}                   # Show all commands"
    echo ""
    
    if [ -d "$INSTALL_DIR/examples" ]; then
        echo -e "${BLUE}4. Try examples:${NC}"
        echo -e "   ${GREEN}ls $INSTALL_DIR/examples/${NC}"
        echo -e "   ${GREEN}klang run $INSTALL_DIR/examples/hello.kl${NC}"
        echo ""
    fi
    
    echo -e "${BLUE}Documentation:${NC} https://github.com/$GITHUB_REPO"
    echo -e "${BLUE}Issues:${NC} https://github.com/$GITHUB_REPO/issues"
    echo ""
    echo -e "${YELLOW}If 'klang' command is not found after reloading:${NC}"
    echo -e "  ${GREEN}export PATH=\"\$PATH:$BIN_DIR\"${NC}"
    echo ""
    echo -e "Happy coding with KLang! 🚀"
    echo ""
}

# Main installation flow
main() {
    show_banner
    detect_platform
    check_dependencies
    install_klang
    setup_path
    verify_installation
    show_next_steps
}

# Handle errors
trap 'echo -e "\n${RED}Installation failed!${NC}"; exit 1' ERR

# Run installation
main "$@"
