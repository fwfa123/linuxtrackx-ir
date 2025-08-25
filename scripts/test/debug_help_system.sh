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

# Enhanced environment variable validation
print_status "Step 7.5: Validating environment variable configuration..."
echo "   Checking for duplicate QT_HELP_PATH exports..."

# Check if there are multiple QT_HELP_PATH exports in the environment
QT_HELP_PATH_COUNT=$(env | grep -c "^QT_HELP_PATH=" || echo "0")
if [ "$QT_HELP_PATH_COUNT" -eq 1 ]; then
    print_success "Single QT_HELP_PATH export detected"
elif [ "$QT_HELP_PATH_COUNT" -gt 1 ]; then
    print_error "Multiple QT_HELP_PATH exports detected - this will cause conflicts!"
    env | grep "^QT_HELP_PATH="
else
    print_warning "No QT_HELP_PATH export found"
fi

# Check for conflicting Qt environment variables
echo "   Checking for Qt environment conflicts..."
if [ -n "$QT_DIR" ] || [ -n "$QTDIR" ] || [ -n "$QT_SELECT" ]; then
    print_warning "System Qt environment variables detected that may conflict:"
    [ -n "$QT_DIR" ] && echo "     QT_DIR: $QT_DIR"
    [ -n "$QTDIR" ] && echo "     QTDIR: $QTDIR"
    [ -n "$QT_SELECT" ] && echo "     QT_SELECT: $QT_SELECT"
else
    print_success "No conflicting Qt environment variables detected"
fi

# Validate plugin paths
echo "   Validating plugin paths..."
if [ -d "$QT_PLUGIN_PATH" ]; then
    PLUGIN_COUNT=$(find "$QT_PLUGIN_PATH" -type f -name "*.so" 2>/dev/null | wc -l)
    print_success "Plugin path valid: $QT_PLUGIN_PATH ($PLUGIN_COUNT plugins)"
else
    print_error "Plugin path invalid: $QT_PLUGIN_PATH"
fi

if [ -d "$QT_SQL_DRIVER_PATH" ]; then
    SQL_DRIVER_COUNT=$(find "$QT_SQL_DRIVER_PATH" -type f -name "*.so" 2>/dev/null | wc -l)
    print_success "SQL driver path valid: $QT_SQL_DRIVER_PATH ($SQL_DRIVER_COUNT drivers)"
else
    print_error "SQL driver path invalid: $QT_SQL_DRIVER_PATH"
fi

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

# Step 8.5: Test enhanced help system runtime setup and error handling
print_status "Step 8.5: Testing enhanced help system runtime setup..."
echo "   Testing comprehensive help system error handling and fallbacks..."

# Test the enhanced help runtime setup logic
HELP_RUNTIME_ROOT="${XDG_DATA_HOME:-$HOME/.local/share}/linuxtrack/help"
HELP_SYSTEM_STATUS="initializing"
HELP_SYSTEM_ERRORS=()
HELP_SYSTEM_WARNINGS=()

echo "   Help runtime root: $HELP_RUNTIME_ROOT"

# Test directory creation with fallback
if mkdir -p "$HELP_RUNTIME_ROOT/ltr_gui" "$HELP_RUNTIME_ROOT/mickey" 2>/dev/null; then
    print_success "Help runtime directories created successfully"
else
    print_warning "Primary help directories failed, testing fallback..."
    FALLBACK_HELP_ROOT="$HOME/.linuxtrack/help"
    if mkdir -p "$FALLBACK_HELP_ROOT/ltr_gui" "$FALLBACK_HELP_ROOT/mickey" 2>/dev/null; then
        HELP_RUNTIME_ROOT="$FALLBACK_HELP_ROOT"
        print_success "Fallback help directories created successfully"
        HELP_SYSTEM_STATUS="using_fallback"
    else
        print_error "Both primary and fallback directories failed"
        HELP_SYSTEM_STATUS="completely_failed"
    fi
fi

# Test enhanced help file copying with validation
HELP_FILES_COPIED=0
HELP_COMPONENTS_READY=0

echo "   Testing enhanced help file copying..."
for comp in ltr_gui mickey; do
    COMPONENT_READY=true
    COMPONENT_FILES=0
    
    echo "     Processing $comp component..."
    
    for f in help.qhc help.qch; do
        source_file="$APPDIR/usr/share/linuxtrack/help/$comp/$f"
        target_file="$HELP_RUNTIME_ROOT/$comp/$f"
        
        if [ -f "$source_file" ]; then
            if cp -f "$source_file" "$target_file" 2>/dev/null; then
                if chmod u+w "$target_file" 2>/dev/null; then
                    echo "       ✓ Copied $f for $comp"
                    ((HELP_FILES_COPIED++))
                    ((COMPONENT_FILES++))
                else
                    echo "       ⚠️  Could not make $f writable for $comp"
                    COMPONENT_READY=false
                fi
            else
                echo "       ✗ Failed to copy $f for $comp"
                COMPONENT_READY=false
            fi
        else
            echo "       ⚠️  Help file $f not found for $comp"
            COMPONENT_READY=false
        fi
    done
    
    # Validate component readiness
    if [ "$COMPONENT_READY" = true ] && [ $COMPONENT_FILES -ge 2 ]; then
        echo "       ✅ $comp component ready ($COMPONENT_FILES files)"
        ((HELP_COMPONENTS_READY++))
    else
        echo "       ❌ $comp component not ready"
    fi
done

echo "   Total help files copied: $HELP_FILES_COPIED"
echo "   Components ready: $HELP_COMPONENTS_READY/2"

# Test environment variable handling based on status
if [ "$HELP_SYSTEM_STATUS" = "completely_failed" ] || [ $HELP_FILES_COPIED -eq 0 ]; then
    echo "   Note: Help system will be disabled at runtime"
    unset QT_HELP_PATH
else
    export QT_HELP_PATH="$HELP_RUNTIME_ROOT"
    echo "   QT_HELP_PATH set to: $QT_HELP_PATH"
fi

# Validate runtime help system
if [ $HELP_FILES_COPIED -gt 0 ] && [ $HELP_COMPONENTS_READY -gt 0 ]; then
    print_success "Enhanced help system runtime setup successful"
    echo "   Runtime help files:"
    find "$HELP_RUNTIME_ROOT" -type f 2>/dev/null | while read -r file; do
        echo "     - $file"
    done
else
    print_warning "Help system runtime setup incomplete - functionality may be limited"
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