#!/bin/bash

# LinuxTrack Qt5 Standalone GUI Test Script
# This script runs the modernized LinuxTrack GUI for testing

echo "🚀 Starting LinuxTrack Qt5 Standalone GUI Test..."
echo "📍 Location: $(pwd)"
echo ""

# Change to the GUI directory
cd "$(dirname "$0")/src/qt_gui" || {
    echo "❌ Error: Could not find GUI directory"
    exit 1
}

# Check if the binary exists
if [ ! -f "./ltr_gui_qt5_standalone_debug" ]; then
    echo "❌ Error: GUI binary not found. Please build first with: make"
    exit 1
fi

echo "✅ GUI binary found: $(ls -lh ./ltr_gui_qt5_standalone_debug)"
echo ""
echo "🔧 Setting up Qt5 environment..."
echo "   - Qt Plugin Path: /usr/lib64/qt5/plugins"
echo "   - Platform: xcb (X11)"
echo ""
echo "🎯 Launching GUI (press Ctrl+C to stop)..."
echo "   Expected behavior:"
echo "   - No 'GUI already running' dialog"
echo "   - No 'TrackIR permissions' dialog" 
echo "   - All device detection works safely"
echo "   - GUI loads with all tabs functional"
echo ""

# Run the GUI with proper Qt5 environment
export QT_PLUGIN_PATH=/usr/lib64/qt5/plugins
export QT_QPA_PLATFORM=xcb

echo "🟢 Starting GUI now..."
./ltr_gui_qt5_standalone_debug

echo ""
echo "📊 Test completed. GUI exit status: $?"
echo "✅ Qt4→Qt5 migration test finished." 