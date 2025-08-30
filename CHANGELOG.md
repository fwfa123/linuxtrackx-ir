# Changelog - LinuxTrack X-IR

This document tracks the changes and improvements made in the LinuxTrack X-IR fork compared to the original LinuxTrack project.

> **Note**: This version (0.99.24) continues development on GitLab for future development and releases.

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