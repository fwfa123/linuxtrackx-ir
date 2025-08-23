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

# Check for USB libraries required for device detection
usb_libs_missing=true
for usb_lib in libusb-1.0.so.0 libudev.so.1; do
    if [[ -f "$APPDIR/usr/lib/$usb_lib" ]]; then
        print_status "Found USB library: $usb_lib"
        usb_libs_missing=false
    fi
done
if $usb_libs_missing; then
    print_warning "USB libraries missing - TrackIR device detection may fail"
fi

# Verify rpath settings on critical libraries
print_status "Verifying rpath settings on TrackIR libraries"
for lib in usr/lib/linuxtrack/libtir.so* usr/lib/linuxtrack/libltusb1.so*; do
    if [[ -f "$APPDIR/$lib" ]]; then
        rpath=$(patchelf --print-rpath "$APPDIR/$lib" 2>/dev/null || echo "NONE")
        if [[ "$rpath" == "NONE" || -z "$rpath" ]]; then
            print_warning "No rpath set for $(basename "$lib")"
        else
            print_status "rpath for $(basename "$lib"): $rpath"
        fi
    fi
done

# Ensure Qt essentials
# Check for Qt plugins in new and legacy locations
if [[ ! -e "$APPDIR/usr/plugins/platforms/libqxcb.so" && ! -e "$APPDIR/usr/lib/qt5/plugins/platforms/libqxcb.so" ]]; then
    print_error "Qt platform plugin (xcb) missing"
    failures=$((failures+1))
fi
# Check SQLite driver in both locations
SQLITE_USR_PLUGINS=$(ls "$APPDIR/usr/plugins/sqldrivers/"libqsqlite.so* 2>/dev/null | wc -l)
SQLITE_QT5_PLUGINS=$(ls "$APPDIR/usr/lib/qt5/plugins/sqldrivers/"libqsqlite.so* 2>/dev/null | wc -l)

if [[ $SQLITE_USR_PLUGINS -gt 0 ]] && [[ $SQLITE_QT5_PLUGINS -gt 0 ]]; then
    print_success "Qt SQLite driver present in both locations"
elif [[ $SQLITE_USR_PLUGINS -gt 0 ]] || [[ $SQLITE_QT5_PLUGINS -gt 0 ]]; then
    print_warning "Qt SQLite driver present in only one location; may cause issues"
else
    print_error "Qt SQLite driver missing from both locations; help system will fail"
    failures=$((failures+1))
fi

# ldd audit: ensure libs resolve inside AppDir, except allowlist
allow_regex="^/(lib|usr/lib|usr/lib64)/(drm|dri|nvidia|amd|mesa|pci)"  # GPUs/drivers allowed
# Broader system stack allowances (graphics, X11, fonts, IPC). These are typically provided by host.
system_allow_patterns=(
  '^/lib/.*/libGL(dispatch|X)?\\.so\\.'
  '^/lib/.*/libOpenGL\\.so\\.'
  '^/lib/.*/libX(11|au|dmcp|render|ext|randr|fixes|cursor|inerama)?\\.so\\.'
  '^/lib/.*/libxcb(.*)?\\.so\\.'
  '^/lib/.*/libfontconfig\\.so\\.'
  '^/lib/.*/libfreetype\\.so\\.'
  '^/lib/.*/libharfbuzz\\.so\\.'
  '^/lib/.*/libgraphite2\\.so\\.'
  '^/lib/.*/libdbus-1\\.so\\.'
  '^/lib/.*/libsystemd\\.so\\.'
  '^/lib/.*/libgcrypt\\.so\\.'
  '^/lib/.*/libgpg-error\\.so\\.'
  '^/lib/.*/libuuid\\.so\\.'
  '^/lib/.*/libpng16\\.so\\.'
  '^/lib/.*/libexpat\\.so\\.'
  '^/lib/.*/lib(deflate|lz4|lzma|zstd)\\.so\\.'
  '^/lib/.*/lib(md|md4c)\\.so\\.'
  '^/lib/.*/lib(SM|ICE)\\.so\\.'
  '^/lib/.*/libjpeg\\.so\\.'      # Allow system JPEG library
  '^/lib/.*/libv4l2\\.so\\.'     # Allow system V4L2 libraries for now
  '^/lib/.*/libv4lconvert\\.so\\.'
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
                        print_error "Critical external dep (should be bundled): $elf -> $so_path"
                        is_critical=true
                        critical_external_deps=$((critical_external_deps+1))
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


