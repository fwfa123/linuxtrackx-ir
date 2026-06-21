#include "scp_form.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QShowEvent>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "ltr_gui_prefs.h"
#include "ltr_model.h"
#include "pose_crosshair_panel.h"
#include "pose_monitor_strip.h"
#include "tracker.h"

namespace {

axis_t axisForListIndex(int index)
{
  switch(index){
    case 0: return PITCH;
    case 1: return YAW;
    case 2: return ROLL;
    case 3: return TX;
    case 4: return TY;
    case 5: return TZ;
    default: return YAW;
  }
}

}

ScpForm::ScpForm(QWidget *parent)
  : QWidget(parent), poseMonitor(NULL), poseCrosshairs(NULL), axisList(NULL), curveStack(NULL),
    panTuningBox(NULL), panTuningLayout(NULL), panTuningHint(NULL),
    modelTweaker(NULL), recenterButton(NULL), yaw(NULL), pitch(NULL), roll(NULL),
    x(NULL), y(NULL), z(NULL)
{
  ui.setupUi(this);
  setMinimumSize(760, 620);
  resize(960, 1228);

  poseMonitor = new PoseMonitorStrip(this);
  ui.verticalLayout_2->insertWidget(0, poseMonitor);
  poseCrosshairs = new PoseCrosshairPanel(this);
  ui.verticalLayout_2->insertWidget(1, poseCrosshairs);

  curveStack = new QStackedWidget(this);
  pitch = new SCurve(PITCH, QString::fromUtf8("Pitch - looking down/up"), QString::fromUtf8("Down"),
                     QString::fromUtf8("Up"), curveStack);
  yaw = new SCurve(YAW, QString::fromUtf8("Yaw - looking left/right"), QString::fromUtf8("Right"),
                   QString::fromUtf8("Left"), curveStack);
  roll = new SCurve(ROLL, QString::fromUtf8("Roll - tilting head left/right"),
                    QString::fromUtf8("Clockwise"), QString::fromUtf8("Counter-clockwise"), curveStack);
  x = new SCurve(TX, QString::fromUtf8("Sideways translation"), QString::fromUtf8("Left"),
                 QString::fromUtf8("Right"), curveStack);
  y = new SCurve(TY, QString::fromUtf8("Up/down translation"), QString::fromUtf8("Down"),
                 QString::fromUtf8("Up"), curveStack);
  z = new SCurve(TZ, QString::fromUtf8("Back/forth translation"), QString::fromUtf8("Forth"),
                 QString::fromUtf8("Back"), curveStack);
  curveStack->addWidget(pitch);
  curveStack->addWidget(yaw);
  curveStack->addWidget(roll);
  curveStack->addWidget(x);
  curveStack->addWidget(y);
  curveStack->addWidget(z);

  axisList = new QListWidget(this);
  axisList->setMaximumWidth(132);
  axisList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  const QString axisLabels[6] = {
    tr("Pitch"), tr("Yaw"), tr("Roll"),
    tr("X Translation"), tr("Y Translation"), tr("Z Translation")
  };
  for(int i = 0; i < 6; ++i){
    axisList->addItem(axisLabels[i]);
  }

  QWidget *curveArea = new QWidget(this);
  QHBoxLayout *curveAreaLayout = new QHBoxLayout(curveArea);
  curveAreaLayout->setContentsMargins(0, 0, 0, 0);
  curveAreaLayout->addWidget(axisList);
  curveAreaLayout->addWidget(curveStack, 1);

  ui.verticalLayout->removeWidget(ui.SCPTab);
  ui.SCPTab->hide();
  ui.verticalLayout->addWidget(curveArea, 1);

  panTuningBox = new QGroupBox(tr("Pan tuning (model hc)"), this);
  panTuningBox->setCheckable(true);
  panTuningBox->setChecked(false);
  panTuningLayout = new QVBoxLayout(panTuningBox);
  ui.verticalLayout_2->insertWidget(3, panTuningBox);

  recenterButton = new QPushButton(tr("Recenter"), this);
  ui.horizontalLayout->insertWidget(0, recenterButton);

  ui.verticalLayout_2->setStretch(2, 1);
  ui.verticalLayout_2->setStretch(3, 0);

  QObject::connect(axisList, SIGNAL(currentRowChanged(int)), this, SLOT(on_AxisList_currentRowChanged(int)));
  QObject::connect(recenterButton, SIGNAL(pressed()), this, SLOT(on_SCPRecenterButton_pressed()));
  QObject::connect(ModelTuningSync::inst(), SIGNAL(headCenterChanged(const QString &)),
    this, SLOT(onHeadCenterChanged(const QString &)));

  axisList->setCurrentRow(1);
  on_AxisList_currentRowChanged(axisList->currentRow());
}

