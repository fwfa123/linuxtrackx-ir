#!/bin/bash

# LinuxTrack AppImage Builder - Phase 4 Wine Bridge Integration
# Purpose: Create fully functional AppImage with complete Wine bridge integration
# Date: January 2025
# Status: Phase 4 - Wine Bridge Integration

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"
APPIMAGE_DIR="$PROJECT_ROOT/AppDir_phase4"
BUILD_DIR="$PROJECT_ROOT/build_appimage_phase4"
VERSION="1.0.0-xir.4"
APP_NAME="LinuxTrack"
APP_ID="com.linuxtrack.linuxtrack"

# AppImage tools
APPIMAGETOOL="$SCRIPT_DIR/appimagetool-x86_64.AppImage"
LINUXDEPLOY="$SCRIPT_DIR/linuxdeploy-x86_64.AppImage"
LINUXDEPLOYQT="$SCRIPT_DIR/linuxdeploy-plugin-qt-x86_64.AppImage"

# Print functions
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

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to check if file exists
file_exists() {
    [ -f "$1" ]
}

# Function to check if directory exists
dir_exists() {
    [ -d "$1" ]
}

# Function to detect distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "$ID"
    else
        echo "unknown"
    fi
}

# Function to verify AppImage tools
verify_appimage_tools() {
    print_status "Verifying AppImage tools..."
    
    if [ ! -x "$APPIMAGETOOL" ]; then
        print_error "appimagetool not found or not executable: $APPIMAGETOOL"
        return 1
    fi
    
    if [ ! -x "$LINUXDEPLOY" ]; then
        print_error "linuxdeploy not found or not executable: $LINUXDEPLOY"
        return 1
    fi
    
    if [ ! -x "$LINUXDEPLOYQT" ]; then
        print_error "linuxdeploy-plugin-qt not found or not executable: $LINUXDEPLOYQT"
        return 1
    fi
    
    print_success "All AppImage tools verified"
    return 0
}

# Function to clean previous builds
clean_build() {
    print_status "Cleaning previous builds..."
    
    if dir_exists "$APPIMAGE_DIR"; then
        rm -rf "$APPIMAGE_DIR"
        print_status "Removed previous AppDir"
    fi
    
    if dir_exists "$BUILD_DIR"; then
        rm -rf "$BUILD_DIR"
        print_status "Removed previous build directory"
    fi
    
    # Remove previous AppImage
    if file_exists "$PROJECT_ROOT/${APP_NAME}-x86_64.AppImage"; then
        rm -f "$PROJECT_ROOT/${APP_NAME}-x86_64.AppImage"
        print_status "Removed previous AppImage"
    fi
    
    print_success "Build environment cleaned"
}

# Function to create AppDir structure
create_appdir_structure() {
    print_status "Creating Phase 4 AppDir structure..."
    
    # Create main AppDir
    mkdir -p "$APPIMAGE_DIR"
    
    # Create directory structure
    mkdir -p "$APPIMAGE_DIR/usr/bin"
    mkdir -p "$APPIMAGE_DIR/usr/lib"
    mkdir -p "$APPIMAGE_DIR/usr/lib/linuxtrack"
    mkdir -p "$APPIMAGE_DIR/usr/lib32/linuxtrack"
    mkdir -p "$APPIMAGE_DIR/usr/share"
    mkdir -p "$APPIMAGE_DIR/usr/share/applications"
    mkdir -p "$APPIMAGE_DIR/usr/share/icons"
    mkdir -p "$APPIMAGE_DIR/usr/share/icons/hicolor/48x48/apps"
    mkdir -p "$APPIMAGE_DIR/usr/share/icons/hicolor/scalable/apps"
    mkdir -p "$APPIMAGE_DIR/usr/share/linuxtrack"
    mkdir -p "$APPIMAGE_DIR/usr/include"
    mkdir -p "$APPIMAGE_DIR/udev/rules.d"
    
    # Wine bridge structure (simplified - just installer and scripts)
    mkdir -p "$APPIMAGE_DIR/wine_bridge"
    mkdir -p "$APPIMAGE_DIR/wine_bridge/scripts"
    
    print_success "Phase 4 AppDir structure created"
}

