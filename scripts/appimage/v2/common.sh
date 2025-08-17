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
    print_status "Writing minimal AppRun"
    cat > "$APPDIR/AppRun" << 'EOF'
#!/usr/bin/env bash
set -e

APPDIR="$(dirname "$(readlink -f "$0")")"

# Minimal, production-safe Qt env
# Prefer linuxdeploy-plugin-qt layout under usr/plugins, then legacy path
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

# Ensure bundled libs are preferred
export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack"

# Prefer XCB by default for compatibility (can be overridden by env)
if [ "${FORCE_XCB:-1}" = "1" ] && [ -z "${QT_QPA_PLATFORM:-}" ]; then
    export QT_QPA_PLATFORM="xcb"
fi

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


