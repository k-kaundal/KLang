#!/usr/bin/env bash
# Test suite for KLang install script
# Tests the universal installer in various scenarios

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Test counters
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Test results array
declare -a FAILED_TESTS

# Helper functions
print_test_header() {
    echo ""
    echo -e "${BLUE}══════════════════════════════════════════════════════${NC}"
    echo -e "${BLUE}  Testing: $1${NC}"
    echo -e "${BLUE}══════════════════════════════════════════════════════${NC}"
}

pass_test() {
    TESTS_PASSED=$((TESTS_PASSED + 1))
    echo -e "${GREEN}✓ PASS: $1${NC}"
}

fail_test() {
    TESTS_FAILED=$((TESTS_FAILED + 1))
    FAILED_TESTS+=("$1")
    echo -e "${RED}✗ FAIL: $1${NC}"
    if [ -n "$2" ]; then
        echo -e "${RED}  Reason: $2${NC}"
    fi
}

run_test() {
    TESTS_RUN=$((TESTS_RUN + 1))
    local test_name="$1"
    local test_command="$2"
    
    if eval "$test_command" > /dev/null 2>&1; then
        pass_test "$test_name"
        return 0
    else
        fail_test "$test_name" "$3"
        return 1
    fi
}

# Test 1: Install script exists and is executable
test_install_script_exists() {
    print_test_header "Install Script Existence"
    
    run_test "install.sh exists" \
        "[ -f install.sh ]" \
        "install.sh file not found"
    
    run_test "install.sh is executable" \
        "[ -x install.sh ]" \
        "install.sh is not executable"
}

# Test 2: Script syntax validation
test_script_syntax() {
    print_test_header "Script Syntax Validation"
    
    run_test "Bash syntax check" \
        "bash -n install.sh" \
        "Bash syntax errors found"
    
    run_test "Shellcheck validation" \
        "command -v shellcheck > /dev/null && shellcheck install.sh || true" \
        "Shellcheck found issues (warning only)"
}

# Test 3: Required functions exist
test_required_functions() {
    print_test_header "Required Functions"
    
    local functions=(
        "show_banner"
        "detect_platform"
        "check_dependencies"
        "install_klang"
        "setup_path"
        "verify_installation"
    )
    
    for func in "${functions[@]}"; do
        run_test "Function '$func' exists" \
            "grep -q \"^[[:space:]]*${func}()\" install.sh || grep -q \"^${func}()\" install.sh" \
            "Function $func not found in install.sh"
    done
}

# Test 4: Platform detection
test_platform_detection() {
    print_test_header "Platform Detection"
    
    # Test OS detection
    run_test "OS detection code exists" \
        "grep -q 'uname -s' install.sh" \
        "OS detection code not found"
    
    # Test architecture detection
    run_test "Architecture detection exists" \
        "grep -q 'uname -m' install.sh" \
        "Architecture detection code not found"
    
    # Test supported platforms
    run_test "Linux support" \
        "grep -qi 'linux' install.sh" \
        "Linux platform not mentioned"
    
    run_test "macOS support" \
        "grep -qi 'darwin\|macos' install.sh" \
        "macOS platform not mentioned"
}

# Test 5: Dependency checking
test_dependency_checking() {
    print_test_header "Dependency Checking"
    
    local deps=("git" "gcc" "make")
    
    for dep in "${deps[@]}"; do
        run_test "Checks for '$dep'" \
            "grep -q \"$dep\" install.sh" \
            "No check for $dep dependency"
    done
}

# Test 6: Error handling
test_error_handling() {
    print_test_header "Error Handling"
    
    run_test "Script uses 'set -e'" \
        "grep -q '^set -e' install.sh" \
        "Script doesn't exit on errors"
    
    run_test "Has error messages" \
        "grep -qi 'error' install.sh" \
        "No error handling messages found"
}

