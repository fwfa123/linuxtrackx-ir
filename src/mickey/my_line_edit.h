#ifndef TEST__H
#define TEST__H

#include <QLineEdit>

class myLineEdit : public QLineEdit
{
  Q_OBJECT
 public:
  myLineEdit(QWidget *parent = 0);
  void setTargetString(QString &res){tgt = &res;};
  /* Identify which hotkey this capture belongs to, so the native scan code
   * can be persisted with the right QSettings key (e.g. quick_recenter_xkeycode). */
  void setHotKeyId(int id){ m_hotKeyId = id; }
 private:
  virtual void keyPressEvent(QKeyEvent * event);
  QString *tgt;
  int       m_hotKeyId;
};



#endif
