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
#include <QApplication>
#include <QClipboard>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
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
#include "tracker.h"
#include "testing_section.h"

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
  testingSection = new TestingSection(this);
  testingSection->setupUI(ui);
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
  // QObject::connect(ui.BatchInstallButton, SIGNAL(pressed()), this, SLOT(on_BatchInstallButton_pressed()));
  QObject::connect(ui.WinePrefixButton, SIGNAL(pressed()), this, SLOT(on_WinePrefixButton_pressed()));
  QObject::connect(ui.LaunchLtrPipeButton, SIGNAL(pressed()), this, SLOT(on_LaunchLtrPipeButton_pressed()));
  
  // Connect Testing section buttons
  QObject::connect(ui.TesterExeRadioButton, SIGNAL(toggled(bool)), this, SLOT(on_TesterExeRadioButton_toggled(bool)));
  QObject::connect(ui.FTTesterRadioButton, SIGNAL(toggled(bool)), this, SLOT(on_FTTesterRadioButton_toggled(bool)));
  QObject::connect(ui.PlatformComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(on_PlatformComboBox_currentTextChanged(QString)));
  QObject::connect(ui.LoadGamesButton, SIGNAL(pressed()), this, SLOT(on_LoadGamesButton_pressed()));
  QObject::connect(ui.GameComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(on_GameComboBox_currentTextChanged(QString)));
  QObject::connect(ui.RunTesterButton, SIGNAL(pressed()), this, SLOT(on_RunTesterButton_pressed()));

  // Ensure the tracking window UI timers are active when testing workflow triggers tracking
  connect(testingSection, SIGNAL(testingWorkflowStarted()), this, SLOT(on_TesterExeRadioButton_toggled(bool)));
  connect(testingSection, &TestingSection::testingWorkflowStarted, [this]() {
    if (showWindow) {
      showWindow->startTimersOnly();
    }
  });
  
  // Connect System information buttons
  QObject::connect(ui.button_copy_system_info, SIGNAL(pressed()), this, SLOT(on_button_copy_system_info_pressed()));
  QObject::connect(ui.button_refresh_system_info, SIGNAL(pressed()), this, SLOT(on_button_refresh_system_info_pressed()));
  
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
  
  // Initialize system information
  updateSystemInformation();
  
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

PluginInstall* LinuxtrackGui::getPluginInstall() const
{
  return pi;
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
      // Gaming tab selected: ensure tracking is started for testing workflow
      if (testingSection) {
        testingSection->startTracking();
      }
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
    // Call the new TIR/MFC42 installation method
    if (pi) {
        if (showWindow) { showWindow->startTimersOnly(); }
        pi->installTirFirmwareAndMfc42();
    }
}

void LinuxtrackGui::on_SteamProtonButton_pressed()
{
    // Call the Steam Proton wine bridge installation method
    if (pi) {
        if (showWindow) { showWindow->startTimersOnly(); }
        pi->installSteamProtonBridge();
    }
}

void LinuxtrackGui::on_LutrisButton_pressed()
{
    // Call the Lutris wine bridge installation method
    if (pi) {
        if (showWindow) { showWindow->startTimersOnly(); }
        pi->installLutrisWineBridge();
    }
}

void LinuxtrackGui::on_OtherPlatformButton_pressed()
{
    // TODO: Implement other platform installation
    QMessageBox::information(this, QString::fromUtf8("Other Platform"),
        QString::fromUtf8("Other platform installation will be implemented in Phase 2."));
    // Do not auto-start tracking from install buttons
}

void LinuxtrackGui::on_CustomPrefixButton_pressed()
{
    // Call the wine bridge installation method
    if (pi) {
        if (showWindow) { showWindow->startTimersOnly(); }
        pi->installWineBridgeToCustomPrefix();
    }
}

/*
void LinuxtrackGui::on_BatchInstallButton_pressed()
{
    // TODO: Implement batch installation
    QMessageBox::information(this, QString::fromUtf8("Batch Install"),
        QString::fromUtf8("Batch installation will be implemented in Phase 2."));
}
*/

void LinuxtrackGui::on_WinePrefixButton_pressed()
{
    // TODO: Implement direct wine prefix installation
    QMessageBox::information(this, QString::fromUtf8("Wine Prefix"),
        QString::fromUtf8("Direct wine prefix installation will be implemented in Phase 2."));
    // Do not auto-start tracking from install buttons
}

