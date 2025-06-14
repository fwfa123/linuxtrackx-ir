#!/bin/bash

# LinuxTrack Qt5 GUI Build Script
# This script attempts to build the Qt5 version of the GUI

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

echo "Building LinuxTrack GUI with Qt5..."
echo "Project root: $PROJECT_ROOT"

# Check if we have Qt5
if ! command -v qmake &> /dev/null; then
    echo "Error: qmake not found. Please install Qt5 development packages."
    echo "Run: ./dev-scripts/setup_deps.sh"
    exit 1
fi

# Show Qt version
echo "Qt version: $(qmake -v)"

# Create build directory
BUILD_DIR="$PROJECT_ROOT/build_qt5"
mkdir -p "$BUILD_DIR"

# Change to qt_gui directory
cd src/qt_gui

echo "Cleaning previous build..."
rm -f ltr_gui_qt5 Makefile* *.o moc_*.cpp ui_*.h qrc_*.cpp

echo "Generating Makefile with qmake..."
qmake ltr_gui_qt5.pro

echo "Building with make..."
if make -j$(nproc) 2>&1 | tee "$BUILD_DIR/build.log"; then
    echo "Build completed successfully!"
    
    if [ -f "ltr_gui_qt5" ]; then
        echo "Executable created: $(pwd)/ltr_gui_qt5"
        echo "Size: $(ls -lh ltr_gui_qt5 | awk '{print $5}')"
        
        # Test if it runs (basic check)
        echo "Testing if executable runs..."
        if timeout 5 ./ltr_gui_qt5 --version 2>/dev/null || timeout 5 ./ltr_gui_qt5 --help 2>/dev/null; then
            echo "Basic executable test passed!"
        else
            echo "Warning: Executable may have runtime issues"
        fi
    else
        echo "Warning: Build succeeded but executable not found"
    fi
else
    echo "Build failed! Check build log: $BUILD_DIR/build.log"
    echo ""
    echo "Common issues and solutions:"
    echo "1. Missing Qt5 development packages:"
    echo "   - Run: ./dev-scripts/setup_deps.sh"
    echo ""
    echo "2. Missing linuxtrack core library:"
    echo "   - Need to build core library first"
    echo "   - Or comment out -lltr from .pro file for GUI-only build"
    echo ""
    echo "3. Missing headers:"
    echo "   - Some source files may need Qt5-specific updates"
    echo ""
    echo "Last few lines of build log:"
    tail -20 "$BUILD_DIR/build.log" || echo "Could not read build log"
    exit 1
fi

echo ""
echo "Build script completed!"
echo "Log file: $BUILD_DIR/build.log" 