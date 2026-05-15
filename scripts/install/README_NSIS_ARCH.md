# NSIS Installation Script for Arch Linux

## Overview

The `install_nsis_arch.sh` script helps install NSIS (Nullsoft Scriptable Install System) on Arch Linux systems when the AUR package has issues. This script is specifically designed to handle the common problems users face when trying to install NSIS on Arch Linux for building LinuxTrack X-IR Windows compatibility components.

## Why This Script is Needed

The NSIS package in the AUR (Arch User Repository) can sometimes have issues:
- Build failures due to dependency conflicts
- Outdated package versions
- Missing build dependencies
- AUR helper compatibility issues

This script installs **full NSIS** (makensis **and** installer **Stubs**). Stubs are required to build `linuxtrack-wine.exe`; a compiler-only install will fail with `Error: reading stub ".../zlib-x86-unicode"`.

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
- `zlib` or `zlib-ng-compat` - Compression library (`zlib` is omitted on CachyOS etc. when `zlib-ng-compat` is already installed)
- `bzip2` - Compression library

### NSIS Components
- `makensis` - NSIS compiler
- `Stubs/` - Installer stubs (e.g. `zlib-x86-unicode`) under `/usr/share/nsis/Stubs/` (AUR/pacman) or `/usr/local/share/nsis/Stubs/` (manual build)

## Troubleshooting

### Common Issues

**"sudo access required"**
- Make sure you have sudo privileges
- Run: `sudo pacman -S --needed git base-devel`

**"yay installation failed"**
- Try manual installation: `./scripts/install/install_nsis_arch.sh --manual`
- Or install yay manually from: https://github.com/Jguer/yay

**"`zlib` and `zlib-ng-compat` are in conflict" (CachyOS, etc.)**
- Do **not** remove `zlib-ng-compat` when pacman asks
- The script skips `zlib` when `zlib-ng-compat` is installed; if an older script failed mid-run, install deps manually: `sudo pacman -S --needed scons pcre2 bzip2`
- Confirm libz: `pkg-config --exists zlib && echo OK`

**"scons: command not found"**
- Usually means the pacman step above failed (often the zlib conflict). Install `scons` explicitly, then re-run the script

**"`/usr/local/makensis`: Permission denied"**
- The build succeeded but install to `/usr/local` needs root. Current script runs `sudo scons ... install-compiler` after the user build step; re-run the script or install manually from the extracted source directory

**"Build failed"**
- Check internet connection
- Ensure you have sufficient disk space
- Try updating your system: `sudo pacman -Syu`

**AUR: `gpg: keyserver receive failed` / `PGP keys need importing` (mingw-w64-zlib)**

The `nsis` package pulls in AUR MinGW dependencies that must be signed. If yay cannot reach a keyserver:

```bash
mkdir -p ~/.gnupg
printf '%s\n' 'keyserver hkps://keys.openpgp.org' >> ~/.gnupg/dirmngr.conf
gpgconf --kill dirmngr
gpg --recv-keys 5ED46A6721D365587791E2AA783FCD8E58BCAFBA
yay -S nsis mingw-w64-gcc
```

If `keys.openpgp.org` fails, try `hkps://keyserver.ubuntu.com` in `dirmngr.conf` instead. On CachyOS you can also import the key from the [mingw-w64-zlib AUR page](https://aur.archlinux.org/packages/mingw-w64-zlib) (PGP key link).

**"Error: reading stub ... zlib-x86-unicode" / Wine bridge installer fails**
- Install full NSIS: `yay -S nsis mingw-w64-gcc`
- Remove broken compiler-only install: `sudo rm -f /usr/local/bin/makensis /usr/local/makensis`
- Re-run: `./scripts/install/install_nsis_arch.sh --force`
- Verify: `test -f /usr/share/nsis/Stubs/zlib-x86-unicode && which makensis`
- Reconfigure CMake from a clean build dir so `MAKENSIS_EXECUTABLE` points at `/usr/bin/makensis`

**"NSIS verification failed"**
- Run `./scripts/install/install_nsis_arch.sh --verify`
- Check stubs: `ls /usr/share/nsis/Stubs/zlib-x86-unicode`

### Manual Fallback

If all automated methods fail:

1. Visit: https://nsis.sourceforge.io/Download
2. Download NSIS manually
3. Extract and install to `/usr/local`
4. Add to PATH: `export PATH=$PATH:/usr/local/bin`

## Integration with LinuxTrack X-IR

After successful NSIS installation, build LinuxTrack X-IR with **CMake** (see [arch-linux.md](../../docs/readme/arch-linux.md)):

```bash
cd linuxtrackx-ir
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

Verify that `linuxtrack-wine.exe` was installed under `/opt/share/linuxtrack/wine/` and that the bridge DLLs are present under `/opt/lib/linuxtrack/wine_bridge/`.

## Support

If you encounter issues with this script:

1. Check the troubleshooting section above
2. Review the script output for specific error messages
3. Try running with verbose output: `bash -x ./scripts/install/install_nsis_arch.sh`
4. Report issues on the LinuxTrack X-IR GitHub repository

## License

This script is part of LinuxTrack X-IR and is released under the MIT License. 