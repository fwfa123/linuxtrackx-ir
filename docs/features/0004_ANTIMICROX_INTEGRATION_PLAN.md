# Antimicrox Integration Plan

## Context
The Antimicrox function needs to be integrated into the existing Gaming tab in `ltr.ui`. The current "Launch ltr_pipe for Antimicrox" button will be replaced with a comprehensive ltr_pipe control interface that supports multiple game formats and provides device naming capabilities. This feature will allow users to launch `ltr_pipe` with appropriate settings for various game integrations, enabling head tracking data to be used as controller input for games through Antimicrox or direct game support.

## Relevant Files and Functions

### UI Files
- `src/qt_gui/ltr.ui` - Contains the LaunchLtrPipeButton in the Gaming tab (to be replaced)
- `src/qt_gui/ltr_gui.cpp` - Contains the `on_LaunchLtrPipeButton_pressed()` slot (currently placeholder)
- `src/qt_gui/ltr_gui.h` - Contains the slot declaration
- New UI elements to be added: format combobox, device name input, control buttons

### Core Functionality
- `src/ltr_pipe.c` - The ltr_pipe executable source that outputs tracking data
- `src/qt_gui/ltr_gui.cpp` - Main GUI class with QProcess integration examples

### Configuration Management
- `src/qt_gui/ltr_gui.cpp` - Contains QSettings usage for persistent configuration
- Existing profile management in testing section

## Implementation Details

### Phase 1: New ltr_pipe Control Interface

#### 1.1 Replace LaunchLtrPipeButton with Comprehensive Control Interface
**File**: `src/qt_gui/ltr.ui`
**UI Elements to Add**:
1. **Format Selection Combobox**: Dropdown with supported game formats
2. **Device Name Input**: Text field for custom input device naming
3. **Control Buttons**: Start, Stop, Pause icons with appropriate functionality
4. **Status Display**: Show current ltr_pipe status and device information

**Supported Formats** (based on README.ltr_pipe analysis):
- **uinput-abs**: Joystick emulation for Antimicrox (Linux)
- **FlightGear**: Custom protocol for FlightGear simulator
- **IL-2 Shturmovik**: DeviceLink format for IL-2 series
- **Silent Wings**: Remote control format for Silent Wings
- **Default**: Tab-separated values for custom processing

#### 1.2 New Control Interface Implementation
**File**: `src/qt_gui/ltr_gui.cpp`
**New Functions to Add**:
- `on_FormatComboBox_currentTextChanged()` - Handle format selection changes
- `on_StartButton_pressed()` - Launch ltr_pipe with selected format
- `on_StopButton_pressed()` - Stop ltr_pipe and cleanup uinput devices
- `on_PauseButton_pressed()` - Pause/resume ltr_pipe (send USR1 signal)
- `on_DeviceNameTextChanged()` - Handle custom device naming

**Algorithm for Start Button**:
1. Validate ltr_pipe executable exists
2. Get selected format from combobox
3. Get custom device name from input field (default to "tir1" if empty)
4. Launch ltr_pipe with appropriate format flags
5. Update UI status and enable/disable controls
6. Store device name and format in QSettings for persistence

**Algorithm for Stop Button**:
1. Stop ltr_pipe process (send SIGTERM)
2. Clean up uinput devices to prevent naming conflicts
3. Reset UI status and enable start button
4. Clear device information display

#### 1.3 ltr_pipe Executable Detection
**Algorithm**:
1. Check common installation paths:
   - `/opt/linuxtrack/bin/ltr_pipe`
   - `/usr/local/bin/ltr_pipe`
   - System PATH via `QStandardPaths::findExecutable()`
2. Fallback to relative path if running from build directory
3. Provide clear error message if executable not found

#### 1.4 Device Naming and uinput Management
**Algorithm**:
1. **Device Name Input**: 
   - Default suggestion: "tir1" (no spaces, simple identifier)
   - Validate input: alphanumeric characters only, no spaces
   - Store in QSettings for persistence across sessions
2. **uinput Device Cleanup**:
   - When stopping ltr_pipe, remove associated uinput devices
   - Use `ioctl(fd, UI_DEV_DESTROY)` or equivalent cleanup
   - Prevent device name conflicts on subsequent starts
3. **Device Persistence**:
   - Store last used device name and format in QSettings
   - Restore previous settings when UI is reopened

### Phase 2: Format-Specific Configuration

#### 2.1 Format-Specific Parameter Management
**Algorithm**:
1. **uinput-abs Format**:
   - Uinput range customization (100-10000, default: 1000)
   - Device name customization (default: "tir1")
   - Output destination: `/dev/uinput`
2. **Game-Specific Formats**:
   - FlightGear: Network port configuration (default: 6543)
   - IL-2 Shturmovik: DeviceLink port configuration (default: 6543)
   - Silent Wings: Remote control port configuration (default: 6543)
   - Default: Output destination (stdout, file, or network)
