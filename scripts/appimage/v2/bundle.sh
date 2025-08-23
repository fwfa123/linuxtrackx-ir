#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Bundle: dependencies into AppDir (linuxdeploy-first)"

[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR. Run prepare.sh first."

# Ensure tools
[[ -x "$LINUXDEPLOY" ]] || die "linuxdeploy not executable at $LINUXDEPLOY"
[[ -x "$LINUXDEPLOY_QT" ]] || print_warning "linuxdeploy-plugin-qt not found or not executable; will try without"

pushd "$APPDIR" >/dev/null
    # linuxdeploy to discover and copy runtime deps
    print_status "Running linuxdeploy"
    DESKTOP_FILE="usr/share/applications/linuxtrack.desktop"
    ICON_FILE="linuxtrack.png"
    [ -f "$ICON_FILE" ] || ICON_FILE="usr/share/icons/hicolor/48x48/apps/linuxtrack.png"
    "$LINUXDEPLOY" --appdir . \
        -e usr/bin/ltr_gui \
        -d "$DESKTOP_FILE" \
        -i "$ICON_FILE" \
        --plugin qt \
        || print_warning "linuxdeploy returned non-zero; continuing"

    # Qt plugin to collect Qt libs/plugins
    if [[ -x "$LINUXDEPLOY_QT" && -f usr/bin/ltr_gui ]]; then
        print_status "Running linuxdeploy-plugin-qt"
        "$LINUXDEPLOY_QT" --appdir . || print_warning "linuxdeploy-plugin-qt failed; continuing"
    fi

    # Ensure Qt plugin directories exist
    ensure_dir usr/lib/qt5/plugins/platforms
    ensure_dir usr/lib/qt5/plugins/sqldrivers
    ensure_dir usr/plugins/platforms
    ensure_dir usr/plugins/sqldrivers

    # Bundle 3D model assets (required by GL 3D View)
    print_status "Bundling 3D model assets"
    ensure_dir usr/share/linuxtrack
    ASSET_SRC_DIR="$PROJECT_ROOT/src/qt_gui"
    ASSETS=("sphere.obj" "sparow_opaq.obj" "sparow_glass.obj")
    for a in "${ASSETS[@]}"; do
        if [[ ! -f "$ASSET_SRC_DIR/$a" ]]; then
            die "Missing 3D asset: $ASSET_SRC_DIR/$a"
        fi
        cp -f "$ASSET_SRC_DIR/$a" usr/share/linuxtrack/
    done
    print_success "3D model assets copied to usr/share/linuxtrack/"

    # Ensure platform plugin (xcb) fallback if plugin didn't bundle
    if [[ ! -f usr/plugins/platforms/libqxcb.so && ! -f usr/lib/qt5/plugins/platforms/libqxcb.so ]]; then
        print_status "Ensuring Qt xcb platform plugin is present"
        while read -r p; do
            [[ -z "$p" ]] && continue
            if [[ -f "$p" ]]; then
                cp "$p" usr/plugins/platforms/ 2>/dev/null || cp "$p" usr/lib/qt5/plugins/platforms/
                print_success "Copied Qt platform plugin: $(basename "$p")"
                break
            fi
        done < <(printf "%s\n" \
            /usr/lib/x86_64-linux-gnu/qt5/plugins/platforms/libqxcb.so \
            /usr/lib/qt5/plugins/platforms/libqxcb.so \
            /usr/lib/qt/plugins/platforms/libqxcb.so)
    fi

    # Ensure SQLite driver with comprehensive search
    print_status "Ensuring Qt SQLite driver is present"
    SQLITE_FOUND=false

    # Search multiple locations for SQLite driver
    SQLITE_LOCATIONS=(
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so"
        "/usr/lib/qt5/plugins/sqldrivers/libqsqlite.so"
        "/usr/lib/qt/plugins/sqldrivers/libqsqlite.so"
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so.5"
        "/usr/lib/qt5/plugins/sqldrivers/libqsqlite.so.5"
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so.5.15"
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so.5.15.2"
    )

    for candidate in "${SQLITE_LOCATIONS[@]}"; do
        if [[ -f "$candidate" ]]; then
            # Ensure both directories exist
            ensure_dir usr/plugins/sqldrivers
            ensure_dir usr/lib/qt5/plugins/sqldrivers

            # Copy to both plugin directories to ensure it's found
            cp "$candidate" usr/plugins/sqldrivers/
            cp "$candidate" usr/lib/qt5/plugins/sqldrivers/
            print_success "Copied SQLite driver: $(basename "$candidate")"
            SQLITE_FOUND=true
            break
        fi
    done

    # Also search using find for any SQLite drivers
    if [[ "$SQLITE_FOUND" = false ]]; then
        SQLITE_FIND=$(find /usr -name "libqsqlite.so*" -type f 2>/dev/null | head -1)
        if [[ -n "$SQLITE_FIND" && -f "$SQLITE_FIND" ]]; then
            # Ensure both directories exist
            ensure_dir usr/plugins/sqldrivers
            ensure_dir usr/lib/qt5/plugins/sqldrivers

            cp "$SQLITE_FIND" usr/plugins/sqldrivers/
            cp "$SQLITE_FIND" usr/lib/qt5/plugins/sqldrivers/
            print_success "Found and copied SQLite driver from find: $(basename "$SQLITE_FIND")"
            SQLITE_FOUND=true
        fi
    fi

    if [[ "$SQLITE_FOUND" = false ]]; then
        print_error "SQLite plugin not found in any standard location - help system will fail"
    else
        print_success "SQLite driver successfully bundled"
    fi

    # Bundle additional system libraries for self-contained runtime
    print_status "Bundling common system libraries for self-contained runtime"
    for so in \
        libX11.so.6 libX11-xcb.so.1 libXrender.so.1 libXau.so.6 libXdmcp.so.6 \
        libxcb.so.1 libxcb-icccm.so.4 libxcb-image.so.0 libxcb-shm.so.0 libxcb-keysyms.so.1 libxcb-randr.so.0 \
        libxcb-render-util.so.0 libxcb-render.so.0 libxcb-shape.so.0 libxcb-sync.so.1 libxcb-xfixes.so.0 libxcb-xinerama.so.0 libxcb-xkb.so.1 libxcb-xinput.so.0 \
        libfreetype.so.6 libfontconfig.so.1 libharfbuzz.so.0 libgraphite2.so.3 \
        libdbus-1.so.3 libsystemd.so.0 libgcrypt.so.20 libgpg-error.so.0 \
        libuuid.so.1 libexpat.so.1 libpng16.so.16 libdeflate.so.0 liblz4.so.1 liblzma.so.5 libzstd.so.1 libmd.so.0 libmd4c.so.0 \
        libSM.so.6 libICE.so.6; do
        # Do NOT bundle OpenGL driver libraries; they must come from the host
        if [[ "$so" == libGL.so.1 || "$so" == libOpenGL.so.0 || "$so" == libGLX.so.0 || "$so" == libGLdispatch.so.0 || "$so" == libGLU.so.1 ]]; then
            continue
        fi
        for dir in /lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu /lib /usr/lib; do
            if [[ -f "$dir/$so" ]]; then
                cp -n "$dir/$so" usr/lib/ 2>/dev/null || true
                break
            fi
        done
    done

    # Extra libs frequently not bundled by linuxdeploy but required
    for so in libcom_err.so.2 libusb-1.0.so.0 libudev.so.1 libv4l2.so.0 libv4lconvert.so.0 libjpeg.so.62; do
        for dir in /lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu /lib /usr/lib; do
            if [[ -f "$dir/$so" ]]; then
                cp -n "$dir/$so" usr/lib/ 2>/dev/null || true
                print_status "Bundled critical library: $so"
                break
            fi
        done
    done

    # CRITICAL: Ensure all linuxtrack libraries use bundled dependencies
    print_status "Fixing library dependencies to use bundled versions"
    for lib in usr/lib/linuxtrack/*.so*; do
        if [[ -f "$lib" && -x "$lib" ]]; then
            # Set rpath to look in parent directory where bundled libs are located
            patchelf --set-rpath '$ORIGIN/..' "$lib" 2>/dev/null || true
            print_status "Fixed rpath for $(basename "$lib")"
        fi
    done

    # Normalize plugin layout: prefer usr/plugins, remove duplicates under usr/lib/qt5/plugins
    if [[ -d usr/lib/qt5/plugins ]]; then
        print_status "Normalizing Qt plugin layout to usr/plugins"
        rsync -a usr/lib/qt5/plugins/ usr/plugins/ 2>/dev/null || true
        rm -rf usr/lib/qt5/plugins
    fi

    # RPATH: binaries and libraries
    print_status "Setting rpath for binaries"
    if command -v patchelf >/dev/null 2>&1; then
        for bin in usr/bin/*; do
            [[ -f "$bin" && -x "$bin" ]] || continue
            set_rpath_binary "$bin"
        done
        print_status "Setting rpath for libraries"
        find usr/lib usr/lib/linuxtrack -name "*.so*" -type f -print0 2>/dev/null | while IFS= read -r -d '' so; do
            set_rpath_library "$so"
        done
    else
        print_warning "patchelf not available; skipping rpath adjustments"
    fi
    
    # Ensure no OpenGL driver libraries are bundled (can break GL context)
    print_status "Removing bundled OpenGL driver libraries (use host drivers)"
    rm -f usr/lib/libGL.so.* usr/lib/libOpenGL.so.* usr/lib/libGLX.so.* usr/lib/libGLdispatch.so.* usr/lib/libGLU.so.* 2>/dev/null || true
    
popd >/dev/null

    # Verify library dependencies are correctly resolved
    print_status "Verifying library dependency resolution"
    if command -v ldd >/dev/null 2>&1; then
        # Check if critical libraries are linking to bundled versions
        LIBUSB_BUNDLED=$(ldd usr/lib/linuxtrack/libltusb1.so.0.0.0 2>/dev/null | grep "libusb-1.0.so.0" | grep "AppDir_v2" | wc -l)
        LIBUDEV_BUNDLED=$(ldd usr/lib/linuxtrack/libltusb1.so.0.0.0 2>/dev/null | grep "libudev.so.1" | grep "AppDir_v2" | wc -l)

        if [[ $LIBUSB_BUNDLED -gt 0 ]]; then
            print_success "libltusb1.so correctly links to bundled libusb"
        else
            print_warning "libltusb1.so still links to system libusb"
        fi

        if [[ $LIBUDEV_BUNDLED -gt 0 ]]; then
            print_success "libltusb1.so correctly links to bundled libudev"
        else
            print_warning "libltusb1.so still links to system libudev"
        fi
    fi

    # Verify SQLite driver is in both locations
    SQLITE_USR_PLUGINS=$(ls usr/plugins/sqldrivers/libqsqlite.so* 2>/dev/null | wc -l)
    SQLITE_QT5_PLUGINS=$(ls usr/lib/qt5/plugins/sqldrivers/libqsqlite.so* 2>/dev/null | wc -l)

    if [[ $SQLITE_USR_PLUGINS -gt 0 ]]; then
        print_success "SQLite driver found in usr/plugins/sqldrivers/"
    else
        print_warning "SQLite driver missing from usr/plugins/sqldrivers/"
    fi

    if [[ $SQLITE_QT5_PLUGINS -gt 0 ]]; then
        print_success "SQLite driver found in usr/lib/qt5/plugins/sqldrivers/"
    else
        print_warning "SQLite driver missing from usr/lib/qt5/plugins/sqldrivers/"
    fi

    print_success "Bundle complete"


