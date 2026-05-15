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

# Wine bridge needs makensis plus installer stubs (e.g. zlib-x86-unicode).
NSIS_STUB_NAME="zlib-x86-unicode"

# makensis prints version to stderr on some builds; /VERSION and -VERSION both exist.
get_makensis_version() {
    local version
    version=$(makensis /VERSION 2>&1 | head -n1)
    if [[ -z "$version" ]]; then
        version=$(makensis -VERSION 2>&1 | head -n1)
    fi
    if [[ -z "$version" ]]; then
        echo "unknown"
    else
        echo "$version"
    fi
}

find_nsis_stub() {
    local dir
    for dir in /usr/share/nsis/Stubs /usr/local/share/nsis/Stubs; do
        if [[ -f "$dir/$NSIS_STUB_NAME" ]]; then
            echo "$dir/$NSIS_STUB_NAME"
            return 0
        fi
    done
    return 1
}

# Full NSIS install suitable for linuxtrack-wine.exe (not compiler-only).
check_nsis_wine_ready() {
    command -v makensis &>/dev/null || return 1
    find_nsis_stub &>/dev/null
}

# Remove compiler-only manual installs that break stub lookup (/usr/Stubs/...).
remove_minimal_nsis_local() {
    local removed=false
    if [[ -e /usr/local/bin/makensis ]]; then
        sudo rm -f /usr/local/bin/makensis
        removed=true
    fi
    if [[ -e /usr/local/makensis ]]; then
        sudo rm -f /usr/local/makensis
        removed=true
    fi
    if [[ -d /usr/local/Stubs ]]; then
        sudo rm -rf /usr/local/Stubs
        removed=true
    fi
    if [[ "$removed" = true ]]; then
        print_status "Removed incomplete /usr/local NSIS (compiler-only) install"
    fi
}

report_nsis_status() {
    local version
    version=$(get_makensis_version)
    print_success "NSIS ready for Wine bridge (makensis: $(command -v makensis), version: $version)"
    print_success "NSIS stub: $(find_nsis_stub)"
}

# Function to install NSIS via AUR
install_nsis_aur() {
    print_status "Attempting to install NSIS via AUR..."
    
    # Try to install via yay
    if yay -S --needed nsis mingw-w64-gcc --noconfirm; then
        print_success "NSIS installed successfully via AUR"
        return 0
    else
        print_warning "AUR installation failed, trying alternative methods..."
        return 1
    fi
}

# Install pacman build deps for manual NSIS compile (stubs require MinGW on Linux).
# CachyOS and some Arch derivatives ship zlib-ng-compat instead of zlib; the two packages conflict.
install_nsis_build_deps() {
    local deps=(scons pcre2 bzip2 mingw-w64-gcc)

    if pacman -Q zlib-ng-compat &>/dev/null; then
        print_status "zlib-ng-compat present; skipping zlib package (provides libz)"
        if ! pkg-config --exists zlib 2>/dev/null; then
            print_error "zlib-ng-compat is installed but pkg-config cannot find zlib"
            print_status "Try: sudo pacman -S zlib-ng-compat"
            return 1
        fi
    elif ! pacman -Q zlib &>/dev/null; then
        deps+=(zlib)
    fi

    print_status "Installing build dependencies: ${deps[*]}"
    if ! sudo pacman -S --needed --noconfirm "${deps[@]}"; then
        print_error "Failed to install NSIS build dependencies"
        return 1
    fi

    if ! command -v scons &>/dev/null; then
        print_error "scons not found after installing dependencies"
        return 1
    fi

    return 0
}

# Function to install NSIS manually (full install: makensis + Stubs under PREFIX/share/nsis)
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
    if ! install_nsis_build_deps; then
        cd -
        rm -rf "$temp_dir"
        return 1
    fi

    # Full install (includes Stubs). Do not use SKIPSTUBS — Wine bridge needs them.
    local -a nsis_scons_opts=(
        SKIPPLUGINS=all SKIPUTILS=all SKIPMISC=all
        PREFIX=/usr/local
    )

    print_status "Building NSIS with stubs (this may take several minutes)..."
    if ! scons "${nsis_scons_opts[@]}"; then
        print_error "Failed to build NSIS"
        cd -
        rm -rf "$temp_dir"
        return 1
    fi

    print_status "Installing NSIS to /usr/local (requires sudo)..."
    if ! sudo scons "${nsis_scons_opts[@]}" install; then
        print_error "Failed to install NSIS to /usr/local"
        cd -
        rm -rf "$temp_dir"
        return 1
    fi

    if ! check_nsis_wine_ready; then
        print_error "NSIS installed but stubs are missing at /usr/local/share/nsis/Stubs"
        print_status "Prefer package install: yay -S nsis"
        cd -
        rm -rf "$temp_dir"
        return 1
    fi

    print_success "NSIS built and installed successfully (with Stubs)"
    cd -
    rm -rf "$temp_dir"
    return 0
}

