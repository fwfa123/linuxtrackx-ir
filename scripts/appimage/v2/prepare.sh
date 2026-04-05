#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Prepare: build and install into AppDir (v2)"

if [ "${CLEAN:-0}" = "1" ]; then
    print_status "Cleaning previous AppDir at $APPDIR"
    rm -rf "$APPDIR"
fi

ensure_dir "$APPDIR"

pushd "$PROJECT_ROOT" >/dev/null
    require_cmd cmake
    require_cmd make
    require_qhelpgenerator

    # Standard AppImage does not ship Wiimote (wii_server). CMake DISABLE_WIIMOTE=ON overrides libcwiid on the build host.

    print_status "Preparing CMake build"
    rm -rf build
    mkdir -p build

    # README installation level 5 (cumulative): Wine + X-Plane + webcam + OSC — same flags as docs (no OpenCV facetrack)
    : "${XPLANE_SDK_PATH:=/opt/xplane-sdk/CHeaders}"
    print_status "Configuring with CMake (README level 5: LTR32 + X-Plane + webcam + OSC; ENABLE_FACE_TRACKER=OFF; no Wiimote)"
    cd build
    cmake .. \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DENABLE_LDCONFIG=OFF \
        -DENABLE_LTR_32LIB_ON_X64=ON \
        -DENABLE_WEBCAM=ON \
        -DENABLE_OSC=ON \
        -DENABLE_FACE_TRACKER=OFF \
        -DENABLE_XPLANE=ON \
        -DDISABLE_WIIMOTE=ON \
        "-DXPLANE_SDK_PATH=${XPLANE_SDK_PATH}"

    print_status "Building"
    cmake --build . -j"$JOBS"
    cd ..

    print_status "Qt Help artifacts: decide whether to regenerate"

    # Allow callers to control help regeneration explicitly
    REGENERATE_HELP=${REGENERATE_HELP:-1}

    have_repo_artifacts=true
    [[ -f src/qt_gui/help.qhc && -f src/qt_gui/help.qch ]] || have_repo_artifacts=false
    if [[ -f src/mickey/help.qhc && -f src/mickey/help.qch ]]; then
        :
    else
        have_repo_artifacts=false
    fi

    if [[ "$REGENERATE_HELP" != "1" && "$have_repo_artifacts" = true ]]; then
        print_status "REGENERATE_HELP=0 and artifacts exist → skipping help regeneration"
    else
        print_status "Regenerating Qt Help artifacts (Qt6)"

        print_status "Verifying Qt tools availability and compatibility"
        QMAKE_BIN=""
        if command -v qmake6 >/dev/null 2>&1; then
            QMAKE_BIN="qmake6"
        elif [[ -x /usr/lib64/qt6/bin/qmake ]]; then
            QMAKE_BIN="/usr/lib64/qt6/bin/qmake"
        elif [[ -x /usr/lib/qt6/bin/qmake ]]; then
            QMAKE_BIN="/usr/lib/qt6/bin/qmake"
        elif command -v qmake >/dev/null 2>&1; then
            QMAKE_BIN="qmake"
        fi

        QT_VERSION="unknown"
        if [[ -n "$QMAKE_BIN" ]]; then
            QT_VERSION=$("$QMAKE_BIN" -query QT_VERSION 2>/dev/null || echo "unknown")
        fi
        if [[ "$QT_VERSION" = "unknown" ]]; then
            die "Cannot detect Qt6 qmake; install qt6-base-devel (e.g. qmake6 or /usr/lib64/qt6/bin/qmake)"
        fi

        if ! find_qhelpgenerator_path; then
            die "qhelpgenerator not found; install Qt6 help tools (Fedora: qt6-doctools; Debian/Ubuntu: qt6-tools-dev-tools)"
        fi
        QHELPGEN="$QHELPGENERATOR_PATH"
        if [[ "$QHELPGEN" != *qt6* ]] && [[ "$QHELPGEN" != *lib64/qt6* ]] && [[ "$QHELPGEN" != *lib/qt6* ]]; then
            print_warning "Using qhelpgenerator: $QHELPGEN — ensure it matches Qt6 ($QT_VERSION)"
        fi

        print_success "Qt $QT_VERSION detected, using qhelpgenerator: $QHELPGEN"

        rm -f src/mickey/help.qhc src/mickey/help.qch
        rm -f src/qt_gui/help.qhc src/qt_gui/help.qch

        if [[ -f src/mickey/mickey.qhp && -f src/mickey/mickey.qhcp ]]; then
            print_status "Generating mickey help files"
            cd src/mickey
            # help.qch must exist before the collection (.qhc) registers it
            $QHELPGEN mickey.qhp -o help.qch
            $QHELPGEN mickey.qhcp -o help.qhc
            cd ../..
            print_success "Mickey help files generated"
        else
            print_status "Mickey Qt Help project not present; HTML help only (see src/mickey/help/)"
        fi

        if [[ -f src/qt_gui/ltr_gui.qhp && -f src/qt_gui/ltr_gui.qhcp ]]; then
            print_status "Generating qt_gui help files (project + collection)"
            cd src/qt_gui
            # Collection (.qhc) registers help.qch — generate content first
            $QHELPGEN ltr_gui.qhp -o help.qch
            $QHELPGEN ltr_gui.qhcp -o help.qhc
            cd ../..
            print_success "Qt GUI help files generated"
        elif [[ -f src/qt_gui/ltr_gui.qhp ]]; then
            print_status "Generating qt_gui help.qch from ltr_gui.qhp only"
            cd src/qt_gui
            $QHELPGEN ltr_gui.qhp -o help.qch
            cd ../..
            print_success "Qt GUI help.qch generated (no .qhcp — collection optional)"
        else
            print_warning "ltr_gui.qhp not found; skipping Qt Help binary generation"
        fi
    fi

    if [[ ! -d src/qt_gui/help ]]; then
        die "Required HTML help missing: src/qt_gui/help/"
    fi

    print_status "Preflight: verifying Qt Help outputs and format compatibility"
    MISSING=0
    INCOMPATIBLE=0

    validate_help_file() {
        local help_file="$1"
        if [[ ! -f "$help_file" ]]; then
            print_error "Missing help artifact: $help_file"
            return 1
        fi
        if command -v sqlite3 >/dev/null 2>&1; then
            print_status "Validating help file format: $help_file"
            if sqlite3 "$help_file" ".tables" >/dev/null 2>&1; then
                print_success "Help file $help_file is a valid SQLite database"
                local tables
                tables=$(sqlite3 "$help_file" ".tables" 2>/dev/null)
                if [[ "$tables" =~ ContentsTable|FileDataTable|NamespaceTable ]]; then
                    print_success "Help file $help_file has expected tables"
                else
                    print_warning "Help file $help_file unexpected schema: $tables"
                fi
            else
                print_error "Help file $help_file is not a valid SQLite database"
                INCOMPATIBLE=1
            fi
        else
            die "sqlite3 not available - help file validation required for AppImage build"
        fi
        return 0
    }

    for component in "mickey" "qt_gui"; do
        if [[ -f "src/$component/help.qhc" ]]; then
            validate_help_file "src/$component/help.qhc" || die "help.qhc validation failed: $component"
        fi
        if [[ -f "src/$component/help.qch" ]]; then
            validate_help_file "src/$component/help.qch" || die "help.qch validation failed: $component"
        fi
    done

    if [[ $INCOMPATIBLE -ne 0 ]]; then
        print_warning "Some help files may have compatibility issues - regenerate with Qt6 qhelpgenerator"
    fi

    print_success "Qt Help preflight complete (HTML under src/*/help/ required; .qhc/.qch optional)"

    print_status "Installing to AppDir"
    cd build
    cmake --install . --prefix "$APPDIR/usr"
    cd ..
