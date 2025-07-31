#!/bin/bash

# LinuxTrack Wine Bridge 64-bit Rebuild Script for Arch Linux
# This script rebuilds the wine bridge components for 64-bit compatibility

set -e

echo "=== LinuxTrack Wine Bridge 64-bit Rebuild for Arch Linux ==="
echo "Date: $(date)"
echo

# Check if we're on Arch Linux
if ! command -v pacman >/dev/null 2>&1; then
    echo "ERROR: This script is designed for Arch Linux systems"
    echo "pacman package manager not found"
    exit 1
fi

echo "Detected Arch Linux system"
echo

# Check wine installation
echo "=== Checking Wine Installation ==="
if ! command -v wine >/dev/null 2>&1; then
    echo "ERROR: Wine not installed. Please install wine:"
    echo "  sudo pacman -S wine"
    exit 1
fi

if ! command -v winegcc >/dev/null 2>&1; then
    echo "ERROR: winegcc not found. Please install wine-mono:"
    echo "  sudo pacman -S wine-mono"
    exit 1
fi

echo "Wine installation OK: $(wine --version)"
echo

# Check if we have 64-bit wine support
echo "=== Checking 64-bit Wine Support ==="
if [ -d "/usr/lib/wine/x86_64-unix" ]; then
    echo "Found 64-bit wine libraries: /usr/lib/wine/x86_64-unix"
    WINE64_LIBS_PATH="/usr/lib/wine/x86_64-unix"
else
    echo "ERROR: 64-bit wine libraries not found"
    echo "This script requires 64-bit wine support"
    exit 1
fi

echo "Using 64-bit wine library path: $WINE64_LIBS_PATH"
echo

# Create backup of current installation
echo "=== Creating Backup ==="
BACKUP_DIR="wine_bridge_backup_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$BACKUP_DIR"

if [ -d "$HOME/.wine/drive_c/Program Files (x86)/Linuxtrack" ]; then
    cp -r "$HOME/.wine/drive_c/Program Files (x86)/Linuxtrack" "$BACKUP_DIR/"
    echo "Backed up current wine bridge installation"
fi

echo "Backup created in: $BACKUP_DIR"
echo

# Configure build for 64-bit
echo "=== Configuring Build for 64-bit ==="

# Set environment variables for 64-bit build
export WINE64_LIBS="-L$WINE64_LIBS_PATH"
export CFLAGS="-m64"
export CXXFLAGS="-m64"
export LDFLAGS="-m64"

echo "Build environment configured for 64-bit"
echo "WINE64_LIBS: $WINE64_LIBS"
echo "CFLAGS: $CFLAGS"
echo "CXXFLAGS: $CXXFLAGS"
echo "LDFLAGS: $LDFLAGS"
echo

# Rebuild wine bridge components
echo "=== Rebuilding Wine Bridge Components ==="

# Clean previous build
echo "Cleaning previous build..."
make clean 2>/dev/null || true

# Configure for 64-bit wine support
echo "Configuring build system..."
./configure --enable-wine64 --with-wine64-libs="$WINE64_LIBS_PATH" 2>/dev/null || {
    echo "WARNING: Configure failed, trying without explicit wine64-libs"
    ./configure --enable-wine64 2>/dev/null || {
        echo "ERROR: Configure failed"
        exit 1
    }
}

# Build wine bridge components
echo "Building wine bridge components..."
if make -C src/wine_bridge; then
    echo "SUCCESS: Wine bridge components built successfully"
else
    echo "ERROR: Build failed"
    echo "Trying alternative build method..."
    
    # Try building individual components
    cd src/wine_bridge/client
    if make check_data.exe.so NPClient64.dll.so; then
        echo "SUCCESS: Client components built"
    else
        echo "ERROR: Client components build failed"
        exit 1
    fi
    
    cd ../tester
    if make Tester64.exe.so; then
        echo "SUCCESS: Tester components built"
    else
        echo "ERROR: Tester components build failed"
        exit 1
    fi
    
    cd ../..
fi

echo

