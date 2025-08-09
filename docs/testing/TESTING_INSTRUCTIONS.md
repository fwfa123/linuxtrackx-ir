# LinuxTrack Testing Instructions

## Local Git Repository Testing

Your LinuxTrack Qt5 repository is now ready for testing! The changes have been committed and pushed to your local git repository.

### 🔧 **Repository Location**

The prepared repository is available at:
```
/media/mario/Local_Git/git-repos/linuxtrack-clean-june14/ (branch: trackir2winebridge)
```

### 📋 **Testing Process for Another OS**

#### Step 1: Copy the Repository
```bash
# Since this is a local development setup, copy the prepared repository
cp -r /media/mario/Local_Git/git-repos/linuxtrack-clean-june14 linuxtrack-test
cd linuxtrack-test

# Verify you're on the prepared branch
git branch
```

#### Step 2: Quick Test Build (Automated)
```bash
# Test the one-command build process
./build.sh --deps --clean --install

# If successful, launch the application
./run_qt5_gui.sh

# For Wayland sessions (if needed)
./run_qt5_gui.sh --force-x11
```

#### Step 3: Manual Build Test (Alternative)
```bash
# Test manual build process as documented in README
autoreconf -fiv
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install
sudo ./setup.sh
```

### 🧪 **What to Test**

#### ✅ **Build System Testing**
- [ ] Dependencies install correctly on target OS
- [ ] Autotools configuration completes without errors
- [ ] Qt5 is properly detected (not Qt6)
- [ ] All libraries build successfully
- [ ] GUI application compiles and links

#### ✅ **TrackIR Hardware Testing**
- [ ] TrackIR 5 device is detected by system
- [ ] Device permissions are correct
- [ ] LinuxTrack GUI detects TrackIR without "permission errors"
- [ ] Tracking works and responds to head movement

#### ✅ **Display Server Testing**
- [ ] **X11**: Application launches normally with `./run_qt5_gui.sh`
- [ ] **Wayland**: Launcher detects Wayland and provides guidance
- [ ] **Wayland + Force X11**: `./run_qt5_gui.sh --force-x11` works without warnings

### 🎯 **Success Confirmation**

**The test is successful if:**
- Build completes with `./build.sh --deps --clean --install`
- GUI launches with `./run_qt5_gui.sh`
- TrackIR 5 is detected and shows **no permission errors**
- Head tracking works smoothly

The repository is professionally prepared and ready for public distribution once testing confirms everything works as expected! 🎮✈️🏎️


Testing noted on Kubuntu 25.10
We to encourage installing Wine first  
Do we need wine development installed?
If wine is installed they should use ./configure --prefix=/usr/local --with-wine-libs=-L/opt/wine-devel/bin
Is /use/local the best location?  Previous builds on the old app were ./configure --prefix=$HOME/linuxtrack --with-wine-libs=-L/opt/wine-devel/bin



Key Points

    It seems likely that the installation of LinuxTrack on Ubuntu 25.10 is failing due to missing 32-bit development libraries, as the build process is attempting to compile 32-bit components.
    Research suggests that installing packages like gcc-multilib and libc6-dev-i386 may resolve the issue by providing necessary headers like bits/wordsize.h and bits/libc-header-start.h.
    The evidence leans toward compatibility issues with Ubuntu 25.10, a future release, potentially affecting 32-bit support, but standard fixes for earlier versions should still apply.

Direct Answer
Overview

The log file indicates that the installation of LinuxTrack on Ubuntu 25.10 is failing due to missing header files, specifically bits/wordsize.h and bits/libc-header-start.h, during the compilation of 32-bit libraries. This suggests that the system lacks necessary 32-bit development packages.
Likely Cause

It appears that the build process is trying to compile 32-bit components (liblinuxtrack32_la-*.lo), but the required 32-bit development libraries are not installed. These headers are part of the GNU C Library (glibc), and their absence typically points to missing multilib support.

## Recommended Improvements

### Ubuntu/Debian Build Requirements

Based on testing on Kubuntu 25.10, the following packages are required before building:

#### Essential Dependencies
```bash
# Install wine first (required for wine bridge support)
sudo apt install wine

# Install 32-bit development libraries
sudo apt install gcc-multilib libc6-dev-i386

# Install other build dependencies
sudo apt install build-essential autotools-dev libtool pkg-config
sudo apt install libmxml-dev libusb-1.0-0-dev libcwiid-dev liblo-dev
sudo apt install qt5-qmake qtbase5-dev qtbase5-dev-tools
sudo apt install libv4l-dev
```

#### Wine Development Support
For full wine bridge functionality, consider installing wine development packages:
```bash
sudo apt install wine-dev
# or
sudo apt install winehq-devel
```

### Build Process Improvements Needed

1. **Configure Script Enhancement**
   - Improve wine detection logic in `configure.ac`
   - Add proper 32-bit library dependency checks
   - Better error messages for missing multilib support

2. **Documentation Updates**
   - Add distro-specific dependency installation guides
   - Clarify wine installation requirements
   - Document multilib requirements

3. **Build Script Enhancement**
   - Add dependency checking in `build.sh`
   - Automatic package installation prompts
   - Better error handling for missing 32-bit support

### Troubleshooting Ubuntu 25.10+ Issues

If you encounter the `bits/wordsize.h` or `bits/libc-header-start.h` errors:

```bash
# Install 32-bit development support
sudo apt install gcc-multilib libc6-dev-i386

# Verify 32-bit headers are available
ls /usr/include/bits/wordsize.h
ls /usr/include/bits/libc-header-start.h
```

If wine bridge support is needed but not detected:
```bash
# Verify wine installation
wine --version
which wine

# Check wine development packages
dpkg -l | grep wine
```

### Next Testing Priority

The build system needs updating to handle these Ubuntu/Debian requirements automatically. Testing should focus on:

1. Enhanced dependency detection in configure script
2. Automatic package installation prompts
3. Better error messages for missing dependencies
4. Wine bridge functionality once build succeeds

Your testing has identified critical gaps in cross-platform compatibility that will significantly improve the user experience for Ubuntu/Debian users! 🎯



After new 
32bit dependencies installed

./run_qt5_gui.sh

Dialog box popped up statingh Can't copy '~/linuxtrack-test/src/qt_gui/../share/linuxtrack/linuxtrack1.conf' to '~/.config/linuxtrack/linuxtrack1.conf'!

🚀 Starting LinuxTrack Qt5 GUI (Modernized from Qt4)
📍 Working directory: ~/linuxtrack-test
🔍 Detected: Wayland session
⚠  Qt5 GUI works better with X11. Use --force-x11 to run with XWayland
💡 To force X11 mode: ./run_qt5_gui.sh --force-x11
🔧 Library path: ~/linuxtrack-test/src/.libs
📂 Available libraries:
lrwxrwxrwx 1 mario mario      15 Jun 28 11:40 ~/linuxtrack-test/src/.libs/libltr.so -> libltr.so.0.0.0
lrwxrwxrwx 1 mario mario      15 Jun 28 11:40 ~/linuxtrack-test/src/.libs/libltr.so.0 -> libltr.so.0.0.0
-rwxrwxr-x 1 mario mario 1317656 Jun 28 11:40 ~/linuxtrack-test/src/.libs/libltr.so.0.0.0
📂 GUI directory: ~/linuxtrack-test/src/qt_gui
✨ Launching Qt5 GUI...
terminate called without an active exception
./run_qt5_gui.sh: line 45: 20008 Aborted                 (core dumped) LD_LIBRARY_PATH="$LIB_PATH:/usr/local/lib:$LD_LIBRARY_PATH" ./ltr_gui
🎯 Qt5 GUI closed.


Looking at the /usr/local/bin/
it did not include the gui but had these files
file:///usr/local/bin/wii_server
file:///usr/local/bin/osc_server
file:///usr/local/bin/ltr_server1
file:///usr/local/bin/ltr_recenter
file:///usr/local/bin/ltr_pipe
file:///usr/local/bin/ltr_extractor




To install the wine development lib that the linuxtrack build currently needs
#!/bin/bash

# Script to automate the installation of Wine development libraries on Debian-based systems
# Supports Ubuntu 25.10 (Plucky Puffin) and other Debian-based distributions
# Defaults to noble repository and handles missing development packages
# Requires sudo privileges and internet connection

# Initialize log file
LOG_FILE="/tmp/wine_install_error.log"
echo "[START] Wine installation script started at $(date)" | tee "$LOG_FILE"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Log functions
info() { echo -e "${GREEN}[INFO]${NC} $1" | tee -a "$LOG_FILE"; }
warn() { echo -e "${YELLOW}[WARN]${NC} $1" | tee -a "$LOG_FILE"; }
error() { echo -e "${RED}[ERROR]${NC} $1" | tee -a "$LOG_FILE"; exit 1; }

# Early checks
info "Checking Bash environment..."
if [ ! -x "/bin/bash" ]; then
    error "Bash not found at /bin/bash. Install it with: sudo apt install bash"
fi
info "Bash found: /bin/bash"

info "Checking script permissions..."
if [ ! -x "$0" ]; then
    warn "Script is not executable. Attempting to fix..."
    chmod +x "$0" || error "Failed to make script executable. Run: chmod +x $0"
fi
info "Script permissions OK."

info "Checking /tmp write permissions..."
if ! touch "$LOG_FILE" 2>>"$LOG_FILE"; then
    error "Cannot write to $LOG_FILE. Check /tmp permissions: sudo chmod 1777 /tmp"
fi
info "/tmp write permissions OK."

info "Environment details:"
info "Shell: $SHELL"
info "User: $USER"
info "Current directory: $PWD"
info "Script path: $0"

# Check basic commands
info "Checking basic commands..."
for cmd in echo touch sudo apt wget gpg; do
    if ! command -v "$cmd" &>/dev/null; then
        error "$cmd is required but not installed. Install it with: sudo apt install $cmd"
    fi
    info "$cmd found."
done

# Check system compatibility
info "Checking for Debian-based system..."
if ! command -v apt &>/dev/null; then
    error "This script requires a Debian-based system with apt (e.g., Ubuntu, Debian)."
fi
if [ -f /etc/os-release ]; then
    . /etc/os-release
    info "System: $ID, Version: $VERSION_CODENAME"
else
    warn "Cannot detect system details (/etc/os-release missing)."
fi

# Step 1: Clean up old WineHQ keys and repository files
info "Cleaning up old WineHQ keys and repository files..."
sudo rm -f /etc/apt/keyrings/winehq-archive* 2>>"$LOG_FILE"
sudo rm -f /etc/apt/sources.list.d/winehq-*.sources 2>>"$LOG_FILE"
info "Cleanup complete."

# Step 2: Enable 32-bit architecture
info "Enabling 32-bit architecture..."
sudo dpkg --add-architecture i386 2>>"$LOG_FILE" || warn "Failed to enable 32-bit architecture."
info "Running apt update..."
if ! sudo apt update > "$LOG_FILE.apt" 2>&1; then
    warn "apt update failed. Full output in $LOG_FILE.apt"
    cat "$LOG_FILE.apt" >> "$LOG_FILE"
    error "Failed to update package list. Check $LOG_FILE and $LOG_FILE.apt."
fi
info "Package list updated."

# Step 3: Add WineHQ repository
info "Adding WineHQ repository..."
sudo mkdir -pm755 /etc/apt/keyrings 2>>"$LOG_FILE" || error "Failed to create /etc/apt/keyrings."
info "Downloading WineHQ GPG key..."
wget -O- https://dl.winehq.org/wine-builds/winehq.key 2>>"$LOG_FILE" | sudo gpg --dearmor -o /etc/apt/keyrings/winehq-archive-keyring.gpg 2>>"$LOG_FILE" || error "Failed to download or convert WineHQ GPG key."

# Use noble repository by default
DISTRO_CODENAME="noble"
info "Using codename: $DISTRO_CODENAME (defaulting to noble for stability)"

# Add WineHQ repository
info "Adding WineHQ repository..."
echo "Types: deb
URIs: https://dl.winehq.org/wine-builds/ubuntu/
Suites: $DISTRO_CODENAME
Components: main
Architectures: amd64 i386
Signed-By: /etc/apt/keyrings/winehq-archive-keyring.gpg" | sudo tee /etc/apt/sources.list.d/winehq-$DISTRO_CODENAME.sources 2>>"$LOG_FILE" || error "Failed to add WineHQ repository."

