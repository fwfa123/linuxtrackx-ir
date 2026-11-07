#include "pose_crosshair_panel.h"

#include <QGroupBox>
#include <QSettings>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <cmath>

#include "pose_crosshair_widget.h"
#include "tracker.h"

namespace {

float axisSpan(axis_t axis)
{
  const float mult = TRACKER.axisGet(axis, AXIS_MULT);
  if(mult == 0.f){
    return 1.f;
  }
  const float kl = TRACKER.axisGet(axis, AXIS_LLIMIT) / mult;
  const float kr = TRACKER.axisGet(axis, AXIS_RLIMIT) / mult;
  return std::max(kl, kr);
}

}

PoseCrosshairPanel::PoseCrosshairPanel(QWidget *parent)
  : QWidget(parent), box(NULL), contentHost(NULL), showMeasured(true)
{
  box = new QGroupBox(tr("Pose maps"), this);
  box->setCheckable(true);
  box->setFlat(false);

  contentHost = new QWidget(box);
  QVBoxLayout *boxLayout = new QVBoxLayout(contentHost);
  boxLayout->setContentsMargins(0, 0, 0, 0);

  sourceCombo = new QComboBox(contentHost);
  sourceCombo->addItem(tr("Measured (camera input)"));
  sourceCombo->addItem(tr("Filtered (game output)"));
  boxLayout->addWidget(sourceCombo);

  lookWidget = new PoseCrosshairWidget(contentHost);
  lookWidget->setTitle(tr("Look"));
  lookWidget->setEdgeLabels(tr("Left"), tr("Right"), tr("Up"), tr("Down"));
  lookWidget->setUnits(true, true);
  lookWidget->setInvertHorizontal(true);
  lookWidget->setDeadzoneStyle(PoseCrosshairWidget::CircleDeadzone);
  lookWidget->setMinimumHeight(170);

  leanWidget = new PoseCrosshairWidget(contentHost);
  leanWidget->setTitle(tr("Lean"));
  leanWidget->setEdgeLabels(tr("Left"), tr("Right"), tr("Forward"), tr("Back"));
  leanWidget->setUnits(false, false);
  leanWidget->setInvertVertical(true);
  leanWidget->setDeadzoneStyle(PoseCrosshairWidget::SquareDeadzone);
  leanWidget->setMinimumHeight(170);

  raiseWidget = new PoseCrosshairWidget(contentHost);
  raiseWidget->setTitle(tr("Raise/Roll"));
  raiseWidget->setEdgeLabels(tr("Roll L"), tr("Roll R"), tr("Up"), tr("Down"));
  raiseWidget->setUnits(true, false);
  raiseWidget->setInvertHorizontal(true);
  raiseWidget->setDeadzoneStyle(PoseCrosshairWidget::SquareDeadzone);
  raiseWidget->setMinimumHeight(170);

  QVBoxLayout *mapsLayout = new QVBoxLayout();
  mapsLayout->addWidget(lookWidget, 1);
  mapsLayout->addWidget(leanWidget, 1);
  mapsLayout->addWidget(raiseWidget, 1);
  boxLayout->addLayout(mapsLayout, 1);

  QVBoxLayout *groupLayout = new QVBoxLayout(box);
  groupLayout->addWidget(contentHost);

  QVBoxLayout *outer = new QVBoxLayout(this);
  outer->setContentsMargins(0, 0, 0, 0);
  outer->addWidget(box);

  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  setMinimumWidth(200);
  setMaximumWidth(280);

  QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
  const bool expanded = settings.value(QStringLiteral("curveEditor/poseMapsExpanded"), true).toBool();
  box->setChecked(expanded);
  applyExpanded(expanded);

  connect(box, SIGNAL(toggled(bool)), this, SLOT(onExpandedToggled(bool)));
  connect(sourceCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onSourceChanged(int)));
  connect(&TRACKER, SIGNAL(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)),
          this, SLOT(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)));
  connect(&TRACKER, SIGNAL(axisChanged(int, int)), this, SLOT(axisChanged(int, int)));
  connect(&TRACKER, SIGNAL(initAxes(void)), this, SLOT(initAxes()));

  updateRanges();
  updateDeadzones();
}

bool PoseCrosshairPanel::isExpanded() const
{
  return box != NULL && box->isChecked();
}

void PoseCrosshairPanel::setExpanded(bool expanded)
{
  if(box == NULL){
    return;
  }
  if(box->isChecked() == expanded){
    applyExpanded(expanded);
    return;
  }
  box->setChecked(expanded);
}

void PoseCrosshairPanel::applyExpanded(bool expanded)
{
  if(contentHost != NULL){
    contentHost->setVisible(expanded);
  }
}

void PoseCrosshairPanel::onExpandedToggled(bool expanded)
{
  applyExpanded(expanded);
  QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
  settings.setValue(QStringLiteral("curveEditor/poseMapsExpanded"), expanded);
  emit expansionChanged(expanded);
}

void PoseCrosshairPanel::onSourceChanged(int index)
{
  showMeasured = (index == 0);
}

void PoseCrosshairPanel::updateRanges()
{
  lookWidget->setRange(axisSpan(YAW), axisSpan(PITCH));
  leanWidget->setRange(axisSpan(TX), axisSpan(TZ));
  raiseWidget->setRange(axisSpan(ROLL), axisSpan(TY));
}

void PoseCrosshairPanel::updateDeadzones()
{
  lookWidget->setDeadzone(TRACKER.axisFlatPrefix(YAW),
                          TRACKER.axisFlatPrefix(PITCH));
  leanWidget->setDeadzone(TRACKER.axisFlatPrefix(TX),
                          TRACKER.axisFlatPrefix(TZ));
  raiseWidget->setDeadzone(TRACKER.axisFlatPrefix(ROLL),
                           TRACKER.axisFlatPrefix(TY));
}

void PoseCrosshairPanel::axisChanged(int axis, int elem)
{
  (void) axis;
  if(elem == AXIS_DEADZONE || elem == AXIS_CURVE || elem == AXIS_LLIMIT || elem == AXIS_RLIMIT
     || elem == AXIS_MULT){
    updateRanges();
    updateDeadzones();
  }
}

void PoseCrosshairPanel::initAxes()
{
  updateRanges();
  updateDeadzones();
}

void PoseCrosshairPanel::newPose(linuxtrack_full_pose_t *raw_pose, linuxtrack_pose_t *unfiltered,
                                 linuxtrack_pose_t *pose)
{
  (void) unfiltered;
  if(raw_pose == NULL || pose == NULL){
    return;
  }

  updateRanges();
  updateDeadzones();

  if(showMeasured){
    lookWidget->setPosition(raw_pose->pose.raw_yaw, raw_pose->pose.raw_pitch);
    leanWidget->setPosition(raw_pose->pose.raw_tx, raw_pose->pose.raw_tz);
    raiseWidget->setPosition(raw_pose->pose.raw_roll, raw_pose->pose.raw_ty);
  }else{
    lookWidget->setPosition(pose->yaw, pose->pitch);
    leanWidget->setPosition(pose->tx, pose->tz);
    raiseWidget->setPosition(pose->roll, pose->ty);
  }
}
