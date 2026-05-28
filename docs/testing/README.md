# LinuxTrack Testing Guide

## Overview

This directory contains testing documentation for LinuxTrack. **Current releases target Qt6 + CMake**.

## Quick Testing Reference

### Essential Testing Commands
```bash
# Build (from repo root)
mkdir -p build && cd build && cmake .. -DCMAKE_INSTALL_PREFIX=/opt && cmake --build . -j$(nproc)

# Launch application (from repo root)
./scripts/test/run_gui.sh

# Force X11 mode (for Wayland sessions)
./scripts/test/run_gui.sh --force-x11
# or
./scripts/test/run_gui_x11.sh
```

### Success Criteria
- Build completes without errors
- GUI launches successfully
- TrackIR hardware detected (no permission errors)
- Head tracking responds to movement

## Distribution-Specific Testing

### Ubuntu/Debian Testing
**Key Requirements:**
- Install 32-bit development libraries: `sudo apt install gcc-multilib libc6-dev-i386`
- Wine installation for wine bridge support
- **Qt6** development packages (see [readme/debian-ubuntu.md](../readme/debian-ubuntu.md))

**Common Issues:**
- Missing `bits/wordsize.h` or `bits/libc-header-start.h` → Install multilib packages
- Wine bridge detection failures → Verify wine installation
- Configuration file copy errors → Check permissions

### Kubuntu Testing
**Specific Notes:**
- Prefer **Qt6** packages from [readme/debian-ubuntu.md](../readme/debian-ubuntu.md)
- 32-bit support requires explicit package installation
- Wayland compatibility: use `run_gui_x11.sh` for optimal tracking

### MX Linux Testing
**Advantages:**
- Stable build environment
- See [readme/debian-ubuntu.md](../readme/debian-ubuntu.md) for Debian-family package guidance

## Testing Files

### Current Testing Documentation
- **`TESTING_GUIDE.md`** - General testing guidelines

### File Descriptions
- Large files contain detailed build logs and troubleshooting steps
- Medium files provide procedural guidance

## Testing Priorities

### High Priority
1. **Build System Validation** - Ensure CMake configures and builds correctly
2. **Qt6 Detection** - Verify `find_package(Qt6 …)` succeeds
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

## Manual Checks
```bash
# Check the built GUI and TrackIR access
./scripts/test/run_gui.sh
lsusb | grep 131d
groups "$USER"
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
1. **Missing Dependencies** → Install required packages (see `docs/readme/` for your distro)
2. **Qt Version Conflicts** → Ensure Qt6 packages are installed
3. **32-bit Library Issues** → Install multilib packages
4. **Permission Errors** → Add user to plugdev group

### Hardware Testing Issues
1. **Device Not Detected** → Check USB connection and udev rules
2. **Permission Denied** → Verify user group membership
3. **No Tracking Data** → Check IR LED functionality and camera

### Display Issues
1. **Wayland Compatibility** → Use `run_gui_x11.sh` for optimal performance
2. **GUI Not Launching** → Check Qt6 installation and library paths

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

**Note**: Refer to `TESTING_GUIDE.md` for comprehensive procedural information.
