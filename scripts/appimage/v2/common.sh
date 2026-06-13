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

# Reject legacy AppImageKit appimagetool (embeds libfuse2-dependent runtime).
reject_legacy_appimagetool() {
    local tool="$1"
    [[ -e "$tool" ]] || die "appimagetool not found: $tool"
    chmod +x "$tool" 2>/dev/null || true
    local ver
    ver=$(APPIMAGE_EXTRACT_AND_RUN=1 "$tool" --version 2>/dev/null | head -1 || true)
    if [[ "$ver" == *"5735cc5"* ]]; then
        die "Legacy appimagetool ($ver). Run scripts/appimage/v2/fetch_toolchain.sh"
    fi
}

ensure_appimage_toolchain() {
    if [[ ! -f "$APPIMAGE_RUNTIME" ]] || [[ ! -e "$APPIMAGETOOL" ]]; then
        "$SCRIPT_DIR/fetch_toolchain.sh"
    fi
    if [[ -z "$(command -v appimagetool 2>/dev/null || true)" && -e "$LOCAL_APPIMAGETOOL" ]]; then
        APPIMAGETOOL="$LOCAL_APPIMAGETOOL"
    fi
    if [[ -f "$LOCAL_APPIMAGE_RUNTIME" && ! -f "${APPIMAGE_RUNTIME:-}" ]]; then
        APPIMAGE_RUNTIME="$LOCAL_APPIMAGE_RUNTIME"
    fi
    reject_legacy_appimagetool "$APPIMAGETOOL"
    [[ -f "$APPIMAGE_RUNTIME" ]] || die "type2 runtime not found: $APPIMAGE_RUNTIME (run fetch_toolchain.sh)"
}

# Fail if AppImage embeds legacy AppImageKit / libfuse.so.2 runtime.
verify_appimage_runtime() {
    local appimage="$1"
    [[ -f "$appimage" ]] || die "AppImage not found: $appimage"

    if strings "$appimage" 2>/dev/null | grep -q 'libfuse\.so\.2'; then
        die "AppImage contains legacy libfuse.so.2 runtime marker"
    fi

    local ver=""
    ver=$("$appimage" --appimage-version 2>/dev/null || true)

    if [[ "$ver" == "5735cc5" || ( "$ver" == *"5735cc5"* && "$ver" != *"type2-runtime"* ) ]]; then
        die "Legacy AppImageKit runtime embedded: $ver"
    fi
    if [[ "$ver" == *"type2-runtime"* ]]; then
        print_success "AppImage runtime OK: $ver"
        return 0
    fi

    # Fallback when AppImage cannot execute (e.g. sandbox without FUSE).
    local embedded
    embedded=$(strings "$appimage" 2>/dev/null | grep -m1 'type2-runtime/commit/' || true)
    if [[ -n "$embedded" ]]; then
        print_success "AppImage runtime OK: $embedded"
        return 0
    fi

    die "Expected type2-runtime in AppImage, --appimage-version='${ver:-<empty>}'"
}

# Qt6 qhelpgenerator: often not on default PATH. Fedora installs it under
# /usr/lib64/qt6/libexec/qhelpgenerator (package qt6-doctools), not always in .../bin/.
find_qhelpgenerator_path() {
    QHELPGENERATOR_PATH=""
    if command -v qhelpgenerator-qt6 >/dev/null 2>&1; then
        QHELPGENERATOR_PATH="$(command -v qhelpgenerator-qt6)"
        return 0
    fi
    local _qhg
    for _qhg in \
        /usr/lib64/qt6/bin/qhelpgenerator \
        /usr/lib/qt6/bin/qhelpgenerator \
        /usr/lib/x86_64-linux-gnu/qt6/bin/qhelpgenerator \
        /usr/lib64/qt6/libexec/qhelpgenerator \
        /usr/lib/qt6/libexec/qhelpgenerator \
        /usr/lib/x86_64-linux-gnu/qt6/libexec/qhelpgenerator
    do
        if [[ -x "$_qhg" ]]; then
            QHELPGENERATOR_PATH="$_qhg"
            return 0
        fi
    done
    if command -v qhelpgenerator >/dev/null 2>&1; then
        QHELPGENERATOR_PATH="$(command -v qhelpgenerator)"
        return 0
    fi
    return 1
}

require_qhelpgenerator() {
    find_qhelpgenerator_path || die "Missing qhelpgenerator. Fedora: sudo dnf install qt6-doctools (provides libexec/qhelpgenerator). Debian/Ubuntu: qt6-tools-dev-tools. Optionally add to PATH: /usr/lib64/qt6/bin:/usr/lib64/qt6/libexec"
}
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
    elif [[ "$lib" == *"/flexiblas/"* ]]; then
        patchelf --set-rpath '$ORIGIN/..:$ORIGIN' "$lib" 2>/dev/null || true
    elif [[ "$lib" == *"/plugins/"* ]]; then
        patchelf --set-rpath '$ORIGIN/../../lib:$ORIGIN' "$lib" 2>/dev/null || true
    else
        patchelf --set-rpath '$ORIGIN' "$lib" 2>/dev/null || true
    fi
}

