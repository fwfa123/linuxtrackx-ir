#!/bin/bash

# LinuxTrack Installation Script
# Handles complete installation including udev rules and desktop integration

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

print_status "LinuxTrack Installation Script"
print_status "=============================="
echo ""

# Check if we're in the right directory
if [ ! -f "configure.ac" ] || [ ! -d "src" ]; then
    print_error "Please run this script from the LinuxTrack source directory"
    exit 1
fi

# Check if LinuxTrack is built
if [ ! -f "src/qt_gui/ltr_gui" ] || [ ! -f "src/mickey/mickey" ]; then
    print_error "LinuxTrack is not built. Please run 'make' first."
    print_status "Or use './dev-scripts/build_mx_linux.sh' for a complete build and install."
    exit 1
fi

# Check if running as root
if [ "$EUID" -ne 0 ]; then
    print_error "This script must be run as root (use sudo)"
    exit 1
fi

print_status "Installing LinuxTrack..."
make install

print_success "LinuxTrack installed successfully!"
echo ""

print_status "Installation includes:"
print_status "  ✓ Executables (ltr_gui, mickey, ltr_server1, etc.)"
print_status "  ✓ Libraries and data files"
print_status "  ✓ udev rules for TrackIR devices (automatic)"
print_status "  ✓ Desktop integration (automatic)"
print_status "  ✓ Wine bridge components (if built)"
echo ""

print_status "Next steps:"
print_status "1. Add your user to the plugdev group:"
print_status "   sudo usermod -a -G plugdev $SUDO_USER"
print_status ""
print_status "2. Log out and log back in for group changes to take effect"
print_status ""
print_status "3. Test the installation:"
print_status "   ltr_gui"
print_status ""
print_status "4. Run verification (optional):"
print_status "   ./verify_installation.sh"
echo ""

print_success "Installation complete!" 