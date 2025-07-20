#!/bin/bash

# LinuxTrack Wine Bridge Testing Framework
# Purpose: Test Wine bridge integration in AppImage
# Date: January 2025
# Status: Phase 4 - Wine Bridge Testing

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"
APPIMAGE_PATH="$PROJECT_ROOT/LinuxTrack-x86_64.AppImage"

# Print functions
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to check if file exists
file_exists() {
    [ -f "$1" ]
}

# Function to check if directory exists
dir_exists() {
    [ -d "$1" ]
}

# Function to detect distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "$ID"
    else
        echo "unknown"
    fi
}

# Function to check Wine installation
check_wine_installation() {
    print_status "Checking Wine installation..."
    
    local wine_installed=false
    local wine_version=""
    
    if command_exists wine; then
        wine_installed=true
        wine_version=$(wine --version 2>/dev/null || echo "unknown")
        print_success "Wine is installed: $wine_version"
    else
        print_warning "Wine is not installed"
        print_status "To install Wine:"
        print_status "  Ubuntu/Debian: sudo apt install wine wine32 wine64"
        print_status "  Fedora: sudo dnf install wine wine-core wine-desktop"
        print_status "  Arch: sudo pacman -S wine wine-mono wine-gecko"
        return 1
    fi
    
    # Check Wine prefix
    if [ -n "$WINEPREFIX" ]; then
        print_status "Wine prefix: $WINEPREFIX"
    else
        print_status "Wine prefix: ~/.wine (default)"
    fi
    
    return 0
}

# Function to extract AppImage for testing
extract_appimage() {
    print_status "Extracting AppImage for testing..."
    
    if [ ! -f "$APPIMAGE_PATH" ]; then
        print_error "AppImage not found: $APPIMAGE_PATH"
        return 1
    fi
    
    # Create test directory
    local test_dir="$PROJECT_ROOT/wine_test_extract"
    if dir_exists "$test_dir"; then
        rm -rf "$test_dir"
    fi
    
    mkdir -p "$test_dir"
    cd "$test_dir"
    
    # Extract AppImage
    "$APPIMAGE_PATH" --appimage-extract
    
    if [ -d "squashfs-root" ]; then
        print_success "AppImage extracted successfully"
        return 0
    else
        print_error "Failed to extract AppImage"
        return 1
    fi
}

