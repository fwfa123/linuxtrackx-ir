# MX Linux Build Guide for LinuxTrack

**Distribution**: MX Linux 23.6+ (Debian-based)  
**Last Updated**: July 2025  
**Status**: Tested and Verified ✅

## 🎯 Overview

This guide shows how to build LinuxTrack on MX Linux and similar Debian-based distributions. **NEW**: LinuxTrack now supports MinGW cross-compilation, eliminating the need for wine-devel packages and making the build process much simpler!

## 🚀 Quick Start (Recommended)

### Method 1: MinGW Build (No Wine Required) ⭐ **RECOMMENDED**
```bash
# Clone the repository
git clone <repository-url>
cd linuxtrack-clean-june14

# Install minimal dependencies
sudo apt update
sudo apt install -y build-essential cmake
sudo apt install -y qtbase5-dev qttools5-dev-tools
sudo apt install -y libopencv-dev libusb-1.0-0-dev libmxml-dev
sudo apt install -y mingw-w64 gcc-mingw-w64 g++-mingw-w64

# Build LinuxTrack
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)
```

### Method 2: Automated Build Script
```bash
# Clone the repository
git clone <repository-url>
cd linuxtrack-clean-june14

# Run the MX Linux specific build script
./dev-scripts/build_mx_linux.sh
```

## 🔧 Dependencies

### Core Dependencies (Required)
```bash
sudo apt update
sudo apt install -y build-essential cmake
sudo apt install -y qtbase5-dev qttools5-dev-tools qt5-qmake
sudo apt install -y libopencv-dev libusb-1.0-0-dev libmxml-dev
sudo apt install -y libx11-dev libxrandr-dev
```

### Windows Compatibility (Choose One)

#### Option A: MinGW Cross-Compilation ⭐ **RECOMMENDED**
```bash
# Lightweight, no Wine runtime needed
sudo apt install -y mingw-w64 gcc-mingw-w64 g++-mingw-w64
```

**Why MinGW is Better:**
- ✅ **Future-Proof**: Industry standard cross-compilation
- ✅ **Portable**: PE binaries work on Windows and Wine
- ✅ **Efficient**: No Wine runtime overhead
- ✅ **Maintainable**: Simpler build process

#### Option B: Wine Development Tools (Legacy) ⚠️ **NOT RECOMMENDED**
```bash
# Larger, requires Wine runtime
sudo apt install -y wine-development wine32-development wine64-development
```

**Why wine-devel is Deprecated:**
- ❌ **Wine-Specific**: Only works with Wine, not native Windows
- ❌ **Heavy Dependencies**: Large wine-devel packages
- ❌ **Performance Overhead**: Wine translation layer
- ❌ **Maintenance Burden**: Complex build environment

**Note**: The winegcc approach produces `.exe.so`/`.dll.so` files that are Linux ELF binaries with Wine wrappers, not genuine Windows PE binaries.

### Optional: 32-bit Support (if needed)
```bash
sudo apt install -y gcc-multilib g++-multilib libc6-dev-i386
sudo apt install -y lib32gcc-s1 lib32stdc++6
```

## 🏗️ Build Process

### Step 1: Clean Previous Builds
```bash
make distclean 2>/dev/null || true
rm -f configure aclocal.m4 config.h.in
rm -rf autom4te.cache/
```

### Step 2: Fix Autotools (MX Linux Specific)
```bash
# Check available automake versions
ls /usr/bin/automake-*

# Create symlinks if needed (replace X.Y with your version)
sudo ln -sf /usr/bin/automake-1.16 /usr/bin/automake-1.17 2>/dev/null || true
sudo ln -sf /usr/bin/aclocal-1.16 /usr/bin/aclocal-1.17 2>/dev/null || true
```

### Step 3: Configure and Build
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake (MinGW support)
cmake .. -DCMAKE_INSTALL_PREFIX=/opt

# Or configure with specific options
# cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_DEBUG=ON