# Update package list
info "Updating package list after adding WineHQ repository..."
if ! sudo apt update > "$LOG_FILE.apt" 2>&1; then
    warn "apt update failed. Full output in $LOG_FILE.apt"
    cat "$LOG_FILE.apt" >> "$LOG_FILE"
    error "Failed to update package list. Check $LOG_FILE and $LOG_FILE.apt."
fi
info "WineHQ repository updated."

# Step 4: Install Wine development libraries
info "Installing Wine development libraries..."
if ! sudo apt install -y --install-recommends libwine-development libwine-development:i386 2>>"$LOG_FILE"; then
    warn "libwine-development not found. Trying libwine instead..."
    if ! sudo apt install -y --install-recommends libwine libwine:i386 2>>"$LOG_FILE"; then
        error "Failed to install Wine libraries. Check $LOG_FILE."
    fi
    info "libwine installed as fallback."
else
    info "Wine development libraries installed."
fi

# Step 5: Install additional dependencies
info "Installing additional dependencies..."
sudo apt install -y gcc-multilib g++-multilib libx11-dev:i386 libfreetype6-dev:i386 2>>"$LOG_FILE" || warn "Failed to install some dependencies. Install manually if needed."

# Step 6: Check for libfaudio0
info "Checking for libfaudio0..."
if ! dpkg -l | grep -q libfaudio0; then
    info "Adding Wine OBS repository for libfaudio0..."
    sudo add-apt-repository -y 'deb http://download.opensuse.org/repositories/Emulators:/Wine:/Debian/xUbuntu_24.04/ ./' 2>>"$LOG_FILE" || warn "Failed to add Wine OBS repository."
    sudo apt update > "$LOG_FILE.apt" 2>&1 || warn "Failed to update package list for OBS repository."
    sudo apt install -y libfaudio0 libfaudio0:i386 2>>"$LOG_FILE" || warn "Failed to install libfaudio0."
fi

# Step 7: Verify installation
info "Verifying installation..."
WINE_VERSION=$(wine-development --version 2>/dev/null || wine --version 2>/dev/null || echo "Not installed")
if [ "$WINE_VERSION" != "Not installed" ]; then
    info "Wine version: $WINE_VERSION"
else
    warn "Wine runtime not installed."
fi
if dpkg -l | grep -q libwine; then
    info "Wine libraries installed:"
    dpkg -l | grep libwine | tee -a "$LOG_FILE"
else
    error "Wine libraries not found. Check $LOG_FILE."
fi

# Step 8: Install winetricks (optional)
info "Installing winetricks (optional)..."
sudo apt install -y winetricks 2>>"$LOG_FILE" || warn "Failed to install winetricks."
if command -v winetricks &>/dev/null; then
    winetricks --self-update 2>>"$LOG_FILE" || warn "Failed to update winetricks."
fi

# Step 9: Configure Wine (optional)
info "Configuring Wine (optional)..."
if command -v winecfg &>/dev/null; then
    winecfg &>/dev/null || warn "Failed to run winecfg."
else
    warn "winecfg not found. Skipping configuration."
fi

info "Installation complete! Wine libraries are ready."
info "Log file: $LOG_FILE"
info "To view log: cat $LOG_FILE"
info "To view apt output: cat $LOG_FILE.apt"
echo "For assistance, visit https://wiki.winehq.org/Ubuntu or https://forum.winehq.org."

## 🐛 Ubuntu 25.10 Testing Results Analysis

### Issues Identified:

#### 1. **Missing Qt5 Development Modules**
The build failed with:
- `Unknown module(s) in QT: help` - requires qttools5-dev
- `Unknown module(s) in QT: x11extras help` - requires libqt5x11extras5-dev

#### 2. **GUI Crash**
The Qt5 GUI crashed with `terminate called without an active exception` and created a core dump.

#### 3. **Configuration File Path Error**
Error: `"Can't copy '~/linuxtrack-test/src/qt_gui/../share/linuxtrack/linuxtrack1.conf' to '~/.config/linuxtrack/linuxtrack1.conf'!"`

## 🍷 Wine 32-bit Development Libraries

**Important:** LinuxTrack's wine bridge functionality currently requires 32-bit Wine development libraries for proper integration with Windows applications and games.

### Automated Wine Installation Script

A comprehensive bash script is included above that automates the installation of Wine development libraries on Ubuntu/Debian systems. This script:

- ✅ **Handles 32-bit architecture setup** (`dpkg --add-architecture i386`)
- ✅ **Installs WineHQ repository** with proper GPG keys
- ✅ **Installs both 64-bit and 32-bit Wine libraries** (`libwine-development`, `libwine-development:i386`)
- ✅ **Manages dependencies** including `libfaudio0` from OBS repository
- ✅ **Provides comprehensive error logging** and verification steps
- ✅ **Defaults to stable Ubuntu repository** (noble) for maximum compatibility

### Usage:
```bash
# Make the script executable (if needed)
chmod +x wine_install_script.sh

# Run the installation
./wine_install_script.sh
```

### Why 32-bit Wine Libraries Are Needed:
Many Windows applications and games that use head tracking (like flight simulators, racing games) are still 32-bit or require 32-bit compatibility layers. LinuxTrack's wine bridge provides this crucial functionality.

### Future Development Plans:
> **Note:** We plan to modernize this approach in future versions by examining how other projects like **OpenTrack** handle 64-bit wine integration. This could potentially eliminate the need for 32-bit dependencies while maintaining compatibility.

---

### 🔧 Fixes for Ubuntu 25.10:

#### Install Missing Dependencies:
```bash
# Install missing Qt5 development packages
sudo apt update
sudo apt install -y qttools5-dev libqt5x11extras5-dev qttools5-dev-tools

# Install additional dependencies that might be missing
sudo apt install -y qt5-qmake qtbase5-dev qtbase5-dev-tools
```

#### Fix Configuration Path Issue:
```bash
# Create the LinuxTrack config directory
mkdir -p ~/.config/linuxtrack

# Copy the default configuration
sudo cp /usr/local/share/linuxtrack/linuxtrack1.conf ~/.config/linuxtrack/
sudo chown $USER:$USER ~/.config/linuxtrack/linuxtrack1.conf
```

#### Rebuild After Installing Dependencies:
```bash
cd linuxtrack-test
make clean
./build.sh
```

#### Alternative: Use Qt5-specific qmake:
If the build still fails, explicitly use the Qt5 qmake:
```bash
# In src/qt_gui directory
/usr/bin/qmake-qt5 -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack" ltr_gui.pro
make

# In src/mickey directory  
/usr/bin/qmake-qt5 -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack" mickey.pro
make
```

### 🧪 Verification Steps:
1. **Check Qt5 modules availability:**
   ```bash
   pkg-config --list-all | grep -i qt5
   ```

2. **Verify library paths:**
   ```bash
   echo $LD_LIBRARY_PATH
   ldconfig -p | grep linuxtrack
   ```

3. **Test GUI launch with debug info:**
   ```bash
   cd linuxtrack-test
   export QT_DEBUG_PLUGINS=1
   ./run_qt5_gui.sh
   ```

4. **Check for missing dependencies:**
   ```bash
   ldd src/qt_gui/ltr_gui | grep "not found"
   ```

### 📝 Recommended Testing Procedure for Ubuntu 25.10:

1. **Pre-build dependency check:**
   ```bash
   sudo apt install -y build-essential autotools-dev autoconf libtool \
   pkg-config qttools5-dev libqt5x11extras5-dev qttools5-dev-tools \
   qt5-qmake qtbase5-dev qtbase5-dev-tools libusb-1.0-0-dev \
   libcwiid-dev liblo-dev libmxml-dev libv4l-dev
   ```

2. **Clean build:**
   ```bash
   make clean
   autoreconf -fiv
   ./configure
   make -j$(nproc)
   sudo make install
   ```

3. **Post-install configuration:**
   ```bash
   sudo ldconfig
   mkdir -p ~/.config/linuxtrack
   cp /usr/local/share/linuxtrack/linuxtrack1.conf ~/.config/linuxtrack/
   ```

### 🎯 Next Steps for Ubuntu Testing:
- [ ] Install missing Qt5 development packages
- [ ] Rebuild with clean environment
- [ ] Test GUI launch with proper configuration
- [ ] Verify TrackIR hardware detection
- [ ] Document any remaining issues

**Status:** Issues identified and fixes provided for Ubuntu 25.10 compatibility. Requires rebuild with proper dependencies.

---

## 📝 Final Note: Wine 32-bit Dependencies

**⚠️ Important:** Don't forget about the **Wine 32-bit development libraries installation script** included in this document! 

The comprehensive bash script provided above is **essential for wine bridge functionality** and enables LinuxTrack to work with:
- Windows flight simulators
- Racing games with head tracking
- Any Windows application requiring head tracking integration

This script handles all the complex repository setup, architecture configuration, and dependency management automatically.

**Future Development Note:** We're planning to modernize this approach by studying how projects like **OpenTrack** handle 64-bit wine integration, which may eliminate the need for 32-bit dependencies while maintaining full compatibility.

---


Run Results
TrackIR device was found, but you don't have permissions to access it.
 Please install the file 99-TIR.rules to the udev rules directory
(consult help and your distro documentation for details).
You are going to need administrator privileges to do that

Logfile
Linuxtrack version 0.99.20
[Sat 2025-06-28 12:47:35 EDT] Starting ltr_gui
[Sat 2025-06-28 12:47:35 EDT] Preferences read OK!
[Sat 2025-06-28 12:47:35 EDT] Dumping prefs:
[Sat 2025-06-28 12:47:35 EDT] [Global]
Model = NP TrackClip


[Wiimote]
Capture-device = Wiimote
Capture-device-id = Wiimote
Running-indication = 0100
Paused-indication = 0010


[TrackIR]
Capture-device = Tir
Capture-device-id = Tir
Threshold = 140
Min-blob = 4
Max-blob = 2500
Status-led-brightness = 0
Ir-led-brightness = 7
Status-signals = On


[NP TrackClip]
Model-type = Cap
Cap-X = 67
Cap-Y = 54
Cap-Z = 96
Head-Y = 160
Head-Z = 50
Active = no


[NP TrackClip Pro]
Model-type = Clip
Clip-Y1 = 40
Clip-Y2 = 110
Clip-Z1 = 30
Clip-Z2 = 50
Head-X = -100
Head-Y = -100
Head-Z = 50
Active = yes


[Face]
Model-type = Face


[Single Point Reflective]
Model-type = SinglePoint
Active = no


[Single LED]
Model-type = SinglePoint
Active = yes


[Default]
Title = Default
Pitch-enabled = Yes
Pitch-deadzone = 0.0
Pitch-left-curvature = 0.5
Pitch-right-curvature = 0.5
Pitch-sensitivity = 5.000000
Pitch-left-limit = 80.000000
Pitch-right-limit = 80.000000
Pitch-filter = 0.2
Pitch-inverted = No
Yaw-enabled = Yes
Yaw-deadzone = 0.0
Yaw-left-curvature = 0.5
Yaw-right-curvature = 0.5
Yaw-sensitivity = 5.000000
Yaw-left-limit = 130.000000
Yaw-right-limit = 130.000000
Yaw-filter = 0.2
Yaw-inverted = No
Roll-enabled = Yes
Roll-deadzone = 0.0
Roll-left-curvature = 0.5
Roll-right-curvature = 0.5
Roll-sensitivity = 1.500000
Roll-left-limit = 45.000000
Roll-right-limit = 45.000000
Roll-filter = 0.2
Roll-inverted = No
Xtranslation-enabled = Yes
Xtranslation-deadzone = 0.0
Xtranslation-left-curvature = 0.5
Xtranslation-right-curvature = 0.5
Xtranslation-sensitivity = 5.000000
Xtranslation-left-limit = 300.000000
Xtranslation-right-limit = 300.000000
Xtranslation-filter = 0.2
Xtranslation-inverted = No
Ytranslation-enabled = Yes
Ytranslation-deadzone = 0.0
Ytranslation-left-curvature = 0.5
Ytranslation-right-curvature = 0.5
Ytranslation-sensitivity = 5.000000
Ytranslation-left-limit = 300.000000
Ytranslation-right-limit = 300.000000
Ytranslation-filter = 0.2
Ytranslation-inverted = No
Ztranslation-enabled = Yes
Ztranslation-deadzone = 0.0
Ztranslation-left-curvature = 0.5
Ztranslation-right-curvature = 0.5
Ztranslation-sensitivity = 2.000000
Ztranslation-left-limit = 300.000000
Ztranslation-right-limit = 1.000000
Ztranslation-filter = 0.5
Ztranslation-inverted = No



