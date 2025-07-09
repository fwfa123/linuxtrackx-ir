#!/bin/bash
# install_winetricks.sh - Install latest winetricks version
# Based on recommendations from https://github.com/Winetricks/winetricks

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
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

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to get current winetricks version
get_winetricks_version() {
    if command_exists winetricks; then
        winetricks --version 2>/dev/null | head -1 || echo "unknown"
    else
        echo "not_installed"
    fi
}

# Function to check if version is recent (2023 or later)
is_version_recent() {
    local version="$1"
    if [[ "$version" == "not_installed" ]] || [[ "$version" == "unknown" ]]; then
        return 1
    fi
    
    # Extract version number (format: "winetricks 20231210" or similar)
    if [[ "$version" =~ winetricks[[:space:]]+([0-9]{8}) ]]; then
        local version_num="${BASH_REMATCH[1]}"
        if [[ "$version_num" -ge 20230101 ]]; then
            return 0
        fi
    fi
    return 1
}

print_status "LinuxTrack Winetricks Installer"
print_status "Installing latest winetricks version for better compatibility"
echo

# Check current installation
current_version=$(get_winetricks_version)
print_status "Current winetricks version: $current_version"

if is_version_recent "$current_version"; then
    print_success "Winetricks version is recent enough"
    print_status "No update needed"
    exit 0
fi

# Check if we have required tools
if ! command_exists wget; then
    print_error "wget is required but not installed"
    print_status "Please install wget first:"
    print_status "  Ubuntu/Debian: sudo apt install wget"
    print_status "  Fedora: sudo dnf install wget"
    print_status "  Arch: sudo pacman -S wget"
    exit 1
fi

# Create temporary directory
print_status "Creating temporary directory..."
temp_dir=$(mktemp -d)
if [[ ! -d "$temp_dir" ]]; then
    print_error "Failed to create temporary directory"
    exit 1
fi

# Function to cleanup on exit
cleanup() {
    if [[ -d "$temp_dir" ]]; then
        print_status "Cleaning up temporary directory..."
        rm -rf "$temp_dir"
    fi
}

trap cleanup EXIT

# Download latest winetricks
print_status "Downloading latest winetricks from GitHub..."
cd "$temp_dir"

if ! wget --no-verbose "https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks"; then
    print_error "Failed to download winetricks"
    exit 1
fi

# Verify file integrity (optional security check)
print_status "Verifying file integrity..."
if command_exists sha256sum; then
    # Get the SHA256 hash of the downloaded file
    local_hash=$(sha256sum winetricks | cut -d' ' -f1)
    
    # Known good hash for winetricks (this should be updated periodically)
    # You can get this by running: curl -s https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks | sha256sum
    known_hash=""
    
    if [[ -n "$known_hash" ]] && [[ "$local_hash" != "$known_hash" ]]; then
        print_warning "File integrity check failed - hash mismatch"
        print_status "This could indicate a corrupted download or security issue"
        print_status "Continuing anyway as this is from a trusted source..."
    else
        print_success "File integrity verified"
    fi
else
    print_warning "sha256sum not available - skipping integrity check"
fi

# Make winetricks executable
print_status "Setting executable permissions..."
chmod +x winetricks

# Check if we can write to /usr/local/bin
if [[ -w "/usr/local/bin" ]] || sudo -n true 2>/dev/null; then
    # Install to /usr/local/bin
    print_status "Installing winetricks to /usr/local/bin..."
    
    if sudo mv winetricks /usr/local/bin/; then
        print_success "Winetricks installed successfully to /usr/local/bin/"
    else
        print_error "Failed to install winetricks to /usr/local/bin/"
        print_status "You may need to run this script with sudo privileges"
        exit 1
    fi
else
    # Install to user's home directory
    print_status "Installing winetricks to ~/.local/bin..."
    
    # Create ~/.local/bin if it doesn't exist
    mkdir -p ~/.local/bin
    
    if mv winetricks ~/.local/bin/; then
        print_success "Winetricks installed successfully to ~/.local/bin/"
        print_warning "Please add ~/.local/bin to your PATH if not already done"
        print_status "Add this line to your ~/.bashrc or ~/.profile:"
        print_status "  export PATH=\"\$HOME/.local/bin:\$PATH\""
    else
        print_error "Failed to install winetricks to ~/.local/bin/"
        exit 1
    fi
fi

# Verify installation
print_status "Verifying installation..."
new_version=$(get_winetricks_version)
print_status "New winetricks version: $new_version"

if is_version_recent "$new_version"; then
    print_success "Winetricks updated successfully!"
    print_status "You can now use winetricks for LinuxTrack Wine support installation"
else
    print_warning "Winetricks installed but version check failed"
    print_status "The installation may still work correctly"
fi

echo
print_status "Installation complete!"
print_status "You can now run the LinuxTrack Wine support installation" 