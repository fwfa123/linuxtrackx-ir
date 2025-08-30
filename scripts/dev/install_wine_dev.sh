#!/bin/bash

# Wine Development Package Installer
# This script helps install 32-bit Wine development packages for different Linux distributions
# to enable winegcc functionality for building Wine applications

set -e  # Exit on any error

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

# Function to detect distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        DISTRO=$ID
        VERSION=$VERSION_ID
    elif [ -f /etc/debian_version ]; then
        DISTRO="debian"
        VERSION=$(cat /etc/debian_version)
    elif [ -f /etc/redhat-release ]; then
        DISTRO="fedora"
        VERSION=$(cat /etc/redhat-release)
    elif [ -f /etc/arch-release ]; then
        DISTRO="arch"
        VERSION="rolling"
    else
        print_error "Could not detect distribution"
        exit 1
    fi
    
    print_status "Detected distribution: $DISTRO $VERSION"
}

# Function to check if running as root
check_root() {
    if [ "$EUID" -ne 0 ]; then
        print_error "This script must be run as root (use sudo)"
        exit 1
    fi
}

# Function to check if winegcc is already available
check_winegcc() {
    if command -v winegcc >/dev/null 2>&1; then
        print_success "winegcc is already available"
        winegcc --version
        return 0
    elif command -v winegcc-stable >/dev/null 2>&1; then
        print_success "winegcc-stable is available"
        winegcc-stable --version
        return 0
    else
        print_warning "winegcc not found, proceeding with installation"
        return 1
    fi
}

# Function to install on Debian/Ubuntu
install_debian_ubuntu() {
    print_status "Installing Wine development packages for Debian/Ubuntu"
    
    # Note: wine-development packages are only available in Debian Sid
    # For Debian Bullseye and newer, we use the stable wine packages
    print_warning "Note: wine-development packages are only available in Debian Sid"
    print_status "Using stable wine packages with development libraries"
    
    # Update package lists
    apt update
    
    # Install 32-bit architecture support
    dpkg --add-architecture i386
    
    # Update package lists again after adding architecture
    apt update
    
    # Install Wine development packages
    apt install -y \
        wine \
        wine32 \
        wine64 \
        libwine \
        libwine:i386 \
        libwine:amd64 \
        libwine-dev \
        libwine-dev:i386 \
        libwine-dev:amd64 \
        wine-binfmt \
        fonts-wine \
        wine64-tools
    
    # Install additional development tools
    apt install -y \
        build-essential \
        gcc-multilib \
        g++-multilib \
        libc6-dev:i386 \
        libc6-dev:amd64
    
    print_success "Wine development packages installed for Debian/Ubuntu"

    # Symlink workaround for Debian/Ubuntu: winegcc-stable -> winegcc, wineg++-stable -> wineg++, wrc-stable -> wrc
    if command -v winegcc-stable >/dev/null 2>&1 && ! command -v winegcc >/dev/null 2>&1; then
        print_status "Creating symlink: /usr/bin/winegcc -> /usr/bin/winegcc-stable (Debian workaround)"
        ln -sf /usr/bin/winegcc-stable /usr/bin/winegcc
    fi
    if command -v wineg++-stable >/dev/null 2>&1 && ! command -v wineg++ >/dev/null 2>&1; then
        print_status "Creating symlink: /usr/bin/wineg++ -> /usr/bin/wineg++-stable (Debian workaround)"
        ln -sf /usr/bin/wineg++-stable /usr/bin/wineg++
    fi
    if command -v wrc-stable >/dev/null 2>&1 && ! command -v wrc >/dev/null 2>&1; then
        print_status "Creating symlink: /usr/bin/wrc -> /usr/bin/wrc-stable (Debian workaround)"
        ln -sf /usr/bin/wrc-stable /usr/bin/wrc
    fi
    
    # Offer WineHQ repository option
    print_status "For the latest Wine development version, consider using WineHQ repository:"
    print_status "See: https://wiki.winehq.org/Debian"
}

# Function to install on Fedora
install_fedora() {
    print_status "Installing Wine development packages for Fedora"

    # Update package lists
    dnf update -y

    # Install Wine development packages
    dnf install -y \
        wine \
        wine-devel \
        wine-devel.i686 \
        wine-devel.x86_64

    # Install additional development tools
    dnf install -y \
        gcc \
        gcc-c++ \
        glibc-devel \
        glibc-devel.i686 \
        glibc-devel.x86_64

    # Ensure 32-bit Wine WoW64 support
    print_status "Ensuring 32-bit Wine support is properly configured"
    if ! dnf list installed wine.i686 >/dev/null 2>&1; then
        print_status "Installing 32-bit Wine runtime for WoW64 support"
        dnf install -y wine.i686 2>/dev/null || print_warning "32-bit Wine runtime not available in repositories"
    fi

    print_success "Wine development packages installed for Fedora"
}

