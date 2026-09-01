#include "joy_hotkey_monitor.h"

#include "dyn_load.h"
#include "joy_driver_prefs.h"
#include "utils.h"

#include <QRegularExpression>
#include <cstring>
#ifdef __linux__
#  include <linux/input-event-codes.h>
#endif

typedef joystickNames_t *(*enum_joysticks_t)(ifc_type_t ifc);
typedef void (*free_joysticks_t)(joystickNames_t *nl);
typedef int (*open_by_name_t)(const char *name);
typedef void (*joy_close_t)(int fd);
typedef int (*read_buttons_t)(int fd, joy_button_event_t *out, int max_events);

static enum_joysticks_t enum_joysticks_fun = NULL;
static free_joysticks_t free_joysticks_fun = NULL;
static open_by_name_t open_by_name_fun = NULL;
static joy_close_t joy_close_fun = NULL;
static read_buttons_t read_buttons_fun = NULL;

static lib_fun_def_t joy_functions[] = {
  {(char *)"ltr_int_joy_enum_joysticks", (void *)&enum_joysticks_fun},
  {(char *)"ltr_int_joy_free_joysticks", (void *)&free_joysticks_fun},
  {(char *)"ltr_int_joy_open_by_name", (void *)&open_by_name_fun},
  {(char *)"ltr_int_joy_close", (void *)&joy_close_fun},
  {(char *)"ltr_int_joy_read_buttons", (void *)&read_buttons_fun},
  {NULL, NULL}
};

static void *g_libhandle = NULL;
static int g_lib_users = 0;

static bool loadJoyLib()
{
  if(g_libhandle != NULL){
    return true;
  }
  g_libhandle = ltr_int_load_library((char *)"libjoy", joy_functions);
  return g_libhandle != NULL;
}

static void retainJoyLib()
{
  if(loadJoyLib()){
    ++g_lib_users;
  }
}

static void releaseJoyLib()
{
  if(g_lib_users > 0){
    --g_lib_users;
  }
  if(g_lib_users == 0 && g_libhandle != NULL){
    ltr_int_unload_library(g_libhandle, joy_functions);
    g_libhandle = NULL;
    enum_joysticks_fun = NULL;
    free_joysticks_fun = NULL;
    open_by_name_fun = NULL;
    joy_close_fun = NULL;
    read_buttons_fun = NULL;
  }
}

bool JoyHotkey::isJoyBinding(const QString &binding)
{
  return binding.startsWith(QString::fromUtf8("Joy::"));
}

QString JoyHotkey::encode(const QString &deviceName, int buttonCode)
{
  return QString::fromUtf8("Joy::%1::%2").arg(deviceName).arg(buttonCode);
}

bool JoyHotkey::decode(const QString &binding, QString *deviceName, int *buttonCode)
{
  if(!isJoyBinding(binding)){
    return false;
  }
  const QString body = binding.mid(5); /* after "Joy::" */
  const int sep = body.lastIndexOf(QString::fromUtf8("::"));
  if(sep <= 0){
    return false;
  }
  bool ok = false;
  const int code = body.mid(sep + 2).toInt(&ok);
  if(!ok || code < 0){
    return false;
  }
  if(deviceName){
    *deviceName = body.left(sep);
  }
  if(buttonCode){
    *buttonCode = code;
  }
  return true;
}

