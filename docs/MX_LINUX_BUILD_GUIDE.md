# MX Linux Build Guide for LinuxTrack

**Distribution**: MX Linux 23.6+ (Debian-based)  
**Last Updated**: July 2025  
**Status**: Tested and Verified

## 🎯 Overview

This guide addresses the specific build issues encountered on MX Linux and similar Debian-based distributions. MX Linux has some unique characteristics that require special handling during the LinuxTrack build process.

## 🚨 Common Issues on MX Linux

### Issue 1: `aclocal-1.17: command not found`
**Error**: `WARNING: 'aclocal-1.17' is missing on your system`

**Cause**: MX Linux uses a different version of automake than what the build system expects.

**Solution**: The build script automatically creates symlinks to the available version.

### Issue 2: 32-bit Development Libraries Missing
**Error**: `fatal error: bits/libc-header-start.h: No such file or directory`

**Cause**: MX Linux doesn't include 32-bit development libraries by default.

**Solution**: Install the required 32-bit development packages.

## 🚀 Quick Start (Recommended)

### Method 1: Automated Build Script
```bash
# Clone the repository
git clone <repository-url>
cd linuxtrack-clean-june14

# Run the MX Linux specific build script
./dev-scripts/build_mx_linux.sh
```

This script automatically:
- Installs all required dependencies
- Fixes autotools version issues
- Installs 32-bit development libraries
- Configures and builds LinuxTrack
- Verifies the build

### Method 2: Manual Build with Smart Script
```bash
# Clone the repository
git clone <repository-url>
cd linuxtrack-clean-june14

# Use the smart wine bridge build script
./dev-scripts/build_wine_bridge.sh
```

This script will:
- Detect and fix autotools issues
- Offer to install missing dependencies
- Choose the best available build method

## 🔧 Manual Installation

If you prefer to install dependencies manually:

### Step 1: Install Essential Dependencies
```bash
sudo apt update
sudo apt install -y build-essential autoconf automake libtool
sudo apt install -y qtbase5-dev qttools5-dev-tools
sudo apt install -y libopencv-dev libusb-1.0-0-dev libmxml-dev
sudo apt install -y libx11-dev libxrandr-dev
```

### Step 2: Fix Autotools Version Issues
```bash
# Check available automake versions
ls /usr/bin/automake-*

# Create symlinks if needed (replace X.Y with your version)
sudo ln -sf /usr/bin/automake-X.Y /usr/bin/automake-1.17
sudo ln -sf /usr/bin/aclocal-X.Y /usr/bin/aclocal-1.17
```

### Step 3: Install 32-bit Development Libraries
```bash
sudo apt install -y gcc-multilib g++-multilib libc6-dev-i386
sudo apt install -y lib32gcc-s1 lib32stdc++6
```

### Step 4: Install Wine Bridge Dependencies
```bash
# MinGW cross-compilation (recommended)
sudo apt install -y mingw-w64 gcc-mingw-w64 g++-mingw-w64

# NSIS installer
sudo apt install -y nsis

# Wine development tools (optional)
sudo apt install -y wine-development wine32-development wine64-development
```

### Step 5: Build LinuxTrack
```bash
# Clean any previous builds
make distclean 2>/dev/null || true
rm -f configure aclocal.m4 config.h.in
rm -rf autom4te.cache/

# Regenerate build system
autoreconf -fiv

# Configure and build
./configure --prefix=/opt/linuxtrack
make -j$(nproc)
```

## 🔍 Troubleshooting

### Problem: Still getting `aclocal-1.17` errors
**Solution**:
```bash
# Check what automake versions are available
ls /usr/bin/automake-*

# Create manual symlinks
sudo ln -sf /usr/bin/automake-1.16 /usr/bin/automake-1.17
sudo ln -sf /usr/bin/aclocal-1.16 /usr/bin/aclocal-1.17
```

