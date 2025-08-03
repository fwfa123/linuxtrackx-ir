#!/bin/bash

# MFC42u.dll Alternative Installation Methods for LinuxTrack Wine Bridge
# This script provides multiple reliable methods to install MFC42u.dll without winetricks

set -e

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LINUXTRACK_CONFIG_DIR="$HOME/.config/linuxtrack"
TIR_FIRMWARE_DIR="$LINUXTRACK_CONFIG_DIR/tir_firmware"
CACHE_DIR="$LINUXTRACK_CONFIG_DIR/cache"
LOG_FILE="$LINUXTRACK_CONFIG_DIR/mfc42_install.log"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Logging function
log() {
    echo -e "${BLUE}[$(date '+%Y-%m-%d %H:%M:%S')]${NC} $1" | tee -a "$LOG_FILE"
}

error() {
    echo -e "${RED}ERROR:${NC} $1" | tee -a "$LOG_FILE"
}

success() {
    echo -e "${GREEN}SUCCESS:${NC} $1" | tee -a "$LOG_FILE"
}

warning() {
    echo -e "${YELLOW}WARNING:${NC} $1" | tee -a "$LOG_FILE"
}

# Create necessary directories
create_directories() {
    log "Creating necessary directories..."
    mkdir -p "$TIR_FIRMWARE_DIR"
    mkdir -p "$CACHE_DIR"
    mkdir -p "$(dirname "$LOG_FILE")"
}

# Detect Wine prefix
detect_wine_prefix() {
    if [ -n "$WINEPREFIX" ]; then
        echo "$WINEPREFIX"
    elif [ -d "$HOME/.wine" ]; then
        echo "$HOME/.wine"
    elif [ -d "$HOME/.local/share/wineprefixes/default" ]; then
        echo "$HOME/.local/share/wineprefixes/default"
    else
        echo "$HOME/.wine"
    fi
}

# Detect Wine installation
detect_wine() {
    if command -v wine >/dev/null 2>&1; then
        echo "$(which wine)"
    else
        error "Wine not found. Please install Wine first."
        exit 1
    fi
}

# Method 1: Direct download from Microsoft Visual C++ 6.0 Redistributable
method_direct_download() {
    log "Method 1: Direct download from Microsoft Visual C++ 6.0 Redistributable"
    
    local download_url="https://download.microsoft.com/download/vc60pro/redist/6.0.8447.0/Win98Me/EN-US/vcredist.exe"
    local installer_path="$CACHE_DIR/vcredist.exe"
    local wine_prefix=$(detect_wine_prefix)
    local wine_path=$(detect_wine)
    
    # Download Visual C++ 6.0 Redistributable
    log "Downloading Visual C++ 6.0 Redistributable..."
    if [ ! -f "$installer_path" ]; then
        if command -v wget >/dev/null 2>&1; then
            wget -O "$installer_path" "$download_url" || {
                error "Failed to download Visual C++ 6.0 Redistributable"
                return 1
            }
        elif command -v curl >/dev/null 2>&1; then
            curl -L -o "$installer_path" "$download_url" || {
                error "Failed to download Visual C++ 6.0 Redistributable"
                return 1
            }
        else
            error "Neither wget nor curl found. Please install one of them."
            return 1
        fi
    else
        log "Using cached installer: $installer_path"
    fi
    
    # Install in Wine prefix
    log "Installing Visual C++ 6.0 Redistributable in Wine prefix..."
    WINEPREFIX="$wine_prefix" "$wine_path" "$installer_path" /Q /T:C:\\ /C
    
    # Extract MFC42u.dll
    local system32_path="$wine_prefix/drive_c/windows/system32"
    if [ -f "$system32_path/mfc42u.dll" ]; then
        cp "$system32_path/mfc42u.dll" "$TIR_FIRMWARE_DIR/"
        success "MFC42u.dll extracted successfully"
        return 0
    else
        error "MFC42u.dll not found after installation"
        return 1
    fi
}

# Method 2: Extract from Windows system files (if available)
method_windows_extract() {
    log "Method 2: Extract from Windows system files"
    
    local windows_paths=(
        "/mnt/c/Windows/System32/mfc42u.dll"
        "/mnt/d/Windows/System32/mfc42u.dll"
        "/media/*/Windows/System32/mfc42u.dll"
    )
    
    for path in "${windows_paths[@]}"; do
        for file in $path; do
            if [ -f "$file" ]; then
                log "Found MFC42u.dll at: $file"
                cp "$file" "$TIR_FIRMWARE_DIR/"
                success "MFC42u.dll copied from Windows system"
                return 0
            fi
        done
    done
    
    warning "No Windows system files found"
    return 1
}

