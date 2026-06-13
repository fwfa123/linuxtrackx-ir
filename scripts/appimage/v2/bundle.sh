#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

set -E
trap 'print_error "bundle.sh failed at ${BASH_SOURCE[0]}:$LINENO: $BASH_COMMAND"' ERR

print_status "Bundle: dependencies into AppDir (custom recursive bundler)"

[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR. Run prepare.sh first."

# ============================================================================
# Excludelist — libraries that MUST come from the host system.
# Everything else (ICU, xcb, fontconfig, freetype, dbus, etc.) gets bundled.
# ============================================================================
EXCLUDELIST=(
    ld-linux-x86-64.so.2
    libc.so.6
    libm.so.6
    libpthread.so.0
    libdl.so.2
    librt.so.1
    libresolv.so.2
    libnss_dns.so.2
    libnss_files.so.2
    libnss_compat.so.2
    libnss_nis.so.2
    libnss_nisplus.so.2
    libnsl.so.1
    linux-vdso.so.1
    libgcc_s.so.1
    libstdc++.so.6
    libGL.so.1
    libEGL.so.1
    libGLX.so.0
    libGLdispatch.so.0
    libOpenGL.so.0
    libGLESv2.so.2
    libGLESv1_CM.so.1
    libgbm.so.1
    libdrm.so.2
    libvulkan.so.1
)

is_excluded() {
    local name="$1"
    for excl in "${EXCLUDELIST[@]}"; do
        [[ "$name" == "$excl" ]] && return 0
    done
    return 1
}

# ============================================================================
# Recursive ldd closure — iterate until no new dependencies are discovered.
# This catches everything linuxdeploy missed: ICU, OpenCV chains, etc.
# ============================================================================
bundle_recursive_deps() {
    local appdir="$1"
    local libdir="$appdir/usr/lib"
    local changed=1 round=0

    mkdir -p "$libdir"

    while [[ $changed -eq 1 && $round -lt 50 ]]; do
        changed=0
        round=$((round + 1))

        while IFS= read -r -d '' elf; do
            while IFS= read -r line; do
                local so_path
                so_path=$(awk '/=>/{print $3}' <<<"$line" | tr -d ' ')
                [[ -z "$so_path" || "$so_path" == "not" || ! -f "$so_path" ]] && continue
                [[ "$so_path" == "$appdir"/* ]] && continue

                local base
                base=$(basename "$so_path")
                is_excluded "$base" && continue
                [[ -f "$libdir/$base" ]] && continue

                cp -L "$so_path" "$libdir/$base" 2>/dev/null || cp "$so_path" "$libdir/$base"
                changed=1
            done < <(ldd "$elf" 2>/dev/null || true)
        done < <(find "$appdir" -type f \( -executable -o -name '*.so*' \) -print0 2>/dev/null)
    done

    if [[ $round -ge 50 ]]; then
        print_warning "Dependency closure hit safety cap (50 rounds)"
    fi
    print_success "Dependency closure complete after $round round(s)"
}

# Copy one versioned .so and refresh its usr/lib symlink (overwrite stale copies).
copy_system_shared_lib() {
    local libdir="$1"
    local link_name="$2"
    local d resolved base src=""

    for d in /usr/lib /usr/lib64 /lib /lib64; do
        [[ -e "$d/$link_name" ]] || continue
        resolved=$(readlink -f "$d/$link_name")
        [[ -f "$resolved" ]] || continue
        src="$resolved"
        break
    done
    [[ -n "$src" ]] || return 1

    base=$(basename "$src")
    cp -Lf "$src" "$libdir/$base"
    ln -sfn "$base" "$libdir/$link_name"
    return 0
}

# OpenCV expects a matched BLAS/LAPACK set. Recursive bundling can leave an
# older libblas.so.3 in AppDir (ldd passes, dlopen fails on missing ssyrk_64_).
sync_lapack_stack() {
    local libdir="$1/usr/lib"
    local name copied=0

    mkdir -p "$libdir"
    for name in libblas.so.3 libcblas.so.3 liblapack.so.3 libgfortran.so.5; do
        if copy_system_shared_lib "$libdir" "$name"; then
            copied=$((copied + 1))
        fi
    done
    if [[ $copied -gt 0 ]]; then
        print_success "Synced LAPACK/BLAS family ($copied libs) from builder system"
    fi
}

verify_linuxtrack_dlopen() {
    local appdir="$1"
    local lp="$appdir/usr/lib"
    local lib failed=0

    [[ -d "$appdir/usr/lib/flexiblas" ]] && lp="$lp:$appdir/usr/lib/flexiblas"
    lp="$lp:$appdir/usr/lib/linuxtrack"

    for lib in libwc.so.0 libp3eft.so.0; do
        [[ -e "$appdir/usr/lib/linuxtrack/$lib" ]] || continue
        if LD_LIBRARY_PATH="$lp" python3 -c "import ctypes, os; ctypes.CDLL(os.path.join('${appdir}','usr/lib/linuxtrack','${lib}'))" 2>/dev/null; then
            print_success "dlopen OK: usr/lib/linuxtrack/$lib"
        else
            print_error "dlopen failed: usr/lib/linuxtrack/$lib"
            LD_LIBRARY_PATH="$lp" python3 -c "import ctypes, os; ctypes.CDLL(os.path.join('${appdir}','usr/lib/linuxtrack','${lib}'))" 2>&1 | head -3 || true
            failed=1
        fi
    done
    return $failed
}

# ============================================================================
# Locate qmake for Qt path queries
# ============================================================================
find_qmake() {
    for _c in qmake6 /usr/lib64/qt6/bin/qmake /usr/lib/qt6/bin/qmake \
              /usr/lib/x86_64-linux-gnu/qt6/bin/qmake qmake; do
        if command -v "$_c" >/dev/null 2>&1; then
            echo "$_c"; return 0
        fi
        if [[ -x "$_c" ]]; then
            echo "$_c"; return 0
        fi
    done
    return 1
}

# ============================================================================
# Main bundling
# ============================================================================
pushd "$APPDIR" >/dev/null

    QMAKE=$(find_qmake) || print_warning "qmake not found; Qt paths will use fallbacks"

    # ------------------------------------------------------------------
    # 1. Qt plugins — copy directly from known paths (replaces linuxdeploy-plugin-qt)
    # ------------------------------------------------------------------
    print_status "Bundling Qt6 plugins"
    QT_PLUGIN_SRC=""
    if [[ -n "${QMAKE:-}" ]]; then
        QT_PLUGIN_SRC=$("$QMAKE" -query QT_INSTALL_PLUGINS 2>/dev/null || true)
    fi
    if [[ -z "$QT_PLUGIN_SRC" || ! -d "$QT_PLUGIN_SRC" ]]; then
        for _d in /usr/lib/x86_64-linux-gnu/qt6/plugins /usr/lib64/qt6/plugins /usr/lib/qt6/plugins; do
            [[ -d "$_d" ]] && { QT_PLUGIN_SRC="$_d"; break; }
        done
    fi
    if [[ -n "$QT_PLUGIN_SRC" && -d "$QT_PLUGIN_SRC" ]]; then
        for subdir in platforms sqldrivers imageformats iconengines xcbglintegrations; do
            [[ -d "$QT_PLUGIN_SRC/$subdir" ]] || continue
            ensure_dir "usr/plugins/$subdir"
            shopt -s nullglob
            for so in "$QT_PLUGIN_SRC/$subdir"/*.so; do
                cp -f "$so" "usr/plugins/$subdir/"
            done
            shopt -u nullglob
            print_status "Copied Qt plugins: $subdir"
        done
        print_success "Qt plugins bundled from $QT_PLUGIN_SRC"
    else
        print_warning "Could not locate Qt plugin directory"
    fi

    # ------------------------------------------------------------------
    # 2. Qt shared libraries
    # ------------------------------------------------------------------
    print_status "Bundling Qt6 shared libraries"
    QT_LIB_DIR=""
    if [[ -n "${QMAKE:-}" ]]; then
        QT_LIB_DIR=$("$QMAKE" -query QT_INSTALL_LIBS 2>/dev/null || true)
    fi
    if [[ -z "$QT_LIB_DIR" || ! -d "$QT_LIB_DIR" ]]; then
        for _d in /usr/lib/x86_64-linux-gnu /usr/lib64 /usr/lib; do
            if [[ -f "$_d/libQt6Core.so.6" || -f "$_d/libQt6Core.so" ]]; then
                QT_LIB_DIR="$_d"; break
            fi
        done
    fi
    if [[ -n "$QT_LIB_DIR" && -d "$QT_LIB_DIR" ]]; then
        _qt_n=0
        shopt -s nullglob
        for _qf in "$QT_LIB_DIR"/libQt6*.so*; do
            [[ -e "$_qf" ]] || continue
            _qbase=$(basename "$_qf")
            [[ "$_qbase" == *.prl ]] && continue
            cp -L -f "$_qf" "usr/lib/$_qbase" 2>/dev/null || cp -f "$_qf" "usr/lib/"
            _qt_n=$((_qt_n + 1))
        done
        shopt -u nullglob
        print_success "Copied $_qt_n Qt6 libraries from $QT_LIB_DIR"
    else
        print_warning "Could not locate Qt library directory"
    fi

    # ------------------------------------------------------------------
    # 3. Qt translations
    # ------------------------------------------------------------------
    print_status "Bundling Qt translation catalogs"
    ensure_dir usr/share/qt6/translations
    QT_TRANS_DIR=""
    if [[ -n "${QMAKE:-}" ]]; then
        QT_TRANS_DIR=$("$QMAKE" -query QT_INSTALL_TRANSLATIONS 2>/dev/null || true)
    fi
    if [[ -z "$QT_TRANS_DIR" || ! -d "$QT_TRANS_DIR" ]]; then
        for _d in /usr/share/qt6/translations /usr/lib64/qt6/translations /usr/lib/qt6/translations \
                  /usr/lib/x86_64-linux-gnu/qt6/translations; do
            [[ -d "$_d" ]] && { QT_TRANS_DIR="$_d"; break; }
        done
    fi
    if [[ -n "$QT_TRANS_DIR" && -d "$QT_TRANS_DIR" ]]; then
        _n=0
        shopt -s nullglob
        for _f in "$QT_TRANS_DIR"/qtbase_*.qm "$QT_TRANS_DIR"/qt_help_*.qm "$QT_TRANS_DIR"/qtdeclarative_*.qm; do
            [[ -f "$_f" ]] || continue
            cp -f "$_f" usr/share/qt6/translations/
            _n=$((_n + 1))
        done
        shopt -u nullglob
        print_status "Copied $_n Qt .qm files"
    else
        print_warning "Could not locate Qt translations directory"
    fi

    # ------------------------------------------------------------------
    # 4. qt.conf — tells Qt to use AppDir paths, not host paths
    # ------------------------------------------------------------------
    print_status "Writing usr/bin/qt.conf"
    cat > usr/bin/qt.conf << 'QTEOF'
[Paths]
Prefix = ..
Plugins = plugins:lib/qt6/plugins
Libraries = lib
Binaries = bin
QTEOF
    if [[ -d usr/share/qt6/translations ]]; then
        echo "Translations = share/qt6/translations" >> usr/bin/qt.conf
    fi
    print_success "qt.conf written"

    # ------------------------------------------------------------------
    # 5. 3D model assets
    # ------------------------------------------------------------------
    print_status "Bundling 3D model assets"
    ensure_dir usr/share/linuxtrack
    ASSET_SRC_DIR="$PROJECT_ROOT/src/qt_gui"
    for a in sphere.obj sparow_opaq.obj sparow_glass.obj; do
        [[ -f "$ASSET_SRC_DIR/$a" ]] || die "Missing 3D asset: $ASSET_SRC_DIR/$a"
        cp -f "$ASSET_SRC_DIR/$a" usr/share/linuxtrack/
    done
    print_success "3D model assets bundled"

    # ------------------------------------------------------------------
    # 6. Qt Help system files
    # ------------------------------------------------------------------
    print_status "Bundling Qt Help system files"
    ensure_dir usr/share/linuxtrack/help/ltr_gui
    ensure_dir usr/share/linuxtrack/help/mickey

    for _pair in qt_gui:ltr_gui mickey:mickey; do
        _src_name="${_pair%%:*}"
        _dest_name="${_pair##*:}"
        for _f in help.qhc help.qch; do
            if [[ -f "$PROJECT_ROOT/src/$_src_name/$_f" ]]; then
                cp -f "$PROJECT_ROOT/src/$_src_name/$_f" "usr/share/linuxtrack/help/$_dest_name/"
                print_status "Copied $_dest_name/$_f"
            fi
        done
        if [[ -d "$PROJECT_ROOT/src/$_src_name/help" ]]; then
            cp -r "$PROJECT_ROOT/src/$_src_name/help"/* "usr/share/linuxtrack/help/$_dest_name/" 2>/dev/null || true
            print_status "Copied $_dest_name help content"
        fi
    done
    print_success "Qt Help system files bundled"

    # ------------------------------------------------------------------
    # 7. FlexiBLAS backends (before dependency closure so ldd pulls their deps)
    # ------------------------------------------------------------------
    for flexdir in /usr/lib64/flexiblas /usr/lib/flexiblas /usr/lib/x86_64-linux-gnu/flexiblas; do
        [[ -d "$flexdir" ]] || continue
        ensure_dir usr/lib/flexiblas
        shopt -s nullglob
        for so in "$flexdir"/*.so*; do
            cp -n "$so" usr/lib/flexiblas/ 2>/dev/null || true
        done
        shopt -u nullglob
        print_status "Bundled FlexiBLAS backends from $flexdir"
        break
    done

    # ------------------------------------------------------------------
    # 8. Recursive dependency closure
    # ------------------------------------------------------------------
    print_status "Running recursive dependency closure on all ELF files"
    _ltr_lp="$(pwd)/usr/lib"
    [[ -d usr/lib/flexiblas ]] && _ltr_lp="$_ltr_lp:$(pwd)/usr/lib/flexiblas"
    _ltr_lp="$_ltr_lp:$(pwd)/usr/lib/linuxtrack"
    export LD_LIBRARY_PATH="${_ltr_lp}${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
    bundle_recursive_deps "$(pwd)"
    sync_lapack_stack "$(pwd)"
    bundle_recursive_deps "$(pwd)"

    # ------------------------------------------------------------------
    # 9. Remove GPU/GL stacks (must come from host hardware drivers)
    # ------------------------------------------------------------------
    print_status "Removing bundled GL/EGL/GBM libraries (must use host drivers)"
    rm -f usr/lib/libGL.so.* usr/lib/libOpenGL.so.* usr/lib/libGLX.so.* \
          usr/lib/libGLdispatch.so.* usr/lib/libGLU.so.* \
          usr/lib/libEGL.so.* usr/lib/libGLESv2.so.* usr/lib/libGLESv1_CM.so.* \
          usr/lib/libgbm.so.* 2>/dev/null || true

    # ------------------------------------------------------------------
    # 10. Set rpaths
    # ------------------------------------------------------------------
    print_status "Setting rpaths for binaries and libraries"
    if command -v patchelf >/dev/null 2>&1; then
        for bin in usr/bin/*; do
            [[ -f "$bin" && -x "$bin" ]] || continue
            set_rpath_binary "$bin"
        done

        find usr/lib -maxdepth 1 -name "*.so*" -type f -print0 2>/dev/null | \
            while IFS= read -r -d '' so; do
                set_rpath_library "$so"
            done

        if [[ -d usr/lib/flexiblas ]]; then
            find usr/lib/flexiblas -name "*.so*" -type f -print0 2>/dev/null | \
                while IFS= read -r -d '' so; do
                    set_rpath_library "$so"
                done
        fi

        if [[ -d usr/lib/linuxtrack ]]; then
            find usr/lib/linuxtrack -name "*.so*" -type f -print0 2>/dev/null | \
                while IFS= read -r -d '' so; do
                    set_rpath_library "$so"
                done
        fi

        find usr/plugins -name "*.so" -type f -print0 2>/dev/null | \
            while IFS= read -r -d '' so; do
                patchelf --set-rpath '$ORIGIN/../../lib:$ORIGIN' "$so" 2>/dev/null || true
            done

        print_success "rpaths set"
    else
        print_warning "patchelf not available; skipping rpath adjustments"
    fi

    # ------------------------------------------------------------------
    # 10b. Verify webcam / face-track drivers actually dlopen
    # ------------------------------------------------------------------
    print_status "Verifying libwc / libp3eft dlopen under AppDir LD_LIBRARY_PATH"
    if ! verify_linuxtrack_dlopen "$(pwd)"; then
        die "libwc/libp3eft dlopen failed — face tracking / webcam will not load"
    fi

    # ------------------------------------------------------------------
    # 11. Desktop file at AppDir root (required by appimagetool)
    # ------------------------------------------------------------------
    DESKTOP_FILE="usr/share/applications/linuxtrack.desktop"
    if [[ -f "$DESKTOP_FILE" && ! -f "$(basename "$DESKTOP_FILE")" ]]; then
        cp "$DESKTOP_FILE" "$(basename "$DESKTOP_FILE")"
    fi

    # ------------------------------------------------------------------
    # 12. CLI tool wrappers
    # ------------------------------------------------------------------
    print_status "Creating wrapper scripts for CLI tools"
    create_cli_wrapper() {
        local tool_name="$1"
        local wrapper_path="usr/bin/${tool_name}"
        local binary_path="usr/bin/${tool_name}.bin"

        if [[ -f "$wrapper_path" && -x "$wrapper_path" && ! -f "$binary_path" ]]; then
            mv "$wrapper_path" "$binary_path"
            cat > "$wrapper_path" << WEOF
#!/usr/bin/env bash
SCRIPT_DIR="\$(cd "\$(dirname "\${BASH_SOURCE[0]}")" && pwd)"
APPDIR="\${APPDIR:-}"
if [[ -z "\$APPDIR" ]]; then
    SEARCH_DIR="\$SCRIPT_DIR"
    while [[ "\$SEARCH_DIR" != "/" ]]; do
        [[ -f "\$SEARCH_DIR/AppRun" ]] && { APPDIR="\$SEARCH_DIR"; break; }
        SEARCH_DIR="\$(dirname "\$SEARCH_DIR")"
    done
fi
if [[ -n "\$APPDIR" && -f "\$APPDIR/AppRun" ]]; then
    export APPDIR
    export LD_LIBRARY_PATH="\${APPDIR}/usr/lib:\${APPDIR}/usr/lib/linuxtrack\${LD_LIBRARY_PATH:+:\$LD_LIBRARY_PATH}"
    exec "\${APPDIR}/usr/bin/${tool_name}.bin" "\$@"
else
    exec "\${SCRIPT_DIR}/${tool_name}.bin" "\$@"
fi
WEOF
            chmod +x "$wrapper_path"
            print_status "Created wrapper for ${tool_name}"
        elif [[ -f "$binary_path" ]]; then
            print_status "Wrapper for ${tool_name} already exists"
        else
            print_warning "${tool_name} binary not found, skipping wrapper"
        fi
    }

    create_cli_wrapper "ltr_pipe"
    create_cli_wrapper "ltr_extractor"
    create_cli_wrapper "ltr_recenter"
    create_cli_wrapper "ltr_server1"

    # ------------------------------------------------------------------
    # 13. Verify key libraries are bundled
    # ------------------------------------------------------------------
    print_status "Verifying key bundled libraries"
    _ok=0 _warn=0
    for _check in libQt6Core.so libusb-1.0.so libudev.so libmxml.so "libicu*.so"; do
        if [[ -n "$(find usr/lib -maxdepth 1 \( -type f -o -type l \) -name "${_check}*" -print -quit 2>/dev/null)" ]]; then
            print_success "Bundled: $_check"
            _ok=$((_ok + 1))
        else
            print_warning "Missing: $_check"
            _warn=$((_warn + 1))
        fi
    done
    print_status "Verification: $_ok present, $_warn warnings"

    print_success "Bundle complete"

popd >/dev/null

print_success "Bundle script completed successfully"
