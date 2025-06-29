#!/bin/bash

# LinuxTrack RPM Package Build Script
# This script builds RPM packages for testing installation

set -e

echo "=================================================="
echo " LinuxTrack RPM Package Builder"
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

for tool in rpmbuild rpmdev-setuptree; do
    if ! command -v $tool >/dev/null 2>&1; then
        missing_tools+=($tool)
    fi
done

if [[ ${#missing_tools[@]} -gt 0 ]]; then
    echo "ERROR: Missing required tools: ${missing_tools[*]}"
    echo ""
    echo "Install with:"
    echo "  sudo dnf install rpm-build rpmdevtools"
    echo "  # or on older systems:"
    echo "  sudo yum install rpm-build rpmdevtools"
    exit 1
fi

echo "✓ Build tools available"
echo ""

# Set up RPM build environment
echo "Setting up RPM build environment..."
rpmdev-setuptree
echo "✓ RPM tree setup complete"
echo ""

# Get version from configure.ac
VERSION=$(grep "AC_INIT" configure.ac | sed 's/.*\[\([0-9.]*\)\].*/\1/')
if [[ -z "$VERSION" ]]; then
    VERSION="0.99.20"
fi

echo "Building LinuxTrack version: $VERSION"
echo ""

# Create source tarball
echo "Creating source tarball..."
TARBALL="linuxtrack-${VERSION}.tar.gz"

# Clean any autotools files first
make distclean 2>/dev/null || true
rm -f configure aclocal.m4 config.h.in
rm -rf autom4te.cache/

# Create clean tarball
tar --exclude-vcs --exclude="debian" --exclude="*.tar.gz" \
    -czf ~/rpmbuild/SOURCES/$TARBALL \
    --transform "s,^,linuxtrack-${VERSION}/," \
    .

echo "✓ Source tarball created: ~/rpmbuild/SOURCES/$TARBALL"
echo ""

# Copy spec file
echo "Copying spec file..."
cp linuxtrack.spec ~/rpmbuild/SPECS/
echo "✓ Spec file copied"
echo ""

# Validate spec file
echo "Validating spec file..."
if ! rpmlint ~/rpmbuild/SPECS/linuxtrack.spec; then
    echo "WARNING: rpmlint found issues with spec file"
    echo "Continuing anyway..."
fi
echo ""

# Build RPM packages
echo "Building RPM packages..."
echo "This may take several minutes..."
echo ""

if rpmbuild -ba ~/rpmbuild/SPECS/linuxtrack.spec; then
    echo ""
    echo "=================================================="
    echo " Build Successful!"
    echo "=================================================="
    echo ""
    echo "Generated packages:"
    find ~/rpmbuild/RPMS/ -name "*.rpm" -type f | head -10
    echo ""
    echo "Source packages:"
    find ~/rpmbuild/SRPMS/ -name "*.rpm" -type f | head -5
    echo ""
    echo "To install:"
    echo "  sudo dnf install ~/rpmbuild/RPMS/*/linuxtrack-*.rpm"
    echo "  # or on older systems:"
    echo "  sudo yum localinstall ~/rpmbuild/RPMS/*/linuxtrack-*.rpm"
    echo ""
    echo "To test installation:"
    echo "  sudo dnf install ~/rpmbuild/RPMS/*/linuxtrack-*.rpm && linuxtrack-verify"
    echo ""
    echo "To remove:"
    echo "  sudo dnf remove linuxtrack"
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
    echo "  sudo dnf install qt5-qtbase-devel qt5-qttools-devel"
    echo "  sudo dnf install libusb1-devel mxml-devel opencv-devel"
    echo ""
    exit 1
fi 