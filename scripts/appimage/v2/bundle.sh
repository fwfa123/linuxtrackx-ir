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
        --output appimage \
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

    # Ensure SQLite driver
    if ! ls usr/plugins/sqldrivers/libqsqlite.so* usr/lib/qt5/plugins/sqldrivers/libqsqlite.so* >/dev/null 2>&1; then
        print_status "Ensuring Qt SQLite driver is present"
        while read -r candidate; do
            [[ -z "$candidate" ]] && continue
            if [[ -f "$candidate" ]]; then
                cp "$candidate" usr/plugins/sqldrivers/ 2>/dev/null || cp "$candidate" usr/lib/qt5/plugins/sqldrivers/
                print_success "Copied SQLite driver: $(basename "$candidate")"
                break
            fi
        done < <(printf "%s\n" \
            /usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so \
            /usr/lib/qt5/plugins/sqldrivers/libqsqlite.so \
            /usr/lib/qt/plugins/sqldrivers/libqsqlite.so \
            /usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so.5 \
            /usr/lib/qt5/plugins/sqldrivers/libqsqlite.so.5)
        if ! ls usr/plugins/sqldrivers/libqsqlite.so* usr/lib/qt5/plugins/sqldrivers/libqsqlite.so* >/dev/null 2>&1; then
            print_warning "SQLite plugin not found; help system may not work inside AppImage"
        fi
    fi

    # Bundle additional system libraries for self-contained runtime
    print_status "Bundling common system libraries for self-contained runtime"
    for so in \
        libGL.so.1 libOpenGL.so.0 libGLX.so.0 libGLdispatch.so.0 \
        libX11.so.6 libX11-xcb.so.1 libXrender.so.1 libXau.so.6 libXdmcp.so.6 \
        libxcb.so.1 libxcb-icccm.so.4 libxcb-image.so.0 libxcb-shm.so.0 libxcb-keysyms.so.1 libxcb-randr.so.0 \
        libxcb-render-util.so.0 libxcb-render.so.0 libxcb-shape.so.0 libxcb-sync.so.1 libxcb-xfixes.so.0 libxcb-xinerama.so.0 libxcb-xkb.so.1 libxcb-xinput.so.0 \
        libfreetype.so.6 libfontconfig.so.1 libharfbuzz.so.0 libgraphite2.so.3 \
        libdbus-1.so.3 libsystemd.so.0 libgcrypt.so.20 libgpg-error.so.0 \
        libuuid.so.1 libexpat.so.1 libpng16.so.16 libdeflate.so.0 liblz4.so.1 liblzma.so.5 libzstd.so.1 libmd.so.0 libmd4c.so.0 \
        libSM.so.6 libICE.so.6; do
        for dir in /lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu /lib /usr/lib; do
            if [[ -f "$dir/$so" ]]; then
                cp -n "$dir/$so" usr/lib/ 2>/dev/null || true
                break
            fi
        done
    done

    # Extra libs frequently not bundled by linuxdeploy but required
    for so in libcom_err.so.2 libusb-1.0.so.0; do
        for dir in /lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu /lib /usr/lib; do
            if [[ -f "$dir/$so" ]]; then
                cp -n "$dir/$so" usr/lib/ 2>/dev/null || true
                break
            fi
        done
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
popd >/dev/null

print_success "Bundle complete"


