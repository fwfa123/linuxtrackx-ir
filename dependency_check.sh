#!/bin/bash
# dependency_check.sh - Verify all LinuxTrack dependencies
# This script checks for all required dependencies for building LinuxTrack X-IR

set -e

echo "=== LinuxTrack X-IR Dependency Check ==="
echo "This script verifies all required dependencies for building LinuxTrack X-IR"
echo

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print status
print_status() {
    local status=$1
    local message=$2
    if [ "$status" = "OK" ]; then
        echo -e "${GREEN}✓${NC} $message"
    elif [ "$status" = "WARN" ]; then
        echo -e "${YELLOW}⚠${NC} $message"
    else
        echo -e "${RED}✗${NC} $message"
    fi
}

# Function to check if command exists
check_command() {
    local cmd=$1
    local name=${2:-$1}
    if command -v "$cmd" >/dev/null 2>&1; then
        print_status "OK" "$name found"
        return 0
    else
        print_status "FAIL" "$name missing"
        return 1
    fi
}

# Function to check if pkg-config module exists
check_pkgconfig() {
    local module=$1
    if pkg-config --exists "$module" 2>/dev/null; then
        print_status "OK" "$module found"
        return 0
    else
        print_status "FAIL" "$module missing"
        return 1
    fi
}

# Function to check if header file exists
check_header() {
    local header=$1
    local name=${2:-$1}
    if [ -f "$header" ]; then
        print_status "OK" "$name found"
        return 0
    else
        print_status "FAIL" "$name missing"
        return 1
    fi
}

# Initialize counters
total_checks=0
passed_checks=0
failed_checks=0
warn_checks=0

# Build tools
echo -e "${BLUE}=== Build Tools ===${NC}"
for tool in gcc g++ make autoconf automake libtool bison flex pkg-config; do
    total_checks=$((total_checks + 1))
    if check_command "$tool"; then
        passed_checks=$((passed_checks + 1))
    else
        failed_checks=$((failed_checks + 1))
    fi
done
echo

# Qt5 verification
echo -e "${BLUE}=== Qt5 Framework ===${NC}"
total_checks=$((total_checks + 1))
if check_command "qmake-qt5" "qmake-qt5"; then
    passed_checks=$((passed_checks + 1))
    qt_version=$(qmake-qt5 -query QT_VERSION 2>/dev/null || echo "unknown")
    print_status "OK" "Qt version: $qt_version"
else
    failed_checks=$((failed_checks + 1))
fi

# Qt5 modules
for module in Qt5Core Qt5Widgets Qt5OpenGL Qt5Help Qt5Network Qt5X11Extras; do
    total_checks=$((total_checks + 1))
    if check_pkgconfig "$module"; then
        passed_checks=$((passed_checks + 1))
    else
        failed_checks=$((failed_checks + 1))
    fi
done
echo

# Core libraries
echo -e "${BLUE}=== Core Libraries ===${NC}"
for lib in opencv4 libusb-1.0 mxml; do
    total_checks=$((total_checks + 1))
    if check_pkgconfig "$lib"; then
        passed_checks=$((passed_checks + 1))
    else
        failed_checks=$((failed_checks + 1))
    fi
done
echo

# Optional libraries
echo -e "${BLUE}=== Optional Libraries ===${NC}"
for lib in liblo libcwiid; do
    total_checks=$((total_checks + 1))
    if check_pkgconfig "$lib"; then
        passed_checks=$((passed_checks + 1))
    else
        warn_checks=$((warn_checks + 1))
        print_status "WARN" "$lib missing (optional)"
    fi
done
echo

# Development headers
echo -e "${BLUE}=== Development Headers ===${NC}"

# Qt5 headers
for qt_module in Core Widgets OpenGL Help Network X11Extras; do
    header_path="/usr/include/qt5/Qt$qt_module/Qt$qt_module"
    total_checks=$((total_checks + 1))
    if check_header "$header_path" "Qt5 $qt_module headers"; then
        passed_checks=$((passed_checks + 1))
    else
        failed_checks=$((failed_checks + 1))
    fi
done

