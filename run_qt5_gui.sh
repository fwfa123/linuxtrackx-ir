#!/bin/bash
# LinuxTrack Qt5 GUI Launcher
# This script sets the correct library path and launches the modernized Qt5 GUI

echo "🚀 Starting LinuxTrack Qt5 GUI (Modernized from Qt4)"
echo "📍 Working directory: $(pwd)"

# Get absolute path to libraries
LIB_PATH="$(pwd)/src/.libs"
export LD_LIBRARY_PATH="$LIB_PATH:$LD_LIBRARY_PATH"

echo "🔧 Library path: $LIB_PATH"
echo "📂 Available libraries:"
ls -la "$LIB_PATH"/libltr.so*

# Change to GUI directory and launch
cd src/qt_gui
echo "📂 GUI directory: $(pwd)"
echo "✨ Launching Qt5 GUI..."

./ltr_gui_qt5_debug

echo "�� Qt5 GUI closed." 