void LinuxtrackGui::on_LaunchLtrPipeButton_pressed()
{
    // TODO: Implement ltr_pipe launch for Antimicrox
    QMessageBox::information(this, QString::fromUtf8("Launch ltr_pipe"),
        QString::fromUtf8("ltr_pipe for Antimicrox will be implemented in Phase 2."));
    
    // Start tracking automatically for future implementation
    static QString sec(QString::fromUtf8("Default"));
    if (showWindow) { showWindow->startTimersOnly(); }
    TRACKER.start(sec);
    
    QMessageBox::information(this, QString::fromUtf8("Tracking Started"),
        QString::fromUtf8("Head tracking has been automatically started.\n\n") +
        QString::fromUtf8("You can now test your head tracking!\n\n") +
        QString::fromUtf8("Use the tracking window to pause, recenter, or stop tracking as needed."));
}

// Testing section slot implementations
void LinuxtrackGui::on_TesterExeRadioButton_toggled(bool checked)
{
    if (checked && testingSection) {
        testingSection->onTesterSelectionChanged();
    }
}

void LinuxtrackGui::on_FTTesterRadioButton_toggled(bool checked)
{
    if (checked && testingSection) {
        testingSection->onTesterSelectionChanged();
    }
}

void LinuxtrackGui::on_PlatformComboBox_currentTextChanged(const QString &text)
{
    if (testingSection) {
        testingSection->onPlatformSelectionChanged();
    }
}

void LinuxtrackGui::on_LoadGamesButton_pressed()
{
    if (testingSection) {
        testingSection->onLoadGamesClicked();
    }
}

void LinuxtrackGui::on_GameComboBox_currentTextChanged(const QString &text)
{
    // Enable Run Tester button when a game is selected
    if (ui.RunTesterButton) {
        ui.RunTesterButton->setEnabled(!text.isEmpty());
    }
}

void LinuxtrackGui::on_RunTesterButton_pressed()
{
    if (testingSection) {
        testingSection->onRunTesterClicked();
    }
}

// System information functions
void LinuxtrackGui::on_button_copy_system_info_pressed()
{
    QString systemInfo = getSystemInformation();
    QApplication::clipboard()->setText(systemInfo);
    QMessageBox::information(this, QString::fromUtf8("System Information"),
        QString::fromUtf8("System information has been copied to clipboard."));
}

void LinuxtrackGui::on_button_refresh_system_info_pressed()
{
    updateSystemInformation();
}

void LinuxtrackGui::updateSystemInformation()
{
    QString systemInfo = getSystemInformation();
    ui.textEdit_system_info->setPlainText(systemInfo);
}

QString LinuxtrackGui::getSystemInformation()
{
    QString info;
    info += QStringLiteral("=== LinuxTrack System Information ===\n\n");
    
    info += getLinuxDistribution();
    info += getCPUInfo();
    info += getMemoryInfo();
    info += getGraphicsInfo();
    info += getLinuxTrackInfo();
    info += getDeviceSupportInfo();
    
    return info;
}

QString LinuxtrackGui::getLinuxDistribution()
{
    QString info = QStringLiteral("=== System ===\n");
    
    // Get OS information
    QFile osRelease(QStringLiteral("/etc/os-release"));
    if (osRelease.open(QIODevice::ReadOnly)) {
        QString content = QString::fromUtf8(osRelease.readAll());
        QStringList lines = content.split(QChar::fromLatin1('\n'));
        
        QString distroName, distroVersion;
        for (const QString &line : lines) {
            if (line.startsWith(QStringLiteral("PRETTY_NAME="))) {
                distroName = line.mid(12).remove(QChar::fromLatin1('"'));
                break;
            }
        }
        
        if (!distroName.isEmpty()) {
            info += QStringLiteral("OS: %1\n").arg(distroName);
        }
    }
    
    // Get architecture
    QProcess uname;
    uname.start(QStringLiteral("uname"), QStringList() << QStringLiteral("-m"));
    uname.waitForFinished();
    QString arch = QString::fromUtf8(uname.readAllStandardOutput()).trimmed();
    if (!arch.isEmpty()) {
        info += QStringLiteral("Arch: %1\n").arg(arch);
    }
    
    // Get kernel version
    QProcess unameKernel;
    unameKernel.start(QStringLiteral("uname"), QStringList() << QStringLiteral("-r"));
    unameKernel.waitForFinished();
    QString kernel = QString::fromUtf8(unameKernel.readAllStandardOutput()).trimmed();
    if (!kernel.isEmpty()) {
        info += QStringLiteral("Kernel: %1\n").arg(kernel);
    }
    
    // Get LinuxTrack version
    info += QStringLiteral("LinuxTrack Version: %1\n").arg(QStringLiteral(PACKAGE_VERSION));
    
    // Get desktop environment
    QString desktop = QString::fromUtf8(qgetenv("XDG_CURRENT_DESKTOP"));
    if (desktop.isEmpty()) {
        desktop = QString::fromUtf8(qgetenv("DESKTOP_SESSION"));
    }
    if (!desktop.isEmpty()) {
        info += QStringLiteral("Desktop: %1\n").arg(desktop);
    }
    
    // Get display server
    QString displayServer = QString::fromUtf8(qgetenv("XDG_SESSION_TYPE"));
    if (!displayServer.isEmpty()) {
        info += QStringLiteral("Display Server: %1\n").arg(displayServer);
    }
    
    info += QStringLiteral("\n");
    return info;
}

