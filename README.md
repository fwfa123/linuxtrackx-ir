# LinuxTrack - Head Tracking for Linux

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/uglyDwarf/linuxtrack)
[![License](https://img.shields.io/badge/license-GPL-blue)](https://github.com/uglyDwarf/linuxtrack/blob/master/COPYING)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS-lightgrey)](https://github.com/uglyDwarf/linuxtrack)

LinuxTrack is a project that brings head tracking to Linux and macOS, enabling immersive gaming and simulation experiences. Track your head movement using webcams, IR cameras, or specialized hardware and translate that into game input.

## 🚀 Quick Start

### For MX Linux / Debian Users (Recommended)
```bash
# Install dependencies
sudo apt install -y build-essential autoconf automake libtool
sudo apt install -y qtbase5-dev qttools5-dev-tools libopencv-dev
sudo apt install -y libusb-1.0-0-dev libmxml-dev mingw-w64

# Build LinuxTrack
git clone <repository-url>
cd linuxtrack-clean-june14
autoreconf -fiv
./configure --prefix=/opt
make -j$(nproc)
sudo make install
```

### For Other Linux Distributions
See our distribution-specific guides in the `docs/` directory.

## 🎉 What's New

### 🎉 MinGW Cross-Compilation Support (Phase 1 Complete)
LinuxTrack now supports building Windows compatibility components using MinGW, eliminating the need for wine-devel packages:

- ✅ **Simplified Dependencies**: No wine-devel required
- ✅ **Smaller Footprint**: Fewer packages to install
- ✅ **Better Compatibility**: Standard Windows PE binaries
- ✅ **Faster Builds**: No Wine runtime overhead
- ✅ **NSIS Installers Updated**: All installer scripts now use native .exe/.dll files

### Why MinGW is the Best Approach
**MinGW vs winegcc Comparison:**

| Aspect | MinGW (Recommended) | winegcc (Legacy) |
|--------|-------------------|------------------|
| **Binary Format** | Native Windows PE (.exe/.dll) | Linux ELF with Wine wrapper (.exe.so/.dll.so) |
| **Portability** | Works on Windows and Wine | Only works with Wine |
| **Performance** | Native Windows performance | Wine runtime overhead |
| **Dependencies** | Minimal (mingw-w64 only) | Heavy (wine-devel packages, not supported) |
| **Future-Proof** | ✅ Industry standard | ❌ Wine-specific |
| **CI/CD** | Easy automated builds | Complex Wine environment |

**Bottom Line**: MinGW produces genuine Windows binaries that work everywhere. winegcc and wine-devel are no longer supported in this project.

### 🚧 Build System Modernization Progress
- ✅ **Phase 1 Complete**: NSIS installer scripts updated for MinGW
- 🔄 **Phase 2**: Update Makefile.am files for native Windows binaries
- 🔄 **Phase 3**: Update documentation and build scripts
- 🔄 **Phase 4**: Final testing and validation

### Built Windows Components
- **NPClient.dll** / **NPClient64.dll** - TrackIR API compatibility (32/64-bit)
- **FreeTrackClient.dll** - FreeTrack API compatibility
- **Testing Tools** - Tester.exe, Tester64.exe for validation
- **Utilities** - Controller.exe for hotkeys, check_data.exe for validation

## 🎮 Gaming Compatibility

LinuxTrack works with games and simulators that support:
- **TrackIR** protocol (most flight/racing simulators)
- **FreeTrack** protocol (alternative tracking standard)
- **OpenTrack** compatibility
- **Custom applications** via LinuxTrack API

### Popular Supported Games
- Microsoft Flight Simulator (via Wine)
- DCS World
- IL-2 Sturmovik series
- War Thunder
- Elite Dangerous
- Euro Truck Simulator 2
- American Truck Simulator

## 🔧 Build Requirements

### Core Dependencies
- **Build Tools**: gcc, g++, make, autoconf, automake, libtool
- **GUI Framework**: Qt5 (qtbase5-dev, qttools5-dev-tools, libqt5x11extras5-dev)
- **Computer Vision**: OpenCV (libopencv-dev)
- **Hardware Access**: libusb-1.0-dev, libmxml-dev
- **X11 Support**: libx11-dev, libxrandr-dev

### Windows Compatibility (MinGW Only)
- **MinGW** (required): mingw-w64, gcc-mingw-w64, g++-mingw-w64

## 📖 Documentation

- **[MX Linux Build Guide](docs/MX_LINUX_BUILD_GUIDE.md)** - Complete guide for MX Linux users
- **[Wine Bridge Guide](docs/WINE_BRIDGE_BUILD_GUIDE.md)** - Windows compatibility details
- **[Qt4 to Qt5 Migration](docs/QT4_TO_QT5_MIGRATION.md)** - Technical migration notes

## 🏗️ Build Process

### Simple Build
```bash
# Clean previous builds (if any)
make distclean 2>/dev/null || true

# Regenerate build system
autoreconf -fiv

# Configure
./configure --prefix=/opt

# Build
make -j$(nproc)

# Install
sudo make install
sudo usermod -a -G plugdev $USER  # Add user to required group
```

### Build Options
```bash
# Debug build
./configure --prefix=/opt --enable-debug

# Disable Windows components
./configure --prefix=/opt --disable-wine-bridge

# Custom Qt5 location
./configure --prefix=/opt QMAKE=/usr/bin/qmake-qt5
```

## 🎯 Supported Hardware

### USB Devices
- **TrackIR 4 & 5** - Full support via reverse engineering
- **PlayStation Eye** - USB camera with IR filter removal
- **Generic USB cameras** - For optical tracking
- **TrackHat sensors** - Specialized tracking hardware

### DIY Solutions
- **Webcam + IR LEDs** - Build your own IR tracking setup
- **Wii Remote** - Nintendo Wiimote tracking
- **FaceTrackNoIR compatible** - Face detection tracking

## ⚙️ Configuration

### Post-Installation Setup
1. **Add user to groups**:
   ```bash
   sudo usermod -a -G plugdev $USER
   ```
   Log out and back in for changes to take effect.

2. **Launch GUI**:
   ```bash
   ltr_gui
   ```

3. **Configure tracking**:
   - Select your tracking device
   - Calibrate tracking points
   - Set up game profiles
   - Test tracking functionality

## 🔍 Troubleshooting

### Common Issues

| Problem | Solution |
|---------|----------|
| `aclocal-1.17: command not found` | See [MX Linux Guide](docs/MX_LINUX_BUILD_GUIDE.md#troubleshooting) |
| `winegcc: command not found` | MinGW is required. Install with: `sudo apt install mingw-w64` |
| Qt5 not found | Install Qt5: `sudo apt install qtbase5-dev qttools5-dev-tools libqt5x11extras5-dev` |
| Permission denied on device | Add user to plugdev group |
| No tracking detected | Check device connection and driver installation |

### Getting Help
1. **Check the docs**: Start with distribution-specific guides
2. **Verify installation**: Run `ltr_gui` to test basic functionality
3. **Check logs**: Look for error messages in terminal output
4. **Hardware test**: Verify USB devices are detected

## 📁 Project Structure

```
linuxtrack-clean-june14/
├── src/                    # Main source code
│   ├── qt_gui/            # Qt5 GUI application
│   ├── wine_bridge/       # Windows compatibility layer
│   ├── mickey/            # TrackIR device support
│   └── ...                # Core tracking libraries
├── docs/                  # Documentation
├── dev-scripts/           # Build automation scripts
└── README.md              # This file
```

## 🤝 Contributing

LinuxTrack is an open-source project that welcomes contributions:

1. **Bug Reports**: Use the issue tracker for bugs and feature requests
2. **Code Contributions**: Submit pull requests with improvements
3. **Documentation**: Help improve our guides and documentation
4. **Testing**: Test on different distributions and hardware setups

### Development Setup
```bash
# Clone repository
git clone <repository-url>
cd linuxtrack-clean-june14

# Create development build
./configure --prefix=/opt --enable-debug
make -j$(nproc)
```

## 📜 License

LinuxTrack is released under the GNU General Public License v2.0. See the [COPYING](COPYING) file for details.

## 🌐 Links

- **Original Project**: [LinuxTrack on GitHub](https://github.com/uglyDwarf/linuxtrack)
- **Downloads**: [Official Downloads](https://github.com/uglyDwarf/linuxtrack/wiki/Downloads)
- **Documentation**: [Project Wiki](https://github.com/uglyDwarf/linuxtrack/wiki)
- **Community**: [OpenTrack Community](https://github.com/opentrack/opentrack) (compatible project)

## 🙏 Acknowledgments

- **Original Author**: uglyDwarf and the LinuxTrack team
- **Community Contributors**: All developers who have contributed code and testing
- **Reverse Engineering**: TrackIR protocol reverse engineering efforts
- **Hardware Support**: Community-driven hardware compatibility testing

---

**Ready to start tracking?** Follow the [MX Linux Build Guide](docs/MX_LINUX_BUILD_GUIDE.md) for step-by-step instructions!