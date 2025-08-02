# 🧪 Wine Prefix Tester Integration TODO

**Branch**: `feature/wine-prefix-tester-integration`  
**Date**: August 2, 2025  
**Status**: PLANNING

---

## 📋 Overview

This feature adds the ability to run Tester.exe/Tester4.exe within Wine prefixes after Wine Bridge installation, allowing users to verify TrackIR device detection and functionality. This will be implemented across all gaming platforms (Steam Proton, Lutris, Custom Prefix).

## 🎯 Goals

### Primary Objectives
1. **Post-Installation Testing**: Run Tester.exe/Tester4.exe after Wine Bridge installation
2. **Cross-Platform Support**: Work with Steam Proton, Lutris, and Custom Prefixes
3. **User Validation**: Allow users to verify TrackIR device detection
4. **Interactive Experience**: Provide clear feedback and testing results

### User Experience Goals
- **One-Click Testing**: Easy access to run tester after installation
- **Clear Results**: Understandable feedback about TrackIR detection
- **Platform Agnostic**: Same experience across all gaming platforms
- **Error Handling**: Graceful handling of testing failures

---

## 🔧 Technical Requirements

### Core Functionality
1. **Tester Executable Detection**: Find Tester.exe/Tester64.exe in Wine prefix (already installed by NSIS)
2. **Wine Prefix Environment**: Set up proper environment for tester execution
3. **Process Management**: Launch and monitor tester process
4. **Result Interpretation**: Parse and display tester output
5. **Error Handling**: Handle missing files, execution failures, etc.

### Simplified Implementation Approach
Since the Tester executables are already installed to the Wine prefix during the NSIS installation process, our implementation is significantly simplified:

- **No System-Wide Search**: Testers are prefix-specific, not system-wide
- **Direct Prefix Access**: Use the same prefix path where Wine Bridge was installed
- **Consistent Location**: Testers are always in the same location within the prefix
- **Post-Installation Testing**: Can only run after successful Wine Bridge installation

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

### Phase 1: Core Tester Integration

#### 1.1 Tester Executable Management
**Files to Create/Modify:**
- `src/qt_gui/wine_tester.h` - Tester integration interface
- `src/qt_gui/wine_tester.cpp` - Core tester functionality
- `src/qt_gui/plugin_install.h` - Add tester methods
- `src/qt_gui/plugin_install.cpp` - Implement tester integration

**Implementation:**
```cpp
class WineTester {
public:
    // Tester detection (in Wine prefix)
    QString findTesterInPrefix(const QString& prefixPath);
    bool isTesterAvailableInPrefix(const QString& prefixPath);
    
    // Tester execution
    bool runTesterInPrefix(const QString& prefixPath, const QString& winePath);
    QString getTesterOutput();
    
    // Result interpretation
    bool parseTesterResults(const QString& output);
    QString getTesterStatus();
    
    // Error handling
    QString getLastError();
    QString getDebugInfo();
};
```

#### 1.2 Tester Executable Detection
**Implementation:**
- Tester.exe/Tester64.exe are already installed to the Wine prefix during NSIS installation
- Look for testers in the Wine prefix directory where Wine Bridge was installed
- Common locations within prefix:
  - `[prefix]/drive_c/Program Files/Linuxtrack/`
  - `[prefix]/drive_c/Program Files (x86)/Linuxtrack/`
- Verify executable permissions
- Handle both 32-bit (Tester.exe) and 64-bit (Tester64.exe) versions

#### 1.3 Basic Wine Environment Setup
**Implementation:**
- Set up basic Wine environment variables
- Configure Wine prefix path
- Handle Wine version selection
- Set up process environment

### Phase 2: Platform-Specific Integration

#### 2.1 Steam Proton Integration
**Files to Modify:**
- `src/qt_gui/steam_integration.h` - Add tester methods
- `src/qt_gui/steam_integration.cpp` - Implement Proton tester

**Implementation:**
```cpp
// Add to SteamIntegration class
bool runTesterInSteamProton(const QString& gameId);
QString getSteamProtonTesterResults(const QString& gameId);
```

#### 2.2 Lutris Integration
**Files to Modify:**
- `src/qt_gui/lutris_integration.h` - Add tester methods
- `src/qt_gui/lutris_integration.cpp` - Implement Lutris tester

**Implementation:**
```cpp
// Add to LutrisIntegration class
bool runTesterInLutrisPrefix(const QString& gameName);
QString getLutrisTesterResults(const QString& gameName);
```

#### 2.3 Custom Prefix Integration
**Files to Modify:**
- `src/qt_gui/plugin_install.h` - Add custom prefix tester
- `src/qt_gui/plugin_install.cpp` - Implement custom prefix tester

**Implementation:**
```cpp
// Add to PluginInstall class
bool runTesterInCustomPrefix(const QString& prefixPath);
QString getCustomPrefixTesterResults(const QString& prefixPath);
```

### Phase 3: UI Integration

