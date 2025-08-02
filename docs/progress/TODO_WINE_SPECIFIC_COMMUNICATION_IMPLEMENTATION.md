# 🔧 Wine-Specific Communication Implementation TODO

**Branch**: `feature/wine-prefix-tester-integration`  
**Date**: December 2024  
**Status**: IN PROGRESS

---

## 📋 Overview

**PROBLEM**: The current Wine Bridge tries to dynamically load Linux libraries from within Wine, which doesn't work. The old version worked because it had a different approach.

**SOLUTION**: Implement a Wine-specific communication approach that doesn't rely on dynamic library loading.

---

## 🎯 Goals

### Primary Objectives
1. **Wine-Compatible IPC**: Direct socket communication from Wine Bridge to LinuxTrack master
2. **Automatic TrackIR Control**: TrackIR device starts/stops automatically with Wine Bridge apps
3. **No Dynamic Loading**: Avoid dynamic library loading that doesn't work in Wine
4. **Process Cleanup**: Proper cleanup when Wine Bridge applications exit

### User Experience Goals
- **Automatic Start**: TrackIR starts when Tester.exe or games launch
- **Automatic Stop**: TrackIR stops when Tester.exe or games close
- **UI Feedback**: LinuxTrack UI shows correct tracking state
- **Clean Exit**: No orphaned processes left running

---

## 🔧 Technical Approach

### Phase 1: Wine-Specific Socket Communication
**Files to Modify:**
- `src/wine_bridge/client/NPClient_main.c` - Add Wine-compatible socket communication
- `src/wine_bridge/ft_client/FreeTrackClient_main.c` - Add similar communication

**Implementation Strategy:**
1. **Direct Socket Implementation**: Use Wine's socket support directly
2. **Unix Domain Sockets**: Communicate with LinuxTrack master via `/tmp/ltr_m_sock`
3. **Command Protocol**: Send CMD_WAKEUP (3) and CMD_PAUSE (2) commands
4. **Error Handling**: Graceful fallback when socket communication fails

### Phase 2: Process Management
**Implementation:**
1. **DLL Cleanup**: Register cleanup handlers in DLL_PROCESS_DETACH
2. **Resource Management**: Proper socket cleanup and resource management
3. **Multiple Instances**: Handle multiple Wine Bridge instances properly

### Phase 3: Testing and Validation
**Testing:**
1. **Tester.exe Integration**: Test automatic start/stop with Tester.exe
2. **Game Integration**: Test with Steam Proton games
3. **Error Scenarios**: Test when LinuxTrack master is not running
4. **Performance**: Ensure no performance impact

---

## 📁 Implementation Plan

### Step 1: Research Wine Socket Support
- [x] Test Wine's Unix domain socket support
- [x] Verify socket path accessibility from Wine
- [x] Test basic socket communication from Wine to Linux

### Step 2: Implement Wine-Specific Socket Communication
- [x] Add socket communication functions to NPClient_main.c
- [x] Implement CMD_WAKEUP and CMD_PAUSE sending
- [x] Add error handling and fallback mechanisms
- [x] Test basic socket communication

### Step 3: Integrate with Wine Bridge Functions
- [x] Modify NPCLIENT_NP_StartDataTransmission to use socket communication
- [x] Modify NPCLIENT_NP_StopDataTransmission to use socket communication
- [x] Add cleanup handlers for proper process management
- [x] Test automatic start/stop functionality

### Step 4: FreeTrack Client Integration
- [ ] Add similar socket communication to FreeTrackClient_main.c
- [ ] Test FreeTrack functionality
- [ ] Ensure consistent behavior across both clients

### Step 5: Testing and Validation
- [ ] Test with Tester.exe
- [ ] Test with Steam Proton games
- [ ] Test error scenarios (LinuxTrack not running)
- [ ] Test multiple instances
- [ ] Validate UI integration

---

## 🧪 Testing Strategy

### Unit Testing
- [ ] **Socket Communication**: Test socket connection and message sending
- [ ] **Command Protocol**: Test CMD_WAKEUP and CMD_PAUSE commands
- [ ] **Error Handling**: Test socket failures and timeouts
- [ ] **Cleanup Logic**: Test process exit cleanup

### Integration Testing
- [ ] **Tester.exe Integration**: Test with real Tester.exe
- [ ] **Game Integration**: Test with Steam Proton games
- [ ] **LinuxTrack UI**: Verify UI state changes
- [ ] **Process Management**: Test cleanup and orphan prevention

### User Testing
- [ ] **End-to-End Flow**: Complete Wine Bridge → LinuxTrack workflow
- [ ] **Multiple Scenarios**: Test various game and application combinations
- [ ] **Error Recovery**: Test behavior when LinuxTrack is not running
- [ ] **Performance**: Test impact on system performance

---

## 📊 Success Criteria

### Functional Requirements
- [ ] **Automatic Start**: TrackIR starts when Wine Bridge apps launch
- [ ] **Automatic Stop**: TrackIR stops when Wine Bridge apps close
- [ ] **Socket Communication**: Wine Bridge communicates with LinuxTrack master
- [ ] **Process Cleanup**: No orphaned processes left running
- [ ] **UI Integration**: LinuxTrack UI reflects correct state

### User Experience Requirements
- [ ] **Seamless Operation**: No manual intervention required
- [ ] **Reliable Behavior**: Consistent start/stop behavior
- [ ] **Error Recovery**: Graceful handling of failures
- [ ] **Performance**: No significant performance impact

### Technical Requirements
- [ ] **Wine Compatibility**: Works properly in Wine/Proton environment
- [ ] **Socket Protocol**: Proper implementation of socket communication
- [ ] **Error Handling**: Robust error handling and recovery
- [ ] **Process Management**: Proper cleanup and resource management
- [ ] **Cross-Platform**: Works with Steam Proton, Lutris, Custom Prefix

---

## 🚀 Current Progress

### Completed
- [x] **Investigation**: Understood why old version worked vs current version
- [x] **Documentation**: Created comprehensive TODO for tracking
- [x] **Clean State**: Committed working improvements, discarded failed attempts

### In Progress
- [ ] **Step 1**: Research Wine Socket Support
- [ ] **Step 2**: Implement Wine-Specific Socket Communication
- [ ] **Step 3**: Integrate with Wine Bridge Functions
- [ ] **Step 4**: FreeTrack Client Integration
- [ ] **Step 5**: Testing and Validation

### Next Actions
1. **Research Wine's Unix domain socket capabilities**
2. **Test basic socket communication from Wine to Linux**
3. **Implement Wine-specific socket communication functions**
4. **Integrate with NPClient functions**

---

*Last Updated: December 2024*  
*Status: In Progress - Phase 1* 