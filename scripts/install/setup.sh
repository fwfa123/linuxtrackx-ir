#!/bin/bash

# LinuxTrack Installation Script
# Handles installation and system configuration

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
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

# Check if running as root for system-wide installation
check_privileges() {
    if [ "$EUID" -eq 0 ]; then
        INSTALL_PREFIX="/usr/local"
        SYSTEM_INSTALL=true
    else
        INSTALL_PREFIX="$HOME/.local"
        SYSTEM_INSTALL=false
        print_warning "Installing to user directory: $INSTALL_PREFIX"
        print_warning "For system-wide installation, run with sudo"
    fi
}

# Install udev rules (requires root)
install_udev_rules() {
    if [ "$SYSTEM_INSTALL" = true ]; then
        if [ -f src/99-TIR.rules ]; then
            print_status "Installing udev rules..."
            cp src/99-TIR.rules /lib/udev/rules.d/
            udevadm control --reload-rules
            udevadm trigger
            print_success "Udev rules installed"
        else
            print_warning "Udev rules file not found"
        fi
    else
        print_warning "Cannot install udev rules without root privileges"
        print_warning "To install udev rules manually:"
        print_warning "  sudo cp src/99-TIR.rules /lib/udev/rules.d/"
        print_warning "  sudo udevadm control --reload-rules"
        print_warning "  sudo udevadm trigger"
    fi
}

# Create and configure groups
setup_groups() {
    if [ "$SYSTEM_INSTALL" = true ]; then
        # Create linuxtrack group if it doesn't exist
        if ! getent group linuxtrack > /dev/null 2>&1; then
            print_status "Creating linuxtrack group..."
            groupadd linuxtrack
        fi
        
        # Add current user to required groups
        if [ -n "$SUDO_USER" ]; then
            USERNAME="$SUDO_USER"
        else
            USERNAME="$USER"
        fi
        
        print_status "Adding user $USERNAME to required groups..."
        usermod -a -G plugdev,linuxtrack,input "$USERNAME"
        print_success "User added to groups"
        print_warning "User must log out and log back in for group changes to take effect"
    else
        print_warning "Cannot configure groups without root privileges"
        print_warning "To configure groups manually:"
        print_warning "  sudo groupadd linuxtrack  # if group doesn't exist"
        print_warning "  sudo usermod -a -G plugdev,linuxtrack,input $USER"
    fi
}

# Create desktop entry
create_desktop_entry() {
    local desktop_dir
    if [ "$SYSTEM_INSTALL" = true ]; then
        desktop_dir="/usr/share/applications"
    else
        desktop_dir="$HOME/.local/share/applications"
        mkdir -p "$desktop_dir"
    fi
    
    print_status "Creating desktop entry..."
    cat > "$desktop_dir/linuxtrack.desktop" << EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=LinuxTrack
Comment=Head tracking for Linux
Exec=$INSTALL_PREFIX/bin/ltr_gui
Icon=linuxtrack
Terminal=false
Categories=Game;Utility;
StartupNotify=true
EOF
    
    if [ "$SYSTEM_INSTALL" = true ]; then
        update-desktop-database "$desktop_dir" 2>/dev/null || true
    else
        update-desktop-database "$desktop_dir" 2>/dev/null || true
    fi
    
    print_success "Desktop entry created"
}

# Create uninstall script
create_uninstall_script() {
    local uninstall_script="$INSTALL_PREFIX/bin/linuxtrack-uninstall"
    
    print_status "Creating uninstall script..."
    cat > "$uninstall_script" << 'EOF'
#!/bin/bash

# LinuxTrack Uninstall Script

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}Uninstalling LinuxTrack...${NC}"

# Remove installed files
if [ -f /usr/local/bin/ltr_gui ]; then
    PREFIX="/usr/local"
elif [ -f "$HOME/.local/bin/ltr_gui" ]; then
    PREFIX="$HOME/.local"
else
    echo -e "${RED}LinuxTrack installation not found${NC}"
    exit 1
fi

