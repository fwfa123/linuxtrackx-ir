# LinuxTrack Wine Support - Modern Installation Guide

## Overview

LinuxTrack provides head tracking support for Windows applications running under Wine. This guide covers the modern installation approach that replaces the old Wine-based extraction method with more reliable alternatives.

## Modern Installation Methods

### Method 1: Automatic Winetricks Installation (Recommended for Debian/Ubuntu/MX)

**Important**: LinuxTrack now automatically handles winetricks installation and updates to address the issue of outdated packaged versions mentioned on the [winetricks GitHub page](https://github.com/Winetricks/winetricks?tab=readme-ov-file).

The most reliable approach for Debian-based systems is to use winetricks:

```bash
# Option 1: Let LinuxTrack handle it automatically (Recommended)
# Simply run the LinuxTrack GUI and use "Install Linuxtrack-Wine support"
# The system will automatically:
# - Check if winetricks is installed
# - Download and install the latest version if needed
# - Install MFC42 via winetricks

# Option 2: Manual winetricks installation (if needed)
# Install winetricks if not already installed
sudo apt install winetricks  # Ubuntu/Debian/MX
sudo dnf install winetricks  # Fedora/RHEL
sudo pacman -S winetricks    # Arch Linux

# Option 3: Install latest winetricks manually (recommended by winetricks maintainers)
# Use the provided script to install the latest version
./scripts/install_winetricks.sh

# Install MFC42 via winetricks
winetricks mfc42
```

### Method 2: Package Manager Installation (Fedora/RHEL/Arch only)

Some distributions provide MFC42 packages:

```bash
# Fedora/RHEL/CentOS
sudo dnf install mfc42

# Arch Linux/Manjaro
sudo pacman -S mfc42

# Note: Debian/Ubuntu/MX do not have MFC42 packages in their repositories
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
   - Second: Check winetricks availability and version
   - Third: Download and install latest winetricks if needed (addresses outdated packaged versions)
   - Fourth: Try winetricks installation
   - Fifth: Try package manager installation
   - Sixth: Fall back to cabextract (old method)

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

### Winetricks Not Available or Outdated
LinuxTrack now automatically handles this issue:

1. **Automatic Installation**: The GUI will automatically download and install the latest winetricks version
2. **Manual Installation**: Use the provided script:
   ```bash
   ./scripts/install_winetricks.sh
   ```
3. **Traditional Installation**: Install via package manager (may be outdated):
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
- **Addresses Outdated Versions**: Automatically installs latest winetricks to avoid packaged version issues

## Testing Installation

After installation, test your setup:

1. Run a Windows application under Wine
2. Check if head tracking works
3. Verify `mfc42u.dll` is available in your Wine prefix

## Security Considerations

### Winetricks Installation Security

The automatic winetricks installation downloads the latest version directly from the official winetricks GitHub repository:

- **Source**: `https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks`
- **Encryption**: HTTPS download with certificate validation
- **Verification**: Optional SHA256 integrity checking (if available)
- **Trust**: Same source recommended by winetricks maintainers

### Security Features

1. **Trusted Source**: Downloads from official winetricks GitHub repository
2. **HTTPS Encryption**: All downloads use encrypted connections
3. **Temporary Files**: Downloads to secure temporary directories with automatic cleanup
4. **User Control**: Requires explicit user action to initiate installation
5. **Graceful Fallback**: Falls back to manual installation if automatic fails

### Manual Verification (Optional)

If you want to verify the download manually:

```bash
# Get the current winetricks hash
curl -s https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks | sha256sum

# Compare with downloaded file
sha256sum /usr/local/bin/winetricks
```

## Support

If you encounter issues:

1. Check the LinuxTrack log for detailed error messages
2. Verify Wine is properly installed and configured
3. Run the winetricks installation script: `./scripts/install_winetricks.sh` 