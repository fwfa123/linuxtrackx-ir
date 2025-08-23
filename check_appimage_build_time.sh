#!/bin/bash

echo "=== LinuxTrack-X-IR AppImage Build Time Analysis ==="
echo

# Find AppImage files
APPIMAGE_FILES=$(find . -name "*.AppImage" -type f)

if [ -z "$APPIMAGE_FILES" ]; then
    echo "❌ No AppImage files found in current directory"
    exit 1
fi

for appimage in $APPIMAGE_FILES; do
    echo "📦 Analyzing: $appimage"
    echo "   ─────────────────────────────────"

    # 1. File system timestamps
    echo "1. File System Timestamps:"
    stat "$appimage" | grep -E "(File|Size|Access|Modify|Change|Birth)" | sed 's/^/   /'

    echo

    # 2. AppImage internal structure (if it's a valid AppImage)
    echo "2. AppImage Internal Analysis:"

    # Check if it's a valid AppImage by looking for the magic bytes
    if file "$appimage" | grep -q "AppImage"; then
        echo "   ✅ Valid AppImage format detected"

        # Extract some internal timestamps
        echo "3. Internal File Timestamps (sample):"
        # Mount the AppImage and check some key files
        if command -v fuse2fs &> /dev/null; then
            # Try to mount and check timestamps
            TMPDIR=$(mktemp -d)
            if mount "$appimage" "$TMPDIR" 2>/dev/null; then
                echo "   Mounted AppImage temporarily..."
                echo "   Key file timestamps:"
                find "$TMPDIR" -name "*.so*" -o -name "ltr_gui" -o -name "AppRun*" | head -5 | while read file; do
                    if [ -f "$file" ]; then
                        echo "   $(basename "$file"): $(stat -c '%y' "$file" | cut -d'.' -f1)"
                    fi
                done
                umount "$TMPDIR"
                rmdir "$TMPDIR"
            else
                echo "   ⚠️  Cannot mount AppImage (missing fusermount or permissions)"
            fi
        else
            echo "   ⚠️  fuse2fs not available for detailed analysis"
        fi
    else
        echo "   ❌ Not a valid AppImage format"
    fi

    echo

    # 3. Build context from current directory
    echo "4. Build Context:"
    if [ -d "AppDir_v2" ]; then
        echo "   ✅ AppDir_v2 exists (build directory)"
        if [ -f "AppDir_v2/usr/bin/ltr_gui" ]; then
            echo "   ltr_gui build time: $(stat -c '%y' AppDir_v2/usr/bin/ltr_gui | cut -d'.' -f1)"
        fi
    else
        echo "   ❌ AppDir_v2 missing (not a build directory)"
    fi

    echo

    # 4. Git information if available
    echo "5. Git Context:"
    if git log -1 --oneline 2>/dev/null; then
        echo "   Latest commit: $(git log -1 --oneline)"
        echo "   Commit time: $(git log -1 --format=%cd --date=local)"
    else
        echo "   ⚠️  Not in a git repository or no git history"
    fi

    echo "   ─────────────────────────────────"
    echo
done

echo "=== Analysis Complete ==="
echo
echo "💡 Tips for verifying fresh builds:"
echo "   - Look for timestamps within the last few minutes/hours"
echo "   - Check that internal file timestamps match the AppImage modification time"
echo "   - Verify AppDir_v2 exists and has recent build artifacts"
echo "   - Check git commit history for recent build activity"
