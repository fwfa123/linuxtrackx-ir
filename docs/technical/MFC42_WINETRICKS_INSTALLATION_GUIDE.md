# MFC42 Winetricks Installation Guide

## Overview

This guide covers the MFC42 winetricks installation approach used in LinuxTrack. MFC42 (Microsoft Foundation Classes 4.2) libraries are required for TrackIR compatibility and are installed via winetricks for optimal reliability.

## Why MFC42?

- **TrackIR Compatibility**: MFC42 works correctly with TrackIR applications
- **Proven Reliability**: Winetricks provides consistent installation across distributions
- **User-Friendly**: Clear progress updates and multiple fallback options
- **Cross-Distribution**: Works on Ubuntu, Fedora, Arch Linux, and other distributions

## Installation Methods

### Method 1: LinuxTrack GUI (Recommended)

1. **Launch LinuxTrack GUI**
   ```bash
   ltr_gui
   ```

2. **Install TrackIR Firmware**
   - Click "Install TrackIR Firmware"
   - Follow the firmware extraction process

3. **Install MFC42 Libraries**
   - After firmware installation, you'll see a confirmation dialog
   - Click "Yes" to proceed with MFC42 installation
   - Choose "Install via winetricks mfc42 (Recommended)" from the dropdown
   - Click "Install with Winetricks"

4. **Wait for Installation**
   - Installation may take 5-10 minutes
   - Progress updates will appear every 30 seconds
   - The system may appear to hang - this is normal

### Method 2: Command Line Installation

```bash
# Install winetricks if not already installed
sudo apt install winetricks  # Ubuntu/Debian/MX
sudo dnf install winetricks  # Fedora/RHEL
sudo pacman -S winetricks    # Arch Linux

# Install MFC42 via winetricks
winetricks mfc42

# Copy MFC42 libraries to LinuxTrack firmware directory
cp ~/.wine/drive_c/windows/system32/mfc42u.dll ~/.config/linuxtrack/tir_firmware/
```

### Method 3: Alternative Installation Script

```bash
# Use the alternative installation script
cd /path/to/linuxtrack/scripts/install/
chmod +x mfc42_alternative_installers.sh
./mfc42_alternative_installers.sh install
```

### Method 4: Manual Installation

1. **Download Visual C++ 6.0 Redistributable**
   - Go to: https://download.microsoft.com/download/vc60pro/redist/6.0.8447.0/Win98Me/EN-US/vcredist.exe
   - Download the file

2. **Extract MFC42 Libraries**
   ```bash
   # Create temporary directory
   mkdir -p /tmp/mfc42_extract
   cd /tmp/mfc42_extract
   
   # Extract using Wine
   wine vcredist.exe /Q /T:C:\ /C
   
   # Find and copy mfc42u.dll
   find . -name "mfc42u.dll" -exec cp {} ~/.config/linuxtrack/tir_firmware/ \;
   ```

## Troubleshooting

### Problem: Winetricks Installation Appears to Hang

**Symptoms:**
- System appears frozen during installation
- No progress updates for several minutes

**Solutions:**
```bash
# 1. This is normal - installation can take 5-10 minutes
# 2. Check if winetricks is actually running
ps aux | grep winetricks

# 3. Monitor Wine prefix creation
ls -la /tmp/linuxtrack_mfc42_winetricks_*

# 4. Look for progress messages in LinuxTrack GUI
```

### Problem: Winetricks Not Available

**Solutions:**
```bash
# Install winetricks for your distribution
sudo apt install winetricks  # Ubuntu/Debian/MX
sudo dnf install winetricks  # Fedora/RHEL
sudo pacman -S winetricks    # Arch Linux

# Verify installation
winetricks --version
```

### Problem: MFC42 Installation Fails

**Solutions:**
```bash
# 1. Try alternative winetricks command
winetricks vcrun6

# 2. Use alternative installation script
cd /path/to/linuxtrack/scripts/install/
./mfc42_alternative_installers.sh install

# 3. Manual installation fallback
# Download Visual C++ 6.0 Redistributable manually
# Extract mfc42u.dll and copy to firmware directory
```

### Problem: MFC42 Libraries Not Found

