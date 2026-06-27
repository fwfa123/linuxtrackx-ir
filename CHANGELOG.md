# Changelog - LinuxTrack X-IR

This document tracks the changes and improvements made in the LinuxTrack X-IR fork compared to the original LinuxTrack project.

> **Note**: **2.0.x** is on `main`. **1.0.x** maintenance and the pre-2.0 AppImage line remain on `legacy/1.0.x`.

## [2.1.0] - 2026-06-27

### Added
- **Detailed Axes Setup** overhaul: live pose strip, resizable response graph with coordinate grid, pan tuning sync, and **Pose maps** (Look, Lean, Raise/Roll) with mapper-style teal guide rings, live red dead-zone overlay, and green position dot (measured or filtered source).
- **Tracking Setup**: **Restore axis defaults** and **Save as axis defaults** using a separate `AxisBaseline` profile section (distinct from the Default profile used for new games).

### Fixed
- Pose map dead-zone overlay now uses normalized `AXIS_DEADZONE` fraction (0–1), matching the axis sliders.
- Wine bridge install path resolution for custom Wine prefixes.
- **Build**: CMake `-m32` compile probe for 32-bit `linuxtrack32` headers; Debian/Ubuntu multilib packages documented in `docs/readme/debian-ubuntu.md`.

## [2.0.1] - 2026-06-13

### Fixed
- **AppImage**: embed modern static `type2-runtime` (FUSE3) via updated `appimagetool`; no longer requires host `libfuse2` on FUSE3-only distros (Ubuntu 24.04+, Fedora Atomic, etc.). Build pipeline fetches/pins runtime, rejects legacy AppImageKit `5735cc5` runtime.

## [2.0.0] - 2026-05-24 — WOW64 rearchitecture (now on `main`)

### Changed
- **Docs / UI**: Removed stated Wine 11+ and WOW64-only runtime requirements; bridge supports 32-bit and 64-bit Wine/Proton prefixes (MinGW-w64 still required at build time).
- **Branch policy**: `main` is 2.0.0; `legacy/1.0.x` preserves the 1.0.2 / 0.99.x release line.

### Major Changes
- **Version 2.0.0**: Major version bump reflecting a new application architecture (Wine WOW64 bridge, Qt6/CMake-first build, modern toolchain requirements) rather than incremental `0.99.x` updates.
- **Wine bridge**: WOW64-focused build and install path (see `docs/guides/WINE_BRIDGE_MODERN.md`).
- **Breaking — NSIS installer removed**: `linuxtrack-wine.exe`, `makensis`, and NSIS stub dependencies are no longer used. Wine bridge builds with **MinGW only**. Lutris, Steam Proton, custom prefix, and AppImage all install via native file copy + `wine reg` (`WineBridgeInstall` in `ltr_gui`, or `scripts/install/install_wine_bridge.sh`).

