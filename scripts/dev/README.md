# Development Scripts

This directory contains development-related scripts for the linuxtrackx-ir project.

## Wine Development Scripts

### `install_wine_dev.sh`
Main installer for 32-bit Wine development packages across different Linux distributions.
- Supports Debian/Ubuntu, Fedora, and Arch Linux
- Automatically handles 32-bit architecture support
- Installs winegcc and related development tools
- **On Debian/Ubuntu, automatically creates symlinks for winegcc, wineg++, and wrc if only the -stable versions are present (for build compatibility)**

### `test_wine_dev.sh`
Comprehensive testing script for Wine development installation.
- Tests winegcc availability and functionality
- Verifies 32-bit library support
- Tests simple compilation with winegcc

### `wine_dev_setup.sh`
Interactive setup guide for Wine development environment.
- User-friendly menu system
- Automatic or manual installation options
- Built-in testing and help features

### `README_wine_dev.md`
Detailed documentation for Wine development setup.
- Complete usage instructions
- Troubleshooting guide
- Manual installation steps

### `WINE_DEV_SCRIPTS.md`
Overview of all Wine development scripts and their purposes.

## Qt Development Scripts

### `build_qt5_gui.sh`
Builds the Qt5 GUI components.

### `test_qt5_gui.sh`
Tests the Qt5 GUI functionality.

### `build_qt5_standalone.sh`
Builds standalone Qt5 applications.

### `test_qt5_standalone.sh`
Tests standalone Qt5 applications.

## Wine Bridge Scripts

### `build_wine_bridge.sh`
Builds the Wine bridge components for Windows compatibility.

## Dependency Scripts

### `setup_deps.sh`
Sets up development dependencies for the project.

## Quick Start

### Wine Development Setup (Recommended)
For most users, start with the interactive setup:
```bash
./wine_dev_setup.sh
```

This provides a user-friendly menu with options for:
- Automatic installation
- Manual installation instructions
- Testing current installation
- Help and documentation

### Alternative: Direct Installation
For advanced users or automation:
```bash
sudo ./install_wine_dev.sh
```

### Qt Development
```bash
./build_qt5_gui.sh
./test_qt5_gui.sh
```

### Wine Bridge
```bash
./build_wine_bridge.sh
```

## Usage

All scripts include help options. Run any script with `--help` for usage information:
```bash
./wine_dev_setup.sh          # Interactive setup (recommended)
./install_wine_dev.sh --help # Direct installer
./test_wine_dev.sh           # Test installation
``` 