#!/bin/bash
# Repository cleanup script for LinuxTrack X-IR
# This script removes build artifacts and prepares the repo for archiving

echo "=== LinuxTrack X-IR Repository Cleanup for Archiving ==="
echo ""

# Function to safely remove files from git tracking and disk
remove_from_git_and_disk() {
    local pattern="$1"
    local description="$2"
    
    echo "Removing $description..."
    files=$(git ls-files | grep -E "$pattern" || true)
    if [ -n "$files" ]; then
        echo "  Found files:"
        echo "$files" | sed 's/^/    /'
        echo "$files" | xargs -r git rm --cached
        echo "$files" | xargs -r rm -f
        echo "  ✅ Removed from git tracking and disk"
    else
        echo "  ✅ No files found"
    fi
    echo ""
}

# Function to remove directories from git tracking and disk
remove_dir_from_git_and_disk() {
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
    
    if [ -d "$dir" ]; then
        rm -rf "$dir"
        echo "  ✅ Removed from disk"
    fi
    echo ""
}

# Function to remove untracked files matching pattern
remove_untracked_files() {
    local pattern="$1"
    local description="$2"
    
    echo "Removing untracked $description..."
    files=$(find . -name "$pattern" -type f 2>/dev/null | grep -v ".git/" || true)
    if [ -n "$files" ]; then
        echo "  Found files:"
        echo "$files" | sed 's/^/    /'
        echo "$files" | xargs -r rm -f
        echo "  ✅ Removed from disk"
    else
        echo "  ✅ No files found"
    fi
    echo ""
}

# Function to remove untracked directories
remove_untracked_dirs() {
    local dir_pattern="$1"
    local description="$2"
    
    echo "Removing untracked $description..."
    dirs=$(find . -name "$dir_pattern" -type d 2>/dev/null | grep -v ".git/" || true)
    if [ -n "$dirs" ]; then
        echo "  Found directories:"
        echo "$dirs" | sed 's/^/    /'
        echo "$dirs" | xargs -r rm -rf
        echo "  ✅ Removed from disk"
    else
        echo "  ✅ No directories found"
    fi
    echo ""
}

# Remove build artifacts from git tracking and disk
remove_from_git_and_disk "\.(bin|obj)$" "binary object files"
remove_from_git_and_disk "\.(log|status|h\.in~)$" "autotools generated files"
remove_from_git_and_disk "^(config\.(log|status|h|h\.in~)|Makefile|configure|aclocal\.m4|libtool|ltmain\.sh|stamp-h1|ylwrap|compile|install-sh|missing|ar-lib|depcomp)$" "autotools build files"

# Remove AppImage build artifacts (but keep the final AppImages)
echo "Removing AppImage build artifacts (keeping final AppImages)..."
if [ -d "AppDir_phase4" ]; then
    git rm --cached -r "AppDir_phase4" 2>/dev/null || true
    rm -rf "AppDir_phase4"
    echo "  ✅ Removed AppDir from git tracking and disk"
fi

# Remove temporary test scripts (keep essential ones)
echo "Removing temporary test scripts..."
temp_scripts=("test_wayland_x11.sh" "test_theme_integration.sh")
for script in "${temp_scripts[@]}"; do
    if [ -f "$script" ]; then
        git rm --cached "$script" 2>/dev/null || true
        rm -f "$script"
        echo "  ✅ Removed $script from git tracking and disk"
    fi
done

# Remove analysis files
remove_from_git_and_disk "(clang_tidy_results\.txt|cppcheck_results\.xml)$" "analysis result files"

# Remove Cursor IDE files
remove_dir_from_git_and_disk ".cursor" "Cursor IDE files"
remove_dir_from_git_and_disk "linuxtrack new.cursor" "Cursor IDE backup files"

# Remove autom4te cache
remove_dir_from_git_and_disk "autom4te.cache" "autotools cache"

# Remove build directory
remove_dir_from_git_and_disk "build" "build directory"

# Remove any remaining build artifacts
echo "Removing any remaining build artifacts..."
git ls-files | grep -E "\.(o|so|a|la|lo|exe|dll|tmp|bak|orig|rej|swp|swo|core|dump|crash|prof|gcov|gcda|gcno)$" | xargs -r git rm --cached
git ls-files | grep -E "\.(o|so|a|la|lo|exe|dll|tmp|bak|orig|rej|swp|swo|core|dump|crash|prof|gcov|gcda|gcno)$" | xargs -r rm -f

