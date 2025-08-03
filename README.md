# LinuxTrack X-IR v0.99.21 - Modern Head Tracking for Linux

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/fwfa123/linuxtrackx-ir)
[![License](https://img.shields.io/badge/license-MIT-blue)](https://github.com/fwfa123/linuxtrackx-ir/blob/master/LICENSE.md)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)](https://github.com/fwfa123/linuxtrackx-ir)

**LinuxTrack X-IR** is a modern fork of the original LinuxTrack project, bringing head tracking to Linux and Windows with updated build systems, Qt5 support, and Wine compatibility. Track your head movement using TrackIR devices, webcams, or specialized hardware and translate that into game input.

### Why the **X** in the name?  I am concentrating on TrackIR support and I do not 
have a Mac and other items I am not supporting these at this time, although the code 
reamains embedded.

Lots left TODO  (https://github.com/fwfa123/linuxtrackx-ir/blob/main/docs/progress/TODO_FUTURE_DEVELOPMENT.md)
## 🚀 Quick Start

### **Recommended: AppImage Installation (Easiest)**
```bash
# Download and run the latest AppImage
chmod +x LinuxTrack-X-IR-*.AppImage
./LinuxTrack-X-IR-*.AppImage
```

### **Alternative: Build from Source**
Choose your distribution below for detailed instructions.

## 📦 Installation Options

### **Option 1: AppImage (Recommended for most users)**
- ✅ **No dependencies to install** - Everything included
- ✅ **Works on any Linux distribution** - Universal compatibility
- ✅ **Easy updates** - Just download new AppImage
- ✅ **No system modifications** - Runs from any location

```bash
# Download AppImage from releases
chmod +x LinuxTrack-X-IR-*.AppImage

# For X11 (recommended)
./LinuxTrack-X-IR-*.AppImage

# For Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ./LinuxTrack-X-IR-*.AppImage
```

### **Option 2: Build from Source**

#### **Debian / Ubuntu / MX Linux**
```bash
# Install dependencies
sudo apt install -y build-essential autoconf automake libtool qtbase5-dev qttools5-dev-tools qttools5-dev libqt5x11extras5-dev libopencv-dev libusb-1.0-0-dev libmxml-dev libx11-dev libxrandr-dev bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev wine-staging

# Build and install
git clone <repository-url>
cd linuxtrackx-ir
cd scripts/dev && ./wine_dev_setup.sh && cd ../..
autoreconf -fiv
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install
sudo usermod -a -G plugdev,input $USER
```

#### **Fedora / RHEL / CentOS**
```bash
# Install dependencies
sudo dnf install -y gcc gcc-c++ make autoconf automake libtool qt5-qtbase-devel qt5-qttools-devel qttools5-dev qt5-qtx11extras-devel opencv-devel libusb1-devel libmxml-devel libX11-devel libXrandr-devel bison flex nsis glibc-devel.i686 libstdc++-devel.i686 v4l-utils-devel wine-staging

# Build and install
git clone <repository-url>
cd linuxtrackx-ir
cd scripts/dev && ./wine_dev_setup.sh && cd ../..
autoreconf -fiv
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install
sudo usermod -a -G plugdev,input $USER
```

#### **Arch Linux / Manjaro**

**⚠️ IMPORTANT: Arch Linux users may encounter 32-bit library issues during build. See the troubleshooting section below for the complete solution.**

#### **Method 1: Automated Build Script (Recommended)**
```bash
./scripts/build_arch_linux.sh
```

#### **Method 2: Manual Build with 32-bit Library Fix**
This method addresses the common 32-bit library issues on Arch Linux:

**Step 1: Install Dependencies**
```bash
# Install build dependencies
sudo pacman -S --needed base-devel autoconf automake libtool qt5-base qt5-tools qt5-x11extras opencv libusb mxml libx11 libxrandr bison flex lib32-glibc lib32-gcc-libs v4l-utils multilib-devel
yay -S nsis cwiid liblo-ipv6
```

**Step 2: Build Missing 32-bit Libraries (CRITICAL)**
```bash
# Run the automated script to build missing 32-bit libraries
./scripts/build_32bit_libs.sh
```

**Step 3: Build LinuxTrack**
```bash
# Clone and configure
git clone <repository-url>
cd linuxtrackx-ir
autoreconf -fiv

# Configure with explicit 64-bit flags to prevent 32-bit compilation issues
CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64" ./configure --prefix=/usr/local --disable-ltr-32lib-on-x64

# Build with parallel compilation
make -j$(nproc)

# Install
sudo make install
sudo usermod -a -G plugdev,input $USER
```

#### **Method 3: Optimized Wine Installation (Advanced)**
For users who need Wine bridge compatibility:

```bash
# Clean up existing Wine installations
sudo pacman -R wine-staging wine-gecko wine-mono winetricks 2>/dev/null || true
sudo pacman -Rns $(pacman -Qtdq) 2>/dev/null || true

# Install optimized Wine packages
cd /tmp
git clone https://aur.archlinux.org/wine-stable.git
cd wine-stable
makepkg -sri

# Install Wine Mono and Gecko
cd /tmp
git clone https://aur.archlinux.org/wine-stable-mono.git
cd wine-stable-mono
makepkg -sri
sudo pacman -S wine-gecko

# Build LinuxTrack with Wine support (after building 32-bit libraries)
cd linuxtrackx-ir
CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64" ./configure --prefix=/usr/local --enable-ltr-32lib-on-x64
make -j$(nproc)
sudo make install
```

#### **Method 4: Prebuilt Installation**
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
- **Parallel builds** using all CPU cores for faster compilation
- **ccache integration** for 70-90% faster future builds
- **CPU-specific optimizations** for your hardware
- **Complete Wine suite**: wine-stable, wine-mono, wine-gecko
- **Conflict resolution** to ensure clean installation

**🔧 32-bit Library Solution:**
The 32-bit library build system was developed to solve the common Arch Linux build issues:
- **Automated script** for building missing 32-bit libraries
- **Proper compiler flags** to prevent 32-bit/64-bit conflicts
- **Parallel compilation** for fast builds
- **Clean installation** with correct symlinks

*This solution was developed through collaborative community effort, addressing the issues documented in [GitHub Issue #206](https://github.com/uglyDwarf/linuxtrack/issues/206) and providing a complete, repeatable solution for Arch Linux users.*

### **Option 3: Build AppImage (Advanced)**

#### **AppImage Dependencies**
For optimal AppImage creation with enhanced Qt5 support and Wayland compatibility:

```bash
# Essential build dependencies
sudo apt-get install -y build-essential autoconf automake libtool

# Qt5 development dependencies (enhanced)
sudo apt-get install -y qtbase5-dev qtwayland5-dev libqt5waylandclient5 libqt5x11extras5-dev

# Additional dependencies for AppImage creation
sudo apt-get install -y patchelf appstream-util

# Standard LinuxTrack dependencies
sudo apt-get install -y qttools5-dev-tools qttools5-dev libopencv-dev libusb-1.0-0-dev libmxml-dev libx11-dev libxrandr-dev bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev wine-staging
```

#### **Build AppImage**
```bash
# Clone repository
git clone <repository-url>
cd linuxtrackx-ir

# Build and create AppImage
./scripts/appimage/build_appimage_phase4.sh --clean

# The AppImage will be created as: LinuxTrack-X-IR-0.99.21-x86_64.AppImage
```

#### **AppImage Features**
- ✅ **Self-contained**: All dependencies bundled
- ✅ **Cross-distribution**: Works on any Linux distribution
- ✅ **Qt5 Enhanced**: Full Qt5 development support
- ✅ **Wayland Compatible**: Includes Wayland libraries for modern desktop environments
- ✅ **Wine Bridge**: Complete Wine integration for Windows game compatibility
- ✅ **Help System**: Integrated Qt help system with SQLite support
- ✅ **Library Isolation**: Complete library isolation for maximum compatibility
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
- **✅ User-Friendly Process** - Clear progress updates and user confirmation
- **✅ Multiple Fallback Options** - Alternative installation methods available
- **✅ Cross-Distribution Support** - Works across different Linux distributions

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
- **TrackIR 4 & 5** - Full support via reverse engineering


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
| `winegcc: command not found` | Install Wine development tools: `sudo apt install wine-devel` (Debian/Ubuntu) or `sudo dnf install wine-devel` (Fedora) or `sudo pacman -S wine` (Arch) |
| `bits/libc-header-start.h: No such file or directory` | Install 32-bit headers: `sudo apt install gcc-multilib libc6-dev-i386` (Debian/Ubuntu) or `sudo dnf install glibc-devel.i686 libstdc++-devel.i686` (Fedora) or `sudo pacman -S lib32-glibc lib32-gcc-libs` (Arch) |
| MFC42 installation fails | Use the built-in MFC42 installer in the GUI, or manually run `winetricks mfc42` |
| GUI not displaying on Wayland | Force X11 compatibility: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | Add user to required groups: `sudo usermod -a -G plugdev,input $USER` |
| Application not appearing in launcher | Use `--prefix=/usr/local` instead of `/opt` during installation |
| Firmware extraction fails | Run `./scripts/wine_check.sh` to diagnose Wine issues |

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

# Then build LinuxTrack with explicit 64-bit flags
CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64" ./configure --prefix=/usr/local --disable-ltr-32lib-on-x64
make -j$(nproc)
```

**What the script does:**
- Downloads source code for both missing libraries
- Builds 32-bit versions with proper compiler flags
- Installs to `/usr/lib32/` with correct symlinks
- Uses parallel compilation for fast builds
- Cleans up build files automatically

#### **Common Arch Linux Wine Issues:**

| Problem | Solution |
|---------|----------|
| `wine-staging` conflicts with `wine-stable` | Remove conflicting packages: `sudo pacman -R wine-staging wine-gecko wine-mono winetricks` |
| `lib32-unixodbc` not found | This package doesn't exist in Arch. Remove it from PKGBUILD dependencies |
| Wine build takes hours | Use optimized builds with parallel compilation: `make -j$(nproc)` |
| Missing Wine dependencies | Install complete suite: `wine-stable`, `wine-stable-mono`, `wine-gecko` |
| Wine bridge not working | Ensure wine-stable is installed, not wine-staging |
| Orphaned packages after Wine removal | Clean up: `sudo pacman -Rns $(pacman -Qtdq)` |
| **Missing 32-bit libraries (liblo, mxml)** | **Use the automated build script: `./scripts/build_32bit_libs.sh`** |
| **32-bit/64-bit compilation conflicts** | **Use explicit 64-bit flags: `CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64"`** |
| **Qt5 Makefile not generated** | **Manually generate: `cd src/qt_gui && /usr/bin/qmake-qt5 -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack" ltr_gui.pro`** |

#### **Build System Issues:**

**Problem:** Configure script sets 32-bit flags even when disabled
**Solution:** Use explicit 64-bit flags during configure:
```bash
CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64" ./configure --prefix=/usr/local --disable-ltr-32lib-on-x64
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
./configure --prefix=/usr/local --enable-ltr-32lib-on-x64
make -j$(nproc)
```

This script will:
- Download and build 32-bit versions of `mxml` and `liblo`
- Install them to `/usr/lib32/`
- Create necessary symlinks
- Clean up build files automatically
```

### **Getting Help**
1. **Try AppImage first** - Easiest installation method
2. **Check display server** - Verify X11/Wayland compatibility with `echo $XDG_SESSION_TYPE`
3. **Verify installation** - Run `ltr_gui` to test basic functionality
4. **Check logs** - Look for error messages in terminal output
5. **Distribution guides** - See the `docs/` directory for detailed guides

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
./configure --prefix=/usr/local --enable-debug
make -j$(nproc)
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

This project is a fork of the original LinuxTrack project. The original project was abandoned, and this fork continues development with modern build systems and enhanced compatibility. We maintain full attribution to the original authors and respect their contributions to the open-source community.

**Original Project**: [LinuxTrack by uglyDwarf](https://github.com/uglyDwarf/linuxtrack)  
**License**: MIT License (same as original)  
**Fork Purpose**: Modernization and continued development
