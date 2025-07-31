# Arch Linux Wine Bridge Issues and Solutions

## Overview

This document details the investigation and solutions for wine bridge installation issues on Arch Linux, specifically related to 32-bit winegcc compilation and executable compatibility problems.

## Problem Description

### Core Issue
The wine bridge installer fails to work properly on Arch Linux due to fundamental differences in how 32-bit wine libraries are organized compared to other distributions (Fedora, Debian, Ubuntu).

### Symptoms
1. **NSIS Installer Fails**: `check_data.exe` fails to run with error `c000007b` (not a valid Win32 application)
2. **All Executables Broken**: All wine bridge executables are 32-bit Linux ELF binaries instead of Windows PE executables
3. **Symlinks Not Created**: The critical symlinks to `TIRViews.dll` and `mfc140u.dll` are not created
4. **Build System Issues**: winegcc 32-bit compilation fails due to missing library paths

## Technical Investigation

### File Analysis Results

#### Executables (All Broken)
```bash
# All these are 32-bit Linux ELF binaries, not Windows PE executables
check_data.exe:     ELF 32-bit LSB shared object
Controller.exe:     ELF 32-bit LSB shared object  
Tester.exe:         ELF 32-bit LSB shared object
TrackIR.exe:        ELF 32-bit LSB shared object
FreeTrackTester.exe: ELF 32-bit LSB shared object
```

#### DLLs (All Broken)
```bash
# All these are 32-bit Linux ELF binaries, not Windows DLLs
NPClient.dll:       ELF 32-bit LSB shared object
FreeTrackClient.dll: ELF 32-bit LSB shared object
```

#### Symlinks (Working)
```bash
# These work correctly when manually created
TIRViews.dll -> /home/user/.config/linuxtrack/tir_firmware/TIRViews.dll
mfc140u.dll -> /home/user/.config/linuxtrack/tir_firmware/mfc140u.dll
```

### Root Cause Analysis

#### 1. Wine Library Path Issues
**Problem**: winegcc can't find 32-bit wine libraries in expected locations
- **Expected**: `/usr/lib32/` with wine library symlinks
- **Actual**: Libraries in `/usr/lib/wine/i386-windows/` but not symlinked in `/usr/lib32/`

#### 2. Build System Configuration
**Problem**: The build system doesn't specify the correct wine library path for Arch Linux
- **Other distros**: Libraries in standard locations
- **Arch Linux**: Libraries in `/usr/lib/wine/i386-windows/` need explicit path specification

#### 3. Wine Package Differences
**Problem**: Arch Linux uses different wine package names and structure
- **Standard**: `wine`, `wine-mono`
- **Arch Linux**: `wine-stable`, `wine-stable-mono` (from AUR)

## Solutions Found

