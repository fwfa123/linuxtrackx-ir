#include <QPainter>
#include <QFontMetrics>
#include <cmath>

#include "scview.h"
#include <math.h>
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

struct CurveGeom {
  PlotArea plot;
  float maxK;
  float maxF;
  float factor;
  float lLimit;
  float rLimit;
  bool ok;
};

CurveGeom buildCurveGeom(axis_t axis, const QFontMetrics &fm, const QSize &sz)
{
  CurveGeom g;
  g.factor = TRACKER.axisGet(axis, AXIS_MULT);
  g.lLimit = TRACKER.axisGet(axis, AXIS_LLIMIT);
  g.rLimit = TRACKER.axisGet(axis, AXIS_RLIMIT);
  g.maxF = (g.lLimit > g.rLimit) ? g.lLimit : g.rLimit;
  const float kl = (g.factor != 0.f) ? g.lLimit / g.factor : 0.f;
  const float kr = (g.factor != 0.f) ? g.rLimit / g.factor : 0.f;
  g.maxK = (kl > kr) ? kl : kr;
  const int legendHeight = fm.height() * 3 + 8;
  const int bottomMargin = fm.height() + 6;
  const int leftMargin = leftMarginForTicks(fm, axis, g.maxF, g.maxK);
  g.plot = makePlotArea(sz.width(), sz.height(), leftMargin, bottomMargin, legendHeight);
  g.ok = g.factor > 1e-6f && g.lLimit > 1e-5f && g.rLimit > 1e-5f &&
         g.maxK > 0.f && g.maxF > 0.f && g.plot.halfW > 1.f && g.plot.height > 1.f;
  return g;
}

QPointF controlPointPixel(const ltr_curve_pt &pt, bool right, bool invert, const CurveGeom &g)
{
  const float lim = right ? g.rLimit : g.lLimit;
  const float physical = (right ? 1.f : -1.f) * pt.x * lim / g.factor;
  const float output = pt.y * lim;
  return QPointF(valueToPixelX(physical, g.maxK, invert, g.plot),
                 outputToPixelY(output, g.maxF, g.plot));
}

bool pixelToNormalized(const QPointF &pos, bool invert, const CurveGeom &g,
                       bool forceHalf, bool &right, float &nx, float &ny)
{
  if(!g.ok){
    return false;
  }
  const float display = (static_cast<float>(pos.x()) - g.plot.centerX) / g.plot.halfW * g.maxK;
  float physical = invert ? -display : display;
  if(!forceHalf){
    right = physical >= 0.f;
  }else if(right && physical < 0.f){
    physical = 0.f;
  }else if(!right && physical > 0.f){
    physical = 0.f;
  }
  const float lim = right ? g.rLimit : g.lLimit;
  nx = std::fabs(physical) * g.factor / lim;
  const float output = (g.plot.bottom - static_cast<float>(pos.y())) / g.plot.height * g.maxF;
  ny = output / lim;
  if(nx < 0.f) nx = 0.f;
  if(nx > 1.f) nx = 1.f;
  if(ny < 0.f) ny = 0.f;
  if(ny > 1.f) ny = 1.f;
  return true;
}

void drawControlPoints(QPainter &painter, axis_t axis, bool invert, const CurveGeom &g,
                       bool hasSelection, bool selectedRight, int selectedIndex)
{
  if(!g.ok){
    return;
  }
  const QColor fill = painter.pen().color().lighter(160);
  const QColor outline(40, 90, 160);
  const QColor selectedOutline(220, 120, 40);
  painter.setRenderHint(QPainter::Antialiasing, true);
  for(int side = 0; side < 2; ++side){
    const bool right = side == 1;
    ltr_curve_pt pts[LTR_CURVE_MAX_POINTS];
    const int count = TRACKER.axisGetCurve(axis, right, pts, LTR_CURVE_MAX_POINTS);
    for(int i = 0; i < count; ++i){
      const QPointF at = controlPointPixel(pts[i], right, invert, g);
      const bool endpoint = (i == 0 || i + 1 == count);
      const bool selected = hasSelection && selectedRight == right && selectedIndex == i;
      const float radius = selected ? 7.f : (endpoint ? 4.f : 5.5f);
      painter.setPen(QPen(selected ? selectedOutline : outline, selected ? 2.5 : 1.5));
      painter.setBrush(endpoint ? QColor(outline.red(), outline.green(), outline.blue(), 70) : fill);
      painter.drawEllipse(at, radius, radius);
    }
  }
}

} // namespace

