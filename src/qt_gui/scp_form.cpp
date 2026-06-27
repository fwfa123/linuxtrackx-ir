#include "scp_form.h"

#include <QLabel>
#include <QShowEvent>

#include "ltr_gui_prefs.h"
#include "ltr_model.h"
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
  : QWidget(parent), panTuningHint(NULL), modelTweaker(NULL),
    yaw(NULL), pitch(NULL), roll(NULL), x(NULL), y(NULL), z(NULL)
{
  ui.setupUi(this);
  setupCurves();

  QObject::connect(ModelTuningSync::inst(), SIGNAL(headCenterChanged(const QString &)),
    this, SLOT(onHeadCenterChanged(const QString &)));

  on_AxisList_currentRowChanged(ui.AxisList->currentRow());
}

void ScpForm::setupCurves()
{
  pitch = new SCurve(PITCH, QString::fromUtf8("Pitch - looking down/up"), QString::fromUtf8("Down"),
                     QString::fromUtf8("Up"), ui.curvePagePitch);
  ui.curvePagePitchLayout->addWidget(pitch, 1);

  yaw = new SCurve(YAW, QString::fromUtf8("Yaw - looking left/right"), QString::fromUtf8("Right"),
                   QString::fromUtf8("Left"), ui.curvePageYaw);
  ui.curvePageYawLayout->addWidget(yaw, 1);

  roll = new SCurve(ROLL, QString::fromUtf8("Roll - tilting head left/right"),
                    QString::fromUtf8("Clockwise"), QString::fromUtf8("Counter-clockwise"), ui.curvePageRoll);
  ui.curvePageRollLayout->addWidget(roll, 1);

  x = new SCurve(TX, QString::fromUtf8("Sideways translation"), QString::fromUtf8("Left"),
                 QString::fromUtf8("Right"), ui.curvePageTx);
  ui.curvePageTxLayout->addWidget(x, 1);

  y = new SCurve(TY, QString::fromUtf8("Up/down translation"), QString::fromUtf8("Down"),
                 QString::fromUtf8("Up"), ui.curvePageTy);
  ui.curvePageTyLayout->addWidget(y, 1);

  z = new SCurve(TZ, QString::fromUtf8("Back/forth translation"), QString::fromUtf8("Forth"),
                 QString::fromUtf8("Back"), ui.curvePageTz);
  ui.curvePageTzLayout->addWidget(z, 1);
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
  panTuningSection.clear();

  if(modelTweaker != NULL){
    ui.panTuningLayout->removeWidget(modelTweaker);
    delete modelTweaker;
    modelTweaker = NULL;
  }
  if(panTuningHint != NULL){
    ui.panTuningLayout->removeWidget(panTuningHint);
    delete panTuningHint;
    panTuningHint = NULL;
  }

  QString section;
  if(!PREF.getActiveModel(section)){
    panTuningHint = new QLabel(tr("No active model — configure Model Setup first."), ui.panTuningBox);
    ui.panTuningLayout->addWidget(panTuningHint);
    return;
  }

  QString type;
  if(!PREF.getKeyVal(section, QString::fromUtf8("Model-type"), type)){
    panTuningHint = new QLabel(tr("Could not read model type for the active model."), ui.panTuningBox);
    ui.panTuningLayout->addWidget(panTuningHint);
    return;
  }

  panTuningSection = section;

  if(type.compare(QString::fromUtf8("Clip"), Qt::CaseInsensitive) == 0){
    modelTweaker = new ClipTweaking(section, ui.panTuningBox);
    ui.panTuningLayout->addWidget(modelTweaker);
    return;
  }
  if(type.compare(QString::fromUtf8("Cap"), Qt::CaseInsensitive) == 0){
    modelTweaker = new CapTweaking(section, ui.panTuningBox);
    ui.panTuningLayout->addWidget(modelTweaker);
    return;
  }

  panTuningHint = new QLabel(
      tr("Pan tuning head center sliders are available for 3-point Clip or Cap models only."), ui.panTuningBox);
  panTuningHint->setWordWrap(true);
  ui.panTuningLayout->addWidget(panTuningHint);
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
  if(row < 0){
    return;
  }
  ui.curveStack->setCurrentIndex(row);
  ui.poseMonitor->setActiveAxis(axisForListIndex(row));
}

void ScpForm::on_SCPRecenterButton_pressed()
{
  TRACKER.recenter();
}

void ScpForm::on_SCPCloseButton_pressed()
{
  close();
}
