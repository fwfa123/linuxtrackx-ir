# LinuxTrack - Head Tracking for Linux

**LinuxTrack** is a head tracking solution for Linux that provides 6DOF (6 degrees of freedom) head tracking for flight simulators, games, and other applications. This repository contains the **modernized Qt5 version** with improved compatibility and stability.

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
- ⚠️ **Wayland**: Functional but may require `--force-x11` for some features

## 🔍 Troubleshooting

### Common Issues

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

1. **Check the troubleshooting guide**: [TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)
2. **Run diagnostics**: `./build.sh --check`
3. **Enable debug logging**: Edit `~/.config/linuxtrack/linuxtrack1.conf` and set debug flags
4. **Check device detection**: `lsusb | grep -i trackir`

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