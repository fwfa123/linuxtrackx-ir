# LinuxTrack Wine Support - Modern MFC140 Installation Guide

## Overview

LinuxTrack provides head tracking support for Windows applications running under Wine. This guide covers the modern MFC140 installation approach that replaces the old MFC42/winetricks method with a more reliable and copyright-free solution.

## Modern MFC140 Installation Methods

### Method 1: Built-in MFC140 Installer (Recommended)

**Important**: LinuxTrack now uses modern Visual C++ 2015-2022 MFC libraries (MFC140) instead of legacy MFC42, eliminating copyright issues and improving compatibility.

The most reliable approach is to use the built-in MFC140 installer:

```bash
# Option 1: Use the built-in MFC140 installer (Recommended)
# Simply run the LinuxTrack GUI and use "Install Linuxtrack-Wine support"
# The system will automatically:
# - Download Visual C++ 2015-2022 Redistributable
# - Extract MFC140 libraries
# - Install mfc140u.dll to the correct location

# Option 2: Manual installation (if needed)
# Download Visual C++ 2015-2022 Redistributable manually:
# - Go to: https://aka.ms/vs/17/release/vc_redist.x64.exe
# - Download and run the installer in Wine
# - Extract mfc140u.dll from the installation

# Option 3: Browse for existing installer
# If you already have the Visual C++ 2015-2022 Redistributable:
# - Use the "Browse for Installer" option in the GUI
# - Select your existing installer file
```

### Method 2: Package Manager Installation (Legacy - Not Recommended)

Some distributions provide MFC42 packages, but these are not recommended due to copyright issues:

```bash
# Fedora/RHEL/CentOS (Legacy - Not Recommended)
sudo dnf install mfc42

# Arch Linux/Manjaro (Legacy - Not Recommended)
sudo pacman -S mfc42

# Note: These packages use legacy MFC42 libraries with copyright issues
# Use the built-in MFC140 installer instead
```

### Method 3: Manual Installation (Legacy - Not Recommended)

As a last resort, you can manually copy `mfc140u.dll` from a Windows system:

1. Download Visual C++ 2015-2022 Redistributable from Microsoft
2. Extract `mfc140u.dll` from the installation
3. Place it in the LinuxTrack firmware directory:
   ```bash
   sudo cp mfc140u.dll /usr/share/linuxtrack/tir_firmware/
   sudo chmod 644 /usr/share/linuxtrack/tir_firmware/mfc140u.dll
   ```

## Installation Process

### Step 1: Install TrackIR Firmware
1. Open `ltr_gui`
2. Go to the **Misc** tab
3. Click **Reinstall TrackIR firmware**
4. Follow the prompts to download and extract TrackIR firmware

### Step 2: Install Wine Support
1. In the **Misc** tab, click **Install Linuxtrack-Wine support**
2. The system will automatically try modern MFC140 installation methods:
   - First: Check for existing `mfc140u.dll`
   - Second: Download Visual C++ 2015-2022 Redistributable
   - Third: Extract MFC140 libraries using Wine
   - Fourth: Install `mfc140u.dll` to the correct location
   - Fifth: Fall back to manual installation if needed

### Step 3: Select Wine Prefix
1. Choose your Wine prefix (bottle) for installation
2. The LinuxTrack Wine Bridge will be installed to that prefix

## Troubleshooting

### MFC140 Installation Fails
If the MFC140 installation fails, the system will automatically try alternative methods. You'll see a dialog with installation instructions.

### Download Fails
If the automatic download fails, try manual installation:

1. **Download manually**: Go to https://aka.ms/vs/17/release/vc_redist.x64.exe
2. **Use browse option**: Select the downloaded file in the GUI
3. **Check network**: Ensure you have internet access for downloads

### Wine Issues
If Wine extraction fails, try these steps:

1. **Update Wine**: Ensure you have a recent version of Wine
2. **Check Wine prefix**: Verify your Wine prefix is working
3. **Manual extraction**: Use the browse options to select existing files
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