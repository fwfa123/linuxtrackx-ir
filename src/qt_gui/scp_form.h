#ifndef SCP_FORM__H
#define SCP_FORM__H

#include <QWidget>
#include <QShowEvent>
#include "ui_scp_form.h"
#include "scurve.h"

class ScpForm : public QWidget
{
  Q_OBJECT
 public:
  ScpForm(QWidget *parent = 0);
  ~ScpForm();
  void setEmbedded(bool embedded);

 protected:
  void showEvent(QShowEvent *event);

 private slots:
  void on_SCPCloseButton_pressed();
  void on_SCPRecenterButton_pressed();
  void on_AxisList_currentRowChanged(int row);
  void on_showPoseMapsButton_clicked();
  void onPoseMapsExpansionChanged(bool expanded);

 private:
  void setupCurves();
  void setupCurveSplitter();
  void applyPoseMapsColumn(bool expanded);

  Ui::SCPForm ui;
  SCurve *yaw, *pitch, *roll;
  SCurve *x, *y, *z;
  bool embedded;
  int savedMapsColumnWidth;
};


#endif
