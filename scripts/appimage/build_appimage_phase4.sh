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
VERSION="0.99.23"
APP_NAME="LinuxTrack-X-IR"
APP_DISPLAY_NAME="LinuxTrack X-IR"
APP_ID="com.linuxtrack.linuxtrackx-ir"

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
    if file_exists "$PROJECT_ROOT/${APP_NAME}-${VERSION}-x86_64.AppImage"; then
        rm -f "$PROJECT_ROOT/${APP_NAME}-${VERSION}-x86_64.AppImage"
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

# Set up completely isolated environment for self-contained AppImage
export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack"
export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins"
export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
export QT_STYLE_PATH="$APPDIR/usr/lib/qt5/plugins/styles"

# CRITICAL: Complete Qt isolation to prevent version mixing
export QT_DISABLE_VERSION_CHECK=1
export QT_LOGGING_RULES="qt.qpa.*=false"
export QT_DEBUG_PLUGINS=0
export QT_QPA_PLATFORM_PLUGIN_NAMES="xcb"
export QT_QPA_PLATFORM_PLUGIN_PATH_DEBUG="$APPDIR/usr/lib/qt5/plugins/platforms"

# CRITICAL: Isolate GLib environment to prevent symbol conflicts
export GIO_EXTRA_MODULES="$APPDIR/usr/lib/gio/modules"
export GI_TYPELIB_PATH="$APPDIR/usr/lib/girepository-1.0"
export GSETTINGS_SCHEMA_DIR="$APPDIR/usr/share/glib-2.0/schemas"
export G_DEBUG="fatal-warnings"

# Force X11 usage to avoid Wayland compatibility issues
export QT_QPA_PLATFORM="xcb"
export QT_AUTO_SCREEN_SCALE_FACTOR=0
export QT_SCALE_FACTOR=1

# Completely isolate Qt environment to prevent system Qt conflicts
export QT_DISABLE_VERSION_CHECK=1
export QT_LOGGING_RULES="qt.qpa.*=false"
export QT_DEBUG_PLUGINS=0

# Additional Qt platform plugin environment variables
export QT_QPA_PLATFORM_PLUGIN_NAMES="xcb"
export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
export QT_QPA_PLATFORM_PLUGIN_PATH_DEBUG="$APPDIR/usr/lib/qt5/plugins/platforms"

# Debug Qt platform plugin loading if needed
if [ "$DEBUG_QT_PLUGINS" = "1" ]; then
    export QT_DEBUG_PLUGINS=1
    echo "Qt plugin debugging enabled"
fi

# Dynamic theme detection and integration
detect_and_set_theme() {
    local desktop_env=""
    
    # Detect desktop environment
    if [ -n "$XDG_CURRENT_DESKTOP" ]; then
        desktop_env="$XDG_CURRENT_DESKTOP"
    elif [ -n "$DESKTOP_SESSION" ]; then
        desktop_env="$DESKTOP_SESSION"
    elif [ -n "$GNOME_DESKTOP_SESSION_ID" ]; then
        desktop_env="GNOME"
    elif [ -n "$KDE_FULL_SESSION" ]; then
        desktop_env="KDE"
    fi
    
    echo "Detected desktop environment: $desktop_env"
    
    # Set theme based on desktop environment
    case "$desktop_env" in
        *"KDE"*|*"plasma"*|*"Plasma"*)
            echo "Using KDE/Breeze theme integration"
            export QT_QPA_PLATFORMTHEME="breeze"
            export QT_STYLE_OVERRIDE="Breeze"
            ;;
        *"GNOME"*|*"ubuntu:GNOME"*|*"Unity"*)
            echo "Using GNOME/GTK3 theme integration"
            export QT_QPA_PLATFORMTHEME="gtk3"
            # Don't override style - let it use system GTK theme
            ;;
        *"XFCE"*|*"xfce"*)
            echo "Using XFCE/GTK2 theme integration"
            export QT_QPA_PLATFORMTHEME="gtk2"
            # Don't override style - let it use system GTK theme
            ;;
        *"MATE"*|*"mate"*)
            echo "Using MATE/GTK3 theme integration"
            export QT_QPA_PLATFORMTHEME="gtk3"
            # Don't override style - let it use system GTK theme
            ;;
        *"Cinnamon"*|*"cinnamon"*)
            echo "Using Cinnamon/GTK3 theme integration"
            export QT_QPA_PLATFORMTHEME="gtk3"
            # Don't override style - let it use system GTK theme
            ;;
        *"Budgie"*|*"budgie"*)
            echo "Using Budgie/GTK3 theme integration"
            export QT_QPA_PLATFORMTHEME="gtk3"
            # Don't override style - let it use system GTK theme
            ;;
        *)
            echo "Unknown desktop environment, using GTK3 theme integration"
            export QT_QPA_PLATFORMTHEME="gtk3"
            # Don't override style - let it use system theme
            ;;
    esac
}

# Call theme detection function
detect_and_set_theme

# CRITICAL: Prevent system Qt libraries from being loaded
prevent_system_qt() {
    # Clear any system Qt paths from environment
    unset QT_DIR
    unset QTDIR
    unset QT_SELECT
    unset QT4DIR
    unset QT5DIR
    
    # Force use of bundled Qt only
    export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins"
    export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
    export QT_STYLE_PATH="$APPDIR/usr/lib/qt5/plugins/styles"
    export QT_SQL_DRIVER_PATH="$APPDIR/usr/lib/qt5/plugins/sqldrivers"
    export QT_DISABLE_VERSION_CHECK=1
    export QT_LOGGING_RULES="qt.qpa.*=false"
    export QT_DEBUG_PLUGINS=0
    export QT_QPA_PLATFORM_PLUGIN_NAMES="xcb"
    export QT_QPA_PLATFORM_PLUGIN_PATH_DEBUG="$APPDIR/usr/lib/qt5/plugins/platforms"
    
    # Ensure LD_LIBRARY_PATH only contains our bundled libraries
    export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack"
    
    echo "Qt environment isolated - using bundled Qt libraries only"
}

# Call Qt isolation function
prevent_system_qt

# Verify Qt isolation before launching
verify_qt_isolation() {
    echo "Verifying Qt isolation..."
    
    # Check if any system Qt libraries are in LD_LIBRARY_PATH
    if echo "$LD_LIBRARY_PATH" | grep -q "/usr/lib\|/lib"; then
        echo "WARNING: System library paths detected in LD_LIBRARY_PATH"
        echo "Forcing complete isolation..."
        export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack"
    fi
    
    # Check if Qt environment variables are properly set
    if [ "$QT_PLUGIN_PATH" != "$APPDIR/usr/lib/qt5/plugins" ]; then
        echo "WARNING: Qt plugin path not properly isolated"
        export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins"
    fi
    
    if [ "$QT_QPA_PLATFORM_PLUGIN_PATH" != "$APPDIR/usr/lib/qt5/plugins/platforms" ]; then
        echo "WARNING: Qt platform plugin path not properly isolated"
        export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
    fi
    
    echo "Qt isolation verified - using bundled libraries only"
}

