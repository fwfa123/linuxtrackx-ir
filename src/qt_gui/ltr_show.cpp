#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QThread>
#include <QPainter>
#include <QSettings>
#include <iostream>
#include <ltr_show.h>
#include <ltr_gui_prefs.h>
#include <cal.h>
#include <utils.h>
#include <pref_global.h>
#include <pref.hpp>
#include <tracking.h>
#include <iostream>
#include <scp_form.h>
#include <ltr_state.h>
#include <string.h>

#include <linuxtrack.h>
#include <ltr_server.h>
#include <ipc_utils.h>
#include <unistd.h>
#include <tracker.h>
#include <ltlib_int.h>

#include "buffering.h"
#include <QApplication>
#include "ltr_gui.h"
#include "../mickey/hotkey.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

QWidget *label;
static bool running = false;
static bool camViewEnable = true;
static int cnt = 0;
static int frames = 0;
static float fps_buffer[8] ={0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static int fps_ptr = 0;
//!!!TBD multithread sync!!!

// Hotkey ID constants
static const int HOTKEY_TOGGLE_TRACKING = 0;
static const int HOTKEY_QUICK_RECENTER = 1;



LtrGuiForm::LtrGuiForm(const Ui::LinuxtrackMainForm &tmp_gui, QSettings &settings)
              : glw(NULL), cv(NULL), allowClose(false), main_gui(tmp_gui),
                contextMenu(nullptr), dockAction(nullptr), undockAction(nullptr),
                toggleHotKey(nullptr), recenterHotKey(nullptr), hotkeySettings(&settings)
{
  ui.setupUi(this);
  cv = new CameraView(label);
  ui.pix_box->addWidget(cv);
  trackerStopped();
  settings.beginGroup(QString::fromUtf8("TrackingWindow"));
  camViewEnable = ! settings.value(QString::fromUtf8("camera_view"), false).toBool();
  bool check3DV = settings.value(QString::fromUtf8("3D_view"), false).toBool();
  settings.endGroup();
  main_gui.DisableCamView->setCheckState(camViewEnable ? Qt::Unchecked : Qt::Checked);
  main_gui.Disable3DView->setCheckState(check3DV ? Qt::Checked : Qt::Unchecked);
  glw = new Window(ui.tabWidget, main_gui.Disable3DView);
  ui.ogl_box->addWidget(glw);
  timer = new QTimer(this);
  fpsTimer = new QTimer(this);
  stopwatch = new QElapsedTimer();
  frames = 0;
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  connect(fpsTimer, SIGNAL(timeout()), this, SLOT(updateFps()));
  if(!connect(&TRACKER, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)))){
    //std::cout<<"Problem connecting signal1!\n";
  }
  if(!connect(&TRACKER, SIGNAL(newFrame(struct frame_type *)), 
              this, SLOT(newFrameDelivered(struct frame_type *)))){
    //std::cout<<"Problem connecting signal2!\n";
  }
  connect(main_gui.DisableCamView, SIGNAL(stateChanged(int)), 
          this, SLOT(disableCamView_stateChanged(int)));
  connect(main_gui.Disable3DView, SIGNAL(stateChanged(int)), 
          this, SLOT(disable3DView_stateChanged(int)));
  
  // Create context menu
  createContextMenu();
  
  // Set a reasonable default size for the tracking window (appropriate for camera view)
  QSize camViewSize(640, 480);
  setMinimumSize(camViewSize);
  resize(camViewSize);
  
  // Initialize hotkeys
  initHotkeys();
}

//Assuming that frame dimensions can't change while running!!!
void LtrGuiForm::newFrameDelivered(struct frame_type *frame)
{
  (void) frame;
  if(cnt == 0){
    TRACKER.recenter();
  }
  ++cnt;
  ++frames;
}


void LtrGuiForm::updateFps()
{
  int msec = stopwatch->restart();
  if(msec > 0){
    fps_buffer[fps_ptr] = 1000 * frames / msec;
    fps_ptr = (fps_ptr + 1) & 7;
    frames = 0;
  }
}

LtrGuiForm::~LtrGuiForm()
{
  if(running){
    TRACKER.stop();
  }

  // Clean up hotkeys
  if(toggleHotKey){
    delete toggleHotKey;
  }
  if(recenterHotKey){
    delete recenterHotKey;
  }

  delete glw;
}

void LtrGuiForm::StorePrefs(QSettings &settings)
{
  bool camEna = (main_gui.DisableCamView->checkState() == Qt::Checked) ? true : false;
  bool tdEna = (main_gui.Disable3DView->checkState() == Qt::Checked) ? true : false;
  settings.beginGroup(QString::fromUtf8("TrackingWindow"));
  settings.setValue(QString::fromUtf8("camera_view"), camEna);
  settings.setValue(QString::fromUtf8("3D_view"), tdEna);
  settings.endGroup();
}



