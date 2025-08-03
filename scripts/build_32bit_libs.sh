#!/bin/bash
# Build 32-bit versions of missing libraries for LinuxTrack on Arch Linux
# Usage: ./scripts/build_32bit_libs.sh

set -e

echo "Building 32-bit libraries for LinuxTrack on Arch Linux..."
echo "This script will build lib32-mxml and lib32-liblo"

# Check if we're on Arch Linux
if ! grep -q "Arch Linux" /etc/os-release 2>/dev/null; then
    echo "Warning: This script is designed for Arch Linux"
fi

# Install required build dependencies
echo "Installing build dependencies..."
sudo pacman -S --needed base-devel multilib-devel

# Create temporary build directory
BUILD_DIR="/tmp/linuxtrack-32bit-builds"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Set 32-bit compiler flags
export CFLAGS="-m32"
export CXXFLAGS="-m32"
export LDFLAGS="-m32"
export PKG_CONFIG_PATH='/usr/lib32/pkgconfig'

# Function to build a library
build_32bit_lib() {
    local lib_name=$1
    local version=$2
    local source_url=$3
    
    echo "Building lib32-$lib_name..."
    
    # Download and extract source
    wget "$source_url"
    tar -xf "$lib_name-$version.tar.gz"
    cd "$lib_name-$version"
    
    # Configure for 32-bit
    ./configure \
        --program-suffix="-32" \
        --libdir=/usr/lib32 \
        --libexecdir=/usr/lib32 \
        --includedir=/usr/include/"$lib_name"32 \
        --build=i686-pc-linux-gnu \
        --prefix=/usr
    
    # Build and install
    make -j$(nproc)
    sudo make install
    
    # Create symlinks for compatibility
    sudo ln -sf /usr/lib32/lib"$lib_name".so.1 /usr/lib32/lib"$lib_name".so
    
    cd ..
    echo "lib32-$lib_name built and installed successfully!"
}

# Build mxml
build_32bit_lib "mxml" "3.3.1" "https://github.com/michaelrsweet/mxml/releases/download/v3.3.1/mxml-3.3.1.tar.gz"

# Build liblo
build_32bit_lib "liblo" "0.32" "https://github.com/radarsat1/liblo/releases/download/0.32/liblo-0.32.tar.gz"

echo ""
echo "32-bit libraries built successfully!"
echo "You can now try building LinuxTrack again."
echo ""
echo "To clean up build files:"
echo "rm -rf $BUILD_DIR" 