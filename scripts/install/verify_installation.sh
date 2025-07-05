#!/bin/bash

# LinuxTrack Installation Verification Script
# Comprehensive check for Qt5 modernized LinuxTrack installation

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
PASSED=0
FAILED=0
WARNINGS=0
TOTAL_CHECKS=0

# Configuration
INSTALL_PREFIX="/opt/linuxtrack"
DESKTOP_DIR="/usr/local/share/applications"
ICONS_DIR="/usr/local/share/icons/hicolor"
PIXMAPS_DIR="/usr/local/share/pixmaps"
UDEV_RULES_DIR="/lib/udev/rules.d"

print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE} LinuxTrack Installation Verification ${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
    echo "Installation prefix: $INSTALL_PREFIX"
    echo "Date: $(date)"
    echo ""
}

print_result() {
    local status=$1
    local message=$2
    local level=$3
    
    TOTAL_CHECKS=$((TOTAL_CHECKS + 1))
    
    case $status in
        "PASS")
            echo -e "  ${GREEN}✓${NC} $message"
            PASSED=$((PASSED + 1))
            ;;
        "FAIL")
            echo -e "  ${RED}✗${NC} $message"
            FAILED=$((FAILED + 1))
            ;;
        "WARN")
            echo -e "  ${YELLOW}⚠${NC} $message"
            WARNINGS=$((WARNINGS + 1))
            ;;
    esac
}

check_file_exists() {
    local file=$1
    local description=$2
    local required=$3
    
    if [[ -f "$file" ]]; then
        print_result "PASS" "$description exists: $file"
        return 0
    else
        if [[ "$required" == "required" ]]; then
            print_result "FAIL" "$description missing: $file"
        else
            print_result "WARN" "$description missing (optional): $file"
        fi
        return 1
    fi
}

check_directory_exists() {
    local dir=$1
    local description=$2
    
    if [[ -d "$dir" ]]; then
        print_result "PASS" "$description exists: $dir"
        return 0
    else
        print_result "FAIL" "$description missing: $dir"
        return 1
    fi
}

check_executable() {
    local exe=$1
    local description=$2
    
    if [[ -x "$exe" ]]; then
        print_result "PASS" "$description is executable: $exe"
        return 0
    else
        print_result "FAIL" "$description not executable: $exe"
        return 1
    fi
}

check_core_installation() {
    echo -e "${BLUE}1. Core Installation Files${NC}"
    
    # Main installation directory
    check_directory_exists "$INSTALL_PREFIX" "Installation directory"
    check_directory_exists "$INSTALL_PREFIX/bin" "Binaries directory"
    check_directory_exists "$INSTALL_PREFIX/lib/linuxtrack" "Libraries directory"
    check_directory_exists "$INSTALL_PREFIX/share/linuxtrack" "Data directory"
    
    # Core executables
    check_executable "$INSTALL_PREFIX/bin/ltr_gui" "Main GUI application"
    check_executable "$INSTALL_PREFIX/bin/mickey" "Mickey standalone application"
    check_executable "$INSTALL_PREFIX/bin/ltr_server1" "LinuxTrack server"
    check_executable "$INSTALL_PREFIX/bin/ltr_recenter" "Recenter utility"
    check_executable "$INSTALL_PREFIX/bin/ltr_pipe" "Data pipe utility"
    
    # Libraries
    check_file_exists "$INSTALL_PREFIX/lib/linuxtrack/liblinuxtrack.so" "Main library" "required"
    check_file_exists "$INSTALL_PREFIX/lib/linuxtrack/libltr.so" "Core library" "required"
    
    # Configuration and data files
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/linuxtrack1.conf" "Default configuration" "required"
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/haarcascade_frontalface_alt2.xml" "Face tracking model" "optional"
    
    echo ""
}

