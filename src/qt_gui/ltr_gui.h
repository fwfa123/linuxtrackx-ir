#ifndef LTR_GUI__H
#define LTR_GUI__H

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include <QCloseEvent>
#include <QShowEvent>
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
class TestingSection;
class AboutDialog;
class UpdateGamesExtractor;

class LinuxtrackGui : public QMainWindow
{
  Q_OBJECT
 public:
  LinuxtrackGui(QWidget *parent = 0);
  ~LinuxtrackGui();
  // Expose PluginInstall instance for TestingSection integration
  PluginInstall* getPluginInstall() const;
 protected:
  void closeEvent(QCloseEvent *event);
  void showEvent(QShowEvent *event) override;
 public slots:
  void show();
 private slots:
  void on_QuitButton_pressed();
  //void on_EditSCButton_pressed();
  void on_SaveButton_pressed();
  void on_DefaultsButton_pressed();
  void on_DiscardChangesButton_pressed();
  void on_HelpButton_pressed();
  void on_AboutAction_triggered();
  void on_LtrTab_currentChanged(int index);
  void trackerStateHandler(linuxtrack_state_type current_state);
  void on_LegacyPose_stateChanged(int state);
  void on_LegacyRotation_stateChanged(int state);
  void on_FocalLength_valueChanged(double val);
  void on_TransRotDisable_stateChanged(int state);
  void on_VideoOnDelayMode_activated(int index);
  void on_VideoOnDelayValue_valueChanged(int value);
  void on_UsbResetCheck_toggled(bool checked);
  void on_ViewLogButton_pressed();
  void on_XplanePluginButton_pressed();
  void on_PackageLogsButton_pressed();
  void on_LaunchMickeyButton_pressed();
  void logsPackaged(int exitCode, QProcess::ExitStatus exitStatus);
  
  // Gaming tab slots
  void on_InstallTirMfcButton_pressed();
  void on_SteamProtonButton_pressed();
  void on_LutrisButton_pressed();
  void on_CustomPrefixButton_pressed();
  // void on_BatchInstallButton_pressed();
  void on_OtherPlatformButton_pressed();
  void on_WinePrefixButton_pressed();
  void on_UpdateGamesButton_pressed();
  
  // ltr_pipe control slots
  void on_FormatComboBox_currentTextChanged(const QString &text);
  void on_StartLtrPipeButton_pressed();
  void on_StopLtrPipeButton_pressed();
  void on_PauseLtrPipeButton_pressed();
  void on_DeviceNameEdit_textChanged(const QString &text);

  // ltr_udp control slots
  void on_StartLtrUdpButton_pressed();
  void on_StopLtrUdpButton_pressed();
  void on_LtrUdpProtocolComboBox_currentTextChanged(const QString &text);
  void on_LtrUdpIpEdit_textChanged(const QString &text);
  void on_LtrUdpPortSpinBox_valueChanged(int value);
  
  // Testing section slots
  void on_TesterExeRadioButton_toggled(bool checked);
  void on_FTTesterRadioButton_toggled(bool checked);
  void on_ControllerExeRadioButton_toggled(bool checked);
  void on_PlatformComboBox_currentTextChanged(const QString &text);
  void on_LoadGamesButton_pressed();
  void on_GameComboBox_currentTextChanged(const QString &text);
  void on_RunTesterButton_pressed();
  
  // System information slots
  void on_button_copy_system_info_pressed();
  void on_button_refresh_system_info_pressed();
  void updateSystemInformation();
  QString getSystemInformation();
  QString getLinuxDistribution();
  QString getCPUInfo();
  QString getMemoryInfo();
  QString getGraphicsInfo();
  QString getLinuxTrackInfo();
  
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
  TestingSection *testingSection;
  PluginInstall *pi;
  ProfileSelector *ps;
  XPluginInstall *xpInstall;
  UpdateGamesExtractor *updateGamesExtractor;
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

  // Gaming prerequisites helpers
  void refreshGamingPrereqStatus();
  void setGamingControlsEnabled(bool enabled);

  // general init functions
  void initVideoOnDelay();
  void initUsbReset();

  // helper function to find the linuxtrack library path for sub-processes
  QString findLinuxtrackLibPath(const QString &binaryPath);
  
  // ltr_pipe helper functions
  QString findLtrPipeExecutable();
  QStringList buildLtrPipeArguments(const QString &format, const QString &deviceName);
  void stopLtrPipeProcess();
  void pauseLtrPipeProcess();
  void resumeLtrPipeProcess();
  void cleanupUinputDevices();
  void initializeLtrPipeInterface();

  // ltr_udp helper functions
  QString findLtrUdpExecutable();
  void initializeLtrUdpInterface();
  void stopLtrUdpProcess();

  // keep track of tracker state
  linuxtrack_state_type trackerState = STOPPED;
  
  // ltr_pipe state management
  bool ltrPipePaused = false;

  // ltr_udp state management
  bool ltrUdpRunning = false;
};

#endif
