#ifndef JOY_HOTKEY_MONITOR__H
#define JOY_HOTKEY_MONITOR__H

#include <QObject>
#include <QString>
#include <QVector>
#include <QTimer>

class JoyHotkey
{
public:
  static bool isJoyBinding(const QString &binding);
  static QString encode(const QString &deviceName, int buttonCode);
  static bool decode(const QString &binding, QString *deviceName, int *buttonCode);
  static QString displayName(const QString &binding);
  static QString displayTooltip(const QString &binding);
  static QString shortDeviceName(const QString &fullName);
  static QString friendlyButtonName(int buttonCode);
};

class JoyButtonMonitor : public QObject
{
  Q_OBJECT
public:
  explicit JoyButtonMonitor(QObject *parent = nullptr);
  ~JoyButtonMonitor() override;

  bool available() const;
  void clearBindings();
  bool setBinding(int hotkeyId, const QString &binding);
  void removeBinding(int hotkeyId);

signals:
  void activated(int hotkeyId, bool pressed);

private slots:
  void poll();

private:
  struct DeviceState {
    QString name;
    int fd;
  };

  struct Binding {
    int hotkeyId;
    QString deviceName;
    int buttonCode;
  };

  bool ensureLibrary();
  bool openDevice(const QString &deviceName);
  bool deviceIsOpen(const QString &deviceName) const;
  void closeUnusedDevices();
  void closeAllDevices();

  void *libhandle;
  QTimer *timer;
  QVector<DeviceState> devices;
  QVector<Binding> bindings;
  bool libOk;
};

#endif
