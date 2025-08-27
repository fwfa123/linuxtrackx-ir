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
    require_cmd autoreconf
    require_cmd make
    require_cmd qhelpgenerator

    print_status "Regenerating build system"
    make distclean >/dev/null 2>&1 || true
    autoreconf -fiv

    print_status "Configuring"
    ./configure --prefix=/usr

    print_status "Building"
    make -j"$JOBS"

    print_status "Qt Help artifacts: decide whether to regenerate"

    # Allow callers to control help regeneration explicitly
    REGENERATE_HELP=${REGENERATE_HELP:-1}

    have_repo_artifacts=true
    [[ -f src/qt_gui/help.qhc && -f src/qt_gui/help.qch ]] || have_repo_artifacts=false
    [[ -f src/mickey/help.qhc && -f src/mickey/help.qch ]] || have_repo_artifacts=false

    if [[ "$REGENERATE_HELP" != "1" && "$have_repo_artifacts" = true ]]; then
        print_status "REGENERATE_HELP=0 and artifacts exist → skipping help regeneration"
    else
        print_status "Regenerating Qt Help artifacts"

        # Verify Qt tools are available and compatible
        print_status "Verifying Qt tools availability and compatibility"
        QT_VERSION=$(qmake -query QT_VERSION 2>/dev/null || echo "unknown")

        if [[ "$QT_VERSION" = "unknown" ]]; then
            die "Cannot detect qmake Qt version; install Qt development tools"
        fi

        # Extract major version from Qt version
        QT_MAJOR=$(sed -n 's/^\([0-9]\)\..*/\1/p' <<<"$QT_VERSION" | head -1 || true)

        if [[ -z "$QT_MAJOR" ]]; then
            die "Failed to parse Qt major version from: '$QT_VERSION'"
        fi

        # Verify Qt5 qhelpgenerator is available (prefer Qt5-specific version)
        QT5_QHELPGENERATOR=""
        if command -v qhelpgenerator-qt5 >/dev/null 2>&1; then
            QT5_QHELPGENERATOR="qhelpgenerator-qt5"
        elif [[ -f "/usr/lib/qt5/bin/qhelpgenerator" ]]; then
            QT5_QHELPGENERATOR="/usr/lib/qt5/bin/qhelpgenerator"
        elif command -v qhelpgenerator >/dev/null 2>&1; then
            QT5_QHELPGENERATOR="qhelpgenerator"
            print_warning "Using system qhelpgenerator - this may cause compatibility issues if it's not Qt5"
        else
            die "qhelpgenerator not found; install Qt5 help tools (qt5-tools-help or similar)"
        fi

        print_success "Qt $QT_VERSION detected, using qhelpgenerator: $QT5_QHELPGENERATOR"

        # Clean up before regeneration to avoid mixing outputs
        rm -f src/mickey/help.qhc src/mickey/help.qch
        rm -f src/qt_gui/help.qhc src/qt_gui/help.qch

        # Generate help files with the verified toolchain
        if [[ -f src/mickey/mickey.qhp && -f src/mickey/mickey.qhcp ]]; then
            print_status "Generating mickey help files"
            cd src/mickey
            $QT5_QHELPGENERATOR mickey.qhcp -o help.qhc
            $QT5_QHELPGENERATOR mickey.qhp -o help.qch
            cd ../..
            print_success "Mickey help files generated"
        else
            print_warning "Mickey help source files not found, skipping help generation"
        fi

        if [[ -f src/qt_gui/ltr_gui.qhp && -f src/qt_gui/ltr_gui.qhcp ]]; then
            print_status "Generating qt_gui help files"
            cd src/qt_gui
            $QT5_QHELPGENERATOR ltr_gui.qhcp -o help.qhc
            $QT5_QHELPGENERATOR ltr_gui.qhp -o help.qch
            cd ../..
            print_success "Qt GUI help files generated"
        else
            print_warning "Qt GUI help source files not found, skipping help generation"
        fi
    fi

    # Fail early if required help source files are missing
    if [[ ! -f src/mickey/mickey.qhp || ! -f src/mickey/mickey.qhcp || ! -f src/qt_gui/ltr_gui.qhp || ! -f src/qt_gui/ltr_gui.qhcp ]]; then
        die "Required help source files missing. Cannot proceed with AppImage build."
    fi

    # Preflight: verify Qt Help outputs exist and validate format compatibility
    print_status "Preflight: verifying Qt Help outputs and format compatibility"
    MISSING=0
    INCOMPATIBLE=0
    
    # Function to validate help file format
    validate_help_file() {
        local help_file="$1"
        local component="$2"
        
        if [[ ! -f "$help_file" ]]; then
            print_error "Missing help artifact: $help_file"
            return 1
        fi
        
        # Basic help file validation using sqlite3 if available
        if command -v sqlite3 >/dev/null 2>&1; then
            print_status "Validating help file format: $help_file"
            
            # Check if it's a valid SQLite database
            if sqlite3 "$help_file" ".tables" >/dev/null 2>&1; then
                print_success "Help file $help_file is a valid SQLite database"
                
                # Check for required tables (basic compatibility check)
                local tables=$(sqlite3 "$help_file" ".tables" 2>/dev/null)
                if [[ "$tables" =~ ContentsTable|FileDataTable ]]; then
                    print_success "Help file $help_file has required tables - format appears compatible"
                else
                    print_error "Help file $help_file missing required tables - format incompatible"
                    die "Help file format validation failed. Cannot proceed with AppImage build."
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
    
    # Validate all help files
    for component in "mickey" "qt_gui"; do
        if [[ -f "src/$component/help.qhc" ]]; then
            validate_help_file "src/$component/help.qhc" "$component"
            if [[ $? -ne 0 ]]; then
                die "Help file validation failed for $component. Cannot proceed with AppImage build."
            fi
        else
            die "Missing help collection file: src/$component/help.qhc. Cannot proceed with AppImage build."
        fi
        
        if [[ -f "src/$component/help.qch" ]]; then
            validate_help_file "src/$component/help.qch" "$component"
            if [[ $? -ne 0 ]]; then
                die "Help file validation failed for $component. Cannot proceed with AppImage build."
            fi
        else
            die "Missing help database file: src/$component/help.qch. Cannot proceed with AppImage build."
        fi
    done
    
    # Report overall status
    if [[ $MISSING -ne 0 ]]; then
        die "Qt Help generation failed; install qhelpgenerator and ensure .qhp/.qhcp are valid"
    fi
    
    if [[ $INCOMPATIBLE -ne 0 ]]; then
        print_warning "Some help files may have compatibility issues - consider regenerating with compatible Qt version"
    fi
    
    print_success "All Qt Help files generated and validated"

    print_status "Installing to AppDir"
    make install DESTDIR="$APPDIR"
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


