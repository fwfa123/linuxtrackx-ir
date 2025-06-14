#!/bin/bash

# LinuxTrack Modernization - Dependency Setup Script
# This script sets up development dependencies for Qt5 migration

set -e

echo "Setting up LinuxTrack modernization development environment..."

# Detect distribution
if [ -f /etc/os-release ]; then
    . /etc/os-release
    DISTRO=$ID
    VERSION=$VERSION_ID
else
    echo "Cannot detect distribution"
    exit 1
fi

echo "Detected: $DISTRO $VERSION"

# Install Qt5 development dependencies
case $DISTRO in
    "fedora")
        echo "Installing Fedora dependencies..."
        sudo dnf install -y \
            qt5-qtbase-devel \
            qt5-qttools-devel \
            qt5-qtwidgets-devel \
            qt5-qtopengl-devel \
            qt5-qthelp-devel \
            cmake \
            ninja-build \
            gcc-c++ \
            mxml-devel \
            libusb1-devel \
            opencv-devel \
            libv4l-devel \
            autotools \
            automake \
            libtool \
            bison \
            flex \
            pkgconfig
        ;;
    "ubuntu"|"debian")
        echo "Installing Ubuntu/Debian dependencies..."
        sudo apt update
        sudo apt install -y \
            qtbase5-dev \
            qttools5-dev \
            qttools5-dev-tools \
            libqt5opengl5-dev \
            libqt5help5 \
            qthelp5-dev-tools \
            cmake \
            ninja-build \
            build-essential \
            libmxml-dev \
            libusb-1.0-0-dev \
            libopencv-dev \
            libv4l-dev \
            autotools-dev \
            automake \
            libtool \
            bison \
            flex \
            pkg-config
        ;;
    "arch")
        echo "Installing Arch Linux dependencies..."
        sudo pacman -S --noconfirm \
            qt5-base \
            qt5-tools \
            cmake \
            ninja \
            gcc \
            mxml \
            libusb \
            opencv \
            v4l-utils \
            autoconf \
            automake \
            libtool \
            bison \
            flex \
            pkgconf
        ;;
    *)
        echo "Unsupported distribution: $DISTRO"
        echo "Please install Qt5 development packages manually"
        echo "Required packages:"
        echo "- Qt5 base development"
        echo "- Qt5 tools and widgets"
        echo "- Qt5 OpenGL support"
        echo "- mxml, libusb-1.0, opencv development libraries"
        echo "- Build tools: cmake, ninja, gcc/g++, autotools"
        exit 1
        ;;
esac

echo "Development dependencies installed successfully!"

# Verify Qt5 installation
echo "Verifying Qt5 installation..."
qmake -v || echo "Warning: qmake not found in PATH"
which cmake > /dev/null || echo "Warning: cmake not found"

echo "Setup complete!"
echo ""
echo "Next steps:"
echo "1. Run 'source dev-env/bin/activate' to activate Python environment"
echo "2. Run the Qt5 migration script when ready" 