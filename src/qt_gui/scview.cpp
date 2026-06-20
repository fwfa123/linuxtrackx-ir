#include <QPainter>
#include <QFontMetrics>
#include <cmath>

#include "scview.h"
#include <math.h>
#include <iostream>
#include "tracker.h"

namespace {

bool axisUsesMm(axis_t axis)
{
  return axis == TX || axis == TY || axis == TZ;
}

QString formatTickValue(float value, axis_t axis)
{
  const int precision = (axisUsesMm(axis) && std::fabs(value) < 100.f) ? 1 : 0;
  return QString::number(value, 'f', precision);
}

float niceStep(float range, int targetTicks)
{
  if(range <= 0.f || targetTicks <= 0){
    return 1.f;
  }
  const float rough = range / static_cast<float>(targetTicks);
  const float magnitude = std::pow(10.f, std::floor(std::log10(rough)));
  const float residual = rough / magnitude;
  float nice;
  if(residual <= 1.f){
    nice = 1.f;
  }else if(residual <= 2.f){
    nice = 2.f;
  }else if(residual <= 5.f){
    nice = 5.f;
  }else{
    nice = 10.f;
  }
  return nice * magnitude;
}

struct PlotArea {
  QRectF rect;
  float centerX;
  float halfW;
  float top;
  float bottom;
  float height;
};

float displayInput(float value, bool invert)
{
  return invert ? -value : value;
}

float valueToPixelX(float physical, float max_k, bool invert, const PlotArea &plot)
{
  if(max_k <= 0.f){
    return plot.centerX;
  }
  return plot.centerX + plot.halfW * displayInput(physical, invert) / max_k;
}

float outputToPixelY(float output, float max_f, const PlotArea &plot)
{
  if(max_f <= 0.f){
    return plot.bottom;
  }
  return plot.bottom - output * (plot.height / max_f);
}

int leftMarginForTicks(const QFontMetrics &fm, axis_t axis, float max_f, float max_k)
{
  int margin = 8;
  if(max_f > 0.f){
    const float yStep = niceStep(max_f, 5);
    for(float yVal = 0.f; yVal <= max_f + yStep * 0.01f; yVal += yStep){
      margin = std::max(margin, fm.horizontalAdvance(formatTickValue(yVal, axis)) + 8);
    }
  }
  if(max_k > 0.f){
    margin = std::max(margin, fm.horizontalAdvance(formatTickValue(-max_k, axis)) + 8);
  }
  return margin;
}

PlotArea makePlotArea(int width, int height, int leftMargin, int bottomMargin, int topMargin)
{
  PlotArea plot;
  const int rightMargin = 4;
  plot.rect = QRectF(leftMargin, topMargin,
                     width - leftMargin - rightMargin,
                     height - topMargin - bottomMargin);
  plot.halfW = plot.rect.width() / 2.f;
  plot.centerX = plot.rect.left() + plot.halfW;
  plot.top = plot.rect.top();
  plot.bottom = plot.rect.bottom();
  plot.height = plot.rect.height();
  return plot;
}

void drawValueLegend(QPainter &painter, const QFontMetrics &fm, int width, int legendHeight,
                     float measured, float curved, float filtered, const QColor &textColor,
                     const QColor &filteredColor, const QColor &curvedColor)
{
  const int lineHeight = fm.height();
  const int rowCount = 3;
  const int blockHeight = rowCount * lineHeight;
  const int topPad = std::max(2, (legendHeight - blockHeight) / 2);
  int textY = topPad + fm.ascent();

  painter.setPen(textColor);
  painter.drawText(8, textY, QString::fromUtf8("Measured: %1").arg(measured, 0, 'f', 2));
  textY += lineHeight;
  painter.setPen(curvedColor);
  painter.drawText(8, textY, QString::fromUtf8("Curved: %1").arg(curved, 0, 'f', 2));
  textY += lineHeight;
  painter.setPen(filteredColor);
  painter.drawText(8, textY, QString::fromUtf8("Filtered: %1").arg(filtered, 0, 'f', 2));

  QColor separatorColor(textColor.red(), textColor.green(), textColor.blue(), 80);
  QPen separator(separatorColor, 1.0);
  separator.setCosmetic(true);
  painter.setPen(separator);
  painter.drawLine(0, legendHeight - 1, width, legendHeight - 1);
}

QPen makeGridPen(const QColor &color)
{
  QPen pen(color, 1.0, Qt::CustomDashLine);
  pen.setDashPattern({5, 4});
  pen.setCosmetic(true);
  return pen;
}

QPen makeAxisPen(const QColor &color)
{
  QPen pen(color, 1.0, Qt::SolidLine);
  pen.setCosmetic(true);
  return pen;
}

void drawGrid(QPainter &painter, axis_t axis, float max_k, float max_f, bool invert,
              const PlotArea &plot, const QFontMetrics &fm,
              const QColor &gridColor, const QColor &axisColor)
{
  painter.save();
  painter.setRenderHint(QPainter::Antialiasing, false);

  const QPen gridPen = makeGridPen(gridColor);
  const QPen axisPen = makeAxisPen(axisColor);
  const QPen labelPen(axisColor);

  if(max_f > 0.f){
    const float yStep = niceStep(max_f, 5);
    for(float yVal = 0.f; yVal <= max_f + yStep * 0.01f; yVal += yStep){
      const float py = outputToPixelY(yVal, max_f, plot);
      if(py < plot.top - 0.5f || py > plot.bottom + 0.5f){
        continue;
      }
      painter.setPen(std::fabs(yVal) < 0.001f ? axisPen : gridPen);
      painter.drawLine(QLineF(plot.rect.left(), py, plot.rect.right(), py));

      painter.setPen(labelPen);
      painter.drawText(QRectF(0.f, py - fm.height() / 2.f, plot.rect.left() - 4.f, fm.height()),
                       Qt::AlignRight | Qt::AlignVCenter,
                       formatTickValue(yVal, axis));
    }
  }

  if(max_k > 0.f){
    const float xStep = niceStep(2.f * max_k, 6);
    const float start = -std::ceil(max_k / xStep) * xStep;
    for(float xVal = start; xVal <= max_k + xStep * 0.01f; xVal += xStep){
      const float px = valueToPixelX(xVal, max_k, invert, plot);
      if(px < plot.rect.left() - 0.5f || px > plot.rect.right() + 0.5f){
        continue;
      }
      painter.setPen(std::fabs(xVal) < 0.001f ? axisPen : gridPen);
      painter.drawLine(QLineF(px, plot.top, px, plot.bottom));

      painter.setPen(labelPen);
      painter.drawText(QRectF(px - 30.f, plot.bottom + 2.f, 60.f, fm.height()),
                       Qt::AlignHCenter | Qt::AlignTop,
                       formatTickValue(xVal, axis));
    }
  }

  painter.restore();
}

} // namespace

