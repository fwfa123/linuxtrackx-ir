# LinuxTrack - Qt5 Migration & Wine Bridge Complete

**LinuxTrack** is a head tracking solution for Linux that supports TrackIR hardware and provides 6DOF head tracking for flight simulators and other applications.

## 🎯 Current Status: Qt5 Migration & Wine Bridge Complete

This repository contains the **successfully migrated Qt5 version** of LinuxTrack with **complete wine bridge integration**, featuring comprehensive testing on modern Linux distributions and professional-grade installation system.

### ✅ Major Features
- **Qt5 GUI** - Modern Qt5-based user interface
- **TrackIR Support** - Full TrackIR hardware compatibility
- **Wine Bridge Integration** - Complete wine bridge for Windows games compatibility
- **Professional Installation** - Complete installation system with desktop integration
- **System Integration** - udev rules, desktop entries, and proper system integration
- **Multi-Platform** - Support for various Linux distributions
- **Modern Distribution Support** - Enhanced compatibility with Ubuntu, Fedora, Arch, OpenSUSE

## 🚀 Quick Start

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt install build-essential autoconf automake libtool
sudo apt install qtbase5-dev qttools5-dev-tools
sudo apt install libopencv-dev libusb-1.0-0-dev libmxml-dev
sudo apt install libx11-dev libxrandr-dev

# For Wine Bridge Support (Multiple build methods available)
# Method 1: MinGW Cross-Compilation (Recommended - No wine-devel required)
sudo apt install mingw-w64 gcc-mingw-w64 g++-mingw-w64 nsis

# Method 2: Wine-Development (Traditional - Complex setup)
# sudo apt install wine-development wine32-development wine64-development
# sudo apt install gcc-multilib libc6-dev-i386 nsis

# Fedora
sudo dnf groupinstall "Development Tools"
sudo dnf install qt5-qtbase-devel qt5-qttools-devel
sudo dnf install opencv-devel libusb1-devel mxml-devel
sudo dnf install libX11-devel libXrandr-devel

# For Wine Bridge Support (Multiple build methods available)
# Method 1: MinGW Cross-Compilation (Recommended - No wine-devel required)
sudo dnf install mingw64-gcc mingw64-gcc-c++ nsis

# Method 2: Wine-Development (Traditional - Complex setup)
# sudo dnf install wine-devel wine-core wine-desktop
# sudo dnf install gcc-multilib glibc-devel.i686 nsis
```

**Note**: Wine bridge support is automatically detected and enabled using multiple build methods. The system will use MinGW cross-compilation (recommended) if available, falling back to wine-devel if needed. No additional configure flags are needed.

### Build and Install
```bash
# Clone the repository
git clone <repository-url>
cd linuxtrack-clean-june14

# Configure and build (wine bridge support is automatic with multiple build methods)
./configure --prefix=/opt/linuxtrack
make -j$(nproc)

# Or use the smart build script for automatic wine bridge setup
./dev-scripts/build_wine_bridge.sh

# Install (requires root privileges)
sudo ./install.sh

# Or manually:
# sudo make install
# sudo usermod -a -G plugdev $USER
# Log out and log back in for group changes to take effect
```

### Verify Installation
```bash
# Run the verification script
./verify_installation.sh
```

## 📁 Project Structure

- `src/qt_gui/` - Qt5 GUI application (`ltr_gui`)
- `src/mickey/` - Mickey standalone application
- `src/wine_bridge/` - Wine bridge components for Windows games
- `src/` - Core libraries and server components
- `dev-scripts/` - Build and installation scripts
- `doc/` - Documentation and help files
- `docs/` - Additional documentation (including wine dependencies)

## 🔧 Configuration

LinuxTrack configuration is stored in:
- System default: `/opt/linuxtrack/share/linuxtrack/linuxtrack1.conf`
- User config: `~/.config/linuxtrack/linuxtrack1.conf`

## 🎮 Usage

### Main GUI
```bash
# Standard launch
ltr_gui

# For Wayland users (force X11 for proper tracking display)
export QT_QPA_PLATFORM=xcb
ltr_gui