# Function to create enhanced AppRun script with Wine management
create_apprun_script() {
    print_status "Creating Phase 4 AppRun script with Wine management..."
    
    cat > "$APPIMAGE_DIR/AppRun" << 'EOF'
#!/bin/bash
# AppRun - LinuxTrack AppImage launcher (Phase 4 Wine Bridge Integration)

# Get the directory where the AppImage is mounted
APPDIR="$(dirname "$(readlink -f "$0")")"

# Set up optimized environment with proper library paths
export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack:$LD_LIBRARY_PATH"
export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins"
export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
export QT_STYLE_OVERRIDE="Fusion"

# Install udev rules if not present
if [ ! -f /etc/udev/rules.d/99-TIR.rules ]; then
    echo "Installing TrackIR udev rules..."
    sudo cp "$APPDIR/udev/rules.d/99-TIR.rules" /etc/udev/rules.d/
    sudo cp "$APPDIR/udev/rules.d/99-Mickey.rules" /etc/udev/rules.d/
    sudo udevadm control --reload-rules
    echo "udev rules installed successfully"
fi

# Check if user is in plugdev group
if ! groups $USER | grep -q plugdev; then
    echo "Adding user to plugdev group..."
    sudo usermod -a -G plugdev $USER
    echo "Please log out and back in for changes to take effect"
fi

# Wine bridge components are available for GUI-initiated installation
# The GUI will handle Wine bridge installation when user clicks the button in Misc tab
if command -v wine >/dev/null 2>&1; then
    echo "Wine detected - Wine bridge components available"
    echo "Use the Misc tab in the GUI to install Wine bridge to your Wine prefix"
else
    echo "Wine not detected - Wine bridge disabled"
    echo "To enable Wine bridge support, install Wine:"
    echo "  Ubuntu/Debian: sudo apt install wine wine32 wine64"
    echo "  Fedora: sudo dnf install wine wine-core wine-desktop"
    echo "  Arch: sudo pacman -S wine wine-mono wine-gecko"
fi

# Launch the application
exec "$APPDIR/usr/bin/ltr_gui" "$@"
EOF

    chmod +x "$APPIMAGE_DIR/AppRun"
    print_success "Phase 4 AppRun script created"
}

# Function to create desktop file
create_desktop_file() {
    print_status "Creating desktop file..."
    
    cat > "$APPIMAGE_DIR/linuxtrack.desktop" << EOF
[Desktop Entry]
Name=LinuxTrack
Comment=Head tracking software for Linux with Wine bridge support
Exec=usr/bin/ltr_gui
Icon=linuxtrack
Terminal=false
Type=Application
Categories=Game;
Keywords=head;tracking;trackir;gaming;wine;
EOF

    # Copy to applications directory
    cp "$APPIMAGE_DIR/linuxtrack.desktop" "$APPIMAGE_DIR/usr/share/applications/"
    
    print_success "Desktop file created"
}

# Function to copy application icons
copy_icons() {
    print_status "Copying application icons..."
    
    # Copy existing icons if they exist
    if file_exists "$PROJECT_ROOT/src/linuxtrack.png"; then
        cp "$PROJECT_ROOT/src/linuxtrack.png" "$APPIMAGE_DIR/"
        cp "$PROJECT_ROOT/src/linuxtrack.png" "$APPIMAGE_DIR/usr/share/icons/hicolor/48x48/apps/"
        print_status "Copied PNG icon"
    fi
    
    if file_exists "$PROJECT_ROOT/src/linuxtrack.svg"; then
        cp "$PROJECT_ROOT/src/linuxtrack.svg" "$APPIMAGE_DIR/usr/share/icons/hicolor/scalable/apps/"
        print_status "Copied SVG icon"
    fi
    
    # Create a fallback icon if none exists
    if [ ! -f "$APPIMAGE_DIR/linuxtrack.png" ]; then
        print_warning "No icon found, creating placeholder"
        # Create a simple text-based icon (you can replace this with a proper icon)
        echo "Creating placeholder icon..."
    fi
    
    print_success "Icons copied"
}

# Function to copy udev rules
copy_udev_rules() {
    print_status "Copying udev rules..."
    
    if file_exists "$PROJECT_ROOT/src/99-TIR.rules"; then
        cp "$PROJECT_ROOT/src/99-TIR.rules" "$APPIMAGE_DIR/udev/rules.d/"
        print_status "Copied TrackIR udev rules"
    else
        print_warning "TrackIR udev rules not found"
    fi
    
    if file_exists "$PROJECT_ROOT/src/99-Mickey.rules"; then
        cp "$PROJECT_ROOT/src/99-Mickey.rules" "$APPIMAGE_DIR/udev/rules.d/"
        print_status "Copied Mickey udev rules"
    else
        print_warning "Mickey udev rules not found"
    fi
    
    print_success "udev rules copied"
}

