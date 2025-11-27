#!/bin/bash

# Wine Development Environment Test Script
# This script tests if Wine development tools are properly installed and functional

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Test results tracking
TESTS_PASSED=0
TESTS_FAILED=0
TESTS_TOTAL=0

# Function to print colored output
print_header() {
    echo -e "${CYAN}$1${NC}"
}

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
    ((TESTS_PASSED++))
    ((TESTS_TOTAL++))
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
    ((TESTS_FAILED++))
    ((TESTS_TOTAL++))
}

print_test() {
    echo -e "${BLUE}[TEST]${NC} $1"
    # Note: TESTS_TOTAL is incremented by print_success/print_error, not here
}

# Function to run a test
run_test() {
    local test_name="$1"
    local test_command="$2"
    
    print_test "$test_name"
    if eval "$test_command" >/dev/null 2>&1; then
        print_success "$test_name"
        return 0
    else
        print_error "$test_name"
        return 1
    fi
}

# Function to check command availability
check_command() {
    local cmd="$1"
    local alt_cmd="${2:-}"
    
    if command -v "$cmd" >/dev/null 2>&1; then
        print_success "$cmd is available"
        "$cmd" --version 2>/dev/null | head -n 1 || true
        return 0
    elif [ -n "$alt_cmd" ] && command -v "$alt_cmd" >/dev/null 2>&1; then
        # Alternative command found - still counts as a successful test
        echo -e "${YELLOW}[WARNING]${NC} $cmd not found, but $alt_cmd is available"
        print_success "$alt_cmd is available (alternative to $cmd)"
        "$alt_cmd" --version 2>/dev/null | head -n 1 || true
        return 0
    else
        print_error "$cmd is not available"
        return 1
    fi
}

# Function to check for 32-bit libraries
check_32bit_libs() {
    print_test "Checking for 32-bit library directories"
    
    local found=0
    local lib_dirs=(
        "/usr/lib/i386-linux-gnu"
        "/usr/lib32"
        "/lib32"
        "/usr/lib/i686-linux-gnu"
    )
    
    for dir in "${lib_dirs[@]}"; do
        if [ -d "$dir" ]; then
            print_success "Found 32-bit library directory: $dir"
            found=1
            break
        fi
    done
    
    if [ $found -eq 0 ]; then
        # Still count as a test, but as informational (not pass/fail)
        # since missing 32-bit libs may be normal on some systems
        echo -e "${YELLOW}[WARNING]${NC} No 32-bit library directories found (this may be normal on some systems)"
        # Increment total to keep test count consistent
        ((TESTS_TOTAL++))
    fi
}

# Function to test winegcc compilation
test_winegcc_compilation() {
    print_test "Testing winegcc compilation"
    
    # Create a temporary test C file
    local test_file=$(mktemp /tmp/wine_test_XXXXXX.c)
    local test_exe="${test_file%.c}.exe"
    
    # Simple C program that prints "Hello from Wine!"
    cat > "$test_file" << 'EOF'
#include <stdio.h>
int main() {
    printf("Hello from Wine!\n");
    return 0;
}
EOF
    
    # Find winegcc command
    local winegcc_cmd=""
    if command -v winegcc >/dev/null 2>&1; then
        winegcc_cmd="winegcc"
    elif command -v winegcc-stable >/dev/null 2>&1; then
        winegcc_cmd="winegcc-stable"
    else
        print_error "winegcc compilation test (winegcc not found)"
        rm -f "$test_file" "$test_exe" 2>/dev/null
        return 1
    fi
    
    # Try to compile
    if "$winegcc_cmd" -o "$test_exe" "$test_file" 2>/dev/null; then
        if [ -f "$test_exe" ]; then
            print_success "winegcc compilation test (compiled successfully)"
            rm -f "$test_file" "$test_exe" 2>/dev/null
            return 0
        else
            print_error "winegcc compilation test (executable not created)"
            rm -f "$test_file" "$test_exe" 2>/dev/null
            return 1
        fi
    else
        print_error "winegcc compilation test (compilation failed)"
        rm -f "$test_file" "$test_exe" 2>/dev/null
        return 1
    fi
}

