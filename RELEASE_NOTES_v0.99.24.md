# LinuxTrack X-IR v0.99.24 Release Notes

## Overview

This release brings significant improvements to cross-distribution compatibility, Wine bridge functionality, and overall user experience. Major enhancements include Fedora/Nobara Linux full support, comprehensive Flatpak integration, and critical bug fixes for tester selection and Wine bridge packaging.

## New Features and Enhancements

### 🏗️ **0007: AppImage Builder Rework**
- **Complete redesign** of AppImage building process with modular architecture
- **Deterministic builds** with reproducible results and proper validation
- **Cross-distribution compatibility** with automatic library detection
- **Enhanced Wine bridge packaging** with proper 32-bit/64-bit support
- **CI-ready pipeline** with comprehensive error handling and logging

### 🐧 **0008: Flatpak Steam/Lutris Detection**
- **Full Flatpak integration** for both Steam and Lutris platforms
- **Automatic detection** of Flatpak installations alongside native packages
- **Seamless game library scanning** for Flatpak-managed games
- **Robust fallback mechanisms** when Flatpak is not available
- **Enhanced user feedback** with clear installation guidance

### 🔧 **0009: Lutris Detection Fix**
- **Improved RPM package detection** for Fedora and other RPM-based distributions
- **Binary-first detection logic** that works before user data creation
- **Enhanced logging and debugging** for troubleshooting detection issues
- **Backward compatibility** with existing detection methods

### 🎯 **0010: FT Tester Selection Fix**
- **Correct FreeTrack tester launching** from the Gaming tab
- **Proper tester type differentiation** between TrackIR and FreeTrack protocols
- **Architecture-aware selection** with automatic 32-bit/64-bit detection
- **Enhanced error handling** with clear user feedback

### 🍷 **0011: Winetricks Detection Enhancement**
- **Robust winetricks detection** across all Linux distributions
- **Flatpak sandbox support** with automatic host system detection
- **Improved path resolution** with fallback to common installation locations
- **Enhanced error messages** with Fedora-specific guidance
- **Version verification** to ensure winetricks functionality

### 🎯 **0012: Fedora/Nobara Wine Setup**
- **Complete Fedora compatibility** with correct package names
- **Full Nobara Linux support** as recognized Fedora-based distribution
- **Enhanced 32-bit library detection** for modern Fedora releases
- **Wine debug package cleanup** removing non-existent packages
- **Comprehensive documentation** with distribution-specific instructions

## Critical Bug Fixes

### 🔧 **Global -m32 Flag Issue Resolution**
- **Fixed Debian build breakage** caused by global `-m32` compilation flag
- **Wine bridge isolation** - `-m32` flag now only applied to Wine components
- **Regular LinuxTrack components** compile normally without forced 32-bit mode
- **Cross-distribution compatibility** restored for all supported platforms

### 🐛 **Additional Fixes**
- **Improved tester selection logic** in Gaming tab
- **Enhanced Wine bridge error handling** and user feedback
- **Fixed Flatpak game detection** edge cases
- **Corrected package dependency detection** for various distributions

## Platform Support

### ✅ **Fully Supported**
- **Ubuntu/Debian** (all versions with current LTS support)
- **Fedora** (38, 39, 40, 41, 42)
- **Nobara Linux** (all versions)
- **Arch Linux** (with AUR packages)
- **RHEL/CentOS** (7, 8, 9)
- **openSUSE** (Leap and Tumbleweed)

### 🚀 **Enhanced Support**
- **Flatpak applications** (Steam, Lutris) fully integrated
- **AppImage deployment** with improved portability
- **Multi-architecture builds** (x86_64 with 32-bit Wine support)

## Technical Improvements

### 🏗️ **Build System**
- **Modular AppImage builder** with configuration-driven approach
- **Enhanced cross-compilation support** for multi-architecture targets
- **Improved dependency resolution** with better error reporting
- **Deterministic builds** with reproducible artifacts

### 🍷 **Wine Bridge**
- **Robust winetricks integration** with multiple detection methods
- **Enhanced Flatpak compatibility** for sandboxed environments
- **Improved MFC42 installation** with better error handling
- **32-bit/64-bit architecture detection** and proper library selection

### 🎮 **Gaming Integration**
- **Steam Flatpak support** with automatic game detection
- **Lutris Flatpak support** with seamless integration
- **Enhanced game library scanning** for all supported platforms
- **Improved error messages** for installation troubleshooting

## User Experience Improvements

### 🎨 **Interface Enhancements**
- **Clearer error messages** with actionable guidance
- **Enhanced installation wizards** with better progress feedback
- **Improved tester selection** with visual feedback
- **Better Flatpak integration** with user-friendly prompts

### 📚 **Documentation**
- **Updated installation guides** for all supported distributions
- **Enhanced troubleshooting section** with common issues
- **Comprehensive platform-specific instructions**
- **Improved Wine bridge setup documentation**

## Migration Guide

### For Existing Users
1. **Reinstall Wine Bridge**: Due to tester improvements, reinstall the Wine bridge in all Wine/Proton prefixes
2. **Update MFC42 Runtime**: Reinstall TrackIR firmware and MFC42 components
3. **Test New Features**: Try the enhanced Flatpak support and improved tester selection

### For New Users
- **Follow updated installation guides** in the README for your specific distribution
- **Use the Gaming tab** for easier Wine bridge installation
- **Enable Flatpak detection** for Steam/Lutris games installed via Flatpak

## Known Issues and Limitations

### Wine Bridge
- **32-bit library requirements** may need manual installation on some systems
- **Flatpak sandboxing** may limit some advanced features
- **Wine version compatibility** may vary with different Wine/Proton versions

### Platform-Specific
- **Wayland support** is still experimental in some environments
- **NVIDIA driver compatibility** may require additional configuration
- **SELinux/AppArmor** policies may need adjustment for optimal performance

## Future Roadmap

### Planned for v0.99.25
- **Wayland native support** improvements
- **Enhanced VR headset integration**
- **Performance optimizations** for high-refresh-rate tracking
- **Additional platform support** (Gentoo, NixOS)

### Long-term Goals
- **Plugin architecture** for custom tracker support
- **Cloud synchronization** for settings and profiles
- **Mobile companion app** for configuration
- **Advanced calibration algorithms**

## Credits and Acknowledgments

### Contributors
- **Core Development Team**: fwfa123, community contributors
- **Testing and QA**: Fedora, Nobara, Ubuntu, and Arch Linux users
- **Documentation**: Comprehensive updates by the development team

### Special Thanks
- **Fedora/Nobara Community** for extensive testing and feedback
- **Wine Development Team** for ongoing compatibility support
- **Flatpak Maintainers** for sandboxing framework improvements

## Support and Resources

### Getting Help
- **GitLab Issues**: [Report bugs and request features](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues)
- **Documentation**: Comprehensive guides in `/docs/` directory
- **Community Forums**: GitLab discussions and issue threads

### System Requirements
- **Linux Kernel**: 4.15+ (5.0+ recommended)
- **GCC**: 7.0+ with C++17 support
- **Qt5**: 5.12+ with QtHelp and QtNetwork
- **Wine**: Staging/Proton 6.0+ for Wine bridge
- **RAM**: 2GB minimum, 4GB recommended
- **Storage**: 500MB for installation, additional space for Wine prefixes

---

**Release Date**: December 2024
**Version**: 0.99.24
**Previous Version**: 0.99.23
**Git Commit**: [TBD - will be updated after commit]

For detailed installation instructions, see `README.md`. For troubleshooting common issues, refer to `docs/troubleshooting/`.
