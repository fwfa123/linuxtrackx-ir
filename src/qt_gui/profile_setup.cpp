#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include "profile_setup.h"
#include "scp_form.h"
#include "tracker.cpp"
#include "ltr_profiles.h"


ProfileSetup::ProfileSetup(const QString &name, QWidget *parent) : QWidget(parent), sc(NULL), profileName(name),
  initializing(true)
{
  ui.setupUi(this);
  connect();
  sc = new ScpForm();
  TRACKER.setProfile(profileName);
  initializing = false;
}


ProfileSetup::~ProfileSetup()
{
  delete sc;
}

void ProfileSetup::connect()
{
  QObject::connect(&TRACKER, SIGNAL(initAxes(void)), this, SLOT(initAxes(void)));
  QObject::connect(&TRACKER, SIGNAL(axisChanged(int, int)), this, SLOT(axisChanged(int, int)));
  QObject::connect(&TRACKER, SIGNAL(setCommonFF(float)), this, SLOT(setCommonFF(float)));
  
  // Connect Detail Axes Setup controls explicitly (auto-connect broken due to dynamic widget creation)
  // Enable checkboxes
  QObject::connect(ui.PitchEnable, SIGNAL(stateChanged(int)), this, SLOT(on_PitchEnable_stateChanged(int)));
  QObject::connect(ui.YawEnable, SIGNAL(stateChanged(int)), this, SLOT(on_YawEnable_stateChanged(int)));
  QObject::connect(ui.RollEnable, SIGNAL(stateChanged(int)), this, SLOT(on_RollEnable_stateChanged(int)));
  QObject::connect(ui.TxEnable, SIGNAL(stateChanged(int)), this, SLOT(on_TxEnable_stateChanged(int)));
  QObject::connect(ui.TyEnable, SIGNAL(stateChanged(int)), this, SLOT(on_TyEnable_stateChanged(int)));
  QObject::connect(ui.TzEnable, SIGNAL(stateChanged(int)), this, SLOT(on_TzEnable_stateChanged(int)));
  
  // Invert checkboxes
  QObject::connect(ui.PitchInvert, SIGNAL(stateChanged(int)), this, SLOT(on_PitchInvert_stateChanged(int)));
  QObject::connect(ui.YawInvert, SIGNAL(stateChanged(int)), this, SLOT(on_YawInvert_stateChanged(int)));
  QObject::connect(ui.RollInvert, SIGNAL(stateChanged(int)), this, SLOT(on_RollInvert_stateChanged(int)));
  QObject::connect(ui.TxInvert, SIGNAL(stateChanged(int)), this, SLOT(on_TxInvert_stateChanged(int)));
  QObject::connect(ui.TyInvert, SIGNAL(stateChanged(int)), this, SLOT(on_TyInvert_stateChanged(int)));
  QObject::connect(ui.TzInvert, SIGNAL(stateChanged(int)), this, SLOT(on_TzInvert_stateChanged(int)));
  
  // Sensitivity sliders
  QObject::connect(ui.PitchSens, SIGNAL(valueChanged(int)), this, SLOT(on_PitchSens_valueChanged(int)));
  QObject::connect(ui.YawSens, SIGNAL(valueChanged(int)), this, SLOT(on_YawSens_valueChanged(int)));
  QObject::connect(ui.RollSens, SIGNAL(valueChanged(int)), this, SLOT(on_RollSens_valueChanged(int)));
  QObject::connect(ui.TxSens, SIGNAL(valueChanged(int)), this, SLOT(on_TxSens_valueChanged(int)));
  QObject::connect(ui.TySens, SIGNAL(valueChanged(int)), this, SLOT(on_TySens_valueChanged(int)));
  QObject::connect(ui.TzSens, SIGNAL(valueChanged(int)), this, SLOT(on_TzSens_valueChanged(int)));
  
  // Smoothing slider
  QObject::connect(ui.Smoothing, SIGNAL(valueChanged(int)), this, SLOT(on_Smoothing_valueChanged(int)));
  
  // Detailed Axis Setup button
  QObject::connect(ui.DetailedAxisSetup, SIGNAL(pressed()), this, SLOT(on_DetailedAxisSetup_pressed()));
}

bool ProfileSetup::close()
{
  sc->close();
  return QWidget::close();
}

void ProfileSetup::on_DetailedAxisSetup_pressed()
{
  sc->show();
}

