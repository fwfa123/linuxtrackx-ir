# 🧪 Wine Prefix Tester Integration TODO

**Branch**: `feature/wine-prefix-tester-integration`  
**Date**: August 2, 2025  
**Status**: PLANNING

---

## 📋 Overview

This feature integrates Tester.exe/Tester64.exe execution directly into the NSIS installer workflow, automatically launching the tester immediately after Wine Bridge installation completes. This provides seamless TrackIR device verification across all gaming platforms (Steam Proton, Lutris, Custom Prefix) without requiring additional user interaction.

## 🎯 Goals

### Primary Objectives
1. **Automatic Testing**: Launch Tester.exe/Tester64.exe automatically after NSIS installation
2. **Seamless Integration**: Integrate testing into existing installation workflow
3. **Cross-Platform Support**: Work with Steam Proton, Lutris, and Custom Prefixes
4. **Immediate Validation**: Provide instant TrackIR device verification

### User Experience Goals
- **Zero-Click Testing**: Automatic testing without user interaction
- **Seamless Workflow**: Installation → Testing happens automatically
- **Clear Results**: Understandable feedback about TrackIR detection
- **Platform Agnostic**: Same experience across all gaming platforms
- **Error Handling**: Graceful handling of testing failures

---

## 🔧 Technical Requirements

### Core Functionality
1. **NSIS Integration**: Modify NSIS installer to launch Tester.exe/Tester64.exe at completion
2. **Automatic Execution**: Launch tester in the same environment as NSIS installer
3. **User Interaction**: Let user interact directly with Tester.exe/Tester64.exe UI
4. **Error Handling**: Handle missing files, execution failures, etc.

### Simplified Implementation Approach
Since the Tester executables are already installed to the Wine prefix during the NSIS installation process, and we're leveraging the existing NSIS installer environment, our implementation is extremely simplified:

- **Perfect Environment**: Already in correct Wine/Proton environment with all variables set
- **No Additional Setup**: No need to recreate environment or manage processes
- **Automatic Execution**: Tester launches immediately after installation completes
- **Seamless Integration**: Part of the existing installation workflow
- **Cross-Platform**: Works with Steam Proton, Lutris, and Custom Prefixes automatically

### Platform-Specific Requirements

#### Steam Proton
- **Environment Variables**: Use same environment as NSIS installer
- **Proton Integration**: Launch tester within Proton environment
- **Prefix Detection**: Use Steam Proton prefix detection logic

#### Lutris
- **Wine Version**: Use Lutris-specific Wine version
- **Prefix Path**: Use Lutris prefix detection logic
- **Environment Setup**: Configure Lutris-specific environment

#### Custom Prefix
- **Direct Wine**: Use system Wine or specified Wine version
- **Prefix Path**: Use user-selected prefix path
- **Environment Variables**: Set up basic Wine environment

---

## 📁 Implementation Plan

### Phase 1: NSIS Integration

#### 1.1 NSIS Installer Modification
**Files to Modify:**
- `src/wine_bridge/installer/linuxtrack-wine.nsi` - Add tester execution
- `src/wine_bridge/installer/linuxtrack-wine64.nsi` - Add tester execution for 64-bit

**Implementation:**
```nsis
; Add to NSIS installer at the end
Section "Post-Installation Testing"
    ; Launch Tester.exe from installed location
    ExecWait '"$INSTDIR\Tester.exe"'
    ; Or for 64-bit: ExecWait '"$INSTDIR\Tester64.exe"'
    
    ; Capture exit code and return to Qt
    SetOutPath $TEMP
    FileOpen $0 "$TEMP\tester_result.txt" w
    FileWrite $0 $R0  ; Exit code
    FileClose $0
SectionEnd
```

#### 1.2 Automatic Tester Launch
**Files to Modify:**
- `src/wine_bridge/installer/linuxtrack-wine.nsi` - Add tester execution
- `src/wine_bridge/installer/linuxtrack-wine64.nsi` - Add tester execution for 64-bit

**Implementation:**
```nsis
; Add to NSIS installer at the end
Section "Post-Installation Testing"
    ; Launch Tester.exe from installed location
    Exec '"$INSTDIR\Tester.exe"'
    ; Or for 64-bit: Exec '"$INSTDIR\Tester64.exe"'
SectionEnd
```

#### 1.3 User Interaction
**Implementation:**
- Tester.exe/Tester64.exe launches with its own UI
- User interacts directly with tester interface
- No additional Qt dialogs needed
- Tester handles all device detection and feedback

### Phase 2: Cross-Platform Testing

#### 2.1 Universal Integration
**Implementation:**
- NSIS installer automatically works with all platforms
- Same tester execution for Steam Proton, Lutris, and Custom Prefix
- No platform-specific code needed for tester execution
- Results parsing is platform-agnostic

#### 2.2 Universal Integration
**Implementation:**
- NSIS installer automatically works with all platforms
- Same tester execution for Steam Proton, Lutris, and Custom Prefix
- No platform-specific code needed for tester execution
- Tester handles all device detection and user interaction

### Phase 3: UI Integration

#### 3.1 Automatic Testing Flow
**Implementation:**
- Tester runs automatically after successful Wine Bridge installation
- No additional user interaction required
- Seamless installation → testing workflow
- Results displayed immediately after testing

#### 3.2 Automatic Tester Launch
**Implementation:**
- Tester.exe/Tester64.exe launches automatically after installation
- User interacts directly with tester's built-in UI
- No additional Qt dialogs needed
- Tester provides all necessary feedback and device information

