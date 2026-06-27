#include "pose_crosshair_widget.h"

#include <QPainter>
#include <QFontMetrics>
#include <QResizeEvent>
#include <cmath>

namespace {

const QColor kPlotBackground(0, 0, 32);
const QColor kCenterCrossColor(16, 16, 48);
const QColor kGridColor(64, 128, 128);
const QColor kDeadzoneFill(24, 0, 0);
const QColor kDotFill(64, 255, 128);
const QColor kValueTextColor(0, 128, 255);
const float kPlotMarginScale = 0.88f;
const float kRingFractions[] = { 0.33f, 0.66f, 1.0f };
const int kRingFractionCount = 3;

// AXIS_DEADZONE is a fraction of axis span (0..1), same normalized space as spline.c.
float plotHalfFromNorm(float norm, float plotHalf)
{
  const float clamped = std::max(0.f, std::min(1.f, norm));
  return clamped * plotHalf * kPlotMarginScale;
}

float normRingExtent(float deadzoneFrac, float ringFrac)
{
  const float dz = std::max(0.f, std::min(1.f, deadzoneFrac));
  return dz + ringFrac * (1.f - dz);
}

void drawGuideRingsCircle(QPainter &painter, const QPoint &plotCenter,
                          float deadzoneH, float deadzoneV,
                          float plotHalfW, float plotHalfH)
{
  QPen gridPen(kGridColor, 1.0, Qt::SolidLine);
  gridPen.setCosmetic(true);
  painter.setPen(gridPen);
  painter.setBrush(Qt::NoBrush);

  for(int i = 0; i < kRingFractionCount; ++i){
    const float f = kRingFractions[i];
    const float halfW = plotHalfFromNorm(normRingExtent(deadzoneH, f), plotHalfW);
    const float halfH = plotHalfFromNorm(normRingExtent(deadzoneV, f), plotHalfH);
    if(halfW < 1.f && halfH < 1.f){
      continue;
    }
    const float rx = std::max(halfW, 0.5f);
    const float ry = std::max(halfH, 0.5f);
    painter.drawEllipse(QRectF(plotCenter.x() - rx, plotCenter.y() - ry, rx * 2.f, ry * 2.f));
  }
}

void drawGuideRingsSquare(QPainter &painter, const QPoint &plotCenter,
                          float deadzoneH, float deadzoneV,
                          float plotHalfW, float plotHalfH)
{
  QPen gridPen(kGridColor, 1.0, Qt::SolidLine);
  gridPen.setCosmetic(true);
  painter.setPen(gridPen);
  painter.setBrush(Qt::NoBrush);

  for(int i = 0; i < kRingFractionCount; ++i){
    const float f = kRingFractions[i];
    const float halfW = plotHalfFromNorm(normRingExtent(deadzoneH, f), plotHalfW);
    const float halfH = plotHalfFromNorm(normRingExtent(deadzoneV, f), plotHalfH);
    if(halfW < 1.f && halfH < 1.f){
      continue;
    }
    const int left = (int)(plotCenter.x() - halfW);
    const int top = (int)(plotCenter.y() - halfH);
    const int w = std::max(1, (int)(halfW * 2.f));
    const int h = std::max(1, (int)(halfH * 2.f));
    painter.drawRect(left, top, w, h);
  }
}

void drawRadialDividers(QPainter &painter, const QPoint &plotCenter, int plotW, int plotH)
{
  const int cx = plotCenter.x();
  const int cy = plotCenter.y();
  const float slope1 = (179.f - 75.f) / (256.f - (-1.f));
  const float slope2 = (75.f - 179.f) / (256.f - (-1.f));
  const float slope3 = (256.f - (-1.f)) / (179.f - 75.f);
  const float slope4 = (256.f - (-1.f)) / (75.f - 179.f);

  QPen gridPen(kGridColor, 1.0, Qt::SolidLine);
  gridPen.setCosmetic(true);
  painter.setPen(gridPen);

  painter.drawLine(0, (int)(cy + slope1 * (0 - cx)), plotW, (int)(cy + slope1 * (plotW - cx)));
  painter.drawLine(0, (int)(cy + slope2 * (0 - cx)), plotW, (int)(cy + slope2 * (plotW - cx)));
  painter.drawLine((int)(cx + (1.f / slope3) * (0 - cy)), 0,
                   (int)(cx + (1.f / slope3) * (plotH - cy)), plotH);
  painter.drawLine((int)(cx + (1.f / slope4) * (0 - cy)), 0,
                   (int)(cx + (1.f / slope4) * (plotH - cy)), plotH);
}

}

