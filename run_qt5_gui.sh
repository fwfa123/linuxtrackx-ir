#!/bin/bash
# LinuxTrack Qt5 GUI Launcher
# This script sets the correct library path and launches the modernized Qt5 GUI

echo "🚀 Starting LinuxTrack Qt5 GUI (Modernized from Qt4)"
echo "📍 Working directory: $(pwd)"

# Get absolute path to libraries
LIB_PATH="$(pwd)/src/.libs"
# FORCE our libraries first to override conflicting system installation
export LD_LIBRARY_PATH="$LIB_PATH:/usr/local/lib:$LD_LIBRARY_PATH"

echo "🔧 Library path: $LIB_PATH"
echo "📂 Available libraries:"
ls -la "$LIB_PATH"/libltr.so*

# Change to GUI directory and launch
cd src/qt_gui
echo "📂 GUI directory: $(pwd)"
echo "✨ Launching Qt5 GUI..."

LD_LIBRARY_PATH="$LIB_PATH:/usr/local/lib:$LD_LIBRARY_PATH" ./ltr_gui

echo "�� Qt5 GUI closed." 