[Sat 2025-06-28 12:47:35 EDT] ================================================
[Sat 2025-06-28 12:47:35 EDT] Preferences read OK!
[Sat 2025-06-28 12:47:35 EDT] Opening logfile viewer.
[Sat 2025-06-28 12:47:35 EDT] Going to create lock '~/.config/linuxtrack/ltr_server.lock' => 20!
[Sat 2025-06-28 12:47:35 EDT] Lock 20 success!
[Sat 2025-06-28 12:47:35 EDT] Passing the lock to protect fifo (pid 18348)!
[Sat 2025-06-28 12:47:35 EDT] Initializing axes for profile 'Default'!
[Sat 2025-06-28 12:47:35 EDT] Closing axes!
[Sat 2025-06-28 12:47:35 EDT] Initializing axes for profile 'Default'!
[Sat 2025-06-28 12:47:35 EDT] Closing axes!
[Sat 2025-06-28 12:47:35 EDT] Initializing axes for profile 'Default'!
[Sat 2025-06-28 12:47:35 EDT] Closing axes!
[Sat 2025-06-28 12:47:35 EDT] Initializing axes for profile 'Default'!
[Sat 2025-06-28 12:47:35 EDT]     WINEARCH='win32'
[Sat 2025-06-28 12:47:35 EDT] Launching wine command: 'wine --version'
[Sat 2025-06-28 12:47:35 EDT] Couldn't load library '/usr/local/lib/linuxtrack/libtir.so.0' - /usr/local/lib/linuxtrack/libtir.so.0: cannot open shared object file: No such file or directory!

