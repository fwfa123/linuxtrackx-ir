#!/bin/bash

echo "=== Quick AppImage Build Check ==="
echo

# Find the main LinuxTrack AppImage (not build tools)
MAIN_APPIMAGE=$(find . -name "LinuxTrack-X-IR*.AppImage" -type f | head -1)

if [ -z "$MAIN_APPIMAGE" ]; then
    echo "❌ No LinuxTrack AppImage found"
    exit 1
fi

echo "📦 Main AppImage: $MAIN_APPIMAGE"
echo

# Quick timestamp check
echo "⏰ Build Timestamps:"
echo "   AppImage created: $(stat -c '%w' "$MAIN_APPIMAGE" | cut -d'.' -f1)"
echo "   AppImage modified: $(stat -c '%y' "$MAIN_APPIMAGE" | cut -d'.' -f1)"

# Check ltr_gui build time if AppDir exists
if [ -f "AppDir_v2/usr/bin/ltr_gui" ]; then
    echo "   ltr_gui built: $(stat -c '%y' AppDir_v2/usr/bin/ltr_gui | cut -d'.' -f1)"
fi

# Check git info
if git log -1 --oneline &>/dev/null; then
    echo "   Latest commit: $(git log -1 --oneline)"
fi

echo

# Compare with current time
CURRENT_TIME=$(date +%s)
APPIMAGE_TIME=$(stat -c '%Y' "$MAIN_APPIMAGE")
DIFF_MINUTES=$(( (CURRENT_TIME - APPIMAGE_TIME) / 60 ))

if [ $DIFF_MINUTES -lt 60 ]; then
    echo "✅ AppImage built recently ($DIFF_MINUTES minutes ago)"
elif [ $DIFF_MINUTES -lt 1440 ]; then
    HOURS=$((DIFF_MINUTES / 60))
    echo "🟡 AppImage built $HOURS hours ago"
else
    DAYS=$((DIFF_MINUTES / 1440))
    echo "⚠️  AppImage built $DAYS days ago"
fi

echo

# Check if it looks like a fresh build
if [ -d "AppDir_v2" ] && [ $DIFF_MINUTES -lt 60 ]; then
    echo "🎉 This looks like a fresh build!"
else
    echo "💡 Tip: Rebuild if you need the latest changes"
fi
