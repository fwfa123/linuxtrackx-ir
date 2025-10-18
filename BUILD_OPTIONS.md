# LinuxTrack Build Options Guide

This document provides step-by-step build configurations for LinuxTrack, from minimal TrackIR-only builds to full-featured installations with all optional components.

## Build Philosophy

LinuxTrack uses a **basic build philosophy** where:
- **Core components** (TrackIR + Wine) are enabled by default when dependencies are found
- **Optional components** (webcam, OSC, Wiimote, X-Plane) are disabled by default
- Each build level adds functionality while maintaining compatibility
- **Security features** (PIE, stack protection) are enabled by default

## Security Features

**PIE (Position Independent Executable)** is enabled by default for security:
- Randomizes memory addresses to prevent buffer overflow attacks
- Required by modern Linux distributions
- Only disable if you encounter compatibility issues: `./configure --disable-pie`

---

## Level 1: TrackIR Only (Minimal Build)

**Use Case**: Native Linux games and applications only. No Wine/Steam support.

**Functionality**:
- ✅ TrackIR hardware support
- ✅ LinuxTrack server for native games
- ✅ Alternative tracking methods (AntiMicro joystick detection)
- ❌ No Wine bridge support
- ❌ No Steam game compatibility

### Configure Command
```bash
./configure --prefix=/opt
```

### Required Libraries

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt update
sudo apt install build-essential autoconf automake libtool pkg-config
sudo apt install libusb-1.0-0-dev zlib1g-dev
sudo apt install bison flex
sudo apt install qt5-qmake qtbase5-dev
sudo apt install libmxml-dev
```

#### Arch Linux:
```bash
sudo pacman -S base-devel autoconf automake libtool pkg-config
sudo pacman -S libusb zlib
sudo pacman -S bison flex
sudo pacman -S qt5-base qt5-tools
sudo pacman -S libmxml
```

#### Fedora/RHEL:
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install autoconf automake libtool pkg-config
sudo dnf install libusb1-devel zlib-devel
sudo dnf install bison flex
sudo dnf install qt5-qtbase-devel qt5-qmake
sudo dnf install libmxml-devel
```

### Build and Install
```bash
make -j$(nproc)
sudo make install
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui
```

---

## Level 2: TrackIR + Wine64 Support

**Use Case**: Modern 64-bit Wine games and Steam (Proton). No legacy 32-bit Wine support.

**Functionality**:
- ✅ All TrackIR functionality
- ✅ Wine64 bridge for 64-bit games
- ✅ Steam Proton compatibility
- ❌ No 32-bit Wine game support

### Configure Command
```bash
./configure --prefix=/opt
```

### Additional Libraries (beyond Level 1)

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt install wine64 wine64-tools
sudo apt install makensis  # For installer creation
```

#### Arch Linux:
```bash
sudo pacman -S wine-staging
sudo pacman -S nsis  # For installer creation
```

#### Fedora/RHEL:
```bash
sudo dnf install wine-core wine-tools
sudo dnf install nsis  # For installer creation
```

### Wine Configuration Notes:
- **Wine Bridge Components**: Builds NPClient64.dll.so for 64-bit TrackIR API compatibility
- **Steam Proton**: Compatible with Steam's Proton for Windows games
- **Modern Games**: Supports 64-bit Windows games running under Wine

### Build and Install
```bash
make -j$(nproc)
sudo make install
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui
```

---

## Level 3: TrackIR + Wine32 + Wine64 (Full Wine Support)

**Use Case**: Complete Wine compatibility for both modern and legacy games.

**Functionality**:
- ✅ All TrackIR functionality
- ✅ Wine64 bridge for 64-bit games
- ✅ Wine32 bridge for 32-bit games
- ✅ Full Steam compatibility
- ✅ Legacy game support

### Configure Command
```bash
./configure --prefix=/opt --enable-ltr-32lib-on-x64
```

### Additional Libraries (beyond Level 2)

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt install wine32 wine32-tools
sudo apt install gcc-multilib libc6-dev-i386
```

