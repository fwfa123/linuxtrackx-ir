#!/bin/bash

# LinuxTrack Wine Bridge Symlink Creator for Arch Linux
# This script creates the symlinks that check_data.exe would create
# Bypasses the need for wine bridge executables to work on Arch Linux

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to find wine prefix
find_wine_prefix() {
    if [ -n "$WINEPREFIX" ]; then
        echo "$WINEPREFIX"
    elif [ -d "$HOME/.wine" ]; then
        echo "$HOME/.wine"
    else
        echo "$HOME/.wine"
    fi
}

# Function to create symlinks
create_symlinks() {
    local wine_prefix="$1"
    local linuxtrack_dir="$wine_prefix/drive_c/Program Files (x86)/Linuxtrack"
    local config_dir="$HOME/.config/linuxtrack/tir_firmware"
    
    print_status "Creating wine bridge symlinks..."
    
    # Check if wine bridge directory exists
    if [ ! -d "$linuxtrack_dir" ]; then
        print_error "Wine bridge directory not found: $linuxtrack_dir"
        print_status "Please install the wine bridge first using the NSIS installer"
        return 1
    fi
    
    # Check if firmware directory exists
    if [ ! -d "$config_dir" ]; then
        print_error "TrackIR firmware directory not found: $config_dir"
        print_status "Please extract TrackIR firmware using the LinuxTrack GUI first"
        return 1
    fi
    
    # Create symlinks
    cd "$linuxtrack_dir"
    
    # Create TIRViews.dll symlink
    if [ -f "$config_dir/TIRViews.dll" ]; then
        ln -sf "$config_dir/TIRViews.dll" "TIRViews.dll"
        print_success "Created TIRViews.dll symlink"
    else
        print_warning "TIRViews.dll not found in firmware directory"
    fi
    
    # Create MFC library symlink (try MFC140 first, then MFC42)
    if [ -f "$config_dir/mfc140u.dll" ]; then
        ln -sf "$config_dir/mfc140u.dll" "mfc140u.dll"
        print_success "Created mfc140u.dll symlink"
    elif [ -f "$config_dir/mfc42u.dll" ]; then
        ln -sf "$config_dir/mfc42u.dll" "mfc42u.dll"
        print_success "Created mfc42u.dll symlink"
    else
        print_warning "No MFC library found in firmware directory"
    fi
    
    print_success "Wine bridge symlinks created successfully"
}

# Function to verify symlinks
verify_symlinks() {
    local wine_prefix="$1"
    local linuxtrack_dir="$wine_prefix/drive_c/Program Files (x86)/Linuxtrack"
    
    print_status "Verifying symlinks..."
    
    if [ -d "$linuxtrack_dir" ]; then
        cd "$linuxtrack_dir"
        
        if [ -L "TIRViews.dll" ]; then
            print_success "TIRViews.dll symlink exists"
        else
            print_warning "TIRViews.dll symlink not found"
        fi
        
        if [ -L "mfc140u.dll" ] || [ -L "mfc42u.dll" ]; then
            print_success "MFC library symlink exists"
        else
            print_warning "MFC library symlink not found"
        fi
    else
        print_error "Wine bridge directory not found"
    fi
}

# Main function
main() {
    print_status "LinuxTrack Wine Bridge Symlink Creator for Arch Linux"
    print_status "This script creates the symlinks that check_data.exe would create"
    echo
    
    local wine_prefix=$(find_wine_prefix)
    print_status "Using wine prefix: $wine_prefix"
    
    # Create symlinks
    if create_symlinks "$wine_prefix"; then
        # Verify symlinks
        verify_symlinks "$wine_prefix"
        
        print_success "Wine bridge symlinks setup complete!"
        print_status "You can now use head tracking in Windows applications under Wine"
    else
        print_error "Failed to create wine bridge symlinks"
        exit 1
    fi
}

# Run main function
main "$@" 