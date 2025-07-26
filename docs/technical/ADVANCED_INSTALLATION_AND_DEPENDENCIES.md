# Advanced Installation & Dependency Analysis

This document provides in-depth information for advanced users, developers, and maintainers who need to:
- Troubleshoot complex build issues
- Verify and install all dependencies
- Understand advanced build options
- Perform distribution-specific setup

---

## 📋 Dependency Verification Checklist

To ensure you have all required dependencies, run these verification commands on your system:

### Core Build Tools
```bash
which gcc g++ make autoconf automake libtool bison flex pkg-config
```

### Qt5 Dependencies (Critical)
```bash
qmake-qt5 -query QT_VERSION
pkg-config --exists Qt5Core Qt5Widgets Qt5OpenGL Qt5Help Qt5Network Qt5X11Extras
```

### Library Dependencies
```bash
pkg-config --exists opencv4
pkg-config --exists libusb-1.0
pkg-config --exists mxml
pkg-config --exists liblo  # Optional: OSC support
pkg-config --exists libcwiid  # Optional: Wii support
ls /usr/include/xplane_sdk/XPLM/XPLMPlugin.h  # Optional: X-Plane support
```

### Development Headers
```bash
ls /usr/include/qt5/QtCore/QtCore
ls /usr/include/qt5/QtWidgets/QtWidgets
ls /usr/include/qt5/QtOpenGL/QtOpenGL
ls /usr/include/qt5/QtHelp/QtHelp
```

### Wine Dependencies (Windows Components)
```bash
which winegcc wineg++
winegcc --version
```

### 32-bit Development Support
```bash
ls /usr/include/i386-linux-gnu/bits/libc-header-start.h 2>/dev/null || ls /usr/include/bits/libc-header-start.h 2>/dev/null || echo "32-bit headers not found"
```

---

## 🐧 Distribution-Specific Dependencies

### Debian/Ubuntu/MX Linux
```bash
sudo apt install -y build-essential autoconf automake libtool qtbase5-dev qttools5-dev-tools qttools5-dev libqt5x11extras5-dev libopencv-dev libusb-1.0-0-dev libmxml-dev libx11-dev libxrandr-dev bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev wine-staging winetricks liblo7 liblo-dev libcwiid1 libcwiid-dev
sudo apt install -y wine-devel wine32:i386
```

### Fedora/RHEL/CentOS
```bash
sudo dnf install -y gcc gcc-c++ make autoconf automake libtool qt5-qtbase-devel qt5-qttools-devel qttools5-dev qt5-qtx11extras-devel opencv-devel libusb1-devel libmxml-devel libX11-devel libXrandr-devel bison flex nsis glibc-devel.i686 libstdc++-devel.i686 v4l-utils-devel wine-staging winetricks liblo liblo-devel libcwiid libcwiid-devel
sudo dnf install -y wine-devel wine-tools
```

### Arch Linux/Manjaro
```bash
sudo pacman -S --needed base-devel autoconf automake libtool qt5-base qt5-tools qt5-x11extras opencv libusb mxml libx11 libxrandr bison flex lib32-glibc lib32-gcc-libs v4l-utils wine-staging winetricks liblo libcwiid
./scripts/install/install_nsis_arch.sh
```

---

## 🛠️ Advanced Build Options

### Simple Build
```bash
make distclean 2>/dev/null || true
autoreconf -fiv
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install
sudo usermod -a -G plugdev $USER
```

### Build Options
```bash
# Debug build
./configure --prefix=/usr/local --enable-debug

# Disable PIE (not recommended for security)
./configure --prefix=/usr/local --disable-pie

# Disable Windows components
./configure --prefix=/usr/local --disable-wine-bridge

# Custom Qt5 location
./configure --prefix=/usr/local QMAKE=/usr/bin/qmake-qt5

# Enable XPlane plugin support
./configure --prefix=/usr/local --with-xplane-sdk=/usr/include/xplane_sdk
```

---

## 🧩 Optional Features & Libraries

### XPlane Plugin Support
- **XPlane SDK** (required for X-Plane plugin): Download from [Laminar Research Developer Site](https://developer.x-plane.com/sdk/plugin-sdk-downloads/)
- **Default location**: `/usr/include/xplane_sdk/`
- **Custom location**: Use `--with-xplane-sdk=/path/to/sdk` during configure

### OSC Support
- **liblo** (required for OSC support): Lightweight OSC (Open Sound Control) library
- **Installation**: `sudo apt install liblo7 liblo-dev` (Debian/Ubuntu/MX) or `sudo dnf install liblo liblo-devel` (Fedora/RHEL) or `sudo pacman -S liblo` (Arch)
- **Features**: Enables OSC server for real-time tracking data transmission
- **Usage**: Builds `osc_server` binary for sending tracking data via OSC protocol

### Wiimote Support
- **libcwiid** (required for Wiimote support): Library to interface with Nintendo Wiimote
- **Installation**: `sudo apt install libcwiid1 libcwiid-dev` (Debian/Ubuntu/MX) or `sudo dnf install libcwiid libcwiid-devel` (Fedora/RHEL) or `sudo pacman -S libcwiid` (Arch)
- **Features**: Enables Wiimote tracking for head movement detection
- **Usage**: Builds `wii_server` binary for Wiimote connection and tracking

---

## 🐞 Troubleshooting Advanced Build Issues

### Qt5 Issues
```bash
qmake-qt5 -query QT_VERSION
pkg-config --modversion Qt5Core
sudo apt install --reinstall qtbase5-dev qttools5-dev-tools libqt5x11extras5-dev qttools5-dev
```

### Wine Issues
```bash
wine --version
winegcc --version
sudo apt install wine-devel wine32:i386  # Debian/Ubuntu
sudo dnf install wine-devel wine-tools   # Fedora
sudo pacman -S wine-staging              # Arch
```

### 32-bit Support Issues
```bash
# Enable 32-bit architecture (Debian/Ubuntu)
sudo dpkg --add-architecture i386
sudo apt update
sudo apt install gcc-multilib libc6-dev-i386

# Install 32-bit libraries (Fedora)
sudo dnf install glibc-devel.i686 libstdc++-devel.i686

# Install 32-bit libraries (Arch)
sudo pacman -S lib32-glibc lib32-gcc-libs
```

### X-Plane SDK Issues
```bash
ls /usr/include/xplane_sdk/XPLM/XPLMPlugin.h
sudo mkdir -p /usr/include/xplane_sdk
sudo cp -r /path/to/extracted/SDK/CHeaders/* /usr/include/xplane_sdk/
./configure --prefix=/usr/local 2>&1 | grep "XPlane plugin"
```

---

## 📚 See Also
- Main [README](../../README.md) for user-focused quick start and installation
- [docs/troubleshooting/TROUBLESHOOTING.md](../troubleshooting/TROUBLESHOOTING.md) for detailed troubleshooting
- [docs/guides/](../guides/) for platform-specific build and setup guides 