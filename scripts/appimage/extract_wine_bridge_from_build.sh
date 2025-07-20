#!/bin/bash

# Extract Wine Bridge Components from Full Build
# Purpose: Run full build and extract Wine bridge binaries for AppImage
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
BUILD_DIR="$PROJECT_ROOT/temp_wine_build"
OUTPUT_DIR="$PROJECT_ROOT/prebuilt/wine_bridge"

# Function to check build requirements
check_build_requirements() {
    print_status "Checking build requirements..."
    
    local missing_deps=()
    
    # Check for basic build tools
    if ! command -v make >/dev/null 2>&1; then
        missing_deps+=("make")
    fi
    
    if ! command -v gcc >/dev/null 2>&1; then
        missing_deps+=("gcc")
    fi
    
    if ! command -v g++ >/dev/null 2>&1; then
        missing_deps+=("g++")
    fi
    
    # Check for Wine (optional, for testing)
    if ! command -v wine >/dev/null 2>&1; then
        print_warning "Wine not found - will skip testing"
    fi
    
    if [ ${#missing_deps[@]} -gt 0 ]; then
        print_error "Missing build dependencies:"
        for dep in "${missing_deps[@]}"; do
            print_error "  - $dep"
        done
        return 1
    fi
    
    print_success "All build requirements met"
    return 0
}

# Function to run full build process
run_full_build() {
    print_status "Running full build process..."
    
    # Create temporary build directory
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
    fi
    mkdir -p "$BUILD_DIR"
    
    cd "$BUILD_DIR"
    
    # Copy source to build directory
    cp -r "$PROJECT_ROOT"/* .
    
    # Clean any previous builds
    make distclean 2>/dev/null || true
    
    # Regenerate build system
    autoreconf -fiv || {
        print_error "Failed to regenerate build system"
        return 1
    }
    
    # Configure with all options enabled
    ./configure --prefix=/opt || {
        print_error "Failed to configure build"
        return 1
    }
    
    # Build everything
    make -j$(nproc) || {
        print_error "Failed to build LinuxTrack"
        return 1
    }
    
    # Install to build directory
    make install DESTDIR="$BUILD_DIR" || {
        print_error "Failed to install to build directory"
        return 1
    }
    
    print_success "Full build completed successfully"
}

# Function to extract Wine bridge components
extract_wine_components() {
    print_status "Extracting Wine bridge components..."
    
    # Create output directory
    mkdir -p "$OUTPUT_DIR"
    
    # Extract from /opt/bin/
    if [ -d "$BUILD_DIR/opt/bin" ]; then
        print_status "Extracting from /opt/bin/..."
        find "$BUILD_DIR/opt/bin" -name "*wine*" -o -name "*Wine*" -o -name "*track*" -o -name "*Track*" | while read file; do
            if [ -f "$file" ]; then
                cp "$file" "$OUTPUT_DIR/"
                print_status "Copied: $(basename "$file")"
            fi
        done
    fi
    
    # Extract from /opt/share/linuxtrack/wine/
    if [ -d "$BUILD_DIR/opt/share/linuxtrack/wine" ]; then
        print_status "Extracting from /opt/share/linuxtrack/wine/..."
        cp -r "$BUILD_DIR/opt/share/linuxtrack/wine"/* "$OUTPUT_DIR/" 2>/dev/null || true
    fi
    
    # Look for Wine bridge components in other locations
    print_status "Searching for Wine bridge components..."
    
    # Search for DLL files
    find "$BUILD_DIR" -name "*.dll" | while read file; do
        if [ -f "$file" ]; then
            cp "$file" "$OUTPUT_DIR/"
            print_status "Copied DLL: $(basename "$file")"
        fi
    done
    
    # Search for EXE files
    find "$BUILD_DIR" -name "*.exe" | while read file; do
        if [ -f "$file" ]; then
            cp "$file" "$OUTPUT_DIR/"
            print_status "Copied EXE: $(basename "$file")"
        fi
    done
    
    # Search for Wine bridge specific files
    find "$BUILD_DIR" -name "*wine*" -o -name "*Wine*" -o -name "*track*" -o -name "*Track*" | while read file; do
        if [ -f "$file" ] && [ ! -d "$file" ]; then
            # Skip if already copied
            if [ ! -f "$OUTPUT_DIR/$(basename "$file")" ]; then
                cp "$file" "$OUTPUT_DIR/"
                print_status "Copied: $(basename "$file")"
            fi
        fi
    done
    
    print_success "Wine bridge components extracted"
}

# Function to create installation script
create_installation_script() {
    print_status "Creating Wine bridge installation script..."
    
    cat > "$OUTPUT_DIR/install_wine_bridge.sh" << 'EOF'
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
APPDIR="$(dirname "$(readlink -f "$0")")"
WINE_BRIDGE_DIR="$APPDIR"

# Function to install Wine bridge components
install_wine_bridge() {
    local wine_prefix="$1"
    
    print_status "Installing Wine bridge to $wine_prefix..."
    
    # Check if Wine is available
    if ! command -v wine >/dev/null 2>&1; then
        print_error "Wine is not installed. Please install Wine first."
        print_status "Ubuntu/Debian: sudo apt install wine wine32 wine64"
        print_status "Fedora: sudo dnf install wine wine-core wine-desktop"
        print_status "Arch: sudo pacman -S wine wine-mono wine-gecko"
        exit 1
    fi
    
    # Set Wine prefix
    export WINEPREFIX="$wine_prefix"
    
    # Create system32 directory if it doesn't exist
    mkdir -p "$wine_prefix/drive_c/windows/system32"
    
    # Copy DLL files
    find "$WINE_BRIDGE_DIR" -name "*.dll" | while read file; do
        if [ -f "$file" ]; then
            cp "$file" "$wine_prefix/drive_c/windows/system32/"
            print_status "Copied: $(basename "$file")"
        fi
    done
    
    # Copy EXE files to Program Files
    mkdir -p "$wine_prefix/drive_c/Program Files/LinuxTrack"
    find "$WINE_BRIDGE_DIR" -name "*.exe" | while read file; do
        if [ -f "$file" ]; then
            cp "$file" "$wine_prefix/drive_c/Program Files/LinuxTrack/"
            print_status "Copied: $(basename "$file")"
        fi
    done
    
    # Register DLLs (non-interactive)
    cd "$wine_prefix/drive_c/windows/system32"
    for dll in *.dll; do
        if [ -f "$dll" ]; then
            wine regsvr32 /s "$dll" 2>/dev/null || true
        fi
    done
    
    print_success "Wine bridge installation completed"
}

# Main installation
main() {
    print_status "LinuxTrack Wine Bridge Installation"
    print_status "==================================="
    
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

    chmod +x "$OUTPUT_DIR/install_wine_bridge.sh"
    print_success "Installation script created"
}

# Function to verify extracted components
verify_components() {
    print_status "Verifying extracted components..."
    
    local component_count=0
    
    # Count all files in output directory
    if [ -d "$OUTPUT_DIR" ]; then
        component_count=$(find "$OUTPUT_DIR" -type f | wc -l)
        print_status "Found $component_count components"
        
        # List the components
        find "$OUTPUT_DIR" -type f | while read file; do
            print_status "  - $(basename "$file")"
        done
    fi
    
    if [ "$component_count" -gt 0 ]; then
        print_success "Wine bridge components extracted successfully"
        return 0
    else
        print_warning "No Wine bridge components found"
        return 1
    fi
}

# Function to clean up build directory
cleanup_build() {
    print_status "Cleaning up build directory..."
    
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        print_success "Build directory cleaned up"
    fi
}

# Function to show help
show_help() {
    echo "Extract Wine Bridge Components from Full Build"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --keep-build    Keep the build directory for inspection"
    echo "  --help          Show this help message"
    echo ""
    echo "This script runs the full LinuxTrack build process and extracts"
    echo "Wine bridge components from the standard installation locations."
}

# Main function
main() {
    local keep_build=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --keep-build)
                keep_build=true
                shift
                ;;
            --help)
                show_help
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                show_help
                exit 1
                ;;
        esac
    done
    
    print_status "Starting Wine bridge component extraction..."
    print_status "Project root: $PROJECT_ROOT"
    print_status "Build directory: $BUILD_DIR"
    print_status "Output directory: $OUTPUT_DIR"
    
    # Check build requirements
    if ! check_build_requirements; then
        exit 1
    fi
    
    # Run full build
    if ! run_full_build; then
        print_error "Full build failed"
        exit 1
    fi
    
    # Extract Wine bridge components
    extract_wine_components
    
    # Create installation script
    create_installation_script
    
    # Verify components
    verify_components
    
    # Clean up if requested
    if [ "$keep_build" = false ]; then
        cleanup_build
    else
        print_status "Build directory kept at: $BUILD_DIR"
    fi
    
    print_success "Wine bridge component extraction completed successfully!"
    print_status "Components are ready for inclusion in AppImage"
    print_status "Location: $OUTPUT_DIR"
}

# Run main function
main "$@" 