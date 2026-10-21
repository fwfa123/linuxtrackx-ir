#include <QMessageBox>
#include <QSizePolicy>
#include <iostream>
#include "hotkey.h"
#include "hotkey_setup_dlg.h"
#include "hotkey_label.h"

#ifdef LTR_GUI_JOY_HOTKEYS
#include "../qt_gui/joy_hotkey_monitor.h"
#endif

HotKey::HotKey(const QString &iLabel, const QString &iPrefId, int iHotKeyId, QWidget *parent) 
           : QWidget(parent), label(iLabel), prefId(iPrefId), hotKeyId(iHotKeyId), hotKey(QString::fromUtf8("None"))
{
  ui.setupUi(this);
  ui.AssignButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  ui.AssignButton->setToolTip(QString());
  s = new shortcut();
  s->setHotKeyId(hotKeyId);   // so setShortCut can look up the saved X keycode
  QObject::connect(s, SIGNAL(activated(bool)), this, SLOT(shortcutActivated(bool)));
}

void HotKey::shortcutActivated(bool pressed)
{
  emit activated(hotKeyId, pressed);
}

bool HotKey::setHotKey(const QString &newHK)
{
#ifdef LTR_GUI_JOY_HOTKEYS
  if(JoyHotkey::isJoyBinding(newHK)){
    s->resetShortcut();
    emit newHotKey(prefId, newHK);
    ui.AssignButton->setText(label + QString::fromUtf8(" ") + JoyHotkey::displayName(newHK));
    ui.AssignButton->setToolTip(JoyHotkey::displayTooltip(newHK));
    hotKey = newHK;
    return true;
  }
#endif

  if(newHK.compare(QString::fromUtf8("None"), Qt::CaseInsensitive) != 0){
    QKeySequence sequence(newHK);
    if(!s->setShortcut(sequence)){
      QMessageBox::warning(this, QString::fromUtf8("Hotkey not usable."),
        QString::fromUtf8("Couldn't register new hotkey!"));
      return false;
    }
  }else{
    s->resetShortcut();
  }
  emit newHotKey(prefId, newHK);
  ui.AssignButton->setText(label + QString::fromUtf8(" ") + hotkeyDisplayString(newHK));
  ui.AssignButton->setToolTip(QString());
  hotKey = newHK;
  return true;
}

void HotKey::getHotKey(QString &hk) const
{
  hk = hotKey;
}


void HotKey::on_AssignButton_pressed()
{
  // Save the current binding so we can restore it if user cancels or gets nothing.
  QString savedHotKey = hotKey;

  // Temporarily ungrab this widget's keys from X11 before opening the dialog.
  // This is critical: while the dialog is open, any key that was previously
  // grabbed by XGrabKey would be intercepted and never reach myLineEdit.
  s->resetShortcut();

  QString newHK;
  int res;
  hotKeySetupDlg *dlg = new hotKeySetupDlg(newHK, this);
  dlg->setHotKeyId(hotKeyId);   // so myLineEdit persists the right QSettings key
  res = dlg->exec();
  delete dlg;

  if(res == QDialog::Accepted && !newHK.isEmpty()){
    // User picked a new key — re-grab it.
    setHotKey(newHK);
  }else{
    // User cancelled or entered nothing — restore the previous binding.
    if(!savedHotKey.isEmpty() && savedHotKey != QString::fromUtf8("None")){
      s->resetShortcut();                    // clean slate (in case partial state)
      setHotKey(savedHotKey);                // re-grab what was there before
    }else{
      hotKey = QString::fromUtf8("None");   // truly no binding to restore
    }
  }
}

#include "moc_hotkey.cpp"
