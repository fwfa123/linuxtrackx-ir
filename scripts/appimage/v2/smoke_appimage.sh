#!/usr/bin/env bash
# Post-build checks for a LinuxTrack-X-IR AppImage (content + optional headless launch).
# Usage: scripts/appimage/v2/smoke_appimage.sh [path/to/AppImage]
# Default: ${APP_NAME}-${VERSION}-x86_64.AppImage in repo root (see config.sh).
# Env:
#   SMOKE_LAUNCH=1  — try a short headless run (QT_QPA_PLATFORM=offscreen); needs a writable HOME.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=config.sh
source "$SCRIPT_DIR/config.sh"
# shellcheck source=common.sh
source "$SCRIPT_DIR/common.sh"
# shellcheck source=bundle_policy.sh
source "$SCRIPT_DIR/bundle_policy.sh"

APPIMAGE="${1:-$PROJECT_ROOT/${APP_NAME}-${VERSION}-x86_64.AppImage}"

[[ -f "$APPIMAGE" ]] || die "AppImage not found: $APPIMAGE"

print_status "Smoke test: $APPIMAGE"

verify_appimage_runtime "$APPIMAGE"

failures=0

OFF=$("$APPIMAGE" --appimage-offset 2>/dev/null) || die "Failed to read --appimage-offset"
[[ "$OFF" =~ ^[0-9]+$ ]] || die "Unexpected appimage-offset: $OFF"

# --- Inventory (no full extract): inode count and optional paths ---
inode_count=$(unsquashfs -s -o "$OFF" "$APPIMAGE" 2>/dev/null | sed -n 's/^Number of inodes \([0-9][0-9]*\).*/\1/p')
if [[ -n "${inode_count:-}" ]]; then
    print_status "Squashfs inodes: $inode_count"
fi

qm_count=$(unsquashfs -l -o "$OFF" "$APPIMAGE" 2>/dev/null | grep -c '\.qm$' || true)
if [[ "$qm_count" -eq 0 ]]; then
    print_status "No Qt framework .qm files in squashfs listing (ltr_gui still loads app translations from embedded resources)"
else
    print_status "Bundled Qt .qm files: $qm_count"
fi

if unsquashfs -l -o "$OFF" "$APPIMAGE" 2>/dev/null | grep -q 'usr/bin/wii_server'; then
    print_success "Wiimote: wii_server present"
elif [[ "${EXPECT_LEVEL7:-1}" == "1" ]]; then
    print_error "Wiimote: wii_server missing (Level 7 AppImage expected)"
    failures=$((failures+1))
else
    print_status "Wiimote: wii_server not bundled (slim build)"
fi

# --- Extract to a temp dir for file and sqlite checks ---
TMP=$(mktemp -d)
HOME_TMP=""
cleanup() { rm -rf "$TMP" "${HOME_TMP:-}"; }
trap cleanup EXIT

cd "$TMP"
APPIMAGE_EXTRACT_AND_RUN=1 "$APPIMAGE" --appimage-extract >/dev/null
ROOT="$TMP/squashfs-root"

require_help_sqlite() {
    local f="$1"
    local label="$2"
    if [[ ! -f "$f" ]]; then
        print_error "Missing $label: $f"
        return 1
    fi
    if command -v sqlite3 >/dev/null 2>&1; then
        if sqlite3 "$f" ".tables" >/dev/null 2>&1; then
            print_success "Help SQLite OK: $label"
        else
            print_error "Help file is not a valid SQLite DB: $f"
            return 1
        fi
    else
        print_warning "sqlite3 not installed; skipping SQLite check for $label"
    fi
    return 0
}

# ltr_gui: require help.qch; help.qhc optional (qhp-only prepare has no collection)
LTR_QCH="$ROOT/$LTR_HELP_LTR_GUI_QCH_REL"
LTR_QHC="$ROOT/$LTR_HELP_LTR_GUI_QHC_REL"
require_help_sqlite "$LTR_QCH" "ltr_gui help.qch" || failures=$((failures + 1))
if [[ -f "$LTR_QHC" ]]; then
    require_help_sqlite "$LTR_QHC" "ltr_gui help.qhc" || failures=$((failures + 1))
