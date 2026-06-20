#ifndef POSE_CROSSHAIR_PANEL__H
#define POSE_CROSSHAIR_PANEL__H

#include <QWidget>
#include <QComboBox>
#include <ltlib.h>

class PoseCrosshairWidget;

class PoseCrosshairPanel : public QWidget
{
  Q_OBJECT
 public:
  explicit PoseCrosshairPanel(QWidget *parent = 0);

 public slots:
  void newPose(linuxtrack_full_pose_t *raw_pose, linuxtrack_pose_t *unfiltered,
               linuxtrack_pose_t *pose);

 private slots:
  void onSourceChanged(int index);

 private:
  void updateRanges();

  PoseCrosshairWidget *lookWidget;
  PoseCrosshairWidget *leanWidget;
  PoseCrosshairWidget *raiseWidget;
  QComboBox *sourceCombo;
  bool showMeasured;
};

#endif