ScpForm::~ScpForm()
{
}

void ScpForm::showEvent(QShowEvent *event)
{
  QWidget::showEvent(event);
  refreshPanTuning();
}

void ScpForm::refreshPanTuning()
{
  if(panTuningBox == NULL || panTuningLayout == NULL){
    return;
  }

  panTuningSection.clear();

  if(modelTweaker != NULL){
    panTuningLayout->removeWidget(modelTweaker);
    delete modelTweaker;
    modelTweaker = NULL;
  }
  if(panTuningHint != NULL){
    panTuningLayout->removeWidget(panTuningHint);
    delete panTuningHint;
    panTuningHint = NULL;
  }

  QString section;
  if(!PREF.getActiveModel(section)){
    panTuningHint = new QLabel(tr("No active model — configure Model Setup first."), panTuningBox);
    panTuningLayout->addWidget(panTuningHint);
    return;
  }

  QString type;
  if(!PREF.getKeyVal(section, QString::fromUtf8("Model-type"), type)){
    panTuningHint = new QLabel(tr("Could not read model type for the active model."), panTuningBox);
    panTuningLayout->addWidget(panTuningHint);
    return;
  }

  panTuningSection = section;

  if(type.compare(QString::fromUtf8("Clip"), Qt::CaseInsensitive) == 0){
    modelTweaker = new ClipTweaking(section, panTuningBox);
    panTuningLayout->addWidget(modelTweaker);
    return;
  }
  if(type.compare(QString::fromUtf8("Cap"), Qt::CaseInsensitive) == 0){
    modelTweaker = new CapTweaking(section, panTuningBox);
    panTuningLayout->addWidget(modelTweaker);
    return;
  }

  panTuningHint = new QLabel(
      tr("Pan tuning hc sliders are available for 3-point Clip or Cap models only."), panTuningBox);
  panTuningHint->setWordWrap(true);
  panTuningLayout->addWidget(panTuningHint);
}

void ScpForm::reloadPanTuningSliders()
{
  if(modelTweaker == NULL || panTuningSection.isEmpty()){
    return;
  }
  ClipTweaking *clip = qobject_cast<ClipTweaking *>(modelTweaker);
  if(clip != NULL){
    clip->reloadFromPrefs();
    return;
  }
  CapTweaking *cap = qobject_cast<CapTweaking *>(modelTweaker);
  if(cap != NULL){
    cap->reloadFromPrefs();
  }
}

void ScpForm::onHeadCenterChanged(const QString &section)
{
  if(section != panTuningSection){
    return;
  }
  reloadPanTuningSliders();
}

void ScpForm::on_AxisList_currentRowChanged(int row)
{
  if(row < 0 || curveStack == NULL){
    return;
  }
  curveStack->setCurrentIndex(row);
  if(poseMonitor != NULL){
    poseMonitor->setActiveAxis(axisForListIndex(row));
  }
}

void ScpForm::on_SCPRecenterButton_pressed()
{
  TRACKER.recenter();
}

void ScpForm::on_SCPCloseButton_pressed()
{
  close();
}
