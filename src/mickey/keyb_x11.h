#ifndef KEYB_X11__h
#define KEYB_X11__h

// Qt headers only — no X11 includes here.
// Display and Window are forward-declared below so this header never pulls in Xlib.h,
// which avoids the '#define Status int' macro conflict with Qt6.
#include <QKeySequence>
#include <QAbstractEventDispatcher>
#include <QMutex>
#include <map>
#include <QSocketNotifier>
#include <QTimer>

class shortcut;

/* Use unsigned int instead of X11 KeyCode so this header doesn't need
 * Xlib/Xproto.  The actual XGrabKey calls cast to KeyCode where needed. */
typedef std::pair<unsigned int, unsigned int> keyPair_t;
typedef std::map<keyPair_t, shortcut*> shortcutHash_t;

bool setShortCut(const QKeySequence &s, shortcut* id);
bool unsetShortcut(shortcut* id);


/*
class shortcutPimpl : public QObject
{
  Q_OBJECT
  public:
   ~shortcutPimpl();
   bool setShortcut(const QKeySequence &s, shortcut* id);
   bool unsetShortcut(shortcut* id);
   //void activate(int id){emit activated(id);};
   static shortcutPimpl *createShortcutObject();
  signals:
   void activated(int);
  private:
   shortcutPimpl();
   static void installFilter();
   static void uninstallFilter();
   static QAbstractEventDispatcher::EventFilter prevFilter;
   static bool eventFilter(void *message);
   static shortcutHash_t shortcutHash;
   static int my_x_errhandler(Display* display, XErrorEvent *event);
   static bool grabKeyX(Display *display, Window &window, KeyCode code, unsigned int modifiers);
   static bool ungrabKeyX(Display *display, Window &window, KeyCode code, unsigned int modifiers);
   static bool removeIdFromHash(shortcut* shortcutId, keyPair_t *kp = NULL);
   static bool translateSequence(const QKeySequence &s, KeyCode &code, unsigned int &modifiers);
};
*/

#endif

