#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Load config to get APP_NAME, VERSION, APPDIR, PROJECT_ROOT
source "$SCRIPT_DIR/config.sh"

export CLEAN=1
export STRICT_BUNDLE=${STRICT_BUNDLE:-1}

echo "[INFO] CI build starting for $APP_NAME $VERSION"

if [[ "${CLEAN:-0}" == "1" ]]; then
    echo "[INFO] CLEAN=1 -> removing previous outputs and AppDir"
    rm -f "$PROJECT_ROOT/${APP_NAME}-${VERSION}-x86_64.AppImage" || true
    rm -rf "$APPDIR" || true
fi

echo "[STEP] prepare"
"$SCRIPT_DIR/prepare.sh"
echo "[STEP DONE] prepare"

echo "[STEP] bundle"
"$SCRIPT_DIR/bundle.sh"
echo "[STEP DONE] bundle"

echo "[STEP] optimize"
"$SCRIPT_DIR/optimize.sh" || true
echo "[STEP DONE] optimize"

echo "[STEP] validate"
"$SCRIPT_DIR/validate.sh"
echo "[STEP DONE] validate"

echo "[STEP] package"
"$SCRIPT_DIR/package.sh"
echo "[STEP DONE] package"

OUT_FILE="$PROJECT_ROOT/${APP_NAME}-${VERSION}-x86_64.AppImage"
if [[ -f "$OUT_FILE" ]]; then
    echo "[SUCCESS] AppImage ready: $OUT_FILE"
    ls -lh "$OUT_FILE"
    if [[ "${SMOKE_APPIMAGE:-0}" == "1" && -x "$SCRIPT_DIR/smoke_appimage.sh" ]]; then
        echo "[STEP] smoke_appimage"
        "$SCRIPT_DIR/smoke_appimage.sh" "$OUT_FILE"
        echo "[STEP DONE] smoke_appimage"
    fi
else
    echo "[ERROR] Expected AppImage not found: $OUT_FILE" >&2
    ls -la "$PROJECT_ROOT"/*.AppImage 2>/dev/null || echo "[INFO] No *.AppImage found in $PROJECT_ROOT" >&2
    exit 1
fi

echo "[SUCCESS] CI build pipeline completed"
