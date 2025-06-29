# LinuxTrack - Qt5 Migration Complete

**LinuxTrack** is a head tracking solution for Linux that supports TrackIR hardware and provides 6DOF head tracking for flight simulators and other applications.

## 🎯 Current Status: Qt5 Migration Complete

This repository contains the **successfully migrated Qt5 version** of LinuxTrack, with comprehensive testing on Kubuntu 25.10 and professional-grade installation system.

### ✅ Major Features
- **Qt5 GUI** - Modern Qt5-based user interface
- **TrackIR Support** - Full TrackIR hardware compatibility
- **Professional Installation** - Complete installation system with desktop integration
- **System Integration** - udev rules, desktop entries, and proper system integration
- **Multi-Platform** - Support for various Linux distributions

## 🚀 Quick Start

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt install build-essential autoconf automake libtool
sudo apt install qtbase5-dev qttools5-dev-tools
sudo apt install libopencv-dev libusb-1.0-0-dev libmxml-dev
sudo apt install libx11-dev libxrandr-dev

# Fedora
sudo dnf groupinstall "Development Tools"
sudo dnf install qt5-qtbase-devel qt5-qttools-devel
sudo dnf install opencv-devel libusb1-devel mxml-devel
sudo dnf install libX11-devel libXrandr-devel
```

### Build and Install
```bash
# Clone the repository
git clone <repository-url>
cd linuxtrack-clean-june14

# Configure and build
./configure --prefix=/opt/linuxtrack
make -j$(nproc)

# Install (requires root privileges)
sudo make install
sudo make install-udev-rules
sudo make install-desktop

# Add user to plugdev group
sudo usermod -a -G plugdev $USER
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
- `src/` - Core libraries and server components
- `dev-scripts/` - Build and installation scripts
- `doc/` - Documentation and help files

## 🔧 Configuration

LinuxTrack configuration is stored in:
- System default: `/opt/linuxtrack/share/linuxtrack/linuxtrack1.conf`
- User config: `~/.config/linuxtrack/linuxtrack1.conf`

## 🎮 Usage

### Main GUI
```bash
ltr_gui
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
- Professional installation system
- System integration (udev rules, desktop entries)
- Comprehensive testing on Kubuntu 25.10
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

**Version**: 0.99.20 (Qt5)  
**Last Updated**: June 29, 2025  
**Status**: Production Ready

## 🎯 Features

- **TrackIR 5 Support**: Full support for TrackIR 5 devices with proper device communication
- **Multiple Input Methods**: Support for TrackIR, Wiimote, webcam, and face tracking  
- **Qt5 GUI**: Modern Qt5-based configuration interface
- **Wine Integration**: Seamless integration with Windows games via Wine
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

# For Wayland users (force X11 mode for better compatibility)
./run_qt5_gui.sh --force-x11
```

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
- ✅ **openSUSE Leap 15.4+**
- ✅ **Arch Linux** (current)

### Display Server Compatibility
- ✅ **X11**: Fully supported (recommended)
- ⚠️ **Wayland**: Functional via XWayland compatibility layer
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
2. **🔍 Health Check**: `./linuxtrack_health_check.sh` - Automated diagnosis
3. **🚑 Quick Recovery**: `./linuxtrack_quick_recovery.sh` - Automated fixes
4. **📖 Troubleshooting Guides**:
   - **System Upgrade Issues**: [QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)
   - **TrackIR Hardware**: [TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)
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
│   ├── wine_bridge/       # Wine integration
│   └── ...
├── doc/                   # Documentation
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
LinuxTrack automatically integrates with Wine to provide head tracking for Windows games:

1. Install LinuxTrack
2. Launch your Windows game through Wine
3. LinuxTrack will automatically provide head tracking data

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
./linuxtrack_health_check.sh

# Expected output: All systems operational
# Exit codes: 0=Good, 1=Warning, 2=Critical
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
- **Monthly**: Run `./linuxtrack_health_check.sh`
- **Before Updates**: Backup working configuration
- **After Updates**: Run `./linuxtrack_quick_recovery.sh` if needed

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
- **TrackIR Protocol**: Reverse-engineered community implementation
- **Special Thanks**: All contributors and testers who helped make this possible

## 📞 Support

- **Documentation**: Check this README and troubleshooting guides
- **Issues**: Use GitHub issues for bug reports
- **Discussions**: Use GitHub discussions for questions and ideas
- **Hardware**: Ensure your TrackIR device is properly connected and recognized

---

**Note**: This is a community-maintained modernization of LinuxTrack. While we strive for stability and compatibility, this software is provided as-is. Always test thoroughly with your specific setup.

---

## 🚨 Important Notes

### Display Server Compatibility
- **X11**: Recommended for full functionality
- **Wayland**: Works but may require launching with `--force-x11` flag for some features

### Qt Version Requirements
- **Requires Qt5**: This version is specifically built for Qt5
- **Qt6 systems**: Will automatically detect and use Qt5 if available
- **Mixed Qt environments**: Use `export QT_SELECT=qt5` if needed

### TrackIR 5 Users
If you experience permission errors despite correct setup, see our detailed troubleshooting guide - this is often a library conflict issue rather than actual permission problems. 