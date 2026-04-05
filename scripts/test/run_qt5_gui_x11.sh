#!/bin/bash

# LinuxTrack GUI launch (X11 mode). Legacy filename; Qt6 build.
# Forces X11 for compatibility on Wayland sessions.

echo "🚀 LinuxTrack GUI - X11 mode (Qt6 build; legacy script names)"
echo "💡 This ensures real-time blob tracking display works correctly"
echo ""

# Force X11 mode for full functionality
export XDG_SESSION_TYPE=x11
export QT_QPA_PLATFORM=xcb

# Launch the GUI with all arguments passed through
exec ./run_qt5_gui.sh "$@" 