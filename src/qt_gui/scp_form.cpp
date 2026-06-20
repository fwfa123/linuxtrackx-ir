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
  ui.SCPPitch->addWidget(pitch, 1);
  ui.SCPYaw->addWidget(yaw, 1);
  ui.SCPRoll->addWidget(roll, 1);
  ui.SCPX->addWidget(x, 1);
  ui.SCPY->addWidget(y, 1);
  ui.SCPZ->addWidget(z, 1);
  ui.verticalLayout_3->setStretch(0, 1);
  ui.verticalLayout_4->setStretch(0, 1);
  ui.verticalLayout_5->setStretch(0, 1);
  ui.verticalLayout_6->setStretch(0, 1);
  ui.verticalLayout_7->setStretch(0, 1);
  ui.verticalLayout_8->setStretch(0, 1);
  ui.verticalLayout_2->setStretch(1, 1);
  ui.SCPTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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