# Or use the launcher script
./run_qt5_gui.sh --force-x11
```

### Mickey (Standalone)
```bash
mickey
```

### Server
```bash
ltr_server1
```

## 📋 Migration Status

### ✅ Completed
- Qt4 → Qt5 migration (100%)
- Wine bridge integration (100%)
- Professional installation system
- System integration (udev rules, desktop entries)
- Modern distribution compatibility
- Comprehensive testing on modern Linux distributions
- Build system modernization

### 🔄 In Progress
- Final testing and validation
- Documentation updates
- Release preparation

## 🤝 Contributing

This project is actively maintained. Please report issues and submit pull requests for improvements.

## 📄 License

LinuxTrack is released under the GPL license. See the LICENSE file for details.

## 🙏 Acknowledgments

- Original LinuxTrack developers
- Qt5 migration contributors
- Testing and validation team

---

**Version**: 0.99.20 (Qt5 + Wine Bridge)  
**Last Updated**: July 2025  
**Status**: Production Ready with Complete Wine Integration

## 🎯 Features

- **TrackIR 5 Support**: Full support for TrackIR 5 devices with proper device communication
- **Multiple Input Methods**: Support for TrackIR, Wiimote, webcam, and face tracking  
- **Qt5 GUI**: Modern Qt5-based configuration interface
- **Complete Wine Bridge**: Full wine bridge integration for Windows games compatibility
- **Modern Distribution Support**: Enhanced compatibility with Ubuntu, Fedora, Arch, OpenSUSE
- **Profile Management**: Multiple tracking profiles for different applications
- **Real-time Tracking**: Low-latency head tracking with configurable sensitivity

## 🔧 Hardware Support

### TrackIR Devices
- **TrackIR 5**: Full support (USB ID: 131d:0159) ✅ **Primary focus**
- **TrackIR 4**: Supported
- **TrackIR 3**: Supported

### Other Input Devices
- **Webcam**: Face tracking and point tracking
- **Nintendo Wiimote**: Motion-based head tracking
- **Custom LED setups**: Single or multi-point LED tracking

## 🚀 Quick Start

### Easy Installation (Recommended)

```bash
# Clone the repository
git clone <repository-url>
cd linuxtrack

# Build and install with dependencies (requires sudo for system setup)
./build.sh --deps --clean --install

# Launch the application
./run_qt5_gui.sh

# For Wayland users (force X11 mode for proper tracking display)
./run_qt5_gui.sh --force-x11
```

### MX Linux Users (Specialized Build)

If you're using MX Linux or similar Debian-based distributions:

```bash
# Use the specialized MX Linux build script
./dev-scripts/build_mx_linux.sh

# Or use the smart wine bridge build script
./dev-scripts/build_wine_bridge.sh
```

See [docs/MX_LINUX_BUILD_GUIDE.md](docs/MX_LINUX_BUILD_GUIDE.md) for detailed MX Linux instructions.

### Manual Installation

If you prefer to install dependencies manually:

```bash
# Install dependencies (Fedora/RHEL)
sudo dnf install gcc-c++ make autoconf automake libtool qt5-devel libusb1-devel v4l-utils-devel

# Install dependencies (Ubuntu/Debian)  
sudo apt install build-essential autoconf automake libtool qt5-qmake qtbase5-dev libusb-1.0-0-dev v4l-utils libv4l-dev

# Build the project
autoreconf -fiv
./configure --prefix=/usr/local
make -j$(nproc)

# Install (requires sudo)
sudo make install

