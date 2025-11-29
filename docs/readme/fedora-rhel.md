# LinuxTrack Build Guide: Fedora/RHEL

## Quick Package Installation

### Core Dependencies (All Levels)
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake pkg-config
sudo dnf install libusb1-devel zlib-devel bison flex
sudo dnf install qt5-qtbase-devel qt5-qmake qt5-qttools-devel
sudo dnf install libmxml-devel mesa-libGL-devel mesa-libGLU-devel
sudo dnf install sqlite  # Required for Qt help system
```

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

### Webcam Support (Level 3+)
```bash
sudo dnf install libv4l-devel v4l-utils opencv-devel
```

### OSC Support (Level 4+)
```bash
sudo dnf install liblo-devel
```

### Wiimote Support (Level 5+)
```bash
sudo dnf install libcwiid-devel
```

### X-Plane Support (Level 6)
```bash
# Download X-Plane SDK from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Extract to: /opt/xplane-sdk/
sudo mkdir -p /opt/xplane-sdk
sudo tar -xzf XPSDK*.tar.gz -C /opt/xplane-sdk/
```

## Qt5 Tools PATH Fix (IMPORTANT)

On Fedora/RHEL systems, Qt5 tools are installed in `/usr/lib64/qt5/bin/` which may not be in your default PATH. This causes build failures.

**Before building, add Qt5 tools to your PATH:**
```bash
export PATH="/usr/lib64/qt5/bin:$PATH"
```

**To make this permanent, add to your `~/.bashrc`:**
```bash
echo 'export PATH="/usr/lib64/qt5/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

**Verify Qt5 tools are accessible:**
```bash
which qhelpgenerator qmake moc
# Should show: /usr/lib64/qt5/bin/qhelpgenerator, etc.
```

## Build Commands

### Level 1: TrackIR Only
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 2: TrackIR + Wine (Most Common)
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 3: TrackIR + Wine + Webcam
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 4: TrackIR + Wine + Webcam + OSC
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 5: TrackIR + Wine + Webcam + OSC + Wiimote
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 6: Complete Build with X-Plane
```bash
mkdir build && cd build
export PATH="/usr/lib64/qt5/bin:$PATH"  # Required for Qt5 tools
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders
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
ls /usr/local/lib/linuxtrack/wine_bridge/
```

## Troubleshooting

### Qt5 Tools PATH Issues (Most Common)

| Problem | Solution |
|---------|----------|
| `qhelpgenerator: command not found` | Add Qt5 tools to PATH: `export PATH="/usr/lib64/qt5/bin:$PATH"` |
| Build fails at help generation step | Qt5 tools not in PATH. Run: `export PATH="/usr/lib64/qt5/bin:$PATH" && make -j$(nproc)` |
| `qmake: command not found` | Qt5 tools not in PATH. Add `/usr/lib64/qt5/bin` to PATH |
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

# If Qt5 qmake is not found
sudo mkdir -p /usr/lib/qt5/bin
sudo ln -s /usr/lib64/qt5/bin/qmake /usr/lib/qt5/bin/qmake
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
| GUI not displaying on Wayland | Force X11: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | Add to groups: `sudo usermod -a -G plugdev,input $USER` |
| Application not in launcher | Use `/usr/local` prefix instead of `/opt` |

## Advanced: Custom Build Combinations

Combine cmake flags for your specific needs:

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
export PATH="/usr/lib64/qt5/bin:$PATH"
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
export PATH="/usr/lib64/qt5/bin:$PATH"
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_OSC=ON
```

See the main README for all available CMake options.