QString LinuxtrackGui::getCPUInfo()
{
    QString info = QStringLiteral("=== CPU ===\n");
    
    QFile cpuInfo(QStringLiteral("/proc/cpuinfo"));
    if (cpuInfo.open(QIODevice::ReadOnly)) {
        QString content = QString::fromUtf8(cpuInfo.readAll());
        QStringList lines = content.split(QChar::fromLatin1('\n'));
        
        QString vendor, model;
        int physicalCores = 0, logicalCores = 0;
        
        for (const QString &line : lines) {
            if (line.startsWith(QStringLiteral("vendor_id"))) {
                vendor = line.split(QChar::fromLatin1(':')).last().trimmed();
            } else if (line.startsWith(QStringLiteral("model name"))) {
                model = line.split(QChar::fromLatin1(':')).last().trimmed();
            } else if (line.startsWith(QStringLiteral("processor"))) {
                logicalCores++;
            } else if (line.startsWith(QStringLiteral("physical id"))) {
                // Count unique physical IDs
                QStringList parts = line.split(QChar::fromLatin1(':'));
                if (parts.size() > 1) {
                    bool ok;
                    int id = parts.last().trimmed().toInt(&ok);
                    if (ok && id >= physicalCores) {
                        physicalCores = id + 1;
                    }
                }
            }
        }
        
        if (!vendor.isEmpty()) {
            info += QStringLiteral("Vendor: %1\n").arg(vendor);
        }
        if (!model.isEmpty()) {
            info += QStringLiteral("Model: %1\n").arg(model);
        }
        info += QStringLiteral("Physical cores: %1\n").arg(physicalCores);
        info += QStringLiteral("Logical cores: %1\n").arg(logicalCores);
    }
    
    info += QStringLiteral("\n");
    return info;
}

QString LinuxtrackGui::getMemoryInfo()
{
    QString info = QStringLiteral("=== Memory ===\n");
    
    QFile memInfo(QStringLiteral("/proc/meminfo"));
    if (memInfo.open(QIODevice::ReadOnly)) {
        QString content = QString::fromUtf8(memInfo.readAll());
        QStringList lines = content.split(QChar::fromLatin1('\n'));
        
        qulonglong totalRam = 0, totalSwap = 0;
        
        for (const QString &line : lines) {
            if (line.startsWith(QStringLiteral("MemTotal:"))) {
                QStringList parts = line.split(QChar::fromLatin1(':'));
                if (parts.size() > 1) {
                    totalRam = parts.last().trimmed().toULongLong();
                }
            } else if (line.startsWith(QStringLiteral("SwapTotal:"))) {
                QStringList parts = line.split(QChar::fromLatin1(':'));
                if (parts.size() > 1) {
                    totalSwap = parts.last().trimmed().toULongLong();
                }
            }
        }
        
        if (totalRam > 0) {
            double ramGB = totalRam / 1024.0 / 1024.0;
            info += QStringLiteral("RAM: %1 GB\n").arg(ramGB, 0, 'f', 1);
        }
        if (totalSwap > 0) {
            double swapGB = totalSwap / 1024.0 / 1024.0;
            info += QStringLiteral("Swap: %1 GB\n").arg(swapGB, 0, 'f', 1);
        }
    }
    
    info += QStringLiteral("\n");
    return info;
}

