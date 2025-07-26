#ifndef WINE_LAUNCHER__H
#define WINE_LAUNCHER__H

#include <QObject>
#include <QProcess>

class WineLauncher:public QObject
{
 Q_OBJECT
 public:
  WineLauncher();
  ~WineLauncher();
  void setEnv(const QString &var, const QString &val);
  void run(const QString &tgt);
  void run(const QString &tgt, const QStringList &params);
  bool wineAvailable();
  QProcessEnvironment getProcessEnvironment() const { return env; }
  
  // NEW: Wine version selection methods
  QString selectBestWineVersion();
  bool isWineVersionCompatible(const QString &winePath);
  bool isWinetricksCompatible();
  QString getWineVersion(const QString &winePath);
  QString getWinetricksVersion();
  void setWinePath(const QString &path);
  
 private:
  bool check();
  void envSet(const QString var, const QString val);
  QProcess wine;
  QProcessEnvironment env;
  QString winePath;
  bool available;
  
  // NEW: Helper methods for version comparison
  bool parseWineVersion(const QString &versionString, int &major, int &minor);
  bool parseWinetricksVersion(const QString &versionString, int &year, int &month, int &day);
  bool isVersionGreaterOrEqual(int major1, int minor1, int major2, int minor2);
  bool isDateGreaterOrEqual(int year1, int month1, int day1, int year2, int month2, int day2);
  
 private slots:
  void finished(int exitCode, QProcess::ExitStatus exitStatus);
  void error(QProcess::ProcessError error);
 signals:
  void finished(bool result);
};

#endif
