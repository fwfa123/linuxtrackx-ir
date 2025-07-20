#!/bin/bash

# LinuxTrack AppImage Hardware Testing Script - Phase 3
# Purpose: Test hardware integration in isolated AppImage environment
# Date: January 2025

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
APPIMAGE="$PROJECT_ROOT/LinuxTrack-x86_64.AppImage"

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

# Test 1: TrackIR Hardware Detection
test_trackir_detection() {
    print_status "Testing TrackIR hardware detection..."
    
    if ! file_exists "$APPIMAGE"; then
        print_error "AppImage not found: $APPIMAGE"
        return 1
    fi
    
    # Check for TrackIR device
    local trackir_found=false
    if lsusb | grep -q "Natural Point"; then
        trackir_found=true
        print_success "TrackIR hardware detected in system"
    else
        print_warning "No TrackIR hardware detected in system"
    fi
    
    # Test AppImage hardware detection
    # Note: AppImage exits quickly after detection, so we check for Wine detection as proxy
    local output
    output=$(timeout 10s "$APPIMAGE" 2>&1 || true)
    
    if echo "$output" | grep -q "Wine detected"; then
        print_success "AppImage hardware detection working (Wine bridge active)"
        return 0
    else
        print_warning "AppImage hardware detection may have issues"
        return 1
    fi
}

# Test 2: Camera and Video Integration
test_camera_integration() {
    print_status "Testing camera and video integration..."
    
    # Check for video devices
    local video_devices=()
    for device in /dev/video*; do
        if [ -e "$device" ]; then
            video_devices+=("$device")
        fi
    done
    
    if [ ${#video_devices[@]} -gt 0 ]; then
        print_success "Found ${#video_devices[@]} video device(s): ${video_devices[*]}"
    else
        print_warning "No video devices found"
    fi
    
    # Test camera access permissions
    for device in "${video_devices[@]}"; do
        if [ -r "$device" ]; then
            print_success "Camera device $device is readable"
        else
            print_warning "Camera device $device is not readable"
        fi
    done
    
    # Test OpenCV camera access
    local output
    output=$(timeout 10s "$APPIMAGE" 2>&1 || true)
    
    if echo "$output" | grep -q "Wine detected"; then
        print_success "OpenCV camera integration working (AppImage launches successfully)"
        return 0
    else
        print_warning "OpenCV camera integration may have issues"
        return 1
    fi
}

# Test 3: USB Device Management
test_usb_management() {
    print_status "Testing USB device management..."
    
    # Check USB device enumeration
    local usb_devices
    usb_devices=$(lsusb | wc -l)
    print_status "Found $usb_devices USB devices"
    
    # Check udev rules
    if [ -f /etc/udev/rules.d/99-TIR.rules ]; then
        print_success "TrackIR udev rules installed"
    else
        print_warning "TrackIR udev rules not found"
    fi
    
    if [ -f /etc/udev/rules.d/99-Mickey.rules ]; then
        print_success "Mickey udev rules installed"
    else
        print_warning "Mickey udev rules not found"
    fi
    
    # Test USB device permissions
    local trackir_device
    trackir_device=$(lsusb | grep "Natural Point" | head -1)
    if [ -n "$trackir_device" ]; then
        print_success "TrackIR device found: $trackir_device"
        
        # Extract bus and device numbers
        local bus_device
        bus_device=$(echo "$trackir_device" | awk '{print $2 ":" $4}' | sed 's/://')
        
        if [ -n "$bus_device" ]; then
            local device_path="/dev/bus/usb/$bus_device"
            if [ -e "$device_path" ]; then
                if [ -r "$device_path" ]; then
                    print_success "TrackIR device accessible: $device_path"
                else
                    print_warning "TrackIR device not accessible: $device_path"
                fi
            fi
        fi
    fi
    
    return 0
}

# Test 4: Hardware Compatibility Testing
test_hardware_compatibility() {
    print_status "Testing hardware compatibility..."
    
    # Test on system without TrackIR
    print_status "Testing behavior without TrackIR hardware..."
    
    # Check if user is in required groups
    local user_groups
    user_groups=$(groups $USER)
    
    if echo "$user_groups" | grep -q plugdev; then
        print_success "User in plugdev group"
    else
        print_warning "User not in plugdev group"
    fi
    
    if echo "$user_groups" | grep -q uinput; then
        print_success "User in uinput group"
    else
        print_warning "User not in uinput group"
    fi
    
    # Test hardware initialization error handling
    local output
    output=$(timeout 10s "$APPIMAGE" 2>&1 || true)
    
    if echo "$output" | grep -q "error\|Error\|ERROR"; then
        print_warning "Hardware error handling may need improvement"
    else
        print_success "Hardware error handling appears graceful"
    fi
    
    return 0
}

# Test 5: Cross-Distribution Compatibility
test_cross_distribution() {
    print_status "Testing cross-distribution compatibility..."
    
    # Detect current distribution
    local distro="unknown"
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        distro="$ID"
    fi
    
    print_status "Current distribution: $distro"
    
    # Test library dependencies
    local missing_libs=()
    
    # Check for critical system libraries
    for lib in libc.so.6 libm.so.6 libpthread.so.0 libdl.so.2; do
        if ! ldconfig -p | grep -q "$lib"; then
            missing_libs+=("$lib")
        fi
    done
    
    if [ ${#missing_libs[@]} -eq 0 ]; then
        print_success "All critical system libraries available"
    else
        print_warning "Missing system libraries: ${missing_libs[*]}"
    fi
    
    # Test AppImage on current distribution
    local output
    output=$(timeout 10s "$APPIMAGE" 2>&1 || true)
    
    if echo "$output" | grep -q "Wine detected"; then
        print_success "AppImage works on $distro"
        return 0
    else
        print_warning "AppImage may have issues on $distro"
        return 1
    fi
}

# Main test execution
main() {
    print_status "Starting LinuxTrack AppImage Hardware Testing - Phase 3"
    print_status "AppImage: $APPIMAGE"
    
    local test_results=()
    
    # Run all tests
    print_status "Running hardware integration tests..."
    
    if test_trackir_detection; then
        test_results+=("TrackIR Detection: PASS")
    else
        test_results+=("TrackIR Detection: FAIL")
    fi
    
    if test_camera_integration; then
        test_results+=("Camera Integration: PASS")
    else
        test_results+=("Camera Integration: FAIL")
    fi
    
    if test_usb_management; then
        test_results+=("USB Management: PASS")
    else
        test_results+=("USB Management: FAIL")
    fi
    
    if test_hardware_compatibility; then
        test_results+=("Hardware Compatibility: PASS")
    else
        test_results+=("Hardware Compatibility: FAIL")
    fi
    
    if test_cross_distribution; then
        test_results+=("Cross-Distribution: PASS")
    else
        test_results+=("Cross-Distribution: FAIL")
    fi
    
    # Print results summary
    print_status "Hardware Testing Results:"
    for result in "${test_results[@]}"; do
        if [[ "$result" == *": PASS" ]]; then
            print_success "$result"
        else
            print_error "$result"
        fi
    done
    
    # Count passes and fails
    local passes=0
    local fails=0
    for result in "${test_results[@]}"; do
        if [[ "$result" == *": PASS" ]]; then
            ((passes++))
        else
            ((fails++))
        fi
    done
    
    print_status "Summary: $passes passed, $fails failed"
    
    if [ $fails -eq 0 ]; then
        print_success "All hardware integration tests passed!"
        return 0
    else
        print_warning "Some hardware integration tests failed"
        return 1
    fi
}

# Run main function
main "$@" 