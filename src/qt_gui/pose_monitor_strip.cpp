#include "pose_monitor_strip.h"

#include <QFont>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <cmath>

#include "tracker.h"

static const char *kRowNames[6] = {"Yaw", "Pitch", "Roll", "X", "Y", "Z"};
static const axis_t kRowAxes[6] = {YAW, PITCH, ROLL, TX, TY, TZ};

static const float kCouplingYawDeg = 5.0f;
static const float kCouplingGoodMm = 2.0f;
static const float kCouplingWarnMm = 5.0f;

PoseMonitorStrip::PoseMonitorStrip(QWidget *parent)
  : QWidget(parent), activeAxis(YAW)
{
  QGroupBox *box = new QGroupBox(tr("Live pose (Measured = camera, before curves)"), this);
  QGridLayout *grid = new QGridLayout(box);

  const QString headers[4] = {
    tr("Axis"), tr("Measured"), tr("Curved"), tr("Filtered")
  };
  QFont headerFont = font();
  headerFont.setBold(true);
  for(int col = 0; col < 4; ++col){
    headerLabels[col] = new QLabel(headers[col], box);
    headerLabels[col]->setFont(headerFont);
    grid->addWidget(headerLabels[col], 0, col);
  }

  for(int row = 0; row < 6; ++row){
    for(int col = 0; col < 4; ++col){
      rowLabels[row][col] = new QLabel(QString::fromUtf8("-"), box);
      if(col == 0){
        rowLabels[row][col]->setText(QString::fromUtf8(kRowNames[row]));
      }
      grid->addWidget(rowLabels[row][col], row + 1, col);
    }
  }

  couplingLabel = new QLabel(box);
  couplingLabel->setWordWrap(true);
  grid->addWidget(couplingLabel, 7, 0, 1, 4);

  QVBoxLayout *outer = new QVBoxLayout(this);
  outer->setContentsMargins(0, 0, 0, 0);
  outer->addWidget(box);

  connect(&TRACKER, SIGNAL(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)),
          this, SLOT(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)));
}

void PoseMonitorStrip::setActiveAxis(axis_t axis)
{
  activeAxis = axis;
  QFont normal = font();
  QFont bold = normal;
  bold.setBold(true);
  for(int row = 0; row < 6; ++row){
    const bool active = (kRowAxes[row] == activeAxis);
    rowLabels[row][0]->setFont(active ? bold : normal);
  }
}

void PoseMonitorStrip::updateRow(int row, float measured, float curved, float filtered)
{
  const bool isAngle = (row <= 2);
  if(isAngle){
    rowLabels[row][1]->setText(QString::fromUtf8("%1°").arg(measured, 0, 'f', 1));
    rowLabels[row][2]->setText(QString::fromUtf8("%1°").arg(curved, 0, 'f', 1));
    rowLabels[row][3]->setText(QString::fromUtf8("%1°").arg(filtered, 0, 'f', 1));
  }else{
    rowLabels[row][1]->setText(QString::fromUtf8("%1 mm").arg(measured, 0, 'f', 1));
    rowLabels[row][2]->setText(QString::fromUtf8("%1 mm").arg(curved, 0, 'f', 1));
    rowLabels[row][3]->setText(QString::fromUtf8("%1 mm").arg(filtered, 0, 'f', 1));
  }
}

void PoseMonitorStrip::updateCoupling(float yaw_deg, float tx_mm, float tz_mm)
{
  const float absYaw = std::fabs(yaw_deg);
  const float absTx = std::fabs(tx_mm);
  const float absTz = std::fabs(tz_mm);
  QString text = tr("Pan coupling while yawing: |Measured TX| = %1 mm, |Measured TZ| = %2 mm")
                     .arg(absTx, 0, 'f', 1)
                     .arg(absTz, 0, 'f', 1);
  if(absYaw >= kCouplingYawDeg){
    text += tr(" (yaw %1°) — reduce via Model Setup hc sliders if high").arg(yaw_deg, 0, 'f', 1);
  }else{
    text += tr(" — yaw head ~10° to assess pan leak");
  }

  QString style = QString::fromUtf8("QLabel { padding: 4px; border-radius: 3px; }");
  if(absYaw >= kCouplingYawDeg){
    if(absTx <= kCouplingGoodMm && absTz <= kCouplingGoodMm){
      style = QString::fromUtf8("QLabel { padding: 4px; border-radius: 3px; background: #d4edda; }");
    }else if(absTx <= kCouplingWarnMm && absTz <= kCouplingWarnMm){
      style = QString::fromUtf8("QLabel { padding: 4px; border-radius: 3px; background: #fff3cd; }");
    }else{
      style = QString::fromUtf8("QLabel { padding: 4px; border-radius: 3px; background: #f8d7da; }");
    }
  }
  couplingLabel->setText(text);
  couplingLabel->setStyleSheet(style);
}

void PoseMonitorStrip::newPose(linuxtrack_full_pose_t *raw_pose, linuxtrack_pose_t *unfiltered,
                               linuxtrack_pose_t *pose)
{
  if(raw_pose == NULL || unfiltered == NULL || pose == NULL){
    return;
  }

  updateRow(0, raw_pose->pose.raw_yaw, unfiltered->yaw, pose->yaw);
  updateRow(1, raw_pose->pose.raw_pitch, unfiltered->pitch, pose->pitch);
  updateRow(2, raw_pose->pose.raw_roll, unfiltered->roll, pose->roll);
  updateRow(3, raw_pose->pose.raw_tx, unfiltered->tx, pose->tx);
  updateRow(4, raw_pose->pose.raw_ty, unfiltered->ty, pose->ty);
  updateRow(5, raw_pose->pose.raw_tz, unfiltered->tz, pose->tz);

  updateCoupling(raw_pose->pose.raw_yaw, raw_pose->pose.raw_tx, raw_pose->pose.raw_tz);
  setActiveAxis(activeAxis);
}