void LtrGuiForm::on_startButton_pressed()
{
  timer->start(50);
  fpsTimer->start(250);
  stopwatch->start();
  static QString sec(QString::fromUtf8("Default"));
  TRACKER.start(sec);
}

void LtrGuiForm::startTimersOnly()
{
  timer->start(50);
  fpsTimer->start(250);
  stopwatch->start();
}

void LtrGuiForm::on_recenterButton_pressed()
{
  TRACKER.recenter();
}

void LtrGuiForm::on_pauseButton_pressed()
{
  TRACKER.pause();
}

void LtrGuiForm::on_wakeButton_pressed()
{
  TRACKER.wakeup();
}


void LtrGuiForm::on_stopButton_pressed()
{
  TRACKER.stop();
  timer->stop();
  fpsTimer->stop();
}

void LtrGuiForm::disableCamView_stateChanged(int state)
{
  if(state == Qt::Checked){
    camViewEnable = false;
  }else{
    camViewEnable = true;
  }
}

void LtrGuiForm::disable3DView_stateChanged(int state)
{
  if(state == Qt::Checked){
    glw->close_widget();
  }else{
    glw->prepare_widget();
  }
}

void LtrGuiForm::update()
{
  float fps_mean = 0.0f;
  int i;
  for(i = 0; i < 8; ++i){
    fps_mean += fps_buffer[i];
  }
  int fps = fps_mean / 8.0;
  ui.status->setText(QString::fromUtf8("%1.frame @ %2 fps").arg(cnt).arg(fps, 4));
  cv->redraw();  
}

void LtrGuiForm::stateChanged(int current_state)
{
  switch(current_state){
    case INITIALIZING:
    case RUNNING:
      trackerRunning();
      break;
    case PAUSED:
      trackerPaused();
      break;
    default:
      trackerStopped();
      break;
  }
}

void LtrGuiForm::trackerStopped()
{
  running = false;
  ui.startButton->setDisabled(false);
  ui.pauseButton->setDisabled(true);
  ui.wakeButton->setDisabled(true);
  ui.stopButton->setDisabled(true);
  ui.recenterButton->setDisabled(true);
}

void LtrGuiForm::trackerRunning()
{
  running = true;
  ui.startButton->setDisabled(true);
  ui.pauseButton->setDisabled(false);
  ui.wakeButton->setDisabled(true);
  ui.stopButton->setDisabled(false);
  ui.recenterButton->setDisabled(false);
}

void LtrGuiForm::trackerPaused()
{
  running = true;
  ui.startButton->setDisabled(true);
  ui.pauseButton->setDisabled(true);
  ui.wakeButton->setDisabled(false);
  ui.stopButton->setDisabled(false);
  ui.recenterButton->setDisabled(true);
}

void LtrGuiForm::closeEvent(QCloseEvent *event)
{
  if(allowClose){
    event->accept();
  }else{
    event->ignore();
  }
}

void LtrGuiForm::allowCloseWindow()
{
  allowClose = true;
}

void LtrGuiForm::on_tabWidget_currentChanged(int index)
{
  if(glw == NULL){
    return;
  }
  if(index == 1){
    glw->resumeUpdates();
  }else{
    glw->stopUpdates();
  }
}


CameraView::CameraView(QWidget *parent)
  : QWidget(parent)
{
  scene = new QGraphicsScene();
  item = new QGraphicsPixmapItem();
  scene->addItem(item);
  view = new QGraphicsView();
  view->setScene(scene);
  layout = new QVBoxLayout();
  layout->addWidget(view);
  setLayout(layout);
}

CameraView::~CameraView()
{
}

void CameraView::redraw()
{
  if(!camViewEnable){
    return;
  }
  buffer *b;
  buffering *buf = TRACKER.getBuffers();
  if(buf->readBuffer(&b)){
    item->setPixmap(QPixmap::fromImage(*(b->getImage())));
    buf->bufferRead();
  }
}

// Context menu implementation
void LtrGuiForm::createContextMenu()
{
  contextMenu = new QMenu(this);
  
  dockAction = new QAction(QStringLiteral("Dock to Main Window"), this);
  dockAction->setStatusTip(QStringLiteral("Dock this window to the main Linuxtrack window"));
  connect(dockAction, &QAction::triggered, this, &LtrGuiForm::dockToMainWindow);
  
  undockAction = new QAction(QStringLiteral("Undock from Main Window"), this);
  undockAction->setStatusTip(QStringLiteral("Undock this window from the main Linuxtrack window"));
  connect(undockAction, &QAction::triggered, this, &LtrGuiForm::undockFromMainWindow);
  
  contextMenu->addAction(dockAction);
  contextMenu->addAction(undockAction);
  
  updateContextMenu();
}

