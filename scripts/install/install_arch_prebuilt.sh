#!/bin/bash

# LinuxTrack X-IR Arch Linux Prebuilt Installation Script
# Purpose: Install LinuxTrack with prebuilt Wine bridge components (no Wine development tools required)
# Date: January 2025
# Status: Production Ready

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
PACKAGE_NAME="linuxtrack-wine-bridge-1.0.0.tar.gz"
PACKAGE_URL="https://github.com/fwfa123/linuxtrackx-ir/releases/download/v1.0.0/$PACKAGE_NAME"

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

# Function to detect distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "$ID"
    else
        echo "unknown"
    fi
}

# Function to install dependencies
install_dependencies() {
    local distro=$(detect_distro)
    print_status "Installing dependencies for $distro..."
    
    case "$distro" in
        "arch"|"manjaro")
            # Install core dependencies (no Wine development tools)
            sudo pacman -S --needed base-devel autoconf automake libtool \
                qt5-base qt5-tools qt5-x11extras opencv libusb mxml \
                libx11 libxrandr bison flex lib32-glibc lib32-gcc-libs \
                v4l-utils wine-staging winetricks mfc42 nsis || {
                print_error "Failed to install dependencies"
                return 1
            }
            ;;
        *)
            print_error "Unsupported distribution: $distro"
            print_error "This script is designed for Arch Linux and Manjaro"
            return 1
            ;;
    esac
    
    print_success "Dependencies installed successfully"
    return 0
}

# Function to download Wine bridge package
download_wine_bridge_package() {
    print_status "Downloading Wine bridge package..."
    
    if [ -f "$PACKAGE_NAME" ]; then
        print_warning "Package already exists, skipping download"
        return 0
    fi
    
    if command_exists wget; then
        wget "$PACKAGE_URL" || {
            print_error "Failed to download package with wget"
            return 1
        }
    elif command_exists curl; then
        curl -L -o "$PACKAGE_NAME" "$PACKAGE_URL" || {
            print_error "Failed to download package with curl"
            return 1
        }
    else
        print_error "Neither wget nor curl found. Please install one and try again."
        return 1
    fi
    
    print_success "Wine bridge package downloaded successfully"
    return 0
}

# Function to extract and setup Wine bridge package
setup_wine_bridge_package() {
    print_status "Setting up Wine bridge package..."
    
    if [ ! -f "$PACKAGE_NAME" ]; then
        print_error "Wine bridge package not found: $PACKAGE_NAME"
        return 1
    fi
    
    # Extract package
    tar -xzf "$PACKAGE_NAME" || {
        print_error "Failed to extract package"
        return 1
    }
    
    # Move to project root
    if [ -d "wine_bridge_package" ]; then
        mv wine_bridge_package "$PROJECT_ROOT/prebuilt_wine_bridge" || {
            print_error "Failed to move package to project root"
            return 1
        }
    fi
    
    print_success "Wine bridge package setup complete"
    return 0
}

# Function to modify build system for prebuilt components
modify_build_system() {
    print_status "Modifying build system for prebuilt components..."
    
    cd "$PROJECT_ROOT"
    
    # Run the build system modification script
    if [ -f "scripts/install/modify_build_for_prebuilt.sh" ]; then
        chmod +x scripts/install/modify_build_for_prebuilt.sh
        ./scripts/install/modify_build_for_prebuilt.sh || {
            print_error "Failed to modify build system"
            return 1
        }
    else
        print_error "Build system modification script not found"
        return 1
    fi
    
    print_success "Build system modified successfully"
    return 0
}

# Function to build LinuxTrack
build_linuxtrack() {
    print_status "Building LinuxTrack with prebuilt Wine bridge support..."
    
    cd "$PROJECT_ROOT"
    
    # Clean previous builds
    make distclean 2>/dev/null || true
    
    # Regenerate build system
    autoreconf -fiv || {
        print_error "Failed to regenerate build system"
        return 1
    }
    
    # Configure with prebuilt support
    ./configure --prefix=/opt || {
        print_error "Failed to configure build"
        return 1
    }
    
    # Build
    make -j$(nproc) || {
        print_error "Failed to build LinuxTrack"
        return 1
    }
    
    print_success "LinuxTrack built successfully"
    return 0
}

