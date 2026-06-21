#ifndef SCP_FORM__H
#define SCP_FORM__H

#include <QWidget>
#include <QShowEvent>
#include "ui_scp_form.h"
#include "scurve.h"

class PoseMonitorStrip;
class PoseCrosshairPanel;
class QListWidget;
class QStackedWidget;
class QGroupBox;
class QVBoxLayout;
class QLabel;
class QPushButton;

class ScpForm : public QWidget
{
  Q_OBJECT
 public:
  ScpForm(QWidget *parent = 0);
  ~ScpForm();

 protected:
  void showEvent(QShowEvent *event);

 private slots:
  void on_SCPCloseButton_pressed();
  void on_SCPRecenterButton_pressed();
  void on_AxisList_currentRowChanged(int row);
  void onHeadCenterChanged(const QString &section);

 private:
  void refreshPanTuning();
  void reloadPanTuningSliders();

  Ui::SCPForm ui;
  PoseMonitorStrip *poseMonitor;
  PoseCrosshairPanel *poseCrosshairs;
  QListWidget *axisList;
  QStackedWidget *curveStack;
  QGroupBox *panTuningBox;
  QVBoxLayout *panTuningLayout;
  QLabel *panTuningHint;
  QWidget *modelTweaker;
  QString panTuningSection;
  QPushButton *recenterButton;
  SCurve *yaw, *pitch, *roll;
  SCurve *x, *y, *z;
};


#endif
