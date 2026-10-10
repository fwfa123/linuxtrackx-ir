/*
TODO:
Think through handling for more than one request for the same key.
Reporting problems to user.
*/

// Include keyb_x11.h FIRST - it includes Qt headers before X11 headers
#include "keyb_x11.h"
#include "keyb.h"
#include <cstdio>

// XCB is needed for native event filter
#if defined(QT5_OVERRIDES) || defined(QT6_OVERRIDES)
  #include <xcb/xcb.h>
  #include <QSocketNotifier>
  #include <QTimer>
#endif

// X11 protocol constants (X_GrabKey, etc.) are in Xproto.h
#include <X11/Xproto.h>
#include <X11/keysym.h>
#include <Xi/xi.h>
#include <Xi/xinput.h>

static shortcutHash_t shortcutHash;
static bool errorEncountered;
static QString errMsg;
static Display *display = NULL;
static Window window;
static Window eventWindow = 0;

/* Dedup: track the last raw key event so we don't fire twice when both
 * XGrabKey and XI_RawKeyPress arrive for the same physical press. */
static KeyCode   s_last_raw_keycode = 0;
static Time      s_last_raw_time    = CurrentTime;
static bool      s_last_raw_pressed = false;

/* Cached XInput extension opcode so we can recognise raw key events. */
static int       s_xi_opcode        = -1;

#if defined(QT5_OVERRIDES) || defined(QT6_OVERRIDES)
static QSocketNotifier *x11SocketNotifier = NULL;
#endif

static bool grabKeyX(Display *display, Window &window, KeyCode code, unsigned int modifiers);
static bool ungrabKeyX(Display *display, Window &window, KeyCode code, unsigned int modifiers);

#if defined(QT5_OVERRIDES) || defined(QT6_OVERRIDES)

static hotKeyFilter *hkFilter = NULL;

