#!/bin/bash

# LinuxTrack X-IR Post-Installation Script
# This script performs necessary post-installation steps for LinuxTrack

echo "=== LinuxTrack X-IR Post-Installation Script ==="
echo ""

# Check if running as root
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root (sudo)." >&2
   exit 1
fi

# Step 1: Install icons
echo ""
echo "Step 1: Installing desktop icons..."

# Check if source directory exists
if [ ! -d "/usr/src/linuxtrack" ]; then
    echo "❌ Source directory not found at /usr/src/linuxtrack"
    echo "Please ensure LinuxTrack was built from source"
    exit 1
fi

# Create icon directories
mkdir -p /usr/share/icons/hicolor/48x48/apps/
mkdir -p /usr/share/icons/hicolor/scalable/apps/

# Install main LinuxTrack icon
if [ -f "/usr/src/linuxtrack/src/linuxtrack.png" ]; then
    cp /usr/src/linuxtrack/src/linuxtrack.png /usr/share/icons/hicolor/48x48/apps/
    echo "✅ Installed linuxtrack.png"
fi

if [ -f "/usr/src/linuxtrack/src/linuxtrack.svg" ]; then
    cp /usr/src/linuxtrack/src/linuxtrack.svg /usr/share/icons/hicolor/scalable/apps/
    echo "✅ Installed linuxtrack.svg"
fi

# Install Wii server icon
if [ -f "/usr/src/linuxtrack/src/linuxtrack-wii.png" ]; then
    cp /usr/src/linuxtrack/src/linuxtrack-wii.png /usr/share/icons/hicolor/48x48/apps/
    echo "✅ Installed linuxtrack-wii.png"
fi

if [ -f "/usr/src/linuxtrack/src/linuxtrack-wii.svg" ]; then
    cp /usr/src/linuxtrack/src/linuxtrack-wii.svg /usr/share/icons/hicolor/scalable/apps/
    echo "✅ Installed linuxtrack-wii.svg"
fi

# Update icon cache
gtk-update-icon-cache /usr/share/icons/hicolor/ 2>/dev/null
if [ $? -eq 0 ]; then
    echo "✅ Icon cache updated"
else
    echo "⚠️  Icon cache update failed (may not be critical)"
fi

# Step 3: Update desktop database
echo ""
echo "Step 3: Updating desktop database..."
update-desktop-database /usr/share/applications 2>/dev/null
if [ $? -eq 0 ]; then
    echo "✅ Desktop database updated"
else
    echo "⚠️  Desktop database update failed (may not be critical)"
fi

echo ""
echo "=== Post-Installation Complete ==="
echo ""
echo "LinuxTrack X-IR is now fully installed and configured!"
echo ""
echo "Launch options:"
echo "  Main GUI: ltr_gui"
echo "  Alternative: mickey"
echo "  Wii Server: wii_server"
echo ""
echo "If using Wayland, use: QT_QPA_PLATFORM=xcb ltr_gui"
echo ""
