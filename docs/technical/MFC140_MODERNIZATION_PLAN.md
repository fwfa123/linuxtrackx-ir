# MFC140 Modernization Plan for LinuxTrack Wine Bridge

## Overview
This document outlines the plan to modernize the LinuxTrack Wine bridge from using legacy MFC42 (Visual C++ 6.0) to modern MFC140 (Visual C++ 2015-2022) libraries, eliminating legal copyright issues and improving compatibility.

## Current State Analysis

### Problem Statement
- **Current Dependency**: `mfc42u.dll` (Visual C++ 6.0 MFC from 1998)
- **Legal Issues**: Microsoft copyright on MFC42 libraries
- **Compatibility**: Complex extraction process, unreliable installation
- **Technical Debt**: Legacy code paths for MFC42 extraction

### Key Discovery
- **TrackIR 5.5.2+**: Uses `mfc140u.dll` (Visual C++ 2015-2022 MFC)
- **Wine Compatibility**: Modern Wine installations have VC++ 2015-2022 runtime but missing MFC140
- **Dependency Chain**: Wine Bridge → TIRViews.dll → MFC140 (not MFC42)

## Technical Implementation Plan

### Phase 1: Create MFC140 Extractor Class

#### 1.1 New MFC140 Extractor Implementation
**File**: `src/qt_gui/extractor.h` and `src/qt_gui/extractor.cpp`

```cpp
class Mfc140uExtractor : public Extractor
{
 Q_OBJECT
 public:
  Mfc140uExtractor(QWidget *parent = 0);
  ~Mfc140uExtractor();
  
  // Public method to start automatic installation
  void startAutomaticInstallation();
  
 private:
  void commenceExtraction(QString file);
  void enableButtons(bool enable);
  int stage;
  QProcess *cabextract;
  
  // Modern MFC140 installation methods
  bool tryWinetricksInstall();
  bool tryPackageManagerInstall();
  bool tryVCRedistInstall();
  void showModernInstallationInstructions();
  QString checkWinetricksAvailability();
  bool isWinetricksVersionRecent(const QString& winetricksPath);
  bool installLatestWinetricks();
  
 private slots:
  void wineFinished(bool result);
  void cabextractFinished(int exitCode, QProcess::ExitStatus status);
};
```

#### 1.2 MFC140 Installation Methods

**Method 1: Winetricks Installation**
```bash
# Install Visual C++ 2015-2022 MFC libraries
winetricks vcrun2015
winetricks vcrun2017
winetricks vcrun2019
winetricks vcrun2022
```

**Method 2: Package Manager Installation**
```bash
# Debian/Ubuntu/MX
sudo apt install wine-staging wine32:i386

# Fedora/RHEL
sudo dnf install wine-staging wine-core wine-desktop

# Arch Linux
sudo pacman -S wine-staging wine-mono wine-gecko
```

**Method 3: Direct VCRedist Installation**
- Download Visual C++ 2015-2022 Redistributable from Microsoft
- Install in Wine prefix using winegcc
- Extract MFC140 libraries from installed redistributable

### Phase 2: Update Wine Bridge Views Component

#### 2.1 Modernize TIRViews.dll Loading
**File**: `src/wine_bridge/views/main.cpp`

**Current Implementation**:
```cpp
tvHandle = LoadLibrary("mfc42u.dll");
if(!tvHandle){
  message_("Can't load mfc42u.dll needed by TIRViews.dll.\nTry installing MFC42 (e.g. using winetricks mfc42).");
  return TRUE;
}
```

**New Implementation**:
```cpp
// Try MFC140 first (modern approach)
tvHandle = LoadLibrary("mfc140u.dll");
if(!tvHandle){
  // Fallback to MFC42 for backward compatibility
  tvHandle = LoadLibrary("mfc42u.dll");
  if(!tvHandle){
    message_("Can't load MFC libraries needed by TIRViews.dll.\n"
             "Try installing Visual C++ 2015-2022 MFC libraries:\n"
             "winetricks vcrun2015 vcrun2017 vcrun2019 vcrun2022");
    return TRUE;
  }
}
```

#### 2.2 Enhanced Error Handling
- Detect which MFC version is available
- Provide specific installation instructions based on detected environment
- Support both 32-bit and 64-bit MFC libraries

### Phase 3: Update Installation Process

#### 3.1 Modify Plugin Installation Flow
**File**: `src/qt_gui/plugin_install.cpp`

**Current Flow**:
1. TIR Firmware extraction
2. MFC42 extraction
3. Wine bridge installation

**New Flow**:
1. TIR Firmware extraction (with newer TIRViews.dll)
2. MFC140 installation (with fallback to MFC42)
3. Wine bridge installation

#### 3.2 Update Check Data Component
**File**: `src/wine_bridge/client/check_data.c`

**Current Implementation**:
```c
sprintf(path1, "%s/.config/linuxtrack/tir_firmware/mfc42u.dll", home);
if(symlink(path1, "mfc42u.dll") != 0){
  MessageBox(NULL,
  "Failed to create symlink to mfc42u.dll!\n"
  "Try to install TIRViews support in ltr_gui,\n"
  "or install mfc42 into this bottle using winetricks.",
  "Linuxtrack-wine check",
  MB_OK);
}
```