SCView::SCView(axis_t a, QWidget *parent)
  : QWidget(parent), px(0.0), axis(a), timer(NULL), invert(false),
    symmetrical(false), dragging(false), dragRight(false), dragIndex(-1), dragCount(0),
    selectedRight(false), selectedIndex(-1)
{
  setBackgroundRole(QPalette::Base);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setAutoFillBackground(true);
  setFocusPolicy(Qt::StrongFocus);
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  connect(&TRACKER, SIGNAL(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)), 
          this, SLOT(newPose(linuxtrack_full_pose_t *, linuxtrack_pose_t *, linuxtrack_pose_t *)));
  timer->start(50);
  setMinimumSize(400, 140);
  setMouseTracking(false);
  setToolTip(tr("Click to select or add a control point. Drag to move it. "
                "Right-click or Delete removes an interior point."));
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


static const int spline_points = 101;
static QPointF points[spline_points];

void SCView::paintEvent(QPaintEvent * /* event */)
{
  const QSize sz = size();
  QPainter painter(this);
  const QFontMetrics fm(painter.font());
  const CurveGeom geom = buildCurveGeom(axis, fm, sz);
  const float max_f = geom.maxF;
  const float max_k = geom.maxK;
  const PlotArea plot = geom.plot;

  const int lineHeight = fm.height();
  const int legendHeight = lineHeight * 3 + 8;

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
  drawControlPoints(painter, axis, invert, geom,
                    selectedIndex >= 0, selectedRight, selectedIndex);

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

void SCView::setSymmetrical(bool isSymmetrical)
{
  symmetrical = isSymmetrical;
}

bool SCView::hasSelection() const
{
  return selectedIndex >= 0;
}

bool SCView::selectionIsRemovable() const
{
  if(selectedIndex < 0){
    return false;
  }
  ltr_curve_pt pts[LTR_CURVE_MAX_POINTS];
  const int count = TRACKER.axisGetCurve(axis, selectedRight, pts, LTR_CURVE_MAX_POINTS);
  return selectedIndex > 0 && selectedIndex + 1 < count;
}

void SCView::emitSelectionChanged()
{
  float x = 0.f;
  float y = 0.f;
  bool removable = false;
  if(selectedIndex >= 0){
    ltr_curve_pt pts[LTR_CURVE_MAX_POINTS];
    const int count = TRACKER.axisGetCurve(axis, selectedRight, pts, LTR_CURVE_MAX_POINTS);
    if(selectedIndex < count){
      x = pts[selectedIndex].x;
      y = pts[selectedIndex].y;
      removable = selectedIndex > 0 && selectedIndex + 1 < count;
    }else{
      selectedIndex = -1;
    }
  }
  emit pointSelectionChanged(selectedRight, selectedIndex, x, y, removable, selectedIndex >= 0);
}

void SCView::selectPoint(bool right, int index)
{
  selectedRight = right;
  selectedIndex = index;
  emitSelectionChanged();
  update();
}

void SCView::clearSelection()
{
  if(selectedIndex < 0){
    return;
  }
  selectedIndex = -1;
  emitSelectionChanged();
  update();
}

void SCView::refreshSelectionEmit()
{
  emitSelectionChanged();
}

bool SCView::setSelectedPointCoords(float x, float y)
{
  if(!selectionIsRemovable()){
    return false;
  }
  ltr_curve_pt pts[LTR_CURVE_MAX_POINTS];
  const int count = TRACKER.axisGetCurve(axis, selectedRight, pts, LTR_CURVE_MAX_POINTS);
  const float minX = pts[selectedIndex - 1].x + LTR_CURVE_MIN_GAP;
  const float maxX = pts[selectedIndex + 1].x - LTR_CURVE_MIN_GAP;
  if(minX > maxX){
    return false;
  }
  if(x < minX) x = minX;
  if(x > maxX) x = maxX;
  if(y < 0.f) y = 0.f;
  if(y > 1.f) y = 1.f;
  pts[selectedIndex].x = x;
  pts[selectedIndex].y = y;
  applyHalf(selectedRight, pts, count, true);
  emitSelectionChanged();
  update();
  return true;
}

void SCView::removeSelectedPoint()
{
  if(!selectionIsRemovable()){
    return;
  }
  ltr_curve_pt pts[LTR_CURVE_MAX_POINTS];
  const int count = TRACKER.axisGetCurve(axis, selectedRight, pts, LTR_CURVE_MAX_POINTS);
  for(int i = selectedIndex; i + 1 < count; ++i){
    pts[i] = pts[i + 1];
  }
  applyHalf(selectedRight, pts, count - 1, true);
  clearSelection();
}

void SCView::applyHalf(bool right, const ltr_curve_pt *edited, int count, bool persist)
{
  ltr_curve_pt other[LTR_CURVE_MAX_POINTS];
  const int otherCount = TRACKER.axisGetCurve(axis, !right, other, LTR_CURVE_MAX_POINTS);
  const ltr_curve_pt *leftPts = (symmetrical || !right) ? edited : other;
  const int nLeft = (symmetrical || !right) ? count : otherCount;
  const ltr_curve_pt *rightPts = (symmetrical || right) ? edited : other;
  const int nRight = (symmetrical || right) ? count : otherCount;
  if(persist){
    TRACKER.axisSetCurve(axis, leftPts, nLeft, rightPts, nRight);
  }else{
    TRACKER.axisSetCurvePreview(axis, leftPts, nLeft, rightPts, nRight);
  }
}

namespace {

struct PointHit {
  bool valid;
  bool right;
  int index;
  int count;
};

PointHit hitTestPoint(axis_t axis, bool invert, const CurveGeom &geom, const QPointF &pos)
{
  PointHit hit;
  hit.valid = false;
  hit.right = false;
  hit.index = -1;
  hit.count = 0;
  if(!geom.ok){
    return hit;
  }
  float best = 9.f;
  for(int side = 0; side < 2; ++side){
    const bool right = side == 1;
    ltr_curve_pt pts[LTR_CURVE_MAX_POINTS];
    const int count = TRACKER.axisGetCurve(axis, right, pts, LTR_CURVE_MAX_POINTS);
    for(int i = 0; i < count; ++i){
      const QPointF at = controlPointPixel(pts[i], right, invert, geom);
      const float dx = static_cast<float>(pos.x()) - static_cast<float>(at.x());
      const float dy = static_cast<float>(pos.y()) - static_cast<float>(at.y());
      const float dist = std::sqrt(dx * dx + dy * dy);
      if(dist < best){
        best = dist;
        hit.valid = true;
        hit.right = right;
        hit.index = i;
        hit.count = count;
      }
    }
  }
  return hit;
}

} // namespace

void SCView::mousePressEvent(QMouseEvent *event)
{
  setFocus(Qt::MouseFocusReason);
  const QPointF pos = event->position();
  const CurveGeom geom = buildCurveGeom(axis, fontMetrics(), size());
  const PointHit hit = hitTestPoint(axis, invert, geom, pos);
  const bool endpoint = hit.valid && (hit.index == 0 || hit.index + 1 == hit.count);

  if(event->button() == Qt::RightButton){
    if(hit.valid && !endpoint){
      selectPoint(hit.right, hit.index);
      removeSelectedPoint();
    }
    event->accept();
    return;
  }

  if(event->button() != Qt::LeftButton){
    QWidget::mousePressEvent(event);
    return;
  }

  if(hit.valid){
    selectPoint(hit.right, hit.index);
    if(!endpoint){
      dragging = true;
      dragRight = hit.right;
      dragIndex = hit.index;
      dragCount = TRACKER.axisGetCurve(axis, dragRight, dragPts, LTR_CURVE_MAX_POINTS);
      grabMouse();
    }
    event->accept();
    return;
  }

  if(!hit.valid && geom.ok){
    bool right = false;
    float nx = 0.f;
    float ny = 0.f;
    if(pixelToNormalized(pos, invert, geom, false, right, nx, ny) &&
       nx > LTR_CURVE_MIN_GAP && nx < 1.f - LTR_CURVE_MIN_GAP){
      ltr_curve_pt pts[LTR_CURVE_MAX_POINTS];
      const int count = TRACKER.axisGetCurve(axis, right, pts, LTR_CURVE_MAX_POINTS);
      bool crowded = false;
      for(int i = 0; i < count; ++i){
        if(std::fabs(pts[i].x - nx) < LTR_CURVE_MIN_GAP){
          crowded = true;
          break;
        }
      }
      if(!crowded && count < LTR_CURVE_MAX_POINTS){
        pts[count].x = nx;
        pts[count].y = ny;
        applyHalf(right, pts, count + 1, true);
        /* Points are stored sorted by x; find the new index after apply. */
        const int newCount = TRACKER.axisGetCurve(axis, right, pts, LTR_CURVE_MAX_POINTS);
        int newIndex = -1;
        for(int i = 0; i < newCount; ++i){
          if(std::fabs(pts[i].x - nx) < LTR_CURVE_MIN_GAP * 0.5f){
            newIndex = i;
            break;
          }
        }
        if(newIndex >= 0){
          selectPoint(right, newIndex);
        }
      }else{
        clearSelection();
      }
    }else{
      clearSelection();
    }
  }
  event->accept();
}

void SCView::mouseMoveEvent(QMouseEvent *event)
{
  if(!dragging){
    QWidget::mouseMoveEvent(event);
    return;
  }
  const CurveGeom geom = buildCurveGeom(axis, fontMetrics(), size());
  bool right = dragRight;
  float nx = 0.f;
  float ny = 0.f;
  if(!pixelToNormalized(event->position(), invert, geom, true, right, nx, ny)){
    return;
  }
  if(dragIndex <= 0 || dragIndex + 1 >= dragCount){
    return;
  }
  const float minX = dragPts[dragIndex - 1].x + LTR_CURVE_MIN_GAP;
  const float maxX = dragPts[dragIndex + 1].x - LTR_CURVE_MIN_GAP;
  if(minX > maxX){
    return;
  }
  if(nx < minX) nx = minX;
  if(nx > maxX) nx = maxX;
  dragPts[dragIndex].x = nx;
  dragPts[dragIndex].y = ny;
  applyHalf(dragRight, dragPts, dragCount, false);
  selectedRight = dragRight;
  selectedIndex = dragIndex;
  emitSelectionChanged();
  update();
  event->accept();
}

void SCView::mouseReleaseEvent(QMouseEvent *event)
{
  if(dragging && event->button() == Qt::LeftButton){
    if(dragIndex > 0 && dragIndex + 1 < dragCount){
      applyHalf(dragRight, dragPts, dragCount, true);
      selectPoint(dragRight, dragIndex);
    }
    dragging = false;
    dragIndex = -1;
    dragCount = 0;
    releaseMouse();
    event->accept();
    return;
  }
  QWidget::mouseReleaseEvent(event);
}

void SCView::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace){
    if(selectionIsRemovable()){
      removeSelectedPoint();
      event->accept();
      return;
    }
  }
  if(event->key() == Qt::Key_Escape){
    clearSelection();
    event->accept();
    return;
  }
  QWidget::keyPressEvent(event);
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
      break;
  }
}

