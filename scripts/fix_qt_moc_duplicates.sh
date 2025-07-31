#!/bin/bash

# Fix Qt MOC duplicate definitions by removing #include "moc_*.cpp" lines
# These are causing duplicate symbol definitions during linking

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
QT_GUI_DIR="$PROJECT_ROOT/src/qt_gui"

echo "=== Fixing Qt MOC Duplicate Definitions ==="
echo "Removing #include \"moc_*.cpp\" lines from source files"
echo "Qt GUI directory: $QT_GUI_DIR"
echo

# Find all .cpp files in qt_gui directory
find "$QT_GUI_DIR" -name "*.cpp" -type f | while read -r file; do
    echo "Processing: $(basename "$file")"
    
    # Create a backup
    cp "$file" "$file.backup"
    
    # Remove lines that include moc_*.cpp files
    sed -i '/#include "moc_.*\.cpp"/d' "$file"
    
    echo "  ✅ Fixed"
done

echo
echo "=== Fix Complete ==="
echo "All #include \"moc_*.cpp\" lines have been removed from source files."
echo "The MOC files will be compiled separately by the Makefile."
echo
echo "You can now try building the Qt GUI again." 