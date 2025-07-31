# 🎮 Gaming Tab UI Design Plan

**Branch**: `gaming-tab-and-lutris-install`  
**Date**: January 2025  
**Status**: IMPLEMENTATION IN PROGRESS

---

## 📊 Implementation Progress

### ✅ Completed Features
- **Lutris Integration**: Fully functional with proper Wine version parsing
  - ✅ Wine version detection from YAML config files
  - ✅ Correct Lutris Wine path construction
  - ✅ Interactive NSIS installer launch
  - ✅ Game detection and alphabetical listing
  - ✅ Debug logging for troubleshooting

### 🔄 In Progress
- **Phase 2 Part 1**: Lutris integration (COMPLETED ✅)
- **Phase 2 Part 2**: Steam Proton integration (PLANNED)
- **Phase 2 Part 3**: Gaming Tab UI implementation (PLANNED)

### 📋 Pending Features
- **Phase 1**: Tab structure changes (Misc → System, add Gaming tab)
- **Phase 2**: Platform-specific buttons (Steam Proton, Other Platform)
- **Phase 3**: Utility buttons (Batch Install, etc.)
- **Phase 4**: TrackIR support buttons

---

## 🔧 Technical Implementation Details

### Lutris Integration (COMPLETED ✅)

#### Wine Version Parsing Fix
**Problem**: Original code looked for simple key-value pairs (`wine:` or `wine_version:`) but Lutris uses nested YAML structure.

**Solution**: Implemented proper YAML parsing that:
- Tracks indentation levels to understand structure
- Enters `wine:` section when found
- Looks for `version:` keys within wine section
- Handles nested YAML structure correctly

**Files Modified**:
- `src/qt_gui/lutris_integration.cpp` - Fixed Wine version parsing logic
- `src/qt_gui/lutris_integration.h` - Header file (no changes needed)

**Key Changes**:
```cpp
// OLD: Simple key-value search
if (trimmedLine.startsWith("wine:")) { ... }

// NEW: Proper YAML structure parsing
if (trimmedLine == "wine:") {
    inWineSection = true;
    wineIndent = indent;
}
if (inWineSection && indent > wineIndent) {
    if (trimmedLine.startsWith("version:")) {
        wineVersion = trimmedLine.mid(8).trimmed();
    }
}
```

**Test Results**:
- ✅ Correctly detects Wine versions like `lutris-GE-Proton8-15-x86_64`
- ✅ Constructs proper Wine path: `/home/mario/.local/share/lutris/runners/wine/[version]/bin/wine`
- ✅ Uses Lutris-specific Wine instead of system Wine
- ✅ NSIS installer remains interactive

---

## 📋 UI Design Overview

### Tab Structure Changes
- **New Tab**: "Gaming" (replaces "Wine/Steam" concept)
- **Renamed Tab**: "Misc." → "System"
- **Future-Proof**: Designed to accommodate multiple gaming platforms beyond Wine/Steam/Lutris

---

## 🎯 Gaming Tab Layout Design

### Proposed Button Grid Layout
```
┌─────────────────────────────────────────────────────────┐
│                    GAMING TAB                          │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌─────────────────────────────────────────────────┐   │
│  │           Install Linuxtrack-Wine support      │   │
│  │           (TIR Firmware + MFC140)              │   │
│  └─────────────────────────────────────────────────┘   │
│                                                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐   │
│  │   Steam     │  │   Lutris    │  │   Other     │   │
│  │  (Proton)   │  │             │  │  Platform   │   │
│  └─────────────┘  └─────────────┘  └─────────────┘   │
│                                                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐   │
│  │   Custom    │  │   Batch     │  │   Wine      │   │
│  │   Prefix    │  │  Install    │  │   Prefix    │   │
│  └─────────────┘  └─────────────┘  └─────────────┘   │
│                                                         │
│  ┌─────────────────────────────────────────────────┐   │
│  │           Launch Mickey (TrackIR)              │   │
│  └─────────────────────────────────────────────────┘   │
│                                                         │
│  ┌─────────────────────────────────────────────────┐   │
│  │           Launch ltr_pipe for Antimicrox       │   │
│  └─────────────────────────────────────────────────┘   │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### Button Functionality Breakdown

#### 1. **Install Linuxtrack-Wine support** (Top Center - Full Width)
**Function**: Handle TIR Firmware and MFC140 extraction only
- **Step 1**: TIR Firmware extraction (if not already installed)
- **Step 2**: MFC140 library extraction (if not already installed)
- **Note**: Does NOT proceed to wine bridge installation
- **Workflow**: Same as current implementation, but stops after MFC140

#### 2. **Platform-Specific Buttons** (Grid Row 1)
- **Steam (Proton)**: Install wine bridge to Steam Proton prefixes
- **Lutris**: Install wine bridge to Lutris wine prefixes
- **Other Platform**: Placeholder for future platforms (Epic, GOG, etc.)

#### 3. **Utility Buttons** (Grid Row 2)
- **Custom Prefix**: Manual wine prefix selection (current functionality)
- **Batch Install**: Install to multiple prefixes at once
- **Wine Prefix**: Direct wine prefix installation (legacy option)

#### 4. **TrackIR Support Buttons** (Full Width)
- **Launch Mickey (TrackIR)**: Launch TrackIR emulation
- **Launch ltr_pipe for Antimicrox**: Launch pipe for controller support

---

## 🔧 Technical Implementation Plan

### Phase 1: Tab Structure Changes

#### 1.1 Rename Misc Tab to System
**Files to Modify:**
- `src/qt_gui/ltr.ui` - Change tab title from "Misc." to "System"
- `src/qt_gui/ltr_gui.cpp` - Update any references to Misc tab
- `src/qt_gui/ltr_gui.h` - Update class documentation

**Implementation:**
```xml
<!-- In ltr.ui -->
<widget class="QWidget" name="tab_4">
  <attribute name="title">
   <string>System</string>
  </attribute>