# Other headers
headers=(
    "/usr/include/opencv4/opencv2/opencv.hpp:OpenCV headers"
    "/usr/include/libusb-1.0/libusb.h:libusb headers"
    "/usr/include/mxml.h:mxml headers"
    "/usr/include/libv4l2.h:V4L2 headers"
    "/usr/include/linux/videodev2.h:Video4Linux2 headers"
    "/usr/include/linux/uinput.h:uinput headers"
    "/usr/include/X11/Xlib.h:X11 headers"
    "/usr/include/X11/extensions/Xrandr.h:Xrandr headers"
)

for header_info in "${headers[@]}"; do
    IFS=':' read -r header_path header_name <<< "$header_info"
    total_checks=$((total_checks + 1))
    if check_header "$header_path" "$header_name"; then
        passed_checks=$((passed_checks + 1))
    else
        failed_checks=$((failed_checks + 1))
    fi
done
echo

# MinGW dependencies
echo -e "${BLUE}=== Windows Compatibility (MinGW) ===${NC}"
for mingw in i686-w64-mingw32-gcc x86_64-w64-mingw32-gcc i686-w64-mingw32-g++ x86_64-w64-mingw32-g++; do
    total_checks=$((total_checks + 1))
    if check_command "$mingw"; then
        passed_checks=$((passed_checks + 1))
    else
        warn_checks=$((warn_checks + 1))
        print_status "WARN" "$mingw missing (Windows components will not be built)"
    fi
done

total_checks=$((total_checks + 1))
if check_command "makensis" "NSIS"; then
    passed_checks=$((passed_checks + 1))
else
    warn_checks=$((warn_checks + 1))
    print_status "WARN" "makensis missing (Windows installers will not be built)"
fi
echo

# 32-bit development headers
echo -e "${BLUE}=== 32-bit Development (Optional) ===${NC}"
total_checks=$((total_checks + 1))
if [ -f "/usr/include/i386-linux-gnu/bits/libc-header-start.h" ] || [ -f "/usr/include/bits/libc-header-start.h" ]; then
    passed_checks=$((passed_checks + 1))
    print_status "OK" "32-bit development headers found"
else
    warn_checks=$((warn_checks + 1))
    print_status "WARN" "32-bit development headers missing (32-bit library will not be built)"
fi
echo

# Summary
echo -e "${BLUE}=== Summary ===${NC}"
echo "Total checks: $total_checks"
echo -e "${GREEN}Passed: $passed_checks${NC}"
echo -e "${RED}Failed: $failed_checks${NC}"
echo -e "${YELLOW}Warnings: $warn_checks${NC}"
echo

if [ $failed_checks -eq 0 ]; then
    if [ $warn_checks -eq 0 ]; then
        echo -e "${GREEN}🎉 All dependencies are satisfied! You can build LinuxTrack X-IR.${NC}"
    else
        echo -e "${GREEN}✅ Core dependencies are satisfied!${NC}"
        echo -e "${YELLOW}⚠️  Some optional dependencies are missing, but core functionality will work.${NC}"
    fi
else
    echo -e "${RED}❌ Some required dependencies are missing. Please install them before building.${NC}"
    echo
    echo "For installation instructions, see the README.md file."
    echo "Quick install commands:"
    echo
    echo "Debian/Ubuntu/MX Linux:"
    echo "  sudo apt install build-essential autoconf automake libtool qtbase5-dev qttools5-dev-tools libqt5x11extras5-dev libqt5help5-dev libopencv-dev libusb-1.0-0-dev libmxml-dev libx11-dev libxrandr-dev mingw-w64 bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev"
    echo
    echo "Fedora/RHEL:"
    echo "  sudo dnf install gcc gcc-c++ make autoconf automake libtool qt5-qtbase-devel qt5-qttools-devel qt5-qtx11extras-devel opencv-devel libusb1-devel libmxml-devel libX11-devel libXrandr-devel mingw64-gcc mingw64-gcc-c++ bison flex nsis glibc-devel.i686 libstdc++-devel.i686 v4l-utils-devel"
    echo
    echo "Arch Linux:"
    echo "  sudo pacman -S --needed base-devel autoconf automake libtool qt5-base qt5-tools qt5-x11extras qt5-help opencv libusb libmxml libx11 libxrandr mingw-w64-gcc bison flex nsis lib32-glibc lib32-gcc-libs v4l-utils"
fi

echo
echo "=== Check Complete ===" 