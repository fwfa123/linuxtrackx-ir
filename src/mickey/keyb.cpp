

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include "keyb.h"
// Must be before the keyb_x11.h otherwise a type mismatch occurs
//   (qmetatype must be included before any header file defining Bool,
//    which in this case is X11/Xlib.h)
#include "moc_keyb.cpp"

#ifndef DARWIN
  #include "keyb_x11.h"
#endif

shortcut::shortcut()
{
}

shortcut::~shortcut()
{
#ifndef DARWIN
  unsetShortcut(this);
#endif
}

bool shortcut::setShortcut(const QKeySequence &s)
{
  //printf("Setting shortcut!\n");
#ifdef DARWIN
  /* The historical implementation uses X11 grabs on Linux.  macOS needs a
   * native global-hotkey backend; do not silently claim that registration
   * succeeded until one is available. */
  (void)s;
  return false;
#else
  return setShortCut(s, this);
#endif
}

bool shortcut::resetShortcut()
{
#ifdef DARWIN
  return true;
#else
  return unsetShortcut(this);
#endif
}

void shortcut::activate(bool pressed)
{
  //printf("Firing shortcut!\n");
  emit activated(pressed);
}


