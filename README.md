# LinuxTrack X-IR - Modern Head Tracking for Linux

[![Version](https://img.shields.io/badge/version-2.0.0-blue)](https://gitlab.com/fwfa123/linuxtrackx-ir)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://gitlab.com/fwfa123/linuxtrackx-ir)
[![License](https://img.shields.io/badge/license-MIT-blue)](https://gitlab.com/fwfa123/linuxtrackx-ir/-/blob/main/LICENSE.md)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)](https://gitlab.com/fwfa123/linuxtrackx-ir)

Modern fork of LinuxTrack with enhanced TrackIR support and modern Linux compatibility.

**Version 2.0.0** (`wow64-rearchitecture` branch): major rearchitecture—Wine WOW64 bridge, Qt6 (CMake), and a different build/runtime model than the pre-2.0 `0.99.x` line on `main`. Older docs may still mention Qt5 or `0.99.x` for migration history.

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
| **2: + Wine Support** | Windows games via Wine/Proton | Level 1 + Wine bridge, Steam compatibility (**requires Wine 11.0+ + MinGW-w64 + NSIS**) |
| **3: + X-Plane** | Flight simulator | Level 2 + X-Plane plugin |
| **4: + Webcam** | Webcam / optical tracking (V4L) | Level 3 + webcam drivers |
| **5: + OSC** | External applications/MIDI | Level 4 + Open Sound Control |
| **6: + Face tracking** | OpenCV-based face tracking (`libwc` facetrack, `libp3eft`) | Level 5 + face tracking (OpenCV) |
| **7: + Wiimote** | Nintendo Wii Remote | Level 6 + Wiimote support |

> **⚠️ Important**: Level 2 targets modern WOW64 and requires Wine 11.0+ (or current Proton/Wine Staging), MinGW-w64 cross-compilers, and NSIS.

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
| 2 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt` | + Wine support (Wine 11.0+ + MinGW/NSIS toolchain required) |
| 3 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders` | + X-Plane |
| 4 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON` | + Webcam |
| 5 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON` | + OSC |
| 6 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON` | + Face tracking |
| 7 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON -DENABLE_WIIMOTE=ON` | + Wiimote |

> **Note**: `/opt` is the default install prefix and is recommended for Steam Proton compatibility. Symlinks are automatically created in `/usr/local/bin` for PATH compatibility.

### Distribution-Specific Instructions

- **[Debian/Ubuntu/MX Linux](docs/readme/debian-ubuntu.md)**
- **[Arch Linux](docs/readme/arch-linux.md)** (Arch / CachyOS: read **AppImage vs source** and **Wine WOW64** first)
- **[X-Plane SDK install](docs/readme/x-plane-sdk.md)** (ZIP download, e.g. `XPSDK430.zip` — not `.tar.gz`)
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
- **OSC network** - External application integration via liblo. Original upstream feature request and format discussion: [uglyDwarf/linuxtrack#80 — OSC output implementation](https://github.com/uglyDwarf/linuxtrack/issues/80). This project ships the optional **`osc_server`** binary when **liblo** is present (CMake `osc_server` target; build with `-DENABLE_OSC=ON` per README Level 5). User-facing guide: **[docs/osc.md](docs/osc.md)** (wiki-ready); implementation: `src/osc_server.c` (`/linuxtrack/pose`, `/linuxtrack/point`).

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
| Wine bridge fails | Check Wine 11.0+ runtime, MinGW toolchain, and MFC42 setup |
| Library not found when game launched from Lutris Flatpak | Use Flatseal to allow filesystem access; see [Flatpak doc](docs/readme/flatpak.md) |
| AppImage: Steam/Lutris not found (Flatpak installed) | Use a V2 build with the Flatpak detection fix; confirm `~/.var/app/com.valvesoftware.Steam/data/Steam` exists after launching Flatpak Steam once. See [Fedora/RHEL](docs/readme/fedora-rhel.md) Lutris section. |
| AppImage build: `patchelf not available` | Install `patchelf` on the **build** machine, then rebuild. See [AppImage Build](#appimage-build) and your distro guide. |

**[Detailed troubleshooting guide](docs/readme/troubleshooting.md)** - Distribution-specific issues and advanced diagnostics.

## 🔧 Advanced Features

### Custom Build Combinations
For advanced users, combine CMake flags to build exactly what you need:

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_OSC=ON
```

### AppImage Build
The maintained pipeline lives under **`scripts/appimage/v2/`**. Typical entry points:

```bash
# Full sequence (clean AppDir, build, bundle, validate, package)
./scripts/appimage/v2/ci_build.sh

# Or use the Docker image (recommended; pins Ubuntu 22.04 + deps)
./scripts/appimage/docker_build.sh
```

The v2 **`prepare.sh`** configure line targets README **Level 7** in the table above: `ENABLE_WEBCAM=ON`, `ENABLE_OSC=ON`, **`ENABLE_FACE_TRACKER=ON`**, **`DISABLE_WIIMOTE=OFF`** (Wiimote builds when **libcwiid** is available), and **`ENABLE_XPLANE=ON`** when the X-Plane SDK is present (otherwise X-Plane is turned off for that run). Install the same **development** packages as a Level 4–7 source build on the host (webcam/V4L, **liblo**, **OpenCV**, **libcwiid**); see your [distribution guide](#distribution-specific-instructions). The **`scripts/appimage/Dockerfile`** includes OpenCV and libcwiid dev packages so official container builds can produce **`libp3eft`** and **`wii_server`**. Set **`XPLANE_SDK_PATH`** if headers are not under `/opt/xplane-sdk/CHeaders`.

**AppImage packaging tools (maintainers):** install **`patchelf`** on the build host so v2 can set library rpaths for TrackIR plugins inside the AppImage (`package.sh` / `bundle.sh`). Without it the build still completes but logs `patchelf not available`. Also need **`appimagetool`** (or the script’s bundled copy) and the usual Level 2+ MinGW/NSIS stack if you build the Wine bridge into the image. Package names: `patchelf` on Debian/Ubuntu, Fedora, and Arch — see your distro guide.

Validation (**`validate.sh`**) defaults to **`EXPECT_LEVEL7=1`** (see **`config.sh`**): the AppDir must include OpenCV face-track support (**`libp3eft`**) and **`usr/bin/wii_server`**. Export **`EXPECT_LEVEL7=0`** if you are checking a slim or partial tree. **`ci_build.sh`** runs the full v2 sequence with **`CLEAN=1`** (prepare, bundle, optimize, validate, package).

**End users** who only run the published AppImage still do **not** need OpenCV or libcwiid on the system; those are build-time dependencies for the packager. PS3 Eye LED/blob mode (`libp3e`) does not require OpenCV at build time; face-track mode does.

**Legacy:** `./scripts/appimage/build_appimage_phase4.sh --clean` may still exist for older workflows; prefer v2 for current behavior.

**Runtime (not bundled in the AppImage):** GPU/OpenGL drivers are always from the host. The Wine bridge installer still expects **Wine** on the system when you run its script. Udev rules for hardware may need to be installed manually from the files shipped under the AppImage’s `udev/` folder.

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
- **Wiki (GitHub)**: [linuxtrackx-ir wiki](https://github.com/fwfa123/linuxtrackx-ir/wiki) — edit in the separate `linuxtrackx-ir.wiki` checkout; `scripts/convert_help_to_wiki.py` uses `LINUXTRACK_WIKI_ROOT` or `../linuxtrackx-ir.wiki`
- **Original Project**: [uglyDwarf/linuxtrack](https://github.com/uglyDwarf/linuxtrack)
- **Companion Application**: [LinuxTrack Mapper](https://gitlab.com/open-source4661535/linuxtrack-mapper) - Map head tracking to keyboard inputs

