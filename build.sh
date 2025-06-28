#!/bin/bash

# LinuxTrack Build Script
# Automated build and installation script for LinuxTrack

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
        echo $ID
    elif [ -f /etc/redhat-release ]; then
        echo "rhel"
    elif [ -f /etc/debian_version ]; then
        echo "debian"
    else
        echo "unknown"
    fi
}

# Function to install dependencies
install_dependencies() {
    local distro=$(detect_distro)
    
    print_status "Detected distribution: $distro"
    print_status "Installing dependencies..."
    
    case $distro in
        ubuntu|debian)
            sudo apt-get update
            sudo apt-get install -y \
                build-essential autotools-dev automake libtool \
                qtbase5-dev qttools5-dev-tools qt5-qmake \
                libusb-1.0-0-dev libmxml-dev \
                libv4l-dev libcwiid-dev liblo-dev \
                bison flex pkg-config
            ;;
        fedora)
            sudo dnf groupinstall -y "Development Tools"
            sudo dnf install -y \
                qt5-qtbase-devel qt5-qttools-devel \
                libusb1-devel mxml-devel \
                libv4l-devel cwiid-devel liblo-devel \
                bison flex pkgconfig
            ;;
        centos|rhel)
            sudo yum groupinstall -y "Development Tools"
            sudo yum install -y \
                qt5-qtbase-devel qt5-qttools-devel \
                libusb1-devel mxml-devel \
                libv4l-devel cwiid-devel liblo-devel \
                bison flex pkgconfig
            ;;
        arch)
            sudo pacman -S --noconfirm \
                base-devel autoconf automake libtool \
                qt5-base qt5-tools \
                libusb mxml v4l-utils liblo \
                bison flex pkgconfig
            ;;
        opensuse*)
            sudo zypper install -y \
                -t pattern devel_basis \
                libqt5-qtbase-devel libqt5-qttools-devel \
                libusb-1_0-devel mxml-devel \
                libv4l-devel liblo-devel \
                bison flex pkg-config
            ;;
        *)
            print_warning "Unknown distribution. Please install dependencies manually:"
            print_warning "  - Build tools (gcc, make, autotools)"
            print_warning "  - Qt5 development packages"
            print_warning "  - libusb-1.0, libmxml, libv4l, libcwiid, liblo"
            print_warning "  - bison, flex, pkg-config"
            read -p "Continue anyway? (y/N) " -n 1 -r
            echo
            if [[ ! $REPLY =~ ^[Yy]$ ]]; then
                exit 1
            fi
            ;;
    esac
}

# Function to clean previous build
clean_build() {
    print_status "Cleaning previous build..."
    
    if [ -f Makefile ]; then
        make distclean 2>/dev/null || true
    fi
    
    # Remove autotools generated files
    rm -f config.h config.log config.status stamp-h1 libtool
    rm -rf autom4te.cache/
    
    # Remove build artifacts
    find . -name "*.o" -delete 2>/dev/null || true
    find . -name "*.lo" -delete 2>/dev/null || true
    find . -name "*.la" -delete 2>/dev/null || true
    find . -name "Makefile" -delete 2>/dev/null || true
    
    # Clean Qt files
    find . -name "moc_*.cpp" -delete 2>/dev/null || true
    find . -name "ui_*.h" -delete 2>/dev/null || true
    find . -name "qrc_*.cpp" -delete 2>/dev/null || true
    
    print_success "Build directory cleaned"
}

# Function to check Qt5
check_qt5() {
    print_status "Checking Qt5 installation..."
    
    # Try different qmake names
    for qmake_cmd in qmake-qt5 qmake5 qmake; do
        if command -v $qmake_cmd >/dev/null 2>&1; then
            version=$($qmake_cmd --version | grep "Qt version" | grep -o "5\.[0-9]*\.[0-9]*")
            if [[ $version =~ ^5\. ]]; then
                print_success "Found Qt5 ($version) using $qmake_cmd"
                export QMAKE_CMD=$qmake_cmd
                return 0
            fi
        fi
    done
    
    print_error "Qt5 qmake not found or not Qt5 version"
    print_error "Please install Qt5 development packages"
    exit 1
}

