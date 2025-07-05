#!/bin/bash
# LinuxTrack Quick Recovery Script
# Automatically attempts to fix common issues
# Usage: ./linuxtrack_quick_recovery.sh

echo "🚑 LinuxTrack Quick Recovery - $(date)"
echo "=================================="

# Force exit on any error during critical operations
set -e

# Track recovery actions
ACTIONS_TAKEN=0
RECOVERY_SUCCESS=true

# Function to report actions
report_action() {
    local status="$1"
    local message="$2"
    
    case "$status" in
        "INFO")
            echo "ℹ️ $message"
            ;;
        "ACTION")
            echo "🔧 $message"
            ACTIONS_TAKEN=$((ACTIONS_TAKEN + 1))
            ;;
        "SUCCESS")
            echo "✅ $message"
            ;;
        "ERROR")
            echo "❌ $message"
            RECOVERY_SUCCESS=false
            ;;
    esac
}

# Check if we're in the right directory
if [ ! -f "src/qt_gui/ltr_gui.pro" ] && [ ! -f "configure.ac" ]; then
    report_action "ERROR" "Not in LinuxTrack project directory. Please run from project root."
    exit 1
fi

report_action "INFO" "Starting LinuxTrack automatic recovery sequence..."

# Step 1: Force Qt5 environment
report_action "ACTION" "Setting up Qt5 environment..."
export QT_SELECT=qt5
export PATH="/usr/bin:$PATH"

# Verify Qt5 availability
if command -v qmake-qt5 >/dev/null 2>&1; then
    QT5_VERSION=$(qmake-qt5 --version | head -n1)
    report_action "SUCCESS" "Qt5 confirmed available: $QT5_VERSION"
else
    report_action "ERROR" "Qt5 qmake not found. Cannot proceed with recovery."
    exit 1
fi

# Step 2: Clean previous build artifacts
report_action "ACTION" "Cleaning previous build artifacts..."
if [ -d "src/qt_gui" ]; then
    cd src/qt_gui
    
    # Remove potentially problematic files
    rm -f Makefile
    rm -f ltr_gui
    rm -f ltr_gui_qt5_debug
    rm -f *.o
    rm -f moc_*.cpp
    rm -f qrc_*.cpp
    
    report_action "SUCCESS" "Qt GUI directory cleaned"
    cd ../..
else
    report_action "ERROR" "Qt GUI source directory not found"
    exit 1
fi

# Step 3: Regenerate build system with Qt5
report_action "ACTION" "Regenerating build system with Qt5..."
cd src/qt_gui

# Use Qt5 qmake explicitly
if qmake-qt5 ltr_gui.pro; then
    report_action "SUCCESS" "Qt5 Makefile generated successfully"
else
    report_action "ERROR" "Failed to generate Qt5 Makefile"
    exit 1
fi

# Step 4: Build Qt5 GUI
report_action "ACTION" "Building Qt5 GUI executable..."
make clean >/dev/null 2>&1 || true  # Clean quietly, ignore errors

if make -j$(nproc); then
    if [ -f "ltr_gui" ]; then
        EXECUTABLE_SIZE=$(ls -lh ltr_gui | awk '{print $5}')
        report_action "SUCCESS" "Qt5 GUI built successfully ($EXECUTABLE_SIZE)"
    else
        report_action "ERROR" "Build completed but executable not found"
        exit 1
    fi
else
    report_action "ERROR" "Failed to build Qt5 GUI"
    exit 1
fi

cd ../..

# Step 5: Verify and fix launch script
report_action "ACTION" "Verifying launch script..."
if [ -f "run_qt5_gui.sh" ]; then
    # Make sure it's executable
    chmod +x run_qt5_gui.sh
    
    # Check if it references the correct executable
    if grep -q "ltr_gui_qt5_debug" run_qt5_gui.sh; then
        report_action "ACTION" "Updating launch script executable reference..."
        sed -i 's/ltr_gui_qt5_debug/ltr_gui/g' run_qt5_gui.sh
        report_action "SUCCESS" "Launch script updated"
    else
        report_action "SUCCESS" "Launch script already references correct executable"
    fi
else
    report_action "ACTION" "Creating missing launch script..."
    cat > run_qt5_gui.sh << 'EOF'
#!/bin/bash
echo "🚀 Starting LinuxTrack Qt5 GUI (Auto-recovered)"
echo "📍 Working directory: $(pwd)"

# Set library path and Qt5 environment
export LD_LIBRARY_PATH="$(pwd)/src/.libs:$LD_LIBRARY_PATH"
export QT_SELECT=qt5
echo "🔧 Library path: $LD_LIBRARY_PATH"

# Launch Qt5 GUI
echo "📂 GUI directory: $(pwd)/src/qt_gui"
cd src/qt_gui
echo "✨ Launching Qt5 GUI..."
./ltr_gui
echo "🏁 Qt5 GUI closed."
EOF
    chmod +x run_qt5_gui.sh
    report_action "SUCCESS" "Launch script created"
fi

