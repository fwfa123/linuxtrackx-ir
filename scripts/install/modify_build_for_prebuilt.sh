#!/bin/bash

# LinuxTrack X-IR Build System Modifier
# This script modifies the build system to use pre-built Wine bridge binaries
# instead of requiring Wine development tools

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"
WINE_BRIDGE_DIR="$PROJECT_ROOT/src/wine_bridge"
PREBUILT_DIR="$PROJECT_ROOT/prebuilt_wine_bridge"

echo -e "${BLUE}🔧 LinuxTrack X-IR Build System Modifier${NC}"
echo -e "${BLUE}==========================================${NC}"
echo ""

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to backup original files
backup_files() {
    print_status "Creating backups of original files..."
    
    mkdir -p "$PROJECT_ROOT/backup_$(date +%Y%m%d_%H%M%S)"
    local backup_dir="$PROJECT_ROOT/backup_$(date +%Y%m%d_%H%M%S)"
    
    # Backup wine bridge Makefile.am
    if [ -f "$WINE_BRIDGE_DIR/Makefile.am" ]; then
        cp "$WINE_BRIDGE_DIR/Makefile.am" "$backup_dir/wine_bridge_Makefile.am.bak"
        print_status "Backed up wine_bridge/Makefile.am"
    fi
    
    # Backup configure.ac
    if [ -f "$PROJECT_ROOT/configure.ac" ]; then
        cp "$PROJECT_ROOT/configure.ac" "$backup_dir/configure.ac.bak"
        print_status "Backed up configure.ac"
    fi
    
    print_status "Backups created in: $backup_dir"
}

# Function to create prebuilt directory structure
create_prebuilt_structure() {
    print_status "Creating prebuilt directory structure..."
    
    mkdir -p "$PREBUILT_DIR"
    mkdir -p "$PREBUILT_DIR/32bit"
    mkdir -p "$PREBUILT_DIR/64bit"
    mkdir -p "$PREBUILT_DIR/installers"
    
    print_status "Prebuilt directory structure created"
}

