# Testing Section for Gaming Tab - Technical Plan

**Feature Number**: 0001  
**Date**: August 2025  
**Status**: PLANNING

---

## 📋 Feature Description

Add a Testing section to the Gaming Tab that allows users to select and run either Tester.exe or FT_Tester (FreeTrack Tester) in any available wine prefixes. The system will leverage existing game discovery and sorting logic from the wine bridge installers to present users with available games across Steam, Lutris, and Custom Prefix platforms.

---

## 🎯 Technical Requirements

### Core Functionality
1. **Tester Selection**: Allow users to choose between Tester.exe (TrackIR) or FT_Tester (FreeTrack)
2. **Automatic Tracking**: Start tracking immediately when user begins testing workflow
3. **Platform Selection**: Present Steam, Lutris, and Custom Prefix as platform options
4. **Game Discovery**: Use existing game discovery logic to show available games for selected platform
5. **Prefix Execution**: Execute selected tester in the correct wine prefix for chosen game
6. **Future Expansion**: Design to accommodate additional platforms beyond current three

### User Workflow
1. User selects desired tester (Tester.exe or FT_Tester)
2. **System automatically starts tracking** (same as existing wine bridge installation workflow)
3. User selects platform (Steam, Lutris, Custom Prefix)
4. System displays available games for selected platform using existing discovery logic
5. User selects specific game
6. **System checks for testers in selected prefix**
7. **If testers missing**: Offer user option to install Wine Bridge or cancel
8. **If testers found**: Execute selected tester in the correct wine prefix for that game

---

## 🔧 Implementation Components

### Files to Modify

#### UI Components
- `src/qt_gui/ltr.ui` - Add Testing section to Gaming Tab layout
- `src/qt_gui/ltr_gui.cpp` - Add Testing section button handlers
- `src/qt_gui/ltr_gui.h` - Add Testing section class members

#### Testing Logic
- `src/qt_gui/plugin_install.cpp` - Add tester execution methods
- `src/qt_gui/plugin_install.h` - Add tester execution method declarations
- `src/qt_gui/steam_integration.cpp` - Reuse Steam game discovery logic
- `src/qt_gui/lutris_integration.cpp` - Reuse Lutris game discovery logic

#### Wine Bridge Integration
- `src/wine_bridge/ltr_wine.nsi.in` - Reference existing tester installation paths
- `src/wine_bridge/ltr_wine64.nsi.in` - Reference existing 64-bit tester paths

### New Files to Create
- `src/qt_gui/testing_section.cpp` - Main testing section implementation
- `src/qt_gui/testing_section.h` - Testing section interface
- `src/qt_gui/tester_launcher.cpp` - Tester execution logic
- `src/qt_gui/tester_launcher.h` - Tester launcher interface

---

## 🎮 Platform Integration Strategy

### Steam Proton Integration
- **Game Discovery**: Reuse existing Steam library parsing from `steam_integration.cpp`
- **Prefix Detection**: Use existing Steam Proton prefix detection logic
- **Tester Location**: Look for testers in Steam Proton prefix directories
- **Environment Setup**: Use existing Steam Proton environment configuration

### Lutris Integration
- **Game Discovery**: Reuse existing Lutris game detection from `lutris_integration.cpp`
- **Wine Version**: Use existing Lutris Wine version detection logic
- **Prefix Path**: Use existing Lutris prefix path construction
- **Environment Setup**: Configure Lutris-specific Wine environment

### Custom Prefix Integration
- **Manual Selection**: Allow user to browse and select custom wine prefix
- **Tester Detection**: Search for testers in user-selected prefix
- **Environment Setup**: Use system Wine or user-specified Wine version
- **Validation**: Verify prefix contains required tester executables

---

## 🔍 Tester Detection Algorithm

### Tester File Locations
1. **Primary Location**: Wine prefix root directory (where NSIS installer placed testers)
2. **Secondary Location**: Common Windows program directories within prefix
3. **Fallback Location**: System-wide wine bridge installation directories

### Detection Process
1. **Prefix Validation**: Verify selected wine prefix exists and is valid
2. **Tester Search**: Search for Tester.exe/Tester64.exe or FT_Tester.exe in prefix
3. **Missing Tester Handling**: If testers not found, offer user option to install Wine Bridge
4. **Version Detection**: Determine if 32-bit or 64-bit testers are available
5. **Path Construction**: Build full path to tester executable
6. **Execution Validation**: Verify tester is executable and can be launched

