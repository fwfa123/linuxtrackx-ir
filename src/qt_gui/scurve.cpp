#include "scurve.h"
#include "pref_global.h"
#include "ltr_gui_prefs.h"
#include "ltr_gui.h"
#include "ltr_profiles.h"
#include "tracker.h"
#include <math.h>
#include <QSignalBlocker>
#include <QHBoxLayout>
#include <QFormLayout>

SCurve::SCurve(axis_t a, QString axis_name, QString left_label, QString right_label, QWidget *parent)
  : QWidget(parent), axis(a), symetrical(true), view(NULL), initializing(false),
    updatingPointEditor(false), pointInputSpin(NULL), pointOutputSpin(NULL),
    removePointButton(NULL), pointHintLabel(NULL)
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
  setupPointEditor();
  ui.horizontalLayout_2->setStretch(0, 1);
  ui.horizontalLayout_2->setStretch(1, 0);
  ui.verticalLayout_4->setStretch(0, 0);
  ui.verticalLayout_4->setStretch(1, 1);
  ui.verticalLayout_4->setStretch(2, 0);
  ui.verticalLayout_4->setStretch(3, 0);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  QObject::connect(this, SIGNAL(changed()), view, SLOT(update()));
  QObject::connect(view, SIGNAL(pointSelectionChanged(bool,int,float,float,bool,bool)),
                   this, SLOT(onPointSelectionChanged(bool,int,float,float,bool,bool)));
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

void SCurve::syncEnableInvertFromTracker()
{
  {
    QSignalBlocker block(ui.SCEnable);
    ui.SCEnable->setCheckState(TRACKER.axisGetBool(axis, AXIS_ENABLED) ? Qt::Checked : Qt::Unchecked);
  }
  {
    QSignalBlocker block(ui.SCInvert);
    ui.SCInvert->setCheckState(TRACKER.axisGetBool(axis, AXIS_INVERTED) ? Qt::Checked : Qt::Unchecked);
  }
}

void SCurve::setup_gui()
{
  syncSymmetryFromTracker();
  syncEnableInvertFromTracker();
  ui.SCFactor->setValue(TRACKER.axisGet(axis, AXIS_MULT) * 12);
  setFilter(TRACKER.axisGet(axis, AXIS_FILTER));
  ui.SCRightLimit->setValue(TRACKER.axisGet(axis, AXIS_RLIMIT));
  ui.SCLeftLimit->setValue(TRACKER.axisGet(axis, AXIS_LLIMIT));
}

void SCurve::on_SCEnable_stateChanged(int state)
{
  if(!initializing){
    TRACKER.axisChange(axis, AXIS_ENABLED, state == Qt::Checked);
  }
}

