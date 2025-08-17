#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "Wine bridge staging"

[[ "$WITH_WINE_BRIDGE" = "1" ]] || { print_status "WITH_WINE_BRIDGE=0; skipping"; exit 0; }
[[ -d "$APPDIR" ]] || die "AppDir not found: $APPDIR. Run prepare.sh first."

ensure_dir "$APPDIR/wine_bridge"
ensure_dir "$APPDIR/wine_bridge/scripts"

# Prefer artifacts installed by the project during make install
WINE_SRC_DIR1="$APPDIR/usr/share/linuxtrack/wine"
WINE_SRC_DIR2="$APPDIR/usr/share/linuxtrack"
if [[ -f "$WINE_SRC_DIR1/linuxtrack-wine.exe" ]]; then
    cp "$WINE_SRC_DIR1/linuxtrack-wine.exe" "$APPDIR/wine_bridge/"
    print_success "Staged linuxtrack-wine.exe"
elif [[ -f "$WINE_SRC_DIR2/linuxtrack-wine.exe" ]]; then
    cp "$WINE_SRC_DIR2/linuxtrack-wine.exe" "$APPDIR/wine_bridge/"
    print_success "Staged linuxtrack-wine.exe"
else
    print_warning "linuxtrack-wine.exe not found under $WINE_SRC_DIR1 or $WINE_SRC_DIR2; Wine bridge installer unavailable"
fi

# Installer wrapper script (inside AppImage)
cat > "$APPDIR/wine_bridge/scripts/install_wine_bridge.sh" << 'EOF'
#!/usr/bin/env bash
set -e

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'; BLUE='\033[0;34m'; NC='\033[0m'
info() { echo -e "${BLUE}[INFO]${NC} $*"; }
warn() { echo -e "${YELLOW}[WARNING]${NC} $*"; }
err() { echo -e "${RED}[ERROR]${NC} $*"; }

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
APPDIR="$(dirname "$(dirname "$SCRIPT_DIR")")"

if ! command -v wine >/dev/null 2>&1; then
    err "Wine is not installed. Please install Wine first."
    info "Ubuntu/Debian: sudo apt install wine wine32 wine64"
    info "Fedora: sudo dnf install wine"
    info "Arch: sudo pacman -S wine wine-mono wine-gecko"
    exit 1
fi

INSTALLER="$APPDIR/wine_bridge/linuxtrack-wine.exe"
if [[ ! -f "$INSTALLER" ]]; then
    err "Wine bridge installer not found"
    exit 1
fi

TARGET_PREFIX="${WINEPREFIX:-$HOME/.wine}"
info "Installing Wine bridge into: $TARGET_PREFIX"
export WINEPREFIX="$TARGET_PREFIX"

set +e
wine "$INSTALLER" /S 2>/dev/null
rc=$?
set -e
if [[ $rc -ne 0 ]]; then
    warn "Silent install failed (rc=$rc); attempting interactive"
    wine "$INSTALLER" 2>/dev/null || true
fi

info "Wine bridge installation complete"
EOF
chmod +x "$APPDIR/wine_bridge/scripts/install_wine_bridge.sh"

print_success "Wine bridge staging complete"


