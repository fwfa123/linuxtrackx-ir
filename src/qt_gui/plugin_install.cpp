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
  state(DONE), gui(ui), inst(NULL), dlfw(NULL), dlmfc(NULL), dlmfc140(NULL),
  poem1(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/poem1.txt")),
  poem2(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/poem2.txt")),
  gameData(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/gamedata.txt")),
  mfc42u(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc42u.dll")),
  mfc140u(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc140u.dll")),
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
  if(dlmfc140 != NULL){
    dlmfc140->close();
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
  if(dlmfc140 != NULL){
    dlmfc140->close();
    delete dlmfc140;
    dlmfc140 = NULL;
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
  // NEW: Check wine and winetricks compatibility before proceeding
  std::ostringstream s;
  s<<"Checking wine and winetricks compatibility for extractor...\n";
  ltr_int_log_message(s.str().c_str());
  
  // Check if wine is compatible (9.0+)
  if (!inst->isWineVersionCompatible(inst->selectBestWineVersion())) {
    QMessageBox::critical(NULL, QString::fromUtf8("Wine Version Incompatible"),
      QString::fromUtf8("The extractor requires Wine version 9.0 or higher.\n\n"
      "Current wine version is incompatible. Please install a newer version:\n\n"
      "Ubuntu/Debian/MX:\n"
      "  sudo apt install wine-staging\n"
      "  sudo update-alternatives --config wine\n\n"
      "Fedora:\n"
      "  sudo dnf install wine-staging\n\n"
      "Arch:\n"
      "  sudo pacman -S wine-staging\n\n"
      "After installation, restart the LinuxTrack GUI and try again.")
    );
    return;
  }
  
  // Check if winetricks is compatible (20240105-next+)
  if (!inst->isWinetricksCompatible()) {
    QMessageBox::critical(NULL, QString::fromUtf8("Winetricks Version Incompatible"),
      QString::fromUtf8("The extractor requires Winetricks version 20240105-next or higher.\n\n"
      "Current winetricks version is incompatible. Please update winetricks:\n\n"
      "Method 1 (Recommended):\n"
      "  wget https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks\n"
      "  chmod +x winetricks\n"
      "  sudo mv winetricks /usr/local/bin/\n\n"
      "Method 2:\n"
      "  sudo apt update && sudo apt install winetricks\n\n"
      "After updating, restart the LinuxTrack GUI and try again.")
    );
    return;
  }
  
  s.str(std::string(""));
  s<<"Wine and winetricks compatibility verified. Proceeding with installation...\n";
  ltr_int_log_message(s.str().c_str());
  
  // First check if all required firmware files already exist
  if(isTirFirmwareInstalled() && (isMfc140uInstalled() || isMfc42uInstalled())){
    // All firmware files exist, skip directly to Wine prefix selection
    installLinuxtrackWine();
    state = LTR_W;
    return;
  }
  
  // Otherwise, proceed with the normal extraction flow
  if(!isTirFirmwareInstalled()){
    state = TIR_FW;
    tirFirmwareInstall();
  }else if(!isMfc140uInstalled() && !isMfc42uInstalled()){
    // Try MFC140 first (modern approach), fallback to MFC42
    state = MFC;
    mfc140uInstall();
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

bool PluginInstall::isMfc140uInstalled()
{
  return QFile::exists(mfc140u);
}


void PluginInstall::installLinuxtrackWine()
{
  if(dlfw != NULL){
    dlfw->hide();
  }
  if(dlmfc != NULL){
    dlmfc->hide();
  }
  
  // Double-check that MFC libraries exist before proceeding
  if(!isMfc140uInstalled() && !isMfc42uInstalled()) {
    QMessageBox::warning(NULL, QString::fromUtf8("MFC Libraries Required"),
      QString::fromUtf8("MFC libraries are required but not found in the tir_firmware directory.\\n\\n"
      "Please install Visual C++ 2015-2022 MFC libraries first using one of these methods:\\n\\n"
      "1. Install via winetricks (Recommended):\\n"
      "   sudo apt install winetricks\\n"
      "   winetricks vcrun2015 vcrun2017 vcrun2019 vcrun2022\\n"
      "   # Then copy the DLL to LinuxTrack:\\n"
      "   sudo cp ~/.wine/drive_c/windows/system32/mfc140u.dll /usr/share/linuxtrack/tir_firmware/\\n\\n"
      "2. Install via package manager:\\n"
      "   Debian/Ubuntu/MX: sudo apt install wine-staging wine32:i386\\n"
      "   Fedora: sudo dnf install wine-staging wine-core wine-desktop\\n"
      "   Arch: sudo pacman -S wine-staging wine-mono wine-gecko\\n"
      "   # Then copy the DLL to LinuxTrack:\\n"
      "   sudo cp ~/.wine/drive_c/windows/system32/mfc140u.dll /usr/share/linuxtrack/tir_firmware/\\n\\n"
      "3. Manual installation:\\n"
      "   Download Visual C++ 2015-2022 Redistributable from Microsoft\\n"
      "   Install in Wine and copy mfc140u.dll to:\\n"
      "   sudo cp mfc140u.dll /usr/share/linuxtrack/tir_firmware/\\n\\n"
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

void PluginInstall::mfc140uInstall()
{
  if(!isTirFirmwareInstalled()){
    QMessageBox::warning(NULL, QString::fromUtf8("Mfc140u install"),
                         QString::fromUtf8("Install TrackIR firmware first!"));
    state = TIR_FW;
    tirFirmwareInstall();
    return;
  }
  if(dlmfc140 == NULL){
    dlmfc140 = new Mfc140uExtractor();
    QObject::connect(dlmfc140, SIGNAL(finished(bool)),
      this, SLOT(finished(bool)));
  }
  
  // Always show the dialog first
  dlmfc140->show();
  
  // Then start automatic installation if in automatic flow
  if(state == MFC) {
    dlmfc140->startAutomaticInstallation();
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
  if(dlmfc140 != NULL){
    dlmfc140->hide();
  }
  switch(state){
    case TIR_FW:
      if(ok) {
        state = MFC;
        mfc140uInstall(); // Try MFC140 first (modern approach)
      } else {
        // TIR firmware extraction failed - don't proceed
        state = DONE;
        enableButtons(true);
      }
      break;
    case MFC:
      if(ok) {
        // MFC140 installation successful - proceed to Wine bridge installation
        state = LTR_W;
        installLinuxtrackWine();
      } else {
        // MFC140 extraction failed - try MFC42 fallback
        if(!isMfc42uInstalled()) {
          QMessageBox::information(NULL, QString::fromUtf8("MFC140 Installation Failed"),
            QString::fromUtf8("Visual C++ 2015-2022 MFC installation failed. Trying MFC42 fallback..."));
          mfc42uInstall();
        } else {
          // MFC42 already exists, proceed to Wine bridge installation
          state = LTR_W;
          installLinuxtrackWine();
        }
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

