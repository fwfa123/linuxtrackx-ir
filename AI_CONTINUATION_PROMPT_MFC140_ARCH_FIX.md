# AI Continuation Prompt: MFC140 Extractor Arch Linux Fix

## Issue Summary
The MFC140 extractor was failing on Arch Linux with the error:
```
Failed to initialize Wine prefix (exit code: 1)
```

Root cause: The extractor was trying to create a 32-bit Wine prefix (`WINEARCH=win32`) on Arch Linux, but the Wine installation is 64-bit only and doesn't support 32-bit prefixes.

## Changes Made

### 1. Modified MFC140 Extractor to Use 64-bit Prefix
**File:** `src/qt_gui/extractor.cpp`

**Changes:**
- Removed `env.insert(QString::fromUtf8("WINEARCH"), QString::fromUtf8("win32"));` line
- Updated progress messages from "32-bit" to "64-bit"
- Added proper error output capture for debugging

**Specific sections modified:**
- `tryWinetricksInstall()` function around line 1315-1350
- Wine initialization section around line 1340-1370
- Winetricks environment setup around line 1370-1390

### 2. Added Error Output Capture
Added the same error logging pattern used in the MFC42 extractor:
```cpp
// Capture error output for debugging
QString errorOutput = QString::fromUtf8(wineInit.readAllStandardError());
QString standardOutput = QString::fromUtf8(wineInit.readAllStandardOutput());

if(!errorOutput.isEmpty()) {
  progress(QString::fromUtf8("Wine initialization stderr: %1").arg(errorOutput));
}
if(!standardOutput.isEmpty()) {
  progress(QString::fromUtf8("Wine initialization stdout: %1").arg(standardOutput));
}
```

### 3. Updated Progress Messages
Changed all references from "32-bit Wine prefix" to "64-bit Wine prefix" for clarity.

## Technical Details

### Why This Fix Works
- MFC140 libraries are available for 64-bit Windows applications
- No longer need the 32-bit restriction that MFC42 required
- Arch Linux Wine installation is 64-bit only, so this aligns with the system capabilities

### Error That Was Fixed
```
wine: WINEARCH is set to 'win32' but this is not supported in wow64 mode.
```

## Testing Instructions

1. Build the project on a system with all dependencies
2. Run the MFC140 extractor on Arch Linux
3. Verify that Wine prefix initialization succeeds
4. Check that winetricks can install MFC140 libraries
5. Confirm mfc140u.dll is properly extracted and copied

## Files Modified
- `src/qt_gui/extractor.cpp` - Main changes to MFC140 extractor logic

## Next Steps
1. Build and test on base OS with dependencies
2. Verify the fix resolves the Arch Linux Wine prefix initialization issue
3. Test the complete MFC140 installation flow
4. Update documentation if needed

## Context
This fix addresses the specific issue where the extractor was failing on Arch Linux due to attempting to create 32-bit Wine prefixes when only 64-bit Wine is available. The MFC140 approach was chosen specifically to avoid 32-bit requirements, so this fix aligns with that design decision. 