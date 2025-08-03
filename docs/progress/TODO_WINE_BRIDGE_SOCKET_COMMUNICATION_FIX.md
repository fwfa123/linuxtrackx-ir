# 🔧 Wine Bridge Socket Communication Fix TODO

**Branch**: `feature/wine-prefix-tester-integration`  
**Date**: December 2024  
**Status**: CRITICAL ISSUE

---

## 📋 Overview

**CRITICAL ISSUE**: The Wine Bridge is using the wrong socket path and not properly communicating with the LinuxTrack master. This causes:

1. **TrackIR device doesn't start automatically** when Tester.exe or games launch
2. **TrackIR device doesn't stop automatically** when Tester.exe or games close
3. **Orphaned processes** are left running when Wine Bridge applications exit
4. **No communication** between Wine Bridge and LinuxTrack UI

## 🎯 Goals

### Primary Objectives
1. **Fix Socket Communication**: Wine Bridge should communicate with LinuxTrack master via socket
2. **Automatic TrackIR Control**: TrackIR device should start/stop automatically with Wine Bridge apps
3. **Process Cleanup**: Proper cleanup when Wine Bridge applications exit
4. **UI Integration**: LinuxTrack UI should reflect the state changes

### User Experience Goals
- **Automatic Start**: TrackIR starts when Tester.exe or games launch
- **Automatic Stop**: TrackIR stops when Tester.exe or games close
- **UI Feedback**: LinuxTrack UI shows correct tracking state
- **Clean Exit**: No orphaned processes left running

---

## 🔧 Technical Requirements

### Core Functionality
1. **Socket Communication**: Wine Bridge should use socket to communicate with master
2. **Command Protocol**: Send CMD_WAKEUP and CMD_PAUSE commands to master
3. **Process Management**: Proper cleanup on Wine Bridge exit
4. **Error Handling**: Graceful handling of socket communication failures

### Current Problem
```cpp
// CURRENT (BROKEN) - Wrong socket path
// Wine Bridge is looking for /tmp/linuxtrack.sock but master uses /tmp/ltr_m_sock
snprintf(check_socket_cmd, sizeof(check_socket_cmd), 
         "test -S /tmp/linuxtrack.sock > /dev/null 2>&1");  // ❌ Wrong path

// The functions do call the slave process, but socket communication fails
int __stdcall NPCLIENT_NP_StartDataTransmission(void)
{
  dbg_report("StartDataTransmission request\n");
  linuxtrack_wakeup();  // ❌ Socket communication fails due to wrong path
  return 0;
}

int __stdcall NPCLIENT_NP_StopDataTransmission(void)
{
  dbg_report("StopDataTransmission request\n");
  linuxtrack_suspend();  // ❌ Socket communication fails due to wrong path
  return 0;
}
```

### Required Solution
```cpp
// REQUIRED (FIXED) - Correct socket path
// Fix the socket path to use the correct master socket
snprintf(check_socket_cmd, sizeof(check_socket_cmd), 
         "test -S /tmp/ltr_m_sock > /dev/null 2>&1");  // ✅ Correct path

// The functions already call the slave process correctly
int __stdcall NPCLIENT_NP_StartDataTransmission(void)
{
  dbg_report("StartDataTransmission request\n");
  linuxtrack_wakeup();  // ✅ Will work once socket path is fixed
  return 0;
}

int __stdcall NPCLIENT_NP_StopDataTransmission(void)
{
  dbg_report("StopDataTransmission request\n");
  linuxtrack_suspend();  // ✅ Will work once socket path is fixed
  return 0;
}
```

---

## 📁 Implementation Plan

### Phase 1: Socket Communication Implementation

#### 1.1 Wine Bridge Socket Integration
**Files to Modify:**
- `src/wine_bridge/client/NPClient_main.c` - Add socket communication
- `src/wine_bridge/client/NPClient_dll.h` - Add socket function declarations

**Implementation:**
```cpp
// Add to NPClient_main.c
#include "../ltr_srv_comm.h"
#include "../ipc_utils.h"

static int send_master_command(uint32_t cmd)
{
  int socket = ltr_int_connect_to_socket(ltr_int_master_socket_name());
  if (socket > 0) {
    int result = ltr_int_send_message(socket, cmd, 0);
    close(socket);
    return result;
  }
  return -1;
}

int __stdcall NPCLIENT_NP_StartDataTransmission(void)
{
  dbg_report("StartDataTransmission request\n");
  send_master_command(CMD_WAKEUP);
  return 0;
}

int __stdcall NPCLIENT_NP_StopDataTransmission(void)
{
  dbg_report("StopDataTransmission request\n");
  send_master_command(CMD_PAUSE);
  return 0;
}
```

#### 1.2 FreeTrack Client Integration
**Files to Modify:**
- `src/wine_bridge/ft_client/FreeTrackClient_main.c` - Add socket communication

**Implementation:**
```cpp
// Add similar socket communication for FreeTrack
static int send_master_command(uint32_t cmd)
{
  int socket = ltr_int_connect_to_socket(ltr_int_master_socket_name());
  if (socket > 0) {
    int result = ltr_int_send_message(socket, cmd, 0);
    close(socket);
    return result;
  }
  return -1;
}
```

### Phase 2: Process Management

