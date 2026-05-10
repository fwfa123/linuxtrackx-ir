# LinuxTrack Documentation Index

## 📚 Complete Documentation Guide

This index organizes LinuxTrack documentation. **The supported GUI/toolchain for new releases is Qt6 with CMake** (see [README.md](../README.md) and [readme/](readme/)). Files that refer to Qt5, qmake, or autotools are mostly **historical** migration or troubleshooting material.

---

## 🚀 Getting Started

### New Users
1. **[README.md](../README.md)** - Main project overview and quick start guide
2. **[readme/debian-ubuntu.md](readme/debian-ubuntu.md)** / **[readme/fedora-rhel.md](readme/fedora-rhel.md)** / **[readme/arch-linux.md](readme/arch-linux.md)** - Distribution-specific install (Qt6 / CMake)
3. **[testing/README.md](testing/README.md)** - Testing overview

### Existing Users (After System Changes)
1. **[troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)** - ⚠️ **CRITICAL** - System upgrade resolution
2. **[troubleshooting/QUICK_TROUBLESHOOTING_QT5.md](troubleshooting/QUICK_TROUBLESHOOTING_QT5.md)** - Quick fixes for common issues
3. **[readme/troubleshooting.md](readme/troubleshooting.md)** - Current diagnostics and common fixes

---

## 📁 Documentation Organization

### 🛠️ User Guides (`guides/` + `readme/`)
**For end users and system administrators**
- **[guides/README.md](guides/README.md)** - Guide index and cross-links
- **[guides/MX_LINUX_BUILD_GUIDE.md](guides/MX_LINUX_BUILD_GUIDE.md)** - MX Linux build guide
- **[guides/ARCH_LINUX_INSTALL_GUIDE.md](guides/ARCH_LINUX_INSTALL_GUIDE.md)** - Redirect to [readme/arch-linux.md](readme/arch-linux.md)
- **[readme/debian-ubuntu.md](readme/debian-ubuntu.md)**, **[readme/fedora-rhel.md](readme/fedora-rhel.md)**, **[readme/arch-linux.md](readme/arch-linux.md)** - Primary per-distro docs
- **[archive/guides/](archive/guides/)** - Historical `LEGACY_*` snapshots only

### 🧪 Testing Documentation (`testing/`)
**For testing and validation procedures**
- **[testing/README.md](testing/README.md)** - Testing guide overview
- **[testing/TESTING_GUIDE.md](testing/TESTING_GUIDE.md)** - General testing guidelines (13KB)

### 🔧 Troubleshooting (`troubleshooting/`)
**For problem resolution and diagnostics**
- **[troubleshooting/README.md](troubleshooting/README.md)** - Troubleshooting guide overview
- **[troubleshooting/TROUBLESHOOTING.md](troubleshooting/TROUBLESHOOTING.md)** - Comprehensive troubleshooting guide (14KB)
- **[troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)** - System upgrade conflicts (9KB)
- **[troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)** - Hardware permission issues (9KB)
- **[troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md](troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md)** - TrackIR firmware extraction issues (8KB)
- **[troubleshooting/APPLICATIONID_PARSING_ISSUE.md](troubleshooting/APPLICATIONID_PARSING_ISSUE.md)** - ApplicationID parsing bug causing ((null)) in gamedata.txt (12KB)

### 🍷 Wine Support
**For Wine compatibility and modern installation methods**
- **[WINE_SUPPORT_MODERN.md](WINE_SUPPORT_MODERN.md)** - Modern Wine support installation guide
- **[AppImage_WineBridge.md](AppImage_WineBridge.md)** - AppImage + Wine bridge
- **[archive/WINE_BRIDGE_PACKAGING_PLAN.md](archive/WINE_BRIDGE_PACKAGING_PLAN.md)** - Packaging notes (archived)
- **[troubleshooting/TROUBLESHOOTING.md](troubleshooting/TROUBLESHOOTING.md)** - Wine troubleshooting section
- **[scripts/wine_check.sh](../scripts/wine_check.sh)** - Wine compatibility checker script

