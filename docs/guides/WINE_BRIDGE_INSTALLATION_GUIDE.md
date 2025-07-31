# 🍷 Wine Bridge Installation Guide

**Date**: January 2025  
**Purpose**: Complete guide for installing LinuxTrack with Wine bridge support  
**Status**: **PRODUCTION READY**

---

## 📋 Overview

This guide covers the installation of LinuxTrack X-IR with Wine bridge support for Windows gaming compatibility. The Wine bridge provides TrackIR and FreeTrack protocol support for games running under Wine.

### **Two Installation Methods**

1. **Standard Installation** - Uses Wine development tools (Ubuntu, Fedora, MX Linux)
2. **Prebuilt Installation** - Uses pre-built components (Arch Linux, Manjaro)

---

## 🚀 Quick Start

### **For Arch Linux / Manjaro Users** (Recommended)
```bash
# One-command installation (no Wine development tools required)
./scripts/install/install_arch_prebuilt.sh
```

### **For Ubuntu / Debian / MX Linux Users**
```bash
# Install dependencies
sudo apt install -y build-essential autoconf automake libtool qtbase5-dev qttools5-dev-tools qttools5-dev libqt5x11extras5-dev libopencv-dev libusb-1.0-0-dev libmxml-dev libx11-dev libxrandr-dev bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev wine-staging

# Install Wine development tools
sudo apt install -y wine-devel wine32:i386

# Build and install
autoreconf -fiv
./configure --prefix=/opt
make -j$(nproc)
sudo make install
```

### **For Fedora / RHEL / CentOS Users**
```bash
# Install dependencies
sudo dnf install -y gcc gcc-c++ make autoconf automake libtool qt5-qtbase-devel qt5-qttools-devel qttools5-dev qt5-qtx11extras-devel opencv-devel libusb1-devel libmxml-devel libX11-devel libXrandr-devel bison flex nsis glibc-devel.i686 libstdc++-devel.i686 v4l-utils-devel wine-staging

# Install Wine development tools
sudo dnf install -y wine-devel wine-tools

# Build and install
autoreconf -fiv
./configure --prefix=/opt
make -j$(nproc)
sudo make install
```

---

## 🎯 Installation Methods

### **Method 1: Standard Installation (Wine Development Tools)**

**Supported Distributions**: Ubuntu, Debian, MX Linux, Fedora, RHEL, CentOS

**Requirements**:
- Wine development tools (`wine-devel`, `winegcc`, `wineg++`)
- NSIS for installer creation
- Standard build tools

**Process**:
1. Install dependencies including Wine development tools
2. Build LinuxTrack with Wine bridge support
3. Install to system
4. Wine bridge components are built during installation

**Advantages**:
- ✅ Full control over build process
- ✅ Latest component versions
- ✅ Standard installation method

**Disadvantages**:
- ❌ Requires Wine development tools
- ❌ Complex dependency management
- ❌ Build time required

### **Method 2: Prebuilt Installation (No Wine Development Tools)**

**Supported Distributions**: Arch Linux, Manjaro

**Requirements**:
- Standard build tools (no Wine development tools)
- Pre-built Wine bridge package
- Internet connection for package download

**Process**:
1. Install standard dependencies (no Wine development tools)
2. Download pre-built Wine bridge package
3. Modify build system to use pre-built components
4. Build and install LinuxTrack
5. Install Wine bridge components from package

**Advantages**:
- ✅ No Wine development tools required
- ✅ Faster installation
- ✅ Consistent component versions
- ✅ Works around distribution-specific issues

**Disadvantages**:
- ❌ Requires pre-built package
- ❌ Less control over component versions
- ❌ Internet connection required

---

## 📦 Wine Bridge Components

### **Built Components (32-bit)**
- **NPClient.dll** - TrackIR API compatibility library
- **Controller.exe** - Hotkey controller for pause/resume/recenter
- **Tester.exe** - TrackIR protocol testing application
- **check_data.exe** - Installation verification utility
- **TrackIR.exe** - Fake TrackIR executable required by some games
- **FreeTrackClient.dll** - FreeTrack API compatibility library
- **FreeTrackTester.exe** - FreeTrack protocol testing application

