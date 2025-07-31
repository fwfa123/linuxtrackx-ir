#!/bin/bash

# LinuxTrack X-IR Wine Bridge Arch Linux Fix Script
# This script fixes wine bridge configuration for Arch Linux with wine32

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

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

# Function to check if wine32 is installed
check_wine32() {
    print_status "Checking wine32 installation..."
    
    if pacman -Q wine32 >/dev/null 2>&1; then
        print_success "wine32 is installed"
        return 0
    else
        print_error "wine32 is not installed"
        print_status "Please install wine32 from AUR: yay -S wine32"
        return 1
    fi
}

# Function to check wine32 libraries
check_wine32_libs() {
    print_status "Checking wine32 libraries..."
    
    if [ -d "/usr/lib32/wine/i386-unix" ]; then
        print_success "Found wine32 libraries in /usr/lib32/wine/i386-unix"
        return 0
    elif [ -d "/usr/lib32/wine" ]; then
        print_success "Found wine32 libraries in /usr/lib32/wine"
        return 0
    else
        print_error "wine32 libraries not found"
        return 1
    fi
}

# Function to check winegcc
check_winegcc() {
    print_status "Checking winegcc..."
    
    if command -v winegcc >/dev/null 2>&1; then
        print_success "winegcc found"
        winegcc --version
        return 0
    else
        print_error "winegcc not found"
        return 1
    fi
}

# Function to test winegcc compilation
test_winegcc() {
    print_status "Testing winegcc compilation..."
    
    # Create a simple test file
    cat > /tmp/test_wine.c << 'EOF'
#include <stdio.h>
int main() {
    printf("Hello from winegcc test\n");
    return 0;
}
EOF
    
    # Try to compile with winegcc
    if winegcc -m32 -o /tmp/test_wine.exe /tmp/test_wine.c 2>/dev/null; then
        print_success "winegcc compilation test passed"
        rm -f /tmp/test_wine.exe /tmp/test_wine.c
        return 0
    else
        print_error "winegcc compilation test failed"
        rm -f /tmp/test_wine.c
        return 1
    fi
}

# Function to fix wine bridge configuration
fix_wine_bridge() {
    print_status "Fixing wine bridge configuration..."
    
    # Set environment variables for Arch Linux
    export CFLAGS="-m32 -O2"
    export CXXFLAGS="-m32 -O2"
    export LDFLAGS="-m32"
    
    # Configure with Arch Linux specific wine paths
    if [ -d "/usr/lib32/wine/i386-unix" ]; then
        WINE_LIBS="-L/usr/lib32/wine/i386-unix"
        print_status "Using wine32 path: /usr/lib32/wine/i386-unix"
    elif [ -d "/usr/lib32/wine" ]; then
        WINE_LIBS="-L/usr/lib32/wine"
        print_status "Using wine32 path: /usr/lib32/wine"
    else
        print_error "No wine32 libraries found"
        return 1
    fi
    
    # Reconfigure the build
    autoreconf -fiv
    
    ./configure --prefix=/opt \
                --enable-ltr-32lib-on-x64 \
                --with-wine-libs="$WINE_LIBS" \
                --with-wine64-libs="-L/usr/lib/wine/x86_64-unix"
    
    print_success "Wine bridge configuration fixed"
}

# Function to rebuild wine bridge components
rebuild_wine_bridge() {
    print_status "Rebuilding wine bridge components..."
    
    # Build wine bridge components
    make -C src/wine_bridge clean
    make -C src/wine_bridge
    
    print_success "Wine bridge components rebuilt"
}

# Function to test wine bridge installation
test_wine_bridge() {
    print_status "Testing wine bridge installation..."
    
    # Check if wine bridge components exist
    if [ -f "src/wine_bridge/linuxtrack-wine.exe" ]; then
        print_success "Wine bridge installer found"
    else
        print_error "Wine bridge installer not found"
        return 1
    fi
    
    # Check wine bridge components
    local components=(
        "src/wine_bridge/client/NPClient.dll.so"
        "src/wine_bridge/client/check_data.exe.so"
        "src/wine_bridge/controller/Controller.exe.so"
        "src/wine_bridge/ft_client/FreeTrackClient.dll.so"
        "src/wine_bridge/ft_tester/ftc.exe.so"
        "src/wine_bridge/views/TrackIR.exe.so"
    )
    
    for component in "${components[@]}"; do
        if [ -f "$component" ]; then
            print_success "Found: $component"
        else
            print_warning "Missing: $component"
        fi
    done
}

# Function to install wine bridge in wine prefix
install_wine_bridge() {
    print_status "Installing wine bridge in wine prefix..."
    
    local wine_prefix="${1:-~/.wine}"
    
    if [ -f "src/wine_bridge/linuxtrack-wine.exe" ]; then
        print_status "Installing wine bridge in $wine_prefix"
        WINEPREFIX="$wine_prefix" wine src/wine_bridge/linuxtrack-wine.exe
        print_success "Wine bridge installed in $wine_prefix"
    else
        print_error "Wine bridge installer not found"
        return 1
    fi
}

# Function to show usage
show_usage() {
    echo "LinuxTrack X-IR Wine Bridge Arch Linux Fix Script"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --check-only      Check wine32 installation only"
    echo "  --fix-config      Fix wine bridge configuration only"
    echo "  --rebuild         Rebuild wine bridge components"
    echo "  --test            Test wine bridge installation"
    echo "  --install PREFIX  Install wine bridge in wine prefix"
    echo "  --help            Show this help message"
    echo ""
    echo "Default behavior: Full fix (check + fix-config + rebuild + test)"
}

# Main function
main() {
    print_status "Starting wine bridge Arch Linux fix process..."
    
    # Parse command line arguments
    local check_only=false
    local fix_config_only=false
    local rebuild_only=false
    local test_only=false
    local install_prefix=""
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            --check-only)
                check_only=true
                shift
                ;;
            --fix-config)
                fix_config_only=true
                shift
                ;;
            --rebuild)
                rebuild_only=true
                shift
                ;;
            --test)
                test_only=true
                shift
                ;;
            --install)
                install_prefix="$2"
                shift 2
                ;;
            --help)
                show_usage
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
    done
    
    # Check if we're in the right directory
    if [ ! -f "configure.ac" ]; then
        print_error "This script must be run from the LinuxTrack X-IR source directory"
        exit 1
    fi
    
    # Check wine32 installation
    if [ "$check_only" = true ]; then
        check_wine32
        check_wine32_libs
        check_winegcc
        test_winegcc
        exit 0
    elif [ "$test_only" = true ]; then
        test_wine_bridge
        exit 0
    elif [ "$rebuild_only" = true ]; then
        rebuild_wine_bridge
        exit 0
    elif [ "$fix_config_only" = true ]; then
        fix_wine_bridge
        exit 0
    elif [ -n "$install_prefix" ]; then
        install_wine_bridge "$install_prefix"
        exit 0
    else
        # Full fix process
        check_wine32
        check_wine32_libs
        check_winegcc
        test_winegcc
        fix_wine_bridge
        rebuild_wine_bridge
        test_wine_bridge
    fi
    
    print_success "Wine bridge Arch Linux fix completed successfully!"
}

# Run main function
main "$@" 