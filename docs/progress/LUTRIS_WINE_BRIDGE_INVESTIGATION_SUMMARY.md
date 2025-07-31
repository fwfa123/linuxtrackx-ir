# 🎮 Lutris Wine Bridge Installation - Investigation Summary

**Branch**: `gaming-tab-and-lutris-install`  
**Date**: January 2025  
**Status**: INVESTIGATION IN PROGRESS

---

## 📋 Current Investigation Status

### ✅ Completed Analysis

#### 1. Current Wine Bridge Implementation
**Key Findings:**

**File Structure:**
- `src/qt_gui/plugin_install.cpp` - Main wine bridge installation logic
- `src/qt_gui/wine_launcher.cpp` - Wine environment and prefix management
- `src/qt_gui/ltr.ui` - Current GUI layout with 4 tabs
- `scripts/install/create_wine_bridge_package.sh` - Wine bridge packaging

**Current Installation Process:**
1. **Firmware Check**: Verifies TrackIR firmware and MFC140 libraries are installed
2. **Prefix Selection**: Uses `QFileDialog::getExistingDirectory()` for manual prefix selection
3. **Wine Environment**: Sets `WINEPREFIX` environment variable
4. **Installer Execution**: Runs `linuxtrack-wine.exe` installer in selected prefix
5. **Error Handling**: Basic error handling with user feedback

**Current GUI Structure:**
```
Current Tabs:
├── Device Setup
├── Model Setup  
├── Tracking setup
└── Misc. (contains current wine bridge button)
```

**Current Wine Bridge Button:**
- Located in "Misc." tab
- Button name: `LinuxtrackWineButton`
- Text: "Install Linuxtrack-Wine support..."
- Currently disabled by default until wine installer is found

#### 2. Wine Prefix Management Analysis

**WineLauncher Class Capabilities:**
- **Wine Version Detection**: `selectBestWineVersion()` method
- **Environment Management**: `setEnv()` method for wine environment variables
- **Process Execution**: `run()` method for wine command execution
- **Error Handling**: Comprehensive error logging and user feedback

**Wine Environment Setup:**
- Sets `WINEARCH=win64` for 64-bit prefixes
- Configures wine debug output: `WINEDEBUG=+err+fixme`
- Handles wine path detection and selection
- Supports cross-distribution wine compatibility

#### 3. Installation Script Analysis

**Wine Bridge Package Structure:**
- **Main Installer**: `linuxtrack-wine.exe` (NSIS-based)
- **DLL Files**: Various TrackIR and FreeTrack compatibility DLLs
- **Registry Entries**: Windows registry modifications for DLL registration
- **Silent Installation**: Supports `/S` parameter for silent installation

**Installation Script Features:**
- **Distribution Detection**: Automatic detection of Linux distribution
- **Wine Development Tools**: Installation of required wine development packages
- **Cross-Platform Support**: Handles different package managers (apt, dnf, pacman)
- **Error Recovery**: Fallback options for failed installations

---

## 🔍 Lutris Integration Research Needed

### 1. Lutris Wine Prefix Structure (To Investigate)

**Expected Lutris Directories:**
```
~/.local/share/lutris/
├── runners/wine/          # Wine runners and versions
├── games/                 # Game-specific configurations
└── wineprefix/           # Wine prefixes (if any)

~/.config/lutris/
├── games/                 # Game configuration files (YAML)
└── lutris.conf           # Main Lutris configuration
```

**Research Questions:**
- [ ] How does Lutris organize wine prefixes?
- [ ] Are wine prefixes stored in a standard location?
- [ ] How does Lutris manage different wine versions?
- [ ] What's the relationship between games and wine prefixes?

### 2. Lutris Configuration Analysis (To Investigate)

**Expected Configuration Format:**
- **YAML Configuration**: Game-specific settings in YAML format
- **Wine Runner Configuration**: Wine version and prefix settings
- **Game-Specific Settings**: Individual game wine prefix configurations

**Research Questions:**
- [ ] How to parse Lutris YAML configuration files?
- [ ] How to identify wine prefixes for specific games?
- [ ] How to detect available Lutris wine runners?
- [ ] How to integrate with Lutris wine prefix management?

### 3. Lutris API/Integration Methods (To Investigate)

**Potential Integration Approaches:**
1. **Direct File System Access**: Read Lutris configuration files directly
2. **Lutris Command Line**: Use Lutris CLI if available
3. **Wine Prefix Detection**: Scan for Lutris-managed wine prefixes
4. **Configuration Parsing**: Parse Lutris YAML configuration files

