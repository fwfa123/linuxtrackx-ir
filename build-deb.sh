#!/bin/bash

# LinuxTrack Debian Package Build Script
# This script builds DEB packages for testing installation

set -e

echo "=================================================="
echo " LinuxTrack Debian Package Builder"
echo "=================================================="
echo ""

# Check if we're in the right directory
if [[ ! -f "configure.ac" ]]; then
    echo "ERROR: Run this script from the LinuxTrack source directory"
    exit 1
fi

# Check for required tools
echo "Checking build dependencies..."
missing_tools=()

for tool in debuild dpkg-buildpackage fakeroot; do
    if ! command -v $tool >/dev/null 2>&1; then
        missing_tools+=($tool)
    fi
done

if [[ ${#missing_tools[@]} -gt 0 ]]; then
    echo "ERROR: Missing required tools: ${missing_tools[*]}"
    echo ""
    echo "Install with:"
    echo "  sudo apt install devscripts build-essential fakeroot"
    exit 1
fi

echo "✓ Build tools available"
echo ""

# Clean any previous builds
echo "Cleaning previous build artifacts..."
rm -rf debian/tmp debian/.debhelper debian/files
rm -f ../*.deb ../*.dsc ../*.tar.* ../*.changes ../*.buildinfo
echo "✓ Clean complete"
echo ""

# Generate configure script if needed
if [[ ! -f configure ]]; then
    echo "Generating configure script..."
    autoreconf -fiv
    echo "✓ Configure script generated"
    echo ""
fi

# Validate Debian package files
echo "Validating Debian package configuration..."

if [[ ! -f debian/control ]]; then
    echo "ERROR: debian/control file missing"
    exit 1
fi

if [[ ! -f debian/rules ]]; then
    echo "ERROR: debian/rules file missing"
    exit 1
fi

if [[ ! -x debian/rules ]]; then
    chmod +x debian/rules
fi

echo "✓ Debian configuration valid"
echo ""

# Build source package first
echo "Building source package..."
dpkg-source --build .
echo "✓ Source package built"
echo ""

# Build binary packages
echo "Building binary packages..."
echo "This may take several minutes..."
echo ""

if debuild -us -uc -b; then
    echo ""
    echo "=================================================="
    echo " Build Successful!"
    echo "=================================================="
    echo ""
    echo "Generated packages:"
    ls -la ../*.deb 2>/dev/null || echo "No .deb files found"
    echo ""
    echo "To install:"
    echo "  sudo dpkg -i ../linuxtrack_*.deb"
    echo "  sudo apt-get install -f  # Fix any dependency issues"
    echo ""
    echo "To test installation:"
    echo "  sudo dpkg -i ../linuxtrack_*.deb && linuxtrack-verify"
    echo ""
    echo "To remove:"
    echo "  sudo dpkg -r linuxtrack"
    echo ""
else
    echo ""
    echo "=================================================="
    echo " Build Failed!"
    echo "=================================================="
    echo ""
    echo "Check the build log above for errors."
    echo "Common issues:"
    echo "  - Missing build dependencies"
    echo "  - Qt5 development packages not installed"
    echo "  - OpenCV development libraries missing"
    echo ""
    echo "Install dependencies with:"
    echo "  sudo apt install qtbase5-dev qttools5-dev-tools"
    echo "  sudo apt install libusb-1.0-0-dev libmxml-dev libopencv-dev"
    echo ""
    exit 1
fi 