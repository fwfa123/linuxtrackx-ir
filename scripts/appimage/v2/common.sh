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

# Enhanced help system setup with comprehensive error handling and fallbacks
HELP_RUNTIME_ROOT="${XDG_DATA_HOME:-$HOME/.local/share}/linuxtrack/help"
HELP_SYSTEM_STATUS="initializing"
HELP_SYSTEM_ERRORS=()
HELP_SYSTEM_WARNINGS=()

# Create help directories with comprehensive error handling
echo "Initializing help system runtime environment..."
if ! mkdir -p "$HELP_RUNTIME_ROOT/ltr_gui" "$HELP_RUNTIME_ROOT/mickey" 2>/dev/null; then
    error_msg="Could not create help directories in $HELP_RUNTIME_ROOT"
    HELP_SYSTEM_ERRORS+=("$error_msg")
    echo "Error: $error_msg"
    HELP_SYSTEM_STATUS="directory_creation_failed"
    
    # Try fallback location
    FALLBACK_HELP_ROOT="$HOME/.linuxtrack/help"
    echo "Attempting fallback location: $FALLBACK_HELP_ROOT"
    if mkdir -p "$FALLBACK_HELP_ROOT/ltr_gui" "$FALLBACK_HELP_ROOT/mickey" 2>/dev/null; then
        HELP_RUNTIME_ROOT="$FALLBACK_HELP_ROOT"
        echo "Success: Using fallback help location"
        HELP_SYSTEM_STATUS="using_fallback"
    else
        echo "Error: Fallback location also failed - help system will be disabled"
        HELP_SYSTEM_STATUS="completely_failed"
    fi
fi

# Copy help files with comprehensive error handling and validation
HELP_FILES_COPIED=0
HELP_COMPONENTS_READY=0

echo "Copying help system files to runtime location..."
for comp in ltr_gui mickey; do
    COMPONENT_READY=true
    COMPONENT_FILES=0
    
    echo "  Processing $comp component..."
    
    # Copy help database files (.qhc, .qch)
    for f in help.qhc help.qch; do
        source_file="$APPDIR/usr/share/linuxtrack/help/$comp/$f"
        target_file="$HELP_RUNTIME_ROOT/$comp/$f"
        
        if [ -f "$source_file" ]; then
            if cp -f "$source_file" "$target_file" 2>/dev/null; then
                if chmod u+w "$target_file" 2>/dev/null; then
                    echo "    ✓ Copied $f for $comp"
                    ((HELP_FILES_COPIED++))
                    ((COMPONENT_FILES++))
                else
                    warning_msg="Could not make $f writable for $comp"
                    HELP_SYSTEM_WARNINGS+=("$warning_msg")
                    echo "    ⚠️  $warning_msg"
                    COMPONENT_READY=false
                fi
            else
                error_msg="Failed to copy $f for $comp"
                HELP_SYSTEM_ERRORS+=("$error_msg")
                echo "    ✗ $error_msg"
                COMPONENT_READY=false
            fi
        else
            warning_msg="Help file $f not found for $comp"
            HELP_SYSTEM_WARNINGS+=("$warning_msg")
            echo "    ⚠️  $warning_msg"
            COMPONENT_READY=false
        fi
    done
    
    # Copy help content directories
    content_source="$APPDIR/usr/share/linuxtrack/help/$comp/content"
    content_target="$HELP_RUNTIME_ROOT/$comp/content"
    
    if [ -d "$content_source" ] && [ ! -d "$content_target" ]; then
        if cp -r "$content_source" "$content_target" 2>/dev/null; then
            echo "    ✓ Copied help content for $comp"
        else
            warning_msg="Failed to copy help content for $comp"
            HELP_SYSTEM_WARNINGS+=("$warning_msg")
            echo "    ⚠️  $warning_msg"
            COMPONENT_READY=false
        fi
    elif [ ! -d "$content_source" ]; then
        echo "    ℹ️  No help content directory for $comp"
    else
        echo "    ℹ️  Help content already exists for $comp"
    fi
    
    # Validate component readiness
    if [ "$COMPONENT_READY" = true ] && [ $COMPONENT_FILES -ge 2 ]; then
        echo "    ✅ $comp component ready ($COMPONENT_FILES files)"
        ((HELP_COMPONENTS_READY++))
    else
        echo "    ❌ $comp component not ready"
    fi
    
    echo ""
