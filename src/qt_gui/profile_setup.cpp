#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QSignalBlocker>
#include <QStringList>
#include "profile_setup.h"
#include "scp_form.h"
#include "tracker.cpp"
#include "ltr_profiles.h"


ProfileSetup::ProfileSetup(const QString &name, QWidget *parent) : QWidget(parent), sc(NULL), profileName(name),
  initializing(true)
{
  ui.setupUi(this);
  connect();
  sc = new ScpForm(this);
  sc->setEmbedded(true);
  ui.curvesHostLayout->addWidget(sc);
  TRACKER.setProfile(profileName);
  initializing = false;
}


ProfileSetup::~ProfileSetup()
{
  // ScpForm is a child widget; Qt destroys it with this.
  sc = NULL;
}

void ProfileSetup::loadProfile(const QString &name)
{
  // Qt6 RADICAL FIX: reuse existing widget for new profile instead of
  // deleting/recreating it (which was causing layout crashes).
  profileName = name;
  TRACKER.setProfile(profileName);
}

void ProfileSetup::connect()
{
  QObject::connect(&TRACKER, SIGNAL(initAxes(void)), this, SLOT(initAxes(void)));
  QObject::connect(&TRACKER, SIGNAL(setCommonFF(float)), this, SLOT(setCommonFF(float)));

  QObject::connect(ui.Smoothing, SIGNAL(valueChanged(int)), this, SLOT(on_Smoothing_valueChanged(int)));
  QObject::connect(ui.TrackingRate, SIGNAL(valueChanged(int)), this, SLOT(on_TrackingRate_valueChanged(int)));
}

bool ProfileSetup::close()
{
  return QWidget::close();
}

void ProfileSetup::on_CopyFromDefault_clicked()
{
  copyFromDefault();
}

void ProfileSetup::on_RestoreAxisDefaults_clicked()
{
  restoreAxisDefaults();
}

void ProfileSetup::on_SaveAxisDefaults_clicked()
{
  saveAxisDefaults();
}

void ProfileSetup::restoreAxisDefaults()
{
  if(QMessageBox::warning(this, tr("Warning:"),
        tr("Restore all axis settings on the current profile from your saved axis baseline?"),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) != QMessageBox::Ok){
    return;
  }
  initializing = true;
  TRACKER.restoreAxisDefaults();
  initializing = false;
}

void ProfileSetup::saveAxisDefaults()
{
  if(QMessageBox::warning(this, tr("Warning:"),
        tr("Save the current profile's axis settings as the rollback baseline? "
           "This does not change the Default profile used for new games."),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) != QMessageBox::Ok){
    return;
  }
  initializing = true;
  TRACKER.saveAxisDefaults();
  initializing = false;
}

void ProfileSetup::copyFromDefault()
{
  if(QMessageBox::warning(this, tr("Warning:"),
        tr("Are you sure, you want to overwrite the current profile by default values?"),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) != QMessageBox::Ok){
    return;
  }
  initializing = true;
  TRACKER.fromDefault();
  initializing = false;
}

void ProfileSetup::initAxes()
{
  const QSignalBlocker blockSmoothing(ui.Smoothing);

  ui.Smoothing->setValue(TRACKER.getCommonFilterFactor() * ui.Smoothing->maximum());

  int fps = TRACKER.getTrackingRateFps();
  {
    QSignalBlocker block(ui.TrackingRate);
    ui.TrackingRate->setValue(fps);
    ui.TrackingRateValue->setText(QString::number(fps));
  }
}

void ProfileSetup::setCommonFF(float val)
{
  const QSignalBlocker blockSmoothing(ui.Smoothing);
  ui.Smoothing->setValue(val * ui.Smoothing->maximum());
}

void ProfileSetup::on_TrackingRate_valueChanged(int val)
{
  if(!initializing){
    TRACKER.setTrackingRateFps(val);
    ui.TrackingRateValue->setText(QString::number(val));
  }
}

void ProfileSetup::on_Smoothing_valueChanged(int val)
{
  if(!initializing) TRACKER.setCommonFilterFactor((float)val / ui.Smoothing->maximum());
}

