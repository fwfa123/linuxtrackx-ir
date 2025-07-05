# LinuxTrack X-IR - Modern Head Tracking for Linux

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/fwfa123/linuxtrackx-ir)
[![License](https://img.shields.io/badge/license-MIT-blue)](https://github.com/fwfa123/linuxtrackx-ir/blob/master/LICENSE.md)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)](https://github.com/fwfa123/linuxtrackx-ir)

**LinuxTrack X-IR** is a modern fork of the original LinuxTrack project, bringing head tracking to Linux and Windows with updated build systems, Qt5 support, and MinGW cross-compilation. Track your head movement using TrackIR devices, webcams, or specialized hardware and translate that into game input.

## 🔄 Fork Information

### About This Fork
This project is a **fork** of the original LinuxTrack project created by uglyDwarf. The original project was abandoned, and this fork continues development with modern build systems and enhanced compatibility.

**Original Project**: [LinuxTrack by uglyDwarf](https://github.com/uglyDwarf/linuxtrack)  
**This Fork**: LinuxTrack X-IR (focusing on TrackIR compatibility and modern Linux support)

### Key Differences from Original
- ✅ **Qt5 Modernization**: Updated from Qt4 to Qt5 for current Linux distributions
- ✅ **MinGW Cross-Compilation**: Native Windows binary support (no Wine required)
- ✅ **Modern Build System**: Updated autotools and CMake support
- ✅ **Enhanced Security**: PIE, stack protector, and fortify source enabled by default
- ✅ **TrackIR Focus**: Primary focus on TrackIR 4 & 5 compatibility
- ✅ **Active Development**: Regular updates and community support

## 🙏 Acknowledgments

### Original Authors
This project would not exist without the incredible work of the original LinuxTrack developers:

- **uglyDwarf** - Original creator and main developer of LinuxTrack
- **Tulthix** - Early contributor to the project
- **f.josef** - Main developer and maintainer (302 commits)
- **Michal Navratil** - Significant contributor (20 commits)

**Original Project**: [LinuxTrack on GitHub](https://github.com/uglyDwarf/linuxtrack)

### Current Maintainer
This fork is maintained by **fwfa123** (61 commits), a developer with limited C/C++ experience (primarily Python and JavaScript background). The modernization work was made possible through the assistance of **Cursor AI**, which helped navigate the complex build system updates and Qt4 to Qt5 migration challenges.

### Additional Contributors
- **raven** - Contributor to original linuxtrack repository (4 commits)
- **aledin** - Contributor (2 commits)
- **Cursor AI** - Assisted with Qt5 modernization and build system updates

## 🎯 Project Scope & Focus

### Primary Goals
- **TrackIR Support**: Full compatibility with TrackIR 4 & 5 devices
- **Modern Linux Compatibility**: Updated build system for current distributions
- **Qt5 Migration**: Modernized GUI framework support
- **MinGW Integration**: Improved Windows compatibility layer

### Limited/Untested Features
⚠️ **Important**: The following features were **not tested** and are **not current development priorities**:

- **Wii Remote Support**: Wii tracking functionality may not work
- **Webcam/Face Tracking**: Optical tracking features untested
- **macOS Support**: Mac compatibility not verified
- **X-Plane Plugin**: Requires X-Plane SDK (see [XPlane Plugin Support](#xplane-plugin-support-optional))

### What This Means
- **TrackIR users**: Full support and testing focus
- **Other tracking methods**: May work but not guaranteed
- **Mac users**: Use at your own risk
- **Contributors**: Welcome to test and improve untested features

## 🚀 Quick Start

### For Debian / Ubuntu / MX Linux Users
```bash
# Install dependencies
sudo apt install -y build-essential autoconf automake libtool qtbase5-dev qttools5-dev-tools libqt5x11extras5-dev libqt5help5-dev libopencv-dev libusb-1.0-0-dev libmxml-dev libx11-dev libxrandr-dev mingw-w64 bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev

### For Fedora / RHEL / CentOS Users
```bash
# Install dependencies
sudo dnf install -y gcc gcc-c++ make autoconf automake libtool qt5-qtbase-devel qt5-qttools-devel qt5-qtx11extras-devel opencv-devel libusb1-devel libmxml-devel libX11-devel libXrandr-devel mingw64-gcc mingw64-gcc-c++ bison flex nsis glibc-devel.i686 libstdc++-devel.i686 v4l-utils-devel

### For Arch Linux / Manjaro Users
```bash
# Install dependencies
sudo pacman -S --needed base-devel autoconf automake libtool qt5-base qt5-tools qt5-x11extras qt5-help opencv libusb libmxml libx11 libxrandr mingw-w64-gcc bison flex nsis lib32-glibc lib32-gcc-libs v4l-utils

# Build LinuxTrack
git clone <repository-url>
cd linuxtrackx-ir
autoreconf -fiv
./configure --prefix=/opt
make -j$(nproc)
sudo make install
```

## 🎉 What's New

### 🎉 MinGW Cross-Compilation Support (Phase 1 Complete)
LinuxTrack now supports building Windows compatibility components using MinGW, eliminating the need for wine-devel packages:

- ✅ **Simplified Dependencies**: No wine-devel required
- ✅ **Smaller Footprint**: Fewer packages to install
- ✅ **Better Compatibility**: Standard Windows PE binaries
- ✅ **Faster Builds**: No Wine runtime overhead
- ✅ **NSIS Installers Updated**: All installer scripts now use native .exe/.dll files

### Why MinGW is the Best Approach
**MinGW vs winegcc Comparison:**

| Aspect | MinGW (Recommended) | winegcc (Legacy) |
|--------|-------------------|------------------|
| **Binary Format** | Native Windows PE (.exe/.dll) | Linux ELF with Wine wrapper (.exe.so/.dll.so) |
| **Portability** | Works on Windows and Wine | Only works with Wine |
| **Performance** | Native Windows performance | Wine runtime overhead |
| **Dependencies** | Minimal (mingw-w64 only) | Heavy (wine-devel packages, not supported) |
| **Future-Proof** | ✅ Industry standard | ❌ Wine-specific |
| **CI/CD** | Easy automated builds | Complex Wine environment |

**Bottom Line**: MinGW produces genuine Windows binaries that work everywhere. winegcc and wine-devel are no longer supported in this project.

### 🚧 Build System Modernization Progress
- ✅ **Phase 1 Complete**: NSIS installer scripts updated for MinGW
- ✅ **Phase 2 Complete**: Makefile.am files updated for native Windows binaries
- ✅ **Phase 3 Complete**: Documentation and build scripts updated for MinGW
- 🔄 **Phase 4**: Final testing and validation

### Built Windows Components
- **NPClient.dll** / **NPClient64.dll** - TrackIR API compatibility (32/64-bit)
- **FreeTrackClient.dll** - FreeTrack API compatibility
- **Testing Tools** - Tester.exe, Tester64.exe for validation
- **Utilities** - Controller.exe for hotkeys, check_data.exe for validation

## 🎮 Gaming Compatibility

LinuxTrack works with games and simulators that support:
- **TrackIR** protocol (most flight/racing simulators)
- **FreeTrack** protocol (alternative tracking standard)
- **OpenTrack** compatibility
- **Custom applications** via LinuxTrack API

### Popular Supported Games
- Microsoft Flight Simulator (via Wine)
- DCS World
- IL-2 Sturmovik series
- War Thunder
- Elite Dangerous
- Euro Truck Simulator 2
- American Truck Simulator
- **X-Plane** (requires XPlane SDK for plugin support)

## 🔧 Build Requirements

### Core Dependencies
- **Build Tools**: gcc, g++, make, autoconf, automake, libtool, bison, flex
- **GUI Framework**: Qt5 (qtbase5-dev, qttools5-dev-tools, libqt5x11extras5-dev, libqt5help5-dev)
- **Computer Vision**: OpenCV (libopencv-dev)
- **Hardware Access**: libusb-1.0-dev, libmxml-dev
- **Video Support**: V4L2 development headers (libv4l-dev / v4l-utils-devel / v4l-utils)
- **X11 Support**: libx11-dev, libxrandr-dev

### Windows Compatibility (MinGW Only)
- **MinGW** (required): mingw-w64, gcc-mingw-w64, g++-mingw-w64

### Security Features (Enabled by Default)
- **PIE (Position Independent Executable)**: Enabled by default for native Linux builds
- **Stack Protector**: Built-in buffer overflow protection
- **Fortify Source**: Enhanced security checks for standard library functions

### XPlane Plugin Support (Optional)
- **XPlane SDK** (required for X-Plane plugin): Download from [Laminar Research Developer Site](https://developer.x-plane.com/sdk/plugin-sdk-downloads/)
- **Default location**: `/usr/include/xplane_sdk/`
- **Custom location**: Use `--with-xplane-sdk=/path/to/sdk` during configure

**Note**: Without the XPlane SDK, configure will show "XPlane plugin... no". The SDK is only needed if you want to build X-Plane compatibility plugins.

## 📖 Documentation

### Fork Information
- **[Fork Notice](FORK_NOTICE.md)** - Detailed explanation of the fork relationship
- **[Changelog](CHANGELOG.md)** - What's new and different from the original project
- **[Contributors](AUTHORS)** - Complete list of original and fork contributors

### Build Guides
- **[MX Linux Build Guide](docs/MX_LINUX_BUILD_GUIDE.md)** - Detailed guide for MX Linux users
- **[Wine Bridge Guide](docs/WINE_BRIDGE_BUILD_GUIDE.md)** - Windows compatibility details
- **[Qt4 to Qt5 Migration](docs/QT4_TO_QT5_MIGRATION.md)** - Technical migration notes
- **[Distribution Support](#-quick-start)** - Build instructions for Debian, Ubuntu, Fedora, and Arch Linux

## 🏗️ Build Process

### Simple Build
```bash
# Clean previous builds (if any)
make distclean 2>/dev/null || true

# Regenerate build system
autoreconf -fiv

# Configure (PIE enabled by default for security)
./configure --prefix=/opt

# Build
make -j$(nproc)

# Install
sudo make install
sudo usermod -a -G plugdev $USER  # Add user to required group
```

### Build Options
```bash
# Debug build
./configure --prefix=/opt --enable-debug

# Disable PIE (not recommended for security)
./configure --prefix=/opt --disable-pie

# Disable Windows components
./configure --prefix=/opt --disable-wine-bridge

# Custom Qt5 location
./configure --prefix=/opt QMAKE=/usr/bin/qmake-qt5

# Enable XPlane plugin support
./configure --prefix=/opt --with-xplane-sdk=/usr/include/xplane_sdk
```

## 🎯 Supported Hardware

### USB Devices
- **TrackIR 4 & 5** - Full support via reverse engineering
- **PlayStation Eye** - USB camera with IR filter removal
- **Generic USB cameras** - For optical tracking
- **TrackHat sensors** - Specialized tracking hardware

### DIY Solutions
- **Webcam + IR LEDs** - Build your own IR tracking setup
- **Wii Remote** - Nintendo Wiimote tracking
- **FaceTrackNoIR compatible** - Face detection tracking

## ⚙️ Configuration

### Post-Installation Setup
1. **Add user to groups**:
   ```bash
   sudo usermod -a -G plugdev $USER
   ```
   Log out and back in for changes to take effect.

2. **Launch GUI**:
   ```bash
   ltr_gui
   ```

3. **Configure tracking**:
   - Select your tracking device
   - Calibrate tracking points
   - Set up game profiles
   - Test tracking functionality

## 🔍 Troubleshooting

### Common Issues

| Problem | Solution |
|---------|----------|
| `aclocal-1.17: command not found` | Install autoconf: `sudo apt install autoconf` (Debian/Ubuntu) or `sudo dnf install autoconf` (Fedora) or `sudo pacman -S autoconf` (Arch) |
| `flex: command not found` | Install flex: `sudo apt install flex` (Debian/Ubuntu) or `sudo dnf install flex` (Fedora) or `sudo pacman -S flex` (Arch) |
| `makensis: command not found` | Install NSIS: `sudo apt install nsis` (Debian/Ubuntu) or `sudo dnf install nsis` (Fedora) or `sudo pacman -S nsis` (Arch) |
| `winegcc: command not found` | MinGW is required. Install with: `sudo apt install mingw-w64` (Debian/Ubuntu) or `sudo dnf install mingw64-gcc` (Fedora) or `sudo pacman -S mingw-w64-gcc` (Arch) |
| `bits/libc-header-start.h: No such file or directory` | 32-bit development headers missing. Install: `sudo apt install gcc-multilib libc6-dev-i386` (Debian/Ubuntu/MX) or `sudo dnf install glibc-devel.i686 libstdc++-devel.i686` (Fedora/RHEL) or `sudo pacman -S lib32-glibc lib32-gcc-libs` (Arch) |
| `libv4l2.h: No such file or directory` | V4L2 development headers missing. Install: `sudo apt install libv4l-dev` (Debian/Ubuntu/MX) or `sudo dnf install v4l-utils-devel` (Fedora/RHEL) or `sudo pacman -S v4l-utils` (Arch) |
| `Unknown module(s) in QT: opengl help` | Qt5 help module missing. Install: `sudo apt install libqt5help5-dev` (Debian/Ubuntu/MX) or `sudo dnf install qt5-qtbase-devel` (Fedora) or `sudo pacman -S qt5-help` (Arch) |
| Qt5 not found | Install Qt5: `sudo apt install qtbase5-dev qttools5-dev-tools libqt5x11extras5-dev libqt5help5-dev` (Debian/Ubuntu/MX) or `sudo dnf install qt5-qtbase-devel qt5-qttools-devel qt5-qtx11extras-devel` (Fedora) or `sudo pacman -S qt5-base qt5-tools qt5-x11extras qt5-help` (Arch) |
| Permission denied on device | Add user to plugdev group |
| No tracking detected | Check device connection and driver installation |
| PIE/relocation linker errors | PIE is enabled by default. If you encounter issues, use: `./configure --disable-pie` |
| XPlane plugin shows "no" | Install XPlane SDK from [Laminar Research](https://developer.x-plane.com/sdk/plugin-sdk-downloads/) or use `--with-xplane-sdk=/path/to/sdk` |

### Getting Help
1. **Check the docs**: Start with the [Quick Start](#-quick-start) section for your distribution
2. **Verify installation**: Run `ltr_gui` to test basic functionality
3. **Check logs**: Look for error messages in terminal output
4. **Hardware test**: Verify USB devices are detected
5. **Distribution guides**: See the `docs/` directory for detailed guides

## 📁 Project Structure

```
linuxtrack-clean-june14/
├── src/                    # Main source code
│   ├── qt_gui/            # Qt5 GUI application
│   ├── wine_bridge/       # Windows compatibility layer
│   ├── mickey/            # TrackIR device support
│   └── ...                # Core tracking libraries
├── docs/                  # Documentation
├── dev-scripts/           # Build automation scripts
└── README.md              # This file
```

## 🤝 Contributing

LinuxTrack is an open-source project that welcomes contributions:

1. **Bug Reports**: Use the issue tracker for bugs and feature requests
2. **Code Contributions**: Submit pull requests with improvements
3. **Documentation**: Help improve our guides and documentation
4. **Testing**: Test on different distributions and hardware setups

### Development Setup
```bash
# Clone repository
git clone <repository-url>
cd linuxtrack-clean-june14

# Create development build
./configure --prefix=/opt --enable-debug
make -j$(nproc)
```

## 📜 License

LinuxTrack X-IR is released under the MIT License, maintaining compatibility with the original project's licensing. See the [LICENSE.md](LICENSE.md) file for details.

**Note**: This fork maintains the same MIT license as the original LinuxTrack project to ensure compatibility and respect for the original authors' licensing choice.

## 🌐 Links

- **Original Project**: [LinuxTrack by uglyDwarf](https://github.com/uglyDwarf/linuxtrack)
- **This Fork**: [LinuxTrack X-IR](https://github.com/fwfa123/linuxtrackx-ir)
- **Community**: [OpenTrack Community](https://github.com/opentrack/opentrack) (compatible project)
- **Documentation**: See the `docs/` directory for detailed guides

## 🙏 Acknowledgments

- **Original Authors**: uglyDwarf, Tulthix, f.josef, Michal Navratil, and the original LinuxTrack team
- **Community Contributors**: All developers who have contributed code and testing
- **Reverse Engineering**: TrackIR protocol reverse engineering efforts
- **Hardware Support**: Community-driven hardware compatibility testing
- **AI Assistance**: Cursor AI for helping with build system modernization
- **Fork Contributors**: All contributors to this fork who continue the project's development

---

**Ready to start tracking?** Follow the [Quick Start](#-quick-start) section for your distribution!

---

## 🔄 Fork Notice

This project is a fork of the original LinuxTrack project. The original project was abandoned, and this fork continues development with modern build systems and enhanced compatibility. We maintain full attribution to the original authors and respect their contributions to the open-source community.

**Original Project**: [LinuxTrack by uglyDwarf](https://github.com/uglyDwarf/linuxtrack)  
**License**: MIT License (same as original)  
**Fork Purpose**: Modernization and continued development

## 🔍 Comprehensive Dependency Analysis

### Dependency Verification Checklist

To ensure you have all required dependencies, you can run these verification commands on your system:

#### Core Build Tools
```bash
# Verify build tools
which gcc g++ make autoconf automake libtool bison flex pkg-config
```

#### Qt5 Dependencies (Critical)
```bash
# Verify Qt5 installation
qmake-qt5 -query QT_VERSION
pkg-config --exists Qt5Core Qt5Widgets Qt5OpenGL Qt5Help Qt5Network Qt5X11Extras
```

#### Library Dependencies
```bash
# Verify core libraries
pkg-config --exists opencv4
pkg-config --exists libusb-1.0
pkg-config --exists mxml
pkg-config --exists liblo  # Optional: OSC support
pkg-config --exists libcwiid  # Optional: Wii support
```

#### Development Headers
```bash
# Verify development headers
ls /usr/include/qt5/QtCore/QtCore
ls /usr/include/qt5/QtWidgets/QtWidgets
ls /usr/include/qt5/QtOpenGL/QtOpenGL
ls /usr/include/qt5/QtHelp/QtHelp
ls /usr/include/qt5/QtNetwork/QtNetwork
ls /usr/include/qt5/QtX11Extras/QtX11Extras
ls /usr/include/opencv4/opencv2/opencv.hpp
ls /usr/include/libusb-1.0/libusb.h
ls /usr/include/mxml.h
ls /usr/include/libv4l2.h
ls /usr/include/linux/videodev2.h
ls /usr/include/linux/uinput.h
```

#### X11 Dependencies
```bash
# Verify X11 development
pkg-config --exists x11 xrandr
ls /usr/include/X11/Xlib.h
ls /usr/include/X11/extensions/Xrandr.h
```

#### MinGW Dependencies (Windows Components)
```bash
# Verify MinGW installation
which i686-w64-mingw32-gcc x86_64-w64-mingw32-gcc
which i686-w64-mingw32-g++ x86_64-w64-mingw32-g++
which makensis
```

#### 32-bit Development (Optional)
```bash
# Verify 32-bit development headers
ls /usr/include/i386-linux-gnu/bits/libc-header-start.h 2>/dev/null || echo "32-bit headers not found"
```

### Complete Dependency Matrix

| Component | Debian/Ubuntu/MX | Fedora/RHEL | Arch Linux | Purpose |
|-----------|------------------|-------------|------------|---------|
| **Build Tools** |
| gcc, g++ | build-essential | gcc gcc-c++ | base-devel | C/C++ compilation |
| make | build-essential | make | base-devel | Build system |
| autoconf | autoconf | autoconf | autoconf | Configure script generation |
| automake | automake | automake | automake | Makefile generation |
| libtool | libtool | libtool | libtool | Library building |
| bison | bison | bison | bison | Parser generator |
| flex | flex | flex | flex | Lexical analyzer |
| pkg-config | pkg-config | pkgconfig | pkgconf | Package configuration |
| **Qt5 Framework** |
| Qt5 Core | qtbase5-dev | qt5-qtbase-devel | qt5-base | Core Qt functionality |
| Qt5 Widgets | qtbase5-dev | qt5-qtbase-devel | qt5-base | GUI widgets |
| Qt5 OpenGL | qtbase5-dev | qt5-qtbase-devel | qt5-base | OpenGL integration |
| Qt5 Help | libqt5help5-dev | qt5-qtbase-devel | qt5-help | Help system |
| Qt5 Network | qtbase5-dev | qt5-qtbase-devel | qt5-base | Network support |
| Qt5 X11Extras | libqt5x11extras5-dev | qt5-qtx11extras-devel | qt5-x11extras | X11 integration |
| Qt5 Tools | qttools5-dev-tools | qt5-qttools-devel | qt5-tools | Qt development tools |
| **Computer Vision** |
| OpenCV | libopencv-dev | opencv-devel | opencv | Face tracking, image processing |
| **Hardware Access** |
| libusb | libusb-1.0-0-dev | libusb1-devel | libusb | USB device access |
| mxml | libmxml-dev | libmxml-devel | libmxml | XML parsing |
| **Video Support** |
| V4L2 | libv4l-dev | v4l-utils-devel | v4l-utils | Video4Linux2 support |
| **X11 Support** |
| X11 | libx11-dev | libX11-devel | libx11 | X11 window system |
| Xrandr | libxrandr-dev | libXrandr-devel | libxrandr | X11 resolution management |
| **Windows Compatibility** |
| MinGW 32-bit | mingw-w64 | mingw64-gcc | mingw-w64-gcc | Windows 32-bit compilation |
| MinGW 64-bit | mingw-w64 | mingw64-gcc-c++ | mingw-w64-gcc | Windows 64-bit compilation |
| NSIS | nsis | nsis | nsis | Windows installer creation |
| **32-bit Development** |
| 32-bit libc | libc6-dev-i386 | glibc-devel.i686 | lib32-glibc | 32-bit library support |
| 32-bit gcc | gcc-multilib | libstdc++-devel.i686 | lib32-gcc-libs | 32-bit compilation |
| **Optional Features** |
| liblo | liblo-dev | liblo-devel | liblo | OSC protocol support |
| libcwiid | libcwiid-dev | libcwiid-devel | libcwiid | Wii remote support |
| libcrypto | libssl-dev | openssl-devel | openssl | Cryptographic functions |

### Dependency Verification Script

Create a script to verify all dependencies:

```bash
#!/bin/bash
# dependency_check.sh - Verify all LinuxTrack dependencies

echo "=== LinuxTrack Dependency Check ==="
echo

# Build tools
echo "Checking build tools..."
for tool in gcc g++ make autoconf automake libtool bison flex pkg-config; do
    if command -v $tool >/dev/null 2>&1; then
        echo "✓ $tool found"
    else
        echo "✗ $tool missing"
    fi
done
echo

# Qt5 verification
echo "Checking Qt5..."
if command -v qmake-qt5 >/dev/null 2>&1; then
    echo "✓ qmake-qt5 found: $(qmake-qt5 -query QT_VERSION)"
else
    echo "✗ qmake-qt5 missing"
fi

# Qt5 modules
for module in Qt5Core Qt5Widgets Qt5OpenGL Qt5Help Qt5Network Qt5X11Extras; do
    if pkg-config --exists $module 2>/dev/null; then
        echo "✓ $module found"
    else
        echo "✗ $module missing"
    fi
done
echo

# Libraries
echo "Checking libraries..."
for lib in opencv4 libusb-1.0 mxml; do
    if pkg-config --exists $lib 2>/dev/null; then
        echo "✓ $lib found"
    else
        echo "✗ $lib missing"
    fi
done
echo

# MinGW
echo "Checking MinGW..."
for mingw in i686-w64-mingw32-gcc x86_64-w64-mingw32-gcc; do
    if command -v $mingw >/dev/null 2>&1; then
        echo "✓ $mingw found"
    else
        echo "✗ $mingw missing"
    fi
done

if command -v makensis >/dev/null 2>&1; then
    echo "✓ makensis found"
else
    echo "✗ makensis missing"
fi
echo

echo "=== Check Complete ==="
```

Save this as `dependency_check.sh`, make it executable with `chmod +x dependency_check.sh`, and run it to verify your system has all required dependencies.