#### Arch Linux:
```bash
sudo pacman -S wine32  # From AUR: yay -S wine32
sudo pacman -S lib32-glibc lib32-gcc-libs
```

#### Fedora/RHEL:
```bash
sudo dnf install wine-core.i686 wine-tools.i686
sudo dnf install glibc-devel.i686 gcc.i686
```

### Wine Configuration Notes:
- **Complete Wine Support**: Builds both NPClient.dll.so (32-bit) and NPClient64.dll.so (64-bit)
- **Legacy Game Support**: 32-bit Wine bridge for older Windows games
- **MFC42 Compatibility**: LinuxTrack includes enhanced MFC42 winetricks installation for TrackIR compatibility
- **Steam Integration**: Full compatibility with Steam and Proton

### MFC42 Installation:
After installation, LinuxTrack will automatically handle MFC42 installation via winetricks:
- **Enhanced Debugging**: Comprehensive output for troubleshooting
- **Automatic Fallback**: Falls back to `winetricks vcrun6` if MFC42 fails
- **32-bit Wine Prefix**: Forces `WINEARCH=win32` for optimal compatibility

### Build and Install
```bash
make -j$(nproc)
sudo make install
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui
```

---

## Level 4: TrackIR + Wine + Webcam Support

**Use Case**: Add webcam-based face tracking as an alternative to TrackIR hardware.

**Functionality**:
- ✅ All previous functionality
- ✅ Webcam face tracking
- ✅ V4L2 webcam support
- ✅ Alternative tracking when TrackIR unavailable

### Configure Command
```bash
./configure --prefix=/opt --enable-ltr-32lib-on-x64
```

### Additional Libraries (beyond Level 3)

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt install libv4l-dev v4l-utils
sudo apt install libopencv-dev  # For face tracking
```

#### Arch Linux:
```bash
sudo pacman -S libv4l v4l-utils
sudo pacman -S opencv  # For face tracking
```

#### Fedora/RHEL:
```bash
sudo dnf install libv4l-devel v4l-utils
sudo dnf install opencv-devel  # For face tracking
```

### Build and Install
```bash
make -j$(nproc)
sudo make install
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui
```

---

## Level 5: TrackIR + Wine + Webcam + OSC Support

**Use Case**: Add OSC (Open Sound Control) support for external applications and MIDI controllers.

**Functionality**:
- ✅ All previous functionality
- ✅ OSC protocol support
- ✅ External application integration
- ✅ MIDI controller support

### Configure Command
```bash
./configure --prefix=/opt --enable-ltr-32lib-on-x64
```

### Additional Libraries (beyond Level 4)

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt install liblo-dev
```

#### Arch Linux:
```bash
sudo pacman -S liblo
```

#### Fedora/RHEL:
```bash
sudo dnf install liblo-devel
```

### Build and Install
```bash
make -j$(nproc)
sudo make install
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui
```

---

## Level 6: TrackIR + Wine + Webcam + OSC + Wiimote Support

**Use Case**: Add Nintendo Wii Remote support as an alternative tracking device.

**Functionality**:
- ✅ All previous functionality
- ✅ Wiimote tracking support
- ✅ Alternative input device
- ✅ Motion control support

### Configure Command
```bash
./configure --prefix=/opt --enable-ltr-32lib-on-x64
```

### Additional Libraries (beyond Level 5)

#### Debian/Ubuntu/MX Linux:
```bash
sudo apt install libcwiid-dev
```

#### Arch Linux:
```bash
sudo pacman -S libcwiid  # From AUR: yay -S libcwiid
```

#### Fedora/RHEL:
```bash
sudo dnf install libcwiid-devel
```

### Build and Install
```bash
make -j$(nproc)
sudo make install
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui
```

---

## Level 7: TrackIR + Wine + Webcam + OSC + Wiimote + X-Plane Support

**Use Case**: Complete build with X-Plane flight simulator plugin support.

**Functionality**:
- ✅ All previous functionality
- ✅ X-Plane plugin support
- ✅ Flight simulator integration
- ✅ Complete feature set

