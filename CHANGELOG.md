# Changelog - LinuxTrack X-IR

This document tracks the changes and improvements made in the LinuxTrack X-IR fork compared to the original LinuxTrack project.

## [Unreleased] - Current Development

### Added
- Enhanced documentation with fork notice and attribution
- Improved build system documentation
- Better troubleshooting guides

### Changed
- Updated README with clear fork differentiation
- Enhanced AUTHORS file with proper attribution structure

## [2024] - Qt5 Modernization and MinGW Support

### Major Changes
- **Qt4 to Qt5 Migration**: Complete modernization of GUI framework
- **MinGW Cross-Compilation**: Native Windows binary support (no Wine required)
- **Modern Build System**: Updated autotools and CMake support
- **Enhanced Security**: PIE, stack protector, and fortify source enabled by default

### Added
- **MinGW Support**: Native Windows .exe/.dll generation
- **Security Features**: Position Independent Executables (PIE) by default
- **Modern Dependencies**: Qt5, updated autotools, CMake support
- **Cross-Platform Builds**: Linux to Windows compilation
- **Enhanced Documentation**: Detailed build guides for multiple distributions

### Changed
- **Build System**: Updated from legacy autotools to modern versions
- **GUI Framework**: Migrated from Qt4 to Qt5
- **Windows Compatibility**: Replaced Wine-based approach with MinGW
- **Security**: Enabled modern security features by default
- **Documentation**: Comprehensive guides for modern Linux distributions

### Removed
- **Wine Dependencies**: No longer requires wine-devel packages
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
| **Windows Support** | Wine-based | MinGW native |
| **Build System** | Legacy autotools | Modern autotools + CMake |
| **Security Features** | Basic | PIE, stack protector, fortify |
| **Distribution Support** | Legacy Linux | Modern Linux distributions |
| **Development Status** | Abandoned | Active |
| **Documentation** | Basic | Comprehensive guides |

## Migration Notes

### For Users
- **Existing Users**: The fork maintains compatibility with original configurations
- **New Users**: Follow the updated installation guides for modern distributions
- **TrackIR Users**: Full compatibility maintained and enhanced

### For Developers
- **Build System**: Updated to modern autotools and CMake
- **Dependencies**: Qt5 instead of Qt4, MinGW instead of Wine
- **Security**: Modern security features enabled by default
- **Documentation**: Comprehensive guides and troubleshooting

---

**Note**: This changelog focuses on the differences between the original LinuxTrack project and this fork. For detailed technical changes, see the individual commit history and documentation in the `docs/` directory. 