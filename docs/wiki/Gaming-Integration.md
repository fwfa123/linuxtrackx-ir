# Gaming Platform Integration

LinuxTrack X-IR v0.99.22+ includes comprehensive gaming platform integration for seamless Wine Bridge installation across different gaming platforms. This guide covers setting up head tracking for Windows games running through Wine on Linux.

## Overview

The new Gaming tab provides centralized access to all gaming-related functionality, including:

- **Steam Proton Integration**: Automatic detection and Wine Bridge installation for Steam games
- **Lutris Integration**: Complete Lutris prefix detection and Wine Bridge support
- **Custom Wine Prefix Support**: Manual Wine Bridge installation for custom setups
- **Cross-Distribution Compatibility**: Optimized support for Ubuntu, Fedora, Arch Linux, and more

## Prerequisites

Before setting up Wine Bridge integration, ensure you have the following:

### Essential Requirements
- **TrackIR Firmware**: Must be installed for Wine Bridge to function properly
- **MFC42.dll**: Required for Windows games compatibility (automatically handled by the installer)

### Optional Requirements
- **Steam**: For Steam Proton games integration
- **Lutris**: For Lutris-managed games integration
- **Wine**: For custom Wine prefix setups

## Getting Started

1. **Install LinuxTrack X-IR**: Follow the standard installation process
2. **Install TrackIR Firmware**: Use the built-in firmware installer if you haven't already
3. **Navigate to Gaming Tab**: Open the application and click on the "Gaming" tab
4. **Check Prerequisites**: The tab shows live status of required components

---

## Steam Proton Integration

LinuxTrack X-IR automatically detects Steam installations and Proton versions, including:

- Native Steam installations
- Steam Flatpak installations
- Multiple Proton versions (including beta versions like Proton 9.0 Beta)
- Custom Proton installations

### Setup Process

1. **Open Gaming Tab**: Navigate to the Gaming tab in LinuxTrack X-IR
2. **Verify Prerequisites**: Ensure firmware and MFC42 are installed
3. **Select Steam Target**: Choose "Steam" from the installer targets dropdown
4. **Install Wine Bridge**: Click "Install Wine Bridge"
5. **Automatic Detection**: The system detects your Steam installation automatically
6. **Installation**: Wine Bridge is installed to the appropriate Steam location

### Supported Steam Configurations

| Configuration | Support | Notes |
|---------------|---------|-------|
| Native Steam | ✅ Full | Direct installation |
| Steam Flatpak | ✅ Full | Sandbox-aware installation |
| Multiple Libraries | ✅ Full | All libraries supported |
| Custom Proton | ✅ Full | Custom versions detected |
| Beta Proton Versions | ✅ Full | Including Proton 9.0+ |

---

## Lutris Integration

LinuxTrack X-IR provides complete Lutris integration with advanced features:

- Automatic Lutris prefix detection
- Support for custom Wine runners
- Flatpak Lutris support
- Enhanced Wine path resolution
- YAML configuration parsing

### Setup Process

1. **Open Gaming Tab**: Navigate to the Gaming tab
2. **Verify Prerequisites**: Ensure firmware and MFC42 are installed
3. **Select Lutris Target**: Choose "Lutris" from the installer targets dropdown
4. **Install Wine Bridge**: Click "Install Wine Bridge"
5. **Automatic Detection**: System detects Lutris installation and prefixes
6. **Installation**: Wine Bridge is installed to detected prefixes

### Advanced Lutris Features

#### Custom Wine Runners
LinuxTrack X-IR supports custom Wine runners configured in Lutris:
- lutris-GE-Proton8-15-x86_64
- wine-ge-8-26-x86_64
- Custom Wine versions
- Proton versions

#### Flatpak Support
For Flatpak Lutris installations:
- Automatic sandbox detection
- Proper Wine prefix resolution
- Host system Wine detection

---

## Custom Wine Prefix Setup

For Wine prefixes not managed by Steam or Lutris:

### Setup Process

1. **Navigate to Gaming Tab**: Open the Gaming tab
2. **Access Advanced Menu**: Click the "Advanced" menu button
3. **Select Custom Option**: Choose "Other Platform/Wine Prefix"
4. **Choose Prefix Location**: Browse to your custom Wine prefix
5. **Install Wine Bridge**: Click "Install Wine Bridge"

### Supported Custom Configurations

| Configuration | Support | Setup Method |
|---------------|---------|--------------|
| Standard Wine Prefix | ✅ Full | Direct prefix selection |
| PlayOnLinux | ✅ Full | Manual prefix selection |
| Bottles | ✅ Full | Manual prefix selection |
| Custom Wine Versions | ✅ Full | Version auto-detection |
| Multi-Architecture | ✅ Full | 32/64-bit detection |

---

## Testing Wine Bridge Installation

The Gaming tab includes comprehensive testing functionality:

### Testing Features

