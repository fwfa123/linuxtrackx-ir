#include <iostream>
#include "ui_tir_setup.h"
#include "ltr_gui_prefs.h"
#include "tir_driver_prefs.h"
#include "ltr_gui_prefs.h"
#include "tir_prefs.h"
#include "pathconfig.h"
#include "dyn_load.h"
#include "trackir_permission_dialog.h"
#include <QFile>
#include <QMessageBox>
#include <QTimer>

static QString currentId = QString::fromUtf8("None");
static int tirType = 0;
bool TirPrefs::firmwareOK = false;
bool TirPrefs::permsOK = false;

typedef int (*probe_tir_fun_t)(bool *have_firmware, bool *have_permissions);
static probe_tir_fun_t probe_tir_fun = NULL;
static lib_fun_def_t functions[] = {
  {(char *)"ltr_int_tir_found", (void*) &probe_tir_fun},
  {NULL, NULL}
};


static int probeTir(bool &fwOK, bool &permOK)
{
  void *libhandle = NULL;
  int res = 0;
  if((libhandle = ltr_int_load_library((char *)"libtir", functions)) != NULL){
    res = probe_tir_fun(&fwOK, &permOK);
    ltr_int_unload_library(libhandle, functions);
  }
  return res;
}

/*
void TirPrefs::Connect()
{
  QObject::connect(ui.TirThreshold, SIGNAL(valueChanged(int)),
    this, SLOT(on_TirThreshold_valueChanged(int)));
  QObject::connect(ui.TirMinBlob, SIGNAL(valueChanged(int)),
    this, SLOT(on_TirMinBlob_valueChanged(int)));
  QObject::connect(ui.TirMaxBlob, SIGNAL(valueChanged(int)),
    this, SLOT(on_TirMaxBlob_valueChanged(int)));
  QObject::connect(ui.TirStatusBright, SIGNAL(valueChanged(int)),
    this, SLOT(on_TirStatusBright_valueChanged(int)));
  QObject::connect(ui.TirIrBright, SIGNAL(valueChanged(int)),
    this, SLOT(on_TirIrBright_valueChanged(int)));
  QObject::connect(ui.TirSignalizeStatus, SIGNAL(stateChanged(int)),
    this, SLOT(on_TirSignalizeStatus_stateChanged(int)));
  QObject::connect(ui.TirInstallFirmware, SIGNAL(pressed()),
    this, SLOT(on_TirInstallFirmware_pressed()));
}
*/

TirPrefs::TirPrefs(const QString &dev_id, QWidget *parent) : QWidget(parent), id(dev_id), dlfw(NULL)
{
  // Qt6 RADICAL FIX: Do NOT call ui.setupUi(this) in constructor
  // Widget is created without parent (parent = nullptr) to avoid Qt6 validation issues
  // UI file will be loaded later via SetupUI() after widget is inserted into layout
  //Connect();
  
  // Store device ID for later use
  currentId = dev_id;
  
  // Set initializing flag to prevent signal handlers from running
  initializing = true;
  
  // UI has not been set up yet
  uiSetupComplete = false;
  
  // Note: UI setup and preference initialization will be done in SetupUI()
  // which is called after widget is inserted into layout
}

void TirPrefs::SetupUI()
{
  // Qt6 RADICAL FIX: Load UI file AFTER widget is in layout
  // This avoids Qt6's strict validation during widget construction
  
  // Prevent multiple calls to SetupUI()
  if (uiSetupComplete) {
    std::cout << "[TirPrefs::SetupUI] UI already set up, skipping..." << std::endl;
    return;
  }
  
  // Ensure widget has a parent (should be set when inserted into layout)
  if (!parentWidget()) {
    std::cerr << "[TirPrefs::SetupUI] ERROR: Widget has no parent! SetupUI() must be called after widget is inserted into layout." << std::endl;
    return;
  }
  
  std::cout << "[TirPrefs::SetupUI] Loading UI file..." << std::endl;
  ui.setupUi(this);
  
  // Initialize preferences now that UI is loaded
  ltr_int_tir_init_prefs();
  
  // Qt6 RADICAL FIX: Keep widget visible - hiding and showing triggers layout issues
  // Widget is already in layout, so it's safe to leave it visible
  // We'll just ensure initializing flag prevents signal handlers from running
  setVisible(true);
  
  uiSetupComplete = true;
  std::cout << "[TirPrefs::SetupUI] UI file loaded and preferences initialized, widget visible" << std::endl;
}

