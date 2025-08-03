# 🎯 Tester Game ID Auto-Fill Enhancement TODO

**Branch**: `feature/wine-prefix-tester-integration`  
**Date**: December 2024  
**Status**: PLANNING

---

## 📋 Overview

This enhancement will automatically pre-fill the Game ID in Tester.exe/Tester64.exe based on the Steam/Lutris game information available during NSIS installation. Currently, Tester.exe hardcodes Game ID `2307` (Microsoft Flight), but we can intelligently detect and set the appropriate Game ID based on the game being installed to.

## 🎯 Goals

### Primary Objectives
1. **Automatic Game ID Detection**: Detect the correct TrackIR Game ID based on Steam/Lutris game
2. **Smart Pre-filling**: Pre-fill the Game ID field in Tester.exe with the correct value
3. **Fallback Handling**: Graceful fallback to default Game ID if detection fails
4. **Cross-Platform Support**: Work with Steam Proton, Lutris, and Custom Prefixes

### User Experience Goals
- **Zero-Configuration Testing**: Correct Game ID automatically detected
- **Accurate Testing**: Test with the right Game ID for the specific game
- **Seamless Workflow**: No manual Game ID entry required
- **Backward Compatibility**: Works even if detection fails

---

## 🔧 Technical Requirements

### Core Functionality
1. **Steam Game ID to TrackIR Game ID Mapping**: Create mapping system
2. **Command Line Parameter Support**: Pass Game ID to Tester.exe
3. **NSIS Integration**: Modify installer to pass Game ID parameter
4. **Tester.exe Enhancement**: Parse command line arguments

### Implementation Approach

#### Option A: Command Line Parameters (Recommended)
```nsis
; In NSIS installer
Exec '"$INSTDIR\Tester.exe" /gameid:2307'
```

```cpp
// In Tester.exe main.cpp
int gameId = 2307; // default
// Parse command line for /gameid: parameter
SetDlgItemInt(hwndDlg, IDC_APPID, gameId, true);
```

#### Option B: Registry/Environment Variable
```nsis
; In NSIS installer
WriteRegStr HKLM SOFTWARE\Linuxtrack "LastGameID" "2307"
```

```cpp
// In Tester.exe main.cpp
int gameId = ReadRegistryGameID(); // Read from registry
SetDlgItemInt(hwndDlg, IDC_APPID, gameId, true);
```

#### Option C: Game Detection Logic
```cpp
// In Tester.exe main.cpp
int gameId = DetectGameFromPrefix(); // Analyze Wine prefix
if (gameId == 0) gameId = 2307; // fallback
SetDlgItemInt(hwndDlg, IDC_APPID, gameId, true);
```

---

## 📁 Implementation Plan

### Phase 1: Steam Game ID to TrackIR Game ID Mapping

#### 1.1 Create Mapping System
**Files to Create/Modify:**
- `src/wine_bridge/tester/game_mapping.h` - Game ID mapping definitions
- `src/wine_bridge/tester/game_mapping.c` - Mapping implementation

**Implementation:**
```cpp
// Map Steam Game ID to TrackIR Game ID
int steamToTrackIRGameID(int steamGameId) {
    switch(steamGameId) {
        case 1234567: return 2307; // Microsoft Flight
        case 2345678: return 2305; // MS Flight Simulator X
        case 3456789: return 8150; // MS Flight Simulator 2020
        case 4567890: return 2303; // MS Flight Simulator 2004
        case 5678901: return 2302; // MS Flight Simulator 2002
        default: return 2307; // fallback to Microsoft Flight
    }
}
```

#### 1.2 Available TrackIR Game IDs
From `gamedata.txt`:
- `2307` - "Microsoft Flight" (AFBC54959539C515)
- `2302` - "MS Flight Simulator 2002"
- `2303` - "MS Flight Simulator 2004"
- `2305` - "MS Flight Simulator X"
- `8150` - "MS Flight Simulator 2020"
- `1001` - "IL-2 Forgotten Battles, ACE, Pacific Fighters"
- `1002` - "Assetto Corsa"
- And many others...

