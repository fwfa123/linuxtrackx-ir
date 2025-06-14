#!/bin/bash

# LinuxTrack Qt5 Standalone GUI Build Script
# This script builds a standalone version of the GUI for testing

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

echo "Building LinuxTrack Standalone GUI with Qt5..."
echo "Project root: $PROJECT_ROOT"

# Check if we have Qt5
QMAKE_CMD="qmake-qt5"
if ! command -v "$QMAKE_CMD" &> /dev/null; then
    QMAKE_CMD="qmake"
    if ! command -v "$QMAKE_CMD" &> /dev/null; then
        echo "Error: qmake not found. Please install Qt5 development packages."
        echo "Run: ./dev-scripts/setup_deps.sh"
        exit 1
    fi
fi

# Show Qt version
echo "Qt version: $($QMAKE_CMD -v)"

# Create build directory
BUILD_DIR="$PROJECT_ROOT/build_qt5_standalone"
mkdir -p "$BUILD_DIR"

# Change to qt_gui directory
cd src/qt_gui

echo "Cleaning previous build..."
rm -f ltr_gui_qt5_standalone* Makefile* *.o moc_*.cpp ui_*.h qrc_*.cpp

echo "Generating Makefile with $QMAKE_CMD..."
$QMAKE_CMD ltr_gui_qt5_standalone.pro

echo "Building with make..."
if make -j$(nproc) 2>&1 | tee "$BUILD_DIR/build.log"; then
    echo ""
    echo "🎉 BUILD SUCCESSFUL! 🎉"
    echo ""
    
    # Check if binary was created
    if [ -f "ltr_gui_qt5_standalone_debug" ]; then
        echo "✅ Standalone GUI binary created: ltr_gui_qt5_standalone_debug"
        echo "📁 Location: $(pwd)/ltr_gui_qt5_standalone_debug"
        echo ""
        echo "🚀 Ready to test! Run:"
        echo "   ./dev-scripts/test_qt5_standalone.sh"
    else
        echo "⚠️  Build succeeded but binary not found"
        ls -la ltr_gui_qt5_standalone*
    fi
else
    echo ""
    echo "❌ BUILD FAILED"
    echo "Check the build log: $BUILD_DIR/build.log"
    exit 1
fi

echo ""
echo "Build script completed!"
echo "Log file: $BUILD_DIR/build.log" 