### **Built Components (64-bit)**
- **NPClient64.dll** - TrackIR API compatibility library
- **Tester64.exe** - 64-bit TrackIR protocol testing application

### **Installers**
- **linuxtrack-wine.exe** - Windows NSIS installer for Wine environments

---

## 🔧 Detailed Installation Procedures

### **Arch Linux / Manjaro Prebuilt Installation**

#### **Step 1: Install Dependencies**
```bash
# Install core dependencies (no Wine development tools)
sudo pacman -S --needed base-devel autoconf automake libtool \
    qt5-base qt5-tools qt5-x11extras opencv libusb mxml \
    libx11 libxrandr bison flex lib32-glibc lib32-gcc-libs \
    v4l-utils wine-staging winetricks mfc42 nsis
```

#### **Step 2: Run Automated Installer**
```bash
# Run the automated installation script
./scripts/install/install_arch_prebuilt.sh
```

#### **Step 3: Verify Installation**
```bash
# Check if ltr_gui is available
which ltr_gui

# Launch GUI
ltr_gui
```

### **Ubuntu / Debian / MX Linux Standard Installation**

#### **Step 1: Install Dependencies**
```bash
# Install build dependencies
sudo apt install -y build-essential autoconf automake libtool \
    qtbase5-dev qttools5-dev-tools qttools5-dev libqt5x11extras5-dev \
    libopencv-dev libusb-1.0-0-dev libmxml-dev libx11-dev libxrandr-dev \
    bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev \
    wine-staging winetricks

# Install Wine development tools
sudo apt install -y wine-devel wine32:i386
```

#### **Step 2: Build and Install**
```bash
# Regenerate build system
autoreconf -fiv

# Configure with Wine support
./configure --prefix=/opt

# Build
make -j$(nproc)

# Install
sudo make install

# Add user to required groups
sudo usermod -a -G plugdev $USER
```

#### **Step 3: Verify Installation**
```bash
# Check Wine bridge components
ls -la /opt/lib/linuxtrack/wine_bridge/

# Launch GUI
ltr_gui
```

### **Fedora / RHEL / CentOS Standard Installation**

#### **Step 1: Install Dependencies**
```bash
# Install build dependencies
sudo dnf install -y gcc gcc-c++ make autoconf automake libtool \
    qt5-qtbase-devel qt5-qttools-devel qttools5-dev qt5-qtx11extras-devel \
    opencv-devel libusb1-devel libmxml-devel libX11-devel libXrandr-devel \
    bison flex nsis glibc-devel.i686 libstdc++-devel.i686 v4l-utils-devel \
    wine-staging winetricks mfc42

# Install Wine development tools
sudo dnf install -y wine-devel wine-tools
```

#### **Step 2: Build and Install**
```bash
# Regenerate build system
autoreconf -fiv

# Configure with Wine support
./configure --prefix=/opt

# Build
make -j$(nproc)

# Install
sudo make install

# Add user to required groups
sudo usermod -a -G plugdev $USER
```

#### **Step 3: Verify Installation**
```bash
# Check Wine bridge components
ls -la /opt/lib/linuxtrack/wine_bridge/

# Launch GUI
ltr_gui
```

---

## 🎮 Gaming Compatibility

### **Supported Protocols**
- **TrackIR**: Full compatibility with NPClient.dll/NPClient64.dll
- **FreeTrack**: Full compatibility with FreeTrackClient.dll
- **OpenTrack**: Compatible via TrackIR/FreeTrack protocols

### **Supported Games**
- **Flight Simulators**: DCS World, IL-2 series, X-Plane
- **Racing Games**: Euro Truck Simulator 2, American Truck Simulator
- **Space Sims**: Elite Dangerous, Star Citizen
- **Any TrackIR/FreeTrack compatible game**

### **Testing Applications**
- **Tester.exe/Tester64.exe**: TrackIR protocol testing
- **FreeTrackTester.exe**: FreeTrack protocol testing
- **Controller.exe**: Hotkey testing and configuration

---

## 🔍 Troubleshooting

### **Common Issues**

