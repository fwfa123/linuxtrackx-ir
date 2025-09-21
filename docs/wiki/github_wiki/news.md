# What's new?

This page summarizes the most notable changes in the LinuxTrack X-IR software.

## LinuxTrack X-IR v0.99.26 - Comprehensive Help System Overhaul & AntiMicroX Integration (December 2024)

### Complete Help System Modernization

Completely overhauled the Qt help system with comprehensive documentation updates. All help files now feature proper image alignment, consistent formatting, and enhanced readability across all supported platforms.

### New Help Pages

Added several new help pages including TrackIR Permissions setup guide, comprehensive Gaming Tab documentation, and detailed AntiMicroX integration instructions. The help system now provides step-by-step guidance for all major features.

### AntiMicroX Integration Documentation

Added comprehensive documentation for using LinuxTrack with AntiMicroX to enable head tracking in games that don't natively support TrackIR. Includes detailed setup instructions, axis mapping guides, and real-world gaming examples like Jane's Longbow 2 and MechWarrior 2.

### Enhanced Gaming Tab Documentation

Created detailed documentation for the Gaming Tab interface, covering prerequisites, platform installation, testing workflows, and server configuration. The documentation includes visual guides and troubleshooting information.

### TrackIR Permission Setup Guide

Added a dedicated help page for TrackIR permission setup on Linux systems, including udev rules installation, user group configuration, and troubleshooting steps for common permission issues.

### Image Formatting Improvements

Fixed image alignment and text flow issues throughout the help system. All images now use consistent table-based formatting that works properly with Qt help systems, ensuring optimal display across different platforms.

### Cross-Reference Integration

Enhanced help navigation with improved cross-references between related topics, making it easier for users to find relevant information and follow complete setup workflows.

## LinuxTrack X-IR v0.99.25 - Arch Linux Wine32 Alternative Sources & Enhanced Wine Integration (September 2025)

### Alternative MFC42.dll Sources for Arch Linux

Added VS6SP6.EXE and VC6RedistSetup\_deu.exe as alternative download sources for users on Arch Linux without wine32. The system automatically selects the appropriate extraction method (cabextract vs Wine installer) and verifies downloads with SHA256 checksums for security.

### Enhanced Wine Integration

Fixed critical Lutris Wine path resolution issues and improved Wine version detection. The system now properly handles absolute vs relative Wine paths and supports various Wine runner patterns including lutris-GE-Proton8-15-x86\_64 and wine-ge-8-26-x86\_64.

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

New modular, configuration-driven pipeline under `scripts/appimage/v2/` with build → bundle → wine\_bridge → optimize → validate → package workflow. Self-contained Qt with bundled libraries and explicit `QT_SQL_DRIVER_PATH` ensures Help system works without host Qt dependencies.

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

## Linuxtrack 0.99.10 (Original)

### SDK overhaul

The SDK was overhauled to make it more programmer friendly; Linuxtrack GUI now contains SDK documentation and also two example programs are provided to make Linuxtrack's deployment easier. Another change is addition of "advanced" interface, allowing you to use Linuxtrack as a data source for your own "computer vision" applications.

### Model visualization

Each model type has its own distinct signature picture now, that should prevent problems caused by a wrong model selection.

## Linuxtrack 0.99.8 (Original)

### SmartNav3 support

SmartNav3 support was added; it is still in an early phase, but it should be stable.

## Linuxtrack 0.99.6 (Original)

### FreeTrack Client for Wine

FreeTrack client was added to Wine bridge, allowing games using this interface to utilize Linuxtrack.

## Linuxtrack 0.99.3 (Original)

### Versioning change

I decided to change the versioning scheme - instead of 1.0 beta3 to 0.99.3. Hopefully it will make
packaging a little bit easier.

### New Mac OS X package

After a long pause, caused by deep changes throughout the code and followed by a sudden departure
of my good old Mac Mini, I've created new Mac OS X package.

Please note, that I had to discontinue
support for Mac OS X 10.4 Tiger and 10.5 Leopard, along with Power PC platform - the new packages will
be targeted towards Mac OS X 10.6 Snow Leopard and up.

### Improved 3D preview

The 3D preview model has been reworked - the cockpit should look much better now and also the
sky sphere should add a bit to the impression. Also the new visualization is much less resource
hungry and should run well on low power devices (Atom powered netbooks, ...).

### X-Plane plug-in cooperates with Pilot View

X-Plane plug-in now cooperates with Sandy Barbour's Pilot View plug-in. Please reinstall the X-Plane
plug-in to get this new functionality and don't forget to enable external data usage in Pilot View.
Please note, that you need Pilot View 1.70 to have this feature available.

## Linuxtrack 1.0 Beta 2 (Original)

### Overhauled the pose computing

When using three point models, you can tweak them to minimize the unwanted viewpoint movement when
panning.

### Wine bridge fix

Fixed problems in the Wine bridge that prevented Condor soaring simulator and Falcon AF
to respond to the tracking. If you experience such a problem, please reinstall the Linuxtrack-Wine plug-in.

## Linuxtrack 1.0 Beta 1 (Original)

This Linuxtrack release is the beginning of the Linuxtrack 1.0 Beta run.

### Linuxtrack requires Wine now

If you use any device from NP (TrackIR, SmartNav4), you have to have Wine installed in order to get
a firmware needed to run the device.

When the firmware is extracted, you might find TrackIR icon on your desktop - it is perfectly safe
to remove it.

Technical info: Wine is being used to unpack windows TrackIR driver and extract the firmware from it.
The extraction takes place in a temporary Wine prefix, that is removed when done.

### 64bit X-Plane plug-in

Linuxtrack now contains 64bit enabled X-Plane plug-in - it should work in both X-Plane 9 and X-Plane 10
32 and 64 bit.

### Linuxtrack supports Wine games now

By installing Wine plug-in, you enable Linuxtrack usage in TrackIR enabled windows games.
For now, 32bit games are supported only, but 64bit will be worked on soon.

### Linuxtrack preferences directory changed

Linuxtrack now stores its preferences in your home directory, under .config/linuxtrack directory.
That includes TrackIR firmware too...

### Mickey, the virtual mouse

Linuxtrack contains a virtual mouse application called mickey. It allows you to use head-tracking
instead of, or along with conventional mouse. For the moment, mickey works on Linux only, but Mac OS X
version is planned soon.
