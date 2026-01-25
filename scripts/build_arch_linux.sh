#!/bin/bash

# LinuxTrack X-IR Arch Linux Build Script (Qt6)
# This script provides seamless Arch Linux support with proper 32-bit wine bridge functionality
# Designed for Arch Linux with Qt6 (default on modern Arch Linux systems)

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

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to detect AUR helper
detect_aur_helper() {
    if command_exists yay; then
        echo "yay"
    elif command_exists paru; then
        echo "paru"
    elif command_exists pacaur; then
        echo "pacaur"
    else
        echo "none"
    fi
}

# Function to install AUR helper if needed
install_aur_helper() {
    local aur_helper=$(detect_aur_helper)
    
    if [ "$aur_helper" = "none" ]; then
        print_status "No AUR helper found. Installing yay..."
        sudo pacman -S --needed git base-devel
        git clone https://aur.archlinux.org/yay.git
        cd yay
        makepkg -si --noconfirm
        cd ..
        rm -rf yay
        print_success "yay installed successfully"
    else
        print_status "Using existing AUR helper: $aur_helper"
    fi
}

# Function to install dependencies
install_dependencies() {
    print_status "Installing build dependencies..."
    
    # Core build tools
    sudo pacman -S --needed base-devel cmake pkg-config
    
    # Qt6 dependencies (Arch Linux default)
    sudo pacman -S --needed qt6-base qt6-tools qt6-5compat
    
    # Libraries (zlib omitted: CachyOS etc. use zlib-ng-compat which provides libz; CMake finds it)
    sudo pacman -S --needed opencv libusb mxml mesa glu sqlite
    
    # Build tools
    sudo pacman -S --needed bison flex
    
    # 32-bit support
    sudo pacman -S --needed lib32-glibc lib32-gcc-libs
    
    # Video4Linux
    sudo pacman -S --needed v4l-utils
    
    print_success "Build dependencies installed"
}

# Function to install Wiimote support
install_wiimote_support() {
    print_status "Installing Wiimote support..."
    
    local aur_helper=$(detect_aur_helper)
    
    if [ "$aur_helper" = "none" ]; then
        print_error "No AUR helper found. Please install yay or paru first."
        print_status "You can install yay manually:"
        echo "sudo pacman -S --needed git base-devel"
        echo "git clone https://aur.archlinux.org/yay.git"
        echo "cd yay && makepkg -si"
        return 1
    fi
    
    print_status "Installing cwiid from AUR using $aur_helper..."
    $aur_helper -S cwiid --noconfirm
    
    if [ $? -eq 0 ]; then
        print_success "cwiid installed successfully"
        
        # Test cwiid installation
        if command -v wminput >/dev/null 2>&1; then
            print_success "Wiimote utilities found"
        else
            print_warning "Wiimote utilities not found in PATH"
        fi
    else
        print_error "Failed to install cwiid"
        print_status "Trying alternative package cwiid-git..."
        $aur_helper -S cwiid-git --noconfirm
        
        if [ $? -eq 0 ]; then
            print_success "cwiid-git installed successfully"
        else
            print_error "Failed to install Wiimote support"
            return 1
        fi
    fi
}

# Function to install OSC support (liblo is in official [extra])
install_osc_support() {
    print_status "Installing OSC support (liblo)..."
    
    if sudo pacman -S --needed liblo 2>/dev/null; then
        if pkg-config --exists liblo 2>/dev/null; then
            print_success "liblo installed (official [extra])"
        else
            print_warning "liblo installed but pkg-config may need refresh"
        fi
    else
        print_error "Failed to install liblo. Try: sudo pacman -S liblo"
        return 1
    fi
}

# Function to install X-Plane SDK (manual download; that URL serves HTML, not a .zip)
install_xplane_sdk() {
    local sdk_dir="/opt/xplane-sdk"
    if [ -d "$sdk_dir/CHeaders" ]; then
        print_success "X-Plane SDK found at $sdk_dir"
        if [ ! -f /etc/profile.d/xplane-sdk.sh ]; then
            echo "export XPLANE_SDK_PATH=$sdk_dir" | sudo tee /etc/profile.d/xplane-sdk.sh
            echo "export XPLANE_SDK_INCLUDE=$sdk_dir/CHeaders" | sudo tee -a /etc/profile.d/xplane-sdk.sh
        fi
        return 0
    fi
    print_warning "X-Plane SDK not found at $sdk_dir/CHeaders"
    print_status "Download from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/"
    print_status "Extract so that $sdk_dir/CHeaders exists. Then re-run or pass -DXPLANE_SDK_PATH=$sdk_dir/CHeaders to cmake."
    sudo mkdir -p "$sdk_dir"
    return 0
}

