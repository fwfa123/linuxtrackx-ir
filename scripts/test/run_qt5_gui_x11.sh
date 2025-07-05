#!/bin/bash

# LinuxTrack Qt5 GUI Launch Script (X11 Mode)
# This script automatically forces X11 mode for full blob tracking functionality
# Use this on Wayland systems to avoid blob tracking display issues

echo "🚀 LinuxTrack Qt5 GUI - Launching in X11 mode for full compatibility"
echo "💡 This ensures real-time blob tracking display works correctly"
echo ""

# Force X11 mode for full functionality
export XDG_SESSION_TYPE=x11
export QT_QPA_PLATFORM=xcb

# Launch the GUI with all arguments passed through
exec ./run_qt5_gui.sh "$@" 