#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Validate: auditing AppDir"

[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR. Run prepare.sh first."

failures=0

# True if usr/lib/linuxtrack has any lib<stem>.so or lib<stem>.so.* (file or symlink)
linuxtrack_lib_present() {
    local stem="$1"
    [[ -d "$APPDIR/usr/lib/linuxtrack" ]] || return 1
    [[ -n "$(find "$APPDIR/usr/lib/linuxtrack" -maxdepth 1 \( -name "lib${stem}.so" -o -name "lib${stem}.so.*" \) \( -type f -o -type l \) -print -quit 2>/dev/null)" ]]
}

# Ensure core binaries
for bin in usr/bin/ltr_gui; do
    if [[ ! -x "$APPDIR/$bin" ]]; then
        print_error "Missing or non-executable: $bin"
        failures=$((failures+1))
    fi
done

# Ensure TrackIR device detection libraries (accept versioned SONAME only; AppDir may omit unversioned symlinks)
for stem in tir ltusb1 ltr; do
    if linuxtrack_lib_present "$stem"; then
        print_status "Found TrackIR library: lib${stem}.so* in usr/lib/linuxtrack"
    else
        print_error "Missing TrackIR library: lib${stem}.so* under usr/lib/linuxtrack"
        failures=$((failures+1))
    fi
done

# PS3 Eye plugin (USB 1415:2000)
if linuxtrack_lib_present p3e; then
    print_success "Found PS3 Eye driver library (libp3e)"
else
    print_error "Missing PS3 Eye library: libp3e.so* under usr/lib/linuxtrack"
    failures=$((failures+1))
fi

# Webcam + face tracking (prepare.sh uses -DENABLE_WEBCAM=ON)
if linuxtrack_lib_present wc; then
    print_status "Found webcam driver library (libwc)"
    opencv_bundled=$({ find "$APPDIR/usr/lib" \( -type f -o -type l \) -name 'libopencv_*.so*' 2>/dev/null || true; } | wc -l)
    if [[ "$opencv_bundled" -gt 0 ]]; then
        print_success "OpenCV runtime libraries present in AppDir for libwc / facetrack drivers"
        flexiblas_bundled=$({ find "$APPDIR/usr/lib" \( -type f -o -type l \) -name 'libflexiblas*.so*' 2>/dev/null || true; } | wc -l)
        openblas_bundled=$({ find "$APPDIR/usr/lib" \( -type f -o -type l \) -name 'libopenblas*.so*' 2>/dev/null || true; } | wc -l)
        if [[ "$flexiblas_bundled" -eq 0 ]]; then
            print_warning "OpenCV present but no libflexiblas*.so in AppDir — Fedora/RHEL builds may abort at FlexiBLAS init (check bundle.sh)"
        else
            print_status "FlexiBLAS library present for OpenCV stack"
        fi
        if [[ "$openblas_bundled" -eq 0 ]]; then
            print_warning "OpenCV/FlexiBLAS stack but no libopenblas*.so in AppDir — set FLEXIBLAS_DEFAULT_LIBRARY may fail (check bundle.sh)"
        else
            print_status "OpenBLAS library present for FlexiBLAS backend"
        fi
        if [[ -d "$APPDIR/usr/lib/flexiblas" ]]; then
            print_status "FlexiBLAS backend directory present: usr/lib/flexiblas"
        fi
    else
        print_warning "No libopencv_*.so in AppDir — face tracking may fail if libwc or libp3eft link to OpenCV"
    fi
else
    print_warning "libwc not in AppDir (webcam support may be disabled in this build)"
fi

if linuxtrack_lib_present p3eft; then
    print_status "Found PS3 Eye facetrack library (libp3eft)"
    opencv_for_p3=$({ find "$APPDIR/usr/lib" \( -type f -o -type l \) -name 'libopencv_*.so*' 2>/dev/null || true; } | wc -l)
    if [[ "$opencv_for_p3" -eq 0 ]]; then
        print_warning "libp3eft present but no libopencv_*.so in AppDir — PS3 Eye face tracking may fail at runtime"
    elif [[ "$opencv_for_p3" -gt 0 ]]; then
        flexiblas_p3=$({ find "$APPDIR/usr/lib" \( -type f -o -type l \) -name 'libflexiblas*.so*' 2>/dev/null || true; } | wc -l)
        openblas_p3=$({ find "$APPDIR/usr/lib" \( -type f -o -type l \) -name 'libopenblas*.so*' 2>/dev/null || true; } | wc -l)
        [[ "$flexiblas_p3" -gt 0 ]] || print_warning "libp3eft + OpenCV but no libflexiblas*.so — FlexiBLAS may fail on Fedora/RHEL"
        [[ "$openblas_p3" -gt 0 ]] || print_warning "libp3eft + OpenCV but no libopenblas*.so — FlexiBLAS backend may be missing"
    fi
fi

# Ensure 3D assets exist (GL 3D view)
print_status "Validating 3D model assets"
for asset in usr/share/linuxtrack/sphere.obj usr/share/linuxtrack/sparow_opaq.obj usr/share/linuxtrack/sparow_glass.obj; do
    if [[ ! -f "$APPDIR/$asset" ]]; then
        print_error "Missing 3D model asset: $asset"
        failures=$((failures+1))
    else
        print_status "Found 3D asset: $asset"
    fi
done

# Check for USB libraries required for device detection (both required)
has_libusb=false
has_libudev=false
if [[ -f "$APPDIR/usr/lib/libusb-1.0.so.0" ]]; then
    print_status "Found USB library: libusb-1.0.so.0"
    has_libusb=true
fi
if [[ -f "$APPDIR/usr/lib/libudev.so.1" ]]; then
    print_status "Found USB library: libudev.so.1"
    has_libudev=true
fi
if [[ "$has_libusb" != true || "$has_libudev" != true ]]; then
    missing_list=()
    [[ "$has_libusb" == true ]] || missing_list+=("libusb-1.0.so.0")
    [[ "$has_libudev" == true ]] || missing_list+=("libudev.so.1")
    print_warning "USB libraries missing: ${missing_list[*]} - TrackIR device detection may fail"
fi

# Mini-XML (ltr_gui, ltr_extractor); linuxdeploy often omits on Fedora
if [[ -n "$(find "$APPDIR/usr/lib" \( -type f -o -type l \) -name 'libmxml.so*' -print -quit 2>/dev/null)" ]]; then
    print_status "Found Mini-XML library (libmxml) for GUI/tools"
else
    print_error "Missing libmxml.so* in usr/lib — ltr_gui will fail on clean systems (bundle.sh extra libs)"
    failures=$((failures+1))
fi

# Verify rpath settings on critical libraries
print_status "Verifying rpath settings on TrackIR libraries"
if command -v patchelf >/dev/null 2>&1; then
    for lib in usr/lib/linuxtrack/libtir.so usr/lib/linuxtrack/libtir.so.0 usr/lib/linuxtrack/libtir.so.0.0.0 usr/lib/linuxtrack/libltusb1.so usr/lib/linuxtrack/libltusb1.so.0 usr/lib/linuxtrack/libltusb1.so.0.0.0; do
        if [[ -f "$APPDIR/$lib" ]]; then
            rpath=$(patchelf --print-rpath "$APPDIR/$lib" 2>/dev/null || echo "NONE")
            if [[ "$rpath" == "NONE" || -z "$rpath" ]]; then
                print_warning "No rpath set for $(basename "$lib")"
            else
                print_status "rpath for $(basename "$lib"): $rpath"
            fi
        fi
    done
else
    print_warning "patchelf not available; skipping rpath checks"
fi

# Ensure Qt essentials
# Check for Qt plugins in new and legacy locations
if [[ ! -e "$APPDIR/usr/plugins/platforms/libqxcb.so" && ! -e "$APPDIR/usr/lib/qt5/plugins/platforms/libqxcb.so" ]]; then
    print_error "Qt platform plugin (xcb) missing"
    failures=$((failures+1))
fi
# Check SQLite driver in both locations (robust against no-match under pipefail)
SQLITE_USR_PLUGINS=$({ find "$APPDIR/usr/plugins/sqldrivers" -maxdepth 1 -type f -name 'libqsqlite.so*' 2>/dev/null || true; } | wc -l)
SQLITE_QT5_PLUGINS=$({ find "$APPDIR/usr/lib/qt5/plugins/sqldrivers" -maxdepth 1 -type f -name 'libqsqlite.so*' 2>/dev/null || true; } | wc -l)

if [[ $SQLITE_USR_PLUGINS -gt 0 ]] || [[ $SQLITE_QT5_PLUGINS -gt 0 ]]; then
    print_success "Qt SQLite driver present"
else
    print_error "Qt SQLite driver missing from both locations; help system will fail"
    failures=$((failures+1))
fi

# Check Qt Help system files comprehensively
print_status "Checking Qt Help system files"
HELP_LTR_GUI_QHC=false
HELP_LTR_GUI_QCH=false
HELP_MICKEY_QHC=false
HELP_MICKEY_QCH=false

[[ -f "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qhc" ]] && HELP_LTR_GUI_QHC=true
[[ -f "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qch" ]] && HELP_LTR_GUI_QCH=true
[[ -f "$APPDIR/usr/share/linuxtrack/help/mickey/help.qhc" ]] && HELP_MICKEY_QHC=true
[[ -f "$APPDIR/usr/share/linuxtrack/help/mickey/help.qch" ]] && HELP_MICKEY_QCH=true

# Check ltr_gui help files
if [[ "$HELP_LTR_GUI_QHC" = true ]] && [[ "$HELP_LTR_GUI_QCH" = true ]]; then
    print_success "Qt Help system files present (ltr_gui)"
else
    print_error "Qt Help system files missing from ltr_gui; help system will fail"
    failures=$((failures+1))
fi

# Check mickey help files
if [[ "$HELP_MICKEY_QHC" = true ]] && [[ "$HELP_MICKEY_QCH" = true ]]; then
    print_success "Qt Help system files present (mickey)"
else
    print_warning "Qt Help system files missing from mickey (optional component)"
fi

validate_help_db() {
    local path="$1"
    local label="$2"
    if ! sqlite3 "$path" ".tables" >/dev/null 2>&1; then
        print_error "${label} is not a valid SQLite database"
        failures=$((failures+1))
        return
    fi

    # Only enforce contents tables for .qch (content database). .qhc (collection) has different schema.
    if [[ "$path" == *.qch ]]; then
        local tables
        tables=$(sqlite3 "$path" ".tables" 2>/dev/null || true)
        if [[ "$tables" =~ ContentsTable && "$tables" =~ FileDataTable ]]; then
            print_success "${label} valid: required tables present"
        else
            print_error "${label} missing required tables (ContentsTable, FileDataTable)"
            failures=$((failures+1))
        fi
    else
        print_success "${label} opens as SQLite (schema acceptable for collection)"
    fi
}

# Check if help files are valid SQLite databases with required tables
if command -v sqlite3 >/dev/null 2>&1; then
    if [[ "$HELP_LTR_GUI_QCH" = true ]]; then
        validate_help_db "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qch" "ltr_gui help.qch"
    fi
    if [[ "$HELP_LTR_GUI_QHC" = true ]]; then
        validate_help_db "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qhc" "ltr_gui help.qhc"
    fi
    if [[ "$HELP_MICKEY_QCH" = true ]]; then
        validate_help_db "$APPDIR/usr/share/linuxtrack/help/mickey/help.qch" "mickey help.qch"
    fi
    if [[ "$HELP_MICKEY_QHC" = true ]]; then
        validate_help_db "$APPDIR/usr/share/linuxtrack/help/mickey/help.qhc" "mickey help.qhc"
    fi
else
    print_warning "sqlite3 not available; skipping detailed help database validation"
fi

# Check Qt Help and SQL module libraries (symlinks are normal; search qt5/lib and lib64 layouts)
print_status "Checking Qt Help, SQL, and OpenGL module libraries"
HELP_LIB=0
SQL_LIB=0
OPENGL_LIB=0
for _qt_root in "$APPDIR/usr/lib" "$APPDIR/usr/lib64" "$APPDIR/usr/lib/qt5/lib"; do
    [[ -d "$_qt_root" ]] || continue
    HELP_LIB=$((HELP_LIB + $(find "$_qt_root" \( -type f -o -type l \) -name 'libQt5Help.so*' 2>/dev/null | wc -l)))
    SQL_LIB=$((SQL_LIB + $(find "$_qt_root" \( -type f -o -type l \) -name 'libQt5Sql.so*' 2>/dev/null | wc -l)))
    OPENGL_LIB=$((OPENGL_LIB + $(find "$_qt_root" \( -type f -o -type l \) -name 'libQt5OpenGL.so*' 2>/dev/null | wc -l)))
done
unset _qt_root

if [[ $HELP_LIB -gt 0 ]]; then
    print_success "Qt5Help library present"
else
    print_error "Qt5Help library missing; help system will fail"
    failures=$((failures+1))
fi

if [[ $SQL_LIB -gt 0 ]]; then
    print_success "Qt5Sql library present"
else
    print_error "Qt5Sql library missing; help system will fail"
    failures=$((failures+1))
fi

if [[ $OPENGL_LIB -gt 0 ]]; then
    print_success "Qt5OpenGL library present"
else
    print_error "Qt5OpenGL library missing; 3D view / GL init will fail on clean systems"
    failures=$((failures+1))
fi

# Check Qt Help plugins comprehensively
print_status "Checking Qt Help plugins"
HELP_PLUGINS_USR=$({ find "$APPDIR/usr/plugins/help" -type f -name "*.so" 2>/dev/null || true; } | wc -l)
HELP_PLUGINS_QT5=$({ find "$APPDIR/usr/lib/qt5/plugins/help" -type f -name "*.so" 2>/dev/null || true; } | wc -l)
KIO_HELP_PLUGINS=$({ find "$APPDIR/usr/plugins/kauth/helper" -type f -name "*.so" 2>/dev/null || true; } | wc -l)
KIO_KF5_PLUGINS=$({ find "$APPDIR/usr/plugins/kf5/kio" -type f -name "*.so" 2>/dev/null || true; } | wc -l)

# Total help plugins found
TOTAL_HELP_PLUGINS=$((HELP_PLUGINS_USR + HELP_PLUGINS_QT5))
TOTAL_KIO_PLUGINS=$((KIO_HELP_PLUGINS + KIO_KF5_PLUGINS))

if [[ $TOTAL_HELP_PLUGINS -gt 0 ]]; then
    print_success "Qt Help plugins found: $TOTAL_HELP_PLUGINS total (usr/plugins: $HELP_PLUGINS_USR, usr/lib/qt5/plugins: $HELP_PLUGINS_QT5)"
    
    # List specific plugins found
    if [[ $HELP_PLUGINS_USR -gt 0 ]]; then
        echo "  usr/plugins/help:"
        find "$APPDIR/usr/plugins/help" -type f -name "*.so" 2>/dev/null | while read -r plugin; do
            echo "    - $(basename "$plugin")"
        done
    fi
    
    if [[ $HELP_PLUGINS_QT5 -gt 0 ]]; then
        echo "  usr/lib/qt5/plugins/help:"
        find "$APPDIR/usr/lib/qt5/plugins/help" -type f -name "*.so" 2>/dev/null | while read -r plugin; do
            echo "    - $(basename "$plugin")"
        done
    fi
else
    print_warning "No Qt Help plugins found - help system will have limited functionality"
fi

if [[ $TOTAL_KIO_PLUGINS -gt 0 ]]; then
    print_success "KIO Help plugins found: $TOTAL_KIO_PLUGINS total (kauth/helper: $KIO_HELP_PLUGINS, kf5/kio: $KIO_KF5_PLUGINS)"
    
    # List specific KIO plugins found
    if [[ $KIO_HELP_PLUGINS -gt 0 ]]; then
        echo "  usr/plugins/kauth/helper:"
        find "$APPDIR/usr/plugins/kauth/helper" -type f -name "*.so" 2>/dev/null | while read -r plugin; do
            echo "    - $(basename "$plugin")"
        done
    fi
    
    if [[ $KIO_KF5_PLUGINS -gt 0 ]]; then
        echo "  usr/plugins/kf5/kio:"
        find "$APPDIR/usr/plugins/kf5/kio" -type f -name "*.so" 2>/dev/null | while read -r plugin; do
            echo "    - $(basename "$plugin")"
        done
    fi
else
    print_status "No KIO Help plugins found (optional enhancement)"
fi

# Ensure 32-bit linuxtrack runtime is present when wine bridge is enabled
if [[ "${WITH_WINE_BRIDGE:-1}" == "1" ]]; then
    print_status "Checking 32-bit linuxtrack runtime for Wine"
    LTR32_PATH="$APPDIR/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0"
    LTR32_ALT="$APPDIR/usr/lib/linuxtrack/liblinuxtrack32.so.0"
    if [[ -f "$LTR32_PATH" ]]; then
        print_success "32-bit liblinuxtrack present: ${LTR32_PATH#$APPDIR/}"
    elif [[ -f "$LTR32_ALT" ]]; then
        print_success "32-bit liblinuxtrack present: ${LTR32_ALT#$APPDIR/}"
    else
        print_error "Missing 32-bit liblinuxtrack (expected usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0 or usr/lib/linuxtrack/liblinuxtrack32.so.0)"
        failures=$((failures+1))
    fi
fi

# ldd audit: ensure libs resolve inside AppDir, except allowlist
allow_regex="^/(lib|lib64|usr/lib|usr/lib64)/(drm|dri|nvidia|amd|mesa|pci)"  # GPUs/drivers allowed
# Broader system stack allowances (graphics, X11, fonts, IPC). These are typically provided by host.
system_allow_patterns=(
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libGL(dispatch|X)?\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libOpenGL\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libX(11|au|dmcp|render|ext|randr|fixes|cursor|inerama)?\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libxcb(.*)?\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libfontconfig\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libfreetype\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libharfbuzz\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libgraphite2\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libdbus-1\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libsystemd\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libgcrypt\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libgpg-error\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libuuid\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libpng16\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libexpat\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/lib(deflate|lz4|lzma|zstd)\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/lib(md|md4c)\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/lib(SM|ICE)\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libjpeg\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libv4l2\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libv4lconvert\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libxkbcommon(-x11)?\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libpcre2-(8|16)\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libicu(uc|i18n|data)\\.so\\.'
  '^/(lib|usr/lib|lib64|usr/lib64)/.*/libdouble-conversion\\.so\\.'
)
# Additionally allow core glibc toolchain libs to reside on host (common AppImage practice)
toolchain_allow=(
  "/lib/x86_64-linux-gnu/libc.so.6"
  "/lib/x86_64-linux-gnu/libm.so.6"
  "/lib/x86_64-linux-gnu/libpthread.so.0"
  "/lib/x86_64-linux-gnu/libdl.so.2"
  "/lib/x86_64-linux-gnu/librt.so.1"
  "/lib/x86_64-linux-gnu/libgcc_s.so.1"
  "/lib/x86_64-linux-gnu/libstdc++.so.6"
  "/lib/x86_64-linux-gnu/libresolv.so.2"
  "/lib/x86_64-linux-gnu/libz.so.1"
  # Fedora/RHEL (ldd resolves to /lib64 and /usr/lib64)
  "/lib64/libc.so.6"
  "/lib64/libm.so.6"
  "/lib64/libpthread.so.0"
  "/lib64/libdl.so.2"
  "/lib64/librt.so.1"
  "/lib64/libgcc_s.so.1"
  "/lib64/libstdc++.so.6"
  "/usr/lib64/libgcc_s.so.1"
  "/usr/lib64/libstdc++.so.6"
  "/lib64/libresolv.so.2"
  "/lib64/libz.so.1"
  "/lib64/libnss_dns.so.2"
  "/lib64/libnss_files.so.2"
  "/lib64/ld-linux-x86-64.so.2"
)
# OpenCV / protobuf / gRPC stacks on Fedora pull Abseil, Samba private DSOs, etc. — never bundle these; treat as host stack.
ldd_host_noise_patterns=(
  '^/usr/lib64/samba/.*\\.so'
  '^/lib64/libabsl_.*\\.so'
  '^/usr/lib64/libabsl_.*\\.so'
  '^/lib64/libutf8_(range|validity)[^/]*\\.so'
  '^/usr/lib64/libutf8_(range|validity)[^/]*\\.so'
  '^/lib64/libre2\\.so'
  '^/usr/lib64/libre2\\.so'
  '^/lib64/lib(P|p)cre[^/]*\\.so'
  '^/usr/lib64/lib(P|p)cre[^/]*\\.so'
  '^/lib64/libssl\\.so'
  '^/usr/lib64/libssl\\.so'
  '^/lib64/libcrypto\\.so'
  '^/usr/lib64/libcrypto\\.so'
  '^/lib64/libgssapi.*\\.so'
  '^/usr/lib64/libgssapi.*\\.so'
  '^/lib64/libkrb5.*\\.so'
  '^/usr/lib64/libkrb5.*\\.so'
  '^/lib64/libcom_err\\.so'
  '^/usr/lib64/libcom_err\\.so'
  '^/lib64/libkeyutils\\.so'
  '^/usr/lib64/libkeyutils\\.so'
  '^/lib64/libcares\\.so'
  '^/usr/lib64/libcares\\.so'
  '^/lib64/libnghttp2\\.so'
  '^/usr/lib64/libnghttp2\\.so'
  '^/lib64/libunistring\\.so'
  '^/usr/lib64/libunistring\\.so'
  '^/lib64/libffi\\.so'
  '^/usr/lib64/libffi\\.so'
)

if command -v ldd >/dev/null 2>&1; then
    print_status "Running ldd audit (set VALIDATE_LDD_VERBOSE=1 for per-line host deps; noisy OpenCV/Fedora stacks are summarized)"
    critical_external_deps=0
    ldd_external_other=0
    _ltr_verbose="${VALIDATE_LDD_VERBOSE:-0}"

    while IFS= read -r -d '' elf; do
        while IFS= read -r line; do
            so_path=$(awk '/=>/{print $3}' <<<"$line" | tr -d ' ' || true)
            [[ -z "$so_path" ]] && continue
            [[ "$so_path" = "not" ]] && continue

            if [[ "$so_path" != $APPDIR/* ]]; then
                # Critical: USB stack must be bundled for device access (match soname loosely; Fedora paths differ)
                if [[ "$so_path" == *libusb-1.0.so* ]]; then
                    if [[ -n "$(find "$APPDIR/usr/lib" \( -type f -o -type l \) -name 'libusb-1.0.so*' -print -quit 2>/dev/null)" ]]; then
                        [[ "$_ltr_verbose" == "1" ]] && print_warning "External libusb path but bundled copy exists: $elf -> $so_path"
                    else
                        print_error "Critical external dep (should be bundled): $elf -> $so_path"
                        critical_external_deps=$((critical_external_deps + 1))
                    fi
                    continue
                fi
                if [[ "$so_path" == *libudev.so* ]]; then
                    if [[ -n "$(find "$APPDIR/usr/lib" \( -type f -o -type l \) -name 'libudev.so*' -print -quit 2>/dev/null)" ]]; then
                        [[ "$_ltr_verbose" == "1" ]] && print_warning "External libudev path but bundled copy exists: $elf -> $so_path"
                    else
                        print_error "Critical external dep (should be bundled): $elf -> $so_path"
                        critical_external_deps=$((critical_external_deps + 1))
                    fi
                    continue
                fi

                if [[ "$so_path" =~ $allow_regex ]]; then
                    continue
                fi

                for allowed in "${toolchain_allow[@]}"; do
                    if [[ "$so_path" == "$allowed" ]]; then
                        continue 2
                    fi
                done

                for rx in "${system_allow_patterns[@]}"; do
                    if [[ "$so_path" =~ $rx ]]; then
                        continue 2
                    fi
                done

                for rx in "${ldd_host_noise_patterns[@]}"; do
                    if [[ "$so_path" =~ $rx ]]; then
                        continue 2
                    fi
                done

                ldd_external_other=$((ldd_external_other + 1))
                [[ "$_ltr_verbose" == "1" ]] && print_warning "External dep (acceptable): $elf -> $so_path"
            fi
        done < <(ldd "$elf" 2>/dev/null || true)
    done < <(find "$APPDIR/usr/bin" "$APPDIR/usr/lib" "$APPDIR/usr/lib/linuxtrack" -type f \( -perm -111 -o -name "*.so*" \) -print0 2>/dev/null)

    if [[ $critical_external_deps -gt 0 ]]; then
        print_error "Found $critical_external_deps critical external dependencies (libusb / libudev not bundled in usr/lib)"
        failures=$((failures + critical_external_deps))
    fi
    if [[ "$ldd_external_other" -gt 0 ]]; then
        print_status "ldd audit: $ldd_external_other additional host-only dependency resolutions (graphics/X11/etc.); omitted from log unless VALIDATE_LDD_VERBOSE=1"
    fi
    unset _ltr_verbose
else
    print_warning "ldd not available; skipping dependency audit"
fi

if [[ $failures -gt 0 ]]; then
    die "Validation failed with $failures issue(s)"
fi

print_success "Validation passed"