void LtrGuiForm::contextMenuEvent(QContextMenuEvent *event)
{
  updateContextMenu();
  contextMenu->exec(event->globalPos());
}

void LtrGuiForm::updateContextMenu()
{
  // Check if we're currently docked by looking for a parent QDockWidget
  QWidget *parent = this->parentWidget();
  bool isDocked = false;
  
  while (parent) {
    if (qobject_cast<QDockWidget*>(parent)) {
      isDocked = true;
      break;
    }
    parent = parent->parentWidget();
  }
  
  dockAction->setEnabled(!isDocked);
  undockAction->setEnabled(isDocked);
}

void LtrGuiForm::dockToMainWindow()
{
  // Try to find the main window (LinuxtrackGui) robustly
  QWidget *mainWindow = nullptr;
  QWidget *w = this;
  // Walk up the parent chain
  while (w->parentWidget()) w = w->parentWidget();
  // If the parent chain doesn't yield a LinuxtrackGui, search top-level widgets
  mainWindow = qobject_cast<LinuxtrackGui*>(w);
  if (!mainWindow) {
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
      mainWindow = qobject_cast<LinuxtrackGui*>(widget);
      if (mainWindow) break;
    }
  }
  if (mainWindow) {
    QMetaObject::invokeMethod(mainWindow, "dockTrackingWindow", Qt::DirectConnection);
  }
}

void LtrGuiForm::undockFromMainWindow()
{
  QWidget *mainWindow = nullptr;
  QWidget *w = this;
  while (w->parentWidget()) w = w->parentWidget();
  mainWindow = qobject_cast<LinuxtrackGui*>(w);
  if (!mainWindow) {
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
      mainWindow = qobject_cast<LinuxtrackGui*>(widget);
      if (mainWindow) break;
    }
  }
  if (mainWindow) {
    QMetaObject::invokeMethod(mainWindow, "undockTrackingWindow", Qt::DirectConnection);
  }
}

// Hotkey implementation
static HotKey* addHotKey(const QString &label, const QString &prefId, int id, 
		  QWidget *owner, QObject *target, QGridLayout *dest, QSettings *pref, int row, int column)
{
  HotKey *hk = new HotKey(label, prefId, id, owner);
  QString hkString = pref->value(prefId, QString::fromUtf8("None")).toString();
  if(hk->setHotKey(hkString)){
    dest->addWidget(hk, row, column);
    QObject::connect(hk, SIGNAL(activated(int, bool)), target, SLOT(hotKey_activated(int, bool)));
    QObject::connect(hk, SIGNAL(newHotKey(const QString &, const QString &)), 
		     owner, SLOT(updateHotKey(const QString &, const QString &)));
    return hk;
  }else{
    delete hk;
    return NULL;
  }
}