bool hotKeyFilter::nativeEventFilter(const QByteArray & eventType, void * message, qintptr * result)
{
  Q_UNUSED(result);
  Q_UNUSED(eventType);
  if(message == NULL){
    return false;
  }
  
  // Qt6: Try to process ALL events as potential XCB events
  // Qt6 may use different event type strings or pass events differently
  // We'll check if the message structure matches XCB key events
  
  // First, try to cast as XCB generic event
  xcb_generic_event_t* ev = static_cast<xcb_generic_event_t *>(message);
  
  // Check if this looks like an XCB event by examining response_type
  // Valid XCB event response types are 0-127 (masked)
  uint8_t response_type = ev->response_type & 127;
  
  // XCB_KEY_PRESS = 2, XCB_KEY_RELEASE = 3
  // But we should also check for other valid XCB event types to avoid false positives
  bool pressed = (response_type == XCB_KEY_PRESS);
  bool released = (response_type == XCB_KEY_RELEASE);
  
  
  
  // Qt6: Process key events - process ALL key events since we grab on root window
  if(pressed || released){
    xcb_key_press_event_t *keyEvent = static_cast<xcb_key_press_event_t *>(message);

    // Process all key events since we grab on root window

    unsigned int keyCode = keyEvent->detail;
    unsigned int modifiers = 0;

    // XCB modifier masks map directly to X11 modifier masks:
    // XCB_MOD_MASK_SHIFT (0x01) -> ShiftMask (0x01)
    // XCB_MOD_MASK_CONTROL (0x04) -> ControlMask (0x04)
    // XCB_MOD_MASK_1 (0x08) -> Mod1Mask (0x08) - Alt
    // XCB_MOD_MASK_4 (0x40) -> Mod4Mask (0x40) - Super/Windows
    // Note: XCB_MOD_MASK_2 (0x10) is Num Lock, XCB_MOD_MASK_3 (0x20) is Scroll Lock
    // These should be ignored for hotkey matching

    if(keyEvent->state & XCB_MOD_MASK_SHIFT){
      modifiers |= ShiftMask;
    }
    if(keyEvent->state & XCB_MOD_MASK_CONTROL){
      modifiers |= ControlMask;
    }
    if(keyEvent->state & XCB_MOD_MASK_1){
      modifiers |= Mod1Mask;
    }
    if(keyEvent->state & XCB_MOD_MASK_4){
      modifiers |= Mod4Mask;
    }

    // Debug: Show raw state and converted modifiers
    static int convDebugCount = 0;
    if(convDebugCount < 5){
      fprintf(stderr, "DEBUG: XCB event for window 0x%x: state=0x%x -> X11 modifiers=0x%x\n",
              keyEvent->event, keyEvent->state, modifiers);
      convDebugCount++;
    }

    keyPair_t kp(keyCode, modifiers & (ShiftMask | ControlMask | Mod1Mask | Mod4Mask));
    shortcutHash_t::iterator i = shortcutHash.find(kp);

    // Debug: Log ALL matching attempts
    fprintf(stderr, "DEBUG: Looking for match: keycode=%u, modifiers=0x%x, found=%s\n",
            keyCode, modifiers & (ShiftMask | ControlMask | Mod1Mask | Mod4Mask),
            (i != shortcutHash.end()) ? "YES" : "NO");
    if(i == shortcutHash.end()){
      // Debug: Show what's in the hash
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

#else //QT < 5.0 (not used in Qt6)
// Qt6: setEventFilter API removed, use QAbstractNativeEventFilter instead
// This branch is kept for compatibility but should not be used with Qt6
static bool (*prevFilter)(void *) = NULL;

static bool eventFilter(void *message)
{
  XEvent *event = (XEvent*)message;
  bool pressed = event->type == KeyPress;
  bool released = event->type == KeyRelease;
  if(pressed || released){
    XKeyEvent *key = (XKeyEvent*) event;
    keyPair_t kp(key->keycode, key->state & (ShiftMask | ControlMask | Mod1Mask | Mod4Mask));
    shortcutHash_t::iterator i = shortcutHash.find(kp);
    if(i != shortcutHash.end()){
      i->second->activate(pressed);
      return true;
    }
  }
  if(prevFilter != NULL){
    return prevFilter(message);
  }
  return false;
}
#endif

// Qt6: Alternative approach - use Qt's event system for global hotkeys
// This creates a hidden window to receive the grabbed key events

static void createEventWindow()
{
  if(display == NULL || eventWindow != 0){
    return;
  }

  // Create a hidden window to receive grabbed key events
  eventWindow = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);

  // Select key events on this window
  XSelectInput(display, eventWindow, KeyPressMask | KeyReleaseMask);

  fprintf(stderr, "DEBUG: Created event window 0x%lx for receiving grabbed keys\n", eventWindow);
}

static void destroyEventWindow()
{
  if(display != NULL && eventWindow != 0){
    XDestroyWindow(display, eventWindow);
    eventWindow = 0;
    fprintf(stderr, "DEBUG: Destroyed event window\n");
  }
}

// Qt6: Process X11 events directly via socket notifier (XGrabKey events don't come through native event filter)
static void processX11Events()
{
  if(display == NULL || shortcutHash.empty()){
    return;
  }

  // Process all pending X11 events
  while(XPending(display) > 0){
    XEvent event;
    XNextEvent(display, &event);

    /* Handle XI_RawKeyPress / XI_RawKeyRelease (XInput2 raw key events).
     * These arrive even when a game has grabbed the keyboard with
     * XGrabKeyboard, so they are our fallback for fullscreen games. */
    if(event.type == GenericEvent && s_xi_opcode >= 0){
      if (XGetEventData(display, &event)) {
        XIRawEvent *raw = (XIRawEvent *) event.xdata.generic.data;
        if (raw->event_type == XI_RawKeyPress || raw->event_type == XI_RawKeyRelease) {
          KeyCode    rk     = raw->detail;
          Time       rtime  = raw->timestamp;
          bool       rpress = (raw->event_type == XI_RawKeyPress);

          /* Dedup: skip if the same physical press was already handled by
           * XGrabKey on this (keycode, time, pressed) tuple. */
          if (!(rk == s_last_raw_keycode && rtime == s_last_raw_time &&
                rpress == s_last_raw_pressed)) {
            s_last_raw_keycode = rk;
            s_last_raw_time    = rtime;
            s_last_raw_pressed = rpress;

            /* Raw events carry no modifier mask — read it from the keyboard.
             * We only care about Shift, Control, Mod1 (Alt), and Mod4 (Super). */
            unsigned int modifiers = 0;
            unsigned int state;
            Window       dummy_win;
            int          dx, dy, dd;
            XQueryPointer(display, DefaultRootWindow(display), &dummy_win,
                          &dummy_win, &dx, &dy, &dd, &dd, &state);
            if (state & ShiftMask)      modifiers |= ShiftMask;
            if (state & ControlMask)    modifiers |= ControlMask;
            if (state & Mod1Mask)       modifiers |= Mod1Mask;
            if (state & Mod4Mask)       modifiers |= Mod4Mask;

            keyPair_t kp(rk, modifiers);
            shortcutHash_t::iterator i = shortcutHash.find(kp);
            if (i != shortcutHash.end()) {
              HKDBG("DEBUG: Raw event keycode=%u pressed=%d -> matched\n", rk, rpress);
              i->second->activate(rpress);
            }
          } else {
            HKDBG("DEBUG: Raw event keycode=%u deduped (same press)\n", rk);
          }
        }
        XFreeEventData(display, &event.xcookie);
      }
    }

    // Check if this is a key event
    bool pressed = (event.type == KeyPress);
    bool released = (event.type == KeyRelease);

    if(pressed || released){
      XKeyEvent *keyEvent = &event.xkey;
      keyPair_t kp(keyEvent->keycode, keyEvent->state & (ShiftMask | ControlMask | Mod1Mask | Mod4Mask));
      shortcutHash_t::iterator i = shortcutHash.find(kp);

      if(i != shortcutHash.end()){
        i->second->activate(pressed);
      }
    }
  }
}

static void installFilter()
{
#if defined(QT5_OVERRIDES) || defined(QT6_OVERRIDES)
  hkFilter = new hotKeyFilter();
  QAbstractEventDispatcher *dispatcher = QAbstractEventDispatcher::instance();
  if(dispatcher != NULL){
    dispatcher->installNativeEventFilter(hkFilter);
    fprintf(stderr, "DEBUG: Native event filter installed on dispatcher: %p\n", dispatcher);
  }else{
    fprintf(stderr, "DEBUG: ERROR - QAbstractEventDispatcher::instance() returned NULL!\n");
  }

  // Qt6: Also set up socket notifier to poll for X11 events directly
  // XGrabKey events don't come through native event filter in Qt6
  if(display != NULL && x11SocketNotifier == NULL){
    int fd = ConnectionNumber(display);
    x11SocketNotifier = new QSocketNotifier(fd, QSocketNotifier::Read);
    QObject::connect(x11SocketNotifier, &QSocketNotifier::activated, [](int socket){
      Q_UNUSED(socket);
      processX11Events();
    });
    x11SocketNotifier->setEnabled(true);
    fprintf(stderr, "DEBUG: X11 socket notifier installed on fd %d\n", fd);
  }
#else
  // Qt6: setEventFilter removed - this branch should not be used
  // Keeping for compatibility with older Qt versions
  if(prevFilter == NULL){
    // Note: setEventFilter doesn't exist in Qt6, this will not compile
    // Use QT6_OVERRIDES to use native event filter instead
    #error "setEventFilter not available in Qt6 - define QT6_OVERRIDES to use native event filter"
  }
#endif  
}

static void uninstallFilter()
{
#if defined(QT5_OVERRIDES) || defined(QT6_OVERRIDES)
  QAbstractEventDispatcher::instance()->removeNativeEventFilter(hkFilter);
  delete hkFilter;
  hkFilter = NULL;

  // Clean up socket notifier
  if(x11SocketNotifier != NULL){
    x11SocketNotifier->setEnabled(false);
    delete x11SocketNotifier;
    x11SocketNotifier = NULL;
    fprintf(stderr, "DEBUG: X11 socket notifier removed\n");
  }
#else
  // Qt6: setEventFilter removed - this branch should not be used
  if(prevFilter != NULL){
    // Note: setEventFilter doesn't exist in Qt6
    #error "setEventFilter not available in Qt6 - define QT6_OVERRIDES to use native event filter"
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


static int (*prev_x_errhandler)(Display* display, XErrorEvent* event);

#define ERR_MSG_SIZE 1024

static int my_x_errhandler(Display* display, XErrorEvent *event)
{
  if((event->request_code == X_GrabKey)||(event->request_code == X_UngrabKey)){
    char msg[ERR_MSG_SIZE];
    //to be 100% sure there is a place for ending NULL, I sub 1 from ERR_MSG_SIZE
    XGetErrorText(display, event->error_code, msg, ERR_MSG_SIZE-1);
    //printf("X Error: %s\n", msg);
    errMsg = QString::fromUtf8(msg);
    errorEncountered = true;
  }
  return 0;
}

static unsigned int getModifiers(Qt::KeyboardModifiers mods)
{
  unsigned int modifiers = 0;
  
  modifiers |= (mods & Qt::ShiftModifier)? ShiftMask : 0;
  modifiers |= (mods & Qt::ControlModifier)? ControlMask : 0;
  modifiers |= (mods & Qt::AltModifier)? Mod1Mask : 0;
  modifiers |= (mods & Qt::MetaModifier)? Mod4Mask : 0;
  //Mod2Mask => NumLock!
  //Mod5Mask => ScrollLock!
  // KeypadModifier is NOT an X grab mask; KP_* keycodes distinguish the numpad.
  return modifiers;
}

static KeySym qtKeyToXKeysym(int qtKey, bool fromKeypad)
{
  if(fromKeypad){
    switch(qtKey){
      case Qt::Key_0: return XK_KP_0;
      case Qt::Key_1: return XK_KP_1;
      case Qt::Key_2: return XK_KP_2;
      case Qt::Key_3: return XK_KP_3;
      case Qt::Key_4: return XK_KP_4;
      case Qt::Key_5: return XK_KP_5;
      case Qt::Key_6: return XK_KP_6;
      case Qt::Key_7: return XK_KP_7;
      case Qt::Key_8: return XK_KP_8;
      case Qt::Key_9: return XK_KP_9;
      case Qt::Key_Plus: return XK_KP_Add;
      case Qt::Key_Minus: return XK_KP_Subtract;
      case Qt::Key_Asterisk: return XK_KP_Multiply;
      case Qt::Key_Slash: return XK_KP_Divide;
      case Qt::Key_Period: return XK_KP_Decimal;
      case Qt::Key_Comma: return XK_KP_Separator;
      case Qt::Key_Enter:
      case Qt::Key_Return: return XK_KP_Enter;
      case Qt::Key_Equal: return XK_KP_Equal;
      case Qt::Key_Insert: return XK_KP_Insert;
      case Qt::Key_Delete: return XK_KP_Delete;
      case Qt::Key_Home: return XK_KP_Home;
      case Qt::Key_End: return XK_KP_End;
      case Qt::Key_PageUp: return XK_KP_Page_Up;
      case Qt::Key_PageDown: return XK_KP_Page_Down;
      case Qt::Key_Left: return XK_KP_Left;
      case Qt::Key_Right: return XK_KP_Right;
      case Qt::Key_Up: return XK_KP_Up;
      case Qt::Key_Down: return XK_KP_Down;
      default:
        break;
    }
  }

  /* Qt Latin-1 printable keys match X11 Latin-1 keysyms (e.g. '=' -> XK_equal). */
  if((qtKey >= Qt::Key_Space && qtKey <= Qt::Key_AsciiTilde) ||
     (qtKey >= Qt::Key_nobreakspace && qtKey <= Qt::Key_ydiaeresis)){
    return static_cast<KeySym>(qtKey);
  }

  switch(qtKey){
    case Qt::Key_Escape: return XK_Escape;
    case Qt::Key_Tab: return XK_Tab;
    case Qt::Key_Backtab: return XK_ISO_Left_Tab;
    case Qt::Key_Backspace: return XK_BackSpace;
    case Qt::Key_Return: return XK_Return;
    case Qt::Key_Enter: return XK_KP_Enter;
    case Qt::Key_Insert: return XK_Insert;
    case Qt::Key_Delete: return XK_Delete;
    case Qt::Key_Pause: return XK_Pause;
    case Qt::Key_Print: return XK_Print;
    case Qt::Key_Home: return XK_Home;
    case Qt::Key_End: return XK_End;
    case Qt::Key_Left: return XK_Left;
    case Qt::Key_Up: return XK_Up;
    case Qt::Key_Right: return XK_Right;
    case Qt::Key_Down: return XK_Down;
    case Qt::Key_PageUp: return XK_Page_Up;
    case Qt::Key_PageDown: return XK_Page_Down;
    case Qt::Key_CapsLock: return XK_Caps_Lock;
    case Qt::Key_NumLock: return XK_Num_Lock;
    case Qt::Key_ScrollLock: return XK_Scroll_Lock;
    case Qt::Key_F1: return XK_F1;
    case Qt::Key_F2: return XK_F2;
    case Qt::Key_F3: return XK_F3;
    case Qt::Key_F4: return XK_F4;
    case Qt::Key_F5: return XK_F5;
    case Qt::Key_F6: return XK_F6;
    case Qt::Key_F7: return XK_F7;
    case Qt::Key_F8: return XK_F8;
    case Qt::Key_F9: return XK_F9;
    case Qt::Key_F10: return XK_F10;
    case Qt::Key_F11: return XK_F11;
    case Qt::Key_F12: return XK_F12;
    case Qt::Key_Super_L: return XK_Super_L;
    case Qt::Key_Super_R: return XK_Super_R;
    case Qt::Key_Menu: return XK_Menu;
    default:
      break;
  }

  const QKeySequence key(static_cast<Qt::Key>(qtKey));
  return XStringToKeysym(qPrintable(key.toString(QKeySequence::PortableText)));
}

static bool removeIdFromHash(shortcut* shortcutId, keyPair_t *kp = NULL)
{
  bool res = false;
  shortcutHash_t::iterator i;
  for(i = shortcutHash.begin(); i != shortcutHash.end(); /* empty */){
    if(i->second == shortcutId){
      //printf("Removing id %p\n", shortcutId);
      if(kp != NULL){
	*kp = i->first;
      }
      shortcutHash.erase(i++);
      res = true;
    }else{
      ++i;
    }
  }
  return res;
}

static bool translateSequence(const QKeySequence &s, KeyCode &code, unsigned int &modifiers)
{
  // Qt6: QKeyCombination API - extract key and modifiers separately
  QKeyCombination combo = s[0];
  const int qtKey = static_cast<int>(combo.key());
  const Qt::KeyboardModifiers kmods = combo.keyboardModifiers();
  const bool fromKeypad = (kmods & Qt::KeypadModifier) != 0;
  modifiers = getModifiers(kmods);

  // Debug: Show what modifiers were extracted
  static int regDebugCount = 0;
  if(regDebugCount < 5){
    fprintf(stderr, "DEBUG: translateSequence: QKeySequence='%s', combo.key()=%d, keypad=%d, combo.keyboardModifiers()=0x%x, extracted modifiers=0x%x\n",
            qPrintable(s.toString()), qtKey, fromKeypad ? 1 : 0,
            static_cast<unsigned int>(kmods), modifiers);
    regDebugCount++;
  }

  KeySym sym = qtKeyToXKeysym(qtKey, fromKeypad);
  if(sym == NoSymbol){
    //printf("Unknown symbol!\n");
    return false;
  }
  code = XKeysymToKeycode(display, sym);
  if(code == 0){
    //printf("Unknown code!\n");
    return false;
  }
  return true;
}

bool setShortCut(const QKeySequence &s, shortcut* shortcutId)
{
  if(display == NULL){
    // Qt5/Qt6: QX11Info removed, use X11 APIs directly
    display = XOpenDisplay(NULL);
    if(display == NULL) {
      // X11 display unavailable - cannot set shortcuts
      return false;
    }
    window = DefaultRootWindow(display);

    // Qt6: Create a dedicated window for receiving grabbed key events
    createEventWindow();

    /* Listen for raw key events on the root window so we still receive keys
     * when a game has XGrabKeyboard (e.g. Arma 3 fullscreen). */
    {
      int         event_base, error_base;
      XIEventMask emask;
      unsigned char mask[] = { 1 << XI_RawKeyPress, 1 << XI_RawKeyRelease };
      if (XQueryExtension(display, "XInputExtension", &s_xi_opcode, &event_base, &error_base)) {
        emask.deviceid   = XIAllMasterDevices;
        emask.mask_len   = sizeof(mask);
        emask.mask       = mask;
        XISelectEvents(display, DefaultRootWindow(display), &emask, 1);
      }
    }

    fprintf(stderr, "DEBUG: Opened X11 display, root window=0x%lx, event window=0x%lx\n", window, eventWindow);
  }
  removeIdFromHash(shortcutId);
  if(s.isEmpty()){
    //printf("Empty key sequence!\n");
    return false;
  }

  /* Look up a saved X keycode for this hotkey. If present, use it directly
   * instead of translating via KeySym — this avoids ambiguity on external
   * numpads where the same keysym maps to different physical keycodes. */
  unsigned int modifiers;
  KeyCode      code = 0;
  if(shortcutId && shortcutId->getHotKeyId() >= 0){
    QSettings settings(QString::fromUtf8("linuxtrack"), QString::fromUtf8("ltr_gui"));
    QString key = QString::fromUtf8("HotKeys/xkey_%1").arg(shortcutId->getHotKeyId());
    QVariant v = settings.value(key);
    if(v.isValid() && v.toInt() > 0){
      code = static_cast<KeyCode>(v.toInt());
      HKDBG("DEBUG: Using saved keycode %u for hotkey %d\n", code, shortcutId->getHotKeyId());
    }
  }

  /* Fall back to KeySym translation if no saved keycode. */
  if(code == 0){
    if(!translateSequence(s, code, modifiers)){
      return false;
    }
  }else{
    /* Extract modifiers from the sequence without overwriting the saved code.
     * translateSequence writes both code and modifiers; discard the former. */
    KeyCode dummy;
    translateSequence(s, dummy, modifiers);
  }
  //verify the hotkey doesn't exist already
  keyPair_t kp(code, modifiers);
  shortcutHash_t::iterator i = shortcutHash.find(kp);
  if(i != shortcutHash.end()){
    //printf("The hotkey is taken already!\n");
    return false;
  } 
  
  fprintf(stderr, "DEBUG: About to grab key: keycode=%u, modifiers=0x%x, window=0x%lx\n", 
          code, modifiers, window);
  if(!grabKeyX(display, window, code, modifiers)){
    fprintf(stderr, "DEBUG: ERROR - grabKeyX failed for keycode=%u, modifiers=0x%x\n", code, modifiers);
    //printf("Problem setting shortcut!\n");
    return false;
  }
  fprintf(stderr, "DEBUG: grabKeyX succeeded for keycode=%u, modifiers=0x%x\n", code, modifiers);

  //printf("Shortcut set!\n");
  bool needInstallFilter = shortcutHash.empty();
  shortcutHash.insert(std::pair<std::pair<KeyCode, unsigned int>, shortcut *>
    (keyPair_t(code, modifiers), shortcutId));
  fprintf(stderr, "DEBUG: Shortcut registered: keycode=%u, modifiers=0x%x, hash size=%zu\n", 
          code, modifiers, shortcutHash.size());
  if(needInstallFilter){
    fprintf(stderr, "DEBUG: Installing native event filter (first shortcut registered)\n");
    installFilter();
  }
  return true;
}

bool unsetShortcut(shortcut* id)
{
  keyPair_t kp;
  if(!removeIdFromHash(id, &kp)){
    return false;
  }
  KeyCode code = kp.first;
  unsigned int modifiers = kp.second;
  
  // Ungrab the key first while display is still valid
  bool result = ungrabKeyX(display, window, code, modifiers);
  
  // Close display only after ungrabKeyX() completes (when no shortcuts remain)
  if(shortcutHash.empty()){
    uninstallFilter();
  }
  return result;
}

static bool grabKeyX(Display *display, Window &window, KeyCode code, unsigned int modifiers)
{
  prev_x_errhandler = XSetErrorHandler(my_x_errhandler);
  //Flush...
  XSync(display, false);
  errorEncountered = false;
  errMsg.clear();

  // Qt6: Grab on root window - events grabbed on custom windows aren't delivered through native event filter
  // Fix: Always grab on root window for Qt6 compatibility
  Window grabWindow = window;  // Use root window, not eventWindow

  //No modifiers
  fprintf(stderr, "DEBUG: XGrabKey: code=%u, modifiers=0x%x, window=0x%lx (root), owner_events=true\n", code, modifiers, grabWindow);
  XGrabKey(display, code, modifiers, grabWindow, true, GrabModeAsync, GrabModeAsync);
  XSync(display, false);
  if(errorEncountered){
    fprintf(stderr, "DEBUG: XGrabKey ERROR for code=%u: %s\n", code, qPrintable(errMsg));
    // nothing to clean up...
    return false;
  }
  fprintf(stderr, "DEBUG: XGrabKey succeeded for code=%u, modifiers=0x%x\n", code, modifiers);

  // Also try with owner_events=false in case that helps in Qt6
  // (But don't fail if this errors - it's just an optimization attempt)
  errorEncountered = false;
  XGrabKey(display, code, modifiers, grabWindow, false, GrabModeAsync, GrabModeAsync);
  XSync(display, false);
  if(!errorEncountered){
    fprintf(stderr, "DEBUG: XGrabKey (owner_events=false) also succeeded for code=%u\n", code);
  }

  //Numlock
  XGrabKey(display, code, modifiers | Mod2Mask, window, true, GrabModeAsync, GrabModeAsync);
  XSync(display, false);
  if(errorEncountered){
    goto num_lock_failed;
  }
    
  //ScrollLock
  XGrabKey(display, code, modifiers | Mod5Mask, window, true, GrabModeAsync, GrabModeAsync);
  XSync(display, false);
  if(errorEncountered){
    goto scroll_lock_failed;
  }

  //NumLock + ScrollLock
  XGrabKey(display, code, modifiers | Mod2Mask | Mod5Mask, window, true, GrabModeAsync, GrabModeAsync);
  XSync(display, false);
  if(!errorEncountered){
    XSetErrorHandler(prev_x_errhandler);
    return true;
  }
  
  //Num+scroll lock failed, clean ScrollLock, num lock and no mod
  XUngrabKey(display, code, modifiers | Mod5Mask, window);
 scroll_lock_failed:
  //Scroll lock failed, clean up Numlock and no mod
  XUngrabKey(display, code, modifiers | Mod2Mask, window);
 num_lock_failed:
  //Num lock failed, clean up no mod
  XUngrabKey(display, code, modifiers, window);

  XSetErrorHandler(prev_x_errhandler);
  return false;
}

static bool ungrabKeyX(Display *display, Window &window, KeyCode code, unsigned int modifiers)
{
  prev_x_errhandler = XSetErrorHandler(my_x_errhandler);
  //Flush stuff, so we don't get false errors... Paranoia? ;)
  XSync(display, false);
  errorEncountered = false;

  // Fix: Ungrab from root window (same as grab)
  Window ungrabWindow = window;  // Use root window, not eventWindow

  XUngrabKey(display, code, modifiers, ungrabWindow);
  //Numlock
  XUngrabKey(display, code, modifiers | Mod2Mask, ungrabWindow);
  //ScrollLock
  XUngrabKey(display, code, modifiers | Mod5Mask, ungrabWindow);
  //NumLock + ScrollLock
  XUngrabKey(display, code, modifiers | Mod2Mask | Mod5Mask, ungrabWindow);
  XSync(display, false);
  XSetErrorHandler(prev_x_errhandler);
  return !errorEncountered;
}


