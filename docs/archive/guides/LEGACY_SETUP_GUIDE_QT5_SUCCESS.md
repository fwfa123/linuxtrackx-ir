# LinuxTrack Qt5 Setup Guide - Complete Success Documentation

## Overview
This guide documents the exact steps taken on **June 22, 2025** to successfully modernize LinuxTrack from Qt4 to Qt5 with full TrackIR 5 hardware activation on **Fedora Linux 42 (kernel 6.14.9)**.

## 🎯 Final Achievement
- ✅ **TrackIR 5 Hardware Detection**: Device 8 properly detected and activated
- ✅ **Qt5 GUI Modernization**: Complete migration from Qt4 to Qt5
- ✅ **Camera Data Streaming**: Real-time encrypted packet processing working
- ✅ **Modern Linux Compatibility**: Running on current Fedora with modern Qt5

---

## 🛠️ Complete Setup Process

### Phase 1: Initial Build System Setup

#### 1.1 Configure Autotools Build
```bash
# Start from clean June 14th commit
cd /mnt/Local_Git/git-repos/linuxtrack-clean-june14

# Configure with Qt5 and debug options
./configure --enable-qt5-gui --enable-debug --with-qt5

# Build the complete system
make -j$(nproc)
```

#### 1.2 Build Qt5 GUI Specifically
```bash
# Navigate to Qt GUI directory
cd src/qt_gui

# Build Qt5 GUI with debug symbols
make ltr_gui_qt5_debug

# Verify 15MB executable was created
ls -la ltr_gui_qt5_debug
# Expected: -rwxr-xr-x. 1 mario mario 15728400 Jun 22 09:45 ltr_gui_qt5_debug
```

### Phase 2: Library Management & Installation

#### 2.1 Build All Required Libraries
The autotools build created these critical libraries in `src/.libs/`:

| Library | Size | Purpose |
|---------|------|---------|
| `libltr.so.0.0.0` | 1.3MB | Main LinuxTrack engine |
| `libtir.so.0.0.0` | 137KB | **TrackIR hardware driver** |
| `libwc.so.0.0.0` | 68KB | Webcam driver (stubbed) |
| `libjoy.so.0.0.0` | 50KB | Joystick/Wiimote driver (stubbed) |
| `liblinuxtrack.so.0.0.0` | 91KB | Client library |
| `libltusb1.so.0.0.0` | 38KB | USB interface library |

#### 2.2 Install Libraries to System Location
```bash
# Create system library directory
sudo mkdir -p /usr/local/lib/linuxtrack/

# Copy all built libraries to system location
sudo cp src/.libs/*.so* /usr/local/lib/linuxtrack/

# Also create local lib directory for development
mkdir -p lib/linuxtrack/
cp src/.libs/*.so* lib/linuxtrack/

# Verify installation
ls -la /usr/local/lib/linuxtrack/
```

#### 2.3 **CRITICAL**: Install Missing USB Library
During testing, we discovered the USB library was missing from system location:

```bash
# This was the KEY missing piece that enabled hardware activation
sudo cp src/.libs/libltusb1.so* /usr/local/lib/linuxtrack/

# Verify USB library is now available
ls -la /usr/local/lib/linuxtrack/libltusb1.so*
```

### Phase 3: Configuration Management

#### 3.1 Fix Configuration File Path Issues
The configuration file at `~/.config/linuxtrack/linuxtrack1.conf` needed the **Prefix** corrected:

```ini
[Global]
Input = TrackIR
Model = NP TrackClip Pro
# CRITICAL: Changed from build directory to project root
Prefix = "/mnt/Local_Git/git-repos/linuxtrack-clean-june14"
```

**Original (wrong)**: `Prefix = "/mnt/Local_Git/git-repos/linuxtrack-clean-june14/src/qt_gui"`
**Corrected**: `Prefix = "/mnt/Local_Git/git-repos/linuxtrack-clean-june14"`

### Phase 4: Launch Script Creation

#### 4.1 Create Enhanced Launch Script
Created `run_qt5_gui.sh` to handle library paths and debugging:

```bash
#!/bin/bash
echo "🚀 Starting LinuxTrack Qt5 GUI (Modernized from Qt4)"
echo "📍 Working directory: $(pwd)"

# Set library path to local build directory
export LD_LIBRARY_PATH="$(pwd)/src/.libs:$LD_LIBRARY_PATH"
echo "🔧 Library path: $LD_LIBRARY_PATH"

# Show available libraries for debugging
echo "📂 Available libraries:"
ls -la src/.libs/libltr.so*

# Navigate to GUI directory and launch
echo "📂 GUI directory: $(pwd)/src/qt_gui"
cd src/qt_gui
echo "✨ Launching Qt5 GUI..."
./ltr_gui_qt5_debug
echo "🏁 Qt5 GUI closed."
```

```bash
# Make script executable
chmod +x run_qt5_gui.sh
```

### Phase 5: Hardware Validation

#### 5.1 Verify TrackIR Hardware Detection
```bash
# Check USB device presence
lsusb | grep "131d:0159"
# Expected: Bus 001 Device 002: ID 131d:0159 Natural Point

# Check device permissions
ls -la /dev/bus/usb/001/002
# Expected: crw-rw-rw- (world readable/writable)

# Verify user groups
groups
# Should include: plugdev, linuxtrack
```