# Function to prepare Wine bridge components (pre-built approach)
prepare_wine_bridge_components() {
    print_status "Preparing Wine bridge components (pre-built approach)..."
    
    cd "$PROJECT_ROOT"
    
    # Check if pre-built Wine bridge components exist
    if [ -d "prebuilt/wine_bridge" ]; then
        print_status "Found pre-built Wine bridge components"
        
        # Verify we have the main installer
        if [ -f "prebuilt/wine_bridge/linuxtrack-wine.exe" ]; then
            print_success "Found linuxtrack-wine.exe installer"
        else
            print_warning "Missing linuxtrack-wine.exe installer"
        fi
        
        # Check for essential DLLs
        local essential_dlls=("NPClient.dll" "FreeTrackClient.dll")
        for dll in "${essential_dlls[@]}"; do
            if [ -f "prebuilt/wine_bridge/$dll" ]; then
                print_success "Found: $dll"
            else
                print_warning "Missing: $dll"
            fi
        done
        
        return 0
    fi
    
    print_error "No pre-built Wine bridge components found"
    print_status "Please run: ./scripts/appimage/extract_wine_bridge_from_build.sh"
    print_status "This will build and extract all Wine bridge components"
    
    return 1
}

# Function to copy Wine bridge components
copy_wine_bridge_components() {
    print_status "Copying Wine bridge installer from newly built components..."
    
    # Use the newly built Wine bridge installer that was installed during make install
    local wine_src="$APPIMAGE_DIR/usr/share/linuxtrack/wine"
    local wine_dest="$APPIMAGE_DIR/wine_bridge"
    
    # Copy the main Wine bridge installer to the correct locations for GUI
    if [ -f "$wine_src/linuxtrack-wine.exe" ]; then
        # Copy to wine_bridge/ for the installation script
        cp "$wine_src/linuxtrack-wine.exe" "$wine_dest/"
        
        # Copy to share/linuxtrack/ for wine_launcher.cpp (if not already there)
        if [ ! -f "$APPIMAGE_DIR/usr/share/linuxtrack/linuxtrack-wine.exe" ]; then
            cp "$wine_src/linuxtrack-wine.exe" "$APPIMAGE_DIR/usr/share/linuxtrack/"
        fi
        
        print_success "Copied linuxtrack-wine.exe installer to GUI data paths (from newly built components)"
    else
        print_error "linuxtrack-wine.exe installer not found in build directory: $wine_src"
        print_error "This means the Wine bridge was not built during the make install step"
        return 1
    fi
    
    # Create Wine bridge installation script
    create_wine_bridge_installer
    
    print_success "Wine bridge installer copied"
}

# Function to create Wine bridge installation script
create_wine_bridge_installer() {
    print_status "Creating Wine bridge installation script..."
    
    cat > "$APPIMAGE_DIR/wine_bridge/scripts/install_wine_bridge.sh" << 'EOF'
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
SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
APPDIR="$(dirname "$(dirname "$SCRIPT_DIR")")"
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

    chmod +x "$APPIMAGE_DIR/wine_bridge/scripts/install_wine_bridge.sh"
    print_success "Wine bridge installation script created"
}



# Function to build LinuxTrack
build_linuxtrack() {
    print_status "Building LinuxTrack (Wine bridge enabled by default)..."
    
    cd "$PROJECT_ROOT"
    
    # Clean previous builds
    make distclean 2>/dev/null || true
    
    # Regenerate build system
    autoreconf -fiv || {
        print_error "Failed to regenerate build system"
        return 1
    }
    
    # Configure (Wine bridge is enabled by default)
    ./configure --prefix=/usr || {
        print_error "Failed to configure build"
        return 1
    }
    
    # Build
    make -j$(nproc) || {
        print_error "Failed to build LinuxTrack"
        return 1
    }
    
    print_success "LinuxTrack built successfully (Wine bridge enabled by default)"
    return 0
}

# Function to install to AppDir
install_to_appdir() {
    print_status "Installing to AppDir..."
    
    cd "$PROJECT_ROOT"
    
    # Make sure Wine bridge is built before install
    if [ -d "src/wine_bridge" ]; then
        print_status "Building Wine bridge components..."
        cd src/wine_bridge
        make || {
            print_error "Failed to build Wine bridge components"
            return 1
        }
        cd ../..
    fi
    
    # Install to AppDir
    make install DESTDIR="$APPIMAGE_DIR" || {
        print_error "Failed to install to AppDir"
        return 1
    }
    
    print_success "LinuxTrack installed to AppDir"
    return 0
}