# Function to install on Arch Linux
install_arch() {
    print_status "Installing Wine development packages for Arch Linux"
    
    # Update package lists
    pacman -Sy
    
    # Install Wine development packages
    pacman -S --noconfirm \
        wine \
        wine-mono \
        wine-gecko \
        lib32-wine \
        wine-staging \
        wine-staging-mono \
        wine-staging-gecko \
        lib32-wine-staging
    
    # Install additional development tools
    pacman -S --noconfirm \
        base-devel \
        gcc \
        gcc-multilib \
        lib32-glibc \
        glibc
    
    print_success "Wine development packages installed for Arch Linux"
}

# Function to verify installation
verify_installation() {
    print_status "Verifying Wine development installation..."
    
    # Check if winegcc is available
    if command -v winegcc >/dev/null 2>&1; then
        print_success "winegcc is now available"
        winegcc --version
    elif command -v winegcc-stable >/dev/null 2>&1; then
        print_success "winegcc-stable is now available"
        winegcc-stable --version
        print_status "Note: Use 'winegcc-stable' instead of 'winegcc'"
    else
        print_error "winegcc is still not available after installation"
        print_warning "You may need to restart your terminal or system"
        return 1
    fi
    
    # Check if wine is available
    if command -v wine >/dev/null 2>&1; then
        print_success "wine is available"
        wine --version
    else
        print_error "wine is not available"
        return 1
    fi
    
    # Check for 32-bit libraries
    if [ -d "/usr/lib/i386-linux-gnu" ] || [ -d "/usr/lib32" ] || [ -d "/lib32" ]; then
        print_success "32-bit libraries are available"
    elif command -v winegcc >/dev/null 2>&1; then
        # For Fedora/RHEL systems, check if winegcc is available and WoW64 is supported
        if wine --version 2>/dev/null | grep -q "Staging\|Development" || [ -f "/usr/lib/wine/wine" ]; then
            print_success "32-bit Wine support detected (WoW64)"
        else
            print_warning "32-bit libraries may not be properly installed"
        fi
    else
        print_warning "32-bit libraries may not be properly installed"
    fi
    
    return 0
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -h, --help     Show this help message"
    echo "  -f, --force    Force installation even if winegcc is already available"
    echo "  -v, --verify   Only verify installation without installing"
    echo ""
    echo "This script installs 32-bit Wine development packages for building Wine applications."
    echo "Supported distributions: Debian, Ubuntu, Fedora, Nobara, Arch Linux"
    echo ""
    echo "Examples:"
    echo "  sudo $0              # Install Wine development packages"
    echo "  sudo $0 --force      # Force installation"
    echo "  $0 --verify          # Verify installation only"
}

# Main function
main() {
    local FORCE=false
    local VERIFY_ONLY=false
    
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
            *)
                print_error "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
    done
    
    print_status "Wine Development Package Installer"
    print_status "=================================="
    
    # Detect distribution
    detect_distro
    
    # Check if winegcc is already available
    if check_winegcc && [ "$FORCE" = false ]; then
        print_success "winegcc is already available. Use --force to reinstall."
        exit 0
    fi
    
    # If verify only, skip installation
    if [ "$VERIFY_ONLY" = true ]; then
        verify_installation
        exit $?
    fi
    
    # Check if running as root
    check_root
    
    # Install based on distribution
    case $DISTRO in
        debian|ubuntu)
            install_debian_ubuntu
            ;;
        fedora|rhel|centos|nobara)
            install_fedora
            ;;
        arch|manjaro)
            install_arch
            ;;
        *)
            print_error "Unsupported distribution: $DISTRO"
            print_warning "This script supports Debian, Ubuntu, Fedora, Nobara, and Arch Linux"
            exit 1
            ;;
    esac
    
    # Verify installation
    if verify_installation; then
        print_success "Wine development packages installation completed successfully!"
        print_status "You can now use winegcc to build Wine applications"
    else
        print_error "Installation verification failed"
        print_warning "You may need to restart your terminal or system"
        exit 1
    fi
}

# Run main function with all arguments
main "$@" 