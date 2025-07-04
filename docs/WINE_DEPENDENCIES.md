# MinGW Dependencies for LinuxTrack

This document provides distribution-specific installation instructions for MinGW dependencies required to build and use the LinuxTrack Wine Bridge.

## Overview

The LinuxTrack Wine Bridge now uses MinGW cross-compilation to build Windows-compatible components that provide head tracking support for Windows applications running under Wine. This approach eliminates the need for complex wine-devel packages and produces native Windows PE binaries.

## Distribution-Specific Installation

### Ubuntu/Debian (22.04+)

```bash
# Install MinGW cross-compilation packages
sudo apt update
sudo apt install mingw-w64 gcc-mingw-w64 g++-mingw-w64

# Install NSIS for installer generation
sudo apt install nsis

# Install additional dependencies
sudo apt install build-essential autoconf automake libtool
```

**Package Details:**
- `mingw-w64`: MinGW-w64 cross-compilation toolchain
- `gcc-mingw-w64`: MinGW C compiler
- `g++-mingw-w64`: MinGW C++ compiler
- `nsis`: Nullsoft Scriptable Install System
- `build-essential`: Essential build tools
- `autoconf automake libtool`: Autotools for build system

### Fedora (38+)

```bash
# Install MinGW cross-compilation packages
sudo dnf install mingw64-gcc mingw64-gcc-c++ mingw64-winpthreads

# Install NSIS
sudo dnf install nsis

# Install additional dependencies
sudo dnf install gcc gcc-c++ autoconf automake libtool
```

**Package Details:**
- `mingw64-gcc`: MinGW-w64 C compiler
- `mingw64-gcc-c++`: MinGW-w64 C++ compiler
- `mingw64-winpthreads`: MinGW pthreads library
- `nsis`: NSIS installer system
- `gcc gcc-c++`: Native compilers
- `autoconf automake libtool`: Autotools for build system

### Arch Linux

```bash
# Install MinGW cross-compilation packages
sudo pacman -S mingw-w64-gcc mingw-w64-gcc-fortran

# Install NSIS
sudo pacman -S nsis

# Install additional dependencies
sudo pacman -S base-devel autoconf automake libtool
```

**Package Details:**
- `mingw-w64-gcc`: MinGW-w64 cross-compilation toolchain
- `mingw-w64-gcc-fortran`: MinGW-w64 Fortran compiler (optional)
- `nsis`: NSIS installer system
- `base-devel`: Base development tools
- `autoconf automake libtool`: Autotools for build system

### OpenSUSE (Tumbleweed/Leap 15.5+)

```bash
# Install MinGW cross-compilation packages
sudo zypper install cross-mingw64-gcc cross-mingw64-gcc-c++

# Install NSIS
sudo zypper install nsis

# Install additional dependencies
sudo zypper install gcc gcc-c++ autoconf automake libtool
```

**Package Details:**
- `cross-mingw64-gcc`: MinGW-w64 C compiler
- `cross-mingw64-gcc-c++`: MinGW-w64 C++ compiler
- `nsis`: NSIS installer system
- `gcc gcc-c++`: Native compilers
- `autoconf automake libtool`: Autotools for build system

### Gentoo

```bash
# Install MinGW cross-compilation packages
sudo emerge --ask cross-x86_64-w64-mingw32/gcc

# Install NSIS
sudo emerge --ask dev-util/nsis

# Install additional dependencies
sudo emerge --ask sys-devel/autoconf sys-devel/automake sys-devel/libtool
```

**Package Details:**
- `cross-x86_64-w64-mingw32/gcc`: MinGW-w64 cross-compilation toolchain
- `dev-util/nsis`: NSIS installer system
- `sys-devel/autoconf`: Autoconf build system
- `sys-devel/automake`: Automake build system
- `sys-devel/libtool`: Libtool build system

## Verification

After installation, verify that MinGW cross-compilation tools are available:

