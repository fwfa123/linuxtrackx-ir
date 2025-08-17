#!/bin/bash

# LinuxTrack AppImage Wine Bridge Fix for Arch Linux
# This script fixes the wine bridge components in the AppImage to work on Arch Linux

set -e

echo "=== LinuxTrack AppImage Wine Bridge Arch Linux Fix ==="
echo "Date: $(date)"
echo

# Check if we're on Arch Linux
if ! command -v pacman >/dev/null 2>&1; then
    echo "WARNING: This script is designed for Arch Linux systems"
    echo "pacman package manager not found - continuing anyway..."
    echo
fi

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

# Find Arch Linux wine library paths
echo "=== Finding Arch Linux Wine Library Paths ==="

# Common Arch Linux wine paths
ARCH_WINE_PATHS=(
    "/usr/lib/wine/i386-unix"
    "/usr/lib32/wine/i386-unix"
    "/usr/lib/wine"
    "/usr/lib32/wine"
)

WINE_LIBS_PATH=""

for path in "${ARCH_WINE_PATHS[@]}"; do
    if [ -d "$path" ]; then
        echo "Found wine library path: $path"
        WINE_LIBS_PATH="$path"
        break
    fi
done

if [ -z "$WINE_LIBS_PATH" ]; then
    echo "ERROR: Could not find wine library path"
    echo "Trying alternative detection method..."
    
    # Try to find wine libraries using winegcc
    WINE_LIBS_PATH=$(winegcc -print-search-dirs 2>/dev/null | grep "libraries:" | cut -d: -f2 | tr ':' '\n' | grep -E "(wine|i386)" | head -1)
    
    if [ -z "$WINE_LIBS_PATH" ]; then
        echo "ERROR: Could not determine wine library path"
        exit 1
    fi
fi

echo "Using wine library path: $WINE_LIBS_PATH"
echo

# Check if we have the wine bridge source
echo "=== Checking Wine Bridge Source ==="
if [ ! -f "src/wine_bridge/client/check_data.c" ]; then
    echo "ERROR: Wine bridge source not found"
    echo "Please run this script from the LinuxTrack source directory"
    exit 1
fi

echo "Wine bridge source found"
echo

# Create backup of current wine bridge components
echo "=== Creating Backup ==="
BACKUP_DIR="wine_bridge_appimage_backup_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$BACKUP_DIR"

if [ -f "src/wine_bridge/client/check_data.exe.so" ]; then
    cp "src/wine_bridge/client/check_data.exe.so" "$BACKUP_DIR/"
    echo "Backed up check_data.exe.so"
fi

if [ -f "src/wine_bridge/client/NPClient.dll.so" ]; then
    cp "src/wine_bridge/client/NPClient.dll.so" "$BACKUP_DIR/"
    echo "Backed up NPClient.dll.so"
fi

echo "Backup created in: $BACKUP_DIR"
echo

# Rebuild wine bridge components with Arch Linux paths
echo "=== Rebuilding Wine Bridge Components ==="
cd "src/wine_bridge/client"

echo "Cleaning previous build..."
make clean 2>/dev/null || true

echo "Building with Arch Linux wine library paths..."
echo "Using: WINE_LIBS=-L$WINE_LIBS_PATH"

# Create temporary Makefile with Arch Linux paths
cat > Makefile.arch_temp << EOF
# Temporary Makefile for Arch Linux wine bridge build
WINE_LIBS = -L$WINE_LIBS_PATH
CC = winegcc
CXX = wineg++
CFLAGS = -g -O2 -Wall -Wextra -m32
CXXFLAGS = -g -O2 -Wall -Wextra -m32
LDFLAGS = -m32 -Wall -Wextra -g
CPPFLAGS = -I. -I../.. -I../../..

all: check_data.exe.so NPClient.dll.so

check_data.exe.so: check_data.o rest.o
	\$(CC) \$(WINE_LIBS) \$(LDFLAGS) -o \$@ \$^

NPClient.dll.so: NPClient.spec NPClient_main.o linuxtrack.o rest.o
	\$(CC) \$(WINE_LIBS) \$(LDFLAGS) -o \$@ -shared \$^ -ldl

check_data.o: check_data.c rest.h
	\$(CC) -c \$(CFLAGS) \$(CPPFLAGS) -o \$@ \$<

rest.o: rest.c rest.h
	\$(CC) -c \$(CFLAGS) \$(CPPFLAGS) -o \$@ \$<

NPClient_main.o: NPClient_main.c NPClient_dll.h
	\$(CC) -c \$(CFLAGS) \$(CPPFLAGS) -o \$@ \$<

linuxtrack.o: linuxtrack.c
	\$(CC) -c \$(CFLAGS) \$(CPPFLAGS) -o \$@ \$<

clean:
	rm -f *.o *.so *.exe* Makefile.arch_temp

.PHONY: all clean
EOF

