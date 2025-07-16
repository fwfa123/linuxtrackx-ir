#include <plugin_install.h>
#include <ltr_gui_prefs.h>
#include <iostream>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <zlib.h>
#include "extractor.h"
#include "utils.h"

#ifdef HAVE_CONFIG_H
  #include "../../config.h"
#endif

PluginInstall::PluginInstall(const Ui::LinuxtrackMainForm &ui, QObject *parent):QObject(parent),
  state(DONE), gui(ui), inst(NULL), dlfw(NULL), dlmfc(NULL),
  poem1(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/poem1.txt")),
  poem2(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/poem2.txt")),
  gameData(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/gamedata.txt")),
  mfc42u(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc42u.dll")),
  tirViews(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/TIRViews.dll"))
{
#ifndef DARWIN
  // Check for wine installer in the wine directory location
  QString wineInstallerPath = PREF.getDataPath(QString::fromUtf8("wine/linuxtrack-wine.exe"));
  if(!QFile::exists(wineInstallerPath)){
    enableButtons(false);
    return;
  }
#endif
  inst = new WineLauncher();
  gui.LinuxtrackWineButton->setEnabled(true);
  Connect();
}

void PluginInstall::close()
{
  if(dlfw != NULL){
    dlfw->close();
  }
  if(dlmfc != NULL){
    dlmfc->close();
  }
}

PluginInstall::~PluginInstall()
{
  if(dlfw != NULL){
    dlfw->close();
    delete dlfw;
    dlfw = NULL;
  }
  if(dlmfc != NULL){
    dlmfc->close();
    delete dlmfc;
    dlmfc = NULL;
  }
  delete inst;
}

void PluginInstall::Connect()
{
  QObject::connect(gui.LinuxtrackWineButton, SIGNAL(pressed()),
    this, SLOT(installWinePlugin()));
  QObject::connect(gui.TIRFWButton, SIGNAL(pressed()),
    this, SLOT(on_TIRFWButton_pressed()));
//  QObject::connect(gui.TIRViewsButton, SIGNAL(pressed()),
//    this, SLOT(on_TIRViewsButton_pressed()));
  QObject::connect(inst, SIGNAL(finished(bool)),
    this, SLOT(finished(bool)));
}

void PluginInstall::on_TIRFWButton_pressed()
{
  state = TIR_FW;
  tirFirmwareInstall();
}

/*
void PluginInstall::on_TIRViewsButton_pressed()
{
  state = MFC_ONLY;
  mfc42uInstall();
}
*/

void PluginInstall::installWinePlugin()
{
  // First check if all required firmware files already exist
  if(isTirFirmwareInstalled() && isMfc42uInstalled()){
    // All firmware files exist, skip directly to Wine prefix selection
    installLinuxtrackWine();
    state = LTR_W;
    return;
  }
  
  // Otherwise, proceed with the normal extraction flow
  if(!isTirFirmwareInstalled()){
    state = TIR_FW;
    tirFirmwareInstall();
  }else if(!isMfc42uInstalled()){
    state = MFC;
    mfc42uInstall();
  }else{
    installLinuxtrackWine();
    state = LTR_W;
  }
}



bool PluginInstall::isTirFirmwareInstalled()
{
  return QFile::exists(poem1) && QFile::exists(poem2) && QFile::exists(gameData) && QFile::exists(tirViews);
}

bool PluginInstall::isMfc42uInstalled()
{
  return QFile::exists(mfc42u);
}


void PluginInstall::installLinuxtrackWine()
{
  if(dlfw != NULL){
    dlfw->hide();
  }
  if(dlmfc != NULL){
    dlmfc->hide();
  }
  
  // Double-check that mfc42u.dll exists before proceeding
  if(!isMfc42uInstalled()) {
    QMessageBox::warning(NULL, QString::fromUtf8("MFC42 Required"),
      QString::fromUtf8("mfc42u.dll is required but not found in the tir_firmware directory.\\n\\n"
      "Please install MFC42 support first using one of these methods:\\n\\n"
      "1. Install via winetricks (Recommended for Debian/Ubuntu/MX):\\n"
      "   sudo apt install winetricks\\n"
      "   winetricks mfc42\\n"
      "   # Then copy the DLL to LinuxTrack:\\n"
      "   sudo cp ~/.wine/drive_c/windows/system32/mfc42u.dll /usr/share/linuxtrack/tir_firmware/\\n\\n"
      "2. Install via package manager (Fedora/RHEL/Arch only):\\n"
      "   Fedora: sudo dnf install mfc42\\n"
      "   Arch: sudo pacman -S mfc42\\n"
      "   # Then copy the DLL to LinuxTrack:\\n"
      "   sudo cp /usr/lib/mfc42u.dll /usr/share/linuxtrack/tir_firmware/\\n\\n"
      "3. Manual installation:\\n"
      "   Copy mfc42u.dll from Windows system to:\\n"
      "   sudo cp mfc42u.dll /usr/share/linuxtrack/tir_firmware/\\n\\n"
      "After copying the DLL, try the Wine support installation again.")
    );
    return;
  }
  
#ifndef DARWIN
  QString prefix = QFileDialog::getExistingDirectory(NULL, QString::fromUtf8("Select Wine Prefix..."),
                     QDir::homePath(), QFileDialog::ShowDirsOnly);
  
  // Use the wine directory location
  QString installerPath = PREF.getDataPath(QString::fromUtf8("wine/linuxtrack-wine.exe"));

  inst->setEnv(QString::fromUtf8("WINEPREFIX"), prefix);
  inst->run(installerPath);
#else
  if(isTirFirmwareInstalled() && isMfc42uInstalled()){
    QMessageBox::information(NULL, QString::fromUtf8("Firmware extraction successfull"),
      QString::fromUtf8("Firmware extraction finished successfully!"
      "\nNow you can install linuxtrack-wine.exe to the Wine bottle/prefix of your choice."
      )
    );
  }
#endif
  gui.LinuxtrackWineButton->setEnabled(true);
}

void PluginInstall::tirFirmwareInstall()
{
  if(dlfw == NULL){
    dlfw = new TirFwExtractor();
    QObject::connect(dlfw, SIGNAL(finished(bool)),
      this, SLOT(finished(bool)));
  }
  dlfw->show();
}

void PluginInstall::mfc42uInstall()
{
  if(!isTirFirmwareInstalled()){
    QMessageBox::warning(NULL, QString::fromUtf8("Mfc42u install"),
                         QString::fromUtf8("Install TrackIR firmware first!"));
    state = TIR_FW;
    tirFirmwareInstall();
    return;
  }
  if(dlmfc == NULL){
    dlmfc = new Mfc42uExtractor();
    QObject::connect(dlmfc, SIGNAL(finished(bool)),
      this, SLOT(finished(bool)));
  }
  
  // Always show the dialog first
  dlmfc->show();
  
  // Then start automatic installation if in automatic flow
  if(state == MFC) {
    dlmfc->startAutomaticInstallation();
  }
  // For manual installation, dialog is already shown and user can interact
}

void PluginInstall::finished(bool ok)
{
  if(dlfw != NULL){
    dlfw->hide();
  }
  if(dlmfc != NULL){
    dlmfc->hide();
  }
  switch(state){
    case TIR_FW:
      if(ok) {
        state = MFC;
        mfc42uInstall();
      } else {
        // TIR firmware extraction failed - don't proceed
        state = DONE;
        enableButtons(true);
      }
      break;
    case MFC:
      if(ok) {
        // Only proceed to Wine bridge installation if MFC42 was successfully installed
        state = LTR_W;
        installLinuxtrackWine();
      } else {
        // MFC42 extraction failed - don't proceed to Wine bridge
        // User needs to manually install mfc42u.dll first
        state = DONE;
        enableButtons(true);
      }
      break;
    case LTR_W:
    case TIR_FW_ONLY:
    case MFC_ONLY:
    default:
      state = DONE;
      enableButtons(true);
      break;
  }
}

void PluginInstall::enableButtons(bool ena)
{
  gui.LinuxtrackWineButton->setEnabled(ena);
  gui.TIRFWButton->setEnabled(ena);
  //gui.TIRViewsButton->setEnabled(ena);
}

#include "moc_plugin_install.cpp"

