# ISSUE_69 Hotkey Fix Plan — Code Review Remediation

**Related:** [ISSUE_69_HOTKEY_PLAN.md](./ISSUE_69_HOTKEY_PLAN.md) (original implementation plan)  
**Status:** Phases 1–3 complete; all fixes merged to `main`  
**Priority:** P0 blocking compile + double-fire bug

---

## Executive Summary

The ISSUE_69 hotkey implementation was faithfully executed across all 5 plan steps. The QSettings key naming is consistent between write and read, the changelog entry exists, and `ltr_int_log_message` logging for silent drops works correctly. However, two **blocking** issues prevent compilation and correct runtime behavior:

1. **HKDBG macro is used but never defined** — hard compile failure
2. **Double-processing of grabbed keys** — hotkeys fire twice on most systems

Below are the concrete fixes organized by priority.

---

## Phase 1: Blocking Fixes (P0)

### Fix #1: Define `HKDBG` macro in `src/mickey/keyb_x11.cpp`

**Where:** After the `#include <Xi/xinput.h>` line (~line 25), before any function definitions.

```c
/* Debug logging — replace with fprintf(stderr, ...) if you want it always on. */
#ifndef HKDBG
#define HKDBG(...) fprintf(stderr, "DEBUG: " __VA_ARGS__)
#endif
```

**Why:** `HKDBG` is called 3 times (lines 241, 245, 556) but never defined anywhere in the codebase. This is a hard compile failure.

---

### Fix #2: Remove native event filter — rely on socket notifier only

This eliminates double-processing of grabbed keys and removes the unsafe XCB cast entirely.

#### A) `src/mickey/keyb_x11.cpp` — Delete the entire `hotKeyFilter` class (~lines 70–165):

```cpp
// DELETE THIS ENTIRE BLOCK:
#if defined(QT5_OVERRIDES) || defined(QT6_OVERRIDES)
static hotKeyFilter *hkFilter = NULL;

bool hotKeyFilter::nativeEventFilter(const QByteArray & eventType, void * message, qintptr * result)
{
  Q_UNUSED(result);
  Q_UNUSED(eventType);
  if(message == NULL){ return false; }
  
  xcb_generic_event_t* ev = static_cast<xcb_generic_event_t *>(message);
  uint8_t response_type = ev->response_type & 127;
  bool pressed = (response_type == XCB_KEY_PRESS);
  bool released = (response_type == XCB_KEY_RELEASE);
  
  if(pressed || released){
    xcb_key_press_event_t *keyEvent = static_cast<xcb_key_press_event_t *>(message);
    unsigned int keyCode = keyEvent->detail;
    unsigned int modifiers = 0;

    if(keyEvent->state & XCB_MOD_MASK_SHIFT)   modifiers |= ShiftMask;
    if(keyEvent->state & XCB_MOD_MASK_CONTROL)  modifiers |= ControlMask;
    if(keyEvent->state & XCB_MOD_MASK_1)        modifiers |= Mod1Mask;
    if(keyEvent->state & XCB_MOD_MASK_4)        modifiers |= Mod4Mask;

    keyPair_t kp(keyCode, modifiers & (ShiftMask | ControlMask | Mod1Mask | Mod4Mask));
    shortcutHash_t::iterator i = shortcutHash.find(kp);

    static int convDebugCount = 0;
    if(convDebugCount < 5){
      fprintf(stderr, "DEBUG: XCB event for window 0x%x: state=0x%x -> X11 modifiers=0x%x\n",
              keyEvent->event, keyEvent->state, modifiers);
      convDebugCount++;
    }

    fprintf(stderr, "DEBUG: Looking for match: keycode=%u, modifiers=0x%x, found=%s\n",
            keyCode, modifiers & (ShiftMask | ControlMask | Mod1Mask | Mod4Mask),
            (i != shortcutHash.end()) ? "YES" : "NO");
    if(i == shortcutHash.end()){
      fprintf(stderr, "DEBUG: Hash contents:\n");
      for(shortcutHash_t::iterator j = shortcutHash.begin(); j != shortcutHash.end(); ++j){
        fprintf(stderr, "  keycode=%u, modifiers=0x%x\n", j->first.first, j->first.second);
      }
    }else{
      fprintf(stderr, "DEBUG: MATCH FOUND! Activating shortcut\n");
    }

    if(i != shortcutHash.end()){
      i->second->activate(pressed);
      return true;
    }
  }
  return false;
}
#endif
```