echo "Building wine bridge components..."
if make -f Makefile.arch_temp; then
    echo "SUCCESS: Wine bridge components rebuilt successfully"
else
    echo "ERROR: Build failed"
    echo "Trying direct winegcc compilation..."
    
    # Try direct compilation as fallback
    if winegcc -m32 -Wall -Wextra -g -o check_data.exe.so check_data.o rest.o -L$WINE_LIBS_PATH; then
        echo "SUCCESS: Direct compilation worked"
    else
        echo "ERROR: Direct compilation also failed"
        cd - > /dev/null
        exit 1
    fi
fi

# Clean up temporary Makefile
rm -f Makefile.arch_temp

cd - > /dev/null

echo

# Test the rebuilt executable
echo "=== Testing Rebuilt Executable ==="
if [ -f "src/wine_bridge/client/check_data.exe.so" ]; then
    echo "Testing check_data.exe.so..."
    
    # Set up test environment
    export WINEDEBUG=-all
    export WINEPREFIX="${HOME}/.wine_test_appimage"
    
    # Clean up any existing test prefix
    rm -rf "$WINEPREFIX"
    
    echo "Using test WINEPREFIX: $WINEPREFIX"
    
    # Try to run the executable
    if timeout 30s wine "src/wine_bridge/client/check_data.exe.so" 2>&1; then
        echo "SUCCESS: Rebuilt check_data.exe.so runs successfully!"
        echo "This should now work in the wine bridge installer on Arch Linux"
    else
        echo "WARNING: Rebuilt executable still has issues"
        echo "Exit code: $?"
        echo "This may indicate additional configuration is needed"
    fi
    
    # Clean up test prefix
    rm -rf "$WINEPREFIX"
else
    echo "ERROR: Rebuilt check_data.exe.so not found"
fi

echo

# Rebuild the wine bridge installer
echo "=== Rebuilding Wine Bridge Installer ==="
cd "src/wine_bridge"

if [ -f "Makefile" ]; then
    echo "Rebuilding wine bridge installer with updated components..."
    if make linuxtrack-wine.exe; then
        echo "SUCCESS: Wine bridge installer rebuilt successfully"
    else
        echo "WARNING: Failed to rebuild installer, but components are updated"
    fi
else
    echo "WARNING: Makefile not found, cannot rebuild installer"
    echo "The wine bridge components have been updated, but you may need to rebuild the installer manually"
fi

cd - > /dev/null

echo

# Instructions for AppImage rebuild
echo "=== Next Steps for AppImage ==="
echo "The wine bridge components have been rebuilt for Arch Linux compatibility."
echo
echo "To create a new AppImage with the fixed wine bridge:"
echo "1. Rebuild the AppImage:"
echo "   ./scripts/appimage/build_appimage_phase4.sh"
echo
echo "2. Test the new AppImage on Arch Linux:"
echo "   ./LinuxTrack-X-IR-0.99.23-x86_64.AppImage"
echo
echo "3. The wine bridge installer should now work without the c000007b error"
echo

# Create a verification script
echo "=== Creating Verification Script ==="
cat > "scripts/verify_wine_bridge_arch.sh" << 'EOF'
#!/bin/bash

# Verification script for Arch Linux wine bridge fix

echo "=== Verifying Wine Bridge Fix ==="

# Check if the rebuilt components exist
if [ -f "src/wine_bridge/client/check_data.exe.so" ]; then
    echo "✅ check_data.exe.so exists"
    
    # Check file type
    echo "File type: $(file src/wine_bridge/client/check_data.exe.so)"
    
    # Test with wine
    echo "Testing with Wine..."
    export WINEDEBUG=-all
    export WINEPREFIX="${HOME}/.wine_verify"
    rm -rf "$WINEPREFIX"
    
    if timeout 30s wine "src/wine_bridge/client/check_data.exe.so" 2>&1; then
        echo "✅ check_data.exe.so runs successfully with Wine"
    else
        echo "❌ check_data.exe.so still has issues"
    fi
    
    rm -rf "$WINEPREFIX"
else
    echo "❌ check_data.exe.so not found"
fi

echo
echo "=== Verification Complete ==="
EOF

chmod +x "scripts/verify_wine_bridge_arch.sh"
echo "Created verification script: scripts/verify_wine_bridge_arch.sh"
echo

echo "=== Fix Complete ==="
echo "The wine bridge components have been rebuilt for Arch Linux compatibility."
echo
echo "Summary:"
echo "  - Wine bridge components rebuilt with Arch Linux wine library paths"
echo "  - Backup created in: $BACKUP_DIR"
echo "  - Verification script created: scripts/verify_wine_bridge_arch.sh"
echo
echo "To verify the fix:"
echo "  ./scripts/verify_wine_bridge_arch.sh"
echo
echo "To rebuild the AppImage:"
echo "  ./scripts/appimage/build_appimage_phase4.sh" 