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
        ubuntu|debian)
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
            print_status "  Ubuntu/Debian: sudo apt install mingw-w64 gcc-mingw-w64 g++-mingw-w64"
            print_status "  Fedora: sudo dnf install mingw64-gcc mingw64-gcc-c++"
            print_status "  Arch: sudo pacman -S mingw-w64-gcc"
            print_status "  OpenSUSE: sudo zypper install cross-mingw64-gcc cross-mingw64-gcc-c++"
            return 1
            ;;
    esac
}

# Function to install NSIS
install_nsis() {
    local distro=$(detect_distro)
    
    print_status "Installing NSIS for $distro..."
    
    case $distro in
        ubuntu|debian)
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
            print_status "  Ubuntu/Debian: sudo apt install nsis"
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
    
    local has_wine_devel=false
    local has_mingw=false
    local has_nsis=false
    
    # Check for wine-devel tools
    if command_exists winegcc && command_exists wineg++; then
        has_wine_devel=true
        print_success "Wine development tools found"
    else
        print_warning "Wine development tools not found"
    fi
    
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
    
    # Determine build method
    if [ "$has_wine_devel" = true ] && [ "$has_nsis" = true ]; then
        BUILD_METHOD="wine_devel"
        print_success "Will use wine-devel build method"
    elif [ "$has_mingw" = true ] && [ "$has_nsis" = true ]; then
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
}

# Function to build with wine-devel
build_wine_devel() {
    print_status "Building wine bridge with wine-devel..."
    
    ./configure
    make -j$(nproc)
    
    print_success "Wine bridge built successfully with wine-devel"
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
        "NPClient.dll.so"
        "Controller.exe.so"
        "Tester.exe.so"
        "FreeTrackClient.dll.so"
        "ftc.exe.so"
        "check_data.exe.so"
        "TrackIR.exe.so"
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
    print_status "LinuxTrack Wine Bridge Smart Build Script"
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
        "wine_devel")
            build_wine_devel
            ;;
        "mingw")
            build_mingw
            ;;
        "prebuilt")
            use_prebuilt
            ;;
        *)
            print_error "No build method available"
            exit 1
            ;;
    esac
    
    # Verify the build
    if verify_build; then
        print_success "Wine bridge build completed successfully!"
        print_status "You can now run 'make install' to install LinuxTrack"
    else
        print_error "Wine bridge build verification failed"
        exit 1
    fi
}

# Run main function
main "$@" 