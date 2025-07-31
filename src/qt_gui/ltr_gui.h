#ifndef LTR_GUI__H
#define LTR_GUI__H

#ifdef HAVE_CONFIG_H
  #include "../../config.h"
#endif

#include <QCloseEvent>
#include <QProcess>
#include <QMainWindow>
#include <QDockWidget>
#include <QAction>
#include <QMenu>
#include <linuxtrack.h>

#include "ui_ltr.h"

class LtrGuiForm;
class LtrDevHelp;
class ModelEdit;
//class LtrTracking;
class LogView;
//class ScpForm;
class QSettings;
class HelpViewer;
class PluginInstall;
class DeviceSetup;
class ProfileSelector;
class Guardian;
class XPluginInstall;

class LinuxtrackGui : public QMainWindow
{
  Q_OBJECT
 public:
  LinuxtrackGui(QWidget *parent = 0);
  ~LinuxtrackGui();
 protected:
  void closeEvent(QCloseEvent *event);
 public slots:
  void show();
 private slots:
  void on_QuitButton_pressed();
  //void on_EditSCButton_pressed();
  void on_SaveButton_pressed();
  void on_DefaultsButton_pressed();
  void on_DiscardChangesButton_pressed();
  void on_HelpButton_pressed();
  void on_LtrTab_currentChanged(int index);
  void trackerStateHandler(linuxtrack_state_type current_state);
  void on_LegacyPose_stateChanged(int state);
  void on_LegacyRotation_stateChanged(int state);
  void on_FocalLength_valueChanged(double val);
  void on_TransRotDisable_stateChanged(int state);
  void on_ViewLogButton_pressed();
  void on_XplanePluginButton_pressed();
  void on_PackageLogsButton_pressed();
  void on_LaunchMickeyButton_pressed();
  void logsPackaged(int exitCode, QProcess::ExitStatus exitStatus);
  
  // Gaming tab slots
  void on_InstallTirMfcButton_pressed();
  void on_SteamProtonButton_pressed();
  void on_LutrisButton_pressed();
  void on_OtherPlatformButton_pressed();
  void on_CustomPrefixButton_pressed();
  void on_BatchInstallButton_pressed();
  void on_WinePrefixButton_pressed();
  void on_LaunchLtrPipeButton_pressed();
  
  // Docking slots
  void dockTrackingWindow();
  void undockTrackingWindow();
  void dockToLeft();
  void dockToRight();
  void createDockingMenu();
  
 private:
  QWidget *mainWidget;
  Ui::LinuxtrackMainForm ui;
  LtrGuiForm *showWindow;
  LtrDevHelp *helper;
  DeviceSetup *ds;
  ModelEdit *me;
  Guardian *grd;
  //LtrTracking *track;
  //ScpForm *sc;
  LogView *lv;
  PluginInstall *pi;
  ProfileSelector *ps;
  XPluginInstall *xpInstall;
  bool initialized;
  QSettings *gui_settings;
  void rereadPrefs();
  bool welcome;
  int news_serial;
  QProcess zipper;
  bool guiInit;
  bool showWineWarning;
  
  // Docking members
  QDockWidget *trackingDockWidget;
  QAction *dockAction;
  QAction *undockAction;
  QAction *dockLeftAction;
  QAction *dockRightAction;
  QMenu *dockingMenu;
  bool isTrackingWindowDocked;
  Qt::DockWidgetArea dockArea;
  
  void setupDocking();
  void saveDockingState();
  void loadDockingState();
  void updateDockingActions();
};

#endif
