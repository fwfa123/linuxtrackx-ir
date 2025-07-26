# LinuxTrack X-IR v0.99.21 - Modern Head Tracking for Linux

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/fwfa123/linuxtrackx-ir)
[![License](https://img.shields.io/badge/license-MIT-blue)](https://github.com/fwfa123/linuxtrackx-ir/blob/master/LICENSE.md)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)](https://github.com/fwfa123/linuxtrackx-ir)

**LinuxTrack X-IR** is a modern fork of the original LinuxTrack project, bringing head tracking to Linux and Windows with updated build systems, Qt5 support, and Wine compatibility. Track your head movement using TrackIR devices, webcams, or specialized hardware and translate that into game input.

## 🔄 Fork Information

### About This Fork
This project is a **fork** of the original LinuxTrack project created by uglyDwarf. The original project has slowed, and this fork continues development with modern build systems.

**Original Project**: [LinuxTrack by uglyDwarf](https://github.com/uglyDwarf/linuxtrack)  
**This Fork**: LinuxTrack X-IR (focusing on TrackIR compatibility and modern Linux support)

### Key Differences from Original
- ✅ **Qt5 Modernization**: Updated from Qt4 to Qt5 for current Linux distributions
- ✅ **Wine Compatibility**: Wine-based Windows binary support using winegcc
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
- **Wine Integration**: Wine-based Windows compatibility layer

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

### **Recommended: AppImage Installation**
For most users, we recommend using the **AppImage** for the easiest installation experience. See the [AppImage Installation](#-appimage-installation-recommended) section below.

### **Alternative: Build from Source**
If you prefer to build from source or need custom configurations, follow the distribution-specific instructions below.

### For Debian / Ubuntu / MX Linux Users
```bash
# Install dependencies
sudo apt install -y build-essential autoconf automake libtool qtbase5-dev qttools5-dev-tools qttools5-dev libqt5x11extras5-dev libopencv-dev libusb-1.0-0-dev libmxml-dev libx11-dev libxrandr-dev bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev liblo7 liblo-dev libcwiid1 libcwiid-dev wine-staging winetricks

# Build LinuxTrack
git clone <repository-url>
cd linuxtrackx-ir

# Install Wine development tools (recommended method)
cd scripts/dev
./wine_dev_setup.sh
cd ../..

# Build and install
autoreconf -fiv
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install

# Add user to required group
sudo usermod -a -G plugdev $USER

**Note**: We use `/usr/local` instead of `/opt` to ensure the application appears in your desktop launcher. `/usr/local/bin` is in the default PATH, while `/opt/bin` is not.

# Optional: Install X-Plane SDK for plugin support
# Download from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Extract and install to /usr/include/xplane_sdk/
```

### For Fedora / RHEL / CentOS Users
```bash
# Install dependencies
sudo dnf install -y gcc gcc-c++ make autoconf automake libtool qt5-qtbase-devel qt5-qttools-devel qttools5-dev qt5-qtx11extras-devel opencv-devel libusb1-devel libmxml-devel libX11-devel libXrandr-devel bison flex nsis glibc-devel.i686 libstdc++-devel.i686 v4l-utils-devel wine-staging winetricks

# Build LinuxTrack
git clone <repository-url>
cd linuxtrackx-ir

# Install Wine development tools (recommended method)
cd scripts/dev
./wine_dev_setup.sh
cd ../..

# Build and install
autoreconf -fiv
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install

# Add user to required group
sudo usermod -a -G plugdev $USER

# Optional: Install X-Plane SDK for plugin support
# Download from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Extract and install to /usr/include/xplane_sdk/

# Install Wine development tools (if not using the setup script)
sudo dnf install -y wine-devel wine-tools

### For Arch Linux / Manjaro Users

#### **Recommended: Prebuilt Installation (No Wine Development Tools)**
```bash
# One-command installation (no Wine development tools required)
./scripts/install/install_arch_prebuilt.sh
```

#### **Alternative: Standard Installation (Requires Wine Development Tools)**
```bash
# Install dependencies
sudo pacman -S --needed base-devel autoconf automake libtool qt5-base qt5-tools qt5-x11extras opencv libusb mxml libx11 libxrandr bison flex lib32-glibc lib32-gcc-libs v4l-utils wine wine-mono wine-gecko wine-staging winetricks liblo libcwiid

# Build LinuxTrack
git clone <repository-url>
cd linuxtrackx-ir

# Install NSIS (required for Windows compatibility components)
# If you encounter issues with the AUR package, use our helper script:
./scripts/install/install_nsis_arch.sh

# Build and install
autoreconf -fiv
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install

# Add user to required group
sudo usermod -a -G plugdev $USER

# Optional: Install X-Plane SDK for plugin support
# Download from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Extract and install to /usr/include/xplane_sdk/

# Install Wine development tools (if not using the setup script)
sudo pacman -S wine-staging

**Note**: 
- **Prebuilt installation is recommended** for Arch Linux to avoid Wine development tool issues
- **`qt5-help`** is not needed - Qt5 help functionality is included in `qt5-tools`
- **`libmxml`** is available as **`mxml`** in Arch Linux (extras repository)
- **`nsis`** may have AUR issues - use our dedicated NSIS installation script: `./scripts/install/install_nsis_arch.sh` which provides multiple fallback installation methods.

## 📦 AppImage Installation (Recommended)

### **Easiest Installation Method**
For most users, the **AppImage** is the recommended installation method. It includes all dependencies and works across different Linux distributions:

```bash
# Download and run the latest AppImage
# (AppImage will be available in releases)

# Make executable and run
chmod +x LinuxTrack-X-IR-*.AppImage
./LinuxTrack-X-IR-*.AppImage

# Or integrate with your system
./LinuxTrack-X-IR-*.AppImage --appimage-extract-and-run
```

### **AppImage Benefits**
- ✅ **No dependencies to install** - Everything included
- ✅ **Works on any Linux distribution** - Universal compatibility
- ✅ **Easy updates** - Just download new AppImage
- ✅ **No system modifications** - Runs from any location
- ✅ **Includes Wine components** - TrackIR firmware extraction ready

### **Building Your Own AppImage**
If you want to build the AppImage yourself:

```bash
# Clone repository
git clone <repository-url>
cd linuxtrackx-ir

# Build AppImage (requires Docker or similar isolation)
./scripts/appimage/build_appimage_phase4.sh

# The AppImage will be created in the project root
```

## 🖥️ Display Server Compatibility

### **X11 (Recommended)**
LinuxTrack works best with X11. If you're using X11, simply run:
```bash
ltr_gui
```

### **Wayland Compatibility**
If you're using Wayland, you may need to force X11 compatibility:

```bash
# Force X11 backend for Qt applications
QT_QPA_PLATFORM=xcb ltr_gui

# Or set environment variable permanently
export QT_QPA_PLATFORM=xcb
ltr_gui
```

### **Wayland Best Practices**
- **Use X11 backend**: `QT_QPA_PLATFORM=xcb` ensures compatibility
- **Check your session**: `echo $XDG_SESSION_TYPE` to verify display server
- **Fallback to X11**: If Wayland causes issues, switch to X11 session
- **AppImage compatibility**: AppImage works on both X11 and Wayland with proper environment variables

### **Troubleshooting Display Issues**
```bash
# Check your display server
echo $XDG_SESSION_TYPE

# Force X11 if using Wayland
QT_QPA_PLATFORM=xcb ltr_gui

# Check Qt platform plugins
ls /usr/lib/x86_64-linux-gnu/qt5/plugins/platforms/

# Verify X11 compatibility
xrandr --listmonitors
```

## 🎉 What's New

### 🎉 Wine Compatibility Support (Updated)
LinuxTrack now supports building Windows compatibility components using winegcc for proper Wine integration:

- ✅ **Wine Integration**: Uses winegcc for Wine-compatible binaries
- ✅ **Symlink Support**: Proper symlink creation for Wine environments
- ✅ **Wine Compatibility**: .exe.so and .dll.so files that work with Wine
- ✅ **NSIS Installers**: All installer scripts updated for Wine compatibility
- ✅ **Simplified Dependencies**: Uses standard Wine development tools
- ✅ **Build System Fixed**: Resolved C++ linking issues and NSIS script problems
- ✅ **Automated Setup**: New `scripts/dev/wine_dev_setup.sh` script for easy Wine development environment setup

### 🎉 TrackIR Firmware Extraction Improvements
LinuxTrack now features improved TrackIR firmware extraction with enhanced Wine compatibility:

- ✅ **Simplified Wine Commands**: Direct installer execution without complex registry modifications
- ✅ **Enhanced Wine Architecture**: Uses win64 architecture for better TrackIR installer compatibility
- ✅ **Improved Debug Output**: Reasonable Wine debug level for better troubleshooting
- ✅ **Reliable Extraction**: Successfully extracts TrackIR firmware files (TIRViews.dll, NPClient.dll, etc.)
- ✅ **Clean Installation**: Maintains temporary Wine prefix approach with automatic cleanup

### Why Winegcc is the Best Approach
**Winegcc vs MinGW Comparison:**

| Aspect | winegcc (Recommended) | MinGW (Legacy) |
|--------|---------------------|----------------|
| **Binary Format** | Linux ELF with Wine wrapper (.exe.so/.dll.so) | Native Windows PE (.exe/.dll) |
| **Wine Integration** | Perfect Wine compatibility | May have compatibility issues |
| **Symlink Support** | Native Unix symlinks work | Requires file copying |
| **Dependencies** | Standard (wine-devel packages) | Cross-compilation toolchain |
| **Future-Proof** | ✅ Wine-native approach | ❌ Cross-compilation complexity |
| **CI/CD** | Standard Wine environment | Complex MinGW setup |

**Bottom Line**: winegcc produces Wine-compatible binaries that integrate perfectly with Wine environments. MinGW cross-compilation is no longer supported in this project.

### 🚧 Build System Modernization Progress
- ✅ **Phase 1 Complete**: NSIS installer scripts updated for winegcc
- ✅ **Phase 2 Complete**: Makefile.am files updated for Wine compatibility
- ✅ **Phase 3 Complete**: Documentation and build scripts updated for winegcc
- ✅ **Phase 4 Complete**: C++ linking issues resolved, build system fully functional

### Built Windows Components
- **NPClient.dll.so** / **NPClient64.dll.so** - TrackIR API compatibility (32/64-bit)
- **FreeTrackClient.dll.so** - FreeTrack API compatibility
- **Testing Tools** - Tester.exe.so, Tester64.exe.so for validation
- **Utilities** - Controller.exe.so for hotkeys, check_data.exe.so for validation

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
- **GUI Framework**: Qt5 (qtbase5-dev, qttools5-dev-tools, libqt5x11extras5-dev, qttools5-dev)
- **Computer Vision**: OpenCV (libopencv-dev)
- **Hardware Access**: libusb-1.0-dev, libmxml-dev
- **Video Support**: V4L2 development headers (libv4l-dev / v4l-utils-devel / v4l-utils)
- **X11 Support**: libx11-dev, libxrandr-dev
- **Wine Support**: wine, wine-devel (required for TrackIR firmware extraction and Windows components)

### Windows Compatibility (Wine Only)
- **Wine Development** (required): Use `scripts/dev/wine_dev_setup.sh` for automatic setup
- **Manual Installation**: wine-devel, winegcc, wineg++ (if not using the setup script)

### Security Features (Enabled by Default)
- **PIE (Position Independent Executable)**: Enabled by default for native Linux builds
- **Stack Protector**: Built-in buffer overflow protection
- **Fortify Source**: Enhanced security checks for standard library functions

### XPlane Plugin Support (Optional)
- **XPlane SDK** (required for X-Plane plugin): Download from [Laminar Research Developer Site](https://developer.x-plane.com/sdk/plugin-sdk-downloads/)
- **Default location**: `/usr/include/xplane_sdk/`
- **Custom location**: Use `--with-xplane-sdk=/path/to/sdk` during configure

**Note**: Without the XPlane SDK, configure will show "XPlane plugin... no". The SDK is only needed if you want to build X-Plane compatibility plugins.

### OSC Support (Optional)
- **liblo** (required for OSC support): Lightweight OSC (Open Sound Control) library
- **Installation**: `sudo apt install liblo7 liblo-dev` (Debian/Ubuntu/MX) or `sudo dnf install liblo liblo-devel` (Fedora/RHEL) or `sudo pacman -S liblo` (Arch)
- **Features**: Enables OSC server for real-time tracking data transmission
- **Usage**: Builds `osc_server` binary for sending tracking data via OSC protocol

**Note**: Without liblo, configure will show "OSC support... no". The library is only needed if you want to use OSC for real-time tracking data transmission to other applications.

### Wiimote Support (Optional)
- **libcwiid** (required for Wiimote support): Library to interface with Nintendo Wiimote
- **Installation**: `sudo apt install libcwiid1 libcwiid-dev` (Debian/Ubuntu/MX) or `sudo dnf install libcwiid libcwiid-devel` (Fedora/RHEL) or `sudo pacman -S libcwiid` (Arch)
- **Features**: Enables Wiimote tracking for head movement detection
- **Usage**: Builds `wii_server` binary for Wiimote connection and tracking

**Note**: Without libcwiid, configure will show "Wiimote support... no". The library is only needed if you want to use Nintendo Wiimote for head tracking.

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
./configure --prefix=/usr/local

# Build
make -j$(nproc)

# Install
sudo make install
sudo usermod -a -G plugdev $USER  # Add user to required group
```

### Build Options
```bash
# Debug build
./configure --prefix=/usr/local --enable-debug

# Disable PIE (not recommended for security)
./configure --prefix=/usr/local --disable-pie

# Disable Windows components
./configure --prefix=/usr/local --disable-wine-bridge

# Custom Qt5 location
./configure --prefix=/usr/local QMAKE=/usr/bin/qmake-qt5

# Enable XPlane plugin support
./configure --prefix=/usr/local --with-xplane-sdk=/usr/include/xplane_sdk
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
   # For X11 (recommended)
   ltr_gui
   
   # For Wayland (force X11 compatibility)
   QT_QPA_PLATFORM=xcb ltr_gui
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
| `makensis: command not found` | Install NSIS: `sudo apt install nsis` (Debian/Ubuntu) or `sudo dnf install nsis` (Fedora) or `sudo pacman -S nsis` (Arch) or use `./scripts/install/install_nsis_arch.sh` (Arch Linux AUR issues) |
| `winegcc: command not found` | Wine development tools required. Install with: `sudo apt install wine-devel` (Debian/Ubuntu) or `sudo dnf install wine-devel` (Fedora) or `sudo pacman -S wine` (Arch) |
| `bits/libc-header-start.h: No such file or directory` | 32-bit development headers missing. Install: `sudo apt install gcc-multilib libc6-dev-i386` (Debian/Ubuntu/MX) or `sudo dnf install glibc-devel.i686 libstdc++-devel.i686` (Fedora/RHEL) or `sudo pacman -S lib32-glibc lib32-gcc-libs` (Arch) |
| `libv4l2.h: No such file or directory` | V4L2 development headers missing. Install: `sudo apt install libv4l-dev` (Debian/Ubuntu/MX) or `sudo dnf install v4l-utils-devel` (Fedora/RHEL) or `sudo pacman -S v4l-utils` (Arch) |
| `Unknown module(s) in QT: opengl help` | Qt5 help module missing. Install: `sudo apt install qttools5-dev` (Debian/Ubuntu/MX) or `sudo dnf install qt5-qttools-devel` (Fedora) or `sudo pacman -S qt5-tools` (Arch) |
| Qt5 not found | Install Qt5: `sudo apt install qtbase5-dev qttools5-dev-tools libqt5x11extras5-dev qttools5-dev` (Debian/Ubuntu/MX) or `sudo dnf install qt5-qtbase-devel qt5-qttools-devel qt5-qtx11extras-devel` (Fedora) or `sudo pacman -S qt5-base qt5-tools qt5-x11extras` (Arch) |
| GUI not displaying on Wayland | Force X11 compatibility: `QT_QPA_PLATFORM=xcb ltr_gui` or switch to X11 session |
| Permission denied on device | Add user to plugdev group |
| No tracking detected | Check device connection and driver installation |
| Application not appearing in launcher | **Fixed**: Use `--prefix=/usr/local` instead of `/opt`. If already installed to `/opt`, fix desktop file: `sudo sed -i 's|Exec=ltr_gui|Exec=/opt/bin/ltr_gui|' /usr/share/applications/linuxtrack.desktop && sudo update-desktop-database /usr/share/applications` |
| PIE/relocation linker errors | PIE is enabled by default. If you encounter issues, use: `./configure --disable-pie` |
| XPlane plugin shows "no" | Install XPlane SDK from [Laminar Research](https://developer.x-plane.com/sdk/plugin-sdk-downloads/) or use `--with-xplane-sdk=/path/to/sdk` |
| Firmware extraction fails | **Step 1**: Run `./scripts/wine_check.sh` to diagnose Wine issues. **Step 2**: Install Wine Staging (`sudo apt install wine-staging`) and switch to it (`sudo update-alternatives --config wine`). **Step 3**: Install 32-bit components (`sudo apt install wine32:i386`). **Step 4**: If issues persist, manually install TrackIR with Wine, then use "Browse Directory" option in GUI. |

### Getting Help
1. **Check the docs**: Start with the [Quick Start](#-quick-start) section for your distribution
2. **Try AppImage first**: Use the [AppImage Installation](#-appimage-installation-recommended) for easiest setup
3. **Check display server**: Verify X11/Wayland compatibility with `echo $XDG_SESSION_TYPE`
4. **Verify installation**: Run `ltr_gui` (X11) or `QT_QPA_PLATFORM=xcb ltr_gui` (Wayland) to test basic functionality
5. **Check logs**: Look for error messages in terminal output
6. **Hardware test**: Verify USB devices are detected
7. **Distribution guides**: See the `docs/` directory for detailed guides

## 📁 Project Structure

```
linuxtrackx-ir/
├── src/                    # Main source code
│   ├── qt_gui/            # Qt5 GUI application
│   ├── wine_bridge/       # Windows compatibility layer
│   ├── mickey/            # TrackIR device support
│   └── ...                # Core tracking libraries
├── docs/                  # Documentation
├── scripts/               # Build automation scripts
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
cd linuxtrackx-ir

# Create development build
./configure --prefix=/usr/local --enable-debug
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
ls /usr/include/xplane_sdk/XPLM/XPLMPlugin.h  # Optional: X-Plane support
```

#### Development Headers
```bash
# Verify development headers
ls /usr/include/qt5/QtCore/QtCore
ls /usr/include/qt5/QtWidgets/QtWidgets
ls /usr/include/qt5/QtOpenGL/QtOpenGL
ls /usr/include/qt5/QtHelp/QtHelp
```

#### Wine Dependencies (Windows Components)
```bash
# Verify Wine development tools
which winegcc wineg++
winegcc --version
```

#### 32-bit Development Support
```bash
# Verify 32-bit development headers
ls /usr/include/i386-linux-gnu/bits/libc-header-start.h 2>/dev/null || ls /usr/include/bits/libc-header-start.h 2>/dev/null || echo "32-bit headers not found"
```

### Distribution-Specific Dependencies

#### Debian/Ubuntu/MX Linux
```bash
# Core dependencies
sudo apt install -y build-essential autoconf automake libtool qtbase5-dev qttools5-dev-tools qttools5-dev libqt5x11extras5-dev libopencv-dev libusb-1.0-0-dev libmxml-dev libx11-dev libxrandr-dev bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev wine-staging winetricks liblo7 liblo-dev libcwiid1 libcwiid-dev

# Wine development tools
sudo apt install -y wine-devel wine32:i386
```

#### Fedora/RHEL/CentOS
```bash
# Core dependencies
sudo dnf install -y gcc gcc-c++ make autoconf automake libtool qt5-qtbase-devel qt5-qttools-devel qttools5-dev qt5-qtx11extras-devel opencv-devel libusb1-devel libmxml-devel libX11-devel libXrandr-devel bison flex nsis glibc-devel.i686 libstdc++-devel.i686 v4l-utils-devel wine-staging winetricks liblo liblo-devel libcwiid libcwiid-devel

# Wine development tools
sudo dnf install -y wine-devel wine-tools
```

#### Arch Linux/Manjaro
```bash
# Core dependencies
sudo pacman -S --needed base-devel autoconf automake libtool qt5-base qt5-tools qt5-x11extras opencv libusb mxml libx11 libxrandr bison flex lib32-glibc lib32-gcc-libs v4l-utils wine-staging winetricks liblo libcwiid

# NSIS installation (use our helper script if AUR package fails)
./scripts/install/install_nsis_arch.sh

# Wine development tools (included in wine-staging)
```

### Troubleshooting Dependency Issues

#### Qt5 Issues
```bash
# Check Qt5 installation
qmake-qt5 -query QT_VERSION
pkg-config --modversion Qt5Core

# Reinstall Qt5 if needed
sudo apt install --reinstall qtbase5-dev qttools5-dev-tools libqt5x11extras5-dev qttools5-dev
```

#### Wine Issues
```bash
# Check Wine installation
wine --version
winegcc --version

# Install Wine development tools
sudo apt install wine-devel wine32:i386  # Debian/Ubuntu
sudo dnf install wine-devel wine-tools   # Fedora
sudo pacman -S wine-staging              # Arch
```

#### 32-bit Support Issues
```bash
# Enable 32-bit architecture (Debian/Ubuntu)
sudo dpkg --add-architecture i386
sudo apt update
sudo apt install gcc-multilib libc6-dev-i386

# Install 32-bit libraries (Fedora)
sudo dnf install glibc-devel.i686 libstdc++-devel.i686

# Install 32-bit libraries (Arch)
sudo pacman -S lib32-glibc lib32-gcc-libs
```

#### X-Plane SDK Issues
```bash
# Check X-Plane SDK installation
ls /usr/include/xplane_sdk/XPLM/XPLMPlugin.h

# Manual installation if not found
sudo mkdir -p /usr/include/xplane_sdk
sudo cp -r /path/to/extracted/SDK/CHeaders/* /usr/include/xplane_sdk/

# Verify configure detects X-Plane support
./configure --prefix=/usr/local 2>&1 | grep "XPlane plugin"
```

This comprehensive dependency analysis ensures that all required components are available for building LinuxTrack X-IR with full functionality.