# Function to build LinuxTrack
build_linuxtrack() {
    print_status "Building LinuxTrack..."
    
    # Generate build system
    print_status "Generating build system with autoreconf..."
    autoreconf -fiv
    
    # Configure
    print_status "Configuring build..."
    ./configure --prefix=/usr/local
    
    # Determine number of CPU cores for parallel build
    NPROC=$(nproc 2>/dev/null || echo 4)
    
    # Build
    print_status "Building with $NPROC parallel jobs..."
    make -j$NPROC
    
    print_success "Build completed successfully!"
}

# Function to install
install_linuxtrack() {
    print_status "Installing LinuxTrack..."
    
    sudo make install
    
    # Install udev rules
    if [ -f src/99-TIR.rules ]; then
        print_status "Installing udev rules..."
        sudo cp src/99-TIR.rules /lib/udev/rules.d/
        sudo udevadm control --reload-rules
        sudo udevadm trigger
        print_success "Udev rules installed"
    fi
    
    # Create linuxtrack group if it doesn't exist
    if ! getent group linuxtrack > /dev/null 2>&1; then
        print_status "Creating linuxtrack group..."
        sudo groupadd linuxtrack
    fi
    
    # Add current user to required groups
    print_status "Adding user to required groups..."
    sudo usermod -a -G plugdev,linuxtrack $USER
    
    print_success "Installation completed!"
    print_warning "Please log out and log back in for group changes to take effect"
}

# Function to run tests
run_tests() {
    print_status "Running basic tests..."
    
    # Check if TrackIR device is connected
    if lsusb | grep -q "131d:0159"; then
        print_success "TrackIR 5 device detected"
    else
        print_warning "No TrackIR device detected (this is OK if you don't have one)"
    fi
    
    # Check if executable was built
    if [ -f src/qt_gui/ltr_gui ]; then
        print_success "GUI executable built successfully"
    else
        print_error "GUI executable not found"
        return 1
    fi
    
    # Check library dependencies
    print_status "Checking library dependencies..."
    if ldd src/qt_gui/ltr_gui | grep -q "not found"; then
        print_warning "Some library dependencies may be missing:"
        ldd src/qt_gui/ltr_gui | grep "not found" || true
    else
        print_success "All library dependencies satisfied"
    fi
}

# Main script
main() {
    print_status "LinuxTrack Build Script"
    print_status "======================="
    
    # Parse command line arguments
    INSTALL=false
    CLEAN=false
    DEPS=false
    TEST=false
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            --install|-i)
                INSTALL=true
                shift
                ;;
            --clean|-c)
                CLEAN=true
                shift
                ;;
            --deps|-d)
                DEPS=true
                shift
                ;;
            --test|-t)
                TEST=true
                shift
                ;;
            --help|-h)
                echo "Usage: $0 [options]"
                echo "Options:"
                echo "  --deps, -d     Install dependencies"
                echo "  --clean, -c    Clean before building"
                echo "  --install, -i  Install after building"
                echo "  --test, -t     Run tests after building"
                echo "  --help, -h     Show this help"
                echo ""
                echo "Examples:"
                echo "  $0 --deps --clean --install  # Full build and install"
                echo "  $0 --clean                   # Just build"
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                print_error "Use --help for usage information"
                exit 1
                ;;
        esac
    done
    
    # Install dependencies if requested
    if [ "$DEPS" = true ]; then
        install_dependencies
    fi
    
    # Check Qt5
    check_qt5
    
    # Clean if requested
    if [ "$CLEAN" = true ]; then
        clean_build
    fi
    
    # Build
    build_linuxtrack
    
    # Test if requested
    if [ "$TEST" = true ]; then
        run_tests
    fi
    
    # Install if requested
    if [ "$INSTALL" = true ]; then
        install_linuxtrack
    fi
    
    print_success "Build script completed!"
    
    if [ "$INSTALL" = false ]; then
        print_status "To install LinuxTrack, run:"
        print_status "  sudo make install"
        print_status "Or run this script with --install flag"
    fi
    
    print_status ""
    print_status "To run LinuxTrack GUI:"
    print_status "  ./run_qt5_gui.sh"
    print_status "Or after installation:"
    print_status "  /usr/local/bin/ltr_gui"
}

# Run main function
main "$@" 
