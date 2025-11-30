# Qt6 Segmentation Fault - Debugging Summary for AI Prompt

## Problem
Qt6 migration of `ltr_gui` crashes with segmentation fault immediately after device detection. Qt5 version works perfectly.

**Crash Point**: After "Refresh complete", before any deferred timers fire. Crash is synchronous, not in deferred code.

**Log Output**:
```
[DeviceSetup::on_DeviceSelector_activated] Creating TirPrefs widget for ID: Tir
[DeviceSetup::on_DeviceSelector_activated] TirPrefs widget created (not activated yet)
[DeviceSetup::on_DeviceSelector_activated] Emitting signal...
[DeviceSetup::refresh] Device activation complete
[DeviceSetup::on_RefreshDevices_pressed] Refresh complete
Segmentation fault
```

## What We've Tried (All Failed)

1. **Widget null checks** - Added extensive null pointer validation
2. **Timer delays** - Increased from 0ms → 10ms → 50ms → 60ms
3. **Deferred widget insertion** - Insert widget after 10ms/20ms delay
4. **Immediate widget insertion** - Insert widget synchronously before timers
5. **Deferred activation** - Don't call `Activate()` from constructor, call after insertion
6. **Hide widget initially** - `setVisible(false)` until fully initialized
7. **Parent validation** - Check parent widget exists before operations

## Current Code State

**`TirPrefs` constructor**:
- Calls `ui.setupUi(this)`
- Initializes preferences (`ltr_int_tir_init_prefs()`)
- Sets `initializing = true`
- Sets `setVisible(false)`
- **Does NOT call `Activate()`** (deferred)

**`DeviceSetup::on_DeviceSelector_activated()`**:
- Creates `TirPrefs` widget
- Schedules 10ms timer to: insert widget → call `Activate()` → emit signals → init UI
- **Crash happens before timer fires**

**`TirPrefs::Activate()`**:
- Public method (was private)
- Schedules 20ms timer to initialize all UI widgets
- Shows widget at end

## Key Observations

1. **Qt5 works** - This is Qt6-specific
2. **Crash is synchronous** - Happens during widget creation/function return, not in timers
3. **No timer execution** - Deferred code never runs (crash happens first)
4. **Help system works** - Not a QHelpEngine issue
5. **Device detection works** - USB code is fine

## Critical Question

**Why are device preference widgets dynamically created instead of being in `ltr.ui`?**

Current approach:
- `DeviceSetup` dynamically creates device-specific preference widgets (TirPrefs, WebcamPrefs, etc.)
- Inserts them into `ui.DeviceSetupSite` layout
- Deletes and recreates when device changes

Alternative approaches to consider:
- Use `QStackedWidget` with pre-created widgets
- Include all device preference widgets in UI file, show/hide them
- Use `showEvent()` for initialization instead of constructor/timers

## What to Investigate

1. **Exact crash location** - Use gdb to get stack trace
2. **Widget creation** - Is `ui.setupUi(this)` or parent setup causing crash?
3. **Signal connections** - Are auto-connections from UI file triggering issues?
4. **Layout operations** - Is layout calculation triggered by widget creation?
5. **Qt6 differences** - What changed in Qt6 widget lifecycle that breaks this?

## Files to Review

- `src/qt_gui/tir_prefs.cpp` - Widget creation and activation
- `src/qt_gui/device_setup.cpp` - Widget insertion logic
- `src/qt_gui/device_setup.ui` - UI file structure
- `src/qt_gui/ltr.ui` - Main UI file (contains `DeviceSetupSite` layout)

## Hypothesis

The crash likely occurs because:
- Qt6 validates widget state more strictly during creation
- Layout calculations are triggered immediately on widget creation (even if hidden)
- Parent-child relationship setup triggers layout recalculation
- Signal auto-connections from UI file execute during creation

Since crash is synchronous and happens before any deferred code, the issue is in:
- Constructor execution
- `ui.setupUi(this)` 
- Widget parent assignment
- Or immediate layout operations triggered by widget creation
