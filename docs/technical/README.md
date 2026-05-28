# LinuxTrack Technical Documentation

## Overview

This directory contains technical documentation for LinuxTrack.

**Current release stack (branch `main`):** Qt6 GUI and tools built via CMake (`find_package(Qt6 …)`), MinGW PE Wine bridge, AppImage packaging. Autotools and `qmake` are no longer used.

## Technical Files

### Implementation and Architecture
- **`TECHNICAL_IMPLEMENTATION_GUIDE.md`** — Core implementation details
- **`GAME_DATA_SYSTEM.md`** — Game data system architecture (gamedata.txt, ApplicationID, encryption keys)
- **`INSTALLATION_ANALYSIS_COMPREHENSIVE.md`** — Installation path analysis
- **`ADVANCED_INSTALLATION_AND_DEPENDENCIES.md`** — Deep dependency documentation

### Wine Bridge
- **`WINE_VERSION_SELECTION_IMPLEMENTATION.md`** — Wine version picker implementation
- **[`../guides/WINE_BRIDGE_MODERN.md`](../guides/WINE_BRIDGE_MODERN.md)** — MinGW PE bridge setup (NSIS not required)
- **[`../../src/wine_bridge/WINE_SETUP.md`](../../src/wine_bridge/WINE_SETUP.md)** — In-tree bridge setup reference

### TrackIR / DLL Analysis
- **`TRACKIR_5.5.3_DLL_ARCHITECTURE_ANALYSIS.md`** — TrackIR DLL layout
- **`TRACKIR_FIX_README.md`** — TrackIR fix documentation
- **`TrackIR_MFC42_modern_windows_analysis.md`** — MFC42 on modern Windows

### MFC42 / Wine Prefix
- **`MFC42_WINETRICKS_INSTALLATION_GUIDE.md`** — Winetricks MFC42 install
- **`MFC42_ALTERNATIVE_INSTALLATION_METHODS.md`** — MFC42 fallback methods
- **`MFC42_ALTERNATIVE_INSTALLATION_SUMMARY.md`** — MFC42 method comparison

### Qt Help System
- **`ROBUST_HELP_SYSTEM.md`** — Qt help system design
- **`QT_HELP_SYSTEM_FIX.md`** — Qt help system fixes

### OpenTrack
- **`OPENTRACK_PLUGIN_PLAN.md`** — OpenTrack plugin integration plan

---

## Key Technical Areas

### Build System (CMake)

**Build steps:**
```bash
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)
sudo cmake --install .
```

**Development launch (from repo root):**
```bash
./scripts/test/run_gui.sh
```

### Wine Bridge (MinGW PE)

The Wine bridge is built using MinGW cross-compilers (`i686-w64-mingw32-gcc`, `x86_64-w64-mingw32-gcc`) to produce `NPClient.dll` / `NPClient64.dll`. NSIS and `winegcc` are not required for the current build path.

See `WINE_VERSION_SELECTION_IMPLEMENTATION.md` for how the GUI selects and launches Wine versions.

### Hardware Integration

**TrackIR support:**
- USB device detection (131d:xxxx)
- udev rules for permissions (`/lib/udev/rules.d/99-TIR.rules`)
- IR tracking algorithms
- Camera frame processing (TrackIR 4/5, PlayStation Eye, generic USB)

Current Arch Linux instructions: [`../readme/arch-linux.md`](../readme/arch-linux.md)

### Game Data System
**Primary file**: `GAME_DATA_SYSTEM.md`

- `gamedata.txt` generation and parsing
- ApplicationID parsing and encryption keys
- Wine bridge client communication
- Game registration and lookup

### Display Server Compatibility
**Primary reference**: [`../troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md`](../troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md)

X11 is fully supported. Wayland sessions require XWayland (`QT_QPA_PLATFORM=xcb`). Use `scripts/test/run_gui_x11.sh` to force X11 mode.

---

## Code Organization

```
src/
├── qt_gui/           # Qt6 GUI application
├── mickey/           # TrackIR device support
├── wine_bridge/      # Windows compatibility layer (MinGW PE)
├── .libs/            # Shared libraries
└── [other modules]   # Additional components
```

---

## Contributing to Development

### Code Standards
1. **C/C++**: Follow project coding standards
2. **Qt6**: Use Qt6 APIs (`QStringView`, `QList` etc.)
3. **Documentation**: Maintain technical documentation
4. **Testing**: Include unit and integration tests

### Submission Process
1. **Feature Branches**: Create separate development branches
2. **Code Review**: Submit merge/pull requests for review
3. **Testing**: Verify functionality across distributions
4. **Documentation**: Update relevant technical guides

---

**Note**: This directory covers implementation details for developers and maintainers. For user-facing documentation see `docs/guides/` and `docs/troubleshooting/`.
