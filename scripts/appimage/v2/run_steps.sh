#!/usr/bin/env bash
set -euo pipefail

# Simple orchestrator to run AppImage v2 steps with per-step logs

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Source the shared config to get PROJECT_ROOT, APPDIR, tool paths, etc.
source "$SCRIPT_DIR/config.sh"

# Ensure we run from project root for consistent relative paths
cd "$PROJECT_ROOT"

TS="$(date +%Y%m%d_%H%M%S)"
LOGDIR="$PROJECT_ROOT/scripts/appimage/v2/logs/$TS"
mkdir -p "$LOGDIR"

echo "[INFO] Logs: $LOGDIR"

echo "[INFO] Preflight checks" | tee "$LOGDIR/preflight.txt"
for cmd in autoreconf make qhelpgenerator qmake patchelf ldd; do
  {
    echo "- $cmd:"
    if command -v "$cmd" >/dev/null 2>&1; then
      ("$cmd" --version 2>/dev/null | head -1 || echo present) | sed 's/^/OK /'
    else
      echo "MISSING"
    fi
  } | tee -a "$LOGDIR/preflight.txt"
done

run_step() {
  local name="$1"
  local script_path="$SCRIPT_DIR/$name.sh"
  local log_file="$LOGDIR/$name.log"
  local status_file="$LOGDIR/$name.status"

  echo "[STEP] $name" | tee "$LOGDIR/$name.start"
  if [[ ! -x "$script_path" ]]; then
    if [[ -f "$script_path" ]]; then
      echo "[INFO] Making $script_path executable"
      chmod +x "$script_path" || true
    else
      echo "[ERROR] Missing script: $script_path" | tee -a "$log_file"
      echo 127 > "$status_file"
      return 127
    fi
  fi

  set +e
  bash -x "$script_path" |& tee "$log_file"
  local rc=${PIPESTATUS[0]}
  set -e
  echo "$rc" > "$status_file"
  echo "[STEP STATUS] $name=$rc"
  if [[ "$rc" -ne 0 ]]; then
    echo "[FAIL] $name failed. Last 100 lines:"; tail -n 100 "$log_file" || true
  fi
  return "$rc"
}

# CLEAN=1 to ensure fresh AppDir each run
export CLEAN=${CLEAN:-1}
export JOBS=${JOBS:-$(nproc)}

run_step prepare || exit $?
run_step bundle  || exit $?

# Optional wine bridge step
if [[ -x "$SCRIPT_DIR/wine_bridge.sh" ]] && [[ ${WITH_WINE_BRIDGE:-1} == 1 ]]; then
  run_step wine_bridge || exit $?
fi

# optimize is best-effort
run_step optimize || true

run_step validate || exit $?
run_step package  || exit $?

echo "[SUCCESS] All steps completed. Logs at $LOGDIR"