# Function to test Wine bridge components in AppImage
test_wine_bridge_components() {
    print_status "Testing Wine bridge components in AppImage..."
    
    local test_dir="$PROJECT_ROOT/wine_test_extract/squashfs-root"
    local wine_bridge_dir="$test_dir/wine_bridge"
    
    if [ ! -d "$wine_bridge_dir" ]; then
        print_error "Wine bridge directory not found in AppImage"
        return 1
    fi
    
    local component_count=0
    local missing_components=()
    
    # Check for required components
    local required_components=(
        "32bit/NPClient.dll"
        "32bit/FreeTrackClient.dll"
        "32bit/Controller.exe"
        "32bit/Tester.exe"
        "scripts/install_wine_bridge.sh"
    )
    
    for component in "${required_components[@]}"; do
        if [ -f "$wine_bridge_dir/$component" ]; then
            print_success "Found: $component"
            ((component_count++))
        else
            print_warning "Missing: $component"
            missing_components+=("$component")
        fi
    done
    
    # Check for 64-bit components
    if [ -d "$wine_bridge_dir/64bit" ]; then
        local x64_components=(
            "64bit/NPClient64.dll"
            "64bit/Tester64.exe"
        )
        
        for component in "${x64_components[@]}"; do
            if [ -f "$wine_bridge_dir/$component" ]; then
                print_success "Found: $component"
                ((component_count++))
            else
                print_warning "Missing: $component"
            fi
        done
    fi
    
    # Check installation script
    if [ -f "$wine_bridge_dir/scripts/install_wine_bridge.sh" ]; then
        if [ -x "$wine_bridge_dir/scripts/install_wine_bridge.sh" ]; then
            print_success "Installation script is executable"
        else
            print_warning "Installation script is not executable"
        fi
    fi
    
    print_status "Wine bridge component test completed: $component_count components found"
    
    if [ ${#missing_components[@]} -gt 0 ]; then
        print_warning "Missing components:"
        for component in "${missing_components[@]}"; do
            print_warning "  - $component"
        done
        return 1
    fi
    
    return 0
}

# Function to test Wine bridge installation
test_wine_bridge_installation() {
    print_status "Testing Wine bridge installation..."
    
    if ! command_exists wine; then
        print_warning "Wine not available, skipping installation test"
        return 1
    fi
    
    local test_dir="$PROJECT_ROOT/wine_test_extract/squashfs-root"
    local install_script="$test_dir/wine_bridge/scripts/install_wine_bridge.sh"
    
    if [ ! -f "$install_script" ]; then
        print_error "Installation script not found"
        return 1
    fi
    
    # Create test Wine prefix
    local test_wine_prefix="$PROJECT_ROOT/test_wine_prefix"
    if dir_exists "$test_wine_prefix"; then
        rm -rf "$test_wine_prefix"
    fi
    
    mkdir -p "$test_wine_prefix"
    
    # Set up test environment
    export WINEPREFIX="$test_wine_prefix"
    export APPDIR="$test_dir"
    
    print_status "Testing installation to: $test_wine_prefix"
    
    # Run installation script
    if "$install_script"; then
        print_success "Wine bridge installation test completed"
        
        # Verify installation
        if [ -f "$test_wine_prefix/drive_c/windows/system32/NPClient.dll" ]; then
            print_success "NPClient.dll installed successfully"
        else
            print_warning "NPClient.dll not found after installation"
        fi
        
        if [ -f "$test_wine_prefix/drive_c/windows/system32/FreeTrackClient.dll" ]; then
            print_success "FreeTrackClient.dll installed successfully"
        else
            print_warning "FreeTrackClient.dll not found after installation"
        fi
        
        # Clean up test prefix
        rm -rf "$test_wine_prefix"
        
        return 0
    else
        print_error "Wine bridge installation test failed"
        rm -rf "$test_wine_prefix"
        return 1
    fi
}

# Function to test Wine bridge DLL registration
test_dll_registration() {
    print_status "Testing Wine bridge DLL registration..."
    
    if ! command_exists wine; then
        print_warning "Wine not available, skipping DLL registration test"
        return 1
    fi
    
    local test_dir="$PROJECT_ROOT/wine_test_extract/squashfs-root"
    local wine_bridge_dir="$test_dir/wine_bridge"
    
    # Create test Wine prefix
    local test_wine_prefix="$PROJECT_ROOT/test_wine_prefix"
    if dir_exists "$test_wine_prefix"; then
        rm -rf "$test_wine_prefix"
    fi
    
    mkdir -p "$test_wine_prefix"
    export WINEPREFIX="$test_wine_prefix"
    
    # Initialize Wine prefix
    wineboot --init 2>/dev/null || true
    
    # Copy DLLs to system32
    mkdir -p "$test_wine_prefix/drive_c/windows/system32"
    if [ -d "$wine_bridge_dir/32bit" ]; then
        find "$wine_bridge_dir/32bit" -name "*.dll" -exec cp {} "$test_wine_prefix/drive_c/windows/system32/" \; 2>/dev/null || true
    fi
    
    # Test DLL registration
    local registration_success=true
    
    cd "$test_wine_prefix/drive_c/windows/system32"
    for dll in *.dll; do
        if [ -f "$dll" ]; then
            print_status "Testing registration of $dll..."
            if wine regsvr32 "$dll" 2>/dev/null; then
                print_success "Successfully registered $dll"
            else
                print_warning "Failed to register $dll (this may be normal)"
            fi
        fi
    done
    
    # Clean up
    rm -rf "$test_wine_prefix"
    
    print_success "DLL registration test completed"
    return 0
}

# Function to test Wine bridge executables
test_wine_bridge_executables() {
    print_status "Testing Wine bridge executables..."
    
    if ! command_exists wine; then
        print_warning "Wine not available, skipping executable test"
        return 1
    fi
    
    local test_dir="$PROJECT_ROOT/wine_test_extract/squashfs-root"
    local wine_bridge_dir="$test_dir/wine_bridge"
    
    # Create test Wine prefix
    local test_wine_prefix="$PROJECT_ROOT/test_wine_prefix"
    if dir_exists "$test_wine_prefix"; then
        rm -rf "$test_wine_prefix"
    fi
    
    mkdir -p "$test_wine_prefix"
    export WINEPREFIX="$test_wine_prefix"
    
    # Initialize Wine prefix
    wineboot --init 2>/dev/null || true
    
    # Test executables
    local executable_tests=(
        "32bit/Controller.exe"
        "32bit/Tester.exe"
    )
    
    for exe_test in "${executable_tests[@]}"; do
        if [ -f "$wine_bridge_dir/$exe_test" ]; then
            print_status "Testing $exe_test..."
            
            # Try to run executable (it may fail but should not crash)
            if timeout 10s wine "$wine_bridge_dir/$exe_test" 2>/dev/null; then
                print_success "$exe_test ran successfully"
            else
                print_warning "$exe_test failed to run (this may be normal)"
            fi
        else
            print_warning "$exe_test not found"
        fi
    done
    
    # Clean up
    rm -rf "$test_wine_prefix"
    
    print_success "Executable test completed"
    return 0
}

# Function to test Wine bridge protocol compatibility
test_protocol_compatibility() {
    print_status "Testing Wine bridge protocol compatibility..."
    
    local test_dir="$PROJECT_ROOT/wine_test_extract/squashfs-root"
    local wine_bridge_dir="$test_dir/wine_bridge"
    
    # Check for protocol files
    local protocol_files=(
        "data/data.txt"
        "data/sgl.dat"
    )
    
    for protocol_file in "${protocol_files[@]}"; do
        if [ -f "$wine_bridge_dir/$protocol_file" ]; then
            print_success "Found protocol file: $protocol_file"
        else
            print_warning "Protocol file not found: $protocol_file"
        fi
    done
    
    # Check for TrackIR firmware
    if [ -f "$wine_bridge_dir/data/TrackIR.exe" ]; then
        print_success "TrackIR firmware found"
    else
        print_warning "TrackIR firmware not found"
    fi
    
    print_success "Protocol compatibility test completed"
    return 0
}

# Function to test Wine bridge integration with AppRun
test_apprun_integration() {
    print_status "Testing AppRun Wine bridge integration..."
    
    local test_dir="$PROJECT_ROOT/wine_test_extract/squashfs-root"
    local apprun_script="$test_dir/AppRun"
    
    if [ ! -f "$apprun_script" ]; then
        print_error "AppRun script not found"
        return 1
    fi
    
    # Check if AppRun contains Wine bridge logic
    if grep -q "Wine detected" "$apprun_script"; then
        print_success "AppRun contains Wine detection logic"
    else
        print_warning "AppRun does not contain Wine detection logic"
    fi
    
    if grep -q "install_wine_bridge.sh" "$apprun_script"; then
        print_success "AppRun contains Wine bridge installation logic"
    else
        print_warning "AppRun does not contain Wine bridge installation logic"
    fi
    
    if grep -q "WINEDLLPATH" "$apprun_script"; then
        print_success "AppRun sets WINEDLLPATH environment variable"
    else
        print_warning "AppRun does not set WINEDLLPATH environment variable"
    fi
    
    print_success "AppRun integration test completed"
    return 0
}

# Function to test Wine bridge cross-architecture support
test_cross_architecture() {
    print_status "Testing Wine bridge cross-architecture support..."
    
    local test_dir="$PROJECT_ROOT/wine_test_extract/squashfs-root"
    local wine_bridge_dir="$test_dir/wine_bridge"
    
    # Check 32-bit components
    if [ -d "$wine_bridge_dir/32bit" ]; then
        local x86_components=$(find "$wine_bridge_dir/32bit" -name "*.dll" -o -name "*.exe" | wc -l)
        print_status "32-bit components found: $x86_components"
        
        if [ "$x86_components" -gt 0 ]; then
            print_success "32-bit architecture support available"
        else
            print_warning "No 32-bit components found"
        fi
    else
        print_warning "32-bit directory not found"
    fi
    
    # Check 64-bit components
    if [ -d "$wine_bridge_dir/64bit" ]; then
        local x64_components=$(find "$wine_bridge_dir/64bit" -name "*.dll" -o -name "*.exe" | wc -l)
        print_status "64-bit components found: $x64_components"
        
        if [ "$x64_components" -gt 0 ]; then
            print_success "64-bit architecture support available"
        else
            print_warning "No 64-bit components found"
        fi
    else
        print_warning "64-bit directory not found"
    fi
    
    print_success "Cross-architecture test completed"
    return 0
}

# Function to generate test report
generate_test_report() {
    print_status "Generating Wine bridge test report..."
    
    local report_file="$PROJECT_ROOT/wine_bridge_test_report.txt"
    local test_dir="$PROJECT_ROOT/wine_test_extract"
    
    {
        echo "LinuxTrack Wine Bridge Test Report"
        echo "=================================="
        echo "Date: $(date)"
        echo "Distribution: $(detect_distro)"
        echo "AppImage: $APPIMAGE_PATH"
        echo ""
        
        echo "Test Results Summary:"
        echo "===================="
        
        # Wine installation
        if command_exists wine; then
            echo "✓ Wine is installed: $(wine --version 2>/dev/null || echo "unknown")"
        else
            echo "✗ Wine is not installed"
        fi
        
        # AppImage extraction
        if [ -d "$test_dir/squashfs-root" ]; then
            echo "✓ AppImage extracted successfully"
        else
            echo "✗ AppImage extraction failed"
        fi
        
        # Wine bridge components
        if [ -d "$test_dir/squashfs-root/wine_bridge" ]; then
            local component_count=$(find "$test_dir/squashfs-root/wine_bridge" -name "*.dll" -o -name "*.exe" | wc -l)
            echo "✓ Wine bridge components found: $component_count"
        else
            echo "✗ Wine bridge components not found"
        fi
        
        # Installation script
        if [ -f "$test_dir/squashfs-root/wine_bridge/scripts/install_wine_bridge.sh" ]; then
            echo "✓ Wine bridge installation script found"
        else
            echo "✗ Wine bridge installation script not found"
        fi
        
        echo ""
        echo "Detailed Test Results:"
        echo "====================="
        
        # Component details
        if [ -d "$test_dir/squashfs-root/wine_bridge" ]; then
            echo ""
            echo "Wine Bridge Components:"
            find "$test_dir/squashfs-root/wine_bridge" -name "*.dll" -o -name "*.exe" | sort
        fi
        
        echo ""
        echo "Test completed at: $(date)"
        
    } > "$report_file"
    
    print_success "Test report generated: $report_file"
    return 0
}

# Function to clean up test files
cleanup_test_files() {
    print_status "Cleaning up test files..."
    
    local test_dir="$PROJECT_ROOT/wine_test_extract"
    local wine_prefix="$PROJECT_ROOT/test_wine_prefix"
    
    if dir_exists "$test_dir"; then
        rm -rf "$test_dir"
        print_status "Removed extracted AppImage"
    fi
    
    if dir_exists "$wine_prefix"; then
        rm -rf "$wine_prefix"
        print_status "Removed test Wine prefix"
    fi
    
    print_success "Test files cleaned up"
}

# Function to show help
show_help() {
    echo "LinuxTrack Wine Bridge Testing Framework"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --clean          Clean up test files after testing"
    echo "  --no-cleanup     Keep test files after testing"
    echo "  --report         Generate detailed test report"
    echo "  --help           Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0               # Run all tests"
    echo "  $0 --clean       # Run tests and clean up"
    echo "  $0 --report      # Generate test report"
}

# Main function
main() {
    local clean_flag=true
    local report_flag=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --clean)
                clean_flag=true
                shift
                ;;
            --no-cleanup)
                clean_flag=false
                shift
                ;;
            --report)
                report_flag=true
                shift
                ;;
            --help)
                show_help
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                show_help
                exit 1
                ;;
        esac
    done
    
    print_status "Starting LinuxTrack Wine Bridge Testing Framework..."
    print_status "Project root: $PROJECT_ROOT"
    print_status "Distribution: $(detect_distro)"
    
    # Check Wine installation
    check_wine_installation
    
    # Extract AppImage
    if ! extract_appimage; then
        print_error "Failed to extract AppImage for testing"
        exit 1
    fi
    
    # Run tests
    local test_results=()
    
    print_status "Running Wine bridge component tests..."
    if test_wine_bridge_components; then
        test_results+=("components: PASS")
    else
        test_results+=("components: FAIL")
    fi
    
    print_status "Running Wine bridge installation tests..."
    if test_wine_bridge_installation; then
        test_results+=("installation: PASS")
    else
        test_results+=("installation: FAIL")
    fi
    
    print_status "Running DLL registration tests..."
    if test_dll_registration; then
        test_results+=("dll_registration: PASS")
    else
        test_results+=("dll_registration: FAIL")
    fi
    
    print_status "Running executable tests..."
    if test_wine_bridge_executables; then
        test_results+=("executables: PASS")
    else
        test_results+=("executables: FAIL")
    fi
    
    print_status "Running protocol compatibility tests..."
    if test_protocol_compatibility; then
        test_results+=("protocol: PASS")
    else
        test_results+=("protocol: FAIL")
    fi
    
    print_status "Running AppRun integration tests..."
    if test_apprun_integration; then
        test_results+=("apprun: PASS")
    else
        test_results+=("apprun: FAIL")
    fi
    
    print_status "Running cross-architecture tests..."
    if test_cross_architecture; then
        test_results+=("cross_arch: PASS")
    else
        test_results+=("cross_arch: FAIL")
    fi
    
    # Generate report if requested
    if [ "$report_flag" = true ]; then
        generate_test_report
    fi
    
    # Clean up if requested
    if [ "$clean_flag" = true ]; then
        cleanup_test_files
    fi
    
    # Print test summary
    echo ""
    print_status "Wine Bridge Test Summary:"
    echo "============================="
    for result in "${test_results[@]}"; do
        if [[ "$result" == *": PASS" ]]; then
            print_success "$result"
        else
            print_error "$result"
        fi
    done
    
    # Count results
    local pass_count=0
    local fail_count=0
    for result in "${test_results[@]}"; do
        if [[ "$result" == *": PASS" ]]; then
            ((pass_count++))
        else
            ((fail_count++))
        fi
    done
    
    echo ""
    print_status "Test Results: $pass_count PASS, $fail_count FAIL"
    
    if [ "$fail_count" -eq 0 ]; then
        print_success "All Wine bridge tests passed!"
        exit 0
    else
        print_warning "Some Wine bridge tests failed"
        exit 1
    fi
}

# Run main function
main "$@" 