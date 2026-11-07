#ifndef SCURVE__H
#define SCURVE__H

#include <QWidget>
#include <QString>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>

#include "ui_scurve.h"
#include "scview.h"
#include <axis.h>

class SCurve : public QWidget{
  Q_OBJECT
 public:
  SCurve(axis_t a, QString axis_name, QString left, QString right, QWidget *parent = 0);
  ~SCurve();
 signals:
  void changed();
  void symetryChanged(bool symetrical);
 private slots:
  void on_SCSymetrical_stateChanged(int state);
  void on_SCEnable_stateChanged(int state);
  void on_SCInvert_stateChanged(int state);
  void on_SCFactor_valueChanged(int value);
  void on_SCFilterSlider_valueChanged(int value);
  void on_SCLeftLimit_valueChanged(double d);
  void on_SCRightLimit_valueChanged(double d);
  void axisChanged(int a, int elem);
  void initAxes();
  void onPointSelectionChanged(bool right, int index, float x, float y,
                               bool removable, bool hasSelection);
  void onPointCoordEdited();
  void onRemovePointClicked();
 private:
  void setup_gui();
  void setupPointEditor();
  void syncEnableInvertFromTracker();
  axis_t axis;
  Ui::SCurveForm ui;
  bool symetrical;
  SCView *view;
  bool initializing;
  bool updatingPointEditor;

  QDoubleSpinBox *pointInputSpin;
  QDoubleSpinBox *pointOutputSpin;
  QPushButton *removePointButton;
  QLabel *pointHintLabel;

  void setFilter(float val, bool signal = false);
  void mirrorPointsFromLeft();
  void syncSymmetryFromTracker();
};

#endif
