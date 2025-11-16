# LinuxTrack X-IR - Modern Head Tracking for Linux

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://gitlab.com/fwfa123/linuxtrackx-ir)
[![License](https://img.shields.io/badge/license-MIT-blue)](https://gitlab.com/fwfa123/linuxtrackx-ir/-/blob/main/LICENSE.md)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)](https://gitlab.com/fwfa123/linuxtrackx-ir)

Modern fork of LinuxTrack with enhanced TrackIR support and modern Linux compatibility.

### Why 'X-IR'?
This fork focuses on TrackIR hardware support and modern Linux distributions, while attempting to maintaining compatibility with the original codebase.

## 🚀 Quick Start

### **Recommended: AppImage (Easiest)**
```bash
# Download and run the latest AppImage
chmod +x LinuxTrack-X-IR-*.AppImage
./LinuxTrack-X-IR-*.AppImage
```

### **Alternative: Build from Source**
Choose your installation level below, then see the detailed guides for your distribution.

## 🎯 Installation Levels

Choose the level that matches your needs. Each level includes all features from previous levels.

| Level | Use Case | Features |
|-------|----------|----------|
| **1: TrackIR Only** | Linux native games only | TrackIR hardware, LinuxTrack server |
| **2: + Wine Support** | Windows games via Wine/Proton | Level 1 + Wine bridge, Steam compatibility (**requires 32-bit Wine**) |

> **⚠️ Important**: Level 2 requires 32-bit Wine support for MFC42 library installation. See your distribution's guide for specific installation commands.
| **3: + Webcam** | Alternative face tracking | Level 2 + webcam/OpenCV support |
| **4: + OSC** | External applications/MIDI | Level 3 + Open Sound Control |
| **5: + Wiimote** | Nintendo Wii Remote | Level 4 + Wiimote support |
| **6: + X-Plane** | Flight simulator | Level 5 + X-Plane plugin |

## 🛠️ Build Overview

### Common Build Commands (All Levels)
```bash
# Clone the repository
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir

# Create build directory
mkdir build && cd build

# Configure (see level-specific options below)
cmake .. [OPTIONS]

# Build
cmake --build . -j$(nproc)

# Install
sudo cmake --install .

# Verify
ldconfig -p | grep linuxtrack
ltr_gui

#Uninstall from the Build Directory
sudo cmake --build . --target uninstall
```

### Level-Specific CMake Commands

| Level | CMake Command | Description |
|-------|---------------|-------------|
| 1 | `cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local` | TrackIR only |
| 2 | `cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON` | + Wine support (**requires 32-bit Wine installed**) |
| 3 | `cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON` | + Webcam |
| 4 | `cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON` | + OSC |
| 5 | `cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON` | + Wiimote |
| 6 | `cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders` | + X-Plane |

### Distribution-Specific Instructions

- **[Debian/Ubuntu/MX Linux](docs/readme/debian-ubuntu.md)**
- **[Arch Linux](docs/readme/arch-linux.md)**
- **[Fedora/RHEL](docs/readme/fedora-rhel.md)**

Each guide includes package installation commands and level-specific build examples.

## 🎮 Usage

### First Launch
```bash
# For X11 (recommended)
ltr_gui

# For Wayland (force X11 compatibility if needed)
QT_QPA_PLATFORM=xcb ltr_gui
```

### Configuration Steps
1. **Select your tracking device** (TrackIR, webcam, etc.)
2. **Calibrate tracking points** for accurate head tracking
3. **Set up game profiles** for different applications
4. **Test tracking functionality** before gaming

### Alternative Tracking Methods
- **AntiMicro joystick detection** - Convert head tracking to joystick input
- **Server approach** - Use LinuxTrack server for Wine games
- **OSC network** - External application integration

## 🎯 Supported Hardware

### TrackIR Devices
- **TrackIR 4 & 5** - Primary supported hardware
- Full TrackIR API compatibility

### Alternative Tracking
- **Webcam + IR LEDs** - Build your own IR tracking setup
- **Wiimote** - Nintendo Wii Remote tracking
- **FaceTrackNoIR compatible** - Face detection tracking
- **PlayStation Eye** - USB camera with IR filter removal
- **Generic USB cameras** - For optical tracking

## 🎮 Supported Games

### Native Linux Games
- DCS World
- War Thunder
- Elite Dangerous
- Euro Truck Simulator 2
- American Truck Simulator

### Windows Games (via Wine/Steam Proton)
- Microsoft Flight Simulator
- IL-2 Sturmovik series
- Falcon 4
- Commanche vs Havoc
- **X-Plane** (requires X-Plane SDK)

## 🐛 Troubleshooting

### Quick Recovery (Recommended First Step)
```bash
# Automated recovery script (fixes common issues automatically)
./scripts/install/linuxtrack_quick_recovery.sh

# Or if run from project root:
./linuxtrack_quick_recovery.sh
```

### Quick Diagnosis
```bash
# Check installation
ldconfig -p | grep linuxtrack
ltr_gui

# Check display environment
echo $XDG_SESSION_TYPE

# Check device permissions
groups $USER
```

### Common Issues
| Problem | Solution |
|---------|----------|
| GUI not displaying | Force X11: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied | Add to groups: `sudo usermod -a -G plugdev,input $USER` |
| Library not found | Run: `sudo ldconfig` |
| TrackIR not detected | Check USB: `lsusb \| grep Track` |
| Wine bridge fails | Check Wine 32-bit installation and MFC42 setup |

**[Detailed troubleshooting guide](docs/readme/troubleshooting.md)** - Distribution-specific issues and advanced diagnostics.

## 🔧 Advanced Features

### Custom Build Combinations
For advanced users, combine CMake flags to build exactly what you need:

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_OSC=ON
```

### AppImage Build
For advanced users who want to create AppImages:
```bash
./scripts/appimage/build_appimage_phase4.sh --clean
```

**[Full advanced documentation](docs/technical/)** - CMake options, packaging, and development guides.

## 🤝 Contributing

LinuxTrack X-IR welcomes contributions! See the [GitLab repository](https://gitlab.com/fwfa123/linuxtrackx-ir) for issues and feature requests.

### Development Setup
```bash
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_DEBUG=ON
cmake --build . -j$(nproc)
```

## 📜 License

LinuxTrack X-IR is released under the MIT License, maintaining compatibility with the original project's licensing.

## 🙏 Acknowledgments

This project builds upon the incredible work of the original LinuxTrack developers. Special thanks to uglyDwarf, Tulthix, f.josef, Michal Navratil, and all contributors.

## 🌐 Links

- **GitLab Repository**: [fwfa123/linuxtrackx-ir](https://gitlab.com/fwfa123/linuxtrackx-ir)
- **Wiki**: [GitLab Wiki](https://gitlab.com/fwfa123/linuxtrackx-ir/-/wikis/home)
- **Original Project**: [uglyDwarf/linuxtrack](https://github.com/uglyDwarf/linuxtrack)

