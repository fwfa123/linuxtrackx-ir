# MFC42 Winetricks Approach Implementation Progress

## Overview
This document tracks the progress of implementing a winetricks-based MFC42 installation approach to replace the current MFC140 extraction process.

## Current State Analysis

### Existing MFC42 Infrastructure
**Found**: Comprehensive MFC42 alternative installation script at `scripts/install/mfc42_alternative_installers.sh`
- **Multiple Installation Methods**: Direct download, Windows extraction, package manager, cabextract, alternative sources, symlink fallback
- **Winetricks Integration**: Already referenced in documentation and scripts
- **GUI Gap**: The script exists but GUI integration was never completed

### Current MFC140 Process (Lines 239-310 in check_data.c)
```c
// Try MFC140 first (modern approach) - Lines 239-290
sprintf(path1, "%s/.config/linuxtrack/tir_firmware/mfc140u.dll", home);
printf("DEBUG: mfc140u.dll path: %s\n", path1);
printf("DEBUG: Creating symlink for mfc140u.dll...\n");
if(symlink(path1, "mfc140u.dll") != 0){
  printf("DEBUG: Failed to create symlink for mfc140u.dll, trying MFC42 fallback (error: %d)\n", errno);
  if(errno == EEXIST) {
    printf("DEBUG: mfc140u.dll symlink already exists (this is normal)\n");
  } else {
    // Fallback to MFC42 only for real errors - Lines 255-285
    sprintf(path1, "%s/.config/linuxtrack/tir_firmware/mfc42u.dll", home);
    printf("DEBUG: mfc42u.dll path: %s\n", path1);
    printf("DEBUG: Creating symlink for mfc42u.dll...\n");
    if(symlink(path1, "mfc42u.dll") != 0){
      // Error handling
    } else {
      printf("DEBUG: Successfully created symlink for mfc42u.dll (fallback)\n");
    }
  }
} else {
  printf("DEBUG: Successfully created symlink for mfc140u.dll\n");
}

// Create MFC42u.dll symlink pointing to mfc140u.dll for compatibility - Lines 294-310
printf("DEBUG: Creating MFC42u.dll symlink for compatibility...\n");
if(symlink("mfc140u.dll", "MFC42u.dll") != 0){
  // Error handling
} else {
  printf("DEBUG: Successfully created symlink for MFC42u.dll\n");
}
```

### Target MFC42 Winetricks Process
```c
// Only create MFC42 symlink to firmware folder
sprintf(path1, "%s/.config/linuxtrack/tir_firmware/mfc42u.dll", home);
if(symlink(path1, "mfc42u.dll") != 0){
  // Error handling with winetricks instructions
}
```

## Implementation Plan

### Phase 1: Create MFC42 Winetricks Extractor
- [ ] **File**: `src/qt_gui/extractor.h`
  - [ ] Add `Mfc42uWinetricksExtractor` class (replacing `Mfc140uExtractor`)
  - [ ] Define winetricks installation methods
  - [ ] Add fallback to manual installation
  - [ ] Integrate with existing `mfc42_alternative_installers.sh` script

- [ ] **File**: `src/qt_gui/extractor.cpp`
  - [ ] Implement `Mfc42uWinetricksExtractor` class
  - [ ] Add winetricks installation logic (prefer `winetricks mfc42`)
  - [ ] Add manual installation fallback using existing script
  - [ ] Add error handling and user guidance
  - [ ] Call `scripts/install/mfc42_alternative_installers.sh` as fallback

### Phase 2: Update Plugin Installer
- [ ] **File**: `src/qt_gui/plugin_install.h`
  - [ ] Replace `Mfc140uExtractor` with `Mfc42uWinetricksExtractor`
  - [ ] Update method signatures
  - [ ] Add winetricks availability checking
  - [ ] Uncomment existing `mfc42uInstall()` call (line 89)

- [ ] **File**: `src/qt_gui/plugin_install.cpp`
  - [ ] Implement `mfc42uInstall()` method (currently missing)
  - [ ] Replace `mfc140uInstall()` with `mfc42uInstall()`
  - [ ] Replace MFC140 logic with MFC42 winetricks logic
  - [ ] Update installation flow and state machine
  - [ ] Add winetricks dependency checking
  - [ ] Update `isMfc140uInstalled()` to `isMfc42uInstalled()`

### Phase 3: Update Check Data Component
- [ ] **File**: `src/wine_bridge/client/check_data.c`
  - [ ] Remove MFC140 symlink creation (lines 239-290)
  - [ ] Remove MFC42u.dll symlink to mfc140u.dll (lines 294-310)
  - [ ] Keep only MFC42u.dll symlink to firmware folder
  - [ ] Update error messages to reference winetricks installation
  - [ ] Replace complex fallback logic with simple MFC42-only approach

