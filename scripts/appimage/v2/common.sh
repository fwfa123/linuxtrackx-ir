#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/config.sh"

# Logging
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() { echo -e "${BLUE}[INFO]${NC} $*"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $*"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $*"; }
print_error() { echo -e "${RED}[ERROR]${NC} $*"; }
die() { print_error "$*"; exit 1; }

require_cmd() { command -v "$1" >/dev/null 2>&1 || die "Missing required command: $1"; }
ensure_dir() { mkdir -p "$1"; }
copy_if_exists() { [[ -e "$1" ]] && cp -r "$1" "$2" || true; }

set_rpath_binary() {
    local bin="$1"
    require_cmd patchelf
    patchelf --set-rpath '$ORIGIN/../lib:$ORIGIN/../lib/linuxtrack' "$bin" 2>/dev/null || true
}

set_rpath_library() {
    local lib="$1"
    require_cmd patchelf
    if [[ "$lib" == *"/usr/lib/linuxtrack/"* ]]; then
        patchelf --set-rpath '$ORIGIN:$ORIGIN/..' "$lib" 2>/dev/null || true
    elif [[ "$lib" == *"/usr/lib/qt5/plugins/"* ]]; then
        patchelf --set-rpath '$ORIGIN/../../lib:$ORIGIN' "$lib" 2>/dev/null || true
    else
        patchelf --set-rpath '$ORIGIN' "$lib" 2>/dev/null || true
    fi
}

write_minimal_apprun() {
    print_status "Writing comprehensive AppRun with device detection support"
    cat > "$APPDIR/AppRun" << 'EOF'
#!/usr/bin/env bash
# AppRun - LinuxTrack AppImage launcher with comprehensive Qt isolation and device detection support

# Get the directory where the AppImage is mounted
APPDIR="$(dirname "$(readlink -f "$0")")"

# Set up completely isolated environment for self-contained AppImage
export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack:$APPDIR/usr/lib/i386-linux-gnu/linuxtrack"

# CRITICAL: Complete Qt isolation to prevent version mixing
export QT_DISABLE_VERSION_CHECK=1
export QT_LOGGING_RULES="qt.qpa.*=false"
export QT_DEBUG_PLUGINS=0
export QT_QPA_PLATFORM_PLUGIN_NAMES="xcb"

# Qt plugin paths - support both layouts
QT_PLUG_ROOT="$APPDIR/usr/plugins"
if [ -d "$QT_PLUG_ROOT" ]; then
    export QT_PLUGIN_PATH="$QT_PLUG_ROOT:$APPDIR/usr/lib/qt5/plugins"
    export QT_QPA_PLATFORM_PLUGIN_PATH="$QT_PLUG_ROOT/platforms"
    export QT_SQL_DRIVER_PATH="$QT_PLUG_ROOT/sqldrivers:$APPDIR/usr/lib/qt5/plugins/sqldrivers"
else
    export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins"
    export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
    export QT_SQL_DRIVER_PATH="$APPDIR/usr/lib/qt5/plugins/sqldrivers"
fi
export QT_STYLE_PATH="$APPDIR/usr/lib/qt5/plugins/styles"

# Force X11 usage to avoid Wayland compatibility issues
export QT_QPA_PLATFORM="xcb"
export QT_AUTO_SCREEN_SCALE_FACTOR=0
export QT_SCALE_FACTOR=1

# CRITICAL: Isolate GLib environment to prevent symbol conflicts
export GIO_EXTRA_MODULES="$APPDIR/usr/lib/gio/modules"
export GI_TYPELIB_PATH="$APPDIR/usr/lib/girepository-1.0"
export GSETTINGS_SCHEMA_DIR="$APPDIR/usr/share/glib-2.0/schemas"
export G_DEBUG="fatal-warnings"

# Help system debugging and configuration
export QT_DEBUG_PLUGINS=1
export QT_LOGGING_RULES="qt.help.*=true;qt.qpa.*=false;qt.sql.*=true"
export QT_HELP_PATH="$APPDIR/usr/share/linuxtrack/help"

# Additional Qt environment variables for help system
export QT_IMAGEIO_MAXALLOC=0
export QT_AUTO_SCREEN_SCALE_FACTOR=1

# Prevent system Qt libraries from being loaded
unset QT_DIR QTDIR QT_SELECT QT4DIR QT5DIR

# Ensure X11 is available
if ! xset q >/dev/null 2>&1; then
    echo "Warning: X11 server not responding, but continuing..."
    echo "If the application fails to start, try running: xhost +local:"
fi

# Debug output for device detection
echo "=== Help System Initialization Complete ==="

# Launch the application with proper library isolation
exec "$APPDIR/usr/bin/ltr_gui" "$@"
EOF
    chmod +x "$APPDIR/AppRun"
}

ensure_desktop_and_icons() {
    print_status "Ensuring desktop file and icons"
    # Use installed desktop if present; otherwise create a minimal one
    if [ ! -f "$APPDIR/usr/share/applications/linuxtrack.desktop" ]; then
        ensure_dir "$APPDIR/usr/share/applications"
        cat > "$APPDIR/usr/share/applications/linuxtrack.desktop" << EOF
[Desktop Entry]
Name=$APP_DISPLAY_NAME
Comment=Head tracking software for Linux
Exec=usr/bin/ltr_gui
Icon=linuxtrack
Terminal=false
Type=Application
Categories=Utility;Game;
Keywords=head;tracking;trackir;gaming;wine;
EOF
    fi

    # Icons
    ensure_dir "$APPDIR/usr/share/icons/hicolor/48x48/apps"
    ensure_dir "$APPDIR/usr/share/icons/hicolor/scalable/apps"

    # Prefer project icons if available
    copy_if_exists "$PROJECT_ROOT/src/linuxtrack.png" "$APPDIR/"
    copy_if_exists "$PROJECT_ROOT/src/linuxtrack.png" "$APPDIR/usr/share/icons/hicolor/48x48/apps/"
    copy_if_exists "$PROJECT_ROOT/src/linuxtrack.svg" "$APPDIR/usr/share/icons/hicolor/scalable/apps/"
}

copy_udev_rules_if_present() {
    print_status "Copying udev rules if present"
    ensure_dir "$APPDIR/udev/rules.d"
    copy_if_exists "$PROJECT_ROOT/src/99-TIR.rules" "$APPDIR/udev/rules.d/"
    copy_if_exists "$PROJECT_ROOT/src/99-Mickey.rules" "$APPDIR/udev/rules.d/"
}


