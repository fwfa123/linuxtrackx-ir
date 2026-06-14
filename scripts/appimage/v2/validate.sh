#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"
# shellcheck source=bundle_policy.sh
source "$SCRIPT_DIR/bundle_policy.sh"

print_status "Validate: auditing AppDir"

[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR. Run prepare.sh first."

failures=0

linuxtrack_lib_present() {
    local stem="$1"
    [[ -d "$APPDIR/usr/lib/linuxtrack" ]] || return 1
    [[ -n "$(find "$APPDIR/usr/lib/linuxtrack" -maxdepth 1 \( -name "lib${stem}.so" -o -name "lib${stem}.so.*" \) \( -type f -o -type l \) -print -quit 2>/dev/null)" ]]
}

# ---- X-Plane plugin (optional; expected in Docker AppImage when EXPECT_XPLANE_PLUGIN=1) ----
if [[ "${EXPECT_XPLANE_PLUGIN:-0}" == "1" ]]; then
    if [[ -f "$APPDIR/usr/lib/linuxtrack/xlinuxtrack9.so" ]] || [[ -L "$APPDIR/usr/lib/linuxtrack/xlinuxtrack9.so" ]]; then
        print_success "X-Plane plugin present: usr/lib/linuxtrack/xlinuxtrack9.so"
    else
        print_error "X-Plane plugin missing: usr/lib/linuxtrack/xlinuxtrack9.so (ENABLE_XPLANE / SDK path)"
        failures=$((failures+1))
    fi
fi

# ---- Core binaries ----
for bin in usr/bin/ltr_gui; do
    if [[ ! -x "$APPDIR/$bin" ]]; then
        print_error "Missing or non-executable: $bin"
        failures=$((failures+1))
    fi
done

# ---- TrackIR device detection libraries ----
for stem in tir ltusb1 ltr; do
    if linuxtrack_lib_present "$stem"; then
        print_status "Found TrackIR library: lib${stem}.so*"
    else
        print_error "Missing TrackIR library: lib${stem}.so*"
        failures=$((failures+1))
    fi
done

# ---- PS3 Eye driver ----
if linuxtrack_lib_present p3e; then
    print_success "Found PS3 Eye driver library (libp3e)"
else
    print_error "Missing PS3 Eye library: libp3e.so*"
    failures=$((failures+1))
fi

# ---- Webcam driver ----
if linuxtrack_lib_present wc; then
    print_success "Found webcam driver library (libwc)"
    _libwc=$(find "$APPDIR/usr/lib/linuxtrack" -maxdepth 1 \( -name 'libwc.so.0' -o -name 'libwc.so.0.*' \) \( -type f -o -type l \) -print -quit 2>/dev/null)
    if [[ -n "$_libwc" ]] && command -v ldd >/dev/null 2>&1; then
        if ldd "$_libwc" 2>/dev/null | grep -q 'not found'; then
            print_error "libwc.so.0 has unresolved dependencies (dlopen will fail):"
            ldd "$_libwc" 2>/dev/null | grep 'not found' || true
            failures=$((failures+1))
        else
            print_success "libwc.so.0 dependencies resolve (dlopen-ready)"
        fi
    fi
    if [[ "${EXPECT_FACE_TRACKER:-1}" == "1" ]]; then
        _libft="$APPDIR/usr/lib/linuxtrack/libft.so.0"
        if [[ -e "$_libft" ]]; then
            print_success "Found face-track compat symlink (libft.so.0)"
        elif [[ -n "$_libwc" ]]; then
            print_warning "libft.so.0 missing (create libft -> libwc symlink for Autotools compat)"
        fi
    fi
else
    print_warning "libwc not in AppDir (webcam support may be disabled)"
fi

# ---- README Level 7: OpenCV face tracker + Wiimote ----
if [[ "${EXPECT_LEVEL7:-1}" == "1" ]]; then
    if [[ "${EXPECT_FACE_TRACKER:-1}" == "1" ]]; then
        if linuxtrack_lib_present p3eft; then
            print_success "Found face tracker library (libp3eft)"
        else
            print_error "Missing face tracker library: libp3eft (builder needs OpenCV; ENABLE_FACE_TRACKER=ON)"
            failures=$((failures+1))
        fi
    fi
    if [[ -x "$APPDIR/usr/bin/wii_server" ]]; then
        print_success "Wiimote UI present: usr/bin/wii_server"
    else
        print_error "Missing or non-executable: usr/bin/wii_server (builder needs libcwiid; DISABLE_WIIMOTE=OFF)"
        failures=$((failures+1))
    fi
fi

# ---- 3D assets ----
for asset in usr/share/linuxtrack/sphere.obj usr/share/linuxtrack/sparow_opaq.obj usr/share/linuxtrack/sparow_glass.obj; do
    if [[ ! -f "$APPDIR/$asset" ]]; then
        print_error "Missing 3D model asset: $asset"
        failures=$((failures+1))
    fi
done

# ---- Key bundled libraries ----
for lib_pattern in libusb-1.0.so libudev.so libmxml.so "libicu*.so" libQt6Core.so; do
    if [[ -n "$(find "$APPDIR/usr/lib" -maxdepth 1 \( -type f -o -type l \) -name "${lib_pattern}*" -print -quit 2>/dev/null)" ]]; then
        print_success "Bundled: $lib_pattern"
    else
        print_error "Missing bundled library: $lib_pattern"
        failures=$((failures+1))
    fi
done

# ---- Qt platform plugin ----
if [[ ! -e "$APPDIR/usr/plugins/platforms/libqxcb.so" ]]; then
    print_error "Qt platform plugin (xcb) missing from usr/plugins/platforms/"
    failures=$((failures+1))
fi

# ---- Qt SQLite driver ----
if [[ -n "$(find "$APPDIR/usr/plugins/sqldrivers" -maxdepth 1 -type f -name 'libqsqlite.so*' -print -quit 2>/dev/null)" ]]; then
    print_success "Qt SQLite driver present"
else
    print_error "Qt SQLite driver missing; help system will fail"
    failures=$((failures+1))
fi

# ---- Qt Help, Sql, OpenGLWidgets libraries ----
for _lib_name in libQt6Help libQt6Sql libQt6OpenGLWidgets; do
    if [[ -n "$(find "$APPDIR/usr/lib" -maxdepth 1 \( -type f -o -type l \) -name "${_lib_name}.so*" -print -quit 2>/dev/null)" ]]; then
        print_success "$_lib_name present"
    else
        print_error "$_lib_name missing"
        failures=$((failures+1))
    fi
done

# ---- Qt Help data files ----
if [[ -f "$APPDIR/$LTR_HELP_LTR_GUI_QCH_REL" ]]; then
    print_success "ltr_gui help.qch present"
else
    print_error "ltr_gui help.qch missing: $LTR_HELP_LTR_GUI_QCH_REL"
    failures=$((failures+1))
fi
if [[ -f "$APPDIR/$LTR_HELP_LTR_GUI_QHC_REL" ]]; then
    print_success "ltr_gui help.qhc present"
else
    print_warning "ltr_gui help.qhc missing (optional)"
fi

# ---- Help SQLite validation ----
if ! command -v sqlite3 >/dev/null 2>&1 && ! command -v python3 >/dev/null 2>&1; then
    print_warning "sqlite3 and python3 unavailable; skipping help SQLite checks"
else
    for _hf in "$APPDIR/$LTR_HELP_LTR_GUI_QCH_REL" "$APPDIR/$LTR_HELP_LTR_GUI_QHC_REL"; do
        [[ -f "$_hf" ]] || continue
        _help_sqlite_ok=0
        if command -v sqlite3 >/dev/null 2>&1 && sqlite3 "$_hf" ".tables" >/dev/null 2>&1; then
            _help_sqlite_ok=1
        elif command -v python3 >/dev/null 2>&1 && python3 -c "import sqlite3, sys; sqlite3.connect(sys.argv[1]).execute('select 1').fetchone()" "$_hf" >/dev/null 2>&1; then
            _help_sqlite_ok=1
        fi
        if [[ "$_help_sqlite_ok" == "1" ]]; then
            print_success "$(basename "$_hf") valid SQLite"
        else
            print_error "$(basename "$_hf") is not a valid SQLite database"
            failures=$((failures+1))
        fi
    done
fi

# ---- Wine bridge (MinGW PE payload + native install script; no NSIS) ----
if [[ "${WITH_WINE_BRIDGE:-1}" == "1" ]]; then
    if [[ -f "$APPDIR/wine_bridge/payload/NPClient.dll" ]]; then
        print_success "Wine bridge payload present: wine_bridge/payload/NPClient.dll"
    else
        print_error "Missing Wine bridge payload: wine_bridge/payload/NPClient.dll (MinGW build + wine_bridge.sh required)"
        failures=$((failures+1))
    fi
    if [[ -x "$APPDIR/wine_bridge/scripts/install_wine_bridge.sh" ]]; then
        print_success "Wine bridge install script present"
    else
        print_error "Missing or non-executable: wine_bridge/scripts/install_wine_bridge.sh"
        failures=$((failures+1))
    fi

    LTR32_PATH="$APPDIR/$LTR32_LIB_REL"
    LTR32_ALT="$APPDIR/usr/lib/linuxtrack/liblinuxtrack32.so.0"
    if [[ -f "$LTR32_PATH" ]]; then
        print_success "32-bit liblinuxtrack present: ${LTR32_PATH#$APPDIR/}"
    elif [[ -f "$LTR32_ALT" ]]; then
        print_success "32-bit liblinuxtrack present: ${LTR32_ALT#$APPDIR/}"
    else
        print_warning "Missing 32-bit liblinuxtrack (Wine 32-bit support unavailable)"
    fi
fi

# ---- rpath checks ----
if command -v patchelf >/dev/null 2>&1; then
    for _lib in usr/lib/linuxtrack/libtir.so.0.0.0 usr/lib/linuxtrack/libltusb1.so.0.0.0; do
        [[ -f "$APPDIR/$_lib" ]] || continue
        _rpath=$(patchelf --print-rpath "$APPDIR/$_lib" 2>/dev/null || echo "NONE")
        if [[ "$_rpath" == "NONE" || -z "$_rpath" ]]; then
            print_warning "No rpath set for $(basename "$_lib")"
        else
            print_status "rpath for $(basename "$_lib"): $_rpath"
        fi
    done
fi

# ---- ldd audit: flag libraries resolving outside AppDir ----
if command -v ldd >/dev/null 2>&1; then
    print_status "Running ldd audit"
    _ltr_verbose="${VALIDATE_LDD_VERBOSE:-0}"

    # These must always come from the host
    _host_allowed_re='^/(lib|lib64|usr/lib|usr/lib64)/(libc\.so|libm\.so|libpthread\.so|libdl\.so|librt\.so|ld-linux|libnss_|libresolv|libgcc_s|libstdc\+\+)'
    _gpu_allowed_re='^/(lib|lib64|usr/lib|usr/lib64)/.*(libGL|libEGL|libGLX|libOpenGL|libGLdispatch|libgbm|libdrm|libvulkan|nvidia|mesa|dri)'

    critical_missing=0
    _ldd_audit_paths=("$APPDIR/usr/bin" "$APPDIR/usr/lib")
    [[ -d "$APPDIR/usr/plugins" ]] && _ldd_audit_paths+=("$APPDIR/usr/plugins")
    while IFS= read -r -d '' elf; do
        while IFS= read -r line; do
            so_path=$(awk '/=>/{print $3}' <<<"$line" | tr -d ' ')
            [[ -z "$so_path" || "$so_path" == "not" ]] && continue
            [[ "$so_path" == "$APPDIR"/* ]] && continue

            [[ "$so_path" =~ $_host_allowed_re ]] && continue
            [[ "$so_path" =~ $_gpu_allowed_re ]] && continue

            if [[ "$_ltr_verbose" == "1" ]]; then
                print_warning "External: $(basename "$elf") -> $so_path"
            fi
            critical_missing=$((critical_missing + 1))
        done < <(ldd "$elf" 2>/dev/null || true)
    done < <(find "${_ldd_audit_paths[@]}" -type f \( -perm -111 -o -name "*.so*" \) -print0 2>/dev/null)

    if [[ $critical_missing -gt 0 ]]; then
        print_warning "ldd audit: $critical_missing external dependency resolution(s) (use VALIDATE_LDD_VERBOSE=1 for details)"
    else
        print_success "ldd audit: all dependencies resolve inside AppDir (or host-allowed)"
    fi
fi

if [[ $failures -gt 0 ]]; then
    die "Validation failed with $failures issue(s)"
fi

print_success "Validation passed"
