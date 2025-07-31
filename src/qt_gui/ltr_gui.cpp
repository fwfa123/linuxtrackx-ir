#define NEWS_SERIAL 2

#ifdef HAVE_CONFIG_H
  #include "../../config.h"
#endif

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QSettings>
#include <iostream>
#include "ltr_gui.h"
#include "ltr_gui_prefs.h"
#include "prefs_link.h"
#include "pathconfig.h"
#include "ltr_state.h"

#include "ltr_show.h"
#include "ltr_dev_help.h"
#include "ltr_model.h"
#include "ltr_tracking.h"
#include "log_view.h"
#include "help_view.h"
#include "plugin_install.h"
#include "device_setup.h"
#include "profile_selector.h"
#include "guardian.h"
#include "wine_launcher.h"
#include "xplugin.h"
#include "wine_warn.h"

// Static string constants for better performance
static const QString APP_TITLE = QStringLiteral("Linuxtrack");
static const QString MAIN_WINDOW_GROUP = QStringLiteral("MainWindow");
static const QString TRACKING_WINDOW_GROUP = QStringLiteral("TrackingWindow");
static const QString HELPER_WINDOW_GROUP = QStringLiteral("HelperWindow");
static const QString SIZE_KEY = QStringLiteral("size");
static const QString POS_KEY = QStringLiteral("pos");
static const QString WELCOME_KEY = QStringLiteral("welcome");
static const QString NEWS_KEY = QStringLiteral("news");
static const QString WINE_WARNING_KEY = QStringLiteral("wine_warning");

