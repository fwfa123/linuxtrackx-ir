# Arch Linux Installation Guide for LinuxTrack X-IR

## Overview

This guide provides comprehensive instructions for installing LinuxTrack X-IR on Arch Linux and its derivatives (Manjaro, EndeavourOS, etc.) with full 32-bit wine bridge support. The guide addresses the specific challenges of Arch Linux package management and provides solutions for wine32 integration.

## 🚀 Quick Start (Recommended)

For the easiest installation experience, use our automated build script:

```bash
# Clone the repository
git clone <repository-url>
cd linuxtrackx-ir

# Run the automated build script
./scripts/build_arch_linux.sh
```

This script will:
- Install all required dependencies
- Install wine32 from AUR
- Configure the build for Arch Linux
- Build and install LinuxTrack X-IR
- Verify the installation

## 📦 Installation Methods

### Method 1: Automated Build Script (Recommended)

The automated script handles all the complexity for you:

```bash
# Full installation
./scripts/build_arch_linux.sh

# Install dependencies only
./scripts/build_arch_linux.sh --deps-only

# Install wine32 only
./scripts/build_arch_linux.sh --wine32-only

# Build only (assumes dependencies installed)
./scripts/build_arch_linux.sh --build-only
```

### Method 2: Manual Installation with wine32 Support

For users who prefer manual control:

#### Step 1: Install Dependencies

```bash
# Update system
sudo pacman -Syu

# Install core build tools
sudo pacman -S --needed base-devel autoconf automake libtool

# Install Qt5 dependencies
sudo pacman -S --needed qt5-base qt5-tools qt5-x11extras

# Install libraries
sudo pacman -S --needed opencv libusb mxml libx11 libxrandr

# Install build tools
sudo pacman -S --needed bison flex

# Install 32-bit support
sudo pacman -S --needed lib32-glibc lib32-gcc-libs

# Install Video4Linux
sudo pacman -S --needed v4l-utils
```

#### Step 2: Install AUR Helper (if needed)

```bash
# Install yay (recommended)
sudo pacman -S --needed git base-devel
git clone https://aur.archlinux.org/yay.git
cd yay
makepkg -si
cd ..
rm -rf yay
```

#### Step 3: Install wine32 and NSIS

```bash
# Install wine32 from AUR (essential for 32-bit wine bridge)
yay -S wine32

# Install NSIS
yay -S nsis
```

#### Step 4: Build and Install LinuxTrack X-IR

```bash
# Clone repository
git clone <repository-url>
cd linuxtrackx-ir

# Configure build system
autoreconf -fiv

# Configure with Arch Linux specific options
./configure --prefix=/opt \
            --enable-ltr-32lib-on-x64 \
            --with-wine-libs="-L/usr/lib32/wine/i386-unix" \
            --with-wine64-libs="-L/usr/lib/wine/x86_64-unix"

# Build
make -j$(nproc)

# Install
sudo make install

# Add user to plugdev group
sudo usermod -a -G plugdev $USER
```

### Method 3: Prebuilt Installation

For users who don't need wine bridge functionality:

```bash
./scripts/install/install_arch_prebuilt.sh
```

## 🔧 Wine32 Integration

### Why wine32 is Required

LinuxTrack X-IR's wine bridge components require 32-bit wine libraries to function properly. The standard wine package on Arch Linux doesn't include the necessary 32-bit components, which is why we need the wine32 package from AUR.

### wine32 Installation

The wine32 package provides:
- 32-bit wine libraries in `/usr/lib32/wine/i386-unix`
- 32-bit winegcc compiler
- Compatibility with legacy Windows applications

```bash
# Install wine32 from AUR
yay -S wine32

# Verify installation
ls -la /usr/lib32/wine/i386-unix/
winegcc --version
```

### Wine Bridge Configuration

The build system automatically detects wine32 and configures:
- Wine library paths for Arch Linux
- 32-bit compilation flags
- Proper linking for wine bridge components

## 🎮 Wiimote Support

### Wiimote Integration

LinuxTrack X-IR supports Wiimote input devices through the cwiid library. On Arch Linux, cwiid is available through AUR.

#### Available Packages
- **`cwiid`**: Stable version (svn_history-3, 10 votes)
- **`cwiid-git`**: Latest development version (20220621-1, 28 votes)