# Remove untracked build artifacts
remove_untracked_files "*.o" "object files"
remove_untracked_files "*.lo" "libtool object files"
remove_untracked_files "*.la" "libtool archive files"
remove_untracked_files "*.a" "static library files"
remove_untracked_files "*.so" "shared library files"
remove_untracked_files "*.exe" "executable files"
remove_untracked_files "*.dll" "dynamic library files"
remove_untracked_files "*.tmp" "temporary files"
remove_untracked_files "*.bak" "backup files"
remove_untracked_files "*.orig" "original files"
remove_untracked_files "*.rej" "reject files"
remove_untracked_files "*.swp" "swap files"
remove_untracked_files "*.swo" "swap files"
remove_untracked_files "core" "core dump files"
remove_untracked_files "*.dump" "dump files"
remove_untracked_files "*.crash" "crash files"
remove_untracked_files "*.prof" "profiling files"
remove_untracked_files "*.gcov" "coverage files"
remove_untracked_files "*.gcda" "coverage data files"
remove_untracked_files "*.gcno" "coverage note files"

# Remove untracked build directories
remove_untracked_dirs ".libs" "libtool libraries directory"
remove_untracked_dirs ".deps" "dependency files directory"
remove_untracked_dirs "dist" "distribution directory"
remove_untracked_dirs "tmp" "temporary directory"
remove_untracked_dirs "temp" "temporary directory"
remove_untracked_dirs ".cache" "cache directory"

# Remove Qt generated files
remove_untracked_files "*.gch" "precompiled header files"
remove_untracked_files "ui_*.h" "Qt UI generated headers"
remove_untracked_files "moc_*.h" "Qt MOC generated headers"
remove_untracked_files "moc_*.cpp" "Qt MOC generated source files"
remove_untracked_files "qrc_*.cpp" "Qt resource generated files"
remove_untracked_files "moc_predefs.h" "Qt MOC predefs"
remove_untracked_files ".qmake.stash" "qmake stash files"
remove_untracked_files "*.pro.user" "Qt project user files"
remove_untracked_files "*.qch" "Qt help files"
remove_untracked_files "*.qhc" "Qt help collection files"

# Remove executables
remove_untracked_files "ltr_gui" "GUI executable"
remove_untracked_files "ltr_gui_qt5_debug" "Qt5 debug GUI executable"
remove_untracked_files "ltr_gui_qt5_standalone_debug" "Qt5 standalone debug GUI executable"
remove_untracked_files "ltr_server*" "server executables"
remove_untracked_files "ltr_extractor" "extractor executable"
remove_untracked_files "ltr_pipe" "pipe executable"
remove_untracked_files "ltr_recenter" "recenter executable"
remove_untracked_files "osc_server" "OSC server executable"
remove_untracked_files "test_*" "test executables"

# Remove log files
remove_untracked_files "*.log" "log files"
remove_untracked_files "gui_output.log" "GUI output log"

# Remove backup files
remove_untracked_files "*~" "backup files"
remove_untracked_files "*.bak" "backup files"

# Remove OS specific files
remove_untracked_files ".DS_Store" "macOS files"
remove_untracked_files ".DS_Store?" "macOS files"
remove_untracked_files "._*" "macOS files"
remove_untracked_files ".Spotlight-V100" "macOS files"
remove_untracked_files ".Trashes" "macOS files"
remove_untracked_files "ehthumbs.db" "Windows files"
remove_untracked_files "Thumbs.db" "Windows files"

# Remove distribution packages
remove_untracked_files "*.deb" "Debian packages"
remove_untracked_files "*.rpm" "RPM packages"
remove_untracked_files "*.tar.gz" "tar.gz archives"
remove_untracked_files "*.tar.bz2" "tar.bz2 archives"
remove_untracked_files "*.tar.xz" "tar.xz archives"
remove_untracked_files "*.zip" "zip archives"

# Remove Wine bridge build artifacts
remove_untracked_files "*.ico" "icon files"
remove_untracked_files "*.nsi" "NSIS script files"

# Remove blob files
remove_untracked_files "blob_*.bin" "blob files"

# Clean up any empty directories
echo "Removing empty directories..."
find . -type d -empty -delete 2>/dev/null || true

echo ""
echo "=== Cleanup Summary ==="
echo "✅ Removed build artifacts from git tracking and disk"
echo "✅ Removed AppImage build files (kept final AppImages)"
echo "✅ Removed temporary test scripts"
echo "✅ Removed analysis files"
echo "✅ Removed IDE files"
echo "✅ Removed autotools cache and generated files"
echo "✅ Removed Qt generated files"
echo "✅ Removed executables and libraries"
echo "✅ Removed log and backup files"
echo "✅ Removed OS specific files"
echo "✅ Removed distribution packages"
echo "✅ Removed empty directories"
echo ""
echo "AppImages preserved:"
ls -la *.AppImage 2>/dev/null || echo "  No AppImages found"
echo ""
echo "To complete the cleanup, you can now:"
echo "1. Review the changes: git status"
echo "2. Commit the cleanup: git add . && git commit -m 'Clean up repository for archiving - remove build artifacts'"
echo "3. Push to remote: git push"
echo ""
echo "Repository is now ready for archiving!" 