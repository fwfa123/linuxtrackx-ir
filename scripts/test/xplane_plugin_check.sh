#!/usr/bin/env bash
# Read-only host checks for LinuxTrack X-Plane plugin debugging.
# Usage: scripts/test/xplane_plugin_check.sh [path/to/linuxtrackx-ir.xpl]
# Optional: XPLANE_ROOT=/path/to/X-Plane-12
# Example: XPLANE_ROOT="/media/Fast_Games/linux_games/X-Plane 12"

set -euo pipefail

: "${XPLANE_ROOT:=/media/Fast_Games/linux_games/X-Plane 12}"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() { echo -e "${BLUE}[INFO]${NC} $*"; }
print_ok() { echo -e "${GREEN}[OK]${NC} $*"; }
print_warn() { echo -e "${YELLOW}[WARN]${NC} $*"; }
print_err() { echo -e "${RED}[ERR]${NC} $*"; }

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

PLUGIN_ARG="${1:-}"
PLUGIN_PATH=""

find_plugin() {
    if [[ -n "$PLUGIN_ARG" && -f "$PLUGIN_ARG" ]]; then
        PLUGIN_PATH="$PLUGIN_ARG"
        return 0
    fi
    if [[ -n "${XPLANE_ROOT:-}" ]]; then
        local cand
        cand="$XPLANE_ROOT/Resources/plugins/linuxtrackx-ir/lin_x64/linuxtrackx-ir.xpl"
        if [[ -f "$cand" ]]; then
            PLUGIN_PATH="$cand"
            return 0
        fi
    fi
    local found
    found="$(find "${HOME}" -path '*/Resources/plugins/linuxtrackx-ir/lin_x64/linuxtrackx-ir.xpl' 2>/dev/null | head -1 || true)"
    if [[ -n "$found" ]]; then
        PLUGIN_PATH="$found"
        return 0
    fi
    return 1
}

echo "=== LinuxTrack X-Plane plugin check ==="
print_status "Project: $PROJECT_ROOT"
echo

# --- liblinuxtrack (runtime dependency of plugin) ---
LIB_PATHS=(
    /opt/lib/linuxtrack/liblinuxtrack.so.0
    /usr/local/lib/linuxtrack/liblinuxtrack.so.0
    /usr/lib/linuxtrack/liblinuxtrack.so.0
    /usr/lib/x86_64-linux-gnu/linuxtrack/liblinuxtrack.so.0
)
lib_found=false
for p in "${LIB_PATHS[@]}"; do
    if [[ -e "$p" ]]; then
        print_ok "liblinuxtrack: $p"
        ls -l "$p"
        lib_found=true
        break
    fi
done
if [[ "$lib_found" != true ]]; then
    print_err "liblinuxtrack.so.0 not found under /opt or /usr"
fi
echo

# --- Built plugin in install tree ---
for p in /opt/lib/linuxtrack/xlinuxtrack9.so \
         "$PROJECT_ROOT/build/src/xlinuxtrack9.so"; do
    if [[ -f "$p" ]]; then
        print_ok "xlinuxtrack9 build/install: $p"
        ls -l "$p"
    fi
done
echo

# --- X-Plane installed .xpl ---
if find_plugin; then
    print_ok "X-Plane plugin: $PLUGIN_PATH"
    if command -v ldd >/dev/null 2>&1; then
        print_status "ldd (plugin loads liblinuxtrack at runtime via dlopen):"
        ldd "$PLUGIN_PATH" 2>&1 | sed 's/^/  /'
    fi
else
    print_warn "linuxtrackx-ir.xpl not found (pass path or set XPLANE_ROOT)"
fi
echo

# --- IPC / processes ---
if [[ -S /tmp/ltr_m_sock ]]; then
    print_ok "Master socket: /tmp/ltr_m_sock"
    ls -l /tmp/ltr_m_sock
else
    print_warn "No /tmp/ltr_m_sock (ltr_gui master not running)"
fi
if pgrep -a ltr_server >/dev/null 2>&1; then
    print_status "ltr_server processes:"
    pgrep -a ltr_server | sed 's/^/  /'
else
    print_warn "No ltr_server1 processes"
fi
echo

# --- Logs ---
latest_log=""
for f in /tmp/linuxtrack{00..09}.log; do
    [[ -f "$f" ]] || continue
    if [[ -z "$latest_log" || "$f" -nt "$latest_log" ]]; then
        latest_log="$f"
    fi
done
if [[ -n "$latest_log" ]]; then
    print_ok "Latest log: $latest_log (last 50 lines)"
    tail -n 50 "$latest_log" | sed 's/^/  /'
else
    print_warn "No /tmp/linuxtrack*.log files"
fi
echo

# --- X-Plane Log.txt hint ---
if [[ -n "${XPLANE_ROOT:-}" && -f "$XPLANE_ROOT/Log.txt" ]]; then
    print_status "Recent linuxtrack lines in X-Plane Log.txt:"
    grep -i linuxtrack "$XPLANE_ROOT/Log.txt" 2>/dev/null | tail -20 | sed 's/^/  /' || true
elif find "${HOME}" -maxdepth 5 -name Log.txt 2>/dev/null | head -1 | read -r xp_log; then
    :
fi
echo

print_status "In X-Plane: bind keys to linuxtrack/ltr_start OR linuxtrack/ltr_run (not only ltr_recenter)"
print_status "Or use F8=toggle tracking, F9=pause (after rebuilding plugin)"
print_status "Start tracking in ltr_gui OR press F8/ltr_start in cockpit; GUI preview is separate from X-Plane head view"
print_status "Use 3D cockpit view (view_type 1026) for head tracking"
print_status "Debug plugin: LINUXTRACK_XPLANE_DEBUG=1  IPC: LINUXTRACK_DBG=sc"
print_status "Launch X-Plane from a terminal to see linuxtrack_init stderr (DEBUG lines)"