#### Installation
```bash
# Install cwiid from AUR
yay -S cwiid

# Alternative: Install latest development version
yay -S cwiid-git
```

#### Verification
```bash
# Check if cwiid is installed
pacman -Q cwiid

# Test Wiimote utilities
wminput --help
wmctrl --help
```

#### Usage
```bash
# Connect Wiimote
wmctrl -c

# Test Wiimote input
wminput -c /etc/cwiid/wminput/default
```

## 🎵 OSC Support

### OSC Integration

LinuxTrack X-IR supports OSC (Open Sound Control) for network-based head tracking data transmission. On Arch Linux, liblo is available through AUR.

#### Available Packages
- **`liblo-ipv6`**: Stable version with IPv6 support (1:0.31-1, 1 vote)
- **`liblo-git`**: Latest development version (0.29.r7.g5e0bda0-2, 0 votes)

#### Installation
```bash
# Install liblo from AUR
yay -S liblo-ipv6

# Alternative: Install latest development version
yay -S liblo-git
```

#### Verification
```bash
# Check if liblo is installed
pacman -Q liblo-ipv6

# Test liblo development files
pkg-config --exists liblo
pkg-config --modversion liblo
```

#### Usage
```bash
# OSC server configuration
# LinuxTrack can send head tracking data via OSC
# Configure OSC settings in ltr_gui

# Example OSC message format:
# /linuxtrack/head/x float
# /linuxtrack/head/y float  
# /linuxtrack/head/z float
# /linuxtrack/head/yaw float
# /linuxtrack/head/pitch float
# /linuxtrack/head/roll float
```

## 🛩️ X-Plane Plugin Support

### X-Plane Plugin Integration

LinuxTrack X-IR supports X-Plane plugin development for head tracking integration with the X-Plane flight simulator. The X-Plane SDK is available from the official developer website.

