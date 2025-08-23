#!/usr/bin/env bash
# Debug script for TrackIR device detection in AppImage

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

print_status "TrackIR Device Detection Debug"
print_status "================================="

APPDIR="$PROJECT_ROOT/AppDir_v2"

if [[ ! -d "$APPDIR" ]]; then
    die "AppDir not found: $APPDIR"
fi

# Check if TrackIR libraries exist
print_status "Checking TrackIR libraries..."
for lib in usr/lib/linuxtrack/libtir.so usr/lib/linuxtrack/libltusb1.so; do
    if [[ -f "$APPDIR/$lib" ]]; then
        print_success "Found: $lib"

        # Check library dependencies
        print_status "Dependencies for $lib:"
        LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack" ldd "$APPDIR/$lib" 2>/dev/null | grep -v "linux-vdso" | while read line; do
            echo "  $line"
        done
    else
        print_error "Missing: $lib"
    fi
done

# Check USB libraries
print_status "Checking USB libraries..."
for lib in libusb-1.0.so.0 libudev.so.1; do
    if [[ -f "$APPDIR/usr/lib/$lib" ]]; then
        print_success "Found: $lib"
    else
        print_error "Missing: $lib"
    fi
done

# Test library loading
print_status "Testing library loading..."
if [[ -f "$APPDIR/usr/lib/linuxtrack/libtir.so" ]]; then
    print_status "Attempting to load libtir.so..."
    export LD_LIBRARY_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/linuxtrack"

    # Try to dlopen the library (this simulates what the GUI does)
    python3 -c "
import ctypes
try:
    lib = ctypes.CDLL('$APPDIR/usr/lib/linuxtrack/libtir.so')
    print('SUCCESS: libtir.so loaded successfully')
except Exception as e:
    print('ERROR: Failed to load libtir.so:', str(e))
" 2>/dev/null || print_warning "Python test failed, trying direct test"

    # Check if ltr_int_tir_found function exists
    nm "$APPDIR/usr/lib/linuxtrack/libtir.so" 2>/dev/null | grep ltr_int_tir_found >/dev/null
    if [[ $? -eq 0 ]]; then
        print_success "ltr_int_tir_found function found in libtir.so"
    else
        print_error "ltr_int_tir_found function NOT found in libtir.so"
    fi
else
    print_error "Cannot test libtir.so - file not found"
fi

# Check environment setup
print_status "Checking AppRun environment setup..."
if [[ -f "$APPDIR/AppRun" ]]; then
    print_status "AppRun script found - checking content..."
    grep -n "LD_LIBRARY_PATH" "$APPDIR/AppRun" || print_warning "LD_LIBRARY_PATH not set in AppRun"
    grep -n "QT_DISABLE_VERSION_CHECK" "$APPDIR/AppRun" || print_warning "Qt isolation not configured in AppRun"
else
    print_error "AppRun script not found"
fi

print_status "Debug complete. Check the output above for issues."
