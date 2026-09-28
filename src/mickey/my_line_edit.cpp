
#include "my_line_edit.h"
#include "hotkey_label.h"
#include <iostream>
#include <QKeyEvent>
#include <QKeySequence>

myLineEdit::myLineEdit(QWidget *parent) : QLineEdit(parent), tgt(NULL)
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
}

#include "moc_my_line_edit.cpp"
