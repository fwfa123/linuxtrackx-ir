# LinuxTrack Build Guide: Arch Linux

## Quick Package Installation

### Core Dependencies (All Levels)
```bash
sudo pacman -S base-devel cmake pkg-config
sudo pacman -S libusb zlib bison flex
sudo pacman -S qt5-base qt5-tools qt5-x11extras
sudo pacman -S libmxml mesa glu sqlite  # Required for Qt help system
```

### Wine Support (Level 2+)
```bash
# Use wine-stable for best TrackIR compatibility (avoid wine-staging)
sudo pacman -S wine-stable wine-stable-mono wine-stable-gecko
# For 32-bit support (if multilib enabled)
sudo pacman -S lib32-glibc lib32-gcc-libs
```

### Webcam Support (Level 3+)
```bash
sudo pacman -S libv4l v4l-utils opencv
```

### OSC Support (Level 4+)
```bash
sudo pacman -S liblo
```

### Wiimote Support (Level 5+)
```bash
# From AUR (use yay or your preferred AUR helper)
yay -S libcwiid
```

### X-Plane Support (Level 6)
```bash
# Download X-Plane SDK from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Extract to: /opt/xplane-sdk/
sudo mkdir -p /opt/xplane-sdk
sudo tar -xzf XPSDK*.tar.gz -C /opt/xplane-sdk/
```

## Build Commands

### Level 1: TrackIR Only
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 2: TrackIR + Wine (Most Common)
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 3: TrackIR + Wine + Webcam
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 4: TrackIR + Wine + Webcam + OSC
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 5: TrackIR + Wine + Webcam + OSC + Wiimote
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 6: Complete Build with X-Plane
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders
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

### Wine Issues (Most Common)

| Problem | Solution |
|---------|----------|
| `wine: WINEARCH is set to 'win32' but this is not supported in wow64 mode` | **Use wine-stable instead of wine-staging** - wine-staging doesn't support pure 32-bit prefixes |
| `wine-staging` conflicts | Remove conflicting packages: `sudo pacman -R wine-staging wine-gecko wine-mono winetricks` |
| Missing 32-bit libraries | **Use the automated build script**: `./scripts/build_32bit_libs.sh` |
| 32-bit/64-bit compilation conflicts | Use explicit 64-bit flags: `CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64"` |
| Qt5 Makefile not generated | Manually generate: `cd src/qt_gui && /usr/bin/qmake-qt5 -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack" ltr_gui.pro` |

### Common Issues

| Problem | Solution |
|---------|----------|
| `Couldn't load library 'libwc.so.0'` | Update library cache: `sudo ldconfig` |
| GUI not displaying on Wayland | Force X11: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | Add to groups: `sudo usermod -a -G plugdev,input $USER` |
| Application not in launcher | Use `/usr/local` prefix instead of `/opt` |

### 32-bit Library Issues

Arch Linux requires 32-bit versions of some libraries for Wine bridge compatibility. If you see "skipping incompatible" errors:

**Automated Solution:**
```bash
# Run the automated script to build missing 32-bit libraries
./scripts/build_32bit_libs.sh

# Then rebuild LinuxTrack
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

**What the script builds:**
- `lib32-mxml` (Mini-XML library)
- `lib32-liblo` (Open Sound Control library)

These are installed to `/usr/lib32/` with correct symlinks.

### Qt5 Tools PATH (Rare)
If Qt5 tools aren't in PATH, add them:
```bash
export PATH="/usr/lib/qt5/bin:$PATH"
```

## Advanced: Custom Build Combinations

Combine cmake flags for your specific needs:

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_OSC=ON
```

See the main README for all available CMake options.