[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied
[Sat 2025-06-28 12:48:03 EDT] open: Permission denied

I checked the /lib/udev/rules.d and the 99-tir rules exist.  Group and Owner are root
The user is in the plugdev group

build.sh with --install flag terminal return
/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o liblinuxtrack32_la-ipc_utils.lo `test -f 'ipc_utils.c' || echo './'`ipc_utils.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o linuxtrack.lo linuxtrack.c
ltlib.c:279:23: warning: conflicting types for 'ltr_wakeup' due to enum/integer mismatch; have 'linuxtrack_state_type(void)' [-Wenum-int-mismatch]
  279 | linuxtrack_state_type ltr_wakeup(void)
      |                       ^~~~~~~~~~
ltlib.c:33:5: note: previous declaration of 'ltr_wakeup' with type 'int(void)'
   33 | int ltr_wakeup(void);
      |     ^~~~~~~~~~
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libmacwii_la-macwii_driver.lo `test -f 'macwii_driver.c' || echo './'`macwii_driver.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -fpic -fPIC -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ipc_utils.c -o liblinuxtrack_la-ipc_utils.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -fpic -fPIC -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c utils.c -o liblinuxtrack_la-utils.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -fpic -fPIC -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ltlib.c -o liblinuxtrack_la-ltlib.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libmacwii_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ipc_utils.c  -fPIC -DPIC -o .libs/liblinuxtrack32_la-ipc_utils.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libmacwii_la-wii_driver_prefs.lo `test -f 'wii_driver_prefs.c' || echo './'`wii_driver_prefs.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o runloop.lo runloop.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c linuxtrack.c  -fPIC -DPIC -o .libs/linuxtrack.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c macwii_driver.c  -fPIC -DPIC -o .libs/libmacwii_la-macwii_driver.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libmacwii_la-runloop.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wii_driver_prefs.c  -fPIC -DPIC -o .libs/libmacwii_la-wii_driver_prefs.o
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o joy.lo joy.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/runloop.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c utils.c -o liblinuxtrack32_la-utils.o >/dev/null 2>&1
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pref_flex.cpp -o libltr_la-pref_flex.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c macwii_driver.c -o libmacwii_la-macwii_driver.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ltlib.c -o liblinuxtrack32_la-ltlib.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libwii_la-wiimote_driver.lo `test -f 'wiimote_driver.c' || echo './'`wiimote_driver.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wii_driver_prefs.c -o libmacwii_la-wii_driver_prefs.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c joy.c  -fPIC -DPIC -o .libs/joy.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c linuxtrack.c -o linuxtrack.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libmacwii_la-runloop.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o runloop.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libwii_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libwc_la-webcam_driver.lo `test -f 'webcam_driver.c' || echo './'`webcam_driver.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libwc_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wiimote_driver.c  -fPIC -DPIC -o .libs/libwii_la-wiimote_driver.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ipc_utils.c -o liblinuxtrack32_la-ipc_utils.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libfakeusb_la-fakeusb.lo `test -f 'fakeusb.c' || echo './'`fakeusb.c
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pref_bison.cpp -o libltr_la-pref_bison.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CXX   --mode=compile g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libfakeusb_la-tir_model.lo `test -f 'tir_model.cpp' || echo './'`tir_model.cpp
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c webcam_driver.c  -fPIC -DPIC -o .libs/libwc_la-webcam_driver.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libwc_la-runloop.o
/bin/bash ../libtool  --tag=CXX   --mode=compile g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libfakeusb_la-tir4_model.lo `test -f 'tir4_model.cpp' || echo './'`tir4_model.cpp
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libwii_la-runloop.o
/bin/bash ../libtool  --tag=CXX   --mode=compile g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libfakeusb_la-sn4_model.lo `test -f 'sn4_model.cpp' || echo './'`sn4_model.cpp
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c fakeusb.c  -fPIC -DPIC -o .libs/libfakeusb_la-fakeusb.o
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_model.cpp  -fPIC -DPIC -o .libs/libfakeusb_la-tir_model.o
/bin/bash ../libtool  --tag=CXX   --mode=compile g++ -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libfakeusb_la-sn3_model.lo `test -f 'sn3_model.cpp' || echo './'`sn3_model.cpp
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c wiimote_driver.c -o libwii_la-wiimote_driver.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libp3e_la-ps3eye_driver.lo `test -f 'ps3eye_driver.c' || echo './'`ps3eye_driver.c
/bin/bash ../libtool  --tag=CC   --mode=compile gcc -DHAVE_CONFIG_H -I. -I..   -D_FORTIFY_SOURCE=2 -O2   -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -c -o libp3e_la-runloop.lo `test -f 'runloop.c' || echo './'`runloop.c
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libwc_la-runloop.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libwii_la-runloop.o >/dev/null 2>&1
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c sn4_model.cpp  -fPIC -DPIC -o .libs/libfakeusb_la-sn4_model.o
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir4_model.cpp  -fPIC -DPIC -o .libs/libfakeusb_la-tir4_model.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c joy.c -o joy.o >/dev/null 2>&1
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c sn3_model.cpp  -fPIC -DPIC -o .libs/libfakeusb_la-sn3_model.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c fakeusb.c -o libfakeusb_la-fakeusb.o >/dev/null 2>&1
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c  -fPIC -DPIC -o .libs/libp3e_la-runloop.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ps3eye_driver.c  -fPIC -DPIC -o .libs/libp3e_la-ps3eye_driver.o
pushd qt_gui; qcollectiongenerator -o help.qhc ltr_gui.qhcp; popd
~/linuxtrack-test/src/qt_gui ~/linuxtrack-test/src
pushd qt_gui; qcollectiongenerator -o help.qhc ltr_gui.qhcp; popd
~/linuxtrack-test/src/qt_gui ~/linuxtrack-test/src
The "qcollectiongenerator" tool is deprecated, use "qhelpgenerator" instead.

The "qcollectiongenerator" tool is deprecated, use "qhelpgenerator" instead.

pushd mickey; qcollectiongenerator -o help.qhc mickey.qhcp; popd
Reading collection config file...
Generating help for ltr_gui.qhp...
Building up file structure...
~/linuxtrack-test/src/mickey ~/linuxtrack-test/src
Reading collection config file...
Generating help for ltr_gui.qhp...
Building up file structure...
The "qcollectiongenerator" tool is deprecated, use "qhelpgenerator" instead.

Cannot register namespace "uglyDwarf.com.linuxtrack.1.0".
~/linuxtrack-test/src
pushd mickey; qcollectiongenerator -o help.qhc mickey.qhcp; popd
Insert custom filters...
~/linuxtrack-test/src/mickey ~/linuxtrack-test/src
Insert help data for filter section (1 of 1)...
Insert files...
The "qcollectiongenerator" tool is deprecated, use "qhelpgenerator" instead.

Reading collection config file...
Generating help for mickey.qhp...
Building up file structure...
Insert custom filters...
Insert help data for filter section (1 of 1)...
Insert files...
/bin/bash ../libtool  --tag=CC   --mode=link gcc  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2   -pthread -llo -ldl  -o osc_server osc_server-osc_server.o osc_server-linuxtrack.o  
Insert contents...
Insert indices...
Documentation successfully generated.
Creating collection file...
Reading collection config file...
Generating help for mickey.qhp...
Building up file structure...
Insert custom filters...
Insert help data for filter section (1 of 1)...
Insert files...
Insert contents...
Insert indices...
Documentation successfully generated.
Creating collection file...
Cannot create tables in file ~/linuxtrack-test/src/mickey/help.qhc.
Cannot register filter Mickey 0.99.20.
~/linuxtrack-test/src
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -fpic -fPIC '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./liblt.sym"   -o liblinuxtrack.la -rpath /usr/local/lib/linuxtrack liblinuxtrack_la-ltlib.lo liblinuxtrack_la-utils.lo liblinuxtrack_la-ipc_utils.lo  
~/linuxtrack-test/src
/bin/bash ../libtool  --tag=CC   --mode=link gcc  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -Iqt_gui -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -pthread  -o ltr_extractor ltr_extractor-hashing.o ltr_extractor-digest.o ltr_extractor-game_data.o ltr_extractor-utils.o ltr_extractor-extract.o -lmxml 
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c runloop.c -o libp3e_la-runloop.o >/dev/null 2>&1
/bin/bash ../libtool  --tag=CC   --mode=link gcc  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -ldl  -o ltr_pipe ltr_pipe-ltr_pipe.o ltr_pipe-utils.o ltr_pipe-linuxtrack.o  
Insert contents...
Insert indices...
Documentation successfully generated.
Creating collection file...
~/linuxtrack-test/src
/bin/bash ../libtool  --tag=CC   --mode=link gcc '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -m32 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./liblt.sym" -m32   -o liblinuxtrack32.la -rpath /usr/local/lib32/linuxtrack liblinuxtrack32_la-ltlib.lo liblinuxtrack32_la-utils.lo liblinuxtrack32_la-ipc_utils.lo  
libtool: link: gcc -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o osc_server osc_server-osc_server.o osc_server-linuxtrack.o  -llo -ldl -pthread
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -o liblinuxtrack_int.la  linuxtrack.lo  
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c webcam_driver.c -o libwc_la-webcam_driver.o >/dev/null 2>&1
libtool: link: gcc -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -Iqt_gui -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o ltr_extractor ltr_extractor-hashing.o ltr_extractor-digest.o ltr_extractor-game_data.o ltr_extractor-utils.o ltr_extractor-extract.o  -lmxml -pthread
pushd qt_gui; /usr/bin/qmake -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack"; make; popd
~/linuxtrack-test/src/qt_gui ~/linuxtrack-test/src
pushd mickey; /usr/bin/qmake -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack"; make; popd
~/linuxtrack-test/src/mickey ~/linuxtrack-test/src
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./joy_driver.sym"   -o libjoy.la -rpath /usr/local/lib/linuxtrack runloop.lo joy.lo  
libtool: link: echo "{ global:" > .libs/liblinuxtrack.ver
libtool: link:  cat ./liblt.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/liblinuxtrack.ver
libtool: link: gcc -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o ltr_pipe ltr_pipe-ltr_pipe.o ltr_pipe-utils.o ltr_pipe-linuxtrack.o  -ldl
libtool: link:  echo "local: *; };" >> .libs/liblinuxtrack.ver
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/liblinuxtrack_la-ltlib.o .libs/liblinuxtrack_la-utils.o .libs/liblinuxtrack_la-ipc_utils.o    -g -O2 -fstack-protector   -Wl,-soname -Wl,liblinuxtrack.so.0 -Wl,-version-script -Wl,.libs/liblinuxtrack.ver -o .libs/liblinuxtrack.so.0.0.0
libtool: link: (cd ".libs" && rm -f "liblinuxtrack.so.0" && ln -s "liblinuxtrack.so.0.0.0" "liblinuxtrack.so.0")
libtool: link: echo "{ global:" > .libs/liblinuxtrack32.ver
libtool: link: (cd ".libs" && rm -f "liblinuxtrack.so" && ln -s "liblinuxtrack.so.0.0.0" "liblinuxtrack.so")
libtool: link:  cat ./liblt.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/liblinuxtrack32.ver
libtool: link: ar cr .libs/liblinuxtrack_int.a .libs/linuxtrack.o 
libtool: link: ranlib .libs/liblinuxtrack_int.a
libtool: link:  echo "local: *; };" >> .libs/liblinuxtrack32.ver
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/liblinuxtrack32_la-ltlib.o .libs/liblinuxtrack32_la-utils.o .libs/liblinuxtrack32_la-ipc_utils.o    -m32 -g -O2 -fstack-protector -m32   -Wl,-soname -Wl,liblinuxtrack32.so.0 -Wl,-version-script -Wl,.libs/liblinuxtrack32.ver -o .libs/liblinuxtrack32.so.0.0.0
libtool: link: ( cd ".libs" && rm -f "liblinuxtrack_int.la" && ln -s "../liblinuxtrack_int.la" "liblinuxtrack_int.la" )
libtool: link: ar cr .libs/liblinuxtrack.a  liblinuxtrack_la-ltlib.o liblinuxtrack_la-utils.o liblinuxtrack_la-ipc_utils.o
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c ps3eye_driver.c -o libp3e_la-ps3eye_driver.o >/dev/null 2>&1
libtool: link: ranlib .libs/liblinuxtrack.a
make[4]: Entering directory '~/linuxtrack-test/src/mickey'
/usr/lib/qt5/bin/uic mickey.ui -o ui_mickey.h
/usr/lib/qt5/bin/uic calibration.ui -o ui_calibration.h
/usr/lib/qt5/bin/uic chsettings.ui -o ui_chsettings.h
libtool: link: echo "{ global:" > .libs/libjoy.ver
/usr/lib/qt5/bin/uic ../qt_gui/logview.ui -o ui_logview.h
/usr/lib/qt5/bin/uic hotkey.ui -o ui_hotkey.h
/usr/lib/qt5/bin/uic hotkey_setup.ui -o ui_hotkey_setup.h
libtool: link:  cat ./joy_driver.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libjoy.ver
libtool: link: ( cd ".libs" && rm -f "liblinuxtrack.la" && ln -s "../liblinuxtrack.la" "liblinuxtrack.la" )
g++ -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -dM -E -o moc_predefs.h /usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/data/dummy.cpp
libtool: link: (cd ".libs" && rm -f "liblinuxtrack32.so.0" && ln -s "liblinuxtrack32.so.0.0.0" "liblinuxtrack32.so.0")
libtool: link:  echo "local: *; };" >> .libs/libjoy.ver
gcc -c -pipe -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltlib.o ../ltlib.c
gcc -c -pipe -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o math_utils.o ../math_utils.c
gcc -c -pipe -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o uinput_ifc.o uinput_ifc.c
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/runloop.o .libs/joy.o    -g -O2 -fstack-protector   -Wl,-soname -Wl,libjoy.so.0 -Wl,-version-script -Wl,.libs/libjoy.ver -o .libs/libjoy.so.0.0.0
libtool: link: (cd ".libs" && rm -f "liblinuxtrack32.so" && ln -s "liblinuxtrack32.so.0.0.0" "liblinuxtrack32.so")
gcc -c -pipe -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o piper.o piper.c
libtool: link: ar cr .libs/liblinuxtrack32.a  liblinuxtrack32_la-ltlib.o liblinuxtrack32_la-utils.o liblinuxtrack32_la-ipc_utils.o
libtool: link: ranlib .libs/liblinuxtrack32.a
libtool: link: (cd ".libs" && rm -f "libjoy.so.0" && ln -s "libjoy.so.0.0.0" "libjoy.so.0")
../ltlib.c:279:23: warning: conflicting types for ‘ltr_wakeup’ due to enum/integer mismatch; have ‘linuxtrack_state_type(void)’ [-Wenum-int-mismatch]
  279 | linuxtrack_state_type ltr_wakeup(void)
      |                       ^~~~~~~~~~
../ltlib.c:33:5: note: previous declaration of ‘ltr_wakeup’ with type ‘int(void)’
   33 | int ltr_wakeup(void);
      |     ^~~~~~~~~~
libtool: link: (cd ".libs" && rm -f "libjoy.so" && ln -s "libjoy.so.0.0.0" "libjoy.so")
libtool: link: ( cd ".libs" && rm -f "liblinuxtrack32.la" && ln -s "../liblinuxtrack32.la" "liblinuxtrack32.la" )
/usr/lib/qt5/bin/moc -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/mickey/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include keyb.h -o moc_keyb.cpp
libtool: link: ar cr .libs/libjoy.a  runloop.o joy.o
/usr/lib/qt5/bin/moc -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/mickey/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include transform.h -o moc_transform.cpp
libtool: link: ranlib .libs/libjoy.a
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o help_view.o ../qt_gui/help_view.cpp
libtool: link: ( cd ".libs" && rm -f "libjoy.la" && ln -s "../libjoy.la" "libjoy.la" )
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o help_viewer.o ../qt_gui/help_viewer.cpp
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_gui_prefs.o ../qt_gui/ltr_gui_prefs.cpp
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o keyb_x11.o keyb_x11.cpp
make[4]: Entering directory '~/linuxtrack-test/src/qt_gui'
/usr/lib/qt5/bin/uic ltr_gui.ui -o ui_ltr_gui.h
gcc -c -pipe -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o linuxtrack.o ../linuxtrack.c
/usr/lib/qt5/bin/uic dev_help.ui -o ui_dev_help.h
/usr/lib/qt5/bin/uic ltr.ui -o ui_ltr.h
/usr/lib/qt5/bin/moc -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/mickey/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include hotkey.h -o moc_hotkey.cpp
/usr/lib/qt5/bin/uic model_creation.ui -o ui_model_creation.h
/usr/lib/qt5/bin/uic scurve.ui -o ui_scurve.h
/usr/lib/qt5/bin/uic scp_form.ui -o ui_scp_form.h
/usr/lib/qt5/bin/moc -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/mickey/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include my_line_edit.h -o moc_my_line_edit.cpp
/usr/lib/qt5/bin/uic logview.ui -o ui_logview.h
/usr/lib/qt5/bin/uic wii_setup.ui -o ui_wii_setup.h
/usr/lib/qt5/bin/uic tir_setup.ui -o ui_tir_setup.h
/usr/lib/qt5/bin/uic cap_edit.ui -o ui_cap_edit.h
/usr/lib/qt5/bin/uic clip_edit.ui -o ui_clip_edit.h
/usr/lib/qt5/bin/moc -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/mickey/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include hotkey_setup_dlg.h -o moc_hotkey_setup_dlg.cpp
/usr/lib/qt5/bin/uic single_edit.ui -o ui_single_edit.h
/usr/lib/qt5/bin/uic model_edit.ui -o ui_model_edit.h
/usr/lib/qt5/bin/uic device_setup.ui -o ui_device_setup.h
/usr/lib/qt5/bin/uic profile_setup.ui -o ui_profile_setup.h
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o mouse_linux.o mouse_linux.cpp
/usr/lib/qt5/bin/uic profile_selector.ui -o ui_profile_selector.h
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c sn4_model.cpp -o libfakeusb_la-sn4_model.o >/dev/null 2>&1
/usr/lib/qt5/bin/rcc -name mickey mickey.qrc -o qrc_mickey.cpp
/usr/lib/qt5/bin/uic cap_tweaking.ui -o ui_cap_tweaking.h
/usr/lib/qt5/bin/moc -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/mickey/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ../qt_gui/help_view.h -o moc_help_view.cpp
/usr/lib/qt5/bin/uic clip_tweaking.ui -o ui_clip_tweaking.h
/usr/lib/qt5/bin/uic extractor.ui -o ui_extractor.h
/usr/lib/qt5/bin/moc -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/mickey/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ../qt_gui/help_viewer.h -o moc_help_viewer.cpp
/usr/lib/qt5/bin/uic progress.ui -o ui_progress.h
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir_model.cpp -o libfakeusb_la-tir_model.o >/dev/null 2>&1
/usr/lib/qt5/bin/uic xplugin.ui -o ui_xplugin.h
/usr/lib/qt5/bin/uic wine_warn.ui -o ui_wine_warn.h
/usr/lib/qt5/bin/uic m_ps3eye_setup.ui -o ui_m_ps3eye_setup.h
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o main.o main.cpp
/usr/lib/qt5/bin/uic m_ps3eye_ft_setup.ui -o ui_m_ps3eye_ft_setup.h
/usr/lib/qt5/bin/uic l_wc_setup.ui -o ui_l_wc_setup.h
/usr/lib/qt5/bin/uic l_wcft_setup.ui -o ui_l_wcft_setup.h
/usr/lib/qt5/bin/uic joy_setup.ui -o ui_joy_setup.h
/usr/lib/qt5/bin/moc -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/mickey/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include mickey.h -o moc_mickey.cpp
g++ -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -x c++-header -c precomp_headers.h -o ltr_gui.gch/c++
gcc -pipe -Wall -Wextra -DLTR_GUI -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -x c-header -c precomp_headers.h -o ltr_gui.gch/c
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o keyb.o keyb.cpp
/usr/lib/qt5/bin/rcc -name ltr_rc ltr_rc.qrc -o qrc_ltr_rc.cpp
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o transform.o transform.cpp
gcc -c -include ltr_gui -pipe -Wall -Wextra -DLTR_GUI -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_srv_slave.o ../ltr_srv_slave.c
gcc -c -include ltr_gui -pipe -Wall -Wextra -DLTR_GUI -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_srv_comm.o ../ltr_srv_comm.c
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c sn3_model.cpp -o libfakeusb_la-sn3_model.o >/dev/null 2>&1
gcc -c -include ltr_gui -pipe -Wall -Wextra -DLTR_GUI -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o game_data.o ../game_data.c
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o hotkey.o hotkey.cpp
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o my_line_edit.o my_line_edit.cpp
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c tir4_model.cpp -o libfakeusb_la-tir4_model.o >/dev/null 2>&1
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o hotkey_setup_dlg.o hotkey_setup_dlg.cpp
libtool: compile:  g++ -DHAVE_CONFIG_H -I. -I.. -D_FORTIFY_SOURCE=2 -O2 -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -c pref.cpp -o libltr_la-pref.o >/dev/null 2>&1
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o qrc_mickey.o qrc_mickey.cpp
gcc -c -include ltr_gui -pipe -Wall -Wextra -DLTR_GUI -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o extract.o ../extract.c
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o moc_help_view.o moc_help_view.cpp
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o moc_help_viewer.o moc_help_viewer.cpp
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o mickey.o mickey.cpp
gcc -c -include ltr_gui -pipe -Wall -Wextra -DLTR_GUI -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o digest.o ../digest.c
/bin/bash ../libtool  --tag=CXX   --mode=link g++  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols-regex '^ltr_int'     -o libltr.la -rpath /usr/local/lib/linuxtrack libltr_la-cal.lo libltr_la-list.lo libltr_la-dyn_load.lo libltr_la-math_utils.lo libltr_la-pose.lo libltr_la-pref.lo libltr_la-pref_bison.lo libltr_la-pref_flex.lo libltr_la-pref_global.lo libltr_la-utils.lo libltr_la-image_process.lo libltr_la-tracking.lo libltr_la-ltlib_int.lo libltr_la-spline.lo libltr_la-axis.lo libltr_la-wii_driver_prefs.lo libltr_la-tir_driver_prefs.lo libltr_la-wc_driver_prefs.lo libltr_la-ipc_utils.lo libltr_la-com_proc.lo libltr_la-wii_com.lo libltr_la-joy_driver_prefs.lo libltr_la-ps3_prefs.lo -lm -lpthread -ldl 
libtool: link: /usr/bin/nm -B  .libs/libltr_la-cal.o .libs/libltr_la-list.o .libs/libltr_la-dyn_load.o .libs/libltr_la-math_utils.o .libs/libltr_la-pose.o .libs/libltr_la-pref.o .libs/libltr_la-pref_bison.o .libs/libltr_la-pref_flex.o .libs/libltr_la-pref_global.o .libs/libltr_la-utils.o .libs/libltr_la-image_process.o .libs/libltr_la-tracking.o .libs/libltr_la-ltlib_int.o .libs/libltr_la-spline.o .libs/libltr_la-axis.o .libs/libltr_la-wii_driver_prefs.o .libs/libltr_la-tir_driver_prefs.o .libs/libltr_la-wc_driver_prefs.o .libs/libltr_la-ipc_utils.o .libs/libltr_la-com_proc.o .libs/libltr_la-wii_com.o .libs/libltr_la-joy_driver_prefs.o .libs/libltr_la-ps3_prefs.o   | /usr/bin/sed -n -e 's/^.*[       ]\([ABCDGIRSTW][ABCDGIRSTW]*\)[         ][      ]*\([_A-Za-z][_A-Za-z0-9]*\)$/\1 \2 \2/p' | /usr/bin/sed '/ __gnu_lto/d' | /usr/bin/sed 's/.* //' | sort | uniq > .libs/libltr.exp
libtool: link: /usr/bin/grep -E -e "^ltr_int" ".libs/libltr.exp" > ".libs/libltr.expT"
libtool: link: mv -f ".libs/libltr.expT" ".libs/libltr.exp"
libtool: link: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libltr_la-cal.o .libs/libltr_la-list.o .libs/libltr_la-dyn_load.o .libs/libltr_la-math_utils.o .libs/libltr_la-pose.o .libs/libltr_la-pref.o .libs/libltr_la-pref_bison.o .libs/libltr_la-pref_flex.o .libs/libltr_la-pref_global.o .libs/libltr_la-utils.o .libs/libltr_la-image_process.o .libs/libltr_la-tracking.o .libs/libltr_la-ltlib_int.o .libs/libltr_la-spline.o .libs/libltr_la-axis.o .libs/libltr_la-wii_driver_prefs.o .libs/libltr_la-tir_driver_prefs.o .libs/libltr_la-wc_driver_prefs.o .libs/libltr_la-ipc_utils.o .libs/libltr_la-com_proc.o .libs/libltr_la-wii_com.o .libs/libltr_la-joy_driver_prefs.o .libs/libltr_la-ps3_prefs.o   -lpthread -ldl -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector   -Wl,-soname -Wl,libltr.so.0 -Wl,-retain-symbols-file -Wl,.libs/libltr.exp -o .libs/libltr.so.0.0.0
libtool: link: (cd ".libs" && rm -f "libltr.so.0" && ln -s "libltr.so.0.0.0" "libltr.so.0")
libtool: link: (cd ".libs" && rm -f "libltr.so" && ln -s "libltr.so.0.0.0" "libltr.so")
libtool: link: ar cr .libs/libltr.a  libltr_la-cal.o libltr_la-list.o libltr_la-dyn_load.o libltr_la-math_utils.o libltr_la-pose.o libltr_la-pref.o libltr_la-pref_bison.o libltr_la-pref_flex.o libltr_la-pref_global.o libltr_la-utils.o libltr_la-image_process.o libltr_la-tracking.o libltr_la-ltlib_int.o libltr_la-spline.o libltr_la-axis.o libltr_la-wii_driver_prefs.o libltr_la-tir_driver_prefs.o libltr_la-wc_driver_prefs.o libltr_la-ipc_utils.o libltr_la-com_proc.o libltr_la-wii_com.o libltr_la-joy_driver_prefs.o libltr_la-ps3_prefs.o
libtool: link: ranlib .libs/libltr.a
libtool: link: ( cd ".libs" && rm -f "libltr.la" && ln -s "../libltr.la" "libltr.la" )
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./drivers.sym"    -o libmacwii.la -rpath /usr/local/lib/linuxtrack libmacwii_la-macwii_driver.lo libmacwii_la-runloop.lo libmacwii_la-wii_driver_prefs.lo libltr.la 
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./drivers.sym"    -o libwii.la -rpath /usr/local/lib/linuxtrack libwii_la-wiimote_driver.lo libwii_la-runloop.lo -lcwiid libltr.la 
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./webcam_driver.sym"    -o libwc.la -rpath /usr/local/lib/linuxtrack libwc_la-webcam_driver.lo libwc_la-runloop.lo libltr.la -lv4l2 
/bin/bash ../libtool  --tag=CXX   --mode=link g++  -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2      -o libfakeusb.la -rpath /usr/local/lib/linuxtrack libfakeusb_la-fakeusb.lo libfakeusb_la-tir_model.lo libfakeusb_la-tir4_model.lo libfakeusb_la-sn4_model.lo libfakeusb_la-sn3_model.lo libltr.la 
/bin/bash ../libtool  --tag=CC   --mode=link gcc  -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2  -export-symbols "./ps3eye.sym"    -o libp3e.la -rpath /usr/local/lib/linuxtrack libp3e_la-ps3eye_driver.lo libp3e_la-runloop.lo libltr.la 
/bin/bash ../libtool  --tag=CXX   --mode=link g++  -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2      -o ltr_server1 ltr_server1-ltr_srv_comm.o ltr_server1-ltr_srv_slave.o ltr_srv_master.o ltr_server1-ltr_server1.o ltr_server1-ipc_utils.o -lpthread libltr.la 
/bin/bash ../libtool  --tag=CC   --mode=link gcc  '-DLIB_PATH="/usr/local/lib/linuxtrack/"' -g -O2 -Wall -Wextra -Wformat -Wformat-security          --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2    -pthread  -o ltr_recenter ltr_recenter-ltr_recenter.o ltr_recenter-ltr_srv_comm.o -ldl liblinuxtrack.la libltr.la 
libtool: link: echo "{ global:" > .libs/libp3e.ver
libtool: link: echo "{ global:" > .libs/libwii.ver
libtool: link:  cat ./ps3eye.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libp3e.ver
libtool: link:  cat ./drivers.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libwii.ver
libtool: link: echo "{ global:" > .libs/libwc.ver
libtool: link:  cat ./webcam_driver.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libwc.ver
libtool: link:  echo "local: *; };" >> .libs/libp3e.ver
libtool: link:  echo "local: *; };" >> .libs/libwii.ver
libtool: link: echo "{ global:" > .libs/libmacwii.ver
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/libp3e_la-ps3eye_driver.o .libs/libp3e_la-runloop.o   -Wl,-rpath -Wl,~/linuxtrack-test/src/.libs -Wl,-rpath -Wl,/usr/local/lib/linuxtrack ./.libs/libltr.so  -g -O2 -fstack-protector   -Wl,-soname -Wl,libp3e.so.0 -Wl,-version-script -Wl,.libs/libp3e.ver -o .libs/libp3e.so.0.0.0
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/libwii_la-wiimote_driver.o .libs/libwii_la-runloop.o   -Wl,-rpath -Wl,~/linuxtrack-test/src/.libs -Wl,-rpath -Wl,/usr/local/lib/linuxtrack -lcwiid ./.libs/libltr.so  -g -O2 -fstack-protector   -Wl,-soname -Wl,libwii.so.0 -Wl,-version-script -Wl,.libs/libwii.ver -o .libs/libwii.so.0.0.0
libtool: link:  cat ./drivers.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libmacwii.ver
libtool: link:  echo "local: *; };" >> .libs/libwc.ver
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/libwc_la-webcam_driver.o .libs/libwc_la-runloop.o   -Wl,-rpath -Wl,~/linuxtrack-test/src/.libs -Wl,-rpath -Wl,/usr/local/lib/linuxtrack ./.libs/libltr.so -lv4l2  -g -O2 -fstack-protector   -Wl,-soname -Wl,libwc.so.0 -Wl,-version-script -Wl,.libs/libwc.ver -o .libs/libwc.so.0.0.0
libtool: link:  echo "local: *; };" >> .libs/libmacwii.ver
libtool: link:  gcc -shared  -fPIC -DPIC  .libs/libmacwii_la-macwii_driver.o .libs/libmacwii_la-runloop.o .libs/libmacwii_la-wii_driver_prefs.o   -Wl,-rpath -Wl,~/linuxtrack-test/src/.libs -Wl,-rpath -Wl,/usr/local/lib/linuxtrack ./.libs/libltr.so  -g -O2 -fstack-protector   -Wl,-soname -Wl,libmacwii.so.0 -Wl,-version-script -Wl,.libs/libmacwii.ver -o .libs/libmacwii.so.0.0.0
libtool: link: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libfakeusb_la-fakeusb.o .libs/libfakeusb_la-tir_model.o .libs/libfakeusb_la-tir4_model.o .libs/libfakeusb_la-sn4_model.o .libs/libfakeusb_la-sn3_model.o   -Wl,-rpath -Wl,~/linuxtrack-test/src/.libs -Wl,-rpath -Wl,/usr/local/lib/linuxtrack ./.libs/libltr.so -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector   -Wl,-soname -Wl,libfakeusb.so.0 -o .libs/libfakeusb.so.0.0.0
libtool: link: gcc -DLIB_PATH=\"/usr/local/lib/linuxtrack/\" -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o .libs/ltr_recenter ltr_recenter-ltr_recenter.o ltr_recenter-ltr_srv_comm.o  -ldl ./.libs/liblinuxtrack.so ./.libs/libltr.so -pthread -Wl,-rpath -Wl,/usr/local/lib/linuxtrack
libtool: link: g++ -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o .libs/ltr_server1 ltr_server1-ltr_srv_comm.o ltr_server1-ltr_srv_slave.o ltr_srv_master.o ltr_server1-ltr_server1.o ltr_server1-ipc_utils.o  -lpthread ./.libs/libltr.so -Wl,-rpath -Wl,/usr/local/lib/linuxtrack
libtool: link: (cd ".libs" && rm -f "libp3e.so.0" && ln -s "libp3e.so.0.0.0" "libp3e.so.0")
libtool: link: (cd ".libs" && rm -f "libwii.so.0" && ln -s "libwii.so.0.0.0" "libwii.so.0")
libtool: link: (cd ".libs" && rm -f "libwii.so" && ln -s "libwii.so.0.0.0" "libwii.so")
libtool: link: (cd ".libs" && rm -f "libp3e.so" && ln -s "libp3e.so.0.0.0" "libp3e.so")
libtool: link: (cd ".libs" && rm -f "libwc.so.0" && ln -s "libwc.so.0.0.0" "libwc.so.0")
libtool: link: (cd ".libs" && rm -f "libmacwii.so.0" && ln -s "libmacwii.so.0.0.0" "libmacwii.so.0")
libtool: link: (cd ".libs" && rm -f "libwc.so" && ln -s "libwc.so.0.0.0" "libwc.so")
libtool: link: (cd ".libs" && rm -f "libmacwii.so" && ln -s "libmacwii.so.0.0.0" "libmacwii.so")
libtool: link: ar cr .libs/libp3e.a  libp3e_la-ps3eye_driver.o libp3e_la-runloop.o
libtool: link: ar cr .libs/libwii.a  libwii_la-wiimote_driver.o libwii_la-runloop.o
libtool: link: ranlib .libs/libp3e.a
libtool: link: ranlib .libs/libwii.a
libtool: link: ar cr .libs/libwc.a  libwc_la-webcam_driver.o libwc_la-runloop.o
libtool: link: ar cr .libs/libmacwii.a  libmacwii_la-macwii_driver.o libmacwii_la-runloop.o libmacwii_la-wii_driver_prefs.o
libtool: link: ranlib .libs/libwc.a
libtool: link: ranlib .libs/libmacwii.a
libtool: link: ( cd ".libs" && rm -f "libwii.la" && ln -s "../libwii.la" "libwii.la" )
libtool: link: ( cd ".libs" && rm -f "libp3e.la" && ln -s "../libp3e.la" "libp3e.la" )
libtool: link: ( cd ".libs" && rm -f "libwc.la" && ln -s "../libwc.la" "libwc.la" )
libtool: link: ( cd ".libs" && rm -f "libmacwii.la" && ln -s "../libmacwii.la" "libmacwii.la" )
libtool: link: (cd ".libs" && rm -f "libfakeusb.so.0" && ln -s "libfakeusb.so.0.0.0" "libfakeusb.so.0")
libtool: link: (cd ".libs" && rm -f "libfakeusb.so" && ln -s "libfakeusb.so.0.0.0" "libfakeusb.so")
libtool: link: ar cr .libs/libfakeusb.a  libfakeusb_la-fakeusb.o libfakeusb_la-tir_model.o libfakeusb_la-tir4_model.o libfakeusb_la-sn4_model.o libfakeusb_la-sn3_model.o
libtool: link: ranlib .libs/libfakeusb.a
libtool: link: ( cd ".libs" && rm -f "libfakeusb.la" && ln -s "../libfakeusb.la" "libfakeusb.la" )
g++ -Wl,-rpath-link,/usr/lib/x86_64-linux-gnu -o mickey main.o mickey.o ltlib.o math_utils.o uinput_ifc.o piper.o keyb.o transform.o help_view.o help_viewer.o ltr_gui_prefs.o keyb_x11.o linuxtrack.o hotkey.o my_line_edit.o hotkey_setup_dlg.o mouse_linux.o qrc_mickey.o moc_help_view.o moc_help_viewer.o   -L../.libs -L/usr/local/lib/linuxtrack -lm -lltr -lX11 -ldl -Wl,-rpath,/usr/local/lib/linuxtrack /usr/lib/x86_64-linux-gnu/libQt5Help.so /usr/lib/x86_64-linux-gnu/libQt5Widgets.so /usr/lib/x86_64-linux-gnu/libQt5X11Extras.so /usr/lib/x86_64-linux-gnu/libQt5Gui.so /usr/lib/x86_64-linux-gnu/libQt5Sql.so /usr/lib/x86_64-linux-gnu/libQt5Core.so -lGL -lpthread   
make[4]: Leaving directory '~/linuxtrack-test/src/mickey'
~/linuxtrack-test/src
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o main.o main.cpp
g++ -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -dM -E -o moc_predefs.h /usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/data/dummy.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_gui_prefs.o ltr_gui_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o help_view.o help_view.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o buffering.o buffering.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o objreader.o objreader.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_srv_master.o ../ltr_srv_master.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o hashing.o hashing.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o macwebcam_info.o macwebcam_info.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o help_viewer.o help_viewer.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o webcam_info.o webcam_info.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o qrc_ltr_rc.o qrc_ltr_rc.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_gui.h -o moc_ltr_gui.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_show.h -o moc_ltr_show.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_dev_help.h -o moc_ltr_dev_help.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include tir_prefs.h -o moc_tir_prefs.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_model.h -o moc_ltr_model.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include window.h -o moc_window.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include glwidget.h -o moc_glwidget.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_profiles.h -o moc_ltr_profiles.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include log_view.h -o moc_log_view.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include ltr_state.h -o moc_ltr_state.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include scp_form.h -o moc_scp_form.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include guardian.h -o moc_guardian.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include scurve.h -o moc_scurve.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include scview.h -o moc_scview.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include wiimote_prefs.h -o moc_wiimote_prefs.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include device_setup.h -o moc_device_setup.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include plugin_install.h -o moc_plugin_install.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include tracker.h -o moc_tracker.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include profile_setup.h -o moc_profile_setup.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include profile_selector.h -o moc_profile_selector.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include xplugin.h -o moc_xplugin.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include wine_warn.h -o moc_wine_warn.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include progress.h -o moc_progress.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include extractor.h -o moc_extractor.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include downloading.h -o moc_downloading.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include wine_launcher.h -o moc_wine_launcher.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include macps3eye_prefs.h -o moc_macps3eye_prefs.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include macps3eyeft_prefs.h -o moc_macps3eyeft_prefs.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include webcam_prefs.h -o moc_webcam_prefs.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include webcam_ft_prefs.h -o moc_webcam_ft_prefs.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include joy_prefs.h -o moc_joy_prefs.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include help_view.h -o moc_help_view.cpp
/usr/lib/qt5/bin/moc -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/qt_gui/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include help_viewer.h -o moc_help_viewer.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_gui.o ltr_gui.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_show.o ltr_show.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_dev_help.o ltr_dev_help.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o tir_prefs.o tir_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_model.o ltr_model.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o window.o window.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o glwidget.o glwidget.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_profiles.o ltr_profiles.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o log_view.o log_view.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o ltr_state.o ltr_state.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o scp_form.o scp_form.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o guardian.o guardian.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o scurve.o scurve.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o scview.o scview.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o wiimote_prefs.o wiimote_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o device_setup.o device_setup.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o plugin_install.o plugin_install.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o profile_setup.o profile_setup.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o profile_selector.o profile_selector.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o xplugin.o xplugin.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o wine_warn.o wine_warn.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o progress.o progress.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o extractor.o extractor.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o downloading.o downloading.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o wine_launcher.o wine_launcher.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o macps3eye_prefs.o macps3eye_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o macps3eyeft_prefs.o macps3eyeft_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o webcam_prefs.o webcam_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o webcam_ft_prefs.o webcam_ft_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o joy_prefs.o joy_prefs.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o moc_help_view.o moc_help_view.cpp
g++ -c -include ltr_gui -pipe -Wall -Wextra -DHAVE_CONFIG_H -DHELP_BASE='"ltr_gui/"' -I /usr/local/include -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_OPENGL_LIB -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o moc_help_viewer.o moc_help_viewer.cpp
g++  -o ltr_gui main.o ltr_gui.o ltr_show.o ltr_dev_help.o ltr_gui_prefs.o tir_prefs.o help_view.o buffering.o ltr_model.o window.o glwidget.o objreader.o ltr_profiles.o log_view.o ltr_state.o scp_form.o guardian.o scurve.o scview.o wiimote_prefs.o ltr_srv_master.o device_setup.o ltr_srv_slave.o ltr_srv_comm.o plugin_install.o profile_setup.o profile_selector.o xplugin.o wine_warn.o progress.o extractor.o game_data.o hashing.o downloading.o wine_launcher.o macps3eye_prefs.o macwebcam_info.o macps3eyeft_prefs.o help_viewer.o extract.o digest.o webcam_info.o webcam_prefs.o webcam_ft_prefs.o joy_prefs.o qrc_ltr_rc.o moc_help_view.o moc_help_viewer.o   -L../.libs -L/usr/local/lib -L/usr/local/lib/linuxtrack -lm -lltr -lGLU -lmxml -Wl,-rpath,/usr/local/lib/linuxtrack -Wl,-rpath,/usr/local/lib /usr/lib/x86_64-linux-gnu/libQt5OpenGL.so /usr/lib/x86_64-linux-gnu/libQt5Help.so /usr/lib/x86_64-linux-gnu/libQt5Widgets.so /usr/lib/x86_64-linux-gnu/libQt5Gui.so /usr/lib/x86_64-linux-gnu/libQt5Network.so /usr/lib/x86_64-linux-gnu/libQt5Sql.so /usr/lib/x86_64-linux-gnu/libQt5Core.so -lGL -lpthread   
make[4]: Leaving directory '~/linuxtrack-test/src/qt_gui'
~/linuxtrack-test/src
make[3]: Leaving directory '~/linuxtrack-test/src'
make[2]: Leaving directory '~/linuxtrack-test/src'
Making all in src/mac
make[2]: Entering directory '~/linuxtrack-test/src/mac'
make[2]: Nothing to be done for 'all'.
make[2]: Leaving directory '~/linuxtrack-test/src/mac'
Making all in doc
make[2]: Entering directory '~/linuxtrack-test/doc'
make[2]: Nothing to be done for 'all'.
make[2]: Leaving directory '~/linuxtrack-test/doc'
Making all in src/osc_server_mac
make[2]: Entering directory '~/linuxtrack-test/src/osc_server_mac'
make[2]: Nothing to be done for 'all'.
make[2]: Leaving directory '~/linuxtrack-test/src/osc_server_mac'
make[2]: Entering directory '~/linuxtrack-test'
make[2]: Leaving directory '~/linuxtrack-test'
make[1]: Leaving directory '~/linuxtrack-test'
[SUCCESS] Build completed successfully!
[INFO] Installing LinuxTrack...
[sudo] password for mario: 
Making install in src
make[1]: Entering directory '~/linuxtrack-test/src'
make  install-am
make[2]: Entering directory '~/linuxtrack-test/src'
make[3]: Entering directory '~/linuxtrack-test/src'
 /usr/bin/mkdir -p '/usr/local/bin'
  /bin/bash ../libtool   --mode=install /usr/bin/install -c ltr_server1 osc_server ltr_recenter ltr_extractor ltr_pipe '/usr/local/bin'
libtool: install: /usr/bin/install -c .libs/ltr_server1 /usr/local/bin/ltr_server1
libtool: install: /usr/bin/install -c osc_server /usr/local/bin/osc_server
libtool: install: /usr/bin/install -c .libs/ltr_recenter /usr/local/bin/ltr_recenter
libtool: install: /usr/bin/install -c ltr_extractor /usr/local/bin/ltr_extractor
libtool: install: /usr/bin/install -c ltr_pipe /usr/local/bin/ltr_pipe
 /usr/bin/mkdir -p '/usr/local/lib/linuxtrack'
 /bin/bash ../libtool   --mode=install /usr/bin/install -c   libltr.la libmacwii.la liblinuxtrack.la libjoy.la libwii.la libwc.la libfakeusb.la libp3e.la '/usr/local/lib/linuxtrack'
libtool: install: /usr/bin/install -c .libs/libltr.so.0.0.0 /usr/local/lib/linuxtrack/libltr.so.0.0.0
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libltr.so.0.0.0 libltr.so.0 || { rm -f libltr.so.0 && ln -s libltr.so.0.0.0 libltr.so.0; }; })
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libltr.so.0.0.0 libltr.so || { rm -f libltr.so && ln -s libltr.so.0.0.0 libltr.so; }; })
libtool: install: /usr/bin/install -c .libs/libltr.lai /usr/local/lib/linuxtrack/libltr.la
libtool: warning: relinking 'libmacwii.la'
libtool: install: (cd ~/linuxtrack-test/src; /bin/bash "~/linuxtrack-test/libtool"  --tag CC --mode=relink gcc -I. -I./mac -I.. -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./drivers.sym -o libmacwii.la -rpath /usr/local/lib/linuxtrack libmacwii_la-macwii_driver.lo libmacwii_la-runloop.lo libmacwii_la-wii_driver_prefs.lo libltr.la )
libtool: relink: echo "{ global:" > .libs/libmacwii.ver
libtool: relink:  cat ./drivers.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libmacwii.ver
libtool: relink:  echo "local: *; };" >> .libs/libmacwii.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libmacwii_la-macwii_driver.o .libs/libmacwii_la-runloop.o .libs/libmacwii_la-wii_driver_prefs.o   -Wl,-rpath -Wl,/usr/local/lib/linuxtrack -L/usr/local/lib/linuxtrack -lltr  -g -O2 -fstack-protector   -Wl,-soname -Wl,libmacwii.so.0 -Wl,-version-script -Wl,.libs/libmacwii.ver -o .libs/libmacwii.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libmacwii.so.0.0.0T /usr/local/lib/linuxtrack/libmacwii.so.0.0.0
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libmacwii.so.0.0.0 libmacwii.so.0 || { rm -f libmacwii.so.0 && ln -s libmacwii.so.0.0.0 libmacwii.so.0; }; })
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libmacwii.so.0.0.0 libmacwii.so || { rm -f libmacwii.so && ln -s libmacwii.so.0.0.0 libmacwii.so; }; })
libtool: install: /usr/bin/install -c .libs/libmacwii.lai /usr/local/lib/linuxtrack/libmacwii.la
libtool: install: /usr/bin/install -c .libs/liblinuxtrack.so.0.0.0 /usr/local/lib/linuxtrack/liblinuxtrack.so.0.0.0
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f liblinuxtrack.so.0.0.0 liblinuxtrack.so.0 || { rm -f liblinuxtrack.so.0 && ln -s liblinuxtrack.so.0.0.0 liblinuxtrack.so.0; }; })
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f liblinuxtrack.so.0.0.0 liblinuxtrack.so || { rm -f liblinuxtrack.so && ln -s liblinuxtrack.so.0.0.0 liblinuxtrack.so; }; })
libtool: install: /usr/bin/install -c .libs/liblinuxtrack.lai /usr/local/lib/linuxtrack/liblinuxtrack.la
libtool: install: /usr/bin/install -c .libs/libjoy.so.0.0.0 /usr/local/lib/linuxtrack/libjoy.so.0.0.0
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libjoy.so.0.0.0 libjoy.so.0 || { rm -f libjoy.so.0 && ln -s libjoy.so.0.0.0 libjoy.so.0; }; })
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libjoy.so.0.0.0 libjoy.so || { rm -f libjoy.so && ln -s libjoy.so.0.0.0 libjoy.so; }; })
libtool: install: /usr/bin/install -c .libs/libjoy.lai /usr/local/lib/linuxtrack/libjoy.la
libtool: warning: relinking 'libwii.la'
libtool: install: (cd ~/linuxtrack-test/src; /bin/bash "~/linuxtrack-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./drivers.sym -o libwii.la -rpath /usr/local/lib/linuxtrack libwii_la-wiimote_driver.lo libwii_la-runloop.lo -lcwiid libltr.la )
libtool: relink: echo "{ global:" > .libs/libwii.ver
libtool: relink:  cat ./drivers.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libwii.ver
libtool: relink:  echo "local: *; };" >> .libs/libwii.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libwii_la-wiimote_driver.o .libs/libwii_la-runloop.o   -Wl,-rpath -Wl,/usr/local/lib/linuxtrack -lcwiid -L/usr/local/lib/linuxtrack -lltr  -g -O2 -fstack-protector   -Wl,-soname -Wl,libwii.so.0 -Wl,-version-script -Wl,.libs/libwii.ver -o .libs/libwii.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libwii.so.0.0.0T /usr/local/lib/linuxtrack/libwii.so.0.0.0
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libwii.so.0.0.0 libwii.so.0 || { rm -f libwii.so.0 && ln -s libwii.so.0.0.0 libwii.so.0; }; })
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libwii.so.0.0.0 libwii.so || { rm -f libwii.so && ln -s libwii.so.0.0.0 libwii.so; }; })
libtool: install: /usr/bin/install -c .libs/libwii.lai /usr/local/lib/linuxtrack/libwii.la
libtool: warning: relinking 'libwc.la'
libtool: install: (cd ~/linuxtrack-test/src; /bin/bash "~/linuxtrack-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./webcam_driver.sym -o libwc.la -rpath /usr/local/lib/linuxtrack libwc_la-webcam_driver.lo libwc_la-runloop.lo libltr.la -lv4l2 )
libtool: relink: echo "{ global:" > .libs/libwc.ver
libtool: relink:  cat ./webcam_driver.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libwc.ver
libtool: relink:  echo "local: *; };" >> .libs/libwc.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libwc_la-webcam_driver.o .libs/libwc_la-runloop.o   -Wl,-rpath -Wl,/usr/local/lib/linuxtrack -L/usr/local/lib/linuxtrack -lltr -lv4l2  -g -O2 -fstack-protector   -Wl,-soname -Wl,libwc.so.0 -Wl,-version-script -Wl,.libs/libwc.ver -o .libs/libwc.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libwc.so.0.0.0T /usr/local/lib/linuxtrack/libwc.so.0.0.0
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libwc.so.0.0.0 libwc.so.0 || { rm -f libwc.so.0 && ln -s libwc.so.0.0.0 libwc.so.0; }; })
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libwc.so.0.0.0 libwc.so || { rm -f libwc.so && ln -s libwc.so.0.0.0 libwc.so; }; })
libtool: install: /usr/bin/install -c .libs/libwc.lai /usr/local/lib/linuxtrack/libwc.la
libtool: warning: relinking 'libfakeusb.la'
libtool: install: (cd ~/linuxtrack-test/src; /bin/bash "~/linuxtrack-test/libtool"  --tag CXX --mode=relink g++ -g -O2 -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -o libfakeusb.la -rpath /usr/local/lib/linuxtrack libfakeusb_la-fakeusb.lo libfakeusb_la-tir_model.lo libfakeusb_la-tir4_model.lo libfakeusb_la-sn4_model.lo libfakeusb_la-sn3_model.lo libltr.la )
libtool: relink: g++  -fPIC -DPIC -shared -nostdlib /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/14/crtbeginS.o  .libs/libfakeusb_la-fakeusb.o .libs/libfakeusb_la-tir_model.o .libs/libfakeusb_la-tir4_model.o .libs/libfakeusb_la-sn4_model.o .libs/libfakeusb_la-sn3_model.o   -Wl,-rpath -Wl,/usr/local/lib/linuxtrack -L/usr/local/lib/linuxtrack -lltr -L/usr/lib/gcc/x86_64-linux-gnu/14 -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/14/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/14/../../.. -lstdc++ -lm -lc -lgcc_s /usr/lib/gcc/x86_64-linux-gnu/14/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/14/../../../x86_64-linux-gnu/crtn.o  -g -O2 -g -O2 -fstack-protector   -Wl,-soname -Wl,libfakeusb.so.0 -o .libs/libfakeusb.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libfakeusb.so.0.0.0T /usr/local/lib/linuxtrack/libfakeusb.so.0.0.0
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libfakeusb.so.0.0.0 libfakeusb.so.0 || { rm -f libfakeusb.so.0 && ln -s libfakeusb.so.0.0.0 libfakeusb.so.0; }; })
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libfakeusb.so.0.0.0 libfakeusb.so || { rm -f libfakeusb.so && ln -s libfakeusb.so.0.0.0 libfakeusb.so; }; })
libtool: install: /usr/bin/install -c .libs/libfakeusb.lai /usr/local/lib/linuxtrack/libfakeusb.la
libtool: warning: relinking 'libp3e.la'
libtool: install: (cd ~/linuxtrack-test/src; /bin/bash "~/linuxtrack-test/libtool"  --tag CC --mode=relink gcc -g -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -export-symbols ./ps3eye.sym -o libp3e.la -rpath /usr/local/lib/linuxtrack libp3e_la-ps3eye_driver.lo libp3e_la-runloop.lo libltr.la )
libtool: relink: echo "{ global:" > .libs/libp3e.ver
libtool: relink:  cat ./ps3eye.sym | /usr/bin/sed -e "s/\(.*\)/\1;/" >> .libs/libp3e.ver
libtool: relink:  echo "local: *; };" >> .libs/libp3e.ver
libtool: relink:  gcc -shared  -fPIC -DPIC  .libs/libp3e_la-ps3eye_driver.o .libs/libp3e_la-runloop.o   -Wl,-rpath -Wl,/usr/local/lib/linuxtrack -L/usr/local/lib/linuxtrack -lltr  -g -O2 -fstack-protector   -Wl,-soname -Wl,libp3e.so.0 -Wl,-version-script -Wl,.libs/libp3e.ver -o .libs/libp3e.so.0.0.0
libtool: install: /usr/bin/install -c .libs/libp3e.so.0.0.0T /usr/local/lib/linuxtrack/libp3e.so.0.0.0
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libp3e.so.0.0.0 libp3e.so.0 || { rm -f libp3e.so.0 && ln -s libp3e.so.0.0.0 libp3e.so.0; }; })
libtool: install: (cd /usr/local/lib/linuxtrack && { ln -s -f libp3e.so.0.0.0 libp3e.so || { rm -f libp3e.so && ln -s libp3e.so.0.0.0 libp3e.so; }; })
libtool: install: /usr/bin/install -c .libs/libp3e.lai /usr/local/lib/linuxtrack/libp3e.la
libtool: install: /usr/bin/install -c .libs/libltr.a /usr/local/lib/linuxtrack/libltr.a
libtool: install: chmod 644 /usr/local/lib/linuxtrack/libltr.a
libtool: install: ranlib /usr/local/lib/linuxtrack/libltr.a
libtool: install: /usr/bin/install -c .libs/libmacwii.a /usr/local/lib/linuxtrack/libmacwii.a
libtool: install: chmod 644 /usr/local/lib/linuxtrack/libmacwii.a
libtool: install: ranlib /usr/local/lib/linuxtrack/libmacwii.a
libtool: install: /usr/bin/install -c .libs/liblinuxtrack.a /usr/local/lib/linuxtrack/liblinuxtrack.a
libtool: install: chmod 644 /usr/local/lib/linuxtrack/liblinuxtrack.a
libtool: install: ranlib /usr/local/lib/linuxtrack/liblinuxtrack.a
libtool: install: /usr/bin/install -c .libs/libjoy.a /usr/local/lib/linuxtrack/libjoy.a
libtool: install: chmod 644 /usr/local/lib/linuxtrack/libjoy.a
libtool: install: ranlib /usr/local/lib/linuxtrack/libjoy.a
libtool: install: /usr/bin/install -c .libs/libwii.a /usr/local/lib/linuxtrack/libwii.a
libtool: install: chmod 644 /usr/local/lib/linuxtrack/libwii.a
libtool: install: ranlib /usr/local/lib/linuxtrack/libwii.a
libtool: install: /usr/bin/install -c .libs/libwc.a /usr/local/lib/linuxtrack/libwc.a
libtool: install: chmod 644 /usr/local/lib/linuxtrack/libwc.a
libtool: install: ranlib /usr/local/lib/linuxtrack/libwc.a
libtool: install: /usr/bin/install -c .libs/libfakeusb.a /usr/local/lib/linuxtrack/libfakeusb.a
libtool: install: chmod 644 /usr/local/lib/linuxtrack/libfakeusb.a
libtool: install: ranlib /usr/local/lib/linuxtrack/libfakeusb.a
libtool: install: /usr/bin/install -c .libs/libp3e.a /usr/local/lib/linuxtrack/libp3e.a
libtool: install: chmod 644 /usr/local/lib/linuxtrack/libp3e.a
libtool: install: ranlib /usr/local/lib/linuxtrack/libp3e.a
libtool: finish: PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin:/sbin" ldconfig -n /usr/local/lib/linuxtrack
----------------------------------------------------------------------
Libraries have been installed in:
   /usr/local/lib/linuxtrack

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.
----------------------------------------------------------------------
make  install-exec-hook
make[4]: Entering directory '~/linuxtrack-test/src'
pushd qt_gui;  /usr/bin/qmake -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack"; make INSTALL_ROOT= install; popd
~/linuxtrack-test/src/qt_gui ~/linuxtrack-test/src
make[5]: Entering directory '~/linuxtrack-test/src/qt_gui'
/usr/lib/qt5/bin/qmake -install qinstall -exe ltr_gui /usr/local/bin/ltr_gui
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/qt_gui/sparow_opaq.obj /usr/local/share/linuxtrack/sparow_opaq.obj
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/qt_gui/sparow_glass.obj /usr/local/share/linuxtrack/sparow_glass.obj
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/qt_gui/xm8_detail.png /usr/local/share/linuxtrack/xm8_detail.png
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/qt_gui/sources.txt /usr/local/share/linuxtrack/sources.txt
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/qt_gui/spec.txt /usr/local/share/linuxtrack/spec.txt
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/qt_gui/sphere.obj /usr/local/share/linuxtrack/sphere.obj
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/qt_gui/sphere.png /usr/local/share/linuxtrack/sphere.png
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/qt_gui/sources_mfc.txt /usr/local/share/linuxtrack/sources_mfc.txt
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/qt_gui/win7.reg /usr/local/share/linuxtrack/win7.reg
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/qt_gui/help.qhc /usr/local/share/linuxtrack/help/ltr_gui/help.qhc
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/qt_gui/help.qch /usr/local/share/linuxtrack/help/ltr_gui/help.qch
make[5]: Leaving directory '~/linuxtrack-test/src/qt_gui'
~/linuxtrack-test/src
pushd mickey;  /usr/bin/qmake -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack"; make INSTALL_ROOT= install; popd
~/linuxtrack-test/src/mickey ~/linuxtrack-test/src
make[5]: Entering directory '~/linuxtrack-test/src/mickey'
/usr/lib/qt5/bin/moc -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/mickey/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I~/linuxtrack-test/src/mickey -I~/linuxtrack-test/src -I~/linuxtrack-test/src/qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/15 -I/usr/include/c++/15/x86_64-redhat-linux -I/usr/include/c++/15/backward -I/usr/lib/gcc/x86_64-redhat-linux/15/include -I/usr/local/include -I/usr/include transform.h -o moc_transform.cpp
g++ -c -pipe -DHAVE_CONFIG_H -DHELP_BASE='"mickey/"' -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT5_OVERRIDES -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII -DQT_HELP_LIB -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I../qt_gui -I. -I.. -I../qt_gui -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtHelp -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtX11Extras -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSql -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o transform.o transform.cpp
g++ -Wl,-rpath-link,/usr/lib/x86_64-linux-gnu -o mickey main.o mickey.o ltlib.o math_utils.o uinput_ifc.o piper.o keyb.o transform.o help_view.o help_viewer.o ltr_gui_prefs.o keyb_x11.o linuxtrack.o hotkey.o my_line_edit.o hotkey_setup_dlg.o mouse_linux.o qrc_mickey.o moc_help_view.o moc_help_viewer.o   -L../.libs -L/usr/local/lib/linuxtrack -lm -lltr -lX11 -ldl -Wl,-rpath,/usr/local/lib/linuxtrack /usr/lib/x86_64-linux-gnu/libQt5Help.so /usr/lib/x86_64-linux-gnu/libQt5Widgets.so /usr/lib/x86_64-linux-gnu/libQt5X11Extras.so /usr/lib/x86_64-linux-gnu/libQt5Gui.so /usr/lib/x86_64-linux-gnu/libQt5Sql.so /usr/lib/x86_64-linux-gnu/libQt5Core.so -lGL -lpthread   
/usr/lib/qt5/bin/qmake -install qinstall -exe mickey /usr/local/bin/mickey
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/mickey/help.qhc /usr/local/share/linuxtrack/help/mickey/help.qhc
/usr/lib/qt5/bin/qmake -install qinstall ~/linuxtrack-test/src/mickey/help.qch /usr/local/share/linuxtrack/help/mickey/help.qch
make[5]: Leaving directory '~/linuxtrack-test/src/mickey'
~/linuxtrack-test/src
pushd wii_server;  /usr/bin/qmake -spec linux-g++ "LIBDIR=/usr/local/lib/linuxtrack"; make INSTALL_ROOT= install; popd
~/linuxtrack-test/src/wii_server ~/linuxtrack-test/src
make[5]: Entering directory '~/linuxtrack-test/src/wii_server'
/usr/lib/qt5/bin/uic wii_server.ui -o ui_wii_server.h
g++ -c -pipe -DHAVE_CONFIG_H -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o main.o main.cpp
g++ -pipe -DHAVE_CONFIG_H -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -dM -E -o moc_predefs.h /usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/data/dummy.cpp
/usr/lib/qt5/bin/moc -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/wii_server/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/wii_server -I~/linuxtrack-test/src/wii_server -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/14 -I/usr/include/x86_64-linux-gnu/c++/14 -I/usr/include/c++/14/backward -I/usr/lib/gcc/x86_64-linux-gnu/14/include -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include wii_server.h -o moc_wii_server.cpp
g++ -c -pipe -DHAVE_CONFIG_H -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o wii_server.o wii_server.cpp
/usr/lib/qt5/bin/moc -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB --include ~/linuxtrack-test/src/wii_server/moc_predefs.h -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -I~/linuxtrack-test/src/wii_server -I~/linuxtrack-test/src/wii_server -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I~/linuxtrack-test/src -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/14 -I/usr/include/x86_64-linux-gnu/c++/14 -I/usr/include/c++/14/backward -I/usr/lib/gcc/x86_64-linux-gnu/14/include -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include wiimote.h -o moc_wiimote.cpp
g++ -c -pipe -DHAVE_CONFIG_H -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o wiimote.o wiimote.cpp
/usr/lib/qt5/bin/rcc -name wii_server wii_server.qrc -o qrc_wii_server.cpp
g++ -c -pipe -DHAVE_CONFIG_H -fvisibility=hidden -O2 -Wall -Wextra -Wformat -Wformat-security --param ssp-buffer-size=4 -fstack-protector -D_FORTIFY_SOURCE=2 -g -Wall -Wextra -D_REENTRANT -fPIC -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -I. -I.. -I.. -I.. -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++ -o qrc_wii_server.o qrc_wii_server.cpp
g++  -o wii_server main.o wii_server.o wiimote.o qrc_wii_server.o   -L../.libs -L/usr/local/lib/linuxtrack -lm -lltr -lcwiid -Wl,-rpath,/usr/local/lib/linuxtrack /usr/lib/x86_64-linux-gnu/libQt5Widgets.so /usr/lib/x86_64-linux-gnu/libQt5Gui.so /usr/lib/x86_64-linux-gnu/libQt5Core.so -lGL -lpthread   
/usr/lib/qt5/bin/qmake -install qinstall -exe wii_server /usr/local/bin/wii_server
make[5]: Leaving directory '~/linuxtrack-test/src/wii_server'
~/linuxtrack-test/src
make[4]: Leaving directory '~/linuxtrack-test/src'
 /usr/bin/mkdir -p '/usr/local/share/applications'
 /usr/bin/install -c -m 644 linuxtrack.desktop linuxtrack-wii.desktop '/usr/local/share/applications'
 /usr/bin/mkdir -p '/usr/local/share/linuxtrack'
 /usr/bin/install -c -m 644 linuxtrack1.conf linuxtrack.c linuxtrack_hello_world.c linuxtrack_hello_world_adv.c '/usr/local/share/linuxtrack'
 /usr/bin/mkdir -p '/usr/local/share/pixmaps'
 /usr/bin/install -c -m 644 linuxtrack.svg linuxtrack.png linuxtrack.xpm linuxtrack-wii.svg linuxtrack-wii.png linuxtrack-wii.xpm '/usr/local/share/pixmaps'
 /usr/bin/mkdir -p '/usr/local/include'
 /usr/bin/install -c -m 644 linuxtrack.h '/usr/local/include'
 /usr/bin/mkdir -p '/usr/local/lib32/linuxtrack'
 /bin/bash ../libtool   --mode=install /usr/bin/install -c   liblinuxtrack32.la '/usr/local/lib32/linuxtrack'