TirPrefs::~TirPrefs()
{
  //std::cout<<"Destructing tirprefs!\n";
  if(dlfw != NULL){
    //std::cout<<"Closing dlfw!\n";
    dlfw->close();
    delete dlfw;
  }
}

bool TirPrefs::Activate(const QString &ID, bool init)
{
  initializing = init;
  QString sec;
  if(PREF.getFirstDeviceSection(QString::fromUtf8("Tir"), sec)){
    QString currentDev, currentSection;
    deviceType_t devType;
    if(!PREF.getActiveDevice(devType, currentDev, currentSection) || (sec !=currentSection)){
      PREF.activateDevice(sec);
    }
  }else{
    sec = QString::fromUtf8("TrackIR");
    initializing = false;
    if(PREF.createSection(sec)){
      PREF.addKeyVal(sec, QString::fromUtf8("Capture-device"), QString::fromUtf8("Tir"));
      PREF.addKeyVal(sec, QString::fromUtf8("Capture-device-id"), ID);
      PREF.addKeyVal(sec, QString::fromUtf8("Threshold"), QString::number(140));
      PREF.addKeyVal(sec, QString::fromUtf8("Min-blob"), QString::number(4));
      PREF.addKeyVal(sec, QString::fromUtf8("Max-blob"), QString::number(2500));
      PREF.addKeyVal(sec, QString::fromUtf8("Status-led-brightness"), QString::number(0));
      PREF.addKeyVal(sec, QString::fromUtf8("Ir-led-brightness"), QString::number(7));
      PREF.addKeyVal(sec, QString::fromUtf8("Status-signals"), QString::fromUtf8("on"));
      PREF.addKeyVal(sec, QString::fromUtf8("Grayscale"), QString::fromUtf8("on"));
      PREF.addKeyVal(sec, QString::fromUtf8("Video-on-delay"), QString::number(0));
      PREF.activateDevice(sec);
    }else{
      return false;
    }
  }
  // Preferences already initialized in constructor, just update currentId if needed
  if (currentId != ID) {
    currentId = ID;
  }
  printf("Type: %d\n", tirType);
  std::cout << "[TirPrefs::Activate] Device type: " << tirType << ", ID: " << ID.toStdString() << std::endl;
  
  // Qt6 RADICAL FIX: Defer UI initialization to next event loop iteration
  // This ensures all current operations complete and layout is stable before we modify widgets
  // Using QMetaObject::invokeMethod with queued connection ensures this happens after current call stack
  std::cout << "[TirPrefs::Activate] Scheduling UI initialization via queued connection..." << std::endl;
  QMetaObject::invokeMethod(this, "initializeUI", Qt::QueuedConnection, Q_ARG(QString, ID));
  
  std::cout << "[TirPrefs::Activate] UI initialization scheduled, returning true" << std::endl;
  return true;
}