### Problem: 32-bit compilation still failing
**Solution**:
```bash
# Install additional 32-bit packages
sudo apt install -y libc6-dev-i386 libstdc++6-dev-i386
sudo apt install -y gcc-multilib g++-multilib

# Verify installation
ls /usr/include/i386-linux-gnu/bits/libc-header-start.h
```

### Problem: Wine bridge not building
**Solution**:
```bash
# Try MinGW method instead of wine-devel
./dev-scripts/build_wine_bridge.sh

# Or install wine development tools from backports
sudo apt install -y wine-development wine32-development wine64-development
```

### Problem: Qt5 not found
**Solution**:
```bash
# Install Qt5 development packages
sudo apt install -y qtbase5-dev qttools5-dev-tools qt5-qmake

# Verify Qt5 installation
qmake-qt5 --version
```

## 📋 System Requirements

### Minimum Requirements
- **OS**: MX Linux 23.6+ (or similar Debian-based)
- **RAM**: 2GB available
- **Disk Space**: 1GB free space
- **Internet**: Required for dependency installation

### Recommended Requirements
- **RAM**: 4GB+ available
- **CPU**: Multi-core processor
- **Disk Space**: 2GB+ free space

## 🔧 Configuration Options

### Build Configuration
```bash
# Standard installation
./configure --prefix=/opt/linuxtrack

# With debugging enabled
./configure --prefix=/opt/linuxtrack --enable-debug

# With specific Qt5 path
./configure --prefix=/opt/linuxtrack QMAKE=/usr/bin/qmake-qt5
```

### Wine Bridge Options
```bash
# Use MinGW cross-compilation (recommended for MX Linux)
./configure --prefix=/opt/linuxtrack

# Use wine-devel (if available)
./configure --prefix=/opt/linuxtrack --with-wine-devel

# Disable wine bridge entirely
./configure --prefix=/opt/linuxtrack --disable-wine-bridge
```

## 🎮 Post-Installation

### Install LinuxTrack
```bash
# Use the installation script (recommended)
sudo ./install.sh

# Or manually:
sudo make install
sudo usermod -a -G plugdev $USER
```

### Add User to Required Groups
```bash
sudo usermod -a -G plugdev $USER
# Log out and log back in for group changes to take effect
```

### Verify Installation
```bash
# Run the verification script
./verify_installation.sh

# Test the GUI
ltr_gui
```

## 📞 Support

### Getting Help
1. **Check this guide first** - Most issues are covered here
2. **Run the verification script** - `./verify_installation.sh`
3. **Check the logs** - Look for error messages in the build output
4. **Use the automated scripts** - They handle most issues automatically

### Common Error Messages and Solutions

| Error | Solution |
|-------|----------|
| `aclocal-1.17: command not found` | Run `./dev-scripts/build_mx_linux.sh` |
| `bits/libc-header-start.h: No such file` | Install 32-bit dev libraries |
| `winegcc: command not found` | Use MinGW method instead |
| `qmake-qt5: command not found` | Install Qt5 development packages |

### Reporting Issues
If you encounter issues not covered in this guide:
1. Run `./verify_installation.sh` and include the output
2. Include your MX Linux version: `cat /etc/os-release`
3. Include the full build log
4. Mention which build method you used

## 🔄 Updates and Maintenance

### Updating LinuxTrack
```bash
# Pull latest changes
git pull origin master

# Rebuild with the MX Linux script
./dev-scripts/build_mx_linux.sh
```

### System Updates
After system updates, if LinuxTrack stops working:
```bash
# Run the quick recovery script
./linuxtrack_quick_recovery.sh

# Or rebuild if needed
./dev-scripts/build_mx_linux.sh
```

---

**Note**: This guide is specifically tailored for MX Linux. For other distributions, see the main [WINE_BRIDGE_BUILD_GUIDE.md](WINE_BRIDGE_BUILD_GUIDE.md). 