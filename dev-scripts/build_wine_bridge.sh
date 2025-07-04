#!/bin/bash

# LinuxTrack Wine Bridge Smart Build Script
# Automatically detects and uses the best available build method

set -e

# Colors for output
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

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to detect distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "$ID"
    elif command_exists lsb_release; then
        lsb_release -si | tr '[:upper:]' '[:lower:]'
    else
        echo "unknown"
    fi
}

# Function to install MinGW dependencies
install_mingw_deps() {
    local distro=$(detect_distro)
    
    print_status "Installing MinGW dependencies for $distro..."
    
    case $distro in
        ubuntu|debian|mx)
            sudo apt update
            sudo apt install -y mingw-w64 gcc-mingw-w64 g++-mingw-w64
            ;;
        fedora|rhel|centos)
            sudo dnf install -y mingw64-gcc mingw64-gcc-c++
            ;;
        arch|manjaro)
            sudo pacman -S --noconfirm mingw-w64-gcc
            ;;
        opensuse|suse)
            sudo zypper install -y cross-mingw64-gcc cross-mingw64-gcc-c++
            ;;
        *)
            print_error "Unsupported distribution: $distro"
            print_status "Please install MinGW toolchain manually:"
            print_status "  Ubuntu/Debian/MX: sudo apt install mingw-w64 gcc-mingw-w64 g++-mingw-w64"
            print_status "  Fedora: sudo dnf install mingw64-gcc mingw64-gcc-c++"
            print_status "  Arch: sudo pacman -S mingw-w64-gcc"
            print_status "  OpenSUSE: sudo zypper install cross-mingw64-gcc cross-mingw64-gcc-c++"
            return 1
            ;;
    esac
}

# Function to install 32-bit development libraries
install_32bit_deps() {
    local distro=$(detect_distro)
    
    print_status "Installing 32-bit development libraries for $distro..."
    
    case $distro in
        ubuntu|debian|mx)
            sudo apt update
            sudo apt install -y gcc-multilib g++-multilib libc6-dev-i386
            sudo apt install -y lib32gcc-s1 lib32stdc++6
            ;;
        fedora|rhel|centos)
            sudo dnf install -y glibc-devel.i686 libstdc++-devel.i686
            ;;
        arch|manjaro)
            sudo pacman -S --noconfirm lib32-glibc lib32-gcc-libs
            ;;
        opensuse|suse)
            sudo zypper install -y glibc-devel-32bit libstdc++6-devel-32bit
            ;;
        *)
            print_error "Unsupported distribution: $distro"
            print_status "Please install 32-bit development libraries manually"
            return 1
            ;;
    esac
}

# Function to fix autotools version issues
fix_autotools() {
    print_status "Checking and fixing autotools version issues..."
    
    # Check if aclocal-1.17 exists
    if ! command_exists aclocal-1.17; then
        print_warning "aclocal-1.17 not found, checking available versions..."
        
        # Find available aclocal versions
        local aclocal_versions=($(ls /usr/bin/aclocal-* 2>/dev/null | grep -E 'aclocal-[0-9]+\.[0-9]+' | sort -V))
        
        if [ ${#aclocal_versions[@]} -gt 0 ]; then
            local latest_version=${aclocal_versions[-1]}
            print_status "Found aclocal version: $latest_version"
            
            # Create symlink if needed
            if [ ! -L "/usr/bin/aclocal-1.17" ]; then
                print_status "Creating symlink: $latest_version -> aclocal-1.17"
                sudo ln -sf "$latest_version" "/usr/bin/aclocal-1.17"
            fi
        else
            print_error "No aclocal versions found"
            print_status "Installing automake..."
            local distro=$(detect_distro)
            case $distro in
                ubuntu|debian|mx)
                    sudo apt update
                    sudo apt install -y automake
                    ;;
                fedora|rhel|centos)
                    sudo dnf install -y automake
                    ;;
                arch|manjaro)
                    sudo pacman -S --noconfirm automake
                    ;;
                opensuse|suse)
                    sudo zypper install -y automake
                    ;;
            esac
        fi
    fi
    
    # Check if automake-1.17 exists
    if ! command_exists automake-1.17; then
        print_warning "automake-1.17 not found, checking available versions..."
        
        # Find available automake versions
        local automake_versions=($(ls /usr/bin/automake-* 2>/dev/null | grep -E 'automake-[0-9]+\.[0-9]+' | sort -V))
        
        if [ ${#automake_versions[@]} -gt 0 ]; then
            local latest_version=${automake_versions[-1]}
            print_status "Found automake version: $latest_version"
            
            # Create symlink if needed
            if [ ! -L "/usr/bin/automake-1.17" ]; then
                print_status "Creating symlink: $latest_version -> automake-1.17"
                sudo ln -sf "$latest_version" "/usr/bin/automake-1.17"
            fi
        fi
    fi
    
    print_success "Autotools version issues resolved"
}

# Function to install NSIS
install_nsis() {
    local distro=$(detect_distro)
    
    print_status "Installing NSIS for $distro..."
    
    case $distro in
        ubuntu|debian|mx)
            sudo apt install -y nsis
            ;;
        fedora|rhel|centos)
            sudo dnf install -y nsis
            ;;
        arch|manjaro)
            sudo pacman -S --noconfirm nsis
            ;;
        opensuse|suse)
            sudo zypper install -y nsis
            ;;
        *)
            print_error "Unsupported distribution: $distro"
            print_status "Please install NSIS manually:"
            print_status "  Ubuntu/Debian/MX: sudo apt install nsis"
            print_status "  Fedora: sudo dnf install nsis"
            print_status "  Arch: sudo pacman -S nsis"
            print_status "  OpenSUSE: sudo zypper install nsis"
            return 1
            ;;
    esac
}

