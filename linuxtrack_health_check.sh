#!/bin/bash
# LinuxTrack Health Check Script
# Verifies all aspects of LinuxTrack functionality
# Usage: ./linuxtrack_health_check.sh

echo "🔍 LinuxTrack Health Check - $(date)"
echo "=================================="

# Exit on any error
set -e

# Track overall health status
HEALTH_STATUS="GOOD"
WARNINGS=0
ERRORS=0

# Function to report status
report_status() {
    local status="$1"
    local message="$2"
    
    case "$status" in
        "OK")
            echo "✅ $message"
            ;;
        "WARN")
            echo "⚠️ $message"
            WARNINGS=$((WARNINGS + 1))
            if [ "$HEALTH_STATUS" = "GOOD" ]; then
                HEALTH_STATUS="WARNING"
            fi
            ;;
        "ERROR")
            echo "❌ $message"
            ERRORS=$((ERRORS + 1))
            HEALTH_STATUS="CRITICAL"
            ;;
    esac
}

# Check TrackIR hardware
echo "🔌 Hardware Checks:"
if lsusb | grep -q "131d:0159"; then
    TRACKIR_DEVICE=$(lsusb | grep "131d:0159")
    report_status "OK" "TrackIR 5 hardware detected: $TRACKIR_DEVICE"
    
    # Check device permissions
    DEVICE_PATH=$(find /dev/bus/usb -name "*" -exec ls -la {} \; 2>/dev/null | grep "131d" | head -1)
    if echo "$DEVICE_PATH" | grep -q "rw-rw-rw"; then
        report_status "OK" "TrackIR device permissions correct"
    else
        report_status "WARN" "TrackIR device permissions may be restrictive"
    fi
else
    report_status "ERROR" "TrackIR 5 hardware NOT detected"
fi

# Check Qt5 availability
echo ""
echo "🛠️ Build System Checks:"
if command -v qmake-qt5 >/dev/null 2>&1; then
    QT5_VERSION=$(qmake-qt5 --version | head -n1)
    report_status "OK" "Qt5 qmake available: $QT5_VERSION"
    
    # Check for Qt6 interference
    DEFAULT_QMAKE_VERSION=$(qmake --version 2>/dev/null | head -n1 || echo "qmake not found")
    if echo "$DEFAULT_QMAKE_VERSION" | grep -q "Qt version 6"; then
        report_status "WARN" "Default qmake points to Qt6, may cause build issues"
    fi
else
    report_status "ERROR" "Qt5 qmake NOT available"
fi

# Check executable exists and size
echo ""
echo "📱 Application Checks:"
if [ -f "src/qt_gui/ltr_gui" ]; then
    EXECUTABLE_SIZE=$(ls -lh src/qt_gui/ltr_gui | awk '{print $5}')
    report_status "OK" "Qt5 GUI executable exists ($EXECUTABLE_SIZE)"
    
    # Check if executable is reasonable size (should be > 10MB with debug symbols)
    EXECUTABLE_BYTES=$(stat -c%s "src/qt_gui/ltr_gui")
    if [ "$EXECUTABLE_BYTES" -lt 10000000 ]; then
        report_status "WARN" "Executable size seems small, may lack debug symbols or be incomplete"
    fi
else
    report_status "ERROR" "Qt5 GUI executable missing"
fi

# Check configuration
echo ""
echo "⚙️ Configuration Checks:"
CONFIG_FILE="$HOME/.config/linuxtrack/linuxtrack1.conf"
if [ -f "$CONFIG_FILE" ]; then
    report_status "OK" "Configuration file exists"
    
    # Check prefix path
    PREFIX_PATH=$(grep "^Prefix" "$CONFIG_FILE" | cut -d'=' -f2 | tr -d ' "')
    if [ -n "$PREFIX_PATH" ] && [ "$PREFIX_PATH" != "/src/qt_gui" ]; then
        report_status "OK" "Configuration prefix path looks correct: $PREFIX_PATH"
    else
        report_status "WARN" "Configuration prefix path may be incorrect: $PREFIX_PATH"
    fi
    
    # Check input device setting
    INPUT_DEVICE=$(grep "^Input" "$CONFIG_FILE" | cut -d'=' -f2 | tr -d ' ')
    if [ "$INPUT_DEVICE" = "TrackIR" ]; then
        report_status "OK" "Input device configured for TrackIR"
    else
        report_status "WARN" "Input device not set to TrackIR: $INPUT_DEVICE"
    fi
else
    report_status "ERROR" "Configuration file missing"
fi