**Solutions:**
```bash
# 1. Check if MFC42 libraries are in the correct location
ls -la ~/.config/linuxtrack/tir_firmware/mfc42u.dll

# 2. Verify file permissions
chmod 644 ~/.config/linuxtrack/tir_firmware/mfc42u.dll

# 3. Check Wine prefix for MFC42 libraries
find ~/.wine -name "*mfc42*" -type f

# 4. Copy MFC42 libraries from Wine prefix
cp ~/.wine/drive_c/windows/system32/mfc42u.dll ~/.config/linuxtrack/tir_firmware/
```

## Cross-Distribution Compatibility

### Ubuntu / Debian / MX Linux
```bash
sudo apt install winetricks
winetricks mfc42
```

### Fedora / RHEL / CentOS
```bash
sudo dnf install winetricks
winetricks mfc42
```

### Arch Linux / Manjaro
```bash
sudo pacman -S winetricks
winetricks mfc42
```

### Other Distributions
- Install winetricks using your distribution's package manager
- Use `winetricks mfc42` for installation
- If winetricks is not available, use the alternative installation script

## File Locations

### MFC42 Libraries
- **Source**: `~/.wine/drive_c/windows/system32/mfc42u.dll`
- **Destination**: `~/.config/linuxtrack/tir_firmware/mfc42u.dll`

### Temporary Files
- **Wine Prefix**: `/tmp/linuxtrack_mfc42_winetricks_XXXXXX/`
- **Cache**: `~/.cache/winetricks/`

### Configuration Files
- **LinuxTrack Config**: `~/.config/linuxtrack/`
- **Wine Config**: `~/.wine/`

## Verification

### Check MFC42 Installation
```bash
# Verify MFC42 libraries are installed
ls -la ~/.config/linuxtrack/tir_firmware/mfc42u.dll

# Check file size (should be ~995KB)
ls -lh ~/.config/linuxtrack/tir_firmware/mfc42u.dll

# Verify Wine can find MFC42 libraries
wine reg query "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall" | grep -i mfc
```

### Test TrackIR Compatibility
```bash
# Launch LinuxTrack GUI
ltr_gui

# Check Wine bridge components
ls -la ~/.wine/drive_c/windows/system32/NPClient.dll

# Test Wine bridge functionality
wine ~/.wine/drive_c/windows/system32/check_data.exe
```

## Advanced Configuration

### Custom Wine Prefix
```bash
# Use custom Wine prefix for MFC42 installation
export WINEPREFIX=~/.wine_linuxtrack
winetricks mfc42

# Copy libraries to LinuxTrack firmware directory
cp ~/.wine_linuxtrack/drive_c/windows/system32/mfc42u.dll ~/.config/linuxtrack/tir_firmware/
```

### Offline Installation
```bash
# Download Visual C++ 6.0 Redistributable manually
# Place in ~/.cache/winetricks/

# Install from local file
winetricks mfc42
```

### Multiple Wine Versions
```bash
# Use specific Wine version for MFC42 installation
WINEPREFIX=~/.wine_linuxtrack wine-stable winetricks mfc42
WINEPREFIX=~/.wine_linuxtrack wine-staging winetricks mfc42
```

## Troubleshooting Commands

### Debug Winetricks Installation
```bash
# Enable verbose winetricks output
WINETRICKS_DEBUG=1 winetricks mfc42

# Check winetricks logs
cat ~/.cache/winetricks/winetricks.log
```

### Check Wine Environment
```bash
# Verify Wine installation
wine --version

# Check Wine prefix
ls -la ~/.wine/

# Test Wine functionality
wine notepad
```

### System Diagnostics
```bash
# Check disk space
df -h /tmp

# Check available memory
free -h

# Check system load
uptime

# Monitor system resources during installation
htop
```

## Support

### Getting Help
1. **Check the troubleshooting section** above
2. **Review the main troubleshooting guide**: `docs/troubleshooting/TROUBLESHOOTING.md`
3. **Check LinuxTrack logs**: `~/.config/linuxtrack/linuxtrack.log`
4. **Enable debug mode**: `export LTR_DEBUG=1`
5. **Create bug report** with detailed information

### Community Resources
- **GitHub Issues**: https://github.com/fwfa123/linuxtrackx-ir/issues
- **Documentation**: Check the `docs/` directory
- **Wiki**: https://github.com/uglyDwarf/linuxtrack/wiki

---

**Note**: This guide covers the MFC42 winetricks installation approach. For alternative installation methods, see the main troubleshooting guide. 