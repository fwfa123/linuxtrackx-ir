#include <QPainter>
#include <QFontMetrics>

#include "scview.h"
#include <math.h>
#include <iostream>
#include "tracker.h"

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
  setMinimumSize(400, 100);
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
  QSize sz = size();
  float h = sz.height() - 1;//Why, oh why?
  float w = floor((sz.width() / 2) - 1);
//  float max_f = (axis->getLFactor() > axis->getRFactor()) ? 
//                 axis->getLFactor() : axis->getRFactor();
  float max_f = (TRACKER.axisGet(axis, AXIS_LLIMIT) > TRACKER.axisGet(axis, AXIS_RLIMIT)) ? 
                 TRACKER.axisGet(axis, AXIS_LLIMIT) : TRACKER.axisGet(axis, AXIS_RLIMIT);
  
  spline(points, spline_points);
  float x,y;
  for(int i = 0; i < spline_points; ++i){
    x = points[i].x();
    y = points[i].y();
    points[i].rx() = (x + 1.0f) * w;
    points[i].ry() = (max_f > 0.0) ? h - y * (h / max_f) : h;
  }
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);

  const QColor curveColor = palette().color(QPalette::WindowText);
  const QColor filteredCrossColor(220, 72, 72);
  const QColor curvedCrossColor(72, 145, 220);

  painter.setPen(curveColor);
  painter.drawPolyline(points, spline_points);

  float kl = (TRACKER.axisGet(axis, AXIS_MULT) != 0.0) ? 
        TRACKER.axisGet(axis, AXIS_LLIMIT) / TRACKER.axisGet(axis, AXIS_MULT) : 0.0;
  float kr = (TRACKER.axisGet(axis, AXIS_MULT) != 0.0) ? 
        TRACKER.axisGet(axis, AXIS_RLIMIT) / TRACKER.axisGet(axis, AXIS_MULT) : 0.0;
  float max_k = kl > kr ? kl : kr;
  
  float trx = rx;
  if(invert){
    trx *= -1;
  }
  //Draw cross with current position
  float nx = w + w * trx/max_k;
  float ny = (max_f != 0.0) ? h - fabs(px * (h / max_f)) : h;
  float unx = w + w * trx/max_k;
  float uny = (max_f != 0.0) ? h - fabs(upx * (h / max_f)) : h;

  const QFontMetrics fm(painter.font());
  const int textX = 8;
  int textY = fm.ascent() + 4;
  const int lineHeight = fm.height();

  painter.setPen(curveColor);
  painter.drawText(textX, textY, QString::fromUtf8("Measured: %1").arg(rx, 0, 'f', 2));
  textY += lineHeight;
  painter.drawText(textX, textY, QString::fromUtf8("Curved: %1").arg(upx, 0, 'f', 2));
  textY += lineHeight;
  painter.drawText(textX, textY, QString::fromUtf8("Filtered: %1").arg(px, 0, 'f', 2));

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