### Phase 2: NSIS Installer Enhancement

#### 2.1 Command Line Parameter Support
**Files to Modify:**
- `src/wine_bridge/ltr_wine.nsi` - Add Game ID parameter passing
- `src/wine_bridge/ltr_wine64.nsi` - Add Game ID parameter passing

**Implementation:**
```nsis
; Get Game ID from environment or parameter
Var GameID
Function .onInit
    ; Try to get Game ID from environment
    ${GetParameters} $R0
    ${GetOptions} $R0 "/gameid=" $GameID
    ${If} $GameID == ""
        ; Fallback to default
        StrCpy $GameID "2307"
    ${EndIf}
FunctionEnd

; In installation section
Exec '"$INSTDIR\Tester.exe" /gameid:$GameID'
```

#### 2.2 Steam Integration Enhancement
**Files to Modify:**
- `src/qt_gui/steam_integration.cpp` - Pass Game ID to NSIS installer

**Implementation:**
```cpp
// In SteamIntegration::runWineBridgeInstallerWithProton
QStringList arguments;
arguments << QStringLiteral("run") << installerPath;
arguments << QStringLiteral("/gameid:") + getTrackIRGameID(steamGameId);
```

### Phase 3: Tester.exe Enhancement

#### 3.1 Command Line Argument Parsing
**Files to Modify:**
- `src/wine_bridge/tester/main.cpp` - Add argument parsing

**Implementation:**
```cpp
int parseGameIDFromArgs(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "/gameid:", 8) == 0) {
            return atoi(argv[i] + 8);
        }
    }
    return 2307; // default
}

// In WinMain
int gameId = parseGameIDFromArgs(__argc, __argv);

// In DialogProc WM_INITDIALOG
SetDlgItemInt(hwndDlg, IDC_APPID, gameId, true);
```

#### 3.2 Registry Fallback
**Implementation:**
```cpp
int getGameIDFromRegistry() {
    HKEY hkey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
        "SOFTWARE\\Linuxtrack", 0, KEY_READ, &hkey) == ERROR_SUCCESS) {
        DWORD gameId = 2307;
        DWORD size = sizeof(DWORD);
        if (RegQueryValueEx(hkey, "LastGameID", NULL, NULL, 
            (LPBYTE)&gameId, &size) == ERROR_SUCCESS) {
            RegCloseKey(hkey);
            return gameId;
        }
        RegCloseKey(hkey);
    }
    return 2307; // default
}
```

---

## 🎨 UI Design

### Current Tester.exe UI
- **Game ID Field**: Currently hardcoded to 2307
- **User Interaction**: User can manually change the Game ID
- **Testing Interface**: Shows real-time TrackIR data

### Enhanced Tester.exe UI
- **Game ID Field**: Pre-filled with detected Game ID
- **User Interaction**: User can still manually change if needed
- **Visual Feedback**: Show detected game name if available
- **Fallback Indication**: Show when using default Game ID

---

## 🔍 Technical Challenges

### 1. Steam Game ID to TrackIR Game ID Mapping
**Challenge**: Creating accurate mapping between Steam Game IDs and TrackIR Game IDs
**Solution**: 
- Build comprehensive mapping database
- Use game name matching as fallback
- Allow user override for unknown games

### 2. Command Line Parameter Parsing
**Challenge**: Adding command line argument support to Windows dialog application
**Solution**:
- Use `__argc` and `__argv` in WinMain
- Parse arguments before dialog creation
- Handle invalid parameters gracefully

### 3. Cross-Platform Compatibility
**Challenge**: Ensuring compatibility with Steam Proton, Lutris, and Custom Prefixes
**Solution**:
- Use environment variables for game detection
- Implement platform-specific detection logic
- Maintain consistent fallback behavior

### 4. Backward Compatibility
**Challenge**: Ensuring existing functionality continues to work
**Solution**:
- Default to current behavior (Game ID 2307)
- Graceful degradation if detection fails
- No breaking changes to existing workflow

