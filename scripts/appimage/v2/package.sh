#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Package: creating AppImage"

[[ -x "$APPIMAGETOOL" ]] || die "appimagetool not executable at $APPIMAGETOOL"
[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR"

pushd "$PROJECT_ROOT" >/dev/null
    OUT="${APP_NAME}-${VERSION}-x86_64.AppImage"
    print_status "Building AppImage -> $OUT"
    ARCH=x86_64 "$APPIMAGETOOL" "$APPDIR" "$OUT"

    if [[ "${WITH_ZSYNC}" = "1" ]]; then
        print_status "Generating zsync"
        "$APPIMAGETOOL" --comp zsync "$OUT" || print_warning "zsync generation failed"
    fi

    print_success "AppImage created: $PROJECT_ROOT/$OUT"

    # Post-processing: Fix rpath for critical TrackIR libraries after linuxdeploy-plugin-qt
    print_status "Post-processing: Fixing rpath for TrackIR libraries after Qt deployment"
    if command -v patchelf >/dev/null 2>&1; then
        # Find and fix rpath for critical TrackIR libraries
        find "$APPDIR/usr/lib/linuxtrack" -name "libtir.so*" -o -name "libltusb1.so*" | while read -r lib; do
            if [[ -f "$lib" ]]; then
                # Set rpath to look in parent directory where bundled libs are located
                print_status "Setting rpath for $lib"
                patchelf --set-rpath '$ORIGIN/..' "$lib" 2>&1 || print_error "Failed to set rpath for $(basename "$lib")"
                NEW_RPATH=$(patchelf --print-rpath "$lib" 2>/dev/null || echo "ERROR")
                print_status "New rpath for $(basename "$lib"): $NEW_RPATH"
            fi
        done
    else
        print_warning "patchelf not available; TrackIR libraries may link to system versions"
    fi

    # Verify library dependency resolution after post-processing
    print_status "Verifying library dependency resolution after post-processing"
    if command -v ldd >/dev/null 2>&1; then
        # Check if critical libraries are linking to bundled versions
        LIBUSB_BUNDLED=$(ldd "$APPDIR/usr/lib/linuxtrack/libltusb1.so.0.0.0" 2>/dev/null | grep "libusb-1.0.so.0" | grep "AppDir_v2" | wc -l)
        LIBUDEV_BUNDLED=$(ldd "$APPDIR/usr/lib/linuxtrack/libltusb1.so.0.0.0" 2>/dev/null | grep "libudev.so.1" | grep "AppDir_v2" | wc -l)

        if [[ $LIBUSB_BUNDLED -gt 0 ]]; then
            print_success "libltusb1.so correctly links to bundled libusb"
        else
            print_error "libltusb1.so still links to system libusb"
        fi

        if [[ $LIBUDEV_BUNDLED -gt 0 ]]; then
            print_success "libltusb1.so correctly links to bundled libudev"
        else
            print_error "libltusb1.so still links to system libudev"
        fi
    fi

popd >/dev/null