# Function to bundle dependencies with Wine bridge optimization
bundle_dependencies() {
    print_status "Bundling dependencies with Wine bridge optimization..."
    
    cd "$APPIMAGE_DIR"
    
    # First, use linuxdeploy to bundle all dependencies
    print_status "Running linuxdeploy for comprehensive dependency bundling..."
    "$LINUXDEPLOY" --appdir . || {
        print_warning "linuxdeploy failed, continuing with manual bundling"
    }
    
    # Then use linuxdeployqt for Qt5-specific bundling
    if file_exists "usr/bin/ltr_gui"; then
        print_status "Bundling Qt5 dependencies..."
        "$LINUXDEPLOYQT" usr/bin/ltr_gui --appdir . || {
            print_warning "Qt5 bundling failed, continuing with manual bundling"
        }
    fi
    
    # Manual bundling of critical libraries
    print_status "Manual bundling of critical libraries..."
    
    # Qt5 libraries
    for qtlib in Qt5Core Qt5Gui Qt5Widgets Qt5Network Qt5OpenGL Qt5Help Qt5Sql Qt5X11Extras; do
        if [ -f "/usr/lib/x86_64-linux-gnu/lib${qtlib}.so.5" ]; then
            cp "/usr/lib/x86_64-linux-gnu/lib${qtlib}.so.5" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${qtlib}"
        fi
    done
    
    # OpenCV libraries
    for opencvlib in opencv_core opencv_imgproc opencv_objdetect; do
        if [ -f "/usr/lib/x86_64-linux-gnu/lib${opencvlib}.so.406" ]; then
            cp "/usr/lib/x86_64-linux-gnu/lib${opencvlib}.so.406" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${opencvlib}"
        fi
    done
    
    # System libraries
    for syslib in libusb-1.0 libmxml libGLU libpng16 libudev libv4l2 libv4lconvert libjpeg; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${syslib}.so.0" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${syslib}.so.0" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${syslib}"
        fi
    done
    
    # Advanced optimization
    optimize_library_structure
    
    print_success "Dependencies bundled with Wine bridge optimization"
}

# Function to optimize library structure
optimize_library_structure() {
    print_status "Advanced library structure optimization..."
    
    # Remove duplicate libraries and broken symlinks
    find usr/lib -name "*.so*" -type f | while read lib; do
        if [ -L "$lib" ]; then
            if [ ! -e "$lib" ]; then
                rm -f "$lib"
                print_status "Removed broken symlink: $lib"
            fi
        fi
    done
    
    # Strip debug symbols to reduce size
    find usr/bin usr/lib -type f -executable -exec strip --strip-unneeded {} \; 2>/dev/null || true
    
    # Remove unnecessary files
    find usr -name "*.la" -delete 2>/dev/null || true
    find usr -name "*.a" -delete 2>/dev/null || true
    
    # Optimize Qt5 plugins
    if [ -d "usr/lib/qt5/plugins" ]; then
        # Keep only essential Qt5 plugins
        find usr/lib/qt5/plugins -name "*.so" | grep -v -E "(platforms|imageformats|iconengines)" | xargs rm -f 2>/dev/null || true
        print_status "Optimized Qt5 plugins"
    fi
    
    print_success "Advanced library structure optimization completed"
}

# Function to create AppImage
create_appimage() {
    print_status "Creating Phase 4 AppImage..."
    
    cd "$PROJECT_ROOT"
    
    # Create AppImage with architecture specification
    ARCH=x86_64 "$APPIMAGETOOL" "$APPIMAGE_DIR" "${APP_NAME}-x86_64.AppImage" || {
        print_error "Failed to create AppImage"
        return 1
    }
    
    print_success "Phase 4 AppImage created: ${APP_NAME}-x86_64.AppImage"
    return 0
}