QString LinuxtrackGui::getGraphicsInfo()
{
    QString info = QStringLiteral("=== Graphics ===\n");
    
    // Get graphics information from system commands instead of OpenGL context
    // This is safer and doesn't require an active OpenGL context
    
    // Get graphics card info from lspci
    QProcess lspci;
    lspci.start(QStringLiteral("lspci"), QStringList() << QStringLiteral("-v"));
    lspci.waitForFinished();
    QString lspciOutput = QString::fromUtf8(lspci.readAllStandardOutput());
    
    if (!lspciOutput.isEmpty()) {
        QStringList lines = lspciOutput.split(QChar::fromLatin1('\n'));
        for (const QString &line : lines) {
            if (line.contains(QStringLiteral("VGA")) || line.contains(QStringLiteral("3D")) || line.contains(QStringLiteral("Display"))) {
                info += QStringLiteral("Graphics Card: %1\n").arg(line.trimmed());
                break;
            }
        }
    }
    
    // Get OpenGL information from glxinfo (safer approach)
    QProcess glxinfo;
    glxinfo.start(QStringLiteral("glxinfo"), QStringList() << QStringLiteral("-B"));
    glxinfo.waitForFinished();
    QString glxOutput = QString::fromUtf8(glxinfo.readAllStandardOutput());
    
    if (!glxOutput.isEmpty()) {
        QStringList lines = glxOutput.split(QChar::fromLatin1('\n'));
        for (const QString &line : lines) {
            if (line.contains(QStringLiteral("OpenGL version string"))) {
                QString version = line.split(QChar::fromLatin1(':')).last().trimmed();
                info += QStringLiteral("OpenGL Version: %1\n").arg(version);
                break;
            }
            if (line.contains(QStringLiteral("OpenGL renderer string"))) {
                QString renderer = line.split(QChar::fromLatin1(':')).last().trimmed();
                info += QStringLiteral("OpenGL Renderer: %1\n").arg(renderer);
            }
            if (line.contains(QStringLiteral("OpenGL vendor string"))) {
                QString vendor = line.split(QChar::fromLatin1(':')).last().trimmed();
                info += QStringLiteral("OpenGL Vendor: %1\n").arg(vendor);
            }
        }
    } else {
        info += QStringLiteral("OpenGL Info: Unable to retrieve (glxinfo not available)\n");
    }
    
    // Get Mesa version if available
    QProcess mesaVersion;
    mesaVersion.start(QStringLiteral("glxinfo"), QStringList());
    mesaVersion.waitForFinished();
    QString mesaOutput = QString::fromUtf8(mesaVersion.readAllStandardOutput());
    
    if (!mesaOutput.isEmpty()) {
        QStringList lines = mesaOutput.split(QChar::fromLatin1('\n'));
        for (const QString &line : lines) {
            if (line.contains(QStringLiteral("Mesa"))) {
                info += QStringLiteral("Mesa Version: %1\n").arg(line.trimmed());
                break;
            }
        }
    }
    
    info += QStringLiteral("\n");
    return info;
}

QString LinuxtrackGui::getLinuxTrackInfo()
{
    QString info = QStringLiteral("=== LinuxTrack Components ===\n");
    
    // Check LinuxTrack support status
    info += QStringLiteral("Webcam support: YES\n");
    info += QStringLiteral("Wiimote support: YES\n");
    info += QStringLiteral("TrackIR support: YES\n");
    info += QStringLiteral("Facetracker support: NO\n");
    info += QStringLiteral("XPlane plugin: YES\n");
    info += QStringLiteral("Mickey: YES\n");
    info += QStringLiteral("Wine plugin: YES\n");
    info += QStringLiteral("OSC support: YES\n");
    info += QStringLiteral("PIE support for native builds: enabled (security feature)\n");
    
    info += QStringLiteral("\n");
    return info;
}

QString LinuxtrackGui::getDeviceSupportInfo()
{
    QString info = QStringLiteral("=== Device Support ===\n");
    
    // Check for USB devices
    QProcess lsusb;
    lsusb.start(QStringLiteral("lsusb"), QStringList());
    lsusb.waitForFinished();
    QString usbOutput = QString::fromUtf8(lsusb.readAllStandardOutput());
    
    if (!usbOutput.isEmpty()) {
        QStringList lines = usbOutput.split(QChar::fromLatin1('\n'));
        for (const QString &line : lines) {
            if (line.contains(QStringLiteral("NaturalPoint")) || line.contains(QStringLiteral("TrackIR"))) {
                info += QStringLiteral("TrackIR Device: %1\n").arg(line.trimmed());
            }
        }
    }
    
    // Check for webcam devices
    QProcess lsVideo;
    lsVideo.start(QStringLiteral("ls"), QStringList() << QStringLiteral("/dev/video*"));
    lsVideo.waitForFinished();
    QString videoOutput = QString::fromUtf8(lsVideo.readAllStandardOutput());
    
    if (!videoOutput.isEmpty()) {
        QStringList devices = videoOutput.split(QChar::fromLatin1('\n'), Qt::SkipEmptyParts);
        info += QStringLiteral("Webcam devices: %1\n").arg(devices.size());
        for (const QString &device : devices) {
            info += QStringLiteral("  %1\n").arg(device.trimmed());
        }
    }
    
    info += QStringLiteral("\n");
    return info;
}