QString JoyHotkey::shortDeviceName(const QString &fullName)
{
  QString name = fullName.trimmed();
  if(name.isEmpty()){
    return QString::fromUtf8("Controller");
  }

  /* Collapse duplicate leading brand tokens: "Saitek Saitek X45" -> "Saitek X45" */
  {
    const QStringList parts = name.split(QRegularExpression(QString::fromUtf8("\\s+")),
                                         Qt::SkipEmptyParts);
    QStringList collapsed;
    for(int i = 0; i < parts.size(); ++i){
      if(!collapsed.isEmpty() &&
         collapsed.last().compare(parts[i], Qt::CaseInsensitive) == 0){
        continue;
      }
      collapsed.append(parts[i]);
    }
    name = collapsed.join(QString::fromUtf8(" "));
  }

  struct PrefixMap {
    const char *prefix;
    const char *replacement; /* NULL = strip only */
  };
  static const PrefixMap prefixes[] = {
    {"Sony Interactive Entertainment ", NULL},
    {"Sony Computer Entertainment ", NULL},
    {"Sony ", NULL},
    {"Microsoft ", NULL},
    {"Logitech ", NULL},
    {"Mad Catz ", NULL},
    {"Thrustmaster ", NULL},
    {"HORI ", NULL},
  };
  for(size_t i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]); ++i){
    const QString pref = QString::fromUtf8(prefixes[i].prefix);
    if(name.startsWith(pref, Qt::CaseInsensitive)){
      name = name.mid(pref.size()).trimmed();
      if(prefixes[i].replacement != NULL){
        name = QString::fromUtf8(prefixes[i].replacement);
      }
      break;
    }
  }

  const QString lower = name.toLower();
  if(lower == QString::fromUtf8("wireless controller") ||
     lower == QString::fromUtf8("wireless controller touchpad")){
    name = QString::fromUtf8("PS4");
  }else if(lower.contains(QString::fromUtf8("dualsense"))){
    name = QString::fromUtf8("PS5");
  }else if(lower.contains(QString::fromUtf8("dualshock"))){
    name = QString::fromUtf8("PS4");
  }else if(lower.startsWith(QString::fromUtf8("xbox"))){
    /* "Xbox Wireless Controller" / "Xbox 360 Controller" -> keep short Xbox form */
    if(lower.contains(QString::fromUtf8("360"))){
      name = QString::fromUtf8("Xbox 360");
    }else if(lower.contains(QString::fromUtf8("one")) ||
             lower.contains(QString::fromUtf8("series"))){
      name = QString::fromUtf8("Xbox");
    }else{
      name = QString::fromUtf8("Xbox");
    }
  }

  /* Prefer trailing distinctive tokens if still too long */
  const int maxLen = 22;
  if(name.size() > maxLen){
    const QStringList parts = name.split(QRegularExpression(QString::fromUtf8("\\s+")),
                                         Qt::SkipEmptyParts);
    if(parts.size() >= 2){
      QString tail = parts[parts.size() - 2] + QString::fromUtf8(" ") + parts.last();
      if(tail.size() <= maxLen){
        name = tail;
      }else{
        name = parts.last();
      }
    }
    if(name.size() > maxLen){
      name = name.left(maxLen - 1) + QString::fromUtf8("…");
    }
  }
  return name;
}

