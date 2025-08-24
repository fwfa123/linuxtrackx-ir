# LinuxTrack User Guides

## Overview

This directory contains user-facing guides for LinuxTrack Qt5 modernization, including installation instructions, setup procedures, and maintenance guides.

## Quick Start Guide

### Essential Commands
```bash
# Install dependencies and build
./build.sh --deps --clean --install

# Launch application
./run_qt5_gui.sh

# Health check
./linuxtrack_health_check.sh
```

### Success Indicators
- ✅ Build completes without errors
- ✅ GUI launches successfully
- ✅ TrackIR hardware detected
- ✅ Head tracking responds to movement

## Guide Files

### Current User Guides
- **`MX_LINUX_BUILD_GUIDE.md`** - MX Linux specific build guide (8KB)
- **`SETUP_GUIDE_QT5_SUCCESS.md`** - Proven setup instructions (9KB)
- **`SYSTEM_MAINTENANCE_GUIDE.md`** - System maintenance procedures (10KB)
- **`SUCCESS_SUMMARY.md`** - Project success status (3KB)
- **`FINAL_INTEGRATION_SUMMARY.md`** - Integration completion summary (5KB)
- **`MX_LINUX_FIX_SUMMARY.md`** - MX Linux specific fixes (5KB)
 - **`WINE_BRIDGE_INSTALLATION_GUIDE.md`** - Wine bridge installation steps
 - See also: `../AppImage_WineBridge.md` for AppImage 32-bit/64-bit Wine usage

## Installation Guides

### Distribution-Specific Installation

#### MX Linux (Recommended)
**Primary File**: `MX_LINUX_BUILD_GUIDE.md`

**Advantages:**
- Excellent Qt5 compatibility
- Minimal dependency conflicts
- Stable build environment
- Comprehensive package support

**Installation Steps:**
1. Install dependencies: `sudo apt install build-essential qtbase5-dev mingw-w64`
2. Build system: `autoreconf -fiv && ./configure --prefix=/opt`
3. Compile: `make -j$(nproc)`
4. Install: `sudo make install`

#### Ubuntu/Debian
**Key Requirements:**
- 32-bit development libraries: `sudo apt install gcc-multilib libc6-dev-i386`
- Qt5 packages: `sudo apt install qtbase5-dev qttools5-dev-tools`
- MinGW support: `sudo apt install mingw-w64`

**Common Issues:**
- Missing 32-bit headers → Install multilib packages
- Qt5 detection problems → Use `qt5-qmake` command
- Wine bridge issues → Install wine development packages

#### Fedora/RHEL
**Package Installation:**
```bash
sudo dnf install gcc gcc-c++ make autoconf automake libtool
sudo dnf install qt5-qtbase-devel qt5-qttools-devel
sudo dnf install mingw64-gcc mingw64-gcc-c++
```

#### Arch Linux
**Package Installation:**
```bash
sudo pacman -S --needed base-devel autoconf automake libtool
sudo pacman -S qt5-base qt5-tools mingw-w64-gcc
```

## Setup and Configuration

### Initial Setup
**Primary File**: `SETUP_GUIDE_QT5_SUCCESS.md`

**Setup Steps:**
1. **Build System**: Complete compilation and installation
2. **User Groups**: Add user to plugdev group
3. **udev Rules**: Install TrackIR permission rules
4. **Configuration**: Create initial configuration files
5. **Testing**: Verify hardware detection and tracking

### Hardware Configuration
**TrackIR Setup:**
1. **Physical Connection**: Connect TrackIR device via USB
2. **Device Detection**: Verify with `lsusb | grep 131d`
3. **Permission Setup**: Install udev rules for user access
4. **IR LEDs**: Ensure TrackClip Pro or IR LEDs are active
5. **Camera Calibration**: Follow GUI calibration procedure

### Display Configuration
**X11 Mode (Recommended):**
```bash
# Launch with X11 mode
./run_qt5_gui.sh --force-x11

# Set environment variables
export QT_QPA_PLATFORM=xcb
export XDG_SESSION_TYPE=x11
```

**Wayland Compatibility:**
- Use XWayland mode for optimal performance
- Set appropriate environment variables
- Check display server compatibility

## Maintenance and Updates

### Regular Maintenance
**Primary File**: `SYSTEM_MAINTENANCE_GUIDE.md`

**Maintenance Tasks:**
1. **System Updates**: Keep dependencies current
2. **Health Checks**: Run periodic diagnostics
3. **Configuration Backups**: Preserve working settings
4. **Log Monitoring**: Check for issues and warnings

### Update Procedures
**Before System Updates:**
1. Backup configuration: `cp ~/.config/linuxtrack/ ~/.config/linuxtrack.backup/`
2. Run health check: `./linuxtrack_health_check.sh`
3. Document current state

**After System Updates:**
1. Test functionality: `./run_qt5_gui.sh`
2. Check hardware detection: `lsusb | grep 131d`
3. Verify tracking: Test head movement response
4. Run recovery if needed: `./linuxtrack_quick_recovery.sh`

### Performance Optimization
**System Tuning:**
1. **CPU Priority**: Set appropriate process priorities
2. **Memory Management**: Monitor memory usage
3. **USB Latency**: Optimize USB polling rates
4. **Display Performance**: Use X11 mode for best results

## Troubleshooting

### Common Issues
1. **Build Failures** → Check dependencies and Qt5 installation
2. **Hardware Detection** → Verify udev rules and user groups
3. **Permission Errors** → Add user to plugdev group
4. **Display Issues** → Use X11 mode or check display server

### Quick Fixes
```bash
# Rebuild Qt5 components
cd src/qt_gui && qmake-qt5 ltr_gui.pro && make -j$(nproc)

# Fix permissions
sudo usermod -a -G plugdev,input $USER

# Reinstall udev rules
sudo cp 99-TIR.rules /lib/udev/rules.d/
sudo udevadm control --reload-rules
```

## Success Verification

### Project Status
**Primary File**: `SUCCESS_SUMMARY.md`

**Success Indicators:**
- ✅ **Qt5 Migration**: Complete GUI modernization
- ✅ **Hardware Support**: TrackIR 4 & 5 compatibility
- ✅ **Build System**: Modern autotools configuration
- ✅ **Cross-Platform**: MinGW Windows support
- ✅ **Security**: PIE and stack protector enabled

### Integration Status
**Primary File**: `FINAL_INTEGRATION_SUMMARY.md`

**Integration Features:**
- **MinGW Support**: Windows compatibility components
- **Distribution Support**: Multi-distribution compatibility
- **Hardware Integration**: Full TrackIR support
- **Display Compatibility**: X11 and Wayland support

## Getting Help

### Self-Help Resources
1. **Health Check Script**: Automated diagnostics
2. **Quick Recovery Script**: Automatic problem resolution
3. **Troubleshooting Guides**: Issue-specific solutions
4. **Documentation Index**: Navigation to relevant resources

### When to Seek Help
1. **Unusual Errors**: Not covered in existing guides
2. **Hardware Issues**: New or unsupported devices
3. **Distribution Problems**: OS-specific issues
4. **Performance Issues**: Optimization and tuning

---

**Note**: Start with the distribution-specific guide for your OS, then refer to the setup guide for detailed instructions. The maintenance guide provides ongoing support for system updates and optimization. 