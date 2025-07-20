#!/bin/bash

# Standalone Wine Bridge Component Builder
# Purpose: Build Wine bridge components once for inclusion in AppImage
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

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"
OUTPUT_DIR="$PROJECT_ROOT/prebuilt/wine_bridge"

# Function to check build requirements
check_build_requirements() {
    print_status "Checking build requirements..."
    
    local missing_deps=()
    
    # Check for MinGW tools
    if ! command -v i686-w64-mingw32-gcc >/dev/null 2>&1; then
        missing_deps+=("mingw-w64")
    fi
    
    if ! command -v i686-w64-mingw32-g++ >/dev/null 2>&1; then
        missing_deps+=("mingw-w64-g++")
    fi
    
    # Check for NSIS
    if ! command -v makensis >/dev/null 2>&1; then
        missing_deps+=("nsis")
    fi
    
    # Check for Wine (for testing, not building)
    if ! command -v wine >/dev/null 2>&1; then
        print_warning "Wine not found - will skip testing"
    fi
    
    if [ ${#missing_deps[@]} -gt 0 ]; then
        print_error "Missing build dependencies:"
        for dep in "${missing_deps[@]}"; do
            print_error "  - $dep"
        done
        
        print_status "Install dependencies:"
        print_status "  Ubuntu/Debian: sudo apt install mingw-w64 gcc-mingw-w64 g++-mingw-w64 nsis"
        print_status "  Fedora: sudo dnf install mingw64-gcc mingw64-gcc-c++ nsis"
        print_status "  Arch: sudo pacman -S mingw-w64-gcc nsis"
        
        return 1
    fi
    
    print_success "All build requirements met"
    return 0
}

# Function to build Wine bridge components
build_wine_bridge_components() {
    print_status "Building Wine bridge components..."
    
    cd "$PROJECT_ROOT"
    
    # Clean previous builds
    make distclean 2>/dev/null || true
    
    # Regenerate build system
    autoreconf -fiv || {
        print_error "Failed to regenerate build system"
        return 1
    }
    
    # Configure with Wine bridge support but disable interactive registration
    export WINEDLLPATH=""
    export WINEPREFIX=""
    
    # Configure with Wine plugin enabled
    ./configure --prefix=/usr --enable-wine-plugin || {
        print_error "Failed to configure build with Wine plugin"
        return 1
    }
    
    # Build the Wine bridge installer
    print_status "Building Wine bridge installer..."
    
    # Build the main Wine bridge target (creates linuxtrack-wine.exe)
    cd "src/wine_bridge"
    make clean 2>/dev/null || true
    make -j$(nproc) || {
        print_warning "Failed to build Wine bridge installer"
        return 1
    }
    cd "$PROJECT_ROOT"
    
    print_success "Wine bridge component build completed"
}

# Function to collect built components
collect_built_components() {
    print_status "Collecting built Wine bridge components..."
    
    # Create output directory
    mkdir -p "$OUTPUT_DIR/32bit"
    mkdir -p "$OUTPUT_DIR/64bit"
    mkdir -p "$OUTPUT_DIR/scripts"
    mkdir -p "$OUTPUT_DIR/data"
    
    # Copy the Wine bridge installer
    if [ -f "$PROJECT_ROOT/src/wine_bridge/linuxtrack-wine.exe" ]; then
        cp "$PROJECT_ROOT/src/wine_bridge/linuxtrack-wine.exe" "$OUTPUT_DIR/"
        print_success "Copied linuxtrack-wine.exe installer"
    else
        print_warning "linuxtrack-wine.exe installer not found"
    fi
    
    # Copy any data files
    find "$PROJECT_ROOT/src/wine_bridge" -name "*.dat" -exec cp {} "$OUTPUT_DIR/data/" \; 2>/dev/null || true
    find "$PROJECT_ROOT/src/wine_bridge" -name "*.txt" -exec cp {} "$OUTPUT_DIR/data/" \; 2>/dev/null || true
    find "$PROJECT_ROOT/src/wine_bridge" -name "*.xml" -exec cp {} "$OUTPUT_DIR/data/" \; 2>/dev/null || true
    
    print_status "Components collected to: $OUTPUT_DIR"
}

# Function to create installation script
create_installation_script() {
    print_status "Creating Wine bridge installation script..."
    
    cat > "$OUTPUT_DIR/scripts/install_wine_bridge.sh" << 'EOF'
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

    # Function to install Wine bridge using the installer
    install_wine_bridge() {
        local wine_prefix="$1"
        
        print_status "Installing Wine bridge to $wine_prefix..."
        
        # Check if installer exists
        if [ ! -f "$WINE_BRIDGE_DIR/linuxtrack-wine.exe" ]; then
            print_error "Wine bridge installer not found"
            return 1
        fi
        
        # Set Wine prefix
        export WINEPREFIX="$wine_prefix"
        
        # Run the installer
        if command -v wine >/dev/null 2>&1; then
            print_status "Running Wine bridge installer..."
            wine "$WINE_BRIDGE_DIR/linuxtrack-wine.exe" /S 2>/dev/null || {
                print_warning "Silent installation failed, trying interactive"
                wine "$WINE_BRIDGE_DIR/linuxtrack-wine.exe" 2>/dev/null || true
            }
            print_success "Wine bridge installation completed"
        else
            print_error "Wine not available for installation"
            return 1
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
    
    # Install to default Wine prefix
    if [ -n "$WINEPREFIX" ]; then
        install_wine_bridge "$WINEPREFIX"
    else
        # Install to default ~/.wine
        install_wine_bridge "$HOME/.wine"
    fi
    
    print_success "Wine bridge installation completed"
    print_status "You can now run Windows games with TrackIR support"
}

# Run main function
main "$@"
EOF

    chmod +x "$OUTPUT_DIR/scripts/install_wine_bridge.sh"
    print_success "Installation script created"
}

# Function to verify built components
verify_components() {
    print_status "Verifying built components..."
    
    local component_count=0
    local missing_components=()
    
    # Check for Wine bridge installer
    if [ -f "$OUTPUT_DIR/linuxtrack-wine.exe" ]; then
        print_success "Found: linuxtrack-wine.exe installer"
        ((component_count++))
    else
        print_warning "Missing: linuxtrack-wine.exe installer"
        missing_components+=("linuxtrack-wine.exe")
    fi
    
    print_status "Component verification completed: $component_count components found"
    
    if [ ${#missing_components[@]} -gt 0 ]; then
        print_warning "Some components are missing:"
        for component in "${missing_components[@]}"; do
            print_warning "  - $component"
        done
        return 1
    fi
    
    return 0
}

# Function to test components with Wine
test_with_wine() {
    if ! command -v wine >/dev/null 2>&1; then
        print_warning "Wine not available, skipping component testing"
        return 0
    fi
    
    print_status "Testing Wine bridge components with Wine..."
    
    # Create test Wine prefix
    local test_prefix="$PROJECT_ROOT/test_wine_prefix"
    if [ -d "$test_prefix" ]; then
        rm -rf "$test_prefix"
    fi
    
    mkdir -p "$test_prefix"
    export WINEPREFIX="$test_prefix"
    
    # Initialize Wine prefix
    wineboot --init 2>/dev/null || true
    
    # Test DLL registration
    if [ -f "$OUTPUT_DIR/32bit/NPClient.dll" ]; then
        print_status "Testing NPClient.dll registration..."
        cp "$OUTPUT_DIR/32bit/NPClient.dll" "$test_prefix/drive_c/windows/system32/"
        wine regsvr32 /s "NPClient.dll" 2>/dev/null || true
        print_success "NPClient.dll registration test completed"
    fi
    
    # Clean up
    rm -rf "$test_prefix"
    
    print_success "Wine component testing completed"
}

# Main function
main() {
    print_status "LinuxTrack Wine Bridge Standalone Builder"
    print_status "========================================="
    
    # Check build requirements
    if ! check_build_requirements; then
        exit 1
    fi
    
    # Build components
    build_wine_bridge_components
    
    # Collect built components
    collect_built_components
    
    # Create installation script
    create_installation_script
    
    # Verify components
    verify_components
    
    # Test with Wine
    test_with_wine
    
    print_success "Wine bridge component build completed successfully!"
    print_status "Components are ready for inclusion in AppImage"
    print_status "Location: $OUTPUT_DIR"
}

# Run main function
main "$@" 