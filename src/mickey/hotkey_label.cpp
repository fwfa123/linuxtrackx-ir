#include "hotkey_label.h"

#include <QKeySequence>

QString hotkeyDisplayString(const QString &stored)
{
  if(stored.isEmpty() ||
     stored.compare(QString::fromUtf8("None"), Qt::CaseInsensitive) == 0){
    return stored;
  }

  const QKeySequence seq(stored);
  if(seq.isEmpty()){
    return stored;
  }

  const QKeyCombination combo = seq[0];
  const Qt::KeyboardModifiers mods = combo.keyboardModifiers();
  if(!(mods & Qt::KeypadModifier)){
    return seq.toString(QKeySequence::NativeText);
  }

  QString out;
  if(mods & Qt::ControlModifier){
    out += QString::fromUtf8("Ctrl+");
  }
  if(mods & Qt::AltModifier){
    out += QString::fromUtf8("Alt+");
  }
  if(mods & Qt::ShiftModifier){
    out += QString::fromUtf8("Shift+");
  }
  if(mods & Qt::MetaModifier){
    out += QString::fromUtf8("Meta+");
  }

  out += QString::fromUtf8("NumPad ");
  const QKeySequence keyOnly(combo.key());
  out += keyOnly.toString(QKeySequence::NativeText);
  return out;
}