#### B) `src/mickey/keyb_x11.cpp` — Delete the Qt5 `eventFilter` fallback (~lines 167–190):

```cpp
// DELETE THIS ENTIRE BLOCK:
#else //QT < 5.0
static bool (*prevFilter)(void *) = NULL;
static bool eventFilter(void *message) { ... }
#endif
```

#### C) `src/mickey/keyb_x11.cpp` — Rewrite `installFilter()`:

```c
static void installFilter()
{
#if defined(QT5_OVERRIDES) || defined(QT6_OVERRIDES)
  // Set up socket notifier to poll for X11 events directly.
  // This handles both XGrabKey grabbed keys and XI_RawKeyPress raw events.
  if(display != NULL && x11SocketNotifier == NULL){
    int fd = ConnectionNumber(display);
    x11SocketNotifier = new QSocketNotifier(fd, QSocketNotifier::Read);
    QObject::connect(x11SocketNotifier, &QSocketNotifier::activated, [](int socket){
      Q_UNUSED(socket);
      processX11Events();
    });
    x11SocketNotifier->setEnabled(true);
    HKDBG("X11 socket notifier installed on fd %d\n", fd);
  }
#endif  
}
```

#### D) `src/mickey/keyb_x11.cpp` — Rewrite `uninstallFilter()`:

```c
static void uninstallFilter()
{
#if defined(QT5_OVERRIDES) || defined(QT6_OVERRIDES)
  // Clean up socket notifier
  if(x11SocketNotifier != NULL){
    x11SocketNotifier->setEnabled(false);
    delete x11SocketNotifier;
    x11SocketNotifier = NULL;
    HKDBG("X11 socket notifier removed\n");
  }
#endif
  /* Reset dedup state so stale entries don't block future bindings. */
  s_last_raw_keycode = 0;
  s_last_raw_time    = CurrentTime;
  s_last_raw_pressed = false;

  // Close X11 display when no shortcuts remain
  if(display != NULL){
    destroyEventWindow();
    XCloseDisplay(display);
    display = NULL;
    window = 0;
  }
}
```

#### E) `src/mickey/keyb_x11.cpp` — Remove `createEventWindow()` and `destroyEventWindow()` entirely. They're no longer needed since we grab on root window directly.

#### F) `src/mickey/keyb_x11.cpp` — In `grabKeyX()`, remove the redundant `grabWindow` variable:

```c
// BEFORE (line ~615):
Window grabWindow = window;  // Use root window, not eventWindow
...
XGrabKey(display, code, modifiers, grabWindow, true, GrabModeAsync, GrabModeAsync);

// AFTER:
XGrabKey(display, code, modifiers, window, true, GrabModeAsync, GrabModeAsync);
```

#### G) `src/mickey/keyb_x11.cpp` — Remove the dead second XGrabKey call (~lines 625–630):

```c
// DELETE THIS BLOCK:
errorEncountered = false;
XGrabKey(display, code, modifiers, grabWindow, false, GrabModeAsync, GrabModeAsync);
XSync(display, false);
if(!errorEncountered){
    fprintf(stderr, "DEBUG: XGrabKey (owner_events=false) also succeeded for code=%u\n", code);
}
```

#### H) `src/mickey/keyb_x11.h` — Remove the `hotKeyFilter` class declaration and its include:

```cpp
// DELETE THIS BLOCK (~lines 27–38):
#if defined(QT5_OVERRIDES) || defined(QQT6_OVERRIDES)
class hotKeyFilter : public QAbstractNativeEventFilter
{
 protected:
  bool nativeEventFilter(const QByteArray & eventType, void * message, qintptr * result); 
};
#endif

// DELETE THIS LINE:
#include <QAbstractNativeEventFilter>
```

---

## Phase 2: Correctness Fixes (P1–P2)

### Fix #3: Reset dedup state in `setShortCut()` after successful registration

**Where:** In `src/mickey/keyb_x11.cpp`, right after the `shortcutHash.insert(...)` call (~line 590):

```c
  shortcutHash.insert(std::pair<std::pair<KeyCode, unsigned int>, shortcut *>
    (keyPair_t(code, modifiers), shortcutId));
  
  /* Reset dedup on new binding so stale entries don't suppress the first press. */
  s_last_raw_keycode = 0;
  s_last_raw_time    = CurrentTime;
  s_last_raw_pressed = false;

  if(needInstallFilter){
    HKDBG("Installing native event filter (first shortcut registered)\n");
    installFilter();
  }
```

