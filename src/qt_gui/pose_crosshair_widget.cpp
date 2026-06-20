#include "pose_crosshair_widget.h"

#include <QPainter>
#include <QFontMetrics>
#include <cmath>

PoseCrosshairWidget::PoseCrosshairWidget(QWidget *parent)
  : QWidget(parent), horizDegrees(true), vertDegrees(true), invertHorizontal(false), invertVertical(false),
    maxHorizontal(45.f), maxVertical(45.f), horizValue(0.f), vertValue(0.f)
{
  setMinimumSize(200, 170);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void PoseCrosshairWidget::setTitle(const QString &newTitle)
{
  title = newTitle;
  update();
}

void PoseCrosshairWidget::setEdgeLabels(const QString &left, const QString &right,
                                        const QString &up, const QString &down)
{
  labelLeft = left;
  labelRight = right;
  labelUp = up;
  labelDown = down;
  update();
}

void PoseCrosshairWidget::setUnits(bool horizontalDegrees, bool verticalDegrees)
{
  horizDegrees = horizontalDegrees;
  vertDegrees = verticalDegrees;
  update();
}

void PoseCrosshairWidget::setRange(float maxHoriz, float maxVert)
{
  maxHorizontal = std::max(maxHoriz, 1.f);
  maxVertical = std::max(maxVert, 1.f);
  update();
}

void PoseCrosshairWidget::setInvertHorizontal(bool invert)
{
  invertHorizontal = invert;
  update();
}

void PoseCrosshairWidget::setInvertVertical(bool invert)
{
  invertVertical = invert;
  update();
}

void PoseCrosshairWidget::setPosition(float horizontal, float vertical)
{
  horizValue = horizontal;
  vertValue = vertical;
  update();
}

void PoseCrosshairWidget::resizeEvent(QResizeEvent *event)
{
  QWidget::resizeEvent(event);
  center = QPoint(width() / 2 - 1, height() / 2 - 1);
}

void PoseCrosshairWidget::paintEvent(QPaintEvent * /* event */)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);

  const QColor textColor = palette().color(QPalette::WindowText);
  const QColor gridColor(textColor.red(), textColor.green(), textColor.blue(), 70);
  const QColor axisColor(textColor.red(), textColor.green(), textColor.blue(), 180);
  const QColor dotFill(64, 220, 120);
  const QFontMetrics fm(painter.font());
  const int lineHeight = fm.height();

  const int headerHeight = lineHeight * 2 + 10;
  const int footerHeight = lineHeight + 8;
  const int sideBand = std::max(40, std::max(fm.horizontalAdvance(labelLeft),
                                              fm.horizontalAdvance(labelRight)) + 12);

  const QRect plotRect(sideBand, headerHeight,
                       width() - sideBand * 2,
                       height() - headerHeight - footerHeight);
  if(plotRect.width() < 4 || plotRect.height() < 4){
    return;
  }

  const QPoint plotCenter = plotRect.center();
  const float plotHalfW = plotRect.width() / 2.f;
  const float plotHalfH = plotRect.height() / 2.f;

  QPen gridPen(gridColor, 1.0, Qt::DashLine);
  gridPen.setCosmetic(true);
  QPen axisPen(axisColor, 1.0, Qt::SolidLine);
  axisPen.setCosmetic(true);

  painter.setPen(axisPen);
  painter.drawLine(plotRect.left(), plotCenter.y(), plotRect.right(), plotCenter.y());
  painter.drawLine(plotCenter.x(), plotRect.top(), plotCenter.x(), plotRect.bottom());

  painter.setPen(gridPen);
  painter.drawLine(plotRect.left(), plotRect.top(), plotRect.right(), plotRect.top());
  painter.drawLine(plotRect.left(), plotRect.bottom(), plotRect.right(), plotRect.bottom());
  painter.drawLine(plotRect.left(), plotRect.top(), plotRect.left(), plotRect.bottom());
  painter.drawLine(plotRect.right(), plotRect.top(), plotRect.right(), plotRect.bottom());

  const float displayHoriz = invertHorizontal ? -horizValue : horizValue;
  const float displayVert = invertVertical ? -vertValue : vertValue;
  const float scale = 0.88f;
  const float dotX = plotCenter.x() + (displayHoriz / maxHorizontal) * plotHalfW * scale;
  const float dotY = plotCenter.y() - (displayVert / maxVertical) * plotHalfH * scale;

  const int dotRadius = 5;
  QRectF dotRect(dotX - dotRadius, dotY - dotRadius, dotRadius * 2, dotRadius * 2);
  dotRect = dotRect.intersected(QRectF(plotRect));
  if(!dotRect.isEmpty()){
    painter.setPen(QPen(textColor, 1));
    painter.setBrush(dotFill);
    painter.drawEllipse(dotRect);
  }

  const QString horizSuffix = horizDegrees ? QString::fromUtf8("°") : QString::fromUtf8(" mm");
  const QString vertSuffix = vertDegrees ? QString::fromUtf8("°") : QString::fromUtf8(" mm");
  const QString valueText = QString::fromUtf8("%1%2, %3%4")
                                .arg(horizValue, 0, 'f', 1)
                                .arg(horizSuffix)
                                .arg(vertValue, 0, 'f', 1)
                                .arg(vertSuffix);

  painter.setPen(textColor);
  painter.drawText(QRect(4, 4, width() - 8, lineHeight),
                   Qt::AlignHCenter | Qt::AlignVCenter, title);
  painter.drawText(QRect(4, 4 + lineHeight, width() - 8, lineHeight),
                   Qt::AlignHCenter | Qt::AlignVCenter, valueText);

  painter.setPen(axisColor);
  painter.drawText(QRect(4, plotCenter.y() - lineHeight / 2, sideBand - 6, lineHeight),
                   Qt::AlignRight | Qt::AlignVCenter, labelLeft);
  painter.drawText(QRect(width() - sideBand + 2, plotCenter.y() - lineHeight / 2, sideBand - 6, lineHeight),
                   Qt::AlignLeft | Qt::AlignVCenter, labelRight);
  painter.drawText(QRect(plotRect.left(), plotRect.top() + 2, plotRect.width(), lineHeight),
                   Qt::AlignHCenter | Qt::AlignTop, labelUp);
  painter.drawText(QRect(plotRect.left(), plotRect.bottom() - lineHeight - 2, plotRect.width(), lineHeight),
                   Qt::AlignHCenter | Qt::AlignBottom, labelDown);
}
