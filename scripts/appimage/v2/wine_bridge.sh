#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Wine bridge staging"

[[ "$WITH_WINE_BRIDGE" = "1" ]] || { print_status "WITH_WINE_BRIDGE=0; skipping"; exit 0; }
[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR. Run prepare.sh first."

ensure_dir "$APPDIR/wine_bridge"
ensure_dir "$APPDIR/wine_bridge/payload"
ensure_dir "$APPDIR/wine_bridge/scripts"

PAYLOAD_SRC=""
for d in \
  "$APPDIR/usr/lib/linuxtrack/wine_bridge" \
  "$APPDIR/opt/lib/linuxtrack/wine_bridge" \
  "/opt/lib/linuxtrack/wine_bridge"; do
  if [[ -f "$d/NPClient.dll" ]]; then
    PAYLOAD_SRC="$d"
    break
  fi
done

if [[ -n "$PAYLOAD_SRC" ]]; then
  cp -a "$PAYLOAD_SRC/." "$APPDIR/wine_bridge/payload/"
  print_success "Staged wine_bridge payload from $PAYLOAD_SRC"
else
  print_warning "NPClient.dll payload not found; Wine bridge install will fail in AppImage"
fi

# Installer wrapper (native copy + wine reg; no NSIS)
cat > "$APPDIR/wine_bridge/scripts/install_wine_bridge.sh" << 'EOF'
#!/usr/bin/env bash
set -euo pipefail

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'; BLUE='\033[0;34m'; NC='\033[0m'
info() { echo -e "${BLUE}[INFO]${NC} $*"; }
warn() { echo -e "${YELLOW}[WARNING]${NC} $*"; }
err() { echo -e "${RED}[ERROR]${NC} $*"; }

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
APPDIR="$(dirname "$(dirname "$SCRIPT_DIR")")"
PAYLOAD="$APPDIR/wine_bridge/payload"

if ! command -v wine >/dev/null 2>&1; then
    err "Wine is not installed. Please install Wine first."
    exit 1
fi

if [[ ! -f "$PAYLOAD/NPClient.dll" ]]; then
    err "Wine bridge payload not found in AppImage"
    exit 1
fi

TARGET_PREFIX="${WINEPREFIX:-$HOME/.wine}"
info "Installing Wine bridge into: $TARGET_PREFIX"
export WINEPREFIX="$TARGET_PREFIX"

if grep -q '#arch=win64' "$TARGET_PREFIX/user.reg" 2>/dev/null; then
  INSTALL_SUBDIR="Program Files (x86)/Linuxtrack"
  REG_PATH='C:\Program Files (x86)\Linuxtrack\'
else
  INSTALL_SUBDIR="Program Files/Linuxtrack"
  REG_PATH='C:\Program Files\Linuxtrack\'
fi

INSTALL_DIR="$TARGET_PREFIX/drive_c/$INSTALL_SUBDIR"
mkdir -p "$INSTALL_DIR"

WIN64=0
grep -q '#arch=win64' "$TARGET_PREFIX/user.reg" 2>/dev/null && WIN64=1

copy_required() {
  local src="$1" dst="$2"
  if [[ ! -f "$PAYLOAD/$src" ]]; then
    err "Required payload file missing: $PAYLOAD/$src"
    exit 1
  fi
  cp -f "$PAYLOAD/$src" "$INSTALL_DIR/$dst"
}

copy_optional() {
  local src="$1" dst="$2"
  [[ -f "$PAYLOAD/$src" ]] && cp -f "$PAYLOAD/$src" "$INSTALL_DIR/$dst"
}

copy_required NPClient.dll NPClient.dll
if [[ "$WIN64" -eq 1 ]]; then
  copy_required NPClient64.dll NPClient64.dll
else
  copy_optional NPClient64.dll NPClient64.dll
fi
for pair in \
  "check_data.exe:check_data.exe" \
  "Controller.exe:Controller.exe" \
  "Tester.exe:Tester.exe" \
  "Tester64.exe:Tester64.exe" \
  "TrackIR.exe:TrackIR.exe" \
  "FreeTrackClient.dll:FreeTrackClient.dll" \
  "ftc.exe:FreeTrackTester.exe"; do
  copy_optional "${pair%%:*}" "${pair##*:}"
done

if [[ ! -f "$INSTALL_DIR/NPClient.dll" ]]; then
  err "NPClient.dll was not installed into the prefix."
  exit 1
fi
if [[ "$WIN64" -eq 1 && ! -f "$INSTALL_DIR/NPClient64.dll" ]]; then
  err "NPClient64.dll was not installed (required for WOW64 prefixes)."
  exit 1
fi

INSTALL_DIR_WIN="${REG_PATH%\\}"
wine reg add 'HKLM\SOFTWARE\Linuxtrack' /v Install_dir /t REG_SZ /d "$INSTALL_DIR_WIN" /f
wine reg add 'HKCU\Software\NaturalPoint\NATURALPOINT\NPClient Location' /v Path /t REG_SZ /d "$REG_PATH" /f
wine reg add 'HKCU\Software\Freetrack\FreetrackClient' /v Path /t REG_SZ /d "$REG_PATH" /f || true

FIRMWARE="$HOME/.config/linuxtrack/tir_firmware"
for name in TIRViews.dll mfc42u.dll mfc42.dll; do
  [[ -f "$FIRMWARE/$name" ]] && ln -sf "$FIRMWARE/$name" "$INSTALL_DIR/$name"
done

[[ -f "$INSTALL_DIR/check_data.exe" ]] && wine "$INSTALL_DIR/check_data.exe" || true
info "Wine bridge installation complete: $INSTALL_DIR"
EOF
chmod +x "$APPDIR/wine_bridge/scripts/install_wine_bridge.sh"

print_success "Wine bridge staging complete"

# Helper: run Wine with bundled 32-bit liblinuxtrack when present
cat > "$APPDIR/wine_bridge/scripts/run_with_ltr32.sh" << 'EOF'
#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
APPDIR_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"

LTR32="$APPDIR_ROOT/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0"
if [[ ! -f "$LTR32" ]]; then
  echo "[ERROR] Bundled 32-bit linuxtrack library not found in AppImage." >&2
  exit 1
fi

export LINUXTRACK_LIBS="$LTR32"
exec wine "$@"
EOF
chmod +x "$APPDIR/wine_bridge/scripts/run_with_ltr32.sh"
