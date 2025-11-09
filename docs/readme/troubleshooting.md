# LinuxTrack Troubleshooting Guide

This guide covers the most common issues across all supported distributions. For distribution-specific issues, see the individual build guides.

## Quick Diagnosis

### 1. Check Your Build
```bash
# Verify installation
ldconfig -p | grep linuxtrack

# Test basic functionality
ltr_server1 --help

# Test GUI
ltr_gui

# Check for Wine bridge (if built)
ls /usr/local/lib/linuxtrack/wine_bridge/
```

### 2. Check Display Environment
```bash
# Check display server
echo $XDG_SESSION_TYPE

# If Wayland, test X11 compatibility
QT_QPA_PLATFORM=xcb ltr_gui
```

### 3. Check Device Permissions
```bash
# Check if user is in required groups
groups $USER

# Add if missing (logout/login required)
sudo usermod -a -G plugdev,input $USER
```

## Common Issues

### Build Failures

| Problem | Symptom | Solution |
|---------|---------|----------|
| Missing 32-bit headers | `bits/libc-header-start.h: No such file or directory` | **REQUIRED for Wine support**: Install 32-bit dev headers for your distro |
| Qt5 tools not found | `qmake: command not found` | Add Qt5 bin directory to PATH (see distro guide) |
| Wine dev tools missing | `winegcc: command not found` | Install Wine development packages |
| OpenCV detection failed | Build succeeds but no webcam | Install OpenCV dev packages (non-critical) |
| ldconfig permission denied | Warning during install | Use `-DENABLE_LDCONFIG=OFF` for packaging |

### Runtime Issues

| Problem | Symptom | Solution |
|---------|---------|----------|
| Library not found | `Couldn't load library 'libwc.so.0'` | Run `sudo ldconfig` |
| GUI not displaying | Window doesn't appear | Force X11: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied | Device access error | Add user to `plugdev,input` groups |
| TrackIR not detected | No tracking | Check USB device: `lsusb \| grep Track` |
| Wine bridge fails | Windows games don't track | Check Wine installation and MFC42 setup |

### Wine-Specific Issues

| Problem | Symptom | Solution |
|---------|---------|----------|
| MFC42 installation fails | TrackIR not working in Wine | Use GUI MFC42 installer or manual winetricks |
| 32-bit Wine prefix issues | Wine errors | Ensure wine-stable (not staging) on Arch |
| Wine bridge components missing | No NPClient.dll.so | Check 32-bit library build |
| Wine version conflicts | Incompatible Wine version | Use Wine 6-7 series |

### Distribution-Specific Quick Fixes

#### Debian/Ubuntu
```bash
# 32-bit headers (critical for Wine)
sudo apt install gcc-multilib libc6-dev-i386

# Qt5 tools (rare)
sudo apt install qttools5-dev-tools
```

#### Arch Linux
```bash
# 32-bit libraries (critical for Wine)
./scripts/build_32bit_libs.sh

# Wine (use stable, not staging)
sudo pacman -S wine-stable
```

#### Fedora/RHEL
```bash
# Qt5 tools PATH (critical)
export PATH="/usr/lib64/qt5/bin:$PATH"

# 32-bit headers (critical for Wine)
sudo dnf install glibc-devel.i686 gcc.i686

# 32-bit libraries
sudo dnf install gcc-c++.i686 zlib-ng-compat-devel.i686
```

## Advanced Diagnostics

### Wine Bridge Debugging
```bash
# Test Wine environment
wine --version

# Check MFC42 installation
wine reg query "HKLM\\Software\\Classes\\CLSID\\{98BF1CE3-0x0000-0000-0000-000000000000}"

# Test TrackIR DLL loading
cd /usr/local/lib/linuxtrack/wine_bridge/
wine NPClient64.dll  # Should not crash
```

### Build System Debugging
```bash
# Check CMake configuration
cd build
cmake .. --system-information | grep -A 10 "CMAKE_SYSTEM"

# Verbose build
cmake --build . -j$(nproc) -v

# Check library dependencies
ldd /usr/local/bin/ltr_gui
```

### Device Debugging
```bash
# Check USB devices
lsusb
lsusb -v | grep -A 10 TrackIR

# Check device permissions
ls -l /dev/bus/usb/ | head -5
udevadm info -a -n /dev/bus/usb/XXX/YYY  # Replace with device numbers
```

## Getting Help

1. **Check the docs**: See `docs/readme/` for your distribution
2. **Test with AppImage**: Try the AppImage first to verify hardware works
3. **Search existing issues**: Check GitLab issues for similar problems
4. **Provide diagnostics**: Include output from the commands above when reporting issues

## Known Limitations

- **Wayland**: Some features work better with X11 (force with `QT_QPA_PLATFORM=xcb`)
- **32-bit Wine**: Required for TrackIR compatibility, even on 64-bit systems
- **Qt5 themes**: Some desktop themes may affect GUI appearance
- **Multiple TrackIR devices**: Only one device supported simultaneously

## Emergency Recovery

If everything is broken:

```bash
# Clean reinstall
sudo rm -rf /usr/local/lib/linuxtrack/
sudo rm -rf /usr/local/bin/ltr_*
sudo rm -rf /usr/local/share/linuxtrack/

# Rebuild from scratch
rm -rf build/
mkdir build && cd build
cmake .. [your options]
cmake --build . -j$(nproc)
sudo cmake --install .

# Update library cache
sudo ldconfig
```
