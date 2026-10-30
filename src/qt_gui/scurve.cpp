#include "scurve.h"
#include "pref_global.h"
#include "ltr_gui_prefs.h"
#include "ltr_gui.h"
#include "ltr_profiles.h"
#include "tracker.h"
#include <math.h>
#include <QSignalBlocker>

SCurve::SCurve(axis_t a, QString axis_name, QString left_label, QString right_label, QWidget *parent)
  : QWidget(parent), axis(a), symetrical(true), view(NULL), initializing(false)
{
  symetrical = TRACKER.axisIsSymetrical(axis);
  ui.setupUi(this);
  ui.SCTitle->setText(axis_name);
  ui.SCLeftLabel->setText(left_label);
  ui.SCRightLabel->setText(right_label);
  QObject::connect(&TRACKER, SIGNAL(axisChanged(int, int)),
                    this, SLOT(axisChanged(int, int)));
  QObject::connect(&TRACKER, SIGNAL(initAxes(void)),
                    this, SLOT(initAxes(void)));

  view = new SCView(axis, this);
  ui.SCView->addWidget(view, 1);
  ui.horizontalLayout_2->setStretch(0, 1);
  ui.horizontalLayout_2->setStretch(1, 0);
  ui.verticalLayout_4->setStretch(0, 0);
  ui.verticalLayout_4->setStretch(1, 1);
  ui.verticalLayout_4->setStretch(2, 0);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  QObject::connect(this, SIGNAL(changed()), view, SLOT(update()));
  initAxes();
}

SCurve::~SCurve()
{
  delete view;
}

void SCurve::syncSymmetryFromTracker()
{
  symetrical = TRACKER.axisIsSymetrical(axis);
  {
    QSignalBlocker block(ui.SCSymetrical);
    ui.SCSymetrical->setCheckState(symetrical ? Qt::Checked : Qt::Unchecked);
  }
  ui.SCRightLimit->setDisabled(symetrical);
  if(view != NULL){
    view->setSymmetrical(symetrical);
  }
}

void SCurve::setup_gui()
{
  syncSymmetryFromTracker();
  ui.SCFactor->setValue(TRACKER.axisGet(axis, AXIS_MULT) * 12);
  setFilter(TRACKER.axisGet(axis, AXIS_FILTER));
  ui.SCRightLimit->setValue(TRACKER.axisGet(axis, AXIS_RLIMIT));
  ui.SCLeftLimit->setValue(TRACKER.axisGet(axis, AXIS_LLIMIT));
}

void SCurve::setEnabled(int state)
{
  if(!initializing){
    TRACKER.axisChange(axis, AXIS_ENABLED, state == Qt::Checked);
  }
}

void SCurve::on_SCSymetrical_stateChanged(int state)
{
  switch(state){
    case Qt::Checked:
      symetrical = true;
      ui.SCRightLimit->setDisabled(true);
      ui.SCRightLimit->setValue(ui.SCLeftLimit->value());
      if(!initializing){
        mirrorPointsFromLeft();
      }
      break;
    case Qt::Unchecked:
      symetrical = false;
      ui.SCRightLimit->setDisabled(false);
      break;
    default:
      break;
  }
  if(view != NULL){
    view->setSymmetrical(symetrical);
  }
}

void SCurve::on_SCFactor_valueChanged(int value)
{
  if(!initializing) TRACKER.axisChange(axis, AXIS_MULT, value / 12.0f);
  emit changed();
}

void SCurve::setFilter(float val, bool signal)
{
  if(!signal){
    ui.SCFilterSlider->setValue(round(val * 100.0));
  }
}

void SCurve::on_SCFilterSlider_valueChanged(int value)
{
  if(!initializing) TRACKER.axisChange(axis, AXIS_FILTER, value / 100.0f);
  setFilter(value/100.0, true);
  emit changed();
}

void SCurve::mirrorPointsFromLeft()
{
  ltr_curve_pt pts[LTR_CURVE_MAX_POINTS];
  const int count = TRACKER.axisGetCurve(axis, false, pts, LTR_CURVE_MAX_POINTS);
  if(count >= 2){
    TRACKER.axisSetCurve(axis, pts, count, pts, count);
  }
}

void SCurve::on_SCLeftLimit_valueChanged(double d)
{
  if(!initializing) TRACKER.axisChange(axis, AXIS_LLIMIT, (float)d);
  if(symetrical){
    ui.SCRightLimit->setValue(d);
  }
  emit changed();
}

void SCurve::on_SCRightLimit_valueChanged(double d)
{
  if(!initializing) TRACKER.axisChange(axis, AXIS_RLIMIT, (float)d);
  if(symetrical){
    ui.SCLeftLimit->setValue(d);
  }
  emit changed();
}

void SCurve::axisChanged(int a, int elem)
{
  if(a != axis){
    return;
  }
  switch(elem){
    case AXIS_MULT:
      ui.SCFactor->setValue(TRACKER.axisGet(axis, AXIS_MULT) * 12.0);
      break;
    case AXIS_FILTER:
      setFilter(TRACKER.axisGet(axis, AXIS_FILTER));
      break;
    case AXIS_LLIMIT:
      ui.SCLeftLimit->setValue(TRACKER.axisGet(axis, AXIS_LLIMIT));
      syncSymmetryFromTracker();
      break;
    case AXIS_RLIMIT:
      ui.SCRightLimit->setValue(TRACKER.axisGet(axis, AXIS_RLIMIT));
      syncSymmetryFromTracker();
      break;
    case AXIS_CURVE:
    case AXIS_ENABLED:
      syncSymmetryFromTracker();
      break;
    case AXIS_FULL:
      ui.SCFactor->setValue(TRACKER.axisGet(axis, AXIS_MULT) * 12.0);
      setFilter(TRACKER.axisGet(axis, AXIS_FILTER));
      ui.SCLeftLimit->setValue(TRACKER.axisGet(axis, AXIS_LLIMIT));
      ui.SCRightLimit->setValue(TRACKER.axisGet(axis, AXIS_RLIMIT));
      syncSymmetryFromTracker();
      break;
    default:
      break;
  }
  emit changed();
}

void SCurve::initAxes()
{
  initializing = true;
  setup_gui();
  initializing = false;
}
