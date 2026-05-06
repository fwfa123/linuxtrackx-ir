# LinuxTrack Troubleshooting Guide

This guide covers the most common issues across all supported distributions. For distribution-specific issues, see the individual build guides.

## Wine32 - 64 bit install issue
**[Wine INstall](scripts/dev/README_wine_dev.md)**

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
ls /opt/lib/linuxtrack/wine_bridge/
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
| Qt6 CMake config not found | `Could not find a package configuration file provided by "Qt6"` | **REQUIRED**: Install `qt6-base-dev` (provides `Qt6Config.cmake`). See distro guide for complete Qt6 package list |
| MinGW toolchains missing | `i686-w64-mingw32-gcc: command not found` | **REQUIRED for Wine bridge**: Install MinGW-w64 cross-compilers for your distro |
| Qt6 tools not found | `qmake: command not found` | Add Qt6 bin directory to PATH (see distro guide) |
| Wine bridge toolchain missing | `Wine bridge: disabled (mingw-w64 toolchains and/or makensis not found)` | Install MinGW-w64 toolchains + NSIS |
| OpenCV detection failed | Build succeeds without facetrack / `libp3eft` | **Build-time only**: install OpenCV dev packages on the **builder** (see distro guide). AppImage **end users** should not need system OpenCV if the release bundles `libopencv_*`. |
| ldconfig permission denied | Warning during install | Use `-DENABLE_LDCONFIG=OFF` for packaging |

### Runtime Issues

| Problem | Symptom | Solution |
|---------|---------|----------|
| PS3 Eye plugin missing `ltr_int_find_p3e` | `Couldn't load library '.../libp3e.so.0' - undefined symbol: ltr_int_find_p3e` | **Fixed in current builds**: `ps3eye_driver` resolves `ltr_int_find_p3e` (and related USB helpers) via **dlsym** from `libltusb1` after `dlopen`, same as other `usb_ifc` pointers. Rebuild/reinstall `libp3e` / `libltusb1`. |
| Missing `libp3eft` at runtime | `libp3eft.so.0: cannot open shared object file` | Expected if face tracking was not built: enable `ENABLE_FACE_TRACKER=ON`, install OpenCV dev packages on the **builder**, rebuild, and install. Same as “OpenCV detection failed” above for AppImage/source installs. |
| V4L2 node not usable | `lacks capture+streaming (caps=0x...)` | Many cameras expose several `/dev/video*` nodes; use the one with **Video Capture** (try `v4l2-ctl -d /dev/videoN --all`). Metadata-only nodes lack `VIDEO_CAPTURE`. |
| Missing model / fresh profile | Log: `No Global->Model in preferences`; AppImage/VM abort (older builds asserted) | Ensure `~/.config/linuxtrack/` (or GUI resource dir) gets **`linuxtrack1.conf`** with `[Global]` **`Model = …`** (ship default from `src/linuxtrack1.conf` in the image, or pick a model once in the GUI). |
| Library not found | `Couldn't load library 'libwc.so.0'` | Run `sudo ldconfig` |
| GUI not displaying | Window doesn't appear | Force X11: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Black/blank dialogs | Lutris/Steam game selection dialogs appear black | Force X11: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied | Device access error | Add user to `plugdev,input` groups |
| TrackIR not detected | No tracking | Check USB device: `lsusb \| grep Track` |
| Wine bridge fails | Windows games don't track | Check Wine installation and MFC42 setup |

### Wine-Specific Issues

| Problem | Symptom | Solution |
|---------|---------|----------|
| MFC42 installation fails | TrackIR not working in Wine | Use GUI MFC42 installer or manual winetricks |
| 32-bit Wine prefix issues | Wine errors | Ensure wine-stable (not staging) on Arch |
| Wine bridge components missing | No `NPClient.dll`/`linuxtrack-wine.exe` | Verify MinGW + NSIS are installed, then reconfigure/rebuild |
| Wine version conflicts | Incompatible Wine version | Use Wine 11.0+ (or current Proton/Wine Staging) |

### Distribution-Specific Quick Fixes

#### Debian/Ubuntu
```bash
# MinGW bridge toolchain + NSIS
sudo apt install mingw-w64 nsis

# Qt6 tools (rare)
sudo apt install qt6-tools-dev-tools
```

#### Arch Linux
```bash
# MinGW bridge toolchain + NSIS + Wine runtime
sudo pacman -S mingw-w64-gcc nsis wine wine-mono wine-gecko

# Wayland is default: if GUI does not show, use
QT_QPA_PLATFORM=xcb ltr_gui
```
The `build_32bit_libs.sh` script has been removed. Lib32-mxml and lib32-liblo are not required for the default CMake wine bridge build.

#### Fedora/RHEL
```bash
# Qt6 tools PATH (critical)
export PATH="/usr/lib64/qt6/bin:$PATH"

# MinGW bridge toolchain + NSIS + Wine runtime
sudo dnf install mingw32-gcc mingw32-gcc-c++ mingw32-binutils
sudo dnf install mingw64-gcc mingw64-gcc-c++ mingw64-binutils
sudo dnf install mingw32-nsis wine
```

## Advanced Diagnostics

### Wine Bridge Debugging
```bash
# Test Wine environment
wine --version

# Check MFC42 installation
wine reg query "HKLM\\Software\\Classes\\CLSID\\{98BF1CE3-0x0000-0000-0000-000000000000}"

# Test TrackIR DLL loading
cd /opt/lib/linuxtrack/wine_bridge/
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
ldd /opt/bin/ltr_gui
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
- **Toolchain drift**: Wine bridge build requires MinGW-w64 cross-compilers and NSIS on the build host
- **Qt6 themes**: Some desktop themes may affect GUI appearance
- **Multiple TrackIR devices**: Only one device supported simultaneously

## Emergency Recovery

If everything is broken:

```bash
# Clean reinstall
sudo rm -rf /opt/lib/linuxtrack/
sudo rm -rf /opt/bin/ltr_*
sudo rm -rf /opt/share/linuxtrack/
# Also remove symlinks if they exist
sudo rm -f /usr/local/bin/ltr_*

# Rebuild from scratch
rm -rf build/
mkdir build && cd build
cmake .. [your options]
cmake --build . -j$(nproc)
sudo cmake --install .

# Update library cache
sudo ldconfig
```
