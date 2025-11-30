#include "guardian.h"
#include "device_setup.h"
#include "prefs_link.h"
#include "ltr_model.h"
#include <cstdio>
#include <QMessageBox>
#include <QTimer>

Guardian::Guardian(QWidget *parent) : parentWidget(parent), mdlType(-1), devType(-1), 
  devDesc(QString::fromUtf8(""))
{
}

void Guardian::regTgt(ModelEdit *me)
{
  QObject::connect(me, SIGNAL(modelSelected(int)), this, SLOT(modelSelected(int)));
}

void Guardian::regTgt(DeviceSetup *ds)
{
  QObject::connect(ds, SIGNAL(deviceTypeChanged(int, const QString &)), 
                   this, SLOT(deviceTypeChanged(int, const QString &)));
}

void Guardian::checkDeviceNModel()
{
  // Qt6: Defer message box display to avoid crashes during widget initialization
  // The parent widget may not be fully ready when this is called from ModelEdit constructor
  // Capture values by value to avoid race conditions if device/model changes before lambda executes
  int currentDevType = devType;
  int currentMdlType = mdlType;
  QString currentDevDesc = devDesc;
  QWidget *currentParent = parentWidget;
  
  QTimer::singleShot(100, this, [currentDevType, currentMdlType, currentDevDesc, currentParent]() {
    // Qt6.5: Guard against null parentWidget and null this
    if(currentParent == nullptr){
      return;
    }
    // Note: 'this' is captured by value in the lambda context, but we validate parentWidget
    
    if((currentDevType == WEBCAM_FT) || (currentDevType == MACWEBCAM_FT) || (currentDevType == MACPS3EYE_FT)){
      //face tracker needs face model
      if(currentMdlType != MDL_FACE){
        QMessageBox::warning(currentParent, QObject::tr("Linuxtrack"),
                             QObject::tr("%1 requires Face type Model!").arg(currentDevDesc), QMessageBox::Ok);
      }
    }else if(currentDevType == JOYSTICK){
      //face tracker needs face model
      if(currentMdlType != MDL_ABSOLUTE){
        QMessageBox::warning(currentParent, QObject::tr("Linuxtrack"),
                             QObject::tr("%1 requires Absolute type Model!").arg(currentDevDesc), QMessageBox::Ok);
      }
    }else{
      //ordinary tracker needs other than face model
      if(currentMdlType == MDL_FACE){
        QMessageBox::warning(currentParent, QObject::tr("Linuxtrack"),
                             QObject::tr("%1 won't work correctly with Face type Model!").arg(currentDevDesc), 
                             QMessageBox::Ok);
      }else if(currentMdlType == MDL_ABSOLUTE){
        QMessageBox::warning(currentParent, QObject::tr("Linuxtrack"),
                             QObject::tr("%1 won't work correctly with Absolute type Model!").arg(currentDevDesc), 
                             QMessageBox::Ok);
      }
    }
  });
}

void Guardian::modelSelected(int modelType)
{
  mdlType = modelType;
  if(devType == -1){
    return;
  }
  checkDeviceNModel();
}
  
void Guardian::deviceTypeChanged(int deviceType, const QString &desc)
{
  devType = deviceType;
  devDesc = desc;
  if(mdlType == -1){
    return;
  }
  checkDeviceNModel();
}
  

