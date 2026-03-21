#!/bin/bash
# KLang Installation Script
# Installs KLang binary to system or user directory

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

KLANG_VERSION="0.1.0"
INSTALL_DIR="/usr/local/bin"
USER_INSTALL_DIR="$HOME/.local/bin"

# Show banner
show_banner() {
    echo -e "${BLUE}╔══════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║                                          ║${NC}"
    echo -e "${BLUE}║         KLang Installer v${KLANG_VERSION}         ║${NC}"
    echo -e "${BLUE}║                                          ║${NC}"
    echo -e "${BLUE}╚══════════════════════════════════════════╝${NC}"
    echo ""
}

# Check if klang binary exists
check_binary() {
    if [ ! -f "./klang" ]; then
        echo -e "${RED}Error: klang binary not found!${NC}"
        echo -e "Please build KLang first:"
        echo -e "  ./build.sh"
        exit 1
    fi
}

# Install to system directory (requires sudo)
install_system() {
    echo -e "${BLUE}Installing to system directory: ${INSTALL_DIR}${NC}"
    
    if [ ! -w "$INSTALL_DIR" ]; then
        echo -e "${YELLOW}Note: System installation requires sudo${NC}"
        sudo mkdir -p "$INSTALL_DIR"
        sudo cp klang "$INSTALL_DIR/"
        sudo chmod +x "$INSTALL_DIR/klang"
    else
        mkdir -p "$INSTALL_DIR"
        cp klang "$INSTALL_DIR/"
        chmod +x "$INSTALL_DIR/klang"
    fi
    
    echo -e "${GREEN}✓ Installed to $INSTALL_DIR/klang${NC}"
    
    # Verify installation
    if command -v klang &> /dev/null; then
        echo -e "${GREEN}✓ KLang is now available in PATH${NC}"
    else
        echo -e "${YELLOW}Warning: $INSTALL_DIR may not be in PATH${NC}"
    fi
}

# Install to user directory (no sudo required)
install_user() {
    echo -e "${BLUE}Installing to user directory: ${USER_INSTALL_DIR}${NC}"
    
    mkdir -p "$USER_INSTALL_DIR"
    cp klang "$USER_INSTALL_DIR/"
    chmod +x "$USER_INSTALL_DIR/klang"
    
    echo -e "${GREEN}✓ Installed to $USER_INSTALL_DIR/klang${NC}"
    
    # Check if user bin is in PATH
    if [[ ":$PATH:" != *":$USER_INSTALL_DIR:"* ]]; then
        echo -e "${YELLOW}Note: $USER_INSTALL_DIR is not in your PATH${NC}"
        echo -e "Add this line to your ~/.bashrc or ~/.zshrc:"
        echo -e "  ${BLUE}export PATH=\"\$PATH:$USER_INSTALL_DIR\"${NC}"
        echo ""
        echo -e "Then run: ${BLUE}source ~/.bashrc${NC} (or ~/.zshrc)"
    else
        echo -e "${GREEN}✓ KLang is now available in PATH${NC}"
    fi
}

# Install examples and docs
install_extras() {
    local install_base="$1"
    local share_dir="$install_base/share/klang"
    
    echo -e "${BLUE}Installing examples and documentation...${NC}"
    
    if [ -w "$install_base" ] || [ "$install_base" = "$HOME/.local" ]; then
        mkdir -p "$share_dir"
        
        if [ -d "examples" ]; then
            cp -r examples "$share_dir/"
            echo -e "${GREEN}✓ Installed examples to $share_dir/examples${NC}"
        fi
        
        if [ -d "docs" ]; then
            cp -r docs "$share_dir/"
            echo -e "${GREEN}✓ Installed documentation to $share_dir/docs${NC}"
        fi
    fi
}

# Uninstall KLang
uninstall() {
    echo -e "${YELLOW}Uninstalling KLang...${NC}"
    
    local found=0
    
    # Check system installation
    if [ -f "$INSTALL_DIR/klang" ]; then
        if [ -w "$INSTALL_DIR" ]; then
            rm "$INSTALL_DIR/klang"
        else
            sudo rm "$INSTALL_DIR/klang"
        fi
        echo -e "${GREEN}✓ Removed from $INSTALL_DIR${NC}"
        found=1
    fi
    
    # Check user installation
    if [ -f "$USER_INSTALL_DIR/klang" ]; then
        rm "$USER_INSTALL_DIR/klang"
        echo -e "${GREEN}✓ Removed from $USER_INSTALL_DIR${NC}"
        found=1
    fi
    
    if [ $found -eq 0 ]; then
        echo -e "${YELLOW}KLang not found in standard locations${NC}"
    else
        echo -e "${GREEN}✓ KLang uninstalled${NC}"
    fi
}

# Show usage
show_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --system    Install to system directory (default, requires sudo)"
    echo "  --user      Install to user directory (~/.local/bin)"
    echo "  --uninstall Remove KLang from system"
    echo "  -h, --help  Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                # Install to system (requires sudo)"
    echo "  $0 --user         # Install to user directory"
    echo "  $0 --uninstall    # Uninstall KLang"
    echo ""
}

# Verify installation
verify_install() {
    echo ""
    echo -e "${BLUE}Verifying installation...${NC}"
    
    if command -v klang &> /dev/null; then
        local version=$(klang --version 2>&1 || echo "unknown")
        echo -e "${GREEN}✓ KLang is installed and working${NC}"
        echo -e "  Location: $(which klang)"
        echo ""
        echo -e "${GREEN}Quick start:${NC}"
        echo -e "  klang repl                # Interactive mode"
        echo -e "  klang run script.kl       # Run a script"
        echo ""
    else
        echo -e "${YELLOW}KLang installed but not in PATH${NC}"
        echo -e "You may need to restart your terminal or add the bin directory to PATH"
    fi
}

# Main execution
main() {
    show_banner
    
    case "${1:-}" in
        --user)
            check_binary
            install_user
            install_extras "$HOME/.local"
            verify_install
            ;;
        --uninstall)
            uninstall
            ;;
        -h|--help)
            show_usage
            ;;
        --system|"")
            check_binary
            install_system
            install_extras "/usr/local"
            verify_install
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            show_usage
            exit 1
            ;;
    esac
}

main "$@"
