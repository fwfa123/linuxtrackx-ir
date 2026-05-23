#!/usr/bin/env bash
# Install Linuxtrack Wine bridge PE files into a Wine/Proton prefix (copy + wine reg + firmware links).
# Same layout as ltr_gui Lutris/Steam/custom-prefix native install.
set -euo pipefail

PREFIX="${1:-}"
WINE="${2:-}"

if [[ -z "$PREFIX" || -z "$WINE" ]]; then
  echo "Usage: $0 <WINEPREFIX> <wine-binary>" >&2
  echo "Example: $0 \$HOME/Games/Lutris/my-game \\" >&2
  echo "  \$HOME/.local/share/Steam/compatibilitytools.d/GE-Proton*/files/bin/wine" >&2
  exit 1
fi

PAYLOAD=""
for d in /opt/lib/linuxtrack/wine_bridge /usr/local/lib/linuxtrack/wine_bridge /usr/lib/linuxtrack/wine_bridge; do
  if [[ -f "$d/NPClient.dll" ]]; then
    PAYLOAD="$d"
    break
  fi
done

if [[ -z "$PAYLOAD" ]]; then
  echo "NPClient.dll payload not found (expected under /opt/lib/linuxtrack/wine_bridge)" >&2
  exit 1
fi

if [[ ! -x "$WINE" ]]; then
  echo "Wine binary not found or not executable: $WINE" >&2
  exit 1
fi

WIN64=0
if grep -q '#arch=win64' "$PREFIX/user.reg" 2>/dev/null; then
  WIN64=1
  INSTALL_SUBDIR="Program Files (x86)/Linuxtrack"
  REG_PATH='C:\Program Files (x86)\Linuxtrack\'
else
  INSTALL_SUBDIR="Program Files/Linuxtrack"
  REG_PATH='C:\Program Files\Linuxtrack\'
fi

INSTALL_DIR="$PREFIX/drive_c/$INSTALL_SUBDIR"
mkdir -p "$INSTALL_DIR"

copy_required() {
  local src="$1" dst="$2"
  if [[ ! -f "$PAYLOAD/$src" ]]; then
    echo "Required payload file missing: $PAYLOAD/$src" >&2
    exit 1
  fi
  cp -f "$PAYLOAD/$src" "$INSTALL_DIR/$dst"
  echo "copied $dst"
}

copy_optional() {
  local src="$1" dst="$2"
  if [[ -f "$PAYLOAD/$src" ]]; then
    cp -f "$PAYLOAD/$src" "$INSTALL_DIR/$dst"
    echo "copied $dst"
  fi
}

copy_required NPClient.dll NPClient.dll
if [[ "$WIN64" -eq 1 ]]; then
  copy_required NPClient64.dll NPClient64.dll
else
  copy_optional NPClient64.dll NPClient64.dll
fi
copy_optional check_data.exe check_data.exe
copy_optional Controller.exe Controller.exe
copy_optional Tester.exe Tester.exe
copy_optional Tester64.exe Tester64.exe
copy_optional TrackIR.exe TrackIR.exe
copy_optional FreeTrackClient.dll FreeTrackClient.dll
copy_optional ftc.exe FreeTrackTester.exe

if [[ ! -f "$INSTALL_DIR/NPClient.dll" ]]; then
  echo "NPClient.dll was not installed into the prefix." >&2
  exit 1
fi
if [[ "$WIN64" -eq 1 && ! -f "$INSTALL_DIR/NPClient64.dll" ]]; then
  echo "NPClient64.dll was not installed (required for WOW64 prefixes)." >&2
  exit 1
fi

export WINEPREFIX="$PREFIX"
INSTALL_DIR_WIN="${REG_PATH%\\}"

wine_reg() {
  "$WINE" reg add "$1" /v "$2" /t REG_SZ /d "$3" /f
}

wine_reg 'HKLM\SOFTWARE\Linuxtrack' Install_dir "$INSTALL_DIR_WIN"
wine_reg 'HKCU\Software\NaturalPoint\NATURALPOINT\NPClient Location' Path "$REG_PATH"
wine_reg 'HKCU\Software\Freetrack\FreetrackClient' Path "$REG_PATH" || true

FIRMWARE="$HOME/.config/linuxtrack/tir_firmware"
for name in TIRViews.dll mfc42u.dll mfc42.dll; do
  if [[ -f "$FIRMWARE/$name" ]]; then
    ln -sf "$FIRMWARE/$name" "$INSTALL_DIR/$name"
    echo "linked $name"
  fi
done

if [[ -f "$INSTALL_DIR/check_data.exe" ]]; then
  set +e
  "$WINE" "$INSTALL_DIR/check_data.exe"
  set -e
fi

echo "Done. Install dir: $INSTALL_DIR"
echo "Registry Path: $REG_PATH"