# Function to install LinuxTrack
install_linuxtrack() {
    print_status "Installing LinuxTrack..."
    
    cd "$PROJECT_ROOT"
    
    sudo make install || {
        print_error "Failed to install LinuxTrack"
        return 1
    }
    
    # Add user to required groups
    sudo usermod -a -G plugdev "$USER" 2>/dev/null || true
    
    print_success "LinuxTrack installed successfully"
    return 0
}

# Function to install Wine bridge components
install_wine_bridge_components() {
    print_status "Installing Wine bridge components..."
    
    if [ -d "$PROJECT_ROOT/prebuilt_wine_bridge" ]; then
        cd "$PROJECT_ROOT/prebuilt_wine_bridge"
        
        if [ -f "install_wine_bridge.sh" ]; then
            chmod +x install_wine_bridge.sh
            ./install_wine_bridge.sh || {
                print_error "Failed to install Wine bridge components"
                return 1
            }
        else
            print_error "Wine bridge installation script not found"
            return 1
        fi
    else
        print_error "Prebuilt Wine bridge directory not found"
        return 1
    fi
    
    print_success "Wine bridge components installed successfully"
    return 0
}

# Function to verify installation
verify_installation() {
    print_status "Verifying installation..."
    
    # Check if ltr_gui is available
    if command_exists ltr_gui; then
        print_success "ltr_gui command found"
    else
        print_warning "ltr_gui command not found in PATH"
    fi
    
    # Check Wine bridge components
    if [ -d "$PROJECT_ROOT/prebuilt_wine_bridge" ]; then
        print_success "Prebuilt Wine bridge directory exists"
    else
        print_warning "Prebuilt Wine bridge directory not found"
    fi
    
    # Check Wine installation
    if command_exists wine; then
        print_success "Wine is installed"
    else
        print_warning "Wine is not installed"
    fi
    
    print_success "Installation verification complete"
    return 0
}

# Function to display post-installation instructions
display_post_install_instructions() {
    echo
    echo "=========================================="
    echo "🎉 LinuxTrack X-IR Installation Complete!"
    echo "=========================================="
    echo
    echo "Next Steps:"
    echo "1. Log out and back in for group changes to take effect"
    echo "2. Launch LinuxTrack GUI: ltr_gui"
    echo "3. Configure your tracking device"
    echo "4. Test with your favorite games"
    echo
    echo "Documentation:"
    echo "- Main README: $PROJECT_ROOT/README.md"
    echo "- Wine Bridge Guide: $PROJECT_ROOT/docs/technical/WINE_BRIDGE_PACKAGING_PLAN.md"
    echo "- Troubleshooting: $PROJECT_ROOT/docs/troubleshooting/"
    echo
    echo "Support:"
    echo "- GitHub Issues: https://github.com/fwfa123/linuxtrackx-ir/issues"
    echo "- Health Check: $PROJECT_ROOT/scripts/install/linuxtrack_health_check.sh"
    echo
}

# Main installation function
main() {
    echo "=========================================="
    echo "🍷 LinuxTrack X-IR Arch Linux Installer"
    echo "=========================================="
    echo "This installer uses prebuilt Wine bridge components"
    echo "to avoid Wine development tool requirements."
    echo
    
    # Check if running as root
    if [ "$EUID" -eq 0 ]; then
        print_error "Please do not run this script as root"
        exit 1
    fi
    
    # Check if we're in the right directory
    if [ ! -f "$PROJECT_ROOT/configure.ac" ]; then
        print_error "Please run this script from the LinuxTrack X-IR project root"
        exit 1
    fi
    
    # Installation steps
    install_dependencies || exit 1
    download_wine_bridge_package || exit 1
    setup_wine_bridge_package || exit 1
    modify_build_system || exit 1
    build_linuxtrack || exit 1
    install_linuxtrack || exit 1
    install_wine_bridge_components || exit 1
    verify_installation || exit 1
    
    display_post_install_instructions
    
    print_success "Installation completed successfully!"
}

# Run main function
main "$@" 