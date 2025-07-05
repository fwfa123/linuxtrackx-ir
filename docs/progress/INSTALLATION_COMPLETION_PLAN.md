# 🚀 LinuxTrack Installation System Completion Plan

**Branch**: `installation-system-completion`  
**Objective**: Complete the installation system to achieve 100% functionality parity with original Qt4 system  
**Current Status**: 62.5% → Target: 100%

---

## 📋 PHASE 1: Installation Infrastructure Setup

### **Step 1.1: Fix Installation Paths** ⚙️
**Priority**: High | **Complexity**: Medium | **Est. Time**: 2-3 hours

**Actions**:
- [ ] **1.1.1** Modify `configure.ac` to default installation to `/opt/linuxtrack`
  - Change `--prefix` default from `/usr/local` to `/opt`
  - Add configuration option for user-selectable installation path
  - Test compatibility with system packaging

- [ ] **1.1.2** Update `Makefile.am` installation directories
  - Ensure all components install to correct `/opt/linuxtrack` subdirectories
  - Verify library paths match original system structure
  - Update pkgconfig files to reflect new paths

- [ ] **1.1.3** Create installation directory structure script
  - Automatically create required subdirectories during install
  - Set proper permissions for LinuxTrack directories
  - Handle potential permission issues

**Expected Outcome**: Installation defaults to `/opt/linuxtrack` matching original system

---

### **Step 1.2: Automate udev Rules Installation** 🔧
**Priority**: High | **Complexity**: Low | **Est. Time**: 1-2 hours

**Actions**:
- [ ] **1.2.1** Add udev rules to installation process
  ```bash
  # Add to src/Makefile.am
  install-data-hook:
      $(INSTALL_DATA) $(srcdir)/99-TIR.rules $(DESTDIR)/lib/udev/rules.d/
      $(INSTALL_DATA) $(srcdir)/99-Mickey.rules $(DESTDIR)/lib/udev/rules.d/
      udevadm control --reload-rules || true
  ```

- [ ] **1.2.2** Verify rules installation path
  - Confirm `/lib/udev/rules.d` is correct location (not `/etc/udev/rules.d`)
  - Test on target systems to ensure rules activate correctly
  - Add error handling for systems without udev

**Expected Outcome**: TrackIR device rules automatically installed and activated

---

## 📋 PHASE 2: Resource Files Installation

### **Step 2.1: Add Missing GUI Resources** 🎨
**Priority**: Critical | **Complexity**: Medium | **Est. Time**: 3-4 hours

**Actions**:
- [ ] **2.1.1** Identify and catalog all missing resource files from analysis:
  ```
  Missing Resources (15+ files):
  - 3D models (.3ds files)
  - Textures and graphics (.png, .jpg)
  - GUI icons and images
  - Help system images
  - Configuration templates
  ```

- [ ] **2.1.2** Update `src/qt_gui/Makefile.am` to install resources
  ```makefile
  # Add resource installation rules
  resourcesdir = $(pkgdatadir)/resources
  resources_DATA = cap_1.png cap_2.png cap_3.png \
                   head.3ds trackir.png \
                   logo.png linuxtrack.png
  ```

- [ ] **2.1.3** Verify resource loading in Qt5 application
  - Update resource file paths in application code
  - Test that all 3D models and textures load correctly
  - Fix any Qt5-specific resource loading issues

**Expected Outcome**: All GUI elements display correctly with proper graphics/models

---

### **Step 2.2: Help System Deployment** 📚
**Priority**: Medium | **Complexity**: Medium | **Est. Time**: 2-3 hours

**Actions**:
- [ ] **2.2.1** Install help documentation files
  ```makefile
  # Add to qt_gui/Makefile.am
  helpdir = $(pkgdatadir)/help
  help_DATA = help/*.htm help/*.png help/*.css
  ```

- [ ] **2.2.2** Fix help system paths in Qt5 GUI
  - Update `HelpView` class to find help files in install location
  - Test help system functionality
  - Ensure all help topics display correctly

- [ ] **2.2.3** Generate missing help files if needed
  - Create comprehensive help documentation
  - Include screenshots for Qt5 interface
  - Test help navigation and content display

**Expected Outcome**: Complete help system accessible from GUI

---

## 📋 PHASE 3: Desktop Integration

### **Step 3.1: Create Desktop Files** 🖥️
**Priority**: Medium | **Complexity**: Low | **Est. Time**: 1-2 hours

**Actions**:
- [ ] **3.1.1** Create `linuxtrack.desktop` file
  ```ini
  [Desktop Entry]
  Name=LinuxTrack
  Comment=Head tracking software for Linux
  Exec=/opt/linuxtrack/bin/ltr_gui
  Icon=/opt/linuxtrack/share/pixmaps/linuxtrack.png
  Terminal=false
  Type=Application
  Categories=System;Settings;HardwareSettings;
  ```

- [ ] **3.1.2** Create `mickey.desktop` file for standalone Mickey
  ```ini
  [Desktop Entry]
  Name=Mickey (LinuxTrack)
  Comment=Standalone head tracking input mapper
  Exec=/opt/linuxtrack/bin/mickey
  Icon=/opt/linuxtrack/share/pixmaps/mickey.png
  Terminal=false
  Type=Application
  Categories=System;Settings;HardwareSettings;
  ```

