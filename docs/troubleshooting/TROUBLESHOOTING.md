# LinuxTrack Troubleshooting Guide

This guide covers common installation and runtime issues with LinuxTrack Qt5.

## 📋 Table of Contents

- [Installation Issues](#installation-issues)
- [Permission Problems](#permission-problems)
- [Device Detection Issues](#device-detection-issues)
- [Wine Support Issues](#wine-support-issues)
- [Desktop Integration Problems](#desktop-integration-problems)
- [Runtime Errors](#runtime-errors)
- [Performance Issues](#performance-issues)
- [Configuration Problems](#configuration-problems)
- [Getting Help](#getting-help)

---

## 🔧 Installation Issues

### Problem: Files missing from `/opt/linuxtrack/`

**Symptoms:**
- `verify_installation.sh` reports missing files
- Applications fail to start with "file not found" errors

**Solutions:**
```bash
# 1. Ensure correct installation prefix was used
./configure --prefix=/opt/linuxtrack
make && sudo make install

# 2. Check if installation completed successfully
ls -la /opt/linuxtrack/bin/
ls -la /opt/linuxtrack/lib/linuxtrack/

# 3. Verify permissions
sudo chown -R root:root /opt/linuxtrack/
sudo chmod -R 755 /opt/linuxtrack/
```

### Problem: Build failures during compilation

**Symptoms:**
- Make fails with missing dependencies
- Qt5 related compilation errors
- OpenCV linking problems

**Solutions:**
```bash
# Install required dependencies (Ubuntu/Debian)
sudo apt update
sudo apt install qtbase5-dev qttools5-dev-tools qthelp5-dev-tools
sudo apt install libusb-1.0-0-dev libmxml-dev libopencv-dev
sudo apt install build-essential cmake libtool autoconf automake

# Install required dependencies (Fedora/RHEL)
sudo dnf install qt5-qtbase-devel qt5-qttools-devel qt5-qthelp-devel
sudo dnf install libusb1-devel mxml-devel opencv-devel
sudo dnf install gcc-c++ cmake libtool autoconf automake

# Clean and rebuild
make clean
./configure --prefix=/opt/linuxtrack
make
```

### Problem: `configure` script fails

**Symptoms:**
- "configure: command not found"
- Missing autotools errors

**Solutions:**
```bash
# Generate configure script if missing
autoreconf -fiv

# Ensure autotools are installed
sudo apt install autoconf automake libtool  # Ubuntu/Debian
sudo dnf install autoconf automake libtool  # Fedora/RHEL

# Run configure with proper prefix
./configure --prefix=/opt/linuxtrack
```

---

## 🔐 Permission Problems

### Problem: Device access denied

**Symptoms:**
- TrackIR/hardware not detected
- "Permission denied" errors in logs

**Solutions:**
```bash
# 1. Add user to required groups
sudo usermod -a -G plugdev,input $USER

# 2. Install and reload udev rules
sudo cp /opt/linuxtrack/share/linuxtrack/99-TIR.rules /lib/udev/rules.d/
sudo udevadm control --reload-rules

# 3. Log out and log back in for group changes to take effect
```

### Problem: Cannot write to configuration directory

**Symptoms:**
- Settings not saved
- "Cannot write to ~/.config/linuxtrack/" errors

**Solutions:**
```bash
# Check and fix permissions
ls -ld ~/.config/linuxtrack/
chmod 755 ~/.config/linuxtrack/
chmod 644 ~/.config/linuxtrack/*

# Recreate directory if corrupted
rm -rf ~/.config/linuxtrack/
mkdir -p ~/.config/linuxtrack/
```

---

## 🎮 Device Detection Issues

### Problem: TrackIR device not detected

**Symptoms:**
- Device not listed in GUI
- lsusb shows device but LinuxTrack doesn't see it
- "No compatible device found" messages

**Solutions:**
```bash
# 1. Verify device is recognized by system
lsusb | grep -i naturalpoint
lsusb | grep -i trackir

# 2. Check udev rules are installed and loaded
ls -la /lib/udev/rules.d/99-TIR.rules
sudo udevadm control --reload-rules
sudo udevadm trigger

# 3. Test with different USB ports
# Try USB 2.0 ports if using USB 3.0

# 4. Check for conflicting software
sudo lsof | grep usb  # Look for other software using USB devices
sudo killall -9 any_conflicting_process

# 5. Enable debug logging
export LTR_DEBUG=1
/opt/linuxtrack/bin/ltr_gui

# 6. Check kernel messages
dmesg | grep -i usb
dmesg | grep -i trackir
```

### Problem: Webcam face tracking not working

**Symptoms:**
- Camera detected but no tracking
- Poor tracking performance
- "OpenCV not found" errors

**Solutions:**
```bash
# 1. Test camera access
ls /dev/video*
v4l2-ctl --list-devices

# 2. Install OpenCV if missing
sudo apt install libopencv-dev python3-opencv  # Ubuntu/Debian
sudo dnf install opencv-devel python3-opencv   # Fedora/RHEL

# 3. Check camera permissions
sudo usermod -a -G video $USER
# Log out and log back in

# 4. Test camera functionality
cheese  # Or any camera application
vlc v4l2:///dev/video0

# 5. Verify face detection model
ls -la /opt/linuxtrack/share/linuxtrack/haarcascade_frontalface_alt2.xml

# 6. Improve lighting conditions
# Ensure good lighting on face
# Avoid backlighting
# Use consistent lighting
```

### Problem: Wiimote connection issues

**Symptoms:**
- Cannot connect to Wiimote
- Connection drops frequently
- Bluetooth pairing problems

**Solutions:**
```bash
# 1. Install Bluetooth dependencies
sudo apt install bluetooth bluez bluez-tools cwiid  # Ubuntu/Debian
sudo dnf install bluez cwiid                        # Fedora/RHEL

# 2. Enable Bluetooth service
sudo systemctl enable bluetooth
sudo systemctl start bluetooth

# 3. Pair Wiimote manually
sudo bluetoothctl
power on
discoverable on
scan on
# Press 1+2 on Wiimote, then pair when it appears

# 4. Test cwiid connection
wmgui  # Test Wiimote connection

# 5. Check for interference
# Move away from other Bluetooth devices
# Avoid 2.4GHz WiFi interference
```

---

## 🍷 Wine Support Issues

### Problem: Wine support installation fails

**Symptoms:**
- "Install Linuxtrack-Wine support" button fails
- Wine extraction errors
- mfc42u.dll not found

**Solutions:**
```bash
# 1. Check Wine installation
wine --version
which wine

# 2. Install Wine if missing
# Ubuntu/Debian/MX:
sudo apt install wine-staging wine32:i386

# Fedora/RHEL:
sudo dnf install wine-staging

# Arch Linux:
sudo pacman -S wine-staging

# 3. Try modern installation methods
# Method 1: Winetricks (Debian/Ubuntu/MX)
sudo apt install winetricks
winetricks mfc42

# Method 2: Package manager (Fedora/RHEL/Arch only)
sudo dnf install mfc42     # Fedora/RHEL
sudo pacman -S mfc42       # Arch Linux

# Method 3: Manual installation
# Copy mfc42u.dll from Windows system to:
sudo cp mfc42u.dll /usr/share/linuxtrack/tir_firmware/
sudo chmod 644 /usr/share/linuxtrack/tir_firmware/mfc42u.dll
```

### Problem: TrackIR firmware extraction fails

**Symptoms:**
- Wine crashes during extraction
- "VC6RedistSetup_deu.exe" fails to run
- Missing firmware files

**Solutions:**
```bash
# 1. Use Wine Staging instead of Wine Stable
sudo apt install wine-staging  # Ubuntu/Debian/MX
sudo update-alternatives --config wine  # Select Wine Staging

# 2. Install 32-bit Wine components
sudo apt install wine32:i386  # Ubuntu/Debian/MX

# 3. Try manual TrackIR installation
# Download TrackIR software from NaturalPoint website
# Install manually in Wine, then use "Browse Directory" in LinuxTrack GUI

# 4. Check Wine prefix
WINEPREFIX=~/.wine winecfg  # Configure Wine prefix
WINEPREFIX=~/.wine wineboot --init  # Initialize new prefix

# 5. Use alternative extraction method
# In LinuxTrack GUI, use "Extract from installer" instead of "Download"
```

### Problem: Windows applications don't detect head tracking

**Symptoms:**
- Games don't respond to head movement
- TrackIR not recognized in Windows applications
- "No TrackIR found" messages

**Solutions:**
```bash
# 1. Verify Wine Bridge installation
ls -la ~/.wine/drive_c/Program\ Files/LinuxTrack/
ls -la ~/.wine/drive_c/Program\ Files\ \(x86\)/LinuxTrack/

# 2. Check DLL files are present
ls -la ~/.wine/drive_c/windows/system32/NPClient.dll
ls -la ~/.wine/drive_c/windows/system32/mfc42u.dll

# 3. Test TrackIR compatibility
cd ~/.wine/drive_c/Program\ Files/LinuxTrack/
wine Tester.exe  # Should show tracking data

# 4. Verify LinuxTrack is running
ps aux | grep ltr_gui
ps aux | grep ltr_server

# 5. Check Wine prefix configuration
WINEPREFIX=~/.wine wine regedit
# Look for LinuxTrack registry entries

# 6. Reinstall Wine Bridge
# In LinuxTrack GUI, try "Install Linuxtrack-Wine support" again
```

### Problem: Wine Bridge components missing

**Symptoms:**
- LinuxTrack Wine Bridge not installed
- Missing NPClient.dll or other components
- Wine applications can't find TrackIR

**Solutions:**
```bash
# 1. Verify LinuxTrack installation
ls -la /usr/share/linuxtrack/wine/
ls -la /usr/share/linuxtrack/wine/linuxtrack-wine.exe

# 2. Install Wine Bridge manually
cd /usr/share/linuxtrack/wine/
WINEPREFIX=~/.wine wine linuxtrack-wine.exe

# 3. Check for multiple Wine prefixes
find ~ -name ".wine" -type d
# Install in each prefix where you need head tracking

# 4. Verify installation
ls -la ~/.wine/drive_c/windows/system32/NPClient*.dll
ls -la ~/.wine/drive_c/windows/system32/FreeTrackClient.dll

# 5. Test installation
cd ~/.wine/drive_c/Program\ Files/LinuxTrack/
wine Tester.exe
wine Tester64.exe  # For 64-bit applications
```

### Problem: Wine compatibility issues

**Symptoms:**
- Wine crashes when running LinuxTrack components
- Performance issues in Wine
- Graphics glitches

**Solutions:**
```bash
# 1. Update Wine to latest version
# Ubuntu/Debian/MX:
sudo apt update && sudo apt install wine-staging

# Fedora/RHEL:
sudo dnf update wine-staging

# Arch Linux:
sudo pacman -Syu wine-staging

# 2. Configure Wine for better performance
WINEPREFIX=~/.wine winecfg
# Graphics tab: Enable "Allow the window manager to decorate the windows"
# Graphics tab: Enable "Allow the window manager to control the windows"

# 3. Set Wine environment variables
export WINEDEBUG=-all  # Disable debug output for better performance
export WINEDLLOVERRIDES="winemenubuilder.exe=d"

# 4. Try different Wine versions
# Some applications work better with specific Wine versions
# Test with Wine Stable vs Wine Staging

# 5. Check for conflicting software
# Disable other head tracking software
# Close other Wine applications
```

---

## 🖥️ Desktop Integration Problems

### Problem: Applications not appearing in menu

**Symptoms:**
- LinuxTrack not in applications menu
- Desktop files exist but not recognized
- Icons not displaying

**Solutions:**
```bash
# 1. Verify desktop files installation
ls -la /usr/local/share/applications/linuxtrack*.desktop
ls -la /usr/local/share/applications/mickey.desktop

# 2. Update desktop database
update-desktop-database ~/.local/share/applications/
update-desktop-database /usr/local/share/applications/

# 3. Update icon cache
gtk-update-icon-cache -f -t ~/.local/share/icons/hicolor/
gtk-update-icon-cache -f -t /usr/local/share/icons/hicolor/

# 4. Check desktop file validity
desktop-file-validate /usr/local/share/applications/linuxtrack.desktop

# 5. Force menu refresh (desktop environment specific)
# GNOME: Alt+F2, type 'r', press Enter
# KDE: kbuildsycoca5 --noincremental
# XFCE: xfce4-panel --restart
```

### Problem: Wrong application icons

**Symptoms:**
- Generic icons instead of LinuxTrack icons
- Icons not found in different sizes

**Solutions:**
```bash
# 1. Verify icon installation
find /usr/local/share/icons/ -name "*linuxtrack*"
find /usr/local/share/pixmaps/ -name "*linuxtrack*"

# 2. Reinstall icons
sudo make install  # This should install icons

# 3. Check icon theme cache
gtk-update-icon-cache -f -t /usr/local/share/icons/hicolor/

# 4. Test icon resolution
gtk-launch linuxtrack  # Should show correct icon
```

---

## ⚠️ Runtime Errors

### Problem: Segmentation faults or crashes

**Symptoms:**
- Applications crash immediately
- Segfaults in logs
- "core dumped" messages

**Solutions:**
```bash
# 1. Check library dependencies
ldd /opt/linuxtrack/bin/ltr_gui
ldd /opt/linuxtrack/bin/mickey

# 2. Run with debugging
gdb /opt/linuxtrack/bin/ltr_gui
(gdb) run
# When it crashes: (gdb) bt

# 3. Check for conflicting libraries
ldconfig -p | grep qt
ldconfig -p | grep opencv

# 4. Try different Qt5 environment
export QT_QPA_PLATFORM=xcb  # Force X11
export QT_QPA_PLATFORM=wayland  # Or try Wayland

# 5. Verify clean installation
sudo rm -rf /opt/linuxtrack/
make clean && make install
```

### Problem: "Library not found" errors

**Symptoms:**
- "liblinuxtrack.so: cannot open shared object file"
- Applications won't start due to missing libraries

**Solutions:**
```bash
# 1. Add library path to ldconfig
echo "/opt/linuxtrack/lib/linuxtrack" | sudo tee /etc/ld.so.conf.d/linuxtrack.conf
sudo ldconfig

# 2. Set LD_LIBRARY_PATH temporarily
export LD_LIBRARY_PATH=/opt/linuxtrack/lib/linuxtrack:$LD_LIBRARY_PATH
/opt/linuxtrack/bin/ltr_gui

# 3. Create wrapper script
cat > ~/bin/ltr_gui << 'EOF'
#!/bin/bash
export LD_LIBRARY_PATH=/opt/linuxtrack/lib/linuxtrack:$LD_LIBRARY_PATH
exec /opt/linuxtrack/bin/ltr_gui "$@"
EOF
chmod +x ~/bin/ltr_gui
```

### Problem: Qt5 environment issues

**Symptoms:**
- "Qt platform plugin not found"
- GUI appears but doesn't function properly
- Styling/theme issues

**Solutions:**
```bash
# 1. Install Qt5 platform plugins
sudo apt install qt5-style-plugins qt5-gtk-platformtheme  # Ubuntu/Debian
sudo dnf install qt5-style-plugins                        # Fedora/RHEL

# 2. Set Qt5 environment variables
export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins
export QT_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins

# 3. Try different platforms
export QT_QPA_PLATFORM=xcb    # X11
export QT_QPA_PLATFORM=wayland # Wayland

# 4. Debug Qt5 plugin loading
export QT_DEBUG_PLUGINS=1
/opt/linuxtrack/bin/ltr_gui
```

---

## 🚀 Performance Issues

### Problem: Poor tracking performance

**Symptoms:**
- Jerky or laggy head movement
- High CPU usage
- Tracking accuracy problems

**Solutions:**
```bash
# 1. Adjust tracking settings in GUI
# - Reduce tracking frequency if CPU is overloaded
# - Adjust filtering settings
# - Optimize camera resolution for face tracking

# 2. Check system resources
top  # Look for high CPU usage
free -h  # Check available memory

# 3. Optimize camera settings (for face tracking)
# - Use 640x480 resolution instead of higher
# - Ensure good lighting
# - Reduce camera framerate if needed

# 4. Close competing applications
# - Other camera applications
# - Resource-intensive programs
# - Background video recording

# 5. Check USB bandwidth (for TrackIR)
# - Use USB 2.0 port if possible
# - Avoid USB hubs
# - Connect to root hub
```

### Problem: High latency

**Symptoms:**
- Noticeable delay between head movement and response
- Games feel unresponsive
- Motion lag

**Solutions:**
```bash
# 1. Reduce filtering in LinuxTrack settings
# - Lower smoothing values
# - Reduce response curves
# - Minimize deadzone

# 2. Check game/application settings
# - Disable VSync if possible
# - Reduce graphics quality
# - Enable "gaming mode" if available

# 3. System optimization
# - Close background applications
# - Set performance governor:
echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

# 4. Network latency (for network protocols)
# - Use localhost/127.0.0.1 when possible
# - Check network congestion
# - Use UDP instead of TCP where available
```

---

## ⚙️ Configuration Problems

### Problem: Settings not saved

**Symptoms:**
- Configuration resets every restart
- Changes don't persist
- Cannot save preferences

**Solutions:**
```bash
# 1. Check configuration directory permissions
ls -ld ~/.config/linuxtrack/
chmod 755 ~/.config/linuxtrack/
chmod 644 ~/.config/linuxtrack/*.conf

# 2. Verify configuration files exist
ls -la ~/.config/linuxtrack/
cat ~/.config/linuxtrack/linuxtrack1.conf

# 3. Test with clean configuration
mv ~/.config/linuxtrack ~/.config/linuxtrack.backup
/opt/linuxtrack/bin/ltr_gui  # This should create new config

# 4. Check disk space
df -h ~/.config/
```

### Problem: Profile/device configuration issues

**Symptoms:**
- Device settings not working
- Tracking profiles don't apply
- Calibration problems

**Solutions:**
```bash
# 1. Reset to default configuration
cp /opt/linuxtrack/share/linuxtrack/linuxtrack1.conf ~/.config/linuxtrack/

# 2. Manual configuration editing
# Edit ~/.config/linuxtrack/linuxtrack1.conf
# Check syntax and section names

# 3. Verify device-specific settings
# TrackIR: Check LED brightness, status signals
# Webcam: Verify camera device ID
# Wiimote: Check Bluetooth connection

# 4. Recalibrate device
# Use calibration wizard in GUI
# Ensure proper setup environment
# Follow on-screen instructions carefully
```

---

## 🆘 Getting Help

### Collecting Debug Information

```bash
# Run installation verification
/opt/linuxtrack/bin/verify_installation.sh > verification.txt

# Check device information
lsusb > device_info.txt

# Enable debug mode
export LTR_DEBUG=1
/opt/linuxtrack/bin/ltr_gui 2>&1 | tee debug.log
```

For complete troubleshooting information, see the full guide at:
https://github.com/uglyDwarf/linuxtrack/wiki/Troubleshooting

### Log File Locations

- User configuration: `~/.config/linuxtrack/`
- Application logs: Check GUI log viewer or `~/.config/linuxtrack/linuxtrack.log`
- System logs: `journalctl -u linuxtrack` (if using systemd service)
- udev logs: `udevadm monitor` (real-time device events)

### Enable Debug Mode

```bash
# Enable verbose logging
export LTR_DEBUG=1
export QT_LOGGING_RULES="*=true"
/opt/linuxtrack/bin/ltr_gui 2>&1 | tee linuxtrack_debug.log
```

### Community Resources

- **GitHub Issues:** https://github.com/uglyDwarf/linuxtrack/issues
- **Wiki:** https://github.com/uglyDwarf/linuxtrack/wiki
- **Discussions:** Search existing issues before creating new ones
- **Documentation:** Check `/opt/linuxtrack/share/doc/` if available

### Creating Bug Reports

1. **Search existing issues first**
2. **Use descriptive titles**
3. **Include system information**
4. **Attach verification output**
5. **Describe steps to reproduce**
6. **Include relevant log files**
7. **Mention any recent system changes**

---

## 🔄 Quick Fixes Checklist

When LinuxTrack isn't working, try these in order:

1. **Basic checks:**
   - [ ] Run `verify_installation.sh`
   - [ ] Check device is connected
   - [ ] Verify user is in `plugdev` group
   - [ ] Test with different USB port

2. **Permission fixes:**
   - [ ] `sudo udevadm control --reload-rules`
   - [ ] Log out and log back in
   - [ ] Check `/lib/udev/rules.d/99-TIR.rules` exists

3. **Configuration reset:**
   - [ ] Backup `~/.config/linuxtrack/`
   - [ ] Delete `~/.config/linuxtrack/`
   - [ ] Restart LinuxTrack

4. **Clean reinstall:**
   - [ ] `sudo make uninstall`
   - [ ] `make clean`
   - [ ] `make && sudo make install`
   - [ ] Run verification script

5. **System level:**
   - [ ] Reboot system
   - [ ] Check for system updates
   - [ ] Verify hardware functionality

This troubleshooting guide should resolve most common LinuxTrack issues. If problems persist, collect debug information and seek community help. 