```

#### 1.2 Add Gaming Tab
**Files to Modify:**
- `src/qt_gui/ltr.ui` - Add new Gaming tab
- `src/qt_gui/ltr_gui.cpp` - Add tab management
- `src/qt_gui/ltr_gui.h` - Add new class members

**Implementation:**
```xml
<!-- In ltr.ui -->
<widget class="QWidget" name="tab_5">
  <attribute name="title">
   <string>Gaming</string>
  </attribute>
  <layout class="QVBoxLayout" name="gamingLayout">
    <!-- Button grid layout -->
  </layout>
</widget>
```

### Phase 2: Button Implementation

#### 2.1 Modify Existing Wine Bridge Button
**Files to Modify:**
- `src/qt_gui/plugin_install.cpp` - Split functionality
- `src/qt_gui/plugin_install.h` - Add new methods

**Implementation:**
```cpp
// Split current installWinePlugin() into:
void PluginInstall::installTirFirmwareAndMfc140() {
    // Steps 1-2 only: TIR Firmware + MFC140
    if(!isTirFirmwareInstalled()){
        state = TIR_FW;
        tirFirmwareInstall();
    } else if(!isMfc140uInstalled()){
        state = MFC;
        mfc140uInstall();
    } else {
        // Both already installed
        QMessageBox::information(parentWidget, "Already Installed",
            "TIR Firmware and MFC140 libraries are already installed.");
    }
}

void PluginInstall::installWineBridgeToPrefix(const QString& prefix) {
    // Step 3 only: Wine bridge installation
    if(!isTirFirmwareInstalled() || !isMfc140uInstalled()) {
        QMessageBox::warning(parentWidget, "Prerequisites Required",
            "Please install TIR Firmware and MFC140 libraries first.");
        return;
    }
    
    // Proceed with wine bridge installation
    installLinuxtrackWine();
}
```

#### 2.2 Add Platform-Specific Buttons
**Files to Create:**
- `src/qt_gui/gaming_installer.cpp` - Gaming platform installer
- `src/qt_gui/gaming_installer.h` - Gaming installer interface

**Implementation:**
```cpp
class GamingInstaller {
public:
    // Steam Proton installation
    void installToSteamProton();
    
    // Lutris installation
    void installToLutris();
    
    // Custom prefix installation
    void installToCustomPrefix();
    
    // Batch installation
    void installToMultiplePrefixes();
};
```

### Phase 3: Button Grid Layout

#### 3.1 Gaming Tab Layout Design
**UI Structure:**
```xml
<layout class="QVBoxLayout" name="gamingLayout">
  <!-- Top: Full-width TIR/MFC140 button -->
  <item>
    <widget class="QPushButton" name="InstallTirMfcButton">
      <property name="text">
       <string>Install Linuxtrack-Wine support (TIR Firmware + MFC140)</string>
      </property>
    </widget>
  </item>
  
  <!-- Grid Row 1: Platform buttons -->
  <item>
    <layout class="QHBoxLayout" name="platformButtonsLayout">
      <item>
        <widget class="QPushButton" name="SteamProtonButton">
          <property name="text">
           <string>Steam (Proton)</string>
          </property>
        </widget>
      </item>
      <item>
        <widget class="QPushButton" name="LutrisButton">
          <property name="text">
           <string>Lutris</string>
          </property>
        </widget>
      </item>
      <item>
        <widget class="QPushButton" name="OtherPlatformButton">
          <property name="text">
           <string>Other Platform</string>
          </property>
        </widget>
      </item>
    </layout>
  </item>
  
  <!-- Grid Row 2: Utility buttons -->
  <item>
    <layout class="QHBoxLayout" name="utilityButtonsLayout">
      <item>
        <widget class="QPushButton" name="CustomPrefixButton">
          <property name="text">
           <string>Custom Prefix</string>
          </property>
        </widget>
      </item>
      <item>
        <widget class="QPushButton" name="BatchInstallButton">
          <property name="text">
           <string>Batch Install</string>
          </property>
        </widget>
      </item>
      <item>
        <widget class="QPushButton" name="WinePrefixButton">
          <property name="text">
           <string>Wine Prefix</string>
          </property>
        </widget>
      </item>
    </layout>
  </item>
  
  <!-- TrackIR Support buttons -->
  <item>
    <widget class="QPushButton" name="LaunchMickeyButton">
      <property name="text">
       <string>Launch Mickey (TrackIR)</string>
      </property>
    </widget>
  </item>
  <item>
    <widget class="QPushButton" name="LaunchLtrPipeButton">
      <property name="text">
       <string>Launch ltr_pipe for Antimicrox</string>
      </property>
    </widget>
  </item>
