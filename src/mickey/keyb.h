#ifndef KEYB__H
#define KEYB__H

#include <QObject>
#include <QKeySequence>

class shortcut : public QObject
{
  Q_OBJECT
 public:
  shortcut();
  ~shortcut();
  bool setShortcut(const QKeySequence &s);
  bool resetShortcut();
  void activate(bool pressed);
  /* Identify which hotkey this is, so the saved X keycode can be looked up. */
  void setHotKeyId(int id){ m_hotKeyId = id; }
  int  getHotKeyId() const { return m_hotKeyId; }
 signals:
  void activated(bool pressed);
 private:
  int m_hotKeyId;
};

#endif
