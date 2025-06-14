#!/bin/bash

# LinuxTrack Qt5 Standalone GUI Test Script
# This script launches the standalone Qt5 GUI for testing

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

echo "=== LinuxTrack Qt5 Standalone GUI Test ==="
echo "Project root: $PROJECT_ROOT"

# Check if GUI binary exists
GUI_BINARY="src/qt_gui/ltr_gui_qt5_standalone_debug"
if [ ! -f "$GUI_BINARY" ]; then
    echo "❌ Standalone GUI binary not found: $GUI_BINARY"
    echo "Please run: ./dev-scripts/build_qt5_standalone.sh"
    exit 1
fi

echo "✅ Standalone GUI binary found: $GUI_BINARY"

# Check Qt5 runtime dependencies
echo ""
echo "=== Checking Qt5 Runtime Dependencies ==="
ldd "$GUI_BINARY" | grep -E "(Qt5|libQt)" | head -10

# Check for missing libraries
MISSING_LIBS=$(ldd "$GUI_BINARY" 2>&1 | grep "not found" || true)
if [ -n "$MISSING_LIBS" ]; then
    echo "❌ Missing libraries detected:"
    echo "$MISSING_LIBS"
    echo ""
    echo "Installing missing Qt5 runtime packages..."
    
    # Install Qt5 runtime packages
    if command -v dnf &> /dev/null; then
        sudo dnf install -y qt5-qtbase qt5-qttools qt5-qtwidgets qt5-qtopengl qt5-qthelp qt5-qtnetwork qt5-qtsql
    elif command -v apt &> /dev/null; then
        sudo apt update && sudo apt install -y qtbase5-dev qttools5-dev libqt5widgets5 libqt5opengl5 libqt5help5 libqt5network5 libqt5sql5
    fi
else
    echo "✅ All Qt5 libraries found"
fi

# Create test config directory
TEST_CONFIG_DIR="$HOME/.linuxtrack_standalone_test"
mkdir -p "$TEST_CONFIG_DIR"

# Set environment variables for testing
export LINUXTRACK_CONFIG_DIR="$TEST_CONFIG_DIR"
export QT_QPA_PLATFORM_PLUGIN_PATH="/usr/lib64/qt5/plugins/platforms"

echo ""
echo "=== Launching Qt5 Standalone GUI ==="
echo "Config directory: $TEST_CONFIG_DIR"
echo "Command: $GUI_BINARY"
echo ""
echo "🔧 This is a STANDALONE version for GUI testing only"
echo "📝 No actual tracking functionality - all LinuxTrack functions are stubbed"
echo "🎯 Use this to test the Qt5 GUI interface and verify the migration worked"
echo ""

# Launch with debug output
cd src/qt_gui
echo "Starting GUI in 3 seconds..."
sleep 3

# Run with error handling
if ./ltr_gui_qt5_standalone_debug; then
    echo "✅ GUI launched and closed successfully!"
else
    EXIT_CODE=$?
    echo "❌ GUI exited with code: $EXIT_CODE"
    
    # Provide troubleshooting info
    echo ""
    echo "=== Troubleshooting Information ==="
    echo "Qt version: $(qmake-qt5 -v 2>/dev/null || qmake -v)"
    echo "Display: $DISPLAY"
    echo "XDG_SESSION_TYPE: $XDG_SESSION_TYPE"
    
    # Check for common issues
    if [ -z "$DISPLAY" ]; then
        echo "⚠️  DISPLAY variable not set - GUI needs X11/Wayland"
    fi
    
    exit $EXIT_CODE
fi 