#!/bin/bash

# LinuxTrack Library Path Fix Script
# This script helps fix library path issues for installed applications

echo "=== LinuxTrack Library Path Fix ==="
echo "This script helps fix library path issues for installed applications"
echo

# Check if we're running as root
if [ "$EUID" -ne 0 ]; then
    echo "This script needs to be run as root (use sudo)"
    exit 1
fi

# Function to add library path to ld.so.conf.d
add_library_path() {
    local lib_path="$1"
    local conf_file="/etc/ld.so.conf.d/linuxtrack.conf"
    
    echo "Adding library path: $lib_path"
    echo "$lib_path" | sudo tee "$conf_file" > /dev/null
    
    # Update library cache
    sudo ldconfig
    
    echo "Library path added and cache updated"
}

# Function to create symlinks for missing libraries
create_symlinks() {
    local install_dir="$1"
    local lib_dir="$install_dir/lib/linuxtrack"
    
    if [ -d "$lib_dir" ]; then
        echo "Creating symlinks for libraries in $lib_dir"
        
        # Create symlinks for each .so file
        for lib in "$lib_dir"/*.so*; do
            if [ -f "$lib" ] && [[ "$lib" != *.la ]]; then
                lib_name=$(basename "$lib")
                echo "  Creating symlink for $lib_name"
                sudo ln -sf "$lib" "/usr/lib/$lib_name"
            fi
        done
        
        echo "Library symlinks created"
    else
        echo "Warning: Library directory not found: $lib_dir"
    fi
}

# Main script logic
echo "Choose an option:"
echo "1. Add library path to system (recommended)"
echo "2. Create library symlinks (alternative)"
echo "3. Both"
echo "4. Exit"
echo

read -p "Enter your choice (1-4): " choice

case $choice in
    1)
        read -p "Enter the installation directory (e.g., /opt): " install_dir
        if [ -n "$install_dir" ]; then
            add_library_path "$install_dir/lib/linuxtrack"
        fi
        ;;
    2)
        read -p "Enter the installation directory (e.g., /opt): " install_dir
        if [ -n "$install_dir" ]; then
            create_symlinks "$install_dir"
        fi
        ;;
    3)
        read -p "Enter the installation directory (e.g., /opt): " install_dir
        if [ -n "$install_dir" ]; then
            add_library_path "$install_dir/lib/linuxtrack"
            create_symlinks "$install_dir"
        fi
        ;;
    4)
        echo "Exiting..."
        exit 0
        ;;
    *)
        echo "Invalid choice. Exiting..."
        exit 1
        ;;
esac

echo
echo "=== Fix Complete ==="
echo "You may need to restart your terminal or run 'source ~/.bashrc' for changes to take effect." 