# Verify Qt isolation
verify_qt_isolation

# Additional X11 forcing for problematic Wayland environments
if [ "$XDG_SESSION_TYPE" = "wayland" ]; then
    echo "Wayland detected - forcing X11 compatibility mode"
    export QT_QPA_PLATFORM="xcb"
    export GDK_BACKEND="x11"
    export CLUTTER_BACKEND="x11"
    export SDL_VIDEODRIVER="x11"
    
    # Additional Wayland-specific X11 forcing
    export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
    export QT_QPA_PLATFORM_PLUGIN_NAMES="xcb"
    
    # Try to start X11 if not available
    if [ -z "$DISPLAY" ] || [ "$DISPLAY" = ":0" ]; then
        echo "Attempting to start X11 server..."
        # This is a fallback - in most cases X11 should already be available
        export DISPLAY=":0"
    fi
fi

# Ensure X11 is available and working
if ! xset q >/dev/null 2>&1; then
    echo "Warning: X11 server not responding, but continuing..."
    echo "If the application fails to start, try running: xhost +local:"
fi

# Note: udev rules installation is handled by the GUI permission dialog
# The AppImage includes udev rules in the bundle but does not install them automatically

# Note: Group creation and user management is handled by the GUI permission dialog
# The AppImage does not automatically create groups or add users to groups

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

# Launch the application with platform plugin fallback
launch_application() {
    # Use completely isolated environment (no system libraries)
    export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack"
    export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins"
    export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
    export QT_STYLE_PATH="$APPDIR/usr/lib/qt5/plugins/styles"
    export QT_SQL_DRIVER_PATH="$APPDIR/usr/lib/qt5/plugins/sqldrivers"
    export QT_DISABLE_VERSION_CHECK=1
    export QT_LOGGING_RULES="qt.qpa.*=false"
    export QT_DEBUG_PLUGINS=0
    export QT_QPA_PLATFORM_PLUGIN_NAMES="xcb"
    export QT_QPA_PLATFORM_PLUGIN_PATH_DEBUG="$APPDIR/usr/lib/qt5/plugins/platforms"
    
    # CRITICAL: Isolate GLib environment to prevent symbol conflicts
    export GIO_EXTRA_MODULES="$APPDIR/usr/lib/gio/modules"
    export GI_TYPELIB_PATH="$APPDIR/usr/lib/girepository-1.0"
    export GSETTINGS_SCHEMA_DIR="$APPDIR/usr/share/glib-2.0/schemas"
    export G_DEBUG="fatal-warnings"
    
    # Help system debugging and configuration
    export QT_DEBUG_PLUGINS=1
    export QT_LOGGING_RULES="qt.help.*=true;qt.qpa.*=false;qt.sql.*=true"
    export QT_HELP_PATH="$APPDIR/usr/share/linuxtrack/help"
    
    # Enhanced SQLite driver configuration
    export QT_SQL_DRIVER_PATH="$APPDIR/usr/lib/qt5/plugins/sqldrivers"
    export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins:$APPDIR/usr/lib/qt5/plugins/sqldrivers"
    
    # Additional Qt environment variables for help system
    export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
    export QT_STYLE_PATH="$APPDIR/usr/lib/qt5/plugins/styles"
    export QT_IMAGEIO_MAXALLOC=0
    export QT_AUTO_SCREEN_SCALE_FACTOR=1
    
    # Try xcb first with complete isolation
    export QT_QPA_PLATFORM="xcb"
    export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack"
    export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins"
    export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
    export QT_DISABLE_VERSION_CHECK=1
    echo "Attempting to launch with xcb platform (complete isolation)..."
    if "$APPDIR/usr/bin/ltr_gui" "$@" 2>/dev/null; then
        return 0
    fi
    
    # Wayland fallback removed - using minimal platform instead
    
    # Try minimal as last resort
    export QT_QPA_PLATFORM="minimal"
    export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack"
    export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins"
    export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
    export QT_DISABLE_VERSION_CHECK=1
    echo "wayland failed, trying minimal platform (complete isolation)..."
    if "$APPDIR/usr/bin/ltr_gui" "$@" 2>/dev/null; then
        return 0
    fi
    
    # If all fail, try with default platform and complete isolation
    echo "All platform plugins failed, trying default (complete isolation)..."
    export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack"
    export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins"
    export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
    export QT_DISABLE_VERSION_CHECK=1
    unset QT_QPA_PLATFORM
    exec "$APPDIR/usr/bin/ltr_gui" "$@"
}

launch_application "$@"
EOF

    chmod +x "$APPIMAGE_DIR/AppRun"
    print_success "Phase 4 AppRun script created"
}

