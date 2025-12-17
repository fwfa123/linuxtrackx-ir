#ifndef KEYB_X11__h
#define KEYB_X11__h

// Include Qt headers first to avoid macro conflicts with X11
#include <QKeySequence>
#include <QAbstractEventDispatcher>
#include <QMutex>
#include <map>
#include <QSocketNotifier>
#include <QTimer>

// QX11Info was removed in Qt6 - use X11 APIs directly
// Include X11 after Qt to avoid macro conflicts
#include <X11/Xlib.h>

#if defined(QT5_OVERRIDES) || defined(QT6_OVERRIDES)
  #include <QAbstractNativeEventFilter>
#endif

class shortcut;

typedef std::pair<KeyCode, unsigned int> keyPair_t;
typedef std::map<keyPair_t, shortcut*> shortcutHash_t;

bool setShortCut(const QKeySequence &s, shortcut* id);
bool unsetShortcut(shortcut* id);

#if defined(QT5_OVERRIDES) || defined(QT6_OVERRIDES)
class hotKeyFilter : public QAbstractNativeEventFilter
{
 protected:
  bool nativeEventFilter(const QByteArray & eventType, void * message, qintptr * result); 
};
#endif
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