### Fixed
- **AppImage + Flatpak**: Steam and Lutris detection when installed via Flatpak (ported from main 1.0.2, GitLab #56). Sanitized host `flatpak` invocation and data-first `~/.var/app/...` paths.

## [0.99.30] - Lutris, AppImage & UI Improvements (March 2026)

### Major Changes
- **Lutris / Flatpak / Wine**: Lutris Flatpak wine binary resolution and Proton via `files/bin/wine` (Issue #43); Flatseal doc for library path; Fedora 43 wine-wow64 paths and Wine lib probing
- **UI / Gaming tab**: New Advanced tab (Linuxtrack Server moved from Gaming tab, with warning); Gaming tab "Install Wine Bridge" label, updated help; layout and help image updates
- **Tracking**: Restore Linux PS3Eye (1415:2000) in device selector; fix freeze when re-starting tracking after Stop (Issue #47); fix smoothing save states (Issue #44)

### Fixed
- AppImage GUI unable to find `ltr_pipe` executable
- SIGPIPE crash when tester program exits
- Issue #41: udev path detection, graceful Start failure, socket close on connect fail
- udev rules path: use `/etc/udev/rules.d/` for local admin rules

### Changed
- Troubleshooting and README docs: udev rules paths, quick issues section

**Resolves:** GitHub Issues #41, #43, #44, #47

## [0.99.29] - TIR5V2 Compatibility Release (January 2026)

### Major Changes
- **TrackIR 5 Revision 2 (TIR5V2) Support**: Critical fixes resolving timeout issues that prevented TIR5V2 hardware from initializing properly
- **Enhanced Timing System**: Separate TIR5V2 interface with doubled video delay (240ms) for proper memory mapping support
- **Comprehensive Logging**: TIR5V2-specific USB packet logging for improved debugging capabilities

### Added
- **TIR5V2 Interface Functions**: `init_camera_tir5v2()`, `start_camera_tir5v2()`, `stop_camera_tir5v2()`, `close_camera_tir5v2()`
- **USB Logging Infrastructure**: `log_usb_send()`, `log_usb_receive()`, `log_usb_error()` for TIR5V2 debugging
- **Timing Measurements**: Comprehensive initialization and operation timing logged for diagnostics
- **Device Routing**: Automatic TIR5V2 device detection and routing to enhanced interface

### Fixed
- **Critical**: TIR5V2 timeout issues - "Data receive request timed out" errors resolved
- **Critical**: TIR5V2 device initialization now completes successfully
- FlightGear Z-axis issues with TrackIR
- Build error in hotkey_setup.ui (include path for myLineEdit custom widget)
- USB logging helper function scope issues (compilation error)

### Changed
- **TIR5V2 Initialization**: Enhanced timing throughout camera startup sequence (10-50ms delays)
- **Video On Delay**: TIR5V2 devices now use 240ms delay (doubled from 120ms) for different memory mapping
- **Device Interface Selection**: TIR5V2 devices automatically use `tir5v2` interface while maintaining TIR5 compatibility
- **Logging Verbosity**: Added comprehensive TIR5V2-specific logging with `[TIR5V2]` prefixes

### Technical Details
- TIR5V2 uses different memory address mapping (`0x00886904` vs TIR5's `0x005C6704`)
- TIR5V2 firmware is 69 bytes larger than TIR5
- Requires longer delays for memory access operations
- Separate interface ensures TIR5 devices continue working normally
- Available in both Qt5 (main) and Qt6 (qt6-migration) branches

**Resolves:** GitHub Issue #36

## [0.99.25] - Arch Linux Wine32 Alternative Sources & Enhanced Wine Integration (2025)

### Major Changes
- **Alternative MFC42.dll Sources**: Added VS6SP6.EXE and VC6RedistSetup_deu.exe sources for Arch Linux users without wine32
- **Enhanced Wine Integration**: Fixed Lutris Wine path resolution and improved Wine version detection
- **Automatic Installation**: Streamlined installation process with automatic post-installation steps
- **Professional UI**: Cleaned up Wine bridge application titles and improved user experience

### Added
- Alternative MFC42.dll download sources with SHA256 verification
- Smart extraction method selection (cabextract vs Wine installer) based on source type
- Automatic installation hooks for library path configuration and desktop integration
- Enhanced Lutris Wine path resolution with absolute path detection
- Comprehensive Debian build analysis documentation
- MFC42.dll symlink creation in winebridge installer

### Changed
- Manual download code path now properly parses pipe-delimited source metadata
- Wine bridge application titles no longer display version numbers
- Post-installation script transformed from installation to verification script
- Enhanced Wine version detection with support for various runner patterns
- Improved YAML parsing for Lutris game configurations

### Fixed
- **Critical**: Missing mfc42.dll symlink creation in winebridge installer
- **Critical**: Lutris Wine path doubling issue causing invalid paths
- Wine version detection and path resolution issues
- NPTest dialog architecture identification
- Library loading issues with automatic ldconfig execution
- Desktop file installation and cache update issues

### Technical Details
- MFC42 extraction supports both cabextract and Wine installer methods
- SHA256 verification ensures download integrity
- Smart path resolution detects absolute vs relative Wine paths
- Automatic installation eliminates manual post-installation steps
- Enhanced cross-distribution compatibility for Arch Linux users

## [0.99.24] - Cross-Distribution Compatibility & Wine Bridge Improvements (2025)

### Major Changes
- **Complete Cross-Distribution Support**: Full Fedora/Nobara Linux compatibility with automatic package detection
- **Flatpak Gaming Platform Integration**: Seamless support for Flatpak-managed Steam and Lutris installations
- **Enhanced Wine Bridge Detection**: Robust winetricks detection with Flatpak sandbox support
- **Critical Bug Fixes**: Resolved Debian build issues and FreeTrack tester selection problems

### Added
- Comprehensive Fedora/Nobara Wine setup with correct package names (`qt5-qttools-devel`, `mxml-devel`, `libv4l-devel`, `mingw-nsis-base`)
- Flatpak Steam/Lutris detection and integration (`FlatpakDetector` class)
- Enhanced winetricks detection with multiple fallback paths and Flatpak host support
- Improved RPM package detection for Fedora-based distributions

### Changed
- AppImage v2 config updated to use version 0.99.24
- QT Help "What's New" page updated with v0.99.24 features
- Wine bridge Makefiles properly isolate -m32 flag usage to Wine components only

### Fixed
- **Critical**: Global -m32 compilation flag removed (was breaking Debian builds)
- FreeTrack tester selection logic corrected in Gaming tab
- Enhanced Lutris detection for RPM-based distributions
- Wine bridge error handling and user feedback improvements

### Technical Details
- Wine bridge components use -m32 locally in Makefiles (not globally)
- Flatpak detection supports both native and sandboxed environments
- Cross-distribution package detection with fallback mechanisms
- Enhanced logging and debugging throughout Wine bridge functionality

## [0.99.23] - Modular AppImage Builder v2 and Help System Fix (2025)

### Major Changes
- **AppImage Builder v2**: New modular, configuration-driven pipeline under `scripts/appimage/v2/` (build → bundle → wine_bridge → optimize → validate → package)
- **Self-contained Qt**: Bundle Qt libs and plugins; explicit `QT_SQL_DRIVER_PATH` ensures Help system works without host Qt

### Added
- `scripts/appimage/v2/` with `config.sh`, `common.sh`, `prepare.sh`, `bundle.sh`, `wine_bridge.sh`, `optimize.sh`, `validate.sh`, `package.sh`, `ci_build.sh`
- Validation step: ldd audit, Qt plugin checks, rpath harmonization

### Changed
- AppRun sets plugin/search paths and prefers XCB; rpaths adjusted for local libs
- Documentation updated to reference new AppImage build and artifact names

### Fixed
- **Help System**: Missing SQLite driver inside AppImage resolved; QtHelp loads correctly

## [0.99.22] - Gaming Platform Integration & Automatic Tracking (2025)

### Major Changes
- **Complete Gaming Platform Integration**: Comprehensive Steam Proton and Lutris support
- **Modern UI Design**: Dedicated Gaming and System tabs for better organization
- **Enhanced User Experience**: Prerequisite gating (TrackIR firmware + MFC42), grouped installers, Advanced menu, testing workflow improvements

### Added
- **Gaming Tab UI Improvements**
  - Prerequisites group with live status and Install/Repair actions for TrackIR firmware and MFC42
  - Gated installer buttons and Testing section enabled when prerequisites met
  - Grouped installer targets (Steam, Lutris, Custom Prefix) with Advanced popup for Other Platform/Wine Prefix
  - Testing workflow: auto-load games, status label, filter, last platform/game persistence
  - Visual polish: icons, tooltips, accessibility names; translation via tr()
- **Complete Gaming Platform Integration**: Comprehensive Steam Proton and Lutris support
  - **Steam Proton Integration**: Multi-library support with Proton version detection
  - **Lutris Integration**: Complete Lutris prefix detection and Wine Bridge installation
  - **Proton version detection**: Including beta versions like Proton 9.0 Beta
  - **VDF parsing**: For Steam library management
  - **NSIS installer integration**: Within Proton environment
  - **Comprehensive error handling**: Robust error reporting and detailed logging
- **Modern UI Design**: Dedicated Gaming and System tabs
  - **Gaming Tab**: Consolidated all gaming-related functionality
  - **System Tab**: Comprehensive system information display
  - **Improved user experience**: Better organization and clearer interface
- **Arch Linux Wine Optimization**: Comprehensive optimization of Wine installation for Arch Linux
- **Parallel Build Support**: Optimized builds using all CPU cores with `-j$(nproc)`
- **ccache Integration**: 70-90% faster future builds through compiler caching
- **CPU-Specific Optimizations**: Hardware-specific compiler flags for better performance
- **Complete Wine Suite**: wine-stable, wine-mono, wine-gecko installation approach
- **Enhanced Documentation**: Step-by-step Arch Linux installation with troubleshooting
- **Community Attribution**: Proper credit to Arch Linux community and GitHub Issue #206 contributors

### Changed
- **User Workflow**: Non-autostart installer UX preserved; clearer gating and status-driven flow
- **Gaming Integration**: One-click Wine Bridge installation for both Lutris and Steam Proton games
- **UI Organization**: Reorganized interface with dedicated Gaming and System tabs
- **Error Handling**: Professional-grade error handling with detailed debugging
- **Arch Linux Installation**: Replaced outdated wine32 approach with optimized wine-stable
- **README Updates**: Comprehensive Arch Linux Wine troubleshooting and optimization guide
- **Conflict Resolution**: Added guidance for wine-staging vs wine-stable conflicts
- **Performance**: Dramatically improved Wine build times (hours → minutes)
- **User Experience**: Clear, optimized instructions for Arch Linux users

### Fixed
- **Build Errors**: Added missing tracker.h include to resolve compilation errors
- **Timing Issues**: Fixed tracking start timing to occur before installer begins
- **Arch Linux Wine Conflicts**: Resolved wine-staging vs wine-stable package conflicts
- **Missing Dependencies**: Fixed lib32-unixodbc dependency issues
- **Build Performance**: Eliminated hours-long Wine builds through optimization
- **Documentation**: Added comprehensive troubleshooting for Arch Linux Wine issues

## [0.99.21] - LinuxTrack X-IR Initial Release (2025)

### Major Changes
- **Version Bump**: Moved from 0.99.20 to 0.99.21 to reflect fork status and modernization progress
- **Project Renaming**: Updated project name to "LinuxTrack X-IR" throughout
- **Fork Identification**: Added "-xir.1" suffix to clearly identify this as the linuxtrackx-ir fork
- **Documentation Updates**: Comprehensive documentation reflecting current fork status

### Added
- **Fork Versioning**: Established semantic versioning with fork prefix (-xir.x)
- **Project Identity**: Clear differentiation from original LinuxTrack project
- **Updated Metadata**: Package names, changelog entries, and help system references

### Changed
- **Version Number**: 0.99.20 → 0.99.21
- **Project Name**: LinuxTrack → LinuxTrack X-IR
- **Package Names**: linuxtrack → linuxtrack-xir (in debian packaging)
- **Help System**: Updated Qt help files with new version references

### Versioning Strategy
This release establishes the versioning strategy for the linuxtrackx-ir fork:
- **Format**: `MAJOR.MINOR.PATCH-fork.RELEASE`
- **Example**: `0.99.21`, `0.99.22`, `0.99.23`
- **Rationale**: 
  - Version increment (0.99.20 → 0.99.21) reflects fork status and modernization progress
  - Fork suffix (-xir.x) clearly identifies this as the linuxtrackx-ir fork
  - Maintains semantic versioning principles
  - Acknowledges relationship to original project

## [2024] - Winegcc Build System Fixes

### Major Changes
- **Winegcc Integration**: Restored and fixed winegcc-based Windows compatibility
- **NSIS Script Fixes**: Corrected installer script template to use proper .exe.so/.dll.so extensions
- **C++ Linking Resolution**: Fixed missing C++ standard library linking in winegcc commands
- **Build System Stability**: Resolved all build errors and NSIS installer issues

### Added
- **Winegcc Support**: Restored winegcc for Wine-compatible binary generation
- **NSIS Template Fixes**: Updated ltr_wine.nsi.in to reference correct file extensions
- **C++ Library Linking**: Added -lstdc++ to winegcc commands for proper C++ support
- **Build Verification**: Comprehensive testing of clean build process

### Changed
- **Build System**: Fixed winegcc integration and NSIS installer generation
- **Windows Compatibility**: Restored winegcc approach for better Wine integration
- **Documentation**: Updated build guides to reflect winegcc requirements
- **Dependencies**: Clarified wine-devel package requirements

### Fixed
- **NSIS Installer Errors**: Resolved "no files found" errors in installer generation
- **C++ Linking Issues**: Fixed missing C++ standard library symbols in winegcc builds
- **Build Process**: Eliminated all build errors and warnings
- **File Extension Mismatches**: Corrected .exe.so/.dll.so vs .exe/.dll references

## [2024] - Qt5 Modernization and Wine Support

### Major Changes
- **Qt4 to Qt5 Migration**: Complete modernization of GUI framework
- **Wine-based Windows Support**: Wine-compatible binary generation using winegcc
- **Modern Build System**: Updated autotools and CMake support
- **Enhanced Security**: PIE, stack protector, and fortify source enabled by default

### Added
- **Wine Support**: Wine-based Windows .exe.so/.dll.so generation
- **Security Features**: Position Independent Executables (PIE) by default
- **Modern Dependencies**: Qt5, updated autotools, CMake support
- **Cross-Platform Builds**: Linux to Windows compilation via Wine
- **Enhanced Documentation**: Detailed build guides for multiple distributions

### Changed
- **Build System**: Updated from legacy autotools to modern versions
- **GUI Framework**: Migrated from Qt4 to Qt5
- **Windows Compatibility**: Wine-based approach for better integration
- **Security**: Enabled modern security features by default
- **Documentation**: Comprehensive guides for modern Linux distributions

### Removed
- **Qt4 Dependencies**: Removed legacy Qt4 requirements
- **Legacy Build Tools**: Updated to modern autotools versions

### Fixed
- **Build Issues**: Resolved compatibility with modern Linux distributions
- **Security Vulnerabilities**: Enabled modern security features
- **Dependency Conflicts**: Resolved package conflicts in modern distributions

## [Original] - LinuxTrack by uglyDwarf

### Original Features
- TrackIR 4 & 5 support via reverse engineering
- Qt4-based GUI application
- Wine-based Windows compatibility
- Wii Remote support
- Webcam/Face tracking
- X-Plane plugin support
- FreeTrack protocol support

### Original Contributors
- uglyDwarf - Original creator and main developer
- f.josef - Main developer and maintainer (302 commits)
- Tulthix - Early contributor
- Michal Navratil - Significant contributor (20 commits)
- raven - Contributor (4 commits)
- aledin - Contributor (2 commits)

---

## Version Comparison

| Feature | Original LinuxTrack | LinuxTrack X-IR |
|---------|-------------------|-----------------|
| **Qt Version** | Qt4 | Qt5 |
| **Windows Support** | Wine-based | Wine-based (winegcc) |
| **Build System** | Legacy autotools | Modern autotools + CMake |
| **Security Features** | Basic | PIE, stack protector, fortify |
| **Distribution Support** | Legacy Linux | Modern Linux distributions |
| **Development Status** | Abandoned | Active |
| **Documentation** | Basic | Comprehensive guides |
| **Version Number** | 0.99.20 | 0.99.22 |
| **Gaming Integration** | None | Steam Proton + Lutris support |
| **Automatic Tracking** | Manual | Automatic start before installers |
| **UI Modernization** | Basic Qt4 | Modern Gaming + System tabs |
| **Error Handling** | Basic | Comprehensive with detailed logging |

## Migration Notes

### For Users
- **Existing Users**: The fork maintains compatibility with original configurations
- **New Users**: Follow the updated installation guides for modern distributions
- **TrackIR Users**: Full compatibility maintained and enhanced

### For Developers
- **Build System**: Updated to modern autotools and CMake
- **Dependencies**: Qt5 instead of Qt4, winegcc for Wine compatibility
- **Security**: Modern security features enabled by default
- **Documentation**: Comprehensive guides and troubleshooting

---

**Note**: This changelog focuses on the differences between the original LinuxTrack project and this fork. For detailed technical changes, see the individual commit history and documentation in the `docs/` directory. 