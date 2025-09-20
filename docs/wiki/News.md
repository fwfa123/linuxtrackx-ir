# What's New in LinuxTrack X-IR

This page summarizes the most notable changes in the LinuxTrack X-IR software.

## LinuxTrack X-IR v0.99.25 - Arch Linux Wine32 Alternative Sources & Enhanced Wine Integration (September 2025)

### Alternative MFC42.dll Sources for Arch Linux
Added VS6SP6.EXE and VC6RedistSetup_deu.exe as alternative download sources for users on Arch Linux without wine32. The system automatically selects the appropriate extraction method (cabextract vs Wine installer) and verifies downloads with SHA256 checksums for security.

### Enhanced Wine Integration
Fixed critical Lutris Wine path resolution issues and improved Wine version detection. The system now properly handles absolute vs relative Wine paths and supports various Wine runner patterns including lutris-GE-Proton8-15-x86_64 and wine-ge-8-26-x86_64.

### Automatic Installation Process
Streamlined the installation experience with automatic post-installation steps. The system now handles library path configuration, desktop file installation, and cache updates automatically during 'sudo make install'.

### Professional User Interface
Cleaned up Wine bridge application titles by removing version numbers, providing a more professional appearance. Enhanced error messages and troubleshooting information for better user experience.

### Critical Bug Fixes
Fixed missing mfc42.dll symlink creation in winebridge installer and resolved Wine path doubling issues in Lutris integration. Improved NPTest architecture identification and library loading issues.

## LinuxTrack X-IR v0.99.24 - Cross-Distribution Compatibility & Wine Bridge Improvements (2025)

### Complete Cross-Distribution Support
Added comprehensive support for Fedora, Nobara Linux, and other RPM-based distributions. The Wine development setup now automatically detects and configures the correct packages for Fedora/Nobara, eliminating manual package resolution issues.

### Flatpak Gaming Platform Integration
Implemented full Flatpak support for Steam and Lutris installations. The application now automatically detects and integrates with Flatpak-managed gaming platforms, providing seamless Wine Bridge installation for Flatpak games.

### AppImage Builder Modernization
Completely redesigned the AppImage building process with modular architecture, deterministic builds, and enhanced cross-distribution compatibility. The new system provides better portability and more reliable deployments.

### Enhanced Wine Bridge Detection
Improved winetricks and Wine bridge detection with Flatpak sandbox support. The system now properly detects winetricks in sandboxed environments and provides robust fallback mechanisms for various installation scenarios.

### Critical Bug Fixes
Resolved critical Debian build issues caused by global -m32 compilation flags. Fixed FreeTrack tester selection logic and improved error handling throughout the Wine bridge functionality.

## LinuxTrack X-IR v0.99.23 - Modular AppImage Builder v2 and Help System Fix (2025)

### AppImage Builder v2
New modular, configuration-driven pipeline under `scripts/appimage/v2/` with build → bundle → wine_bridge → optimize → validate → package workflow. Self-contained Qt with bundled libraries and explicit `QT_SQL_DRIVER_PATH` ensures Help system works without host Qt dependencies.

### Validation System
Added comprehensive validation step with ldd audit, Qt plugin checks, and rpath harmonization. AppRun sets proper plugin/search paths and prefers XCB rendering.

### Critical Bug Fixes
Fixed Help System issues by bundling missing SQLite driver inside AppImage, ensuring QtHelp loads correctly across different Linux distributions.

## LinuxTrack X-IR v0.99.22 - Gaming Platform Integration & Automatic Tracking (2025)

### Complete Gaming Platform Integration
Comprehensive Steam Proton and Lutris support with modern UI design featuring dedicated Gaming and System tabs. Enhanced user experience with prerequisite gating, grouped installers, and testing workflow improvements.

### Modern UI Design
Reorganized interface with Gaming Tab for all gaming-related functionality and System Tab for comprehensive system information display. Added visual polish with icons, tooltips, and accessibility features.

### Enhanced User Experience
Implemented prerequisite gating (TrackIR firmware + MFC42), grouped installer buttons, Advanced menu, and testing workflow improvements. Added comprehensive error handling with detailed logging and professional-grade error reporting.

### Technical Improvements
Optimized builds using all CPU cores with `-j$(nproc)`, ccache integration for 70-90% faster future builds, and CPU-specific optimizations. Complete Wine suite installation approach with enhanced documentation and community attribution.

## LinuxTrack X-IR v0.99.21 - Qt5 Modernization (2025)

### Qt4 to Qt5 Migration
Complete modernization of the GUI framework from Qt4 to Qt5, ensuring compatibility with current Linux distributions and modern development practices.

### Wine-based Windows Support
Enhanced Wine integration using winegcc for generating Wine-compatible binaries. This approach provides better compatibility than cross-compilation methods.

### Modern Build System
Updated autotools and added CMake support for more flexible build configurations and better integration with modern development workflows.

### Enhanced Security Features
Enabled modern security features by default including Position Independent Executables (PIE), stack protector, and fortify source for improved security.

---

For installation instructions and getting started, see the [Welcome page](Welcome.md).

For detailed setup information, see the [Device Setup](Device-Setup.md) documentation.

For system information and troubleshooting, see the [System](System.md) documentation.