done

# Set help system environment variables
export QT_HELP_PATH="$HELP_RUNTIME_ROOT"

# Comprehensive help system validation and status reporting
echo "=== Help System Validation ==="

# Determine overall system status
if [ "$HELP_SYSTEM_STATUS" = "completely_failed" ]; then
    echo "❌ Help system completely failed - will be disabled"
    HELP_SYSTEM_STATUS="disabled"
elif [ $HELP_FILES_COPIED -gt 0 ] && [ $HELP_COMPONENTS_READY -gt 0 ]; then
    if [ "$HELP_SYSTEM_STATUS" = "using_fallback" ]; then
        HELP_SYSTEM_STATUS="ready_fallback"
        echo "✅ Help system ready using fallback location"
    else
        HELP_SYSTEM_STATUS="ready"
        echo "✅ Help system ready"
    fi
elif [ $HELP_FILES_COPIED -gt 0 ]; then
    HELP_SYSTEM_STATUS="partial"
    echo "⚠️  Help system partially functional"
else
    HELP_SYSTEM_STATUS="no_files"
    echo "❌ No help files copied - help system disabled"
fi

# Report detailed status
echo "  Status: $HELP_SYSTEM_STATUS"
echo "  Files copied: $HELP_FILES_COPIED"
echo "  Components ready: $HELP_COMPONENTS_READY/2"
echo "  Runtime location: $HELP_RUNTIME_ROOT"

# Report errors and warnings
if [ ${#HELP_SYSTEM_ERRORS[@]} -gt 0 ]; then
    echo "  Errors: ${#HELP_SYSTEM_ERRORS[@]}"
    for error in "${HELP_SYSTEM_ERRORS[@]}"; do
        echo "    ✗ $error"
    done
fi

if [ ${#HELP_SYSTEM_WARNINGS[@]} -gt 0 ]; then
    echo "  Warnings: ${#HELP_SYSTEM_WARNINGS[@]}"
    for warning in "${HELP_SYSTEM_WARNINGS[@]}"; do
        echo "    ⚠️  $warning"
    done
fi

# Set appropriate environment variables based on status
if [ "$HELP_SYSTEM_STATUS" = "disabled" ] || [ "$HELP_SYSTEM_STATUS" = "no_files" ]; then
    echo "  Note: Help system will be disabled at runtime"
    export QT_HELP_PATH=""
    unset QT_HELP_PATH
else
    echo "  Note: Help system will be available at runtime"
fi

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

# Final help system status and launch preparation
echo ""
echo "=== Help System Launch Summary ==="
echo "Final Status: $HELP_SYSTEM_STATUS"
echo "Files Available: $HELP_FILES_COPIED"
echo "Components Ready: $HELP_COMPONENTS_READY/2"
echo "Runtime Path: $HELP_RUNTIME_ROOT"

# Provide user guidance based on status
case "$HELP_SYSTEM_STATUS" in
    "ready"|"ready_fallback")
        echo "✅ Help system will be fully functional"
        echo "   Users can access help content through the application menu"
        ;;
    "partial")
        echo "⚠️  Help system will be partially functional"
        echo "   Some help content may be unavailable"
        ;;
    "disabled"|"no_files")
        echo "❌ Help system will be disabled"
        echo "   Users will not have access to help content"
        echo "   Application functionality will not be affected"
        ;;
    *)
        echo "❓ Help system status unclear"
        echo "   Behavior may be unpredictable"
        ;;
esac

echo ""
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


