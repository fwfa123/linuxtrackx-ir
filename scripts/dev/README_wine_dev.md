# Wine Development Package Installer

This script helps install 32-bit Wine development packages across different Linux distributions to enable `winegcc` functionality for building Wine applications.

## Supported Distributions

- **Debian/Ubuntu**: Uses `apt` package manager
- **Fedora/RHEL/CentOS**: Uses `dnf` package manager  
- **Arch Linux/Manjaro**: Uses `pacman` package manager

## Features

- Automatic distribution detection
- 32-bit architecture support installation
- Comprehensive development package installation
- Installation verification
- Colored output for better user experience
- Force installation option
- Verification-only mode

## Quick Start

1. **Run the interactive setup (Recommended)**:
   ```bash
   ./wine_dev_setup.sh
   ```

2. **Or run the installer directly (Advanced users)**:
   ```bash
   sudo ./install_wine_dev.sh
   ```

3. **Test the installation**:
   ```bash
   ./test_wine_dev.sh
   ```

## Usage

### Recommended: Interactive Setup
For most users, start with the interactive setup guide:
```bash
./wine_dev_setup.sh
```

This provides a user-friendly menu with:
- Current system status display
- Automatic installation option
- Manual installation instructions
- Testing capabilities
- Help and documentation

### Alternative: Direct Installation
For advanced users or automation:

#### Basic Installation
```bash
sudo ./install_wine_dev.sh
```

#### Force Installation
```bash
sudo ./install_wine_dev.sh --force
```

#### Verify Installation Only
```bash
./install_wine_dev.sh --verify
```

#### Show Help
```bash
./install_wine_dev.sh --help
```

## What Gets Installed

### Debian/Ubuntu
- `wine`, `wine32`, `wine64`
- `libwine` (32-bit and 64-bit)
- `libwine-dev` (32-bit and 64-bit)
- `wine-binfmt`, `fonts-wine`
- `wine64-tools` (provides winegcc)
- `build-essential`, `gcc-multilib`, `g++-multilib`
- `libc6-dev` (32-bit and 64-bit)
- **Automatic symlink workaround:** If only `winegcc-stable`, `wineg++-stable`, or `wrc-stable` are present, the installer will create symlinks (`/usr/bin/winegcc`, `/usr/bin/wineg++`, `/usr/bin/wrc`) to ensure compatibility with build systems expecting the standard names.

### Fedora
- `wine`, `wine-devel` (32-bit and 64-bit)
- `wine-devel-debuginfo` (32-bit and 64-bit)
- `gcc`, `gcc-c++`
- `glibc-devel` (32-bit and 64-bit)

### Arch Linux
- `wine`, `wine-mono`, `wine-gecko`
- `lib32-wine`
- `wine-staging` and related packages
- `base-devel`, `gcc`, `gcc-multilib`
- `lib32-glibc`, `glibc`

## Verification

The script verifies the installation by checking:
- `winegcc` availability and version
- `wine` availability and version
- 32-bit library directories

## Troubleshooting

### Common Issues

1. **Permission Denied**: Make sure to run with `sudo`
2. **Package Not Found**: Update your package lists first
3. **Architecture Issues**: The script automatically adds 32-bit architecture support
4. **winegcc Not Found After Installation**: The installer now automatically creates symlinks for `winegcc`, `wineg++`, and `wrc` if only the `-stable` versions are present (Debian/Ubuntu workaround). Try restarting your terminal or system if you still have issues.

### Manual Verification

You can manually verify the installation:
```bash
# Check if winegcc is available
which winegcc
winegcc --version

# Check if wine is available
which wine
wine --version

# Check for 32-bit libraries
ls -la /usr/lib/i386-linux-gnu/ 2>/dev/null || ls -la /usr/lib32/ 2>/dev/null || ls -la /lib32/ 2>/dev/null
```

## Notes

- This script requires root privileges for installation
- Some distributions may require additional configuration after installation
- The script automatically handles 32-bit architecture support
- For development work, you may also need additional libraries depending on your specific project
- **Debian/Ubuntu**: wine-development packages are only available in Debian Sid. For the latest development version, consider using the WineHQ repository: https://wiki.winehq.org/Debian

## References

- [Debian Wine Wiki](https://wiki.debian.org/Wine)
- [Wine GitLab - Debian/Ubuntu Instructions](https://gitlab.winehq.org/wine/wine/-/wikis/Debian-Ubuntu)
- [Wine Official Documentation](https://www.winehq.org/documentation)

## License

This script is provided as-is for educational and development purposes. 