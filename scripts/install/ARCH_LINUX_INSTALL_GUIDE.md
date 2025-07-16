# Arch Linux Installation Guide for LinuxTrack X-IR

## Overview

This guide provides step-by-step instructions for installing LinuxTrack X-IR on Arch Linux and its derivatives (Manjaro, EndeavourOS, etc.). It addresses common package name issues and provides solutions for problematic dependencies.

## Common Issues on Arch Linux

### 1. Package Name Differences
- **`qt5-help`** → Not needed (included in `qt5-tools`)
- **`libmxml`** → **`mxml`** (available in extras repository)
- **`nsis`** → May have AUR issues (use our helper script)

### 2. Repository Differences
- Some packages are in different repositories
- AUR packages may have build issues
- Package names may differ from other distributions

## Installation Methods

### Method 1: Standard Installation (Recommended)

```bash
# Update system
sudo pacman -Syu

# Install core dependencies
sudo pacman -S --needed base-devel autoconf automake libtool
sudo pacman -S --needed qt5-base qt5-tools qt5-x11extras
sudo pacman -S --needed opencv libusb mxml libx11 libxrandr
sudo pacman -S --needed bison flex lib32-glibc lib32-gcc-libs v4l-utils

# Install NSIS using our helper script
./scripts/install/install_nsis_arch.sh

# Clone and build LinuxTrack X-IR
git clone <repository-url>
cd linuxtrackx-ir

# Install Wine development tools
cd scripts/dev
./wine_dev_setup.sh
cd ../..

# Build and install
autoreconf -fiv
./configure --prefix=/opt
make -j$(nproc)
sudo make install
```

### Method 2: One-Command Installation

```bash
# Single command for all dependencies
sudo pacman -S --needed base-devel autoconf automake libtool qt5-base qt5-tools qt5-x11extras opencv libusb mxml libx11 libxrandr bison flex lib32-glibc lib32-gcc-libs v4l-utils

# Install NSIS
./scripts/install/install_nsis_arch.sh
```

### Method 3: Manual Package Installation

If you encounter issues with specific packages:

```bash
# Install packages one by one to identify issues
sudo pacman -S base-devel
sudo pacman -S autoconf automake libtool
sudo pacman -S qt5-base qt5-tools qt5-x11extras
sudo pacman -S opencv libusb mxml
sudo pacman -S libx11 libxrandr
sudo pacman -S bison flex
sudo pacman -S lib32-glibc lib32-gcc-libs
sudo pacman -S v4l-utils
```

## Package Explanations

### Core Build Tools
- **`base-devel`**: Essential build tools (gcc, make, etc.)
- **`autoconf automake libtool`**: Autotools for build system
- **`bison flex`**: Parser generators

### Qt5 Dependencies
- **`qt5-base`**: Core Qt5 libraries
- **`qt5-tools`**: Qt5 development tools (includes help functionality)
- **`qt5-x11extras`**: X11 integration for Qt5

### Libraries
- **`opencv`**: Computer vision library
- **`libusb`**: USB device access
- **`mxml`**: XML parsing library (equivalent to libmxml)
- **`libx11 libxrandr`**: X11 development libraries
- **`v4l-utils`**: Video4Linux utilities

### 32-bit Support
- **`lib32-glibc lib32-gcc-libs`**: 32-bit libraries for Wine compatibility

## Troubleshooting

### Issue: "target not found: qt5-help"
**Solution**: `qt5-help` is not needed. Qt5 help functionality is included in `qt5-tools`.

### Issue: "target not found: libmxml"
**Solution**: Use `mxml` instead of `libmxml`. The package is available in the extras repository.

### Issue: "target not found: nsis"
**Solution**: Use our NSIS installation script:
```bash
./scripts/install/install_nsis_arch.sh
```

### Issue: Package conflicts
**Solution**: Try installing packages individually to identify the problematic package:
```bash
sudo pacman -S --needed package-name
```

### Issue: AUR package build failures
**Solution**: Use our helper scripts that provide multiple installation methods:
- NSIS: `./scripts/install/install_nsis_arch.sh`
- Wine tools: `./scripts/dev/wine_dev_setup.sh`

## Verification

After installation, verify that all components are working:

```bash
# Check if NSIS is installed
makensis /VERSION

# Check if Qt5 tools are available
qmake --version
qhelpgenerator --help

# Check if libraries are found
pkg-config --exists opencv4
pkg-config --exists libusb-1.0
pkg-config --exists mxml

# Test LinuxTrack installation
ltr_gui
```

## Alternative Package Managers

If you prefer different package managers:

### Using yay (AUR helper)
```bash
# Install yay if not already installed
sudo pacman -S --needed git base-devel
git clone https://aur.archlinux.org/yay.git
cd yay
makepkg -si

# Install dependencies
yay -S base-devel autoconf automake libtool qt5-base qt5-tools qt5-x11extras opencv libusb mxml libx11 libxrandr bison flex lib32-glibc lib32-gcc-libs v4l-utils
```

### Using paru (Alternative AUR helper)
```bash
# Install paru
sudo pacman -S --needed git base-devel
git clone https://aur.archlinux.org/paru.git
cd paru
makepkg -si

# Install dependencies
paru -S base-devel autoconf automake libtool qt5-base qt5-tools qt5-x11extras opencv libusb mxml libx11 libxrandr bison flex lib32-glibc lib32-gcc-libs v4l-utils
```

## Distribution-Specific Notes

### Manjaro
- Uses pacman as the primary package manager
- May have additional packages in the Manjaro repositories
- Follow the standard Arch Linux instructions

### EndeavourOS
- Based on Arch Linux
- Uses pacman and yay by default
- Follow the standard Arch Linux instructions

### ArcoLinux
- Based on Arch Linux
- May have additional packages in ArcoLinux repositories
- Follow the standard Arch Linux instructions

## Support

If you encounter issues:

1. **Check the troubleshooting section above**
2. **Run our verification scripts**:
   ```bash
   ./scripts/install/verify_installation.sh
   ```
3. **Check system logs** for specific error messages
4. **Report issues** on the LinuxTrack X-IR GitHub repository

## Additional Resources

- [Arch Linux Wiki](https://wiki.archlinux.org/)
- [AUR Package Database](https://aur.archlinux.org/)
- [LinuxTrack X-IR Documentation](docs/)
- [NSIS Installation Script](scripts/install/install_nsis_arch.sh) 