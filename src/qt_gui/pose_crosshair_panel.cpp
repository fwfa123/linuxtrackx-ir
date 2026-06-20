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
  QGroupBox *box = new QGroupBox(tr("Look and lean map"), this);
  QVBoxLayout *boxLayout = new QVBoxLayout(box);

  sourceCombo = new QComboBox(box);
  sourceCombo->addItem(tr("Measured (camera)"));
  sourceCombo->addItem(tr("Filtered (game output)"));
  boxLayout->addWidget(sourceCombo);

  lookWidget = new PoseCrosshairWidget(box);
  lookWidget->setTitle(tr("Look"));
  lookWidget->setEdgeLabels(tr("Left"), tr("Right"), tr("Up"), tr("Down"));
  lookWidget->setUnits(true);
  lookWidget->setInvertHorizontal(true);

  leanWidget = new PoseCrosshairWidget(box);
  leanWidget->setTitle(tr("Lean"));
  leanWidget->setEdgeLabels(tr("Left"), tr("Right"), tr("Forth"), tr("Back"));
  leanWidget->setUnits(false);
  leanWidget->setInvertVertical(true);

  QHBoxLayout *mapsLayout = new QHBoxLayout();
  mapsLayout->addWidget(lookWidget, 1);
  mapsLayout->addWidget(leanWidget, 1);
  boxLayout->addLayout(mapsLayout, 1);

  QVBoxLayout *outer = new QVBoxLayout(this);
  outer->setContentsMargins(0, 0, 0, 0);
  outer->addWidget(box);

  connect(sourceCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onSourceChanged(int)));
  connect(&TRACKER, SIGNAL(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)),
          this, SLOT(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)));

  updateRanges();
}

void PoseCrosshairPanel::onSourceChanged(int index)
{
  showMeasured = (index == 0);
}

void PoseCrosshairPanel::updateRanges()
{
  lookWidget->setRange(axisSpan(YAW), axisSpan(PITCH));
  leanWidget->setRange(axisSpan(TX), axisSpan(TZ));
}

void PoseCrosshairPanel::newPose(linuxtrack_full_pose_t *raw_pose, linuxtrack_pose_t *unfiltered,
                                 linuxtrack_pose_t *pose)
{
  (void) unfiltered;
  if(raw_pose == NULL || pose == NULL){
    return;
  }

  updateRanges();

  if(showMeasured){
    lookWidget->setPosition(raw_pose->pose.raw_yaw, raw_pose->pose.raw_pitch);
    leanWidget->setPosition(raw_pose->pose.raw_tx, raw_pose->pose.raw_tz);
  }else{
    lookWidget->setPosition(pose->yaw, pose->pitch);
    leanWidget->setPosition(pose->tx, pose->tz);
  }
}
