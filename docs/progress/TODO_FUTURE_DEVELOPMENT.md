# LinuxTrack X-IR Future Development TODO

## 🎉 **Recent Major Achievements**

### ✅ **Complete Gaming Platform Integration - COMPLETED** (August 2025)
- **Lutris Integration**: ✅ Complete Lutris prefix detection and Wine Bridge installation
- **Steam Proton Integration**: ✅ Complete Steam Proton integration with multi-library support
- **Proton version detection** (including beta versions like Proton 9.0 Beta)
- **VDF parsing** for Steam library management
- **NSIS installer integration** within Proton environment
- **Comprehensive error handling** and detailed logging
- **Successfully tested** with real Steam and Lutris installations

### ✅ **Automatic Wine Prefix Architecture Detection - COMPLETED** (January 2025)
- Automatic detection of Wine prefix architecture (win32/win64)
- Intelligent tester selection: `Tester.exe` (32-bit) vs `Tester64.exe` (64-bit)
- `WINEARCH` set per platform execution (Steam Proton, Lutris, Custom Prefix)
- Clear UI feedback showing detected architecture and selected tester
- Fallback behavior when architecture is unknown

### ✅ **Automatic Tracking Start Feature - COMPLETED** (August 2025)
- **Immediate tracking start**: Tracking begins BEFORE NSIS installer launches
- **Enhanced user experience**: Users can test tracking while installer runs
- **Multi-platform support**: Works with Steam Proton, Lutris, and Custom Prefix
- **Future-ready**: Placeholder implementations for upcoming features
- **Seamless workflow**: Eliminates manual tracking window interaction
- **Production tested**: Successfully working in real-world scenarios

### ✅ **UI Modernization - COMPLETED** (August 2025)
- **Gaming Tab**: ✅ New dedicated Gaming tab with Steam Proton and Lutris buttons
- **System Tab**: ✅ Renamed and reorganized Misc tab with comprehensive system information
- **Improved user experience** with better organization and clearer interface

### ✅ **AppImage Builder v2 + Help Runtime Fix - COMPLETED** (August 2025)
- Modular AppImage pipeline (`scripts/appimage/v2/`) producing fully self-contained images
- Bundled Qt SQLite driver and plugin path setup; Qt Help initializes correctly without host Qt
- RPATH harmonization and validation step (ldd audit, Qt plugin checks)

---

## 🎯 Development Priority Overview

This TODO list is organized by complexity and impact, starting with **low-hanging fruit** (quick wins) and progressing to **ambitious long-term goals** that may take significant time and effort to achieve.

---

## 🍎 Phase 1: Low-Hanging Fruit (Quick Wins)

### 1.1 QT Interface Upgrades - Gaming Platform Integration ✅ **COMPLETED**
**Priority: HIGH | Effort: MEDIUM | Impact: HIGH** | **Status: ✅ COMPLETED**