### Phase 4: Update Sources and Configuration
- [ ] **File**: `src/qt_gui/sources_mfc140.txt`
  - [ ] Rename to `sources_mfc42.txt`
  - [ ] Update URLs to MFC42 sources (Visual C++ 6.0 redistributable)
  - [ ] Add winetricks installation instructions
  - [ ] Reference existing `mfc42_alternative_installers.sh` script

### Phase 5: Update Documentation
- [ ] **File**: `docs/technical/MFC42_ALTERNATIVE_INSTALLATION_METHODS.md`
  - [ ] Add winetricks installation section
  - [ ] Update troubleshooting guide
  - [ ] Add cross-distribution compatibility notes

## Technical Implementation Details

### Winetricks Installation Method
```bash
# Install MFC42 using winetricks (preferred method)
winetricks mfc42

# Alternative: Install Visual C++ 6.0 runtime
winetricks vcrun6

# Fallback: Use existing alternative installation script
./scripts/install/mfc42_alternative_installers.sh install

# Manual installation fallback
# Download vcredist.exe and install manually
```

### Existing Infrastructure to Leverage
- **Script**: `scripts/install/mfc42_alternative_installers.sh` - Comprehensive MFC42 installation
- **Methods**: Direct download, Windows extraction, package manager, cabextract, alternative sources
- **GUI Gap**: Script exists but no GUI integration
- **Integration Point**: Call script from GUI as fallback when winetricks fails

### File Structure Changes
```
Current:
~/.config/linuxtrack/tir_firmware/
├── mfc140u.dll (extracted from VC++ 2015-2022)
├── mfc42u.dll (symlink to mfc140u.dll)
└── TIRViews.dll

Target:
~/.config/linuxtrack/tir_firmware/
├── mfc42u.dll (extracted from winetricks)
└── TIRViews.dll
```

### Symlink Changes in check_data.c
```c
// Current (lines 239-310):
// 1. Create mfc140u.dll symlink (lines 239-290)
// 2. Create mfc42u.dll symlink (fallback, lines 255-285)
// 3. Create MFC42u.dll symlink to mfc140u.dll (lines 294-310)

// Target (simplified):
// 1. Create mfc42u.dll symlink only (lines 239-260)
// 2. Remove all MFC140 logic
// 3. Remove MFC42u.dll symlink to mfc140u.dll
```

### Detailed Implementation Plan

#### Step 1: Modify check_data.c (Lines 239-310)
**Current Code to Remove:**
```c
// Lines 239-290: MFC140 symlink creation
sprintf(path1, "%s/.config/linuxtrack/tir_firmware/mfc140u.dll", home);
printf("DEBUG: mfc140u.dll path: %s\n", path1);
printf("DEBUG: Creating symlink for mfc140u.dll...\n");
if(symlink(path1, "mfc140u.dll") != 0){
  // Complex fallback logic
}

// Lines 294-310: MFC42u.dll symlink to mfc140u.dll
printf("DEBUG: Creating MFC42u.dll symlink for compatibility...\n");
if(symlink("mfc140u.dll", "MFC42u.dll") != 0){
  // Error handling
}
```

**New Code to Add:**
```c
// Lines 239-260: Simple MFC42 symlink creation
sprintf(path1, "%s/.config/linuxtrack/tir_firmware/mfc42u.dll", home);
printf("DEBUG: mfc42u.dll path: %s\n", path1);
printf("DEBUG: Creating symlink for mfc42u.dll...\n");
if(symlink(path1, "mfc42u.dll") != 0){
  printf("DEBUG: Failed to create symlink for mfc42u.dll (error: %d)\n", errno);
  if(errno == EEXIST) {
    printf("DEBUG: mfc42u.dll symlink already exists (this is normal)\n");
  } else {
    char error_msg[512];
    sprintf(error_msg, 
      "Failed to create symlink to mfc42u.dll!\n\n"
      "Error code: %d\n"
      "Target path: %s\n"
      "Working directory: %s\n\n"
      "This usually means the MFC42 library is not properly installed.\n"
      "Please install MFC42 using winetricks:\n"
      "winetricks mfc42\n"
      "Or run the MFC42 installation in the LinuxTrack GUI first.",
      errno, path1, cwd);
    
    MessageBox(NULL, error_msg, "Linuxtrack-wine check", MB_OK);
  }
} else {
  printf("DEBUG: Successfully created symlink for mfc42u.dll\n");
}
```

## Progress Tracking

### Completed Tasks
- [x] Created new branch `mfc42-winetricks-approach`
- [x] Created progress tracking document
- [x] **Phase 3: Updated Check Data Component** ✅
  - [x] Removed MFC140 symlink creation (lines 239-290)
  - [x] Removed MFC42u.dll symlink to mfc140u.dll (lines 294-310)
  - [x] Kept only MFC42u.dll symlink to firmware folder
  - [x] Updated error messages to reference winetricks installation
  - [x] Replaced complex fallback logic with simple MFC42-only approach