#### 3.1 Post-Installation Testing
**Implementation:**
- Add "Test TrackIR" button after successful Wine Bridge installation
- Show testing progress and results
- Provide clear success/failure feedback
- Allow retesting if needed

#### 3.2 Testing Results Display
**Implementation:**
- Create results dialog showing:
  - TrackIR device detection status
  - Device information (if detected)
  - Error messages (if any)
  - Recommendations for troubleshooting

#### 3.3 Error Handling and User Guidance
**Implementation:**
- Clear error messages for common issues
- Troubleshooting suggestions
- Links to documentation
- Option to retry or skip testing

---

## 🎨 UI Design

### Post-Installation Flow
```
1. Wine Bridge Installation → 2. Success Message → 3. "Test TrackIR" Button → 4. Testing Results
```

### Testing Results Dialog
```
┌─────────────────────────────────────────────────────────┐
│                    TrackIR Test Results                │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Status: ✅ TrackIR Device Detected                    │
│                                                         │
│  Device Information:                                    │
│  - Device: TrackIR 5                                   │
│  - Firmware: 2.0.0.35                                 │
│  - Status: Connected                                   │
│                                                         │
│  Test Output:                                          │
│  [Detailed tester output here]                         │
│                                                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐   │
│  │    Retry    │  │   Close     │  │   Help      │   │
│  └─────────────┘  └─────────────┘  └─────────────┘   │
└─────────────────────────────────────────────────────────┘
```

---

## 🔍 Technical Challenges

### 1. Tester Executable Location
**Challenge**: Finding Tester.exe/Tester64.exe in Wine prefix after NSIS installation
**Solution**: 
- Tester executables are already installed to the Wine prefix during NSIS installation
- Look for Tester.exe/Tester64.exe in the Wine prefix directory
- Use the same prefix path where Wine Bridge was installed
- No need to search system directories - testers are prefix-specific

### 2. Wine Environment Setup
**Challenge**: Proper environment configuration for different platforms
**Solution**:
- Platform-specific environment setup
- Use existing environment logic from NSIS installer
- Handle Wine version differences
- Proper error handling for environment issues

### 3. Process Management
**Challenge**: Launching and monitoring tester process
**Solution**:
- Use QProcess for process management
- Set up proper environment variables
- Handle process timeouts
- Capture and parse output

### 4. Result Interpretation
**Challenge**: Parsing tester output for meaningful results
**Solution**:
- Define expected output patterns
- Parse device detection status
- Extract device information
- Handle different tester versions

---

## 🧪 Testing Strategy

### Unit Testing
- **Tester Detection**: Test executable finding logic
- **Environment Setup**: Test Wine environment configuration
- **Process Launch**: Test tester process execution
- **Result Parsing**: Test output interpretation

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
- [ ] **Tester Detection**: Finds Tester.exe/Tester64.exe in Wine prefix correctly
- [ ] **Process Launch**: Successfully launches tester in Wine prefix
- [ ] **Result Parsing**: Correctly interprets tester output
- [ ] **Cross-Platform**: Works with Steam Proton, Lutris, Custom Prefix
- [ ] **Error Handling**: Graceful handling of all error scenarios

### User Experience Requirements
- [ ] **Easy Access**: One-click testing after installation
- [ ] **Clear Results**: Understandable feedback about TrackIR status
- [ ] **Helpful Guidance**: Clear error messages and troubleshooting
- [ ] **Consistent Experience**: Same workflow across all platforms

### Technical Requirements
- [ ] **Robust Implementation**: Handles edge cases and errors
- [ ] **Performance**: Fast testing without blocking UI
- [ ] **Maintainability**: Clean, well-documented code
- [ ] **Extensibility**: Easy to add new platforms

---

## 🚀 Implementation Priority

### Phase 1: Foundation (Week 1)
1. **Create WineTester Class**: Basic structure and detection
2. **Tester Executable Detection**: Find and validate tester files
3. **Basic Wine Environment**: Simple Wine prefix testing
4. **Process Management**: Launch and monitor tester process

### Phase 2: Platform Integration (Week 2)
1. **Steam Proton Integration**: Test within Proton environment
2. **Lutris Integration**: Test within Lutris prefixes
3. **Custom Prefix Integration**: Test with user-selected prefixes
4. **Environment Configuration**: Platform-specific setup

### Phase 3: UI Integration (Week 3)
1. **Post-Installation Testing**: Add testing button after installation
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
1. **Research Tester Executable**: Locate and understand Tester.exe/Tester64.exe in Wine prefixes
2. **Study Wine Environment**: Understand Wine prefix environment setup
3. **Create WineTester Class**: Basic structure and detection methods
4. **Test Basic Functionality**: Verify tester can be launched in Wine prefix after NSIS installation

### Success Metrics
- [ ] Tester executable found and launched successfully
- [ ] TrackIR device detected in Wine prefix
- [ ] Results displayed clearly to user
- [ ] Works across all gaming platforms
- [ ] Error handling provides helpful feedback

---

*Last Updated: August 2, 2025*  
*Status: Planning Phase*  
*Next Review: After Phase 1 completion* 