# Setup permissions and udev rules
sudo ./setup.sh
```

## 📋 System Requirements

### Minimum Requirements
- **OS**: Linux (any modern distribution)
- **Qt**: Qt5 (5.12 or newer)
- **GCC**: 7.0 or newer
- **RAM**: 512MB available
- **USB**: USB 2.0 port for TrackIR devices

### Tested Distributions
- ✅ **Fedora 40+** (Primary development platform)
- ✅ **Ubuntu 22.04+** 
- ✅ **Debian 11+**
- ✅ **MX Linux 23.6+** (Specialized build script available)
- ✅ **openSUSE Leap 15.4+**
- ✅ **Arch Linux** (current)
- ✅ **Wine Bridge**: Tested on all major distributions

### Display Server Compatibility
- ✅ **X11**: Fully supported (recommended)
- ⚠️ **Wayland**: Functional via XWayland compatibility layer
  - **Important**: Tracking display in GUI requires X11 for proper functionality
  - Use `./run_qt5_gui.sh --force-x11` for optimal experience
  - Launcher automatically detects Wayland and provides guidance

## 🔍 Troubleshooting

### Common Issues

#### "System Upgrade Broke My Working LinuxTrack"
**Critical Issue**: System upgrades from Qt5 to Qt6 can break working LinuxTrack installations.
**[📖 Complete Qt5/Qt6 System Upgrade Resolution Guide](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)**

**Quick Fix**:
```bash
# Force Qt5 usage after system upgrade
qmake-qt5 --version  # Verify Qt5 is still available
cd src/qt_gui
rm -f Makefile ltr_gui
qmake-qt5 ltr_gui.pro  # Force Qt5 instead of Qt6
make -j$(nproc)
```

#### "TrackIR found but permission errors"
This is a complex issue that can have multiple causes. See our comprehensive guide:
**[📖 TrackIR Permission Troubleshooting Guide](TRACKIR_PERMISSION_TROUBLESHOOTING.md)**

#### Qt Version Conflicts
```bash
# Ensure Qt5 is being used
qmake-qt5 --version

# If using system with Qt6 as default
export QT_SELECT=qt5
```

#### Library Loading Issues
```bash
# Check for missing dependencies
ldd src/qt_gui/ltr_gui | grep "not found"

# Use our library path override
export LD_LIBRARY_PATH="$(pwd)/src/.libs:$LD_LIBRARY_PATH"
```

### Getting Help

1. **📚 Complete Documentation Index**: [DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md) - Navigate all guides
2. **🔍 Health Check**: `./verify_installation.sh` - Automated diagnosis including wine bridge
3. **🚑 Quick Recovery**: `./linuxtrack_quick_recovery.sh` - Automated fixes
4. **📖 Troubleshooting Guides**:
   - **System Upgrade Issues**: [QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)
   - **TrackIR Hardware**: [TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)
   - **Wine Bridge Setup**: [docs/WINE_DEPENDENCIES.md](docs/WINE_DEPENDENCIES.md) - Complete wine setup guide
   - **Quick Fixes**: [QUICK_TROUBLESHOOTING_QT5.md](QUICK_TROUBLESHOOTING_QT5.md)
5. **🔧 System Maintenance**: [SYSTEM_MAINTENANCE_GUIDE.md](SYSTEM_MAINTENANCE_GUIDE.md)

## 🏗️ Development

### Building from Source

```bash
# Development build with debugging
./build.sh --debug

# Clean rebuild
./build.sh --clean

