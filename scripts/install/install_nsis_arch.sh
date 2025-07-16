#!/bin/bash

# LinuxTrack X-IR - NSIS Installation Script for Arch Linux
# This script helps install NSIS on Arch Linux systems when the AUR package has issues
# 
# Author: LinuxTrack X-IR Team
# License: MIT
# Version: 1.0

set -e  # Exit on any error

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
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

# Function to check if running as root
check_root() {
    if [[ $EUID -eq 0 ]]; then
        print_error "This script should not be run as root"
        print_status "Please run as a regular user with sudo privileges"
        exit 1
    fi
}

# Function to check if we're on Arch Linux
check_arch() {
    if [[ ! -f /etc/arch-release ]]; then
        print_error "This script is designed for Arch Linux systems"
        print_status "Detected system: $(cat /etc/os-release | grep PRETTY_NAME | cut -d'"' -f2)"
        exit 1
    fi
}

# Function to check if yay is installed
check_yay() {
    if ! command -v yay &> /dev/null; then
        print_warning "yay is not installed. Attempting to install it..."
        
        # Check if we have sudo access
        if ! sudo -n true 2>/dev/null; then
            print_error "sudo access required to install yay"
            print_status "Please run: sudo pacman -S --needed git base-devel"
            print_status "Then install yay manually from: https://github.com/Jguer/yay"
            exit 1
        fi
        
        # Install yay dependencies
        print_status "Installing yay dependencies..."
        sudo pacman -S --needed git base-devel
        
        # Clone and install yay
        print_status "Installing yay..."
        cd /tmp
        git clone https://aur.archlinux.org/yay.git
        cd yay
        makepkg -si --noconfirm
        cd -
        rm -rf /tmp/yay
        
        print_success "yay installed successfully"
    fi
}

# Function to check if makensis is already available
check_makensis() {
    if command -v makensis &> /dev/null; then
        local version=$(makensis /VERSION 2>/dev/null | head -n1 || echo "unknown")
        print_success "NSIS is already installed (version: $version)"
        return 0
    fi
    return 1
}

# Function to install NSIS via AUR
install_nsis_aur() {
    print_status "Attempting to install NSIS via AUR..."
    
    # Try to install via yay
    if yay -S nsis --noconfirm; then
        print_success "NSIS installed successfully via AUR"
        return 0
    else
        print_warning "AUR installation failed, trying alternative methods..."
        return 1
    fi
}

# Function to install NSIS manually
install_nsis_manual() {
    print_status "Installing NSIS manually from source..."
    
    # Create temporary directory
    local temp_dir=$(mktemp -d)
    cd "$temp_dir"
    
    # Download NSIS source
    print_status "Downloading NSIS source..."
    local nsis_version="3.09"
    local nsis_url="https://sourceforge.net/projects/nsis/files/NSIS%203/$nsis_version/nsis-$nsis_version-src.tar.bz2"
    
    if ! curl -L -o nsis-src.tar.bz2 "$nsis_url"; then
        print_error "Failed to download NSIS source"
        cd -
        rm -rf "$temp_dir"
        return 1
    fi
    
    # Extract source
    print_status "Extracting NSIS source..."
    tar -xf nsis-src.tar.bz2
    cd nsis-$nsis_version-src
    
    # Install build dependencies
    print_status "Installing build dependencies..."
    sudo pacman -S --needed scons pcre2 zlib bzip2
    
    # Build NSIS
    print_status "Building NSIS (this may take several minutes)..."
    if scons SKIPSTUBS=all SKIPPLUGINS=all SKIPUTILS=all SKIPMISC=all NSIS_CONFIG_CONST_DATA_PATH=no PREFIX=/usr/local install-compiler; then
        print_success "NSIS built and installed successfully"
        cd -
        rm -rf "$temp_dir"
        return 0
    else
        print_error "Failed to build NSIS"
        cd -
        rm -rf "$temp_dir"
        return 1
    fi
}

# Function to install NSIS via alternative package managers
install_nsis_alternative() {
    print_status "Trying alternative installation methods..."
    
    # Try pacman (in case it's in the main repos)
    if sudo pacman -S nsis --noconfirm 2>/dev/null; then
        print_success "NSIS installed via pacman"
        return 0
    fi
    
    # Try paru if available
    if command -v paru &> /dev/null; then
        print_status "Trying paru..."
        if paru -S nsis --noconfirm; then
            print_success "NSIS installed via paru"
            return 0
        fi
    fi
    
    return 1
}

