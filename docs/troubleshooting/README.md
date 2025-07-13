# LinuxTrack Troubleshooting Guide

## Overview

This directory contains comprehensive troubleshooting documentation for LinuxTrack Qt5 modernization. These guides help resolve common issues encountered during installation, configuration, and operation.

## Quick Troubleshooting Reference

### Emergency Recovery
```bash
# Quick health check
./linuxtrack_health_check.sh

# Automatic recovery
./linuxtrack_quick_recovery.sh

# Manual Qt5 rebuild
cd src/qt_gui && qmake-qt5 ltr_gui.pro && make -j$(nproc)

# Check TrackIR hardware
lsusb | grep 131d
```

### Most Common Issues
1. **System upgrade broke LinuxTrack** → [QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)
2. **TrackIR permission errors** → [TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)
3. **Firmware extraction fails** → [FIRMWARE_EXTRACTION_TROUBLESHOOTING.md](FIRMWARE_EXTRACTION_TROUBLESHOOTING.md)
4. **ApplicationID shows ((null)) in gamedata.txt** → [APPLICATIONID_PARSING_ISSUE.md](APPLICATIONID_PARSING_ISSUE.md)
5. **Application won't launch** → [QUICK_TROUBLESHOOTING_QT5.md](QUICK_TROUBLESHOOTING_QT5.md)
6. **Display issues on Wayland** → [WAYLAND_COMPATIBILITY_ISSUE.md](WAYLAND_COMPATIBILITY_ISSUE.md)

## Troubleshooting Files

### Current Troubleshooting Documentation
- **`TROUBLESHOOTING.md`** - Comprehensive troubleshooting guide (14KB)
- **`QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md`** - System upgrade conflicts (9KB)
- **`TRACKIR_PERMISSION_TROUBLESHOOTING.md`** - Hardware permission issues (9KB)
- **`FIRMWARE_EXTRACTION_TROUBLESHOOTING.md`** - TrackIR firmware extraction issues (8KB)
- **`APPLICATIONID_PARSING_ISSUE.md`** - ApplicationID parsing bug causing ((null)) in gamedata.txt (12KB)
- **`QUICK_TROUBLESHOOTING_QT5.md`** - Quick fixes for common issues (3KB)
- **`WAYLAND_COMPATIBILITY_ISSUE.md`** - Display server compatibility (2KB)

## Issue Categories

### System Upgrade Issues
**Primary File**: `QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md`

**Common Problems:**
- Qt5/Qt6 version conflicts after system updates
- Library loading errors
- Build system failures
- GUI launch failures

**Solutions:**
- Rebuild Qt5 components
- Fix library paths
- Update udev rules
- Reinstall dependencies

### Hardware Issues
**Primary File**: `TRACKIR_PERMISSION_TROUBLESHOOTING.md`

**Common Problems:**
- TrackIR device not detected
- Permission denied errors
- USB access issues
- IR tracking not working

**Solutions:**
- Add user to plugdev group
- Install/update udev rules
- Check USB connections
- Verify IR LED functionality

### Display Issues
**Primary File**: `WAYLAND_COMPATIBILITY_ISSUE.md`

**Common Problems:**
- GUI not displaying on Wayland
- Display artifacts
- Performance issues
- X11 compatibility problems

**Solutions:**
- Use X11 mode: `./run_qt5_gui.sh --force-x11`
- Set environment variables
- Check display server compatibility
- Verify OpenGL support

### Firmware and Game Data Issues
**Primary Files**: `FIRMWARE_EXTRACTION_TROUBLESHOOTING.md`, `APPLICATIONID_PARSING_ISSUE.md`

**Common Problems:**
- Firmware extraction fails
- ApplicationID shows ((null)) in gamedata.txt
- Games don't get enhanced TrackIR support
- Missing or corrupted firmware files

**Solutions:**
- Use manual firmware extraction process
- Fix ApplicationID parsing bug in game_data.c
- Verify all required firmware files are present
- Check gamedata.txt for correct ApplicationID entries

### Build Issues
**Primary File**: `QUICK_TROUBLESHOOTING_QT5.md`

**Common Problems:**
- Missing dependencies
- Qt5 detection failures
- Compilation errors
- Library linking issues

**Solutions:**
- Install required packages
- Verify Qt5 installation
- Check compiler compatibility
- Fix library paths

## Diagnostic Commands

### System Information
```bash
# Check Qt5 installation
qmake-qt5 --version
qt5-qmake --version

# Check hardware detection
lsusb | grep 131d
dmesg | grep -i trackir

# Check library dependencies
ldd src/qt_gui/ltr_gui
ldd src/.libs/libltr.so.0

# Check permissions
groups $USER
ls -la /dev/bus/usb/
```

### Health Checks
```bash
# Comprehensive health check
./linuxtrack_health_check.sh

# Component-specific checks
./linuxtrack_health_check.sh --qt5
./linuxtrack_health_check.sh --hardware
./linuxtrack_health_check.sh --permissions
./linuxtrack_health_check.sh --libraries
```

### Recovery Procedures
```bash
# Quick recovery
./linuxtrack_quick_recovery.sh

# Manual Qt5 rebuild
cd src/qt_gui
qmake-qt5 ltr_gui.pro
make clean && make -j$(nproc)

# Reinstall udev rules
sudo cp 99-TIR.rules /lib/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
```

## Distribution-Specific Issues

### Ubuntu/Debian
- **32-bit library issues** → Install `gcc-multilib libc6-dev-i386`
- **Qt5 detection problems** → Use `qt5-qmake` instead of `qmake-qt5`
- **Wine bridge issues** → Install wine development packages

### Fedora/RHEL
- **Library conflicts** → Use `dnf` instead of `yum`
- **Qt5 packages** → Install `qt5-qtbase-devel` packages
- **Development tools** → Install `mingw64-gcc` for Windows components

### Arch Linux
- **Package names** → Use `qt5-base` instead of `qtbase5-dev`
- **Build tools** → Install `base-devel` group
- **MinGW support** → Install `mingw-w64-gcc`

## Prevention and Maintenance

### Regular Maintenance
1. **System Updates** - Keep dependencies current
2. **Health Checks** - Run periodic diagnostics
3. **Backup Configuration** - Preserve working settings
4. **Monitor Logs** - Check for early warning signs

### Best Practices
1. **Test After Updates** - Verify functionality after system changes
2. **Document Changes** - Keep track of modifications
3. **Use Virtual Environments** - Isolate development work
4. **Regular Backups** - Preserve working configurations

## Getting Help

### Self-Help Resources
1. **Health Check Script** - Automated diagnostics
2. **Quick Recovery Script** - Automatic problem resolution
3. **Troubleshooting Guides** - Issue-specific solutions
4. **Documentation Index** - Navigation to relevant resources

### When to Seek External Help
1. **Unusual Error Messages** - Not covered in existing guides
2. **Hardware Compatibility** - New or unsupported devices
3. **Distribution Issues** - Problems specific to your OS
4. **Performance Problems** - Optimization and tuning

---

**Note**: Start with the quick troubleshooting guide for immediate issues, then refer to specific guides for detailed solutions. The health check script provides automated diagnostics for most common problems. 