# Build
cmake --build . -j$(nproc)
```

## 🎮 Windows Game Compatibility

LinuxTrack now builds the following Windows components using MinGW:

### Built Components ✅
- **NPClient.dll** (32-bit) - TrackIR API compatibility
- **NPClient64.dll** (64-bit) - 64-bit TrackIR compatibility  
- **FreeTrackClient.dll** (32-bit) - FreeTrack API compatibility
- **Tester.exe** (32-bit) - TrackIR testing utility
- **Tester64.exe** (64-bit) - 64-bit testing utility
- **Controller.exe** (32-bit) - Hotkey controller
- **check_data.exe** (32-bit) - Data validation utility

### Why MinGW is Better
- ✅ **Smaller Dependencies**: No wine-devel packages needed
- ✅ **Faster Builds**: No Wine runtime overhead
- ✅ **Cleaner System**: Fewer packages to maintain
- ✅ **Better Compatibility**: Standard Windows PE binaries
- ✅ **CI/CD Friendly**: Easier automated builds

## 🔍 Troubleshooting

### Problem: `aclocal-1.17: command not found`
**Solution**:
```bash
# Automatic fix
ls /usr/bin/automake-* | head -1 | sed 's/automake/aclocal/' | xargs -I {} sudo ln -sf {} /usr/bin/aclocal-1.17
ls /usr/bin/automake-* | head -1 | xargs -I {} sudo ln -sf {} /usr/bin/automake-1.17
```

### Problem: Wine bridge compilation errors
**Solution**: Use MinGW instead (recommended)
```bash
# Verify MinGW is installed
which i686-w64-mingw32-gcc x86_64-w64-mingw32-gcc

# If not installed:
sudo apt install -y mingw-w64 gcc-mingw-w64 g++-mingw-w64

# Reconfigure and rebuild
rm -rf build && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)
```

### Problem: Qt5 not found
**Solution**:
```bash
sudo apt install -y qtbase5-dev qttools5-dev-tools qt5-qmake libqt5x11extras5-dev
# Reconfigure with CMake (Qt5 auto-detected)
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
```

### Problem: OpenCV not found
**Solution**:
```bash
sudo apt install -y libopencv-dev pkg-config
# Verify installation
pkg-config --modversion opencv4
```

## ⚙️ Build Configuration Options

### Standard Build
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
```

### Debug Build
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_DEBUG=ON
```

### Disable Wine Components (Linux-only)
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
# Wine bridge is auto-detected, but can be disabled by not installing wine development tools
```

### Custom Qt5 Path
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DQt5_DIR=/path/to/qt5
```

## 📦 Installation

### Install LinuxTrack
```bash
sudo make install

# Add user to required groups
sudo usermod -a -G plugdev,input $USER

# Log out and log back in for group changes to take effect
```

### Verify Installation
```bash
# Test the GUI
ltr_gui

# Check Wine bridge components (if built)
ls -la /opt/bin/*.dll /opt/bin/*.exe 2>/dev/null || echo "No Windows components built"
```

## 🔄 Advanced Usage

### Building Only Specific Components
```bash
# Build only the main LinuxTrack library
make -C src

# Build only the Qt GUI
make -C src/qt_gui

# Build only the Wine bridge
make -C src/wine_bridge
```

### Cross-Compilation Details
LinuxTrack automatically detects MinGW compilers and uses them when available:
- `i686-w64-mingw32-gcc` for 32-bit Windows components
- `x86_64-w64-mingw32-gcc` for 64-bit Windows components
- Falls back to wine-devel if MinGW not available

## 📋 System Information

### Check Your Build Environment
```bash
# MX Linux version
cat /etc/os-release | grep VERSION

# Available compilers
which gcc g++ i686-w64-mingw32-gcc x86_64-w64-mingw32-gcc 2>/dev/null

# Qt5 version
qmake-qt5 --version 2>/dev/null || qmake --version

# OpenCV version
pkg-config --modversion opencv4 2>/dev/null || pkg-config --modversion opencv
```

## 📞 Support

### Getting Help
1. **Try the automated script first**: `./dev-scripts/build_mx_linux.sh`
2. **Check dependencies**: Ensure all required packages are installed
3. **Check build logs**: Look for specific error messages
4. **Use MinGW method**: If wine-devel causes issues

### Reporting Issues
Include this information when reporting build issues:
```bash
# System info
uname -a
cat /etc/os-release

# Build environment
which gcc g++ make cmake
which i686-w64-mingw32-gcc x86_64-w64-mingw32-gcc || echo "MinGW not installed"

# CMake configuration output
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt 2>&1 | tail -20
```

---

**Success Indicator**: If the build completes without errors and you see Windows `.dll` and `.exe` files in `src/wine_bridge/*/`, your build was successful!

**Next Steps**: After successful build, run `sudo make install` and `ltr_gui` to start using LinuxTrack.