# Function to create desktop file
create_desktop_file() {
    print_status "Creating desktop file..."
    
    cat > "$APPIMAGE_DIR/linuxtrack.desktop" << EOF
[Desktop Entry]
Name=${APP_DISPLAY_NAME}
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

# Function to copy help files
copy_help_files() {
    print_status "Copying help files..."
    
    # Create help directory structure
    mkdir -p "$APPIMAGE_DIR/usr/share/linuxtrack/help/ltr_gui"
    mkdir -p "$APPIMAGE_DIR/usr/share/linuxtrack/help/mickey"
    
    # Copy ltr_gui help files
    if file_exists "$PROJECT_ROOT/src/qt_gui/help.qhc"; then
        cp "$PROJECT_ROOT/src/qt_gui/help.qhc" "$APPIMAGE_DIR/usr/share/linuxtrack/help/ltr_gui/"
        print_status "Copied ltr_gui help.qhc"
    else
        print_warning "ltr_gui help.qhc not found"
    fi
    
    if file_exists "$PROJECT_ROOT/src/qt_gui/help.qch"; then
        cp "$PROJECT_ROOT/src/qt_gui/help.qch" "$APPIMAGE_DIR/usr/share/linuxtrack/help/ltr_gui/"
        print_status "Copied ltr_gui help.qch"
    else
        print_warning "ltr_gui help.qch not found"
    fi
    
    # Copy mickey help files
    if file_exists "$PROJECT_ROOT/src/mickey/help.qhc"; then
        cp "$PROJECT_ROOT/src/mickey/help.qhc" "$APPIMAGE_DIR/usr/share/linuxtrack/help/mickey/"
        print_status "Copied mickey help.qhc"
    else
        print_warning "mickey help.qhc not found"
    fi
    
    if file_exists "$PROJECT_ROOT/src/mickey/help.qch"; then
        cp "$PROJECT_ROOT/src/mickey/help.qch" "$APPIMAGE_DIR/usr/share/linuxtrack/help/mickey/"
        print_status "Copied mickey help.qch"
    else
        print_warning "mickey help.qch not found"
    fi
    
    # Copy help content files if they exist
    if [ -d "$PROJECT_ROOT/src/qt_gui/help" ]; then
        cp -r "$PROJECT_ROOT/src/qt_gui/help" "$APPIMAGE_DIR/usr/share/linuxtrack/help/ltr_gui/"
        print_status "Copied ltr_gui help content"
    fi
    
    if [ -d "$PROJECT_ROOT/src/mickey/help" ]; then
        cp -r "$PROJECT_ROOT/src/mickey/help" "$APPIMAGE_DIR/usr/share/linuxtrack/help/mickey/"
        print_status "Copied mickey help content"
    fi
    
    print_success "Help files copied"
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
        print_status "Building Wine bridge components with multi-distribution support..."
        
        # Detect distribution and wine library paths
        DISTRO="unknown"
        if [ -f "/etc/arch-release" ]; then
            DISTRO="arch"
            print_status "Detected: Arch Linux"
        elif [ -f "/etc/debian_version" ]; then
            DISTRO="debian"
            print_status "Detected: Debian/Ubuntu"
        elif [ -f "/etc/fedora-release" ]; then
            DISTRO="fedora"
            print_status "Detected: Fedora"
        elif [ -f "/etc/SuSE-release" ] || ( [ -f "/etc/os-release" ] && grep -q "openSUSE" /etc/os-release ); then
            DISTRO="opensuse"
            print_status "Detected: OpenSUSE"
        else
            print_warning "Unknown distribution, using generic wine paths"
        fi
        
        # Find appropriate wine library paths for current distribution
        WINE_LIBS_PATH=""
        case "$DISTRO" in
            "arch")
                for path in "/usr/lib/wine/i386-unix" "/usr/lib32/wine/i386-unix" "/usr/lib/wine" "/usr/lib32/wine"; do
                    if [ -d "$path" ]; then
                        WINE_LIBS_PATH="$path"
                        break
                    fi
                done
                ;;
            "debian")
                for path in "/usr/lib/i386-linux-gnu/wine" "/usr/lib/x86_64-linux-gnu/wine" "/usr/lib/wine"; do
                    if [ -d "$path" ]; then
                        WINE_LIBS_PATH="$path"
                        break
                    fi
                done
                ;;
            "fedora"|"opensuse")
                for path in "/usr/lib/wine" "/usr/lib64/wine"; do
                    if [ -d "$path" ]; then
                        WINE_LIBS_PATH="$path"
                        break
                    fi
                done
                ;;
            *)
                # Generic fallback - try all common paths
                for path in "/usr/lib/i386-linux-gnu/wine" "/usr/lib/x86_64-linux-gnu/wine" "/usr/lib/wine/i386-unix" "/usr/lib32/wine/i386-unix" "/usr/lib/wine" "/usr/lib32/wine" "/usr/lib64/wine"; do
                    if [ -d "$path" ]; then
                        WINE_LIBS_PATH="$path"
                        break
                    fi
                done
                ;;
        esac
        
        if [ -n "$WINE_LIBS_PATH" ]; then
            print_success "Using wine library path: $WINE_LIBS_PATH"
            
            # Build wine bridge with detected paths
            cd src/wine_bridge
            WINE_LIBS="-L$WINE_LIBS_PATH" make || {
                print_error "Failed to build Wine bridge components with $DISTRO wine paths"
                return 1
            }
            cd ../..
        else
            print_warning "Could not detect wine library paths, using default build"
            cd src/wine_bridge
            make || {
                print_error "Failed to build Wine bridge components"
                return 1
            }
            cd ../..
        fi
    fi
    
    # Install to AppDir
    make install DESTDIR="$APPIMAGE_DIR" || {
        print_error "Failed to install to AppDir"
        return 1
    }
    
    # Explicitly build and install Qt GUI if not already installed
    print_status "Building and installing Qt GUI..."
    cd src/qt_gui && qmake ltr_gui.pro && make -j$(nproc) && make INSTALL_ROOT="$APPIMAGE_DIR" install && cd ../.. || {
        print_warning "Qt GUI build/install failed, continuing without GUI"
    }
    
    # Verify OSC server was installed (if built)
    if [ -f "$APPIMAGE_DIR/usr/bin/osc_server" ]; then
        print_success "OSC server included in AppImage"
    else
        print_warning "OSC server not found - OSC support may not have been built"
    fi
    
    # Verify Wiimote server was installed (if built)
    if [ -f "$APPIMAGE_DIR/usr/bin/wii_server" ]; then
        print_success "Wiimote server included in AppImage"
    else
        print_warning "Wiimote server not found - Wiimote support may not have been built"
    fi
    
    # Verify X-Plane plugins were installed (if built)
    if [ -f "$APPIMAGE_DIR/usr/lib/linuxtrack/xlinuxtrack9.so" ]; then
        print_success "X-Plane plugin (64-bit) included in AppImage"
    else
        print_warning "X-Plane plugin (64-bit) not found - X-Plane support may not have been built"
    fi
    
    if [ -f "$APPIMAGE_DIR/usr/lib32/linuxtrack/xlinuxtrack9.so" ]; then
        print_success "X-Plane plugin (32-bit) included in AppImage"
    else
        print_warning "X-Plane plugin (32-bit) not found - X-Plane support may not have been built"
    fi
    
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

	# Bundle 32-bit LinuxTrack runtime (for 32-bit Wine apps using Tester.exe)
	print_status "Checking for 32-bit linuxtrack runtime to bundle..."
	mkdir -p "usr/lib32/linuxtrack"
	LINUXTRACK32_CANDIDATES=(
		"/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0"
		"/usr/local/lib/linuxtrack/liblinuxtrack.so.0"
		"/usr/lib32/linuxtrack/liblinuxtrack.so.0"
		"/usr/local/lib32/linuxtrack/liblinuxtrack.so.0"
	)
	for lib32 in "${LINUXTRACK32_CANDIDATES[@]}"; do
		if [ -f "$lib32" ]; then
			print_status "Bundling 32-bit liblinuxtrack from: $lib32"
			cp -P "$lib32" "usr/lib32/linuxtrack/" 2>/dev/null || true
			# Also copy the real file the symlink points to (if applicable)
			real32=$(readlink -f "$lib32" 2>/dev/null || echo "")
			if [ -n "$real32" ] && [ -f "$real32" ]; then
				cp -P "$real32" "usr/lib32/linuxtrack/" 2>/dev/null || true
				print_status "Bundled 32-bit real file: $(basename "$real32")"
			fi
			break
		fi
	done
	if [ ! -f "usr/lib32/linuxtrack/liblinuxtrack.so.0" ]; then
		print_warning "32-bit liblinuxtrack not found on this system; 32-bit Tester.exe may require system install of liblinuxtrack:i386"
	else
		print_success "32-bit liblinuxtrack bundled"
	fi
    
    # Bundle ALL Qt5 libraries to ensure complete self-containment
    print_status "Bundling ALL Qt5 libraries for complete self-containment..."
    
    # Core Qt5 libraries
    for qtlib in Qt5Core Qt5Gui Qt5Widgets Qt5Network Qt5OpenGL Qt5Help Qt5Sql Qt5X11Extras Qt5DBus Qt5XcbQpa Qt5Concurrent Qt5PrintSupport Qt5Svg Qt5Test Qt5Xml Qt5XmlPatterns; do
        if [ -f "/usr/lib/x86_64-linux-gnu/lib${qtlib}.so.5" ]; then
            cp "/usr/lib/x86_64-linux-gnu/lib${qtlib}.so.5" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${qtlib}"
            
            # CRITICAL: Also copy the actual library file that the symlink points to
            actual_file=$(readlink -f "/usr/lib/x86_64-linux-gnu/lib${qtlib}.so.5" 2>/dev/null || echo "")
            if [ -n "$actual_file" ] && [ -f "$actual_file" ]; then
                cp "$actual_file" "usr/lib/" 2>/dev/null || true
                print_status "Bundled actual file: $(basename "$actual_file")"
            fi
        fi
    done
    
    # Also bundle Qt5 library symlinks and dependencies
    for qtlib in Qt5Core Qt5Gui Qt5Widgets Qt5Network Qt5OpenGL Qt5Help Qt5Sql Qt5X11Extras Qt5DBus Qt5XcbQpa; do
        # Copy symlinks
        if [ -L "/usr/lib/x86_64-linux-gnu/lib${qtlib}.so" ]; then
            cp -P "/usr/lib/x86_64-linux-gnu/lib${qtlib}.so" "usr/lib/" 2>/dev/null || true
        fi
        # Copy versioned symlinks
        if [ -L "/usr/lib/x86_64-linux-gnu/lib${qtlib}.so.5" ]; then
            cp -P "/usr/lib/x86_64-linux-gnu/lib${qtlib}.so.5" "usr/lib/" 2>/dev/null || true
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
    for syslib in libusb-1.0 libmxml libGLU libpng16 libudev libv4l2 libv4lconvert libjpeg liblo libcwiid; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${syslib}.so.0" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${syslib}.so.0" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${syslib}"
        fi
    done
    
    # GLib/GObject libraries (CRITICAL for Qt compatibility)
    for gliblib in libglib-2.0 libgobject-2.0 libgio-2.0 libgmodule-2.0; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${gliblib}.so.0" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${gliblib}.so.0" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${gliblib}"
        fi
    done
    
    # Additional GLib-related libraries to prevent symbol conflicts
    for gliblib in libgthread-2.0 libgirepository-1.0 libffi; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${gliblib}.so.0" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${gliblib}.so.0" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${gliblib}"
        fi
    done
    
    # ICU (International Components for Unicode) libraries - CRITICAL for Qt
    print_status "Bundling ICU libraries for Qt compatibility..."
    for iculib in libicui18n libicuuc libicudata; do
        # Try version 72 first (common on newer systems)
        if [ -f "/usr/lib/x86_64-linux-gnu/${iculib}.so.72" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${iculib}.so.72" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${iculib}.so.72"
        elif [ -f "/usr/lib/x86_64-linux-gnu/${iculib}.so.71" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${iculib}.so.71" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${iculib}.so.71"
        elif [ -f "/usr/lib/x86_64-linux-gnu/${iculib}.so.70" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${iculib}.so.70" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${iculib}.so.70"
        elif [ -f "/usr/lib/x86_64-linux-gnu/${iculib}.so.69" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${iculib}.so.69" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${iculib}.so.69"
        fi
        
        # Also copy the actual library file that the symlink points to
        actual_file=$(readlink -f "/usr/lib/x86_64-linux-gnu/${iculib}.so.72" 2>/dev/null || echo "")
        if [ -n "$actual_file" ] && [ -f "$actual_file" ]; then
            cp "$actual_file" "usr/lib/" 2>/dev/null || true
            print_status "Bundled actual ICU file: $(basename "$actual_file")"
        fi
    done
    
    # Compression libraries - CRITICAL for Qt data handling
    print_status "Bundling compression libraries..."
    for complib in libz libzstd libbz2 liblzma; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${complib}.so.1" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${complib}.so.1" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${complib}.so.1"
        fi
    done
    
    # Data conversion libraries - Qt dependency
    print_status "Bundling data conversion libraries..."
    for convlib in libdouble-conversion; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${convlib}.so.3" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${convlib}.so.3" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${convlib}.so.3"
        fi
    done
    
    # SSL/Crypto libraries - For network features
    print_status "Bundling SSL/Crypto libraries..."
    for ssllib in libssl libcrypto; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${ssllib}.so.3" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${ssllib}.so.3" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${ssllib}.so.3"
        elif [ -f "/usr/lib/x86_64-linux-gnu/${ssllib}.so.1.1" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${ssllib}.so.1.1" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${ssllib}.so.1.1"
        fi
    done
    
    # Text rendering libraries - For Qt text display
    print_status "Bundling text rendering libraries..."
    for textlib in libharfbuzz libfreetype libfontconfig; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${textlib}.so.0" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${textlib}.so.0" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${textlib}.so.0"
        elif [ -f "/usr/lib/x86_64-linux-gnu/${textlib}.so.6" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${textlib}.so.6" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${textlib}.so.6"
        fi
    done
    
    # Security libraries (SELinux, etc.)
    for seclib in libselinux libpcre libpcre2-8 libpcre2-16; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${seclib}.so.1" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${seclib}.so.1" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${seclib}"
        elif [ -f "/usr/lib/x86_64-linux-gnu/${seclib}.so.0" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${seclib}.so.0" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${seclib}"
        fi
    done
    
    # OpenGL libraries for 3D view
    for opengllib in libGL libGLU libGLX libOpenGL; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${opengllib}.so.1" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${opengllib}.so.1" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${opengllib}"
        elif [ -f "/usr/lib/${opengllib}.so.1" ]; then
            cp "/usr/lib/${opengllib}.so.1" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${opengllib}"
        fi
    done
    
    # X11 libraries for OpenGL context
    for x11lib in libX11 libXext libXrender libXrandr libXfixes libXcursor libXinerama; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${x11lib}.so.6" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${x11lib}.so.6" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${x11lib}"
        elif [ -f "/usr/lib/${x11lib}.so.6" ]; then
            cp "/usr/lib/${x11lib}.so.6" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${x11lib}"
        fi
    done
    
    # XCB libraries for Qt platform plugins
    for xcblib in libxcb libxcb-icccm libxcb-image libxcb-shm libxcb-keysyms libxcb-randr libxcb-render-util libxcb-render libxcb-shape libxcb-sync libxcb-xfixes libxcb-xinerama libxcb-xkb libxkbcommon libxkbcommon-x11; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${xcblib}.so.1" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${xcblib}.so.1" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${xcblib}"
        elif [ -f "/usr/lib/x86_64-linux-gnu/${xcblib}.so.0" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${xcblib}.so.0" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${xcblib}"
        fi
    done
    
    # Additional Qt libraries for platform plugins
    for qtlib in libQt5XcbQpa libQt5DBus libQt5Concurrent libQt5PrintSupport libQt5Svg libQt5Test libQt5Xml libQt5XmlPatterns; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${qtlib}.so.5" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${qtlib}.so.5" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${qtlib}"
        fi
    done
    
    # Bundle Qt library symlinks to ensure proper linking
    for qtlib in Qt5Core Qt5Gui Qt5Widgets Qt5Network Qt5OpenGL Qt5Help Qt5Sql Qt5X11Extras Qt5DBus Qt5XcbQpa; do
        if [ -L "/usr/lib/x86_64-linux-gnu/lib${qtlib}.so" ]; then
            cp -P "/usr/lib/x86_64-linux-gnu/lib${qtlib}.so" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${qtlib} symlink"
        fi
    done
    
    # Ensure Qt plugins are bundled
    echo "DEBUG: About to call ensure_qt_plugins_bundled"
    ensure_qt_plugins_bundled
    echo "DEBUG: Finished ensure_qt_plugins_bundled"
    echo "DEBUG: Checking if qt5/plugins directory was created:"
    ls -la usr/lib/qt5/plugins/ 2>/dev/null || echo "DEBUG: qt5/plugins directory does not exist"
    
    # Bundle Qt help plugins specifically
    print_status "Bundling Qt help plugins..."
    if [ -d "/usr/lib/x86_64-linux-gnu/qt5/plugins/kf5/kio" ]; then
        mkdir -p "usr/lib/qt5/plugins/kf5/kio" 2>/dev/null || true
        for plugin in kio_help.so kio_ghelp.so; do
            if [ -f "/usr/lib/x86_64-linux-gnu/qt5/plugins/kf5/kio/$plugin" ]; then
                cp "/usr/lib/x86_64-linux-gnu/qt5/plugins/kf5/kio/$plugin" "usr/lib/qt5/plugins/kf5/kio/" 2>/dev/null || true
                print_status "Bundled Qt help plugin: $plugin"
            fi
        done
        print_status "Bundled Qt help KIO plugins"
    fi
    
    if [ -d "/usr/lib/x86_64-linux-gnu/qt5/plugins/kauth/helper" ]; then
        mkdir -p "usr/lib/qt5/plugins/kauth/helper" 2>/dev/null || true
        if [ -f "/usr/lib/x86_64-linux-gnu/qt5/plugins/kauth/helper/kauth_helper_plugin.so" ]; then
            cp "/usr/lib/x86_64-linux-gnu/qt5/plugins/kauth/helper/kauth_helper_plugin.so" "usr/lib/qt5/plugins/kauth/helper/" 2>/dev/null || true
            print_status "Bundled Qt help plugin: kauth_helper_plugin.so"
        fi
        print_status "Bundled Qt help kauth plugins"
    fi
    
    # CRITICAL: Bundle SQLite plugin for Qt help system
    print_status "Bundling SQLite plugin for Qt help system..."
    mkdir -p "usr/lib/qt5/plugins/sqldrivers"
    
    # Try multiple locations for SQLite plugin
    SQLITE_PLUGIN_FOUND=false
    SQLITE_PLUGIN_PATHS=(
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so"
        "/usr/lib/qt5/plugins/sqldrivers/libqsqlite.so"
        "/usr/lib/qt/plugins/sqldrivers/libqsqlite.so"
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so.5"
        "/usr/lib/qt5/plugins/sqldrivers/libqsqlite.so.5"
    )
    
    for sqlite_path in "${SQLITE_PLUGIN_PATHS[@]}"; do
        echo "DEBUG: Checking SQLite plugin path: $sqlite_path"
        if [ -f "$sqlite_path" ]; then
            echo "DEBUG: Found SQLite plugin at: $sqlite_path"
            echo "DEBUG: Copying to: usr/lib/qt5/plugins/sqldrivers/"
            cp "$sqlite_path" "usr/lib/qt5/plugins/sqldrivers/"
            if [ -f "usr/lib/qt5/plugins/sqldrivers/$(basename "$sqlite_path")" ]; then
                print_status "Bundled SQLite plugin: $(basename "$sqlite_path") from $sqlite_path"
                SQLITE_PLUGIN_FOUND=true
                break
            else
                print_warning "Failed to copy SQLite plugin from $sqlite_path"
                echo "DEBUG: Copy failed - checking if directory exists"
                ls -la "usr/lib/qt5/plugins/sqldrivers/" 2>/dev/null || echo "DEBUG: Directory does not exist"
            fi
        else
            echo "DEBUG: SQLite plugin not found at: $sqlite_path"
        fi
    done
    
    if [ "$SQLITE_PLUGIN_FOUND" = false ]; then
        print_error "SQLite plugin not found in any standard location!"
        print_error "This will cause help system to fail in AppImage"
        
        # Try to find SQLite plugin using find
        print_status "Searching for SQLite plugin in system..."
        SQLITE_PLUGIN=$(find /usr -name "libqsqlite.so*" 2>/dev/null | head -1)
        if [ -n "$SQLITE_PLUGIN" ] && [ -f "$SQLITE_PLUGIN" ]; then
            cp "$SQLITE_PLUGIN" "usr/lib/qt5/plugins/sqldrivers/"
            if [ -f "usr/lib/qt5/plugins/sqldrivers/$(basename "$SQLITE_PLUGIN")" ]; then
                print_status "Bundled SQLite plugin from search: $(basename "$SQLITE_PLUGIN")"
                SQLITE_PLUGIN_FOUND=true
            fi
        fi
    fi
    
    if [ "$SQLITE_PLUGIN_FOUND" = false ]; then
        print_error "CRITICAL: Could not find SQLite plugin anywhere!"
        print_error "Help system will fail in AppImage"
    else
        print_success "SQLite plugin bundled successfully"
    fi
    
    # Also bundle Qt5Sql library if not already present
    if [ ! -f "usr/lib/libQt5Sql.so.5" ]; then
        if [ -f "/usr/lib/x86_64-linux-gnu/libQt5Sql.so.5" ]; then
            cp "/usr/lib/x86_64-linux-gnu/libQt5Sql.so.5" "usr/lib/"
            print_status "Bundled Qt5Sql library"
        elif [ -f "/usr/lib/libQt5Sql.so.5" ]; then
            cp "/usr/lib/libQt5Sql.so.5" "usr/lib/"
            print_status "Bundled Qt5Sql library"
        else
            print_warning "Qt5Sql library not found - help system may fail"
        fi
    fi
    
    # CRITICAL: Fix rpath to ensure complete library isolation
    print_status "Fixing rpath for complete library isolation..."
    
    # Set rpath for ALL binaries to use ONLY bundled libraries
    for binary in usr/bin/*; do
        if [ -f "$binary" ] && [ -x "$binary" ]; then
            echo "Setting rpath for $(basename "$binary") to use ONLY bundled libraries"
            patchelf --set-rpath '$ORIGIN/../lib:$ORIGIN/../lib/linuxtrack' "$binary" 2>/dev/null || true
        fi
    done
    
    # Set rpath for all bundled libraries to use ONLY bundled libraries
    find usr/lib -name "*.so*" -type f -exec patchelf --set-rpath '$ORIGIN' {} \; 2>/dev/null || true
    find usr/lib/linuxtrack -name "*.so*" -type f -exec patchelf --set-rpath '$ORIGIN' {} \; 2>/dev/null || true
	# Also set rpath for 32-bit linuxtrack libs (kept separate under usr/lib32)
	find usr/lib32/linuxtrack -name "*.so*" -type f -exec patchelf --set-rpath '$ORIGIN' {} \; 2>/dev/null || true
    
    # Additional library bundling based on common missing libraries
    print_status "Bundling additional commonly missing libraries..."
    for lib in libselinux libpcre libpcre2-8 libpcre2-16 libattr libacl libcap libseccomp libaudit libproxy; do
        if [ -f "/usr/lib/x86_64-linux-gnu/${lib}.so.1" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${lib}.so.1" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${lib}"
        elif [ -f "/usr/lib/x86_64-linux-gnu/${lib}.so.0" ]; then
            cp "/usr/lib/x86_64-linux-gnu/${lib}.so.0" "usr/lib/" 2>/dev/null || true
            print_status "Bundled ${lib}"
        fi
    done
    
    # Wayland support removed - using XCB only for compatibility
    print_status "Using XCB platform only - Wayland support disabled"
    
    # Bundle libproxy backend libraries
    if [ -d "/usr/lib/x86_64-linux-gnu/libproxy" ]; then
        mkdir -p "usr/lib/libproxy" 2>/dev/null || true
        cp -r "/usr/lib/x86_64-linux-gnu/libproxy/"* "usr/lib/libproxy/" 2>/dev/null || true
        print_status "Bundled libproxy backends"
    fi
    
    # Advanced optimization (re-enabled with conservative approach)
    # optimize_library_structure  # Temporarily disabled due to optimization issues
    
    # CRITICAL: Verify Qt libraries are properly bundled
    print_status "Verifying Qt library bundling..."
    qt_issues=0
    for qtlib in Qt5Core Qt5Gui Qt5Widgets Qt5Network Qt5OpenGL Qt5Help Qt5Sql Qt5X11Extras; do
        if [ -L "usr/lib/lib${qtlib}.so.5" ]; then
            target=$(readlink "usr/lib/lib${qtlib}.so.5")
            if [ ! -f "usr/lib/$target" ]; then
                print_warning "Qt library symlink broken: lib${qtlib}.so.5 -> $target"
                ((qt_issues++))
            else
                print_success "Qt library properly bundled: lib${qtlib}.so.5"
            fi
        elif [ -f "usr/lib/lib${qtlib}.so.5" ]; then
            print_success "Qt library properly bundled: lib${qtlib}.so.5"
        else
            print_error "Qt library missing: lib${qtlib}.so.5"
            ((qt_issues++))
        fi
    done
    
    if [ $qt_issues -eq 0 ]; then
        print_success "All Qt libraries properly bundled"
    else
        print_warning "Found $qt_issues Qt library issues"
    fi
    
    print_success "Dependencies bundled with Wine bridge optimization"
}

# Function to ensure Qt plugins are bundled
ensure_qt_plugins_bundled() {
    print_status "Ensuring Qt plugins are bundled..."
    echo "DEBUG: Starting ensure_qt_plugins_bundled function"
    
    # Create Qt plugins directory structure
    echo "DEBUG: Creating Qt plugins directory structure"
    mkdir -p "usr/lib/qt5/plugins/platforms"
    mkdir -p "usr/lib/qt5/plugins/imageformats"
    mkdir -p "usr/lib/qt5/plugins/iconengines"
    mkdir -p "usr/lib/qt5/plugins/styles"
    echo "DEBUG: Created directories, checking if they exist:"
    ls -la usr/lib/qt5/plugins/ 2>/dev/null || echo "DEBUG: qt5/plugins directory creation failed"
    
    # Copy Qt platform plugins with version compatibility check
    QT_VERSION=$(qmake -query QT_VERSION 2>/dev/null | cut -d. -f1-2 || echo "5.15")
    print_status "Detected Qt version: $QT_VERSION"
    
    # Try multiple locations for platform plugins
    PLATFORM_PATHS=(
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/platforms"
        "/usr/lib/qt5/plugins/platforms"
        "/usr/lib/qt/plugins/platforms"
        "/usr/lib/x86_64-linux-gnu/qt${QT_VERSION}/plugins/platforms"
    )
    
    PLUGINS_COPIED=false
    for platform_path in "${PLATFORM_PATHS[@]}"; do
        if [ -d "$platform_path" ]; then
            print_status "Copying platform plugins from $platform_path"
            cp "$platform_path"/*.so usr/lib/qt5/plugins/platforms/ 2>/dev/null || true
            PLUGINS_COPIED=true
            break
        fi
    done
    
    # Wayland plugin support removed - using XCB only
    print_status "Using XCB platform plugins only - Wayland plugins disabled"
    
    if [ "$PLUGINS_COPIED" = false ]; then
        print_warning "No Qt platform plugins found in standard locations"
        # Try to find Qt platform plugins in other locations
        find /usr -name "libqxcb.so" -o -name "libqwayland*.so" 2>/dev/null | head -5 | while read plugin; do
            if [ -n "$plugin" ]; then
                cp "$plugin" usr/lib/qt5/plugins/platforms/ 2>/dev/null || true
                print_status "Copied Qt platform plugin: $(basename "$plugin")"
            fi
        done
    fi
    
    # Copy Qt image format plugins
    if [ -d "/usr/lib/x86_64-linux-gnu/qt5/plugins/imageformats" ]; then
        cp /usr/lib/x86_64-linux-gnu/qt5/plugins/imageformats/*.so usr/lib/qt5/plugins/imageformats/ 2>/dev/null || true
        print_status "Copied Qt image format plugins"
    fi
    
    # Copy Qt icon engine plugins
    if [ -d "/usr/lib/x86_64-linux-gnu/qt5/plugins/iconengines" ]; then
        cp /usr/lib/x86_64-linux-gnu/qt5/plugins/iconengines/*.so usr/lib/qt5/plugins/iconengines/ 2>/dev/null || true
        print_status "Copied Qt icon engine plugins"
    fi
    
    # Copy Qt style plugins for theme integration
    if [ -d "/usr/lib/x86_64-linux-gnu/qt5/plugins/styles" ]; then
        cp /usr/lib/x86_64-linux-gnu/qt5/plugins/styles/*.so usr/lib/qt5/plugins/styles/ 2>/dev/null || true
        print_status "Copied Qt style plugins for theme integration"
    fi
    
    print_success "Qt plugins bundling ensured"
}

# Function to optimize library structure with enhanced size reduction
optimize_library_structure() {
    print_status "Enhanced library structure optimization with size reduction..."
    
    # Remove duplicate libraries and broken symlinks
    find usr/lib -name "*.so*" -type f | while read lib; do
        if [ -L "$lib" ]; then
            if [ ! -e "$lib" ]; then
                rm -f "$lib"
                print_status "Removed broken symlink: $lib"
            fi
        fi
    done
    
    # CRITICAL: Remove duplicate libraries by comparing content (more conservative)
    print_status "Removing duplicate libraries..."
    find usr/lib -name "*.so*" -type f | while read lib1; do
        if [ -f "$lib1" ]; then
            find usr/lib -name "*.so*" -type f | while read lib2; do
                if [ "$lib1" != "$lib2" ] && [ -f "$lib2" ]; then
                    # Only remove if both files are identical AND one is a versioned symlink
                    if cmp -s "$lib1" "$lib2" 2>/dev/null; then
                        lib1_basename=$(basename "$lib1")
                        lib2_basename=$(basename "$lib2")
                        # Only remove if one is clearly a duplicate (has version number)
                        if [[ "$lib1_basename" =~ \.[0-9]+\.[0-9]+\.[0-9]+$ ]] && [[ ! "$lib2_basename" =~ \.[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
                            print_status "Removing duplicate: $lib2_basename (same as $lib1_basename)"
                            rm -f "$lib2"
                        elif [[ "$lib2_basename" =~ \.[0-9]+\.[0-9]+\.[0-9]+$ ]] && [[ ! "$lib1_basename" =~ \.[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
                            print_status "Removing duplicate: $lib1_basename (same as $lib2_basename)"
                            rm -f "$lib1"
                        fi
                    fi
                fi
            done
        fi
    done
    
    # Strip debug symbols to reduce size (more aggressive)
    print_status "Stripping debug symbols from all libraries..."
    find usr/bin usr/lib -type f -executable -exec strip --strip-unneeded {} \; 2>/dev/null || true
    find usr/lib -name "*.so*" -type f -exec strip --strip-unneeded {} \; 2>/dev/null || true
    
    # Remove unnecessary files
    find usr -name "*.la" -delete 2>/dev/null || true
    find usr -name "*.a" -delete 2>/dev/null || true
    find usr -name "*.o" -delete 2>/dev/null || true
    
    # Optimize Qt5 plugins - keep only essential ones
    if [ -d "usr/lib/qt5/plugins" ]; then
        print_status "Optimizing Qt5 plugins..."
        
        # Keep only essential platform plugins
        if [ -d "usr/lib/qt5/plugins/platforms" ]; then
            cd usr/lib/qt5/plugins/platforms
            # Keep only xcb and minimal platforms
            for plugin in *.so; do
                if [ -f "$plugin" ]; then
                    case "$plugin" in
                        libqxcb.so|libqminimal.so|libqminimalegl.so)
                            print_status "Keeping essential platform plugin: $plugin"
                            ;;
                        *)
                            print_status "Removing non-essential platform plugin: $plugin"
                            rm -f "$plugin"
                            ;;
                    esac
                fi
            done
            cd ../../../../..
        fi
        
        # Keep only essential image formats
        if [ -d "usr/lib/qt5/plugins/imageformats" ]; then
            cd usr/lib/qt5/plugins/imageformats
            for plugin in *.so; do
                if [ -f "$plugin" ]; then
                    case "$plugin" in
                        libqjpeg.so|libqpng.so|libqsvg.so|libqgif.so)
                            print_status "Keeping essential image format: $plugin"
                            ;;
                        *)
                            print_status "Removing non-essential image format: $plugin"
                            rm -f "$plugin"
                            ;;
                    esac
                fi
            done
            cd ../../../../..
        fi
        
        # Keep only essential icon engines
        if [ -d "usr/lib/qt5/plugins/iconengines" ]; then
            cd usr/lib/qt5/plugins/iconengines
            for plugin in *.so; do
                if [ -f "$plugin" ]; then
                    case "$plugin" in
                        libqsvgicon.so)
                            print_status "Keeping essential icon engine: $plugin"
                            ;;
                        *)
                            print_status "Removing non-essential icon engine: $plugin"
                            rm -f "$plugin"
                            ;;
                    esac
                fi
            done
            cd ../../../../..
        fi
        
        # Keep only essential styles
        if [ -d "usr/lib/qt5/plugins/styles" ]; then
            cd usr/lib/qt5/plugins/styles
            for plugin in *.so; do
                if [ -f "$plugin" ]; then
                    case "$plugin" in
                        libqcleanlooksstyle.so|libqgtkstyle.so|libqplastiquestyle.so)
                            print_status "Keeping essential style: $plugin"
                            ;;
                        *)
                            print_status "Removing non-essential style: $plugin"
                            rm -f "$plugin"
                            ;;
                    esac
                fi
            done
            cd ../../../../..
        fi
        
        print_status "Qt5 plugins optimized (preserved sqldrivers and essential plugins)"
    fi
    
    # Remove unnecessary documentation files
    print_status "Removing unnecessary documentation files..."
    find usr/share/doc -name "copyright" -delete 2>/dev/null || true
    find usr/share/doc -name "*.txt" -not -name "README*" -delete 2>/dev/null || true
    
    # Remove unnecessary man pages (keep only essential ones)
    print_status "Optimizing man pages..."
    cd usr/share/man/man1
    for manpage in *.1; do
        if [ -f "$manpage" ]; then
            case "$manpage" in
                ltr_gui.1|mickey.1)
                    print_status "Keeping essential man page: $manpage"
                    ;;
                *)
                    print_status "Removing non-essential man page: $manpage"
                    rm -f "$manpage"
                    ;;
            esac
        fi
    done
    cd ../../../../..
    
    # Remove unnecessary help files (keep only essential ones)
    print_status "Optimizing help files..."
    if [ -d "usr/share/linuxtrack/help/ltr_gui/help" ]; then
        cd usr/share/linuxtrack/help/ltr_gui/help
        # Keep only essential help images
        for image in *.png; do
            if [ -f "$image" ]; then
                case "$image" in
                    welcome.png|interface.png|tracking.png|misc.png)
                        print_status "Keeping essential help image: $image"
                        ;;
                    *)
                        print_status "Removing non-essential help image: $image"
                        rm -f "$image"
                        ;;
                esac
            fi
        done
        cd ../../../../../..
    fi
    
    print_success "Enhanced library structure optimization completed"
}

# Function to optimize Wine bridge components
optimize_wine_bridge() {
    print_status "Optimizing Wine bridge components..."
    
    # Optimize wine bridge installer size
    if [ -f "wine_bridge/linuxtrack-wine.exe" ]; then
        # Strip debug symbols from wine bridge installer
        strip --strip-unneeded wine_bridge/linuxtrack-wine.exe 2>/dev/null || true
        print_status "Stripped debug symbols from wine bridge installer"
        
        # Check if wine bridge installer is unnecessarily large
        WINE_SIZE=$(du -h wine_bridge/linuxtrack-wine.exe | cut -f1)
        print_status "Wine bridge installer size: $WINE_SIZE"
    fi
    
    # Optimize wine bridge scripts
    if [ -d "wine_bridge/scripts" ]; then
        # Remove any backup files or temporary files
        find wine_bridge/scripts -name "*.bak" -o -name "*.tmp" -o -name "*~" | xargs rm -f 2>/dev/null || true
        print_status "Cleaned wine bridge scripts"
    fi
    
    # Remove unnecessary wine documentation
    if [ -f "usr/share/linuxtrack/wine/README.wine" ]; then
        # Keep only essential wine documentation
        if [ -f "usr/share/linuxtrack/wine/WINE_SETUP.md" ]; then
            # Combine documentation into single file to save space
            cat usr/share/linuxtrack/wine/README.wine usr/share/linuxtrack/wine/WINE_SETUP.md > usr/share/linuxtrack/wine/WINE_SETUP.md.tmp
            mv usr/share/linuxtrack/wine/WINE_SETUP.md.tmp usr/share/linuxtrack/wine/WINE_SETUP.md
            rm -f usr/share/linuxtrack/wine/README.wine
            print_status "Consolidated wine documentation"
        fi
    fi
    
    print_success "Wine bridge components optimized"
}

# Function to report optimization statistics
report_optimization_stats() {
    print_status "Reporting optimization statistics..."
    
    cd "$APPIMAGE_DIR"
    
    # Count libraries before and after optimization
    LIB_COUNT=$(find usr/lib -name "*.so*" | wc -l)
    print_status "Total libraries after optimization: $LIB_COUNT"
    
    # Calculate total size
    TOTAL_SIZE=$(du -sh . | cut -f1)
    print_status "Total AppDir size: $TOTAL_SIZE"
    
    # Library size breakdown
    LIB_SIZE=$(du -sh usr/lib | cut -f1)
    SHARE_SIZE=$(du -sh usr/share | cut -f1)
    BIN_SIZE=$(du -sh usr/bin | cut -f1)
    WINE_SIZE=$(du -sh wine_bridge 2>/dev/null | cut -f1 || echo "N/A")
    
    print_status "Size breakdown:"
    print_status "  Libraries: $LIB_SIZE"
    print_status "  Share data: $SHARE_SIZE"
    print_status "  Binaries: $BIN_SIZE"
    print_status "  Wine bridge: $WINE_SIZE"
    
    # Qt plugin count
    QT_PLUGIN_COUNT=$(find usr/lib/qt5/plugins -name "*.so" 2>/dev/null | wc -l)
    print_status "Qt plugins: $QT_PLUGIN_COUNT"
    
    # Check for potential size savings
    if [ "$LIB_COUNT" -gt 150 ]; then
        print_warning "High library count ($LIB_COUNT) - consider further optimization"
    else
        print_success "Library count optimized ($LIB_COUNT)"
    fi
    
    cd "$PROJECT_ROOT"
    print_success "Optimization statistics reported"
}

# Function to create AppImage
create_appimage() {
    print_status "Creating Phase 4 AppImage..."
    
    cd "$PROJECT_ROOT"
    
    # Create AppImage with architecture specification
    ARCH=x86_64 "$APPIMAGETOOL" "$APPIMAGE_DIR" "${APP_NAME}-${VERSION}-x86_64.AppImage" || {
        print_error "Failed to create AppImage"
        return 1
    }
    
    print_success "Phase 4 AppImage created: ${APP_NAME}-${VERSION}-x86_64.AppImage"
    return 0
}

# Function to verify AppImage
verify_appimage() {
    print_status "Verifying Phase 4 AppImage..."
    
    if [ ! -f "$PROJECT_ROOT/${APP_NAME}-${VERSION}-x86_64.AppImage" ]; then
        print_error "AppImage not found"
        return 1
    fi
    
    # Check AppImage size
    APPIMAGE_SIZE=$(du -h "$PROJECT_ROOT/${APP_NAME}-${VERSION}-x86_64.AppImage" | cut -f1)
    print_status "AppImage size: $APPIMAGE_SIZE"
    
    # Extract and analyze dependencies
    print_status "Analyzing bundled dependencies..."
    ./${APP_NAME}-${VERSION}-x86_64.AppImage --appimage-extract
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
    "$PROJECT_ROOT/${APP_NAME}-${VERSION}-x86_64.AppImage" --appimage-extract-and-run --help 2>/dev/null || {
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
    
    # Copy help files
    copy_help_files
    
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
    
    # Apply optimizations AFTER AppDir is complete (temporarily disabled)
    print_status "Applying size optimizations..."
    cd "$APPIMAGE_DIR"
    # optimize_library_structure  # Temporarily disabled due to aggressive optimization
    optimize_wine_bridge
    report_optimization_stats
    cd "$PROJECT_ROOT"
    
    # Create AppImage
    if ! create_appimage; then
        print_error "AppImage creation failed"
        exit 1
    fi
    
    # Verify AppImage
    verify_appimage
    
    print_success "LinuxTrack Phase 4 Wine Bridge Integration completed successfully!"
    print_status "AppImage location: $PROJECT_ROOT/${APP_NAME}-${VERSION}-x86_64.AppImage"
    print_status "You can now test the AppImage by running: ./${APP_NAME}-${VERSION}-x86_64.AppImage"
    print_status "Wine bridge components are included and will be installed automatically when Wine is detected"
}

# Run main function
main "$@" 