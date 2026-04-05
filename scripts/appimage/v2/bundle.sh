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
    # linuxdeploy / plugin use an embedded strip that breaks modern ELF (.relr.dyn); CLI --dont-strip + NO_STRIP for plugin
    export NO_STRIP=1

    # CRITICAL: Set LD_LIBRARY_PATH so linuxdeploy can find liblinuxtrack.so.0
    # The linuxtrack libraries are installed in usr/lib/linuxtrack (subdirectory)
    # Without this, linuxdeploy fails with "Could not find dependency: liblinuxtrack.so.0"
    # which then causes linuxdeploy-plugin-qt to crash without bundling Qt libraries
    export LD_LIBRARY_PATH="$(pwd)/usr/lib:$(pwd)/usr/lib/linuxtrack${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
    print_status "Set LD_LIBRARY_PATH=$LD_LIBRARY_PATH"

    # linuxdeploy to discover and copy runtime deps
    # --dont-strip: bundled strip in linuxdeploy AppImage is too old for modern ELF (.relr.dyn); fails on Fedora/glibc toolchains
    print_status "Running linuxdeploy (with --dont-strip for RELR-safe binaries)"
    DESKTOP_FILE="usr/share/applications/linuxtrack.desktop"
    ICON_FILE="linuxtrack.png"
    [ -f "$ICON_FILE" ] || ICON_FILE="usr/share/icons/hicolor/48x48/apps/linuxtrack.png"
    "$LINUXDEPLOY" --appdir . --dont-strip \
        -e usr/bin/ltr_gui \
        -d "$DESKTOP_FILE" \
        -i "$ICON_FILE" \
        --plugin qt \
        || print_warning "linuxdeploy returned non-zero; continuing"

    # Ensure desktop file is at AppDir root (required by appimagetool)
    if [[ -f "$DESKTOP_FILE" && ! -f "$(basename "$DESKTOP_FILE")" ]]; then
        print_status "Copying desktop file to AppDir root for appimagetool"
        cp "$DESKTOP_FILE" "$(basename "$DESKTOP_FILE")"
    fi

    # Qt plugin to collect Qt libs/plugins
    if [[ -x "$LINUXDEPLOY_QT" && -f usr/bin/ltr_gui ]]; then
        print_status "Running linuxdeploy-plugin-qt"
        "$LINUXDEPLOY_QT" --appdir . || print_warning "linuxdeploy-plugin-qt failed; continuing"

        # Qt framework .qm files (standard dialogs, Qt Help) — app-specific strings remain in ltr_gui resources
        print_status "Bundling Qt translation catalogs from host Qt"
        mkdir -p usr/share/qt6/translations
        _qmake=""
        for _cand in qmake6 /usr/lib64/qt6/bin/qmake /usr/lib/qt6/bin/qmake qmake; do
            if command -v "$_cand" >/dev/null 2>&1; then
                _qmake=$(command -v "$_cand")
                break
            fi
            if [[ -x "$_cand" ]]; then
                _qmake="$_cand"
                break
            fi
        done
        _trans=""
        if [[ -n "$_qmake" ]]; then
            _trans=$("$_qmake" -query QT_INSTALL_TRANSLATIONS 2>/dev/null || true)
        fi
        if [[ -z "$_trans" || ! -d "$_trans" ]]; then
            for _d in /usr/lib64/qt6/translations /usr/lib/qt6/translations /usr/lib/x86_64-linux-gnu/qt6/translations; do
                if [[ -d "$_d" ]]; then
                    _trans="$_d"
                    break
                fi
            done
        fi
        if [[ -n "$_trans" && -d "$_trans" ]]; then
            _n=0
            shopt -s nullglob
            for _f in "$_trans"/qtbase_*.qm "$_trans"/qt_help_*.qm "$_trans"/qtdeclarative_*.qm; do
                [[ -f "$_f" ]] || continue
                cp -f "$_f" usr/share/qt6/translations/
                _n=$((_n + 1))
            done
            shopt -u nullglob
            print_status "Copied $_n Qt .qm files from $_trans"
        else
            print_warning "Could not locate host Qt translations (qtbase/qt_help); Qt standard dialogs may stay English-only"
        fi
        unset _qmake _trans _f _n _cand _d

        # Replace qt.conf so QLibraryInfo uses AppDir usr/ prefix (not host Qt paths).
        # setLibraryPaths() before QApplication caused segfaults on Fedora; qt.conf is the supported fix.
        if [[ -f usr/bin/qt.conf ]]; then
            cp -f usr/bin/qt.conf usr/bin/qt.conf.from_linuxdeploy
        fi
        print_status "Writing usr/bin/qt.conf for AppDir-only Qt6 paths (fixes Qt version mix on Fedora)"
        cat > usr/bin/qt.conf << 'QTEOF'