**Research Questions:**
- [ ] Does Lutris provide a command-line interface?
- [ ] Are there existing tools for Lutris integration?
- [ ] How to safely modify Lutris wine prefixes?
- [ ] What are the best practices for external Lutris integration?

---

## 🎯 Technical Implementation Plan

### Phase 1: GUI Modifications

#### 1.1 New Wine/Steam Tab Design
**Proposed Layout:**
```
Wine/Steam Tab:
├── Wine Bridge Installation Section
│   ├── Install Wine Bridge (Default Prefix)
│   ├── Install Wine Bridge in Steam Proton
│   └── Install Wine Bridge in Lutris Prefix
├── TrackIR Support Section
│   ├── Launch Mickey (TrackIR)
│   └── Launch ltr_pipe for Antimicrox
└── System Information Section (Future)
    └── Wine/Proton/Lutris Detection
```

**Required UI Changes:**
- Add new tab to `src/qt_gui/ltr.ui`
- Move existing wine bridge button from Misc tab
- Add new buttons for Steam Proton and Lutris
- Add system information display area

#### 1.2 Code Structure Modifications
**Files to Modify:**
- `src/qt_gui/ltr.ui` - Add new Wine/Steam tab
- `src/qt_gui/ltr_gui.cpp` - Add tab management and button handlers
- `src/qt_gui/ltr_gui.h` - Add new class members and methods
- `src/qt_gui/plugin_install.cpp` - Add Lutris installation logic

**New Files to Create:**
- `src/qt_gui/lutris_installer.cpp` - Lutris-specific installer class
- `src/qt_gui/lutris_installer.h` - Lutris installer header
- `scripts/install/install_lutris_wine_bridge.sh` - Lutris installation script

### Phase 2: Lutris Integration Implementation

#### 2.1 Lutris Detection and Configuration
**Implementation Plan:**
1. **Lutris Installation Detection**
   - Check for Lutris installation in standard locations
   - Verify Lutris configuration directory exists
   - Detect available Lutris wine runners

2. **Wine Prefix Discovery**
   - Parse Lutris game configuration files
   - Extract wine prefix paths from YAML configurations
   - Validate wine prefix accessibility and permissions

3. **User Interface for Prefix Selection**
   - Display list of available Lutris wine prefixes
   - Allow user to select specific prefix for installation
   - Provide prefix information (game name, wine version, etc.)

#### 2.2 Installation Process Design
**Process Flow:**
1. **Lutris Detection**
   ```cpp
   bool detectLutrisInstallation();
   QStringList getLutrisWinePrefixes();
   QString getLutrisConfigurationPath();
   ```

2. **Prefix Selection**
   ```cpp
   QString selectLutrisWinePrefix(QWidget* parent);
   bool validateWinePrefix(const QString& prefix);
   ```

3. **Installation Execution**
   ```cpp
   bool installWineBridgeToLutrisPrefix(const QString& prefix);
   bool verifyWineBridgeInstallation(const QString& prefix);
   ```

4. **Error Handling**
   ```cpp
   void handleLutrisInstallationError(const QString& error);
   bool rollbackLutrisInstallation(const QString& prefix);
   ```

### Phase 3: Error Handling and User Experience

#### 3.1 Error Scenarios
**Identified Error Cases:**
- [ ] Lutris not installed
- [ ] No Lutris wine prefixes found
- [ ] Wine bridge files not available
- [ ] Installation permission issues
- [ ] Wine bridge already installed
- [ ] Installation verification failure

#### 3.2 User Experience Design
**User Interface Flow:**
1. **Button Click** → Check Lutris installation
2. **Lutris Found** → Scan for wine prefixes
3. **Prefixes Found** → Show selection dialog
4. **Prefix Selected** → Execute installation
5. **Installation Complete** → Verify and provide feedback

**Error Handling Strategy:**
- **User-Friendly Messages**: Clear, actionable error messages
- **Detailed Logging**: Comprehensive logging for debugging
- **Graceful Fallbacks**: Alternative installation methods
- **Rollback Capability**: Ability to undo failed installations

---

## 🚀 Next Investigation Steps

### Immediate Actions (This Week)

#### 1. Lutris Installation and Analysis
- [ ] **Install Lutris on test system**
  - Document installation process
  - Analyze configuration directory structure
  - Identify wine prefix organization