#### 2.1 Cleanup on Exit
**Implementation:**
```cpp
// Add cleanup function
static void cleanup_on_exit(void)
{
  send_master_command(CMD_PAUSE);
}

// Register cleanup function
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      // Register cleanup
      break;
    case DLL_PROCESS_DETACH:
      cleanup_on_exit();
      break;
  }
  return TRUE;
}
```

#### 2.2 Error Handling
**Implementation:**
```cpp
static int send_master_command(uint32_t cmd)
{
  int socket = ltr_int_connect_to_socket(ltr_int_master_socket_name());
  if (socket > 0) {
    int result = ltr_int_send_message(socket, cmd, 0);
    close(socket);
    if (result == 0) {
      dbg_report("Successfully sent command %d to master\n", cmd);
    } else {
      dbg_report("Failed to send command %d to master\n", cmd);
    }
    return result;
  } else {
    dbg_report("Failed to connect to master socket\n");
    return -1;
  }
}
```

### Phase 3: Testing and Validation

#### 3.1 Test Scenarios
- **Tester.exe Launch**: TrackIR should start automatically
- **Tester.exe Close**: TrackIR should stop automatically
- **Game Launch**: TrackIR should start automatically
- **Game Close**: TrackIR should stop automatically
- **LinuxTrack UI**: Should show correct tracking state
- **Process Cleanup**: No orphaned processes

#### 3.2 Error Scenarios
- **LinuxTrack not running**: Graceful fallback
- **Socket connection failed**: Error handling
- **Master not responding**: Timeout handling
- **Multiple Wine Bridge instances**: Proper coordination

---

## 🔍 Technical Challenges

### 1. Cross-Environment Communication
**Challenge**: Wine Bridge runs in Wine/Proton environment, LinuxTrack runs on Linux host
**Solution**: 
- Use Unix domain sockets that work across Wine/Linux boundary
- Ensure socket path is accessible from Wine environment
- Handle Wine-specific socket path mapping

### 2. Process Lifecycle Management
**Challenge**: Wine Bridge processes may exit unexpectedly
**Solution**:
- Register cleanup handlers in DLL_PROCESS_DETACH
- Use atexit() for additional cleanup
- Monitor process state and cleanup on exit

### 3. Socket Connection Reliability
**Challenge**: Socket connection may fail or timeout
**Solution**:
- Implement retry logic for socket connections
- Add timeout handling for socket operations
- Graceful fallback when master is not available

### 4. Multiple Instance Coordination
**Challenge**: Multiple Wine Bridge instances may run simultaneously
**Solution**:
- Each instance should send its own commands
- Master should handle multiple clients properly
- Proper cleanup when instances exit

---

## 🧪 Testing Strategy

### Unit Testing
- **Socket Communication**: Test socket connection and message sending
- **Command Protocol**: Test CMD_WAKEUP and CMD_PAUSE commands
- **Error Handling**: Test socket failures and timeouts
- **Cleanup Logic**: Test process exit cleanup

### Integration Testing
- **Tester.exe Integration**: Test with real Tester.exe
- **Game Integration**: Test with Steam Proton games
- **LinuxTrack UI**: Verify UI state changes
- **Process Management**: Test cleanup and orphan prevention

### User Testing
- **End-to-End Flow**: Complete Wine Bridge → LinuxTrack workflow
- **Multiple Scenarios**: Test various game and application combinations
- **Error Recovery**: Test behavior when LinuxTrack is not running
- **Performance**: Test impact on system performance

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
- [ ] **Socket Protocol**: Proper implementation of socket communication
- [ ] **Error Handling**: Robust error handling and recovery
- [ ] **Process Management**: Proper cleanup and resource management
- [ ] **Cross-Platform**: Works with Steam Proton, Lutris, Custom Prefix

---

## 🚀 Implementation Priority

### Phase 1: Core Socket Communication (Week 1)
1. **Socket Integration**: Add socket communication to Wine Bridge
2. **Command Protocol**: Implement CMD_WAKEUP and CMD_PAUSE
3. **Error Handling**: Add robust error handling
4. **Basic Testing**: Test socket communication

### Phase 2: Process Management (Week 2)
1. **Cleanup Logic**: Implement proper process cleanup
2. **Lifecycle Management**: Handle DLL_PROCESS_DETACH
3. **Multiple Instances**: Handle multiple Wine Bridge instances
4. **Integration Testing**: Test with real applications

### Phase 3: Testing and Polish (Week 3)
1. **Comprehensive Testing**: Test all scenarios and edge cases
2. **Performance Optimization**: Optimize socket communication
3. **Documentation**: Update documentation with socket communication
4. **User Testing**: Real-world testing and validation

### Phase 4: Deployment and Monitoring (Week 4)
1. **Deployment**: Deploy fixes to production
2. **Monitoring**: Monitor for any remaining issues
3. **User Feedback**: Collect and address user feedback
4. **Documentation**: Final documentation updates

---

## 📋 Next Steps

### Immediate Actions
1. **Research Socket Communication**: Understand current socket protocol
2. **Modify Wine Bridge**: Add socket communication to NPClient
3. **Test Basic Communication**: Verify socket connection works
4. **Implement Cleanup**: Add proper process cleanup

### Success Metrics
- [ ] TrackIR starts automatically when Tester.exe launches
- [ ] TrackIR stops automatically when Tester.exe closes
- [ ] LinuxTrack UI shows correct tracking state
- [ ] No orphaned processes left running
- [ ] Works with Steam Proton, Lutris, and Custom Prefix

---

*Last Updated: December 2024*  
*Status: Critical Issue - High Priority* 