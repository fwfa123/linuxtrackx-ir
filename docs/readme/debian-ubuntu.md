# LinuxTrack Build Guide: Debian/Ubuntu/MX Linux

## Quick Package Installation

### Core Dependencies (All Levels)
```bash
sudo apt update
sudo apt install build-essential cmake pkg-config
sudo apt install libusb-1.0-0-dev zlib1g-dev bison flex
sudo apt install qtbase5-dev qttools5-dev-tools qttools5-dev libqt5x11extras5-dev
sudo apt install libmxml-dev libx11-dev libxrandr-dev libgl1-mesa-dev libglu1-mesa-dev libqt5opengl5-dev
sudo apt install sqlite3  # Required for Qt help system
```

### Wine Support (Level 2+)
```bash
sudo apt install wine wine-staging wine64 wine64-tools libwine-dev wine32-tools
sudo apt install gcc-multilib libc6-dev-i386  # REQUIRED: 32-bit development headers
sudo apt install winetricks  # REQUIRED: For MFC42 library installation
sudo apt install cabextract wget  # REQUIRED: For alternative installation methods
sudo apt install nsis  # REQUIRED: For Wine bridge installer generation
```

### Webcam Support (Level 3+)
```bash
sudo apt install libv4l-dev v4l-utils libopencv-dev
```

### OSC Support (Level 4+)
```bash
sudo apt install liblo-dev
```

### Wiimote Support (Level 5+)
```bash
sudo apt install libcwiid-dev
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

### Common Issues

| Problem | Solution |
|---------|----------|
| `winegcc: command not found` | Install Wine development tools: `sudo apt install libwine-dev wine32-tools` |
| `wine: WINEARCH is set to 'win32' but this is not supported in wow64 mode` | Install full 32-bit Wine: `sudo apt install wine wine32 wine32-tools` |
| `bits/libc-header-start.h: No such file or directory` | **REQUIRED**: Install 32-bit headers: `sudo apt install gcc-multilib libc6-dev-i386` |
| `Wine plugin: disabled (winegcc/wineg++/makensis not found)` | Install NSIS: `sudo apt install nsis` |
| GUI not displaying on Wayland | Force X11: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | Add user to groups: `sudo usermod -a -G plugdev,input $USER` |
| `qmake: command not found` | Install Qt5 tools: `sudo apt install qttools5-dev-tools` |
| OpenCV detection failed | Build succeeds but no webcam support. Install: `sudo apt install libopencv-dev` |

### Qt5 Tools PATH (Rare)
If Qt5 tools aren't found, they should be in `/usr/lib/x86_64-linux-gnu/qt5/bin/`. Add to PATH if needed:
```bash
export PATH="/usr/lib/x86_64-linux-gnu/qt5/bin:$PATH"
```

## Advanced: Custom Build Combinations

If you want features from different levels, combine the cmake flags:

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_OSC=ON
```

See the main README for all available CMake options.