void ProfileSetup::copyFromDefault()
{
  if(QMessageBox::warning(this, tr("Warning:"), 
        tr("Are you sure, you want to overwrite the current profile by default values?"), 
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) != QMessageBox::Ok){
    return;
  }
  TRACKER.fromDefault();
}

void ProfileSetup::initAxes()
{
  ui.PitchEnable->setCheckState(TRACKER.axisGetBool(PITCH, AXIS_ENABLED)?Qt::Checked:Qt::Unchecked);
  ui.YawEnable->setCheckState(TRACKER.axisGetBool(YAW, AXIS_ENABLED)?Qt::Checked:Qt::Unchecked);
  ui.RollEnable->setCheckState(TRACKER.axisGetBool(ROLL, AXIS_ENABLED)?Qt::Checked:Qt::Unchecked);
  ui.TxEnable->setCheckState(TRACKER.axisGetBool(TX, AXIS_ENABLED)?Qt::Checked:Qt::Unchecked);
  ui.TyEnable->setCheckState(TRACKER.axisGetBool(TY, AXIS_ENABLED)?Qt::Checked:Qt::Unchecked);
  ui.TzEnable->setCheckState(TRACKER.axisGetBool(TZ, AXIS_ENABLED)?Qt::Checked:Qt::Unchecked);
  ui.PitchInvert->setCheckState(TRACKER.axisGetBool(PITCH, AXIS_INVERTED)?Qt::Checked:Qt::Unchecked);
  ui.YawInvert->setCheckState(TRACKER.axisGetBool(YAW, AXIS_INVERTED)?Qt::Checked:Qt::Unchecked);
  ui.RollInvert->setCheckState(TRACKER.axisGetBool(ROLL, AXIS_INVERTED)?Qt::Checked:Qt::Unchecked);
  ui.TxInvert->setCheckState(TRACKER.axisGetBool(TX, AXIS_INVERTED)?Qt::Checked:Qt::Unchecked);
  ui.TyInvert->setCheckState(TRACKER.axisGetBool(TY, AXIS_INVERTED)?Qt::Checked:Qt::Unchecked);
  ui.TzInvert->setCheckState(TRACKER.axisGetBool(TZ, AXIS_INVERTED)?Qt::Checked:Qt::Unchecked);
  ui.PitchSens->setValue(TRACKER.axisGet(PITCH, AXIS_MULT) * 12.0);
  ui.RollSens->setValue(TRACKER.axisGet(ROLL, AXIS_MULT) * 12.0);
  ui.YawSens->setValue(TRACKER.axisGet(YAW, AXIS_MULT) * 12.0);
  ui.TxSens->setValue(TRACKER.axisGet(TX, AXIS_MULT) * 12.0);
  ui.TySens->setValue(TRACKER.axisGet(TY, AXIS_MULT) * 12.0);
  ui.TzSens->setValue(TRACKER.axisGet(TZ, AXIS_MULT) * 12.0);
  ui.Smoothing->setValue(TRACKER.getCommonFilterFactor() * ui.Smoothing->maximum());
}



void ProfileSetup::axisChanged(int axis, int elem)
{
  if(elem == AXIS_MULT){
    switch(axis){
      case PITCH:
        ui.PitchSens->setValue(TRACKER.axisGet(PITCH, AXIS_MULT) * 12.0);
        break;
      case YAW:
        ui.YawSens->setValue(TRACKER.axisGet(YAW, AXIS_MULT) * 12.0);
        break;
      case ROLL:
        ui.RollSens->setValue(TRACKER.axisGet(ROLL, AXIS_MULT) * 12.0);
        break;
      case TX:
        ui.TxSens->setValue(TRACKER.axisGet(TX, AXIS_MULT) * 12.0);
        break;
      case TY:
        ui.TySens->setValue(TRACKER.axisGet(TY, AXIS_MULT) * 12.0);
        break;
      case TZ:
        ui.TzSens->setValue(TRACKER.axisGet(TZ, AXIS_MULT) * 12.0);
        break;
    }
  }
}

void ProfileSetup::setCommonFF(float val)
{
  //std::cout<<"Setting common ff "<<val<<"\n";
  ui.Smoothing->setValue(val * ui.Smoothing->maximum());
}

void ProfileSetup::on_PitchEnable_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(PITCH, AXIS_ENABLED, (state == Qt::Checked));
}

