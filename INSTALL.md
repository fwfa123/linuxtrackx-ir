# LinuxTrack X-IR Installation Guide

## For Windows Users New to Linux 🐧

Welcome! This guide is designed to make installing LinuxTrack X-IR as simple as possible, especially if you're new to Linux.

## Quick Start (3 Steps)

### Step 1: Install Dependencies
Copy and paste ONE of these commands based on your Linux distribution:

**MX Linux / Ubuntu / Debian:**
```bash
sudo apt install -y build-essential autoconf automake libtool qtbase5-dev qttools5-dev-tools qttools5-dev libqt5x11extras5-dev libopencv-dev libusb-1.0-0-dev libmxml-dev libx11-dev libxrandr-dev mingw-w64 bison flex nsis gcc-multilib libc6-dev-i386 libv4l-dev wine-staging wine32:i386
```

**Fedora / RHEL / CentOS:**
```bash
sudo dnf install -y gcc gcc-c++ make autoconf automake libtool qt5-qtbase-devel qt5-qttools-devel qttools5-dev qt5-qtx11extras-devel opencv-devel libusb1-devel libmxml-devel libX11-devel libXrandr-devel mingw64-gcc mingw64-gcc-c++ bison flex nsis glibc-devel.i686 libstdc++-devel.i686 v4l-utils-devel wine-staging
```

**Arch Linux / Manjaro:**
```bash
sudo pacman -S --needed base-devel autoconf automake libtool qt5-base qt5-tools qt5-x11extras qt5-help opencv libusb libmxml libx11 libxrandr mingw-w64-gcc bison flex nsis lib32-glibc lib32-gcc-libs v4l-utils wine-staging
```

### Step 2: Build and Install
```bash
# Download and build
git clone https://github.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
autoreconf -fiv
./configure --prefix=/opt
make -j$(nproc)
sudo make install

# Add your user to required group
sudo usermod -a -G plugdev $USER
```

### Step 3: Test Installation
```bash
# Log out and back in (for group changes to take effect)
# Then run:
ltr_gui
```

## What's Different from Windows?

| Windows Concept | Linux Equivalent | What You Need to Know |
|----------------|------------------|----------------------|
| `.exe installer` | `package manager` | Use `apt`, `dnf`, or `pacman` instead of downloading installers |
| `Program Files` | `/opt` or `/usr/local` | LinuxTrack installs to `/opt` by default |
| `Device drivers` | `udev rules` | Your user needs to be in the `plugdev` group |
| `TrackIR software` | `Wine + TrackIR` | We use Wine to run the Windows TrackIR installer |

## Troubleshooting

### Wine Issues (Most Common)
If TrackIR firmware extraction fails:

1. **Check your Wine setup:**
   ```bash
   ./scripts/wine_check.sh
   ```

2. **If Wine Staging isn't active:**
   ```bash
   sudo update-alternatives --config wine
   # Select Wine Staging from the menu
   ```

3. **If 32-bit components are missing:**
   ```bash
   sudo apt install wine32:i386  # Debian/Ubuntu/MX
   ```

### Permission Issues
If you get "permission denied" errors:
```bash
# Add yourself to the required group
sudo usermod -a -G plugdev $USER

# Log out and back in, then try again
```

### Build Issues
If the build fails:
```bash
# Clean and try again
make distclean
autoreconf -fiv
./configure --prefix=/opt
make -j$(nproc)
```

## TrackIR Setup

1. **Launch the GUI:**
   ```bash
   ltr_gui
   ```

2. **Extract TrackIR firmware:**
   - Click "Install Firmware" 
   - Select your TrackIR installer (.exe file)
   - Wait for extraction to complete

3. **Configure tracking:**
   - Select your tracking device
   - Calibrate tracking points
   - Test tracking functionality

## Common Questions

**Q: Why do I need Wine?**
A: TrackIR firmware is distributed as a Windows installer. Wine lets us run it on Linux.

**Q: What's the difference between Wine and Wine Staging?**
A: Wine Staging has newer features and better compatibility with Windows installers.

**Q: Do I need to install TrackIR software separately?**
A: No! The LinuxTrack GUI extracts the necessary files from the TrackIR installer automatically.

**Q: Will this work with my games?**
A: Yes! LinuxTrack works with any game that supports TrackIR protocol (most flight/racing simulators).

## Getting Help

- **Check the main README**: More detailed technical information
- **Run the Wine check script**: `./scripts/wine_check.sh`
- **Check the troubleshooting section**: In the main README
- **Community support**: Open an issue on GitHub

## Success!

Once you see the LinuxTrack GUI and can extract TrackIR firmware, you're all set! Your TrackIR device should work just like it did on Windows, but now you're running Linux. 🎉

---

*This guide is designed to be simple and user-friendly. If you need more technical details, see the main README.md file.* 