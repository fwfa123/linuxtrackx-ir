# LinuxTrack Wine Support - Modern Installation Guide

## Overview

LinuxTrack provides head tracking support for Windows applications running under Wine. This guide covers the modern installation approach that replaces the old Wine-based extraction method with more reliable alternatives.

## Modern Installation Methods

### Method 1: Package Manager Installation (Recommended)

The most reliable approach is to install `mfc42u.dll` through your distribution's package manager:

#### Ubuntu/Debian/MX Linux
```bash
sudo apt update
sudo apt install libmfc42
```

#### Fedora/RHEL/CentOS
```bash
sudo dnf install mfc42
```

#### Arch Linux/Manjaro
```bash
sudo pacman -S mfc42
```

#### OpenSUSE
```bash
sudo zypper install mfc42
```

### Method 2: Winetricks Installation

If package manager installation is not available, use winetricks:

```bash
# Install winetricks if not already installed
sudo apt install winetricks  # Ubuntu/Debian/MX
sudo dnf install winetricks  # Fedora
sudo pacman -S winetricks    # Arch

# Install mfc42 in your Wine prefix
WINEPREFIX=~/.wine winetricks mfc42
```

### Method 3: Manual Installation

As a last resort, you can manually copy `mfc42u.dll` from a Windows system:

1. Copy `mfc42u.dll` from a Windows system (typically found in `C:\Windows\System32\`)
2. Place it in the LinuxTrack firmware directory:
   ```bash
   sudo cp mfc42u.dll /usr/share/linuxtrack/tir_firmware/
   sudo chmod 644 /usr/share/linuxtrack/tir_firmware/mfc42u.dll
   ```

## Installation Process

### Step 1: Install TrackIR Firmware
1. Open `ltr_gui`
2. Go to the **Misc** tab
3. Click **Reinstall TrackIR firmware**
4. Follow the prompts to download and extract TrackIR firmware

### Step 2: Install Wine Support
1. In the **Misc** tab, click **Install Linuxtrack-Wine support**
2. The system will automatically try modern installation methods:
   - First: Check for existing `mfc42u.dll`
   - Second: Try winetricks installation
   - Third: Suggest package manager installation
   - Fourth: Fall back to cabextract (old method)

### Step 3: Select Wine Prefix
1. Choose your Wine prefix (bottle) for installation
2. The LinuxTrack Wine Bridge will be installed to that prefix

## Troubleshooting

### Wine Extraction Fails
If the old Wine extraction method fails, the system will automatically try modern alternatives. You'll see a dialog with installation instructions.

### Package Manager Not Found
If your distribution doesn't have the required package, use winetricks instead:

```bash
winetricks mfc42
```

### Winetricks Not Available
Install winetricks first:
```bash
# Ubuntu/Debian/MX
sudo apt install winetricks

# Fedora
sudo dnf install winetricks

# Arch
sudo pacman -S winetricks
```

### Manual Installation Required
If all automatic methods fail, manually install `mfc42u.dll`:

1. Download from a trusted source (e.g., Windows system)
2. Copy to `/usr/share/linuxtrack/tir_firmware/`
3. Set proper permissions: `chmod 644 mfc42u.dll`

## Why Modern Approach?

The old method of extracting `mfc42u.dll` from Microsoft Visual C++ 6.0 Redistributable had several issues:

1. **Wine Compatibility**: Old installers often fail in modern Wine versions
2. **Security**: Downloading and running old installers is less secure
3. **Reliability**: Package managers provide verified, tested packages
4. **Maintenance**: Package managers handle updates automatically

## Benefits of Modern Approach

- **More Reliable**: Uses tested, verified packages
- **Better Security**: Packages are signed and verified
- **Easier Maintenance**: Automatic updates through package managers
- **Better Compatibility**: Works with modern Wine versions
- **Clearer Error Messages**: Better user guidance when issues occur

## Testing Installation

After installation, test your setup:

1. Run a Windows application under Wine
2. Check if head tracking works
3. Verify `mfc42u.dll` is available in your Wine prefix

## Support

If you encounter issues:

1. Check the LinuxTrack log for detailed error messages
2. Verify Wine is properly installed and configured
3. Ensure TrackIR firmware is installed
4. Try the alternative installation methods described above

For additional help, see the main LinuxTrack documentation or visit the project's support channels. 