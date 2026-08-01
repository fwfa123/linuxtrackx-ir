#ifndef HOTKEY_DIALOG__H
#define HOTKEY_DIALOG__H

#include "ui_hotkey_setup.h"
#include <QString>
#include <QVector>

class QTimer;

class hotKeySetupDlg : public QDialog
{
  Q_OBJECT
 public:
  hotKeySetupDlg(QString &res, QWidget *parent = 0);
  ~hotKeySetupDlg() override;
 private slots:
  void pollJoysticks();
 private:
  void startJoyCapture();
  void stopJoyCapture();

  Ui::HotKeySetupDlg ui;
  QString *result;
  QTimer *joyTimer;
  void *joyLib;
  QVector<int> joyFds;
  QVector<QString> joyNames;
  void *readButtonsFun;
  void *closeFun;
};


#endif