SCView::SCView(axis_t a, QWidget *parent)
  : QWidget(parent), parentWidget(parent), px(0.0), axis(a), timer(NULL), invert(false)
{
  setBackgroundRole(QPalette::Base);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setAutoFillBackground(true);
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  connect(&TRACKER, SIGNAL(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)), 
          this, SLOT(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)));
  timer->start(50);
  setMinimumSize(400, 140);
  switch(a){
    case YAW:
    case ROLL:
    case TZ:
      invert = true;
      break;
    default:
      break;
  }
}

SCView::~SCView()
{
  timer->stop();
  delete timer;
}

void SCView::redraw()
{
  update();
}

void SCView::changeEvent(QEvent *event)
{
  QWidget::changeEvent(event);
  if(event->type() == QEvent::PaletteChange || event->type() == QEvent::StyleChange){
    update();
  }
}

//QSize SCView::sizeHint() const
//{
//  if(parentWidget){
//    std::cout<<"Size: "<<parentWidget->size().height() <<" "<< parentWidget->size().width() << "\n";
//    return parentWidget->size();
//  }else{
//    return minimumSizeHint();
//  }
//}

//QSize SCView::minimumSizeHint() const
//{
//  return QSize(320, 180);
//}   


int SCView::spline(QPointF points[], int num_points)
{
  float x, kl, kr, max_k;
  float k = 2.0f / (num_points - 1);
  kl = (TRACKER.axisGet(axis, AXIS_MULT) != 0.0) ? 
        TRACKER.axisGet(axis, AXIS_LLIMIT) / TRACKER.axisGet(axis, AXIS_MULT) : 0.0;
  kr = (TRACKER.axisGet(axis, AXIS_MULT) != 0.0) ? 
        TRACKER.axisGet(axis, AXIS_RLIMIT) / TRACKER.axisGet(axis, AXIS_MULT) : 0.0;
  max_k = kl > kr ? kl : kr;
  for(int i = 0; i < num_points; ++i){
    x = -1.0f + k * i;
    points[i].ry() = fabs(TRACKER.axisGetValue(axis, x * max_k));
    if(invert){
      x *= -1;
    }
    points[i].rx() = x;
  }
  return 0;
}


float SCView::spline(float x)
{
  return fabs(TRACKER.axisGetValue(axis, x));
}

