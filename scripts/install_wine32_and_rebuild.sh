#!/bin/bash

# LinuxTrack Wine Bridge 32-bit Support Installation Script for Arch Linux
# This script installs wine32 from AUR and rebuilds wine bridge components

set -e

echo "=== LinuxTrack Wine Bridge 32-bit Support Installation ==="
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

# Check if yay is available
if ! command -v yay >/dev/null 2>&1; then
    echo "ERROR: yay is required to install AUR packages"
    echo "Please install yay first:"
    echo "  sudo pacman -S --needed git base-devel"
    echo "  git clone https://aur.archlinux.org/yay.git"
    echo "  cd yay && makepkg -si"
    exit 1
fi

echo "yay AUR helper found"
echo

# Check current wine installation
echo "=== Checking Current Wine Installation ==="
if command -v wine >/dev/null 2>&1; then
    echo "Current wine version: $(wine --version)"
    
    # Check if it's wine32
    if pacman -Q wine32 >/dev/null 2>&1; then
        echo "wine32 is already installed"
        WINE32_INSTALLED=true
    else
        echo "Regular wine is installed, will replace with wine32"
        WINE32_INSTALLED=false
    fi
else
    echo "No wine installation found"
    WINE32_INSTALLED=false
fi

echo

# Install wine32 from AUR
echo "=== Installing wine32 from AUR ==="
if [ "$WINE32_INSTALLED" = false ]; then
    echo "Installing wine32 package from AUR..."
    echo "This will replace the current wine installation"
    echo "The build may take 30-60 minutes depending on your system"
    echo
    
    read -p "Do you want to proceed with wine32 installation? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo "Installation cancelled"
        exit 1
    fi
    
    echo "Installing wine32..."
    yay -S wine32
    
    if [ $? -eq 0 ]; then
        echo "SUCCESS: wine32 installed successfully"
    else
        echo "ERROR: wine32 installation failed"
        exit 1
    fi
else
    echo "wine32 is already installed"
fi

echo

# Verify wine32 installation
echo "=== Verifying wine32 Installation ==="
if command -v wine >/dev/null 2>&1; then
    echo "Wine version: $(wine --version)"
    
    # Check for 32-bit wine libraries
    if [ -d "/usr/lib32/wine/i386-unix" ]; then
        echo "Found 32-bit wine libraries: /usr/lib32/wine/i386-unix"
        WINE32_LIBS_PATH="/usr/lib32/wine/i386-unix"
    else
        echo "WARNING: 32-bit wine libraries not found in expected location"
        echo "Checking alternative locations..."
        
        # Look for 32-bit wine libraries
        WINE32_LIBS_PATH=$(find /usr/lib* -name "i386-unix" -type d 2>/dev/null | head -1)
        if [ -n "$WINE32_LIBS_PATH" ]; then
            echo "Found 32-bit wine libraries: $WINE32_LIBS_PATH"
        else
            echo "ERROR: Could not find 32-bit wine libraries"
            exit 1
        fi
    fi
    
    # Check winegcc
    if command -v winegcc >/dev/null 2>&1; then
        echo "winegcc found: $(winegcc --version 2>/dev/null | head -1)"
    else
        echo "ERROR: winegcc not found"
        exit 1
    fi
else
    echo "ERROR: wine not found after installation"
    exit 1
fi

echo

# Create backup of current wine bridge installation
echo "=== Creating Backup ==="
BACKUP_DIR="wine_bridge_backup_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$BACKUP_DIR"

if [ -d "$HOME/.wine/drive_c/Program Files (x86)/Linuxtrack" ]; then
    cp -r "$HOME/.wine/drive_c/Program Files (x86)/Linuxtrack" "$BACKUP_DIR/"
    echo "Backed up current wine bridge installation"
fi

echo "Backup created in: $BACKUP_DIR"
echo

# Configure build for 32-bit
echo "=== Configuring Build for 32-bit ==="

# Set environment variables for 32-bit build
export WINE_LIBS="-L$WINE32_LIBS_PATH"
export CFLAGS="-m32"
export CXXFLAGS="-m32"
export LDFLAGS="-m32"

echo "Build environment configured for 32-bit"
echo "WINE_LIBS: $WINE_LIBS"
echo "CFLAGS: $CFLAGS"
echo "CXXFLAGS: $CXXFLAGS"
echo "LDFLAGS: $LDFLAGS"
echo

# Rebuild wine bridge components
echo "=== Rebuilding Wine Bridge Components ==="

# Clean previous build
echo "Cleaning previous build..."
make clean 2>/dev/null || true

# Configure for 32-bit wine support
echo "Configuring build system..."
./configure --enable-wine --with-wine-libs="$WINE32_LIBS_PATH" 2>/dev/null || {
    echo "WARNING: Configure failed, trying without explicit wine-libs"
    ./configure --enable-wine 2>/dev/null || {
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
    if make check_data.exe.so NPClient.dll.so; then
        echo "SUCCESS: Client components built"
    else
        echo "ERROR: Client components build failed"
        exit 1
    fi
    
    cd ../tester
    if make Tester.exe.so; then
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
    export WINEPREFIX="${HOME}/.wine_test_32bit"
    
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

if [ -f "src/wine_bridge/client/NPClient.dll.so" ]; then
    cp "src/wine_bridge/client/NPClient.dll.so" "$INSTALL_DIR/NPClient.dll"
    echo "Installed NPClient.dll"
fi

# Copy tester components
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
echo "The wine bridge has been rebuilt and installed for 32-bit compatibility."
echo
echo "Files installed in: $INSTALL_DIR"
echo "Backup created in: $BACKUP_DIR"
echo
echo "Benefits of this installation:"
echo "1. Full 32-bit wine support for older games"
echo "2. Compatibility with classic TrackIR games"
echo "3. Support for both 32-bit and 64-bit wine prefixes"
echo "4. Better compatibility with legacy Windows applications"
echo
echo "To test the installation, run:"
echo "  cd '$INSTALL_DIR'"
echo "  WINEPREFIX='$HOME/.wine' wine check_data.exe"
echo
echo "To create a 32-bit wine prefix for older games:"
echo "  WINEPREFIX=~/.wine32 WINEARCH=win32 wineboot --init"
echo
echo "If you experience issues, try:"
echo "1. Restarting Wine: wineboot --restart"
echo "2. Reinstalling wine-mono: sudo pacman -S wine-mono"
echo "3. Checking wine configuration: winecfg" 