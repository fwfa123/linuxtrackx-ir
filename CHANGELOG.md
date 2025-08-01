# Changelog - LinuxTrack X-IR

This document tracks the changes and improvements made in the LinuxTrack X-IR fork compared to the original LinuxTrack project.

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

## [Unreleased] - Current Development

### Added
- **Arch Linux Wine Optimization**: Comprehensive optimization of Wine installation for Arch Linux
- **Parallel Build Support**: Optimized builds using all CPU cores with `-j$(nproc)`
- **ccache Integration**: 70-90% faster future builds through compiler caching
- **CPU-Specific Optimizations**: Hardware-specific compiler flags for better performance
- **Complete Wine Suite**: wine-stable, wine-mono, wine-gecko installation approach
- **Enhanced Documentation**: Step-by-step Arch Linux installation with troubleshooting
- **Community Attribution**: Proper credit to Arch Linux community and GitHub Issue #206 contributors

### Changed
- **Arch Linux Installation**: Replaced outdated wine32 approach with optimized wine-stable
- **README Updates**: Comprehensive Arch Linux Wine troubleshooting and optimization guide
- **Conflict Resolution**: Added guidance for wine-staging vs wine-stable conflicts
- **Performance**: Dramatically improved Wine build times (hours → minutes)
- **User Experience**: Clear, optimized instructions for Arch Linux users

### Fixed
- **Arch Linux Wine Conflicts**: Resolved wine-staging vs wine-stable package conflicts
- **Missing Dependencies**: Fixed lib32-unixodbc dependency issues
- **Build Performance**: Eliminated hours-long Wine builds through optimization
- **Documentation**: Added comprehensive troubleshooting for Arch Linux Wine issues

### Acknowledgments
- **Arch Linux Community**: Foundation for Wine optimization techniques
- **GitHub Issue #206 Contributors**: [uglyDwarf/linuxtrack#206](https://github.com/uglyDwarf/linuxtrack/issues/206) - Community members who contributed to solving Wine compatibility issues
- **AUR Package Maintainers**: Maintainers of wine-stable and wine-stable-mono packages
- **Arch Linux Wine Users**: Community members who tested and validated optimization approaches

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
| **Version Number** | 0.99.20 | 0.99.21 |

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