# Function to install Wine (multilib). AUR wine32 is fallback only.
install_wine32() {
    print_status "Installing Wine (multilib)..."
    
    if pacman -Q wine >/dev/null 2>&1 && [ -d /usr/lib32/wine ] || [ -d /usr/lib32/wine/i386-unix ]; then
        print_success "Wine and 32-bit libs appear present"
        return 0
    fi
    
    sudo pacman -S --needed wine wine-mono wine-gecko
    sudo pacman -S --needed lib32-glibc lib32-gcc-libs
    sudo pacman -S lib32-wine 2>/dev/null || true
    
    if command -v winegcc >/dev/null 2>&1; then
        print_success "Wine (multilib) installed"
    else
        print_warning "winegcc not found. If build fails, enable multilib in /etc/pacman.conf and install lib32-wine, or try AUR wine32 as fallback."
    fi
}

# Function to install NSIS
install_nsis() {
    print_status "Checking NSIS installation..."
    
    if command_exists makensis; then
        print_success "NSIS is already installed"
        return 0
    fi
    
    # Try to install NSIS using our helper script
    if [ -f "scripts/install/install_nsis_arch.sh" ]; then
        print_status "Installing NSIS using helper script..."
        chmod +x scripts/install/install_nsis_arch.sh
        ./scripts/install/install_nsis_arch.sh
    else
        print_warning "NSIS helper script not found. Installing manually..."
        local aur_helper=$(detect_aur_helper)
        if [ "$aur_helper" != "none" ]; then
            $aur_helper -S nsis --noconfirm
        else
            print_error "No AUR helper available for NSIS installation"
            return 1
        fi
    fi
}

# Function to verify Wine (multilib) installation
verify_wine32() {
    print_status "Verifying Wine (multilib)..."
    
    if ! command -v wine >/dev/null 2>&1; then
        print_error "wine not found"
        return 1
    fi
    
    if [ ! -d "/usr/lib32/wine" ] && [ ! -d "/usr/lib32/wine/i386-unix" ]; then
        print_warning "32-bit wine libs not found at /usr/lib32/wine. Enable multilib and install lib32-wine (or AUR wine32 as fallback)."
    fi
    
    # Test winegcc
    if ! command_exists winegcc; then
        print_error "winegcc not found"
        return 1
    fi
    
    # Test winegcc compilation
    print_status "Testing winegcc compilation..."
    # Create a minimal test C file
    cat > /tmp/test_winegcc.c << 'EOF'
int main(void) { return 0; }
EOF
    
    # Test winegcc compilation (compile only, no linking to avoid library dependency issues)
    if winegcc -m32 -c -o /tmp/test_winegcc.o /tmp/test_winegcc.c 2>/dev/null; then
        # Verify the output file exists and is not empty
        if [ -s /tmp/test_winegcc.o ]; then
            print_success "winegcc compilation test passed"
            rm -f /tmp/test_winegcc.c /tmp/test_winegcc.o
        else
            print_error "winegcc produced empty output file"
            rm -f /tmp/test_winegcc.c /tmp/test_winegcc.o
            return 1
        fi
    else
        print_error "winegcc compilation test failed"
        rm -f /tmp/test_winegcc.c /tmp/test_winegcc.o
        return 1
    fi
    
    print_success "Wine installation verified"
}

# Function to configure build
configure_build() {
    print_status "Configuring build with CMake..."
    
    # Create build directory
    mkdir -p build
    cd build
    
    # Configure with CMake
    cmake .. \
        -DCMAKE_INSTALL_PREFIX=/opt \
        -DENABLE_LTR_32LIB_ON_X64=ON \
        -DWINE_LIBS_PATH="/usr/lib32/wine/i386-unix" \
        -DWINE64_LIBS_PATH="/usr/lib/wine/x86_64-unix"
    
    # Verify config.h was generated (required for wine bridge components)
    if [ ! -f "config.h" ]; then
        print_error "config.h not found after CMake configuration"
        cd ..
        return 1
    fi
    
    cd ..
    
    print_success "Build configured successfully"
}

# Function to build
build_project() {
    print_status "Building LinuxTrack X-IR..."
    
    # Build with all available cores
    local cores=$(nproc)
    cd build
    cmake --build . -j$cores
    cd ..
    
    print_success "Build completed successfully"
}

# Function to install
install_project() {
    print_status "Installing LinuxTrack X-IR..."
    
    cd build
    sudo cmake --install .
    cd ..
    
    # Add user to plugdev group
    sudo usermod -a -G plugdev,input $USER
    
    print_success "Installation completed successfully"
}

# Function to verify installation
verify_installation() {
    print_status "Verifying installation..."
    
    if command -v ltr_gui >/dev/null 2>&1; then
        print_success "ltr_gui found in PATH"
    elif [ -f "/opt/bin/ltr_gui" ]; then
        print_success "ltr_gui at /opt/bin/ltr_gui (ensure /opt/bin in PATH)"
    else
        print_error "ltr_gui not found"
        return 1
    fi
    
    # Check wine bridge components
    if [ -f "/opt/share/linuxtrack/linuxtrack-wine.exe" ]; then
        print_success "Wine bridge components installed"
    else
        print_warning "Wine bridge components not found"
    fi
    
    print_success "Installation verification completed"
}

