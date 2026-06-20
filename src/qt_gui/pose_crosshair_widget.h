#ifndef POSE_CROSSHAIR_WIDGET__H
#define POSE_CROSSHAIR_WIDGET__H

#include <QWidget>
#include <QString>

class PoseCrosshairWidget : public QWidget
{
  Q_OBJECT
 public:
  explicit PoseCrosshairWidget(QWidget *parent = 0);

  void setTitle(const QString &title);
  void setEdgeLabels(const QString &left, const QString &right,
                     const QString &up, const QString &down);
  void setUnits(bool degrees);
  void setRange(float maxHorizontal, float maxVertical);
  void setInvertHorizontal(bool invert);
  void setInvertVertical(bool invert);
  void setPosition(float horizontal, float vertical);

 protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);

 private:
  QString title;
  QString labelLeft;
  QString labelRight;
  QString labelUp;
  QString labelDown;
  bool useDegrees;
  bool invertHorizontal;
  bool invertVertical;
  float maxHorizontal;
  float maxVertical;
  float horizValue;
  float vertValue;
  QPoint center;
};

#endif
