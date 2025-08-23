#!/bin/bash

echo "=== LinuxTrack-X-IR Fix Verification ==="
echo

# Check if SVG icons exist
echo "1. Checking SVG icons:"
ICONS=("play.svg" "stop.svg" "pause.svg")
for icon in "${ICONS[@]}"; do
    if [ -f "src/qt_gui/icons/$icon" ]; then
        echo "   ✅ $icon found"
    else
        echo "   ❌ $icon missing"
    fi
done

echo

# Check if icons are in Qt resource file
echo "2. Checking Qt resource file:"
if grep -q "icons/play.svg" src/qt_gui/ltr_rc.qrc && \
   grep -q "icons/stop.svg" src/qt_gui/ltr_rc.qrc && \
   grep -q "icons/pause.svg" src/qt_gui/ltr_rc.qrc; then
    echo "   ✅ All icons added to ltr_rc.qrc"
else
    echo "   ❌ Some icons missing from ltr_rc.qrc"
fi

echo

# Check Qt signal/slot fix
echo "3. Checking Qt signal/slot fix:"
if grep -q "on_TesterExeRadioButton_toggled.*true" src/qt_gui/ltr_gui.cpp && \
   ! grep -q "SIGNAL(testingWorkflowStarted()).*SLOT(on_TesterExeRadioButton_toggled(bool))" src/qt_gui/ltr_gui.cpp; then
    echo "   ✅ Qt signal/slot connection fixed"
else
    echo "   ❌ Qt signal/slot issue may still exist"
fi

echo

# Check AppRun library path fix
echo "4. Checking AppRun library path:"
if grep -q "LD_LIBRARY_PATH.*usr/lib/linuxtrack" AppDir_v2/AppRun.wrapped; then
    echo "   ✅ LD_LIBRARY_PATH fix applied"
else
    echo "   ❌ LD_LIBRARY_PATH fix missing"
fi

echo

# Check if libraries exist in AppImage
echo "5. Checking library presence:"
LIBS=("libtir.so.0" "libwc.so.0" "libjoy.so.0")
for lib in "${LIBS[@]}"; do
    if [ -f "AppDir_v2/usr/lib/linuxtrack/$lib" ]; then
        echo "   ✅ $lib found in AppImage"
    else
        echo "   ❌ $lib missing from AppImage"
    fi
done

echo
echo "=== Fix Verification Complete ==="
echo "If all checks show ✅, the fixes should resolve the runtime issues."