# Official repos or AUR — both ship makensis and Stubs (AUR may pull mingw-w64-gcc).
install_nsis_pacman() {
    print_status "Trying pacman (nsis + mingw-w64-gcc)..."
    if sudo pacman -S --needed --noconfirm nsis mingw-w64-gcc 2>/dev/null; then
        print_success "NSIS installed via pacman"
        return 0
    fi
    return 1
}

install_nsis_paru() {
    if ! command -v paru &> /dev/null; then
        return 1
    fi
    print_status "Trying paru (AUR nsis)..."
    if paru -S --needed nsis mingw-w64-gcc --noconfirm; then
        print_success "NSIS installed via paru"
        return 0
    fi
    return 1
}

# Function to verify NSIS installation (Wine bridge requires stubs + working makensis).
verify_installation() {
    if ! check_nsis_wine_ready; then
        print_error "NSIS is not ready for Wine bridge builds"
        if command -v makensis &>/dev/null; then
            print_error "makensis is on PATH but installer stubs are missing"
            print_status "Install full NSIS: yay -S nsis   (or re-run this script with --force)"
        fi
        return 1
    fi

    report_nsis_status

    print_status "Testing NSIS compile (requires stubs)..."
    local test_dir
    test_dir=$(mktemp -d)
    cat > "$test_dir/test.nsi" << 'EOF'
Name "Test Installer"
OutFile "test-installer.exe"
InstallDir "$PROGRAMFILES\TestApp"

Section "Main Application"
    SetOutPath $INSTDIR
    File /oname=test.txt "test.txt"
SectionEnd
EOF
    echo "Test file" > "$test_dir/test.txt"

    if (cd "$test_dir" && makensis test.nsi >/dev/null 2>&1); then
        print_success "NSIS compile test passed (Wine bridge installer can be built)"
        rm -rf "$test_dir"
        return 0
    fi

    print_error "NSIS compile test failed — linuxtrack-wine.exe will not build"
    print_status "Ensure /usr/bin/makensis is used: which -a makensis"
    rm -rf "$test_dir"
    return 1
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
    echo "Installs full NSIS (makensis + Stubs) required to build linuxtrack-wine.exe."
    echo ""
    echo "The script will try multiple installation methods:"
    echo "1. pacman (nsis, if available in repos)"
    echo "2. AUR nsis via yay"
    echo "3. AUR nsis via paru"
    echo "4. Manual source build (makensis + Stubs under /usr/local/share/nsis)"
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
    
  # Incomplete compiler-only installs block AUR makensis on PATH
    if command -v makensis &>/dev/null && ! find_nsis_stub &>/dev/null; then
        print_warning "makensis found but NSIS stubs are missing (Wine bridge installer will fail)"
        remove_minimal_nsis_local
    fi

    if check_nsis_wine_ready && [ "$FORCE" = false ]; then
        if [ "$VERIFY_ONLY" = true ]; then
            verify_installation
            exit $?
        else
            report_nsis_status
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
        if install_nsis_pacman; then
            install_success=true
        elif install_nsis_aur; then
            install_success=true
        elif install_nsis_paru; then
            install_success=true
        fi
    fi

    if [ "$install_success" = false ]; then
        print_status "Package installs failed, trying manual source build (with Stubs)..."
        remove_minimal_nsis_local
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
        print_status "Install full NSIS (must include Stubs for Wine bridge):"
        print_status "  yay -S nsis mingw-w64-gcc"
        print_status "  test -f /usr/share/nsis/Stubs/zlib-x86-unicode && which makensis"
        exit 1
    fi
}

# Run main function with all arguments
main "$@"
