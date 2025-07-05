# TrackIR Firmware Extraction Troubleshooting Guide

## Overview
This document covers common issues encountered when extracting TrackIR firmware using the linuxtrack GUI tool and provides solutions for each problem.

## Common Error Messages

### "Can't find a blob matching the installer"
**Symptoms:**
- GUI tool fails to extract firmware
- Tool falls back to Wine installation method
- No blob files are used during extraction

**Root Cause:**
- Missing or incorrectly named blob files (`blob_*.bin`)
- Blob files don't match the installer's hash
- Blob extraction method is not being used

**Solutions:**
1. **Check for blob files** in the expected directory
2. **Verify blob file names** match the installer hash
3. **Use manual extraction** as fallback (see below)

### Wine Installation Failures
**Symptoms:**
- Wine installation method fails
- Permission errors during installation
- System DLL writing failures

**Root Causes:**
- Insufficient disk space
- Wine registry compatibility issues
- Permission problems in Wine environment

**Solutions:**
1. **Free up disk space** - ensure adequate free space
2. **Update Wine registry settings** - use Windows 10 instead of Windows 7
3. **Use manual extraction** method

## Manual Firmware Extraction Process

### Step 1: Extract Files Using Wine
```bash
# Create temporary Wine prefix
export WINEPREFIX=/tmp/wine_trackir
winecfg  # Set to Windows 10

# Install TrackIR
wine TrackIR_5.5.3.exe

# Extract firmware files from installed directory
cp "$WINEPREFIX/drive_c/Program Files/NaturalPoint/TrackIR5/"*.txt /tmp/
cp "$WINEPREFIX/drive_c/Program Files/NaturalPoint/TrackIR5/"*.fw.gz /tmp/
cp "$WINEPREFIX/drive_c/Program Files/NaturalPoint/TrackIR5/"*.dll /tmp/
```

### Step 2: Organize Files in Correct Structure
```bash
# Create timestamped folder
mkdir -p ~/.config/linuxtrack/$(date +%y%m%d_%H%M%S)

# Copy all firmware files
cp /tmp/poem*.txt ~/.config/linuxtrack/[timestamped_folder]/
cp /tmp/*.fw.gz ~/.config/linuxtrack/[timestamped_folder]/
cp /tmp/gamedata.txt ~/.config/linuxtrack/[timestamped_folder]/
cp /tmp/mfc42u.dll ~/.config/linuxtrack/[timestamped_folder]/
cp /tmp/TIRViews.dll ~/.config/linuxtrack/[timestamped_folder]/

# Create symlink to latest
cd ~/.config/linuxtrack
ln -sf [timestamped_folder] tir_firmware
```

## Required Firmware Files

The complete set of firmware files includes:

| File | Size | Description |
|------|------|-------------|
| `gamedata.txt` | ~12KB | Game compatibility data |
| `mfc42u.dll` | ~995KB | Microsoft Foundation Classes DLL |
| `poem1.txt` | 106 bytes | Firmware poem 1 |
| `poem2.txt` | 98 bytes | Firmware poem 2 |
| `sn4.fw.gz` | ~48KB | SN4 firmware |
| `tir4.fw.gz` | ~9.7KB | TrackIR 4 firmware |
| `tir5.fw.gz` | ~24KB | TrackIR 5 firmware |
| `tir5v2.fw.gz` | ~24KB | TrackIR 5 v2 firmware |
| `TIRViews.dll` | ~110KB | TrackIR Views DLL |

## File Organization Structure

```
~/.config/linuxtrack/
├── 250705_112800/          # Timestamped folder
│   ├── gamedata.txt
│   ├── mfc42u.dll
│   ├── poem1.txt
│   ├── poem2.txt
│   ├── sn4.fw.gz
│   ├── tir4.fw.gz
│   ├── tir5.fw.gz
│   ├── tir5v2.fw.gz
│   └── TIRViews.dll
├── tir_firmware -> 250705_112800  # Symlink to latest
└── [other config files]
```

## Code Improvements Made

### Windows Registry Compatibility
- Updated from `win7.reg` to `win10.reg` for better compatibility with newer TrackIR installers
- Updated build files to include the new registry file

### Blob Extraction Method
- The preferred method uses pre-created blob files for fast extraction
- Blob files must match the installer's hash
- Fallback to Wine installation when blobs are not available

## Prevention and Best Practices

1. **Maintain Adequate Disk Space**
   - Ensure sufficient free space before extraction
   - Clean up temporary files after extraction

2. **Use Latest Registry Settings**
   - Always use Windows 10 registry for new installers
   - Test with different Windows versions if needed

3. **Verify File Integrity**
   - Check file sizes match expected values
   - Ensure all required files are present
   - Remove any incorrect files (like `sgl.dat`)

4. **Backup Working Configurations**
   - Keep copies of working firmware folders
   - Document successful extraction procedures

## Related Documentation

- [TrackIR Permission Troubleshooting](TRACKIR_PERMISSION_TROUBLESHOOTING.md)
- [General Troubleshooting Guide](TROUBLESHOOTING.md)
- [Wine Legacy Documentation](../wine-legacy/)

## Version History

- **2025-07-05**: Initial documentation based on TrackIR 5.5.3 extraction experience
- Added manual extraction process
- Documented file organization requirements
- Included code improvements for Windows 10 compatibility 