//Should be odd...
static const int spline_points = 101;
static QPointF points[spline_points];

void SCView::paintEvent(QPaintEvent * /* event */)
{
  const QSize sz = size();
  const float max_f = (TRACKER.axisGet(axis, AXIS_LLIMIT) > TRACKER.axisGet(axis, AXIS_RLIMIT)) ?
                      TRACKER.axisGet(axis, AXIS_LLIMIT) : TRACKER.axisGet(axis, AXIS_RLIMIT);

  const float kl = (TRACKER.axisGet(axis, AXIS_MULT) != 0.0) ?
                   TRACKER.axisGet(axis, AXIS_LLIMIT) / TRACKER.axisGet(axis, AXIS_MULT) : 0.0;
  const float kr = (TRACKER.axisGet(axis, AXIS_MULT) != 0.0) ?
                   TRACKER.axisGet(axis, AXIS_RLIMIT) / TRACKER.axisGet(axis, AXIS_MULT) : 0.0;
  const float max_k = kl > kr ? kl : kr;

  QPainter painter(this);

  const QFontMetrics fm(painter.font());
  const int lineHeight = fm.height();
  const int legendHeight = lineHeight * 3 + 8;
  const int bottomMargin = fm.height() + 6;
  const int leftMargin = leftMarginForTicks(fm, axis, max_f, max_k);
  const PlotArea plot = makePlotArea(sz.width(), sz.height(), leftMargin, bottomMargin, legendHeight);

  const QColor curveColor = palette().color(QPalette::WindowText);
  const QColor filteredCrossColor(220, 72, 72);
  const QColor curvedCrossColor(72, 145, 220);
  const QColor gridColor(curveColor.red(), curveColor.green(), curveColor.blue(), 90);
  const QColor axisColor(curveColor.red(), curveColor.green(), curveColor.blue(), 210);

  drawValueLegend(painter, fm, sz.width(), legendHeight, rx, upx, px,
                  curveColor, filteredCrossColor, curvedCrossColor);

  drawGrid(painter, axis, max_k, max_f, invert, plot, fm, gridColor, axisColor);

  painter.setRenderHint(QPainter::Antialiasing, true);

  spline(points, spline_points);
  float x, y;
  for(int i = 0; i < spline_points; ++i){
    x = points[i].x();
    y = points[i].y();
    points[i].rx() = plot.centerX + plot.halfW * x;
    points[i].ry() = outputToPixelY(y, max_f, plot);
  }

  painter.setPen(curveColor);
  painter.drawPolyline(points, spline_points);

  const float nx = valueToPixelX(rx, max_k, invert, plot);
  const float ny = outputToPixelY(std::fabs(px), max_f, plot);
  const float unx = valueToPixelX(rx, max_k, invert, plot);
  const float uny = outputToPixelY(std::fabs(upx), max_f, plot);

  painter.setPen(filteredCrossColor);
  const float crossHalf = 10.0f;
  painter.drawLine(QLineF(nx, ny - crossHalf, nx, ny + crossHalf));
  painter.drawLine(QLineF(nx - crossHalf, ny, nx + crossHalf, ny));

  painter.setPen(curvedCrossColor);
  painter.drawLine(QLineF(unx, uny - crossHalf, unx, uny + crossHalf));
  painter.drawLine(QLineF(unx - crossHalf, uny, unx + crossHalf, uny));

  painter.end();
}

void SCView::newPose(linuxtrack_full_pose_t *raw_pose, linuxtrack_pose_t *unfiltered, linuxtrack_pose_t *pose)
{
  (void) pose;
  switch(axis){
    case PITCH:
      rx = raw_pose->pose.raw_pitch;
      px = pose->pitch;
      upx = unfiltered->pitch;
      break;
    case ROLL:
      rx = raw_pose->pose.raw_roll;
      px = pose->roll;
      upx = unfiltered->roll;
      break;
    case YAW:
      rx = raw_pose->pose.raw_yaw;
      px = pose->yaw;
      upx = unfiltered->yaw;
      break;
    case TX:
      rx = raw_pose->pose.raw_tx;
      px = pose->tx;
      upx = unfiltered->tx;
      break;
    case TY:
      rx = raw_pose->pose.raw_ty;
      px = pose->ty;
      upx = unfiltered->ty;
      break;
    case TZ:
      rx = raw_pose->pose.raw_tz;
      px = pose->tz;
      upx = unfiltered->tz;
      break;
    default:
      //don't mind MISC stuff
      break;
  }
}

/*
void SCView::movePoint(float new_x)
{
  px = new_x;
}
*/