# Step 6: Check and fix configuration
report_action "ACTION" "Checking configuration..."
CONFIG_FILE="$HOME/.config/linuxtrack/linuxtrack1.conf"
if [ -f "$CONFIG_FILE" ]; then
    # Check for problematic prefix paths
    PREFIX_PATH=$(grep "^Prefix" "$CONFIG_FILE" | cut -d'=' -f2 | tr -d ' "' || echo "")
    if echo "$PREFIX_PATH" | grep -q "/src/qt_gui"; then
        report_action "ACTION" "Fixing hardcoded prefix path in configuration..."
        PROJECT_ROOT=$(pwd)
        sed -i "s|^Prefix = .*|Prefix = \"$PROJECT_ROOT\"|" "$CONFIG_FILE"
        report_action "SUCCESS" "Configuration prefix path corrected"
    else
        report_action "SUCCESS" "Configuration prefix path looks correct"
    fi
    
    # Ensure TrackIR is selected as input
    if ! grep -q "^Input = TrackIR" "$CONFIG_FILE"; then
        report_action "ACTION" "Setting input device to TrackIR..."
        if grep -q "^Input" "$CONFIG_FILE"; then
            sed -i 's/^Input = .*/Input = TrackIR/' "$CONFIG_FILE"
        else
            echo "Input = TrackIR" >> "$CONFIG_FILE"
        fi
        report_action "SUCCESS" "Input device set to TrackIR"
    fi
else
    report_action "ACTION" "Creating basic configuration file..."
    mkdir -p "$HOME/.config/linuxtrack"
    cat > "$CONFIG_FILE" << EOF
[Global]
Input = TrackIR
Model = NP TrackClip Pro
Prefix = "$(pwd)"
EOF
    report_action "SUCCESS" "Basic configuration created"
fi

# Step 7: Check required libraries
report_action "ACTION" "Verifying required libraries..."
REQUIRED_LIBS=("libltr.so" "libtir.so" "libwc.so" "libjoy.so" "libltusb1.so")
MISSING_LIBS=()

for lib in "${REQUIRED_LIBS[@]}"; do
    if ! ls src/.libs/${lib}* >/dev/null 2>&1; then
        MISSING_LIBS+=("$lib")
    fi
done

if [ ${#MISSING_LIBS[@]} -gt 0 ]; then
    report_action "ACTION" "Missing libraries detected, rebuilding core libraries..."
    if make -j$(nproc) >/dev/null 2>&1; then
        report_action "SUCCESS" "Core libraries rebuilt"
    else
        report_action "ERROR" "Failed to rebuild core libraries"
        RECOVERY_SUCCESS=false
    fi
else
    report_action "SUCCESS" "All required libraries present"
fi

# Step 8: Test launch capability
report_action "ACTION" "Testing application launch..."

# Kill any existing instances first
pkill -f ltr_gui >/dev/null 2>&1 || true
sleep 2

# Test launch with timeout
timeout 10s ./run_qt5_gui.sh >/dev/null 2>&1 &
LAUNCH_PID=$!
sleep 5

if pgrep -f ltr_gui >/dev/null; then
    MEMORY_USAGE=$(ps -o rss -p $(pgrep ltr_gui) --no-headers | awk '{print int($1/1024)}' 2>/dev/null || echo "unknown")
    report_action "SUCCESS" "Application launches successfully (${MEMORY_USAGE}MB RAM)"
    
    # Clean up test process
    pkill -f ltr_gui >/dev/null 2>&1 || true
    wait $LAUNCH_PID 2>/dev/null || true
else
    report_action "ERROR" "Application failed to launch during test"
    RECOVERY_SUCCESS=false
    # Clean up any hanging processes
    kill $LAUNCH_PID 2>/dev/null || true
fi

# Step 9: Check TrackIR hardware detection
report_action "ACTION" "Checking TrackIR hardware detection..."
if lsusb | grep -q "131d:0159"; then
    TRACKIR_DEVICE=$(lsusb | grep "131d:0159")
    report_action "SUCCESS" "TrackIR hardware detected: $TRACKIR_DEVICE"
else
    report_action "ERROR" "TrackIR hardware not detected - check USB connection"
    RECOVERY_SUCCESS=false
fi

# Step 10: Verify udev rules
report_action "ACTION" "Checking udev rules installation..."
if [ -f "/lib/udev/rules.d/99-TIR.rules" ]; then
    report_action "SUCCESS" "udev rules are installed"
else
    if [ -f "src/99-TIR.rules" ]; then
        report_action "ACTION" "Installing missing udev rules..."
        if sudo cp src/99-TIR.rules /lib/udev/rules.d/ && sudo udevadm control --reload-rules; then
            report_action "SUCCESS" "udev rules installed and reloaded"
        else
            report_action "ERROR" "Failed to install udev rules (permission issue?)"
            RECOVERY_SUCCESS=false
        fi
    else
        report_action "ERROR" "udev rules source file not found"
        RECOVERY_SUCCESS=false
    fi
fi

# Final status report
echo ""
echo "=================================="
if [ "$RECOVERY_SUCCESS" = true ]; then
    echo "🎉 LinuxTrack Quick Recovery: SUCCESSFUL"
    echo "✅ $ACTIONS_TAKEN recovery action(s) completed"
    echo ""
    echo "🚀 Ready to use:"
    echo "   ./run_qt5_gui.sh"
    echo ""
    echo "📊 Run health check to verify full functionality:"
    echo "   ./linuxtrack_health_check.sh"
else
    echo "⚠️ LinuxTrack Quick Recovery: PARTIAL"
    echo "🔧 $ACTIONS_TAKEN recovery action(s) completed"
    echo "❌ Some issues could not be automatically resolved"
    echo ""
    echo "📖 For complex issues, see:"
    echo "   QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md"
    echo "   TRACKIR_PERMISSION_TROUBLESHOOTING.md"
    echo ""
    echo "📊 Run health check for detailed diagnosis:"
    echo "   ./linuxtrack_health_check.sh"
fi

echo ""
echo "📋 Recovery Summary:"
echo "   Actions Taken: $ACTIONS_TAKEN"
echo "   Success: $RECOVERY_SUCCESS"
echo "   Time: $(date)"

# Set appropriate exit code
if [ "$RECOVERY_SUCCESS" = true ]; then
    exit 0
else
    exit 1
fi 