---

## 🎯 UI Design Strategy

### Testing Section Layout
- **Section Header**: "Testing" with clear description
- **Tester Selection**: Radio buttons for Tester.exe vs FT_Tester
- **Platform Selection**: Dropdown or buttons for Steam/Lutris/Custom
- **Game List**: Scrollable list of available games for selected platform
- **Execute Button**: Launch selected tester in chosen game's prefix

### User Experience Flow
1. **Clear Workflow**: Step-by-step selection process
2. **Automatic Tracking**: Tracking starts immediately when testing begins
3. **Visual Feedback**: Show selected options and current step
4. **Missing Tester Handling**: Offer Wine Bridge installation when testers not found
5. **Error Handling**: Clear messages for missing testers or invalid prefixes
6. **Progress Indication**: Show when tester is launching
7. **Result Feedback**: Indicate successful launch or error conditions

---

## 🔧 Technical Implementation Phases

### Phase 1: Core Testing Infrastructure
- Create testing section UI components
- Implement basic tester detection logic
- Add tester execution framework
- Create error handling and user feedback

### Phase 2: Platform Integration
- Integrate with existing Steam game discovery
- Integrate with existing Lutris game discovery
- Implement custom prefix selection
- Add platform-specific environment setup

### Phase 3: UI Polish and Testing
- Complete UI layout and styling
- Add comprehensive error handling
- Implement user feedback and progress indicators
- Test across all supported platforms

---

## 🎯 Success Criteria

### Functional Requirements
- Users can select between Tester.exe and FT_Tester
- **Tracking starts automatically when testing workflow begins**
- Users can choose from Steam, Lutris, or Custom Prefix platforms
- System displays available games for selected platform
- **System checks for testers in selected prefix before execution**
- **If testers missing, offer Wine Bridge installation option**
- Selected tester launches in correct wine prefix
- System handles missing testers gracefully
- System provides clear error messages and feedback

### Technical Requirements
- Reuses existing game discovery logic from wine bridge installers
- Maintains compatibility with existing wine bridge installation
- Supports both 32-bit and 64-bit tester executables
- Handles different wine versions and environments
- Provides extensible architecture for future platforms

### User Experience Requirements
- Intuitive step-by-step workflow
- Clear visual feedback for all actions
- Helpful error messages for troubleshooting
- Consistent experience across all platforms
- Fast and responsive interface

---

## 🔍 Risk Assessment

### Technical Risks
- **Tester Location**: Testers may not be found in expected locations
- **Missing Wine Bridge**: Selected prefix may not have Wine Bridge installed
- **Wine Environment**: Different wine versions may affect tester execution
- **Platform Differences**: Steam Proton vs Lutris vs Custom may have different requirements
- **Permission Issues**: Wine prefix access permissions may prevent tester execution

### Mitigation Strategies
- **Comprehensive Search**: Search multiple locations for tester executables
- **Missing Tester Detection**: Check for testers before attempting execution
- **Wine Bridge Installation Offer**: Provide immediate option to install Wine Bridge when testers missing
- **Environment Validation**: Verify wine environment before launching testers
- **Platform-Specific Logic**: Handle each platform's unique requirements
- **Permission Checking**: Verify prefix access before attempting execution
- **Fallback Options**: Provide alternative execution methods if primary fails

---

## 🚀 Future Expansion Considerations

### Additional Platforms
- **Epic Games Store**: Integration with Epic Games Launcher
- **GOG Galaxy**: Integration with GOG Galaxy client
- **Battle.net**: Integration with Battle.net launcher
- **Custom Launchers**: Support for other gaming platforms

### Enhanced Testing Features
- **Batch Testing**: Test multiple games simultaneously
- **Automated Testing**: Run tests without user interaction
- **Test Results**: Capture and display tester output
- **Test History**: Track previous test results and configurations

### Advanced Integration
- **Game-Specific Testing**: Different testers for different game types
- **Profile Management**: Save and load testing configurations
- **Remote Testing**: Test wine prefixes on remote systems
- **Cloud Integration**: Sync testing results across devices 