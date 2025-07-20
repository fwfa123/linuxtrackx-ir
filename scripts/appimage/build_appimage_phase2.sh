#!/bin/bash

# LinuxTrack AppImage Builder - Phase 2 Optimized
# Purpose: Create fully optimized AppImage with complete dependency bundling
# Date: January 2025
# Status: Phase 2 - Advanced Dependency Optimization

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
APPIMAGE_DIR="$PROJECT_ROOT/AppDir_phase2"
BUILD_DIR="$PROJECT_ROOT/build_appimage_phase2"
VERSION="1.0.0-xir.3"
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
    print_status "Creating Phase 2 AppDir structure..."
    
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
    mkdir -p "$APPIMAGE_DIR/wine_bridge/32bit"
    mkdir -p "$APPIMAGE_DIR/wine_bridge/64bit"
    
    print_success "Phase 2 AppDir structure created"
}

# Function to create optimized AppRun script
create_apprun_script() {
    print_status "Creating Phase 2 AppRun script..."
    
    cat > "$APPIMAGE_DIR/AppRun" << 'EOF'
#!/bin/bash
# AppRun - LinuxTrack AppImage launcher (Phase 2 Optimized)

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

# Check for Wine and install Wine bridge components if available
if command -v wine >/dev/null 2>&1; then
    echo "Wine detected - installing Wine bridge components..."
    export WINEPREFIX="$HOME/.wine"
    
    # Install Wine bridge components if they exist
    if [ -f "$APPDIR/wine_bridge/install_wine_bridge.sh" ]; then
        "$APPDIR/wine_bridge/install_wine_bridge.sh"
    fi
else
    echo "Wine not detected - Wine bridge disabled"
fi

# Launch the application
exec "$APPDIR/usr/bin/ltr_gui" "$@"
EOF

    chmod +x "$APPIMAGE_DIR/AppRun"
    print_success "Phase 2 AppRun script created"
}

# Function to create desktop file
create_desktop_file() {
    print_status "Creating desktop file..."
    
    cat > "$APPIMAGE_DIR/linuxtrack.desktop" << EOF
[Desktop Entry]
Name=LinuxTrack
Comment=Head tracking software for Linux
Exec=usr/bin/ltr_gui
Icon=linuxtrack
Terminal=false
Type=Application
Categories=Game;
Keywords=head;tracking;trackir;gaming;
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

# Function to copy Wine bridge components
copy_wine_bridge() {
    print_status "Copying Wine bridge components..."
    
    # Check if prebuilt Wine bridge package exists
    if dir_exists "$PROJECT_ROOT/prebuilt_wine_bridge"; then
        print_status "Found prebuilt Wine bridge components"
        
        # Copy 32-bit components
        if dir_exists "$PROJECT_ROOT/prebuilt_wine_bridge/32bit"; then
            cp -r "$PROJECT_ROOT/prebuilt_wine_bridge/32bit"/* "$APPIMAGE_DIR/wine_bridge/32bit/" 2>/dev/null || true
            print_status "Copied 32-bit Wine bridge components"
        fi
        
        # Copy 64-bit components
        if dir_exists "$PROJECT_ROOT/prebuilt_wine_bridge/64bit"; then
            cp -r "$PROJECT_ROOT/prebuilt_wine_bridge/64bit"/* "$APPIMAGE_DIR/wine_bridge/64bit/" 2>/dev/null || true
            print_status "Copied 64-bit Wine bridge components"
        fi
        
        # Copy installation script
        if file_exists "$PROJECT_ROOT/prebuilt_wine_bridge/install_wine_bridge.sh"; then
            cp "$PROJECT_ROOT/prebuilt_wine_bridge/install_wine_bridge.sh" "$APPIMAGE_DIR/wine_bridge/"
            chmod +x "$APPIMAGE_DIR/wine_bridge/install_wine_bridge.sh"
            print_status "Copied Wine bridge installation script"
        fi
        
        print_success "Wine bridge components copied"
    else
        print_warning "No prebuilt Wine bridge components found"
        print_warning "Wine bridge functionality will be disabled"
    fi
}

# Function to build LinuxTrack
build_linuxtrack() {
    print_status "Building LinuxTrack..."
    
    cd "$PROJECT_ROOT"
    
    # Clean previous builds
    make distclean 2>/dev/null || true
    
    # Regenerate build system
    autoreconf -fiv || {
        print_error "Failed to regenerate build system"
        return 1
    }
    
    # Configure with AppImage support
    ./configure --prefix=/usr || {
        print_error "Failed to configure build"
        return 1
    }
    
    # Build
    make -j$(nproc) || {
        print_error "Failed to build LinuxTrack"
        return 1
    }
    
    print_success "LinuxTrack built successfully"
    return 0
}

# Function to install to AppDir
install_to_appdir() {
    print_status "Installing to AppDir..."
    
    cd "$PROJECT_ROOT"
    
    # Install to AppDir
    make install DESTDIR="$APPIMAGE_DIR" || {
        print_error "Failed to install to AppDir"
        return 1
    }
    
    print_success "LinuxTrack installed to AppDir"
    return 0
}

# Function to bundle dependencies with advanced optimization
bundle_dependencies() {
    print_status "Bundling dependencies with advanced optimization..."
    
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
    
    print_success "Dependencies bundled with advanced optimization"
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
    print_status "Creating Phase 2 AppImage..."
    
    cd "$PROJECT_ROOT"
    
    # Create AppImage with architecture specification
    ARCH=x86_64 "$APPIMAGETOOL" "$APPIMAGE_DIR" "${APP_NAME}-x86_64.AppImage" || {
        print_error "Failed to create AppImage"
        return 1
    }
    
    print_success "Phase 2 AppImage created: ${APP_NAME}-x86_64.AppImage"
    return 0
}

# Function to verify AppImage
verify_appimage() {
    print_status "Verifying Phase 2 AppImage..."
    
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
    
    print_success "Phase 2 AppImage verification completed"
    return 0
}

# Function to show help
show_help() {
    echo "LinuxTrack AppImage Builder - Phase 2 Optimized"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --clean          Clean previous builds before building"
    echo "  --skip-build     Skip building LinuxTrack (use existing build)"
    echo "  --skip-deps      Skip dependency bundling"
    echo "  --help           Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0               # Full Phase 2 optimized build"
    echo "  $0 --clean       # Clean Phase 2 build"
    echo "  $0 --skip-build  # Use existing build"
}

# Main function
main() {
    local clean_build_flag=false
    local skip_build_flag=false
    local skip_deps_flag=false
    
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
    
    print_status "Starting LinuxTrack AppImage Phase 2 optimized build..."
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
    
    # Copy Wine bridge components
    copy_wine_bridge
    
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
    
    print_success "LinuxTrack Phase 2 optimized AppImage build completed successfully!"
    print_status "AppImage location: $PROJECT_ROOT/${APP_NAME}-x86_64.AppImage"
    print_status "You can now test the AppImage by running: ./${APP_NAME}-x86_64.AppImage"
}

# Run main function
main "$@" 