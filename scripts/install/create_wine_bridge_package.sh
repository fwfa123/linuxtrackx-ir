#!/bin/bash

# LinuxTrack X-IR Wine Bridge Package Creator
# This script builds and packages the Wine bridge executables for distribution
# to avoid requiring Wine development libraries on problematic distributions

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
PACKAGE_DIR="$PROJECT_ROOT/wine_bridge_package"
BUILD_DIR="$PROJECT_ROOT/build_wine_bridge"
VERSION="1.0.0"

echo -e "${BLUE}🍷 LinuxTrack X-IR Wine Bridge Package Creator${NC}"
echo -e "${BLUE}================================================${NC}"
echo ""

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
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
    if command_exists pacman; then
        echo "arch"
    elif command_exists apt; then
        echo "debian"
    elif command_exists dnf; then
        echo "fedora"
    else
        echo "unknown"
    fi
}

# Function to install Wine development tools
install_wine_dev() {
    local distro=$(detect_distro)
    
    print_status "Installing Wine development tools for $distro..."
    
    case $distro in
        "arch")
            print_warning "Arch Linux detected - Wine development tools may be problematic"
            print_status "Attempting to install wine-staging and wine-devel..."
            sudo pacman -S --needed wine-staging wine-devel || {
                print_error "Failed to install Wine development tools on Arch Linux"
                print_error "This is expected - we'll use pre-built binaries instead"
                return 1
            }
            ;;
        "debian")
            sudo apt update
            sudo apt install -y wine-devel wine32:i386 wine64 || {
                print_error "Failed to install Wine development tools"
                return 1
            }
            ;;
        "fedora")
            sudo dnf install -y wine-devel wine-tools || {
                print_error "Failed to install Wine development tools"
                return 1
            }
            ;;
        *)
            print_error "Unsupported distribution: $distro"
            return 1
            ;;
    esac
    
    return 0
}

# Function to verify Wine development tools
verify_wine_dev() {
    print_status "Verifying Wine development tools..."
    
    if ! command_exists winegcc; then
        print_error "winegcc not found"
        return 1
    fi
    
    if ! command_exists wineg++; then
        print_error "wineg++ not found"
        return 1
    fi
    
    if ! command_exists makensis; then
        print_error "makensis (NSIS) not found"
        return 1
    fi
    
    print_status "Wine development tools verified successfully"
    return 0
}

