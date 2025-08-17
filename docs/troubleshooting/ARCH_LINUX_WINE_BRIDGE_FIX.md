# Arch Linux Wine Bridge Fix Guide

## Problem Description

The LinuxTrack **AppImage runs perfectly** on Arch Linux, but the **wine bridge installer** (`linuxtrack-wine.exe`) extracted from the AppImage fails with error code `c000007b` when trying to execute `check_data.exe` during the NSIS installation process.

## Root Cause

The issue occurs specifically during the **NSIS installer execution** within the wine bridge:

1. ✅ **AppImage runs fine** on Arch Linux
2. ✅ **User extracts wine bridge installer** from AppImage  
3. ✅ **Wine bridge installer starts** (`linuxtrack-wine.exe`)
4. ❌ **NSIS installer tries to run `check_data.exe`** → **c000007b error**
5. ❌ **Wine bridge installation fails**

The `check_data.exe` file inside the wine bridge installer was **built on Debian/Ubuntu** with Debian wine library paths, but when the NSIS installer executes it on Arch Linux, Wine can't find the libraries at the expected locations.

## NSIS Installer Flow

The NSIS installer (`ltr_wine64.nsi`) contains this critical line:

```nsis
Exec '"$INSTDIR\check_data.exe"'
```

This is where the `c000007b` error occurs - when the NSIS installer tries to run the `check_data.exe` component.

## Solutions

### Solution 1: AppImage-Specific Fix (Recommended)

Use the AppImage-specific fix script that rebuilds the wine bridge components before they get packaged:

```bash
# Fix the wine bridge components for AppImage
./scripts/fix_appimage_wine_bridge.sh

# Rebuild the AppImage with fixed components
./scripts/appimage/build_appimage_phase4.sh
```

This script:
- Rebuilds `check_data.exe` with Arch Linux wine library paths
- Tests the rebuilt component with Wine
- Prepares components for AppImage packaging
- Creates verification scripts

### Solution 2: Quick Fix Script

Use the general Arch-specific fix script:

```bash
# Run the diagnostic script first
./scripts/test_wine_bridge_arch.sh

# Run the fix script
./scripts/fix_wine_bridge_arch.sh
```

### Solution 3: Manual Fix

If the scripts don't work, manually fix the wine library paths:

1. **Find your Arch Linux wine paths**:
   ```bash
   ls -la /usr/lib/wine/
   ls -la /usr/lib32/wine/
   ```

2. **Update the build configuration**:
   ```bash
   cd src/wine_bridge/client
   
   # Create Arch-specific Makefile
   cat > Makefile.arch << 'EOF'
   WINE_LIBS = -L/usr/lib/wine/i386-unix
   CC = winegcc
   CFLAGS = -g -O2 -Wall -Wextra -m32
   LDFLAGS = -m32 -Wall -Wextra -g
   
   check_data.exe.so: check_data.o rest.o
       $(CC) $(WINE_LIBS) $(LDFLAGS) -o $@ $^
   
   check_data.o: check_data.c rest.h
       $(CC) -c $(CFLAGS) -o $@ $<
   
   rest.o: rest.c rest.h
       $(CC) -c $(CFLAGS) -o $@ $<
   EOF
   
   # Rebuild
   make -f Makefile.arch
   ```

3. **Rebuild the wine bridge installer**:
   ```bash
   cd ../..
   cd src/wine_bridge
   make linuxtrack-wine.exe
   ```

4. **Rebuild the AppImage**:
   ```bash
   cd ../..
   ./scripts/appimage/build_appimage_phase4.sh
   ```

### Solution 4: Update Configure.ac (Long-term Fix)

For a permanent solution that works across all distributions:

```bash
# Update configure.ac with better distribution detection
./scripts/update_configure_arch.sh

# Regenerate build system
./autogen.sh
./configure

# Rebuild everything
make clean
make
```

## Verification

After applying any fix, verify the solution:

```bash
# Test the rebuilt executable directly
wine src/wine_bridge/client/check_data.exe.so

# Test the wine bridge installer
wine src/wine_bridge/linuxtrack-wine.exe

# Test the new AppImage
./LinuxTrack-X-IR-0.99.23-x86_64.AppImage
```

## AppImage-Specific Workflow

The complete workflow for fixing the AppImage issue:

1. **Fix wine bridge components**:
   ```bash
   ./scripts/fix_appimage_wine_bridge.sh
   ```

2. **Verify the fix**:
   ```bash
   ./scripts/verify_wine_bridge_arch.sh
   ```

3. **Rebuild AppImage**:
   ```bash
   ./scripts/appimage/build_appimage_phase4.sh
   ```

4. **Test on Arch Linux**:
   ```bash
   ./LinuxTrack-X-IR-0.99.23-x86_64.AppImage
   ```

## Troubleshooting

### Wine Version Issues

If you still get errors, check your Wine version:

```bash
wine --version
```

Arch Linux may have a newer Wine version that requires additional configuration.

### Missing Dependencies

Ensure all required wine packages are installed:

```bash
# Arch Linux wine packages
sudo pacman -S wine wine-mono wine-gecko

# Additional packages that might be needed
sudo pacman -S lib32-gnutls lib32-libldap lib32-gpgme
```

### Wine Prefix Issues

Try with a clean wine prefix:

```bash
export WINEPREFIX="${HOME}/.wine_linuxtrack"
rm -rf "$WINEPREFIX"
wine src/wine_bridge/client/check_data.exe.so
```

### Debug Information

Enable wine debugging to get more information:

```bash
export WINEDEBUG=+all
wine src/wine_bridge/client/check_data.exe.so
```

## Prevention

To prevent this issue in future builds:

1. **Use the updated configure.ac** with better distribution detection
2. **Test on multiple distributions** before releasing
3. **Use the Arch-specific build scripts** when building on Arch Linux
4. **Include cross-distribution testing** in the build process

## Files Modified

- `src/wine_bridge/client/check_data.exe.so` - Rebuilt with Arch Linux paths
- `src/wine_bridge/linuxtrack-wine.exe` - Updated wine bridge installer
- `scripts/fix_appimage_wine_bridge.sh` - AppImage-specific fix script
- `scripts/verify_wine_bridge_arch.sh` - Verification script
- `configure.ac` - Updated with better distribution detection

## Related Issues

- Issue #206: TrackIR Arch Linux fixes
- Wine bridge compatibility across distributions
- Wine library path detection improvements
- AppImage cross-distribution compatibility

## Support

If you continue to experience issues:

1. Run the diagnostic script and share the output
2. Check the wine version and installed packages
3. Verify wine library paths on your system
4. Test with a clean wine prefix
5. Ensure the AppImage was rebuilt after fixing the components

## References

- [Wine Architecture Documentation](https://wiki.winehq.org/Architecture)
- [Arch Linux Wine Package](https://archlinux.org/packages/extra/x86_64/wine/)
- [LinuxTrack Wine Bridge Documentation](../WINE_SUPPORT_MODERN.md)
- [NSIS Installer Documentation](https://nsis.sourceforge.io/) 