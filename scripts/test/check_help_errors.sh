#!/bin/bash

echo "=== Checking for SQLite Database Driver Errors ==="
echo "Running LinuxTrack AppImage and monitoring for errors..."

# Run the AppImage and capture all output
echo "Starting AppImage..."
./LinuxTrack-x86_64.AppImage 2>&1 | tee /tmp/appimage_output.log &

# Wait a moment for the app to start
sleep 3

# Check if the process is still running
if pgrep -f "LinuxTrack-x86_64.AppImage" > /dev/null; then
    echo "✅ AppImage started successfully"
    
    # Wait a bit more for help system to initialize
    sleep 5
    
    # Check the log for SQLite errors
    echo ""
    echo "=== Checking for SQLite Errors ==="
    if grep -i "sqlite" /tmp/appimage_output.log; then
        echo "❌ SQLite errors found in output!"
        grep -i "sqlite" /tmp/appimage_output.log
    else
        echo "✅ No SQLite errors found in output"
    fi
    
    echo ""
    echo "=== Checking for Database Driver Errors ==="
    if grep -i "database" /tmp/appimage_output.log; then
        echo "❌ Database errors found in output!"
        grep -i "database" /tmp/appimage_output.log
    else
        echo "✅ No database errors found in output"
    fi
    
    echo ""
    echo "=== Checking for Help System Errors ==="
    if grep -i "help" /tmp/appimage_output.log | grep -i "error\|fail"; then
        echo "❌ Help system errors found in output!"
        grep -i "help" /tmp/appimage_output.log | grep -i "error\|fail"
    else
        echo "✅ No help system errors found in output"
    fi
    
    echo ""
    echo "=== Full AppImage Output ==="
    cat /tmp/appimage_output.log
    
    # Kill the AppImage
    pkill -f "LinuxTrack-x86_64.AppImage"
    sleep 2
else
    echo "❌ AppImage failed to start"
    echo "=== Full Output ==="
    cat /tmp/appimage_output.log
fi

# Cleanup
rm -f /tmp/appimage_output.log

echo ""
echo "Test completed!" 