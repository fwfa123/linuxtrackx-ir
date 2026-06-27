#ifndef POSE_MONITOR_STRIP__H
#define POSE_MONITOR_STRIP__H

#include <QWidget>
#include <QLabel>
#include <axis.h>
#include <ltlib.h>

class PoseMonitorStrip : public QWidget
{
  Q_OBJECT
 public:
  explicit PoseMonitorStrip(QWidget *parent = 0);
  void setActiveAxis(axis_t axis);

 public slots:
  void newPose(linuxtrack_full_pose_t *raw_pose, linuxtrack_pose_t *unfiltered,
               linuxtrack_pose_t *pose);

 private:
  void updateRow(int row, float measured, float curved, float filtered);

  QLabel *headerLabels[4];
  QLabel *rowLabels[6][4];
  axis_t activeAxis;
};

#endif