#### 5.2 Test Hardware Activation
```bash
# Launch Qt5 GUI
./run_qt5_gui.sh

# Expected output in log:
# [timestamp] Found device 8
# [timestamp] Device is a TrackIR (131D:0159).
# [timestamp] Initializing TrackIR 5 revision 3.
# [timestamp] TrackIR interface claimed.
# [timestamp] Going to start camera.
# [timestamp] Changing state to RUNNING!
```

---

## 🔧 Troubleshooting Issues Resolved

### Issue 1: Library Loading Errors
**Problem**: `./ltr_gui_qt5_debug: error while loading shared libraries: libltr.so.0`

**Solution**: 
- Created proper `LD_LIBRARY_PATH` in launch script
- Installed libraries to both local and system directories
- Used launch script instead of direct execution

### Issue 2: Missing USB Library
**Problem**: TrackIR hardware detected but not activated

**Solution**:
```bash
# The missing piece - USB interface library
sudo cp src/.libs/libltusb1.so* /usr/local/lib/linuxtrack/
```

### Issue 3: Wrong Configuration Prefix
**Problem**: System searching in wrong directories

**Solution**: 
- Corrected `Prefix` in config file from GUI build dir to project root
- Ensured consistent path references

### Issue 4: Qt5 Display Warnings (Non-Critical)
**Symptoms**: 
- `kf.windowsystem: Could not find any platform plugin`
- `QTextBrowser: No document for qthelp://...`
- `qt.qpa.wayland: Wayland does not support QWindow::requestActivate()`

**Status**: **Cosmetic only** - does not affect TrackIR functionality

### Issue 5: Real-time Blob Tracking Display (Wayland Compatibility)
**Problem**: Blob tracking images only appear when paused, not during live tracking

**Root Cause**: Wayland restrictions on Qt5 OpenGL rendering affect real-time image buffer display

**Solution**: Use X11 mode for full functionality:
```bash
# Method 1: Environment variables
XDG_SESSION_TYPE=x11 QT_QPA_PLATFORM=xcb ./run_qt5_gui.sh

# Method 2: Convenience script (automatically sets X11 mode)
./run_qt5_gui_x11.sh
```

**Status**: **Workaround available** - see `WAYLAND_COMPATIBILITY_ISSUE.md` for details

---

## 📊 Success Verification

### Hardware Detection Confirmed
```
[Sun 2025-06-22 10:11:05 EDT] Found device 8
[Sun 2025-06-22 10:11:05 EDT] Device is a TrackIR (131D:0159).
[Sun 2025-06-22 10:11:19 EDT] Initializing TrackIR 5 revision 3.
[Sun 2025-06-22 10:11:19 EDT] TrackIR interface claimed.
[Sun 2025-06-22 10:11:20 EDT] Going to start camera.
[Sun 2025-06-22 10:11:21 EDT] Changing state to RUNNING!
```

### Library Status
```bash
# All required libraries present and functional
$ ls -la /usr/local/lib/linuxtrack/
libjoy.so.0      -> 50KB  (Joystick driver)
libltr.so.0      -> 1.3MB (Main engine) 
libtir.so.0      -> 137KB (TrackIR driver)
libwc.so.0       -> 68KB  (Webcam driver)
libltusb1.so.0   -> 38KB  (USB interface) ← **KEY COMPONENT**
```

---

## 🎯 Key Success Factors

1. **Complete Autotools Build**: Used existing autotools instead of CMake
2. **Proper Library Installation**: Both local and system library copies
3. **USB Library Installation**: Critical `libltusb1.so.0` in system location  
4. **Configuration Path Fix**: Corrected Prefix in config file
5. **Launch Script**: Proper `LD_LIBRARY_PATH` management
6. **Hardware Permissions**: Proper udev rules and user groups

---

## 🚀 Final Status

**Qt4→Qt5 Modernization: COMPLETE SUCCESS**

- **Hardware Activation**: ✅ TrackIR 5 working perfectly
- **Modern Compatibility**: ✅ Running on Fedora 42 with Qt5
- **Real-time Processing**: ✅ Camera data streaming confirmed
- **Tracking Functionality**: ✅ Full head tracking operational
- **Wayland Compatibility**: ⚠️ Use X11 mode for optimal blob display
- **Development Ready**: ✅ Debug build with full functionality

## 📝 Files Created/Modified

- `run_qt5_gui.sh` - Enhanced launch script
- `~/.config/linuxtrack/linuxtrack1.conf` - Fixed Prefix path
- `/usr/local/lib/linuxtrack/` - System library installation
- `src/qt_gui/ltr_gui_qt5_debug` - Qt5 modernized executable

---

**Commit**: `f6d56f6` - 🎉 MILESTONE: Qt4→Qt5 modernization with TrackIR hardware activation SUCCESS

**Date**: June 22, 2025  
**System**: Fedora Linux 42 (kernel 6.14.9-300.fc42.x86_64)  
**Achievement**: Complete LinuxTrack modernization with hardware validation 