# LinuxTrack Build Guide: Debian/Ubuntu/MX Linux

## Quick Package Installation

### Core Dependencies (All Levels)
```bash
sudo apt update
sudo apt install build-essential cmake pkg-config
sudo apt install libusb-1.0-0-dev zlib1g-dev bison flex
sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools
sudo apt install libqt6opengl6-dev  # Required for Qt6OpenGL CMake config
sudo apt install libmxml-dev libx11-dev libxrandr-dev libgl1-mesa-dev libglu1-mesa-dev
```

### WineBridge Support (Level 2+, WOW64 branch)
```bash
sudo apt install wine wine-staging
sudo apt install mingw-w64  # REQUIRED: builds real PE DLL/EXE artifacts
sudo apt install winetricks  # REQUIRED: For MFC42 library installation
sudo apt install cabextract wget  # REQUIRED: For alternative installation methods
sudo apt install nsis  # REQUIRED: For Wine bridge installer generation
```

WineBridge in this branch targets Wine WOW64 behavior and requires Wine 11.0+.

### X-Plane Support (Level 3+)
```bash
# Download from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Official SDK archives are ZIP (e.g. XPSDK430.zip); the version in the filename may change.
# Inside the zip: SDK/CHeaders/ (no longer .tar.gz).
sudo apt install unzip

sudo mkdir -p /opt/xplane-sdk
tmpdir=$(mktemp -d)
unzip -q ~/Downloads/XPSDK*.zip -d "$tmpdir"
sudo cp -a "$tmpdir"/SDK/. /opt/xplane-sdk/
rm -rf "$tmpdir"

# CMake expects:
test -f /opt/xplane-sdk/CHeaders/XPLM/XPLMPlugin.h && echo "X-Plane SDK OK"
```

### Webcam Support (Level 4+)
```bash
sudo apt install libv4l-dev v4l-utils
```

### AppImage / packaging build (maintainers)
The AppImage v2 `prepare.sh` configures with `-DENABLE_WEBCAM=ON`, `-DENABLE_OSC=ON`, `-DENABLE_XPLANE=ON`, and `-DENABLE_FACE_TRACKER=OFF` (CMake default; matches README **Level 5**). On the machine that runs the v2 pipeline (or legacy `build_appimage_phase4.sh` if present), install **Webcam Support (Level 4+)** so `libwc` and **PS3 Eye** (`libp3e`) build. To bundle OpenCV **facetrack** (`libp3eft`), add `-DENABLE_FACE_TRACKER=ON` and install **Face tracking (Level 6+)** packages on the builder. People who only download the AppImage do not need `libopencv-dev` on their system.

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

## Build Commands

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

## Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI
ltr_gui

# Test Wine bridge (if built)
ls /opt/lib/linuxtrack/wine_bridge/
```

## Troubleshooting

### Common Issues

| Problem | Solution |
|---------|----------|
| `Could not find a package configuration file provided by "Qt6"` | **REQUIRED**: Install Qt6 development packages: `sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools libqt6opengl6-dev` |
| `x86_64-w64-mingw32-gcc: command not found` | Install MinGW toolchain: `sudo apt install mingw-w64` |
| Wine older than required baseline | Upgrade to Wine 11.0+ (or current Proton / Wine Staging) |
| `Wine bridge: disabled (mingw-w64 toolchains and/or makensis not found)` | Install toolchain + NSIS: `sudo apt install mingw-w64 nsis` |
| GUI not displaying on Wayland | Force X11: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | Add user to groups: `sudo usermod -a -G plugdev,input $USER` |
| `qmake: command not found` | Install Qt6 tools: `sudo apt install qt6-tools-dev-tools` |
| OpenCV detection failed | Facetrack / `libp3eft` missing; webcam without face mode may still work. On the **build** machine: `sudo apt install libopencv-dev` and configure with `-DENABLE_FACE_TRACKER=ON`. AppImage users should get OpenCV from the bundle, not the distro. |

### Qt6 Tools PATH (Rare)
If Qt6 tools aren't found, they should be in `/usr/lib/x86_64-linux-gnu/qt6/bin/`. Add to PATH if needed:
```bash
export PATH="/usr/lib/x86_64-linux-gnu/qt6/bin:$PATH"
```

## Advanced: Custom Build Combinations

If you want features from different levels, combine the cmake flags:

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_OSC=ON
```

See the main README for all available CMake options.