#### 2. Lutris Configuration Research
- [ ] **Analyze Lutris configuration files**
  - Document YAML configuration format
  - Identify wine prefix configuration patterns
  - Research wine runner management

#### 3. Wine Prefix Structure Analysis
- [ ] **Create test Lutris wine prefixes**
  - Document prefix directory structure
  - Analyze wine version management
  - Test wine bridge compatibility

### Week 2 Actions

#### 1. Integration Method Research
- [ ] **Research Lutris integration methods**
  - Investigate Lutris command-line tools
  - Research existing Lutris integrations
  - Document available integration approaches

#### 2. Technical Design
- [ ] **Design implementation approach**
  - Plan code structure modifications
  - Design user interface flow
  - Plan error handling strategy

### Week 3 Actions

#### 1. Implementation Planning
- [ ] **Create detailed implementation plan**
  - List all required code changes
  - Plan testing strategy
  - Create timeline and milestones

#### 2. Begin Implementation
- [ ] **Start with GUI modifications**
  - Add new Wine/Steam tab
  - Implement basic button functionality
  - Begin Lutris detection logic

---

## 📊 Investigation Progress

### ✅ Completed (Phase 1)
- [x] **Current Wine Bridge Implementation Analysis**
  - Documented installation process flow
  - Analyzed wine prefix management
  - Identified GUI structure and button handling
  - Documented error handling patterns

- [x] **Wine Environment Management Analysis**
  - Analyzed WineLauncher class capabilities
  - Documented wine version detection
  - Identified environment variable handling
  - Documented cross-distribution compatibility

- [x] **Installation Script Analysis**
  - Analyzed wine bridge package structure
  - Documented installation script features
  - Identified distribution detection methods
  - Documented error recovery mechanisms

### 🔄 In Progress (Phase 2)
- [ ] **Lutris Integration Research**
  - Lutris installation and configuration analysis
  - Wine prefix structure investigation
  - Integration method research

### ⏳ Planned (Phase 3)
- [ ] **Technical Design**
  - GUI modification planning
  - Code structure design
  - Implementation approach planning

### ⏳ Planned (Phase 4)
- [ ] **Implementation Planning**
  - Detailed code modification plan
  - Testing strategy development
  - Timeline and milestone creation

---

## 📝 Key Findings and Insights

### Current Implementation Strengths
1. **Robust Wine Management**: The WineLauncher class provides excellent wine environment management
2. **Cross-Distribution Support**: Current implementation handles different Linux distributions well
3. **Error Handling**: Good error handling and user feedback mechanisms
4. **Modular Design**: Clean separation between GUI and installation logic

### Challenges Identified
1. **Lutris Integration Complexity**: Lutris may not have a simple API for external integration
2. **Configuration Parsing**: Need to parse Lutris YAML configuration files
3. **Wine Prefix Management**: Different wine versions and configurations may require different approaches
4. **User Experience**: Need to provide clear feedback for complex installation process

### Opportunities
1. **Reusable Components**: Can leverage existing WineLauncher class for Lutris integration
2. **Consistent UI**: Can follow existing GUI patterns for new features
3. **Error Handling**: Can extend existing error handling patterns
4. **Documentation**: Can build on existing documentation structure

---

## 🎯 Success Criteria

### Technical Success
- [ ] **Lutris Detection**: Reliably detect Lutris installation and configuration
- [ ] **Prefix Discovery**: Successfully identify and list Lutris wine prefixes
- [ ] **Installation Process**: Successfully install wine bridge to selected Lutris prefix
- [ ] **Error Handling**: Comprehensive error handling for all failure scenarios
- [ ] **User Feedback**: Clear progress indication and error messages

### User Experience Success
- [ ] **Simple Interface**: Intuitive user interface for Lutris integration
- [ ] **Clear Feedback**: Users understand what's happening during installation
- [ ] **Fallback Options**: Alternative methods if Lutris integration fails
- [ ] **Documentation**: Clear documentation for users

### Integration Success
- [ ] **Backward Compatibility**: Existing functionality continues to work
- [ ] **Cross-Distribution Support**: Works across different Linux distributions
- [ ] **Testing Coverage**: Comprehensive testing of new functionality
- [ ] **Documentation**: Updated documentation reflecting new features

---

*Last Updated: January 2025*  
*Next Review: [Date + 1 week]* 