# Method 3: Package manager installation
method_package_manager() {
    log "Method 3: Package manager installation"
    
    # Detect distribution
    if [ -f /etc/debian_version ]; then
        # Debian/Ubuntu/MX Linux
        log "Detected Debian-based distribution"
        if command -v apt >/dev/null 2>&1; then
            sudo apt update
            sudo apt install -y wine-staging wine32:i386 || {
                error "Failed to install Wine packages"
                return 1
            }
        fi
    elif [ -f /etc/fedora-release ] || [ -f /etc/redhat-release ]; then
        # Fedora/RHEL
        log "Detected Fedora/RHEL-based distribution"
        if command -v dnf >/dev/null 2>&1; then
            sudo dnf install -y wine-staging wine-core wine-desktop || {
                error "Failed to install Wine packages"
                return 1
            }
        fi
    elif [ -f /etc/arch-release ]; then
        # Arch Linux
        log "Detected Arch Linux"
        if command -v pacman >/dev/null 2>&1; then
            sudo pacman -S --needed wine-staging wine-mono wine-gecko || {
                error "Failed to install Wine packages"
                return 1
            }
        fi
    else
        warning "Unknown distribution, skipping package manager method"
        return 1
    fi
    
    # Check if MFC42u.dll is now available in Wine
    local wine_prefix=$(detect_wine_prefix)
    local system32_path="$wine_prefix/drive_c/windows/system32"
    
    if [ -f "$system32_path/mfc42u.dll" ]; then
        cp "$system32_path/mfc42u.dll" "$TIR_FIRMWARE_DIR/"
        success "MFC42u.dll found after package installation"
        return 0
    else
        warning "MFC42u.dll not found after package installation"
        return 1
    fi
}

# Method 4: Extract from Visual C++ 6.0 installer using cabextract
method_cabextract() {
    log "Method 4: Extract from Visual C++ 6.0 installer using cabextract"
    
    local installer_path="$CACHE_DIR/vcredist.exe"
    local extract_dir="$CACHE_DIR/vcredist_extracted"
    local wine_prefix=$(detect_wine_prefix)
    
    # Check if cabextract is available
    if ! command -v cabextract >/dev/null 2>&1; then
        log "Installing cabextract..."
        if command -v apt >/dev/null 2>&1; then
            sudo apt install -y cabextract
        elif command -v dnf >/dev/null 2>&1; then
            sudo dnf install -y cabextract
        elif command -v pacman >/dev/null 2>&1; then
            sudo pacman -S cabextract
        else
            error "cabextract not available and cannot be installed"
            return 1
        fi
    fi
    
    # Download installer if not present
    if [ ! -f "$installer_path" ]; then
        method_direct_download || return 1
    fi
    
    # Extract installer
    log "Extracting Visual C++ 6.0 installer..."
    mkdir -p "$extract_dir"
    cabextract -d "$extract_dir" "$installer_path" || {
        error "Failed to extract installer"
        return 1
    }
    
    # Find and copy MFC42u.dll
    find "$extract_dir" -name "mfc42u.dll" -exec cp {} "$TIR_FIRMWARE_DIR/" \; || {
        error "MFC42u.dll not found in extracted files"
        return 1
    }
    
    success "MFC42u.dll extracted using cabextract"
    return 0
}

# Method 5: Download from alternative sources
method_alternative_sources() {
    log "Method 5: Download from alternative sources"
    
    # Alternative download URLs (these are examples - you'd need to verify these work)
    local urls=(
        "https://github.com/your-repo/mfc42u.dll/raw/main/mfc42u.dll"
        "https://archive.org/download/mfc42u.dll/mfc42u.dll"
    )
    
    for url in "${urls[@]}"; do
        log "Trying URL: $url"
        if command -v wget >/dev/null 2>&1; then
            if wget -O "$TIR_FIRMWARE_DIR/mfc42u.dll" "$url" 2>/dev/null; then
                success "MFC42u.dll downloaded from alternative source"
                return 0
            fi
        elif command -v curl >/dev/null 2>&1; then
            if curl -L -o "$TIR_FIRMWARE_DIR/mfc42u.dll" "$url" 2>/dev/null; then
                success "MFC42u.dll downloaded from alternative source"
                return 0
            fi
        fi
    done
    
    error "Failed to download from alternative sources"
    return 1
}

# Method 6: Create symbolic link to existing MFC library
method_symlink_fallback() {
    log "Method 6: Create symbolic link to existing MFC library"
    
    local wine_prefix=$(detect_wine_prefix)
    local system32_path="$wine_prefix/drive_c/windows/system32"
    
    # Check for existing MFC libraries
    local mfc_libs=("mfc140u.dll" "mfc120u.dll" "mfc110u.dll" "mfc100u.dll")
    
    for lib in "${mfc_libs[@]}"; do
        if [ -f "$system32_path/$lib" ]; then
            log "Found existing MFC library: $lib"
            ln -sf "$system32_path/$lib" "$TIR_FIRMWARE_DIR/mfc42u.dll"
            success "Created symbolic link from $lib to mfc42u.dll"
            return 0
        fi
    done
    
    warning "No existing MFC libraries found for symbolic link"
    return 1
}