# Function to verify NSIS installation
verify_installation() {
    if check_makensis; then
        print_success "NSIS installation verified successfully"
        
        # Test NSIS functionality
        print_status "Testing NSIS functionality..."
        local test_script=$(mktemp)
        cat > "$test_script" << 'EOF'
!define VERSION "1.0"
!define RELEASE "1"

Name "Test Installer"
OutFile "test-installer.exe"
InstallDir "$PROGRAMFILES\TestApp"

Section "Main Application"
    SetOutPath $INSTDIR
    File /oname=test.txt "test.txt"
SectionEnd
EOF
        
        # Create a test file
        echo "Test file" > test.txt
        
        if makensis "$test_script" >/dev/null 2>&1; then
            print_success "NSIS is working correctly"
            rm -f test.txt test-installer.exe
        else
            print_warning "NSIS installed but may have issues"
        fi
        
        rm -f "$test_script"
        return 0
    else
        print_error "NSIS installation verification failed"
        return 1
    fi
}

# Function to show usage
show_usage() {
    echo "LinuxTrack X-IR - NSIS Installation Script for Arch Linux"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -h, --help     Show this help message"
    echo "  -f, --force    Force reinstallation even if NSIS is already installed"
    echo "  -v, --verify   Only verify existing installation"
    echo "  -m, --manual   Skip AUR and install manually from source"
    echo ""
    echo "This script helps install NSIS on Arch Linux systems for building"
    echo "LinuxTrack X-IR Windows compatibility components."
    echo ""
    echo "The script will try multiple installation methods:"
    echo "1. AUR installation via yay"
    echo "2. Alternative package managers (paru)"
    echo "3. Manual installation from source"
    echo ""
}

# Main function
main() {
    local FORCE=false
    local VERIFY_ONLY=false
    local MANUAL_ONLY=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_usage
                exit 0
                ;;
            -f|--force)
                FORCE=true
                shift
                ;;
            -v|--verify)
                VERIFY_ONLY=true
                shift
                ;;
            -m|--manual)
                MANUAL_ONLY=true
                shift
                ;;
            *)
                print_error "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
    done
    
    print_status "LinuxTrack X-IR - NSIS Installation Script for Arch Linux"
    print_status "=========================================================="
    
    # Check if we're on Arch Linux
    check_arch
    
    # Check if running as root
    check_root
    
    # Check if NSIS is already installed
    if check_makensis && [ "$FORCE" = false ]; then
        if [ "$VERIFY_ONLY" = true ]; then
            verify_installation
            exit $?
        else
            print_success "NSIS is already installed. Use --force to reinstall."
            exit 0
        fi
    fi
    
    # If verify only, exit
    if [ "$VERIFY_ONLY" = true ]; then
        print_error "NSIS is not installed"
        exit 1
    fi
    
    # Check for yay
    check_yay
    
    # Installation methods
    local install_success=false
    
    if [ "$MANUAL_ONLY" = false ]; then
        # Try AUR installation first
        if install_nsis_aur; then
            install_success=true
        else
            # Try alternative package managers
            if install_nsis_alternative; then
                install_success=true
            fi
        fi
    fi
    
    # If AUR methods failed, try manual installation
    if [ "$install_success" = false ]; then
        print_status "All package manager methods failed, trying manual installation..."
        if install_nsis_manual; then
            install_success=true
        fi
    fi
    
    # Verify installation
    if [ "$install_success" = true ]; then
        if verify_installation; then
            print_success "NSIS installation completed successfully!"
            print_status "You can now build LinuxTrack X-IR with Windows compatibility support"
            exit 0
        else
            print_error "NSIS installation verification failed"
            exit 1
        fi
    else
        print_error "All installation methods failed"
        print_status "Please try installing NSIS manually:"
        print_status "1. Visit: https://nsis.sourceforge.io/Download"
        print_status "2. Download and install NSIS manually"
        print_status "3. Add NSIS to your PATH"
        exit 1
    fi
}

# Run main function with all arguments
main "$@"