void TirPrefs::initializeUI(const QString &/*ID*/)
{
  std::cout << "[TirPrefs::initializeUI] Starting UI initialization..." << std::endl;
  
  // Qt6: Ensure widget is in a layout and parent is ready before modifying widgets
  // Widget should have parent since it was inserted into layout before SetupUI() was called
  if (!parentWidget()) {
    std::cerr << "[TirPrefs::initializeUI] ERROR: Widget has no parent! This should not happen." << std::endl;
    initializing = false;
    return;
  }
  
  std::cout << "[TirPrefs::initializeUI] Widget is valid, parent exists, checking UI widgets..." << std::endl;
  
  // Validate critical UI widgets exist before accessing them
  // Note: After SetupUI() is called, widgets should always be ready
  // If they're not, something is seriously wrong (corrupted UI file, Qt bug, etc.)
  if (!ui.TirThreshold || !ui.TirMaxBlob || !ui.TirMinBlob) {
    std::cerr << "[TirPrefs::initializeUI] ERROR: Critical UI widgets not initialized!" << std::endl;
    std::cerr << "  TirThreshold: " << (ui.TirThreshold ? "OK" : "NULL") << std::endl;
    std::cerr << "  TirMaxBlob: " << (ui.TirMaxBlob ? "OK" : "NULL") << std::endl;
    std::cerr << "  TirMinBlob: " << (ui.TirMinBlob ? "OK" : "NULL") << std::endl;
    std::cerr << "  This indicates a serious problem - UI setup may have failed" << std::endl;
    // Mark as not initializing to prevent blocking, but UI will be uninitialized
    initializing = false;
    return;
  }
  std::cout << "[TirPrefs::initializeUI] All critical UI widgets validated, proceeding with initialization..." << std::endl;
    
    // Store values to set
    int threshold = ltr_int_tir_get_threshold();
    int maxBlob = ltr_int_tir_get_max_blob();
    int minBlob = ltr_int_tir_get_min_blob();
    int irBright = ltr_int_tir_get_ir_brightness();
    int statusBright = ltr_int_tir_get_status_brightness();
    bool statusIndication = ltr_int_tir_get_status_indication();
    bool useGrayscale = ltr_int_tir_get_use_grayscale();
    
    // Set all UI values with null checks
    if (ui.TirThreshold) ui.TirThreshold->setValue(threshold);
    if (ui.TirMaxBlob) ui.TirMaxBlob->setValue(maxBlob);
    if (ui.TirMinBlob) ui.TirMinBlob->setValue(minBlob);
    if (ui.TirIrBright) ui.TirIrBright->setValue(irBright);
    if (ui.TirStatusBright) ui.TirStatusBright->setValue(statusBright);
    
    Qt::CheckState state = statusIndication ? Qt::Checked : Qt::Unchecked;
    if (ui.TirSignalizeStatus) ui.TirSignalizeStatus->setCheckState(state);
    
    Qt::CheckState grayscaleState = useGrayscale ? Qt::Checked : Qt::Unchecked;
    if (ui.TirUseGrayscale) ui.TirUseGrayscale->setCheckState(grayscaleState);
    
    // Set firmware label
    if (ui.TirFwLabel) {
      if(firmwareOK){
        if((tirType < TIR4) || (tirType == TIR5V3)){
          ui.TirFwLabel->setText(QString::fromUtf8("Firmware not needed!"));
        }else{
          ui.TirFwLabel->setText(QString::fromUtf8("Firmware found!"));
          if (ui.TirInstallFirmware) {
            ui.TirInstallFirmware->setText(QString::fromUtf8("Reinstall Firmware"));
          }
        }
      }else{
        ui.TirFwLabel->setText(QString::fromUtf8("Firmware not found - TrackIr will not work!"));
        // Defer message box to avoid blocking during layout
        QTimer::singleShot(100, this, [this]() {
          if (this) {
            QMessageBox::warning(this, tr("TrackIR Firmware Installation"),
                tr("TrackIR device was found, but you don't have the firmware installed."));
          }
        });
      }
    }
    
    // Hide/show widgets based on device type
    if((tirType < TIR5) || (tirType == SMARTNAV4)){
      if (ui.TirIrBright) {
        ui.TirIrBright->setDisabled(true);
        ui.TirIrBright->setHidden(true);
      }
      if (ui.TirStatusBright) {
        ui.TirStatusBright->setDisabled(true);
        ui.TirStatusBright->setHidden(true);
      }
      if (ui.StatusBrightLabel) ui.StatusBrightLabel->setHidden(true);
      if (ui.StatusBrightLabelOff) ui.StatusBrightLabelOff->setHidden(true);
      if (ui.StatusBrightLabelBright) ui.StatusBrightLabelBright->setHidden(true);
      if (ui.IRBrightLabel) ui.IRBrightLabel->setHidden(true);
      if (ui.IRBrightLabelLow) ui.IRBrightLabelLow->setHidden(true);
      if (ui.IRBrightLabelHigh) ui.IRBrightLabelHigh->setHidden(true);
    }
    if(tirType != SMARTNAV4){
      if (ui.TirUseGrayscale) {
        ui.TirUseGrayscale->setDisabled(true);
        ui.TirUseGrayscale->setHidden(true);
      }
      if (ui.TirUseGrayscaleLabel) ui.TirUseGrayscaleLabel->setHidden(true);
    }
    if(tirType == SMARTNAV3){
      if (ui.TirThreshold) {
        ui.TirThreshold->setMinimum(40);
      }
      if (ui.TirThresholdMin) {
        ui.TirThresholdMin->setText(QString::fromUtf8("40"));
      }
    }else{
      if (ui.TirThreshold) {
        ui.TirThreshold->setMinimum(30);
      }
      if (ui.TirThresholdMin) {
        ui.TirThresholdMin->setText(QString::fromUtf8("30"));
      }
    }
    
    initializing = false;
    
    // Widget is already visible (set in SetupUI()), no need to show it again
    // All UI modifications are complete, widget should be fully functional now
    
  std::cout << "[TirPrefs::initializeUI] UI initialization complete, initializing flag set to false" << std::endl;
}



