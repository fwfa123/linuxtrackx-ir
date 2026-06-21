#ifndef SCP_FORM__H
#define SCP_FORM__H

#include <QWidget>
#include <QShowEvent>
#include "ui_scp_form.h"
#include "scurve.h"

class QLabel;
class QWidget;

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
  void setupCurves();
  void refreshPanTuning();
  void reloadPanTuningSliders();

  Ui::SCPForm ui;
  QLabel *panTuningHint;
  QWidget *modelTweaker;
  QString panTuningSection;
  SCurve *yaw, *pitch, *roll;
  SCurve *x, *y, *z;
};


#endif
