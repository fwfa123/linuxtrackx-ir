# 0009 - Lutris Detection Fix Plan

## Technical Plan: Fix Lutris Detection for Fedora RPM Installations

### Context
The current `LutrisIntegration::isLutrisInstalled()` method fails to detect Lutris when installed via Fedora RPM packages because it only checks for user data files (database and config directory) rather than verifying the actual binary installation. When Lutris is freshly installed via RPM, the binary exists in the system PATH but the user data files haven't been created yet since the application hasn't been launched.

### Files and Functions to Change

**Primary file to modify:**
- `/media/mario/Local_Git/git-repos/linuxtrackx-ir/src/qt_gui/lutris_integration.cpp`

**Function to replace:**
- `LutrisIntegration::isLutrisInstalled()` (lines 126-185)

### Algorithm Steps

The new detection algorithm will follow these steps:

1. **Binary Existence Check**: Use `QStandardPaths::findExecutable("lutris")` to locate the Lutris binary in the system PATH
2. **Early Exit**: If binary not found, return false with appropriate logging
3. **Binary Location Logging**: Log the found binary path for debugging
4. **User Data Check**: Verify database file and config directory existence (but don't fail if missing)
5. **Status Logging**: Log the state of user data files for debugging
6. **Installation Confirmation**: Return true if binary exists, regardless of user data state

### Implementation Details

The replacement function will:
- Use `QStandardPaths::findExecutable("lutris")` for cross-platform binary detection
- Maintain existing logging patterns using `ltr_int_log_message()`
- Preserve the existing database path and config path member variables
- Keep the existing Flatpak fallback detection logic as a secondary check
- Return the same boolean result type as the current implementation

### Dependencies
- Requires `<QStandardPaths>` (already included in the header)
- No additional Qt modules needed
- No changes to class interface or member variables required

### Expected Outcome
This fix will correctly detect Lutris installations in Fedora RPM scenarios where the binary is installed but user data files haven't been created yet. The solution addresses the core issue by prioritizing binary detection over user data file existence, which is the proper approach for detecting software installations across different package managers and installation methods.

### Risk Assessment
- **Low Risk**: The change is localized to a single function
- **Backward Compatible**: Maintains the same function signature and return type
- **Testable**: Can be verified by installing Lutris via RPM and confirming detection works before first launch

### Success Criteria
1. Lutris installed via Fedora RPM is detected before first launch
2. Existing detection methods for other installation types remain functional
3. No breaking changes to the LutrisIntegration class interface
4. Proper logging maintained for debugging purposes