- [ ] **3.1.3** Install desktop files and icons
  ```makefile
  # Add to Makefile.am
  desktopdir = $(datadir)/applications
  desktop_DATA = linuxtrack.desktop mickey.desktop
  
  pixmapsdir = $(datadir)/pixmaps
  pixmaps_DATA = linuxtrack.png mickey.png
  ```

**Expected Outcome**: Applications appear in system application menu

---

### **Step 3.2: System Service Integration** ⚙️
**Priority**: Low | **Complexity**: Medium | **Est. Time**: 2-3 hours

**Actions**:
- [ ] **3.2.1** Create systemd service file (optional)
  - For automatic LinuxTrack server startup
  - User-level service for per-user operation
  - System-level service for multi-user systems

- [ ] **3.2.2** Add service installation option
  - Configure-time option to install systemd service
  - Installation scripts to enable/disable service
  - Documentation for service management

**Expected Outcome**: Optional automatic LinuxTrack server startup

---

## 📋 PHASE 4: Configuration System

### **Step 4.1: Configuration Files Installation** 📁
**Priority**: High | **Complexity**: Medium | **Est. Time**: 2-3 hours

**Actions**:
- [ ] **4.1.1** Install default configuration files
  ```makefile
  # Add to src/Makefile.am
  configdir = $(sysconfdir)/linuxtrack
  config_DATA = linuxtrack1.conf
  ```

- [ ] **4.1.2** Create user configuration directory handling
  - Automatically create `~/.config/linuxtrack/` on first run
  - Copy default configurations to user directory
  - Handle configuration migration from old versions

- [ ] **4.1.3** Install firmware and model files
  ```makefile
  # Add firmware and models installation
  firmwaredir = $(pkgdatadir)/firmware
  firmware_DATA = firmware/*.bin firmware/*.hex
  
  modelsdir = $(pkgdatadir)/models  
  models_DATA = models/*.ltr models/*.xml
  ```

**Expected Outcome**: Complete configuration system with defaults and user customization

---

## 📋 PHASE 5: Installation Verification & Testing

### **Step 5.1: Create Installation Verification Script** ✅
**Priority**: High | **Complexity**: Low | **Est. Time**: 2-3 hours

**Actions**:
- [ ] **5.1.1** Create comprehensive installation test script
  ```bash
  #!/bin/bash
  # verify_installation.sh
  echo "LinuxTrack Installation Verification"
  
  # Check all required files exist
  # Test udev rules are active
  # Verify desktop integration
  # Test application startup
  # Check permissions and paths
  ```

- [ ] **5.1.2** Add installation verification to build process
  - Optional `make verify` target
  - Post-installation verification option
  - Automated testing for package builds

- [ ] **5.1.3** Create troubleshooting guide
  - Common installation issues and solutions
  - Permission problems and fixes
  - Device detection troubleshooting

**Expected Outcome**: Reliable installation verification and troubleshooting

---

## 📋 PHASE 6: Package Integration

### **Step 6.1: Distribution Package Support** 📦
**Priority**: Medium | **Complexity**: High | **Est. Time**: 4-5 hours

**Actions**:
- [ ] **6.1.1** Create DEB package configuration
  - debian/control file with proper dependencies
  - debian/install files for correct file placement
  - Post-installation scripts for udev rules

- [ ] **6.1.2** Create RPM package configuration  
  - linuxtrack.spec file for RPM building
  - Proper file manifests and dependencies
  - Post-installation scriptlets

- [ ] **6.1.3** Test package installation
  - Clean system installation testing
  - Upgrade path testing
  - Uninstallation cleanup verification

**Expected Outcome**: Ready-to-distribute system packages

---

## 🏁 IMPLEMENTATION TIMELINE

### **Week 1: Core Installation (Phases 1-2)**
- Days 1-2: Installation paths and udev rules
- Days 3-5: Resource files and help system

### **Week 2: Integration & Testing (Phases 3-5)**  
- Days 1-2: Desktop integration
- Days 3-4: Configuration system
- Day 5: Verification and testing

### **Week 3: Packaging (Phase 6)**
- Days 1-3: Package creation and testing
- Days 4-5: Documentation and final verification

---

## 🎯 SUCCESS CRITERIA

### **Must-Have (100% Complete Installation)**
- [x] ✅ Installation defaults to `/opt/linuxtrack`
- [x] ✅ All GUI resources and 3D models install correctly
- [x] ✅ udev rules automatically installed and activated
- [x] ✅ Desktop integration (menu entries, icons)
- [x] ✅ Complete help system deployment
- [x] ✅ Configuration files and templates installed

### **Should-Have (Enhanced User Experience)**
- [x] ✅ Installation verification script
- [x] ✅ Comprehensive troubleshooting documentation
- [x] ✅ Clean uninstallation process

### **Nice-to-Have (Distribution Ready)**
- [x] ✅ DEB/RPM package support
- [x] ✅ Systemd service integration
- [x] ✅ Automated testing framework

---

## 🚀 GETTING STARTED

**Next Commands to Execute:**
```bash
# 1. Start with Phase 1.1 - Fix installation paths
./configure --help  # Review current configuration options
vim configure.ac     # Modify default installation path

# 2. Test current installation 
make && sudo make install  # See what's currently installed
ls -la /usr/local/   # Check current installation structure

# 3. Begin systematic implementation of each phase
```

**Ready to begin implementation!** Each phase builds upon the previous, creating a complete installation system that matches the original Qt4 functionality while leveraging modern Qt5 capabilities. 