# LinuxTrack X-IR

[![Version](https://img.shields.io/badge/version-2.0.1-blue)](https://gitlab.com/fwfa123/linuxtrackx-ir)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://gitlab.com/fwfa123/linuxtrackx-ir)
[![License](https://img.shields.io/badge/license-MIT-blue)](https://gitlab.com/fwfa123/linuxtrackx-ir/-/blob/main/LICENSE.md)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)](https://gitlab.com/fwfa123/linuxtrackx-ir)

Modern fork of LinuxTrack with enhanced TrackIR support and modern Linux compatibility. Uses **Qt6 (CMake)** and a **MinGW PE Wine bridge** — no NSIS, no `winegcc`.

---

## Option A — AppImage (no build required)

Download the latest AppImage from the [Releases page](https://gitlab.com/fwfa123/linuxtrackx-ir/-/releases):

```bash
chmod +x LinuxTrack-x86_64.AppImage
./LinuxTrack-x86_64.AppImage
```

> If you run Lutris or games from **Flatpak**, see [docs/readme/flatpak.md](docs/readme/flatpak.md) — Flatpak sandboxing requires a Flatseal filesystem grant.

---

## Option B — Build from Source

### Step 1: Install dependencies and clone

Pick your distribution:

- **[Arch Linux / CachyOS](docs/readme/arch-linux.md)**
- **[Debian / Ubuntu / MX Linux](docs/readme/debian-ubuntu.md)**
- **[Fedora / RHEL](docs/readme/fedora-rhel.md)**

Each guide has a **Step 1** block with the exact package commands for Level 2 (TrackIR + Wine — the most common build).

### Step 2: Build (Level 2 — most common)

```bash
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Step 3: Launch

```bash
ltr_gui
```

On Wayland, if the window does not appear:
```bash
QT_QPA_PLATFORM=xcb ltr_gui
```

### Step 4: Verify

```bash
ldconfig -p | grep linuxtrack
ltr_server1 --help
ls /opt/lib/linuxtrack/wine_bridge/NPClient.dll   # Level 2+ Wine bridge
```

---

## Installation Levels (reference)

Each level includes all features from lower levels. Install the matching dependencies from your distro guide, then use the CMake command from the table.

| Level | Use Case | Features |
|-------|----------|----------|
| **1: TrackIR Only** | Linux native games only | TrackIR hardware, LinuxTrack server |
| **2: + Wine Support** | Windows games via Wine/Proton | Level 1 + Wine bridge, Steam compatibility (MinGW-w64 at build time) |
| **3: + X-Plane** | Flight simulator | Level 2 + X-Plane plugin |
| **4: + Webcam** | Webcam / optical tracking (V4L) | Level 3 + webcam drivers |
| **5: + OSC** | External applications/MIDI | Level 4 + Open Sound Control |
| **6: + Face tracking** | OpenCV-based face tracking | Level 5 + face tracking (OpenCV) |
| **7: + Wiimote** | Nintendo Wii Remote | Level 6 + Wiimote support |

> **Level 2 note:** requires **MinGW-w64** cross-compilers at build time. Install the bridge into Wine/Proton prefixes from the GUI or `scripts/install/install_wine_bridge.sh`. **NSIS is not used.**

### Level-Specific CMake Commands

| Level | CMake Command |
|-------|---------------|
| 1 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt` |
| 2 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt` |
| 3 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders` |
| 4 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON` |
| 5 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON` |
| 6 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON` |
| 7 | `cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON -DENABLE_WIIMOTE=ON` |

> `/opt` is the default install prefix and is recommended for Steam Proton compatibility. Symlinks are created in `/usr/local/bin` for PATH compatibility.

### Uninstall

```bash
# From the build directory
sudo cmake --build . --target uninstall
```

### Migrating from /usr/local to /opt

See the [Migration Guide](docs/readme/MIGRATION_GUIDE_USR_LOCAL_TO_OPT.md).

---

## Usage

### Configuration Steps
1. **Select your tracking device** (TrackIR, webcam, etc.)
2. **Calibrate tracking points** for accurate head tracking
3. **Set up game profiles** for different applications
4. **Test tracking functionality** before gaming

### Alternative Tracking Methods
- **AntiMicro joystick detection** - Convert head tracking to joystick input
- **Server approach** - Use LinuxTrack server for Wine games
- **OSC network** - External application integration via liblo; see [docs/osc.md](docs/osc.md). Build with `-DENABLE_OSC=ON` (Level 5).

### Companion Applications
- **[LinuxTrack Mapper](https://gitlab.com/open-source4661535/linuxtrack-mapper)** - Map head tracking zones to keyboard inputs for games that support keyboard look controls.

---

## Supported Hardware

### TrackIR Devices
- **TrackIR 4 & 5** - Primary supported hardware
- Full TrackIR API compatibility

### Alternative Tracking
- **Webcam + IR LEDs** - Build your own IR tracking setup
- **Wiimote** - Nintendo Wii Remote tracking
- **FaceTrackNoIR compatible** - Face detection tracking
- **PlayStation Eye** - USB camera with IR filter removal
- **Generic USB cameras** - For optical tracking

---

## Supported Games

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

### Game-Specific Workarounds

Per-title hints (Wine/Lutris/Proton) are in [docs/GAME_WORKAROUNDS.md](docs/GAME_WORKAROUNDS.md). Example: **Arma 2** — do not install `FreeTrackClient.dll` in the prefix; use NPClient only.

---

## Troubleshooting

### Quick Recovery
```bash
./scripts/install/linuxtrack_quick_recovery.sh
```

### Quick Diagnosis
```bash
ldconfig -p | grep linuxtrack
ltr_gui
echo $XDG_SESSION_TYPE
groups $USER
```

### Common Issues

| Problem | Solution |
|---------|----------|
| GUI not displaying | Force X11: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied | Add to groups: `sudo usermod -a -G plugdev,input,uinput,video $USER` (webcams need **`video`**) |
| Webcam not listed | Device list empty despite **Webcam support: YES** | Add **`video`** group; see [troubleshooting guide](docs/readme/troubleshooting.md#webcam-not-listed-all-distros) |
| Library not found | Run: `sudo ldconfig` |
| TrackIR not detected | Check USB: `lsusb \| grep Track` |
| Wine bridge fails | Check Wine/Proton prefix, MinGW toolchain (build), and MFC42 setup |
| Arma 2 crashes at startup with Wine bridge | Remove `FreeTrackClient.dll` from the prefix; see [Game workarounds](docs/GAME_WORKAROUNDS.md#arma-2-arma2exe--wine--lutris) |
| Library not found when game launched from Lutris Flatpak | Use Flatseal to allow filesystem access; see [Flatpak doc](docs/readme/flatpak.md) |
| AppImage: Steam/Lutris not found (Flatpak installed) | Use a V2 build with the Flatpak detection fix; confirm `~/.var/app/com.valvesoftware.Steam/data/Steam` exists after launching Flatpak Steam once. See [Fedora/RHEL](docs/readme/fedora-rhel.md) Lutris section. |
| AppImage build: `patchelf not available` | Install `patchelf` on the **build** machine, then rebuild. See [AppImage Build](#appimage-build) and your distro guide. |
| AppImage: `dlopen(): error loading libfuse.so.2` on Ubuntu 24.04+ / FUSE3-only distros | You have an **old-build AppImage**; download a rebuild with the modern runtime, or run with `--appimage-extract-and-run`, or (legacy workaround only) install `libfuse2t64` / `fuse2` — **never** install the `fuse` package on Ubuntu 22.04+. |

**[Detailed troubleshooting guide](docs/readme/troubleshooting.md)**
**[Game workarounds](docs/GAME_WORKAROUNDS.md)**

---

## Advanced

### AppImage Build

The maintained pipeline is under `scripts/appimage/v2/`:

```bash
# Full sequence (prepare, bundle, validate, package)
./scripts/appimage/v2/ci_build.sh

# Or via Docker (recommended; pins Ubuntu 22.04 + all deps)
./scripts/appimage/docker_build.sh
```

The v2 `prepare.sh` configure line targets Level 7: `ENABLE_WEBCAM=ON`, `ENABLE_OSC=ON`, `ENABLE_FACE_TRACKER=ON`, `ENABLE_XPLANE=ON` (when SDK present), and Wiimote when libcwiid is available. Install the matching Level 4–7 development packages on the build host. The `scripts/appimage/Dockerfile` includes OpenCV and libcwiid.

Validation (`validate.sh`) defaults to `EXPECT_LEVEL7=1` — export `EXPECT_LEVEL7=0` for a slim tree. `ci_build.sh` runs the full sequence with `CLEAN=1`.

**End users** running the published AppImage do not need OpenCV or libcwiid on the system.

**Runtime (not bundled):** GPU/OpenGL drivers are from the host. Wine must be installed on the system when running the Wine bridge installer. Udev rules may need to be installed from the AppImage's `udev/` folder.

**Maintainer packages:** install `patchelf` on the build host so v2 can set library rpaths for TrackIR plugins inside the AppImage.

### Custom Build Combinations

```bash
# TrackIR + X-Plane without Wine/Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Wine + OSC without Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_OSC=ON
```

**[Full technical documentation](docs/technical/)**

---

## Contributing

LinuxTrack X-IR welcomes contributions. See the [GitLab repository](https://gitlab.com/fwfa123/linuxtrackx-ir) for issues and feature requests.

```bash
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_DEBUG=ON
cmake --build . -j$(nproc)
```

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

---

## Version Notes

**Version 2.0.0** on `main`: major rearchitecture — native Wine bridge (MinGW PE payloads), Qt6 (CMake), different build/runtime model from pre-2.0. The **1.0.x** AppImage/Qt5-era tree is preserved on [`legacy/1.0.x`](https://gitlab.com/fwfa123/linuxtrackx-ir/-/tree/legacy/1.0.x).

---

## License

LinuxTrack X-IR is released under the MIT License. See [LICENSE.md](LICENSE.md).

## Acknowledgments

Built upon the work of the original LinuxTrack developers. Special thanks to uglyDwarf, Tulthix, f.josef, Michal Navratil, and all contributors.

## Links

- **GitLab Repository**: [fwfa123/linuxtrackx-ir](https://gitlab.com/fwfa123/linuxtrackx-ir)
- **Wiki (GitLab)**: [Project wiki](https://gitlab.com/fwfa123/linuxtrackx-ir/-/wikis/home)
- **Wiki (GitHub)**: [linuxtrackx-ir wiki](https://github.com/fwfa123/linuxtrackx-ir/wiki) — edit in the separate `linuxtrackx-ir.wiki` checkout; `tools/wiki/convert_help_to_wiki.py` uses `LINUXTRACK_WIKI_ROOT` or `../linuxtrackx-ir.wiki`
- **Original Project**: [uglyDwarf/linuxtrack](https://github.com/uglyDwarf/linuxtrack)
- **Companion Application**: [LinuxTrack Mapper](https://gitlab.com/open-source4661535/linuxtrack-mapper)
