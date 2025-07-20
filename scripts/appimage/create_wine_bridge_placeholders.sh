#!/bin/bash

# Create Wine Bridge Placeholder Components
# Purpose: Create placeholder files and documentation for Wine bridge components
# Date: January 2025

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"
WINE_BRIDGE_DIR="$PROJECT_ROOT/prebuilt/wine_bridge"

# Create directory structure
mkdir -p "$WINE_BRIDGE_DIR/32bit"
mkdir -p "$WINE_BRIDGE_DIR/64bit"
mkdir -p "$WINE_BRIDGE_DIR/scripts"
mkdir -p "$WINE_BRIDGE_DIR/data"

# Create placeholder DLL files
print_status "Creating placeholder Wine bridge components..."

# 32-bit components
cat > "$WINE_BRIDGE_DIR/32bit/NPClient.dll" << 'EOF'
# Placeholder for NPClient.dll
# This is a placeholder file. The actual NPClient.dll should be built separately.
# See README_WINE_BRIDGE.txt for build instructions.
EOF

cat > "$WINE_BRIDGE_DIR/32bit/FreeTrackClient.dll" << 'EOF'
# Placeholder for FreeTrackClient.dll
# This is a placeholder file. The actual FreeTrackClient.dll should be built separately.
# See README_WINE_BRIDGE.txt for build instructions.
EOF

cat > "$WINE_BRIDGE_DIR/32bit/Controller.exe" << 'EOF'
# Placeholder for Controller.exe
# This is a placeholder file. The actual Controller.exe should be built separately.
# See README_WINE_BRIDGE.txt for build instructions.
EOF

cat > "$WINE_BRIDGE_DIR/32bit/Tester.exe" << 'EOF'
# Placeholder for Tester.exe
# This is a placeholder file. The actual Tester.exe should be built separately.
# See README_WINE_BRIDGE.txt for build instructions.
EOF

# 64-bit components
cat > "$WINE_BRIDGE_DIR/64bit/NPClient64.dll" << 'EOF'
# Placeholder for NPClient64.dll
# This is a placeholder file. The actual NPClient64.dll should be built separately.
# See README_WINE_BRIDGE.txt for build instructions.
EOF

cat > "$WINE_BRIDGE_DIR/64bit/Tester64.exe" << 'EOF'
# Placeholder for Tester64.exe
# This is a placeholder file. The actual Tester64.exe should be built separately.
# See README_WINE_BRIDGE.txt for build instructions.
EOF

# Create installation script
cat > "$WINE_BRIDGE_DIR/scripts/install_wine_bridge.sh" << 'EOF'
#!/bin/bash
# Wine Bridge Installation Script for LinuxTrack AppImage

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Get the directory where the AppImage is mounted
APPDIR="$(dirname "$(dirname "$(readlink -f "$0")")")"
WINE_BRIDGE_DIR="$APPDIR/wine_bridge"