echo "Removing files from $PREFIX..."
rm -f "$PREFIX/bin/ltr_gui"
rm -f "$PREFIX/bin/ltr_server1"
rm -f "$PREFIX/bin/ltr_recenter"
rm -f "$PREFIX/bin/ltr_pipe"
rm -f "$PREFIX/bin/ltr_extractor"
rm -f "$PREFIX/bin/osc_server"
rm -rf "$PREFIX/lib/linuxtrack"
rm -rf "$PREFIX/lib32/linuxtrack"

# Remove desktop entry
rm -f /usr/share/applications/linuxtrack.desktop
rm -f "$HOME/.local/share/applications/linuxtrack.desktop"

# Remove udev rules (if running as root)
if [ "$EUID" -eq 0 ]; then
    if [ -f /lib/udev/rules.d/99-TIR.rules ]; then
        echo "Removing udev rules..."
        rm -f /lib/udev/rules.d/99-TIR.rules
        udevadm control --reload-rules
        udevadm trigger
    fi
fi

echo -e "${GREEN}LinuxTrack uninstalled successfully${NC}"
echo -e "${YELLOW}Note: User groups and configuration files in ~/.config/linuxtrack were not removed${NC}"
EOF
    
    chmod +x "$uninstall_script"
    print_success "Uninstall script created at $uninstall_script"
}

# Print post-installation instructions
print_post_install() {
    print_success "LinuxTrack installation completed!"
    echo ""
    print_status "Installation Summary:"
    print_status "  Prefix: $INSTALL_PREFIX"
    print_status "  Executable: $INSTALL_PREFIX/bin/ltr_gui"
    print_status "  Libraries: $INSTALL_PREFIX/lib/linuxtrack/"
    echo ""
    
    if [ "$SYSTEM_INSTALL" = true ]; then
        print_status "System configuration completed"
    else
        print_warning "Manual system configuration required:"
        print_warning "  - Install udev rules with root privileges"
        print_warning "  - Add user to plugdev and linuxtrack groups"
    fi
    
    echo ""
    print_status "To run LinuxTrack:"
    if [ "$SYSTEM_INSTALL" = false ]; then
        print_status "  $INSTALL_PREFIX/bin/ltr_gui"
        print_status "  (Add $INSTALL_PREFIX/bin to PATH for easier access)"
    else
        print_status "  ltr_gui  (from command line)"
        print_status "  Or launch from applications menu"
    fi
    
    echo ""
    print_status "For X11 session (if using Wayland):"
    print_status "  ./run_qt5_gui_x11.sh"
    
    echo ""
    print_status "Configuration files stored in: ~/.config/linuxtrack/"
    print_status "Log files: Check GUI log viewer or ~/.config/linuxtrack/linuxtrack.log"
    
    if [ "$SYSTEM_INSTALL" = true ]; then
        echo ""
        print_warning "Please log out and log back in for group changes to take effect"
    fi
    
    echo ""
    print_status "To uninstall: $INSTALL_PREFIX/bin/linuxtrack-uninstall"
}

# Main installation function
main() {
    print_status "LinuxTrack Installation Script"
    print_status "=============================="
    
    # Check if make install has been run
    if [ ! -f src/qt_gui/ltr_gui ] && [ ! -f .libs/ltr_gui ]; then
        print_error "LinuxTrack must be built first!"
        print_error "Run: ./build.sh --clean --install"
        print_error "Or: make && make install"
        exit 1
    fi
    
    check_privileges
    
    print_status "Installing to: $INSTALL_PREFIX"
    
    # Run make install
    if [ "$SYSTEM_INSTALL" = true ]; then
        make install
    else
        make install prefix="$INSTALL_PREFIX"
    fi
    
    install_udev_rules
    setup_groups
    create_desktop_entry
    create_uninstall_script
    print_post_install
}

# Check command line arguments
case "${1:-}" in
    --help|-h)
        echo "Usage: $0 [options]"
        echo "Options:"
        echo "  --help, -h     Show this help"
        echo ""
        echo "Installation:"
        echo "  sudo $0        # System-wide installation (recommended)"
        echo "  $0             # User installation"
        echo ""
        echo "Note: System-wide installation requires sudo for udev rules and group setup"
        exit 0
        ;;
    *)
        main "$@"
        ;;
esac 