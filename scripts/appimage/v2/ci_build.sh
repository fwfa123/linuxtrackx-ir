#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

"$SCRIPT_DIR/prepare.sh"
"$SCRIPT_DIR/bundle.sh"
"$SCRIPT_DIR/wine_bridge.sh"
"$SCRIPT_DIR/optimize.sh"
"$SCRIPT_DIR/validate.sh"
"$SCRIPT_DIR/package.sh"

echo "[SUCCESS] CI build pipeline completed"