```bash
# Check MinGW C compiler availability
which i686-w64-mingw32-gcc
i686-w64-mingw32-gcc --version

# Check MinGW C++ compiler availability
which i686-w64-mingw32-g++
i686-w64-mingw32-g++ --version

# Check 64-bit MinGW C compiler availability
which x86_64-w64-mingw32-gcc
x86_64-w64-mingw32-gcc --version

# Check makensis availability
which makensis
makensis /VERSION
```

## Troubleshooting

### Common Issues

#### 1. MinGW Compilers Not Found

**Problem**: `i686-w64-mingw32-gcc` or `x86_64-w64-mingw32-gcc` not found in PATH

**Solution**:
```bash
# Ubuntu/Debian
sudo apt install mingw-w64 gcc-mingw-w64 g++-mingw-w64

# Fedora
sudo dnf install mingw64-gcc mingw64-gcc-c++

# Arch
sudo pacman -S mingw-w64-gcc

# OpenSUSE
sudo zypper install cross-mingw64-gcc cross-mingw64-gcc-c++
```

#### 2. Cross-Compilation Fails

**Problem**: Cannot compile Windows components

**Solution**:
```bash
# Ensure MinGW toolchain is properly installed
sudo apt install mingw-w64

# Verify compiler availability
which i686-w64-mingw32-gcc
which x86_64-w64-mingw32-gcc

# Check if build system detects MinGW
./configure --help | grep -i mingw
```

#### 3. NSIS Not Found

**Problem**: `makensis` command not available

**Solution**:
```bash
# Ubuntu/Debian
sudo apt install nsis

# Fedora
sudo dnf install nsis

# Arch
sudo pacman -S nsis

# OpenSUSE
sudo zypper install nsis
```

#### 4. Wine Library Path Issues

**Problem**: Wine libraries not found during linking

**Solution**:
```bash
# Check wine library locations
find /usr -name "wine" -type d 2>/dev/null

# Configure with explicit wine library path
./configure --with-wine-libs="-L/path/to/wine/libs"
```

### Distribution-Specific Issues

#### Ubuntu/Debian
- **Issue**: Wine packages may be in different repositories
- **Solution**: Enable wine repository if needed
```bash
sudo add-apt-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ jammy main'
sudo apt update
```

#### Fedora
- **Issue**: Wine packages may require additional repositories
- **Solution**: Enable RPM Fusion if needed
```bash
sudo dnf install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm
```

#### Arch Linux
- **Issue**: Wine packages may be in AUR
- **Solution**: Use AUR helper or manual installation
```bash
yay -S wine-staging
```

## Wine Version Compatibility

### Minimum Requirements
- **Wine**: 6.0 or later
- **winegcc/wineg++**: 6.0 or later
- **makensis**: 3.0 or later

### Recommended Versions
- **Wine**: 8.0 or later
- **winegcc/wineg++**: 8.0 or later
- **makensis**: 3.08 or later

### Testing Wine Compatibility

```bash
# Test wine compilation
winegcc -o test.exe test.c
wine test.exe

# Test wine linking
winegcc -o test.exe test.c -luser32
wine test.exe
```

## Environment Variables

Set these environment variables if needed:

```bash
# Wine library path
export WINE_LIBS="-L/usr/lib/wine"

# Wine64 library path
export WINE64_LIBS="-L/usr/lib64/wine"

# Wine prefix (optional)
export WINEPREFIX="$HOME/.wine"
```

## Next Steps

After installing Wine dependencies:

1. **Configure LinuxTrack**: Run `./configure --enable-wine-plugin`
2. **Build Wine Bridge**: Run `make` to compile wine components
3. **Test Installation**: Use `make install` to install wine bridge
4. **Verify Functionality**: Test wine bridge with Windows applications

For more information, see the main [WINE_SETUP.md](../src/wine_bridge/WINE_SETUP.md) documentation. 