# Qt6 Segmentation Fault Debugging Summary

**Date**: 2025-11-30  
**Issue**: Segmentation fault in `ltr_gui` after device detection when migrating from Qt5 to Qt6  
**Qt Version**: 6.4.2 (targeting Qt6.5 compatibility)  
**Status**: UNRESOLVED - Still crashing after multiple fix attempts

## Problem Description

The application crashes with a segmentation fault immediately after:
1. Help system initialization completes successfully ✅
2. Device detection succeeds ("Found device 8", "Type: 8") ✅
3. `TirPrefs` widget is created
4. Function returns ("Refresh complete")
5. **CRASH** - Segmentation fault

**Key Observation**: The Qt5 version works perfectly, indicating this is a Qt6-specific widget lifecycle/layout issue.

## Root Cause Analysis

The crash occurs when `TirPrefs` widget is created and inserted into a layout. The issue is related to:
- Qt6's stricter widget lifecycle management compared to Qt5
- Layout calculation happening while widgets are being modified
- `TirPrefs::Activate()` scheduling a `QTimer::singleShot` that modifies UI widgets before the widget is safely in a layout

## Attempted Fixes

### Attempt 1: Widget Access Safety Checks
**Files Modified**: `src/qt_gui/tir_prefs.cpp`
- Added null pointer checks before accessing UI widgets
- Increased timer delay from 0ms to 10ms
- Added widget readiness validation
- **Result**: ❌ Still crashes

### Attempt 2: Increased Timer Delays
**Files Modified**: `src/qt_gui/tir_prefs.cpp`
- Increased delay from 10ms to 50ms, then 60ms
- Added parent widget validation
- **Result**: ❌ Still crashes

### Attempt 3: Deferred Widget Insertion
**Files Modified**: `src/qt_gui/device_setup.cpp`
- Deferred widget insertion by 10ms, then 20ms
- Inserted widget before `Activate()` timer fires
- **Result**: ❌ Still crashes (crash happens before insertion timer fires)

### Attempt 4: Immediate Widget Insertion for TrackIR
**Files Modified**: `src/qt_gui/device_setup.cpp`
- Inserted TrackIR widget immediately (synchronously) after creation
- Ensured widget is in layout before `Activate()` timer fires
- **Result**: ❌ Still crashes

### Attempt 5: Deferred Activation (Radical Approach)
**Files Modified**: 
- `src/qt_gui/tir_prefs.cpp` - Removed `Activate()` call from constructor
- `src/qt_gui/tir_prefs.h` - Made `Activate()` public
- `src/qt_gui/device_setup.cpp` - Call `Activate()` after widget insertion

**Changes**:
1. Constructor no longer calls `Activate()` - only initializes preferences
2. Widget is created but not activated
3. Widget inserted into layout (deferred 10ms)
4. `Activate()` called after insertion
5. Widget hidden initially, shown after UI initialization

**Result**: ❌ Still crashes - crash happens before insertion timer fires

### Attempt 6: Hide Widget Until Initialized
**Files Modified**: `src/qt_gui/tir_prefs.cpp`
- Widget set to `setVisible(false)` in constructor
- Widget shown only after all UI initialization completes
- **Result**: ❌ Still crashes

## Current Code State

### Key Files Modified

**`src/qt_gui/tir_prefs.cpp`**:
- Constructor: Initializes preferences, sets `initializing = true`, hides widget
- `Activate()`: Public method, schedules 20ms timer for UI initialization
- Timer lambda: Validates widgets, sets all UI values with null checks, shows widget at end

**`src/qt_gui/device_setup.cpp`**:
- TrackIR handling: Creates widget, defers insertion + activation + signals to 10ms timer
- Other devices: Deferred insertion (10ms)

**`src/qt_gui/tir_prefs.h`**:
- `Activate()` made public (was private)

## Crash Pattern

The crash consistently occurs:
- After "Refresh complete" message
- Before any deferred timers fire
- Immediately after function returns
- No "lambda executing" messages appear (timers never fire)

This suggests the crash happens during:
- Event loop processing
- Layout calculation triggered by widget creation
- Signal/slot connections
- Or some other synchronous operation

## Observations

1. **Qt5 works perfectly** - This is purely a Qt6 compatibility issue
2. **Help system works** - Not related to QHelpEngine
3. **Device detection works** - USB/libusb code is fine
4. **Crash is immediate** - Happens synchronously, not in deferred code
5. **Widget never fully initializes** - Timers never execute

## Questions to Investigate

1. **Why are layouts separate?** - User asked why device preference widgets aren't part of `ltr.ui` directly. Could using a `QStackedWidget` or pre-creating widgets help?

2. **What triggers the crash?** - The crash happens before any timers fire. What synchronous operation is causing it?
   - Widget creation itself?
   - Parent-child relationship setup?
   - Signal connections?
   - Layout operations triggered by widget creation?

3. **Alternative approaches**:
   - Use `QStackedWidget` to switch between device preference widgets?
   - Pre-create all device widgets and show/hide instead of create/destroy?
   - Use `showEvent()` instead of constructor/timers?
   - Create widgets in UI file instead of dynamically?

## Current Logging Output

```
=== Help System Initialization Complete ===
[DeviceSetup::on_RefreshDevices_pressed] Called, refreshing devices...
[DeviceSetup::refresh] Starting device refresh...
[DeviceSetup::refresh] Adding TrackIR devices...
Found device 8
[DeviceSetup::refresh] TrackIR devices added, result: true
[DeviceSetup::refresh] Activating device selector at index: 0
[DeviceSetup::on_DeviceSelector_activated] Creating TirPrefs widget for ID: Tir
[DeviceSetup::on_DeviceSelector_activated] TirPrefs widget created (not activated yet)
[DeviceSetup::on_DeviceSelector_activated] Emitting signal...
[DeviceSetup::refresh] Device activation complete
[DeviceSetup::on_RefreshDevices_pressed] Refresh complete
Segmentation fault
```

**Note**: No timer messages appear, indicating crash happens before any deferred operations execute.

## Files Modified (Summary)

1. `src/qt_gui/tir_prefs.cpp` - Widget initialization, activation, UI updates
2. `src/qt_gui/tir_prefs.h` - Made `Activate()` public
3. `src/qt_gui/device_setup.cpp` - Widget creation and insertion logic
4. `src/qt_gui/guardian.cpp` - Added null checks
5. `src/qt_gui/profile_selector.cpp` - Added null checks
6. `CMakeLists.txt` - Qt6 version requirement (6.2+)
7. `src/qt_gui/CMakeLists.txt` - Qt6.5 compatibility notes
8. `src/qt_gui/help_view.cpp` - Qt6.2+ compatibility messages

## Next Steps to Consider

1. **Use gdb/valgrind** to get exact stack trace of where crash occurs
2. **Investigate widget creation** - Maybe the crash is in `ui.setupUi(this)` or widget parent setup
3. **Try QStackedWidget approach** - Pre-create widgets, switch between them
4. **Check signal connections** - Maybe a signal connection is triggering the crash
5. **Simplify widget creation** - Create minimal widget first, add complexity later
6. **Use Qt6-specific APIs** - Maybe there's a Qt6 way to handle this that we're missing

## Key Insight

The crash happens **synchronously** during widget creation/insertion, not in deferred code. This suggests the problem is:
- Widget constructor doing something Qt6 doesn't like
- Parent-child relationship setup
- Layout operations triggered immediately
- Signal/slot auto-connections
- Or something in the UI file loading (`ui.setupUi(this)`)

The fact that Qt5 works suggests Qt6 has stricter validation or different behavior in one of these areas.
