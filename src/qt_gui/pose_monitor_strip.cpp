#include "pose_monitor_strip.h"

#include <QFont>
#include <QSettings>
#include <QVBoxLayout>

#include "tracker.h"

static const char *kRowNames[6] = {"Yaw", "Pitch", "Roll", "X", "Y", "Z"};
static const axis_t kRowAxes[6] = {YAW, PITCH, ROLL, TX, TY, TZ};

PoseMonitorStrip::PoseMonitorStrip(QWidget *parent)
  : QWidget(parent), box(NULL), gridHost(NULL), activeAxis(YAW)
{
  box = new QGroupBox(tr("Live pose (Measured = camera, before curves)"), this);
  box->setCheckable(true);
  box->setFlat(false);

  gridHost = new QWidget(box);
  QGridLayout *grid = new QGridLayout(gridHost);
  grid->setContentsMargins(0, 0, 0, 0);

  const QString headers[4] = {
    tr("Axis"), tr("Measured"), tr("Curved"), tr("Filtered")
  };
  QFont headerFont = font();
  headerFont.setBold(true);
  for(int col = 0; col < 4; ++col){
    headerLabels[col] = new QLabel(headers[col], gridHost);
    headerLabels[col]->setFont(headerFont);
    grid->addWidget(headerLabels[col], 0, col);
  }

  for(int row = 0; row < 6; ++row){
    for(int col = 0; col < 4; ++col){
      rowLabels[row][col] = new QLabel(QString::fromUtf8("-"), gridHost);
      if(col == 0){
        rowLabels[row][col]->setText(QString::fromUtf8(kRowNames[row]));
      }
      grid->addWidget(rowLabels[row][col], row + 1, col);
    }
  }

  QVBoxLayout *boxLayout = new QVBoxLayout(box);
  boxLayout->addWidget(gridHost);

  QVBoxLayout *outer = new QVBoxLayout(this);
  outer->setContentsMargins(0, 0, 0, 0);
  outer->addWidget(box);

  QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
  const bool expanded = settings.value(QStringLiteral("curveEditor/livePoseExpanded"), false).toBool();
  box->setChecked(expanded);
  applyExpanded(expanded);

  connect(box, SIGNAL(toggled(bool)), this, SLOT(onExpandedToggled(bool)));
  connect(&TRACKER, SIGNAL(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)),
          this, SLOT(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)));
}

void PoseMonitorStrip::applyExpanded(bool expanded)
{
  if(gridHost != NULL){
    gridHost->setVisible(expanded);
  }
}

void PoseMonitorStrip::onExpandedToggled(bool expanded)
{
  applyExpanded(expanded);
  QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
  settings.setValue(QStringLiteral("curveEditor/livePoseExpanded"), expanded);
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

  setActiveAxis(activeAxis);
}
