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
VERSION="0.99.24"

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


