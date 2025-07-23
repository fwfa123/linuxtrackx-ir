#!/bin/bash
# Qt Help System Debugging Script for LinuxTrack AppImage

echo "=== Qt Help System Debug Script ==="
echo "Testing Qt help system in AppImage environment..."
echo ""

# Extract AppImage for testing
echo "1. Extracting AppImage..."
./LinuxTrack-x86_64.AppImage --appimage-extract >/dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "ERROR: Failed to extract AppImage"
    exit 1
fi

echo "2. Setting up AppImage environment..."
export APPDIR="$(pwd)/squashfs-root"
export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack"
export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins"
export QT_QPA_PLATFORM_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins/platforms"
export QT_HELP_PATH="$APPDIR/usr/share/linuxtrack/help"
export QT_DEBUG_PLUGINS=1
export QT_LOGGING_RULES="qt.help.*=true;qt.qpa.*=false"

echo "3. Verifying help file structure..."
echo "   Help collection file:"
ls -la "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qhc"
echo "   Help database file:"
ls -la "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qch"
echo "   Help content directory:"
ls -la "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help/"

echo "4. Testing Qt help database content..."
if command -v sqlite3 >/dev/null 2>&1; then
    echo "   Database tables:"
    sqlite3 "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qch" ".tables" 2>/dev/null
    echo "   Contents table count:"
    sqlite3 "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qch" "SELECT COUNT(*) as total_topics FROM ContentsTable;" 2>/dev/null
    echo "   File data table count:"
    sqlite3 "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qch" "SELECT COUNT(*) as total_files FROM FileDataTable;" 2>/dev/null
else
    echo "   sqlite3 not available, skipping database tests"
fi

echo "5. Testing Qt help plugins..."
echo "   KIO plugins:"
ls -la "$APPDIR/usr/lib/qt5/plugins/kf5/kio/"
echo "   Kauth plugins:"
ls -la "$APPDIR/usr/lib/qt5/plugins/kauth/helper/"

echo "6. Testing Qt help library loading..."
echo "   Qt5Help library:"
ls -la "$APPDIR/usr/lib/libQt5Help.so.5"
echo "   Qt5Sql library:"
ls -la "$APPDIR/usr/lib/libQt5Sql.so.5"

echo "7. Testing Qt help environment variables..."
echo "   QT_HELP_PATH: $QT_HELP_PATH"
echo "   QT_PLUGIN_PATH: $QT_PLUGIN_PATH"
echo "   LD_LIBRARY_PATH: $LD_LIBRARY_PATH"

echo "8. Testing Qt help library dependencies..."
if [ -f "$APPDIR/usr/bin/ltr_gui" ]; then
    echo "   ltr_gui Qt help dependencies:"
    ldd "$APPDIR/usr/bin/ltr_gui" | grep -E "(Qt5Help|Qt5Sql)" || echo "   No Qt help dependencies found"
fi

echo "9. Testing help file path resolution..."
echo "   Expected path: $APPDIR/usr/bin/../share/linuxtrack/help/ltr_gui/help.qhc"
echo "   Resolved path: $APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qhc"
echo "   File exists: $(test -f "$APPDIR/usr/share/linuxtrack/help/ltr_gui/help.qhc" && echo "YES" || echo "NO")"

echo "10. Testing Qt help URL scheme..."
echo "   Testing URL: qthelp://uglyDwarf.com.linuxtrack.1.0/doc/help/welcome.htm"
echo "   This URL should be resolved by the Qt help system"

echo ""
echo "=== Qt Help Debug Complete ==="
echo "If the help system is still blank, check the following:"
echo "1. Qt help plugins are properly loaded"
echo "2. Help database is accessible and valid"
echo "3. Qt help URL scheme is properly registered"
echo "4. Application is using the correct help file path"

# Cleanup
rm -rf squashfs-root