3. **Parameter Storage**:
   - Store format-specific parameters in QSettings
   - Maintain separate settings for each format type
   - Provide sensible defaults for all parameters

#### 2.2 Advanced ltr_pipe Options
**Algorithm**:
1. Add configuration options for each format:
   - Network settings (host, port) for network-based formats
   - File output paths for file-based formats
   - LinuxTrack profile selection (default: "Default")
   - Custom output filtering and transformation
2. Store all preferences in QSettings with format-specific groups
3. Apply appropriate settings when launching ltr_pipe

### Phase 3: User Experience Enhancements

#### 3.1 Status Monitoring and Device Information
**Algorithm**:
1. **Process Status Monitoring**:
   - Monitor ltr_pipe process status (running, stopped, paused)
   - Show active/inactive status with visual indicators
   - Display current format and device name in status area
2. **Device Information Display**:
   - Show created uinput device path (e.g., `/dev/input/eventX`)
   - Display device name as it appears in system
   - Provide device removal status after cleanup
3. **Control State Management**:
   - Enable/disable controls based on current state
   - Show appropriate button states (Start/Stop/Pause)
   - Provide visual feedback for all operations

#### 3.2 UI Layout and Gaming Tab Integration
**Algorithm**:
1. **Replace LaunchLtrPipeButton**:
   - Remove existing "Launch ltr_pipe for Antimicrox" button
   - Add new "ltr_pipe Control" section in Gaming tab
   - Position below existing gaming platform buttons
2. **Move Launch Mickey Button**:
   - Relocate "Launch Mickey (TrackIR)" button to System tab
   - Maintain functionality in new location
   - Update tab navigation and button connections
3. **New Control Interface Layout**:
   - Format selection combobox (top)
   - Device name input field (middle)
   - Control buttons row (Start/Stop/Pause)
   - Status display area (bottom)

## Technical Requirements

### Dependencies
- `ltr_pipe` executable must be built and available
- Linux uinput support (kernel module loaded)
- Appropriate permissions for `/dev/uinput` access
- QProcess for process management
- QSettings for configuration persistence
- Linux input subsystem access for device management
- Signal handling for process control (SIGTERM, SIGUSR1)

### Error Handling
- Executable not found scenarios
- Permission denied for uinput access
- Process launch failures
- Configuration file errors
- uinput device creation failures
- Device cleanup failures
- Process signal handling errors
- Graceful fallback to manual command line instructions

### Cross-Platform Considerations
- Linux-specific uinput functionality
- Windows compatibility (different output formats)
- macOS compatibility (different input methods)
- Graceful degradation for unsupported platforms

## Implementation Notes

### ltr_pipe Command Line Formats
Based on the man page and source code analysis:

**uinput-abs (Antimicrox)**:
```bash
ltr_pipe --format-uinput-abs --uinput-abs-range=1000 --output-file=/dev/uinput
```

**FlightGear (Network)**:
```bash
ltr_pipe --format-flightgear --output-net-udp --dst-port=6543
```

**IL-2 Shturmovik**:
```bash
ltr_pipe --format-il2 --output-net-udp --dst-port=6543
```

**Silent Wings**:
```bash
ltr_pipe --format-silentwings --output-net-udp --dst-port=6543
```

**Default Output**:
```bash
ltr_pipe --format-default --output-stdout
```

### QProcess Integration Pattern
Follow existing patterns in the codebase:
```cpp
QProcess::startDetached(ltrPipePath, QStringList() 
    << "--format-uinput-abs" 
    << "--uinput-abs-range=1000" 
    << "--output-file=/dev/uinput");
```

### Configuration Persistence
Use existing QSettings pattern with format-specific groups:
```cpp
QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
settings.beginGroup(QStringLiteral("ltr_pipe"));
settings.beginGroup(QStringLiteral("uinput-abs"));
settings.setValue(QStringLiteral("device_name"), "tir1");
settings.setValue(QStringLiteral("uinput_range"), 1000);
settings.endGroup();
settings.endGroup();
```

### Device Management
For uinput device cleanup:
```cpp
// After stopping ltr_pipe, cleanup uinput devices
// This prevents naming conflicts on subsequent starts
// Implementation will use Linux input subsystem APIs
```

## Testing Requirements

### Functional Testing
- Verify ltr_pipe launches with correct parameters for each format
- Test uinput device creation and accessibility for Antimicrox
- Validate device naming and persistence across sessions
- Test uinput device cleanup and conflict prevention
- Validate configuration persistence for all format types
- Test error handling for various failure scenarios

### Integration Testing
- Verify coordination with existing tracking functionality
- Test format switching and parameter changes
- Validate status monitoring and device information display
- Test process control (start/stop/pause) functionality
- Verify device management integration with Linux input subsystem

### User Experience Testing
- Verify clear error messages and user guidance
- Test format selection and parameter configuration
- Validate status display clarity and device information
- Test workflow integration with gaming tab
- Verify Launch Mickey button relocation to System tab
- Test device naming validation and suggestions
