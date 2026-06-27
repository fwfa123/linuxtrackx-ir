# LinuxTrack: Debian / Ubuntu / MX Linux

> Built with **CMake + Qt6**. The Wine bridge ships as real **MinGW-w64 PE** artifacts — the old winegcc / classic Wine Unix library build path is no longer used.

---

## Option A — AppImage (no build required)

Download the latest AppImage from the [Releases page](https://gitlab.com/fwfa123/linuxtrackx-ir/-/releases), make it executable, and run:

```bash
chmod +x LinuxTrack-x86_64.AppImage
./LinuxTrack-x86_64.AppImage
```

The AppImage bundles the GUI and Wine bridge. No system packages beyond Wine (for prefix setup) are required.

---

## Option B — Build from Source

### Step 1: Install dependencies (Level 2 — TrackIR + Wine, most common)

```bash
# Core build tools and Qt6
sudo apt update
sudo apt install build-essential cmake pkg-config
sudo apt install libusb-1.0-0-dev zlib1g-dev bison flex
sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools libqt6opengl6-dev
sudo apt install libmxml-dev libx11-dev libxrandr-dev libgl1-mesa-dev libglu1-mesa-dev

# 32-bit native library (liblinuxtrack32.so) — host GCC -m32, not MinGW
# Required on amd64 for 32-bit Wine/Proton; build-essential alone is not enough.
sudo dpkg --add-architecture i386   # safe to repeat if already enabled
sudo apt update
sudo apt install gcc-multilib g++-multilib libc6-dev-i386

# Wine bridge (MinGW cross-compiler builds the PE DLLs; Wine provides the runtime prefix)
sudo apt install wine wine-staging mingw-w64 winetricks cabextract wget
```

> NSIS is **not** required. The bridge builds with MinGW-w64 only (`makensis` and `linuxtrack-wine.exe` are legacy).
>
> **MinGW vs multilib:** `mingw-w64` builds the Windows PE DLLs/EXEs. `gcc-multilib` / `libc6-dev-i386` build the native 32-bit `liblinuxtrack32.so` that 32-bit Wine processes load. Both are needed for a full Level 2 install on amd64.

### Step 2: Clone and build

Always configure from a dedicated `build/` directory (never the repo root). If you copied a tree from another machine or an old build failed partway, remove `build/` and reconfigure after installing dependencies.

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
ls /opt/lib/linuxtrack/liblinuxtrack32.so.0      # 32-bit native shim (amd64 builds)
ls /opt/lib/linuxtrack/wine_bridge/NPClient.dll   # Level 2+ Wine bridge
```

---

## All Dependency Levels (reference)

### Core Dependencies (All Levels)
```bash
sudo apt update
sudo apt install build-essential cmake pkg-config
sudo apt install libusb-1.0-0-dev zlib1g-dev bison flex
sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools
sudo apt install libqt6opengl6-dev  # Required for Qt6OpenGL CMake config
sudo apt install libmxml-dev libx11-dev libxrandr-dev libgl1-mesa-dev libglu1-mesa-dev

# amd64 only: native 32-bit liblinuxtrack32.so (ENABLE_LTR_32LIB_ON_X64, on by default)
sudo dpkg --add-architecture i386
sudo apt update
sudo apt install gcc-multilib g++-multilib libc6-dev-i386
```

### WineBridge Support (Level 2+)
```bash
sudo apt install wine wine-staging
sudo apt install mingw-w64  # REQUIRED: builds real PE DLL/EXE artifacts
sudo apt install winetricks  # REQUIRED: For MFC42 library installation
sudo apt install cabextract wget  # REQUIRED: For alternative installation methods
```

NSIS is **not** required for v2.0.0; the Wine bridge builds with MinGW-w64 only.

Wine bridge installs into 32-bit or 64-bit Wine/Proton prefixes via native copy + `wine reg` (MinGW-w64 required at build time).

### X-Plane Support (Level 3+)
```bash
# Download from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Official SDK archives are ZIP (e.g. XPSDK430.zip); the version in the filename may change.
# Inside the zip: SDK/CHeaders/ (no longer .tar.gz).
sudo apt install unzip

sudo mkdir -p /opt/xplane-sdk
sudo rm -rf /tmp/xplane-sdk-extract
sudo unzip -q ~/Downloads/XPSDK*.zip -d /tmp/xplane-sdk-extract
sudo cp -a /tmp/xplane-sdk-extract/SDK/. /opt/xplane-sdk/
sudo rm -rf /tmp/xplane-sdk-extract

# CMake expects:
test -f /opt/xplane-sdk/CHeaders/XPLM/XPLMPlugin.h && echo "X-Plane SDK OK"
```

### Webcam Support (Level 4+)
```bash
sudo apt install libv4l-dev v4l-utils
```

### AppImage / packaging build (maintainers)
The AppImage v2 `prepare.sh` configures with `-DENABLE_WEBCAM=ON`, `-DENABLE_OSC=ON`, `-DENABLE_XPLANE=ON`, and `-DENABLE_FACE_TRACKER=OFF` (CMake default; matches README **Level 5**). On the machine that runs the v2 pipeline (or legacy `build_appimage_phase4.sh` if present), install **Webcam Support (Level 4+)** so `libwc` and **PS3 Eye** (`libp3e`) build. To bundle OpenCV **facetrack** (`libp3eft`), add `-DENABLE_FACE_TRACKER=ON` and install **Face tracking (Level 6+)** packages on the builder. People who only download the AppImage do not need `libopencv-dev` on their system.

```bash
sudo apt install patchelf   # rpath fixups for TrackIR libs in the AppImage (package.sh)
```

### OSC Support (Level 5+)
```bash
sudo apt install liblo-dev
```

### Face tracking (Level 6+)
Requires Level 4 (webcam). Pass `-DENABLE_FACE_TRACKER=ON` to enable OpenCV-based face tracking in `libwc` and `libp3eft` when CMake finds OpenCV (default is **OFF**).
```bash
sudo apt install libopencv-dev
```

### Wiimote Support (Level 7+)
```bash
sudo apt install libcwiid-dev

# Verify pkg-config can see the installed cwiid.pc
pkg-config --exists cwiid && pkg-config --modversion cwiid || echo "cwiid.pc not found; check libcwiid installation."
```

---

## All Build Levels (reference)

### Level 1: TrackIR Only
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 2: TrackIR + Wine (Most Common)
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 3: TrackIR + Wine + X-Plane
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 4: TrackIR + Wine + X-Plane + Webcam
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 5: TrackIR + Wine + X-Plane + Webcam + OSC
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 6: + Face tracking
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 7: + Wiimote
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON -DENABLE_WIIMOTE=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

---

## Troubleshooting

### Common Issues

| Problem | Solution |
|---------|----------|
| `bits/libc-header-start.h: No such file or directory` on `linuxtrack32` / `ltlib.c` | Install multilib: `sudo apt install gcc-multilib libc6-dev-i386` (run `sudo dpkg --add-architecture i386` first if needed). MinGW/Wine bridge targets may still compile; this failure is the separate native 32-bit library. Wipe `build/` and reconfigure after installing. |
| `Could not find a package configuration file provided by "Qt6"` | **REQUIRED**: Install Qt6 development packages: `sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools libqt6opengl6-dev` |
| `x86_64-w64-mingw32-gcc: command not found` | Install MinGW toolchain: `sudo apt install mingw-w64` |
| Wine bridge install fails in prefix | Verify prefix path, `wine` binary, and payload under `/opt/lib/linuxtrack/wine_bridge/`; try current Proton or Wine Staging if issues persist |
| `Wine bridge: disabled (mingw-w64 toolchains not found)` | Install MinGW toolchain: `sudo apt install mingw-w64` |
| GUI not displaying on Wayland | Force X11: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | Add user to groups: `sudo usermod -a -G plugdev,input,uinput,video $USER` (re-login). Webcams need **`video`**; udev rules do not cover `/dev/video*`. |
| Webcam not listed | System tab **Webcam support: YES** is compile-time only | Add **`video`** group; run `v4l2-ctl --list-devices`; check System tab for **libwc driver** status; `sudo ldconfig` if `libwc.so.0` missing |
| `qmake: command not found` | Install Qt6 tools: `sudo apt install qt6-tools-dev-tools` |
| OpenCV detection failed | Facetrack / `libp3eft` missing; webcam without face mode may still work. On the **build** machine: `sudo apt install libopencv-dev` and configure with `-DENABLE_FACE_TRACKER=ON`. AppImage users should get OpenCV from the bundle, not the distro. |
| AppImage build: `patchelf not available` | `sudo apt install patchelf`, then rebuild the AppImage |
| Stale Qt/autotools files in source tree after moving machines | Remove gitignored artifacts, then fresh `build/`: `rm -f src/pathconfig.h src/qt_gui/ui_*.h config.h`; `rm -rf build/` |
| Skip native 32-bit library (not recommended for Wine) | `cmake .. -DENABLE_LTR_32LIB_ON_X64=OFF` |

### Qt6 Tools PATH (Rare)
If Qt6 tools aren't found, they should be in `/usr/lib/x86_64-linux-gnu/qt6/bin/`. Add to PATH if needed:
```bash
export PATH="/usr/lib/x86_64-linux-gnu/qt6/bin:$PATH"
```

---

## Advanced: Custom Build Combinations

If you want features from different levels, combine the cmake flags:

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_OSC=ON
```

See the main README for all available CMake options.
