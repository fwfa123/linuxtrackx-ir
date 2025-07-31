#!/bin/bash

# LinuxTrack Wine Bridge Multi-Distribution Build Script
# This script builds wine bridge components that work across multiple distributions

set -e

echo "=== LinuxTrack Wine Bridge Multi-Distribution Build ==="
echo "Date: $(date)"
echo

# Check wine installation
echo "=== Checking Wine Installation ==="
if ! command -v wine >/dev/null 2>&1; then
    echo "ERROR: Wine not installed"
    exit 1
fi

if ! command -v winegcc >/dev/null 2>&1; then
    echo "ERROR: winegcc not found"
    exit 1
fi

echo "Wine installation OK: $(wine --version)"
echo

# Detect distribution
echo "=== Detecting Distribution ==="
DISTRO="unknown"
if [ -f "/etc/arch-release" ]; then
    DISTRO="arch"
    echo "Detected: Arch Linux"
elif [ -f "/etc/debian_version" ]; then
    DISTRO="debian"
    echo "Detected: Debian/Ubuntu"
elif [ -f "/etc/fedora-release" ]; then
    DISTRO="fedora"
    echo "Detected: Fedora"
elif [ -f "/etc/SuSE-release" ] || ( [ -f "/etc/os-release" ] && grep -q "openSUSE" /etc/os-release ); then
    DISTRO="opensuse"
    echo "Detected: OpenSUSE"
else
    echo "WARNING: Unknown distribution, using generic paths"
fi

echo

# Find wine library paths for current distribution
echo "=== Finding Wine Library Paths ==="

# Common wine paths for different distributions
case "$DISTRO" in
    "arch")
        WINE_PATHS=(
            "/usr/lib/wine/i386-unix"
            "/usr/lib32/wine/i386-unix"
            "/usr/lib/wine"
            "/usr/lib32/wine"
        )
        ;;
    "debian")
        WINE_PATHS=(
            "/usr/lib/i386-linux-gnu/wine"
            "/usr/lib/x86_64-linux-gnu/wine"
            "/usr/lib/wine"
        )
        ;;
    "fedora")
        WINE_PATHS=(
            "/usr/lib/wine"
            "/usr/lib64/wine"
        )
        ;;
    "opensuse")
        WINE_PATHS=(
            "/usr/lib/wine"
            "/usr/lib64/wine"
        )
        ;;
    *)
        # Generic fallback
        WINE_PATHS=(
            "/usr/lib/i386-linux-gnu/wine"
            "/usr/lib/x86_64-linux-gnu/wine"
            "/usr/lib/wine/i386-unix"
            "/usr/lib32/wine/i386-unix"
            "/usr/lib/wine"
            "/usr/lib32/wine"
            "/usr/lib64/wine"
        )
        ;;
esac

WINE_LIBS_PATH=""
for path in "${WINE_PATHS[@]}"; do
    if [ -d "$path" ]; then
        echo "Found wine library path: $path"
        WINE_LIBS_PATH="$path"
        break
    fi
done

if [ -z "$WINE_LIBS_PATH" ]; then
    echo "ERROR: Could not find wine library path"
    exit 1
fi

echo "Using wine library path: $WINE_LIBS_PATH"
echo

# Create backup
echo "=== Creating Backup ==="
BACKUP_DIR="wine_bridge_multi_backup_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$BACKUP_DIR"

if [ -f "src/wine_bridge/client/check_data.exe.so" ]; then
    cp "src/wine_bridge/client/check_data.exe.so" "$BACKUP_DIR/"
    echo "Backed up check_data.exe.so"
fi

if [ -f "src/wine_bridge/client/NPClient.dll.so" ]; then
    cp "src/wine_bridge/client/NPClient.dll.so" "$BACKUP_DIR/"
    echo "Backed up NPClient.dll.so"
fi

echo "Backup created in: $BACKUP_DIR"
echo

# Build wine bridge components
echo "=== Building Wine Bridge Components ==="
cd "src/wine_bridge/client"

echo "Cleaning previous build..."
make clean 2>/dev/null || true

echo "Building with wine library path: $WINE_LIBS_PATH"

