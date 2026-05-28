# LinuxTrack Troubleshooting Guide

## Overview

This directory contains troubleshooting documentation for LinuxTrack. The current release stack is **Qt6 + CMake**.

## Quick Troubleshooting Reference

### Emergency Recovery
```bash
# Automatic recovery
./scripts/install/linuxtrack_quick_recovery.sh

# Manual CMake rebuild
mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)

# Check TrackIR hardware
lsusb | grep 131d
```

### Most Common Issues
1. **TrackIR permission errors** → [TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)
2. **Firmware extraction fails** → [FIRMWARE_EXTRACTION_TROUBLESHOOTING.md](FIRMWARE_EXTRACTION_TROUBLESHOOTING.md)
3. **ApplicationID shows `((null))` in gamedata.txt** → [APPLICATIONID_PARSING_ISSUE.md](APPLICATIONID_PARSING_ISSUE.md)
4. **Application won't launch** → [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
5. **Display issues on Wayland** → [WAYLAND_COMPATIBILITY_ISSUE.md](WAYLAND_COMPATIBILITY_ISSUE.md)
6. **Steam (Flatpak) no tracking / debug logs** → [STEAM_FLATPAK_DEBUG_LOGS.md](STEAM_FLATPAK_DEBUG_LOGS.md)
7. **Per-game Wine/Proton workarounds** → [../GAME_WORKAROUNDS.md](../GAME_WORKAROUNDS.md)

## Troubleshooting Files

- **`TROUBLESHOOTING.md`** - Comprehensive troubleshooting guide
- **`TRACKIR_PERMISSION_TROUBLESHOOTING.md`** - Hardware permission issues
- **`FIRMWARE_EXTRACTION_TROUBLESHOOTING.md`** - TrackIR firmware extraction issues
- **`APPLICATIONID_PARSING_ISSUE.md`** - ApplicationID parsing bug causing `((null))` in gamedata.txt
- **`WAYLAND_COMPATIBILITY_ISSUE.md`** - Display server compatibility
- **`STEAM_FLATPAK_DEBUG_LOGS.md`** - Flatpak Steam: Flatseal tracking fix, `/opt` notes, Proton logs
- **[`../GAME_WORKAROUNDS.md`](../GAME_WORKAROUNDS.md)** - Per-title hints (e.g. Arma 2: omit FreeTrackClient.dll)

## Issue Categories

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
- Display artifacts or blank windows
- Performance issues on Wayland

**Solutions:**
- Use X11 mode: `./scripts/test/run_gui.sh --force-x11` or `./scripts/test/run_gui_x11.sh`
- Set `QT_QPA_PLATFORM=xcb` to force XWayland

### Firmware and Game Data Issues
**Primary Files**: `FIRMWARE_EXTRACTION_TROUBLESHOOTING.md`, `APPLICATIONID_PARSING_ISSUE.md`

**Common Problems:**
- Firmware extraction fails
- ApplicationID shows `((null))` in gamedata.txt
- Games don't get enhanced TrackIR support
- Missing or corrupted firmware files

**Solutions:**
- Use manual firmware extraction process
- Fix ApplicationID parsing bug in game_data.c
- Verify all required firmware files are present
- Check gamedata.txt for correct ApplicationID entries

### Build / Launch Issues
**Primary File**: `TROUBLESHOOTING.md`

**Common Problems:**
- Missing Qt6 or CMake dependencies
- Compilation errors
- Library linking issues

**Solutions:**
- See per-distro install guides in `docs/readme/`
- Verify Qt6 development packages are installed
- Check CMake output for missing `find_package` dependencies

## Diagnostic Commands

### System Information
```bash
# Check Qt6 installation
qmake6 --version || qmake --version

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
ltr_gui
lsusb | grep 131d
groups "$USER"
```

### Recovery Procedures
```bash
# Quick recovery
./scripts/install/linuxtrack_quick_recovery.sh

# Manual CMake rebuild
mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)

# Reinstall udev rules
sudo cp 99-TIR.rules /lib/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
```

## Distribution-Specific Issues

### Ubuntu/Debian
- **32-bit library issues** → Install `gcc-multilib libc6-dev-i386`
- **Qt6 detection problems** → Install `qt6-base-dev` and `libqt6*-dev`
- **Wine bridge issues** → Install wine development packages

### Fedora/RHEL
- **Library conflicts** → Use `dnf` for package management
- **Qt6 packages** → Install `qt6-qtbase-devel` packages
- **Development tools** → Install `mingw64-gcc` for Windows components

### Arch Linux
- **Package names** → Use `qt6-base` for Qt6 development
- **Build tools** → Install `base-devel` group
- **MinGW support** → Install `mingw-w64-gcc`

## Prevention and Maintenance

### Regular Maintenance
1. **System Updates** - Keep dependencies current
2. **Health Checks** - Run periodic diagnostics
3. **Backup Configuration** - Preserve working settings
4. **Monitor Logs** - Check for early warning signs

---

**Note**: Start with the quick troubleshooting reference above, then follow links to issue-specific guides for detailed solutions.