[Paths]
Prefix = ..
Plugins = plugins:lib/qt6/plugins
Libraries = lib
Binaries = bin
QTEOF
        if [[ -d usr/share/qt6/translations ]]; then
            echo "Translations = share/qt6/translations" >> usr/bin/qt.conf
            print_status "qt.conf: Translations = share/qt6/translations"
        elif [[ -d usr/share/qt5/translations ]]; then
            echo "Translations = share/qt5/translations" >> usr/bin/qt.conf
            print_status "qt.conf: Translations = share/qt5/translations (legacy layout)"
        fi
        print_success "usr/bin/qt.conf: Prefix=.. (relative to usr/bin → usr/), plugins = usr/plugins and usr/lib/qt6/plugins"
    fi

    # Synchronize full Qt6 shared libraries from the build host (same Qt as the CMake build).
    # linuxdeploy-plugin-qt can leave gaps; the dynamic linker may then resolve libQt6*.so from
    # host /lib64 (older Qt), causing Qt_6.10 / Qt_*_PRIVATE_API symbol errors at runtime.
    if [[ -f usr/bin/ltr_gui ]]; then
        print_status "Ensuring full Qt6 library set from build host (QT_INSTALL_LIBS → usr/lib)"
        _qmake_for_libs=""
        for _c in qmake6 /usr/lib64/qt6/bin/qmake /usr/lib/qt6/bin/qmake qmake; do
            if command -v "$_c" >/dev/null 2>&1; then
                _qmake_for_libs=$(command -v "$_c")
                break
            fi
            if [[ -x "$_c" ]]; then
                _qmake_for_libs="$_c"
                break
            fi
        done
        _qt6_libdir=""
        if [[ -n "$_qmake_for_libs" ]]; then
            _qt6_libdir=$("$_qmake_for_libs" -query QT_INSTALL_LIBS 2>/dev/null || true)
        fi
        if [[ -z "$_qt6_libdir" || ! -d "$_qt6_libdir" ]]; then
            for _d in /usr/lib64/qt6/lib /usr/lib/qt6/lib; do
                if [[ -d "$_d" && ( -f "$_d/libQt6Core.so.6" || -f "$_d/libQt6Core.so" ) ]]; then
                    _qt6_libdir="$_d"
                    break
                fi
            done
        fi
        if [[ -n "$_qt6_libdir" && -d "$_qt6_libdir" ]]; then
            _qt6_n=0
            shopt -s nullglob
            for _qf in "$_qt6_libdir"/libQt6*.so*; do
                [[ -e "$_qf" ]] || continue
                _qbase=$(basename "$_qf")
                [[ "$_qbase" == *.prl ]] && continue
                cp -L -f "$_qf" "usr/lib/$_qbase" 2>/dev/null || cp -f "$_qf" "usr/lib/"
                _qt6_n=$((_qt6_n + 1))
            done
            shopt -u nullglob
            print_success "Synced $_qt6_n Qt6 libraries from $_qt6_libdir into usr/lib (avoids host /lib64 Qt mix)"
        else
            print_warning "Could not locate QT_INSTALL_LIBS — verify usr/lib contains a full libQt6* set"
        fi
        unset _qmake_for_libs _qt6_libdir _qf _qbase _qt6_n _c _d
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

    # Qt6 Help / SQL / OpenGLWidgets — copy from host if linuxdeploy missed them
    print_status "Verifying Qt6 Help module bundling"
    if [[ -z "$(find usr/lib \( -name 'libQt6Help.so' -o -name 'libQt6Help.so.*' \) \( -type f -o -type l \) -print -quit 2>/dev/null)" ]]; then
        print_warning "Qt6 Help library not found under usr/lib; copying from host"
        for qt6help in /usr/lib64/libQt6Help.so.6* /usr/lib/x86_64-linux-gnu/libQt6Help.so.6* \
            /usr/lib64/qt6/lib/libQt6Help.so.6* /usr/lib/qt6/lib/libQt6Help.so.6*; do
            if [[ -e "$qt6help" ]]; then
                cp -L -f "$qt6help" usr/lib/ 2>/dev/null || cp -f "$qt6help" usr/lib/
                print_success "Copied Qt6 Help library: $(basename "$qt6help")"
                break
            fi
        done
    else
        print_success "Qt6 Help library found in bundled libraries"
    fi

    print_status "Verifying Qt6 SQL module bundling"
    if [[ -z "$(find usr/lib \( -name 'libQt6Sql.so' -o -name 'libQt6Sql.so.*' \) \( -type f -o -type l \) -print -quit 2>/dev/null)" ]]; then
        print_warning "Qt6 Sql library not found under usr/lib; copying from host"
        for qt6sql in /usr/lib64/libQt6Sql.so.6* /usr/lib/x86_64-linux-gnu/libQt6Sql.so.6* \
            /usr/lib64/qt6/lib/libQt6Sql.so.6* /usr/lib/qt6/lib/libQt6Sql.so.6*; do
            if [[ -e "$qt6sql" ]]; then
                cp -L -f "$qt6sql" usr/lib/ 2>/dev/null || cp -f "$qt6sql" usr/lib/
                print_success "Copied Qt6 Sql library: $(basename "$qt6sql")"
                break
            fi
        done
    else
        print_success "Qt6 Sql library found in bundled libraries"
    fi

    print_status "Verifying Qt6 OpenGLWidgets module bundling (QOpenGLWidget)"
    if [[ -z "$(find usr/lib \( -name 'libQt6OpenGLWidgets.so' -o -name 'libQt6OpenGLWidgets.so.*' \) \( -type f -o -type l \) -print -quit 2>/dev/null)" ]]; then
        print_warning "Qt6OpenGLWidgets not under usr/lib; copying from host"
        for qt6ogl in /usr/lib64/libQt6OpenGLWidgets.so.6* /usr/lib/x86_64-linux-gnu/libQt6OpenGLWidgets.so.6* \
            /usr/lib64/qt6/lib/libQt6OpenGLWidgets.so.6* /usr/lib/qt6/lib/libQt6OpenGLWidgets.so.6*; do
            if [[ -e "$qt6ogl" ]]; then
                cp -L -f "$qt6ogl" usr/lib/ 2>/dev/null || cp -f "$qt6ogl" usr/lib/
                print_success "Copied Qt6OpenGLWidgets library: $(basename "$qt6ogl")"
                break
            fi
        done
    else
        print_success "Qt6OpenGLWidgets library found in bundled libraries"
    fi

    # Qt6 removed the old Qt5 network bearer plugins; nothing to copy for bearer.

    print_status "Ensuring Qt xcbglintegrations plugins from build host (Qt6)"
    for gintdir in /usr/lib64/qt6/plugins/xcbglintegrations /usr/lib/x86_64-linux-gnu/qt6/plugins/xcbglintegrations /usr/lib/qt6/plugins/xcbglintegrations; do
        if [[ -d "$gintdir" ]]; then
            ensure_dir usr/lib/qt6/plugins/xcbglintegrations
            ensure_dir usr/plugins/xcbglintegrations
            shopt -s nullglob
            for gso in "$gintdir"/*.so; do
                [[ -e "$gso" ]] || continue
                cp -f "$gso" usr/lib/qt6/plugins/xcbglintegrations/ 2>/dev/null || true
                cp -f "$gso" usr/plugins/xcbglintegrations/ 2>/dev/null || true
            done
            shopt -u nullglob
            print_status "Copied xcbglintegrations from $gintdir"
            break
        fi
    done

    ensure_dir usr/lib/qt6/plugins/platforms
    ensure_dir usr/lib/qt6/plugins/sqldrivers
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
        
        # Validate collection file format compatibility
        if command -v sqlite3 >/dev/null 2>&1; then
            print_status "Validating ltr_gui help.qhc format compatibility..."
            if sqlite3 "$PROJECT_ROOT/src/qt_gui/help.qhc" ".tables" >/dev/null 2>&1; then
                # Check if collection file has proper namespace registration
                namespace_info=$(sqlite3 "$PROJECT_ROOT/src/qt_gui/help.qhc" "SELECT Name, FilePath FROM NamespaceTable;" 2>/dev/null)
                if [[ -n "$namespace_info" ]]; then
                    print_success "ltr_gui help.qhc collection format validation passed"
                    print_status "Collection references content file: $namespace_info"
                else
                    print_warning "ltr_gui help.qhc missing namespace registration"
                fi
            else
                print_warning "ltr_gui help.qhc is not a valid SQLite database"
            fi
        fi
    else
        print_warning "ltr_gui help.qhc not found — optional if using HTML-only help"
    fi
    
    if [[ -f "$PROJECT_ROOT/src/qt_gui/help.qch" ]]; then
        cp -f "$PROJECT_ROOT/src/qt_gui/help.qch" usr/share/linuxtrack/help/ltr_gui/
        print_success "Copied ltr_gui help.qch"
        
        # Validate content file format compatibility
        if command -v sqlite3 >/dev/null 2>&1; then
            print_status "Validating ltr_gui help.qch format compatibility..."
            if sqlite3 "$PROJECT_ROOT/src/qt_gui/help.qch" ".tables" >/dev/null 2>&1; then
                tables=$(sqlite3 "$PROJECT_ROOT/src/qt_gui/help.qch" ".tables" 2>/dev/null)
                if [[ "$tables" =~ ContentsTable && "$tables" =~ FileDataTable ]]; then
                    print_success "ltr_gui help.qch content format validation passed"
                    print_status "Content file contains required tables: ContentsTable, FileDataTable"
                else
                    print_warning "ltr_gui help.qch missing required tables - may have compatibility issues"
                    print_status "Found tables: $tables"
                fi
            else
                print_warning "ltr_gui help.qch is not a valid SQLite database"
            fi
        fi
    else
        print_warning "ltr_gui help.qch not found — optional if using HTML-only help"
    fi
    
    # Copy mickey help files with compatibility validation
    if [[ -f "$PROJECT_ROOT/src/mickey/help.qhc" ]]; then
        cp -f "$PROJECT_ROOT/src/mickey/help.qhc" usr/share/linuxtrack/help/mickey/
        print_success "Copied mickey help.qhc"
        
        # Validate collection file format compatibility
        if command -v sqlite3 >/dev/null 2>&1; then
            print_status "Validating mickey help.qhc format compatibility..."
            if sqlite3 "$PROJECT_ROOT/src/mickey/help.qhc" ".tables" >/dev/null 2>&1; then
                # Check if collection file has proper namespace registration
                namespace_info=$(sqlite3 "$PROJECT_ROOT/src/mickey/help.qhc" "SELECT Name, FilePath FROM NamespaceTable;" 2>/dev/null)
                if [[ -n "$namespace_info" ]]; then
                    print_success "mickey help.qhc collection format validation passed"
                    print_status "Collection references content file: $namespace_info"
                else
                    print_warning "mickey help.qhc missing namespace registration"
                fi
            else
                print_warning "mickey help.qhc is not a valid SQLite database"
            fi
        fi
    else
        print_warning "mickey help.qhc not found — optional (Mickey uses HTML under src/mickey/help/)"
    fi
    
    if [[ -f "$PROJECT_ROOT/src/mickey/help.qch" ]]; then
        cp -f "$PROJECT_ROOT/src/mickey/help.qch" usr/share/linuxtrack/help/mickey/
        print_success "Copied mickey help.qch"
        
        # Validate content file format compatibility
        if command -v sqlite3 >/dev/null 2>&1; then
            print_status "Validating mickey help.qch format compatibility..."
            if sqlite3 "$PROJECT_ROOT/src/mickey/help.qch" ".tables" >/dev/null 2>&1; then
                tables=$(sqlite3 "$PROJECT_ROOT/src/mickey/help.qch" ".tables" 2>/dev/null)
                if [[ "$tables" =~ ContentsTable && "$tables" =~ FileDataTable ]]; then
                    print_success "mickey help.qch content format validation passed"
                    print_status "Content file contains required tables: ContentsTable, FileDataTable"
                else
                    print_warning "mickey help.qch missing required tables - may have compatibility issues"
                    print_status "Found tables: $tables"
                fi
            else
                print_warning "mickey help.qch is not a valid SQLite database"
            fi
        fi
    else
        print_warning "mickey help.qch not found — optional (Mickey uses HTML under src/mickey/help/)"
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
    print_success "  - Core libraries: Qt6Help and Qt6Sql (already bundled or copied above)"
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

    # Ensure platform plugin (xcb) fallback if plugin didn't bundle (Qt6)
    if [[ ! -f usr/plugins/platforms/libqxcb.so && ! -f usr/lib/qt6/plugins/platforms/libqxcb.so && ! -f usr/lib/qt5/plugins/platforms/libqxcb.so ]]; then
        print_status "Ensuring Qt xcb platform plugin is present"
        while read -r p; do
            [[ -z "$p" ]] && continue
            if [[ -f "$p" ]]; then
                cp "$p" usr/plugins/platforms/ 2>/dev/null || cp "$p" usr/lib/qt6/plugins/platforms/ 2>/dev/null || cp "$p" usr/lib/qt5/plugins/platforms/
                print_success "Copied Qt platform plugin: $(basename "$p")"
                break
            fi
        done < <(printf "%s\n" \
            /usr/lib64/qt6/plugins/platforms/libqxcb.so \
            /usr/lib/x86_64-linux-gnu/qt6/plugins/platforms/libqxcb.so \
            /usr/lib/qt6/plugins/platforms/libqxcb.so \
            /usr/lib64/qt5/plugins/platforms/libqxcb.so \
            /usr/lib/x86_64-linux-gnu/qt5/plugins/platforms/libqxcb.so \
            /usr/lib/qt5/plugins/platforms/libqxcb.so \
            /usr/lib/qt/plugins/platforms/libqxcb.so)
    fi

    print_status "Ensuring Qt6 SQLite driver is present"
    SQLITE_FOUND=false

    SQLITE_LOCATIONS=(
        "/usr/lib64/qt6/plugins/sqldrivers/libqsqlite.so"
        "/usr/lib/x86_64-linux-gnu/qt6/plugins/sqldrivers/libqsqlite.so"
        "/usr/lib/qt6/plugins/sqldrivers/libqsqlite.so"
        "/usr/lib64/qt5/plugins/sqldrivers/libqsqlite.so"
        "/usr/lib64/qt5/plugins/sqldrivers/libqsqlite.so.5"
        "/usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so"
        "/usr/lib/qt5/plugins/sqldrivers/libqsqlite.so"
    )

    for candidate in "${SQLITE_LOCATIONS[@]}"; do
        if [[ -f "$candidate" ]]; then
            ensure_dir usr/plugins/sqldrivers
            ensure_dir usr/lib/qt6/plugins/sqldrivers
            ensure_dir usr/lib/qt5/plugins/sqldrivers
            cp "$candidate" usr/plugins/sqldrivers/ || die "copy sqldriver to usr/plugins/sqldrivers"
            cp "$candidate" usr/lib/qt6/plugins/sqldrivers/ 2>/dev/null || true
            cp "$candidate" usr/lib/qt5/plugins/sqldrivers/ 2>/dev/null || true
            print_success "Copied SQLite driver: $(basename "$candidate")"
            SQLITE_FOUND=true
            break
        fi
    done

    if [[ "$SQLITE_FOUND" = false ]]; then
        SQLITE_FIND=""
        for search_root in /usr/lib64/qt6/plugins/sqldrivers /usr/lib/qt6/plugins/sqldrivers /usr/lib/x86_64-linux-gnu/qt6/plugins/sqldrivers \
            /usr/lib64/qt5/plugins/sqldrivers /usr/lib/qt5/plugins/sqldrivers /usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers; do
            if [[ -d "$search_root" ]]; then
                SQLITE_FIND=$(find "$search_root" -maxdepth 1 -name "libqsqlite.so*" -type f 2>/dev/null | head -1)
                [[ -n "$SQLITE_FIND" ]] && break
            fi
        done
        if [[ -z "$SQLITE_FIND" ]]; then
            SQLITE_FIND=$(find /usr/lib64/qt6 /usr/lib/qt6 /usr/lib/x86_64-linux-gnu/qt6 /usr/lib64/qt5 /usr/lib/qt5 -path "*/sqldrivers/libqsqlite.so*" -type f 2>/dev/null | head -1)
        fi
        if [[ -n "$SQLITE_FIND" && -f "$SQLITE_FIND" ]]; then
            ensure_dir usr/plugins/sqldrivers
            ensure_dir usr/lib/qt6/plugins/sqldrivers
            ensure_dir usr/lib/qt5/plugins/sqldrivers
            cp "$SQLITE_FIND" usr/plugins/sqldrivers/ || die "copy sqldriver"
            cp "$SQLITE_FIND" usr/lib/qt6/plugins/sqldrivers/ 2>/dev/null || true
            cp "$SQLITE_FIND" usr/lib/qt5/plugins/sqldrivers/ 2>/dev/null || true
            print_success "Found and copied SQLite driver: $(basename "$SQLITE_FIND")"
            SQLITE_FOUND=true
        fi
    fi

    [[ "$SQLITE_FOUND" = true ]] || die "SQLite plugin not found — install qt6-base / qt6 sqldrivers on build host"
    print_success "SQLite driver successfully bundled"

    # Bundle additional system libraries for self-contained runtime
    print_status "Bundling common system libraries for self-contained runtime"
    for so in \
        libX11.so.6 libX11-xcb.so.1 libXrender.so.1 libXau.so.6 libXdmcp.so.6 \
        libxcb.so.1 libxcb-glx.so.0 libxcb-icccm.so.4 libxcb-image.so.0 libxcb-shm.so.0 libxcb-keysyms.so.1 libxcb-randr.so.0 \
        libxcb-render-util.so.0 libxcb-render.so.0 libxcb-shape.so.0 libxcb-sync.so.1 libxcb-xfixes.so.0 libxcb-xinerama.so.0 libxcb-xkb.so.1 libxcb-xinput.so.0 \
        libfreetype.so.6 libfontconfig.so.1 libharfbuzz.so.0 libgraphite2.so.3 \
        libdbus-1.so.3 libsystemd.so.0 libgcrypt.so.20 libgpg-error.so.0 \
        libuuid.so.1 libexpat.so.1 libxml2.so.2 libpng16.so.16 libdeflate.so.0 liblz4.so.1 liblzma.so.5 libzstd.so.1 libmd.so.0 libmd4c.so.0 \
        libSM.so.6 libICE.so.6; do
        # Do NOT bundle OpenGL driver libraries; they must come from the host
        if [[ "$so" == libGL.so.1 || "$so" == libOpenGL.so.0 || "$so" == libGLX.so.0 || "$so" == libGLdispatch.so.0 || "$so" == libGLU.so.1 ]]; then
            continue
        fi
        # Fedora/RHEL use /usr/lib64; Debian/Ubuntu use multiarch paths — search 64-bit dirs first
        for dir in /usr/lib64 /lib64 /lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu /lib /usr/lib; do
            if [[ -f "$dir/$so" ]]; then
                cp -n "$dir/$so" usr/lib/ 2>/dev/null || true
                break
            fi
        done
    done
    if [[ ! -e usr/lib/libxcb-glx.so.0 ]]; then
        print_warning "libxcb-glx.so.0 missing from AppDir — Qt needs it for GLX (3D view). On Fedora/RHEL install package libxcb (no separate libxcb-glx RPM; the .so is in libxcb). Debian/Ubuntu: libxcb-glx0. Then rebuild."
    fi

    # Extra libs frequently not bundled by linuxdeploy but required (Fedora: /usr/lib64 /lib64 first)
    _ltr_host_lib_dirs=(/usr/lib64 /lib64 /lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu /usr/lib /lib)
    for so in libcom_err.so.2 libusb-1.0.so.0 libudev.so.1 libv4l2.so.0 libv4lconvert.so.0 libjpeg.so.62 libmxml.so.1; do
        for dir in "${_ltr_host_lib_dirs[@]}"; do
            if [[ -f "$dir/$so" ]]; then
                cp -n "$dir/$so" usr/lib/ 2>/dev/null || cp -f "$dir/$so" usr/lib/ 2>/dev/null || true
                print_status "Bundled critical library: $so"
                break
            fi
        done
    done
    unset _ltr_host_lib_dirs

    # ICU (libicui18n, libicuuc, libicudata) — Qt / libxml2 may link it; soname varies (e.g. .77 on Arch).
    # linuxdeploy often omits it; runtime then fails on hosts without matching system ICU.
    print_status "Bundling ICU libraries from host (libicu*.so*)"
    _icu_n=0
    for _icudir in /usr/lib64 /lib64 /usr/lib /lib/x86_64-linux-gnu; do
        [[ -d "$_icudir" ]] || continue
        shopt -s nullglob
        for _icu in "$_icudir"/libicu*.so*; do
            [[ -e "$_icu" ]] || continue
            _ib=$(basename "$_icu")
            cp -L -f "$_icu" "usr/lib/$_ib" 2>/dev/null || cp -f "$_icu" "usr/lib/"
            _icu_n=$((_icu_n + 1))
        done
        shopt -u nullglob
    done
    if [[ "$_icu_n" -gt 0 ]]; then
        print_success "Bundled $_icu_n ICU shared libraries into usr/lib"
    else
        print_warning "No libicu*.so* found on build host — if ltr_gui links ICU, install libicu on the builder and rebuild"
    fi
    unset _icu_n _icudir _icu _ib

    # libwc / libp3eft are dlopen'd (not linked from ltr_gui); linuxdeploy does not pull OpenCV deps.
    # Fedora OpenCV pulls dozens of libopencv_* modules (direct + transitive). Single-pass ldd on the
    # plugin misses deps that only appear once intermediate OpenCV DSOs are in usr/lib — iterate to closure.
    _ltr_appdir_abs="$(pwd -P)"
    ltr_dep_bundlable() {
        case "$1" in
            # Intentionally omit libgomp: bundling it shadows the host and can load two OpenMP runtimes
            # (OpenCV vs Qt/Mesa), breaking GLX/EGL context creation on Fedora bare metal.
            libopencv*.so*|libopencv_*.so*|libtbb*.so*|libflexiblas*.so*|libopenblas*.so*|libblas.so*|liblapack.so*|libgfortran*.so*|libquadmath.so*|libprotobuf*.so*)
                return 0
                ;;
        esac
        return 1
    }
    ltr_copy_ldd_host_deps() {
        local elf="$1"
        local tag="$2"
        local did_copy=0
        [[ -f "$elf" ]] || return 1
        while IFS= read -r line; do
            local so_path base dest
            so_path=$(awk '/=>/{print $3}' <<<"$line" | tr -d ' ')
            [[ -z "$so_path" || "$so_path" == "not" || ! -f "$so_path" ]] && continue
            [[ "$so_path" == "$_ltr_appdir_abs"/* ]] && continue
            base=$(basename "$so_path")
            # Never vendor GL/EGL/GBM stacks; must match host GPU drivers (same rule as libGL*.so below).
            case "$base" in
                libEGL.so*|libGLES*.so*|libgbm.so*|libGL.so*|libGLX.so*|libOpenGL.so*|libGLdispatch.so*|libGLU.so*)
                    continue
                    ;;
            esac
            ltr_dep_bundlable "$base" || continue
            dest="usr/lib/$base"
            if [[ ! -f "$dest" ]]; then
                if cp -f "$so_path" usr/lib/ 2>/dev/null; then
                    print_status "Bundled ${tag} dependency: $base"
                    did_copy=1
                fi
            fi
        done < <(ldd "$elf" 2>/dev/null || true)
        [[ "$did_copy" -eq 1 ]] && return 0
        return 1
    }
    print_status "Bundling libwc/libp3eft OpenCV+ stack (iterative ldd closure)"
    _ltr_round=0
    _ltr_any=1
    while [[ "$_ltr_any" -eq 1 && "$_ltr_round" -lt 40 ]]; do
        _ltr_any=0
        _ltr_round=$((_ltr_round + 1))
        for plugin_base in libwc libp3eft; do
            plug_so=""
            for candidate in usr/lib/linuxtrack/${plugin_base}.so.0.0.0 usr/lib/linuxtrack/${plugin_base}.so.0 usr/lib/linuxtrack/${plugin_base}.so; do
                if [[ -f "$candidate" ]]; then
                    plug_so="$candidate"
                    break
                fi
            done
            [[ -n "$plug_so" ]] || continue
            if ltr_copy_ldd_host_deps "$plug_so" "$plugin_base"; then
                _ltr_any=1
            fi
        done
        shopt -s nullglob
        for elf in usr/lib/libopencv*.so* usr/lib/libtbb*.so* usr/lib/libflexiblas*.so* usr/lib/libprotobuf*.so*; do
            [[ -f "$elf" ]] || continue
            if ltr_copy_ldd_host_deps "$elf" "$(basename "$elf")"; then
                _ltr_any=1
            fi
        done
        shopt -u nullglob
    done
    if [[ "$_ltr_round" -ge 40 ]]; then
        print_warning "OpenCV bundle iteration hit safety cap (40); check ldd for missing deps"
    fi
    unset -f ltr_dep_bundlable ltr_copy_ldd_host_deps
    unset _ltr_appdir_abs _ltr_round _ltr_any

    # FlexiBLAS backend wrappers (Fedora/RHEL); dlopen'd and not always listed on libwc ldd
    for flexdir in /usr/lib64/flexiblas /usr/lib/flexiblas; do
        [[ -d "$flexdir" ]] || continue
        ensure_dir usr/lib/flexiblas
        shopt -s nullglob
        for so in "$flexdir"/*.so*; do
            [[ -f "$so" ]] || continue
            cp -n "$so" usr/lib/flexiblas/ 2>/dev/null || cp -f "$so" usr/lib/flexiblas/ 2>/dev/null || true
        done
        shopt -u nullglob
        print_status "Bundled FlexiBLAS backends from $flexdir"
        break
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

    if [[ -d usr/lib/qt6/plugins ]] || [[ -d usr/lib/qt5/plugins ]]; then
        print_status "Qt plugin layouts: usr/plugins, usr/lib/qt6/plugins (and legacy usr/lib/qt5/plugins if present)"
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
    
    # Ensure no OpenGL / EGL / GBM stacks are bundled (must match host GPU drivers; OpenCV ldd can pull these on some hosts)
    print_status "Removing bundled GL/EGL/GBM libraries (use host drivers)"
    rm -f usr/lib/libGL.so.* usr/lib/libOpenGL.so.* usr/lib/libGLX.so.* usr/lib/libGLdispatch.so.* usr/lib/libGLU.so.* \
        usr/lib/libEGL.so.* usr/lib/libGLESv2.so.* usr/lib/libGLESv1_CM.so.* usr/lib/libgbm.so.* 2>/dev/null || true
    # Strip bundled libgomp if present (e.g. older bundle or linuxdeploy); prefer host OpenMP for one runtime in-process
    rm -f usr/lib/libgomp.so* 2>/dev/null || true
    
    # Create wrapper scripts for CLI tools to ensure AppImage environment is set
    print_status "Creating wrapper scripts for CLI tools (ltr_pipe, ltr_extractor, ltr_recenter, ltr_server1)"
    create_cli_wrapper() {
        local tool_name="$1"
        local wrapper_path="usr/bin/${tool_name}"
        local binary_path="usr/bin/${tool_name}.bin"
        
        # If binary exists, rename it and create wrapper
        if [[ -f "$wrapper_path" && -x "$wrapper_path" && ! -f "$binary_path" ]]; then
            mv "$wrapper_path" "$binary_path"
            cat > "$wrapper_path" << EOF
#!/usr/bin/env bash
# Wrapper script for ${tool_name} to ensure AppImage environment is properly set
# This allows ${tool_name} to work when run directly from the AppImage

# Get the directory where this script is located
SCRIPT_DIR="\$(cd "\$(dirname "\${BASH_SOURCE[0]}")" && pwd)"

# Detect AppImage context by looking for AppRun in parent directories
# or checking if we're in a typical AppImage structure (usr/bin exists with AppRun in parent)
APPDIR="\${APPDIR:-}"
if [[ -z "\$APPDIR" ]]; then
    # Check if we're in usr/bin and AppRun exists in parent
    if [[ "\$SCRIPT_DIR" == */usr/bin ]] && [[ -f "\$(dirname "\$SCRIPT_DIR")/AppRun" ]]; then
        APPDIR="\$(dirname "\$SCRIPT_DIR")"
    # Check if we're in a squashfs-root (extracted AppImage)
    elif [[ "\$SCRIPT_DIR" == */squashfs-root/usr/bin ]] || [[ "\$SCRIPT_DIR" == */AppDir/usr/bin ]]; then
        APPDIR="\$(dirname "\$(dirname "\$SCRIPT_DIR")")"
    # Try to find AppRun by walking up the directory tree
    else
        SEARCH_DIR="\$SCRIPT_DIR"
        while [[ "\$SEARCH_DIR" != "/" ]]; do
            if [[ -f "\$SEARCH_DIR/AppRun" ]]; then
                APPDIR="\$SEARCH_DIR"
                break
            fi
            SEARCH_DIR="\$(dirname "\$SEARCH_DIR")"
        done
    fi
fi

# Set AppImage environment if detected
if [[ -n "\$APPDIR" && -d "\$APPDIR" && -f "\$APPDIR/AppRun" ]]; then
    export APPDIR
    export LD_LIBRARY_PATH="\${APPDIR}/usr/lib:\${APPDIR}/usr/lib/linuxtrack:\${APPDIR}/usr/lib/i386-linux-gnu/linuxtrack\${LD_LIBRARY_PATH:+:\$LD_LIBRARY_PATH}"
    
    # Set LINUXTRACK_LIBS for Wine bridge compatibility
    if [[ -f "\${APPDIR}/usr/lib/linuxtrack/liblinuxtrack32.so.0" ]]; then
        export LINUXTRACK_LIBS="\${APPDIR}/usr/lib/linuxtrack/liblinuxtrack32.so.0:\${APPDIR}/usr/lib/linuxtrack/liblinuxtrack.so.0"
    elif [[ -f "\${APPDIR}/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0" ]]; then
        export LINUXTRACK_LIBS="\${APPDIR}/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0:\${APPDIR}/usr/lib/linuxtrack/liblinuxtrack.so.0"
    elif [[ -f "\${APPDIR}/usr/lib/linuxtrack/liblinuxtrack.so.0" ]]; then
        export LINUXTRACK_LIBS="\${APPDIR}/usr/lib/linuxtrack/liblinuxtrack.so.0"
    fi
    
    # Execute the actual binary with full path
    exec "\${APPDIR}/usr/bin/${tool_name}.bin" "\$@"
else
    # Not in AppImage, execute directly (system installation or already has environment)
    exec "\${SCRIPT_DIR}/${tool_name}.bin" "\$@"
fi
EOF
            chmod +x "$wrapper_path"
            print_status "Created wrapper for ${tool_name}"
        elif [[ -f "$binary_path" ]]; then
            print_status "Wrapper for ${tool_name} already exists"
        else
            print_warning "${tool_name} binary not found, skipping wrapper creation"
        fi
    }
    
    # Create wrappers for all CLI tools
    create_cli_wrapper "ltr_pipe"
    create_cli_wrapper "ltr_extractor"
    create_cli_wrapper "ltr_recenter"
    create_cli_wrapper "ltr_server1"

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

    SQLITE_USR_PLUGINS=$({ find usr/plugins/sqldrivers -maxdepth 1 -type f -name 'libqsqlite.so*' 2>/dev/null || true; } | wc -l || true)
    SQLITE_QT6_PLUGINS=$({ find usr/lib/qt6/plugins/sqldrivers -maxdepth 1 -type f -name 'libqsqlite.so*' 2>/dev/null || true; } | wc -l || true)
    SQLITE_QT5_PLUGINS=$({ find usr/lib/qt5/plugins/sqldrivers -maxdepth 1 -type f -name 'libqsqlite.so*' 2>/dev/null || true; } | wc -l || true)

    if [[ $SQLITE_USR_PLUGINS -gt 0 ]]; then
        print_success "SQLite driver found in usr/plugins/sqldrivers/"
    else
        print_warning "SQLite driver missing from usr/plugins/sqldrivers/"
    fi
    if [[ $SQLITE_QT6_PLUGINS -gt 0 ]]; then
        print_success "SQLite driver found in usr/lib/qt6/plugins/sqldrivers/"
    else
        print_warning "SQLite driver missing from usr/lib/qt6/plugins/sqldrivers/"
    fi
    if [[ $SQLITE_QT5_PLUGINS -gt 0 ]]; then
        print_success "SQLite driver found in usr/lib/qt5/plugins/sqldrivers/ (legacy)"
    fi

    print_success "Bundle complete"

    # Ensure 32-bit linuxtrack runtime is bundled if available on system
    print_status "Ensuring 32-bit liblinuxtrack is bundled if available"
    DEST32_DIR="$(pwd)/usr/lib/i386-linux-gnu/linuxtrack"
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

popd >/dev/null

print_success "Bundle script completed successfully"
