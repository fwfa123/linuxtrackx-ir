# TrackIR 5.5.2 Installer Analysis

## Executive Summary
**CRITICAL DISCOVERY**: The latest TrackIR 5.5.2 installer still uses **MFC42u.dll**, not MFC140u.dll as previously documented. This contradicts the earlier assumption that newer TrackIR versions had migrated to MFC140.

## Installation Analysis

### Installer Details
- **File**: TrackIR_5.5.2.exe
- **Type**: PE32 executable (GUI) Intel 80386, for MS Windows
- **Installation Path**: `~/.wine/drive_c/Program Files (x86)/TrackIR5/`
- **Installation Date**: August 3, 2024

### Key Findings

#### 1. **TIRViews.dll Analysis**
- **File Size**: 122,016 bytes (identical to existing file)
- **Dependencies**: Still uses **MFC42u.dll** (not MFC140u.dll)
- **Comparison**: The installed TIRViews.dll is **bit-for-bit identical** to the existing file in `~/.config/linuxtrack/tir_firmware/`

#### 2. **Dependencies Confirmed**
The latest TrackIR 5.5.2 TIRViews.dll has the same dependencies as the existing file:
- **MFC42u.dll** - Microsoft Foundation Classes (9 ordinal imports)
- **msvcrt.dll** - Microsoft Visual C++ Runtime (18 functions)
- **msvcp60.dll** - Microsoft Visual C++ Standard Library (4 functions)
- **KERNEL32.dll** - Windows Kernel API (35+ functions)
- **USER32.dll** - Windows User Interface API (5 functions)
- **ADVAPI32.dll** - Windows Advanced API (5 functions)

#### 3. **Installed Files**
```
TrackIR5/
├── TIRViews.dll (122,016 bytes - identical to existing)
├── TrackIR5.exe (78,817,952 bytes - main application)
├── TIRMouse.exe (57,504 bytes - mouse utility)
├── NPClient.dll (367,776 bytes - TrackIR API)
├── NPClient64.dll (35,488 bytes - 64-bit API)
├── opencv_world454.dll (41,324,544 bytes - OpenCV library)
├── sgl.dat (93,519 bytes - configuration data)
├── sgl.sig (64 bytes - signature file)
├── ProfileMap.dat (5,481 bytes - profile mapping)
├── Settings.xml (4,228 bytes - settings)
├── simconnect.manifest (635 bytes - manifest)
├── simconnectSP2.manifest (634 bytes - SP2 manifest)
├── tirmouse.exe.manifest (966 bytes - mouse manifest)
├── Data/ (directory)
├── Licenses/ (directory)
├── Profiles/ (directory)
└── Styles/ (directory)
```

## Implications for LinuxTrack Project

### 1. **MFC140 Modernization Status**
- **Reality Check**: TrackIR 5.5.2 still uses MFC42u.dll
- **Documentation Error**: The assumption that newer TrackIR versions use MFC140 was incorrect
- **Current Status**: Both old and new TrackIR versions use MFC42u.dll

### 2. **Wine Bridge Compatibility**
- **Good News**: The existing MFC42 fallback in your Wine bridge is correct
- **No Change Needed**: The current implementation that tries MFC140 first, then falls back to MFC42 is appropriate
- **Backward Compatibility**: Your system correctly handles both scenarios

### 3. **Project Documentation Update Needed**
The following documentation should be updated:
- **MFC140_MODERNIZATION_PLAN.md**: Remove incorrect assumption about TrackIR 5.5.2+
- **MFC140_IMPLEMENTATION_SUMMARY.md**: Update to reflect actual findings
- **WINE_SUPPORT_MODERN.md**: Correct the version information

## Technical Analysis

### File Integrity Verification
- ✅ **PE headers valid** for both files
- ✅ **Import tables identical** - same dependencies
- ✅ **Export tables identical** - same functions (TIRViewsStart, TIRViewsStop, TIRViewsVersion)
- ✅ **File checksums match** - bit-for-bit identical

### Dependency Analysis
Both files require the same 6 dependencies:
1. **MFC42u.dll** (Critical - MFC framework)
2. **msvcrt.dll** (Critical - C runtime)
3. **msvcp60.dll** (Critical - C++ runtime)
4. **KERNEL32.dll** (Critical - Windows kernel)
5. **USER32.dll** (Critical - GUI functions)
6. **ADVAPI32.dll** (Critical - Registry functions)

## Recommendations

### 1. **Update Project Documentation**
```bash
# Update the MFC140 modernization documentation to reflect actual findings
# Remove incorrect assumptions about TrackIR 5.5.2+ using MFC140
```

### 2. **Maintain Current Wine Bridge Implementation**
The current implementation is correct:
- Try MFC140 first (for future compatibility)
- Fallback to MFC42 (for current TrackIR versions)
- This provides the best of both worlds

### 3. **Test with Latest Installation**
```bash
# Test the Wine bridge with the newly installed TrackIR 5.5.2
# Verify that MFC42 fallback works correctly
```

## Conclusion

**Key Discovery**: TrackIR 5.5.2 still uses MFC42u.dll, not MFC140u.dll. This means:

1. **Your existing TIRViews.dll is current** - it's identical to the latest version
2. **Your MFC42 dependency analysis is correct** - the latest version still needs MFC42u.dll
3. **Your Wine bridge implementation is appropriate** - the MFC140→MFC42 fallback strategy is sound
4. **Project documentation needs updating** - remove incorrect assumptions about MFC140 migration

The MFC140 modernization in your project is still valuable for future compatibility, but the current TrackIR versions still rely on MFC42. 