void LtrGuiForm::initHotkeys()
{
  // Create hotkey layout dynamically - insert after the buttons but before the status label
  QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
  if(!mainLayout){
    return; // Cannot find main layout
  }

  // Find the position of the horizontal layout (buttons) in the main layout
  int buttonLayoutIndex = -1;
  for(int i = 0; i < mainLayout->count(); ++i){
    QLayoutItem *item = mainLayout->itemAt(i);
    if(item && item->layout()){
      QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(item->layout());
      if(hLayout){ // Found the horizontal button layout
        buttonLayoutIndex = i;
        break;
      }
    }
  }

  if(buttonLayoutIndex == -1){
    return; // Could not find button layout
  }

  // Create hotkey frame with compact horizontal layout
  QFrame *hotkeyFrame = new QFrame();
  hotkeyFrame->setFrameShape(QFrame::StyledPanel);
  hotkeyFrame->setFrameShadow(QFrame::Raised);
  // Set size policy to prevent excessive expansion
  hotkeyFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

  QHBoxLayout *hotkeyHBox = new QHBoxLayout(hotkeyFrame);
  hotkeyHBox->setContentsMargins(8, 5, 8, 5);
  hotkeyHBox->setSpacing(10);

  // Add compact informational label with tooltip
  QLabel *infoLabel = new QLabel(QString::fromUtf8("⚠️ Global Hotkeys:"));
  QString tooltipText = QString::fromUtf8("These hotkeys work system-wide. For per-game hotkeys, use controller.exe installed via Wine Bridge in each game prefix.\n\nWarning: Do not run both global hotkeys and controller.exe simultaneously - they will conflict!");
  infoLabel->setToolTip(tooltipText);
  infoLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
  hotkeyHBox->addWidget(infoLabel);

  // Create grid layout for hotkeys (arranged horizontally in single row)
  QGridLayout *hotkeyLayout = new QGridLayout();
  hotkeyLayout->setHorizontalSpacing(10);
  hotkeyLayout->setVerticalSpacing(0);
  hotkeyLayout->setContentsMargins(0, 0, 0, 0);

  hotkeySettings->beginGroup(QString::fromUtf8("HotKeys"));

  toggleHotKey = addHotKey(QString::fromUtf8("Pause/Resume:"), QString::fromUtf8("tracking_toggle"),
			   HOTKEY_TOGGLE_TRACKING, this, this, hotkeyLayout, hotkeySettings, 0, 0);
  if(!toggleHotKey){
    QMessageBox::warning(this, QString::fromUtf8("Hotkey Setup Warning"),
      QString::fromUtf8("Failed to register pause/resume hotkey. Hotkey functionality may be limited."));
  }

  recenterHotKey = addHotKey(QString::fromUtf8("Recenter:"), QString::fromUtf8("quick_recenter"),
			   HOTKEY_QUICK_RECENTER, this, this, hotkeyLayout, hotkeySettings, 0, 1);
  if(!recenterHotKey){
    QMessageBox::warning(this, QString::fromUtf8("Hotkey Setup Warning"),
      QString::fromUtf8("Failed to register recenter hotkey. Hotkey functionality may be limited."));
  }

  hotkeySettings->endGroup();

  hotkeyHBox->addLayout(hotkeyLayout);

  // Add clear button inline
  QPushButton *clearButton = new QPushButton(QString::fromUtf8("Clear"));
  clearButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
  connect(clearButton, SIGNAL(pressed()), this, SLOT(clearHotkeys()));
  hotkeyHBox->addWidget(clearButton);

  // Add spacer to push everything to the left
  hotkeyHBox->addStretch();

  // Insert hotkey frame after the button layout
  mainLayout->insertWidget(buttonLayoutIndex + 1, hotkeyFrame);
}

void LtrGuiForm::hotKey_activated(int id, bool pressed)
{
  if(!pressed){
    return;
  }

  // Only activate hotkeys when tracking is active (running or paused)
  linuxtrack_state_type current_state = ltr_int_get_tracking_state();
  if(current_state != RUNNING && current_state != PAUSED){
    return; // Tracker not active, ignore hotkey
  }

  switch(id){
    case HOTKEY_TOGGLE_TRACKING: // toggle pause/resume
      {
        if(current_state == RUNNING){
          TRACKER.pause();
        }else if(current_state == PAUSED){
          TRACKER.wakeup();
        }
      }
      break;
    case HOTKEY_QUICK_RECENTER: // quick recenter
      TRACKER.recenter();
      break;
  }
}

void LtrGuiForm::updateHotKey(const QString &prefId, const QString &hk)
{
  hotkeySettings->beginGroup(QString::fromUtf8("HotKeys"));
  hotkeySettings->setValue(prefId, hk);
  hotkeySettings->endGroup();
  
  // Sync to mickey.conf
  syncHotkeysToMickey();
}

void LtrGuiForm::syncHotkeysToMickey()
{
  QSettings mickeySettings(QString::fromUtf8("linuxtrack"), QString::fromUtf8("mickey"));
  
  hotkeySettings->beginGroup(QString::fromUtf8("HotKeys"));
  QString trackingToggle = hotkeySettings->value(QString::fromUtf8("tracking_toggle"), QString::fromUtf8("None")).toString();
  QString quickRecenter = hotkeySettings->value(QString::fromUtf8("quick_recenter"), QString::fromUtf8("None")).toString();
  hotkeySettings->endGroup();
  
  mickeySettings.beginGroup(QString::fromUtf8("HotKeys"));
  mickeySettings.setValue(QString::fromUtf8("tracking_toggle"), trackingToggle);
  mickeySettings.setValue(QString::fromUtf8("quick_recenter"), quickRecenter);
  mickeySettings.endGroup();
}

void LtrGuiForm::clearHotkeys()
{
  QString noneStr = QString::fromUtf8("None");
  
  if(toggleHotKey){
    toggleHotKey->setHotKey(noneStr);
  }
  if(recenterHotKey){
    recenterHotKey->setHotKey(noneStr);
  }
  
  hotkeySettings->beginGroup(QString::fromUtf8("HotKeys"));
  hotkeySettings->setValue(QString::fromUtf8("tracking_toggle"), noneStr);
  hotkeySettings->setValue(QString::fromUtf8("quick_recenter"), noneStr);
  hotkeySettings->endGroup();
  
  // Sync to mickey.conf
  syncHotkeysToMickey();
}