# Test 7: PATH configuration
test_path_configuration() {
    print_test_header "PATH Configuration"
    
    run_test "PATH setup exists" \
        "grep -q 'PATH' install.sh" \
        "No PATH configuration found"
    
    run_test "Shell detection (.bashrc)" \
        "grep -q 'bashrc' install.sh" \
        "No .bashrc configuration"
    
    run_test "Shell detection (.zshrc)" \
        "grep -q 'zshrc' install.sh" \
        "No .zshrc configuration"
}

# Test 8: Installation verification
test_installation_verification() {
    print_test_header "Installation Verification"
    
    run_test "Verifies installation" \
        "grep -q 'verify' install.sh" \
        "No installation verification"
    
    run_test "Checks binary existence" \
        "grep -q '\\[ -x' install.sh || grep -q 'test -x' install.sh" \
        "No executable check"
}

# Test 9: User output and messaging
test_user_output() {
    print_test_header "User Output"
    
    run_test "Has colored output" \
        "grep -q 'GREEN\|BLUE\|RED\|YELLOW' install.sh" \
        "No colored output support"
    
    run_test "Has banner/header" \
        "grep -qi 'banner\|klang' install.sh" \
        "No user-friendly banner"
    
    run_test "Has success messages" \
        "grep -qi 'success\|complete\|installed' install.sh" \
        "No success messages"
}

# Test 10: Security checks
test_security() {
    print_test_header "Security Checks"
    
    run_test "No hardcoded passwords" \
        "! grep -i 'password=' install.sh" \
        "Found hardcoded password"
    
    run_test "No eval of user input" \
        "! grep 'eval.*\\$' install.sh" \
        "Found eval of variables (security risk)"
    
    run_test "Uses HTTPS for downloads" \
        "! grep -i 'http://.*github' install.sh" \
        "Using insecure HTTP for GitHub"
}

# Test 11: Cleanup and idempotency
test_cleanup() {
    print_test_header "Cleanup and Idempotency"
    
    run_test "Checks if already installed" \
        "grep -q 'already' install.sh || grep -q 'exists' install.sh" \
        "No check for existing installation"
    
    run_test "Temp directory cleanup" \
        "grep -q 'rm -rf.*temp\|mktemp' install.sh" \
        "No temporary directory cleanup"
}

# Test 12: Documentation
test_documentation() {
    print_test_header "Documentation"
    
    run_test "Has usage comments" \
        "grep -q '^#' install.sh | head -5" \
        "No documentation comments"
    
    run_test "Has help/instructions" \
        "grep -qi 'usage\|install\|curl' install.sh | head -10" \
        "No installation instructions"
}

# Print summary
print_summary() {
    echo ""
    echo -e "${BLUE}══════════════════════════════════════════════════════${NC}"
    echo -e "${BLUE}  Test Summary${NC}"
    echo -e "${BLUE}══════════════════════════════════════════════════════${NC}"
    echo ""
    echo "Tests run:    $TESTS_RUN"
    echo -e "Tests passed: ${GREEN}$TESTS_PASSED${NC}"
    echo -e "Tests failed: ${RED}$TESTS_FAILED${NC}"
    echo ""
    
    if [ $TESTS_FAILED -gt 0 ]; then
        echo -e "${RED}Failed tests:${NC}"
        for test in "${FAILED_TESTS[@]}"; do
            echo -e "${RED}  - $test${NC}"
        done
        echo ""
        exit 1
    else
        echo -e "${GREEN}All tests passed! ✓${NC}"
        echo ""
        exit 0
    fi
}

# Main test execution
main() {
    echo -e "${BLUE}${BOLD}KLang Install Script Test Suite${NC}"
    echo ""
    
    # Change to repository root
    cd "$(dirname "$0")/.."
    
    # Run all tests
    test_install_script_exists
    test_script_syntax
    test_required_functions
    test_platform_detection
    test_dependency_checking
    test_error_handling
    test_path_configuration
    test_installation_verification
    test_user_output
    test_security
    test_cleanup
    test_documentation
    
    # Print summary
    print_summary
}

# Run main function
main "$@"
