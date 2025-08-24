#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Validate: auditing AppDir"

[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR. Run prepare.sh first."

failures=0

# Ensure core binaries
for bin in usr/bin/ltr_gui; do
    if [[ ! -x "$APPDIR/$bin" ]]; then
        print_error "Missing or non-executable: $bin"
        failures=$((failures+1))
    fi
done

# Ensure TrackIR device detection libraries
for lib in usr/lib/linuxtrack/libtir.so usr/lib/linuxtrack/libltusb1.so usr/lib/linuxtrack/libltr.so; do
    if [[ ! -f "$APPDIR/$lib" ]]; then
        print_error "Missing TrackIR library: $lib"
        failures=$((failures+1))
    else
        print_status "Found TrackIR library: $lib"
    fi
done

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

# Check Qt Help system files
print_status "Checking Qt Help system files"
HELP_LTR_GUI_QHC=false
HELP_LTR_GUI_QCH=false
[[ -f "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qhc" ]] && HELP_LTR_GUI_QHC=true
[[ -f "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qch" ]] && HELP_LTR_GUI_QCH=true

if [[ "$HELP_LTR_GUI_QHC" = true ]] && [[ "$HELP_LTR_GUI_QCH" = true ]]; then
    print_success "Qt Help system files present (ltr_gui)"
else
    print_error "Qt Help system files missing from ltr_gui; help system will fail"
    failures=$((failures+1))
fi

# Check if help files are valid SQLite databases (optional but helpful)
if command -v sqlite3 >/dev/null 2>&1 && [[ "$HELP_LTR_GUI_QCH" = true ]]; then
    if sqlite3 "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qch" ".tables" >/dev/null 2>&1; then
        print_success "Qt Help content file (help.qch) is valid SQLite database"
    else
        print_warning "Qt Help content file (help.qch) appears to be corrupted or invalid"
    fi
fi

# Check Qt Help and SQL module libraries
print_status "Checking Qt Help and SQL module libraries"
HELP_LIB=$({ find "$APPDIR/usr/lib" -maxdepth 1 -type f -name 'libQt5Help.so*' 2>/dev/null || true; } | wc -l)
SQL_LIB=$({ find "$APPDIR/usr/lib" -maxdepth 1 -type f -name 'libQt5Sql.so*' 2>/dev/null || true; } | wc -l)

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

# Check Qt Help plugins
print_status "Checking Qt Help plugins"
HELP_PLUGINS=$({ find "$APPDIR/usr/plugins/help" -type f -name "*.so" 2>/dev/null || true; } | wc -l)
KIO_HELP_PLUGINS=$({ find "$APPDIR/usr/plugins/kauth/helper" -type f -name "*.so" 2>/dev/null || true; } | wc -l)

if [[ $HELP_PLUGINS -gt 0 ]]; then
    print_success "Qt Help plugins found: $HELP_PLUGINS"
else
    print_status "No Qt Help plugins found (optional)"
fi

if [[ $KIO_HELP_PLUGINS -gt 0 ]]; then
    print_success "KIO Help plugins found: $KIO_HELP_PLUGINS"
else
    print_status "No KIO Help plugins found (optional)"
fi

# Ensure 32-bit linuxtrack runtime is present when wine bridge is enabled
if [[ "${WITH_WINE_BRIDGE:-1}" == "1" ]]; then
    print_status "Checking 32-bit linuxtrack runtime for Wine"
    LTR32_PATH="$APPDIR/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0"
    if [[ -f "$LTR32_PATH" ]]; then
        print_success "32-bit liblinuxtrack present: ${LTR32_PATH#$APPDIR/}"
    else
        print_error "Missing 32-bit liblinuxtrack.so.0 at usr/lib/i386-linux-gnu/linuxtrack/ (required for 32-bit Wine prefixes)"
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
)

if command -v ldd >/dev/null 2>&1; then
    print_status "Running ldd audit"
    critical_external_deps=0

    while IFS= read -r -d '' elf; do
        while IFS= read -r line; do
            so_path=$(awk '/=>/{print $3}' <<<"$line" | tr -d ' ' || true)
            [[ -z "$so_path" ]] && continue
            [[ "$so_path" = "not" ]] && continue

            if [[ "$so_path" != $APPDIR/* ]]; then
                # Check for critical dependencies that should be bundled
                critical_deps=("libusb-1.0.so" "libudev.so")
                is_critical=false
                for critical in "${critical_deps[@]}"; do
                    if [[ "$so_path" == *"$critical"* ]]; then
                        # If a bundled copy exists inside AppDir, downgrade to warning
                        so_name=$(basename "$so_path")
                        if [[ -f "$APPDIR/usr/lib/$so_name" ]]; then
                            print_warning "External dep resolved to system but bundled present: $elf -> $so_path (using $APPDIR/usr/lib/$so_name at runtime)"
                        else
                            print_error "Critical external dep (should be bundled): $elf -> $so_path"
                            is_critical=true
                            critical_external_deps=$((critical_external_deps+1))
                        fi
                        break
                    fi
                done

                if [[ "$is_critical" = true ]]; then
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

                # Allow broad system patterns
                for rx in "${system_allow_patterns[@]}"; do
                    if [[ "$so_path" =~ $rx ]]; then
                        continue 2
                    fi
                done

                print_warning "External dep (acceptable): $elf -> $so_path"
            fi
        done < <(ldd "$elf" 2>/dev/null || true)
    done < <(find "$APPDIR/usr/bin" "$APPDIR/usr/lib" "$APPDIR/usr/lib/linuxtrack" -type f \( -perm -111 -o -name "*.so*" \) -print0 2>/dev/null)

    if [[ $critical_external_deps -gt 0 ]]; then
        print_error "Found $critical_external_deps critical external dependencies"
        failures=$((failures+critical_external_deps))
    fi
else
    print_warning "ldd not available; skipping dependency audit"
fi

if [[ $failures -gt 0 ]]; then
    die "Validation failed with $failures issue(s)"
fi

print_success "Validation passed"