# Check library loading
echo ""
echo "📚 Library Checks:"
if [ -f "src/qt_gui/ltr_gui" ]; then
    MISSING_LIBS=$(ldd src/qt_gui/ltr_gui 2>/dev/null | grep "not found" || true)
    if [ -z "$MISSING_LIBS" ]; then
        report_status "OK" "All libraries load successfully"
    else
        report_status "ERROR" "Library loading issues detected:"
        echo "$MISSING_LIBS"
    fi
    
    # Check for required LinuxTrack libraries
    REQUIRED_LIBS=("libltr.so" "libtir.so" "libwc.so" "libjoy.so" "libltusb1.so")
    for lib in "${REQUIRED_LIBS[@]}"; do
        if ls src/.libs/${lib}* >/dev/null 2>&1; then
            LIB_SIZE=$(ls -lh src/.libs/${lib}* | head -1 | awk '{print $5}')
            report_status "OK" "$lib available ($LIB_SIZE)"
        else
            report_status "ERROR" "$lib missing from src/.libs/"
        fi
    done
fi

# Check udev rules
echo ""
echo "🔧 System Integration Checks:"
UDEV_RULES_FILE="/lib/udev/rules.d/99-TIR.rules"
if [ -f "$UDEV_RULES_FILE" ]; then
    report_status "OK" "udev rules installed"
    
    # Check if rules contain TrackIR USB ID
    if grep -q "131d" "$UDEV_RULES_FILE"; then
        report_status "OK" "udev rules contain TrackIR USB ID"
    else
        report_status "WARN" "udev rules may not contain TrackIR USB ID"
    fi
else
    report_status "ERROR" "udev rules NOT installed"
fi

# Check user groups
USER_GROUPS=$(groups)
if echo "$USER_GROUPS" | grep -q "plugdev"; then
    report_status "OK" "User in plugdev group"
else
    report_status "WARN" "User NOT in plugdev group"
fi

if echo "$USER_GROUPS" | grep -q "linuxtrack"; then
    report_status "OK" "User in linuxtrack group"
else
    report_status "WARN" "User NOT in linuxtrack group"
fi

# Check launch script
echo ""
echo "🚀 Launch Script Checks:"
if [ -f "run_qt5_gui.sh" ]; then
    if [ -x "run_qt5_gui.sh" ]; then
        report_status "OK" "Launch script exists and is executable"
        
        # Check if script references correct executable
        if grep -q "ltr_gui" "run_qt5_gui.sh"; then
            report_status "OK" "Launch script references correct executable"
        else
            report_status "WARN" "Launch script may reference wrong executable"
        fi
    else
        report_status "WARN" "Launch script exists but is not executable"
    fi
else
    report_status "ERROR" "Launch script missing"
fi

# Performance check (if GUI is not running)
echo ""
echo "📊 Performance Checks:"
if ! pgrep -f ltr_gui >/dev/null; then
    # Quick launch test
    echo "🧪 Testing quick launch..."
    timeout 15s ./run_qt5_gui.sh >/dev/null 2>&1 &
    LAUNCH_PID=$!
    sleep 5
    
    if pgrep -f ltr_gui >/dev/null; then
        MEMORY_USAGE=$(ps -o rss -p $(pgrep ltr_gui) --no-headers | awk '{print int($1/1024)}')
        report_status "OK" "Application launches successfully (${MEMORY_USAGE}MB RAM)"
        
        # Clean up test process
        pkill -f ltr_gui >/dev/null 2>&1 || true
        wait $LAUNCH_PID 2>/dev/null || true
    else
        report_status "ERROR" "Application failed to launch during test"
        # Clean up any hanging processes
        kill $LAUNCH_PID 2>/dev/null || true
    fi
else
    report_status "OK" "Application already running (skipping launch test)"
fi

# Summary
echo ""
echo "=================================="
case "$HEALTH_STATUS" in
    "GOOD")
        echo "🎉 LinuxTrack system health: EXCELLENT"
        echo "✅ All systems operational"
        ;;
    "WARNING")
        echo "⚠️ LinuxTrack system health: WARNING"
        echo "⚠️ $WARNINGS warning(s) detected - system functional but needs attention"
        ;;
    "CRITICAL")
        echo "❌ LinuxTrack system health: CRITICAL"
        echo "❌ $ERRORS error(s) detected - system may not function properly"
        echo "🔧 Run ./linuxtrack_quick_recovery.sh to attempt automatic fixes"
        ;;
esac

echo ""
echo "📋 Health Check Summary:"
echo "   Warnings: $WARNINGS"
echo "   Errors: $ERRORS"
echo "   Status: $HEALTH_STATUS"
echo "   Time: $(date)"

# Exit with appropriate code
case "$HEALTH_STATUS" in
    "GOOD") exit 0 ;;
    "WARNING") exit 1 ;;
    "CRITICAL") exit 2 ;;
esac 