bool TirPrefs::AddAvailableDevices(QComboBox &combo, QWidget *parent)
{
  bool res = false;
  QString id;
  deviceType_t dt;
  bool tir_selected = false;

  tirType = probeTir(firmwareOK, permsOK);
  if(!permsOK){
    // Check if we should show the dialog
    if(TrackIRPermissionDialog::shouldShowDialog()){
      TrackIRPermissionDialog dialog(parent);
      dialog.exec();
    }
    // Continue anyway - user might have chosen to skip
  }
  if(tirType == 0){
    return res;
  }

  if(PREF.getActiveDevice(dt,id)){
    if(dt == TIR){
      tir_selected = true;
    }
  }

  PrefsLink *pl = new PrefsLink(TIR, QString::fromUtf8("Tir"));
  QVariant v;
  v.setValue(*pl);
  combo.addItem(QString::fromUtf8("TrackIR/SmartNav"), v);
  if(tir_selected){
    combo.setCurrentIndex(combo.count() - 1);
    res = true;
  }
  return res;
}

void TirPrefs::on_TirThreshold_valueChanged(int i)
{
  if(!initializing) ltr_int_tir_set_threshold(i);
}

void TirPrefs::on_TirMinBlob_valueChanged(int i)
{
  if(!initializing) ltr_int_tir_set_min_blob(i);
}

void TirPrefs::on_TirMaxBlob_valueChanged(int i)
{
  if(!initializing) ltr_int_tir_set_max_blob(i);
}

void TirPrefs::on_TirStatusBright_valueChanged(int i)
{
  if(!initializing) ltr_int_tir_set_status_brightness(i);
}

void TirPrefs::on_TirIrBright_valueChanged(int i)
{
  if(!initializing) ltr_int_tir_set_ir_brightness(i);
}

void TirPrefs::on_TirSignalizeStatus_stateChanged(int state)
{
  if(!initializing) ltr_int_tir_set_status_indication(state == Qt::Checked);
}

void TirPrefs::on_TirUseGrayscale_stateChanged(int state)
{
  if(!initializing) ltr_int_tir_set_use_grayscale(state == Qt::Checked);
}

void TirPrefs::TirFirmwareDLFinished(bool state)
{
  if(state){
    dlfw->hide();
    probeTir(firmwareOK, permsOK);
    if(firmwareOK){
      ui.TirFwLabel->setText(QString::fromUtf8("Firmware found!"));
      //ui.TirInstallFirmware->setDisabled(true);
      ui.TirInstallFirmware->setText(QString::fromUtf8("Reinstall Firmware"));
    }else{
      ui.TirFwLabel->setText(QString::fromUtf8("Firmware not found - TrackIr will not work!"));
    }
  }
}

void TirPrefs::on_TirInstallFirmware_pressed()
{
  if(dlfw == NULL){
    dlfw = new TirFwExtractor(this);
    QObject::connect(dlfw, SIGNAL(finished(bool)),
      this, SLOT(TirFirmwareDLFinished(bool)));
  }
  dlfw->show();
  dlfw->raise();
}


