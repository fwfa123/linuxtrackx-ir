# LinuxTrack Documentation Index

## Complete Documentation Guide

This index organizes LinuxTrack documentation. **The supported GUI/toolchain is Qt6 with CMake** (see [README.md](../README.md) and [readme/](readme/)). The project uses a MinGW PE Wine bridge and AppImage packaging.

---

## Getting Started

### New Users
1. **[README.md](../README.md)** - Main project overview and quick start
2. **[readme/debian-ubuntu.md](readme/debian-ubuntu.md)** / **[readme/fedora-rhel.md](readme/fedora-rhel.md)** / **[readme/arch-linux.md](readme/arch-linux.md)** - Distribution-specific install (Qt6 / CMake)
3. **[testing/README.md](testing/README.md)** - Testing overview

### Existing Users (After System Changes)
1. **[readme/troubleshooting.md](readme/troubleshooting.md)** - Current diagnostics and common fixes
2. **[troubleshooting/TROUBLESHOOTING.md](troubleshooting/TROUBLESHOOTING.md)** - Comprehensive troubleshooting

---

## Documentation Organization

### User Guides (`guides/` + `readme/`)
**For end users and system administrators**
- **[guides/README.md](guides/README.md)** - Supplemental guide index and cross-links
- **[guides/WINE_BRIDGE_MODERN.md](guides/WINE_BRIDGE_MODERN.md)** - Modern MinGW Wine bridge details
- **[readme/debian-ubuntu.md](readme/debian-ubuntu.md)**, **[readme/fedora-rhel.md](readme/fedora-rhel.md)**, **[readme/arch-linux.md](readme/arch-linux.md)** - Per-distro install docs

### Testing Documentation (`testing/`)
**For testing and validation procedures**
- **[testing/README.md](testing/README.md)** - Testing guide overview
- **[testing/TESTING_GUIDE.md](testing/TESTING_GUIDE.md)** - General testing guidelines

### Game Workarounds
- **[GAME_WORKAROUNDS.md](GAME_WORKAROUNDS.md)** - Per-title Wine/Proton/Lutris hints (community-tested)

### Troubleshooting (`troubleshooting/`)
**For problem resolution and diagnostics**
- **[troubleshooting/README.md](troubleshooting/README.md)** - Troubleshooting guide overview
- **[troubleshooting/TROUBLESHOOTING.md](troubleshooting/TROUBLESHOOTING.md)** - Comprehensive troubleshooting guide
- **[troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)** - Hardware permission issues
- **[troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md](troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md)** - TrackIR firmware extraction issues
- **[troubleshooting/APPLICATIONID_PARSING_ISSUE.md](troubleshooting/APPLICATIONID_PARSING_ISSUE.md)** - ApplicationID parsing bug causing `((null))` in gamedata.txt
- **[troubleshooting/STEAM_FLATPAK_DEBUG_LOGS.md](troubleshooting/STEAM_FLATPAK_DEBUG_LOGS.md)** - Steam Flatpak logging
- **[troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md](troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md)** - Wayland display issues

### Wine Support
**For Wine compatibility and modern installation methods**
- **[WINE_SUPPORT_MODERN.md](WINE_SUPPORT_MODERN.md)** - Modern Wine support installation guide
- **[AppImage_WineBridge.md](AppImage_WineBridge.md)** - AppImage + Wine bridge
- **[troubleshooting/TROUBLESHOOTING.md](troubleshooting/TROUBLESHOOTING.md)** - Wine troubleshooting section

