# Wayland Compatibility Issue - Real-Time Blob Tracking Display

## Issue Summary
**Qt5 GUI works perfectly on X11 but has display issues with Wayland on Fedora KDE Plasma**

### Symptoms
- TrackIR 5 hardware detection: ✅ **Working**
- Head tracking functionality: ✅ **Working** 
- Real-time blob tracking display: ❌ **Broken on Wayland only**
  - On X11: Real-time blob tracking images display correctly during active tracking
  - On Wayland: Blob tracking images only appear when paused, not during live tracking

### Solution
**Force X11 mode for full functionality:**

```bash
XDG_SESSION_TYPE=x11 QT_QPA_PLATFORM=xcb ./run_qt5_gui.sh
```

### Evidence
**Wayland behavior (broken):**
```
qt.qpa.wayland: Wayland does not support QWindow::requestActivate()
kf.windowsystem: Could not find any platform plugin
```

**X11 behavior (working):**
```
Debug flags: p (enables real-time blob tracking display)
# Real-time blob tracking displays correctly
```

### Root Cause
The issue appears to be related to Wayland's restrictions on OpenGL/graphics rendering in Qt5 applications, specifically affecting the real-time image buffer display in the blob tracking widget.

### Recommendation
**For immediate use**: Launch with X11 mode as shown above

**For long-term solution**: Consider investigating Qt5 Wayland-specific OpenGL context management or migrating the blob tracking display to use Wayland-compatible rendering methods.

---
**Status**: Issue Identified - Workaround Available  
**Date**: $(date +%Y-%m-%d)  
**Priority**: Medium (workaround exists) 