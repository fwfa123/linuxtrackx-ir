# FT Tester Selection Fix - Technical Plan

## Problem Description

In the Gaming tab's testing section, there are two radio buttons for tester selection:
- Tester.exe (TrackIR)
- FT_Tester (FreeTrack)

The radio button selection logic works correctly - when FT_Tester is selected, `currentTesterType` is properly set to "FT_Tester". However, when the tester is launched, it always runs one of the Tester.exe files instead of the FreeTrackTester.exe.

## Root Cause Analysis

The issue is in the `selectAppropriateTester()` method in `testing_section.cpp` (line 841). This method is called from `runSelectedTester()` to determine which tester executable to launch.

**Current Logic Flow:**
1. User selects FT_Tester radio button
2. `onTesterSelectionChanged()` sets `currentTesterType = "FT_Tester"`
3. `runSelectedTester()` calls `selectAppropriateTester(prefixPath, prefixArch, currentTesterType)`
4. `selectAppropriateTester()` calls `TesterLauncher::findAppropriateTester(prefixPath, arch)` - **IGNORING the testerType parameter**
5. `TesterLauncher::findAppropriateTester()` only looks for "Tester.exe" and "Tester64.exe" variants

**Expected Logic Flow:**
1. User selects FT_Tester radio button
2. `currentTesterType` is set to "FT_Tester"
3. `selectAppropriateTester()` should call `TesterLauncher::findTesterInPrefix(prefixPath, currentTesterType)`
4. `TesterLauncher::findTesterInPrefix()` correctly handles FT_Tester by looking for "FT_Tester.exe" or "FreeTrackTester.exe"

## Files That Need Changes

### Primary File: `src/qt_gui/testing_section.cpp`

**Method to modify:** `selectAppropriateTester()` (starting at line 841)

**Current Implementation:**
```cpp
QString TestingSection::selectAppropriateTester(const QString &prefixPath, WineArchitecture arch, const QString &preferredTester)
{
    // First, try to find a tester based on the detected architecture
    QString appropriateTester = TesterLauncher::findAppropriateTester(prefixPath, arch);
    
    if (!appropriateTester.isEmpty()) {
        return appropriateTester;
    }
    
    // Fallback logic...
}
```

**Proposed Implementation:**
```cpp
QString TestingSection::selectAppropriateTester(const QString &prefixPath, WineArchitecture arch, const QString &preferredTester)
{
    // Handle FT_Tester specifically by looking for FreeTrack tester files
    if (preferredTester == QString::fromUtf8("FT_Tester")) {
        QString ftTester = TesterLauncher::findTesterInPrefix(prefixPath, preferredTester);
        if (!ftTester.isEmpty()) {
            return ftTester;
        }
    }
    
    // For Tester.exe, use architecture-based selection
    if (preferredTester == QString::fromUtf8("Tester.exe")) {
        QString appropriateTester = TesterLauncher::findAppropriateTester(prefixPath, arch);
        if (!appropriateTester.isEmpty()) {
            return appropriateTester;
        }
    }
    
    // Fallback logic for any other cases...
}
```

## Algorithm Explanation

### Tester Selection Logic

The fix implements a two-tier selection process:

1. **Specific Tester Type Selection:**
   - If `preferredTester` is "FT_Tester", call `TesterLauncher::findTesterInPrefix()` with "FT_Tester"
   - This method correctly searches for "FT_Tester.exe" or "FreeTrackTester.exe"

2. **Architecture-Based Selection:**
   - If `preferredTester` is "Tester.exe", use the existing architecture-based selection
   - `TesterLauncher::findAppropriateTester()` prioritizes based on Wine prefix architecture:
     - 64-bit prefixes: prefers "Tester64.exe", falls back to "Tester.exe"
     - 32-bit prefixes: prefers "Tester.exe", falls back to "Tester64.exe"

3. **Fallback Logic:**
   - If no specific tester is found, search all standard locations for any available tester

### Search Locations

Both methods search in the following Wine prefix locations:
- Root of prefix
- `drive_c/windows/`
- `drive_c/Program Files/`
- `drive_c/Program Files/Linuxtrack/`
- `drive_c/Program Files (x86)/`
- `drive_c/Program Files (x86)/Linuxtrack/`

## Expected Behavior After Fix

- **FT_Tester selected:** Will launch FreeTrackTester.exe or FT_Tester.exe
- **Tester.exe selected:** Will launch Tester.exe or Tester64.exe based on architecture
- **No tester found:** Will show "Tester Not Found" dialog with Wine Bridge installation option

## Testing Requirements

1. **Test FT_Tester Selection:**
   - Select FT_Tester radio button
   - Choose a game/platform
   - Click "Run Tester"
   - Verify FreeTrackTester.exe launches (not Tester.exe)

2. **Test Tester.exe Selection:**
   - Select Tester.exe radio button
   - Choose a game/platform
   - Click "Run Tester"
   - Verify Tester.exe or Tester64.exe launches based on architecture

3. **Test Architecture Detection:**
   - Test with both 32-bit and 64-bit Wine prefixes
   - Verify appropriate tester is selected for each architecture

## No Additional Files Required

This fix only requires changes to the existing `testing_section.cpp` file. All necessary helper methods already exist in `TesterLauncher` class.