# Test the rebuilt executable
echo "=== Testing Rebuilt Executable ==="
if [ -f "src/wine_bridge/client/check_data.exe.so" ]; then
    echo "Testing check_data.exe.so..."
    
    # Set up test environment
    export WINEDEBUG=-all
    export WINEPREFIX="${HOME}/.wine_test_64bit"
    
    # Clean up any existing test prefix
    rm -rf "$WINEPREFIX"
    
    echo "Using test WINEPREFIX: $WINEPREFIX"
    
    # Try to run the executable
    if timeout 30s wine "src/wine_bridge/client/check_data.exe.so" 2>&1; then
        echo "SUCCESS: Rebuilt check_data.exe.so runs successfully!"
    else
        echo "WARNING: Rebuilt executable still has issues"
        echo "Exit code: $?"
    fi
    
    # Clean up test prefix
    rm -rf "$WINEPREFIX"
else
    echo "ERROR: Rebuilt check_data.exe.so not found"
fi

echo

# Install the rebuilt components
echo "=== Installing Rebuilt Components ==="

# Create installation directory
INSTALL_DIR="$HOME/.wine/drive_c/Program Files (x86)/Linuxtrack"
mkdir -p "$INSTALL_DIR"

# Copy rebuilt components
echo "Copying rebuilt components to $INSTALL_DIR..."

# Copy client components
if [ -f "src/wine_bridge/client/check_data.exe.so" ]; then
    cp "src/wine_bridge/client/check_data.exe.so" "$INSTALL_DIR/check_data.exe"
    echo "Installed check_data.exe"
fi

if [ -f "src/wine_bridge/client/NPClient64.dll.so" ]; then
    cp "src/wine_bridge/client/NPClient64.dll.so" "$INSTALL_DIR/NPClient64.dll"
    echo "Installed NPClient64.dll"
fi

if [ -f "src/wine_bridge/client/NPClient.dll.so" ]; then
    cp "src/wine_bridge/client/NPClient.dll.so" "$INSTALL_DIR/NPClient.dll"
    echo "Installed NPClient.dll"
fi

# Copy tester components
if [ -f "src/wine_bridge/tester/Tester64.exe.so" ]; then
    cp "src/wine_bridge/tester/Tester64.exe.so" "$INSTALL_DIR/Tester64.exe"
    echo "Installed Tester64.exe"
fi

if [ -f "src/wine_bridge/tester/Tester.exe.so" ]; then
    cp "src/wine_bridge/tester/Tester.exe.so" "$INSTALL_DIR/Tester.exe"
    echo "Installed Tester.exe"
fi

# Copy other components
if [ -f "src/wine_bridge/controller/Controller.exe.so" ]; then
    cp "src/wine_bridge/controller/Controller.exe.so" "$INSTALL_DIR/Controller.exe"
    echo "Installed Controller.exe"
fi

if [ -f "src/wine_bridge/views/TrackIR.exe.so" ]; then
    cp "src/wine_bridge/views/TrackIR.exe.so" "$INSTALL_DIR/TrackIR.exe"
    echo "Installed TrackIR.exe"
fi

if [ -f "src/wine_bridge/ft_client/FreeTrackClient.dll.so" ]; then
    cp "src/wine_bridge/ft_client/FreeTrackClient.dll.so" "$INSTALL_DIR/FreeTrackClient.dll"
    echo "Installed FreeTrackClient.dll"
fi

if [ -f "src/wine_bridge/ft_tester/ftc.exe.so" ]; then
    cp "src/wine_bridge/ft_tester/ftc.exe.so" "$INSTALL_DIR/FreeTrackTester.exe"
    echo "Installed FreeTrackTester.exe"
fi

echo

# Test the installation
echo "=== Testing Installation ==="
cd "$INSTALL_DIR"

echo "Testing check_data.exe..."
if WINEPREFIX="$HOME/.wine" wine check_data.exe 2>&1; then
    echo "SUCCESS: check_data.exe runs successfully!"
else
    echo "WARNING: check_data.exe still has issues"
    echo "Exit code: $?"
fi

echo

echo "=== Installation Complete ==="
echo "The wine bridge has been rebuilt and installed for 64-bit compatibility."
echo
echo "Files installed in: $INSTALL_DIR"
echo "Backup created in: $BACKUP_DIR"
echo
echo "To test the installation, run:"
echo "  cd '$INSTALL_DIR'"
echo "  WINEPREFIX='$HOME/.wine' wine check_data.exe"
echo
echo "If you still experience issues, try:"
echo "1. Restarting Wine: wineboot --restart"
echo "2. Reinstalling wine-mono: sudo pacman -S wine-mono"
echo "3. Checking wine configuration: winecfg" 