# AI Prompt: LinuxTrack Wine Bridge TIRViews.dll Loading Issue

## Context
We are debugging a LinuxTrack Wine bridge issue where TIRViews.dll fails to load when starting fresh, but works when the LinuxTrack daemon is running first.

## Problem Summary
- **Issue**: TIRViews.dll cannot be loaded when starting Wine bridge fresh
- **Workaround**: Works when LinuxTrack daemon is started first
- **Error**: "Can't load TIRViews.dll - please reinstall TrackIR firmware to get it"
- **MFC140 Modernization**: Successfully implemented and working

## Key Findings from Investigation

### 1. Registry Setup is Correct
- NaturalPoint registry entries exist: `HKCU "Software\NaturalPoint\NATURALPOINT\NPClient Location" "Path" "C:\Program Files (x86)\Linuxtrack\"`
- NPClient.dll exists and is found successfully
- Registry setup is working properly

### 2. Wine Debug Analysis
- **NPClient.dll loads successfully**: `Loaded L"C:\\Program Files (x86)\\Linuxtrack\\NPClient.dll" at 7DED0000`
- **TrackIR.exe loads but immediately unloads**: 
  ```
  01f0:trace:module:LdrUnloadDll (L"TrackIR.exe") - START
  01f0:trace:module:LdrUnloadDll END
  ```
- **TIRViews.dll is NEVER loaded**: No LoadLibrary attempts for TIRViews.dll found in debug output
- **No error messages**: No explicit error messages in Wine debug output

### 3. Code Analysis
- TIRViews.dll loading happens in `WM_CREATE` message handler in `src/wine_bridge/views/main.cpp` (lines 75-100)
- Loading sequence: MFC140/MFC42 → TIRViews.dll → NPClient.dll
- TrackIR.exe is the component that loads TIRViews.dll, not Tester.exe

### 4. Architecture Understanding
- **Tester.exe**: Test application that launches TrackIR.exe
- **TrackIR.exe**: Views component that loads TIRViews.dll and creates the window
- **NPClient.dll**: TrackIR API compatibility layer
- **TIRViews.dll**: Core TrackIR functionality (depends on MFC140/MFC42)

## Current Hypothesis
The issue is that **TrackIR.exe is failing to create its window or process the WM_CREATE message** when starting fresh. When the LinuxTrack daemon is running first, it provides some initialization context that allows TrackIR.exe to start properly.

## Files to Investigate
- `src/wine_bridge/views/main.cpp` - Main TrackIR.exe implementation
- `src/wine_bridge/views/rest.c` - Registry and file path functions
- `src/wine_bridge/views/rest.h` - Function declarations

## Next Steps to Investigate

### 1. TrackIR.exe Direct Execution
- Run TrackIR.exe directly with Wine debug to see if it fails independently
- Check if the issue is specific to Tester.exe launching TrackIR.exe

### 2. Window Creation Debugging
- Add debug output to TrackIR.exe to see if WM_CREATE is being processed
- Check if window creation is failing before reaching TIRViews.dll loading

### 3. LinuxTrack Daemon Integration
- Investigate what the LinuxTrack daemon provides that allows TrackIR.exe to start
- Check for shared memory, IPC, or other initialization requirements

### 4. Dependency Chain Analysis
- Verify all dependencies are available when starting fresh
- Check if any missing dependencies are provided by the running daemon

### 5. Wine Bridge Initialization
- Investigate the Wine bridge startup sequence
- Check if there are any initialization steps missing when starting fresh

## Technical Environment
- **Wine Version**: wine-staging 10.12
- **LinuxTrack**: Built from source with MFC140 modernization
- **Wine Prefix**: `~/.wine`
- **Installation**: Wine bridge installed via NSIS installer

## Questions to Answer
1. Why does TrackIR.exe unload immediately when starting fresh?
2. What does the LinuxTrack daemon provide that allows TrackIR.exe to start?
3. Is there a missing initialization step in the Wine bridge startup?
4. Are there any IPC or shared memory requirements not being met?
5. Is there a timing issue with component initialization?

## Success Criteria
- TrackIR.exe starts successfully when launched fresh
- TIRViews.dll loads properly without requiring daemon to be running first
- Wine bridge provides full TrackIR functionality without workarounds

---

**Please continue the investigation focusing on why TrackIR.exe fails to start properly when the LinuxTrack daemon is not running first.** 