#ifndef POSE_CROSSHAIR_PANEL__H
#define POSE_CROSSHAIR_PANEL__H

#include <QWidget>
#include <QComboBox>
#include <QGroupBox>
#include <ltlib.h>

class PoseCrosshairWidget;

class PoseCrosshairPanel : public QWidget
{
  Q_OBJECT
 public:
  explicit PoseCrosshairPanel(QWidget *parent = 0);
  bool isExpanded() const;
  void setExpanded(bool expanded);

 signals:
  void expansionChanged(bool expanded);

 public slots:
  void newPose(linuxtrack_full_pose_t *raw_pose, linuxtrack_pose_t *unfiltered,
               linuxtrack_pose_t *pose);
  void axisChanged(int axis, int elem);
  void initAxes();

 private slots:
  void onSourceChanged(int index);
  void onExpandedToggled(bool expanded);

 private:
  void updateRanges();
  void updateDeadzones();
  void applyExpanded(bool expanded);

  QGroupBox *box;
  QWidget *contentHost;
  PoseCrosshairWidget *lookWidget;
  PoseCrosshairWidget *leanWidget;
  PoseCrosshairWidget *raiseWidget;
  QComboBox *sourceCombo;
  bool showMeasured;
};

#endif
