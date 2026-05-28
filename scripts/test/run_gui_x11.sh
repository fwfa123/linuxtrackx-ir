#!/bin/bash
# LinuxTrack GUI launch (X11 mode). Qt6 build.
# Forces X11 for compatibility on Wayland sessions.

echo "LinuxTrack GUI - X11 mode (Qt6 build)"
echo "This ensures real-time blob tracking display works correctly"
echo ""

# Force X11 mode for full functionality
export XDG_SESSION_TYPE=x11
export QT_QPA_PLATFORM=xcb

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
exec "${SCRIPT_DIR}/run_gui.sh" "$@"
