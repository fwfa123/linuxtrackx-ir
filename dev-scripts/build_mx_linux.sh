#!/bin/bash

# LinuxTrack MX Linux Build Script
# Specialized for MX Linux 23.6+ and similar Debian-based distributions

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

print_status "LinuxTrack MX Linux Build Script"
print_status "================================="
print_status "This script addresses MX Linux specific build issues"
echo ""

# Check if we're in the right directory
if [ ! -f "configure.ac" ] || [ ! -d "src/wine_bridge" ]; then
    print_error "Please run this script from the LinuxTrack source directory"
    exit 1
fi

# Step 1: Install essential build dependencies
print_status "Step 1: Installing essential build dependencies..."
sudo apt update
sudo apt install -y build-essential autoconf automake libtool
sudo apt install -y qtbase5-dev qttools5-dev-tools
sudo apt install -y libopencv-dev libusb-1.0-0-dev libmxml-dev
sudo apt install -y libx11-dev libxrandr-dev
print_success "Essential dependencies installed"
echo ""

# Step 2: Fix autotools version issues
print_status "Step 2: Fixing autotools version issues..."
if ! command_exists aclocal-1.17; then
    print_warning "aclocal-1.17 not found, creating symlink..."
    
    # Find available aclocal versions
    aclocal_versions=($(ls /usr/bin/aclocal-* 2>/dev/null | grep -E 'aclocal-[0-9]+\.[0-9]+' | sort -V))
    
    if [ ${#aclocal_versions[@]} -gt 0 ]; then
        latest_version=${aclocal_versions[-1]}
        print_status "Found aclocal version: $latest_version"
        sudo ln -sf "$latest_version" "/usr/bin/aclocal-1.17"
        print_success "Created symlink: $latest_version -> aclocal-1.17"
    else
        print_error "No aclocal versions found, installing automake..."
        sudo apt install -y automake
    fi
fi

if ! command_exists automake-1.17; then
    print_warning "automake-1.17 not found, creating symlink..."
    
    # Find available automake versions
    automake_versions=($(ls /usr/bin/automake-* 2>/dev/null | grep -E 'automake-[0-9]+\.[0-9]+' | sort -V))
    
    if [ ${#automake_versions[@]} -gt 0 ]; then
        latest_version=${automake_versions[-1]}
        print_status "Found automake version: $latest_version"
        sudo ln -sf "$latest_version" "/usr/bin/automake-1.17"
        print_success "Created symlink: $latest_version -> automake-1.17"
    fi
fi
print_success "Autotools version issues resolved"
echo ""

# Step 3: Install 32-bit development libraries
print_status "Step 3: Installing 32-bit development libraries..."
sudo apt install -y gcc-multilib g++-multilib libc6-dev-i386
sudo apt install -y lib32gcc-s1 lib32stdc++6
print_success "32-bit development libraries installed"
echo ""

# Step 4: Install wine bridge dependencies
print_status "Step 4: Installing wine bridge dependencies..."
print_status "Installing MinGW cross-compilation tools..."
sudo apt install -y mingw-w64 gcc-mingw-w64 g++-mingw-w64

print_status "Installing NSIS installer..."
sudo apt install -y nsis

print_status "Installing wine development tools (optional)..."
sudo apt install -y wine-development wine32-development wine64-development || print_warning "Wine development tools not available, will use MinGW"
print_success "Wine bridge dependencies installed"
echo ""

# Step 5: Clean and regenerate build system
print_status "Step 5: Cleaning and regenerating build system..."
make distclean 2>/dev/null || true
rm -f configure aclocal.m4 config.h.in
rm -rf autom4te.cache/

print_status "Regenerating build system..."
autoreconf -fiv
print_success "Build system regenerated"
echo ""

# Step 6: Configure and build
print_status "Step 6: Configuring and building LinuxTrack..."
./configure --prefix=/opt/linuxtrack

print_status "Building LinuxTrack (this may take several minutes)..."
make -j$(nproc)
print_success "LinuxTrack built successfully"
echo ""

# Step 7: Verify build
print_status "Step 7: Verifying build..."
if [ -f "src/qt_gui/ltr_gui" ]; then
    print_success "Qt GUI built successfully"
else
    print_error "Qt GUI build failed"
    exit 1
fi

if [ -f "src/mickey/mickey" ]; then
    print_success "Mickey built successfully"
else
    print_error "Mickey build failed"
    exit 1
fi

# Check wine bridge components
wine_dir="src/wine_bridge"
required_wine_files=(
    "NPClient.dll.so"
    "Controller.exe.so"
    "Tester.exe.so"
    "FreeTrackClient.dll.so"
    "ftc.exe.so"
    "check_data.exe.so"
    "TrackIR.exe.so"
)

missing_wine_files=()
for file in "${required_wine_files[@]}"; do
    if [ ! -f "$wine_dir/$file" ]; then
        missing_wine_files+=("$file")
    fi
done

if [ ${#missing_wine_files[@]} -eq 0 ]; then
    print_success "All wine bridge components built successfully"
else
    print_warning "Some wine bridge components missing:"
    for file in "${missing_wine_files[@]}"; do
        print_warning "  - $file"
    done
    print_status "This is normal if wine development tools are not available"
fi

echo ""
print_success "LinuxTrack build completed successfully!"
print_status "You can now run 'sudo make install' to install LinuxTrack"
print_status "Or run 'sudo ./linuxtrack_install.sh' for complete installation with setup"
print_status "Or run './dev-scripts/build_wine_bridge.sh' for wine bridge specific options" 