# Function to check if components are placeholders
check_placeholders() {
    local has_placeholders=false
    
    # Check for placeholder files
    for file in "$WINE_BRIDGE_DIR/32bit"/* "$WINE_BRIDGE_DIR/64bit"/*; do
        if [ -f "$file" ]; then
            if head -1 "$file" | grep -q "Placeholder"; then
                has_placeholders=true
                break
            fi
        fi
    done
    
    if [ "$has_placeholders" = true ]; then
        print_warning "Wine bridge components are placeholders"
        print_status "Please build the actual Wine bridge components first"
        print_status "See README_WINE_BRIDGE.txt for build instructions"
        return 1
    fi
    
    return 0
}

# Function to install components to Wine prefix
install_to_wine_prefix() {
    local wine_prefix="$1"
    local arch="$2"
    
    print_status "Installing Wine bridge components to $wine_prefix ($arch)..."
    
    # Check if components are placeholders
    if ! check_placeholders; then
        return 1
    fi
    
    # Create system32 directory if it doesn't exist
    mkdir -p "$wine_prefix/drive_c/windows/system32"
    
    # Copy DLL files
    if [ -d "$WINE_BRIDGE_DIR/$arch" ]; then
        find "$WINE_BRIDGE_DIR/$arch" -name "*.dll" -exec cp {} "$wine_prefix/drive_c/windows/system32/" \; 2>/dev/null || true
        print_status "Copied DLL files to $wine_prefix"
    fi
    
    # Copy EXE files to a convenient location
    mkdir -p "$wine_prefix/drive_c/Program Files/LinuxTrack"
    if [ -d "$WINE_BRIDGE_DIR/$arch" ]; then
        find "$WINE_BRIDGE_DIR/$arch" -name "*.exe" -exec cp {} "$wine_prefix/drive_c/Program Files/LinuxTrack/" \; 2>/dev/null || true
        print_status "Copied EXE files to $wine_prefix"
    fi
    
    # Register DLLs
    if command -v wine >/dev/null 2>&1; then
        cd "$wine_prefix/drive_c/windows/system32"
        for dll in *.dll; do
            if [ -f "$dll" ]; then
                wine regsvr32 "$dll" 2>/dev/null || true
            fi
        done
        print_status "Registered DLL files"
    fi
}

# Main installation
main() {
    print_status "LinuxTrack Wine Bridge Installation"
    print_status "==================================="
    
    # Check if Wine is available
    if ! command -v wine >/dev/null 2>&1; then
        print_error "Wine is not installed. Please install Wine first."
        print_status "Ubuntu/Debian: sudo apt install wine wine32 wine64"
        print_status "Fedora: sudo dnf install wine wine-core wine-desktop"
        print_status "Arch: sudo pacman -S wine wine-mono wine-gecko"
        exit 1
    fi
    
    # Check for placeholders first
    if ! check_placeholders; then
        exit 1
    fi
    
    # Install to default Wine prefix
    if [ -n "$WINEPREFIX" ]; then
        install_to_wine_prefix "$WINEPREFIX" "32bit"
        if [ -d "$WINE_BRIDGE_DIR/64bit" ]; then
            install_to_wine_prefix "$WINEPREFIX" "64bit"
        fi
    else
        # Install to default ~/.wine
        install_to_wine_prefix "$HOME/.wine" "32bit"
        if [ -d "$WINE_BRIDGE_DIR/64bit" ]; then
            install_to_wine_prefix "$HOME/.wine" "64bit"
        fi
    fi
    
    print_success "Wine bridge installation completed"
    print_status "You can now run Windows games with TrackIR support"
}

# Run main function
main "$@"
EOF

chmod +x "$WINE_BRIDGE_DIR/scripts/install_wine_bridge.sh"

# Create README for Wine bridge
cat > "$WINE_BRIDGE_DIR/README_WINE_BRIDGE.txt" << 'EOF'
LinuxTrack Wine Bridge - Build Instructions
==========================================

This directory contains placeholder files for the Wine bridge components.
To use Wine bridge functionality, you need to build the actual components.

Prerequisites:
-------------
- Wine development environment
- MinGW cross-compilation tools
- NSIS installer (for creating installers)

Build Instructions:
------------------

1. Install build dependencies:
   Ubuntu/Debian:
   sudo apt install wine wine32 wine64 mingw-w64 gcc-mingw-w64 g++-mingw-w64 nsis

   Fedora:
   sudo dnf install wine wine-core wine-desktop mingw64-gcc mingw64-gcc-c++ nsis

   Arch:
   sudo pacman -S wine wine-mono wine-gecko mingw-w64-gcc nsis

2. Build Wine bridge components:
   cd /path/to/linuxtrackx-ir
   ./scripts/dev/build_wine_bridge.sh

3. Copy built components:
   The build script will create the necessary DLL and EXE files.
   Copy them to replace the placeholder files in this directory.

Required Components:
-------------------
32-bit components:
- NPClient.dll (TrackIR client library)
- FreeTrackClient.dll (FreeTrack client library)
- Controller.exe (Hotkey controller)
- Tester.exe (TrackIR protocol tester)

64-bit components:
- NPClient64.dll (64-bit TrackIR client library)
- Tester64.exe (64-bit TrackIR protocol tester)

Installation:
-------------
Once you have built the components, the AppImage will automatically
install them to your Wine prefix when you run it.

Usage:
------
1. Install Wine on your system
2. Run the LinuxTrack AppImage
3. The AppImage will detect Wine and install the bridge components
4. Launch your Windows games - they should detect TrackIR support

For more information, see the main LinuxTrack documentation.
EOF

print_success "Wine bridge placeholder components created"
print_status "Location: $WINE_BRIDGE_DIR"
print_status "Components are placeholders - build actual components separately"
print_status "See README_WINE_BRIDGE.txt for build instructions" 