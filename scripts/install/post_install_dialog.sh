#!/bin/bash

# Post-installation dialog for LinuxTrack
# Shows important instructions to users after successful installation

# Function to detect available dialog systems
detect_dialog_system() {
    if command -v zenity >/dev/null 2>&1; then
        echo "zenity"
    elif command -v kdialog >/dev/null 2>&1; then
        echo "kdialog"
    elif command -v xmessage >/dev/null 2>&1; then
        echo "xmessage"
    else
        echo "text"
    fi
}

# Function to show text-based dialog
show_text_dialog() {
    echo "================================================================================"
    echo "                    LinuxTrack Installation Complete!"
    echo "================================================================================"
    echo ""
    echo "LinuxTrack has been successfully installed!"
    echo ""
    echo "To complete the setup, please follow these steps:"
    echo ""
    echo "1. Device Setup:"
    echo "   • Unplug your tracking device (TrackIR, webcam, etc.)"
    echo "   • Wait 5 seconds"
    echo "   • Plug it back in"
    echo ""
    echo "2. System Integration:"
    echo "   • Log out and log back in, OR"
    echo "   • Restart your computer"
    echo ""
    echo "3. First Launch:"
    echo "   • Launch LinuxTrack from your Applications menu"
    echo "   • Configure your tracking device"
    echo "   • Test the tracking functionality"
    echo ""
    echo "Note: The device unplug/replug step ensures proper udev rule activation"
    echo "      for device permissions."
    echo ""
    echo "For help and documentation, visit the LinuxTrack website or check the"
    echo "installed documentation in /opt/share/doc/linuxtrack/"
    echo ""
    echo "================================================================================"
    echo ""
    read -p "Press Enter to continue..."
}

# Main execution
main() {
    # Check if we're in a graphical environment
    if [ -z "$DISPLAY" ] && [ -z "$WAYLAND_DISPLAY" ]; then
        # No graphical environment, use text mode
        show_text_dialog
        return 0
    fi

    # For now, just use text mode to keep it simple
    show_text_dialog
}

# Run main function
main "$@"