# Create Makefile with current distribution's wine paths
cat > Makefile.multi << EOF
# Multi-distribution Makefile for wine bridge
WINE_LIBS = -L$WINE_LIBS_PATH
CC = winegcc
CXX = wineg++
CFLAGS = -g -O2 -Wall -Wextra -m32
CXXFLAGS = -g -O2 -Wall -Wextra -m32
LDFLAGS = -m32 -Wall -Wextra -g
CPPFLAGS = -I. -I../.. -I../../..

all: check_data.exe.so NPClient.dll.so

check_data.exe.so: check_data.o rest.o
	\$(CC) \$(WINE_LIBS) \$(LDFLAGS) -o \$@ \$^

NPClient.dll.so: NPClient.spec NPClient_main.o linuxtrack.o rest.o
	\$(CC) \$(WINE_LIBS) \$(LDFLAGS) -o \$@ -shared \$^ -ldl

check_data.o: check_data.c rest.h
	\$(CC) -c \$(CFLAGS) \$(CPPFLAGS) -o \$@ \$<

rest.o: rest.c rest.h
	\$(CC) -c \$(CFLAGS) \$(CPPFLAGS) -o \$@ \$<

NPClient_main.o: NPClient_main.c NPClient_dll.h
	\$(CC) -c \$(CFLAGS) \$(CPPFLAGS) -o \$@ \$<

linuxtrack.o: linuxtrack.c
	\$(CC) -c \$(CFLAGS) \$(CPPFLAGS) -o \$@ \$<

clean:
	rm -f *.o *.so *.exe* Makefile.multi

.PHONY: all clean
EOF

echo "Building wine bridge components..."
if make -f Makefile.multi; then
    echo "SUCCESS: Wine bridge components built successfully"
else
    echo "ERROR: Build failed"
    cd - > /dev/null
    exit 1
fi

# Clean up temporary Makefile
rm -f Makefile.multi

cd - > /dev/null

echo

# Test the built executable
echo "=== Testing Built Executable ==="
if [ -f "src/wine_bridge/client/check_data.exe.so" ]; then
    echo "Testing check_data.exe.so..."
    
    # Set up test environment
    export WINEDEBUG=-all
    export WINEPREFIX="${HOME}/.wine_test_multi"
    
    # Clean up any existing test prefix
    rm -rf "$WINEPREFIX"
    
    echo "Using test WINEPREFIX: $WINEPREFIX"
    
    # Try to run the executable
    if timeout 30s wine "src/wine_bridge/client/check_data.exe.so" 2>&1; then
        echo "SUCCESS: check_data.exe.so runs successfully on $DISTRO!"
    else
        echo "WARNING: check_data.exe.so has issues on $DISTRO"
        echo "Exit code: $?"
    fi
    
    # Clean up test prefix
    rm -rf "$WINEPREFIX"
else
    echo "ERROR: check_data.exe.so not found"
fi

echo

# Create distribution-specific build instructions
echo "=== Creating Distribution-Specific Build Instructions ==="

cat > "scripts/build_wine_bridge_instructions.md" << EOF
# Wine Bridge Multi-Distribution Build Instructions

## Current Build
- **Distribution**: $DISTRO
- **Wine Library Path**: $WINE_LIBS_PATH
- **Build Date**: $(date)

## Cross-Distribution Compatibility