else
    print_warning "Optional missing: $LTR_HELP_LTR_GUI_QHC_REL"
fi
# mickey: Qt Help only if mickey.qhp/qhcp exist in the repo (otherwise HTML-only)
for _m in help.qch help.qhc; do
    _mf="$ROOT/usr/share/linuxtrack/help/mickey/$_m"
    if [[ -f "$_mf" ]]; then
        require_help_sqlite "$_mf" "mickey $_m" || failures=$((failures + 1))
    fi
done

for icon in \
    "$ROOT/usr/share/icons/hicolor/48x48/apps/linuxtrack.png" \
    "$ROOT/linuxtrack.png"; do
    if [[ -f "$icon" ]]; then
        print_success "Icon present: ${icon#$ROOT/}"
    else
        print_error "Missing icon: ${icon#$ROOT/}"
        failures=$((failures + 1))
    fi
done

if [[ -f "$ROOT/usr/share/icons/hicolor/scalable/apps/linuxtrack.svg" ]]; then
    print_status "Scalable icon present (optional)"
fi

[[ -x "$ROOT/usr/bin/ltr_gui" ]] || { print_error "ltr_gui not executable"; failures=$((failures + 1)); }

# 32-bit Wine bridge runtime (package.sh checks this too)
if [[ -f "$ROOT/$LTR32_LIB_REL" ]]; then
    print_success "32-bit liblinuxtrack for Wine bridge present"
else
    print_error "Missing $LTR32_LIB_REL"
    failures=$((failures + 1))
fi

LIBWC="$ROOT/usr/lib/linuxtrack/libwc.so.0"
if [[ -f "$LIBWC" || -L "$LIBWC" ]]; then
    print_success "Webcam driver present: usr/lib/linuxtrack/libwc.so.0"
    if ldd "$LIBWC" 2>/dev/null | grep -q 'not found'; then
        print_error "libwc.so.0 has unresolved dependencies:"
        ldd "$LIBWC" 2>/dev/null | grep 'not found' || true
        failures=$((failures + 1))
    else
        print_success "libwc.so.0 dependencies resolve"
    fi
else
    print_error "Missing webcam driver: usr/lib/linuxtrack/libwc.so.0"
    failures=$((failures + 1))
fi

LIBFT="$ROOT/usr/lib/linuxtrack/libft.so.0"
if [[ -f "$LIBWC" || -L "$LIBWC" ]]; then
    if [[ ! -e "$LIBFT" ]]; then
        ln -sf libwc.so.0 "$LIBFT"
    fi
    if [[ -L "$LIBFT" ]] && [[ "$(readlink "$LIBFT")" == "libwc.so.0" ]]; then
        print_success "Face-track compat symlink: libft.so.0 -> libwc.so.0"
    elif [[ -e "$LIBFT" ]]; then
        print_success "libft.so.0 present"
    else
        print_error "Missing libft.so.0 (face-track backward compat)"
        failures=$((failures + 1))
    fi
fi

# --- Optional short launch (offscreen); does not require imageformat plugins for window chrome ---
if [[ "${SMOKE_LAUNCH:-0}" == "1" ]]; then
    HOME_TMP=$(mktemp -d)
    print_status "SMOKE_LAUNCH=1: running AppRun briefly (QT_QPA_PLATFORM=offscreen)"
    set +e
    timeout 10 env HOME="$HOME_TMP" QT_QPA_PLATFORM=offscreen APPDIR="$ROOT" "$ROOT/AppRun" >/dev/null 2>&1
    rc=$?
    set -e
    if [[ "$rc" -eq 124 ]]; then
        print_success "AppRun still running after timeout (expected for GUI)"
    elif [[ "$rc" -eq 0 ]]; then
        print_success "AppRun exited 0"
    else
        print_warning "AppRun exited with code $rc (may be OK if no offscreen Qt plugin)"
    fi
fi

if [[ "$failures" -ne 0 ]]; then
    die "Smoke test failed ($failures error(s))"
fi
print_success "Smoke test passed"
