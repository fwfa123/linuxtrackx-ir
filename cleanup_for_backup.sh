#!/bin/bash

# Cleanup script for linuxtrack backup preparation
# This removes all build artifacts and temporary files

echo "Cleaning up linuxtrack build artifacts for backup..."

# Remove generated Makefiles
find . -name "Makefile" -delete

# Remove config files
rm -f config.h config.log config.status

# Remove automake cache
rm -rf autom4te.cache

# Remove libtool files
find . -name "*.lo" -delete
find . -name "*.la" -delete
find . -name ".libs" -type d -exec rm -rf {} + 2>/dev/null
find . -name "_libs" -type d -exec rm -rf {} + 2>/dev/null

# Remove object files
find . -name "*.o" -delete

# Remove libraries and executables
find . -name "*.so" -delete
find . -name "*.so.*" -delete
find . -name "*.a" -delete
find . -name "*.exe" -delete
find . -name "*.dll" -delete

# Remove Qt generated files
find . -name "moc_*.cpp" -delete
find . -name "ui_*.h" -delete
find . -name "qrc_*.cpp" -delete
find . -name "*.pro.user" -delete
find . -name "*.pro.user.*" -delete

# Remove backup files
find . -name "*~" -delete
find . -name "*.bak" -delete
find . -name "*.tmp" -delete

# Remove core dumps
find . -name "core" -delete
find . -name "*.core" -delete

# Remove any remaining build artifacts in lib directory
rm -rf lib/linuxtrack/*.so*

echo "Cleanup completed! The directory is now ready for backup."
echo "Only source code and essential files remain." 