### Configure Command
```bash
./configure --prefix=/opt --enable-ltr-32lib-on-x64 --with-xplane-sdk=/opt/xplane-sdk/CHeaders
```

### Additional Libraries (beyond Level 6)

#### X-Plane SDK Setup:
```bash
# Download X-Plane SDK from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Extract to: /opt/xplane-sdk/
sudo mkdir -p /opt/xplane-sdk
sudo tar -xzf XPSDK*.tar.gz -C /opt/xplane-sdk/
```

### Build and Install
```bash
make -j$(nproc)
sudo make install
```

### Verification
```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test GUI (X11 recommended)
ltr_gui

# Test GUI on Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui
```

---

## Quick Reference: All Libraries Combined

### Debian/Ubuntu/MX Linux (Complete Build):
```bash
sudo apt update
sudo apt install build-essential autoconf automake libtool pkg-config
sudo apt install libusb-1.0-0-dev zlib1g-dev bison flex
sudo apt install qt5-qmake qtbase5-dev libmxml-dev
sudo apt install wine64 wine64-tools wine32 wine32-tools makensis
sudo apt install gcc-multilib libc6-dev-i386
sudo apt install libv4l-dev v4l-utils libopencv-dev
sudo apt install liblo-dev libcwiid-dev
```

### Arch Linux (Complete Build):
```bash
sudo pacman -S base-devel autoconf automake libtool pkg-config
sudo pacman -S libusb zlib bison flex qt5-base qt5-tools libmxml
sudo pacman -S wine-staging wine32 nsis
sudo pacman -S lib32-glibc lib32-gcc-libs
sudo pacman -S libv4l v4l-utils opencv liblo libcwiid
```

### Fedora/RHEL (Complete Build):
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install autoconf automake libtool pkg-config
sudo dnf install libusb1-devel zlib-devel bison flex
sudo dnf install qt5-qtbase-devel qt5-qmake libmxml-devel
sudo dnf install wine-core wine-tools wine-core.i686 wine-tools.i686 nsis
sudo dnf install glibc-devel.i686 gcc.i686
sudo dnf install libv4l-devel v4l-utils opencv-devel liblo-devel libcwiid-devel
```

---

## Build Verification

After installation, verify your build:

```bash
# Check installed components
ldconfig -p | grep linuxtrack

# Test TrackIR support
ltr_server1 --help

# Test Wine bridge (if installed)
ls /opt/lib/linuxtrack/wine_bridge/

# Check webcam support (if installed)
ls /opt/lib/linuxtrack/libwc.so*
```

---

## Troubleshooting

### Common Issues:

| Problem | Solution |
|---------|----------|
| `winegcc: command not found` | Install Wine development tools: `sudo apt install libwine-dev wine32-tools` (Debian/Ubuntu) or `sudo dnf install wine-devel` (Fedora) or `sudo pacman -S wine` (Arch) |
| `Couldn't load library 'libwc.so.0'` | Library cache not updated. Run: `sudo ldconfig` then restart LinuxTrack |
| `bits/libc-header-start.h: No such file or directory` | Install 32-bit headers: `sudo apt install gcc-multilib libc6-dev-i386` (Debian/Ubuntu) or `sudo dnf install glibc-devel.i686 libstdc++-devel.i686` (Fedora) or `sudo pacman -S lib32-glibc lib32-gcc-libs` (Arch) |
| GUI not displaying on Wayland | Force X11 compatibility: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | Add user to required groups: `sudo usermod -a -G plugdev,input $USER` |
| Application not appearing in launcher | Use `--prefix=/usr/local` instead of `/opt` during installation |
| Firmware extraction fails | Run `./scripts/wine_check.sh` to diagnose Wine issues |

### Distribution-Specific Issues:

#### **Debian/Ubuntu/MX Linux**
| Problem | Solution |
|---------|----------|
| `configure: error: Qt5 development headers not found` | Install: `sudo apt install qtbase5-dev qttools5-dev libqt5x11extras5-dev` |
| `configure: opencv4 >= 0.29.0... no` | This is normal - OpenCV detection issue but build succeeds. Install: `sudo apt install libopencv-dev` |
| `qmake: command not found` | Install: `sudo apt install qttools5-dev-tools` |
| 32-bit build fails | This is normal if OpenCV detection failed. Core functionality still works. |
| Wine bridge compilation errors | These don't affect core LinuxTrack functionality |