**New Implementation**:
```c
// Try MFC140 first
sprintf(path1, "%s/.config/linuxtrack/tir_firmware/mfc140u.dll", home);
if(symlink(path1, "mfc140u.dll") != 0){
  // Fallback to MFC42
  sprintf(path1, "%s/.config/linuxtrack/tir_firmware/mfc42u.dll", home);
  if(symlink(path1, "mfc42u.dll") != 0){
    MessageBox(NULL,
    "Failed to create symlink to MFC libraries!\n"
    "Try to install Visual C++ 2015-2022 MFC libraries:\n"
    "winetricks vcrun2015 vcrun2017 vcrun2019 vcrun2022",
    "Linuxtrack-wine check",
    MB_OK);
  }
}
```

### Phase 4: Update Build System

#### 4.1 Modify Wine Bridge Makefile
**File**: `src/wine_bridge/Makefile.am`

**Add MFC140 Support**:
```makefile
# Add MFC140 library detection
MFC140_LIBS = -lmfc140u
MFC42_LIBS = -lmfc42u

# Conditional linking based on availability
if HAVE_MFC140
WINE_LIBS += $(MFC140_LIBS)
else
WINE_LIBS += $(MFC42_LIBS)
endif
```

#### 4.2 Update Configuration Script
**File**: `configure.ac`

**Add MFC140 Detection**:
```autoconf
# Check for MFC140 libraries
AC_CHECK_LIB([mfc140u], [main], [have_mfc140=yes], [have_mfc140=no])
AM_CONDITIONAL([HAVE_MFC140], [test "x$have_mfc140" = "xyes"])

# Check for MFC42 libraries (fallback)
AC_CHECK_LIB([mfc42u], [main], [have_mfc42=yes], [have_mfc42=no])
AM_CONDITIONAL([HAVE_MFC42], [test "x$have_mfc42" = "xyes"])
```

### Phase 5: Update Documentation

#### 5.1 Installation Guides
- Update `WINE_SETUP.md` with MFC140 installation instructions
- Modify troubleshooting guides to include MFC140 solutions
- Update AppImage build documentation

#### 5.2 User Documentation
- Update README.md with new MFC140 requirements
- Modify installation scripts to use modern MFC libraries
- Update error messages and help text

## Implementation Strategy

### Development Approach
1. **Feature Branch**: `feature/mfc140-upgrade`
2. **Incremental Development**: Implement changes in phases
3. **Backward Compatibility**: Maintain MFC42 fallback during transition
4. **Testing**: Thorough testing before merging to main

### Testing Plan
1. **Unit Tests**: Test MFC140 extractor functionality
2. **Integration Tests**: Verify Wine bridge with MFC140
3. **Compatibility Tests**: Ensure MFC42 fallback works
4. **AppImage Tests**: Verify AppImage builds successfully

### Migration Strategy
1. **Phase 1**: Implement MFC140 support alongside MFC42
2. **Phase 2**: Update default behavior to prefer MFC140
3. **Phase 3**: Deprecate MFC42 extraction (keep fallback)
4. **Phase 4**: Remove MFC42 code paths (future release)

## Benefits

### Legal Benefits
- **Eliminates Copyright Issues**: No more Microsoft MFC42 copyright concerns
- **Redistributable Libraries**: Visual C++ 2015-2022 MFC is legally redistributable
- **Modern Licensing**: Uses current Microsoft licensing terms

### Technical Benefits
- **Better Compatibility**: Modern MFC libraries work better with current Wine
- **Improved Reliability**: More stable installation process
- **Enhanced Features**: Access to newer TrackIR features and stability
- **Future-Proof**: Aligns with current Windows development standards

### User Benefits
- **Simplified Installation**: Less complex extraction process
- **Better Error Messages**: More helpful troubleshooting information
- **Improved Performance**: Modern libraries may provide better performance
- **Enhanced Stability**: More reliable head tracking experience

## Risk Mitigation

### Backward Compatibility
- Maintain MFC42 fallback during transition period
- Provide clear migration path for existing users
- Test thoroughly with existing game configurations

### Rollback Plan
- Feature branch allows easy rollback if issues arise
- Maintain MFC42 code paths during initial deployment
- Monitor user feedback and bug reports

### Testing Strategy
- Comprehensive testing on multiple distributions
- Test with various Wine versions and configurations
- Verify compatibility with existing game profiles

## Timeline

### Week 1: Foundation
- Create feature branch
- Implement MFC140 extractor class
- Update Wine bridge views component

### Week 2: Integration
- Modify installation process
- Update build system
- Implement backward compatibility

### Week 3: Testing
- Comprehensive testing
- Bug fixes and refinements
- Documentation updates

### Week 4: Deployment
- Final testing and validation
- Merge to main branch
- Release preparation

## Success Criteria

### Technical Success
- [ ] MFC140 libraries install successfully via winetricks
- [ ] Wine bridge loads TIRViews.dll with MFC140
- [ ] Backward compatibility with MFC42 maintained
- [ ] AppImage builds successfully with new approach

### User Success
- [ ] Installation process is simpler and more reliable
- [ ] Error messages are more helpful
- [ ] No regression in existing functionality
- [ ] Improved compatibility with modern Wine versions

### Legal Success
- [ ] No Microsoft copyright issues with MFC libraries
- [ ] Uses legally redistributable Visual C++ 2015-2022 libraries
- [ ] Clear licensing compliance

## Conclusion

This modernization plan addresses the core issues with the current MFC42 dependency while providing a clear path forward for the LinuxTrack Wine bridge. The approach maintains backward compatibility while introducing modern MFC140 support, ensuring a smooth transition for users and eliminating legal concerns.

The implementation strategy focuses on incremental development with thorough testing, allowing for easy rollback if issues arise. The benefits include improved reliability, better compatibility, and elimination of legal copyright issues. 