- [x] **Phase 4: Updated Sources and Configuration** ✅
  - [x] Renamed `sources_mfc140.txt` to `sources_mfc42.txt`
  - [x] Updated URLs to MFC42 sources (Visual C++ 6.0 redistributable)
  - [x] Added winetricks installation instructions
  - [x] Referenced existing `mfc42_alternative_installers.sh` script
- [x] **Phase 2: Updated Plugin Installer** ✅
  - [x] Replaced `Mfc140uExtractor` with `Mfc42uWinetricksExtractor` in header
  - [x] Updated method signatures from MFC140 to MFC42
  - [x] Implemented `mfc42uInstall()` method (was missing)
  - [x] Updated installation flow and state machine
  - [x] Updated `isMfc140uInstalled()` to `isMfc42uInstalled()`
  - [x] Updated all references in ltr_gui.cpp
- [x] **Phase 1: Create MFC42 Winetricks Extractor** ✅
  - [x] Add `Mfc42uWinetricksExtractor` class (replacing `Mfc140uExtractor`)
  - [x] Define winetricks installation methods
  - [x] Add fallback to manual installation
  - [x] Integrate with existing `mfc42_alternative_installers.sh` script
  - [x] Implement `Mfc42uWinetricksExtractor` class
  - [x] Add winetricks installation logic (prefer `winetricks mfc42`)
  - [x] Add manual installation fallback using existing script
  - [x] Add error handling and user guidance
  - [x] Call `scripts/install/mfc42_alternative_installers.sh` as fallback

### In Progress
- [ ] Phase 5: Update Documentation

### Blocked Tasks
- None currently

### Next Steps
1. ✅ Implement `Mfc42uWinetricksExtractor` class
2. ✅ Update plugin installer to use new extractor
3. ✅ Modify check_data.c to remove MFC140 logic
4. Test winetricks installation process
5. Update documentation

## Testing Plan

### Test Cases
1. **Winetricks Available**: Test automatic winetricks installation
2. **Winetricks Unavailable**: Test manual installation fallback
3. **Cross-Distribution**: Test on Ubuntu, Fedora, Arch Linux
4. **Error Handling**: Test with missing dependencies
5. **Symlink Creation**: Verify check_data.c creates correct symlinks

### Test Commands
```bash
# Test winetricks installation
winetricks mfc42

# Test manual installation
wine vcredist.exe /Q /T:C:\ /C

# Test symlink creation
ls -la ~/.wine/drive_c/Program\ Files\ \(x86\)/Linuxtrack/

# Test wine bridge functionality
wine ~/.wine/drive_c/Program\ Files\ \(x86\)/Linuxtrack/check_data.exe
```

## Risk Assessment

### High Risk
- **Winetricks Dependency**: Requires winetricks to be installed
- **Cross-Distribution Compatibility**: Different package managers and versions
- **User Experience**: More complex installation process

### Medium Risk
- **File Extraction**: Need to extract mfc42u.dll from winetricks installation
- **Error Handling**: More failure points to handle
- **Documentation**: Need comprehensive user guidance

### Low Risk
- **Symlink Changes**: Simple modification to check_data.c
- **Backward Compatibility**: Can maintain fallback to manual installation

## Success Criteria

### Functional Requirements
- [ ] Winetricks-based MFC42 installation works
- [ ] Manual installation fallback works
- [ ] Symlinks are created correctly
- [ ] Wine bridge functions properly
- [ ] Cross-distribution compatibility

### Non-Functional Requirements
- [ ] User-friendly error messages
- [ ] Comprehensive documentation
- [ ] Minimal user intervention required
- [ ] Robust error handling
- [ ] Performance comparable to current approach

## Notes and Observations

### Key Insights
1. **MFC140 Failure**: MFC140 doesn't work correctly with TrackIR - need to go back to MFC42
2. **Existing Infrastructure**: Comprehensive MFC42 installation script already exists
3. **GUI Gap**: The script works but GUI integration was never completed
4. **Winetricks Advantage**: Direct access to MFC42 libraries via `winetricks mfc42`
5. **Simplification Opportunity**: Remove MFC140 dependency entirely, use MFC42 only
6. **User Experience**: Winetricks is more reliable than manual downloads
7. **Avoid Duplication**: Don't recreate what already exists in `mfc42_alternative_installers.sh`

### Technical Considerations
1. **File Extraction**: Need to locate mfc42u.dll in winetricks installation
2. **Path Resolution**: Handle different winetricks installation paths
3. **Error Recovery**: Provide clear guidance when winetricks fails
4. **Testing**: Comprehensive testing across distributions

### Future Improvements
1. **Package Manager Integration**: Direct package manager installation
2. **Automated Testing**: CI/CD pipeline for testing
3. **User Interface**: Better GUI for installation process
4. **Documentation**: Video tutorials and screenshots 