# Build with specific Qt version
export QT_SELECT=qt5
./build.sh
```

### Project Structure

```
linuxtrack/
├── src/                    # Core source code
│   ├── qt_gui/            # Qt5 GUI application
│   ├── mickey/            # TrackIR communication
│   ├── wine_bridge/       # Wine bridge components
│   │   ├── client/        # TrackIR/FreeTrack client libraries
│   │   ├── controller/    # Hotkey controller
│   │   ├── tester/        # TrackIR testing tools
│   │   └── views/         # TrackIR views component
│   └── ...
├── doc/                   # Documentation
├── docs/                  # Additional documentation
│   └── WINE_DEPENDENCIES.md # Wine setup guides
├── build.sh              # Automated build script
├── setup.sh              # System setup and installation
└── run_qt5_gui.sh        # Application launcher
```

## 🔧 Configuration

### Configuration Files
- **Main config**: `~/.config/linuxtrack/linuxtrack1.conf`
- **Log files**: `~/.config/linuxtrack/linuxtrack.log`
- **Profiles**: `~/.config/linuxtrack/profiles/`

### Key Settings
- **Input device**: TrackIR, Wiimote, or webcam
- **Tracking model**: TrackClip, TrackClip Pro, or face tracking
- **Sensitivity**: Adjustable for all 6 axes
- **Filtering**: Noise reduction and smoothing

## 🎮 Game Integration

### Wine/Windows Games
LinuxTrack includes a complete wine bridge for seamless integration with Windows games:

1. **Install LinuxTrack (wine bridge support is automatic if wine tools are installed)**:
   ```bash
   ./configure --prefix=/opt/linuxtrack
   make -j$(nproc)
   sudo make install
   ```

2. **Install wine bridge to your wine prefix**:
   - Use the GUI: Click "Install Linuxtrack-Wine support..." in the main interface
   - Or use the command line: `linuxtrack-wine-launcher.sh`

3. **Launch your Windows game through Wine**
4. **LinuxTrack will automatically provide head tracking data**

**Supported Protocols**:
- **TrackIR**: Full TrackIR protocol support for games like DCS, IL-2, X-Plane
- **FreeTrack**: FreeTrack protocol support for additional game compatibility

### Native Linux Games
- **FlightGear**: Built-in LinuxTrack support
- **X-Plane**: Via plugin (included)
- **Custom applications**: Use LinuxTrack API

## 🔐 Permissions Setup

### Automatic Setup (Recommended)
```bash
sudo ./setup.sh
```

### Manual Setup
```bash
# Install udev rules
sudo cp src/99-TIR.rules /lib/udev/rules.d/
sudo udevadm control --reload-rules

# Add user to required groups
sudo usermod -a -G plugdev,linuxtrack $USER

# Reboot or re-login for group changes to take effect
```

## 🔧 Automated Maintenance

### Health Monitoring
```bash
# Comprehensive system health check
./verify_installation.sh

# Expected output: All systems operational including wine bridge
# Exit codes: 0=Good, 1=Warning, 2=Critical
```

### Wine Bridge Verification
```bash
# Check wine bridge functionality
./verify_installation.sh

# Verifies: Wine development tools, wine runtime, wine libraries
# Tests: Wine bridge installation, desktop integration, launcher script
```

### Quick Recovery
```bash
# Automatic problem resolution
./linuxtrack_quick_recovery.sh

# Handles: Qt5/Qt6 conflicts, missing executables, configuration issues
# Exit codes: 0=Success, 1=Partial recovery
```

### System Upgrade Protection
```bash
# Before system upgrade
cp src/qt_gui/ltr_gui src/qt_gui/ltr_gui.backup
cp ~/.config/linuxtrack/linuxtrack1.conf ~/.config/linuxtrack/linuxtrack1.conf.backup