### 💻 Technical Documentation (`technical/`)
**For developers and technical implementation**
- **[technical/README.md](technical/README.md)** - Technical documentation overview
- **[technical/TECHNICAL_IMPLEMENTATION_GUIDE.md](technical/TECHNICAL_IMPLEMENTATION_GUIDE.md)** - Implementation details (13KB)
- **[technical/GAME_DATA_SYSTEM.md](technical/GAME_DATA_SYSTEM.md)** - Game data system architecture (15KB)
- **[technical/QT4_TO_QT5_MIGRATION.md](technical/QT4_TO_QT5_MIGRATION.md)** - Historical Qt4→Qt5 migration (superseded by Qt6 stack; see [QT5_TO_QT6_MIGRATION.md](technical/QT5_TO_QT6_MIGRATION.md))
- **[technical/MODERNIZATION_PLAN.md](technical/MODERNIZATION_PLAN.md)** - Original modernization plan (7KB)
- **[archive/WINE_BRIDGE_PACKAGING_PLAN.md](archive/WINE_BRIDGE_PACKAGING_PLAN.md)** - Wine bridge packaging and distribution plan (13KB, archived)
- **[technical/APPIMAGE_IMPLEMENTATION_PLAN.md](technical/APPIMAGE_IMPLEMENTATION_PLAN.md)** - AppImage packaging implementation plan (25KB)
- **[technical/QT5_HELP_SYSTEM_APPIMAGE_FIX_PLAN.md](technical/QT5_HELP_SYSTEM_APPIMAGE_FIX_PLAN.md)** - AppImage help bundling notes (filename legacy; verify against current Qt6 AppImage build)

---

## 🎯 Quick Reference by Issue

### Most Common Issues
1. **System upgrade broke LinuxTrack** → [troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)
2. **TrackIR permission errors** → [troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)
3. **Firmware extraction fails** → [troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md](troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md)
4. **ApplicationID shows ((null)) in gamedata.txt** → [troubleshooting/APPLICATIONID_PARSING_ISSUE.md](troubleshooting/APPLICATIONID_PARSING_ISSUE.md)
5. **Wine support installation fails** → [WINE_SUPPORT_MODERN.md](WINE_SUPPORT_MODERN.md)
6. **Application won't launch** → [troubleshooting/QUICK_TROUBLESHOOTING_QT5.md](troubleshooting/QUICK_TROUBLESHOOTING_QT5.md)
7. **Display issues on Wayland** → [troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md](troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md)

### AppImage-Specific Issues
8. **Help system not working in AppImage** → [technical/QT5_HELP_SYSTEM_APPIMAGE_FIX_PLAN.md](technical/QT5_HELP_SYSTEM_APPIMAGE_FIX_PLAN.md)
9. **AppImage won't run on different distributions** → [technical/APPIMAGE_IMPLEMENTATION_PLAN.md](technical/APPIMAGE_IMPLEMENTATION_PLAN.md)

### Distribution-Specific Guides
- **MX Linux** → [guides/MX_LINUX_BUILD_GUIDE.md](guides/MX_LINUX_BUILD_GUIDE.md)
- **Ubuntu/Debian** → [readme/debian-ubuntu.md](readme/debian-ubuntu.md)
- **Fedora/RHEL** → [readme/fedora-rhel.md](readme/fedora-rhel.md)
- **Arch Linux** → [readme/arch-linux.md](readme/arch-linux.md) (WOW64 / CachyOS); historical Arch–Wine notes → [archive/technical/](archive/technical/) (`ISSUE_206_*`, `ARCH_LINUX_WINE_BRIDGE_*`)

### Development Resources
- **Qt / framework migration** → [technical/QT5_TO_QT6_MIGRATION.md](technical/QT5_TO_QT6_MIGRATION.md), [technical/QT4_TO_QT5_MIGRATION.md](technical/QT4_TO_QT5_MIGRATION.md) (older)
- **Build System** → [technical/TECHNICAL_IMPLEMENTATION_GUIDE.md](technical/TECHNICAL_IMPLEMENTATION_GUIDE.md)
- **Project status / releases** → [../CHANGELOG.md](../CHANGELOG.md), [../README.md](../README.md)

