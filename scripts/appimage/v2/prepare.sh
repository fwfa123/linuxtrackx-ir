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

    print_status "Regenerating build system"
    make distclean >/dev/null 2>&1 || true
    autoreconf -fiv

    print_status "Configuring"
    ./configure --prefix=/usr

    print_status "Building"
    make -j"$JOBS"

    print_status "Installing to AppDir"
    make install DESTDIR="$APPDIR"
popd >/dev/null

write_minimal_apprun
ensure_desktop_and_icons
copy_udev_rules_if_present

print_success "Prepare complete"