static QMessageBox::StandardButton warnQuestion(const QString &message)
{
 return QMessageBox::warning(nullptr, APP_TITLE, message, 
                           QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
}

static QMessageBox::StandardButton warningMessage(const QString &message)
{
 return QMessageBox::warning(nullptr, APP_TITLE, message, QMessageBox::Ok);
}

static QMessageBox::StandardButton infoMessage(const QString &message)
{
 QMessageBox msgBox;
 msgBox.setWindowIcon(QIcon(QStringLiteral(":/ltr/linuxtrack.svg")));
 msgBox.setWindowTitle(APP_TITLE);
 msgBox.setText(message);
 msgBox.setStandardButtons(QMessageBox::Ok);
 msgBox.setIcon(QMessageBox::Information);
 return static_cast<QMessageBox::StandardButton>(msgBox.exec());
}

LinuxtrackGui::LinuxtrackGui(QWidget *parent) : QMainWindow(parent), mainWidget(nullptr), 
  showWindow(nullptr), helper(nullptr), ds(nullptr), me(nullptr), grd(nullptr), lv(nullptr), 
  pi(nullptr), ps(nullptr), xpInstall(nullptr), initialized(false), gui_settings(nullptr), 
  welcome(false), news_serial(-1), guiInit(true), showWineWarning(true),
  trackingDockWidget(nullptr), dockAction(nullptr), undockAction(nullptr), 
  dockLeftAction(nullptr), dockRightAction(nullptr), dockingMenu(nullptr),
  isTrackingWindowDocked(false), dockArea(Qt::LeftDockWidgetArea)
{
  mainWidget = new QWidget(this);
  ui.setupUi(mainWidget);
  PREF;
  setWindowTitle(QStringLiteral("Linuxtrack GUI v") + QStringLiteral(PACKAGE_VERSION));
  setWindowIcon(QIcon(QStringLiteral(":/ltr/linuxtrack.svg")));
  setCentralWidget(mainWidget);
  
  grd = new Guardian(this);
  me = new ModelEdit(grd, this);
  lv = new LogView();
  pi = new PluginInstall(ui, this);
  ps = new ProfileSelector(this);
  QObject::connect(&STATE, SIGNAL(stateChanged(linuxtrack_state_type)),
                   this, SLOT(trackerStateHandler(linuxtrack_state_type)));
  QObject::connect(&zipper, SIGNAL(finished(int, QProcess::ExitStatus)),
                   this, SLOT(logsPackaged(int, QProcess::ExitStatus)));
  
  // Connect main GUI buttons explicitly (auto-connect broken due to custom central widget)
  QObject::connect(ui.HelpButton, SIGNAL(pressed()), this, SLOT(on_HelpButton_pressed()));
  QObject::connect(ui.DefaultsButton, SIGNAL(pressed()), this, SLOT(on_DefaultsButton_pressed()));
  QObject::connect(ui.DiscardChangesButton, SIGNAL(pressed()), this, SLOT(on_DiscardChangesButton_pressed()));
  QObject::connect(ui.SaveButton, SIGNAL(pressed()), this, SLOT(on_SaveButton_pressed()));
  QObject::connect(ui.QuitButton, SIGNAL(pressed()), this, SLOT(on_QuitButton_pressed()));
  QObject::connect(ui.XplanePluginButton, SIGNAL(pressed()), this, SLOT(on_XplanePluginButton_pressed()));
  QObject::connect(ui.ViewLogButton, SIGNAL(pressed()), this, SLOT(on_ViewLogButton_pressed()));
  QObject::connect(ui.PackageLogsButton, SIGNAL(pressed()), this, SLOT(on_PackageLogsButton_pressed()));
  QObject::connect(ui.LaunchMickeyButton, SIGNAL(pressed()), this, SLOT(on_LaunchMickeyButton_pressed()));
  
  // Connect Gaming tab buttons
  QObject::connect(ui.InstallTirMfcButton, SIGNAL(pressed()), this, SLOT(on_InstallTirMfcButton_pressed()));
  QObject::connect(ui.SteamProtonButton, SIGNAL(pressed()), this, SLOT(on_SteamProtonButton_pressed()));
  QObject::connect(ui.LutrisButton, SIGNAL(pressed()), this, SLOT(on_LutrisButton_pressed()));
  QObject::connect(ui.OtherPlatformButton, SIGNAL(pressed()), this, SLOT(on_OtherPlatformButton_pressed()));
  QObject::connect(ui.CustomPrefixButton, SIGNAL(pressed()), this, SLOT(on_CustomPrefixButton_pressed()));
  QObject::connect(ui.BatchInstallButton, SIGNAL(pressed()), this, SLOT(on_BatchInstallButton_pressed()));
  QObject::connect(ui.WinePrefixButton, SIGNAL(pressed()), this, SLOT(on_WinePrefixButton_pressed()));
  QObject::connect(ui.LaunchLtrPipeButton, SIGNAL(pressed()), this, SLOT(on_LaunchLtrPipeButton_pressed()));
  
  ui.ModelEditSite->addWidget(me);
  ui.ProfileSetupSite->addWidget(ps);

  gui_settings = new QSettings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
  showWindow = new LtrGuiForm(ui, *gui_settings);
  helper = new LtrDevHelp();
  
  // Setup docking functionality
  setupDocking();
  
  // Load main window settings
  gui_settings->beginGroup(MAIN_WINDOW_GROUP);
  resize(gui_settings->value(SIZE_KEY, QSize(763, 627)).toSize());
  move(gui_settings->value(POS_KEY, QPoint(0, 0)).toPoint());
  welcome = gui_settings->value(WELCOME_KEY, true).toBool();
  news_serial = gui_settings->value(NEWS_KEY, -1).toInt();
  showWineWarning = gui_settings->value(WINE_WARNING_KEY, true).toBool();
  gui_settings->endGroup();
  
  // Load tracking window settings
  gui_settings->beginGroup(TRACKING_WINDOW_GROUP);
  showWindow->resize(gui_settings->value(SIZE_KEY, QSize(800, 600)).toSize());
  showWindow->move(gui_settings->value(POS_KEY, QPoint(0, 0)).toPoint());
  gui_settings->endGroup();
  
  // Load helper window settings
  gui_settings->beginGroup(HELPER_WINDOW_GROUP);
  helper->resize(gui_settings->value(SIZE_KEY, QSize(300, 80)).toSize());
  helper->move(gui_settings->value(POS_KEY, QPoint(0, 0)).toPoint());
  gui_settings->endGroup();
  
  HelpViewer::LoadPrefs(*gui_settings);

  ui.LegacyPose->setChecked(ltr_int_use_alter());
  ui.LegacyRotation->setChecked(ltr_int_use_oldrot());
  ui.TransRotDisable->setChecked(!ltr_int_do_tr_align());
  ui.FocalLength->setValue(ltr_int_get_focal_length());
  
  WineLauncher wl;
  if (!wl.wineAvailable() && showWineWarning) {
    WineWarn w(this);
    if (w.exec() == QDialog::Accepted) {
      showWineWarning = false;
    }
  }
  guiInit = false;
}

void LinuxtrackGui::show()
{
  ds = new DeviceSetup(grd, ui.DeviceSetupSite, this);
  ui.DeviceSetupSite->insertWidget(0, ds);
  
  // Show tracking window based on docking state
  if (isTrackingWindowDocked) {
    trackingDockWidget->show();
  } else {
    showWindow->show();
  }
  
  const QString dbg = QProcessEnvironment::systemEnvironment().value(QStringLiteral("LINUXTRACK_DBG"));
  if (dbg.contains(QChar::fromLatin1('d'))) {
    helper->show();
  }
  
  QMainWindow::show();
  
  if (welcome) {
    HelpViewer::ChangePage(QStringLiteral("welcome.htm"));
    HelpViewer::ShowWindow();
  } else if (news_serial < NEWS_SERIAL) {
    HelpViewer::ChangePage(QStringLiteral("news.htm"));
    HelpViewer::ShowWindow();
  } else {
    HelpViewer::ChangePage(QStringLiteral("dev_setup.htm"));
  }
}

LinuxtrackGui::~LinuxtrackGui()
{
  PrefProxy::ClosePrefs();
  delete pi;
  pi = nullptr;
  delete showWindow;
  showWindow = nullptr;
  delete me;
  me = nullptr;
  delete helper;
  helper = nullptr;
  delete gui_settings;
  gui_settings = nullptr;
  delete ps;
  ps = nullptr;
  delete grd;
  grd = nullptr;
  delete lv;
  lv = nullptr;
  delete ds;
  ds = nullptr;
  if (xpInstall != nullptr) {
    // xpInstall cleanup if needed
  }
}

void LinuxtrackGui::closeEvent(QCloseEvent *event)
{
  static bool invokedAlready = false;
  if (invokedAlready) {
    event->accept();
    return;
  }
  invokedAlready = true;
  
  TRACKER.stop();
  PREF.SavePrefsOnExit();
  HelpViewer::CloseWindow();
  
  // Save main window settings
  gui_settings->beginGroup(MAIN_WINDOW_GROUP);
  gui_settings->setValue(SIZE_KEY, size());
  gui_settings->setValue(POS_KEY, pos());
  gui_settings->setValue(WELCOME_KEY, false);
  gui_settings->setValue(NEWS_KEY, NEWS_SERIAL);
  gui_settings->setValue(WINE_WARNING_KEY, showWineWarning);
  gui_settings->endGroup();
  
  // Save tracking window settings
  gui_settings->beginGroup(TRACKING_WINDOW_GROUP);
  gui_settings->setValue(SIZE_KEY, showWindow->size());
  gui_settings->setValue(POS_KEY, showWindow->pos());
  gui_settings->endGroup();
  
  // Save helper window settings
  gui_settings->beginGroup(HELPER_WINDOW_GROUP);
  gui_settings->setValue(SIZE_KEY, helper->size());
  gui_settings->setValue(POS_KEY, helper->pos());
  gui_settings->endGroup();
  
  // Save docking state
  saveDockingState();
  
  HelpViewer::StorePrefs(*gui_settings);
  showWindow->StorePrefs(*gui_settings);
  showWindow->allowCloseWindow();
  showWindow->close();
  helper->close();
  lv->close();
  ps->close();
  pi->close();
  event->accept();
}

void LinuxtrackGui::on_QuitButton_pressed()
{
  close();
}

void LinuxtrackGui::on_XplanePluginButton_pressed()
{
  if (xpInstall == nullptr) {
    xpInstall = new XPluginInstall();
  }
  xpInstall->exec();
}

void LinuxtrackGui::on_SaveButton_pressed()
{
  PREF.savePrefs();
}

void LinuxtrackGui::on_ViewLogButton_pressed()
{
  lv->show();
}

void LinuxtrackGui::rereadPrefs()
{
  PREF.rereadPrefs();
  if (initialized && ds != nullptr) {
    ds->refresh();
    //track->refresh();
  }
}

void LinuxtrackGui::on_DefaultsButton_pressed()
{
  if (warnQuestion(QStringLiteral("You are about to load default settings, removing all changes you ever did!\n") +
                          QStringLiteral("Do you really want to do that?")) == QMessageBox::Ok) {
    PREF.copyDefaultPrefs();
    rereadPrefs();
    if (ds != nullptr) {
      ds->refresh();
    }
  }
}

void LinuxtrackGui::on_DiscardChangesButton_pressed()
{
  if (warnQuestion(QStringLiteral("You are about to discard modifications you did since last save!\n") +
                          QStringLiteral("Do you really want to do that?")) == QMessageBox::Ok) {
     rereadPrefs();
  }
}

void LinuxtrackGui::on_HelpButton_pressed()
{
  HelpViewer::ShowWindow();
}

void LinuxtrackGui::on_LtrTab_currentChanged(int index)
{
  switch (index) {
    case 0:
      HelpViewer::ChangePage(QStringLiteral("dev_setup.htm"));
      break;
    case 1:
      HelpViewer::ChangePage(QStringLiteral("model_setup.htm"));
      break;
    case 2:
      HelpViewer::ChangePage(QStringLiteral("axes_setup.htm"));
      break;
    case 3:
      HelpViewer::ChangePage(QStringLiteral("misc.htm"));
      break;
    default:
      break;
  }
}

void LinuxtrackGui::trackerStateHandler(linuxtrack_state_type current_state)
{
  const bool isActive = (current_state == INITIALIZING || 
                        current_state == RUNNING || 
                        current_state == PAUSED);
  
  ui.DefaultsButton->setDisabled(isActive);
  ui.DiscardChangesButton->setDisabled(isActive);
  
  // Note: Other UI elements are commented out in original code
  // Uncomment and enable/disable as needed:
  // ui.DeviceSelector->setDisabled(isActive);
  // ui.CameraOrientation->setDisabled(isActive);
  // ui.ModelSelector->setDisabled(isActive);
  // ui.Profiles->setDisabled(isActive);
  // ui.LegacyPose->setDisabled(isActive);
  // ui.LegacyRotation->setDisabled(isActive);
}

void LinuxtrackGui::on_LegacyPose_stateChanged(int state)
{
  if (guiInit) return;
  
  const bool enabled = (state == Qt::Checked);
  ltr_int_set_use_alter(enabled);
  TRACKER.miscChange(MISC_ALTER, enabled);
}

void LinuxtrackGui::on_LegacyRotation_stateChanged(int state)
{
  if (guiInit) return;
  
  const bool enabled = (state == Qt::Checked);
  ltr_int_set_use_oldrot(enabled);
  TRACKER.miscChange(MISC_LEGR, enabled);
}

void LinuxtrackGui::on_TransRotDisable_stateChanged(int state)
{
  if (guiInit) return;
  
  const bool enabled = (state == Qt::Checked);
  ltr_int_set_tr_align(!enabled);  // Note: inverted logic
  TRACKER.miscChange(MISC_ALIGN, !enabled);
}

void LinuxtrackGui::on_FocalLength_valueChanged(double val)
{
  if (guiInit) return;
  
  ltr_int_set_focal_length(val);
  TRACKER.miscChange(MISC_FOCAL_LENGTH, static_cast<float>(val));
}

void LinuxtrackGui::on_PackageLogsButton_pressed()
{
  ui.PackageLogsButton->setEnabled(false);
  
  const QString fname = QFileDialog::getSaveFileName(this, QStringLiteral("Save the package as..."),
                                       QDir::homePath(), QStringLiteral("Zip (*.zip)"));
  if (fname.isEmpty()) {
    ui.PackageLogsButton->setEnabled(true);
    return;
  }
  
  QStringList args;
  args << QStringLiteral("-c") << QStringLiteral("zip %1 /tmp/linuxtrack*").arg(fname);
  zipper.start(QStringLiteral("bash"), args);
}

void LinuxtrackGui::on_LaunchMickeyButton_pressed()
{
  // Try to find mickey binary in common locations
  QStringList mickeyPaths;
  
  // Check AppImage internal path first (for AppImage distribution)
  QString appImagePath = QCoreApplication::applicationDirPath() + QStringLiteral("/mickey");
  mickeyPaths << appImagePath;
  
  // Then check system paths
  mickeyPaths << QStringLiteral("/opt/bin/mickey")
              << QStringLiteral("/usr/local/bin/mickey")
              << QStringLiteral("/usr/bin/mickey")
              << QDir::homePath() + QStringLiteral("/.local/bin/mickey");
  
  QString mickeyPath;
  for (const QString &path : mickeyPaths) {
    if (QFile::exists(path)) {
      mickeyPath = path;
      break;
    }
  }
  
  if (mickeyPath.isEmpty()) {
    warningMessage(QStringLiteral("Mickey binary not found!\n\n"
                                 "Please ensure LinuxTrack is properly installed with Mickey support.\n"
                                 "Expected locations:\n"
                                 "- AppImage internal path (for AppImage distribution)\n"
                                 "- /opt/bin/mickey\n"
                                 "- /usr/local/bin/mickey\n"
                                 "- /usr/bin/mickey"));
    return;
  }
  
  // Launch mickey
  QProcess::startDetached(mickeyPath, QStringList());
  infoMessage(QStringLiteral("Mickey (TrackIR) launched successfully!"));
}

void LinuxtrackGui::logsPackaged(int exitCode, QProcess::ExitStatus exitStatus)
{
  (void)exitCode;
  if ((exitCode == 0) && (exitStatus == QProcess::NormalExit)) {
    infoMessage(QStringLiteral("Package created successfully..."));
  } else {
    warningMessage(QStringLiteral("Couldn't create the package!\n"
    "Please check that you have write access to the destination directory!"));
  }
  ui.PackageLogsButton->setEnabled(true);
}

// Docking implementation
void LinuxtrackGui::setupDocking()
{
  // Create dock widget for tracking window
  trackingDockWidget = new QDockWidget(QStringLiteral("Tracking Window"), this); // Correct label
  trackingDockWidget->setWidget(showWindow);
  trackingDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  trackingDockWidget->setFeatures(QDockWidget::DockWidgetMovable | 
                                  QDockWidget::DockWidgetFloatable |
                                  QDockWidget::DockWidgetClosable);
  // Qt5: No setWidgetResizable, so set minimum/preferred size for both dock and content
  QSize camViewSize(640, 480);
  trackingDockWidget->setMinimumWidth(camViewSize.width());
  trackingDockWidget->setMinimumHeight(camViewSize.height());
  trackingDockWidget->resize(camViewSize);
  showWindow->setMinimumSize(camViewSize);
  showWindow->resize(camViewSize);

  // Create docking actions
  dockAction = new QAction(QStringLiteral("Dock Tracking Window"), this);
  dockAction->setStatusTip(QStringLiteral("Dock the tracking window to the main window"));
  connect(dockAction, &QAction::triggered, this, &LinuxtrackGui::dockTrackingWindow);
  
  undockAction = new QAction(QStringLiteral("Undock Tracking Window"), this);
  undockAction->setStatusTip(QStringLiteral("Undock the tracking window from the main window"));
  connect(undockAction, &QAction::triggered, this, &LinuxtrackGui::undockTrackingWindow);
  
  dockLeftAction = new QAction(QStringLiteral("Dock to Left"), this);
  dockLeftAction->setStatusTip(QStringLiteral("Dock the tracking window to the left side"));
  connect(dockLeftAction, &QAction::triggered, this, &LinuxtrackGui::dockToLeft);
  
  dockRightAction = new QAction(QStringLiteral("Dock to Right"), this);
  dockRightAction->setStatusTip(QStringLiteral("Dock the tracking window to the right side"));
  connect(dockRightAction, &QAction::triggered, this, &LinuxtrackGui::dockToRight);
  
  // Create docking menu
  createDockingMenu();
  
  // Load docking state
  loadDockingState();
  
  // Apply initial docking state
  if (isTrackingWindowDocked) {
    addDockWidget(dockArea, trackingDockWidget);
    trackingDockWidget->show();
    showWindow->show(); // Show the tracking window content when docked
  } else {
    showWindow->show();
    trackingDockWidget->hide(); // Hide the dock widget when not docked
  }
  
  updateDockingActions();

  // Ensure Quit button is connected to main window's close slot
  connect(ui.QuitButton, &QPushButton::clicked, this, &QWidget::close);
}

void LinuxtrackGui::createDockingMenu()
{
  dockingMenu = new QMenu(QStringLiteral("Tracking Window Docking"), this);
  dockingMenu->addAction(dockAction);
  dockingMenu->addAction(undockAction);
  dockingMenu->addSeparator();
  dockingMenu->addAction(dockLeftAction);
  dockingMenu->addAction(dockRightAction);
  
  // Add menu to window menu bar or create one if it doesn't exist
  if (!menuBar()) {
    setMenuBar(new QMenuBar(this));
  }
  menuBar()->addMenu(dockingMenu);
}

// Helper to update docking actions
void LinuxtrackGui::updateDockingActions()
{
    if (isTrackingWindowDocked) {
        if (dockAction) dockAction->setEnabled(false);
        if (undockAction) undockAction->setEnabled(true);
    } else {
        if (dockAction) dockAction->setEnabled(true);
        if (undockAction) undockAction->setEnabled(false);
    }
}

// Dock the tracking window into the main window
void LinuxtrackGui::dockTrackingWindow()
{
    if (!trackingDockWidget->isVisible()) {
        if (showWindow->isVisible() && showWindow->isWindow()) {
            showWindow->hide();
        }
        trackingDockWidget->setWidget(showWindow);
        if (!this->findChild<QDockWidget*>(QStringLiteral("Tracking Window"))) {
            this->addDockWidget(Qt::LeftDockWidgetArea, trackingDockWidget);
        }
        trackingDockWidget->show();
        showWindow->setParent(trackingDockWidget);
        showWindow->show();
        isTrackingWindowDocked = true;
        updateDockingActions();
    }
}

// Undock the tracking window from the main window
void LinuxtrackGui::undockTrackingWindow()
{
    if (trackingDockWidget->isVisible()) {
        trackingDockWidget->hide();
        trackingDockWidget->setWidget(nullptr);
        showWindow->setParent(nullptr);
        showWindow->setWindowFlags(Qt::Window);
        showWindow->show();
        isTrackingWindowDocked = false;
        updateDockingActions();
    }
}

void LinuxtrackGui::dockToLeft()
{
  dockArea = Qt::LeftDockWidgetArea;
  if (isTrackingWindowDocked) {
    removeDockWidget(trackingDockWidget);
    addDockWidget(dockArea, trackingDockWidget);
    trackingDockWidget->show();
  }
  saveDockingState();
}

void LinuxtrackGui::dockToRight()
{
  dockArea = Qt::RightDockWidgetArea;
  if (isTrackingWindowDocked) {
    removeDockWidget(trackingDockWidget);
    addDockWidget(dockArea, trackingDockWidget);
    trackingDockWidget->show();
  }
  saveDockingState();
}

void LinuxtrackGui::saveDockingState()
{
  gui_settings->beginGroup(QStringLiteral("Docking"));
  gui_settings->setValue(QStringLiteral("tracking_window_docked"), isTrackingWindowDocked);
  gui_settings->setValue(QStringLiteral("dock_area"), static_cast<int>(dockArea));
  gui_settings->endGroup();
}

void LinuxtrackGui::loadDockingState()
{
  gui_settings->beginGroup(QStringLiteral("Docking"));
  isTrackingWindowDocked = gui_settings->value(QStringLiteral("tracking_window_docked"), true).toBool(); // Default to docked
  dockArea = static_cast<Qt::DockWidgetArea>(
    gui_settings->value(QStringLiteral("dock_area"), static_cast<int>(Qt::LeftDockWidgetArea)).toInt()
  );
  gui_settings->endGroup();
}

// Gaming tab slot implementations
void LinuxtrackGui::on_InstallTirMfcButton_pressed()
{
    // Call the new TIR/MFC140 installation method
    if (pi) {
        pi->installTirFirmwareAndMfc140();
    }
}

void LinuxtrackGui::on_SteamProtonButton_pressed()
{
    // TODO: Implement Steam Proton installation
    QMessageBox::information(this, QString::fromUtf8("Steam Proton"),
        QString::fromUtf8("Steam Proton installation will be implemented in Phase 2."));
}

void LinuxtrackGui::on_LutrisButton_pressed()
{
    // Call the Lutris wine bridge installation method
    if (pi) {
        pi->installLutrisWineBridge();
    }
}

void LinuxtrackGui::on_OtherPlatformButton_pressed()
{
    // TODO: Implement other platform installation
    QMessageBox::information(this, QString::fromUtf8("Other Platform"),
        QString::fromUtf8("Other platform installation will be implemented in Phase 2."));
}

void LinuxtrackGui::on_CustomPrefixButton_pressed()
{
    // Call the wine bridge installation method
    if (pi) {
        pi->installWineBridgeToCustomPrefix();
    }
}

void LinuxtrackGui::on_BatchInstallButton_pressed()
{
    // TODO: Implement batch installation
    QMessageBox::information(this, QString::fromUtf8("Batch Install"),
        QString::fromUtf8("Batch installation will be implemented in Phase 2."));
}

void LinuxtrackGui::on_WinePrefixButton_pressed()
{
    // TODO: Implement direct wine prefix installation
    QMessageBox::information(this, QString::fromUtf8("Wine Prefix"),
        QString::fromUtf8("Direct wine prefix installation will be implemented in Phase 2."));
}

void LinuxtrackGui::on_LaunchLtrPipeButton_pressed()
{
    // TODO: Implement ltr_pipe launch for Antimicrox
    QMessageBox::information(this, QString::fromUtf8("Launch ltr_pipe"),
        QString::fromUtf8("ltr_pipe for Antimicrox will be implemented in Phase 2."));
}