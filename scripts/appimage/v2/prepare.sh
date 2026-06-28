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

    print_status "Preparing CMake build"
    rm -rf build
    mkdir -p build

    check_appimage_build_deps

    _cmake_on_off() { [[ "$1" == "1" ]] && echo ON || echo OFF; }

    _xplane_flag="-DENABLE_XPLANE=OFF"
    if [[ "${ENABLE_XPLANE:-1}" == "1" ]]; then
        if [[ -d "$XPLANE_SDK_PATH" ]]; then
            _xplane_flag="-DENABLE_XPLANE=ON"
        elif [[ "${REQUIRE_XPLANE_SDK:-1}" == "1" ]]; then
            die "X-Plane SDK required but not found at $XPLANE_SDK_PATH"
        else
            print_warning "X-Plane SDK not found at $XPLANE_SDK_PATH; disabling X-Plane plugin"
        fi
    fi

    _disable_wiimote=OFF
    [[ "${ENABLE_WIIMOTE:-1}" == "1" ]] || _disable_wiimote=ON

    _ltr32_flag=OFF
    [[ "${ENABLE_LTR_32LIB_ON_X64:-1}" == "1" ]] && _ltr32_flag=ON

    print_status "Configuring with CMake (Level 7 defaults from config.sh; override with ENABLE_*=0)"
    cd build
    cmake .. \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DENABLE_LDCONFIG=OFF \
        -DENABLE_LTR_32LIB_ON_X64="$_ltr32_flag" \
        -DLIB32DIR=i386-linux-gnu \
        -DENABLE_WEBCAM="$(_cmake_on_off "${ENABLE_WEBCAM:-1}")" \
        -DENABLE_OSC="$(_cmake_on_off "${ENABLE_OSC:-1}")" \
        -DENABLE_FACE_TRACKER="$(_cmake_on_off "${ENABLE_FACE_TRACKER:-1}")" \
        "$_xplane_flag" \
        -DDISABLE_WIIMOTE="$_disable_wiimote" \
        "-DXPLANE_SDK_PATH=${XPLANE_SDK_PATH}"

    print_status "Building"
    cmake --build . -j"$JOBS"
    cd ..

    print_status "Qt Help artifacts: decide whether to regenerate"

    REGENERATE_HELP=${REGENERATE_HELP:-1}

    have_repo_artifacts=true
    [[ -f src/qt_gui/help.qhc && -f src/qt_gui/help.qch ]] || have_repo_artifacts=false
    if [[ -f src/mickey/help.qhc && -f src/mickey/help.qch ]]; then
        :
    else
        have_repo_artifacts=false
    fi

    if [[ "$REGENERATE_HELP" != "1" && "$have_repo_artifacts" = true ]]; then
        print_status "REGENERATE_HELP=0 and artifacts exist -> skipping help regeneration"
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
    INCOMPATIBLE=0

    validate_help_file() {
        local help_file="$1"
        if [[ ! -f "$help_file" ]]; then
            print_error "Missing help artifact: $help_file"
            return 1
        fi
        if command -v sqlite3 >/dev/null 2>&1; then
            if sqlite3 "$help_file" ".tables" >/dev/null 2>&1; then
                print_success "Help file valid: $help_file"
            else
                print_error "Help file $help_file is not a valid SQLite database"
                INCOMPATIBLE=1
            fi
        elif command -v python3 >/dev/null 2>&1; then
            if python3 -c "import sqlite3, sys; sqlite3.connect(sys.argv[1]).execute('select 1').fetchone()" "$help_file" >/dev/null 2>&1; then
                print_success "Help file valid: $help_file (checked with python3)"
            else
                print_error "Help file $help_file is not a valid SQLite database"
                INCOMPATIBLE=1
            fi
        else
            die "sqlite3 or python3 required - help file validation required for AppImage build"
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

    print_success "Qt Help preflight complete"

    print_status "Installing to AppDir"
    cd build
    cmake --install . --prefix "$APPDIR/usr"
    cd ..

    # Normalize 32-bit linuxtrack soname expected by package validation.
    # Some installs produce liblinuxtrack32.so.* in multiarch path; ensure
    # compatibility names liblinuxtrack.so* are present as aliases.
    _ltr32_dir="$APPDIR/usr/lib/i386-linux-gnu/linuxtrack"
    if [[ -d "$_ltr32_dir" ]]; then
        if [[ -e "$_ltr32_dir/liblinuxtrack32.so.0.0.0" && ! -e "$_ltr32_dir/liblinuxtrack.so.0.0.0" ]]; then
            ln -sf liblinuxtrack32.so.0.0.0 "$_ltr32_dir/liblinuxtrack.so.0.0.0"
        fi
        if [[ -e "$_ltr32_dir/liblinuxtrack.so.0.0.0" && ! -e "$_ltr32_dir/liblinuxtrack.so.0" ]]; then
            ln -sf liblinuxtrack.so.0.0.0 "$_ltr32_dir/liblinuxtrack.so.0"
        fi
        if [[ -e "$_ltr32_dir/liblinuxtrack.so.0" && ! -e "$_ltr32_dir/liblinuxtrack.so" ]]; then
            ln -sf liblinuxtrack.so.0 "$_ltr32_dir/liblinuxtrack.so"
        fi
    fi
popd >/dev/null

write_minimal_apprun
ensure_desktop_and_icons
copy_udev_rules_if_present

print_success "Prepare complete"
