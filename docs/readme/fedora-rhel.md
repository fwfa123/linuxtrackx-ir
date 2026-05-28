# LinuxTrack: Fedora / RHEL

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
sudo dnf group install development-tools
sudo dnf install cmake pkg-config
sudo dnf install libusb1-devel zlib-devel bison flex
sudo dnf install qt6-qtbase-devel qt6-qttools-devel qt6-qt5compat-devel
sudo dnf install mxml-devel mesa-libGL-devel mesa-libGLU-devel
sudo dnf install sqlite   # Qt SQL (QSQLITE driver): Lutris game-database integration

# Wine bridge (MinGW cross-compiler builds the PE DLLs; Wine provides the runtime prefix)
sudo dnf install wine
sudo dnf install mingw32-gcc mingw32-gcc-c++ mingw32-binutils
sudo dnf install mingw64-gcc mingw64-gcc-c++ mingw64-binutils
sudo dnf install winetricks cabextract wget
```

> NSIS is **not** required. The bridge builds with MinGW-w64 only (`makensis` and `linuxtrack-wine.exe` are legacy).

### Step 2: Clone and build

```bash
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"   # if CMake does not find moc/uic
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

## All Dependency Levels (reference)

### Core Dependencies (All Levels)
```bash
sudo dnf group install development-tools
sudo dnf install cmake pkg-config
sudo dnf install libusb1-devel zlib-devel bison flex
sudo dnf install qt6-qtbase-devel qt6-qttools-devel qt6-qt5compat-devel
sudo dnf install mxml-devel mesa-libGL-devel mesa-libGLU-devel
sudo dnf install sqlite  # Qt SQL (QSQLITE driver): Lutris game-database integration
```
**Fedora package names:** There is no `qt5-qmake` package—`qmake-qt5` is included in `qt5-qtbase-devel`. On Debian/Ubuntu, Mini-XML is **`libmxml-dev`**.

### Wine Support (Level 2+)
```bash
# Install Wine runtime + MinGW cross toolchains (required for PE bridge build)
sudo dnf install wine
sudo dnf install mingw32-gcc mingw32-gcc-c++ mingw32-binutils
sudo dnf install mingw64-gcc mingw64-gcc-c++ mingw64-binutils
sudo dnf install winetricks  # REQUIRED: For MFC42 library installation
sudo dnf install cabextract wget  # REQUIRED: For alternative installation methods
```

**Fedora 44 / dnf arch:** On x86_64, the MinGW compiler packages are the same RPMs whether you omit the arch or pin `.x86_64` (e.g. when `dnf` lists multilib choices). Equivalent explicit install:

```bash
sudo dnf install mingw32-gcc.x86_64 mingw32-gcc-c++.x86_64 mingw32-binutils.x86_64
sudo dnf install mingw64-gcc.x86_64 mingw64-gcc-c++.x86_64 mingw64-binutils.x86_64
```

**IMPORTANT**: Level 2+ requires **MinGW-w64** toolchains to build the Wine bridge PE artifacts. Runtime: Wine or Proton with a 32-bit or 64-bit prefix.

NSIS is **not** required for v2.0.0; the Wine bridge builds with MinGW-w64 only (`makensis` and `linuxtrack-wine.exe` are legacy).

### X-Plane Support (Level 3+)
```bash
# Download from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Official SDK archives are ZIP (e.g. XPSDK430.zip); the version in the filename may change.
# Inside the zip: SDK/CHeaders/ (no longer .tar.gz).
sudo dnf install unzip

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
sudo dnf install libv4l-devel v4l-utils
```

### AppImage / packaging build (maintainers)
The v2 `prepare.sh` enables `-DENABLE_WEBCAM=ON`, `-DENABLE_OSC=ON`, `-DENABLE_XPLANE=ON`, and `-DENABLE_FACE_TRACKER=OFF` (CMake default; matches README **Level 5**). Install **Webcam Support (Level 4+)** on the build host so webcam and PS3 Eye (`libp3e`) build. To include OpenCV-based facetrack plugins, add `-DENABLE_FACE_TRACKER=ON` and install **Face tracking (Level 6+)** packages on the builder. Users who only run the AppImage are not required to install `opencv-devel` on their system.

```bash
sudo dnf install patchelf   # rpath fixups for TrackIR libs in the AppImage (package.sh)
```

**Qt Help / `qhelpgenerator`:** On Fedora, `qhelpgenerator` is installed as **`/usr/lib64/qt6/libexec/qhelpgenerator`** by **`qt6-doctools`** (it is not always in `.../bin/`). Install before `ci_build.sh` / `prepare.sh`: `sudo dnf install qt6-doctools`.

### OSC Support (Level 5+)
```bash
sudo dnf install liblo-devel
```

### Face tracking (Level 6+)
Requires Level 4 (webcam). Pass `-DENABLE_FACE_TRACKER=ON` to enable OpenCV-based face tracking when CMake finds OpenCV (default is **OFF**).
```bash
sudo dnf install opencv-devel
```

### Wiimote Support (Level 7+)
Fedora may not ship `libcwiid-devel`. If `dnf install libcwiid-devel` fails, build CWiiD (cwiid) from source:
```bash
# Install build prereqs (Bluetooth dev headers + autotools)
sudo dnf install -y git autoconf automake libtool bluez-libs-devel

git clone https://github.com/mzimmerman/cwiid.git
cd cwiid
aclocal
autoconf
./configure \
  --prefix=/usr/local \
  --with-cwiid-config-dir=/etc/cwiid/ \
  --without-gtk \
  --without-python
make -j$(nproc)
sudo make install
sudo ldconfig

# Verify pkg-config can see the installed cwiid.pc
pkg-config --exists cwiid && pkg-config --modversion cwiid
```