write_minimal_apprun() {
    print_status "Writing AppRun"
    cat > "$APPDIR/AppRun" << 'EOF'
#!/usr/bin/env bash
APPDIR="$(dirname "$(readlink -f "$0")")"
export APPDIR

if [ -z "${HOME:-}" ]; then
  HOME="$(getent passwd "$(whoami)" 2>/dev/null | cut -d: -f6)"
  export HOME="${HOME:-/tmp}"
fi

# Library search path — bundled libs first, then linuxtrack plugins
_LTR_LD="$APPDIR/usr/lib"
[ -d "$APPDIR/usr/lib/flexiblas" ] && _LTR_LD="$_LTR_LD:$APPDIR/usr/lib/flexiblas"
export LD_LIBRARY_PATH="$_LTR_LD:$APPDIR/usr/lib/linuxtrack:$APPDIR/usr/lib/i386-linux-gnu/linuxtrack"
unset _LTR_LD

# FlexiBLAS → bundled OpenBLAS (avoids host /etc/flexiblasrc)
for _c in "$APPDIR/usr/lib/libopenblas.so.0" "$APPDIR/usr/lib/libopenblas.so" "$APPDIR/usr/lib"/libopenblas*.so*; do
    [ -f "$_c" ] && { export FLEXIBLAS_DEFAULT_LIBRARY="$_c"; break; }
done
unset _c

# Wine bridge library paths
if [ -f "$APPDIR/usr/lib/linuxtrack/liblinuxtrack32.so.0" ]; then
    export LINUXTRACK_LIBS="$APPDIR/usr/lib/linuxtrack/liblinuxtrack32.so.0:$APPDIR/usr/lib/linuxtrack/liblinuxtrack.so.0"
elif [ -f "$APPDIR/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0" ]; then
    export LINUXTRACK_LIBS="$APPDIR/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0:$APPDIR/usr/lib/linuxtrack/liblinuxtrack.so.0"
elif [ -f "$APPDIR/usr/lib/linuxtrack/liblinuxtrack.so.0" ]; then
    export LINUXTRACK_LIBS="$APPDIR/usr/lib/linuxtrack/liblinuxtrack.so.0"
fi

# Qt isolation
export QT_DISABLE_VERSION_CHECK=1
export QT_PLUGIN_PATH="$APPDIR/usr/plugins"
export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/plugins/platforms"
export QT_SQL_DRIVER_PATH="$APPDIR/usr/plugins/sqldrivers"
export QT_QPA_PLATFORM="xcb"
if [ -n "${LTR_XCB_GL_INTEGRATION:-}" ]; then
    export QT_XCB_GL_INTEGRATION="$LTR_XCB_GL_INTEGRATION"
fi
export QT_AUTO_SCREEN_SCALE_FACTOR=0
export QT_SCALE_FACTOR=1

# GLib isolation
export GIO_EXTRA_MODULES="$APPDIR/usr/lib/gio/modules"
export GSETTINGS_SCHEMA_DIR="$APPDIR/usr/share/glib-2.0/schemas"
unset G_DEBUG 2>/dev/null || true

# Logging
if [ "${LTR_QT_VERBOSE:-0}" = "1" ]; then
    export QT_DEBUG_PLUGINS=1
    export QT_LOGGING_RULES="qt.help.*=true;qt.qpa.gl=true;qt.qpa.*=false;qt.sql.*=true"
else
    export QT_DEBUG_PLUGINS=0
    export QT_LOGGING_RULES="qt.qpa.*=false;qt.sql.*=false;qt.help.*=false"
fi

# Fusion style avoids GTK CSS parse warnings
if [ "${LTR_USE_GTK_QT_STYLE:-0}" != "1" ]; then
    export QT_STYLE_OVERRIDE="${QT_STYLE_OVERRIDE:-Fusion}"
fi

# Help system — copy packaged files to writable location
HELP_ROOT="${XDG_DATA_HOME:-$HOME/.local/share}/linuxtrack/help"
mkdir -p "$HELP_ROOT/ltr_gui" "$HELP_ROOT/mickey" 2>/dev/null || true
for _comp in ltr_gui mickey; do
    for _f in help.qhc help.qch; do
        _src="$APPDIR/usr/share/linuxtrack/help/$_comp/$_f"
        [ -f "$_src" ] && cp -f "$_src" "$HELP_ROOT/$_comp/" 2>/dev/null && \
            chmod u+w "$HELP_ROOT/$_comp/$_f" 2>/dev/null || true
    done
    _csrc="$APPDIR/usr/share/linuxtrack/help/$_comp/content"
    [ -d "$_csrc" ] && [ ! -d "$HELP_ROOT/$_comp/content" ] && \
        cp -r "$_csrc" "$HELP_ROOT/$_comp/" 2>/dev/null || true
done
export QT_HELP_PATH="$HELP_ROOT"
unset HELP_ROOT _comp _f _src _csrc

unset QT_DIR QTDIR QT_SELECT QT4DIR QT5DIR

exec "$APPDIR/usr/bin/ltr_gui" "$@"
EOF
    chmod +x "$APPDIR/AppRun"
}

ensure_desktop_and_icons() {
    print_status "Ensuring desktop file and icons"
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

    ensure_dir "$APPDIR/usr/share/icons/hicolor/48x48/apps"
    ensure_dir "$APPDIR/usr/share/icons/hicolor/scalable/apps"

    copy_if_exists "$PROJECT_ROOT/src/linuxtrack.png" "$APPDIR/"
    copy_if_exists "$PROJECT_ROOT/src/linuxtrack.png" "$APPDIR/usr/share/icons/hicolor/48x48/apps/"
    copy_if_exists "$PROJECT_ROOT/src/linuxtrack.svg" "$APPDIR/usr/share/icons/hicolor/scalable/apps/"
}

copy_udev_rules_if_present() {
    print_status "Copying udev rules if present"
    ensure_dir "$APPDIR/udev/rules.d"
    copy_if_exists "$PROJECT_ROOT/src/99-TIR.rules" "$APPDIR/udev/rules.d/"
    copy_if_exists "$PROJECT_ROOT/src/99-PS3Eye.rules" "$APPDIR/udev/rules.d/"
    copy_if_exists "$PROJECT_ROOT/src/99-Mickey.rules" "$APPDIR/udev/rules.d/"
}
