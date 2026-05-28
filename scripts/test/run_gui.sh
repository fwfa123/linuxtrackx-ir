#!/bin/bash
# LinuxTrack GUI launcher (run from repository root).
# Main-branch builds use Qt6; sets library paths and runs src/qt_gui/ltr_gui.

echo "Starting LinuxTrack GUI (Qt6 build)"
echo "Working directory: $(pwd)"

# Detect display server and handle Wayland
DISPLAY_SERVER=""
if [ "$XDG_SESSION_TYPE" = "wayland" ] || [ -n "$WAYLAND_DISPLAY" ]; then
    DISPLAY_SERVER="Wayland"
    echo "Detected: $DISPLAY_SERVER session"
    echo "GUI may work better with X11. Use --force-x11 to run with XWayland"
    
    # Check for force X11 flag
    if [ "$1" = "--force-x11" ]; then
        echo "Forcing X11 mode via XWayland..."
        export QT_QPA_PLATFORM=xcb
        export GDK_BACKEND=x11
        export CLUTTER_BACKEND=x11
    else
        echo "To force X11 mode: ./run_gui.sh --force-x11"
    fi
elif [ -n "$DISPLAY" ]; then
    DISPLAY_SERVER="X11"
    echo "Detected: $DISPLAY_SERVER session"
else
    echo "No display server detected. Attempting to launch anyway..."
fi

# Get absolute path to libraries
LIB_PATH="$(pwd)/src/.libs"
# FORCE our libraries first to override conflicting system installation
export LD_LIBRARY_PATH="$LIB_PATH:/usr/local/lib:$LD_LIBRARY_PATH"

echo "Library path: $LIB_PATH"
echo "Available libraries:"
ls -la "$LIB_PATH"/libltr.so*

# Change to GUI directory and launch
cd src/qt_gui
echo "GUI directory: $(pwd)"
echo "Launching ltr_gui..."

LD_LIBRARY_PATH="$LIB_PATH:/usr/local/lib:$LD_LIBRARY_PATH" ./ltr_gui

echo "ltr_gui closed."
