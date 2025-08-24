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

    print_status "Generating Qt Help files"
    # Clean up any existing help files to ensure fresh generation
    rm -f src/mickey/help.qhc src/mickey/help.qch
    rm -f src/qt_gui/help.qhc src/qt_gui/help.qch
    
    # Generate help files for components that need them
    if [[ -f src/mickey/mickey.qhp && -f src/mickey/mickey.qhcp ]]; then
        print_status "Generating mickey help files"
        cd src/mickey
        qhelpgenerator mickey.qhcp -o help.qhc
        qhelpgenerator mickey.qhp -o help.qch
        cd ../..
        print_success "Mickey help files generated"
    else
        print_warning "Mickey help source files not found, skipping help generation"
    fi

    if [[ -f src/qt_gui/ltr_gui.qhp && -f src/qt_gui/ltr_gui.qhcp ]]; then
        print_status "Generating qt_gui help files"
        cd src/qt_gui
        qhelpgenerator ltr_gui.qhcp -o help.qhc
        qhelpgenerator ltr_gui.qhp -o help.qch
        cd ../..
        print_success "Qt GUI help files generated"
    else
        print_warning "Qt GUI help source files not found, skipping help generation"
    fi

    # Preflight: verify Qt Help outputs exist, or fail fast
    print_status "Preflight: verifying Qt Help outputs"
    MISSING=0
    for f in \
        "src/mickey/help.qhc" \
        "src/mickey/help.qch" \
        "src/qt_gui/help.qhc" \
        "src/qt_gui/help.qch"; do
        if [[ ! -f "$f" ]]; then
            print_error "Missing help artifact: $f"
            MISSING=1
        fi
    done
    if [[ $MISSING -ne 0 ]]; then
        die "Qt Help generation failed; install qhelpgenerator and ensure .qhp/.qhcp are valid"
    fi

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
        pushd "$TMP_BUILD32" >/dev/null
            export CFLAGS="-m32"
            export CXXFLAGS="-m32"
            export LDFLAGS="-m32"
            # Common Debian/Ubuntu multiarch pkg-config path
            export PKG_CONFIG_PATH="/usr/lib/i386-linux-gnu/pkgconfig"
            "$PROJECT_ROOT/configure" --prefix=/usr >/dev/null 2>&1 || true
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
                    print_warning "32-bit liblinuxtrack build succeeded but library not found"
                fi
            else
                print_warning "Failed to build 32-bit liblinuxtrack (toolchain or deps missing)"
            fi
        popd >/dev/null
        rm -rf "$TMP_BUILD32"
    else
        print_warning "gcc -m32 not available; skipping 32-bit runtime bundling"
    fi
else
    print_warning "gcc not found; skipping 32-bit runtime bundling"
fi

print_success "Prepare complete"