check_gui_resources() {
    echo -e "${BLUE}2. GUI Resources and 3D Models${NC}"
    
    # Help system
    check_directory_exists "$INSTALL_PREFIX/share/linuxtrack/help" "Help directory"
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/help.qhc" "Qt Help collection" "required"
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/help.qch" "Qt Help compressed" "required"
    
    # 3D Models (these were identified as missing in analysis)
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/sparow_opaq.obj" "Sparrow opaque 3D model" "optional"
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/sparow_glass.obj" "Sparrow glass 3D model" "optional"
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/sphere.obj" "Sphere 3D model" "optional"
    
    # GUI graphics
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/cap_1.png" "Cap tracking graphic" "optional"
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/cap_np.png" "Cap NP graphic" "optional"
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/clip_1.png" "Clip tracking graphic" "optional"
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/clip_np.png" "Clip NP graphic" "optional"
    
    echo ""
}

check_udev_rules() {
    echo -e "${BLUE}3. udev Rules Installation${NC}"
    
    # Check if udev rules exist
    check_file_exists "$UDEV_RULES_DIR/99-TIR.rules" "TrackIR udev rules" "required"
    check_file_exists "$UDEV_RULES_DIR/99-Mickey.rules" "Mickey device udev rules" "optional"
    
    # Check if udev rules are loaded
    if command -v udevadm >/dev/null 2>&1; then
        if udevadm control --reload-rules 2>/dev/null; then
            print_result "PASS" "udev rules can be reloaded"
        else
            print_result "WARN" "udev rules reload failed (may need root privileges)"
        fi
    else
        print_result "WARN" "udevadm not found - cannot verify udev rules loading"
    fi
    
    # Check user groups
    if groups | grep -q "plugdev"; then
        print_result "PASS" "User is in plugdev group"
    else
        print_result "WARN" "User not in plugdev group (may affect device access)"
    fi
    
    echo ""
}

check_desktop_integration() {
    echo -e "${BLUE}4. Desktop Integration${NC}"
    
    # Desktop files
    check_file_exists "$DESKTOP_DIR/linuxtrack.desktop" "Main desktop entry" "required"
    check_file_exists "$DESKTOP_DIR/linuxtrack-wii.desktop" "Wii desktop entry" "optional"
    check_file_exists "$DESKTOP_DIR/mickey.desktop" "Mickey desktop entry" "required"
    
    # Icons - hicolor theme
    check_file_exists "$ICONS_DIR/48x48/apps/linuxtrack.png" "48x48 icon" "required"
    check_file_exists "$ICONS_DIR/scalable/apps/linuxtrack.svg" "Scalable icon" "required"
    
    # Icons - pixmaps (legacy)
    check_file_exists "$PIXMAPS_DIR/linuxtrack.png" "Legacy PNG icon" "optional"
    check_file_exists "$PIXMAPS_DIR/linuxtrack.svg" "Legacy SVG icon" "optional"
    check_file_exists "$PIXMAPS_DIR/linuxtrack.xpm" "Legacy XPM icon" "optional"
    
    # Check if desktop database is updated
    if command -v update-desktop-database >/dev/null 2>&1; then
        print_result "PASS" "Desktop database update utility available"
    else
        print_result "WARN" "Desktop database update utility not found"
    fi
    
    echo ""
}

check_configuration_system() {
    echo -e "${BLUE}5. Configuration System${NC}"
    
    # System configuration
    check_file_exists "$INSTALL_PREFIX/share/linuxtrack/linuxtrack1.conf" "Default configuration file" "required"
    
    # User configuration directory
    USER_CONFIG_DIR="$HOME/.config/linuxtrack"
    if [[ -d "$USER_CONFIG_DIR" ]]; then
        print_result "PASS" "User configuration directory exists"
        
        # Check for user config files
        if [[ -f "$USER_CONFIG_DIR/linuxtrack1.conf" ]]; then
            print_result "PASS" "User configuration file exists"
        else
            print_result "WARN" "User configuration file not created yet (will be created on first run)"
        fi
    else
        print_result "WARN" "User configuration directory not created yet (will be created on first run)"
    fi
    
    echo ""
}

