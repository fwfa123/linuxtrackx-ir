# Welcome to LinuxTrack X-IR v0.99.25!

## About LinuxTrack X-IR

First of all, thank you for using LinuxTrack X-IR v0.99.25, a head-tracking software specifically tailored for Linux and supporting TrackIR devices. This is the modernized and actively maintained fork of the original LinuxTrack project, featuring Qt5 modernization, cross-distribution compatibility, and enhanced gaming platform integration.

LinuxTrack X-IR allows you to use your TrackIR device as a head-tracking device, facilitating view control in simulators (flight, racing) or any other type of software. The application provides seamless integration with modern Linux distributions and gaming platforms.

## What's New in v0.99.25

- **Alternative MFC42.dll Sources**: Added VS6SP6.EXE and VC6RedistSetup_deu.exe sources for Arch Linux users without wine32
- **Enhanced Wine Integration**: Improved Lutris Wine path resolution and version detection
- **Automatic Installation**: Streamlined installation process with automatic post-installation steps
- **Cross-Distribution Compatibility**: Full support for Fedora, Nobara, and other RPM-based distributions
- **Flatpak Integration**: Seamless support for Flatpak-managed Steam and Lutris installations
- **Qt5 Modernization**: Complete migration from Qt4 to Qt5 for modern Linux compatibility

## Supported Applications

Among applications utilizing LinuxTrack X-IR are:

- X-Plane flight simulator (with plugin support)
- Flight Gear flight simulator
- Any TrackIR-enabled game running in Wine (Condor soaring simulator, Il-2, Falcon 4, etc.)
- Steam Proton games with Wine Bridge support
- Lutris-managed games with automatic Wine Bridge installation
- Custom Wine prefixes with manual Wine Bridge setup

## Current Development Status

**Active Development**: LinuxTrack X-IR is actively maintained with regular updates and improvements. The project has moved to GitLab as the primary repository while maintaining a GitHub mirror for community access.

**Modern Linux Support**: While legacy code for web-camera, Wiimote, and Mac OS support is still present, the primary focus is on ensuring full compatibility with modern Linux distributions (Ubuntu, Fedora, Arch Linux, Debian, MX Linux, etc.) and TrackIR devices.

**Community-Driven**: Special thanks to the LinuxTrack community for their contributions and feedback. The project benefits from community testing across multiple distributions and use cases.

## Getting Help

If you want to know more, see the following resources:

- [GitLab Project Page](https://gitlab.com/fwfa123/linuxtrackx-ir) (Primary Repository)
- [GitHub Mirror](https://github.com/fwfa123/linuxtrackx-ir) (Community Access)
- [X-Plane.org Forums](https://forums.x-plane.org/index.php?act=idx) (Linux forum)
- [FreeTrack project page](http://www.free-track.net/english/) (Model creation hints and tutorials)

## Reporting Issues

Should you encounter a bug, please take a few minutes and file a bug report at the GitLab Issues page - it will allow the development team to fix the bug that is bothering you, and it also helps to improve LinuxTrack X-IR itself. Any comments, ideas for improvement, and constructive criticism are very welcome!

**GitLab Issues**: [https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues)

## Version Information

- **Current Version**: 0.99.25 (September 17, 2025)
- **Codename**: "Arch Linux Wine32 Alternative Sources"
- **Framework**: Qt5
- **Build System**: Modern autotools + CMake support
- **Security Features**: PIE, stack protector, fortify source enabled

---

For more detailed information about specific features, see the other documentation pages:

- [What's New](News.md) - Recent changes and feature updates
- [Device Setup](Device-Setup.md) - Hardware configuration and setup
- [Gaming Platform Integration](Gaming-Integration.md) - Steam Proton and Lutris support
- [System Information](System.md) - System information and troubleshooting
- [Interface Documentation](Interface.md) - Developer API documentation
