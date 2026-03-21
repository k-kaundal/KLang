#!/bin/bash
# KLang Cross-Platform Build Script
# Builds KLang binary for the current platform

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Detect platform
detect_platform() {
    local os=$(uname -s)
    local arch=$(uname -m)
    
    case "$os" in
        Linux*)
            PLATFORM="linux"
            ;;
        Darwin*)
            PLATFORM="macos"
            ;;
        MINGW*|MSYS*|CYGWIN*)
            PLATFORM="windows"
            ;;
        *)
            echo -e "${RED}Unsupported operating system: $os${NC}"
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
        i386|i686)
            ARCH="x86"
            ;;
        *)
            echo -e "${YELLOW}Warning: Unknown architecture $arch, assuming x64${NC}"
            ARCH="x64"
            ;;
    esac
    
    echo -e "${BLUE}Platform: $PLATFORM-$ARCH${NC}"
}

# Check for required tools
check_requirements() {
    local missing=0
    
    if ! command -v gcc &> /dev/null && ! command -v clang &> /dev/null; then
        echo -e "${RED}Error: No C compiler found (gcc or clang required)${NC}"
        missing=1
    fi
    
    if ! command -v make &> /dev/null; then
        echo -e "${RED}Error: make not found${NC}"
        missing=1
    fi
    
    if [ $missing -eq 1 ]; then
        echo -e "${YELLOW}Please install build tools:${NC}"
        case "$PLATFORM" in
            linux)
                echo "  sudo apt-get install build-essential  # Debian/Ubuntu"
                echo "  sudo yum groupinstall 'Development Tools'  # CentOS/RHEL"
                ;;
            macos)
                echo "  xcode-select --install"
                echo "  # or: brew install gcc make"
                ;;
            windows)
                echo "  Install MSYS2 or MinGW-w64"
                ;;
        esac
        exit 1
    fi
}

# Build KLang
build_klang() {
    echo -e "${BLUE}Building KLang...${NC}"
    
    # Clean previous builds
    make clean > /dev/null 2>&1 || true
    
    # Build
    if make; then
        echo -e "${GREEN}✓ Build successful${NC}"
        
        # Show binary info
        if [ -f "./klang" ]; then
            local size=$(du -h ./klang | cut -f1)
            echo -e "${GREEN}✓ Binary size: $size${NC}"
            echo -e "${GREEN}✓ Location: $(pwd)/klang${NC}"
        fi
    else
        echo -e "${RED}✗ Build failed${NC}"
        exit 1
    fi
}

# Run tests
run_tests() {
    echo -e "${BLUE}Running tests...${NC}"
    
    if make test 2>&1 | tee /tmp/klang_test.log | grep -q "0 failed"; then
        echo -e "${GREEN}✓ All tests passed${NC}"
    else
        echo -e "${RED}✗ Tests failed${NC}"
        cat /tmp/klang_test.log
        exit 1
    fi
}

# Package binary
package_binary() {
    local version="0.1.0"
    local package_name="klang-${version}-${PLATFORM}-${ARCH}"
    local package_dir="dist/${package_name}"
    
    echo -e "${BLUE}Packaging binary...${NC}"
    
    # Create package directory
    mkdir -p "$package_dir"
    
    # Copy files
    cp klang "$package_dir/"
    cp README.md "$package_dir/"
    cp LICENSE "$package_dir/" 2>/dev/null || echo "No LICENSE file"
    cp -r examples "$package_dir/" 2>/dev/null || true
    cp -r docs "$package_dir/" 2>/dev/null || true
    
    # Create archive
    cd dist
    if [ "$PLATFORM" = "windows" ]; then
        # Create zip for Windows
        if command -v zip &> /dev/null; then
            zip -r "${package_name}.zip" "$package_name"
            echo -e "${GREEN}✓ Package created: dist/${package_name}.zip${NC}"
        fi
    else
        # Create tar.gz for Linux/Mac using POSIX format for maximum compatibility
        # Check for GNU tar vs BSD tar
        if tar --version 2>&1 | grep -q "GNU tar"; then
            # GNU tar supports --format=posix
            tar --format=posix -czf "${package_name}.tar.gz" "$package_name"
        else
            # BSD tar (macOS) is already compatible by default
            tar -czf "${package_name}.tar.gz" "$package_name"
        fi
        echo -e "${GREEN}✓ Package created: dist/${package_name}.tar.gz${NC}"

        # Create checksum
        if command -v sha256sum &> /dev/null; then
            sha256sum "${package_name}.tar.gz" > "${package_name}.tar.gz.sha256"
        else
            shasum -a 256 "${package_name}.tar.gz" > "${package_name}.tar.gz.sha256"
        fi
        echo -e "${GREEN}✓ Checksum created: dist/${package_name}.tar.gz.sha256${NC}"
    fi
    cd ..
}

# Main execution
main() {
    echo -e "${BLUE}╔══════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║                                          ║${NC}"
    echo -e "${BLUE}║       KLang Cross-Platform Build        ║${NC}"
    echo -e "${BLUE}║                                          ║${NC}"
    echo -e "${BLUE}╚══════════════════════════════════════════╝${NC}"
    echo ""
    
    detect_platform
    check_requirements
    build_klang
    
    # Optional: run tests
    if [ "${1:-}" = "--with-tests" ] || [ "${1:-}" = "-t" ]; then
        run_tests
    fi
    
    # Optional: create package
    if [ "${1:-}" = "--package" ] || [ "${1:-}" = "-p" ]; then
        package_binary
    fi
    
    echo ""
    echo -e "${GREEN}╔══════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║                                          ║${NC}"
    echo -e "${GREEN}║         Build Complete! ✓                ║${NC}"
    echo -e "${GREEN}║                                          ║${NC}"
    echo -e "${GREEN}╚══════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${BLUE}Run KLang:${NC}"
    echo -e "  ./klang repl              # Interactive mode"
    echo -e "  ./klang run script.kl     # Run a script"
    echo ""
}

# Handle script arguments
case "${1:-}" in
    -h|--help)
        echo "Usage: $0 [OPTIONS]"
        echo ""
        echo "Options:"
        echo "  -h, --help        Show this help message"
        echo "  -t, --with-tests  Build and run tests"
        echo "  -p, --package     Build and create distribution package"
        echo ""
        exit 0
        ;;
    *)
        main "$@"
        ;;
esac
