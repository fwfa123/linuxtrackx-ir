

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include "keyb.h"
// Must be before the keyb_x11.h otherwise a type mismatch occurs
//   (qmetatype must be included before any header file defining Bool,
//    which in this case is X11/Xlib.h)
#include "moc_keyb.cpp"

#ifndef DARWIN
  /* keyb_x11.h uses KeyCode in its keyPair_t typedef, so Xlib/Xproto must be
   * included first. moc_keyb.cpp is already processed at this point,
   * so the Status macro won't conflict with Qt internals. */
  #include <X11/Xlib.h>
  #include <X11/Xproto.h>
  #include "keyb_x11.h"
#endif

shortcut::shortcut()
  : m_hotKeyId(-1)
{
}

shortcut::~shortcut()
{
  unsetShortcut(this);
}

bool shortcut::setShortcut(const QKeySequence &s)
{
  //printf("Setting shortcut!\n");
  return setShortCut(s, this);
}

bool shortcut::resetShortcut()
{
  return unsetShortcut(this);
}

void shortcut::activate(bool pressed)
{
  //printf("Firing shortcut!\n");
  emit activated(pressed);
}


