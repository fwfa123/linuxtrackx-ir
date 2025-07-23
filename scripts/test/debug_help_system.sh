#!/bin/bash
# Comprehensive Help System Debugging Script for LinuxTrack AppImage

echo "=== LinuxTrack Help System Debug Script ==="
echo "This script will help diagnose help system issues in the AppImage"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if AppImage exists
if [ ! -f "LinuxTrack-x86_64.AppImage" ]; then
    print_error "LinuxTrack-x86_64.AppImage not found in current directory"
    exit 1
fi

print_status "Starting comprehensive help system analysis..."

# Step 1: Extract AppImage for analysis
print_status "Step 1: Extracting AppImage for analysis..."
./LinuxTrack-x86_64.AppImage --appimage-extract >/dev/null 2>&1
if [ $? -ne 0 ]; then
    print_error "Failed to extract AppImage"
    exit 1
fi

APPDIR="$(pwd)/squashfs-root"
print_success "AppImage extracted to $APPDIR"

# Step 2: Check AppImage structure
print_status "Step 2: Checking AppImage structure..."
echo "   AppImage contents:"
ls -la "$APPDIR/usr/bin/"
echo "   Qt libraries:"
ls -la "$APPDIR/usr/lib/" | grep -E "(Qt5|qt5)"
echo "   Qt plugins:"
find "$APPDIR/usr/lib/qt5/plugins" -name "*.so" 2>/dev/null | head -10

# Step 3: Check SQLite plugin specifically
print_status "Step 3: Checking SQLite plugin..."
SQLITE_PLUGIN_FOUND=false
if [ -f "$APPDIR/usr/lib/qt5/plugins/sqldrivers/libqsqlite.so" ]; then
    print_success "SQLite plugin found: libqsqlite.so"
    ls -la "$APPDIR/usr/lib/qt5/plugins/sqldrivers/libqsqlite.so"
    SQLITE_PLUGIN_FOUND=true
elif [ -f "$APPDIR/usr/lib/qt5/plugins/sqldrivers/libqsqlite.so.5" ]; then
    print_success "SQLite plugin found: libqsqlite.so.5"
    ls -la "$APPDIR/usr/lib/qt5/plugins/sqldrivers/libqsqlite.so.5"
    SQLITE_PLUGIN_FOUND=true
else
    print_error "SQLite plugin not found in AppImage!"
    echo "   Available SQL drivers:"
    ls -la "$APPDIR/usr/lib/qt5/plugins/sqldrivers/" 2>/dev/null || echo "   No sqldrivers directory found"
fi

# Step 4: Check Qt5Sql library
print_status "Step 4: Checking Qt5Sql library..."
if [ -f "$APPDIR/usr/lib/libQt5Sql.so.5" ]; then
    print_success "Qt5Sql library found"
    ls -la "$APPDIR/usr/lib/libQt5Sql.so.5"
else
    print_error "Qt5Sql library not found in AppImage!"
fi

# Step 5: Check help files
print_status "Step 5: Checking help files..."
HELP_DIR="$APPDIR/usr/share/linuxtrack/help/ltr_gui"
if [ -d "$HELP_DIR" ]; then
    echo "   Help directory found: $HELP_DIR"
    ls -la "$HELP_DIR/"
    
    if [ -f "$HELP_DIR/help.qhc" ]; then
        print_success "Help collection file found: help.qhc"
        ls -la "$HELP_DIR/help.qhc"
    else
        print_error "Help collection file missing: help.qhc"
    fi
    
    if [ -f "$HELP_DIR/help.qch" ]; then
        print_success "Help content file found: help.qch"
        ls -la "$HELP_DIR/help.qch"
    else
        print_error "Help content file missing: help.qch"
    fi
else
    print_error "Help directory not found: $HELP_DIR"
fi

# Step 6: Test SQLite database access
print_status "Step 6: Testing SQLite database access..."
if [ -f "$HELP_DIR/help.qch" ]; then
    if command -v sqlite3 >/dev/null 2>&1; then
        echo "   Testing help database with sqlite3..."
        if sqlite3 "$HELP_DIR/help.qch" ".tables" 2>/dev/null; then
            print_success "Help database is accessible via sqlite3"
            echo "   Database tables:"
            sqlite3 "$HELP_DIR/help.qch" ".tables" 2>/dev/null
        else
            print_error "Help database is not accessible via sqlite3"
        fi
    else
        print_warning "sqlite3 not available, skipping database test"
    fi
else
    print_warning "No help database file to test"
fi

# Step 7: Set up AppImage environment and test
print_status "Step 7: Setting up AppImage environment for testing..."
export APPDIR="$APPDIR"
export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack"
export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt5/plugins"
export QT_SQL_DRIVER_PATH="$APPDIR/usr/lib/qt5/plugins/sqldrivers"
export QT_HELP_PATH="$APPDIR/usr/share/linuxtrack/help"
export QT_DEBUG_PLUGINS=1
export QT_LOGGING_RULES="qt.help.*=true;qt.sql.*=true;qt.qpa.*=false"

