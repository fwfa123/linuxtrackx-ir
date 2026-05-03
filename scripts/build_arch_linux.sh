#!/bin/bash

# LinuxTrack X-IR Arch Linux Build Script (Qt6)
# - Installs broad deps (opencv, v4l-utils, liblo, ...) but configure_build() uses CMake defaults
#   except ENABLE_LTR_32LIB_ON_X64 and WINE_* paths — effectively ~README Level 2 (Wine bridge).
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
    
    # 32-bit support
    sudo pacman -S --needed lib32-glibc lib32-gcc-libs
    
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
    
    # AUR wine32 conflicts with repo "wine"; do not try to install both.
    if pacman -Q wine32 >/dev/null 2>&1; then
        print_status "AUR wine32 is installed — skipping repo wine (packages conflict)."
        sudo pacman -S --needed lib32-glibc lib32-gcc-libs
        print_status "Installing wine-mono and wine-gecko (~400 MiB installed; download may take a minute)..."
        if sudo pacman -S --needed --noconfirm wine-mono wine-gecko; then
            :
        else
            print_warning "wine-mono/wine-gecko install failed or skipped (optional; install manually if Wine installers need them)."
        fi
        if command -v winegcc >/dev/null 2>&1 || command -v wine >/dev/null 2>&1; then
            print_success "Wine stack (wine32) present"
        else
            print_warning "wine/winegcc not in PATH — check wine32 package."
        fi
        return 0
    fi
    
    if pacman -Q wine >/dev/null 2>&1 && { [ -d /usr/lib32/wine ] || [ -d /usr/lib32/wine/i386-unix ]; }; then
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
    
    # Repo multilib layout vs AUR wine32 layout differ; winegcc test below is the real check.
    if [ ! -d "/usr/lib32/wine" ] && [ ! -d "/usr/lib32/wine/i386-unix" ] && [ ! -d "/usr/lib/wine/i386-unix" ]; then
        if ! pacman -Q wine32 >/dev/null 2>&1; then
            print_warning "32-bit wine Unix libs not found under /usr/lib32/wine or /usr/lib/wine/i386-unix. Enable multilib and lib32-wine, or install AUR wine32."
        fi
    fi
    
    # Test winegcc
    if ! command_exists winegcc; then
        print_error "winegcc not found"
        return 1
    fi
    
    # Test winegcc: some WOW64 setups report success but emit a 0-byte .o (GitLab #37).
    print_status "Testing winegcc -m32..."
    rm -f /tmp/test_winegcc.c /tmp/test_winegcc.o /tmp/ltrx_wgg_smoke.exe.so
    cat > /tmp/test_winegcc.c << 'EOF'
int main(void) { return 0; }
EOF
    local wgcc_ok=0
    if winegcc -m32 -c -o /tmp/test_winegcc.o /tmp/test_winegcc.c 2>/dev/null && [ -s /tmp/test_winegcc.o ]; then
        wgcc_ok=1
    elif command -v readelf >/dev/null 2>&1 && winegcc -m32 -c -o /tmp/test_winegcc.o /tmp/test_winegcc.c 2>/dev/null \
        && readelf -h /tmp/test_winegcc.o 2>/dev/null | grep -q 'Class:.*ELF32'; then
        wgcc_ok=1
    elif winegcc -m32 -o /tmp/ltrx_wgg_smoke.exe.so /tmp/test_winegcc.c 2>/dev/null && [ -s /tmp/ltrx_wgg_smoke.exe.so ]; then
        wgcc_ok=1
    fi
    rm -f /tmp/test_winegcc.c /tmp/test_winegcc.o /tmp/ltrx_wgg_smoke.exe.so

    if [ "$wgcc_ok" = 1 ]; then
        print_success "winegcc -m32 smoke test passed"
    else
        print_warning "winegcc -m32 smoke test failed or produced no usable output (common on stock WOW64 Wine). Continuing — if CMake fails on Wine, install AUR wine32 or wine-stable/wine-stable-mono, or run $0 --deps-only / --wine32-only then $0 --configure-only (see docs/readme/arch-linux.md)."
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
    
    # Wine bridge: NSIS installer under share/linuxtrack/wine/ (see src/wine_bridge/CMakeLists.txt)
    if [ -f "/opt/share/linuxtrack/wine/linuxtrack-wine.exe" ]; then
        print_success "Wine bridge installer present: /opt/share/linuxtrack/wine/linuxtrack-wine.exe"
    elif [ -d "/opt/lib/linuxtrack/wine_bridge" ] && compgen -G '/opt/lib/linuxtrack/wine_bridge/*' >/dev/null; then
        print_success "Wine bridge built files present under /opt/lib/linuxtrack/wine_bridge (installer may be missing if NSIS was unavailable at install time)"
    else
        print_warning "Wine bridge not found (no installer at /opt/share/linuxtrack/wine/linuxtrack-wine.exe and no /opt/lib/linuxtrack/wine_bridge). Build with Wine plugin + makensis, or use AppImage bridge."
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
    echo "  --with-wiimote   Try to install AUR cwiid (optional; often broken; see GitLab #8)"
    echo "  --test-wiimote   Test Wiimote support (pkg-config cwiid)"
    echo "  --test-osc       Test OSC support"
    echo "  --test-xplane    Test X-Plane SDK support"
    echo "  --help           Show this help message"
    echo ""
    echo "Default: deps + Wine + OSC + X-Plane SDK check + NSIS + configure/build/install."
    echo "Wiimote is NOT installed unless you pass --with-wiimote."
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
    local with_wiimote=false
    
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
    elif [ "$wine32_only" = true ]; then
        install_wine32
        verify_wine32
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
    
    # Install Wine (multilib) — wine32_only already exited above
    if [ "$build_only" = true ] || [ "$install_only" = true ] || [ "$configure_only" = true ]; then
        :
    else
        install_wine32
        verify_wine32
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
    
    # Install NSIS
    if [ "$build_only" = true ] || [ "$install_only" = true ] || [ "$configure_only" = true ]; then
        # Skip NSIS installation for these modes
        :
    else
        install_nsis
    fi
    
    # Configure build
    if [ "$configure_only" = true ]; then
        # Main flow skips NSIS/Wine/OSC when using split flags — CMake needs makensis + winegcc + wineg++ for WINE_PLUGIN and liblo.pc for OSC.
        print_status "configure-only: ensuring NSIS, liblo, and Wine (same as full run before cmake)..."
        install_nsis
        install_osc_support
        install_wine32
        verify_wine32
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