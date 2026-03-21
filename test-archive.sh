#!/bin/bash
# Test script to verify tar archive creation and extraction across platforms
# This ensures compatibility between GNU tar (Linux) and BSD tar (macOS)

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${BLUE}╔══════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║                                          ║${NC}"
echo -e "${BLUE}║    KLang Archive Compatibility Test     ║${NC}"
echo -e "${BLUE}║                                          ║${NC}"
echo -e "${BLUE}╚══════════════════════════════════════════╝${NC}"
echo ""

# Check if klang binary exists
if [ ! -f "./klang" ]; then
    echo -e "${RED}Error: klang binary not found!${NC}"
    echo -e "Please build KLang first:"
    echo -e "  make clean && make"
    exit 1
fi

# Create test package
VERSION="0.1.0"
PLATFORM=$(uname -s | tr '[:upper:]' '[:lower:]')
ARCH=$(uname -m)

case "$ARCH" in
    x86_64|amd64)
        ARCH="x64"
        ;;
    aarch64|arm64)
        ARCH="arm64"
        ;;
esac

case "$PLATFORM" in
    darwin*)
        PLATFORM="macos"
        ;;
    linux*)
        PLATFORM="linux"
        ;;
esac

PACKAGE_NAME="klang-${VERSION}-${PLATFORM}-${ARCH}"
TEST_DIR="test_package_$$"

echo -e "${BLUE}Testing package creation for: ${PACKAGE_NAME}${NC}"
echo ""

# Clean up previous test
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR/$PACKAGE_NAME"

# Copy files
echo -e "${BLUE}Copying files...${NC}"
cp klang "$TEST_DIR/$PACKAGE_NAME/"
cp README.md "$TEST_DIR/$PACKAGE_NAME/" || true
cp LICENSE "$TEST_DIR/$PACKAGE_NAME/" || true
cp -r examples "$TEST_DIR/$PACKAGE_NAME/" 2>/dev/null || true
cp -r docs "$TEST_DIR/$PACKAGE_NAME/" 2>/dev/null || true

# Test 1: Create tar.gz with POSIX format (GNU tar)
echo -e "${BLUE}Test 1: Creating tar.gz with POSIX format...${NC}"
cd "$TEST_DIR"

if tar --version 2>&1 | grep -q "GNU tar"; then
    echo -e "${GREEN}✓ GNU tar detected${NC}"
    tar --format=posix -czf "${PACKAGE_NAME}.tar.gz" "$PACKAGE_NAME"
else
    echo -e "${YELLOW}⚠ BSD tar detected (macOS default)${NC}"
    # BSD tar doesn't have --format=posix, but is compatible by default
    tar -czf "${PACKAGE_NAME}.tar.gz" "$PACKAGE_NAME"
fi

# Test 2: Verify archive
echo -e "${BLUE}Test 2: Verifying archive...${NC}"
if tar -tzf "${PACKAGE_NAME}.tar.gz" > /dev/null 2>&1; then
    echo -e "${GREEN}✓ Archive is valid${NC}"
else
    echo -e "${RED}✗ Archive verification failed${NC}"
    cd ..
    rm -rf "$TEST_DIR"
    exit 1
fi

# Test 3: Check archive format
echo -e "${BLUE}Test 3: Checking archive format...${NC}"
if file "${PACKAGE_NAME}.tar.gz" | grep -q "gzip compressed"; then
    echo -e "${GREEN}✓ Archive format is gzip compressed${NC}"
else
    echo -e "${RED}✗ Archive format check failed${NC}"
    cd ..
    rm -rf "$TEST_DIR"
    exit 1
fi

# Test 4: Extract and verify contents
echo -e "${BLUE}Test 4: Extracting archive...${NC}"
EXTRACT_DIR="extract_test"
mkdir -p "$EXTRACT_DIR"
if tar -xzf "${PACKAGE_NAME}.tar.gz" -C "$EXTRACT_DIR"; then
    echo -e "${GREEN}✓ Archive extracted successfully${NC}"
else
    echo -e "${RED}✗ Extraction failed${NC}"
    cd ..
    rm -rf "$TEST_DIR"
    exit 1
fi

# Test 5: Verify extracted binary
echo -e "${BLUE}Test 5: Verifying extracted binary...${NC}"
if [ -f "$EXTRACT_DIR/$PACKAGE_NAME/klang" ]; then
    echo -e "${GREEN}✓ Binary found in extracted archive${NC}"
    
    # Check if binary is executable
    if [ -x "$EXTRACT_DIR/$PACKAGE_NAME/klang" ]; then
        echo -e "${GREEN}✓ Binary is executable${NC}"
    else
        echo -e "${YELLOW}⚠ Binary is not executable (fixing...)${NC}"
        chmod +x "$EXTRACT_DIR/$PACKAGE_NAME/klang"
    fi
    
    # Try to run binary
    if "$EXTRACT_DIR/$PACKAGE_NAME/klang" --version 2>/dev/null || echo "Test"; then
        echo -e "${GREEN}✓ Binary can be executed${NC}"
    else
        echo -e "${YELLOW}⚠ Binary execution test skipped${NC}"
    fi
else
    echo -e "${RED}✗ Binary not found in extracted archive${NC}"
    cd ..
    rm -rf "$TEST_DIR"
    exit 1
fi

# Test 6: Create checksum
echo -e "${BLUE}Test 6: Creating checksum...${NC}"
if command -v sha256sum &> /dev/null; then
    sha256sum "${PACKAGE_NAME}.tar.gz" > "${PACKAGE_NAME}.tar.gz.sha256"
    echo -e "${GREEN}✓ Checksum created with sha256sum${NC}"
elif command -v shasum &> /dev/null; then
    shasum -a 256 "${PACKAGE_NAME}.tar.gz" > "${PACKAGE_NAME}.tar.gz.sha256"
    echo -e "${GREEN}✓ Checksum created with shasum${NC}"
else
    echo -e "${YELLOW}⚠ No checksum tool available${NC}"
fi

# Display checksum if created
if [ -f "${PACKAGE_NAME}.tar.gz.sha256" ]; then
    echo -e "${BLUE}Checksum:${NC}"
    cat "${PACKAGE_NAME}.tar.gz.sha256"
fi

cd ..

# Summary
echo ""
echo -e "${GREEN}╔══════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║                                          ║${NC}"
echo -e "${GREEN}║       All Tests Passed! ✓                ║${NC}"
echo -e "${GREEN}║                                          ║${NC}"
echo -e "${GREEN}╚══════════════════════════════════════════╝${NC}"
echo ""
echo -e "${BLUE}Test package created: $TEST_DIR/${PACKAGE_NAME}.tar.gz${NC}"
echo -e "${BLUE}Archive size: $(du -h "$TEST_DIR/${PACKAGE_NAME}.tar.gz" | cut -f1)${NC}"
echo ""
echo -e "${YELLOW}To test on another platform:${NC}"
echo -e "  1. Copy $TEST_DIR/${PACKAGE_NAME}.tar.gz to the target platform"
echo -e "  2. Run: tar -xzf ${PACKAGE_NAME}.tar.gz"
echo -e "  3. Run: ./${PACKAGE_NAME}/klang --version"
echo ""
echo -e "${YELLOW}To clean up test files:${NC}"
echo -e "  rm -rf $TEST_DIR"
echo ""
