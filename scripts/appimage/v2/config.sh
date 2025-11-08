#!/usr/bin/env bash
set -euo pipefail

# Central configuration for AppImage v2 pipeline

# Paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
APPSCRIPTS_DIR="$(dirname "$SCRIPT_DIR")"              # scripts/appimage
PROJECT_ROOT="$(dirname "$(dirname "$APPSCRIPTS_DIR")")"  # repo root

# Ensure PROJECT_ROOT points to the correct linuxtrackx-ir directory
if [[ "$(basename "$PROJECT_ROOT")" != "linuxtrackx-ir" ]]; then
    PROJECT_ROOT="$PROJECT_ROOT/linuxtrackx-ir"
fi

# AppDir (v2)
APPDIR="$PROJECT_ROOT/AppDir_v2"

# App metadata
APP_NAME="LinuxTrack-X-IR"
APP_DISPLAY_NAME="LinuxTrack X-IR"
APP_ID="com.linuxtrack.linuxtrackx-ir"

# Auto-extract version from CMakeLists.txt
extract_version() {
    local cmake_lists="$PROJECT_ROOT/CMakeLists.txt"
    if [[ -f "$cmake_lists" ]]; then
        VERSION=$(grep 'project.*VERSION' "$cmake_lists" | sed 's/.*VERSION[[:space:]]*\([0-9.]*\).*/\1/')
        if [[ -z "$VERSION" ]]; then
            # Fallback to configure.ac if CMakeLists.txt doesn't have version
            local configure_ac="$PROJECT_ROOT/configure.ac"
            if [[ -f "$configure_ac" ]]; then
                VERSION=$(grep 'AC_INIT' "$configure_ac" | sed 's/.*\[[^]]*\],\[\([^]]*\)\],\[.*/\1/')
            fi
        fi
        if [[ -z "$VERSION" ]]; then
            echo "Error: Could not extract version from CMakeLists.txt or configure.ac" >&2
            exit 1
        fi
        echo "Extracted version: $VERSION"
    else
        echo "Error: CMakeLists.txt not found at $cmake_lists" >&2
        exit 1
    fi
}

# Extract version
extract_version

# Tools (expected under scripts/appimage)
APPIMAGETOOL="$APPSCRIPTS_DIR/appimagetool-x86_64.AppImage"
LINUXDEPLOY="$APPSCRIPTS_DIR/linuxdeploy-x86_64.AppImage"
LINUXDEPLOY_QT="$APPSCRIPTS_DIR/linuxdeploy-plugin-qt-x86_64.AppImage"

# Feature toggles
FORCE_XCB="1"              # default to xcb; can be overridden
WITH_WINE_BRIDGE="1"
WITH_HELP="1"
WITH_ZSYNC="0"

# Build toggles
CLEAN="0"
JOBS="${JOBS:-$(nproc)}"

# Environment harmonization
# Commented out to avoid conflict with appimagetool timestamp handling
# export SOURCE_DATE_EPOCH="${SOURCE_DATE_EPOCH:-$(git -C "$PROJECT_ROOT" log -1 --format=%ct 2>/dev/null || date +%s)}"


