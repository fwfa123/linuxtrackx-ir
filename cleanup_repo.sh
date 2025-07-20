#!/bin/bash
# Repository cleanup script for LinuxTrack X-IR
# This script removes tracked files that should be ignored by git

echo "=== LinuxTrack X-IR Repository Cleanup ==="
echo ""

# Function to safely remove files from git tracking
remove_from_git() {
    local pattern="$1"
    local description="$2"
    
    echo "Removing $description..."
    files=$(git ls-files | grep -E "$pattern" || true)
    if [ -n "$files" ]; then
        echo "  Found files:"
        echo "$files" | sed 's/^/    /'
        echo "$files" | xargs -r git rm --cached
        echo "  ✅ Removed from git tracking"
    else
        echo "  ✅ No files found"
    fi
    echo ""
}

# Function to remove directories from git tracking
remove_dir_from_git() {
    local dir="$1"
    local description="$2"
    
    echo "Removing $description..."
    if git ls-files | grep -q "^$dir/"; then
        echo "  Found tracked files in $dir/"
        git ls-files | grep "^$dir/" | xargs -r git rm --cached -r
        echo "  ✅ Removed from git tracking"
    else
        echo "  ✅ No tracked files found"
    fi
    echo ""
}

# Remove build artifacts
remove_from_git "\.(bin|obj)$" "binary object files"
remove_from_git "\.(log|status|h\.in~)$" "autotools generated files"
remove_from_git "^(config\.(log|status|h|h\.in~)|Makefile|configure|aclocal\.m4|libtool|ltmain\.sh|stamp-h1|ylwrap|compile|install-sh|missing|ar-lib|depcomp)$" "autotools build files"

# Remove AppImage build artifacts
echo "Removing AppImage build artifacts..."
if [ -f "LinuxTrack-x86_64.AppImage" ]; then
    git rm --cached "LinuxTrack-x86_64.AppImage" 2>/dev/null || true
    echo "  ✅ Removed AppImage from git tracking"
fi

if [ -d "AppDir_phase4" ]; then
    git rm --cached -r "AppDir_phase4" 2>/dev/null || true
    echo "  ✅ Removed AppDir from git tracking"
fi

# Remove temporary test scripts (keep essential ones)
echo "Removing temporary test scripts..."
temp_scripts=("test_wayland_x11.sh" "test_theme_integration.sh")
for script in "${temp_scripts[@]}"; do
    if [ -f "$script" ]; then
        git rm --cached "$script" 2>/dev/null || true
        echo "  ✅ Removed $script from git tracking"
    fi
done

# Remove analysis files
remove_from_git "(clang_tidy_results\.txt|cppcheck_results\.xml)$" "analysis result files"

# Remove Cursor IDE files
remove_dir_from_git ".cursor" "Cursor IDE files"
remove_dir_from_git "linuxtrack new.cursor" "Cursor IDE backup files"

# Remove autom4te cache
remove_dir_from_git "autom4te.cache" "autotools cache"

# Remove any remaining build artifacts
echo "Removing any remaining build artifacts..."
git ls-files | grep -E "\.(o|so|a|la|lo|exe|dll|tmp|bak|orig|rej|swp|swo|core|dump|crash|prof|gcov|gcda|gcno)$" | xargs -r git rm --cached

echo ""
echo "=== Cleanup Summary ==="
echo "✅ Removed build artifacts from git tracking"
echo "✅ Removed AppImage build files from git tracking"
echo "✅ Removed temporary test scripts from git tracking"
echo "✅ Removed analysis files from git tracking"
echo "✅ Removed IDE files from git tracking"
echo ""
echo "Note: Files are still present on disk but no longer tracked by git"
echo "The updated .gitignore will prevent them from being tracked in the future"
echo ""
echo "To complete the cleanup, you can now:"
echo "1. Review the changes: git status"
echo "2. Commit the cleanup: git add . && git commit -m 'Clean up repository - remove build artifacts'"
echo "3. Push to remote: git push" 