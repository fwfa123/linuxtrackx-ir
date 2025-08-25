#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

# Emit a helpful error on any failure within this script
set -E
trap 'print_error "bundle.sh failed at ${BASH_SOURCE[0]}:$LINENO: $BASH_COMMAND"' ERR

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

    # Inject help runtime handling into the linuxdeploy Qt hook so AppRun sets QT_HELP_PATH to a writable dir
    if [[ -f apprun-hooks/linuxdeploy-plugin-qt-hook.sh ]]; then
        print_status "Patching linuxdeploy Qt hook to set writable QT_HELP_PATH and copy help files"
        cat >> apprun-hooks/linuxdeploy-plugin-qt-hook.sh << 'EOHLP'

# linuxtrack: ensure help collection is writable; copy packaged help to user dir and set QT_HELP_PATH
_ltr_help_root="${XDG_DATA_HOME:-$HOME/.local/share}/linuxtrack/help"
mkdir -p "$_ltr_help_root/ltr_gui" "$_ltr_help_root/mickey" 2>/dev/null || true
for _comp in ltr_gui mickey; do
  for _f in help.qhc help.qch; do
    if [ -f "$APPDIR/usr/share/linuxtrack/help/$_comp/$_f" ]; then
      cp -f "$APPDIR/usr/share/linuxtrack/help/$_comp/$_f" "$_ltr_help_root/$_comp/" 2>/dev/null || true
      chmod u+w "$_ltr_help_root/$_comp/$_f" 2>/dev/null || true
    fi
  done
  if [ -d "$APPDIR/usr/share/linuxtrack/help/$_comp/content" ] && [ ! -d "$_ltr_help_root/$_comp/content" ]; then
    cp -r "$APPDIR/usr/share/linuxtrack/help/$_comp/content" "$_ltr_help_root/$_comp/" 2>/dev/null || true
  fi
