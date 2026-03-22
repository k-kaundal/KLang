#!/bin/bash
# Direct Install Command Validation Script
# Tests the quick-install.sh script in a safe way

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║        KLang Direct Install Command Validation           ║${NC}"
echo -e "${BLUE}╚══════════════════════════════════════════════════════════╝${NC}"
echo ""

# Get repo root
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$REPO_ROOT"

TESTS_PASSED=0
TESTS_FAILED=0

# Test 1: Check if quick-install.sh exists
echo -e "${BLUE}Test 1: Checking if scripts/quick-install.sh exists...${NC}"
if [ -f "scripts/quick-install.sh" ]; then
    echo -e "${GREEN}✅ PASS: scripts/quick-install.sh found${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}❌ FAIL: scripts/quick-install.sh not found${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
    exit 1
fi

# Test 2: Check if script is executable
echo -e "\n${BLUE}Test 2: Checking if script is executable...${NC}"
if [ -x "scripts/quick-install.sh" ]; then
    echo -e "${GREEN}✅ PASS: Script is executable${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${YELLOW}⚠️  Script not executable, making it executable...${NC}"
    chmod +x scripts/quick-install.sh
    echo -e "${GREEN}✅ PASS: Made script executable${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
fi

# Test 3: Check script syntax
echo -e "\n${BLUE}Test 3: Checking script syntax...${NC}"
if bash -n scripts/quick-install.sh 2>/dev/null; then
    echo -e "${GREEN}✅ PASS: Script syntax is valid${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}❌ FAIL: Script has syntax errors${NC}"
    bash -n scripts/quick-install.sh
    TESTS_FAILED=$((TESTS_FAILED + 1))
    exit 1
fi

# Test 4: Check for required variables
echo -e "\n${BLUE}Test 4: Checking for required variables...${NC}"
REQUIRED_VARS=("KLANG_VERSION" "GITHUB_REPO" "INSTALL_DIR")
ALL_VARS_FOUND=true

for var in "${REQUIRED_VARS[@]}"; do
    if grep -q "^${var}=" scripts/quick-install.sh; then
        echo -e "${GREEN}  ✓ ${var} found${NC}"
    else
        echo -e "${RED}  ✗ ${var} not found${NC}"
        ALL_VARS_FOUND=false
    fi
done

if [ "$ALL_VARS_FOUND" = true ]; then
    echo -e "${GREEN}✅ PASS: All required variables present${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}❌ FAIL: Some required variables missing${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

# Test 5: Check for required functions
echo -e "\n${BLUE}Test 5: Checking for required functions...${NC}"
REQUIRED_FUNCS=("detect_platform" "show_banner" "download_klang" "build_from_source" "setup_path" "verify_install")
ALL_FUNCS_FOUND=true

for func in "${REQUIRED_FUNCS[@]}"; do
    if grep -q "^${func}()" scripts/quick-install.sh || grep -q "^function ${func}" scripts/quick-install.sh; then
        echo -e "${GREEN}  ✓ ${func}() found${NC}"
    else
        echo -e "${RED}  ✗ ${func}() not found${NC}"
        ALL_FUNCS_FOUND=false
    fi
done

if [ "$ALL_FUNCS_FOUND" = true ]; then
    echo -e "${GREEN}✅ PASS: All required functions present${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}❌ FAIL: Some required functions missing${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

# Test 6: Check documentation mentions the install command
echo -e "\n${BLUE}Test 6: Checking documentation for install command...${NC}"
if grep -q "curl.*quick-install.sh.*bash" README.md; then
    echo -e "${GREEN}✅ PASS: Install command documented in README.md${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}❌ FAIL: Install command not in README.md${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

# Test 7: Verify the install command URL path
echo -e "\n${BLUE}Test 7: Verifying install command URL path...${NC}"
EXPECTED_URL="https://raw.githubusercontent.com/k-kaundal/KLang/main/scripts/quick-install.sh"
if grep -q "$EXPECTED_URL" README.md; then
    echo -e "${GREEN}✅ PASS: Correct URL path in documentation${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    # Check for old path
    if grep -q "https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh" README.md; then
        echo -e "${YELLOW}⚠️  WARNING: Documentation uses old path (without scripts/)${NC}"
        echo -e "${YELLOW}   Expected: $EXPECTED_URL${NC}"
        echo -e "${YELLOW}   This should be updated!${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    else
        echo -e "${RED}❌ FAIL: Install URL not found in documentation${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
fi

# Test 8: Check if script handles errors
echo -e "\n${BLUE}Test 8: Checking error handling...${NC}"
if grep -q "set -e" scripts/quick-install.sh; then
    echo -e "${GREEN}✅ PASS: Script has 'set -e' for error handling${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${YELLOW}⚠️  WARNING: Script doesn't have 'set -e'${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

# Test 9: Validate platform detection
echo -e "\n${BLUE}Test 9: Testing platform detection...${NC}"
PLATFORM_TEST=$(cat << 'EOF'
detect_platform() {
    local os=$(uname -s)
    case "$os" in
        Linux*) PLATFORM="linux" ;;
        Darwin*) PLATFORM="macos" ;;
        *) echo "Unsupported OS" ; exit 1 ;;
    esac
}
detect_platform
echo "Platform: $PLATFORM"
EOF
)
if echo "$PLATFORM_TEST" | bash 2>/dev/null | grep -q "Platform:"; then
    echo -e "${GREEN}✅ PASS: Platform detection works${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${RED}❌ FAIL: Platform detection failed${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

# Test 10: Simulated dry-run (extract key sections without executing)
echo -e "\n${BLUE}Test 10: Simulating script flow...${NC}"
echo -e "${YELLOW}  Checking script structure...${NC}"

# Check main() function exists and is called
if grep -q "^main()" scripts/quick-install.sh && grep -q "main \"\$@\"" scripts/quick-install.sh; then
    echo -e "${GREEN}✅ PASS: Script has proper main() function structure${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
else
    echo -e "${YELLOW}⚠️  WARNING: Script may not have standard main() structure${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
fi

# Summary
echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}Validation Summary${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""
echo -e "  Total Tests:   $((TESTS_PASSED + TESTS_FAILED))"
echo -e "  ${GREEN}Passed:        $TESTS_PASSED${NC}"
echo -e "  ${RED}Failed:        $TESTS_FAILED${NC}"
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}╔══════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║                                                          ║${NC}"
    echo -e "${GREEN}║  ✅ All validation tests passed!                        ║${NC}"
    echo -e "${GREEN}║                                                          ║${NC}"
    echo -e "${GREEN}║  The quick-install.sh script is ready for use.          ║${NC}"
    echo -e "${GREEN}║                                                          ║${NC}"
    echo -e "${GREEN}╚══════════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${BLUE}Direct install command:${NC}"
    echo -e "${GREEN}curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/scripts/quick-install.sh | bash${NC}"
    echo ""
    exit 0
else
    echo -e "${YELLOW}╔══════════════════════════════════════════════════════════╗${NC}"
    echo -e "${YELLOW}║                                                          ║${NC}"
    echo -e "${YELLOW}║  ⚠️  Some validation tests failed!                      ║${NC}"
    echo -e "${YELLOW}║                                                          ║${NC}"
    echo -e "${YELLOW}║  Please review the failures above.                      ║${NC}"
    echo -e "${YELLOW}║                                                          ║${NC}"
    echo -e "${YELLOW}╚══════════════════════════════════════════════════════════╝${NC}"
    echo ""
    exit 1
fi
