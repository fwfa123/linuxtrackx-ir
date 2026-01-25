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

### Solution 1: Build from source on Arch (recommended)

Build on Arch so the wine bridge is compiled with Arch’s wine paths. The project uses **CMake**; `cmake/FindWineLibs.cmake` detects `/usr/lib32/wine/i386-unix` and `/usr/lib/wine/x86_64-unix`. Ensure multilib and `wine`, `lib32-glibc`, `lib32-gcc-libs` are installed, then:

```bash
cd linuxtrackx-ir
rm -rf build && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON
# If FindWineLibs does not find paths, set explicitly:
# cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON \
#   -DWINE_LIBS_PATH=/usr/lib32/wine/i386-unix \
#   -DWINE64_LIBS_PATH=/usr/lib/wine/x86_64-unix
cmake --build . -j$(nproc)
sudo cmake --install .
```

Use the built `linuxtrack-wine.exe` from the install (e.g. `/opt/share/linuxtrack/`) or rebuild the AppImage on Arch: `./scripts/appimage/build_appimage_phase4.sh`.

### Solution 2: Override Wine paths in CMake

If the auto-detected paths are wrong:

```bash
ls -la /usr/lib32/wine/i386-unix   # or /usr/lib32/wine
ls -la /usr/lib/wine/x86_64-unix   # or /usr/lib/wine
```

Then:

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON \
  -DWINE_LIBS_PATH=/usr/lib32/wine/i386-unix \
  -DWINE64_LIBS_PATH=/usr/lib/wine/x86_64-unix
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Solution 3: AppImage rebuild on Arch

To produce an AppImage whose wine bridge works on Arch, build and run the AppImage script on an Arch system (after a normal CMake build with wine bridge, as in Solution 1):

```bash
./scripts/appimage/build_appimage_phase4.sh
```

The scripts `fix_appimage_wine_bridge.sh`, `test_wine_bridge_arch.sh`, `fix_wine_bridge_arch.sh`, and `verify_wine_bridge_arch.sh` are not part of the current tree; use CMake and the steps above instead.

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

## AppImage workflow when building for Arch

1. **Build on Arch** (Solution 1 or 2) so the wine bridge uses Arch’s paths.
2. **Rebuild AppImage** (on the same Arch system):  
   `./scripts/appimage/build_appimage_phase4.sh`
3. **Test**: run the new AppImage on Arch.

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

1. **Build the wine bridge on the target distro** (or pass correct `-DWINE_LIBS_PATH` / `-DWINE64_LIBS_PATH` when cross-building).
2. **Use `cmake/FindWineLibs.cmake`** for path detection; it knows common Arch paths.
3. **Test on Arch** before publishing an AppImage that includes the wine bridge.

## Relevant files

- `cmake/FindWineLibs.cmake` – detects `/usr/lib32/wine/i386-unix`, `/usr/lib32/wine`, `/usr/lib/wine/x86_64-unix`, etc.
- `src/wine_bridge/` – wine bridge sources; built by CMake with `-DWINE_LIBS_PATH` / `-DWINE64_LIBS_PATH` if needed.

## Related Issues

- Issue #206: TrackIR Arch Linux fixes
- Wine bridge compatibility across distributions
- Wine library path detection improvements
- AppImage cross-distribution compatibility

## Support

If you continue to experience issues:

1. Check the wine version and installed packages (`wine --version`, `ls /usr/lib32/wine/`, `ls /usr/lib/wine/`)
2. Rebuild from source on Arch with Solution 1 or 2
3. Test with a clean wine prefix
4. If using an AppImage, ensure it was built on Arch (or with the correct `WINE_LIBS_PATH`/`WINE64_LIBS_PATH`)

## References

- [Wine Architecture Documentation](https://wiki.winehq.org/Architecture)
- [Arch Linux Wine Package](https://archlinux.org/packages/extra/x86_64/wine/)
- [LinuxTrack Wine Bridge Documentation](../WINE_SUPPORT_MODERN.md)
- [NSIS Installer Documentation](https://nsis.sourceforge.io/) 