#include "scp_form.h"
#include <iostream>
#include "ltr_profiles.h"
#include "pose_monitor_strip.h"

namespace {
axis_t axisForTabIndex(int index)
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
}

ScpForm::ScpForm(QWidget *parent) :QWidget(parent), poseMonitor(NULL)
{
  ui.setupUi(this);
  poseMonitor = new PoseMonitorStrip(this);
  ui.verticalLayout_2->insertWidget(0, poseMonitor);

  pitch = new SCurve(PITCH, QString::fromUtf8("Pitch - looking down/up"), QString::fromUtf8("Down"), 
                     QString::fromUtf8("Up"), this);
  yaw = new SCurve(YAW, QString::fromUtf8("Yaw - looking left/right"), QString::fromUtf8("Right"), 
                   QString::fromUtf8("Left"), this);
  roll = new SCurve(ROLL, QString::fromUtf8("Roll - tilting head left/right"), 
		    QString::fromUtf8("Clockwise"), QString::fromUtf8("Counter-clockwise"), this);
  x = new SCurve(TX, QString::fromUtf8("Sideways translation"), QString::fromUtf8("Left"), 
                 QString::fromUtf8("Right"), this);
  y = new SCurve(TY, QString::fromUtf8("Up/down translation"), QString::fromUtf8("Down"), 
                 QString::fromUtf8("Up"), this);
  z = new SCurve(TZ, QString::fromUtf8("Back/forth translation"), QString::fromUtf8("Forth"), 
                 QString::fromUtf8("Back"), this);
  ui.SCPPitch->addWidget(pitch);
  ui.SCPYaw->addWidget(yaw);
  ui.SCPRoll->addWidget(roll);
  ui.SCPX->addWidget(x);
  ui.SCPY->addWidget(y);
  ui.SCPZ->addWidget(z);

  QObject::connect(ui.SCPTab, SIGNAL(currentChanged(int)), this, SLOT(on_SCPTab_currentChanged(int)));
  on_SCPTab_currentChanged(ui.SCPTab->currentIndex());
}

void ScpForm::on_SCPTab_currentChanged(int index)
{
  if(poseMonitor != NULL){
    poseMonitor->setActiveAxis(axisForTabIndex(index));
  }
}

ScpForm::~ScpForm()
{
  delete yaw;
  delete pitch;
  delete roll;
  delete x;
  delete y;
  delete z;
}

void ScpForm::on_SCPCloseButton_pressed()
{
  close();
}