# After system upgrade (if broken)
./linuxtrack_quick_recovery.sh
```

## 📈 Performance Tips

### Optimal Settings
- **Use X11** for best compatibility
- **Close unnecessary applications** during tracking
- **Adjust filtering** based on your use case (lower for responsiveness, higher for stability)
- **Position TrackIR properly** (see manual for optimal placement)

### Hardware Recommendations
- **TrackIR 5**: Best overall experience
- **USB 2.0 ports**: Directly connected to motherboard (avoid hubs if possible)
- **Good lighting**: For face/point tracking modes

### System Maintenance
- **Monthly**: Run `./verify_installation.sh`
- **Before Updates**: Backup working configuration
- **After Updates**: Run `./linuxtrack_quick_recovery.sh` if needed

### Wayland-Specific Maintenance
- **After System Updates**: Check if Qt5/Qt6 conflicts affect X11 forcing
- **Display Server Changes**: Verify `QT_QPA_PLATFORM=xcb` still works
- **GUI Issues**: If tracking display breaks, try `export QT_QPA_PLATFORM=xcb` before launching

## 🤝 Contributing

We welcome contributions! Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly (especially with TrackIR devices)
5. Submit a pull request

### Development Guidelines
- Follow existing code style
- Test on multiple distributions when possible
- Update documentation for new features
- Ensure Qt5 compatibility

## 📜 License

This project is licensed under the GPL v3 license. See `COPYING` for details.

## 🙏 Credits

- **Original LinuxTrack**: Developed by the LinuxTrack team
- **Qt5 Modernization**: Community-driven modernization effort
- **Wine Bridge Integration**: Complete wine bridge modernization and distribution compatibility
- **TrackIR Protocol**: Reverse-engineered community implementation
- **Special Thanks**: All contributors and testers who helped make this possible

## 📞 Support

- **Documentation**: Check this README and troubleshooting guides
- **Issues**: Use GitHub issues for bug reports
- **Discussions**: Use GitHub discussions for questions and ideas
- **Hardware**: Ensure your TrackIR device is properly connected and recognized

---

**Note**: This is a community-maintained modernization of LinuxTrack with complete wine bridge integration. While we strive for stability and compatibility, this software is provided as-is. Always test thoroughly with your specific setup.

## 🍷 Wine Bridge Features

### Complete Wine Integration
- **TrackIR Protocol**: Full TrackIR protocol support for Windows games
- **FreeTrack Protocol**: FreeTrack protocol support for additional compatibility
- **Modern Distribution Support**: Enhanced compatibility with Ubuntu, Fedora, Arch, OpenSUSE
- **Automated Installation**: GUI and command-line installation options
- **Desktop Integration**: Wine bridge appears in application menu
- **Comprehensive Documentation**: Complete setup and troubleshooting guides

### Multiple Build Methods (No Wine-Dev Required)
- **🏗️ MinGW Cross-Compilation**: Recommended method using standard cross-compilation
- **📦 Pre-Built Binaries**: Zero-compilation option for instant availability
- **🍷 Wine-Development**: Traditional method for maximum compatibility
- **🐳 Containerized Build**: Isolated environment for reproducible builds
- **🤖 Smart Build Script**: Automatic detection and setup of best available method

### Easy Installation
```bash
# Automatic wine bridge setup (recommended)
./dev-scripts/build_wine_bridge.sh

# Manual MinGW installation
sudo apt install mingw-w64 gcc-mingw-w64 g++-mingw-w64 nsis  # Ubuntu/Debian
sudo dnf install mingw64-gcc mingw64-gcc-c++ nsis            # Fedora
sudo pacman -S mingw-w64-gcc nsis                            # Arch
sudo zypper install cross-mingw64-gcc cross-mingw64-gcc-c++ nsis  # OpenSUSE
```

### Wine Bridge Components
- **NPClient.dll.so**: TrackIR client library (32-bit and 64-bit)
- **FreeTrackClient.dll.so**: FreeTrack client library
- **Controller.exe.so**: Hotkey controller for pause/resume/recenter
- **Tester.exe.so**: TrackIR protocol testing tools
- **linuxtrack-wine.exe**: Complete wine installer package

---

## 🚨 Important Notes

### Display Server Compatibility
- **X11**: Recommended for full functionality
- **Wayland**: Works but may require launching with `--force-x11` flag for some features

### Wayland Users - Important Notice
If you're using Wayland (default on many modern distributions), the tracking display in the GUI may not appear correctly. This is a known limitation with Qt5 and Wayland.

**Solution**: Force X11 mode for proper tracking display:
```bash
# Method 1: Use the launcher script with X11 forcing
./run_qt5_gui.sh --force-x11

# Method 2: Set environment variable and run directly
export QT_QPA_PLATFORM=xcb
ltr_gui

# Method 3: Use XWayland session
# Log out and select "GNOME on Xorg" or "KDE Plasma (X11)" at login
```

### Qt Version Requirements
- **Requires Qt5**: This version is specifically built for Qt5
- **Qt6 systems**: Will automatically detect and use Qt5 if available
- **Mixed Qt environments**: Use `export QT_SELECT=qt5` if needed

### TrackIR 5 Users
If you experience permission errors despite correct setup, see our detailed troubleshooting guide - this is often a library conflict issue rather than actual permission problems. 