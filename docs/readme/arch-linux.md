# LinuxTrack Build Guide: Arch Linux

This guide applies to **Arch Linux** and derivatives (**CachyOS**, EndeavourOS, Manjaro, etc.). **CMake + Qt6** is the supported build system.

## Choose Your Path

For day-to-day use, prefer the official **AppImage**. It bundles the GUI and most runtime dependencies users need on rolling distributions.

Build from source when you need a full `/opt` install, packaging work, optional feature builds, or the latest branch changes:

```bash
./scripts/build_arch_linux.sh
```

The Wine bridge in this branch is built as real MinGW PE artifacts. It no longer needs the old winegcc / classic Wine Unix library build path.

## Quick Package Installation

### Core Dependencies (All Levels)
```bash
sudo pacman -S base-devel cmake pkg-config
sudo pacman -S libusb bison flex
sudo pacman -S qt6-base qt6-tools qt6-5compat
sudo pacman -S mxml mesa glu  # package name is mxml, not libmxml
# zlib: if you get "zlib and zlib-ng-compat are in conflict", answer N and do NOT remove zlib-ng-compat.
# zlib-ng-compat provides libz; CMake will find it. Only install zlib if neither is present.
sudo pacman -S zlib   # omit if zlib-ng-compat is already installed (CachyOS, etc.)
```

### Wine Support (Level 2+)
```bash
# Wine runtime for testing/installing into Wine or Proton prefixes
sudo pacman -S wine wine-mono wine-gecko

# MinGW cross toolchain for NPClient.dll / NPClient64.dll and tester PE binaries
sudo pacman -S mingw-w64-gcc

# Wine bridge installer support
./scripts/install/install_nsis_arch.sh

# Runtime prefix helpers for MFC42 setup
sudo pacman -S winetricks cabextract wget
```

**Build model:** MinGW builds the Windows PE bridge files. Wine is still needed at runtime for prefixes and for testing/installing the bridge, but the DLL build no longer depends on `winegcc`, `wine-devel`, AUR `wine32`, or classic `/usr/lib32/wine/...` library paths.

### X-Plane Support (Level 3+)
```bash
# Download from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Official SDK archives are ZIP (e.g. XPSDK430.zip); the version in the filename may change.
# Inside the zip: SDK/CHeaders/ (no longer .tar.gz).
sudo pacman -S unzip

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
sudo pacman -S libv4l v4l-utils
```

### AppImage / packaging build (maintainers)
The v2 `prepare.sh` enables `-DENABLE_WEBCAM=ON`, `-DENABLE_OSC=ON`, `-DENABLE_XPLANE=ON`, and `-DENABLE_FACE_TRACKER=OFF` (same as CMake default; aligns with README **Level 5**). Install **Webcam Support (Level 4+)** on the build host so `libwc` and PS3 Eye (`libp3e`) build. To ship OpenCV facetrack inside the AppImage, add `-DENABLE_FACE_TRACKER=ON` and install **Face tracking (Level 6+)** packages on the builder. End users of the released AppImage do not need OpenCV on the system.

### OSC Support (Level 5+)
```bash
sudo pacman -S liblo   # official [extra], not AUR
```

### Face tracking (Level 6+)
Requires Level 4 (webcam). Install OpenCV and pass `-DENABLE_FACE_TRACKER=ON` (default is **OFF**).
```bash
sudo pacman -S opencv
```

