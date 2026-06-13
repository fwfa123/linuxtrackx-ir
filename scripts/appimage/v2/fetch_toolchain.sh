#!/usr/bin/env bash
# Download modern appimagetool + type2-runtime for local AppImage v2 builds.
# Docker builds vendor these under /opt/; local builds store them in scripts/appimage/.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=config.sh
source "$SCRIPT_DIR/config.sh"
# shellcheck source=common.sh
source "$SCRIPT_DIR/common.sh"

require_cmd wget

download_file() {
    local url="$1"
    local dest="$2"
    local executable="${3:-0}"

    if [[ -f "$dest" && "${FORCE_FETCH_TOOLCHAIN:-0}" != "1" ]]; then
        if [[ "$dest" == "$LOCAL_APPIMAGETOOL" ]]; then
            local ver
            ver=$(APPIMAGE_EXTRACT_AND_RUN=1 "$dest" --version 2>/dev/null | head -1 || true)
            if [[ "$ver" == *"5735cc5"* ]]; then
                print_warning "Legacy appimagetool detected; re-downloading"
            else
                print_status "Already present: $dest"
                return 0
            fi
        else
            print_status "Already present: $dest"
            return 0
        fi
    fi

    print_status "Downloading $url -> $dest"
    wget -q "$url" -O "${dest}.new" || die "wget failed for $url"
    mv "${dest}.new" "$dest"
    if [[ "$executable" == "1" ]]; then
        chmod +x "$dest"
    fi
}

ensure_dir "$APPSCRIPTS_DIR"

download_file "$APPIMAGETOOL_URL" "$LOCAL_APPIMAGETOOL" 1
download_file "$APPIMAGE_RUNTIME_URL" "$LOCAL_APPIMAGE_RUNTIME" 1

reject_legacy_appimagetool "$LOCAL_APPIMAGETOOL"

print_success "AppImage toolchain ready:"
print_status "  appimagetool: $LOCAL_APPIMAGETOOL"
print_status "  runtime:      $LOCAL_APPIMAGE_RUNTIME"
