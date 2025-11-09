# LinuxTrack X-IR - Modern Head Tracking for Linux (Now on GitLab)

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://gitlab.com/fwfa123/linuxtrackx-ir)
[![License](https://img.shields.io/badge/license-MIT-blue)](https://gitlab.com/fwfa123/linuxtrackx-ir/-/blob/main/LICENSE.md)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)](https://gitlab.com/fwfa123/linuxtrackx-ir)

> ⚠️ Repository migrated to GitLab: [GitLab: fwfa123/linuxtrackx-ir](https://gitlab.com/fwfa123/linuxtrackx-ir). All new issues and feature requests should be opened on GitLab.

LinuxTrack X-IR is a modern fork of the original LinuxTrack project, providing enhanced head tracking support for Linux systems with TrackIR devices.

### Why the **X** in the name?  I am concentrating on TrackIR support and I do not 
have a Mac and other items I am not supporting these at this time, although the code 
reamains embedded.

## 🚀 Quick Start

## Wiki
https://gitlab.com/fwfa123/linuxtrackx-ir/-/wikis/home

### **Recommended: AppImage Installation (Easiest)**
```bash
# Download and run the latest AppImage
chmod +x LinuxTrack-X-IR-*.AppImage
./LinuxTrack-X-IR-*.AppImage
```

### **Alternative: Build from Source**
Choose your distribution below for detailed instructions.

# LinuxTrack Build Options Guide

This document provides step-by-step build configurations for LinuxTrack, from minimal TrackIR-only builds to full-featured installations with all optional components.

## Build Philosophy

LinuxTrack uses a **basic build philosophy** where:
- **Core components** (TrackIR + Wine) are enabled by default when dependencies are found
- **Optional components** (webcam, OSC, Wiimote, X-Plane) are disabled by default
- Each build level adds functionality while maintaining compatibility
- **Security features** (PIE, stack protection) are enabled by default

## Security Features

**PIE (Position Independent Executable)** is enabled by default for security:
- Randomizes memory addresses to prevent buffer overflow attacks
- Required by modern Linux distributions
- Only disable if you encounter compatibility issues: `cmake .. -DENABLE_PIE=OFF`

---

## CMake Build Options

LinuxTrack uses CMake for building. Configure your build with CMake options:

### **Core Options**
- `-DCMAKE_INSTALL_PREFIX=PATH` - Installation prefix (default: `/opt`)
- `-DENABLE_WEBCAM=ON` - Enable webcam/face tracking support (disabled by default)
- `-DENABLE_LTR_32LIB_ON_X64=ON` - Enable 32-bit library building for Wine compatibility (enabled by default)
- `-DENABLE_PIE=OFF` - Disable Position Independent Executable (not recommended, enabled by default)

### **Library Configuration**
- `-DENABLE_LDCONFIG=OFF` - Disable automatic ldconfig configuration (useful for packaging systems)

**When to use `-DENABLE_LDCONFIG=OFF`:**
- Building packages for distribution (RPM, DEB, etc.)
- Staging installations with `DESTDIR`
- Systems where package managers handle library paths automatically
- Docker containers or chroot environments
- AppImage builds (self-contained, no system-wide configuration needed)

### **Example Commands**
```bash
# Standard CMake build process
mkdir build && cd build

# Minimal build for packaging
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LDCONFIG=OFF

# Full build with manual library management
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_WEBCAM=ON -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_LDCONFIG=OFF

# Standard user installation
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_WEBCAM=ON -DENABLE_LTR_32LIB_ON_X64=ON

# Configure with CMake (X-Plane SDK path)
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Build and install for any of the examples shown above
cmake --build . -j$(nproc)
sudo cmake --install .
# Later, to uninstall
cd build
sudo make uninstall
```

---

## Level 1: TrackIR Only (Minimal Build)

**Use Case**: Native Linux games and applications only. No Wine/Steam support.

**Functionality**:
- ✅ TrackIR hardware support
- ✅ LinuxTrack server for native games
- ✅ Alternative tracking methods (AntiMicro joystick detection)
- ❌ No Wine bridge support
- ❌ No Steam game compatibility

### Required Libraries

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt update
sudo apt install build-essential cmake pkg-config
sudo apt install libusb-1.0-0-dev zlib1g-dev
sudo apt install bison flex
sudo apt install qtbase5-dev qttools5-dev-tools qttools5-dev libqt5x11extras5-dev
sudo apt install libmxml-dev
sudo apt install libx11-dev libxrandr-dev
sudo apt install libgl1-mesa-dev libglu1-mesa-dev
sudo apt install libqt5opengl5-dev
sudo apt install sqlite3  # Required for Qt help system path fixes
```

#### Arch Linux:
```bash
sudo pacman -S base-devel cmake pkg-config
sudo pacman -S libusb zlib
sudo pacman -S bison flex
sudo pacman -S qt5-base qt5-tools
sudo pacman -S libmxml
sudo pacman -S mesa glu
sudo pacman -S sqlite  # Required for Qt help system path fixes
```

#### Fedora/RHEL:
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake pkg-config
sudo dnf install libusb1-devel zlib-devel
sudo dnf install bison flex
sudo dnf install qt5-qtbase-devel qt5-qmake qt5-qttools-devel
sudo dnf install libmxml-devel
sudo dnf install mesa-libGL-devel mesa-libGLU-devel
sudo dnf install sqlite  # Required for Qt help system path fixes
```

#### **Fedora/RHEL Qt5 Tools PATH Fix (IMPORTANT)**
On Fedora/RHEL systems, Qt5 tools like `qhelpgenerator` are installed in `/usr/lib64/qt5/bin/` which may not be in your default PATH. This can cause build failures at the final step.

**Before building, add Qt5 tools to your PATH:**
```bash
export PATH="/usr/lib64/qt5/bin:$PATH"
```

**To make this permanent, add to your `~/.bashrc`:**
```bash
echo 'export PATH="/usr/lib64/qt5/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

**Verify Qt5 tools are accessible:**
```bash
which qhelpgenerator qmake moc
# Should show: /usr/lib64/qt5/bin/qhelpgenerator, etc.
```
### Build Command
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local

# Build (export PATH for Fedora/RHEL Qt5 tools if needed)
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools on Fedora/RHEL
cmake --build . -j$(nproc)
```

**Note**: Webcam support is disabled by default in minimal builds. To enable webcam support, add `-DENABLE_WEBCAM=ON` to the cmake command.

**Note**: On Fedora/RHEL, Qt tools like `qhelpgenerator` are located in `/usr/lib64/qt5/bin/` and may not be in the default PATH. The export command above adds this directory to your PATH for the build process.

### Install
```bash
sudo cmake --install .
```

**Note**: If you see a warning about `ldconfig` failing, run `sudo ldconfig` manually after installation to update the library cache. For packaging systems or staging installations, use `-DENABLE_LDCONFIG=OFF` during cmake configuration to skip automatic library configuration.

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui NOTE That this may not be needed. As of late I have had sucess simply launching ltr_gui in Wayland.
```

---

## Level 2: TrackIR + Wine64 Support

**Use Case**: Modern 64-bit Wine games and Steam (Proton). No legacy 32-bit Wine support.

**Functionality**:
- ✅ All TrackIR functionality
- ✅ Wine64 bridge for 64-bit games
- ✅ Steam Proton compatibility
- ❌ No 32-bit Wine game support

### Additional Libraries (beyond Level 1)

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt install wine64 wine64-tools
sudo apt install makensis  # For installer creation
```

#### Arch Linux:
```bash
sudo pacman -S wine-staging
sudo pacman -S nsis  # For installer creation
```

#### Fedora/RHEL:
```bash
sudo dnf install wine-devel  # Provides winegcc, wineg++, wrc
sudo dnf install mingw-nsis-base  # Provides makensis for installer creation
```

### Wine Configuration Notes:
- **Wine Bridge Components**: Builds NPClient64.dll.so for 64-bit TrackIR API compatibility
- **Steam Proton**: Compatible with Steam's Proton for Windows games
- **Modern Games**: Supports 64-bit Windows games running under Wine

### Build Command
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local

# Build (export PATH for Fedora/RHEL Qt5 tools if needed)
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools on Fedora/RHEL
cmake --build . -j$(nproc)
```

### Install
```bash
sudo cmake --install .
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui NOTE That this may not be needed. As of late I have had sucess simply launching ltr_gui in Wayland.
```

---

## Level 3: TrackIR + Wine32 + Wine64 (Full Wine Support)

**Use Case**: Complete Wine compatibility for both modern and legacy games.

**Functionality**:
- ✅ All TrackIR functionality
- ✅ Wine64 bridge for 64-bit games
- ✅ Wine32 bridge for 32-bit games
- ✅ Full Steam compatibility
- ✅ Legacy game support

### Additional Libraries (beyond Level 2)

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt install wine wine-staging libwine-dev wine32-tools
sudo apt install gcc-multilib libc6-dev-i386
```

#### Arch Linux:
```bash
sudo pacman -S wine32  # From AUR: yay -S wine32
sudo pacman -S lib32-glibc lib32-gcc-libs
```

#### Fedora/RHEL:
```bash
sudo dnf install wine-devel.i686  # 32-bit Wine development tools
sudo dnf install glibc-devel.i686 gcc.i686
```

### Wine Configuration Notes:
- **Complete Wine Support**: Builds both NPClient.dll.so (32-bit) and NPClient64.dll.so (64-bit)
- **Legacy Game Support**: 32-bit Wine bridge for older Windows games
- **MFC42 Compatibility**: LinuxTrack includes enhanced MFC42 winetricks installation for TrackIR compatibility
- **Steam Integration**: Full compatibility with Steam and Proton

### MFC42 Installation:
After installation, LinuxTrack will automatically handle MFC42 installation via winetricks:
- **Enhanced Debugging**: Comprehensive output for troubleshooting
- **Automatic Fallback**: Falls back to `winetricks vcrun6` if MFC42 fails
- **32-bit Wine Prefix**: Forces `WINEARCH=win32` for optimal compatibility

### Build Command
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake (32-bit library enabled by default)
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON

# Build (export PATH for Fedora/RHEL Qt5 tools if needed)
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools on Fedora/RHEL
cmake --build . -j$(nproc)
```

### Install
```bash
sudo cmake --install .
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui NOTE That this may not be needed. As of late I have had sucess simply launching ltr_gui in Wayland.
```

---

## Level 4: TrackIR + Wine + Webcam Support

**Use Case**: Add webcam-based face tracking as an alternative to TrackIR hardware.

**Functionality**:
- ✅ All previous functionality
- ✅ Webcam face tracking
- ✅ V4L2 webcam support
- ✅ Alternative tracking when TrackIR unavailable

**Note**: Webcam support is **disabled by default** in minimal builds. You must explicitly enable it with `-DENABLE_WEBCAM=ON`.

### Additional Libraries (beyond Level 3)

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt install libv4l-dev v4l-utils
sudo apt install libopencv-dev  # For face tracking
```

#### Arch Linux:
```bash
sudo pacman -S libv4l v4l-utils
sudo pacman -S opencv  # For face tracking
```

#### Fedora/RHEL:
```bash
sudo dnf install libv4l-devel v4l-utils
sudo dnf install opencv-devel  # For face tracking
```

### Build Command
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake (enable webcam support)
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON

# Build (export PATH for Fedora/RHEL Qt5 tools if needed)
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools on Fedora/RHEL
cmake --build . -j$(nproc)
```

### Install
```bash
sudo cmake --install .
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui NOTE That this may not be needed. As of late I have had sucess simply launching ltr_gui in Wayland.
```

---

## Level 5: TrackIR + Wine + Webcam + OSC Support

**Use Case**: Add OSC (Open Sound Control) support for external applications and MIDI controllers.

**Functionality**:
- ✅ All previous functionality
- ✅ OSC protocol support
- ✅ External application integration
- ✅ MIDI controller support

### Additional Libraries (beyond Level 4)

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt install liblo-dev
```

#### Arch Linux:
```bash
sudo pacman -S liblo
```

#### Fedora/RHEL:
```bash
sudo dnf install liblo-devel
```

### Build Command
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake (OSC support auto-enabled if liblo found)
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON

# Build (export PATH for Fedora/RHEL Qt5 tools if needed)
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools on Fedora/RHEL
cmake --build . -j$(nproc)
```

### Install
```bash
sudo cmake --install .
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui NOTE That this may not be needed. As of late I have had sucess simply launching ltr_gui in Wayland.
```

---

## Level 6: TrackIR + Wine + Webcam + OSC + Wiimote Support

**Use Case**: Add Nintendo Wii Remote support as an alternative tracking device.

**Functionality**:
- ✅ All previous functionality
- ✅ Wiimote tracking support
- ✅ Alternative input device
- ✅ Motion control support

### Additional Libraries (beyond Level 5)

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt install libcwiid-dev
```

#### Arch Linux:
```bash
sudo pacman -S libcwiid  # From AUR: yay -S libcwiid
```

#### Fedora/RHEL:
```bash
sudo dnf install libcwiid-devel
```

### Build Command
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake (Wiimote support auto-enabled if libcwiid found)
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON

# Build (export PATH for Fedora/RHEL Qt5 tools if needed)
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools on Fedora/RHEL
cmake --build . -j$(nproc)
```

### Install
```bash
sudo cmake --install .
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui NOTE That this may not be needed. As of late I have had sucess simply launching ltr_gui in Wayland.
```

---

## Level 7: TrackIR + Wine + Webcam + OSC + Wiimote + X-Plane Support

**Use Case**: Complete build with X-Plane flight simulator plugin support.

**Functionality**:
- ✅ All previous functionality
- ✅ X-Plane plugin support
- ✅ Flight simulator integration
- ✅ Complete feature set

### Additional Libraries (beyond Level 6)

#### X-Plane SDK Setup:
```bash
# Download X-Plane SDK from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Extract to: /opt/xplane-sdk/
sudo mkdir -p /opt/xplane-sdk
sudo tar -xzf XPSDK*.tar.gz -C /opt/xplane-sdk/
```

### Build Command
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake (X-Plane SDK path)
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Build (export PATH for Fedora/RHEL Qt5 tools if needed)
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools on Fedora/RHEL
cmake --build . -j$(nproc)
```

### Install
```bash
sudo cmake --install .
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui NOTE That this may not be needed. As of late I have had sucess simply launching ltr_gui in Wayland.
```

---

## Quick Reference: All Libraries Combined

### Debian/Ubuntu/MX Linux (Complete Build):
```bash
sudo apt update
sudo apt install build-essential cmake pkg-config
sudo apt install libusb-1.0-0-dev zlib1g-dev bison flex
sudo apt install qt5-qmake qtbase5-dev qttools5-dev-tools qttools5-dev libqt5x11extras5-dev libmxml-dev
sudo apt install libx11-dev libxrandr-dev
sudo apt install wine wine-staging wine64 wine64-tools wine32 wine32-tools libwine-dev makensis
sudo apt install gcc-multilib libc6-dev-i386
sudo apt install libv4l-dev v4l-utils libopencv-dev
sudo apt install liblo-dev libcwiid-dev
sudo apt install sqlite3  # Required for Qt help system path fixes
```

### Arch Linux (Complete Build):
```bash
sudo pacman -S base-devel cmake pkg-config
sudo pacman -S libusb zlib bison flex qt5-base qt5-tools libmxml
sudo pacman -S wine-staging wine32 nsis
sudo pacman -S lib32-glibc lib32-gcc-libs
sudo pacman -S libv4l v4l-utils opencv liblo libcwiid
sudo pacman -S sqlite  # Required for Qt help system path fixes
```

### Fedora/RHEL (Complete Build):
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake pkg-config
sudo dnf install libusb1-devel zlib-devel bison flex
sudo dnf install qt5-qtbase-devel qt5-qmake libmxml-devel
sudo dnf install wine-devel wine-devel.i686 mingw-nsis-base  # Wine development tools and NSIS
sudo dnf install glibc-devel.i686 gcc.i686
sudo dnf install libv4l-devel v4l-utils opencv-devel liblo-devel libcwiid-devel
sudo dnf install sqlite  # Required for Qt help system path fixes
```

**Note**: For complete builds with all features, use:
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON
```

**Important**: On Fedora/RHEL, add Qt5 tools to PATH before building:
```bash
export PATH="/usr/lib64/qt5/bin:$PATH"
```

---

## Build Verification

After installation, verify your build:

```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test Wine bridge (if installed)
ls /opt/lib/linuxtrack/wine_bridge/

# Check webcam support (if installed)
ls /opt/lib/linuxtrack/libwc.so*
```

---

## Troubleshooting

### Common Issues:

| Problem | Solution |
|---------|----------|
| `winegcc: command not found` | Install Wine development tools: `sudo apt install libwine-dev wine32-tools` (Debian/Ubuntu) or `sudo dnf install wine-devel` (Fedora) or `sudo pacman -S wine` (Arch) |
| `Couldn't load library 'libwc.so.0'` | Library cache not updated. Run: `sudo ldconfig` then restart LinuxTrack |
| `ldconfig permission denied during install` | Use `-DENABLE_LDCONFIG=OFF` during cmake configuration for packaging systems or staging installations |
| `bits/libc-header-start.h: No such file or directory` | Install 32-bit headers: `sudo apt install gcc-multilib libc6-dev-i386` (Debian/Ubuntu) or `sudo dnf install glibc-devel.i686 libstdc++-devel.i686` (Fedora) or `sudo pacman -S lib32-glibc lib32-gcc-libs` (Arch) |
| GUI not displaying on Wayland | Force X11 compatibility: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | Add user to required groups: `sudo usermod -a -G plugdev,input $USER` |
| Application not appearing in launcher | Use `-DCMAKE_INSTALL_PREFIX=/usr/local` instead of `/opt` during installation |
| Firmware extraction fails | Run `./scripts/wine_check.sh` to diagnose Wine issues |

### Distribution-Specific Issues:

#### **Debian/Ubuntu/MX Linux**
| Problem | Solution |
|---------|----------|
| `CMake Error: Could not find Qt5` | Install: `sudo apt install qtbase5-dev qttools5-dev libqt5x11extras5-dev` |
| `Package 'opencv4' not found` | This is normal - OpenCV detection issue but build succeeds. Install: `sudo apt install libopencv-dev` |
| `qmake: command not found` | Install: `sudo apt install qttools5-dev-tools` |
| 32-bit build fails | This is normal if OpenCV detection failed. Core functionality still works. |
| Wine bridge compilation errors | These don't affect core LinuxTrack functionality |

#### **Fedora/RHEL**
| Problem | Solution |
|---------|----------|
| `qhelpgenerator: command not found` | Add Qt5 tools to PATH: `export PATH="/usr/lib64/qt5/bin:$PATH"` |
| Build fails at help generation step | Qt5 tools not in PATH. Run: `export PATH="/usr/lib64/qt5/bin:$PATH" && make -j$(nproc)` |
| `qmake: command not found` | Qt5 tools not in PATH. Add `/usr/lib64/qt5/bin` to PATH |
| `fatal error: bits/c++config.h: No such file or directory` | Install 32-bit C++ development packages: `sudo dnf install gcc-c++.i686` |
| `cannot find -lz: No such file or directory` | Install 32-bit zlib development: `sudo dnf install zlib-ng-compat-devel.i686` |
| `cannot find -lmxml: No such file or directory` | Install 32-bit mxml development: `sudo dnf install mxml-devel.i686` |
| `cannot find -lusb-1.0: No such file or directory` | Install 32-bit libusb development: `sudo dnf install libusb1-devel.i686` |
| `cannot find -lv4l2: No such file or directory` | Install 32-bit v4l development: `sudo dnf install libv4l-devel.i686` |
| Qt5 qmake not found | Create symlink: `sudo ln -s /usr/lib64/qt5/bin/qmake /usr/lib/qt5/bin/qmake` |

**Fedora-Specific Notes:**
- **32-bit Support**: The `--enable-ltr-32lib-on-x64` option enables 32-bit library building, which requires the additional 32-bit development packages listed above
- **Qt5 Path Issues**: If you encounter Qt5 path errors, the symlink `/usr/lib/qt5/bin/qmake -> /usr/lib64/qt5/bin/qmake` may need to be created
- **Wine Bridge**: The Wine bridge components may show compilation warnings but don't affect core LinuxTrack functionality
- **Package Updates**: Fedora package names may change between versions - use `dnf search` to find current package names if installation fails

#### **Qt5 Tools PATH Issues (Other Distributions)**

**Arch Linux Qt5 Tools PATH Fix:**
On some Arch Linux installations, Qt5 tools might be in `/usr/lib/qt5/bin/`:

```bash
export PATH="/usr/lib/qt5/bin:$PATH"
```

**OpenSUSE Qt5 Tools PATH Fix:**
On OpenSUSE, Qt5 tools are typically in `/usr/lib64/qt5/bin/`:

```bash
export PATH="/usr/lib64/qt5/bin:$PATH"
```

**General Qt5 Tools Verification:**
To check if Qt5 tools are accessible on any distribution:

```bash
which qhelpgenerator qmake moc
# Should show paths to Qt5 tools, not "not found"
```

#### **Fedora-Specific Troubleshooting**

| Problem | Solution |
|---------|----------|
| `fatal error: bits/c++config.h: No such file or directory` | Install 32-bit C++ development packages: `sudo dnf install gcc-c++.i686` |
| `cannot find -lz: No such file or directory` | Install 32-bit zlib development: `sudo dnf install zlib-ng-compat-devel.i686` |
| `cannot find -lmxml: No such file or directory` | Install 32-bit mxml development: `sudo dnf install mxml-devel.i686` |
| `cannot find -lusb-1.0: No such file or directory` | Install 32-bit libusb development: `sudo dnf install libusb1-devel.i686` |
| `cannot find -lv4l2: No such file or directory` | Install 32-bit v4l development: `sudo dnf install libv4l-devel.i686` |
| `i386:x86-64 architecture of input file is incompatible` | This is fixed by the additional 32-bit packages above |
| Qt5 qmake not found | Create symlink: `sudo ln -s /usr/lib64/qt5/bin/qmake /usr/lib/qt5/bin/qmake` |
| Wine bridge compilation errors | These don't affect core functionality - LinuxTrack will work without Wine bridge |
| **Build succeeds but Wine bridge fails** | **This is normal** - core LinuxTrack functionality is intact |

**Quick Fix for Common Fedora Issues:**
```bash
# If you encounter missing library errors during build
sudo dnf install -y gcc-c++.i686 zlib-ng-compat-devel.i686 libusb1-devel.i686 mxml-devel.i686 libv4l-devel.i686

# If Qt5 qmake is not found
sudo mkdir -p /usr/lib/qt5/bin
sudo ln -s /usr/lib64/qt5/bin/qmake /usr/lib/qt5/bin/qmake
```

#### **Arch Linux**
| Problem | Solution |
|---------|----------|
| `wine: WINEARCH is set to 'win32' but this is not supported in wow64 mode` | **Use wine-stable instead of wine-staging** - wine-staging doesn't support pure 32-bit prefixes |
| `wine-staging` conflicts with `wine-stable` | Remove conflicting packages: `sudo pacman -R wine-staging wine-gecko wine-mono winetricks` |
| Missing 32-bit libraries (liblo, mxml) | **Use the automated build script: `./scripts/build_32bit_libs.sh`** |
| 32-bit/64-bit compilation conflicts | **Use explicit 64-bit flags: `CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64"`** |
| Qt5 Makefile not generated | **Manually generate: `cd src/qt_gui && /usr/bin/qmake-qt5 -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack" ltr_gui.pro`** |

#### **Arch Linux / Manjaro**


**⚠️ IMPORTANT: For Wine bridge functionality (TrackIR MFC42 support), wine-staging has 32-bit limitations. Use Method 3 below for optimal Wine support.**

#### **Method 1: AppImage (Quick Start - Recommended)**
```bash
# Download and run the AppImage (works immediately on Arch Linux)
chmod +x LinuxTrack-X-IR-*.AppImage
./LinuxTrack-X-IR-*.AppImage

# For Wayland users, force X11 compatibility
QT_QPA_PLATFORM=xcb ./LinuxTrack-X-IR-*.AppImage
```

#### **Method 2: Automated Build Script**
```bash
./scripts/build_arch_linux.sh
```

#### **Method 3: Manual Build with 32-bit Library Fix**
This method addresses the common 32-bit library issues on Arch Linux:

**Step 1: Install Dependencies**
```bash
# Install build dependencies
sudo pacman -S --needed base-devel cmake qt5-base qt5-tools qt5-x11extras opencv libusb mxml libx11 libxrandr bison flex lib32-glibc lib32-gcc-libs v4l-utils multilib-devel
yay -S nsis cwiid liblo-ipv6
```

**Step 2: Build Missing 32-bit Libraries (CRITICAL)**
```bash
# Run the automated script to build missing 32-bit libraries
./scripts/build_32bit_libs.sh
```

**Step 3: Build LinuxTrack**
```bash
# Clone repository
git clone <repository-url>
cd linuxtrackx-ir

# Create build directory
mkdir build && cd build

# Configure with CMake (disable 32-bit library if not needed)
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=OFF

# Build with parallel compilation
cmake --build . -j$(nproc)

# Install
sudo cmake --install .
```

#### **Method 4: Optimized Wine Installation (Recommended for TrackIR)**
For users who need Wine bridge compatibility with TrackIR support:

**Step 1: Install Dependencies**
```bash
# Install required dependencies
sudo pacman -S --needed desktop-file-utils fontconfig lib32-fontconfig freetype2 lib32-freetype2 gcc-libs lib32-gcc-libs gettext lib32-gettext libpcap lib32-libpcap libunwind lib32-libunwind libxcursor lib32-libxcursor libxi lib32-libxi libxkbcommon lib32-libxkbcommon libxrandr lib32-libxrandr wayland lib32-wayland

# Install build dependencies
sudo pacman -S --needed alsa-lib lib32-alsa-lib gnutls lib32-gnutls gst-plugins-base-libs lib32-gst-plugins-base-libs libcups lib32-libcups libgphoto2 libpulse lib32-libpulse libxcomposite lib32-libxcomposite libxinerama lib32-libxinerama libxxf86vm lib32-libxxf86vm mesa lib32-mesa mingw-w64-gcc ocl-icd lib32-ocl-icd opencl-headers pcsclite lib32-pcsclite perl samba sane sdl2 lib32-sdl2 v4l-utils lib32-v4l-utils
```

**Step 2: Build Wine-Stable with CPU Optimizations**
```bash
# Clone and optimize wine-stable build
cd /tmp
git clone https://aur.archlinux.org/wine-stable.git
cd wine-stable

# Set up GPG key for source verification
gpg --keyserver keys.openpgp.org --recv-keys CFDF148828C642A7

# Optimize PKGBUILD for fast compilation (16 cores + native CPU optimizations)
sed -i 's/make$/make -j16/g' PKGBUILD
sed -i 's/export CFLAGS="${CFLAGS/-fno-plt/}"/export CFLAGS="${CFLAGS/-fno-plt/}"\n  export MAKEFLAGS="-j16"\n  export CFLAGS+=" -O2 -march=native -mtune=native"\n  export CXXFLAGS+=" -O2 -march=native -mtune=native"/' PKGBUILD

# Build wine-stable (30-45 minutes on 16-core system)
makepkg -sri --noconfirm
```

**Step 3: Install Wine-Stable-Mono**
```bash
# Install Wine Mono for .NET Framework compatibility
cd /tmp
git clone https://aur.archlinux.org/wine-stable-mono.git
cd wine-stable-mono
makepkg -sri --noconfirm

# Install Wine Gecko for Internet Explorer compatibility
sudo pacman -S wine-gecko
```

**Step 4: Build LinuxTrack with Wine Support**
```bash
# Build LinuxTrack with Wine support (after building 32-bit libraries)
cd linuxtrackx-ir

# Create build directory
mkdir build && cd build

# Configure with CMake (32-bit library enabled)
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DLIB32DIR=lib32

# Build with parallel compilation
cmake --build . -j$(nproc)

# Install
sudo cmake --install .
sudo ldconfig
```

#### **Method 5: Prebuilt Installation**
```bash
./scripts/install/install_arch_prebuilt.sh
```

#### **Testing Features**
```bash
./scripts/test_wiimote_support.sh    # Test Wiimote support
./scripts/test_osc_support.sh        # Test OSC support
./scripts/test_xplane_sdk.sh         # Test X-Plane SDK support
./scripts/test_wine_support.sh       # Test Wine compatibility
```

**🚀 Wine Optimization Details:**
The optimized Wine installation includes:
- **16-core parallel builds** (`-j16`) for maximum compilation speed
- **Native CPU optimizations** (`-march=native -mtune=native`) for your specific hardware
- **O2 optimization level** for balanced speed/size optimization
- **Complete Wine suite**: wine-stable, wine-stable-mono, wine-gecko
- **32-bit Wine prefix support** for TrackIR MFC42 compatibility
- **Build time**: ~30-45 minutes on 16-core systems (vs 2+ hours without optimization)

**🔧 32-bit Library Solution:**
The 32-bit library build system was developed to solve the common Arch Linux build issues:
- **Automated script** for building missing 32-bit libraries
- **Proper compiler flags** to prevent 32-bit/64-bit conflicts
- **Parallel compilation** for fast builds
- **Clean installation** with correct symlinks

*This solution was developed through collaborative community effort, addressing the issues documented in [GitHub Issue #206](https://github.com/uglyDwarf/linuxtrack/issues/206) and providing a complete, repeatable solution for Arch Linux users.*

### Display Server Compatibility

#### **X11 (Recommended)**
LinuxTrack works best with X11:
```bash
ltr_gui
```

#### **Wayland Compatibility**
If you're using Wayland, force X11 compatibility:
```bash
QT_QPA_PLATFORM=xcb ltr_gui NOTE That this may not be needed. As of late I have had sucess simply launching ltr_gui in Wayland.
```

### Build Summary:
After running `cmake ..`, check the build summary for enabled components:
- ✅ Webcam support: yes/no
- ✅ Wiimote support: yes/no  
- ✅ TrackIR support: yes/no
- ✅ Facetracker support: yes/no
- ✅ XPlane plugin: yes/no
- ✅ Mickey: yes/no
- ✅ Wine plugin: yes/no
- ✅ OSC support: yes/no
- ✅ PIE support: enabled/disabled

### Testing Features
```bash
./scripts/test_wiimote_support.sh    # Test Wiimote support
./scripts/test_osc_support.sh        # Test OSC support
./scripts/test_xplane_sdk.sh         # Test X-Plane SDK support
./scripts/test_wine_support.sh       # Test Wine compatibility
```

## Supported Games

### **Native Linux Games**
- DCS World
- War Thunder
- Elite Dangerous
- Euro Truck Simulator 2
- American Truck Simulator

### **Windows Games (via Wine/Steam Proton)**
- Microsoft Flight Simulator
- IL-2 Sturmovik series
- Falcon 4
- Commanche vs Havoc
- **X-Plane** (requires X-Plane SDK)

## Usage

### **First Launch**
```bash
# For X11 (recommended)
ltr_gui

# For Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui NOTE That this may not be needed. As of late I have had sucess simply launching ltr_gui in Wayland.
```

### **Configuration Steps**
1. **Select your tracking device** (TrackIR, webcam, etc.)
2. **Calibrate tracking points** for accurate head tracking
3. **Set up game profiles** for different applications
4. **Test tracking functionality** before gaming

### **Alternative Tracking Methods**
- **AntiMicro joystick detection** - Convert head tracking to joystick input
- **Server approach** - Use LinuxTrack server for Wine games
- **OSC network** - External application integration


### **Option 3: Build AppImage (Advanced)**

#### **AppImage Dependencies**
For optimal AppImage creation with enhanced Qt5 support and Wayland compatibility:

```bash
# Essential build dependencies
sudo apt-get install -y build-essential cmake

# Qt5 development dependencies (enhanced)
sudo apt-get install -y qtbase5-dev qtwayland5-dev libqt5waylandclient5 libqt5x11extras5-dev

# Additional dependencies for AppImage creation
sudo apt-get install -y patchelf appstream-util

# Standard LinuxTrack dependencies (include multilib toolchain and 32-bit dev headers)
sudo apt-get install -y qttools5-dev-tools qttools5-dev libopencv-dev libusb-1.0-0-dev libmxml-dev libx11-dev libxrandr-dev bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev wine-staging \
  libmxml-dev:i386 libusb-1.0-0-dev:i386 liblo-dev:i386 libx11-dev:i386 libxrandr-dev:i386
```

#### **Build AppImage**
```bash
# Clone repository
git clone <repository-url>
cd linuxtrackx-ir

# Build and create AppImage
./scripts/appimage/build_appimage_phase4.sh --clean

# The AppImage will be created as: LinuxTrack-X-IR-0.99.24-x86_64.AppImage
```

#### **AppImage Features**
- ✅ **Self-contained**: All dependencies bundled
- ✅ **Cross-distribution**: Works on any Linux distribution
- ✅ **Qt5 Enhanced**: Full Qt5 development support
- ✅ **Wayland Compatible**: Includes Wayland libraries for modern desktop environments
- ✅ **Wine Bridge**: Complete Wine integration for Windows game compatibility
- ✅ **32-bit + 64-bit Runtime**: Bundles 32-bit liblinuxtrack for 32-bit Wine prefixes
- ✅ **Help System**: Integrated Qt help system with SQLite support
- ✅ **Library Isolation**: Complete library isolation for maximum compatibility
- ✅ **Wine Bridge Guide**: See `docs/AppImage_WineBridge.md` for 32-bit/64-bit Wine usage
```

## 🎮 Usage

### **First Launch**
```bash
# For X11 (recommended)
ltr_gui

# For Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui
```

### **Configuration Steps**
1. **Select your tracking device** (TrackIR, webcam, etc.)
2. **Calibrate tracking points** for accurate head tracking
3. **Set up game profiles** for different applications
4. **Test tracking functionality** before gaming

### **Supported Games**
- Microsoft Flight Simulator (via Wine)
- DCS World
- IL-2 Sturmovik series
- War Thunder
- Elite Dangerous
- Euro Truck Simulator 2
- American Truck Simulator
- Falcon 4
- Commanche vs Havoc
- **X-Plane** (requires XPlane SDK)

## 🎯 Project Focus

### **Primary Goals**
- **TrackIR Support**: Full compatibility with TrackIR 4 & 5 devices
- **Modern Linux Compatibility**: Updated build system for current distributions
- **Qt5 Migration**: Modernized GUI framework support
- **Wine Integration**: Wine-based Windows compatibility layer

### **Full Feature Support on Arch Linux**
✅ **All features are now supported and tested on Arch Linux**:

- **TrackIR Support**: Full compatibility with TrackIR 4 & 5 devices
- **Webcam/Face Tracking**: Full OpenCV integration for optical tracking
- **Wiimote Support**: Available via AUR (cwiid/cwiid-git)
- **OSC Support**: Network-based head tracking via liblo-ipv6
- **X-Plane Plugin**: Full X-Plane SDK 4.1.1 integration
- **Wine Bridge**: Complete 32-bit wine bridge support with wine32
- **Modern Security**: PIE, stack protector, and fortify source enabled

### **Limited/Untested Features**
⚠️ **Important**: The following features are **not current development priorities**:

- **macOS Support**: Mac compatibility not verified
- **Legacy Wii Remote**: Original Wii tracking (use Wiimote instead)

## 🔧 Key Features

### **What's New**
- ✅ **Qt5 Modernization**: Updated from Qt4 to Qt5 for current Linux distributions
- ✅ **Wine Compatibility**: Wine-based Windows binary support using winegcc
- ✅ **Optimized Wine Installation**: Arch Linux optimized builds with parallel compilation and ccache
- ✅ **Modern Build System**: Updated autotools
- ✅ **Enhanced Security**: PIE, stack protector, and fortify source enabled by default
- ✅ **TrackIR Focus**: Primary focus on TrackIR 4 & 5 compatibility
- ✅ **Active Development**: Regular updates and community support
- ✅ **OSC Network Support**: Open Sound Control for network-based head tracking
- ✅ **X-Plane Plugin**: Complete X-Plane plugin development support
- ✅ **Automated Build Scripts**: One-command installation for Arch Linux (WORK IN PROGRESS)
- ✅ **Packaging Support**: Improved DESTDIR support and `--disable-ldconfig` option for distribution maintainers

### **Wine Integration**
LinuxTrack now supports building Windows compatibility components using winegcc:

- **NPClient.dll.so** / **NPClient64.dll.so** - TrackIR API compatibility (32/64-bit)
- **FreeTrackClient.dll.so** - FreeTrack API compatibility

**🚀 Arch Linux Optimization**: The Wine installation process has been optimized for Arch Linux with:
- **Parallel builds** using all CPU cores for faster compilation
- **ccache integration** for 70-90% faster future builds
- **CPU-specific optimizations** for your hardware
- **Complete Wine suite**: wine-stable, wine-mono, wine-gecko

### **MFC42 Winetricks Installation** ✅
LinuxTrack now uses MFC42 libraries installed via winetricks for optimal TrackIR compatibility:

- **✅ TrackIR Compatibility** - MFC42 works correctly with TrackIR applications
- **✅ Winetricks Integration** - Uses `winetricks mfc42` for reliable installation
- **✅ Enhanced Debugging** - Comprehensive logging and fallback mechanisms
- **✅ User-Friendly Process** - Clear progress updates and user confirmation
- **✅ Multiple Fallback Options** - Automatic fallback to `winetricks vcrun6` if needed
- **✅ Cross-Distribution Support** - Works across different Linux distributions
- **✅ 32-bit Wine Prefix** - Forces 32-bit wine prefix for optimal MFC42 compatibility

## 🖥️ Display Server Compatibility

### **X11 (Recommended)**
LinuxTrack works best with X11:
```bash
ltr_gui
```

### **Wayland Compatibility** (WORK IN PROGRESS)
If you're using Wayland, force X11 compatibility:
```bash
QT_QPA_PLATFORM=xcb ltr_gui
```

## 🎯 Supported Hardware

### **USB Devices**
- **TrackIR 4 & 5** - Support


### **DIY Solutions**
- **Webcam + IR LEDs** - Build your own IR tracking setup
- **Wii Remote** - Nintendo Wiimote tracking
- **FaceTrackNoIR compatible** - Face detection tracking
- **PlayStation Eye** - USB camera with IR filter removal
- **Generic USB cameras** - For optical tracking
- **TrackHat sensors** - Specialized tracking hardware

## 🔍 Troubleshooting

### **Common Issues**

| Problem | Solution |
|---------|----------|
| `winegcc: command not found` | Install Wine development tools: `sudo apt install libwine-dev wine32-tools` (Debian/Ubuntu) or `sudo dnf install wine-devel` (Fedora) or `sudo pacman -S wine` (Arch) |
| `Couldn't load library 'libwc.so.0'` | Library cache not updated. Run: `sudo ldconfig` then restart LinuxTrack |
| `ldconfig permission denied during install` | Use `--disable-ldconfig` during configure for packaging systems or staging installations |
| `bits/libc-header-start.h: No such file or directory` | Install 32-bit headers: `sudo apt install gcc-multilib libc6-dev-i386` (Debian/Ubuntu) or `sudo dnf install glibc-devel.i686 libstdc++-devel.i686` (Fedora) or `sudo pacman -S lib32-glibc lib32-gcc-libs` (Arch) |
| MFC42 installation fails | Use the built-in MFC42 installer in the GUI, or manually run `winetricks mfc42`. The enhanced debugging will show detailed output and automatically try `winetricks vcrun6` as fallback |
| GUI not displaying on Wayland | Force X11 compatibility: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | Add user to required groups: `sudo usermod -a -G plugdev,input $USER` |
| Application not appearing in launcher | Use `--prefix=/usr/local` instead of `/opt` during installation |
| Firmware extraction fails | Run `./scripts/wine_check.sh` to diagnose Wine issues |

### Fedora/Nobara Notes (winetricks)

- Install prerequisites:
```bash
sudo dnf install winetricks cabextract wine
```

- Verify winetricks availability:
```bash
which winetricks
winetricks --version
```

- If launching the GUI from a desktop icon, the environment PATH may exclude `/usr/bin`. Launch from a terminal, or ensure `/usr/bin` is present in PATH, then retry the MFC42 installation in the GUI.

- Flatpak/sandboxed environments: the app can use host tools. Verify host winetricks with:
```bash
flatpak-spawn --host which winetricks
```

### **Arch Linux Troubleshooting**

#### **32-bit Library Issues (Most Common Problem)**

**Problem:** Build fails with errors like:
```
/usr/bin/ld: cannot find -lltr: No such file or directory
/usr/bin/ld: .libs/libltr_la-cal.o: file class ELFCLASS32 incompatible with ELFCLASS64
```

**Root Cause:** LinuxTrack requires 32-bit versions of some libraries for Wine bridge compatibility, but these are not available in the official Arch repositories:
- `lib32-mxml` (Mini-XML library)
- `lib32-liblo` (Open Sound Control library)

**Solution:** Use the automated 32-bit library build script:
```bash
# Run the automated script to build and install missing 32-bit libraries
./scripts/build_32bit_libs.sh

# Then build LinuxTrack with CMake
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=OFF
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools on Fedora/RHEL
cmake --build . -j$(nproc)
```

**What the script does:**
- Downloads source code for both missing libraries
- Builds 32-bit versions with proper compiler flags
- Installs to `/usr/lib32/` with correct symlinks
- Uses parallel compilation for speed
- Cleans up build files automatically

#### **Common Arch Linux Wine Issues:**

| Problem | Solution |
|---------|----------|
| `wine: WINEARCH is set to 'win32' but this is not supported in wow64 mode` | **Use wine-stable instead of wine-staging** - wine-staging doesn't support pure 32-bit prefixes |
| `wine-staging` conflicts with `wine-stable` | Remove conflicting packages: `sudo pacman -R wine-staging wine-gecko wine-mono winetricks` |
| `lib32-unixodbc` not found | This package doesn't exist in Arch. Remove it from PKGBUILD dependencies |
| Wine build takes hours | Use optimized builds with parallel compilation: `make -j16` and native CPU optimizations |
| Missing Wine dependencies | Install complete suite: `wine-stable`, `wine-stable-mono`, `wine-gecko` |
| Wine bridge not working | Ensure wine-stable is installed, not wine-staging |
| Orphaned packages after Wine removal | Clean up: `sudo pacman -Rns $(pacman -Qtdq)` |
| **Missing 32-bit libraries (liblo, mxml)** | **Use the automated build script: `./scripts/build_32bit_libs.sh`** |
| **32-bit/64-bit compilation conflicts** | **Use explicit 64-bit flags: `CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64"`** |
| **Qt5 Makefile not generated** | **Manually generate: `cd src/qt_gui && /usr/bin/qmake-qt5 -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack" ltr_gui.pro`** |
| **MFC42 installation fails** | **Use wine-stable with 32-bit prefix support, not wine-staging** |

#### **32-bit Runtime Verification**
```bash
# After install from source
file /usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0

# After running AppImage
ls -l squashfs-root/usr/lib/i386-linux-gnu/linuxtrack/ 2>/dev/null || true
```

#### **Build System Issues:**

**Problem:** CMake may detect 32-bit toolchain when not needed
**Solution:** Use CMake with explicit options:
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=OFF
```

**Problem:** Qt5 Makefile not generated automatically
**Solution:** Manually generate the Makefile:
```bash
cd src/qt_gui
/usr/bin/qmake-qt5 -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack" ltr_gui.pro
```

#### **Verification Steps:**

After building, verify the installation:
```bash
# Check if 32-bit libraries are installed
ls -la /usr/lib32/libmxml* /usr/lib32/liblo*

# Check if LinuxTrack binaries are built
ls -la src/qt_gui/ltr_gui
ls -la src/mickey/mickey

# Test Wine bridge components
ls -la src/wine_bridge/client/NPClient*.so
ls -la src/wine_bridge/client/FreeTrackClient*.so
```

#### **Automated Scripts for Arch Linux**

**`scripts/build_32bit_libs.sh`** - Automated 32-bit library builder
- **Purpose:** Builds missing 32-bit libraries (`lib32-mxml`, `lib32-liblo`) for Arch Linux
- **Usage:** `./scripts/build_32bit_libs.sh`
- **Features:**
  - Downloads latest source code for both libraries
  - Uses proper 32-bit compiler flags (`-m32`)
  - Installs to correct locations (`/usr/lib32/`)
  - Creates necessary symlinks
  - Uses parallel compilation for speed
  - Cleans up build files automatically

**`scripts/lib32-mxml.PKGBUILD`** - PKGBUILD template for AUR
- **Purpose:** Template for creating AUR packages for 32-bit libraries
- **Usage:** Copy to AUR package directory and customize
- **Features:** Follows Arch Linux 32-bit package guidelines

**`docs/ARCH_LINUX_32BIT_LIBRARIES.md`** - Detailed guide
- **Purpose:** Complete manual for building 32-bit libraries
- **Contents:** Step-by-step instructions, troubleshooting, AUR package creation

#### **Missing 32-bit Libraries Solution:**
If you encounter "skipping incompatible" errors for `liblo` or `mxml`, these 32-bit libraries are not available in the official repos or AUR. Use the automated build script:

```bash
# Build missing 32-bit libraries
./scripts/build_32bit_libs.sh

# Then try building LinuxTrack again
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools on Fedora/RHEL
cmake --build . -j$(nproc)
```

This script will:
- Download and build 32-bit versions of `mxml` and `liblo`
- Install them to `/usr/lib32/`
- Create necessary symlinks
- Clean up build files automatically
```

### **Enhanced MFC42 Debugging** 🔍
The latest version includes comprehensive debugging for MFC42 winetricks installation:

- **Winetricks Version Check** - Verifies winetricks functionality before installation
- **Environment Setup** - Forces 32-bit wine prefix (`WINEARCH=win32`) for optimal compatibility
- **Comprehensive Output Capture** - Shows detailed winetricks output for troubleshooting
- **Automatic Fallback** - If `winetricks mfc42` fails, automatically tries `winetricks vcrun6`
- **File System Inspection** - Uses `find` and `ls` commands to verify installed files
- **Progress Updates** - Real-time feedback during long-running installations

**Debugging Output Includes:**
- Winetricks version and functionality test
- Wine prefix creation and environment setup
- Complete winetricks command output
- File system inspection of installed DLLs
- Fallback installation attempts
- Detailed error reporting

### **Debian/Ubuntu Build Troubleshooting**

| Problem | Solution |
|---------|----------|
| `configure: error: Qt5 development headers not found` | Install: `sudo apt install qtbase5-dev qttools5-dev libqt5x11extras5-dev` |
| `configure: opencv4 >= 0.29.0... no` | This is normal - OpenCV detection issue but build succeeds. Install: `sudo apt install libopencv-dev` |
| GUI doesn't display after build | **Force X11 compatibility**: `QT_QPA_PLATFORM=xcb ltr_gui` |
| `qmake: command not found` | Install: `sudo apt install qttools5-dev-tools` |
| 32-bit build fails | This is normal if OpenCV detection failed. Core functionality still works. |
| Wine bridge compilation errors | These don't affect core LinuxTrack functionality |

### **GUI Display Issues**
If `ltr_gui` appears to run but no window appears:
```bash
# Force X11 compatibility (recommended)
QT_QPA_PLATFORM=xcb ltr_gui

# Check display environment
echo $DISPLAY $XDG_SESSION_TYPE

# Run with debugging
QT_DEBUG_PLUGINS=1 ltr_gui 2>&1 | head -20
```

### **Getting Help**
1. **Try AppImage first** - Easiest installation method
2. **Check display server** - Verify X11/Wayland compatibility with `echo $XDG_SESSION_TYPE`
3. **Verify installation** - Run `ltr_gui` to test basic functionality
4. **Check logs** - Look for error messages in terminal output
5. **Distribution guides** - See the `docs/` directory for detailed guides
6. **MFC42 debugging** - Use the enhanced debugging output to troubleshoot winetricks installation

## 📦 Package Maintainers & Distribution Builders

LinuxTrack now includes improved support for packaging systems and distribution maintainers:

### **DESTDIR Support**
The build system now properly respects `DESTDIR` for staging installations:
```bash
# Package building example
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LDCONFIG=OFF
cmake --build . -j$(nproc)
DESTDIR=/tmp/package-staging cmake --install .
```

### **Library Configuration Control**
Use `-DENABLE_LDCONFIG=OFF` to prevent automatic library cache updates:
- **RPM packages**: Let the package manager handle `ldconfig` via `%post` scripts
- **DEB packages**: Use `dh_makeshlibs` and `dh_shlibdeps` instead
- **Staging builds**: Avoid permission issues during `DESTDIR` installations

### **Recommended Package Configuration**
```bash
mkdir build && cd build

# For most distributions
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LDCONFIG=OFF -DENABLE_LTR_32LIB_ON_X64=ON

# For minimal packages (TrackIR only)
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LDCONFIG=OFF

# For full-featured packages
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LDCONFIG=OFF -DENABLE_WEBCAM=ON -DENABLE_LTR_32LIB_ON_X64=ON
```

### **Package Dependencies**
- **Core**: `libusb-1.0`, `zlib`, `libmxml`, `qt5-base`
- **Optional**: `libv4l2` (webcam), `libcwiid` (Wiimote), `liblo` (OSC)
- **Wine**: `wine` or `wine-staging` for Windows game support

---

## 📖 Documentation

### **Build Guides**
- **[MX Linux Build Guide](docs/MX_LINUX_BUILD_GUIDE.md)** - Detailed guide for MX Linux users
- **[Wine Bridge Guide](docs/WINE_BRIDGE_BUILD_GUIDE.md)** - Windows compatibility details
- **[Qt4 to Qt5 Migration](docs/QT4_TO_QT5_MIGRATION.md)** - Technical migration notes
- **[Advanced Installation & Dependency Analysis](docs/technical/ADVANCED_INSTALLATION_AND_DEPENDENCIES.md)** – Full dependency analysis, advanced build options, and troubleshooting

### **Fork Information**
- **[Fork Notice](FORK_NOTICE.md)** - Detailed explanation of the fork relationship
- **[Changelog](CHANGELOG.md)** - What's new and different from the original project
- **[Contributors](AUTHORS)** - Complete list of original and fork contributors

## 🤝 Contributing

LinuxTrack is an open-source project that welcomes contributions:

1. **Bug Reports**: Use the issue tracker for bugs and feature requests
2. **Code Contributions**: Submit pull requests with improvements
3. **Documentation**: Help improve our guides and documentation
4. **Testing**: Test on different distributions and hardware setups

### **Development Setup**
```bash
git clone <repository-url>
cd linuxtrackx-ir
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_DEBUG=ON
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools on Fedora/RHEL
cmake --build . -j$(nproc)
```

## 📜 License

LinuxTrack X-IR is released under the MIT License, maintaining compatibility with the original project's licensing. See the [LICENSE.md](LICENSE.md) file for details.

## 🌐 Links

- **Original Project**: [LinuxTrack by uglyDwarf](https://github.com/uglyDwarf/linuxtrack)
- **This Fork**: [LinuxTrack X-IR](https://github.com/fwfa123/linuxtrackx-ir)
- **Documentation**: See the `docs/` directory for detailed guides

## 🙏 Acknowledgments

### **Original Authors**
This project would not exist without the incredible work of the original LinuxTrack developers:

- **uglyDwarf** - Original creator and main developer of LinuxTrack
- **Tulthix** - Early contributor to the project
- **f.josef** - Main developer and maintainer (302 commits)
- **Michal Navratil** - Significant contributor (20 commits)

### **Current Maintainer**
This fork is maintained by **fwfa123**

### **Additional Contributors**
- **raven** - Contributor to original linuxtrack repository (4 commits)
- **aledin** - Contributor (2 commits)
- **Cursor AI** - Assisted with Qt5 modernization and build system updates

### **Arch Linux Wine Optimization Contributors**
The optimized Wine installation approach for Arch Linux was developed through collaborative community effort:

- **Arch Linux Community** - Provided the foundation for Wine optimization techniques
- **GitHub Issue #206 Contributors** - [uglyDwarf/linuxtrack#206](https://github.com/uglyDwarf/linuxtrack/issues/206) - Community members who contributed to solving Wine compatibility issues on Arch Linux
- **AUR Package Maintainers** - Maintainers of wine-stable and wine-stable-mono packages
- **Arch Linux Wine Users** - Community members who tested and validated the optimization approaches

---

**Ready to start tracking?** Follow the [Quick Start](#-quick-start) section above!

---

## 🔄 Fork Notice

This project is a fork of the original LinuxTrack project. The original project slowed down, and this fork continues development with modern build systems and enhanced compatibility. We maintain full attribution to the original authors and respect their contributions to the open-source community.

**Original Project**: [LinuxTrack by uglyDwarf](https://github.com/uglyDwarf/linuxtrack)  
**License**: MIT License (same as original)  
**Fork Purpose**: Modernization and continued development