### Technical Documentation (`technical/`)
**For developers and technical implementation**
- **[technical/README.md](technical/README.md)** - Technical documentation overview
- **[technical/TECHNICAL_IMPLEMENTATION_GUIDE.md](technical/TECHNICAL_IMPLEMENTATION_GUIDE.md)** - Implementation details
- **[technical/GAME_DATA_SYSTEM.md](technical/GAME_DATA_SYSTEM.md)** - Game data system architecture
- **[technical/WINE_VERSION_SELECTION_IMPLEMENTATION.md](technical/WINE_VERSION_SELECTION_IMPLEMENTATION.md)** - Wine version picker implementation
- **[technical/TRACKIR_5.5.3_DLL_ARCHITECTURE_ANALYSIS.md](technical/TRACKIR_5.5.3_DLL_ARCHITECTURE_ANALYSIS.md)** - TrackIR DLL layout analysis
- **[technical/TRACKIR_FIX_README.md](technical/TRACKIR_FIX_README.md)** - TrackIR fixes documentation
- **[technical/MFC42_WINETRICKS_INSTALLATION_GUIDE.md](technical/MFC42_WINETRICKS_INSTALLATION_GUIDE.md)** - Winetricks MFC42 install
- **[technical/MFC42_ALTERNATIVE_INSTALLATION_METHODS.md](technical/MFC42_ALTERNATIVE_INSTALLATION_METHODS.md)** - MFC42 fallback methods
- **[technical/MFC42_ALTERNATIVE_INSTALLATION_SUMMARY.md](technical/MFC42_ALTERNATIVE_INSTALLATION_SUMMARY.md)** - MFC42 method summary
- **[technical/OPENTRACK_PLUGIN_PLAN.md](technical/OPENTRACK_PLUGIN_PLAN.md)** - OpenTrack plugin plan
- **[technical/ROBUST_HELP_SYSTEM.md](technical/ROBUST_HELP_SYSTEM.md)** - Qt help system design
- **[technical/QT_HELP_SYSTEM_FIX.md](technical/QT_HELP_SYSTEM_FIX.md)** - Qt help system fixes
- **[technical/ADVANCED_INSTALLATION_AND_DEPENDENCIES.md](technical/ADVANCED_INSTALLATION_AND_DEPENDENCIES.md)** - Deep install dependencies
- **[technical/INSTALLATION_ANALYSIS_COMPREHENSIVE.md](technical/INSTALLATION_ANALYSIS_COMPREHENSIVE.md)** - Install path analysis
- **[technical/TrackIR_MFC42_modern_windows_analysis.md](technical/TrackIR_MFC42_modern_windows_analysis.md)** - MFC42 on Windows analysis

---

## Quick Reference by Issue

### Most Common Issues
1. **TrackIR permission errors** → [troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)
2. **Firmware extraction fails** → [troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md](troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md)
3. **ApplicationID shows `((null))` in gamedata.txt** → [troubleshooting/APPLICATIONID_PARSING_ISSUE.md](troubleshooting/APPLICATIONID_PARSING_ISSUE.md)
4. **Wine support installation fails** → [WINE_SUPPORT_MODERN.md](WINE_SUPPORT_MODERN.md)
5. **Application won't launch** → [troubleshooting/TROUBLESHOOTING.md](troubleshooting/TROUBLESHOOTING.md)
6. **Display issues on Wayland** → [troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md](troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md)

### AppImage-Specific Issues
7. **Help system not working in AppImage** → [technical/ROBUST_HELP_SYSTEM.md](technical/ROBUST_HELP_SYSTEM.md) and [technical/QT_HELP_SYSTEM_FIX.md](technical/QT_HELP_SYSTEM_FIX.md)
8. **AppImage Steam/Flatpak issues** → [troubleshooting/STEAM_FLATPAK_DEBUG_LOGS.md](troubleshooting/STEAM_FLATPAK_DEBUG_LOGS.md) and [readme/flatpak.md](readme/flatpak.md)

### Distribution-Specific Guides
- **Ubuntu/Debian / MX Linux** → [readme/debian-ubuntu.md](readme/debian-ubuntu.md)
- **Fedora/RHEL** → [readme/fedora-rhel.md](readme/fedora-rhel.md)
- **Arch Linux** → [readme/arch-linux.md](readme/arch-linux.md)

### Development Resources
- **Build system** → [technical/TECHNICAL_IMPLEMENTATION_GUIDE.md](technical/TECHNICAL_IMPLEMENTATION_GUIDE.md)
- **Wine bridge** → [guides/WINE_BRIDGE_MODERN.md](guides/WINE_BRIDGE_MODERN.md), [src/wine_bridge/WINE_SETUP.md](../src/wine_bridge/WINE_SETUP.md)
- **Project releases / changelog** → [../CHANGELOG.md](../CHANGELOG.md), [../README.md](../README.md)
- **Wiki maintenance** → `tools/wiki/convert_help_to_wiki.py` (uses `LINUXTRACK_WIKI_ROOT` or `../linuxtrackx-ir.wiki`)

---

## Emergency Procedures

### Quick Recovery (Immediate)
1. **Run automated recovery**: `./scripts/install/linuxtrack_quick_recovery.sh`
2. **Check status**: `ltr_gui`, `lsusb | grep 131d`, and `groups "$USER"`
3. **If still broken**: See [troubleshooting/README.md](troubleshooting/README.md)