libtool: install: /usr/bin/install -c .libs/liblinuxtrack32.so.0.0.0 /usr/local/lib32/linuxtrack/liblinuxtrack32.so.0.0.0
libtool: install: (cd /usr/local/lib32/linuxtrack && { ln -s -f liblinuxtrack32.so.0.0.0 liblinuxtrack32.so.0 || { rm -f liblinuxtrack32.so.0 && ln -s liblinuxtrack32.so.0.0.0 liblinuxtrack32.so.0; }; })
libtool: install: (cd /usr/local/lib32/linuxtrack && { ln -s -f liblinuxtrack32.so.0.0.0 liblinuxtrack32.so || { rm -f liblinuxtrack32.so && ln -s liblinuxtrack32.so.0.0.0 liblinuxtrack32.so; }; })
libtool: install: /usr/bin/install -c .libs/liblinuxtrack32.lai /usr/local/lib32/linuxtrack/liblinuxtrack32.la
libtool: install: /usr/bin/install -c .libs/liblinuxtrack32.a /usr/local/lib32/linuxtrack/liblinuxtrack32.a
libtool: install: chmod 644 /usr/local/lib32/linuxtrack/liblinuxtrack32.a
libtool: install: ranlib /usr/local/lib32/linuxtrack/liblinuxtrack32.a
libtool: finish: PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin:/sbin" ldconfig -n /usr/local/lib32/linuxtrack
----------------------------------------------------------------------
Libraries have been installed in:
   /usr/local/lib32/linuxtrack

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.
----------------------------------------------------------------------
 /usr/bin/mkdir -p '/usr/local/share/man/man1'
 /usr/bin/install -c -m 644 ltr_gui.1 ltr_server1.1 mickey.1 wii_server.1 ltr_extractor.1 ltr_pipe.1 '/usr/local/share/man/man1'
