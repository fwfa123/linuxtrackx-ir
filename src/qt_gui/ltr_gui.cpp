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
 return QMessageBox::information(nullptr, APP_TITLE, message, QMessageBox::Ok);
}

LinuxtrackGui::LinuxtrackGui(QWidget *parent) : QWidget(parent), ds(nullptr),
  xpInstall(nullptr), initialized(false), news_serial(-1), guiInit(true), showWineWarning(true)
{
  ui.setupUi(this);
  PREF;
  setWindowTitle(QStringLiteral("Linuxtrack GUI v") + QStringLiteral(PACKAGE_VERSION));
  grd = new Guardian(this);
  me = new ModelEdit(grd, this);
  lv = new LogView();
  pi = new PluginInstall(ui, this);
  ps = new ProfileSelector(this);
  QObject::connect(&STATE, SIGNAL(stateChanged(linuxtrack_state_type)),
                   this, SLOT(trackerStateHandler(linuxtrack_state_type)));
  QObject::connect(&zipper, SIGNAL(finished(int, QProcess::ExitStatus)),
                   this, SLOT(logsPackaged(int, QProcess::ExitStatus)));
  ui.ModelEditSite->addWidget(me);
  ui.ProfileSetupSite->addWidget(ps);

  gui_settings = new QSettings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
  showWindow = new LtrGuiForm(ui, *gui_settings);
  helper = new LtrDevHelp();
  
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
  showWindow->show();
  
  const QString dbg = QProcessEnvironment::systemEnvironment().value(QStringLiteral("LINUXTRACK_DBG"));
  if (dbg.contains(QChar::fromLatin1('d'))) {
    helper->show();
  }
  
  QWidget::show();
  
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
  if (initialized) {
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
    ds->refresh();
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

#include "moc_ltr_gui.cpp"


