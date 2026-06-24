#ifndef DEVICE_SETUP__H
#define DEVICE_SETUP__H

#include <QWidget>
#include <ui_device_setup.h>

class Guardian;

class DeviceSetup : public QWidget
{
  Q_OBJECT
 public:
  DeviceSetup(Guardian *grd, QBoxLayout *tgt, QWidget *parent = 0);
  ~DeviceSetup();
  void refresh();
 private:
  Ui::DeviceSetupForm ui;
  static QString descs[8];
  static int orientValues[8];
  QWidget *devPrefs;
  bool initialized;
  void initOrientations();
  void initVideoOnDelay();
  void initUsbReset();
  QBoxLayout *target;
 private slots:
  void on_DeviceSelector_activated(int index);
  void on_CameraOrientation_activated(int index);
  void on_RefreshDevices_pressed();
  void on_VideoOnDelayMode_activated(int index);
  void on_VideoOnDelayValue_valueChanged(int value);
  void on_UsbResetCheck_toggled(bool checked);
 signals:
  void deviceTypeChanged(int deviceType, const QString &desc);
};

#endif