---

## 🚨 Emergency Procedures

### Quick Recovery (Immediate)
1. **Run automated recovery**: `./scripts/install/linuxtrack_quick_recovery.sh`
2. **Check status**: `ltr_gui`, `lsusb | grep 131d`, and `groups "$USER"`
3. **If still broken**: See [troubleshooting/README.md](troubleshooting/README.md)

### System Upgrade Recovery
1. **[troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)** - Complete upgrade resolution
2. **Emergency commands**: See "Emergency Recovery" sections
3. **Manual recovery**: See step-by-step procedures

### Hardware Issues
1. **[troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)** - Hardware troubleshooting
2. **Basic diagnostics**: `lsusb | grep 131d` for TrackIR detection
3. **Permission fixes**: See udev rules sections

---

## 📋 Documentation by Audience

### End Users
- **[README.md](../README.md)** - Main overview
- **[guides/README.md](guides/README.md)** - User guides
- **[troubleshooting/QUICK_TROUBLESHOOTING_QT5.md](troubleshooting/QUICK_TROUBLESHOOTING_QT5.md)** - Quick fixes
- **[readme/troubleshooting.md](readme/troubleshooting.md)** - Health checks and common fixes

### System Administrators
- **[readme/troubleshooting.md](readme/troubleshooting.md)** - Common issues and maintenance hints
- **[troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)** - Upgrade management
- **[troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)** - Permission management

### Developers
- **[technical/README.md](technical/README.md)** - Technical documentation
- **[technical/QT5_TO_QT6_MIGRATION.md](technical/QT5_TO_QT6_MIGRATION.md)** - Qt6 migration notes (current stack)
- **[technical/MODERNIZATION_PLAN.md](technical/MODERNIZATION_PLAN.md)** - Technical architecture (partially historical)
- **[technical/QT4_TO_QT5_MIGRATION.md](technical/QT4_TO_QT5_MIGRATION.md)** - Older Qt4→Qt5 details
- **[technical/QT5_HELP_SYSTEM_APPIMAGE_FIX_PLAN.md](technical/QT5_HELP_SYSTEM_APPIMAGE_FIX_PLAN.md)** - AppImage help system fix

### Testers
- **[testing/README.md](testing/README.md)** - Testing procedures
- **[testing/TESTING_GUIDE.md](testing/TESTING_GUIDE.md)** - Testing guidelines
- **[testing/README.md](testing/README.md)** - Manual testing checks
- **[README.md](../README.md)** - Expected install levels and behavior

---

## 🔍 Search Guide

### By Keywords

#### "Qt" framework (versions, upgrades)
- **[technical/QT5_TO_QT6_MIGRATION.md](technical/QT5_TO_QT6_MIGRATION.md)** — Qt5→Qt6 (current direction for `main`)
- **[troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)**
- **[technical/QT5_MODERNIZATION_STATUS.md](technical/QT5_MODERNIZATION_STATUS.md)** (historical context)
- **[troubleshooting/QUICK_TROUBLESHOOTING_QT5.md](troubleshooting/QUICK_TROUBLESHOOTING_QT5.md)** (filename legacy; many items apply to Qt6 builds)

#### "TrackIR" or "Hardware"
- **[troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)**
- **[README.md](../README.md)**
- **[guides/README.md](guides/README.md)**

#### "System Upgrade" or "Broken"
- **[troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)**
- **[scripts/install/linuxtrack_quick_recovery.sh](../scripts/install/linuxtrack_quick_recovery.sh)**
- **[readme/troubleshooting.md](readme/troubleshooting.md)**

#### "Permission" or "Access Denied"
- **[troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md](troubleshooting/TRACKIR_PERMISSION_TROUBLESHOOTING.md)**
- **[readme/troubleshooting.md](readme/troubleshooting.md)**