### Hardware Issues
1. **[troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)** - Hardware troubleshooting
2. **Basic diagnostics**: `lsusb | grep 131d` for TrackIR detection
3. **Permission fixes**: See udev rules sections

---

## Documentation by Audience

### End Users
- **[README.md](../README.md)** - Main overview
- **[guides/README.md](guides/README.md)** - User guides
- **[readme/troubleshooting.md](readme/troubleshooting.md)** - Health checks and common fixes
- **[troubleshooting/TROUBLESHOOTING.md](troubleshooting/TROUBLESHOOTING.md)** - Full troubleshooting

### System Administrators
- **[readme/troubleshooting.md](readme/troubleshooting.md)** - Common issues and maintenance hints
- **[troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)** - Permission management

### Developers
- **[technical/README.md](technical/README.md)** - Technical documentation
- **[technical/TECHNICAL_IMPLEMENTATION_GUIDE.md](technical/TECHNICAL_IMPLEMENTATION_GUIDE.md)** - Implementation details
- **[technical/GAME_DATA_SYSTEM.md](technical/GAME_DATA_SYSTEM.md)** - Game data system

### Testers
- **[testing/README.md](testing/README.md)** - Testing procedures
- **[testing/TESTING_GUIDE.md](testing/TESTING_GUIDE.md)** - Testing guidelines
- **[README.md](../README.md)** - Expected install levels and behavior

---

## Search Guide

### By Keywords

#### "TrackIR" or "Hardware"
- **[troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)**
- **[README.md](../README.md)**
- **[guides/README.md](guides/README.md)**

#### "Permission" or "Access Denied"
- **[troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)**
- **[readme/troubleshooting.md](readme/troubleshooting.md)**

#### "Wayland" or "Display"
- **[troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md](troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md)**
- `scripts/test/run_gui_x11.sh` — force X11 mode on Wayland

#### "Wine" or "Windows"
- **[WINE_SUPPORT_MODERN.md](WINE_SUPPORT_MODERN.md)**
- **[AppImage_WineBridge.md](AppImage_WineBridge.md)**
- **[guides/WINE_BRIDGE_MODERN.md](guides/WINE_BRIDGE_MODERN.md)**
- **[troubleshooting/TROUBLESHOOTING.md](troubleshooting/TROUBLESHOOTING.md)** (Wine section)

#### "AppImage" or "Packaging" or "Distribution"
- **[AppImage_WineBridge.md](AppImage_WineBridge.md)**
- **[technical/ROBUST_HELP_SYSTEM.md](technical/ROBUST_HELP_SYSTEM.md)**
- `scripts/appimage/v2/` — AppImage build pipeline scripts

#### "ApplicationID" or "gamedata.txt" or "((null))"
- **[troubleshooting/APPLICATIONID_PARSING_ISSUE.md](troubleshooting/APPLICATIONID_PARSING_ISSUE.md)**
- **[technical/GAME_DATA_SYSTEM.md](technical/GAME_DATA_SYSTEM.md)**
- **[troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md](troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md)**

---

## Quick Reference

### Emergency Commands
```bash
# Automatic recovery
./scripts/install/linuxtrack_quick_recovery.sh

# Rebuild GUI after CMake changes (from repo root)
mkdir -p build && cd build && cmake .. -DCMAKE_INSTALL_PREFIX=/opt && cmake --build . -j$(nproc)

# Check TrackIR hardware
lsusb | grep 131d

# Launch GUI helper (from repo root)
./scripts/test/run_gui.sh
```

### Key File Locations
- **Executable**: `src/qt_gui/ltr_gui`
- **Libraries**: `src/.libs/lib*.so*`
- **Configuration**: `~/.config/linuxtrack/linuxtrack1.conf`
- **udev Rules**: `/lib/udev/rules.d/99-TIR.rules`
- **Launch helper**: `scripts/test/run_gui.sh` (sets library paths, launches `ltr_gui`)

---

## Support Resources

### Self-Help (Recommended)
1. **Health Check**: `ltr_gui`, `lsusb | grep 131d`, and `groups "$USER"`
2. **Quick Recovery**: `./scripts/install/linuxtrack_quick_recovery.sh`
3. **Troubleshooting Guides**: See issue-specific guides above

### Contributing to Documentation
- **Location**: All `.md` files in organized subdirectories under `docs/`
- **Style**: Markdown
- **Testing**: Verify all commands and procedures before documenting

---

**Documentation Status**: Actively maintained (see `readme/` and `guides/` for current install paths).
**Automation**: `scripts/install/linuxtrack_quick_recovery.sh`