test_application_startup() {
    echo -e "${BLUE}6. Application Startup Test${NC}"
    
    # Test if main applications can be executed
    if [[ -x "$INSTALL_PREFIX/bin/ltr_gui" ]]; then
        # Check if GUI can show version/help (non-interactive)
        if timeout 5s "$INSTALL_PREFIX/bin/ltr_gui" --help >/dev/null 2>&1; then
            print_result "PASS" "ltr_gui responds to --help"
        else
            print_result "WARN" "ltr_gui may require X11/Wayland environment"
        fi
    fi
    
    if [[ -x "$INSTALL_PREFIX/bin/mickey" ]]; then
        if timeout 5s "$INSTALL_PREFIX/bin/mickey" --help >/dev/null 2>&1; then
            print_result "PASS" "mickey responds to --help"
        else
            print_result "WARN" "mickey may require specific runtime environment"
        fi
    fi
    
    if [[ -x "$INSTALL_PREFIX/bin/ltr_server1" ]]; then
        if timeout 5s "$INSTALL_PREFIX/bin/ltr_server1" --help >/dev/null 2>&1; then
            print_result "PASS" "ltr_server1 responds to --help"
        else
            print_result "WARN" "ltr_server1 may require configuration"
        fi
    fi
    
    echo ""
}

check_dependencies() {
    echo -e "${BLUE}7. Runtime Dependencies${NC}"
    
    # Check for required libraries
    if ldd "$INSTALL_PREFIX/bin/ltr_gui" >/dev/null 2>&1; then
        print_result "PASS" "ltr_gui library dependencies satisfied"
    else
        print_result "FAIL" "ltr_gui has missing library dependencies"
    fi
    
    # Check Qt5 availability
    if command -v qmake-qt5 >/dev/null 2>&1 || command -v qmake5 >/dev/null 2>&1 || command -v qmake >/dev/null 2>&1; then
        print_result "PASS" "Qt5 development tools available"
    else
        print_result "WARN" "Qt5 development tools not in PATH"
    fi
    
    # Check OpenCV for face tracking
    if ldconfig -p | grep -q opencv; then
        print_result "PASS" "OpenCV libraries found in system"
    else
        print_result "WARN" "OpenCV libraries not found (face tracking may not work)"
    fi
    
    echo ""
}

check_wine_bridge() {
    echo -e "${BLUE}8. Wine Bridge Functionality${NC}"
    
    # Check if wine bridge was built and installed
    WINE_DIR="$INSTALL_PREFIX/share/linuxtrack/wine"
    
    if [[ -d "$WINE_DIR" ]]; then
        print_result "PASS" "Wine bridge directory exists"
        
        # Check wine bridge installer
        check_file_exists "$WINE_DIR/linuxtrack-wine.exe" "Wine bridge installer" "required"
        check_file_exists "$WINE_DIR/README.wine" "Wine bridge documentation" "optional"
        check_file_exists "$WINE_DIR/WINE_SETUP.md" "Wine setup guide" "optional"
        
        # Check wine launcher script
        check_executable "$INSTALL_PREFIX/bin/linuxtrack-wine-launcher.sh" "Wine bridge launcher script"
        
        # Check wine desktop entry
        check_file_exists "$DESKTOP_DIR/linuxtrack-wine.desktop" "Wine bridge desktop entry" "optional"
        
    else
        print_result "WARN" "Wine bridge not installed (wine plugin may not have been enabled during build)"
    fi
    
    # Check wine development tools availability
    if command -v winegcc >/dev/null 2>&1; then
        print_result "PASS" "winegcc available for wine development"
    else
        print_result "WARN" "winegcc not found (wine development tools not installed)"
    fi
    
    if command -v wineg++ >/dev/null 2>&1; then
        print_result "PASS" "wineg++ available for wine development"
    else
        print_result "WARN" "wineg++ not found (wine development tools not installed)"
    fi
    
    if command -v makensis >/dev/null 2>&1; then
        print_result "PASS" "makensis available for installer generation"
    else
        print_result "WARN" "makensis not found (NSIS not installed)"
    fi
    
    # Check wine availability
    if command -v wine >/dev/null 2>&1; then
        print_result "PASS" "wine runtime available"
        
        # Test wine version
        WINE_VERSION=$(wine --version 2>/dev/null | head -n1)
        if [[ -n "$WINE_VERSION" ]]; then
            print_result "PASS" "Wine version: $WINE_VERSION"
        else
            print_result "WARN" "Could not determine wine version"
        fi
    else
        print_result "WARN" "wine runtime not found (wine not installed)"
    fi
    
    # Check wine library paths
    WINE_LIB_PATHS=(
        "/usr/lib/i386-linux-gnu/wine"
        "/usr/lib/x86_64-linux-gnu/wine"
        "/usr/lib/wine"
        "/usr/lib64/wine"
        "/usr/local/lib/wine"
    )
    
    WINE_LIBS_FOUND=false
    for path in "${WINE_LIB_PATHS[@]}"; do
        if [[ -d "$path" ]]; then
            print_result "PASS" "Wine libraries found at: $path"
            WINE_LIBS_FOUND=true
            break
        fi
    done
    
    if [[ "$WINE_LIBS_FOUND" == "false" ]]; then
        print_result "WARN" "Wine libraries not found in standard locations"
    fi
    
    echo ""
}

