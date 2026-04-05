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

    : "${XPLANE_SDK_PATH:=/opt/xplane-sdk/CHeaders}"
    if [[ "${REQUIRE_XPLANE_SDK:-0}" == "1" && ! -d "$XPLANE_SDK_PATH" ]]; then
        die "X-Plane SDK required but not found at $XPLANE_SDK_PATH (install headers under CHeaders/XPLM or rebuild the Docker image with WITH_XPLANE_SDK=1)"
    fi
    _xplane_flag="-DENABLE_XPLANE=ON"
    if [[ ! -d "$XPLANE_SDK_PATH" ]]; then
        print_warning "X-Plane SDK not found at $XPLANE_SDK_PATH; disabling X-Plane plugin"
        _xplane_flag="-DENABLE_XPLANE=OFF"
    fi

    print_status "Configuring with CMake (LTR32 + webcam + OSC; ENABLE_FACE_TRACKER=OFF; no Wiimote)"
    cd build
    cmake .. \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DENABLE_LDCONFIG=OFF \
        -DENABLE_LTR_32LIB_ON_X64=ON \
        -DLIB32DIR=i386-linux-gnu \
        -DENABLE_WEBCAM=ON \
        -DENABLE_OSC=ON \
        -DENABLE_FACE_TRACKER=OFF \
        "$_xplane_flag" \
        -DDISABLE_WIIMOTE=ON \
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

    print_success "Qt Help preflight complete"

    print_status "Installing to AppDir"
    cd build
    cmake --install . --prefix "$APPDIR/usr"
    cd ..
popd >/dev/null

write_minimal_apprun
ensure_desktop_and_icons
copy_udev_rules_if_present

print_success "Prepare complete"
