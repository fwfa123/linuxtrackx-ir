#ifndef SCVIEW__H
#define SCVIEW__H

#include <QWidget>
#include <QTimer>
#include <QEvent>
#include <QMouseEvent>
#include <axis.h>
#include <ltlib.h>

class SCView : public QWidget
{
     Q_OBJECT
 public:
  SCView(axis_t a, QWidget *parent = 0);
  ~SCView();
  void setSymmetrical(bool symmetrical);
 public slots:
  void redraw();
  void newPose(linuxtrack_full_pose_t *raw_pose, linuxtrack_pose_t *unfiltered, linuxtrack_pose_t *pose);
 protected:
  void paintEvent(QPaintEvent *event);
  void changeEvent(QEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

 private:
  int spline(QPointF points[], int num_points);
  float rx;
  float px;
  float upx;
  axis_t axis;
  QTimer *timer;
  bool invert;
  bool symmetrical;
  bool dragging;
  bool dragRight;
  int dragIndex;
  int dragCount;
  ltr_curve_pt dragPts[LTR_CURVE_MAX_POINTS];

  void applyHalf(bool right, const ltr_curve_pt *pts, int count, bool persist);
};

#endif
