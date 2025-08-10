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
    QMessageBox::warning(parentWidget, QString::fromUtf8("MFC42 Library Required"),
      QString::fromUtf8("MFC42 library is required but not found in the tir_firmware directory.\\n\\n"
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
  QString prefix = QFileDialog::getExistingDirectory(parentWidget, QString::fromUtf8("Select Wine Prefix..."),
                     QDir::homePath(), QFileDialog::ShowDirsOnly);

  // Handle user cancellation gracefully
  if (prefix.isEmpty()) {
    return;
  }

  // Validate the selected directory exists
  QDir chosenPrefix(prefix);
  if (!chosenPrefix.exists()) {
    QMessageBox::warning(parentWidget, QString::fromUtf8("Invalid Prefix"),
      QString::fromUtf8("The selected directory does not exist."));
    return;
  }
  
  // Resolve installer via centralized resolver
  QString installerPath = InstallerPaths::resolveWineBridgeInstallerPath();
  if (installerPath.isEmpty()) {
    QMessageBox::critical(parentWidget, QString::fromUtf8("Installer Not Found"),
      QString::fromUtf8("Could not find linuxtrack-wine.exe in expected locations."));
    return;
  }

  // Do not auto-start tracking here; installation should be non-intrusive

  // Ensure launcher is available (should be constructed in constructor)
  if (!inst) {
    QMessageBox::critical(parentWidget, QString::fromUtf8("Installer Not Available"),
      QString::fromUtf8("Wine installer launcher is not available."));
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
    QMessageBox::warning(parentWidget, QString::fromUtf8("Mfc42u install"),
                         QString::fromUtf8("Install TrackIR firmware first!"));
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
        QMessageBox::information(getParentWidget(), QString::fromUtf8("Installation Complete"),
          QString::fromUtf8("Linuxtrack Wine Bridge has been successfully installed!\n\n") +
          QString::fromUtf8("You can now test your head tracking in your games!\n\n") +
          QString::fromUtf8("Use the tracking window to pause, recenter, or stop tracking as needed."));
      }
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

// New method for TIR/MFC42 installation only
void PluginInstall::installTirFirmwareAndMfc42()
{
  // Set flag to indicate this is TIR/MFC42 only installation
  isTirMfcOnlyInstallation = true;
  
  // Check if all required firmware files already exist
  if(isTirFirmwareInstalled() && isMfc42uInstalled()){
    QMessageBox::information(getParentWidget(), QString::fromUtf8("Already Installed"),
      QString::fromUtf8("TIR Firmware and MFC42 libraries are already installed."));
    isTirMfcOnlyInstallation = false;  // Reset flag
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
    QMessageBox::information(getParentWidget(), QString::fromUtf8("Already Installed"),
      QString::fromUtf8("TIR Firmware and MFC42 libraries are already installed."));
    isTirMfcOnlyInstallation = false;  // Reset flag
  }
}

// New method for wine bridge installation to custom prefix
void PluginInstall::installWineBridgeToCustomPrefix()
{
  // Check prerequisites
  if(!isTirFirmwareInstalled() || !isMfc42uInstalled()) {
    QMessageBox::warning(getParentWidget(), QString::fromUtf8("Prerequisites Required"),
      QString::fromUtf8("Please install TIR Firmware and MFC42 libraries first using the button above."));
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
    QMessageBox::warning(getParentWidget(), QString::fromUtf8("Prerequisites Required"),
      QString::fromUtf8("Please install TIR Firmware and MFC42 libraries first using the button above."));
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
  
  // Show information dialog about interactive installation (align with Steam flow)
  QMessageBox::information(getParentWidget(), QString::fromUtf8("Starting Interactive Installation"),
    QString::fromUtf8("Starting Linuxtrack Wine Bridge installation for: ") + selectedGame + QString::fromUtf8("\n\n") +
    QString::fromUtf8("The NSIS installer will open in a new window.\n") +
    QString::fromUtf8("Please follow the installation prompts in that window."));
  
  // Install to the selected game
  bool success = lutrisIntegration->installToLutrisGame(selectedSlug);
  
  if (success) {
    QMessageBox::information(getParentWidget(), QString::fromUtf8("Installation Completed"),
      QString::fromUtf8("Linuxtrack Wine Bridge has been successfully installed for: ") + selectedGame + QString::fromUtf8("\n\n") +
      QString::fromUtf8("You can now use Linuxtrack with this game in Lutris!"));
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
  if(!isTirFirmwareInstalled() || !isMfc42uInstalled()) {
    QMessageBox::warning(getParentWidget(), QString::fromUtf8("Prerequisites Required"),
      QString::fromUtf8("Please install TIR Firmware and MFC42 libraries first using the button above."));
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
  
  // Do not auto-start tracking here; installation should be non-intrusive
  
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


