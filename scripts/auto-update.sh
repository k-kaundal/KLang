#!/usr/bin/env bash
# KLang Auto-Update Script
# Automatically updates KLang to the latest version

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Configuration
GITHUB_REPO="k-kaundal/KLang"
GITHUB_API="https://api.github.com/repos/${GITHUB_REPO}"
INSTALL_DIR="${KLANG_INSTALL_DIR:-$HOME/.klang}"
BIN_DIR="$INSTALL_DIR/bin"

# Get current version
get_current_version() {
    if [ -x "$BIN_DIR/klang" ]; then
        "$BIN_DIR/klang" --version 2>/dev/null | head -1 | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' || echo "unknown"
    else
        echo "unknown"
    fi
}

# Get latest version from GitHub
get_latest_version() {
    curl -fsSL "${GITHUB_API}/releases/latest" | \
        grep '"tag_name"' | \
        sed -E 's/.*"([^"]+)".*/\1/' | \
        sed 's/^v//'
}

# Compare versions
version_greater_than() {
    local ver1="$1"
    local ver2="$2"
    
    # If either version is unknown, assume update needed
    if [ "$ver1" = "unknown" ] || [ "$ver2" = "unknown" ]; then
        return 0
    fi
    
    # Use sort -V for version comparison
    if [ "$(printf '%s\n' "$ver1" "$ver2" | sort -V | head -n1)" != "$ver1" ]; then
        return 0  # ver1 > ver2
    else
        return 1  # ver1 <= ver2
    fi
}

# Backup current installation
backup_installation() {
    if [ -x "$BIN_DIR/klang" ]; then
        local backup_dir="$INSTALL_DIR/backup"
        local timestamp=$(date +%Y%m%d_%H%M%S)
        
        echo -e "${BLUE}Creating backup...${NC}"
        mkdir -p "$backup_dir"
        cp "$BIN_DIR/klang" "$backup_dir/klang_${timestamp}"
        echo -e "${GREEN}✓ Backup created at $backup_dir/klang_${timestamp}${NC}"
    fi
}

# Restore from backup
restore_backup() {
    local backup_dir="$INSTALL_DIR/backup"
    local latest_backup=$(ls -t "$backup_dir"/klang_* 2>/dev/null | head -1)
    
    if [ -n "$latest_backup" ]; then
        echo -e "${YELLOW}Restoring from backup...${NC}"
        cp "$latest_backup" "$BIN_DIR/klang"
        chmod +x "$BIN_DIR/klang"
        echo -e "${GREEN}✓ Restored from backup${NC}"
        return 0
    else
        echo -e "${RED}No backup found to restore${NC}"
        return 1
    fi
}

# Download and install update
install_update() {
    local version="$1"
    local temp_dir=$(mktemp -d)
    
    echo -e "${BLUE}Downloading KLang ${version}...${NC}"
    
    # Clone repository at specific version
    if ! git clone --depth 1 --branch "v${version}" \
        "https://github.com/${GITHUB_REPO}.git" "$temp_dir/klang" 2>/dev/null; then
        echo -e "${RED}Failed to download update${NC}"
        rm -rf "$temp_dir"
        return 1
    fi
    
    cd "$temp_dir/klang"
    
    echo -e "${BLUE}Building KLang ${version}...${NC}"
    if ! make clean > /dev/null 2>&1 || ! make > /dev/null 2>&1; then
        echo -e "${RED}Build failed${NC}"
        rm -rf "$temp_dir"
        return 1
    fi
    
    echo -e "${BLUE}Installing update...${NC}"
    mkdir -p "$BIN_DIR"
    cp klang "$BIN_DIR/"
    chmod +x "$BIN_DIR/klang"
    
    # Update examples and docs if they exist
    [ -d examples ] && cp -r examples "$INSTALL_DIR/" 2>/dev/null || true
    [ -d docs ] && cp -r docs "$INSTALL_DIR/" 2>/dev/null || true
    [ -d stdlib ] && cp -r stdlib "$INSTALL_DIR/" 2>/dev/null || true
    
    cd - > /dev/null
    rm -rf "$temp_dir"
    
    echo -e "${GREEN}✓ Update installed successfully${NC}"
    return 0
}

