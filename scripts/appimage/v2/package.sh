#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Package: creating AppImage"

[[ -x "$APPIMAGETOOL" ]] || die "appimagetool not executable at $APPIMAGETOOL"
[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR"

pushd "$PROJECT_ROOT" >/dev/null
    OUT="${APP_NAME}-${VERSION}-x86_64.AppImage"
    print_status "Building AppImage -> $OUT"
    ARCH=x86_64 "$APPIMAGETOOL" "$APPDIR" "$OUT"

    if [[ "${WITH_ZSYNC}" = "1" ]]; then
        print_status "Generating zsync"
        "$APPIMAGETOOL" --comp zsync "$OUT" || print_warning "zsync generation failed"
    fi

    print_success "AppImage created: $PROJECT_ROOT/$OUT"
popd >/dev/null


