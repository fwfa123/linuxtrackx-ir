# LinuxTrack Build Guide: Fedora/RHEL

## Quick Package Installation

### Core Dependencies (All Levels)
```bash
sudo dnf group install development-tools
sudo dnf install cmake pkg-config
sudo dnf install libusb1-devel zlib-devel bison flex
sudo dnf install qt6-qtbase-devel qt6-qttools-devel qt6-qt5compat-devel qt6-qttools-help
sudo dnf install mxml-devel mesa-libGL-devel mesa-libGLU-devel
sudo dnf install sqlite  # Qt SQL (QSQLITE driver): Lutris game-database integration
```
**Fedora package names:** There is no `qt5-qmake` package—`qmake-qt5` is included in `qt5-qtbase-devel`. On Debian/Ubuntu, Mini-XML is **`libmxml-dev`**.

### Wine Support (Level 2+)
```bash
# Install full Wine with 32-bit support (CRITICAL for MFC42)
sudo dnf install wine wine.i686
sudo dnf install wine-devel wine-devel.i686  # Wine development tools (32-bit and 64-bit)
sudo dnf install glibc-devel.i686 libstdc++-devel.i686  # REQUIRED: 32-bit development headers (C and C++)
sudo dnf install mingw32-nsis  # REQUIRED: NSIS installer (includes stub files for building Windows installers)
sudo dnf install winetricks  # REQUIRED: For MFC42 library installation
sudo dnf install cabextract wget  # REQUIRED: For alternative installation methods
```

**IMPORTANT**: The full `wine` and `wine.i686` packages are required for 32-bit Wine prefix creation. The error "WINEARCH is set to 'win32' but this is not supported in wow64 mode" indicates missing 32-bit Wine support. Wine mono and gecko components are downloaded automatically when needed.

### X-Plane Support (Level 3+)
```bash
# Download X-Plane SDK from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Extract to: /opt/xplane-sdk/
sudo mkdir -p /opt/xplane-sdk
sudo tar -xzf XPSDK*.tar.gz -C /opt/xplane-sdk/
```

### Webcam Support (Level 4+)
```bash
sudo dnf install libv4l-devel v4l-utils
```

### AppImage / packaging build (maintainers)
The v2 `prepare.sh` enables `-DENABLE_WEBCAM=ON`, `-DENABLE_OSC=ON`, `-DENABLE_XPLANE=ON`, and `-DENABLE_FACE_TRACKER=OFF` (CMake default; matches README **Level 5**). Install **Webcam Support (Level 4+)** on the build host so webcam and PS3 Eye (`libp3e`) build. To include OpenCV-based facetrack plugins, add `-DENABLE_FACE_TRACKER=ON` and install **Face tracking (Level 6+)** packages on the builder. Users who only run the AppImage are not required to install `opencv-devel` on their system.

**Qt Help / `qhelpgenerator`:** On Fedora, `qhelpgenerator` is in **`qt6-qttools-help`**, not in `qt6-qttools` alone. Install it (already listed in core deps above) before running `ci_build.sh` / `prepare.sh`.

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

## Qt6 and in-app help

In-app help is built from HTML in the repo; AppImage builds may run `qhelpgenerator`. Install **`qt6-qttools-help`** so `/usr/lib64/qt6/bin/qhelpgenerator` exists. If CMake does not find moc/uic, add Qt6 to your PATH: `export PATH="/usr/lib64/qt6/bin:$PATH"`.

**Verify Qt6 tools are accessible (optional):**
```bash
test -x /usr/lib64/qt6/bin/qhelpgenerator && echo OK || echo "Install: sudo dnf install qt6-qttools-help"
which qmake6 moc
# qhelpgenerator is usually only under /usr/lib64/qt6/bin/, not on default PATH
```

## Build Commands

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
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 3: TrackIR + Wine + X-Plane
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 4: TrackIR + Wine + X-Plane + Webcam
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 5: TrackIR + Wine + X-Plane + Webcam + OSC
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 6: + Face tracking
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 7: + Wiimote
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt6/bin:$PATH"  # Optional: if CMake does not find moc/uic
export PKG_CONFIG_PATH="/usr/local/lib64/pkgconfig:/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH"
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON -DENABLE_WIIMOTE=ON
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

### Qt6 Tools PATH Issues (Most Common)

| Problem | Solution |
|---------|----------|
| `qmake` / `qmake6: command not found` | Qt6 tools not in PATH. Add `/usr/lib64/qt6/bin` to PATH: `export PATH="/usr/lib64/qt6/bin:$PATH"` |
| `Could not find a package configuration file provided by "Qt6"` | Install Qt6 development packages: `sudo dnf install qt6-qtbase-devel qt6-qttools-devel` |
| `qhelpgenerator: command not found` | Install **`qt6-qttools-help`** (not only `qt6-qttools`), then `export PATH="/usr/lib64/qt6/bin:$PATH"` |
| `fatal error: bits/c++config.h: No such file or directory` | Install 32-bit C++ development: `sudo dnf install gcc-c++.i686` |

### 32-bit Library Issues

| Problem | Solution |
|---------|----------|
| `cannot find -lz: No such file or directory` | Install 32-bit zlib: `sudo dnf install zlib-ng-compat-devel.i686` |
| `cannot find -lmxml: No such file or directory` | Install 32-bit mxml: `sudo dnf install mxml-devel.i686` |
| `cannot find -lusb-1.0: No such file or directory` | Install 32-bit libusb: `sudo dnf install libusb1-devel.i686` |
| `cannot find -lv4l2: No such file or directory` | Install 32-bit v4l: `sudo dnf install libv4l-devel.i686` |
| `i386:x86-64 architecture of input file is incompatible` | Install all 32-bit packages listed above |

**Quick fix for common issues:**
```bash
# If you encounter missing library errors during build
sudo dnf install -y gcc-c++.i686 zlib-ng-compat-devel.i686 libusb1-devel.i686 mxml-devel.i686 libv4l-devel.i686

# If Qt6 qmake is not found
sudo mkdir -p /usr/lib/qt6/bin
sudo ln -s /usr/lib64/qt6/bin/qmake /usr/lib/qt6/bin/qmake
```

### Wine Issues

| Problem | Solution |
|---------|----------|
| `winegcc: command not found` | Install Wine development: `sudo dnf install wine-devel` |
| 32-bit build fails | Install 32-bit headers: `sudo dnf install glibc-devel.i686 gcc.i686` |
| `wine: WINEARCH is set to 'win32' but this is not supported in wow64 mode` | **Install full 32-bit Wine**: `sudo dnf install wine wine.i686` |
| `Error: reading stub "/usr/share/nsis/Stubs/zlib-x86-unicode"` | Install NSIS with stubs: `sudo dnf install mingw32-nsis` |
| `Error initializing CEXEBuild: error setting default stub` | Install NSIS with stubs: `sudo dnf install mingw32-nsis` |

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

If you see **"Lutris wine binary not found for version: GE-Proton10-32"** (or a similar version) with Flatpak Lutris, the app should now use the Flatpak runners path automatically. If the issue persists, check the exact Wine/Proton version in the game's Lutris config and ensure that runner is installed in Lutris (e.g. via Lutris → Runners).

If games show **"All library search attempts failed"** when launched from Lutris Flatpak, grant filesystem access using Flatseal; see **[Flatpak (Lutris and games)](flatpak.md)**.

## Advanced: Custom Build Combinations

Combine cmake flags for your specific needs:

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
export PATH="/usr/lib64/qt6/bin:$PATH"
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
export PATH="/usr/lib64/qt6/bin:$PATH"
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_OSC=ON
```

See the main README for all available CMake options.