---

### Fix #4: Fix `grabKeyX()` cleanup to ungrab from the correct window

**Where:** In `src/mickey/keyb_x11.cpp`, the NumLock/ScrollLock grab/ungrab calls.

The current code grabs on `grabWindow` (root) but cleans up on `window`. Since both are set to root in practice, this works by accident. Make it explicit:

```c
// In grabKeyX(), change all references from 'window' to DefaultRootWindow(display)
// for the NumLock/ScrollLock grabs to be consistent with the primary grab:

  //Numlock
  XGrabKey(display, code, modifiers | Mod2Mask, DefaultRootWindow(display), true, GrabModeAsync, GrabModeAsync);
  
  //ScrollLock  
  XGrabKey(display, code, modifiers | Mod5Mask, DefaultRootWindow(display), true, GrabModeAsync, GrabModeAsync);

  //NumLock + ScrollLock
  XGrabKey(display, code, modifiers | Mod2Mask | Mod5Mask, DefaultRootWindow(display), true, GrabModeAsync, GrabModeAsync);
```

And in `ungrabKeyX()`, use the same:

```c
// In ungrabKeyX(), use DefaultRootWindow(display) consistently:
  Window root = DefaultRootWindow(display);
  XUngrabKey(display, code, modifiers, root);
  XUngrabKey(display, code, modifiers | Mod2Mask, root);
  XUngrabKey(display, code, modifiers | Mod5Mask, root);
  XUngrabKey(display, code, modifiers | Mod2Mask | Mod5Mask, root);
```

---

## Phase 3: Polish (P3–P4) — ✅ COMPLETE

### Fix #5: Guard `cd` in `scripts/test/run_gui.sh` ✅

**Where:** Before line 45 (`cd src/qt_gui`) — **DONE** (commit 8248292).

```bash
# Change to GUI directory and launch
if [ ! -d "src/qt_gui" ]; then
    echo "Error: src/qt_gui not found. Run this script from the repository root." >&2
    exit 1
fi
cd src/qt_gui
```

---

### Fix #6: Replace all `fprintf(stderr, "DEBUG:")` with `HKDBG()` in keyb_x11.cpp ✅

**Status:** Already completed in a prior phase. Verified — zero remaining `fprintf(stderr, "DEBUG:` calls outside the HKDBG macro definition itself.

---

## Summary of Changes by File

| File | Changes | Effort |
|------|---------|--------|
| `src/mickey/keyb_x11.cpp` | Add HKDBG define; remove hotKeyFilter class + install/uninstall; rewrite to socket-notifier-only; fix grabWindow/window consistency; reset dedup in setShortCut(); replace fprintf with HKDBG | **Medium** (~30 edits) |
| `src/mickey/keyb_x11.h` | Remove hotKeyFilter class declaration + QAbstractNativeEventFilter include | **Small** (2 lines removed) |
| `scripts/test/run_gui.sh` | Add cd guard before `cd src/qt_gui` | **Trivial** (4 lines added) |

---

## Verification After Fixes

1. **Compile:** `HKDBG` defined → no compile errors
2. **Single-fire test:** Bind a key, press it — should fire exactly once (no double-processing)
3. **Re-bind test:** Change binding while tracking → new key fires immediately (dedup reset)
4. **Unbind-all test:** Clear all hotkeys → X11 display closes cleanly (no leaks)
5. **External numpad test:** Bind external pad keycode → number-row key does not fire it

---

## Original Issues from Code Review (for reference)

### What was NOT broken (confirmed good):
- ✅ QSettings key format (`HotKeys/xkey_%N`) — consistent write/read across org/app
- ✅ `ltr_int_log_message` declaration accessible via `<utils.h>` in ltr_show.cpp
- ✅ XInput2 XISelectEvents setup and raw event handling logic
- ✅ Modifier state extraction from XQueryPointer for raw events
- ✅ Desktop files correctly set `QT_QPA_PLATFORM=xcb`
- ✅ run_gui.sh Wayland detection and default-to-xcb behavior
- ✅ Silent drop logging in hotKey_activated()
- ✅ Changelog entry present

### Issues that were noted but not fixed (out of scope for this plan):
- Dead Qt5 code path with `#error` in install/uninstallFilter — misleading since project targets Qt6 only. Consider removing entirely in a future cleanup.
- Magic number `convDebugCount < 5` — arbitrary limit with no explanation. Replace with named constant or remove.