### Solution 1: GitHub Issue Analysis
From [GitHub Issue #206](https://github.com/uglyDwarf/linuxtrack/issues/206), the solution involves:

#### Required Packages
```bash
# Install from AUR
yay -S wine-stable wine-stable-mono
```

#### Build Configuration
```bash
# Key configure flag for Arch Linux
./configure --prefix=$HOME/linuxtrack --with-wine-libs=-L/usr/lib/wine
```

#### Verification
After successful build, you should see:
```
checking Webcam support... yes
checking Wiimote support... no
checking TrackIR support... yes
checking Facetracker support... yes
checking XPlane plugin... no
checking Mickey... yes
checking Wine plugin... yes  ← This is critical
checking OSC support... yes
```

### Solution 2: Manual Library Symlinks
Create missing 32-bit wine library symlinks:

```bash
# Create symlinks in /usr/lib32/
sudo ln -sf /usr/lib/wine/i386-windows/libadvapi32.so /usr/lib32/libadvapi32.so
sudo ln -sf /usr/lib/wine/i386-windows/libuser32.so /usr/lib32/libuser32.so
sudo ln -sf /usr/lib/wine/i386-windows/libkernel32.so /usr/lib32/libkernel32.so
sudo ln -sf /usr/lib/wine/i386-windows/libntdll.so /usr/lib32/libntdll.so
sudo ln -sf /usr/lib/wine/i386-windows/libwinecrt0.a /usr/lib32/libwinecrt0.a
```

### Solution 3: Manual Symlink Creation
Bypass the broken `check_data.exe` by manually creating the required symlinks:

```bash
# Create symlinks in wine bridge installation directory
cd ~/.wine/drive_c/Program\ Files\ \(x86\)/Linuxtrack/
ln -sf ~/.config/linuxtrack/tir_firmware/TIRViews.dll TIRViews.dll
ln -sf ~/.config/linuxtrack/tir_firmware/mfc140u.dll mfc140u.dll
```

## What `check_data.exe` Does

### Critical Functions
1. **Symlink Creation**: Creates `TIRViews.dll` and `mfc140u.dll` symlinks
2. **Firmware Validation**: Checks if `gamedata.txt` exists and can be parsed
3. **Poetry Validation**: Reads `poem1.txt` and `poem2.txt` (firmware signatures)
4. **User Feedback**: Shows error messages if firmware files are missing

### Code Location
- **File**: `src/wine_bridge/client/check_data.c`
- **Functions**: `game_data_get_desc()`, `getSomeSeriousPoetry()`
- **NSIS Integration**: Called from `ltr_wine.nsi` line 75

## Alternative Approaches

### Option 1: AppImage Integration
Modify AppImage build to create symlinks during installation instead of relying on `check_data.exe`:

```bash
# Add to AppImage installation script
create_wine_bridge_symlinks() {
    local wine_prefix="$1"
    local linuxtrack_dir="$wine_prefix/drive_c/Program Files (x86)/Linuxtrack"
    local config_dir="$HOME/.config/linuxtrack/tir_firmware"
    
    if [ -d "$config_dir" ]; then
        mkdir -p "$linuxtrack_dir"
        ln -sf "$config_dir/TIRViews.dll" "$linuxtrack_dir/TIRViews.dll"
        ln -sf "$config_dir/mfc140u.dll" "$linuxtrack_dir/mfc140u.dll"
    fi
}
```

### Option 2: Cross-Distribution Build
Build wine bridge on a working distribution (Fedora/Debian) and package the working executables for Arch Linux.

### Option 3: Native Linux Alternative
Develop a native Linux alternative to the wine bridge that doesn't require Windows executables.

## Testing and Verification

### Test Commands
```bash
# Test wine bridge installation
wine ~/.wine/drive_c/Program\ Files\ \(x86\)/Linuxtrack/check_data.exe

# Test symlink creation
ls -la ~/.wine/drive_c/Program\ Files\ \(x86\)/Linuxtrack/ | grep -E "(TIRViews|mfc)"

# Test winegcc 32-bit compilation
winegcc -m32 -o test.exe.so test.c
```

### Expected Results
- **Working**: Symlinks created, no error messages
- **Broken**: Error `c000007b` or segfaults

## Distribution-Specific Notes

### Arch Linux
- **Issue**: 32-bit wine libraries not in expected locations
- **Solution**: Use `wine-stable` packages and correct library paths
- **Status**: Requires manual intervention

### Fedora/Debian/Ubuntu
- **Issue**: None (works out of the box)
- **Solution**: Standard wine packages sufficient
- **Status**: Fully functional

## Recommendations

### For AppImage Development
1. **Pre-create symlinks** during AppImage installation
2. **Include working wine bridge** built on compatible distribution
3. **Add Arch Linux detection** and apply fixes automatically

### For Users
1. **Install wine-stable packages** from AUR
2. **Use manual symlink creation** as temporary workaround
3. **Report issues** to maintainers for proper fixes

### For Developers
1. **Fix build system** to detect Arch Linux and use correct paths
2. **Add wine library path detection** in configure script
3. **Test on multiple distributions** before releases

## Related Files

### Source Code
- `src/wine_bridge/client/check_data.c` - Main symlink creation logic
- `src/wine_bridge/ltr_wine.nsi` - NSIS installer configuration
- `src/wine_bridge/client/Makefile.am` - Build configuration

### Scripts
- `scripts/fix_wine_bridge_arch.sh` - Arch Linux fix script
- `scripts/build_arch_linux.sh` - Arch Linux build script
- `scripts/create_wine_bridge_symlinks.sh` - Manual symlink creation

### Documentation
- `docs/guides/ARCH_LINUX_INSTALL_GUIDE.md` - Installation guide
- `docs/troubleshooting/ARCH_LINUX_WINE_BRIDGE_FIX.md` - Troubleshooting guide

## References

- [GitHub Issue #206](https://github.com/uglyDwarf/linuxtrack/issues/206) - Original Arch Linux issue
- [AUR wine-stable-mono](https://aur.archlinux.org/packages/wine-stable-mono) - Required wine package
- [Wine Development Documentation](https://wiki.winehq.org/Development) - Wine build system details

---

**Last Updated**: $(date)
**Investigation Date**: $(date)
**Status**: Issue identified, solutions documented, implementation pending 