---

## All Build Levels (reference)

### Level 1: TrackIR Only
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 2: TrackIR + Wine (Most Common)
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 3: TrackIR + Wine + X-Plane
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 4: TrackIR + Wine + X-Plane + Webcam
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 5: TrackIR + Wine + X-Plane + Webcam + OSC
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 6: + Face tracking
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 7: + Wiimote
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
export PKG_CONFIG_PATH="/usr/local/lib64/pkgconfig:/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH"
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON -DENABLE_WIIMOTE=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

---

## Qt6 and In-App Help

In-app help is built from HTML in the repo; AppImage builds may run `qhelpgenerator`. On Fedora install **`qt6-doctools`** so **`/usr/lib64/qt6/libexec/qhelpgenerator`** exists (not under `bin/`). If CMake does not find moc/uic, add Qt6 to your PATH: `export PATH="/usr/lib64/qt6/bin:$PATH"`.

**Verify Qt6 tools are accessible (optional):**
```bash
test -x /usr/lib64/qt6/libexec/qhelpgenerator && echo OK || echo "Install: sudo dnf install qt6-doctools"
which qmake6 moc
# qhelpgenerator is under libexec on Fedora; moc/qmake6 are under .../qt6/bin/
```

---

## Troubleshooting

### Qt6 Tools PATH Issues (Most Common)

| Problem | Solution |
|---------|----------|
| `qmake` / `qmake6: command not found` | Qt6 tools not in PATH. Add `/usr/lib64/qt6/bin` to PATH: `export PATH="/usr/lib64/qt6/bin:$PATH"` |
| `Could not find a package configuration file provided by "Qt6"` | Install Qt6 development packages: `sudo dnf install qt6-qtbase-devel qt6-qttools-devel` |
| `qhelpgenerator: command not found` | Install **`qt6-doctools`**. On Fedora the binary is **`/usr/lib64/qt6/libexec/qhelpgenerator`**; add `export PATH="/usr/lib64/qt6/bin:/usr/lib64/qt6/libexec:$PATH"` if tools are not found. |
| `fatal error: bits/c++config.h: No such file or directory` | Install the normal C++ compiler package: `sudo dnf install gcc-c++` |

### Legacy 32-bit Library Notes

The current Wine bridge is built with MinGW PE toolchains, so it no longer needs Fedora/RHEL `*.i686` development packages for the DLL bridge. If you are reading older notes that recommend `wine-devel.i686`, `glibc-devel.i686`, `libstdc++-devel.i686`, `mxml-devel.i686`, or similar packages for the Wine bridge, those notes apply to the archived winegcc-style build path.

If Qt6 qmake is not found, prefer adding Qt6 tools to `PATH` rather than creating symlinks:

```bash
export PATH="/usr/lib64/qt6/bin:/usr/lib64/qt6/libexec:$PATH"
```

### Wine Issues

| Problem | Solution |
|---------|----------|
| `i686-w64-mingw32-gcc: command not found` | Install MinGW 32-bit tools: `sudo dnf install mingw32-gcc mingw32-gcc-c++ mingw32-binutils` |
| `x86_64-w64-mingw32-gcc: command not found` | Install MinGW 64-bit tools: `sudo dnf install mingw64-gcc mingw64-gcc-c++ mingw64-binutils` |
| Wine bridge install fails in prefix | Verify prefix path, `wine` binary, and that MinGW-built payload exists under `/opt/lib/linuxtrack/wine_bridge/`; try current Proton or Wine Staging if issues persist |

### Common Issues

| Problem | Solution |
|---------|----------|
| `Couldn't load library 'libwc.so.0'` | Update library cache: `sudo ldconfig` |
| Wiimote build disabled (`pkg-config` cannot find `cwiid`) | Ensure `cwiid.pc` is installed, then set `PKG_CONFIG_PATH="/usr/local/lib64/pkgconfig:/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH"` and rerun CMake with `-DENABLE_WIIMOTE=ON` |
| GUI not displaying on Wayland | Force X11: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | Add to groups: `sudo usermod -a -G plugdev,input $USER` |
| Application not in launcher | Use `/usr/local` prefix instead of `/opt` |

### Lutris (Flatpak)

When Lutris is installed via Flatpak, game data and Wine/Proton runners live under **`~/.var/app/net.lutris.Lutris/data/lutris/`** (e.g. runners under `runners/wine/` and `runners/proton/`). LinuxtrackX-ir detects this path automatically when you run the prefix tester or install the Wine bridge.

**AppImage + Flatpak Steam or Lutris:** If the native GUI finds Steam/Lutris but the AppImage reports "not found" and the log mentions `flatpak: symbol lookup error` / `libcurl`, the AppImage was invoking the host `flatpak` command with AppImage libraries on `LD_LIBRARY_PATH`. Current builds clear that for host tools and check `~/.var/app/...` data paths directly. After installing Flatpak Steam, launch it once so `~/.var/app/com.valvesoftware.Steam/data/Steam` exists.

If you see **"Lutris wine binary not found for version: GE-Proton10-32"** (or a similar version) with Flatpak Lutris, the app should now use the Flatpak runners path automatically. If the issue persists, check the exact Wine/Proton version in the game's Lutris config and ensure that runner is installed in Lutris (e.g. via Lutris → Runners).

If games show **"All library search attempts failed"** when launched from Lutris Flatpak, grant filesystem access using Flatseal; see **[Flatpak (Lutris and games)](flatpak.md)**.

---

## Advanced: Custom Build Combinations

Combine cmake flags for your specific needs:

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
export PATH="/usr/lib64/qt6/bin:$PATH"
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
export PATH="/usr/lib64/qt6/bin:$PATH"
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_OSC=ON
```

See the main README for all available CMake options.