# Function to check build requirements
check_build_requirements() {
    print_status "Checking build requirements..."
    
    # Fix autotools version issues first
    fix_autotools
    
    local has_mingw=false
    local has_nsis=false
    local has_32bit_deps=false
    
    # Check for MinGW tools
    if command_exists i686-w64-mingw32-gcc && command_exists i686-w64-mingw32-g++; then
        has_mingw=true
        print_success "MinGW cross-compilation tools found"
    else
        print_warning "MinGW cross-compilation tools not found"
    fi
    
    # Check for NSIS
    if command_exists makensis; then
        has_nsis=true
        print_success "NSIS installer found"
    else
        print_warning "NSIS installer not found"
    fi
    
    # Check for 32-bit development libraries
    if [ -f "/usr/include/i386-linux-gnu/bits/libc-header-start.h" ] || [ -f "/usr/include/bits/libc-header-start.h" ]; then
        has_32bit_deps=true
        print_success "32-bit development libraries found"
    else
        print_warning "32-bit development libraries not found"
    fi
    
    # Determine build method
    if [ "$has_mingw" = true ] && [ "$has_nsis" = true ]; then
        BUILD_METHOD="mingw"
        print_success "Will use MinGW cross-compilation method"
    else
        BUILD_METHOD="none"
        print_error "No suitable build method available"
        
        # Offer to install dependencies
        if [ "$has_mingw" = false ]; then
            read -p "Install MinGW cross-compilation tools? (y/n): " -n 1 -r
            echo
            if [[ $REPLY =~ ^[Yy]$ ]]; then
                install_mingw_deps
                has_mingw=true
            fi
        fi
        
        if [ "$has_nsis" = false ]; then
            read -p "Install NSIS installer? (y/n): " -n 1 -r
            echo
            if [[ $REPLY =~ ^[Yy]$ ]]; then
                install_nsis
                has_nsis=true
            fi
        fi
        
        # Re-check after installation
        if [ "$has_mingw" = true ] && [ "$has_nsis" = true ]; then
            BUILD_METHOD="mingw"
            print_success "Will use MinGW cross-compilation method"
        else
            print_error "Still no suitable build method available"
            return 1
        fi
    fi
    
    # Offer to install 32-bit dependencies if missing
    if [ "$has_32bit_deps" = false ]; then
        print_warning "32-bit development libraries are required for building 32-bit components"
        read -p "Install 32-bit development libraries? (y/n): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            install_32bit_deps
        else
            print_warning "Build may fail without 32-bit development libraries"
        fi
    fi
}

# Function to build with MinGW
build_mingw() {
    print_status "Building wine bridge with MinGW cross-compilation..."
    
    # Configure with MinGW support
    ./configure
    
    # Build wine bridge components
    make -j$(nproc)
    
    print_success "Wine bridge built successfully with MinGW"
}

# Function to use pre-built binaries
use_prebuilt() {
    print_status "Using pre-built wine bridge binaries..."
    
    if [ -d "prebuilt/wine_bridge" ]; then
        cp -r prebuilt/wine_bridge/* src/wine_bridge/
        print_success "Pre-built binaries copied successfully"
    else
        print_error "Pre-built binaries not found"
        return 1
    fi
}

# Function to verify build
verify_build() {
    print_status "Verifying wine bridge build..."
    
    local wine_dir="src/wine_bridge"
    local required_files=(
        "NPClient.dll"
        "Controller.exe"
        "Tester.exe"
        "FreeTrackClient.dll"
        "ftc.exe"
        "check_data.exe"
        "TrackIR.exe"
    )
    
    local missing_files=()
    
    for file in "${required_files[@]}"; do
        if [ ! -f "$wine_dir/$file" ]; then
            missing_files+=("$file")
        fi
    done
    
    if [ ${#missing_files[@]} -eq 0 ]; then
        print_success "All wine bridge components built successfully"
        return 0
    else
        print_error "Missing wine bridge components:"
        for file in "${missing_files[@]}"; do
            print_error "  - $file"
        done
        return 1
    fi
}

# Main function
main() {
    print_status "LinuxTrack Wine Bridge Smart Build Script (MinGW-only)"
    print_status "=========================================="
    
    # Check if we're in the right directory
    if [ ! -f "configure.ac" ] || [ ! -d "src/wine_bridge" ]; then
        print_error "Please run this script from the LinuxTrack source directory"
        exit 1
    fi
    
    # Check build requirements
    check_build_requirements
    
    # Build based on available method
    case $BUILD_METHOD in
        "mingw")
            build_mingw
            ;;
        "prebuilt")
            use_prebuilt
            ;;
        *)
            print_error "No build method available"
            ;;
    esac
}

# Run main function
main "$@" 