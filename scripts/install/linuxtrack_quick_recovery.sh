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
if [ ! -f "CMakeLists.txt" ] && [ ! -f "src/qt_gui/CMakeLists.txt" ]; then
    report_action "ERROR" "Not in LinuxTrack project directory. Please run from project root."
    exit 1
fi

report_action "INFO" "Starting LinuxTrack automatic recovery sequence..."

# Step 1: Verify CMake and Qt5 availability
report_action "ACTION" "Verifying build tools..."
export QT_SELECT=qt5
export PATH="/usr/bin:$PATH"

# Check for CMake
if ! command -v cmake >/dev/null 2>&1; then
    report_action "ERROR" "CMake not found. Please install cmake package."
    exit 1
fi

CMAKE_VERSION=$(cmake --version | head -n1)
report_action "SUCCESS" "CMake confirmed available: $CMAKE_VERSION"

# Check for Qt5 (CMake will find it, but verify qhelpgenerator for help files)
if command -v qhelpgenerator-qt5 >/dev/null 2>&1 || command -v qhelpgenerator >/dev/null 2>&1; then
    report_action "SUCCESS" "Qt5 tools available"
else
    report_action "INFO" "Qt5 qhelpgenerator not found (help files may not be generated)"
fi

# Step 2: Clean previous build artifacts
report_action "ACTION" "Cleaning previous build artifacts..."
if [ -d "build" ]; then
    rm -rf build
    report_action "SUCCESS" "Previous build directory removed"
fi

# Also clean any legacy autotools artifacts
if [ -d "src/qt_gui" ]; then
    cd src/qt_gui
    rm -f Makefile ltr_gui ltr_gui_qt5_debug *.o moc_*.cpp qrc_*.cpp 2>/dev/null || true
    cd ../..
fi

# Step 3: Configure build with CMake
report_action "ACTION" "Configuring build with CMake..."
mkdir -p build
cd build

# Configure with CMake (use default prefix /opt, can be overridden)
if cmake .. -DCMAKE_INSTALL_PREFIX=/opt; then
    report_action "SUCCESS" "CMake configuration completed successfully"
else
    report_action "ERROR" "Failed to configure build with CMake"
    cd ..
    exit 1
fi

# Step 4: Build project
report_action "ACTION" "Building LinuxTrack with CMake..."
if cmake --build . -j$(nproc); then
    # Check for built executables
    if [ -f "src/qt_gui/ltr_gui" ]; then
        EXECUTABLE_SIZE=$(ls -lh src/qt_gui/ltr_gui | awk '{print $5}')
        report_action "SUCCESS" "LinuxTrack built successfully (ltr_gui: $EXECUTABLE_SIZE)"
    else
        report_action "ERROR" "Build completed but ltr_gui executable not found"
        cd ..
        exit 1
    fi
else
    report_action "ERROR" "Failed to build LinuxTrack"
    cd ..
    exit 1
fi

cd ..

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
# Get script directory (project root)
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR" || exit 1

echo "🚀 Starting LinuxTrack Qt5 GUI (Auto-recovered)"
echo "📍 Working directory: $SCRIPT_DIR"

# Set library path and Qt5 environment
export LD_LIBRARY_PATH="$SCRIPT_DIR/build/src:$LD_LIBRARY_PATH"
export QT_SELECT=qt5
echo "🔧 Library path: $LD_LIBRARY_PATH"

# Launch Qt5 GUI from build directory
GUI_EXECUTABLE="$SCRIPT_DIR/build/src/qt_gui/ltr_gui"
echo "📂 GUI executable: $GUI_EXECUTABLE"
if [ -f "$GUI_EXECUTABLE" ]; then
    echo "✨ Launching Qt5 GUI..."
    "$GUI_EXECUTABLE"
    echo "🏁 Qt5 GUI closed."
else
    echo "❌ Error: ltr_gui executable not found. Please build the project first."
    echo "   Expected location: $GUI_EXECUTABLE"
    exit 1
fi
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

# Core libraries (always built)
REQUIRED_LIBS=("libltr.so" "libtir.so" "libjoy.so" "libltusb1.so")

# Check if webcam support is enabled (libwc.so is conditional)
# libwc.so is only built when ENABLE_WEBCAM=ON AND WITH_LIBV4L2=TRUE
if [ -f "build/CMakeCache.txt" ]; then
    # Check if ENABLE_WEBCAM is ON in CMake cache
    if grep -q "^ENABLE_WEBCAM:BOOL=ON" build/CMakeCache.txt 2>/dev/null; then
        # Note: libwc also requires WITH_LIBV4L2, but we check ENABLE_WEBCAM as the primary indicator
        # If ENABLE_WEBCAM is ON but libwc.so doesn't exist, it may be due to missing libv4l2,
        # which is still worth checking/reporting
        REQUIRED_LIBS+=("libwc.so")
        report_action "INFO" "Webcam support enabled - checking for libwc.so"
    else
        report_action "INFO" "Webcam support disabled - skipping libwc.so check"
    fi
elif [ -d "build/src" ]; then
    # If build exists but no cache (unlikely), check if libwc.so was built
    # If it exists, include it in checks; if not, assume webcam is disabled
    if find build/src -name "libwc.so*" -type f >/dev/null 2>&1; then
        REQUIRED_LIBS+=("libwc.so")
        report_action "INFO" "libwc.so found in build - including in checks"
    fi
fi

MISSING_LIBS=()

# Check for libraries in build directory (CMake output location)
for lib in "${REQUIRED_LIBS[@]}"; do
    if ! find build/src -name "${lib}*" -type f >/dev/null 2>&1; then
        MISSING_LIBS+=("$lib")
    fi
done

if [ ${#MISSING_LIBS[@]} -gt 0 ]; then
    report_action "ACTION" "Missing libraries detected, rebuilding project..."
    cd build
    if cmake --build . -j$(nproc) >/dev/null 2>&1; then
        report_action "SUCCESS" "Project rebuilt successfully"
    else
        report_action "ERROR" "Failed to rebuild project"
        RECOVERY_SUCCESS=false
    fi
    cd ..
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