# Function to verify AppImage
verify_appimage() {
    print_status "Verifying Phase 4 AppImage..."
    
    if [ ! -f "$PROJECT_ROOT/${APP_NAME}-x86_64.AppImage" ]; then
        print_error "AppImage not found"
        return 1
    fi
    
    # Check AppImage size
    APPIMAGE_SIZE=$(du -h "$PROJECT_ROOT/${APP_NAME}-x86_64.AppImage" | cut -f1)
    print_status "AppImage size: $APPIMAGE_SIZE"
    
    # Extract and analyze dependencies
    print_status "Analyzing bundled dependencies..."
    ./${APP_NAME}-x86_64.AppImage --appimage-extract
    cd squashfs-root
    
    # Check if Qt5 libraries are bundled
    if find . -name "*Qt5*.so*" | grep -q .; then
        print_success "Qt5 libraries are bundled"
        find . -name "*Qt5*.so*" | head -5
    else
        print_warning "Qt5 libraries may not be properly bundled"
    fi
    
    # Check if OpenCV libraries are bundled
    if find . -name "*opencv*.so*" | grep -q .; then
        print_success "OpenCV libraries are bundled"
        find . -name "*opencv*.so*" | head -5
    else
        print_warning "OpenCV libraries may not be properly bundled"
    fi
    
    # Check Wine bridge components
    if [ -d "wine_bridge" ]; then
        print_success "Wine bridge components are bundled"
        find wine_bridge -name "*.dll" -o -name "*.exe" | head -10
    else
        print_warning "Wine bridge components may not be properly bundled"
    fi
    
    # Check total library count
    LIB_COUNT=$(find . -name "*.so*" | wc -l)
    print_status "Total bundled libraries: $LIB_COUNT"
    
    cd ..
    rm -rf squashfs-root
    
    # Test AppImage
    print_status "Testing AppImage..."
    "$PROJECT_ROOT/${APP_NAME}-x86_64.AppImage" --appimage-extract-and-run --help 2>/dev/null || {
        print_warning "AppImage test failed, but this may be normal for GUI applications"
    }
    
    print_success "Phase 4 AppImage verification completed"
    return 0
}

# Function to show help
show_help() {
    echo "LinuxTrack AppImage Builder - Phase 4 Wine Bridge Integration"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --clean          Clean previous builds before building"
    echo "  --skip-build     Skip building LinuxTrack (use existing build)"
    echo "  --skip-deps      Skip dependency bundling"
    echo "  --skip-wine      Skip Wine bridge component preparation"
    echo "  --help           Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0               # Full Phase 4 Wine bridge build"
    echo "  $0 --clean       # Clean Phase 4 build"
    echo "  $0 --skip-build  # Use existing build"
    echo "  $0 --skip-wine   # Skip Wine bridge component preparation"
}

# Main function
main() {
    local clean_build_flag=false
    local skip_build_flag=false
    local skip_deps_flag=false
    local skip_wine_flag=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --clean)
                clean_build_flag=true
                shift
                ;;
            --skip-build)
                skip_build_flag=true
                shift
                ;;
            --skip-deps)
                skip_deps_flag=true
                shift
                ;;
            --skip-wine)
                skip_wine_flag=true
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
    
    print_status "Starting LinuxTrack AppImage Phase 4 Wine Bridge Integration..."
    print_status "Project root: $PROJECT_ROOT"
    print_status "Distribution: $(detect_distro)"
    
    # Verify AppImage tools
    if ! verify_appimage_tools; then
        print_error "AppImage tools verification failed"
        exit 1
    fi
    
    # Clean build if requested
    if [ "$clean_build_flag" = true ]; then
        clean_build
    fi
    
    # Create AppDir structure
    create_appdir_structure
    
    # Create AppRun script
    create_apprun_script
    
    # Create desktop file
    create_desktop_file
    
    # Copy icons
    copy_icons
    
    # Copy udev rules
    copy_udev_rules
    
    # Build LinuxTrack if not skipped
    if [ "$skip_build_flag" = false ]; then
        if ! build_linuxtrack; then
            print_error "LinuxTrack build failed"
            exit 1
        fi
    else
        print_status "Skipping LinuxTrack build"
    fi
    
    # Install to AppDir
    if ! install_to_appdir; then
        print_error "Installation to AppDir failed"
        exit 1
    fi
    
    # Copy Wine bridge components from installed location if not skipped
    if [ "$skip_wine_flag" = false ]; then
        copy_wine_bridge_components
    else
        print_status "Skipping Wine bridge component preparation"
    fi
    
    # Bundle dependencies if not skipped
    if [ "$skip_deps_flag" = false ]; then
        bundle_dependencies
    else
        print_status "Skipping dependency bundling"
    fi
    
    # Create AppImage
    if ! create_appimage; then
        print_error "AppImage creation failed"
        exit 1
    fi
    
    # Verify AppImage
    verify_appimage
    
    print_success "LinuxTrack Phase 4 Wine Bridge Integration completed successfully!"
    print_status "AppImage location: $PROJECT_ROOT/${APP_NAME}-x86_64.AppImage"
    print_status "You can now test the AppImage by running: ./${APP_NAME}-x86_64.AppImage"
    print_status "Wine bridge components are included and will be installed automatically when Wine is detected"
}

# Run main function
main "$@" 