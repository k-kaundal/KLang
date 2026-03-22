#!/bin/bash

# KLang Setup Script
# Quick setup for KLang development environment

set -e  # Exit on error

KLANG_VERSION="0.1.0"
BOLD='\033[1m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BOLD}${BLUE}"
echo "╔══════════════════════════════════════════╗"
echo "║                                          ║"
echo "║          KLang Setup Script              ║"
echo "║            Version $KLANG_VERSION                 ║"
echo "║                                          ║"
echo "╚══════════════════════════════════════════╝"
echo -e "${NC}"

# Check prerequisites
echo -e "${BOLD}Checking prerequisites...${NC}"

check_command() {
    if command -v $1 &> /dev/null; then
        echo -e "  ${GREEN}✓${NC} $1 found"
        return 0
    else
        echo -e "  ${RED}✗${NC} $1 not found"
        return 1
    fi
}

# Check required tools
MISSING_TOOLS=0

if ! check_command "gcc"; then
    echo -e "    ${BLUE}Install with: sudo apt-get install gcc${NC}"
    MISSING_TOOLS=1
fi

if ! check_command "make"; then
    echo -e "    ${BLUE}Install with: sudo apt-get install make${NC}"
    MISSING_TOOLS=1
fi

if ! check_command "git"; then
    echo -e "    ${BLUE}Install with: sudo apt-get install git${NC}"
    MISSING_TOOLS=1
fi

if [ $MISSING_TOOLS -eq 1 ]; then
    echo -e "\n${RED}Error: Missing required tools. Please install them first.${NC}"
    exit 1
fi

echo ""

# Build KLang
echo -e "${BOLD}Building KLang...${NC}"
make clean > /dev/null 2>&1 || true
if make; then
    echo -e "  ${GREEN}✓${NC} Build successful"
else
    echo -e "  ${RED}✗${NC} Build failed"
    exit 1
fi

echo ""

# Run tests
echo -e "${BOLD}Running tests...${NC}"
if make test 2>&1 | grep -q "0 failed"; then
    echo -e "  ${GREEN}✓${NC} All tests passed"
else
    echo -e "  ${RED}✗${NC} Some tests failed"
    exit 1
fi

echo ""

# Verify examples
echo -e "${BOLD}Verifying examples...${NC}"
./klang run examples/hello.kl > /dev/null 2>&1 && echo -e "  ${GREEN}✓${NC} hello.kl works" || echo -e "  ${RED}✗${NC} hello.kl failed"
./klang run examples/factorial.kl > /dev/null 2>&1 && echo -e "  ${GREEN}✓${NC} factorial.kl works" || echo -e "  ${RED}✗${NC} factorial.kl failed"
./klang run examples/oop_demo.kl > /dev/null 2>&1 && echo -e "  ${GREEN}✓${NC} oop_demo.kl works" || echo -e "  ${RED}✗${NC} oop_demo.kl failed"

echo ""

# Installation option
echo -e "${BOLD}Setup complete!${NC}"
echo ""
echo -e "To install KLang system-wide (optional):"
echo -e "  ${BLUE}sudo make install${NC}"
echo ""
echo -e "To add KLang to your PATH temporarily:"
echo -e "  ${BLUE}export PATH=\$PATH:$(pwd)${NC}"
echo ""
echo -e "To add KLang to your PATH permanently:"
echo -e "  ${BLUE}echo 'export PATH=\$PATH:$(pwd)' >> ~/.bashrc${NC}"
echo ""
echo -e "${BOLD}Quick Start:${NC}"
echo -e "  ${GREEN}./klang repl${NC}              # Start interactive REPL"
echo -e "  ${GREEN}./klang run examples/hello.kl${NC}  # Run a script"
echo ""
echo -e "${BOLD}Documentation:${NC}"
echo -e "  📖 docs/GETTING_STARTED.md    - Setup guide"
echo -e "  📘 docs/LANGUAGE_REFERENCE.md - Syntax reference"
echo -e "  🔧 docs/USAGE_GUIDE.md        - Usage guide"
echo ""
echo -e "${GREEN}${BOLD}Happy coding with KLang! 🚀${NC}"