void ProfileSetup::on_YawEnable_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(YAW, AXIS_ENABLED, (state == Qt::Checked));
}

void ProfileSetup::on_RollEnable_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(ROLL, AXIS_ENABLED, (state == Qt::Checked));
}

void ProfileSetup::on_TxEnable_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(TX, AXIS_ENABLED, (state == Qt::Checked));
}

void ProfileSetup::on_TyEnable_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(TY, AXIS_ENABLED, (state == Qt::Checked));
}

void ProfileSetup::on_TzEnable_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(TZ, AXIS_ENABLED, (state == Qt::Checked));
}

void ProfileSetup::on_PitchInvert_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(PITCH, AXIS_INVERTED, (state == Qt::Checked));
}

void ProfileSetup::on_YawInvert_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(YAW, AXIS_INVERTED, (state == Qt::Checked));
}

void ProfileSetup::on_RollInvert_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(ROLL, AXIS_INVERTED, (state == Qt::Checked));
}

void ProfileSetup::on_TxInvert_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(TX, AXIS_INVERTED, (state == Qt::Checked));
}

void ProfileSetup::on_TyInvert_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(TY, AXIS_INVERTED, (state == Qt::Checked));
}

void ProfileSetup::on_TzInvert_stateChanged(int state)
{
  if(!initializing) TRACKER.axisChange(TZ, AXIS_INVERTED, (state == Qt::Checked));
}

void ProfileSetup::on_PitchSens_valueChanged(int val)
{
  if(!initializing) TRACKER.axisChange(PITCH, AXIS_MULT, val / 12.0f);
}

void ProfileSetup::on_YawSens_valueChanged(int val)
{
  if(!initializing) TRACKER.axisChange(YAW, AXIS_MULT, val / 12.0f);
}

void ProfileSetup::on_RollSens_valueChanged(int val)
{
  if(!initializing) TRACKER.axisChange(ROLL, AXIS_MULT, val / 12.0f);
}

void ProfileSetup::on_TxSens_valueChanged(int val)
{
  if(!initializing) TRACKER.axisChange(TX, AXIS_MULT, val / 12.0f);
}

void ProfileSetup::on_TySens_valueChanged(int val)
{
  if(!initializing) TRACKER.axisChange(TY, AXIS_MULT, val / 12.0f);
}

void ProfileSetup::on_TzSens_valueChanged(int val)
{
  if(!initializing) TRACKER.axisChange(TZ, AXIS_MULT, val / 12.0f);
}

void ProfileSetup::on_Smoothing_valueChanged(int val)
{
  if(!initializing) TRACKER.setCommonFilterFactor((float)val / ui.Smoothing->maximum());
}

void ProfileSetup::importProfile(QTextStream &tf)
{
  int version;
  int ival;
  float fval;
  tf>>version;
  tf>>fval;
  TRACKER.setCommonFilterFactor(fval);
  for(int i = PITCH; i <= TZ; ++i){
    tf>>ival;
    TRACKER.axisChange((axis_t)i, AXIS_ENABLED, ival != 0);
    tf>>ival;
    TRACKER.axisChange((axis_t)i, AXIS_INVERTED, ival != 0);
    for(int j = AXIS_DEADZONE; j <= AXIS_FILTER; ++j){
      tf>>fval;
      TRACKER.axisChange((axis_t)i, (axis_param_t)j, fval);
    }
  }
  
}

void ProfileSetup::exportProfile(QTextStream &tf)
{
  tf<<profileName<<"\n";
  tf<<"1"<<"\n"; //Version of the profile format
  tf<<TRACKER.getCommonFilterFactor()<<"\n";
  for(int i = PITCH; i <= TZ; ++i){
    tf<<(TRACKER.axisGetBool((axis_t)i, AXIS_ENABLED) ? QString::fromUtf8("1") : QString::fromUtf8("0"))<<
      QString::fromUtf8(" ");
    tf<<(TRACKER.axisGetBool((axis_t)i, AXIS_INVERTED) ? QString::fromUtf8("1") : QString::fromUtf8("0"))<<
      QString::fromUtf8(" ");
    for(int j = AXIS_DEADZONE; j < AXIS_FILTER; ++j){
      tf<<TRACKER.axisGet((axis_t)i, (axis_param_t)j)<<QString::fromUtf8(" ");
    }
    tf<<TRACKER.axisGet((axis_t)i, AXIS_FILTER)<<"\n";
  }
}


