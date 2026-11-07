#include "scp_form.h"

#include <QShowEvent>
#include <QSizePolicy>
#include <QSplitter>
#include <QTimer>

#include "tracker.h"

namespace {

axis_t axisForListIndex(int index)
{
  switch(index){
    case 0: return PITCH;
    case 1: return YAW;
    case 2: return ROLL;
    case 3: return TX;
    case 4: return TY;
    case 5: return TZ;
    default: return YAW;
  }
}

const int kDefaultMapsColumnWidth = 220;

}

ScpForm::ScpForm(QWidget *parent)
  : QWidget(parent),
    yaw(NULL), pitch(NULL), roll(NULL), x(NULL), y(NULL), z(NULL),
    embedded(false), savedMapsColumnWidth(kDefaultMapsColumnWidth)
{
  ui.setupUi(this);
  setupCurves();
  setupCurveSplitter();

  QObject::connect(ui.poseCrosshairs, SIGNAL(expansionChanged(bool)),
    this, SLOT(onPoseMapsExpansionChanged(bool)));

  applyPoseMapsColumn(ui.poseCrosshairs->isExpanded());
  on_AxisList_currentRowChanged(ui.AxisList->currentRow());
}

void ScpForm::setEmbedded(bool embed)
{
  embedded = embed;
  ui.SCPCloseButton->setVisible(!embed);
  if(embed){
    setWindowFlags(Qt::Widget);
    /* Width flexible; floor height so Profile axesScroll can scroll on short tabs. */
    setMinimumWidth(0);
    setMinimumHeight(480);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  }
  if(ui.curveEditorPane != NULL){
    ui.curveEditorPane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  }
  if(ui.poseCrosshairs != NULL){
    ui.poseCrosshairs->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  }
}

void ScpForm::setupCurveSplitter()
{
  ui.curveSplitter->setStretchFactor(0, 1);
  ui.curveSplitter->setStretchFactor(1, 0);
  ui.curveSplitter->setChildrenCollapsible(false);
}

void ScpForm::applyPoseMapsColumn(bool expanded)
{
  if(ui.poseCrosshairs == NULL || ui.curveSplitter == NULL){
    return;
  }

  ui.showPoseMapsButton->setVisible(!expanded);

  if(expanded){
    ui.poseCrosshairs->setVisible(true);
    ui.poseCrosshairs->setMinimumWidth(200);
    ui.poseCrosshairs->setMaximumWidth(280);
    QList<int> sizes = ui.curveSplitter->sizes();
    int total = 0;
    for(int i = 0; i < sizes.size(); ++i){
      total += sizes.at(i);
    }
    if(total <= 0){
      total = width() > 0 ? width() : 900;
    }
    int mapsWidth = savedMapsColumnWidth;
    if(mapsWidth < 200){
      mapsWidth = kDefaultMapsColumnWidth;
    }
    if(mapsWidth > 280){
      mapsWidth = 280;
    }
    const int editorWidth = qMax(400, total - mapsWidth);
    ui.curveSplitter->setSizes(QList<int>() << editorWidth << mapsWidth);
  }else{
    QList<int> sizes = ui.curveSplitter->sizes();
    if(sizes.size() >= 2 && sizes.at(1) > 40){
      savedMapsColumnWidth = sizes.at(1);
    }
    ui.poseCrosshairs->setMinimumWidth(0);
    ui.poseCrosshairs->setMaximumWidth(0);
    ui.poseCrosshairs->setVisible(false);
    QList<int> sizesNow = ui.curveSplitter->sizes();
    int total = 0;
    for(int i = 0; i < sizesNow.size(); ++i){
      total += sizesNow.at(i);
    }
    if(total <= 0){
      total = width() > 0 ? width() : 900;
    }
    ui.curveSplitter->setSizes(QList<int>() << total << 0);
  }
}

void ScpForm::onPoseMapsExpansionChanged(bool expanded)
{
  applyPoseMapsColumn(expanded);
}

void ScpForm::on_showPoseMapsButton_clicked()
{
  ui.poseCrosshairs->setExpanded(true);
}

void ScpForm::setupCurves()
{
  pitch = new SCurve(PITCH, QString::fromUtf8("Pitch - looking down/up"), QString::fromUtf8("Down"),
                     QString::fromUtf8("Up"), ui.curvePagePitch);
  ui.curvePagePitchLayout->addWidget(pitch, 1);

  yaw = new SCurve(YAW, QString::fromUtf8("Yaw - looking left/right"), QString::fromUtf8("Right"),
                   QString::fromUtf8("Left"), ui.curvePageYaw);
  ui.curvePageYawLayout->addWidget(yaw, 1);

  roll = new SCurve(ROLL, QString::fromUtf8("Roll - tilting head left/right"),
                    QString::fromUtf8("Clockwise"), QString::fromUtf8("Counter-clockwise"), ui.curvePageRoll);
  ui.curvePageRollLayout->addWidget(roll, 1);

  x = new SCurve(TX, QString::fromUtf8("Sideways translation"), QString::fromUtf8("Left"),
                 QString::fromUtf8("Right"), ui.curvePageTx);
  ui.curvePageTxLayout->addWidget(x, 1);

  y = new SCurve(TY, QString::fromUtf8("Up/down translation"), QString::fromUtf8("Down"),
                 QString::fromUtf8("Up"), ui.curvePageTy);
  ui.curvePageTyLayout->addWidget(y, 1);

  z = new SCurve(TZ, QString::fromUtf8("Back/forth translation"), QString::fromUtf8("Forward"),
                 QString::fromUtf8("Back"), ui.curvePageTz);
  ui.curvePageTzLayout->addWidget(z, 1);
}

ScpForm::~ScpForm()
{
}

void ScpForm::showEvent(QShowEvent *event)
{
  QWidget::showEvent(event);
  /* Defer splitter sizing until the widget has a real width. */
  QTimer::singleShot(0, this, [this]() {
    applyPoseMapsColumn(ui.poseCrosshairs->isExpanded());
  });
}

void ScpForm::on_AxisList_currentRowChanged(int row)
{
  if(row < 0){
    return;
  }
  ui.curveStack->setCurrentIndex(row);
  ui.poseMonitor->setActiveAxis(axisForListIndex(row));
}

void ScpForm::on_SCPRecenterButton_pressed()
{
  TRACKER.recenter();
}

void ScpForm::on_SCPCloseButton_pressed()
{
  close();
}