# Function to test wine execution
test_wine_execution() {
    print_test "Testing wine execution"
    
    # Create a temporary test C file
    local test_file=$(mktemp /tmp/wine_test_XXXXXX.c)
    local test_exe="${test_file%.c}.exe"
    
    # Simple C program
    cat > "$test_file" << 'EOF'
#include <stdio.h>
int main() {
    printf("Wine test successful!\n");
    return 0;
}
EOF
    
    # Find winegcc command
    local winegcc_cmd=""
    if command -v winegcc >/dev/null 2>&1; then
        winegcc_cmd="winegcc"
    elif command -v winegcc-stable >/dev/null 2>&1; then
        winegcc_cmd="winegcc-stable"
    else
        print_error "wine execution test (winegcc not found)"
        rm -f "$test_file" "$test_exe" 2>/dev/null
        return 1
    fi
    
    # Compile
    if ! "$winegcc_cmd" -o "$test_exe" "$test_file" 2>/dev/null; then
        print_error "wine execution test (compilation failed)"
        rm -f "$test_file" "$test_exe" 2>/dev/null
        return 1
    fi
    
    # Check if wine is available
    if ! command -v wine >/dev/null 2>&1; then
        print_error "wine execution test (wine not found)"
        rm -f "$test_file" "$test_exe" 2>/dev/null
        return 1
    fi
    
    # Try to run with wine
    if wine "$test_exe" >/dev/null 2>&1; then
        print_success "wine execution test (executed successfully)"
        rm -f "$test_file" "$test_exe" 2>/dev/null
        return 0
    else
        # Execution failed - count as failure but note it might be normal
        # Wine may require initial setup (winecfg) on first run
        echo -e "${YELLOW}[WARNING]${NC} wine execution test (execution may have failed, but this could be normal)"
        print_status "Note: Wine may require initial setup (winecfg) on first run"
        # Count as failed to keep accounting accurate, even though it might be expected
        ((TESTS_FAILED++))
        ((TESTS_TOTAL++))
        rm -f "$test_file" "$test_exe" 2>/dev/null
        return 0  # Don't return error code, as wine setup might be needed
    fi
}

# Main function
main() {
    print_header "Wine Development Environment Test"
    print_header "================================="
    echo ""
    
    print_status "Testing Wine development installation..."
    echo ""
    
    # Test 1: Check for winegcc
    echo "=== Compiler Tests ==="
    check_command "winegcc" "winegcc-stable"
    echo ""
    
    # Test 2: Check for wineg++
    check_command "wineg++" "wineg++-stable"
    echo ""
    
    # Test 3: Check for wrc (Wine Resource Compiler)
    check_command "wrc" "wrc-stable"
    echo ""
    
    # Test 4: Check for wine
    echo "=== Runtime Tests ==="
    check_command "wine"
    echo ""
    
    # Test 5: Check for 32-bit libraries
    echo "=== Library Tests ==="
    check_32bit_libs
    echo ""
    
    # Test 6: Test winegcc compilation
    echo "=== Compilation Tests ==="
    test_winegcc_compilation
    echo ""
    
    # Test 7: Test wine execution
    echo "=== Execution Tests ==="
    test_wine_execution
    echo ""
    
    # Summary
    print_header "Test Summary"
    print_header "============"
    echo ""
    echo "Total tests: $TESTS_TOTAL"
    echo -e "${GREEN}Passed: $TESTS_PASSED${NC}"
    echo -e "${RED}Failed: $TESTS_FAILED${NC}"
    echo ""
    
    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}[SUCCESS]${NC} All tests passed! Wine development environment is properly configured."
        return 0
    else
        echo -e "${RED}[ERROR]${NC} Some tests failed. Please review the errors above."
        print_status "You may need to:"
        print_status "  1. Run the installer: sudo ./install_wine_dev.sh"
        print_status "  2. Configure Wine: winecfg"
        print_status "  3. Check for missing dependencies"
        return 1
    fi
}

# Run main function
main "$@"
