#ifndef POSE_CROSSHAIR_WIDGET__H
#define POSE_CROSSHAIR_WIDGET__H

#include <QWidget>
#include <QString>
#include <QPixmap>
#include <QRect>

class PoseCrosshairWidget : public QWidget
{
  Q_OBJECT
 public:
  enum DeadzoneStyle {
    CircleDeadzone,
    SquareDeadzone
  };

  explicit PoseCrosshairWidget(QWidget *parent = 0);

  void setTitle(const QString &title);
  void setEdgeLabels(const QString &left, const QString &right,
                     const QString &up, const QString &down);
  void setUnits(bool horizontalDegrees, bool verticalDegrees);
  void setRange(float maxHorizontal, float maxVertical);
  void setDeadzoneStyle(DeadzoneStyle style);
  void setDeadzone(float horizontal, float vertical);
  void setInvertHorizontal(bool invert);
  void setInvertVertical(bool invert);
  void setPosition(float horizontal, float vertical);

 protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);

 private:
  void rebuildPlotBackground();
  QRect plotRectForSize(const QSize &size) const;
  static float valueToPlotOffset(float value, float maxSpan, float plotHalf);

  QString title;
  QString labelLeft;
  QString labelRight;
  QString labelUp;
  QString labelDown;
  bool horizDegrees;
  bool vertDegrees;
  DeadzoneStyle deadzoneStyle;
  bool invertHorizontal;
  bool invertVertical;
  float maxHorizontal;
  float maxVertical;
  float deadzoneHorizontal;
  float deadzoneVertical;
  float horizValue;
  float vertValue;
  QPoint center;
  QRect plotRect;
  QPixmap plotBackground;
  bool plotBackgroundValid;
};

#endif
