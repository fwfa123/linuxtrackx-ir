#include <plugin_install.h>
#include <ltr_gui_prefs.h>
#include <iostream>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <zlib.h>
#include "extractor.h"
#include "utils.h"

#ifdef HAVE_CONFIG_H
  #include "../../config.h"
#endif

PluginInstall::PluginInstall(const Ui::LinuxtrackMainForm &ui, QObject *parent):QObject(parent),
  state(DONE), gui(ui), inst(NULL), dlfw(NULL), dlmfc140(NULL), lutrisIntegration(NULL), steamIntegration(NULL),
  isTirMfcOnlyInstallation(false),
  poem1(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/poem1.txt")),
  poem2(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/poem2.txt")),
  gameData(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/gamedata.txt")),
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
  Connect();
}

void PluginInstall::close()
{
  if(dlfw != NULL){
    dlfw->close();
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
  if(dlmfc140 != NULL){
    dlmfc140->close();
    delete dlmfc140;
    dlmfc140 = NULL;
  }
  delete inst;
  if (lutrisIntegration) {
    delete lutrisIntegration;
  }
  if (steamIntegration) {
    delete steamIntegration;
  }
}

void PluginInstall::Connect()
{
  // Note: LinuxtrackWineButton moved to Gaming tab as CustomPrefixButton
  // Connection handled in ltr_gui.cpp
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
  // Check if all required firmware files already exist
  if(isTirFirmwareInstalled() && isMfc140uInstalled()){
    // All firmware files exist, skip directly to Wine prefix selection
    installLinuxtrackWine();
    state = LTR_W;
    return;
  }
  
  // Otherwise, proceed with the normal extraction flow
  if(!isTirFirmwareInstalled()){
    state = TIR_FW;
    tirFirmwareInstall();
  }else if(!isMfc140uInstalled()){
    // Install MFC140 (modern approach only)
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



bool PluginInstall::isMfc140uInstalled()
{
  return QFile::exists(mfc140u);
}


void PluginInstall::installLinuxtrackWine()
{
  if(dlfw != NULL){
    dlfw->hide();
  }
  
  // Double-check that MFC140 library exists before proceeding
  if(!isMfc140uInstalled()) {
    // Get the main window by finding the top-level widget
    QWidget *parentWidget = nullptr;
    QObject *obj = parent();
    while (obj && !parentWidget) {
      parentWidget = qobject_cast<QWidget*>(obj);
      if (parentWidget && parentWidget->isWindow()) {
        break; // Found the main window
      }
      obj = obj->parent();
    }
    if (!parentWidget) {
      parentWidget = qobject_cast<QWidget*>(parent()); // Fallback
    }
    QMessageBox::warning(parentWidget, QString::fromUtf8("MFC140 Library Required"),
      QString::fromUtf8("MFC140 library is required but not found in the tir_firmware directory.\\n\\n"
      "Please install Visual C++ 2015-2022 MFC libraries first using the MFC140 installer.")
    );
    return;
  }
  
#ifndef DARWIN
  // Get the main window by finding the top-level widget
  QWidget *parentWidget = nullptr;
  QObject *obj = parent();
  while (obj && !parentWidget) {
    parentWidget = qobject_cast<QWidget*>(obj);
    if (parentWidget && parentWidget->isWindow()) {
      break; // Found the main window
    }
    obj = obj->parent();
  }
  if (!parentWidget) {
    parentWidget = qobject_cast<QWidget*>(parent()); // Fallback
  }
  QString prefix = QFileDialog::getExistingDirectory(parentWidget, QString::fromUtf8("Select Wine Prefix..."),
                     QDir::homePath(), QFileDialog::ShowDirsOnly);
  
  // Use the wine directory location
  QString installerPath = PREF.getDataPath(QString::fromUtf8("wine/linuxtrack-wine.exe"));

  inst->setEnv(QString::fromUtf8("WINEPREFIX"), prefix);
  inst->run(installerPath);
#else
  if(isTirFirmwareInstalled() && isMfc140uInstalled()){
    // Get the main window by finding the top-level widget
    QWidget *parentWidget = nullptr;
    QObject *obj = parent();
    while (obj && !parentWidget) {
      parentWidget = qobject_cast<QWidget*>(obj);
      if (parentWidget && parentWidget->isWindow()) {
        break; // Found the main window
      }
      obj = obj->parent();
    }
    if (!parentWidget) {
      parentWidget = qobject_cast<QWidget*>(parent()); // Fallback
    }
    QMessageBox::information(parentWidget, QString::fromUtf8("Firmware extraction successfull"),
      QString::fromUtf8("Firmware extraction finished successfully!"
      "\nNow you can install linuxtrack-wine.exe to the Wine bottle/prefix of your choice."
      )
    );
  }
#endif
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



void PluginInstall::mfc140uInstall()
{
  if(!isTirFirmwareInstalled()){
    // Get the main window by finding the top-level widget
    QWidget *parentWidget = nullptr;
    QObject *obj = parent();
    while (obj && !parentWidget) {
      parentWidget = qobject_cast<QWidget*>(obj);
      if (parentWidget && parentWidget->isWindow()) {
        break; // Found the main window
      }
      obj = obj->parent();
    }
    if (!parentWidget) {
      parentWidget = qobject_cast<QWidget*>(parent()); // Fallback
    }
    QMessageBox::warning(parentWidget, QString::fromUtf8("Mfc140u install"),
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
  
  // Show the dialog (same as TIR firmware installer)
  dlmfc140->show();
  
  // Don't start automatic installation - let user interact with UI first
  // The user can choose to browse for installer or directory
}

void PluginInstall::finished(bool ok)
{
  if(dlfw != NULL){
    dlfw->hide();
  }
  if(dlmfc140 != NULL){
    dlmfc140->hide();
  }
  switch(state){
    case TIR_FW:
      if(ok) {
        if (isTirMfcOnlyInstallation) {
          // For TIR/MFC140 only installation, go to TIR_MFC_ONLY state
          state = TIR_MFC_ONLY;
        } else {
          // For full installation, go to MFC state
          state = MFC;
        }
        mfc140uInstall();
      } else {
        // TIR firmware extraction failed - don't proceed
        state = DONE;
        enableButtons(true);
        isTirMfcOnlyInstallation = false;  // Reset flag
      }
      break;
    case MFC:
      if(ok) {
        // MFC140 installation successful - proceed to Wine bridge installation
        state = LTR_W;
        installLinuxtrackWine();
      } else {
        // MFC140 extraction failed - don't proceed
        state = DONE;
        enableButtons(true);
      }
      break;
    case TIR_MFC_ONLY:
      if(ok) {
        // MFC140 installation successful - show completion message and stop
        QMessageBox::information(getParentWidget(), QString::fromUtf8("Installation Complete"),
          QString::fromUtf8("TIR Firmware and MFC140 libraries have been successfully installed.\n\n") +
          QString::fromUtf8("You can now use the other buttons to install the wine bridge to specific platforms."));
        state = DONE;
        enableButtons(true);
        isTirMfcOnlyInstallation = false;  // Reset flag
      } else {
        // MFC140 extraction failed - don't proceed
        state = DONE;
        enableButtons(true);
        isTirMfcOnlyInstallation = false;  // Reset flag
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
  // Note: LinuxtrackWineButton moved to Gaming tab as CustomPrefixButton
  // Button state managed in ltr_gui.cpp
  gui.TIRFWButton->setEnabled(ena);
  //gui.TIRViewsButton->setEnabled(ena);
}

// Helper method to get parent widget
QWidget* PluginInstall::getParentWidget()
{
  QWidget *parentWidget = nullptr;
  QObject *obj = parent();
  while (obj && !parentWidget) {
    parentWidget = qobject_cast<QWidget*>(obj);
    if (parentWidget && parentWidget->isWindow()) {
      break; // Found the main window
    }
    obj = obj->parent();
  }
  if (!parentWidget) {
    parentWidget = qobject_cast<QWidget*>(parent()); // Fallback
  }
  return parentWidget;
}

// New method for TIR/MFC140 installation only
void PluginInstall::installTirFirmwareAndMfc140()
{
  // Set flag to indicate this is TIR/MFC140 only installation
  isTirMfcOnlyInstallation = true;
  
  // Check if all required firmware files already exist
  if(isTirFirmwareInstalled() && isMfc140uInstalled()){
    QMessageBox::information(getParentWidget(), QString::fromUtf8("Already Installed"),
      QString::fromUtf8("TIR Firmware and MFC140 libraries are already installed."));
    isTirMfcOnlyInstallation = false;  // Reset flag
    return;
  }
  
  // Otherwise, proceed with the normal extraction flow
  if(!isTirFirmwareInstalled()){
    state = TIR_FW;
    tirFirmwareInstall();
  } else if(!isMfc140uInstalled()){
    // Install MFC140 (modern approach only)
    state = TIR_MFC_ONLY;  // Use new state to prevent wine bridge installation
    mfc140uInstall();
  } else {
    // Both already installed
    QMessageBox::information(getParentWidget(), QString::fromUtf8("Already Installed"),
      QString::fromUtf8("TIR Firmware and MFC140 libraries are already installed."));
    isTirMfcOnlyInstallation = false;  // Reset flag
  }
}

// New method for wine bridge installation to custom prefix
void PluginInstall::installWineBridgeToCustomPrefix()
{
  // Check prerequisites
  if(!isTirFirmwareInstalled() || !isMfc140uInstalled()) {
    QMessageBox::warning(getParentWidget(), QString::fromUtf8("Prerequisites Required"),
      QString::fromUtf8("Please install TIR Firmware and MFC140 libraries first using the button above."));
    return;
  }
  
  // Proceed with wine bridge installation (existing functionality)
  installLinuxtrackWine();
}

// New method for Lutris wine bridge installation
void PluginInstall::installLutrisWineBridge()
{
  // Check prerequisites
  if(!isTirFirmwareInstalled() || !isMfc140uInstalled()) {
    QMessageBox::warning(getParentWidget(), QString::fromUtf8("Prerequisites Required"),
      QString::fromUtf8("Please install TIR Firmware and MFC140 libraries first using the button above."));
    return;
  }
  
  // Initialize Lutris integration if not already done
  if (!lutrisIntegration) {
    lutrisIntegration = new LutrisIntegration(this);
  }
  
  // Check if Lutris is installed
  if (!lutrisIntegration->isLutrisInstalled()) {
    QMessageBox::warning(getParentWidget(), QString::fromUtf8("Lutris Not Found"),
      QString::fromUtf8("Lutris is not installed or not properly configured.\n\n") +
      QString::fromUtf8("Error: ") + lutrisIntegration->getLastError());
    return;
  }
  
  // Get available Lutris games
  QList<LutrisGame> games = lutrisIntegration->getLutrisGames();
  if (games.isEmpty()) {
    QMessageBox::information(getParentWidget(), QString::fromUtf8("No Lutris Games Found"),
      QString::fromUtf8("No Wine-based games were found in Lutris.\n\n") +
      QString::fromUtf8("Please install some games in Lutris first, then try again.\n\n") +
      QString::fromUtf8("Debug Info: ") + lutrisIntegration->getDebugInfo());
    return;
  }
  
  // Create game selection dialog
  QStringList gameNames;
  QStringList gameSlugs;
  for (const LutrisGame &game : games) {
    QString displayName = game.game_name.isEmpty() ? game.game_slug : game.game_name;
    gameNames.append(displayName);
    gameSlugs.append(game.game_slug);
  }
  
  bool ok;
  QString selectedGame = QInputDialog::getItem(getParentWidget(),
    QString::fromUtf8("Select Lutris Game"),
    QString::fromUtf8("Choose a game to install Linuxtrack Wine Bridge:"),
    gameNames, 0, false, &ok);
  
  if (!ok || selectedGame.isEmpty()) {
    return; // User cancelled
  }
  
  // Find the selected game
  int selectedIndex = gameNames.indexOf(selectedGame);
  if (selectedIndex == -1) {
    QMessageBox::warning(getParentWidget(), QString::fromUtf8("Selection Error"),
      QString::fromUtf8("Invalid game selection."));
    return;
  }
  
  QString selectedSlug = gameSlugs[selectedIndex];
  
  // Show information dialog about interactive installation
  QMessageBox::information(getParentWidget(), QString::fromUtf8("Starting Interactive Installation"),
    QString::fromUtf8("Starting Linuxtrack Wine Bridge installation for: ") + selectedGame + QString::fromUtf8("\n\n") +
    QString::fromUtf8("The NSIS installer will open in a new window.\n") +
    QString::fromUtf8("Please follow the installation prompts in that window.\n\n") +
    QString::fromUtf8("Click OK to start the installation."));
  
  // Install to the selected game
  bool success = lutrisIntegration->installToLutrisGame(selectedSlug);
  
  if (success) {
    QMessageBox::information(getParentWidget(), QString::fromUtf8("Installation Started"),
      QString::fromUtf8("Linuxtrack Wine Bridge installer has been launched for: ") + selectedGame + QString::fromUtf8("\n\n") +
      QString::fromUtf8("Please complete the installation in the NSIS window that opened.\n") +
      QString::fromUtf8("You can now use Linuxtrack with this game in Lutris once the installation is complete."));
  } else {
    QMessageBox::critical(getParentWidget(), QString::fromUtf8("Installation Failed"),
      QString::fromUtf8("Failed to start Linuxtrack Wine Bridge installation for: ") + selectedGame + QString::fromUtf8("\n\n") +
      QString::fromUtf8("Error: ") + lutrisIntegration->getLastError() + QString::fromUtf8("\n\n") +
      QString::fromUtf8("Debug Info: ") + lutrisIntegration->getDebugInfo());
  }
}

// New method for Steam Proton bridge installation
void PluginInstall::installSteamProtonBridge()
{
  // Check prerequisites
  if(!isTirFirmwareInstalled() || !isMfc140uInstalled()) {
    QMessageBox::warning(getParentWidget(), QString::fromUtf8("Prerequisites Required"),
      QString::fromUtf8("Please install TIR Firmware and MFC140 libraries first using the button above."));
    return;
  }
  
  // Initialize Steam integration if not already done
  if (!steamIntegration) {
    steamIntegration = new SteamIntegration(this);
  }
  
  // Check if Steam is installed
  if (!steamIntegration->isSteamInstalled()) {
    QMessageBox::warning(getParentWidget(), QString::fromUtf8("Steam Not Found"),
      QString::fromUtf8("Steam is not installed or not properly configured.\n\n") +
      QString::fromUtf8("Error: ") + steamIntegration->getLastError());
    return;
  }
  
  // Get available Steam games with Proton prefixes
  QList<SteamGame> games = steamIntegration->getSteamGames();
  if (games.isEmpty()) {
    QMessageBox::information(getParentWidget(), QString::fromUtf8("No Steam Games Found"),
      QString::fromUtf8("No Steam games with Proton prefixes were found.\n\n") +
      QString::fromUtf8("Please install some games in Steam first, then try again.\n\n") +
      QString::fromUtf8("Debug Info: ") + steamIntegration->getDebugInfo());
    return;
  }
  
  // Filter games that have Proton prefixes
  QList<SteamGame> protonGames;
  for (const SteamGame &game : games) {
    if (!game.prefix_path.isEmpty()) {
      protonGames.append(game);
    }
  }
  
  if (protonGames.isEmpty()) {
    QMessageBox::information(getParentWidget(), QString::fromUtf8("No Proton Games Found"),
      QString::fromUtf8("No Steam games with Proton prefixes were found.\n\n") +
      QString::fromUtf8("Please install some games that use Proton in Steam first, then try again.\n\n") +
      QString::fromUtf8("Debug Info: ") + steamIntegration->getDebugInfo());
    return;
  }
  
  // Create game selection dialog
  QStringList gameNames;
  QStringList gameIds;
  for (const SteamGame &game : protonGames) {
    QString displayName = game.game_name.isEmpty() ? game.game_id : game.game_name;
    gameNames.append(displayName);
    gameIds.append(game.game_id);
  }
  
  bool ok;
  QString selectedGame = QInputDialog::getItem(getParentWidget(),
    QString::fromUtf8("Select Steam Game"),
    QString::fromUtf8("Choose a game to install Linuxtrack Wine Bridge:"),
    gameNames, 0, false, &ok);
  
  if (!ok || selectedGame.isEmpty()) {
    return; // User cancelled
  }
  
  // Find the selected game
  int selectedIndex = gameNames.indexOf(selectedGame);
  if (selectedIndex == -1) {
    QMessageBox::warning(getParentWidget(), QString::fromUtf8("Selection Error"),
      QString::fromUtf8("Invalid game selection."));
    return;
  }
  
  QString selectedGameId = gameIds[selectedIndex];
  
  // Show information dialog about interactive installation
  QMessageBox::information(getParentWidget(), QString::fromUtf8("Starting Interactive Installation"),
    QString::fromUtf8("Starting Linuxtrack Wine Bridge installation for: ") + selectedGame + QString::fromUtf8("\n\n") +
    QString::fromUtf8("The NSIS installer will open in a new window.\n") +
    QString::fromUtf8("Please follow the installation prompts in that window.\n\n") +
    QString::fromUtf8("Click OK to start the installation."));
  
  // Install to the selected game
  bool success = steamIntegration->installToSteamGame(selectedGameId);
  
  if (success) {
    QMessageBox::information(getParentWidget(), QString::fromUtf8("Installation Completed"),
      QString::fromUtf8("Linuxtrack Wine Bridge has been successfully installed for: ") + selectedGame + QString::fromUtf8("\n\n") +
      QString::fromUtf8("You can now use Linuxtrack with this game in Steam!"));
  } else {
    QMessageBox::critical(getParentWidget(), QString::fromUtf8("Installation Failed"),
      QString::fromUtf8("Failed to start Linuxtrack Wine Bridge installation for: ") + selectedGame + QString::fromUtf8("\n\n") +
      QString::fromUtf8("Error: ") + steamIntegration->getLastError() + QString::fromUtf8("\n\n") +
      QString::fromUtf8("Debug Info: ") + steamIntegration->getDebugInfo());
  }
}