#### **Wine Development Tools Not Found**
```bash
# Ubuntu/Debian/MX
sudo apt install wine-devel wine32:i386

# Fedora/RHEL
sudo dnf install wine-devel wine-tools

# Arch Linux (use prebuilt method instead)
./scripts/install/install_arch_prebuilt.sh
```

#### **NSIS Not Found**
```bash
# Ubuntu/Debian/MX
sudo apt install nsis

# Fedora/RHEL
sudo dnf install nsis

# Arch Linux
sudo pacman -S nsis
```

#### **32-bit Libraries Missing**
```bash
# Ubuntu/Debian/MX
sudo apt install gcc-multilib libc6-dev-i386

# Fedora/RHEL
sudo dnf install glibc-devel.i686 libstdc++-devel.i686

# Arch Linux
sudo pacman -S lib32-glibc lib32-gcc-libs
```

#### **Wine Bridge Components Not Working**
```bash
# Check Wine installation
wine --version

# Check Wine bridge components
ls ~/.wine/drive_c/windows/system32/NPClient.dll

# Run verification
wine ~/.wine/drive_c/windows/system32/check_data.exe
```

#### **Permission Issues**
```bash
# Add user to required groups
sudo usermod -a -G plugdev $USER

# Log out and back in for changes to take effect
```

### **Getting Help**
1. **Check the docs**: Start with the [Quick Start](#-quick-start) section
2. **Verify installation**: Run `ltr_gui` to test basic functionality
3. **Check logs**: Look for error messages in terminal output
4. **Hardware test**: Verify USB devices are detected
5. **Distribution guides**: See the `docs/` directory for detailed guides

---

## 📚 Additional Resources

### **Documentation**
- **[Main README](../README.md)** - Project overview and quick start
- **[Wine Bridge Packaging Plan](../technical/WINE_BRIDGE_PACKAGING_PLAN.md)** - Technical implementation details
- **[Troubleshooting Guide](../troubleshooting/TROUBLESHOOTING.md)** - Problem resolution
- **[Testing Guide](../testing/README.md)** - Testing procedures

### **Scripts**
- **[Arch Linux Installer](../../scripts/install/install_arch_prebuilt.sh)** - Automated Arch Linux installation
- **[Health Check](../../scripts/install/linuxtrack_health_check.sh)** - System health verification
- **[Wine Check](../../scripts/wine_check.sh)** - Wine compatibility checker

### **Support**
- **GitHub Issues**: [LinuxTrack X-IR Issues](https://github.com/fwfa123/linuxtrackx-ir/issues)
- **Community**: [OpenTrack Community](https://github.com/opentrack/opentrack)
- **Documentation**: See the `docs/` directory for comprehensive guides

---

## 🎯 Quick Reference

### **Installation Commands by Distribution**

| Distribution | Command |
|--------------|---------|
| **Arch Linux** | `./scripts/install/install_arch_prebuilt.sh` |
| **Manjaro** | `./scripts/install/install_arch_prebuilt.sh` |
| **Ubuntu** | `sudo apt install wine-devel && autoreconf -fiv && ./configure --prefix=/opt && make -j$(nproc) && sudo make install` |
| **Debian** | `sudo apt install wine-devel && autoreconf -fiv && ./configure --prefix=/opt && make -j$(nproc) && sudo make install` |
| **MX Linux** | `sudo apt install wine-devel && autoreconf -fiv && ./configure --prefix=/opt && make -j$(nproc) && sudo make install` |
| **Fedora** | `sudo dnf install wine-devel && autoreconf -fiv && ./configure --prefix=/opt && make -j$(nproc) && sudo make install` |
| **RHEL** | `sudo dnf install wine-devel && autoreconf -fiv && ./configure --prefix=/opt && make -j$(nproc) && sudo make install` |

### **Verification Commands**
```bash
# Check installation
which ltr_gui

# Launch GUI
ltr_gui

# Check Wine bridge components
ls -la ~/.wine/drive_c/windows/system32/NPClient.dll

# Health check
./scripts/install/linuxtrack_health_check.sh
```

---

**Ready to start tracking?** Follow the [Quick Start](#-quick-start) section for your distribution! 