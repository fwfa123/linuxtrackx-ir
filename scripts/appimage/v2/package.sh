#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Package: creating AppImage"

[[ -e "$APPIMAGETOOL" ]] || die "appimagetool not found at $APPIMAGETOOL"
chmod +x "$APPIMAGETOOL" 2>/dev/null || true
[[ -x "$APPIMAGETOOL" ]] || die "appimagetool not executable at $APPIMAGETOOL"
[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR"

pushd "$PROJECT_ROOT" >/dev/null
    OUT="${APP_NAME}-${VERSION}-x86_64.AppImage"
    print_status "Building AppImage -> $OUT"
    # Enable extract-and-run to avoid FUSE dependency issues on some distros
    ARCH=x86_64 APPIMAGE_EXTRACT_AND_RUN=${APPIMAGE_EXTRACT_AND_RUN:-1} "$APPIMAGETOOL" "$APPDIR" "$OUT" || die "appimagetool failed to create $OUT"

    [[ -f "$OUT" ]] || die "Expected AppImage not created: $OUT"

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

    # Verify critical contents inside the final AppImage (help system and 32-bit runtime)
    print_status "Verifying contents inside the AppImage"
    TMP_EXTRACT_DIR=$(mktemp -d)
    if [[ -f "$OUT" ]]; then
        pushd "$TMP_EXTRACT_DIR" >/dev/null
            APPIMAGE_EXTRACT_AND_RUN=1 "$PROJECT_ROOT/$OUT" --appimage-extract >/dev/null 2>&1 || true
            ROOT_DIR="squashfs-root"
            # Help files for ltr_gui and mickey
            HELP_OK=1
            for f in \
                "$ROOT_DIR/usr/share/linuxtrack/help/ltr_gui/help.qhc" \
                "$ROOT_DIR/usr/share/linuxtrack/help/ltr_gui/help.qch" \
                "$ROOT_DIR/usr/share/linuxtrack/help/mickey/help.qhc" \
                "$ROOT_DIR/usr/share/linuxtrack/help/mickey/help.qch"; do
                if [[ ! -f "$f" ]]; then
                    print_error "Missing help artifact in AppImage: ${f#${ROOT_DIR}/}"
                    HELP_OK=0
                fi
            done

            # 32-bit runtime library for Wine bridge
            LTR32_PATH="$ROOT_DIR/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0"
            if [[ ! -f "$LTR32_PATH" ]]; then
                print_error "Missing 32-bit liblinuxtrack in AppImage: usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0"
                HELP_OK=0
            fi
        popd >/dev/null
        rm -rf "$TMP_EXTRACT_DIR"
        if [[ $HELP_OK -ne 1 ]]; then
            die "AppImage content validation failed"
        else
            print_success "AppImage content validation passed"
        fi
    else
        print_warning "Cannot verify AppImage contents; $OUT not found"
    fi

popd >/dev/null