#### Create New "Gaming" Tab ✅ **COMPLETED**
- [x] Add new tab to main GUI interface
- [x] Move "Install Wine Support" button from Misc tab to Gaming tab
- [x] Add "Install Wine Bridge in Steam Proton" button ✅ **COMPLETED**
- [x] Add "Install Wine Bridge in Lutris Prefix" button ✅ **COMPLETED**
- [x] Move "Launch Mickey (TrackIR)" button from Misc tab to Gaming tab
- [x] Add automatic tracking start to all gaming installation buttons ✅ **COMPLETED**
- [ ] Add "Launch ltr_pipe for Antimicrox Support" button
  - Reference: [GitHub Issue #206 Comment](https://github.com/uglyDwarf/linuxtrack/issues/206#issuecomment-2423819525)
  - Manual process: `ltr_pipe | antimicrox --profile /path/to/profile.json`

#### Rename and Reorganize Misc Tab ✅ **COMPLETED**
- [x] Rename "Misc." tab to "System" tab
- [x] Reorganize as system information display (similar to Lutris System Tab)
- [x] Add system information display:
  - [x] Wine version detection
  - [x] Winetricks version detection
  - [x] Proton Tricks version detection
  - [x] XPlane SDK detection
  - [x] LinuxTrack installation status
  - [x] Wine Bridge installation status
  - [x] System compatibility information

### 1.2 Documentation Updates
**Priority: HIGH | Effort: MEDIUM | Impact: HIGH**

#### Simplify README
- [ ] Streamline main README.md to be more concise and clear
- [ ] Move detailed technical information to Wiki
- [ ] Focus on quick start and essential information
- [ ] Add clear troubleshooting section

### 1.3 Wine Prefix Architecture Detection ✅ **COMPLETED**
- [x] Detect wine prefix architecture without relying on filename
- [x] Select appropriate tester based on detected architecture
- [x] Set `WINEARCH` per platform execution path
- [x] Provide user feedback and fallback logic

#### Create GitHub Wiki
- [ ] Set up comprehensive Wiki structure
- [ ] Move detailed installation guides to Wiki
- [ ] Create troubleshooting guides
- [ ] Add hardware compatibility matrix
- [ ] Create user guides for different use cases
- [ ] Add developer documentation

#### Update Help System
- [ ] Update help documentation to reflect all changes
- [ ] Ensure help system matches current GUI
- [ ] Add screenshots and step-by-step guides
- [ ] Update Wine/Steam integration help
  - Runtime fix completed in v0.99.23 (bundled SQLite driver + plugin paths)

---

## 🎉 **Phase 1 Summary - COMPLETED**

### ✅ **Major Achievements in Phase 1**
- **Complete Gaming Platform Integration**: Both Lutris and Steam Proton support
- **Automatic Tracking Start Feature**: Seamless tracking initiation before installers
- **Modern UI Design**: Dedicated Gaming and System tabs
- **Comprehensive Error Handling**: Robust error reporting and user feedback
- **Multi-Platform Support**: Works with games on different drives and platforms
- **Production Ready**: All features tested and working

### 🚀 **User Impact**
- **One-click Wine Bridge installation** for both Lutris and Steam Proton games
- **Immediate tracking feedback** - no waiting for installer completion
- **Improved user experience** with better organized interface
- **Comprehensive system information** in dedicated System tab
- **Professional-grade error handling** with detailed debugging
- **Seamless workflow** - users can test tracking while installer runs
- **Modern Gaming tab** with all gaming-related functionality consolidated

---

## 🍊 Phase 2: Medium Complexity (Significant Improvements)

### 2.1 Autotools to CMake Migration
**Priority: MEDIUM | Effort: HIGH | Impact: MEDIUM**

#### CMake Build System
- [ ] Complete CMakeLists.txt implementation
- [ ] Migrate all autotools configurations to CMake
- [ ] Update build scripts to use CMake
- [ ] Test build system across different distributions
- [ ] Update documentation for CMake build process
- [ ] Ensure all dependencies are properly handled
- [ ] Test AppImage building with CMake

#### Benefits of CMake Migration
- Better IDE integration
- More modern build system
- Easier dependency management
- Better cross-platform support

### 2.2 NSIS to Modern Installer Migration
**Priority: MEDIUM | Effort: HIGH | Impact: MEDIUM**

#### Replace NSIS with Modern Installer System
- [ ] Research modern installer alternatives:
  - [ ] **WiX Toolset** (Windows-focused, XML-based)
  - [ ] **Inno Setup** (Pascal-based, cross-platform)
  - [ ] **NSIS 3.x** (if upgrading from current NSIS 2.x)
  - [ ] **Custom installer** using Qt Installer Framework
  - [ ] **AppImage/Flatpak** for Linux distribution
- [ ] Evaluate Wine compatibility requirements
- [ ] Choose best installer system for cross-platform support
- [ ] Migrate existing NSIS scripts to new system
- [ ] Update Wine bridge installer scripts
- [ ] Test installer functionality across platforms
- [ ] Update build automation scripts
- [ ] Update documentation for new installer process

#### Benefits of Modern Installer System
- Better cross-platform support
- More maintainable installer scripts
- Improved user experience
- Better integration with modern packaging systems
- Reduced dependency on legacy tools

---

## 🍇 Phase 3: High Complexity (Major Upgrades)

### 3.1 Qt5 to Qt6 Migration
**Priority: MEDIUM | Effort: VERY HIGH | Impact: HIGH**

#### Qt6 Upgrade Process
- [ ] Analyze Qt5 to Qt6 compatibility issues
- [ ] Update all Qt5-specific code to Qt6
- [ ] Handle deprecated Qt5 APIs
- [ ] Update build system for Qt6
- [ ] Test GUI functionality with Qt6
- [ ] Update documentation for Qt6 requirements

#### Help System Migration
- [ ] Migrate from Qt Help to web-based help system
- [ ] Implement embedded web browser for help
- [ ] Create HTML-based help documentation
- [ ] Ensure help system works offline
- [ ] Add search functionality to help system

#### Qt6 Benefits
- Better Wayland support
- Improved performance
- Modern UI components
- Better accessibility
- Future-proof framework

---

## 🍓 Phase 4: Very High Complexity (Code Modernization)

### 4.1 C to C++ Migration (Module by Module)
**Priority: LOW | Effort: VERY HIGH | Impact: MEDIUM**

#### Gradual C++ Migration Strategy
- [ ] **Phase 4.1**: Start with utility modules
  - [ ] Convert `utils.c` to `utils.cpp`
  - [ ] Convert `math_utils.c` to `math_utils.cpp`
  - [ ] Convert `list.c` to `list.cpp`

- [ ] **Phase 4.2**: Core tracking modules
  - [ ] Convert `tracking.c` to `tracking.cpp`
  - [ ] Convert `pose.c` to `pose.cpp`
  - [ ] Convert `axis.c` to `axis.cpp`

- [ ] **Phase 4.3**: Device driver modules
  - [ ] Convert `tir_driver.c` to `tir_driver.cpp`
  - [ ] Convert `tir4_driver.c` to `tir4_driver.cpp`
  - [ ] Convert `webcam_driver.c` to `webcam_driver.cpp`

- [ ] **Phase 4.4**: GUI and interface modules
  - [ ] Convert remaining C modules to C++
  - [ ] Implement proper C++ classes and inheritance
  - [ ] Add modern C++ features (smart pointers, RAII, etc.)

#### C++ Migration Benefits
- Better type safety
- Object-oriented design
- Modern C++ features
- Better memory management
- Improved maintainability

---

## 🎯 Implementation Guidelines

### Development Workflow
1. **Start with Phase 1**: Focus on user-facing improvements first
2. **Test thoroughly**: Each phase should be fully tested before moving to next
3. **Maintain compatibility**: Ensure existing functionality continues to work
4. **Document changes**: Update documentation as you go
5. **User feedback**: Gather feedback from users for each phase

### Testing Strategy
- [ ] Unit tests for each module
- [ ] Integration tests for GUI changes
- [ ] Cross-distribution testing
- [ ] Hardware compatibility testing
- [ ] Wine compatibility testing

### Documentation Requirements
- [ ] Update README for each phase
- [ ] Create migration guides
- [ ] Update Wiki with new features
- [ ] Create troubleshooting guides
- [ ] Document breaking changes

---

## 📊 Progress Tracking

### Phase 1 Progress
- [x] Gaming Tab: 100% complete ✅
- [x] System Tab: 100% complete ✅
- [x] Automatic Tracking Start: 100% complete ✅
- [ ] Documentation Updates: 0% complete

### Phase 2 Progress
- [ ] CMake Migration: 0% complete
- [ ] NSIS to Modern Installer Migration: 0% complete

### Phase 3 Progress
- [ ] Qt6 Migration: 0% complete
- [ ] Help System Migration: 0% complete

### Phase 4 Progress
- [ ] C++ Migration: 0% complete

---

## 🚀 Quick Start for Developers

### Setting Up Development Environment
```bash
# Clone repository
git clone <repository-url>
cd linuxtrackx-ir

# Create development branch
git checkout -b feature/qt-interface-upgrades

# Build current version
autoreconf -fiv
./configure --prefix=/opt --enable-debug
make -j$(nproc)
```

### Recommended configure for multi-arch (Debian/Ubuntu)
```bash
./configure --prefix=/usr --with-lib32-dir=i386-linux-gnu --enable-ltr-32lib-on-x64
```

### Starting with Phase 1
1. **Begin with Wine/Steam Tab**: This provides immediate user value
2. **Focus on one feature at a time**: Don't try to do everything at once
3. **Test each change**: Ensure existing functionality isn't broken
4. **Document as you go**: Keep documentation updated

---

## 📝 Notes

### Current State Analysis
- **Qt5 GUI**: Currently using Qt5 with some modern features
- **Build System**: Mix of autotools and basic CMake support
- **Code Base**: Primarily C with some C++ components
- **Documentation**: Comprehensive but could be better organized

### Risk Assessment
- **Phase 1**: Low risk, high reward
- **Phase 2**: Medium risk, medium reward
- **Phase 3**: High risk, high reward
- **Phase 4**: Very high risk, long-term reward

### Success Criteria
- [ ] All phases maintain backward compatibility
- [ ] User experience improves with each phase
- [ ] Code becomes more maintainable
- [ ] Documentation remains comprehensive and accessible
- [ ] Community feedback is positive

---

*Last Updated: 2025-08-17*
*Next Review: [Date + 1 month]* 