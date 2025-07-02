#!/bin/bash
# LinuxTrack Wine Bridge Launcher
# This script helps users install the wine bridge components

LINUXTRACK_DATA_DIR="@pkgdatadir@"
WINE_INSTALLER="$LINUXTRACK_DATA_DIR/wine/linuxtrack-wine.exe"

echo "LinuxTrack Wine Bridge Launcher"
echo "==============================="
echo ""

# Check if wine is available
if ! command -v wine >/dev/null 2>&1; then
    echo "ERROR: Wine is not installed or not in PATH"
    echo "Please install Wine first:"
    echo "  Ubuntu/Debian: sudo apt install wine"
    echo "  Fedora: sudo dnf install wine"
    echo "  Arch: sudo pacman -S wine"
    exit 1
fi

# Check if the installer exists
if [ ! -f "$WINE_INSTALLER" ]; then
    echo "ERROR: Wine installer not found at $WINE_INSTALLER"
    echo "Please ensure LinuxTrack is properly installed with wine support enabled"
    exit 1
fi

echo "Wine Bridge Installer found: $WINE_INSTALLER"
echo ""
echo "This will install LinuxTrack Wine Bridge components to your Wine prefix."
echo "You will be prompted to select a Wine prefix (bottle) for installation."
echo ""
echo "Note: You may need to install this in each Wine prefix where you want"
echo "head tracking support for Windows applications."
echo ""

read -p "Continue with installation? (y/N): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Installation cancelled."
    exit 0
fi

echo "Starting Wine Bridge installer..."
echo ""

# Run the wine installer
wine "$WINE_INSTALLER"

echo ""
echo "Wine Bridge installation completed."
echo "You can now use head tracking in Windows applications running under Wine."
echo ""
echo "For more information, see: $LINUXTRACK_DATA_DIR/wine/README.wine" 