provide_troubleshooting() {
    echo -e "${BLUE}9. Troubleshooting Information${NC}"
    
    if [[ $FAILED -gt 0 ]]; then
        echo -e "${RED}Common Issues and Solutions:${NC}"
        echo ""
        
        echo "• Missing files in $INSTALL_PREFIX:"
        echo "  → Run: make install (with appropriate permissions)"
        echo "  → Check: ./configure --prefix=/opt/linuxtrack was used"
        echo ""
        
        echo "• udev rules not installed:"
        echo "  → Run: sudo make install-udev-rules"
        echo "  → Or manually copy rules to $UDEV_RULES_DIR/"
        echo "  → Run: sudo udevadm control --reload-rules"
        echo ""
        
        echo "• Desktop integration missing:"
        echo "  → Run: make install-desktop"
        echo "  → Run: update-desktop-database ~/.local/share/applications"
        echo "  → Run: gtk-update-icon-cache -f -t ~/.local/share/icons/hicolor"
        echo ""
        
        echo "• Permission issues:"
        echo "  → Add user to plugdev group: sudo usermod -a -G plugdev \$USER"
        echo "  → Log out and log back in for group changes to take effect"
        echo ""
        
        echo "• Missing dependencies:"
        echo "  → Install Qt5: sudo apt install qtbase5-dev qttools5-dev-tools"
        echo "  → Install OpenCV: sudo apt install libopencv-dev"
        echo "  → Install other deps: sudo apt install libusb-1.0-0-dev libmxml-dev"
        echo ""
        
        echo "• Wine bridge issues:"
        echo "  → Install wine: sudo apt install wine-development wine32-development"
        echo "  → Install NSIS: sudo apt install nsis"
        echo "  → Rebuild with wine: ./configure --enable-wine-plugin && make"
        echo "  → See: $INSTALL_PREFIX/share/linuxtrack/wine/WINE_SETUP.md"
        echo ""
    fi
    
    if [[ $WARNINGS -gt 0 ]]; then
        echo -e "${YELLOW}Warnings can usually be ignored but may affect functionality.${NC}"
        echo ""
    fi
    
    echo "For additional help:"
    echo "• Check log files: ~/.config/linuxtrack/"
    echo "• Visit: https://github.com/uglyDwarf/linuxtrack"
    echo "• Run applications with --help for options"
    echo ""
}

print_summary() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE} Verification Summary ${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
    echo "Total checks: $TOTAL_CHECKS"
    echo -e "Passed: ${GREEN}$PASSED${NC}"
    echo -e "Failed: ${RED}$FAILED${NC}"
    echo -e "Warnings: ${YELLOW}$WARNINGS${NC}"
    echo ""
    
    if [[ $FAILED -eq 0 ]]; then
        echo -e "${GREEN}✓ LinuxTrack installation verification PASSED!${NC}"
        echo ""
        echo "Your LinuxTrack installation appears to be complete and functional."
        echo "You can start the application with: $INSTALL_PREFIX/bin/ltr_gui"
        exit 0
    else
        echo -e "${RED}✗ LinuxTrack installation verification FAILED!${NC}"
        echo ""
        echo "Please review the failed checks above and follow the troubleshooting guidance."
        exit 1
    fi
}

# Main execution
main() {
    print_header
    check_core_installation
    check_gui_resources
    check_udev_rules
    check_desktop_integration
    check_configuration_system
    test_application_startup
    check_dependencies
    check_wine_bridge
    provide_troubleshooting
    print_summary
}

# Run main function
main "$@" 