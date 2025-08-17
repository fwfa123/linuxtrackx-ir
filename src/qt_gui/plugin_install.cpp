#include <plugin_install.h>
#include <ltr_gui_prefs.h>
#include <iostream>
#include <QProcess>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <zlib.h>
#include "extractor.h"
#include "utils.h"
#include "installer_paths.h"
#include "tracker.h"

#ifdef HAVE_CONFIG_H
  #include "../../config.h"
#endif

PluginInstall::PluginInstall(const Ui::LinuxtrackMainForm &ui, QObject *parent):QObject(parent),
  state(DONE), gui(ui), inst(NULL), dlfw(NULL), dlmfc42(NULL), lutrisIntegration(NULL), steamIntegration(NULL),
  isTirMfcOnlyInstallation(false),
  poem1(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/poem1.txt")),
  poem2(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/poem2.txt")),
  gameData(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/gamedata.txt")),
  mfc42u(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc42u.dll")),
  tirViews(PREF.getRsrcDirPath() + QString::fromUtf8("/tir_firmware/TIRViews.dll"))
{
#ifndef DARWIN
  // Check for wine installer in common locations and warn/disable legacy buttons if not found
  QString wineInstallerPath = InstallerPaths::resolveWineBridgeInstallerPath();
  if(wineInstallerPath.isEmpty()){
    enableButtons(false);
    // Do not return; allow Custom Prefix button flow to run and report a clear error later
  }
#endif
  inst = new WineLauncher();
  Connect();

  // Initialize initial prereq UI if available (safe to call even before main connects)
  const bool ready = isTirFirmwareInstalled() && isMfc42uInstalled();
  emit prereqStatusChanged(ready);
}

void PluginInstall::close()
{
  if(dlfw != NULL){
    dlfw->close();
  }
  if(dlmfc42 != NULL){
    dlmfc42->close();
  }
}

PluginInstall::~PluginInstall()
{
  if(dlfw != NULL){
    dlfw->close();
    delete dlfw;
    dlfw = NULL;
  }
  if(dlmfc42 != NULL){
    dlmfc42->close();
    delete dlmfc42;
    dlmfc42 = NULL;
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
    // Install MFC42 (winetricks approach)
    state = MFC;
    mfc42uInstall();
  }else{
    installLinuxtrackWine();
    state = LTR_W;
  }
}



bool PluginInstall::isTirFirmwareInstalled() const
{
  return QFile::exists(poem1) && QFile::exists(poem2) && QFile::exists(gameData) && QFile::exists(tirViews);
}



bool PluginInstall::isMfc42uInstalled() const
{
  return QFile::exists(mfc42u);
}


void PluginInstall::installLinuxtrackWine()
{
  if(dlfw != NULL){
    dlfw->hide();
  }
  
  // Double-check that MFC42 library exists before proceeding
  if(!isMfc42uInstalled()) {
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
    QMessageBox::warning(parentWidget, QObject::tr("MFC42 Library Required"),
      QObject::tr("MFC42 library is required but not found in the tir_firmware directory.\n\n"
      "Please install MFC42 using winetricks:\n"
      "winetricks mfc42\n"
      "Or run the MFC42 installation in the LinuxTrack GUI first.")
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
  QString prefix = QFileDialog::getExistingDirectory(parentWidget, QObject::tr("Select Wine Prefix..."),
                     QDir::homePath(), QFileDialog::ShowDirsOnly);

  // Handle user cancellation gracefully
  if (prefix.isEmpty()) {
    return;
  }

  // Validate the selected directory exists
  QDir chosenPrefix(prefix);
  if (!chosenPrefix.exists()) {
    QMessageBox::warning(parentWidget, QObject::tr("Invalid Prefix"),
      QObject::tr("The selected directory does not exist."));
    return;
  }
  
  // Resolve installer via centralized resolver
  QString installerPath = InstallerPaths::resolveWineBridgeInstallerPath();
  if (installerPath.isEmpty()) {
    QMessageBox::critical(parentWidget, QObject::tr("Installer Not Found"),
      QObject::tr("Could not find linuxtrack-wine.exe in expected locations."));
    return;
  }

  // Do not auto-start tracking here; installation should be non-intrusive

  // Ensure launcher is available (should be constructed in constructor)
  if (!inst) {
    QMessageBox::critical(parentWidget, QObject::tr("Installer Not Available"),
      QObject::tr("Wine installer launcher is not available."));
    return;
  }

  inst->setEnv(QString::fromUtf8("WINEPREFIX"), prefix);
  inst->run(installerPath);
#else
  if(isTirFirmwareInstalled() && isMfc42uInstalled()){
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
    QMessageBox::information(parentWidget, QObject::tr("Firmware extraction successfull"),
      QObject::tr("Firmware extraction finished successfully!"
      "\nNow you can install linuxtrack-wine.exe to the Wine bottle/prefix of your choice.")
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



void PluginInstall::mfc42uInstall()
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
    QMessageBox::warning(parentWidget, QObject::tr("Mfc42u install"),
                         QObject::tr("Install TrackIR firmware first!"));
    state = TIR_FW;
    tirFirmwareInstall();
    return;
  }
  
  // Get the main window for user interaction
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
  
  // Show user what's about to happen and get confirmation
  QMessageBox::StandardButton reply = QMessageBox::question(parentWidget, 
    QString::fromUtf8("MFC42 Installation"),
    QString::fromUtf8("TrackIR firmware installation completed successfully!\n\n"
    "Next, we need to install MFC42 libraries using winetricks.\n\n"
    "This process will:\n"
    "• Create a temporary Wine prefix\n"
    "• Run 'winetricks mfc42' (this may take several minutes)\n"
    "• Extract the required mfc42u.dll file\n"
    "• Copy it to the firmware directory\n\n"
    "The system may appear to hang during winetricks installation, but this is normal.\n"
    "You'll see progress updates in the installer window.\n\n"
    "Do you want to proceed with MFC42 installation?"),
    QMessageBox::Yes | QMessageBox::No);
  
  if(reply == QMessageBox::No) {
    // User cancelled - stay in current state
    state = DONE;
    enableButtons(true);
    return;
  }
  
  // User confirmed - proceed with MFC42 installation
  if(dlmfc42 == NULL){
    dlmfc42 = new Mfc42uWinetricksExtractor();
    QObject::connect(dlmfc42, SIGNAL(finished(bool)),
      this, SLOT(finished(bool)));
  }
  
  // Show the dialog with clear instructions
  dlmfc42->show();
  
  // Don't start automatic installation - let user interact with UI first
  // The user can choose winetricks installation or manual methods
}

void PluginInstall::finished(bool ok)
{
  if(dlfw != NULL){
    dlfw->hide();
  }
  if(dlmfc42 != NULL){
    dlmfc42->hide();
  }
  switch(state){
    case TIR_FW:
      if(ok) {
        if (isTirMfcOnlyInstallation) {
          // For TIR/MFC42 only installation, go to TIR_MFC_ONLY state
          state = TIR_MFC_ONLY;
        } else {
          // For full installation, go to MFC state
          state = MFC;
        }
        mfc42uInstall();
      } else {
        // TIR firmware extraction failed - don't proceed
        state = DONE;
        enableButtons(true);
        isTirMfcOnlyInstallation = false;  // Reset flag
      }
      break;
    case MFC:
      if(ok) {
        // MFC42 installation successful - proceed to Wine bridge installation
        state = LTR_W;
        installLinuxtrackWine();
        emit prereqStatusChanged(isTirFirmwareInstalled() && isMfc42uInstalled());
      } else {
        // MFC42 extraction failed - don't proceed
        state = DONE;
        enableButtons(true);
      }
      break;
    case TIR_MFC_ONLY:
      if(ok) {
        // MFC42 installation successful - show completion message and stop
        QMessageBox::information(getParentWidget(), QString::fromUtf8("Installation Complete"),
          QString::fromUtf8("TIR Firmware and MFC42 libraries have been successfully installed.\n\n") +
          QString::fromUtf8("You can now use the other buttons to install the wine bridge to specific platforms."));
        state = DONE;
        enableButtons(true);
        isTirMfcOnlyInstallation = false;  // Reset flag
        emit prereqStatusChanged(isTirFirmwareInstalled() && isMfc42uInstalled());
      } else {
        // MFC42 extraction failed - don't proceed
        state = DONE;
        enableButtons(true);
        isTirMfcOnlyInstallation = false;  // Reset flag
      }
      break;
    case LTR_W:
    case TIR_FW_ONLY:
    case MFC_ONLY:
    default:
      if (ok && state == LTR_W) {
        // Wine bridge installation was successful
    QMessageBox::information(getParentWidget(), QObject::tr("Installation Complete"),
      QObject::tr("Linuxtrack Wine Bridge has been successfully installed!\n\n") +
      QObject::tr("You can now test your head tracking in your games!\n\n") +
      QObject::tr("Use the tracking window to pause, recenter, or stop tracking as needed."));
      }
      state = DONE;
      enableButtons(true);
      emit prereqStatusChanged(isTirFirmwareInstalled() && isMfc42uInstalled());
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

// New method for TIR/MFC42 installation only
void PluginInstall::installTirFirmwareAndMfc42()
{
  // Set flag to indicate this is TIR/MFC42 only installation
  isTirMfcOnlyInstallation = true;
  
  // Check if all required firmware files already exist
  if(isTirFirmwareInstalled() && isMfc42uInstalled()){
    QMessageBox::information(getParentWidget(), QObject::tr("Already Installed"),
      QObject::tr("TIR Firmware and MFC42 libraries are already installed."));
    isTirMfcOnlyInstallation = false;  // Reset flag
    emit prereqStatusChanged(true);
    return;
  }
  
  // Otherwise, proceed with the normal extraction flow
  if(!isTirFirmwareInstalled()){
    state = TIR_FW;
    tirFirmwareInstall();
  } else if(!isMfc42uInstalled()){
    // Install MFC42 (winetricks approach)
    state = TIR_MFC_ONLY;  // Use new state to prevent wine bridge installation
    mfc42uInstall();
  } else {
    // Both already installed
    QMessageBox::information(getParentWidget(), QObject::tr("Already Installed"),
      QObject::tr("TIR Firmware and MFC42 libraries are already installed."));
    isTirMfcOnlyInstallation = false;  // Reset flag
    emit prereqStatusChanged(true);
  }
}

// New method for wine bridge installation to custom prefix
void PluginInstall::installWineBridgeToCustomPrefix()
{
  // Check prerequisites
  if(!isTirFirmwareInstalled() || !isMfc42uInstalled()) {
    QMessageBox::warning(getParentWidget(), QObject::tr("Prerequisites Required"),
      QObject::tr("Please install TIR Firmware and MFC42 libraries first using the button above."));
    return;
  }
  
  // Proceed with wine bridge installation (existing functionality)
  installLinuxtrackWine();
}

// New method for Lutris wine bridge installation
void PluginInstall::installLutrisWineBridge()
{
  // Check prerequisites
  if(!isTirFirmwareInstalled() || !isMfc42uInstalled()) {
    QMessageBox::warning(getParentWidget(), QObject::tr("Prerequisites Required"),
      QObject::tr("Please install TIR Firmware and MFC42 libraries first using the button above."));
    return;
  }
  
  // Initialize Lutris integration if not already done
  if (!lutrisIntegration) {
    lutrisIntegration = new LutrisIntegration(this);
  }
  
  // Check if Lutris is installed
  if (!lutrisIntegration->isLutrisInstalled()) {
    QMessageBox::warning(getParentWidget(), QObject::tr("Lutris Not Found"),
      QObject::tr("Lutris is not installed or not properly configured.\n\n") +
      QObject::tr("Error: ") + lutrisIntegration->getLastError());
    return;
  }
  
  // Get available Lutris games
  QList<LutrisGame> games = lutrisIntegration->getLutrisGames();
  if (games.isEmpty()) {
    QMessageBox::information(getParentWidget(), QObject::tr("No Lutris Games Found"),
      QObject::tr("No Wine-based games were found in Lutris.\n\n") +
      QObject::tr("Please install some games in Lutris first, then try again.\n\n") +
      QObject::tr("Debug Info: ") + lutrisIntegration->getDebugInfo());
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
  
  // Show information dialog about interactive installation (align with Steam flow)
  QMessageBox::information(getParentWidget(), QObject::tr("Starting Interactive Installation"),
    QObject::tr("Starting Linuxtrack Wine Bridge installation for: %1\n\n").arg(selectedGame) +
    QObject::tr("The NSIS installer will open in a new window.\n") +
    QObject::tr("Please follow the installation prompts in that window."));
  
  // Install to the selected game
  bool success = lutrisIntegration->installToLutrisGame(selectedSlug);
  
  if (success) {
    QMessageBox::information(getParentWidget(), QObject::tr("Installation Completed"),
      QObject::tr("Linuxtrack Wine Bridge has been successfully installed for: %1\n\n").arg(selectedGame) +
      QObject::tr("You can now use Linuxtrack with this game in Lutris!"));
  } else {
    QMessageBox::critical(getParentWidget(), QObject::tr("Installation Failed"),
      QObject::tr("Failed to start Linuxtrack Wine Bridge installation for: %1\n\n").arg(selectedGame) +
      QObject::tr("Error: ") + lutrisIntegration->getLastError() + QString::fromUtf8("\n\n") +
      QObject::tr("Debug Info: ") + lutrisIntegration->getDebugInfo());
  }
}

// New method for Steam Proton bridge installation
void PluginInstall::installSteamProtonBridge()
{
  // Check prerequisites
  if(!isTirFirmwareInstalled() || !isMfc42uInstalled()) {
    QMessageBox::warning(getParentWidget(), QObject::tr("Prerequisites Required"),
      QObject::tr("Please install TIR Firmware and MFC42 libraries first using the button above."));
    return;
  }
  
  // Initialize Steam integration if not already done
  if (!steamIntegration) {
    steamIntegration = new SteamIntegration(this);
  }
  
  // Check if Steam is installed
  if (!steamIntegration->isSteamInstalled()) {
    QMessageBox::warning(getParentWidget(), QObject::tr("Steam Not Found"),
      QObject::tr("Steam is not installed or not properly configured.\n\n") +
      QObject::tr("Error: ") + steamIntegration->getLastError());
    return;
  }
  
  // Get available Steam games with Proton prefixes
  QList<SteamGame> games = steamIntegration->getSteamGames();
  if (games.isEmpty()) {
    QMessageBox::information(getParentWidget(), QObject::tr("No Steam Games Found"),
      QObject::tr("No Steam games with Proton prefixes were found.\n\n") +
      QObject::tr("Please install some games in Steam first, then try again.\n\n") +
      QObject::tr("Debug Info: ") + steamIntegration->getDebugInfo());
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
    QMessageBox::information(getParentWidget(), QObject::tr("No Proton Games Found"),
      QObject::tr("No Steam games with Proton prefixes were found.\n\n") +
      QObject::tr("Please install some games that use Proton in Steam first, then try again.\n\n") +
      QObject::tr("Debug Info: ") + steamIntegration->getDebugInfo());
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
    QObject::tr("Select Steam Game"),
    QObject::tr("Choose a game to install Linuxtrack Wine Bridge:"),
    gameNames, 0, false, &ok);
  
  if (!ok || selectedGame.isEmpty()) {
    return; // User cancelled
  }
  
  // Find the selected game
  int selectedIndex = gameNames.indexOf(selectedGame);
  if (selectedIndex == -1) {
    QMessageBox::warning(getParentWidget(), QObject::tr("Selection Error"),
      QObject::tr("Invalid game selection."));
    return;
  }
  
  QString selectedGameId = gameIds[selectedIndex];
  
  // Show information dialog about interactive installation
  QMessageBox::information(getParentWidget(), QObject::tr("Starting Interactive Installation"),
    QObject::tr("Starting Linuxtrack Wine Bridge installation for: %1\n\n").arg(selectedGame) +
    QObject::tr("The NSIS installer will open in a new window.\n") +
    QObject::tr("Please follow the installation prompts in that window.\n\n") +
    QObject::tr("Click OK to start the installation."));
  
  // Do not auto-start tracking here; installation should be non-intrusive
  
  // Install to the selected game
  bool success = steamIntegration->installToSteamGame(selectedGameId);
  
  if (success) {
    QMessageBox::information(getParentWidget(), QObject::tr("Installation Completed"),
      QObject::tr("Linuxtrack Wine Bridge has been successfully installed for: %1\n\n").arg(selectedGame) +
      QObject::tr("You can now use Linuxtrack with this game in Steam!"));
  } else {
    QMessageBox::critical(getParentWidget(), QObject::tr("Installation Failed"),
      QObject::tr("Failed to start Linuxtrack Wine Bridge installation for: %1\n\n").arg(selectedGame) +
      QObject::tr("Error: ") + steamIntegration->getLastError() + QString::fromUtf8("\n\n") +
      QObject::tr("Debug Info: ") + steamIntegration->getDebugInfo());
  }
}