The wine bridge components are currently built for **$DISTRO** with wine library path:
\`$WINE_LIBS_PATH\`

## Building for Different Distributions

### For Arch Linux
\`\`\`bash
# On Arch Linux system
./scripts/build_wine_bridge_multi_distro.sh
\`\`\`

### For Debian/Ubuntu
\`\`\`bash
# On Debian/Ubuntu system
./scripts/build_wine_bridge_multi_distro.sh
\`\`\`

### For Fedora
\`\`\`bash
# On Fedora system
./scripts/build_wine_bridge_multi_distro.sh
\`\`\`

## Wine Library Paths by Distribution

| Distribution | 32-bit Path | 64-bit Path |
|--------------|-------------|-------------|
| Arch Linux | \`/usr/lib/wine/i386-unix\` | \`/usr/lib/wine/x86_64-unix\` |
| Debian/Ubuntu | \`/usr/lib/i386-linux-gnu/wine\` | \`/usr/lib/x86_64-linux-gnu/wine\` |
| Fedora | \`/usr/lib/wine\` | \`/usr/lib64/wine\` |
| OpenSUSE | \`/usr/lib/wine\` | \`/usr/lib64/wine\` |

## Testing

After building on each distribution, test the components:

\`\`\`bash
# Test the built component
wine src/wine_bridge/client/check_data.exe.so

# Test the wine bridge installer
wine src/wine_bridge/linuxtrack-wine.exe
\`\`\`

## AppImage Considerations

**IMPORTANT**: The current AppImage contains wine bridge components built for **$DISTRO**.

For maximum compatibility, consider:
1. Building separate AppImages for each major distribution
2. Using dynamic library path detection at runtime
3. Including multiple wine library path fallbacks

## Runtime Library Path Detection

For better cross-distribution compatibility, the wine bridge components could be enhanced to:
1. Detect the current distribution at runtime
2. Search for wine libraries in multiple paths
3. Use fallback mechanisms for missing libraries
EOF

echo "Created build instructions: scripts/build_wine_bridge_instructions.md"
echo

# Create a universal build script
echo "=== Creating Universal Build Script ==="

cat > "scripts/build_wine_bridge_universal.sh" << 'EOF'
#!/bin/bash

# Universal wine bridge build script
# This script attempts to build wine bridge components with universal compatibility

set -e

echo "=== Universal Wine Bridge Build ==="

# Try multiple wine library paths
WINE_PATHS=(
    "/usr/lib/i386-linux-gnu/wine"
    "/usr/lib/x86_64-linux-gnu/wine"
    "/usr/lib/wine/i386-unix"
    "/usr/lib32/wine/i386-unix"
    "/usr/lib/wine"
    "/usr/lib32/wine"
    "/usr/lib64/wine"
)

WINE_LIBS_PATH=""
for path in "${WINE_PATHS[@]}"; do
    if [ -d "$path" ]; then
        echo "Found wine library path: $path"
        WINE_LIBS_PATH="$path"
        break
    fi
done

if [ -z "$WINE_LIBS_PATH" ]; then
    echo "ERROR: Could not find any wine library path"
    exit 1
fi

echo "Using wine library path: $WINE_LIBS_PATH"

# Build with the found path
cd "src/wine_bridge/client"

# Create universal Makefile
cat > Makefile.universal << EOF
WINE_LIBS = -L$WINE_LIBS_PATH
CC = winegcc
CFLAGS = -g -O2 -Wall -Wextra -m32
LDFLAGS = -m32 -Wall -Wextra -g
CPPFLAGS = -I. -I../.. -I../../..

check_data.exe.so: check_data.o rest.o
	\$(CC) \$(WINE_LIBS) \$(LDFLAGS) -o \$@ \$^

check_data.o: check_data.c rest.h
	\$(CC) -c \$(CFLAGS) \$(CPPFLAGS) -o \$@ \$<

rest.o: rest.c rest.h
	\$(CC) -c \$(CFLAGS) \$(CPPFLAGS) -o \$@ \$<
EOF

make -f Makefile.universal
rm -f Makefile.universal

echo "Universal build completed"
EOF

chmod +x "scripts/build_wine_bridge_universal.sh"
echo "Created universal build script: scripts/build_wine_bridge_universal.sh"
echo

echo "=== Build Complete ==="
echo "Wine bridge components built for $DISTRO with wine library path: $WINE_LIBS_PATH"
echo
echo "IMPORTANT NOTES:"
echo "1. This build is optimized for $DISTRO"
echo "2. For cross-distribution compatibility, build on each target distribution"
echo "3. Consider using the universal build script for better compatibility"
echo
echo "Next steps:"
echo "1. Test the components: wine src/wine_bridge/client/check_data.exe.so"
echo "2. Rebuild wine bridge installer: cd src/wine_bridge && make linuxtrack-wine.exe"
echo "3. Rebuild AppImage: ./scripts/appimage/build_appimage_phase4.sh"
echo
echo "For cross-distribution builds, see: scripts/build_wine_bridge_instructions.md" 