#### SDK Information
- **Latest Version**: X-Plane SDK 4.1.1 (10/10/2024)
- **Target**: X-Plane 12.1.0 & newer (64-bit only)
- **Download**: [X-Plane SDK Downloads](https://developer.x-plane.com/sdk/plugin-sdk-downloads/)
- **License**: Free for plugin development

#### SDK Features
- **64-bit only**: No 32-bit plugin support
- **Linux compatibility**: No import libraries needed (symbols resolved at runtime)
- **Modern APIs**: XPLMAvionics, XPLMNavigation, XPLMSound, XPLMWeather
- **Plugin packaging**: New schema with ABI-specific directories

#### Installation
```bash
# Method 1: Automated installation (recommended)
./scripts/build_arch_linux.sh

# Method 2: Manual installation
sudo mkdir -p /opt/xplane-sdk
cd /tmp
curl -L -o xplane-sdk.zip "https://developer.x-plane.com/sdk/plugin-sdk-downloads/"
unzip xplane-sdk.zip -d /opt/xplane-sdk

# Set environment variables
echo "export XPLANE_SDK_PATH=/opt/xplane-sdk" | sudo tee -a /etc/profile.d/xplane-sdk.sh
echo "export XPLANE_SDK_INCLUDE=/opt/xplane-sdk/CHeaders" | sudo tee -a /etc/profile.d/xplane-sdk.sh
source /etc/profile.d/xplane-sdk.sh
```

#### Verification
```bash
# Check SDK installation
ls -la /opt/xplane-sdk/CHeaders/

# Test SDK headers
gcc -I/opt/xplane-sdk/CHeaders -c -o /dev/null /opt/xplane-sdk/CHeaders/XPLMPlugin.h

# Check environment variables
echo $XPLANE_SDK_PATH
echo $XPLANE_SDK_INCLUDE
```

#### Plugin Development
```bash
# Configure build with X-Plane SDK
./configure --prefix=/opt \
            --enable-ltr-32lib-on-x64 \
            --with-wine-libs="-L/usr/lib32/wine/i386-unix" \
            --with-xplane-sdk="/opt/xplane-sdk/CHeaders"

# Build with X-Plane plugin support
make -j$(nproc)

# Install
sudo make install
```

#### Plugin Packaging
```bash
# New plugin packaging schema (recommended)
# <plugin name>/<ABI>/<plugin name>.xpl
# where ABI is one of: mac_x64, win_x64, lin_x64

# Example structure:
# linuxtrack-plugin/
# ├── lin_x64/
# │   └── linuxtrack-plugin.xpl
# └── win_x64/
#     └── linuxtrack-plugin.xpl
```

#### X-Plane Version Compatibility
| X-Plane Version | SDK Version | API Level | Notes |
|-----------------|-------------|-----------|-------|
| X-Plane 12.1.0+ | SDK 4.1.1 | XPLM410 | Latest features |
| X-Plane 12.04+ | SDK 4.1.1 | XPLM400 | 64-bit only |
| X-Plane 11.50+ | SDK 4.1.1 | XPLM303 | Legacy support |
| X-Plane 11.20+ | SDK 4.1.1 | XPLM301 | Legacy support |
| X-Plane 11.10+ | SDK 4.1.1 | XPLM300 | Legacy support |
| X-Plane 10.00+ | SDK 4.1.1 | XPLM210 | 32-bit support |
| X-Plane 9.00+ | SDK 4.1.1 | XPLM200 | Legacy support |

#### API Level Definitions
```c
// Define API level for target X-Plane version
#define XPLM410  // X-Plane 12.1.0 & newer (64-bit only)
#define XPLM400  // X-Plane 12.04 & newer (64-bit only)
#define XPLM303  // X-Plane 11.50 & newer (64-bit only)
#define XPLM301  // X-Plane 11.20 & newer (64-bit only)
#define XPLM300  // X-Plane 11.10 & newer (64-bit only)
#define XPLM210  // X-Plane 10.00 & newer (10.20 required for 64-bit)
#define XPLM200  // X-Plane 9.00 & newer
// No definition - X-Plane 6.70 & newer
```

## 🎯 Feature Support Matrix

| Feature | Status | Notes |
|---------|--------|-------|
| **Webcam support** | ✅ Yes | Full support with OpenCV |
| **Wiimote support** | ✅ Yes | Available via AUR (cwiid or cwiid-git) |
| **TrackIR support** | ✅ Yes | Full support with libusb |
| **Facetracker support** | ✅ Yes | Full support with OpenCV |
| **XPlane plugin** | ✅ Yes | Available via X-Plane SDK 4.1.1 |
| **Mickey** | ✅ Yes | Full support |
| **Wine plugin** | ✅ Yes | With wine32 from AUR |
| **OSC support** | ✅ Yes | Available via AUR (liblo-ipv6 or liblo-git) |
| **PIE support** | ✅ Yes | Security feature enabled |

## 🔍 Troubleshooting

### Common Issues

#### Issue: "wine32 not found"
**Symptoms**: Build fails with wine-related errors
**Solution**:
```bash
# Install wine32 from AUR
yay -S wine32

# Verify installation
ls -la /usr/lib32/wine/i386-unix/
```

#### Issue: "winegcc not found"
**Symptoms**: Build fails during wine bridge compilation
**Solution**:
```bash
# Ensure wine32 is properly installed
pacman -Q wine32

# Check winegcc availability
which winegcc
winegcc --version
```

#### Issue: "c000007b error"
**Symptoms**: Wine bridge components fail to run
**Solution**:
```bash
# Reinstall wine32
yay -R wine32
yay -S wine32

# Verify wine32 libraries
ls -la /usr/lib32/wine/i386-unix/
```

#### Issue: "NSIS not found"
**Symptoms**: Build fails during installer creation
**Solution**:
```bash
# Install NSIS from AUR
yay -S nsis

# Verify installation
makensis /VERSION
```

#### Issue: "Package conflicts"
**Symptoms**: pacman reports package conflicts
**Solution**:
```bash
# Install packages individually to identify conflicts
sudo pacman -S --needed package-name

# Use --force if necessary (use with caution)
sudo pacman -S --needed --force package-name
```

#### Issue: "AUR build failures"
**Symptoms**: yay/paru fails to build AUR packages
**Solution**:
```bash
# Update system first
sudo pacman -Syu

# Install base-devel
sudo pacman -S --needed base-devel

# Try alternative AUR helper
sudo pacman -S --needed git base-devel
git clone https://aur.archlinux.org/paru.git
cd paru && makepkg -si
```

### Verification Commands

After installation, verify all components:

```bash
# Check wine32 installation
pacman -Q wine32
ls -la /usr/lib32/wine/i386-unix/

# Check winegcc
winegcc --version

# Check NSIS
makensis /VERSION

# Check Qt5 tools
qmake --version
qhelpgenerator --help

# Check libraries
pkg-config --exists opencv4
pkg-config --exists libusb-1.0
pkg-config --exists mxml

# Test LinuxTrack installation
ltr_gui
```

## 🛠️ Advanced Configuration

### Custom Wine Paths

If you have wine installed in a custom location:

```bash
./configure --prefix=/opt \
            --with-wine-libs="-L/custom/path/to/wine32" \
            --with-wine64-libs="-L/custom/path/to/wine64"
```

### Build Options

Available configure options:

```bash
./configure --help | grep -E "(wine|32|64)"
```

Common options:
- `--enable-ltr-32lib-on-x64`: Enable 32-bit library on 64-bit host
- `--with-wine-libs`: Specify 32-bit wine libraries path
- `--with-wine64-libs`: Specify 64-bit wine libraries path
- `--disable-pie`: Disable Position Independent Executable

### Environment Variables

Set these for custom builds:

```bash
export CFLAGS="-m32 -O2"
export CXXFLAGS="-m32 -O2"
export LDFLAGS="-m32"
```

## 📚 Package Explanations

### Core Dependencies
- **`base-devel`**: Essential build tools (gcc, make, etc.)
- **`autoconf automake libtool`**: Autotools for build system
- **`bison flex`**: Parser generators

### Qt5 Dependencies
- **`qt5-base`**: Core Qt5 libraries
- **`qt5-tools`**: Qt5 development tools (includes help functionality)
- **`qt5-x11extras`**: X11 integration for Qt5

### Libraries
- **`opencv`**: Computer vision library for webcam and facetracker
- **`libusb`**: USB device access for TrackIR
- **`mxml`**: XML parsing library (equivalent to libmxml)
- **`libx11 libxrandr`**: X11 development libraries
- **`v4l-utils`**: Video4Linux utilities

### 32-bit Support
- **`lib32-glibc lib32-gcc-libs`**: 32-bit libraries for Wine compatibility
- **`wine32`**: 32-bit Wine from AUR (essential for wine bridge)

## 🔄 Alternative Package Managers

### Using paru (Alternative AUR helper)
```bash
# Install paru
sudo pacman -S --needed git base-devel
git clone https://aur.archlinux.org/paru.git
cd paru && makepkg -si

# Install dependencies
paru -S wine32 nsis
```

### Using pacaur (Legacy AUR helper)
```bash
# Install pacaur
sudo pacman -S --needed git base-devel
git clone https://aur.archlinux.org/pacaur.git
cd pacaur && makepkg -si

# Install dependencies
pacaur -S wine32 nsis
```

## 🎮 Usage After Installation

### First Launch
```bash
# Launch the GUI
ltr_gui

# For Wayland users (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui
```

### Wine Bridge Usage
```bash
# Install wine bridge in a wine prefix
WINEPREFIX=~/.wine ./opt/share/linuxtrack/linuxtrack-wine.exe

# For 32-bit applications
WINEPREFIX=~/.wine32 ./opt/share/linuxtrack/linuxtrack-wine.exe
```

### Testing TrackIR
```bash
# Test TrackIR detection
ltr_gui

# Check USB permissions
ls -la /dev/bus/usb/
sudo usermod -a -G plugdev $USER
```

## 📖 Additional Resources

- [Arch Linux Wiki](https://wiki.archlinux.org/)
- [AUR Package Database](https://aur.archlinux.org/)
- [wine32 AUR Package](https://aur.archlinux.org/packages/wine32)
- [LinuxTrack X-IR Documentation](docs/)
- [Build Script Documentation](scripts/build_arch_linux.sh)

## 🆘 Support

If you encounter issues:

1. **Check the troubleshooting section above**
2. **Run our verification scripts**:
   ```bash
   ./scripts/install/verify_installation.sh
   ```
3. **Check system logs** for specific error messages
4. **Report issues** on the LinuxTrack X-IR GitHub repository

## 🎯 Success Indicators

After successful installation, you should see:

- ✅ `ltr_gui` launches without errors
- ✅ Wine bridge components build successfully
- ✅ TrackIR device detection works
- ✅ Webcam support functions properly
- ✅ All Qt5 components load correctly

The wine bridge components should now work properly with 32-bit Windows applications, resolving the c000007b error that was previously encountered on Arch Linux. 