make[3]: Leaving directory '~/linuxtrack-test/src'
make[2]: Leaving directory '~/linuxtrack-test/src'
make[1]: Leaving directory '~/linuxtrack-test/src'
Making install in src/mac
make[1]: Entering directory '~/linuxtrack-test/src/mac'
make[2]: Entering directory '~/linuxtrack-test/src/mac'
make[2]: Nothing to be done for 'install-data-am'.
make[2]: Leaving directory '~/linuxtrack-test/src/mac'
make[1]: Leaving directory '~/linuxtrack-test/src/mac'
Making install in doc
make[1]: Entering directory '~/linuxtrack-test/doc'
make[2]: Entering directory '~/linuxtrack-test/doc'
make[2]: Nothing to be done for 'install-exec-am'.
 /usr/bin/mkdir -p '/usr/local/share/doc/linuxtrack'
 /usr/bin/install -c -m 644 README.ltr_pipe debug_flags.txt '/usr/local/share/doc/linuxtrack'
 /usr/bin/mkdir -p '/usr/local/share/doc/linuxtrack'
 /usr/bin/mkdir -p '/usr/local/share/doc/linuxtrack/fgfs'
 /usr/bin/install -c -m 644  fgfs/README '/usr/local/share/doc/linuxtrack/fgfs'
 /usr/bin/mkdir -p '/usr/local/share/doc/linuxtrack/fgfs/Nasal/LinuxTrack'
 /usr/bin/install -c -m 644  fgfs/Nasal/LinuxTrack/linuxtrack.nas '/usr/local/share/doc/linuxtrack/fgfs/Nasal/LinuxTrack'
 /usr/bin/mkdir -p '/usr/local/share/doc/linuxtrack/fgfs/Input/Joysticks/LinuxTrack'
 /usr/bin/install -c -m 644  fgfs/Input/Joysticks/LinuxTrack/uinput-abs.xml '/usr/local/share/doc/linuxtrack/fgfs/Input/Joysticks/LinuxTrack'
 /usr/bin/mkdir -p '/usr/local/share/doc/linuxtrack/fgfs/Protocol'
 /usr/bin/install -c -m 644  fgfs/Protocol/linuxtrack.nas.xml fgfs/Protocol/linuxtrack.xml '/usr/local/share/doc/linuxtrack/fgfs/Protocol'