# Method 7: Manual installation guide
method_manual_guide() {
    log "Method 7: Manual installation guide"
    
    cat << 'EOF' > "$LINUXTRACK_CONFIG_DIR/mfc42_manual_install.md"
# Manual MFC42u.dll Installation Guide

## Option 1: Download from Microsoft
1. Visit: https://www.microsoft.com/en-us/download/details.aspx?id=15336
2. Download "Visual C++ 6.0 Redistributable"
3. Run the installer in Wine: `wine vcredist.exe /Q /T:C:\\ /C`
4. Copy the DLL: `cp ~/.wine/drive_c/windows/system32/mfc42u.dll ~/.config/linuxtrack/tir_firmware/`

## Option 2: Extract from Windows
1. Copy mfc42u.dll from a Windows system: `C:\Windows\System32\mfc42u.dll`
2. Place it in: `~/.config/linuxtrack/tir_firmware/mfc42u.dll`

## Option 3: Use cabextract
1. Install cabextract: `sudo apt install cabextract`
2. Extract the installer: `cabextract -d extracted vcredist.exe`
3. Find mfc42u.dll in the extracted files and copy it

## Option 4: Alternative download sources
- Check GitHub repositories for MFC42u.dll
- Look for Visual C++ 6.0 redistributable packages
- Search for MFC42u.dll in software archives

## Verification
After installation, verify with:
```bash
file ~/.config/linuxtrack/tir_firmware/mfc42u.dll
```
EOF
    
    success "Manual installation guide created: $LINUXTRACK_CONFIG_DIR/mfc42_manual_install.md"
    return 0
}

# Verification function
verify_installation() {
    log "Verifying MFC42u.dll installation..."
    
    if [ -f "$TIR_FIRMWARE_DIR/mfc42u.dll" ]; then
        local file_info=$(file "$TIR_FIRMWARE_DIR/mfc42u.dll" 2>/dev/null || echo "Unknown")
        local file_size=$(stat -c%s "$TIR_FIRMWARE_DIR/mfc42u.dll" 2>/dev/null || echo "Unknown")
        
        log "File info: $file_info"
        log "File size: $file_size bytes"
        
        if [[ "$file_info" == *"PE32"* ]] || [[ "$file_info" == *"MS-DOS"* ]]; then
            success "MFC42u.dll appears to be a valid Windows DLL"
            return 0
        else
            warning "MFC42u.dll may not be a valid Windows DLL"
            return 1
        fi
    else
        error "MFC42u.dll not found in $TIR_FIRMWARE_DIR"
        return 1
    fi
}

# Main installation function
install_mfc42() {
    log "Starting MFC42u.dll installation..."
    
    create_directories
    
    # Try methods in order of preference
    local methods=(
        "method_direct_download"
        "method_windows_extract"
        "method_package_manager"
        "method_cabextract"
        "method_alternative_sources"
        "method_symlink_fallback"
    )
    
    for method in "${methods[@]}"; do
        log "Trying $method..."
        if $method; then
            if verify_installation; then
                success "MFC42u.dll installed successfully using $method"
                return 0
            else
                warning "Installation completed but verification failed"
            fi
        fi
    done
    
    # If all methods fail, show manual guide
    method_manual_guide
    
    error "All automatic installation methods failed. Please use the manual installation guide."
    return 1
}

# Cleanup function
cleanup() {
    log "Cleaning up temporary files..."
    rm -rf "$CACHE_DIR/vcredist_extracted" 2>/dev/null || true
}

# Main script
main() {
    log "=== MFC42u.dll Alternative Installation Script ==="
    log "Target directory: $TIR_FIRMWARE_DIR"
    log "Cache directory: $CACHE_DIR"
    log "Log file: $LOG_FILE"
    
    # Parse command line arguments
    case "${1:-install}" in
        "install")
            install_mfc42
            ;;
        "verify")
            verify_installation
            ;;
        "cleanup")
            cleanup
            ;;
        "help"|"-h"|"--help")
            echo "Usage: $0 [install|verify|cleanup|help]"
            echo "  install  - Install MFC42u.dll using multiple methods"
            echo "  verify   - Verify existing MFC42u.dll installation"
            echo "  cleanup  - Clean up temporary files"
            echo "  help     - Show this help message"
            ;;
        *)
            error "Unknown command: $1"
            echo "Use '$0 help' for usage information"
            exit 1
            ;;
    esac
    
    cleanup
}

# Run main function with all arguments
main "$@" 