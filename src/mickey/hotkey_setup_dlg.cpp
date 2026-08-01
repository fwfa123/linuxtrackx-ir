#include "hotkey_setup_dlg.h"

#include <QTimer>
#include <unistd.h>

#ifdef LTR_GUI_JOY_HOTKEYS
#include "joy_hotkey_monitor.h"
#include "dyn_load.h"
#include "joy_driver_prefs.h"
#endif

hotKeySetupDlg::hotKeySetupDlg(QString &res, QWidget *parent)
  : QDialog(parent), result(&res), joyTimer(nullptr), joyLib(nullptr),
    readButtonsFun(nullptr), closeFun(nullptr)
{
  ui.setupUi(this);
  ui.lineEdit->setTargetString(res);
#ifdef LTR_GUI_JOY_HOTKEYS
  ui.label->setText(QString::fromUtf8("Press a key or controller button:"));
  startJoyCapture();
#endif
}

hotKeySetupDlg::~hotKeySetupDlg()
{
  stopJoyCapture();
}

void hotKeySetupDlg::startJoyCapture()
{
#ifdef LTR_GUI_JOY_HOTKEYS
  typedef joystickNames_t *(*enum_joysticks_t)(ifc_type_t ifc);
  typedef void (*free_joysticks_t)(joystickNames_t *nl);
  typedef int (*open_by_name_t)(const char *name);
  typedef void (*joy_close_t)(int fd);
  typedef int (*read_buttons_t)(int fd, joy_button_event_t *out, int max_events);

  enum_joysticks_t enum_joysticks_fun = NULL;
  free_joysticks_t free_joysticks_fun = NULL;
  open_by_name_t open_by_name_fun = NULL;
  joy_close_t joy_close_fun = NULL;
  read_buttons_t read_buttons_fun = NULL;

  lib_fun_def_t functions[] = {
    {(char *)"ltr_int_joy_enum_joysticks", (void *)&enum_joysticks_fun},
    {(char *)"ltr_int_joy_free_joysticks", (void *)&free_joysticks_fun},
    {(char *)"ltr_int_joy_open_by_name", (void *)&open_by_name_fun},
    {(char *)"ltr_int_joy_close", (void *)&joy_close_fun},
    {(char *)"ltr_int_joy_read_buttons", (void *)&read_buttons_fun},
    {NULL, NULL}
  };

  joyLib = ltr_int_load_library((char *)"libjoy", functions);
  if(joyLib == NULL || enum_joysticks_fun == NULL || open_by_name_fun == NULL ||
     read_buttons_fun == NULL || joy_close_fun == NULL || free_joysticks_fun == NULL){
    joyLib = nullptr;
    return;
  }

  readButtonsFun = (void *)read_buttons_fun;
  closeFun = (void *)joy_close_fun;

  joystickNames_t *names = enum_joysticks_fun(e_EVDEV);
  if(names == NULL){
    return;
  }
  for(size_t i = 0; i < names->namesFound; ++i){
    int fd = open_by_name_fun(names->nameList[i]);
    if(fd >= 0){
      joyFds.append(fd);
      joyNames.append(QString::fromUtf8(names->nameList[i]));
    }
  }
  free_joysticks_fun(names);

  if(joyFds.isEmpty()){
    return;
  }

  joy_button_event_t trash[32];
  for(int i = 0; i < joyFds.size(); ++i){
    while(read_buttons_fun(joyFds[i], trash, 32) > 0){
    }
  }

  joyTimer = new QTimer(this);
  connect(joyTimer, SIGNAL(timeout()), this, SLOT(pollJoysticks()));
  joyTimer->start(20);
#endif
}

void hotKeySetupDlg::stopJoyCapture()
{
  if(joyTimer){
    joyTimer->stop();
    joyTimer = nullptr;
  }
#ifdef LTR_GUI_JOY_HOTKEYS
  typedef void (*joy_close_t)(int fd);
  joy_close_t joy_close_fun = (joy_close_t)closeFun;
  for(int i = 0; i < joyFds.size(); ++i){
    if(joyFds[i] >= 0){
      if(joy_close_fun){
        joy_close_fun(joyFds[i]);
      }else{
        ::close(joyFds[i]);
      }
    }
  }
  joyFds.clear();
  joyNames.clear();
  if(joyLib != NULL){
    /* Pointers in functions[] were locals; unload without clearing them. */
    lib_fun_def_t empty[] = {{NULL, NULL}};
    ltr_int_unload_library(joyLib, empty);
    joyLib = nullptr;
  }
  readButtonsFun = nullptr;
  closeFun = nullptr;
#else
  joyFds.clear();
  joyNames.clear();
#endif
}

void hotKeySetupDlg::pollJoysticks()
{
#ifdef LTR_GUI_JOY_HOTKEYS
  typedef int (*read_buttons_t)(int fd, joy_button_event_t *out, int max_events);
  read_buttons_t read_buttons_fun = (read_buttons_t)readButtonsFun;
  if(read_buttons_fun == NULL || result == NULL){
    return;
  }
  joy_button_event_t ev[16];
  for(int i = 0; i < joyFds.size(); ++i){
    int n = read_buttons_fun(joyFds[i], ev, 16);
    if(n < 0){
      continue;
    }
    for(int e = 0; e < n; ++e){
      if(ev[e].value == 1){
        *result = JoyHotkey::encode(joyNames[i], (int)ev[e].code);
        ui.lineEdit->setText(JoyHotkey::displayName(*result));
        ui.lineEdit->setToolTip(JoyHotkey::displayTooltip(*result));
        accept();
        return;
      }
    }
  }
#else
  (void)0;
#endif
}

#include "moc_hotkey_setup_dlg.cpp"