PoseCrosshairWidget::PoseCrosshairWidget(QWidget *parent)
  : QWidget(parent), horizDegrees(true), vertDegrees(true),
    deadzoneStyle(CircleDeadzone), invertHorizontal(false), invertVertical(false),
    maxHorizontal(45.f), maxVertical(45.f), deadzoneHorizontal(0.f), deadzoneVertical(0.f),
    horizValue(0.f), vertValue(0.f), plotBackgroundValid(false)
{
  setMinimumSize(256, 256);
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
  plotBackgroundValid = false;
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
  plotBackgroundValid = false;
  update();
}

void PoseCrosshairWidget::setDeadzoneStyle(DeadzoneStyle style)
{
  if(deadzoneStyle != style){
    deadzoneStyle = style;
    plotBackgroundValid = false;
    update();
  }
}

void PoseCrosshairWidget::setDeadzone(float horizontal, float vertical)
{
  const float h = std::max(horizontal, 0.f);
  const float v = std::max(vertical, 0.f);
  if(deadzoneHorizontal != h || deadzoneVertical != v){
    deadzoneHorizontal = h;
    deadzoneVertical = v;
    plotBackgroundValid = false;
    update();
  }
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

float PoseCrosshairWidget::valueToPlotOffset(float value, float maxSpan, float plotHalf)
{
  return (value / maxSpan) * plotHalf * kPlotMarginScale;
}

QRect PoseCrosshairWidget::plotRectForSize(const QSize &size) const
{
  QFontMetrics fm(font());
  const int lineHeight = fm.height();
  const int headerHeight = lineHeight * 2 + 10;
  const int footerHeight = lineHeight + 8;
  const int sideBand = std::max(40, std::max(fm.horizontalAdvance(labelLeft),
                                              fm.horizontalAdvance(labelRight)) + 12);

  return QRect(sideBand, headerHeight,
               size.width() - sideBand * 2,
               size.height() - headerHeight - footerHeight);
}

void PoseCrosshairWidget::resizeEvent(QResizeEvent *event)
{
  QWidget::resizeEvent(event);
  center = QPoint(width() / 2 - 1, height() / 2 - 1);
  plotRect = plotRectForSize(event->size());
  plotBackgroundValid = false;
}

void PoseCrosshairWidget::rebuildPlotBackground()
{
  plotRect = plotRectForSize(size());
  if(plotRect.width() < 4 || plotRect.height() < 4){
    plotBackgroundValid = false;
    return;
  }

  plotBackground = QPixmap(plotRect.size());
  plotBackground.fill(Qt::transparent);

  QPainter painter(&plotBackground);
  painter.setRenderHint(QPainter::Antialiasing, true);

  const QPoint plotCenter(plotRect.width() / 2, plotRect.height() / 2);
  const float plotHalfW = plotRect.width() / 2.f;
  const float plotHalfH = plotRect.height() / 2.f;

  painter.fillRect(0, 0, plotRect.width(), plotRect.height(), kPlotBackground);

  QPen crossPen(kCenterCrossColor, 1.0, Qt::SolidLine);
  crossPen.setCosmetic(true);
  painter.setPen(crossPen);
  painter.drawLine(0, plotCenter.y(), plotRect.width(), plotCenter.y());
  painter.drawLine(plotCenter.x(), 0, plotCenter.x(), plotRect.height());

  if(deadzoneStyle == CircleDeadzone){
    drawGuideRingsCircle(painter, plotCenter,
                         deadzoneHorizontal, deadzoneVertical,
                         plotHalfW, plotHalfH);
    drawRadialDividers(painter, plotCenter, plotRect.width(), plotRect.height());
  }else{
    drawGuideRingsSquare(painter, plotCenter,
                         deadzoneHorizontal, deadzoneVertical,
                         plotHalfW, plotHalfH);
  }

  const float halfW = plotHalfFromNorm(deadzoneHorizontal, plotHalfW);
  const float halfH = plotHalfFromNorm(deadzoneVertical, plotHalfH);

  if(deadzoneStyle == SquareDeadzone){
    if(halfW > 0.5f || halfH > 0.5f){
      QPen gridPen(kGridColor, 1.0, Qt::SolidLine);
      gridPen.setCosmetic(true);
      painter.setPen(gridPen);
      painter.setBrush(Qt::NoBrush);
      painter.drawLine(0, (int)(plotCenter.y() - halfH), plotRect.width(), (int)(plotCenter.y() - halfH));
      painter.drawLine(0, (int)(plotCenter.y() + halfH), plotRect.width(), (int)(plotCenter.y() + halfH));
      painter.drawLine((int)(plotCenter.x() - halfW), 0, (int)(plotCenter.x() - halfW), plotRect.height());
      painter.drawLine((int)(plotCenter.x() + halfW), 0, (int)(plotCenter.x() + halfW), plotRect.height());

      const int rectLeft = (int)(plotCenter.x() - halfW) + 1;
      const int rectTop = (int)(plotCenter.y() - halfH) + 1;
      const int rectW = std::max(1, (int)(halfW * 2.f) - 1);
      const int rectH = std::max(1, (int)(halfH * 2.f) - 1);
      painter.setBrush(kDeadzoneFill);
      painter.setPen(Qt::NoPen);
      painter.drawRect(rectLeft, rectTop, rectW, rectH);
    }
  }else{
    if(halfW > 0.5f || halfH > 0.5f){
      const float rx = std::max(halfW, 0.5f);
      const float ry = std::max(halfH, 0.5f);
      painter.setBrush(kDeadzoneFill);
      painter.setPen(Qt::NoPen);
      painter.drawEllipse(QRectF(plotCenter.x() - rx, plotCenter.y() - ry, rx * 2.f, ry * 2.f));
    }
  }

  plotBackgroundValid = true;
}

void PoseCrosshairWidget::paintEvent(QPaintEvent * /* event */)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);

  const QColor titleColor = palette().color(QPalette::WindowText);
  const QColor edgeColor(kGridColor);
  const QFontMetrics fm(painter.font());
  const int lineHeight = fm.height();

  plotRect = plotRectForSize(size());
  if(plotRect.width() < 4 || plotRect.height() < 4){
    return;
  }

  if(!plotBackgroundValid){
    rebuildPlotBackground();
  }
  if(plotBackgroundValid){
    painter.drawPixmap(plotRect.topLeft(), plotBackground);
  }

  const QPoint plotCenter = plotRect.center();
  const float plotHalfW = plotRect.width() / 2.f;
  const float plotHalfH = plotRect.height() / 2.f;

  const float displayHoriz = invertHorizontal ? -horizValue : horizValue;
  const float displayVert = invertVertical ? -vertValue : vertValue;
  const float dotX = plotCenter.x() + valueToPlotOffset(displayHoriz, maxHorizontal, plotHalfW);
  const float dotY = plotCenter.y() - valueToPlotOffset(displayVert, maxVertical, plotHalfH);

  const int dotRadius = 2;
  QRectF dotRect(dotX - dotRadius, dotY - dotRadius, dotRadius * 2 + 1, dotRadius * 2 + 1);
  dotRect = dotRect.intersected(QRectF(plotRect));
  if(!dotRect.isEmpty()){
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(kDotFill);
    painter.drawEllipse(dotRect);
  }

  const QString horizSuffix = horizDegrees ? QString::fromUtf8("°") : QString::fromUtf8(" mm");
  const QString vertSuffix = vertDegrees ? QString::fromUtf8("°") : QString::fromUtf8(" mm");
  const QString valueText = QString::fromUtf8("%1%2, %3%4")
                                .arg(horizValue, 0, 'f', 1)
                                .arg(horizSuffix)
                                .arg(vertValue, 0, 'f', 1)
                                .arg(vertSuffix);

  const int sideBand = plotRect.left();

  painter.setPen(titleColor);
  painter.drawText(QRect(4, 4, width() - 8, lineHeight),
                   Qt::AlignHCenter | Qt::AlignVCenter, title);
  painter.setPen(kValueTextColor);
  painter.drawText(QRect(4, 4 + lineHeight, width() - 8, lineHeight),
                   Qt::AlignHCenter | Qt::AlignVCenter, valueText);

  painter.setPen(edgeColor);
  painter.drawText(QRect(4, plotCenter.y() - lineHeight / 2, sideBand - 6, lineHeight),
                   Qt::AlignRight | Qt::AlignVCenter, labelLeft);
  painter.drawText(QRect(width() - sideBand + 2, plotCenter.y() - lineHeight / 2, sideBand - 6, lineHeight),
                   Qt::AlignLeft | Qt::AlignVCenter, labelRight);
  painter.drawText(QRect(plotRect.left(), plotRect.top() + 2, plotRect.width(), lineHeight),
                   Qt::AlignHCenter | Qt::AlignTop, labelUp);
  painter.drawText(QRect(plotRect.left(), plotRect.bottom() - lineHeight - 2, plotRect.width(), lineHeight),
                   Qt::AlignHCenter | Qt::AlignBottom, labelDown);
}