### Wiimote Support (Level 7+)
Optional; **cwiid** on AUR is unmaintained and often fails (python2 / old deps — [GitLab #8](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/8)). The **`build_arch_linux.sh`** helper does **not** install Wiimote unless you pass **`--with-wiimote`**.
```bash
# Manual AUR install if you need Wiimote (may fail on current Arch)
yay -S cwiid
# Alternative: yay -S cwiid-git

pkg-config --exists cwiid && pkg-config --modversion cwiid || echo "cwiid.pc not found; check PKG_CONFIG_PATH."
```

---

## Automated build script

[`scripts/build_arch_linux.sh`](../../scripts/build_arch_linux.sh) installs dependencies, Wine runtime packages, MinGW-w64, **liblo** (OSC), checks the X-Plane SDK path, installs NSIS, configures CMake, builds, and installs to `/opt`. **Wiimote (AUR cwiid) is not installed** unless you pass **`--with-wiimote`** ([GitLab #8](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/8)).

**CMake vs packages:** The script’s `cmake ..` line matches roughly **README Level 2** (TrackIR + Wine bridge): it does **not** pass `-DENABLE_WEBCAM=ON`, `-DENABLE_XPLANE=ON`, etc. Defaults leave those **OFF**. The script still installs **opencv**, **v4l-utils**, and **liblo** so you can re-run CMake with higher-level flags without reinstalling packages.

**AUR helper:** If neither **yay** nor **paru** is installed, the script clones and builds **yay** from the AUR (needs **network**, uses **`sudo pacman`**, and implies the usual AUR trust model). Install **paru**/**yay** yourself first if you prefer.

**Split runs** (if a verification step fails or you prefer control — see [GitLab #37](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/37)):

```bash
./scripts/build_arch_linux.sh --deps-only
./scripts/build_arch_linux.sh --deps-only --with-wiimote   # deps + optional Wiimote attempt
./scripts/build_arch_linux.sh --wine-bridge-only
./scripts/build_arch_linux.sh --wine32-only    # legacy alias
./scripts/build_arch_linux.sh --configure-only
./scripts/build_arch_linux.sh --build-only
./scripts/build_arch_linux.sh --install-only   # script runs sudo where needed for install
./scripts/build_arch_linux.sh --with-wiimote   # full run + try AUR cwiid
```

Use `--help` for all flags.

---

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

---

## First launch (Wayland is default on modern Arch)

On **Wayland**, if the GUI does not appear, run:
```bash
QT_QPA_PLATFORM=xcb ltr_gui
```
Otherwise:
```bash
ltr_gui
```

### Camera view / 3D preview quirks

Some users on **source builds** report weak camera preview or a broken 3D view while **in-game tracking still works**. Try `QT_QPA_PLATFORM=xcb`, update GPU/Mesa drivers, and in **Misc** settings try disabling heavy **3D** preview options. This is environment-dependent; there is no single guaranteed fix.

---

## Verification
```bash
ldconfig -p | grep linuxtrack
ltr_server1 --help
ltr_gui
ls /opt/lib/linuxtrack/wine_bridge/   # built bridge pieces (Level 2+)
ls /opt/share/linuxtrack/wine/linuxtrack-wine.exe 2>/dev/null || true  # NSIS installer when built
```

---

## Troubleshooting

### Wine Bridge Issues

| Problem | Solution |
|---------|----------|
| `i686-w64-mingw32-gcc: command not found` | Install `mingw-w64-gcc` |
| `x86_64-w64-mingw32-gcc: command not found` | Install `mingw-w64-gcc` |
| `Wine bridge: disabled (mingw-w64 toolchains and/or makensis not found)` | Install `mingw-w64-gcc` and NSIS (`./scripts/install/install_nsis_arch.sh` or AUR `nsis`), then reconfigure from a clean build dir |
| MFC42 install fails | Use the GUI MFC42 installer or manual `winetricks mfc42` for the target prefix |
| Wine/Proton prefix does not load the bridge | Verify `NPClient.dll` / `NPClient64.dll` were installed into the target prefix and check `NPClient.log` / Steam logs |
| `zlib` and `zlib-ng-compat` in conflict (CachyOS etc.) | Answer **N** (do not remove zlib-ng-compat). Omit `zlib`; run `pkg-config --exists zlib` to confirm |
| Qt6 CMake config not found | `sudo pacman -S qt6-base qt6-tools` |
| **`liblo` not found** / OSC disabled | `sudo pacman -S liblo` then `pkg-config --exists liblo`. Split **`--configure-only`** previously skipped this; current **`build_arch_linux.sh`** runs **`install_osc_support`** before configure. |
| **Webcam disabled** but V4L found | Default **`ENABLE_WEBCAM=OFF`**. Add **`-DENABLE_WEBCAM=ON`** to **`cmake`** (see Level 4) if you need webcam support. |
| X-Plane SDK not found / wrong path | Official download is **ZIP** (`XPSDK*.zip`) with `SDK/CHeaders/` inside. Copy `SDK/` contents to `/opt/xplane-sdk/` — see [x-plane-sdk.md](x-plane-sdk.md) |

### Common issues

| Problem | Solution |
|---------|----------|
| `Couldn't load library 'libwc.so.0'` | `sudo ldconfig` |
| GUI not displaying on Wayland | `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | `sudo usermod -a -G plugdev,input $USER` (re-login) |
| Application not in launcher | `sudo update-desktop-database /opt/share/applications` |

### 32-bit libraries (advanced)

The default MinGW bridge build does not require old `lib32-mxml`, `lib32-liblo`, or Wine Unix library development paths. Historical lib32 notes live in [archive/ARCH_LINUX_32BIT_LIBRARIES.md](../archive/ARCH_LINUX_32BIT_LIBRARIES.md), and the old Arch wine32 walkthrough is archived in [archive/technical/ARCH_LINUX_LEGACY_WINE32_BUILD_WALKTHROUGH.md](../archive/technical/ARCH_LINUX_LEGACY_WINE32_BUILD_WALKTHROUGH.md).

### Qt6 tools PATH (rare)
```bash
export PATH="/usr/lib/qt6/bin:$PATH"
```

---

## Advanced: Custom Build Combinations

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_OSC=ON
```

See the main README for all CMake options.

---

## Historical / archived notes

Older Arch–Wine investigations, including the former AUR `wine32` / classic Wine Unix library walkthrough, live under **[docs/archive/technical/](../archive/technical/)**. **Current** instructions are only this file plus the main [README.md](../../README.md).