QString JoyHotkey::friendlyButtonName(int buttonCode)
{
#ifdef __linux__
  switch(buttonCode){
    case BTN_0: return QString::fromUtf8("Btn0");
    case BTN_1: return QString::fromUtf8("Btn1");
    case BTN_2: return QString::fromUtf8("Btn2");
    case BTN_3: return QString::fromUtf8("Btn3");
    case BTN_4: return QString::fromUtf8("Btn4");
    case BTN_5: return QString::fromUtf8("Btn5");
    case BTN_6: return QString::fromUtf8("Btn6");
    case BTN_7: return QString::fromUtf8("Btn7");
    case BTN_8: return QString::fromUtf8("Btn8");
    case BTN_9: return QString::fromUtf8("Btn9");
    case BTN_LEFT: return QString::fromUtf8("Left");
    case BTN_RIGHT: return QString::fromUtf8("Right");
    case BTN_MIDDLE: return QString::fromUtf8("Middle");
    case BTN_SIDE: return QString::fromUtf8("Side");
    case BTN_EXTRA: return QString::fromUtf8("Extra");
    case BTN_FORWARD: return QString::fromUtf8("Fwd");
    case BTN_BACK: return QString::fromUtf8("Back");
    case BTN_TRIGGER: return QString::fromUtf8("Trigger");
    case BTN_THUMB: return QString::fromUtf8("Thumb");
    case BTN_THUMB2: return QString::fromUtf8("Thumb2");
    case BTN_TOP: return QString::fromUtf8("Top");
    case BTN_TOP2: return QString::fromUtf8("Top2");
    case BTN_PINKIE: return QString::fromUtf8("Pinkie");
    case BTN_BASE: return QString::fromUtf8("Base");
    case BTN_BASE2: return QString::fromUtf8("Base2");
    case BTN_BASE3: return QString::fromUtf8("Base3");
    case BTN_BASE4: return QString::fromUtf8("Base4");
    case BTN_BASE5: return QString::fromUtf8("Base5");
    case BTN_BASE6: return QString::fromUtf8("Base6");
    case BTN_DEAD: return QString::fromUtf8("Dead");
    case BTN_SOUTH: return QString::fromUtf8("A");
    case BTN_EAST: return QString::fromUtf8("B");
    case BTN_C: return QString::fromUtf8("C");
    case BTN_NORTH: return QString::fromUtf8("X");
    case BTN_WEST: return QString::fromUtf8("Y");
    case BTN_Z: return QString::fromUtf8("Z");
    case BTN_TL: return QString::fromUtf8("LB");
    case BTN_TR: return QString::fromUtf8("RB");
    case BTN_TL2: return QString::fromUtf8("LT");
    case BTN_TR2: return QString::fromUtf8("RT");
    case BTN_SELECT: return QString::fromUtf8("Select");
    case BTN_START: return QString::fromUtf8("Start");
    case BTN_MODE: return QString::fromUtf8("Mode");
    case BTN_THUMBL: return QString::fromUtf8("LStick");
    case BTN_THUMBR: return QString::fromUtf8("RStick");
    case BTN_DPAD_UP: return QString::fromUtf8("DPadU");
    case BTN_DPAD_DOWN: return QString::fromUtf8("DPadD");
    case BTN_DPAD_LEFT: return QString::fromUtf8("DPadL");
    case BTN_DPAD_RIGHT: return QString::fromUtf8("DPadR");
    default:
      return QString::fromUtf8("Btn %1").arg(buttonCode);
  }
#else
  return QString::fromUtf8("Btn %1").arg(buttonCode);
#endif
}

QString JoyHotkey::displayName(const QString &binding)
{
  QString device;
  int code = 0;
  if(!decode(binding, &device, &code)){
    return binding;
  }
  return QString::fromUtf8("%1 %2")
           .arg(shortDeviceName(device))
           .arg(friendlyButtonName(code));
}

QString JoyHotkey::displayTooltip(const QString &binding)
{
  QString device;
  int code = 0;
  if(!decode(binding, &device, &code)){
    return binding;
  }
  return QString::fromUtf8("%1\nButton %2 (0x%3)")
           .arg(device)
           .arg(code)
           .arg(code, 0, 16);
}

JoyButtonMonitor::JoyButtonMonitor(QObject *parent)
  : QObject(parent), libhandle(NULL), timer(new QTimer(this)), libOk(false)
{
  retainJoyLib();
  libOk = (g_libhandle != NULL);
  connect(timer, SIGNAL(timeout()), this, SLOT(poll()));
  timer->setInterval(20);
}

JoyButtonMonitor::~JoyButtonMonitor()
{
  timer->stop();
  closeAllDevices();
  releaseJoyLib();
}

bool JoyButtonMonitor::available() const
{
  return libOk;
}

bool JoyButtonMonitor::ensureLibrary()
{
  if(libOk){
    return true;
  }
  retainJoyLib();
  libOk = (g_libhandle != NULL);
  return libOk;
}

void JoyButtonMonitor::clearBindings()
{
  bindings.clear();
  closeUnusedDevices();
  if(bindings.isEmpty()){
    timer->stop();
  }
}