make[2]: Leaving directory '~/linuxtrack-test/doc'
make[1]: Leaving directory '~/linuxtrack-test/doc'
Making install in src/osc_server_mac
make[1]: Entering directory '~/linuxtrack-test/src/osc_server_mac'
make[2]: Entering directory '~/linuxtrack-test/src/osc_server_mac'
make[2]: Nothing to be done for 'install-data-am'.
make[2]: Leaving directory '~/linuxtrack-test/src/osc_server_mac'
make[1]: Leaving directory '~/linuxtrack-test/src/osc_server_mac'
make[1]: Entering directory '~/linuxtrack-test'
make[2]: Entering directory '~/linuxtrack-test'
make[2]: Nothing to be done for 'install-exec-am'.
 /usr/bin/mkdir -p '/usr/local/share/linuxtrack'
 /usr/bin/install -c -m 644 README README.devel README.xplane '/usr/local/share/linuxtrack'
make[2]: Leaving directory '~/linuxtrack-test'
make[1]: Leaving directory '~/linuxtrack-test'
[INFO] Installing udev rules...
[SUCCESS] Udev rules installed
[INFO] Adding user to required groups...
[SUCCESS] Installation completed!
[WARNING] Please log out and log back in for group changes to take effect
[SUCCESS] Build script completed!
[INFO] 
[INFO] To run LinuxTrack GUI:
[INFO]   ./run_qt5_gui.sh
[INFO] Or after installation:
[INFO]   /usr/local/bin/ltr_gui