#### **Fedora/RHEL**
| Problem | Solution |
|---------|----------|
| `fatal error: bits/c++config.h: No such file or directory` | Install 32-bit C++ development packages: `sudo dnf install gcc-c++.i686` |
| `cannot find -lz: No such file or directory` | Install 32-bit zlib development: `sudo dnf install zlib-ng-compat-devel.i686` |
| `cannot find -lmxml: No such file or directory` | Install 32-bit mxml development: `sudo dnf install mxml-devel.i686` |
| `cannot find -lusb-1.0: No such file or directory` | Install 32-bit libusb development: `sudo dnf install libusb1-devel.i686` |
| `cannot find -lv4l2: No such file or directory` | Install 32-bit v4l development: `sudo dnf install libv4l-devel.i686` |
| Qt5 qmake not found | Create symlink: `sudo ln -s /usr/lib64/qt5/bin/qmake /usr/lib/qt5/bin/qmake` |

#### **Arch Linux**
| Problem | Solution |
|---------|----------|
| `wine: WINEARCH is set to 'win32' but this is not supported in wow64 mode` | **Use wine-stable instead of wine-staging** - wine-staging doesn't support pure 32-bit prefixes |
| `wine-staging` conflicts with `wine-stable` | Remove conflicting packages: `sudo pacman -R wine-staging wine-gecko wine-mono winetricks` |
| Missing 32-bit libraries (liblo, mxml) | **Use the automated build script: `./scripts/build_32bit_libs.sh`** |
| 32-bit/64-bit compilation conflicts | **Use explicit 64-bit flags: `CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64"`** |
| Qt5 Makefile not generated | **Manually generate: `cd src/qt_gui && /usr/bin/qmake-qt5 -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack" ltr_gui.pro`** |

### Display Server Compatibility

#### **X11 (Recommended)**
LinuxTrack works best with X11:
```bash
ltr_gui
```

#### **Wayland Compatibility**
If you're using Wayland, force X11 compatibility:
```bash
QT_QPA_PLATFORM=xcb ltr_gui
```

### Build Summary:
After running `./configure`, check the build summary for enabled components:
- ✅ Webcam support: yes/no
- ✅ Wiimote support: yes/no  
- ✅ TrackIR support: yes/no
- ✅ Facetracker support: yes/no
- ✅ XPlane plugin: yes/no
- ✅ Mickey: yes/no
- ✅ Wine plugin: yes/no
- ✅ OSC support: yes/no
- ✅ PIE support: enabled/disabled

### Testing Features
```bash
./scripts/test_wiimote_support.sh    # Test Wiimote support
./scripts/test_osc_support.sh        # Test OSC support
./scripts/test_xplane_sdk.sh         # Test X-Plane SDK support
./scripts/test_wine_support.sh       # Test Wine compatibility
```

## Supported Games

### **Native Linux Games**
- DCS World
- War Thunder
- Elite Dangerous
- Euro Truck Simulator 2
- American Truck Simulator

### **Windows Games (via Wine/Steam Proton)**
- Microsoft Flight Simulator
- IL-2 Sturmovik series
- Falcon 4
- Commanche vs Havoc
- **X-Plane** (requires X-Plane SDK)

## Usage

### **First Launch**
```bash
# For X11 (recommended)
ltr_gui

# For Wayland (force X11 compatibility)
QT_QPA_PLATFORM=xcb ltr_gui
```

### **Configuration Steps**
1. **Select your tracking device** (TrackIR, webcam, etc.)
2. **Calibrate tracking points** for accurate head tracking
3. **Set up game profiles** for different applications
4. **Test tracking functionality** before gaming

### **Alternative Tracking Methods**
- **AntiMicro joystick detection** - Convert head tracking to joystick input
- **Server approach** - Use LinuxTrack server for Wine games
- **OSC network** - External application integration