popd >/dev/null

write_minimal_apprun
ensure_desktop_and_icons
copy_udev_rules_if_present

# Best-effort: bundle 32-bit liblinuxtrack for Wine 32-bit prefixes
print_status "Attempting to bundle 32-bit linuxtrack runtime (best-effort)"
if command -v gcc >/dev/null 2>&1; then
    if echo 'int main(){}' | gcc -m32 -x c - -o /tmp/.ltr32check 2>/dev/null; then
        rm -f /tmp/.ltr32check
        TMP_BUILD32=$(mktemp -d)
        print_status "Building 32-bit liblinuxtrack in $TMP_BUILD32"
        pushd "$PROJECT_ROOT" >/dev/null
            export CFLAGS="-m32"
            export CXXFLAGS="-m32"
            export LDFLAGS="-m32"
            # Common Debian/Ubuntu multiarch pkg-config path
            export PKG_CONFIG_PATH="/usr/lib/i386-linux-gnu/pkgconfig"
            ./configure --prefix=/usr >/dev/null 2>&1 || true
            # Build only the core library if possible; if full build runs, that's fine too
            if make -j"$JOBS" -C src liblinuxtrack.la >/dev/null 2>&1 || make -j"$JOBS" >/dev/null 2>&1; then
                LIB32_PATH="$(pwd)/src/.libs/liblinuxtrack.so.0.0.0"
                if [[ -f "$LIB32_PATH" ]]; then
                    DEST32_DIR="$APPDIR/usr/lib/i386-linux-gnu/linuxtrack"
                    print_status "Bundling 32-bit lib to $DEST32_DIR"
                    mkdir -p "$DEST32_DIR"
                    cp "$LIB32_PATH" "$DEST32_DIR/"
                    ( cd "$DEST32_DIR" && ln -sf liblinuxtrack.so.0.0.0 liblinuxtrack.so.0 )
                    print_success "Bundled 32-bit liblinuxtrack"
                else
                    die "32-bit liblinuxtrack build succeeded but library not found at $LIB32_PATH"
                fi
            else
                die "Failed to build 32-bit liblinuxtrack (toolchain or deps missing)"
            fi
        popd >/dev/null
        rm -rf "$TMP_BUILD32"
    else
        die "gcc -m32 not available; 32-bit runtime required for Wine compatibility"
    fi
else
    die "gcc not found; 32-bit runtime required for Wine compatibility"
fi

print_success "Prepare complete"


