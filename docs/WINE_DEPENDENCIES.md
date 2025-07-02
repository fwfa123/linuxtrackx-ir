# Wine Dependencies for LinuxTrack

This document provides distribution-specific installation instructions for Wine dependencies required to build and use the LinuxTrack Wine Bridge.

## Overview

The LinuxTrack Wine Bridge requires Wine development tools and libraries to compile Windows-compatible components that provide head tracking support for Windows applications running under Wine.

## Distribution-Specific Installation

### Ubuntu/Debian (22.04+)

```bash
# Install Wine development packages
sudo apt update
sudo apt install wine-development wine32-development wine64-development

# Install multilib support for 32-bit compilation
sudo apt install gcc-multilib libc6-dev-i386

# Install NSIS for installer generation
sudo apt install nsis

# Install additional dependencies
sudo apt install libwine-development-dev wine-development-dev
```

**Package Details:**
- `wine-development`: Latest Wine development version
- `wine32-development`: 32-bit Wine support
- `wine64-development`: 64-bit Wine support
- `gcc-multilib`: Multilib compiler support
- `libc6-dev-i386`: 32-bit development libraries
- `nsis`: Nullsoft Scriptable Install System
- `libwine-development-dev`: Wine development headers

### Fedora (38+)

```bash
# Install Wine development packages
sudo dnf install wine-devel wine-core wine-desktop

# Install multilib support
sudo dnf install gcc-multilib glibc-devel.i686

# Install NSIS
sudo dnf install nsis

# Install additional dependencies
sudo dnf install wine-devel-headers wine-devel-libs
```

**Package Details:**
- `wine-devel`: Wine development version
- `wine-core`: Core Wine components
- `wine-desktop`: Wine desktop integration
- `gcc-multilib`: Multilib compiler support
- `glibc-devel.i686`: 32-bit glibc development
- `nsis`: NSIS installer system
- `wine-devel-headers`: Wine development headers
- `wine-devel-libs`: Wine development libraries

### Arch Linux

```bash
# Install Wine development packages
sudo pacman -S wine wine-staging

# Install multilib support
sudo pacman -S mingw-w64-gcc

# Install NSIS
sudo pacman -S nsis

# Install additional dependencies
sudo pacman -S wine-mono wine-gecko
```

**Package Details:**
- `wine`: Standard Wine package
- `wine-staging`: Wine with additional patches
- `mingw-w64-gcc`: Cross-compiler for Windows
- `nsis`: NSIS installer system
- `wine-mono`: .NET runtime for Wine
- `wine-gecko`: Gecko engine for Wine

### OpenSUSE (Tumbleweed/Leap 15.5+)

```bash
# Install Wine development packages
sudo zypper install wine-devel wine-core wine-desktop

# Install multilib support
sudo zypper install gcc-multilib glibc-devel-32bit

# Install NSIS
sudo zypper install nsis

# Install additional dependencies
sudo zypper install wine-devel-headers wine-devel-libs
```

**Package Details:**
- `wine-devel`: Wine development version
- `wine-core`: Core Wine components
- `wine-desktop`: Wine desktop integration
- `gcc-multilib`: Multilib compiler support
- `glibc-devel-32bit`: 32-bit glibc development
- `nsis`: NSIS installer system
- `wine-devel-headers`: Wine development headers
- `wine-devel-libs`: Wine development libraries

### Gentoo

```bash
# Install Wine development packages
sudo emerge --ask app-emulation/wine

# Install multilib support
sudo emerge --ask sys-devel/gcc[multilib]

# Install NSIS
sudo emerge --ask dev-util/nsis

# Install additional dependencies
sudo emerge --ask app-emulation/wine-staging
```

**Package Details:**
- `app-emulation/wine`: Wine package
- `sys-devel/gcc[multilib]`: GCC with multilib support
- `dev-util/nsis`: NSIS installer system
- `app-emulation/wine-staging`: Wine with additional patches

## Verification

After installation, verify that Wine development tools are available:

```bash
# Check winegcc availability
which winegcc
winegcc --version

# Check wineg++ availability
which wineg++
wineg++ --version

# Check makensis availability
which makensis
makensis /VERSION

# Check wine availability
which wine
wine --version
```

## Troubleshooting

### Common Issues

#### 1. Wine Development Tools Not Found

**Problem**: `winegcc` or `wineg++` not found in PATH

**Solution**:
```bash
# Ubuntu/Debian
sudo apt install wine-development-dev

# Fedora
sudo dnf install wine-devel-headers

# Arch
sudo pacman -S wine

# OpenSUSE
sudo zypper install wine-devel-headers
```

#### 2. 32-bit Compilation Fails

**Problem**: Cannot compile 32-bit components

**Solution**:
```bash
# Ubuntu/Debian
sudo apt install gcc-multilib libc6-dev-i386

# Fedora
sudo dnf install gcc-multilib glibc-devel.i686

# Arch
sudo pacman -S mingw-w64-gcc

# OpenSUSE
sudo zypper install gcc-multilib glibc-devel-32bit
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