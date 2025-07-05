# LinuxTrack Testing Guide

## Overview

This directory contains comprehensive testing documentation for LinuxTrack Qt5 modernization. The testing process validates build system compatibility, hardware detection, and functionality across different Linux distributions.

## Quick Testing Reference

### Essential Testing Commands
```bash
# Quick health check
./linuxtrack_health_check.sh

# Build test
./build.sh --deps --clean --install

# Launch application
./run_qt5_gui.sh

# Force X11 mode (for Wayland)
./run_qt5_gui.sh --force-x11
```

### Success Criteria
- ✅ Build completes without errors
- ✅ GUI launches successfully
- ✅ TrackIR hardware detected (no permission errors)
- ✅ Head tracking responds to movement

## Distribution-Specific Testing

### Ubuntu/Debian Testing
**Key Requirements:**
- Install 32-bit development libraries: `sudo apt install gcc-multilib libc6-dev-i386`
- Wine installation for wine bridge support
- Qt5 development packages

**Common Issues:**
- Missing `bits/wordsize.h` or `bits/libc-header-start.h` → Install multilib packages
- Wine bridge detection failures → Verify wine installation
- Configuration file copy errors → Check permissions

### Kubuntu 25.10 Testing
**Specific Notes:**
- Qt5 packages may have different names (`qt5-qmake` vs `qmake-qt5`)
- 32-bit support requires explicit package installation
- Wayland compatibility requires X11 mode for optimal tracking

### MX Linux Testing
**Advantages:**
- Excellent Qt5 compatibility
- Minimal dependency conflicts
- Stable build environment

## Testing Files

### Current Testing Documentation
- **`TESTING_INSTRUCTIONS.md`** - Comprehensive testing procedures (200KB)
- **`KUBUNTU_25_TESTING.md`** - Detailed Kubuntu testing log (328KB)
- **`KUBUNTU_25_QT5_TESTING.md`** - Qt5-specific Kubuntu testing (29KB)
- **`TESTING_GUIDE.md`** - General testing guidelines (13KB)
- **`KUBUNTU_FIXES.md`** - Kubuntu-specific fixes (5KB)

### File Descriptions
- **Large files** contain detailed build logs and troubleshooting steps
- **Medium files** provide procedural guidance
- **Small files** contain specific fixes and workarounds

## Testing Priorities

### High Priority
1. **Build System Validation** - Ensure autotools work correctly
2. **Qt5 Detection** - Verify Qt5 vs Qt6 compatibility
3. **Hardware Detection** - TrackIR device recognition
4. **Permission Setup** - udev rules and user groups

### Medium Priority
1. **Display Server Compatibility** - X11 vs Wayland testing
2. **Library Loading** - Dynamic library dependencies
3. **Configuration Management** - Settings persistence

### Low Priority
1. **Performance Testing** - Tracking latency and accuracy
2. **Game Integration** - Wine compatibility testing
3. **Advanced Features** - Plugin and extension testing

## Automated Testing

### Health Check Script
```bash
# Run comprehensive health check
./linuxtrack_health_check.sh

# Check specific components
./linuxtrack_health_check.sh --qt5
./linuxtrack_health_check.sh --hardware
./linuxtrack_health_check.sh --permissions
```

### Build Verification
```bash
# Verify all components built
ls -la src/qt_gui/ltr_gui
ls -la src/mickey/mickey
ls -la src/.libs/libltr.so*

# Check Windows components (if MinGW available)
ls -la src/wine_bridge/*/*.dll
ls -la src/wine_bridge/*/*.exe
```

## Troubleshooting Testing Issues

### Common Build Problems
1. **Missing Dependencies** → Install required packages
2. **Qt Version Conflicts** → Ensure Qt5 is detected, not Qt6
3. **32-bit Library Issues** → Install multilib packages
4. **Permission Errors** → Add user to plugdev group

### Hardware Testing Issues
1. **Device Not Detected** → Check USB connection and udev rules
2. **Permission Denied** → Verify user group membership
3. **No Tracking Data** → Check IR LED functionality and camera

### Display Issues
1. **Wayland Compatibility** → Use X11 mode for optimal performance
2. **GUI Not Launching** → Check Qt5 installation and library paths
3. **Display Artifacts** → Verify OpenGL compatibility

## Contributing to Testing

### Adding New Test Cases
1. Document the test scenario
2. Include expected results
3. Provide troubleshooting steps
4. Update this README with new information

### Reporting Test Results
1. Include distribution and version
2. Document exact error messages
3. Provide system information
4. Include successful workarounds

---

**Note**: The large testing files contain detailed logs and specific troubleshooting steps. Refer to them for comprehensive information about specific issues encountered during testing. 