#### 3.3 Error Handling and User Guidance
**Implementation:**
- Clear error messages for common issues
- Troubleshooting suggestions
- Links to documentation
- Option to retry testing if needed

---

## 🎨 UI Design

### Automatic Testing Flow
```
1. Wine Bridge Installation → 2. Tester.exe automatically launches → 3. Testing Results displayed
```

### User Experience
- **Automatic Launch**: Tester.exe/Tester64.exe launches immediately after installation
- **Direct Interaction**: User interacts directly with tester's built-in UI
- **Built-in Feedback**: Tester provides all device detection and status information
- **No Additional Dialogs**: No need for separate Qt result dialogs

---

## 🔍 Technical Challenges

### 1. Tester Executable Location
**Challenge**: Finding Tester.exe/Tester64.exe in Wine prefix after NSIS installation
**Solution**: 
- Tester executables are already installed to the Wine prefix during NSIS installation
- Look for Tester.exe/Tester64.exe in the Wine prefix directory
- Use the same prefix path where Wine Bridge was installed
- No need to search system directories - testers are prefix-specific

### 2. NSIS Integration
**Challenge**: Modifying NSIS installer to launch tester automatically
**Solution**:
- Add tester execution to NSIS installer script
- Use ExecWait to launch Tester.exe/Tester64.exe
- Capture exit code and output
- Handle both 32-bit and 64-bit testers

### 3. Automatic Launch
**Challenge**: Launching tester automatically after NSIS installation
**Solution**:
- Use Exec command in NSIS to launch Tester.exe/Tester64.exe
- Launch in same environment as NSIS installer
- Let user interact directly with tester UI
- No need to capture or parse results

### 4. User Experience
**Challenge**: Providing seamless testing experience
**Solution**:
- Tester launches automatically after installation
- User interacts directly with tester's built-in UI
- Tester handles all device detection and feedback
- No additional Qt dialogs needed

---

## 🧪 Testing Strategy

### Unit Testing
- **NSIS Integration**: Test NSIS installer modifications
- **Tester Execution**: Test automatic tester launch
- **User Interaction**: Test that tester UI launches correctly
- **Error Handling**: Test failure scenarios

### Integration Testing
- **Steam Proton**: Test with real Steam Proton prefixes
- **Lutris**: Test with real Lutris prefixes
- **Custom Prefix**: Test with manually selected prefixes
- **Error Scenarios**: Test with missing files, wrong permissions, etc.

### User Testing
- **End-to-End Flow**: Complete installation → testing workflow
- **Error Handling**: Test with various failure scenarios
- **UI Experience**: Test user interface and feedback
- **Cross-Platform**: Test across different gaming platforms

---

## 📊 Success Criteria

### Functional Requirements
- [ ] **NSIS Integration**: Automatically launches Tester.exe/Tester64.exe after installation
- [ ] **User Interaction**: Tester UI launches and user can interact with it
- [ ] **Cross-Platform**: Works with Steam Proton, Lutris, Custom Prefix automatically
- [ ] **Error Handling**: Graceful handling of all error scenarios

### User Experience Requirements
- [ ] **Automatic Testing**: Zero-click testing after installation
- [ ] **Direct Interaction**: User can interact directly with Tester.exe/Tester64.exe UI
- [ ] **Built-in Feedback**: Tester provides all necessary device detection and status
- [ ] **Consistent Experience**: Same workflow across all platforms

### Technical Requirements
- [ ] **Robust Implementation**: Handles edge cases and errors
- [ ] **Performance**: Fast testing without blocking UI
- [ ] **Maintainability**: Clean, well-documented code
- [ ] **Extensibility**: Easy to add new platforms

---

## 🚀 Implementation Priority

### Phase 1: NSIS Integration (Week 1)
1. **Modify NSIS Installer**: Add tester execution to installer scripts
2. **Result Capture**: Capture tester output and exit code
3. **Basic Result Parsing**: Parse and display basic results
4. **Error Handling**: Handle tester execution failures

### Phase 2: Cross-Platform Testing (Week 2)
1. **Universal Integration**: Test with Steam Proton, Lutris, Custom Prefix
2. **Result Display**: Create comprehensive results dialog
3. **Error Handling**: Platform-specific error handling
4. **User Experience**: Polish automatic testing workflow

### Phase 3: UI Integration (Week 3)
1. **Automatic Testing Flow**: Seamless installation → testing workflow
2. **Results Display**: Create results dialog and feedback
3. **Error Handling**: Comprehensive error messages and guidance
4. **User Experience**: Polish UI and workflow

### Phase 4: Testing and Polish (Week 4)
1. **Comprehensive Testing**: Test all scenarios and platforms
2. **Documentation**: Update documentation with testing features
3. **Code Review**: Final review and cleanup
4. **User Testing**: Real-world testing and feedback

---

## 📋 Next Steps

### Immediate Actions
1. **Research NSIS Installer**: Locate and understand NSIS installer scripts
2. **Study Tester Executable**: Understand Tester.exe/Tester64.exe behavior and output
3. **Modify NSIS Scripts**: Add tester execution to installer scripts
4. **Test Basic Functionality**: Verify tester launches automatically after NSIS installation

### Success Metrics
- [ ] Tester launches automatically after NSIS installation
- [ ] User can interact with Tester.exe/Tester64.exe UI
- [ ] Tester provides device detection and status information
- [ ] Works across all gaming platforms automatically
- [ ] Error handling provides helpful feedback

---

*Last Updated: August 2, 2025*  
*Status: Planning Phase*  
*Next Review: After Phase 1 completion* 