static QString formatCurveHalf(axis_t axis, bool right)
{
  ltr_curve_pt pts[LTR_CURVE_MAX_POINTS];
  const int count = TRACKER.axisGetCurve(axis, right, pts, LTR_CURVE_MAX_POINTS);
  QStringList parts;
  for(int i = 0; i < count; ++i){
    parts.append(QString::number(pts[i].x, 'f', 4) + QLatin1Char(',') +
                 QString::number(pts[i].y, 'f', 4));
  }
  if(parts.isEmpty()){
    return QString::fromUtf8(LTR_CURVE_DEFAULT_TEXT);
  }
  return parts.join(QLatin1Char(';'));
}

static bool isLegacyShapeParam(axis_param_t param)
{
  return param == AXIS_DEADZONE || param == AXIS_LCURV || param == AXIS_RCURV;
}

void ProfileSetup::importProfile(QTextStream &tf)
{
  int version = 1;
  int ival;
  float fval = 0.5f;
  tf >> version;
  if(tf.atEnd()){
    TRACKER.setCommonFilterFactor(fval);
  } else {
    tf >> fval;
    if(fval < 0.0f) fval = 0.0f;
    if(fval > 1.0f) fval = 1.0f;
    TRACKER.setCommonFilterFactor(fval);
  }
  for(int i = PITCH; i <= TZ; ++i){
    if(tf.atEnd()) break;
    tf>>ival;
    TRACKER.axisChange((axis_t)i, AXIS_ENABLED, ival != 0);
    if(tf.atEnd()) break;
    tf>>ival;
    TRACKER.axisChange((axis_t)i, AXIS_INVERTED, ival != 0);
    if(version >= 3){
      tf>>fval;
      TRACKER.axisChange((axis_t)i, AXIS_MULT, fval);
      tf>>fval;
      TRACKER.axisChange((axis_t)i, AXIS_LLIMIT, fval);
      tf>>fval;
      TRACKER.axisChange((axis_t)i, AXIS_RLIMIT, fval);
      tf>>fval;
      TRACKER.axisChange((axis_t)i, AXIS_FILTER, fval);
      QString leftPoints;
      QString rightPoints;
      tf >> leftPoints >> rightPoints;
      TRACKER.axisSetCurveText((axis_t)i, leftPoints, rightPoints);
    }else{
      for(int j = AXIS_DEADZONE; j <= AXIS_FILTER; ++j){
        if(tf.atEnd()) break;
        tf>>fval;
        if(!isLegacyShapeParam((axis_param_t)j)){
          TRACKER.axisChange((axis_t)i, (axis_param_t)j, fval);
        }
      }
      TRACKER.axisSetCurveText((axis_t)i,
                               QString::fromUtf8(LTR_CURVE_DEFAULT_TEXT),
                               QString::fromUtf8(LTR_CURVE_DEFAULT_TEXT));
    }
  }
  if (!tf.atEnd()) {
    int trackingRate = 120;
    tf >> trackingRate;
    TRACKER.setTrackingRateFps(trackingRate);
  }
}

void ProfileSetup::exportProfile(QTextStream &tf)
{
  tf<<profileName<<"\n";
  tf<<"3"<<"\n";
  tf<<TRACKER.getCommonFilterFactor()<<"\n";
  for(int i = PITCH; i <= TZ; ++i){
    tf<<(TRACKER.axisGetBool((axis_t)i, AXIS_ENABLED) ? QString::fromUtf8("1") : QString::fromUtf8("0"))<<
      QString::fromUtf8(" ");
    tf<<(TRACKER.axisGetBool((axis_t)i, AXIS_INVERTED) ? QString::fromUtf8("1") : QString::fromUtf8("0"))<<
      QString::fromUtf8(" ");
    tf<<TRACKER.axisGet((axis_t)i, AXIS_MULT)<<QString::fromUtf8(" ");
    tf<<TRACKER.axisGet((axis_t)i, AXIS_LLIMIT)<<QString::fromUtf8(" ");
    tf<<TRACKER.axisGet((axis_t)i, AXIS_RLIMIT)<<QString::fromUtf8(" ");
    tf<<TRACKER.axisGet((axis_t)i, AXIS_FILTER)<<QString::fromUtf8(" ");
    tf<<formatCurveHalf((axis_t)i, false)<<QString::fromUtf8(" ");
    tf<<formatCurveHalf((axis_t)i, true)<<"\n";
  }
  tf<<TRACKER.getTrackingRateFps()<<"\n";
}