void SCurve::on_SCInvert_stateChanged(int state)
{
  if(!initializing){
    TRACKER.axisChange(axis, AXIS_INVERTED, state == Qt::Checked);
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
  if(!initializing){
    TRACKER.axisChange(axis, AXIS_LLIMIT, (float)d);
    if(symetrical){
      TRACKER.axisChange(axis, AXIS_RLIMIT, (float)d);
      QSignalBlocker block(ui.SCRightLimit);
      ui.SCRightLimit->setValue(d);
    }
  }else if(symetrical){
    QSignalBlocker block(ui.SCRightLimit);
    ui.SCRightLimit->setValue(d);
  }
  emit changed();
}

void SCurve::on_SCRightLimit_valueChanged(double d)
{
  if(!initializing){
    TRACKER.axisChange(axis, AXIS_RLIMIT, (float)d);
    if(symetrical){
      TRACKER.axisChange(axis, AXIS_LLIMIT, (float)d);
      QSignalBlocker block(ui.SCLeftLimit);
      ui.SCLeftLimit->setValue(d);
    }
  }else if(symetrical){
    QSignalBlocker block(ui.SCLeftLimit);
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
      {
        QSignalBlocker block(ui.SCLeftLimit);
        ui.SCLeftLimit->setValue(TRACKER.axisGet(axis, AXIS_LLIMIT));
      }
      break;
    case AXIS_RLIMIT:
      {
        QSignalBlocker block(ui.SCRightLimit);
        ui.SCRightLimit->setValue(TRACKER.axisGet(axis, AXIS_RLIMIT));
      }
      break;
    case AXIS_ENABLED:
    case AXIS_INVERTED:
      syncEnableInvertFromTracker();
      break;
    case AXIS_CURVE:
      syncSymmetryFromTracker();
      if(view != NULL){
        view->refreshSelectionEmit();
      }
      break;
    case AXIS_FULL:
      ui.SCFactor->setValue(TRACKER.axisGet(axis, AXIS_MULT) * 12.0);
      setFilter(TRACKER.axisGet(axis, AXIS_FILTER));
      ui.SCLeftLimit->setValue(TRACKER.axisGet(axis, AXIS_LLIMIT));
      ui.SCRightLimit->setValue(TRACKER.axisGet(axis, AXIS_RLIMIT));
      syncSymmetryFromTracker();
      syncEnableInvertFromTracker();
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

void SCurve::setupPointEditor()
{
  QWidget *row = new QWidget(this);
  QHBoxLayout *layout = new QHBoxLayout(row);
  layout->setContentsMargins(0, 4, 0, 4);

  pointHintLabel = new QLabel(
    tr("Select a point to edit. Right-click or Delete removes interior points."), row);
  pointHintLabel->setWordWrap(true);

  pointInputSpin = new QDoubleSpinBox(row);
  pointInputSpin->setDecimals(4);
  pointInputSpin->setRange(0.0, 1.0);
  pointInputSpin->setSingleStep(0.01);
  pointInputSpin->setPrefix(tr("In "));
  pointInputSpin->setEnabled(false);

  pointOutputSpin = new QDoubleSpinBox(row);
  pointOutputSpin->setDecimals(4);
  pointOutputSpin->setRange(0.0, 1.0);
  pointOutputSpin->setSingleStep(0.01);
  pointOutputSpin->setPrefix(tr("Out "));
  pointOutputSpin->setEnabled(false);

  removePointButton = new QPushButton(tr("Remove point"), row);
  removePointButton->setEnabled(false);

  layout->addWidget(pointHintLabel, 1);
  layout->addWidget(pointInputSpin, 0);
  layout->addWidget(pointOutputSpin, 0);
  layout->addWidget(removePointButton, 0);

  /* Insert above Sensitivity/Smoothing (frame_3), after the curve+side panel row. */
  ui.verticalLayout_4->insertWidget(2, row);

  QObject::connect(pointInputSpin, SIGNAL(valueChanged(double)),
                   this, SLOT(onPointCoordEdited()));
  QObject::connect(pointOutputSpin, SIGNAL(valueChanged(double)),
                   this, SLOT(onPointCoordEdited()));
  QObject::connect(removePointButton, SIGNAL(clicked()),
                   this, SLOT(onRemovePointClicked()));
}

void SCurve::onPointSelectionChanged(bool right, int index, float x, float y,
                                     bool removable, bool hasSelection)
{
  (void)right;
  (void)index;
  updatingPointEditor = true;
  if(pointInputSpin != NULL){
    QSignalBlocker b1(pointInputSpin);
    QSignalBlocker b2(pointOutputSpin);
    pointInputSpin->setValue(x);
    pointOutputSpin->setValue(y);
    /* Endpoints are display-only; interior points are editable. */
    const bool editable = hasSelection && removable;
    pointInputSpin->setEnabled(editable);
    pointOutputSpin->setEnabled(editable);
  }
  if(removePointButton != NULL){
    removePointButton->setEnabled(removable);
  }
  updatingPointEditor = false;
}

void SCurve::onPointCoordEdited()
{
  if(updatingPointEditor || initializing || view == NULL){
    return;
  }
  if(!view->selectionIsRemovable()){
    return;
  }
  view->setSelectedPointCoords(static_cast<float>(pointInputSpin->value()),
                               static_cast<float>(pointOutputSpin->value()));
  emit changed();
}

void SCurve::onRemovePointClicked()
{
  if(view == NULL){
    return;
  }
  view->removeSelectedPoint();
  emit changed();
}
