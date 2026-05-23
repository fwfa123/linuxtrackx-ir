#!/bin/bash

# LinuxTrack X-IR Arch Linux Build Script (Qt6)
# - Installs broad deps (opencv, v4l-utils, liblo, ...) and MinGW toolchains for Wine bridge PE builds.
# - Wiimote: only with --with-wiimote (GitLab #8). May bootstrap yay from AUR if no helper exists.

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
        print_warning "No yay/paru found — cloning and building yay from AUR (network + sudo required; review AUR trust if needed)."
        print_status "Installing yay..."
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
    
    # MinGW-w64 toolchains for PE DLL/EXE outputs
    sudo pacman -S --needed mingw-w64-gcc
    
    # Video4Linux
    sudo pacman -S --needed v4l-utils
    
    print_success "Build dependencies installed"
}

# Optional Wiimote (libcwiid): AUR cwiid is unmaintained and often pulls python2/openssl-1.1 (GitLab #8).
# Only invoked when user passes --with-wiimote.
install_wiimote_support() {
    print_status "Installing Wiimote support (optional AUR cwiid)..."
    
    local aur_helper=$(detect_aur_helper)
    
    if [ "$aur_helper" = "none" ]; then
        print_warning "No AUR helper (yay/paru). Install one and re-run with --with-wiimote, or install libcwiid manually."
        echo "  sudo pacman -S --needed git base-devel && git clone https://aur.archlinux.org/yay.git && cd yay && makepkg -si"
        return 0
    fi
    
    print_status "Installing cwiid from AUR using $aur_helper (may fail on modern Arch)..."
    if $aur_helper -S cwiid --noconfirm; then
        print_success "cwiid installed successfully"
        if command -v wminput >/dev/null 2>&1; then
            print_success "Wiimote utilities found"
        else
            print_warning "Wiimote utilities not found in PATH"
        fi
        return 0
    fi
    
    print_warning "cwiid failed; trying cwiid-git..."
    if $aur_helper -S cwiid-git --noconfirm; then
        print_success "cwiid-git installed successfully"
        return 0
    fi
    
    print_warning "Wiimote (cwiid) not installed — AUR packages are outdated (python2 chain). Build continues without Wiimote. See: https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/8"
    return 0
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

# Function to check X-Plane SDK (manual download from developer.x-plane.com)
install_xplane_sdk() {
    local sdk_dir="/opt/xplane-sdk"
    if [ -d "$sdk_dir/CHeaders/XPLM" ]; then
        print_success "X-Plane SDK found at $sdk_dir/CHeaders"
        if [ ! -f /etc/profile.d/xplane-sdk.sh ]; then
            echo "export XPLANE_SDK_PATH=$sdk_dir/CHeaders" | sudo tee /etc/profile.d/xplane-sdk.sh
        fi
        return 0
    fi
    print_warning "X-Plane SDK not found at $sdk_dir/CHeaders"
    print_status "Download a ZIP from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/"
    print_status "  (e.g. XPSDK430.zip — version in the filename may change)"
    print_status "  unzip -q XPSDK*.zip -d /tmp/xpsdk && sudo cp -a /tmp/xpsdk/SDK/. $sdk_dir/"
    print_status "Then re-run or pass -DXPLANE_SDK_PATH=$sdk_dir/CHeaders to cmake."
    sudo mkdir -p "$sdk_dir"
    return 0
}

# Function to install Wine runtime + MinGW bridge toolchains
install_wine_bridge_toolchain() {
    print_status "Installing Wine + MinGW toolchains..."
    sudo pacman -S --needed wine wine-mono wine-gecko mingw-w64-gcc
}

# v2.0.0+: Wine bridge needs MinGW only (no NSIS / linuxtrack-wine.exe)
install_nsis() {
    print_status "NSIS not required for Wine bridge (v2 native install)"
    return 0
}

# Function to verify Wine runtime + MinGW bridge toolchains
verify_wine_bridge_toolchain() {
    print_status "Verifying Wine + MinGW toolchains..."
    if ! command_exists wine; then
        print_error "wine not found"
        return 1
    fi
    if ! command_exists i686-w64-mingw32-gcc || ! command_exists x86_64-w64-mingw32-gcc; then
        print_error "mingw-w64 cross-compilers not found"
        return 1
    fi
    print_success "Wine + MinGW toolchains verified"
}

# Function to configure build
configure_build() {
    print_status "Configuring build with CMake..."
    
    # Create build directory
    mkdir -p build
    cd build
    
    # Configure with CMake
    cmake .. \
        -DCMAKE_INSTALL_PREFIX=/opt
    
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
    
    if [ -f "/opt/lib/linuxtrack/wine_bridge/NPClient.dll" ]; then
        print_success "Wine bridge payload present: /opt/lib/linuxtrack/wine_bridge/NPClient.dll"
    else
        print_warning "Wine bridge not found at /opt/lib/linuxtrack/wine_bridge. Build with MinGW toolchains or use AppImage bridge."
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
    echo "  --wine-bridge-only"
    echo "                   Install Wine runtime + MinGW bridge toolchains only"
    echo "  --wine32-only    Compatibility alias for --wine-bridge-only"
    echo "  --configure-only Configure build only"
    echo "  --build-only     Build only (assumes dependencies installed)"
    echo "  --install-only   Install only (assumes build completed)"
    echo "  --verify-only    Verify installation only"
    echo "  --with-wiimote   Try to install AUR cwiid (optional; often broken; see GitLab #8)"
    echo "  --test-wiimote   Test Wiimote support (pkg-config cwiid)"
    echo "  --test-osc       Test OSC support"
    echo "  --test-xplane    Test X-Plane SDK support"
    echo "  --help           Show this help message"
    echo ""
    echo "Default: deps + Wine + MinGW + OSC + X-Plane SDK check + configure/build/install."
    echo "Wiimote is NOT installed unless you pass --with-wiimote."
}

# Main function
main() {
    print_status "Starting LinuxTrack X-IR Arch Linux build process..."
    
    # Parse command line arguments
    local deps_only=false
    local wine_bridge_only=false
    local configure_only=false
    local build_only=false
    local install_only=false
    local verify_only=false
    local test_wiimote=false
    local test_osc=false
    local test_xplane=false
    local with_wiimote=false
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            --deps-only)
                deps_only=true
                shift
                ;;
            --wine32-only)
                wine_bridge_only=true
                shift
                ;;
            --wine-bridge-only)
                wine_bridge_only=true
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
            --with-wiimote)
                with_wiimote=true
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
        if [ "$with_wiimote" = true ]; then
            install_wiimote_support
        fi
        exit 0
    elif [ "$wine_bridge_only" = true ]; then
        install_wine_bridge_toolchain
        verify_wine_bridge_toolchain
        exit 0
    elif [ "$verify_only" = true ]; then
        verify_installation
        exit 0
    elif [ "$test_wiimote" = true ]; then
        if pkg-config --exists cwiid 2>/dev/null; then print_success "cwiid: found"; else print_warning "cwiid: not found (optional; use --with-wiimote to try AUR, often fails — GitLab #8)"; fi
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
    
    # Install Wine runtime + MinGW bridge toolchains
    if [ "$build_only" = true ] || [ "$install_only" = true ] || [ "$configure_only" = true ]; then
        :
    else
        install_wine_bridge_toolchain
        verify_wine_bridge_toolchain
    fi
    
    # Wiimote (cwiid): optional only — see GitLab #8
    if [ "$build_only" = true ] || [ "$install_only" = true ] || [ "$configure_only" = true ]; then
        :
    elif [ "$with_wiimote" = true ]; then
        install_wiimote_support
    else
        print_status "Skipping Wiimote (cwiid): optional; AUR packages are unmaintained. Pass --with-wiimote to try, or build without Wiimote (default)."
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
    
    # v2: no NSIS package (install_nsis is a no-op stub)
    if [ "$build_only" = true ] || [ "$install_only" = true ] || [ "$configure_only" = true ]; then
        :
    else
        install_nsis
    fi
    
    # Configure build
    if [ "$configure_only" = true ]; then
        print_status "configure-only: ensuring liblo and Wine + MinGW (same as full run before cmake)..."
        install_osc_support
        install_wine_bridge_toolchain
        verify_wine_bridge_toolchain
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