- **Auto-load Games**: Automatically discovers games in your Steam/Lutris libraries
- **Status Labels**: Shows real-time status of Wine Bridge components
- **Filter Options**: Filter games by platform for easier testing
- **Persistent Settings**: Remembers your last selected platform and game

### Testing Process

1. **Ensure Installation**: Verify Wine Bridge is installed for your target platform
2. **Navigate to Gaming Tab**: Open the Gaming tab
3. **Test Wine Bridge**: Click "Test Wine Bridge"
4. **Select Game**: Choose a game from the auto-loaded list
5. **Launch Game**: Start the game to verify head tracking works

### Troubleshooting Tests

If head tracking doesn't work:

1. **Check Prerequisites**: Verify firmware and MFC42 installation
2. **Test Components**: Use NPTest to verify Wine Bridge components
3. **Check Game Configuration**: Ensure game is configured for TrackIR
4. **Review Logs**: Check application logs for error messages

---

## Cross-Distribution Compatibility

LinuxTrack X-IR v0.99.22+ includes enhanced cross-distribution support:

### Ubuntu/Debian/MX Linux
- Optimized Wine installation with winetricks integration
- Automatic MFC42.dll handling
- Debian package support

### Fedora/Nobara
- Automatic package detection and configuration
- RPM package management
- Wine development package handling

### Arch Linux
- Enhanced Wine32 alternative sources
- AUR package support
- Optimized build configurations

### Flatpak Support
- Seamless integration with sandboxed gaming platforms
- Host system detection for Wine components
- Proper path resolution in sandboxed environments

---

## Advanced Configuration

### Manual Wine Bridge Installation

For advanced users who need manual control:

```bash
# Build Wine Bridge manually
cd src/wine_bridge
make clean && make

# Install to custom prefix
WINEPREFIX=~/.wine ./install_wine_bridge.sh

# Test installation
WINEPREFIX=~/.wine wine NPTest.exe
```

### Custom Wine Versions

LinuxTrack X-IR supports various Wine configurations:

```bash
# Using custom Wine binary
export WINE=/path/to/custom/wine
./ltr_gui

# With Wine environment variables
export WINEPREFIX=/path/to/prefix
export WINEARCH=win32
./ltr_gui
```

### Debug Mode

Enable debug logging for troubleshooting:

```bash
# Run with debug output
./ltr_gui --debug

# Check Wine Bridge logs
tail -f ~/.config/linuxtrack/wine_bridge.log

# Test individual components
WINEPREFIX=~/.wine wine NPTest.exe --verbose
```

---

## Troubleshooting

### Common Issues

#### Wine Bridge Not Detected
- Ensure prerequisites are installed
- Check Wine prefix permissions
- Verify Wine installation

#### Games Don't Recognize TrackIR
- Install Wine Bridge to correct prefix
- Check game configuration files
- Verify Wine Bridge components are loaded

#### Performance Issues
- Use appropriate Wine version for your system
- Configure Wine settings for optimal performance
- Check system resources

#### Flatpak Issues
- Ensure Flatpak has access to Wine components
- Check sandbox permissions
- Use Flatpak-compatible Wine versions

### Getting Help

For additional help:

- Check the [Troubleshooting Guide](Troubleshooting.md)
- Review [Application Logs](../logs/)
- Visit the [GitLab Issues Page](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues)
- Check the [Community Forums](https://forums.x-plane.org/index.php?act=idx)

---

## Technical Details

### Wine Bridge Components

The Wine Bridge consists of several components:

- **NPTest.exe**: Testing utility for Wine Bridge functionality
- **TIRViews.dll**: TrackIR interface library for games
- **FreeTrackClient.dll**: FreeTrack protocol support
- **Configuration files**: Game-specific settings and mappings

### Installation Process

1. **Prerequisite Check**: Verifies firmware and MFC42 installation
2. **Platform Detection**: Identifies Steam, Lutris, or custom setup
3. **Wine Detection**: Finds appropriate Wine installation
4. **Component Installation**: Installs Wine Bridge components
5. **Verification**: Tests installation with NPTest
6. **Configuration**: Sets up game-specific configurations

### Compatibility Matrix

| Platform | Wine Bridge | Steam Integration | Lutris Integration | Custom Support |
|----------|-------------|-------------------|-------------------|----------------|
| Ubuntu 20.04+ | ✅ | ✅ | ✅ | ✅ |
| Fedora 35+ | ✅ | ✅ | ✅ | ✅ |
| Arch Linux | ✅ | ✅ | ✅ | ✅ |
| Debian 11+ | ✅ | ✅ | ✅ | ✅ |
| MX Linux | ✅ | ✅ | ✅ | ✅ |
| Flatpak | ✅ | ✅ | ✅ | ✅ |

---

For more information about specific platforms or troubleshooting, see:

- [Device Setup](Device-Setup.md) - Hardware configuration
- [System Information](System.md) - System information and troubleshooting
- [Troubleshooting](Troubleshooting.md) - Common issues and solutions
- [Welcome](Welcome.md) - Getting started guide
- [News](News.md) - Latest features and updates