# Verify installation
verify_installation() {
    if [ -x "$BIN_DIR/klang" ]; then
        local new_version=$("$BIN_DIR/klang" --version 2>/dev/null | head -1)
        echo -e "${GREEN}✓ KLang is working: ${new_version}${NC}"
        return 0
    else
        echo -e "${RED}✗ KLang verification failed${NC}"
        return 1
    fi
}

# Check for updates
check_for_updates() {
    echo -e "${BLUE}Checking for updates...${NC}"
    echo ""
    
    local current_version=$(get_current_version)
    echo -e "Current version: ${YELLOW}${current_version}${NC}"
    
    local latest_version=$(get_latest_version)
    if [ -z "$latest_version" ]; then
        echo -e "${RED}Failed to check for updates${NC}"
        echo -e "${YELLOW}Please check your internet connection${NC}"
        exit 1
    fi
    
    echo -e "Latest version:  ${GREEN}${latest_version}${NC}"
    echo ""
    
    if version_greater_than "$latest_version" "$current_version"; then
        return 0  # Update available
    else
        return 1  # No update available
    fi
}

# Main update function
main() {
    echo ""
    echo -e "${BLUE}╔═══════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║                                                       ║${NC}"
    echo -e "${BLUE}║              ${GREEN}KLang Auto-Update${BLUE}                      ║${NC}"
    echo -e "${BLUE}║                                                       ║${NC}"
    echo -e "${BLUE}╚═══════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    # Check for updates
    if ! check_for_updates; then
        echo -e "${GREEN}✓ KLang is already up to date!${NC}"
        echo ""
        exit 0
    fi
    
    # Confirm update
    if [ "$1" != "--yes" ] && [ "$1" != "-y" ]; then
        read -p "Would you like to update? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            echo "Update cancelled."
            exit 0
        fi
    fi
    
    # Get latest version for update
    local latest_version=$(get_latest_version)
    
    # Backup current installation
    backup_installation
    
    # Install update
    if install_update "$latest_version"; then
        # Verify installation
        if verify_installation; then
            echo ""
            echo -e "${GREEN}╔═══════════════════════════════════════════════════════╗${NC}"
            echo -e "${GREEN}║                                                       ║${NC}"
            echo -e "${GREEN}║              Update Complete! ✓                       ║${NC}"
            echo -e "${GREEN}║                                                       ║${NC}"
            echo -e "${GREEN}╚═══════════════════════════════════════════════════════╝${NC}"
            echo ""
            echo -e "${YELLOW}Run 'klang --version' to see the new version${NC}"
            echo ""
            exit 0
        else
            echo -e "${RED}Update verification failed${NC}"
            restore_backup
            exit 1
        fi
    else
        echo -e "${RED}Update failed${NC}"
        restore_backup
        exit 1
    fi
}

# Parse arguments
case "${1:-}" in
    --check|-c)
        if check_for_updates; then
            echo -e "${GREEN}Update available!${NC}"
            echo -e "${YELLOW}Run 'klang-update' to install${NC}"
            exit 0
        else
            echo -e "${GREEN}Up to date!${NC}"
            exit 0
        fi
        ;;
    --help|-h)
        echo "KLang Auto-Update"
        echo ""
        echo "Usage: klang-update [OPTIONS]"
        echo ""
        echo "Options:"
        echo "  --yes, -y     Update without confirmation"
        echo "  --check, -c   Check for updates only"
        echo "  --help, -h    Show this help message"
        echo ""
        exit 0
        ;;
    *)
        main "$@"
        ;;
esac