</layout>
```

---

## 🎯 Button Functionality Details

### 1. **Install Linuxtrack-Wine support** (TIR Firmware + MFC140)
**Current Implementation**: `plugin_install.cpp` - `installWinePlugin()`
**New Implementation**: Split into separate functions
- **Prerequisites Check**: Verify TIR firmware and MFC140 are installed
- **TIR Firmware**: Extract TrackIR firmware files
- **MFC140**: Extract Visual C++ 2015-2022 MFC libraries
- **Stop Point**: Do NOT proceed to wine bridge installation
- **User Feedback**: Clear indication when prerequisites are complete

### 2. **Steam (Proton)** Button
**Implementation**: New functionality
- **Detection**: Scan for Steam Proton prefixes
- **Selection**: Show list of available Proton prefixes
- **Installation**: Install wine bridge to selected Proton prefix
- **Verification**: Confirm installation success

### 3. **Lutris** Button
**Implementation**: Based on research findings
- **Database Access**: Query `~/.local/share/lutris/pga.db`
- **Prefix Discovery**: Parse YAML configuration files
- **Selection**: Show game names and wine versions
- **Installation**: Install wine bridge to selected Lutris prefix

### 4. **Custom Prefix** Button
**Implementation**: Current wine bridge functionality
- **File Dialog**: `QFileDialog::getExistingDirectory()`
- **Manual Selection**: User selects wine prefix
- **Installation**: Install wine bridge to selected prefix
- **Legacy Support**: Maintains current workflow

### 5. **Wine Prefix** Button
**Implementation**: Direct wine prefix installation
- **Simple Interface**: Quick wine prefix selection
- **Basic Installation**: Minimal environment setup
- **Fallback Option**: For users who prefer direct control

### 6. **Batch Install** Button
**Implementation**: New functionality
- **Multiple Selection**: Allow selection of multiple prefixes
- **Batch Processing**: Install to all selected prefixes
- **Progress Indication**: Show installation progress
- **Error Handling**: Handle partial failures

---

## 🔄 Workflow Preservation

### Current Workflow (Preserved)
```
1. TIR Firmware Extraction → 2. MFC140 Extraction → 3. Wine Bridge Installation
```

### New Workflow (Split)
```
Step 1-2: Install Linuxtrack-Wine support (TIR Firmware + MFC140)
Step 3: Platform-specific wine bridge installation
```

### Benefits of Split Workflow
1. **Clear Separation**: Prerequisites vs. actual installation
2. **Multiple Targets**: Install to multiple platforms after prerequisites
3. **User Control**: Users can choose specific platforms
4. **Error Isolation**: Easier to debug and handle failures
5. **Future Expansion**: Easy to add new platforms

---

## 📊 Implementation Priority

### Phase 1 (Immediate)
1. **Rename Misc to System**
2. **Add Gaming Tab**
3. **Split TIR/MFC140 functionality**
4. **Move existing wine bridge button**

### Phase 2 (Short-term)
1. **Implement Lutris button** (based on research)
2. **Implement Steam Proton button**
3. **Add Custom Prefix button**

### Phase 3 (Medium-term)
1. **Add Batch Install functionality**
2. **Add Other Platform button**
3. **Implement advanced features**

---

## 🎯 Success Criteria

### User Experience
- [ ] **Clear Workflow**: Users understand the split between prerequisites and installation
- [ ] **Intuitive Layout**: Button grid is easy to understand and use
- [ ] **Platform Flexibility**: Users can install to multiple platforms
- [ ] **Error Handling**: Clear feedback for all failure scenarios

### Technical Implementation
- [ ] **Backward Compatibility**: Existing functionality continues to work
- [ ] **Modular Design**: Easy to add new platforms
- [ ] **Error Recovery**: Graceful handling of installation failures
- [ ] **Performance**: Efficient prefix detection and installation

### Future Expansion
- [ ] **Extensible Architecture**: Easy to add new gaming platforms
- [ ] **Consistent Interface**: New platforms follow same patterns
- [ ] **Documentation**: Clear documentation for adding new platforms

---

*Last Updated: January 29, 2025*  
*Status: Lutris Integration COMPLETED ✅*  
*Next Review: February 5, 2025* 