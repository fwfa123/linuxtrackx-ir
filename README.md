# LinuxTrack X-IR - Modern Head Tracking for Linux

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://gitlab.com/fwfa123/linuxtrackx-ir)
[![License](https://img.shields.io/badge/license-MIT-blue)](https://gitlab.com/fwfa123/linuxtrackx-ir/-/blob/main/LICENSE.md)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)](https://gitlab.com/fwfa123/linuxtrackx-ir)

Modern fork of LinuxTrack with enhanced TrackIR support and modern Linux compatibility. **Current releases on `main` use Qt6** (CMake); older docs may still mention Qt5 for migration history.

### Why 'X-IR'?
This fork focuses on TrackIR hardware support and modern Linux distributions, while attempting to maintaining compatibility with the original codebase.

## 🚀 Quick Start

### **Recommended: AppImage (Easiest) not always the latest with all the corrections**
```bash
# Download and run the latest AppImage
chmod +x LinuxTrack-X-IR-*.AppImage
./LinuxTrack-X-IR-*.AppImage
```

### **Alternative: Build from Source**
Choose your installation level below, then see the detailed guides for your distribution.

### **Issues to be aware of as you proceed**
Have you installed Steam or Lutris from Flatpak? There are issues that you will have to correct with the sandbox design of Flatpak

**Build requirements:** the GUI and tools use **Qt6** (CMake `find_package(Qt6 …)`). Use your distribution’s Qt6 development packages; see the distribution guides below. On **KDE Plasma** (Wayland or X11), build from the `main` branch as usual—no separate Qt branch is needed.

```bash
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
```

## 🎯 Installation Levels

Choose the level that matches your needs. Each level includes all features from previous levels.

| Level | Use Case | Features |
|-------|----------|----------|
| **1: TrackIR Only** | Linux native games only | TrackIR hardware, LinuxTrack server |
| **2: + Wine Support** | Windows games via Wine/Proton | Level 1 + Wine bridge, Steam compatibility (**requires 32-bit Wine + NSIS**) |
| **3: + X-Plane** | Flight simulator | Level 2 + X-Plane plugin |
| **4: + Webcam** | Webcam / optical tracking (V4L) | Level 3 + webcam drivers |
| **5: + OSC** | External applications/MIDI | Level 4 + Open Sound Control |
| **6: + Face tracking** | OpenCV-based face tracking (`libwc` facetrack, `libp3eft`) | Level 5 + face tracking (OpenCV) |
| **7: + Wiimote** | Nintendo Wii Remote | Level 6 + Wiimote support |

> **⚠️ Important**: Level 2 requires 32-bit Wine support for MFC42 library installation and NSIS for Wine bridge installer generation. See your distribution's guide for specific installation commands.

## 🛠️ Build Overview

### Common Build Commands (All Levels)
```bash
# Clone the repository
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir

# Create build directory
mkdir build && cd build

# Configure (see level-specific options below)
# Note: Default prefix is /opt (recommended for Steam Proton compatibility)
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
| 1 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt` | TrackIR only (default prefix is `/opt`) |
| 2 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON` | + Wine support (**requires 32-bit Wine installed**) |
| 3 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders` | + X-Plane |
| 4 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON` | + Webcam |
| 5 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON` | + OSC |
| 6 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON` | + Face tracking |
| 7 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON -DENABLE_WIIMOTE=ON` | + Wiimote |

> **Note**: `/opt` is the default install prefix and is recommended for Steam Proton compatibility. Symlinks are automatically created in `/usr/local/bin` for PATH compatibility.

### Distribution-Specific Instructions

- **[Debian/Ubuntu/MX Linux](docs/readme/debian-ubuntu.md)**
- **[Arch Linux](docs/readme/arch-linux.md)**
- **[Fedora/RHEL](docs/readme/fedora-rhel.md)**
- **[Flatpak (Lutris and games)](docs/readme/flatpak.md)** - Using LinuxTrack when Lutris or games run from Flatpak (Flatseal, library path).

Each guide includes package installation commands and level-specific build examples. Fedora users: ensure mxml-devel (or libmxml-devel) and development-tools are installed; see [Fedora/RHEL](docs/readme/fedora-rhel.md). In-app help uses built-in HTML for typical installs; **AppImage v2** builds invoke `qhelpgenerator`—on Fedora install **`qt6-doctools`** (tool lives under `libexec/`); see the Fedora guide.

### Migrating from /usr/local to /opt

If you have an existing installation in `/usr/local` and want to migrate to `/opt` (recommended for Steam Proton compatibility), see the **[Migration Guide](docs/readme/MIGRATION_GUIDE_USR_LOCAL_TO_OPT.md)**.

## 🎮 Usage

### First Launch
On **Wayland** (default on many modern distros, e.g. Arch): if the window does not appear, use `QT_QPA_PLATFORM=xcb ltr_gui`. Otherwise:
```bash
ltr_gui
# If the GUI does not show on Wayland:
QT_QPA_PLATFORM=xcb ltr_gui
```

**Lutris or games from Flatpak:** If you run Lutris (or launch games) from Flatpak, the game runs in a sandbox and cannot see the host's Linuxtrack install. You may see "All library search attempts failed" in the game output. Grant the Lutris Flatpak access to your Linuxtrack install path (e.g. `/opt` or `/usr`) using **Flatseal**; see **[Flatpak (Lutris and games)](docs/readme/flatpak.md)**.

### Configuration Steps
1. **Select your tracking device** (TrackIR, webcam, etc.)
2. **Calibrate tracking points** for accurate head tracking
3. **Set up game profiles** for different applications
4. **Test tracking functionality** before gaming

### Alternative Tracking Methods
- **AntiMicro joystick detection** - Convert head tracking to joystick input
- **Server approach** - Use LinuxTrack server for Wine games
- **OSC network** - External application integration

### Companion Applications
- **[LinuxTrack Mapper](https://gitlab.com/open-source4661535/linuxtrack-mapper)** - Map head tracking zones to keyboard inputs for games that support keyboard look controls. Perfect for flight simulators and racing games that don't have native TrackIR support but support keyboard view controls.

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
| Library not found when game launched from Lutris Flatpak | Use Flatseal to allow filesystem access; see [Flatpak doc](docs/readme/flatpak.md) |

**[Detailed troubleshooting guide](docs/readme/troubleshooting.md)** - Distribution-specific issues and advanced diagnostics.

## 🔧 Advanced Features

### Custom Build Combinations
For advanced users, combine CMake flags to build exactly what you need:

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_OSC=ON
```

### AppImage Build
For advanced users who want to create AppImages:
```bash
./scripts/appimage/build_appimage_phase4.sh --clean
```

Build the AppImage on a machine that has the same **development** packages as a full source build (see your distro guide). The v2 `scripts/appimage/v2/prepare.sh` script matches roughly **Level 5** in the table above: it enables `ENABLE_WEBCAM=ON`, `ENABLE_OSC=ON`, `ENABLE_XPLANE=ON`, and leaves **`ENABLE_FACE_TRACKER=OFF`** (the CMake default—OpenCV facetrack is opt-in). Install **Level 4+** (webcam/V4L) packages on the build host so `libwc` and PS3 Eye (`libp3e`) can build. To bundle OpenCV-based facetrack (`libp3eft`, etc.), you must add `-DENABLE_FACE_TRACKER=ON` and install **Level 6+** OpenCV development packages on the builder; the default script does not. **End users** who only run the published AppImage do **not** need OpenCV on the system. PS3 Eye LED/blob mode (`libp3e`) does not require OpenCV at build time.

**[Full advanced documentation](docs/technical/)** - CMake options, packaging, and development guides.

## 🤝 Contributing

LinuxTrack X-IR welcomes contributions! See the [GitLab repository](https://gitlab.com/fwfa123/linuxtrackx-ir) for issues and feature requests.

### Development Setup
```bash
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_DEBUG=ON
cmake --build . -j$(nproc)
```

## 📜 License

LinuxTrack X-IR is released under the MIT License, maintaining compatibility with the original project's licensing.

## 🙏 Acknowledgments

This project builds upon the incredible work of the original LinuxTrack developers. Special thanks to uglyDwarf, Tulthix, f.josef, Michal Navratil, and all contributors.

## 🌐 Links

- **GitLab Repository**: [fwfa123/linuxtrackx-ir](https://gitlab.com/fwfa123/linuxtrackx-ir)
- **Wiki (GitLab)**: [Project wiki](https://gitlab.com/fwfa123/linuxtrackx-ir/-/wikis/home)
- **Wiki (GitHub)**: [linuxtrackx-ir wiki](https://github.com/fwfa123/linuxtrackx-ir/wiki) — clone the wiki repo with `git clone https://github.com/fwfa123/linuxtrackx-ir.wiki.git` for local edits (see `docs/wiki/README.md`)
- **Original Project**: [uglyDwarf/linuxtrack](https://github.com/uglyDwarf/linuxtrack)
- **Companion Application**: [LinuxTrack Mapper](https://gitlab.com/open-source4661535/linuxtrack-mapper) - Map head tracking to keyboard inputs