done
export QT_HELP_PATH="$_ltr_help_root"
EOHLP
        print_success "Patched qt hook with help runtime handling"
    else
        print_warning "Qt hook not found; cannot inject help runtime handling"
    fi

    # Note: AppRun.wrapped patching removed to avoid duplicate QT_HELP_PATH exports
    # The linuxdeploy Qt hook already sets QT_HELP_PATH correctly
    if [[ -f AppRun.wrapped ]]; then
        print_status "AppRun.wrapped found - QT_HELP_PATH already set by Qt hook"
    else
        print_warning "AppRun.wrapped not found; Qt hook should handle help path"
    fi

    # Ensure Qt Help module is properly bundled
    print_status "Verifying Qt Help module bundling"
    if [[ ! -f usr/lib/libQt5Help.so.5 && ! -f usr/lib/libQt5Help.so ]]; then
        print_warning "Qt5Help library not found in bundled libraries"
        # Try to find and copy Qt5Help from system
        for qt5help in /usr/lib/x86_64-linux-gnu/libQt5Help.so.5* /usr/lib/libQt5Help.so.5* /usr/lib/qt5/lib/libQt5Help.so.5*; do
            if [[ -f "$qt5help" ]]; then
                cp -f "$qt5help" usr/lib/
                print_success "Copied Qt5Help library: $(basename "$qt5help")"
                break
            fi
        done
    else
        print_success "Qt5Help library found in bundled libraries"
    fi

    # Ensure Qt SQL module is properly bundled (required for help system)
    print_status "Verifying Qt SQL module bundling"
    if [[ ! -f usr/lib/libQt5Sql.so.5 && ! -f usr/lib/libQt5Sql.so ]]; then
        print_warning "Qt5Sql library not found in bundled libraries"
        # Try to find and copy Qt5Sql from system
        for qt5sql in /usr/lib/x86_64-linux-gnu/libQt5Sql.so.5* /usr/lib/libQt5Sql.so.5* /usr/lib/qt5/lib/libQt5Sql.so.5*; do
            if [[ -f "$qt5sql" ]]; then
                cp -f "$qt5sql" usr/lib/
                print_success "Copied Qt5Sql library: $(basename "$qt5sql")"
                break
            fi
        done
    else
        print_success "Qt5Sql library found in bundled libraries"
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

    # Bundle Qt Help system files (required for help functionality)
    print_status "Bundling Qt Help system files"
    ensure_dir usr/share/linuxtrack/help/ltr_gui
    ensure_dir usr/share/linuxtrack/help/mickey
    
    # Copy ltr_gui help files with compatibility validation
    if [[ -f "$PROJECT_ROOT/src/qt_gui/help.qhc" ]]; then
        cp -f "$PROJECT_ROOT/src/qt_gui/help.qhc" usr/share/linuxtrack/help/ltr_gui/
        print_success "Copied ltr_gui help.qhc"
        
        # Validate help file format compatibility
        if command -v sqlite3 >/dev/null 2>&1; then
            print_status "Validating ltr_gui help.qhc format compatibility..."
            if sqlite3 "$PROJECT_ROOT/src/qt_gui/help.qhc" ".tables" >/dev/null 2>&1; then
                tables=$(sqlite3 "$PROJECT_ROOT/src/qt_gui/help.qhc" ".tables" 2>/dev/null)
                if [[ "$tables" =~ ContentsTable|FileDataTable ]]; then
                    print_success "ltr_gui help.qhc format validation passed"
                else
                    print_warning "ltr_gui help.qhc missing required tables - may have compatibility issues"
                fi
            else
                print_warning "ltr_gui help.qhc is not a valid SQLite database"
            fi
        fi
    else
        print_error "ltr_gui help.qhc not found - help generation may have failed"
    fi
    
    if [[ -f "$PROJECT_ROOT/src/qt_gui/help.qch" ]]; then
        cp -f "$PROJECT_ROOT/src/qt_gui/help.qch" usr/share/linuxtrack/help/ltr_gui/
        print_success "Copied ltr_gui help.qch"
        
        # Validate help file format compatibility
        if command -v sqlite3 >/dev/null 2>&1; then
            print_status "Validating ltr_gui help.qch format compatibility..."
            if sqlite3 "$PROJECT_ROOT/src/qt_gui/help.qch" ".tables" >/dev/null 2>&1; then
                tables=$(sqlite3 "$PROJECT_ROOT/src/qt_gui/help.qch" ".tables" 2>/dev/null)
                if [[ "$tables" =~ ContentsTable|FileDataTable ]]; then
                    print_success "ltr_gui help.qch format validation passed"
                else
                    print_warning "ltr_gui help.qch missing required tables - may have compatibility issues"
                fi
            else
                print_warning "ltr_gui help.qch is not a valid SQLite database"
            fi
        fi
    else
        print_error "ltr_gui help.qch not found - help generation may have failed"
    fi
    
    # Copy mickey help files with compatibility validation
    if [[ -f "$PROJECT_ROOT/src/mickey/help.qhc" ]]; then
        cp -f "$PROJECT_ROOT/src/mickey/help.qhc" usr/share/linuxtrack/help/mickey/
        print_success "Copied mickey help.qhc"
        
        # Validate help file format compatibility
        if command -v sqlite3 >/dev/null 2>&1; then
            print_status "Validating mickey help.qhc format compatibility..."
            if sqlite3 "$PROJECT_ROOT/src/mickey/help.qhc" ".tables" >/dev/null 2>&1; then
                tables=$(sqlite3 "$PROJECT_ROOT/src/mickey/help.qhc" ".tables" 2>/dev/null)
                if [[ "$tables" =~ ContentsTable|FileDataTable ]]; then
                    print_success "mickey help.qhc format validation passed"
                else
                    print_warning "mickey help.qhc missing required tables - may have compatibility issues"
                fi
            else
                print_warning "mickey help.qhc is not a valid SQLite database"
            fi
        fi
    else
        print_error "mickey help.qhc not found - help generation may have failed"
    fi
    
    if [[ -f "$PROJECT_ROOT/src/mickey/help.qch" ]]; then
        cp -f "$PROJECT_ROOT/src/mickey/help.qch" usr/share/linuxtrack/help/mickey/
        print_success "Copied mickey help.qch"
        
        # Validate help file format compatibility
        if command -v sqlite3 >/dev/null 2>&1; then
            print_status "Validating mickey help.qch format compatibility..."
            if sqlite3 "$PROJECT_ROOT/src/mickey/help.qch" ".tables" >/dev/null 2>&1; then
                tables=$(sqlite3 "$PROJECT_ROOT/src/mickey/help.qch" ".tables" 2>/dev/null)
                if [[ "$tables" =~ ContentsTable|FileDataTable ]]; then
                    print_success "mickey help.qch format validation passed"
                else
                    print_warning "mickey help.qch missing required tables - may have compatibility issues"
                fi
            else
                print_warning "mickey help.qch is not a valid SQLite database"
            fi
        fi
    else
        print_error "mickey help.qch not found - help generation may have failed"
    fi
    
    # Copy help content directories if they exist
    if [[ -d "$PROJECT_ROOT/src/qt_gui/help" ]]; then
        cp -r "$PROJECT_ROOT/src/qt_gui/help"/* usr/share/linuxtrack/help/ltr_gui/ 2>/dev/null || true
        print_success "Copied ltr_gui help content"
    fi
    
    if [[ -d "$PROJECT_ROOT/src/mickey/help" ]]; then
        cp -r "$PROJECT_ROOT/src/mickey/help"/* usr/share/linuxtrack/help/mickey/ 2>/dev/null || true
        print_success "Copied mickey help content"
    fi
    
    print_success "Qt Help system files bundled"

    # Bundle Qt Help system components (Qt Help doesn't use standard plugin architecture)
    print_status "Bundling Qt Help system components"
    ensure_dir usr/plugins/kauth/helper
    ensure_dir usr/plugins/kf5/kio
    
    # Track what components we find
    KIO_HELP_PLUGINS_FOUND=0
    KIO_PLUGINS_FOUND=0
    
    # Copy KIO Help plugins (these provide enhanced help functionality)
    print_status "Searching for KIO Help plugins..."
    KIO_HELP_PLUGIN_LOCATIONS=(
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/kf5/kio/kio_help.so"
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/kf5/kio/kio_ghelp.so"
        "/usr/lib/qt5/plugins/kf5/kio/kio_help.so"
        "/usr/lib/qt5/plugins/kf5/kio/kio_ghelp.so"
    )
    
    for plugin_path in "${KIO_HELP_PLUGIN_LOCATIONS[@]}"; do
        if [[ -f "$plugin_path" ]]; then
            cp -f "$plugin_path" usr/plugins/kf5/kio/ 2>/dev/null || true
            print_status "Bundled KIO Help plugin: $(basename "$plugin_path")"
            KIO_HELP_PLUGINS_FOUND=$((KIO_HELP_PLUGINS_FOUND+1))
        fi
    done
    
    # Copy other KIO plugins for enhanced functionality
    print_status "Searching for additional KIO plugins..."
    KIO_PLUGIN_LOCATIONS=(
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/kf5/kio"
        "/usr/lib/qt5/plugins/kf5/kio"
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/kauth/helper"
        "/usr/lib/qt5/plugins/kauth/helper"
    )
    
    for plugin_dir in "${KIO_PLUGIN_LOCATIONS[@]}"; do
        if [[ -d "$plugin_dir" ]]; then
            for plugin in "$plugin_dir"/*.so; do
                if [[ -f "$plugin" ]]; then
                    # Skip help plugins we already copied
                    if [[ "$plugin" == *"kio_help"* ]] || [[ "$plugin" == *"kio_ghelp"* ]]; then
                        continue
                    fi
                    
                    # Copy to appropriate subdirectories
                    if [[ "$plugin_dir" == *"/kio" ]]; then
                        ensure_dir usr/plugins/kf5/kio
                        cp -f "$plugin" usr/plugins/kf5/kio/ 2>/dev/null || true
                    else
                        cp -f "$plugin" usr/plugins/kauth/helper/ 2>/dev/null || true
                    fi
                    print_status "Bundled KIO plugin: $(basename "$plugin")"
                    KIO_PLUGINS_FOUND=$((KIO_PLUGINS_FOUND+1))
                fi
            done
        fi
    done
    
    # Report Qt Help system bundling status
    print_status "Qt Help system bundling complete:"
    print_success "  - Core libraries: Qt5Help and Qt5Sql (already bundled)"
    print_success "  - SQLite driver: libqsqlite.so (already bundled)"
    
    if [[ $KIO_HELP_PLUGINS_FOUND -gt 0 ]]; then
        print_success "  - KIO Help plugins: $KIO_HELP_PLUGINS_FOUND found"
    else
        print_warning "  - KIO Help plugins: None found (help system will use basic functionality)"
    fi
    
    if [[ $KIO_PLUGINS_FOUND -gt 0 ]]; then
        print_success "  - Additional KIO plugins: $KIO_PLUGINS_FOUND found"
    else
        print_warning "  - Additional KIO plugins: None found"
    fi
    
    print_success "Qt Help system components bundled successfully"

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
        # Check if critical libraries are linking to bundled versions (tolerate no-match under pipefail)
        LIBUSB_BUNDLED=$({ ldd usr/lib/linuxtrack/libltusb1.so.0.0.0 2>/dev/null | grep -F "libusb-1.0.so.0" | grep -F "AppDir_v2"; } | wc -l || true)
        LIBUDEV_BUNDLED=$({ ldd usr/lib/linuxtrack/libltusb1.so.0.0.0 2>/dev/null | grep -F "libudev.so.1" | grep -F "AppDir_v2"; } | wc -l || true)

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
    SQLITE_USR_PLUGINS=$({ find usr/plugins/sqldrivers -maxdepth 1 -type f -name 'libqsqlite.so*' 2>/dev/null || true; } | wc -l || true)
    SQLITE_QT5_PLUGINS=$({ find usr/lib/qt5/plugins/sqldrivers -maxdepth 1 -type f -name 'libqsqlite.so*' 2>/dev/null || true; } | wc -l || true)

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

    # Ensure 32-bit linuxtrack runtime is bundled if available on system
    print_status "Ensuring 32-bit liblinuxtrack is bundled if available"
    DEST32_DIR="$APPDIR/usr/lib/i386-linux-gnu/linuxtrack"
    if [[ ! -f "$DEST32_DIR/liblinuxtrack.so.0" ]]; then
        SYS_LTR32="/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0"
        if [[ -f "$SYS_LTR32" ]]; then
            print_status "Copying 32-bit lib from system: $SYS_LTR32"
            mkdir -p "$DEST32_DIR"
            cp -f "$SYS_LTR32" "$DEST32_DIR/"
            print_success "Bundled 32-bit liblinuxtrack.so.0"
        else
            print_warning "No system 32-bit liblinuxtrack.so.0 found; AppImage will not include 32-bit runtime"
        fi
    else
        print_success "32-bit liblinuxtrack already present in AppDir"
    fi

