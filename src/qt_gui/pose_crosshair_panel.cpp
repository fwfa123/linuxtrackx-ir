#include "pose_crosshair_panel.h"

#include <QGroupBox>
#include <QHBoxLayout>
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
  : QWidget(parent), showMeasured(true)
{
  QGroupBox *box = new QGroupBox(tr("Pose maps"), this);
  QVBoxLayout *boxLayout = new QVBoxLayout(box);

  sourceCombo = new QComboBox(box);
  sourceCombo->addItem(tr("Measured (camera input)"));
  sourceCombo->addItem(tr("Filtered (game output)"));
  boxLayout->addWidget(sourceCombo);

  lookWidget = new PoseCrosshairWidget(box);
  lookWidget->setTitle(tr("Look"));
  lookWidget->setEdgeLabels(tr("Left"), tr("Right"), tr("Up"), tr("Down"));
  lookWidget->setUnits(true, true);
  lookWidget->setInvertHorizontal(true);
  lookWidget->setDeadzoneStyle(PoseCrosshairWidget::CircleDeadzone);

  leanWidget = new PoseCrosshairWidget(box);
  leanWidget->setTitle(tr("Lean"));
  leanWidget->setEdgeLabels(tr("Left"), tr("Right"), tr("Forward"), tr("Back"));
  leanWidget->setUnits(false, false);
  leanWidget->setInvertVertical(true);
  leanWidget->setDeadzoneStyle(PoseCrosshairWidget::SquareDeadzone);

  raiseWidget = new PoseCrosshairWidget(box);
  raiseWidget->setTitle(tr("Raise/Roll"));
  raiseWidget->setEdgeLabels(tr("Roll L"), tr("Roll R"), tr("Up"), tr("Down"));
  raiseWidget->setUnits(true, false);
  raiseWidget->setInvertHorizontal(true);
  raiseWidget->setDeadzoneStyle(PoseCrosshairWidget::SquareDeadzone);

  QHBoxLayout *mapsLayout = new QHBoxLayout();
  mapsLayout->addWidget(lookWidget, 1);
  mapsLayout->addWidget(leanWidget, 1);
  mapsLayout->addWidget(raiseWidget, 1);
  boxLayout->addLayout(mapsLayout, 1);

  QVBoxLayout *outer = new QVBoxLayout(this);
  outer->setContentsMargins(0, 0, 0, 0);
  outer->addWidget(box);

  connect(sourceCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onSourceChanged(int)));
  connect(&TRACKER, SIGNAL(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)),
          this, SLOT(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)));
  connect(&TRACKER, SIGNAL(axisChanged(int, int)), this, SLOT(axisChanged(int, int)));
  connect(&TRACKER, SIGNAL(initAxes(void)), this, SLOT(initAxes()));

  updateRanges();
  updateDeadzones();
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
  lookWidget->setDeadzone(TRACKER.axisGet(YAW, AXIS_DEADZONE),
                          TRACKER.axisGet(PITCH, AXIS_DEADZONE));
  leanWidget->setDeadzone(TRACKER.axisGet(TX, AXIS_DEADZONE),
                          TRACKER.axisGet(TZ, AXIS_DEADZONE));
  raiseWidget->setDeadzone(TRACKER.axisGet(ROLL, AXIS_DEADZONE),
                           TRACKER.axisGet(TY, AXIS_DEADZONE));
}

void PoseCrosshairPanel::axisChanged(int axis, int elem)
{
  (void) axis;
  if(elem == AXIS_DEADZONE || elem == AXIS_LLIMIT || elem == AXIS_RLIMIT
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