# Function to show usage
show_usage() {
    echo "LinuxTrack X-IR Arch Linux Build Script (Qt6)"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --deps-only      Install dependencies only"
    echo "  --wine32-only    Install Wine (multilib) only"
    echo "  --configure-only Configure build only"
    echo "  --build-only     Build only (assumes dependencies installed)"
    echo "  --install-only   Install only (assumes build completed)"
    echo "  --verify-only    Verify installation only"
echo "  --test-wiimote   Test Wiimote support"
echo "  --test-osc       Test OSC support"
echo "  --test-xplane    Test X-Plane SDK support"
echo "  --help           Show this help message"
    echo ""
    echo "Default behavior: Full installation (deps + Wine + build + install)"
}

# Main function
main() {
    print_status "Starting LinuxTrack X-IR Arch Linux build process..."
    
    # Parse command line arguments
    local deps_only=false
    local wine32_only=false
    local configure_only=false
    local build_only=false
    local install_only=false
    local verify_only=false
    local test_wiimote=false
    local test_osc=false
    local test_xplane=false
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            --deps-only)
                deps_only=true
                shift
                ;;
            --wine32-only)
                wine32_only=true
                shift
                ;;
            --configure-only)
                configure_only=true
                shift
                ;;
            --build-only)
                build_only=true
                shift
                ;;
            --install-only)
                install_only=true
                shift
                ;;
            --verify-only)
                verify_only=true
                shift
                ;;
            --test-wiimote)
                test_wiimote=true
                shift
                ;;
            --test-osc)
                test_osc=true
                shift
                ;;
            --test-xplane)
                test_xplane=true
                shift
                ;;
            --help)
                show_usage
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
    done
    
    # Check if we're in the right directory
    if [ ! -f "CMakeLists.txt" ]; then
        print_error "This script must be run from the LinuxTrack X-IR source directory"
        exit 1
    fi
    
    # Install AUR helper if needed
    install_aur_helper
    
    # Install dependencies
    if [ "$deps_only" = true ]; then
        install_dependencies
        exit 0
    elif [ "$wine32_only" = true ]; then
        install_wine32
        verify_wine32
        exit 0
    elif [ "$verify_only" = true ]; then
        verify_installation
        exit 0
    elif [ "$test_wiimote" = true ]; then
        if pkg-config --exists cwiid 2>/dev/null; then print_success "cwiid: found"; else print_warning "cwiid: not found (yay -S cwiid)"; fi
        exit 0
    elif [ "$test_osc" = true ]; then
        if pkg-config --exists liblo 2>/dev/null; then print_success "liblo: found"; else print_warning "liblo: not found (pacman -S liblo)"; fi
        exit 0
    elif [ "$test_xplane" = true ]; then
        if [ -d /opt/xplane-sdk/CHeaders ]; then print_success "X-Plane SDK: found"; else print_warning "X-Plane SDK: not found at /opt/xplane-sdk/CHeaders"; fi
        exit 0
    elif [ "$build_only" = true ] || [ "$install_only" = true ] || [ "$configure_only" = true ]; then
        # Skip dependency installation for these modes
        :
    else
        install_dependencies
    fi
    
    # Install wine32
    if [ "$wine32_only" = true ]; then
        install_wine32
        verify_wine32
        exit 0
    elif [ "$build_only" = true ] || [ "$install_only" = true ] || [ "$configure_only" = true ]; then
        # Skip wine32 installation for these modes
        :
    else
        install_wine32
        verify_wine32
    fi
    
    # Install Wiimote support
    if [ "$build_only" = true ] || [ "$install_only" = true ] || [ "$configure_only" = true ]; then
        # Skip Wiimote installation for these modes
        :
    else
        install_wiimote_support
    fi
    
    # Install OSC support
    if [ "$build_only" = true ] || [ "$install_only" = true ] || [ "$configure_only" = true ]; then
        # Skip OSC installation for these modes
        :
    else
        install_osc_support
    fi
    
    # Install X-Plane SDK
    if [ "$build_only" = true ] || [ "$install_only" = true ] || [ "$configure_only" = true ]; then
        # Skip X-Plane SDK installation for these modes
        :
    else
        install_xplane_sdk
    fi
    
    # Install NSIS
    if [ "$build_only" = true ] || [ "$install_only" = true ] || [ "$configure_only" = true ]; then
        # Skip NSIS installation for these modes
        :
    else
        install_nsis
    fi
    
    # Configure build
    if [ "$configure_only" = true ]; then
        configure_build
        exit 0
    elif [ "$build_only" = true ] || [ "$install_only" = true ]; then
        # Skip configuration for these modes
        :
    else
        configure_build
    fi
    
    # Build project
    if [ "$build_only" = true ]; then
        build_project
        exit 0
    elif [ "$install_only" = true ]; then
        # Skip build for install-only mode
        :
    else
        build_project
    fi
    
    # Install project
    if [ "$install_only" = true ]; then
        install_project
        verify_installation
        exit 0
    else
        install_project
    fi
    
    # Verify installation
    verify_installation
    
    print_success "LinuxTrack X-IR installation completed successfully!"
    print_status "You can now run: ltr_gui"
    print_status "Don't forget to log out and back in for group changes to take effect"
}

# Run main function
main "$@" 