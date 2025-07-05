# TrackIR Permission Errors - Complete Troubleshooting Guide

## Problem Overview

**Symptoms**: LinuxTrack detects TrackIR 5 device but displays persistent permission error dialogs, preventing proper operation despite having correct system permissions.

**Root Cause**: This seemingly simple "permission error" was actually a complex multi-layered issue involving library conflicts, Qt version mismatches, and broken system libraries masquerading as permission problems.

---

## The Deceptive Nature of This Problem

❌ **What it LOOKED like**: Simple USB device permission issue  
✅ **What it ACTUALLY was**: Complex library loading failure with cascading effects

The TrackIR device was properly detected and had correct permissions, but **broken system libraries** were preventing successful initialization, causing the application to display misleading "permission error" messages.

---

## Complete Diagnostic Process

### Step 1: Basic Hardware Verification ✅

```bash
# Verify TrackIR 5 detection
lsusb | grep -i trackir
# Expected: Bus 001 Device 002: ID 131d:0159 Natural Point TrackIR 5

# Check USB device permissions
ls -la /dev/bus/usb/001/002
# Expected: crw-rw-rw- 1 root plugdev (or similar with write permissions)
```

**Result**: ✅ TrackIR 5 properly detected, ✅ Device permissions correct

### Step 2: Udev Rules Verification ✅

```bash
# Check udev rules installation
ls -la /lib/udev/rules.d/99-TIR.rules
# Expected: File exists with proper TrackIR rules

# Verify rules content
cat /lib/udev/rules.d/99-TIR.rules | grep 131d
# Expected: Rules matching TrackIR USB ID 131d:0159

# Test udev rule application
udevadm test $(udevadm info -q path -n /dev/bus/usb/001/002)
# Expected: Rules properly applied, no errors
```

**Result**: ✅ Udev rules properly installed and active

### Step 3: User Group Verification ✅

```bash
# Check user group membership
groups $USER
# Expected: user should be in 'plugdev' and 'linuxtrack' groups

# Verify groups exist
getent group plugdev
getent group linuxtrack
# Expected: Both groups exist and user is member
```

**Result**: ✅ User in correct groups with proper permissions

### Step 4: Application Launch Analysis ❌

```bash
# Launch LinuxTrack and observe behavior
./run_qt5_gui.sh
# Observed: GUI launches but shows TrackIR permission error dialog
# Unexpected: All system-level permissions were correct!
```

**Result**: ❌ Permission errors despite correct system configuration

---

## The Breakthrough: Library Conflict Discovery

### Step 5: Deep Dive - Library Loading Analysis 🔍

The key insight was to look beyond surface-level permissions and investigate **library loading**:

```bash
# Check library dependencies
ldd src/qt_gui/ltr_gui | grep -i "not found"
# Revealed: Missing or broken library dependencies

# Investigate system LinuxTrack installation
ls -la /usr/local/lib/linuxtrack/
# Found: Old, broken system libraries installed
```

### Step 6: Critical Discovery - Broken System Libraries 💡

```bash
# Test library loading
LD_DEBUG=libs ./run_qt5_gui.sh 2>&1 | grep -E "error|undefined"
# Revealed: undefined symbol: v4l2_munmap in system libraries

# Compare system vs local libraries
ls -la /usr/local/lib/linuxtrack/
ls -la src/.libs/
# Found: System libraries were outdated and broken
```

**Root Cause Identified**: 
- Old broken LinuxTrack libraries in `/usr/local/lib/linuxtrack/`
- These libraries had `v4l2_munmap` symbol errors
- Application was loading broken system libraries instead of working local ones
- Library loading failures caused TrackIR initialization to fail
- Failed initialization was misreported as "permission errors"

---

## The Complete Solution

### Fix 1: Library Conflict Resolution

```bash
# Backup broken system libraries
sudo mv /usr/local/lib/linuxtrack /usr/local/lib/linuxtrack.backup

# Create symlinks to working local libraries
sudo mkdir -p /usr/local/lib/linuxtrack
cd /usr/local/lib/linuxtrack

# Link working libraries
sudo ln -sf /path/to/working/src/.libs/libtir.so.0.0.0 libtir.so.0
sudo ln -sf /path/to/working/src/.libs/libwc.so.0.0.0 libwc.so.0
sudo ln -sf /path/to/working/src/.libs/libjoy.so.0.0.0 libjoy.so.0
sudo ln -sf /path/to/working/src/.libs/libltusb1.so.0.0.0 libltusb1.so.0
sudo ln -sf /path/to/working/src/.libs/libltr.so.0.0.0 libltr.so.0
```

### Fix 2: Qt Version Compatibility

```bash
# Ensure Qt5 is being used (not Qt6)
qmake --version
# Should show Qt5, not Qt6

# If Qt6 is default, explicitly use Qt5
export PATH="/usr/bin:$PATH"  # Ensure qmake-qt5 is found first
qmake-qt5 --version  # Verify Qt5 availability
```