# Function to build Wine bridge components
build_wine_bridge() {
    print_status "Building Wine bridge components..."
    
    # Create build directory
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    # Copy source files
    cp -r "$PROJECT_ROOT/src/wine_bridge"/* .
    
    # Build components
    print_status "Building 32-bit components..."
    make -j$(nproc) || {
        print_error "Failed to build 32-bit components"
        return 1
    }
    
    print_status "Building 64-bit components..."
    make -j$(nproc) || {
        print_error "Failed to build 64-bit components"
        return 1
    }
    
    print_status "Wine bridge components built successfully"
    return 0
}

# Function to create package structure
create_package_structure() {
    print_status "Creating package structure..."
    
    # Create package directory
    rm -rf "$PACKAGE_DIR"
    mkdir -p "$PACKAGE_DIR"
    
    # Create directory structure
    mkdir -p "$PACKAGE_DIR/32bit"
    mkdir -p "$PACKAGE_DIR/64bit"
    mkdir -p "$PACKAGE_DIR/installers"
    mkdir -p "$PACKAGE_DIR/docs"
    
    print_status "Package structure created"
}

# Function to copy built components
copy_components() {
    print_status "Copying built components..."
    
    # Copy 32-bit built components (including NPClient.dll)
    if [ -f "$BUILD_DIR/client/NPClient.dll.so" ]; then
        cp "$BUILD_DIR/client/NPClient.dll.so" "$PACKAGE_DIR/32bit/NPClient.dll"
        print_status "Copied NPClient.dll (32-bit)"
    fi
    
    if [ -f "$BUILD_DIR/client/check_data.exe.so" ]; then
        cp "$BUILD_DIR/client/check_data.exe.so" "$PACKAGE_DIR/32bit/check_data.exe"
        print_status "Copied check_data.exe (32-bit)"
    fi
    
    if [ -f "$BUILD_DIR/controller/Controller.exe.so" ]; then
        cp "$BUILD_DIR/controller/Controller.exe.so" "$PACKAGE_DIR/32bit/Controller.exe"
        print_status "Copied Controller.exe (32-bit)"
    fi
    
    if [ -f "$BUILD_DIR/tester/Tester.exe.so" ]; then
        cp "$BUILD_DIR/tester/Tester.exe.so" "$PACKAGE_DIR/32bit/Tester.exe"
        print_status "Copied Tester.exe (32-bit)"
    fi
    
    if [ -f "$BUILD_DIR/ft_client/FreeTrackClient.dll.so" ]; then
        cp "$BUILD_DIR/ft_client/FreeTrackClient.dll.so" "$PACKAGE_DIR/32bit/FreeTrackClient.dll"
        print_status "Copied FreeTrackClient.dll (32-bit)"
    fi
    
    if [ -f "$BUILD_DIR/ft_tester/ftc.exe.so" ]; then
        cp "$BUILD_DIR/ft_tester/ftc.exe.so" "$PACKAGE_DIR/32bit/FreeTrackTester.exe"
        print_status "Copied FreeTrackTester.exe (32-bit)"
    fi
    
    if [ -f "$BUILD_DIR/views/TrackIR.exe.so" ]; then
        cp "$BUILD_DIR/views/TrackIR.exe.so" "$PACKAGE_DIR/32bit/TrackIR.exe"
        print_status "Copied TrackIR.exe (32-bit)"
    fi
    
    # Copy 64-bit built components (including NPClient64.dll)
    if [ -f "$BUILD_DIR/client/NPClient64.dll.so" ]; then
        cp "$BUILD_DIR/client/NPClient64.dll.so" "$PACKAGE_DIR/64bit/NPClient64.dll"
        print_status "Copied NPClient64.dll (64-bit)"
    fi
    
    if [ -f "$BUILD_DIR/tester/Tester64.exe.so" ]; then
        cp "$BUILD_DIR/tester/Tester64.exe.so" "$PACKAGE_DIR/64bit/Tester64.exe"
        print_status "Copied Tester64.exe (64-bit)"
    fi
    
    # Copy installers
    if [ -f "$BUILD_DIR/linuxtrack-wine.exe" ]; then
        cp "$BUILD_DIR/linuxtrack-wine.exe" "$PACKAGE_DIR/installers/"
        print_status "Copied linuxtrack-wine.exe installer"
    fi
    
    # Copy documentation
    if [ -f "$BUILD_DIR/README.wine" ]; then
        cp "$BUILD_DIR/README.wine" "$PACKAGE_DIR/docs/"
        print_status "Copied README.wine"
    fi
    
    if [ -f "$BUILD_DIR/WINE_SETUP.md" ]; then
        cp "$BUILD_DIR/WINE_SETUP.md" "$PACKAGE_DIR/docs/"
        print_status "Copied WINE_SETUP.md"
    fi
    
    print_status "Built components copied successfully"
}



# Function to create package metadata
create_package_metadata() {
    print_status "Creating package metadata..."
    
    cat > "$PACKAGE_DIR/package_info.txt" << EOF
LinuxTrack X-IR Wine Bridge Package
===================================

Version: $VERSION
Build Date: $(date)
Build System: $(uname -s) $(uname -r)
Architecture: $(uname -m)

Components Included:
===================

Built Components (32-bit):
- NPClient.dll - TrackIR API compatibility
- check_data.exe - Installation verification
- Controller.exe - Hotkey controller
- Tester.exe - TrackIR protocol testing
- FreeTrackClient.dll - FreeTrack API compatibility
- FreeTrackTester.exe - FreeTrack protocol testing
- TrackIR.exe - TrackIR views utility

Built Components (64-bit):
- NPClient64.dll - TrackIR API compatibility
- Tester64.exe - 64-bit TrackIR protocol testing

Installers:
- linuxtrack-wine.exe - Windows installer

Documentation:
- README.wine - Wine bridge documentation
- WINE_SETUP.md - Setup guide

Installation:
============
1. Copy the appropriate components to your Wine prefix
2. Run the installer: wine installers/linuxtrack-wine.exe
3. Follow the setup guide in docs/WINE_SETUP.md

Usage:
======
These components provide TrackIR and FreeTrack compatibility
for Windows games running under Wine on Linux.

EOF
    
    print_status "Package metadata created"
}

# Function to create installation script
create_install_script() {
    print_status "Creating installation script..."
    
    cat > "$PACKAGE_DIR/install_wine_bridge.sh" << 'EOF'
#!/bin/bash

# LinuxTrack X-IR Wine Bridge Installer
# This script installs the pre-built Wine bridge components

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to detect Wine prefix
detect_wine_prefix() {
    if [ -n "$WINEPREFIX" ]; then
        echo "$WINEPREFIX"
    elif [ -d "$HOME/.wine" ]; then
        echo "$HOME/.wine"
    else
        echo "$HOME/.wine"
    fi
}

# Function to install components
install_components() {
    local wine_prefix="$1"
    local arch="$2"
    
    print_status "Installing $arch components to $wine_prefix"
    
    # Create system32 directory if it doesn't exist
    local system32_dir="$wine_prefix/drive_c/windows/system32"
    mkdir -p "$system32_dir"
    
    # Copy built components
    if [ "$arch" = "32bit" ]; then
        cp -f 32bit/*.dll "$system32_dir/" 2>/dev/null || true
        cp -f 32bit/*.exe "$system32_dir/" 2>/dev/null || true
    else
        cp -f 64bit/*.dll "$system32_dir/" 2>/dev/null || true
        cp -f 64bit/*.exe "$system32_dir/" 2>/dev/null || true
    fi
    
    print_status "$arch built components installed successfully"
}



# Main installation
main() {
    echo -e "${BLUE}🍷 LinuxTrack X-IR Wine Bridge Installer${NC}"
    echo -e "${BLUE}==========================================${NC}"
    echo ""
    
    # Detect Wine prefix
    local wine_prefix=$(detect_wine_prefix)
    print_status "Using Wine prefix: $wine_prefix"
    
    # Check if Wine is installed
    if ! command -v wine >/dev/null 2>&1; then
        print_error "Wine is not installed. Please install Wine first."
        exit 1
    fi
    
    # Install 32-bit components
    if [ -d "32bit" ]; then
        install_components "$wine_prefix" "32bit"
    fi
    
    # Install 64-bit components
    if [ -d "64bit" ]; then
        install_components "$wine_prefix" "64bit"
    fi
    

    
    # Run installer if available
    if [ -f "installers/linuxtrack-wine.exe" ]; then
        print_status "Running Windows installer..."
        WINEPREFIX="$wine_prefix" wine installers/linuxtrack-wine.exe
    fi
    
    print_status "Installation completed successfully!"
    print_status "See docs/WINE_SETUP.md for usage instructions"
}

main "$@"
EOF
    
    chmod +x "$PACKAGE_DIR/install_wine_bridge.sh"
    print_status "Installation script created"
}

# Function to create archive
create_archive() {
    print_status "Creating package archive..."
    
    cd "$PROJECT_ROOT"
    tar -czf "linuxtrack-wine-bridge-$VERSION.tar.gz" wine_bridge_package/
    
    print_status "Package archive created: linuxtrack-wine-bridge-$VERSION.tar.gz"
}

# Function to clean up
cleanup() {
    print_status "Cleaning up build directory..."
    rm -rf "$BUILD_DIR"
    print_status "Cleanup completed"
}

# Main execution
main() {
    local skip_build=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --skip-build)
                skip_build=true
                shift
                ;;
            --help)
                echo "Usage: $0 [--skip-build] [--help]"
                echo ""
                echo "Options:"
                echo "  --skip-build    Skip building components (use existing)"
                echo "  --help          Show this help message"
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                exit 1
                ;;
        esac
    done
    
    # Check if we're in the right directory
    if [ ! -f "$PROJECT_ROOT/configure.ac" ]; then
        print_error "This script must be run from the LinuxTrack X-IR project root"
        exit 1
    fi
    
    if [ "$skip_build" = false ]; then
        # Try to install Wine development tools
        if ! install_wine_dev; then
            print_warning "Wine development tools installation failed"
            print_warning "This is expected on Arch Linux - we'll use pre-built binaries"
            exit 1
        fi
        
        # Verify Wine development tools
        if ! verify_wine_dev; then
            print_error "Wine development tools verification failed"
            exit 1
        fi
        
        # Build Wine bridge components
        if ! build_wine_bridge; then
            print_error "Failed to build Wine bridge components"
            exit 1
        fi
    else
        print_status "Skipping build step (using existing components)"
    fi
    
    # Create package
    create_package_structure
    copy_components
    create_package_metadata
    create_install_script
    create_archive
    
    if [ "$skip_build" = false ]; then
        cleanup
    fi
    
    print_status "Package creation completed successfully!"
    print_status "Package location: $PROJECT_ROOT/linuxtrack-wine-bridge-$VERSION.tar.gz"
}

main "$@" 