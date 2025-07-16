# NSIS Installation Script for Arch Linux

## Overview

The `install_nsis_arch.sh` script helps install NSIS (Nullsoft Scriptable Install System) on Arch Linux systems when the AUR package has issues. This script is specifically designed to handle the common problems users face when trying to install NSIS on Arch Linux for building LinuxTrack X-IR Windows compatibility components.

## Why This Script is Needed

The NSIS package in the AUR (Arch User Repository) can sometimes have issues:
- Build failures due to dependency conflicts
- Outdated package versions
- Missing build dependencies
- AUR helper compatibility issues

This script provides multiple fallback methods to ensure NSIS can be installed successfully.

## Features

- **Multiple Installation Methods**: Tries AUR, alternative package managers, and manual installation
- **Automatic yay Installation**: Installs yay if not present
- **Build Dependency Management**: Automatically installs required build tools
- **Installation Verification**: Tests NSIS functionality after installation
- **Colored Output**: User-friendly colored status messages
- **Error Handling**: Comprehensive error checking and recovery

## Usage

### Basic Installation
```bash
./scripts/install/install_nsis_arch.sh
```

### Command Line Options
```bash
./scripts/install/install_nsis_arch.sh [OPTIONS]
```

**Available Options:**
- `-h, --help` - Show help message
- `-f, --force` - Force reinstallation even if NSIS is already installed
- `-v, --verify` - Only verify existing installation
- `-m, --manual` - Skip AUR and install manually from source

### Examples

**Check if NSIS is installed:**
```bash
./scripts/install/install_nsis_arch.sh --verify
```

**Force reinstall NSIS:**
```bash
./scripts/install/install_nsis_arch.sh --force
```

**Install manually from source (skip AUR):**
```bash
./scripts/install/install_nsis_arch.sh --manual
```

## Installation Methods

The script tries installation methods in this order:

1. **AUR Installation via yay** (Primary method)
   - Uses `yay -S nsis --noconfirm`
   - Automatically installs yay if not present

2. **Alternative Package Managers**
   - Tries `pacman` (in case NSIS is in main repos)
   - Tries `paru` if available

3. **Manual Installation from Source**
   - Downloads NSIS source from SourceForge
   - Builds and installs manually
   - Installs required build dependencies

## Prerequisites

- Arch Linux system (script checks for `/etc/arch-release`)
- Internet connection for downloading packages and source code
- sudo privileges for package installation
- Basic build tools (git, base-devel)

## What Gets Installed

### Required Dependencies
- `git` - For cloning repositories
- `base-devel` - Essential build tools
- `scons` - Build system for NSIS
- `pcre2` - Regular expression library
- `zlib` - Compression library
- `bzip2` - Compression library

### NSIS Components
- `makensis` - NSIS compiler
- Core NSIS libraries and tools
- Minimal installation (skips unnecessary components)

## Troubleshooting

### Common Issues

**"sudo access required"**
- Make sure you have sudo privileges
- Run: `sudo pacman -S --needed git base-devel`

**"yay installation failed"**
- Try manual installation: `./scripts/install/install_nsis_arch.sh --manual`
- Or install yay manually from: https://github.com/Jguer/yay

**"Build failed"**
- Check internet connection
- Ensure you have sufficient disk space
- Try updating your system: `sudo pacman -Syu`

**"NSIS verification failed"**
- Check if NSIS is in your PATH
- Try logging out and back in
- Run: `which makensis` to verify installation

### Manual Fallback

If all automated methods fail:

1. Visit: https://nsis.sourceforge.io/Download
2. Download NSIS manually
3. Extract and install to `/usr/local`
4. Add to PATH: `export PATH=$PATH:/usr/local/bin`

## Integration with LinuxTrack X-IR

After successful NSIS installation:

1. **Build LinuxTrack X-IR:**
   ```bash
   autoreconf -fiv
   ./configure --prefix=/opt
   make -j$(nproc)
   sudo make install
   ```

2. **Verify Windows Components:**
   - Check that `linuxtrack-wine.exe` was created
   - Verify NSIS installer functionality

3. **Test Installation:**
   ```bash
   ./scripts/install/verify_installation.sh
   ```

## Support

If you encounter issues with this script:

1. Check the troubleshooting section above
2. Review the script output for specific error messages
3. Try running with verbose output: `bash -x ./scripts/install/install_nsis_arch.sh`
4. Report issues on the LinuxTrack X-IR GitHub repository

## License

This script is part of LinuxTrack X-IR and is released under the MIT License. 