### Fix 3: Configuration Path Corrections

```bash
# Fix hardcoded paths in configuration
# Update HELP_BASE in src/qt_gui/ltr_gui.pro
sed -i 's|HELP_BASE=\\"ltr_gui/\\"|HELP_BASE=\\"../\\"|' src/qt_gui/ltr_gui.pro

# Fix config file prefix
sed -i 's|^Prefix = .*|Prefix = "/path/to/project/root"|' ~/.config/linuxtrack/linuxtrack1.conf
```

### Fix 4: Enhanced Launch Script

```bash
# Create improved launch script with library path override
cat > run_qt5_gui.sh << 'EOF'
#!/bin/bash
export LD_LIBRARY_PATH="$(pwd)/src/.libs:$LD_LIBRARY_PATH"
export QT_SELECT=qt5
cd src/qt_gui
./ltr_gui
EOF
chmod +x run_qt5_gui.sh
```

---

## Verification of Success

### Test 1: Library Loading
```bash
# Verify no library loading errors
ldd src/qt_gui/ltr_gui | grep -E "not found|error"
# Expected: No errors
```

### Test 2: Application Launch
```bash
# Launch application
./run_qt5_gui.sh
# Expected: No permission error dialogs
```

### Test 3: TrackIR Communication
```bash
# Check LinuxTrack log for TrackIR communication
tail -f ~/.config/linuxtrack/linuxtrack.log
# Expected: Successful TrackIR initialization messages
```

---

## Why This Was So Deceptive

### 1. **Misleading Error Messages**
- Application reported "permission errors"
- Actual cause was library loading failures
- Error handling code didn't distinguish between permission and library issues

### 2. **Correct Surface-Level Permissions**
- USB device permissions: ✅ Correct
- User groups: ✅ Correct  
- Udev rules: ✅ Correct
- Made it seem like a simple permission issue

### 3. **Hidden Library Conflicts**
- System had both old (broken) and new (working) libraries
- Library path resolution loaded broken system libraries first
- No obvious indication that library loading was the real problem

### 4. **Cascading Failures**
- Library loading failure → TrackIR initialization failure → Permission error dialog
- Each step in the chain looked like it might be the root cause

---

## Prevention for Future Builds

### 1. Clean Installation Practices
```bash
# Always remove old installations before building new ones
sudo rm -rf /usr/local/lib/linuxtrack /usr/local/bin/ltr_* 

# Or use package managers when available
sudo make uninstall  # if supported
```

### 2. Library Path Management
```bash
# Use explicit library paths in launch scripts
export LD_LIBRARY_PATH="/path/to/project/libs:$LD_LIBRARY_PATH"

# Or use rpath during linking (preferred)
./configure --prefix=/usr/local LDFLAGS="-Wl,-rpath,/usr/local/lib/linuxtrack"
```

### 3. Dependency Management
```bash
# Check for library conflicts before installation
ldconfig -p | grep -i linuxtrack
find /usr -name "*linuxtrack*" -o -name "*ltr*" 2>/dev/null
```

---

## Diagnostic Commands Reference

### Quick Permission Check
```bash
# One-liner to check basic TrackIR setup
lsusb | grep 131d && ls -la /lib/udev/rules.d/99-TIR.rules && groups | grep -E "(plugdev|linuxtrack)"
```

### Library Conflict Detection
```bash
# Check for library conflicts
find /usr/local/lib -name "*ltr*" -o -name "*linuxtrack*" 2>/dev/null
ldd $(which ltr_gui 2>/dev/null || echo "src/qt_gui/ltr_gui") | grep -E "not found|error"
```

### Comprehensive Debug Launch
```bash
# Launch with maximum debugging info
LD_DEBUG=all LD_LIBRARY_PATH="$(pwd)/src/.libs" src/qt_gui/ltr_gui 2>&1 | tee debug.log
```

---

## Summary

The "TrackIR permission error" was a **multi-layered system integration issue** disguised as a simple permission problem:

1. **Surface symptoms**: Permission error dialogs
2. **Misleading evidence**: All actual permissions were correct
3. **Hidden root cause**: Broken system libraries preventing TrackIR initialization
4. **Compounding factors**: Qt version conflicts and hardcoded path issues
5. **Solution complexity**: Required library management, build system fixes, and configuration updates

**Key lesson**: When dealing with "permission errors" that persist despite correct permissions, investigate library loading, dependency conflicts, and build environment issues. The real problem may be several layers deeper than the error message suggests.

---

## Quick Fix for New Installations

If you encounter this issue:

```bash
# 1. Check for conflicting system libraries
ls /usr/local/lib/linuxtrack/

# 2. If found, backup and replace with working versions
sudo mv /usr/local/lib/linuxtrack /usr/local/lib/linuxtrack.backup

# 3. Use our build script which handles this automatically
./build.sh --clean --install

# 4. Verify success
./run_qt5_gui.sh
```

The automated build and setup scripts in this repository now handle these complex interactions automatically, preventing this issue for future users. 