echo "   Environment variables set:"
echo "     LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
echo "     QT_PLUGIN_PATH: $QT_PLUGIN_PATH"
echo "     QT_SQL_DRIVER_PATH: $QT_SQL_DRIVER_PATH"
echo "     QT_HELP_PATH: $QT_HELP_PATH"

# Step 8: Test Qt help system directly
print_status "Step 8: Testing Qt help system directly..."
if [ -f "$APPDIR/usr/bin/ltr_gui" ]; then
    echo "   Testing ltr_gui binary..."
    echo "   Binary dependencies:"
    ldd "$APPDIR/usr/bin/ltr_gui" | grep -E "(Qt5|sqlite)" || echo "   No Qt5 or SQLite dependencies found"
    
    echo "   Testing Qt help system with timeout..."
    timeout 10s "$APPDIR/usr/bin/ltr_gui" --help 2>&1 | head -20
    if [ $? -eq 124 ]; then
        print_warning "ltr_gui timed out (this is normal if it started successfully)"
    fi
else
    print_error "ltr_gui binary not found in AppImage"
fi

# Step 9: Check system SQLite plugin
print_status "Step 9: Checking system SQLite plugin availability..."
SYSTEM_SQLITE_FOUND=false
for path in "/usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so" \
            "/usr/lib/qt5/plugins/sqldrivers/libqsqlite.so" \
            "/usr/lib/qt/plugins/sqldrivers/libqsqlite.so"; do
    if [ -f "$path" ]; then
        print_success "System SQLite plugin found: $path"
        SYSTEM_SQLITE_FOUND=true
        break
    fi
done

if [ "$SYSTEM_SQLITE_FOUND" = false ]; then
    print_warning "No system SQLite plugin found in standard locations"
fi

# Step 10: Generate comprehensive report
print_status "Step 10: Generating comprehensive report..."
echo ""
echo "=== HELP SYSTEM ANALYSIS REPORT ==="
echo "Date: $(date)"
echo "AppImage: LinuxTrack-x86_64.AppImage"
echo ""

if [ "$SQLITE_PLUGIN_FOUND" = true ]; then
    print_success "✅ SQLite plugin is bundled in AppImage"
else
    print_error "❌ SQLite plugin is NOT bundled in AppImage"
fi

if [ -f "$APPDIR/usr/lib/libQt5Sql.so.5" ]; then
    print_success "✅ Qt5Sql library is bundled in AppImage"
else
    print_error "❌ Qt5Sql library is NOT bundled in AppImage"
fi

if [ -f "$HELP_DIR/help.qhc" ] && [ -f "$HELP_DIR/help.qch" ]; then
    print_success "✅ Help files are bundled in AppImage"
else
    print_error "❌ Help files are NOT properly bundled in AppImage"
fi

echo ""
echo "=== RECOMMENDATIONS ==="

if [ "$SQLITE_PLUGIN_FOUND" = false ]; then
    echo "1. ❌ CRITICAL: SQLite plugin missing - rebuild AppImage with SQLite plugin"
    echo "   - Check build script for SQLite plugin bundling"
    echo "   - Ensure QT_SQL_DRIVER_PATH is set correctly"
fi

if [ ! -f "$APPDIR/usr/lib/libQt5Sql.so.5" ]; then
    echo "2. ❌ CRITICAL: Qt5Sql library missing - rebuild AppImage with Qt5Sql"
    echo "   - Add Qt5Sql to the list of bundled libraries"
fi

if [ ! -f "$HELP_DIR/help.qhc" ] || [ ! -f "$HELP_DIR/help.qch" ]; then
    echo "3. ❌ CRITICAL: Help files missing - rebuild AppImage with help files"
    echo "   - Ensure help generation is working during build"
    echo "   - Check help file copying in build script"
fi

if [ "$SQLITE_PLUGIN_FOUND" = true ] && [ -f "$APPDIR/usr/lib/libQt5Sql.so.5" ] && [ -f "$HELP_DIR/help.qhc" ] && [ -f "$HELP_DIR/help.qch" ]; then
    echo "✅ All components appear to be properly bundled"
    echo "   If help system still fails, check:"
    echo "   - Environment variable configuration"
    echo "   - Library dependencies and rpath settings"
    echo "   - Qt plugin loading order"
fi

echo ""
echo "=== NEXT STEPS ==="
echo "1. If components are missing, rebuild the AppImage"
echo "2. If components are present but help still fails, run the AppImage with:"
echo "   QT_DEBUG_PLUGINS=1 QT_LOGGING_RULES='qt.help.*=true;qt.sql.*=true' ./LinuxTrack-x86_64.AppImage"
echo "3. Check the console output for detailed error messages"

# Cleanup
print_status "Cleaning up..."
rm -rf "$APPDIR"

print_success "Help system analysis complete!" 