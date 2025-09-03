#!/bin/bash

# LinuxTrack X-IR Post-Installation Verification Script
# This script verifies that LinuxTrack is properly installed and configured
# (Installation is now handled automatically by 'sudo make install')

echo "=== LinuxTrack X-IR Installation Verification ==="
echo ""

# Check if running as root
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root (sudo)." >&2
   exit 1
fi

# Step 1: Verify library cache
echo ""
echo "Step 1: Verifying library cache..."
if ldconfig -p | grep -q "libwc.so.0"; then
    echo "✅ LinuxTrack libraries are properly registered"
else
    echo "⚠️  LinuxTrack libraries not found in cache - running ldconfig..."
    ldconfig
    sleep 1
    if ldconfig -p | grep -q "libwc.so.0"; then
        echo "✅ Library cache updated successfully"
    else
        echo "❌ Failed to register LinuxTrack libraries"
        exit 1
    fi
fi

# Step 2: Verify installation
echo ""
echo "Step 2: Verifying installation..."

# Check icons
ICONS_INSTALLED=true
if [ -f "/usr/share/icons/hicolor/48x48/apps/linuxtrack.png" ]; then
    echo "✅ Main icon installed: linuxtrack.png"
else
    echo "❌ Main icon missing: linuxtrack.png"
    ICONS_INSTALLED=false
fi

if [ -f "/usr/share/icons/hicolor/scalable/apps/linuxtrack.svg" ]; then
    echo "✅ Main scalable icon installed: linuxtrack.svg"
else
    echo "❌ Main scalable icon missing: linuxtrack.svg"
    ICONS_INSTALLED=false
fi

if [ -f "/usr/share/icons/hicolor/48x48/apps/linuxtrack-wii.png" ]; then
    echo "✅ Wii icon installed: linuxtrack-wii.png"
else
    echo "⚠️  Wii icon missing: linuxtrack-wii.png (optional)"
fi

if [ -f "/usr/share/icons/hicolor/scalable/apps/linuxtrack-wii.svg" ]; then
    echo "✅ Wii scalable icon installed: linuxtrack-wii.svg"
else
    echo "⚠️  Wii scalable icon missing: linuxtrack-wii.svg (optional)"
fi

# Update icon cache if needed
if $ICONS_INSTALLED && command -v gtk-update-icon-cache >/dev/null 2>&1; then
    gtk-update-icon-cache /usr/share/icons/hicolor/ 2>/dev/null || true
    echo "✅ Icon cache updated"
fi

# Step 3: Verify desktop files
echo ""
echo "Step 3: Verifying desktop files..."

# Check desktop files
DESKTOP_INSTALLED=true
if [ -f "/usr/share/applications/linuxtrack.desktop" ]; then
    echo "✅ Desktop file installed: linuxtrack.desktop"
else
    echo "❌ Desktop file missing: linuxtrack.desktop"
    DESKTOP_INSTALLED=false
fi

if [ -f "/usr/share/applications/linuxtrack-wii.desktop" ]; then
    echo "✅ Wii desktop file installed: linuxtrack-wii.desktop"
else
    echo "⚠️  Wii desktop file missing: linuxtrack-wii.desktop (optional)"
fi

# Update desktop database if needed
if $DESKTOP_INSTALLED && command -v update-desktop-database >/dev/null 2>&1; then
    update-desktop-database /usr/share/applications 2>/dev/null || true
    echo "✅ Desktop database updated"
fi

echo ""
if $ICONS_INSTALLED && $DESKTOP_INSTALLED; then
    echo "🎉 LinuxTrack installation verification completed successfully!"
    echo "✅ All components are properly installed and configured"
    echo ""
    echo "Launch options:"
    echo "  Main GUI: ltr_gui"
    echo "  Alternative: mickey"
    echo "  Wii Server: wii_server"
    echo ""
    echo "If using Wayland, use: QT_QPA_PLATFORM=xcb ltr_gui"
else
    echo "⚠️  Some components may be missing - check the output above"
    echo "ℹ️  If this is a fresh installation, the automated installation should have handled everything"
    echo "ℹ️  You can manually run: sudo ldconfig && sudo make install (from build directory)"
fi
echo ""
