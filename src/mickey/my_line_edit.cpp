#include "my_line_edit.h"
#include "hotkey_label.h"
#include <iostream>
#include <QKeyEvent>
#include <QKeySequence>
#include <QSettings>

myLineEdit::myLineEdit(QWidget *parent) : QLineEdit(parent), tgt(NULL), m_hotKeyId(-1)
{
}

void myLineEdit::keyPressEvent(QKeyEvent * event)
{
  const int k = event->key();
  switch(k){
    case 0:
    case Qt::Key_unknown:
    case Qt::Key_Shift:
    case Qt::Key_Control:
    case Qt::Key_Alt:
    case Qt::Key_Meta:
      return;
    default:
      break;
  }

  Qt::KeyboardModifiers mods = event->modifiers() &
    (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier |
     Qt::MetaModifier | Qt::KeypadModifier);

  const QKeySequence seq(QKeyCombination(mods, static_cast<Qt::Key>(k)));
  /* PortableText keeps KeypadModifier as Num+ so registration can map KP_* */
  const QString stored = seq.toString(QKeySequence::PortableText);
  setText(hotkeyDisplayString(stored));
  if(tgt != NULL){
    *tgt = stored;
  }

  /* Persist the native X keycode (scan code) under xcb so we can grab it
   * directly later, avoiding XKeysymToKeycode() ambiguity on external pads. */
  if(m_hotKeyId >= 0 && event->nativeScanCode() > 0){
    QSettings settings(QString::fromUtf8("linuxtrack"), QString::fromUtf8("ltr_gui"));
    QString key = QString::fromUtf8("HotKeys/xkey_%1").arg(m_hotKeyId);
    settings.setValue(key, event->nativeScanCode());
  }

}

#include "moc_my_line_edit.cpp"