---

## 🧪 Testing Strategy

### Unit Testing
- **Game ID Mapping**: Test mapping accuracy for known games
- **Command Line Parsing**: Test argument parsing with various inputs
- **Fallback Logic**: Test behavior with invalid/missing parameters
- **Registry Integration**: Test registry reading/writing

### Integration Testing
- **Steam Proton**: Test with real Steam games
- **Lutris**: Test with Lutris game prefixes
- **Custom Prefix**: Test with manually selected prefixes
- **Error Scenarios**: Test with missing files, wrong permissions

### User Testing
- **End-to-End Flow**: Complete installation → testing workflow
- **Game Detection**: Test with various Steam games
- **UI Experience**: Test user interface and feedback
- **Fallback Behavior**: Test when detection fails

---

## 📊 Success Criteria

### Functional Requirements
- [ ] **Game ID Detection**: Automatically detects correct Game ID for Steam/Lutris games
- [ ] **Pre-filling**: Tester.exe Game ID field pre-filled with detected value
- [ ] **Fallback Handling**: Graceful fallback to default Game ID (2307)
- [ ] **User Override**: User can still manually change Game ID if needed
- [ ] **Cross-Platform**: Works with Steam Proton, Lutris, Custom Prefix

### User Experience Requirements
- [ ] **Zero-Configuration**: Correct Game ID automatically detected
- [ ] **Accurate Testing**: Test with appropriate Game ID for the game
- [ ] **Seamless Workflow**: No manual Game ID entry required
- [ ] **Visual Feedback**: Clear indication of detected game
- [ ] **Error Handling**: Helpful messages when detection fails

### Technical Requirements
- [ ] **Backward Compatibility**: Existing functionality continues to work
- [ ] **Performance**: Fast detection without blocking UI
- [ ] **Maintainability**: Clean, well-documented code
- [ ] **Extensibility**: Easy to add new game mappings

---

## 🚀 Implementation Priority

### Phase 1: Core Infrastructure (Week 1)
1. **Game ID Mapping System**: Create mapping database and functions
2. **Command Line Support**: Add argument parsing to Tester.exe
3. **NSIS Integration**: Modify installer to pass Game ID parameter
4. **Basic Testing**: Test with known Steam games

### Phase 2: Steam Integration (Week 2)
1. **Steam Game Detection**: Enhance Steam integration to detect Game ID
2. **Mapping Database**: Build comprehensive Steam to TrackIR mapping
3. **UI Enhancement**: Add visual feedback for detected games
4. **Error Handling**: Implement robust error handling

### Phase 3: Cross-Platform Support (Week 3)
1. **Lutris Integration**: Add Lutris game detection
2. **Custom Prefix Support**: Handle manually selected prefixes
3. **Registry Fallback**: Implement registry-based fallback
4. **Comprehensive Testing**: Test all platforms and scenarios

### Phase 4: Polish and Documentation (Week 4)
1. **UI Polish**: Final UI improvements and feedback
2. **Documentation**: Update user and developer documentation
3. **Code Review**: Final review and cleanup
4. **User Testing**: Real-world testing and feedback

---

## 📋 Next Steps

### Immediate Actions
1. **Research Steam Game Database**: Build comprehensive Steam Game ID database
2. **Study TrackIR Game IDs**: Analyze all available TrackIR Game IDs in gamedata.txt
3. **Design Mapping System**: Create mapping between Steam and TrackIR Game IDs
4. **Prototype Command Line Parsing**: Test command line argument parsing in Tester.exe

### Success Metrics
- [ ] Tester.exe automatically detects correct Game ID for Steam games
- [ ] Game ID field pre-filled with detected value
- [ ] User can still manually override Game ID if needed
- [ ] Works across all gaming platforms (Steam Proton, Lutris, Custom Prefix)
- [ ] Graceful fallback when detection fails

---

*Last Updated: December 2024*  
*Status: Planning Phase* 