#### "Library" or "Loading"
- **[troubleshooting/QUICK_TROUBLESHOOTING_QT5.md](troubleshooting/QUICK_TROUBLESHOOTING_QT5.md)**
- **[troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](troubleshooting/QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)**

#### "Wayland" or "Display"
- **[troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md](troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md)**
- **[run_qt5_gui_x11.sh](../scripts/test/run_qt5_gui_x11.sh)** (legacy script name; Qt6 build)

#### "Wine" or "Windows"
- **[WINE_SUPPORT_MODERN.md](WINE_SUPPORT_MODERN.md)**
- **[AppImage_WineBridge.md](AppImage_WineBridge.md)**
- **[archive/WINE_BRIDGE_PACKAGING_PLAN.md](archive/WINE_BRIDGE_PACKAGING_PLAN.md)**
- **[troubleshooting/TROUBLESHOOTING.md](troubleshooting/TROUBLESHOOTING.md)** (Wine section)
- **[scripts/wine_check.sh](../scripts/wine_check.sh)**

#### "AppImage" or "Packaging" or "Distribution"
- **[technical/APPIMAGE_IMPLEMENTATION_PLAN.md](technical/APPIMAGE_IMPLEMENTATION_PLAN.md)**
- **[technical/QT5_HELP_SYSTEM_APPIMAGE_FIX_PLAN.md](technical/QT5_HELP_SYSTEM_APPIMAGE_FIX_PLAN.md)**
- **[archive/WINE_BRIDGE_PACKAGING_PLAN.md](archive/WINE_BRIDGE_PACKAGING_PLAN.md)**
- **[technical/MODERNIZATION_PLAN.md](technical/MODERNIZATION_PLAN.md)** (Packaging section)

#### "ApplicationID" or "gamedata.txt" or "((null))"
- **[troubleshooting/APPLICATIONID_PARSING_ISSUE.md](troubleshooting/APPLICATIONID_PARSING_ISSUE.md)**
- **[technical/GAME_DATA_SYSTEM.md](technical/GAME_DATA_SYSTEM.md)**
- **[troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md](troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md)**

---

## 🎯 Quick Reference

### Emergency Commands
```bash
# Automatic recovery
./scripts/install/linuxtrack_quick_recovery.sh

# Rebuild GUI after CMake changes (from repo root)
mkdir -p build && cd build && cmake .. -DCMAKE_INSTALL_PREFIX=/opt && cmake --build . -j$(nproc)

# Check TrackIR hardware
lsusb | grep 131d

# Launch application (example helper; from repo root; legacy script name)
./scripts/test/run_qt5_gui.sh
```

### Key File Locations
- **Executable**: `src/qt_gui/ltr_gui`
- **Libraries**: `src/.libs/lib*.so*`
- **Configuration**: `~/.config/linuxtrack/linuxtrack1.conf`
- **udev Rules**: `/lib/udev/rules.d/99-TIR.rules`
- **Launch helpers**: `scripts/test/run_qt5_gui.sh` (legacy name; Qt6-built `ltr_gui`) or `ltr_gui` after install

---

## 📞 Support Resources

### Self-Help (Recommended)
1. **Health Check**: `ltr_gui`, `lsusb | grep 131d`, and `groups "$USER"`
2. **Quick Recovery**: `./scripts/install/linuxtrack_quick_recovery.sh`
3. **Troubleshooting Guides**: See issue-specific guides above

### Documentation History
- **Created**: June 2025 during Qt5 modernization
- **Reorganized**: July 2025 with category-based structure
- **2026**: Qt6 + CMake documented as current release stack; legacy Qt5/qmake references retained only where historically useful

### Contributing to Documentation
- **Location**: All .md files in organized subdirectories
- **Style**: Markdown with emoji indicators
- **Testing**: Verify all commands and procedures before documenting

---

**Documentation Status**: Actively maintained (see `readme/` and `guides/` for current install paths).  
**Automation**: `scripts/install/linuxtrack_quick_recovery.sh`

This documentation index provides complete navigation for all LinuxTrack resources, organized by category for easy access and maintenance. 