void JoyButtonMonitor::removeBinding(int hotkeyId)
{
  for(int i = bindings.size() - 1; i >= 0; --i){
    if(bindings[i].hotkeyId == hotkeyId){
      bindings.removeAt(i);
    }
  }
  closeUnusedDevices();
  if(bindings.isEmpty()){
    timer->stop();
  }
}

bool JoyButtonMonitor::deviceIsOpen(const QString &deviceName) const
{
  for(int i = 0; i < devices.size(); ++i){
    if(devices[i].name == deviceName && devices[i].fd >= 0){
      return true;
    }
  }
  return false;
}

bool JoyButtonMonitor::setBinding(int hotkeyId, const QString &binding)
{
  removeBinding(hotkeyId);
  if(!JoyHotkey::isJoyBinding(binding)){
    return true;
  }
  if(!ensureLibrary()){
    ltr_int_log_message("Joy hotkeys: libjoy not available.\n");
    return false;
  }

  QString device;
  int code = 0;
  if(!JoyHotkey::decode(binding, &device, &code)){
    return false;
  }

  /* Keep the binding even if the device is currently missing so prefs survive
   * unplug/replug and startup races. openDevice is retried in poll(). */
  Binding b;
  b.hotkeyId = hotkeyId;
  b.deviceName = device;
  b.buttonCode = code;
  bindings.append(b);
  if(!timer->isActive()){
    timer->start();
  }

  if(!openDevice(device)){
    ltr_int_log_message("Joy hotkeys: device '%s' not available yet; will retry.\n",
                        device.toUtf8().constData());
    return false;
  }
  return true;
}

bool JoyButtonMonitor::openDevice(const QString &deviceName)
{
  if(deviceIsOpen(deviceName)){
    return true;
  }
  if(open_by_name_fun == NULL){
    return false;
  }
  int fd = open_by_name_fun(deviceName.toUtf8().constData());
  if(fd < 0){
    return false;
  }
  DeviceState st;
  st.name = deviceName;
  st.fd = fd;
  devices.append(st);
  return true;
}

void JoyButtonMonitor::closeUnusedDevices()
{
  for(int i = devices.size() - 1; i >= 0; --i){
    bool used = false;
    for(int j = 0; j < bindings.size(); ++j){
      if(bindings[j].deviceName == devices[i].name){
        used = true;
        break;
      }
    }
    if(!used){
      if(joy_close_fun){
        joy_close_fun(devices[i].fd);
      }
      devices.removeAt(i);
    }
  }
}

void JoyButtonMonitor::closeAllDevices()
{
  for(int i = 0; i < devices.size(); ++i){
    if(joy_close_fun){
      joy_close_fun(devices[i].fd);
    }
  }
  devices.clear();
  bindings.clear();
}

void JoyButtonMonitor::poll()
{
  if(read_buttons_fun == NULL){
    return;
  }

  /* Retry opens for bindings whose devices are missing (hotplug / startup). */
  static int retry_div = 0;
  if((++retry_div % 50) == 0){ /* about once per second at 20ms tick */
    for(int b = 0; b < bindings.size(); ++b){
      if(!deviceIsOpen(bindings[b].deviceName)){
        openDevice(bindings[b].deviceName);
      }
    }
  }

  joy_button_event_t ev[16];
  for(int d = devices.size() - 1; d >= 0; --d){
    int n = read_buttons_fun(devices[d].fd, ev, 16);
    if(n < 0){
      /* Device went away — keep bindings; retry later */
      if(joy_close_fun){
        joy_close_fun(devices[d].fd);
      }
      devices.removeAt(d);
      continue;
    }
    for(int i = 0; i < n; ++i){
      for(int b = 0; b < bindings.size(); ++b){
        if(bindings[b].deviceName == devices[d].name &&
           bindings[b].buttonCode == (int)ev[i].code){
          emit activated(bindings[b].hotkeyId, ev[i].value != 0);
        }
      }
    }
  }
}
