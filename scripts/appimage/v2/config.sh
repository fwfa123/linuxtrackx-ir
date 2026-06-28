#!/usr/bin/env bash
set -euo pipefail

# Central configuration for AppImage v2 pipeline

# Paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
APPSCRIPTS_DIR="$(dirname "$SCRIPT_DIR")"              # scripts/appimage
PROJECT_ROOT="$(dirname "$(dirname "$APPSCRIPTS_DIR")")"  # repo root

# Validate PROJECT_ROOT — it must contain CMakeLists.txt
if [[ ! -f "$PROJECT_ROOT/CMakeLists.txt" ]]; then
    if [[ -f "$PROJECT_ROOT/linuxtrackx-ir/CMakeLists.txt" ]]; then
        PROJECT_ROOT="$PROJECT_ROOT/linuxtrackx-ir"
    else
        echo "Error: Cannot locate CMakeLists.txt from derived PROJECT_ROOT=$PROJECT_ROOT" >&2
        exit 1
    fi
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

# AppImage packaging toolchain (modern type2-runtime; not legacy AppImageKit 5735cc5)
APPIMAGETOOL_URL="${APPIMAGETOOL_URL:-https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage}"
APPIMAGE_RUNTIME_URL="${APPIMAGE_RUNTIME_URL:-https://github.com/AppImage/type2-runtime/releases/download/continuous/runtime-x86_64}"
# continuous tags move; pin commits in Dockerfile comments when auditing reproducibility

LOCAL_APPIMAGETOOL="$APPSCRIPTS_DIR/appimagetool-x86_64.AppImage"
LOCAL_APPIMAGE_RUNTIME="$APPSCRIPTS_DIR/runtime-x86_64"

# Tools — prefer PATH (Docker container), fall back to fetched local copies
APPIMAGETOOL="${APPIMAGETOOL:-$(command -v appimagetool 2>/dev/null || echo "$LOCAL_APPIMAGETOOL")}"
APPIMAGE_RUNTIME="${APPIMAGE_RUNTIME:-$LOCAL_APPIMAGE_RUNTIME}"

# Feature toggles — full README Level 7 AppImage is the default.
# Opt out per feature: export ENABLE_OSC=0, EXPECT_OSC=0, etc. before ci_build.sh.
FORCE_XCB="${FORCE_XCB:-1}"
WITH_WINE_BRIDGE="${WITH_WINE_BRIDGE:-1}"
WITH_HELP="${WITH_HELP:-1}"
WITH_ZSYNC="${WITH_ZSYNC:-0}"

# CMake build features (prepare.sh)
ENABLE_WEBCAM="${ENABLE_WEBCAM:-1}"
ENABLE_OSC="${ENABLE_OSC:-1}"
ENABLE_FACE_TRACKER="${ENABLE_FACE_TRACKER:-1}"
ENABLE_XPLANE="${ENABLE_XPLANE:-1}"
ENABLE_WIIMOTE="${ENABLE_WIIMOTE:-1}"
ENABLE_LTR_32LIB_ON_X64="${ENABLE_LTR_32LIB_ON_X64:-1}"

# X-Plane SDK (required when ENABLE_XPLANE=1 unless REQUIRE_XPLANE_SDK=0)
XPLANE_SDK_PATH="${XPLANE_SDK_PATH:-/opt/xplane-sdk/CHeaders}"
REQUIRE_XPLANE_SDK="${REQUIRE_XPLANE_SDK:-1}"

# Post-build validation (validate.sh) — defaults match build features above
EXPECT_LEVEL7="${EXPECT_LEVEL7:-1}"
EXPECT_FACE_TRACKER="${EXPECT_FACE_TRACKER:-1}"
EXPECT_OSC="${EXPECT_OSC:-1}"
EXPECT_XPLANE_PLUGIN="${EXPECT_XPLANE_PLUGIN:-1}"

# Build toggles
CLEAN="0"
JOBS="${JOBS:-$(nproc)}"

# Environment harmonization
# Commented out to avoid conflict with appimagetool timestamp handling
# export SOURCE_DATE_EPOCH="${SOURCE_DATE_EPOCH:-$(git -C "$PROJECT_ROOT" log -1 --format=%ct 2>/dev/null || date +%s)}"


