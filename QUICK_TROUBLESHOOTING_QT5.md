# Quick Troubleshooting Guide - LinuxTrack Qt5

## 🚨 Common Issues & Quick Fixes

### 1. Library Loading Error
```
./ltr_gui_qt5_debug: error while loading shared libraries: libltr.so.0: cannot open shared object file
```

**Quick Fix:**
```bash
# Use the launch script instead of direct execution
./run_qt5_gui.sh

# OR set library path manually:
cd src/qt_gui
LD_LIBRARY_PATH=../.libs:$LD_LIBRARY_PATH ./ltr_gui_qt5_debug
```

### 2. TrackIR Hardware Not Activating
```
# GUI loads but no "Found device 8" message
```

**Quick Fix:**
```bash
# Install missing USB library
sudo cp src/.libs/libltusb1.so* /usr/local/lib/linuxtrack/

# Verify TrackIR USB device present
lsusb | grep "131d:0159"
```

### 3. Wrong Library Paths
```
# System searching in wrong directories
```

**Quick Fix:**
```bash
# Check/fix config file
nano /home/mario/.config/linuxtrack/linuxtrack1.conf

# Ensure Prefix is set to:
Prefix = "/mnt/Local_Git/git-repos/linuxtrack-clean-june14"
```

### 4. Qt5 Display Warnings (Safe to Ignore)
```
kf.windowsystem: Could not find any platform plugin
QTextBrowser: No document for qthelp://...
```

**Status**: **Cosmetic only** - TrackIR functionality works perfectly

---

## ✅ Quick Success Verification

Run this command to verify everything is working:
```bash
./run_qt5_gui.sh 2>&1 | grep -E "(Found device|TrackIR|Initializing)"
```

**Expected output:**
```
Found device 8
Device is a TrackIR (131D:0159).
Initializing TrackIR 5 revision 3.
```

---

## 🔧 Emergency Reset

If everything breaks, run this sequence:
```bash
# 1. Rebuild everything
make clean && make -j$(nproc)

# 2. Reinstall libraries
sudo cp src/.libs/*.so* /usr/local/lib/linuxtrack/

# 3. Reset config (backup first!)
cp /home/mario/.config/linuxtrack/linuxtrack1.conf /home/mario/.config/linuxtrack/linuxtrack1.conf.backup

# 4. Use launch script
./run_qt5_gui.sh
```

---

## 📱 Quick Status Check Commands

```bash
# Check if TrackIR hardware is connected
lsusb | grep "131d:0159"

# Check if libraries are built
ls -la src/.libs/libltr.so*

# Check if system libraries are installed  
ls -la /usr/local/lib/linuxtrack/

# Check if launch script exists and is executable
ls -la run_qt5_gui.sh
```

**All should return results - if any fail, refer to main setup guide.** 