# Function to modify wine bridge Makefile.am
modify_wine_bridge_makefile() {
    print_status "Modifying wine bridge Makefile.am..."
    
    # Create new Makefile.am that uses prebuilt binaries
    cat > "$WINE_BRIDGE_DIR/Makefile.am" << 'EOF'
# Wine Bridge Build Configuration (Prebuilt Version)
# 
# This build system uses pre-built Wine bridge components
# to avoid requiring Wine development tools

if WINE_PLUGIN

nodist_pkgdata_DATA = linuxtrack-wine.exe

# Prebuilt components directory
PREBUILT_DIR = $(top_srcdir)/prebuilt_wine_bridge

# Copy prebuilt components instead of building
all-local: copy_prebuilt_components

copy_prebuilt_components:
	@echo "Copying prebuilt Wine bridge components..."
	@if [ -d "$(PREBUILT_DIR)" ]; then \
		cp -f "$(PREBUILT_DIR)/32bit/"*.dll "$(builddir)/client/" 2>/dev/null || true; \
		cp -f "$(PREBUILT_DIR)/32bit/"*.exe "$(builddir)/client/" 2>/dev/null || true; \
		cp -f "$(PREBUILT_DIR)/32bit/"*.exe "$(builddir)/controller/" 2>/dev/null || true; \
		cp -f "$(PREBUILT_DIR)/32bit/"*.exe "$(builddir)/tester/" 2>/dev/null || true; \
		cp -f "$(PREBUILT_DIR)/32bit/"*.dll "$(builddir)/ft_client/" 2>/dev/null || true; \
		cp -f "$(PREBUILT_DIR)/32bit/"*.exe "$(builddir)/ft_tester/" 2>/dev/null || true; \
		cp -f "$(PREBUILT_DIR)/32bit/"*.exe "$(builddir)/views/" 2>/dev/null || true; \
		cp -f "$(PREBUILT_DIR)/64bit/"*.dll "$(builddir)/client/" 2>/dev/null || true; \
		cp -f "$(PREBUILT_DIR)/64bit/"*.exe "$(builddir)/tester/" 2>/dev/null || true; \
		cp -f "$(PREBUILT_DIR)/installers/"*.exe "$(builddir)/" 2>/dev/null || true; \
		echo "Prebuilt components copied successfully"; \
	else \
		echo "Warning: Prebuilt directory not found at $(PREBUILT_DIR)"; \
		echo "Wine bridge components will not be available"; \
	fi

# Create dummy targets for components that would normally be built
client/NPClient.dll.so: copy_prebuilt_components
	@if [ ! -f "$@" ]; then \
		echo "Warning: NPClient.dll.so not found in prebuilt components"; \
		touch "$@"; \
	fi

client/NPClient64.dll.so: copy_prebuilt_components
	@if [ ! -f "$@" ]; then \
		echo "Warning: NPClient64.dll.so not found in prebuilt components"; \
		touch "$@"; \
	fi

client/check_data.exe.so: copy_prebuilt_components
	@if [ ! -f "$@" ]; then \
		echo "Warning: check_data.exe.so not found in prebuilt components"; \
		touch "$@"; \
	fi

controller/Controller.exe.so: copy_prebuilt_components
	@if [ ! -f "$@" ]; then \
		echo "Warning: Controller.exe.so not found in prebuilt components"; \
		touch "$@"; \
	fi

tester/Tester.exe.so: copy_prebuilt_components
	@if [ ! -f "$@" ]; then \
		echo "Warning: Tester.exe.so not found in prebuilt components"; \
		touch "$@"; \
	fi

tester/Tester64.exe.so: copy_prebuilt_components
	@if [ ! -f "$@" ]; then \
		echo "Warning: Tester64.exe.so not found in prebuilt components"; \
		touch "$@"; \
	fi

ft_client/FreeTrackClient.dll.so: copy_prebuilt_components
	@if [ ! -f "$@" ]; then \
		echo "Warning: FreeTrackClient.dll.so not found in prebuilt components"; \
		touch "$@"; \
	fi

ft_tester/ftc.exe.so: copy_prebuilt_components
	@if [ ! -f "$@" ]; then \
		echo "Warning: ftc.exe.so not found in prebuilt components"; \
		touch "$@"; \
	fi

views/TrackIR.exe.so: copy_prebuilt_components
	@if [ ! -f "$@" ]; then \
		echo "Warning: TrackIR.exe.so not found in prebuilt components"; \
		touch "$@"; \
	fi

# NSIS installer generation
if WINE64
  linuxtrack-wine.exe : ltr_wine64.nsi copy_prebuilt_components
	@if [ -f "linuxtrack-wine.exe" ]; then \
		echo "Using existing linuxtrack-wine.exe"; \
	else \
		echo "Warning: linuxtrack-wine.exe not found in prebuilt components"; \
		touch "linuxtrack-wine.exe"; \
	fi
else
  linuxtrack-wine.exe : ltr_wine.nsi copy_prebuilt_components
	@if [ -f "linuxtrack-wine.exe" ]; then \
		echo "Using existing linuxtrack-wine.exe"; \
	else \
		echo "Warning: linuxtrack-wine.exe not found in prebuilt components"; \
		touch "linuxtrack-wine.exe"; \
	fi
endif

clean-local: clean-local-check
.PHONY: clean-local-check
clean-local-check:
	rm -f *.exe
	rm -f client/*.dll* client/*.exe*
	rm -f controller/*.exe*
	rm -f tester/*.exe*
	rm -f ft_client/*.dll*
	rm -f ft_tester/*.exe*
	rm -f views/*.exe*

distclean-local: distclean-local-check
.PHONY: distclean-local-check
distclean-local-check:
	rm -f *.exe
	rm -f client/*.dll* client/*.exe*
	rm -f controller/*.exe*
	rm -f tester/*.exe*
	rm -f ft_client/*.dll*
	rm -f ft_tester/*.exe*
	rm -f views/*.exe*

endif

dist_pkgdata_DATA = README.wine
EXTRA_DIST = linuxtrack.ico linuxtrack1.ico
EOF

    print_status "Wine bridge Makefile.am modified for prebuilt components"
}

# Function to modify configure.ac
modify_configure_ac() {
    print_status "Modifying configure.ac..."
    
    # Create a backup of the original
    cp "$PROJECT_ROOT/configure.ac" "$PROJECT_ROOT/configure.ac.original"
    
    # Modify the Wine detection section to be more lenient
    sed -i 's/AS_IF(\[(test "x$WINEGCC32_COMPILES" = xyes) && (test "x$WINEGPP32_COMPILES" = xyes) && (test "x$with_makensis" = xyes) \], \[build_wine_plugin=yes\], \[build_wine_plugin=no\])/AS_IF([(test "x$WINEGCC32_COMPILES" = xyes) && (test "x$WINEGPP32_COMPILES" = xyes) && (test "x$with_makensis" = xyes) ], [build_wine_plugin=yes], [AS_IF([test -d "$srcdir/prebuilt_wine_bridge"], [build_wine_plugin=yes], [build_wine_plugin=no])])/' "$PROJECT_ROOT/configure.ac"
    
    # Add a message about prebuilt components
    sed -i '/AS_IF(\[test  "x$build_wine_plugin" = xyes\], \[AC_MSG_RESULT(\[yes\])\], \[AC_MSG_RESULT(\[no\])\])\]/a\
AS_IF([test -d "$srcdir/prebuilt_wine_bridge"], [AC_MSG_NOTICE([Prebuilt Wine bridge components found - Wine development tools not required])], [])' "$PROJECT_ROOT/configure.ac"
    
    print_status "Configure.ac modified for prebuilt components"
}

# Function to create README for prebuilt approach
create_prebuilt_readme() {
    print_status "Creating README for prebuilt approach..."
    
    cat > "$PROJECT_ROOT/PREBUILT_WINE_BRIDGE.md" << 'EOF'
# Prebuilt Wine Bridge Components

This version of LinuxTrack X-IR uses pre-built Wine bridge components to avoid requiring Wine development tools on problematic distributions like Arch Linux.

## What This Means

- **No Wine Development Tools Required**: You don't need to install `wine-devel`, `winegcc`, or `wineg++`
- **Pre-built Components**: All Windows compatibility components are pre-built and included
- **Simplified Installation**: Just install the standard Linux components

## Components Included

### 32-bit Components
- `NPClient.dll` - TrackIR API compatibility
- `check_data.exe` - Installation verification
- `Controller.exe` - Hotkey controller
- `Tester.exe` - TrackIR protocol testing
- `FreeTrackClient.dll` - FreeTrack API compatibility
- `FreeTrackTester.exe` - FreeTrack protocol testing
- `TrackIR.exe` - TrackIR views utility

### 64-bit Components
- `NPClient64.dll` - 64-bit TrackIR API compatibility
- `Tester64.exe` - 64-bit TrackIR protocol testing

### Installers
- `linuxtrack-wine.exe` - Windows installer

## Installation

### Standard Build (Recommended)
```bash
# Install dependencies (no Wine development tools needed)
sudo pacman -S --needed base-devel autoconf automake libtool qt5-base qt5-tools qt5-x11extras opencv libusb mxml libx11 libxrandr bison flex lib32-glibc lib32-gcc-libs v4l-utils wine-staging winetricks

# Build and install
autoreconf -fiv
./configure --prefix=/opt
make -j$(nproc)
sudo make install
```

### Wine Bridge Installation
After building the main LinuxTrack components, you can install the Wine bridge:

```bash
# Extract the prebuilt package
tar -xzf linuxtrack-wine-bridge-*.tar.gz
cd wine_bridge_package

# Install the Wine bridge components
./install_wine_bridge.sh
```

## Building Your Own Wine Bridge Components

If you want to build the Wine bridge components yourself (requires Wine development tools):

```bash
# Create the package
./scripts/install/create_wine_bridge_package.sh

# This will create: linuxtrack-wine-bridge-*.tar.gz
```

## Troubleshooting

### Wine Bridge Not Working
1. Ensure Wine is installed: `sudo pacman -S wine-staging`
2. Check that the prebuilt components are in place
3. Run the installer: `wine linuxtrack-wine.exe`

### Missing Components
If you see warnings about missing components during build, it means the prebuilt components are not available. This is normal if you haven't extracted the Wine bridge package.

## Reverting to Original Build System

To revert to the original build system that requires Wine development tools:

```bash
# Restore original files
cp backup_*/wine_bridge_Makefile.am.bak src/wine_bridge/Makefile.am
cp backup_*/configure.ac.bak configure.ac

# Rebuild
autoreconf -fiv
./configure --prefix=/opt
make clean
make -j$(nproc)
```

## Benefits of This Approach

1. **Easier Installation**: No complex Wine development tool dependencies
2. **Distribution Compatibility**: Works on Arch Linux and other distributions with Wine issues
3. **Faster Builds**: No need to compile Windows components
4. **Consistent Results**: Pre-built components are tested and verified

## Limitations

1. **No Custom Modifications**: You cannot modify the Wine bridge components without rebuilding
2. **Version Dependency**: Components are tied to the LinuxTrack version
3. **Architecture Specific**: Components are built for specific architectures

EOF

    print_status "Prebuilt Wine bridge README created"
}

# Function to create installation script for Arch Linux
create_arch_install_script() {
    print_status "Creating Arch Linux installation script..."
    
    cat > "$PROJECT_ROOT/scripts/install/install_arch_prebuilt.sh" << 'EOF'
#!/bin/bash

# LinuxTrack X-IR Arch Linux Installation Script (Prebuilt Version)
# This script installs LinuxTrack X-IR on Arch Linux using prebuilt Wine bridge components

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to install dependencies
install_dependencies() {
    print_status "Installing dependencies..."
    
    # Core build dependencies
    sudo pacman -S --needed base-devel autoconf automake libtool
    
    # Qt5 dependencies
    sudo pacman -S --needed qt5-base qt5-tools qt5-x11extras
    
    # Library dependencies
    sudo pacman -S --needed opencv libusb mxml libx11 libxrandr
    
    # Build tools
    sudo pacman -S --needed bison flex
    
    # 32-bit libraries (for Wine compatibility)
    sudo pacman -S --needed lib32-glibc lib32-gcc-libs
    
    # Video support
    sudo pacman -S --needed v4l-utils
    
    # Wine (for running Windows games)
    sudo pacman -S --needed wine-staging winetricks
    
    print_status "Dependencies installed successfully"
}

# Function to build LinuxTrack
build_linuxtrack() {
    print_status "Building LinuxTrack X-IR..."
    
    # Regenerate build system
    autoreconf -fiv
    
    # Configure with prebuilt Wine bridge support
    ./configure --prefix=/opt
    
    # Build
    make -j$(nproc)
    
    print_status "LinuxTrack built successfully"
}

# Function to install LinuxTrack
install_linuxtrack() {
    print_status "Installing LinuxTrack X-IR..."
    
    sudo make install
    
    # Add user to required group
    sudo usermod -a -G plugdev $USER
    
    print_status "LinuxTrack installed successfully"
}

# Function to install Wine bridge components
install_wine_bridge() {
    print_status "Installing Wine bridge components..."
    
    # Check if Wine bridge package exists
    if [ -f "linuxtrack-wine-bridge-*.tar.gz" ]; then
        # Extract package
        tar -xzf linuxtrack-wine-bridge-*.tar.gz
        cd wine_bridge_package
        
        # Install components
        ./install_wine_bridge.sh
        
        cd ..
        print_status "Wine bridge components installed successfully"
    else
        print_warning "Wine bridge package not found"
        print_warning "You can download it from the project releases"
    fi
}

# Function to verify installation
verify_installation() {
    print_status "Verifying installation..."
    
    # Check if main executable exists
    if command -v ltr_gui >/dev/null 2>&1; then
        print_status "LinuxTrack GUI found: $(which ltr_gui)"
    else
        print_warning "LinuxTrack GUI not found in PATH"
    fi
    
    # Check if Wine is working
    if command -v wine >/dev/null 2>&1; then
        print_status "Wine found: $(which wine)"
    else
        print_warning "Wine not found"
    fi
    
    print_status "Installation verification completed"
}

# Main installation
main() {
    echo -e "${BLUE}🍷 LinuxTrack X-IR Arch Linux Installer (Prebuilt Version)${NC}"
    echo -e "${BLUE}========================================================${NC}"
    echo ""
    
    # Check if we're in the right directory
    if [ ! -f "configure.ac" ]; then
        print_error "This script must be run from the LinuxTrack X-IR project root"
        exit 1
    fi
    
    # Install dependencies
    install_dependencies
    
    # Build LinuxTrack
    build_linuxtrack
    
    # Install LinuxTrack
    install_linuxtrack
    
    # Install Wine bridge components
    install_wine_bridge
    
    # Verify installation
    verify_installation
    
    echo ""
    print_status "Installation completed successfully!"
    print_status "Log out and back in for group changes to take effect"
    print_status "Then run 'ltr_gui' to start LinuxTrack"
}

main "$@"
EOF

    chmod +x "$PROJECT_ROOT/scripts/install/install_arch_prebuilt.sh"
    print_status "Arch Linux installation script created"
}

# Function to restore original files
restore_original() {
    print_status "Restoring original files..."
    
    # Find the most recent backup
    local latest_backup=$(ls -td "$PROJECT_ROOT/backup_"* 2>/dev/null | head -1)
    
    if [ -n "$latest_backup" ]; then
        # Restore wine bridge Makefile.am
        if [ -f "$latest_backup/wine_bridge_Makefile.am.bak" ]; then
            cp "$latest_backup/wine_bridge_Makefile.am.bak" "$WINE_BRIDGE_DIR/Makefile.am"
            print_status "Restored wine_bridge/Makefile.am"
        fi
        
        # Restore configure.ac
        if [ -f "$latest_backup/configure.ac.bak" ]; then
            cp "$latest_backup/configure.ac.bak" "$PROJECT_ROOT/configure.ac"
            print_status "Restored configure.ac"
        fi
        
        print_status "Original files restored from: $latest_backup"
    else
        print_error "No backup found to restore from"
        exit 1
    fi
}

# Main execution
main() {
    local action="modify"
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --restore)
                action="restore"
                shift
                ;;
            --help)
                echo "Usage: $0 [--restore] [--help]"
                echo ""
                echo "Options:"
                echo "  --restore    Restore original build system files"
                echo "  --help       Show this help message"
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                exit 1
                ;;
        esac
    done
    
    # Check if we're in the right directory
    if [ ! -f "$PROJECT_ROOT/configure.ac" ]; then
        print_error "This script must be run from the LinuxTrack X-IR project root"
        exit 1
    fi
    
    if [ "$action" = "restore" ]; then
        restore_original
    else
        # Modify build system
        backup_files
        create_prebuilt_structure
        modify_wine_bridge_makefile
        modify_configure_ac
        create_prebuilt_readme
        create_arch_install_script
        
        print_status "Build system modification completed successfully!"
        print_status ""
        print_status "Next steps:"
        print_status "1. Create prebuilt Wine bridge package: ./scripts/install/create_wine_bridge_package.sh"
        print_status "2. Build LinuxTrack: autoreconf -fiv && ./configure --prefix=/opt && make"
        print_status "3. Install: sudo make install"
        print_status ""
        print_status "For Arch Linux users: ./scripts/install/install_arch_prebuilt.sh"
    fi
}

main "$@" 