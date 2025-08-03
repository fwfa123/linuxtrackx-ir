#!/bin/bash

# MFC42u.dll Alternative Installation Integration Script
# This script provides a simple interface for the LinuxTrack GUI to install MFC42u.dll

set -e

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LINUXTRACK_CONFIG_DIR="$HOME/.config/linuxtrack"
TIR_FIRMWARE_DIR="$LINUXTRACK_CONFIG_DIR/tir_firmware"
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
    mkdir -p "$(dirname "$LOG_FILE")"
}

# Check if MFC42u.dll is already installed
check_existing() {
    if [ -f "$TIR_FIRMWARE_DIR/mfc42u.dll" ]; then
        log "MFC42u.dll already exists in $TIR_FIRMWARE_DIR"
        
        # Verify the file
        if file "$TIR_FIRMWARE_DIR/mfc42u.dll" | grep -q "PE32"; then
            success "Existing MFC42u.dll appears to be valid"
            return 0
        else
            warning "Existing MFC42u.dll may be corrupted, will reinstall"
            return 1
        fi
    fi
    return 1
}

# Main installation function
install_mfc42() {
    log "Starting MFC42u.dll alternative installation..."
    
    create_directories
    
    # Check if already installed
    if check_existing; then
        success "MFC42u.dll is already properly installed"
        return 0
    fi
    
    # Try the alternative installation script
    if [ -f "$SCRIPT_DIR/mfc42_alternative_installers.sh" ]; then
        log "Running alternative installation script..."
        if "$SCRIPT_DIR/mfc42_alternative_installers.sh" install; then
            success "MFC42u.dll installed successfully using alternative methods"
            return 0
        else
            error "Alternative installation script failed"
        fi
    else
        error "Alternative installation script not found: $SCRIPT_DIR/mfc42_alternative_installers.sh"
    fi
    
    # Try Python script as fallback
    if [ -f "$SCRIPT_DIR/mfc42_python_installer.py" ]; then
        log "Trying Python installation script as fallback..."
        if python3 "$SCRIPT_DIR/mfc42_python_installer.py" install; then
            success "MFC42u.dll installed successfully using Python script"
            return 0
        else
            error "Python installation script failed"
        fi
    fi
    
    # Manual installation guide
    log "All automatic methods failed, showing manual installation guide..."
    cat << 'EOF' > "$LINUXTRACK_CONFIG_DIR/mfc42_manual_install.md"
# Manual MFC42u.dll Installation Guide

## Quick Installation Methods:

### Method 1: Download from Microsoft
```bash
wget https://download.microsoft.com/download/vc60pro/redist/6.0.8447.0/Win98Me/EN-US/vcredist.exe
wine vcredist.exe /Q /T:C:\\ /C
cp ~/.wine/drive_c/windows/system32/mfc42u.dll ~/.config/linuxtrack/tir_firmware/
```

### Method 2: Extract from Windows (if dual-booting)
```bash
cp /mnt/c/Windows/System32/mfc42u.dll ~/.config/linuxtrack/tir_firmware/
```

### Method 3: Use cabextract
```bash
sudo apt install cabextract
cabextract -d extracted vcredist.exe
find extracted -name "mfc42u.dll" -exec cp {} ~/.config/linuxtrack/tir_firmware/ \;
```

### Method 4: Alternative download sources
- DLL-Files.com: https://www.dll-files.com/mfc42u.dll.html
- DLLme.com: https://www.dllme.com/dll/files/mfc42u.html

## Verification:
```bash
file ~/.config/linuxtrack/tir_firmware/mfc42u.dll
ls -lh ~/.config/linuxtrack/tir_firmware/mfc42u.dll
```
EOF
    
    error "All automatic installation methods failed. Please use the manual installation guide: $LINUXTRACK_CONFIG_DIR/mfc42_manual_install.md"
    return 1
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

# Main script
main() {
    log "=== MFC42u.dll Alternative Installation Integration ==="
    log "Target directory: $TIR_FIRMWARE_DIR"
    log "Log file: $LOG_FILE"
    
    # Parse command line arguments
    case "${1:-install}" in
        "install")
            install_mfc42
            ;;
        "verify")
            verify_installation
            ;;
        "help"|"-h"|"--help")
            echo "Usage: $0 [install|verify|help]"
            echo "  install  - Install MFC42u.dll using alternative methods"
            echo "  verify   - Verify existing MFC42u.dll installation"
            echo "  help     - Show this help message"
            ;;
        *)
            error "Unknown command: $1"
            echo "Use '$0 help' for usage information"
            exit 1
            ;;
    esac
}

# Run main function with all arguments
main "$@" 