#ifdef HAVE_CONFIG_H
  #include "../../config.h"
#endif

#include <iostream>
#include <sstream>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QDebug>
#include "wine_launcher.h"
#include "utils.h"

void WineLauncher::envSet(const QString var, const QString val)
{
    env.insert(var, val);
    std::ostringstream s;
    s<<"    "<<var.toUtf8().constData()<<"='"<<val.toUtf8().constData()<<"'\n";
    ltr_int_log_message(s.str().c_str());
}

WineLauncher::WineLauncher():winePath(QString::fromUtf8("")), available(false)
{
  std::ostringstream s;
  env = QProcessEnvironment::systemEnvironment();
  
  // NEW: Select best wine version before checking
  QString bestWine = selectBestWineVersion();
  if (!bestWine.isEmpty()) {
    winePath = bestWine;
    // Don't add trailing slash - winePath should be the full path to the wine executable
    s.str(std::string(""));
    s<<"Selected wine version: "<<winePath.toUtf8().constData()<<"\n";
    ltr_int_log_message(s.str().c_str());
  }
  
  if(!check()){
    envSet(QString::fromUtf8("WINEARCH"), QString::fromUtf8("win64"));
#ifdef DARWIN
    winePath = QApplication::applicationDirPath()+QString::fromUtf8("/../wine/bin/");
    QString libPath = QApplication::applicationDirPath()+QString::fromUtf8("/../wine/lib/");
    available = true;
    QString path = winePath + QString::fromUtf8(":") + env.value(QString::fromUtf8("PATH"));
    s.str(std::string(""));
    s<<"Using internal wine; adjusting env variables:\n";
    ltr_int_log_message(s.str().c_str());
    envSet(QString::fromUtf8("PATH"), path);
    envSet(QString::fromUtf8("WINESERVER"), winePath+QString::fromUtf8("wineserver"));
    envSet(QString::fromUtf8("WINELOADER"), winePath+QString::fromUtf8("wine"));
    envSet(QString::fromUtf8("WINEDLLPATH"), libPath+QString::fromUtf8("wine/fakedlls"));
    envSet(QString::fromUtf8("DYLD_LIBRARY_PATH"), libPath);
    envSet(QString::fromUtf8("DYLD_PRINT_ENV"), QString::fromUtf8("1"));
    envSet(QString::fromUtf8("DYLD_PRINT_LIBRARIES"), QString::fromUtf8("1"));
    envSet(QString::fromUtf8("WINEDEBUG"), QString::fromUtf8("+file,+seh,+tid,+process,+rundll,+module"));
#endif
  }else{
    available = true;
  }
  // Add Wine debug output to see what's happening (more reasonable level)
  envSet(QString::fromUtf8("WINEDEBUG"), QString::fromUtf8("+err+fixme"));
  QObject::connect(&wine, SIGNAL(finished(int, QProcess::ExitStatus)),
    this, SLOT(finished(int, QProcess::ExitStatus)));
  QObject::connect(&wine, SIGNAL(error(QProcess::ProcessError)),
    this, SLOT(error(QProcess::ProcessError)));
}

WineLauncher::~WineLauncher()
{
  QObject::disconnect(&wine, SIGNAL(finished(int, QProcess::ExitStatus)),
    this, SLOT(finished(int, QProcess::ExitStatus)));
  QObject::disconnect(&wine, SIGNAL(error(QProcess::ProcessError)),
    this, SLOT(error(QProcess::ProcessError)));
  if(wine.state() != QProcess::NotRunning){
    wine.waitForFinished(10000);
  }
  if(wine.state() != QProcess::NotRunning){
    wine.kill();
  }
}

void WineLauncher::setEnv(const QString &var, const QString &val)
{
  env.insert(var, val);
}

// NEW: Set wine path after construction
void WineLauncher::setWinePath(const QString &path)
{
  winePath = path;
  // Don't add trailing slash - winePath should be the full path to the wine executable
  std::ostringstream s;
  s<<"Wine path set to: "<<winePath.toUtf8().constData()<<"\n";
  ltr_int_log_message(s.str().c_str());
}

// NEW: Select best wine version based on compatibility requirements
QString WineLauncher::selectBestWineVersion()
{
  std::ostringstream s;
  s<<"Searching for compatible wine versions (requires 9.0+ for TrackIR 5.5.2 installer)...\n";
  ltr_int_log_message(s.str().c_str());
  
  QStringList winePaths;
  QStringList wineCommands;
  
  // Check common wine installation paths
  winePaths << QStringLiteral("/opt/wine-staging/bin/wine")
            << QStringLiteral("/usr/local/bin/wine-staging")
            << QStringLiteral("/opt/wine/bin/wine")
            << QStringLiteral("/usr/local/bin/wine");
  
  // Check common wine commands
  wineCommands << QStringLiteral("wine-staging")
               << QStringLiteral("wine-development")
               << QStringLiteral("wine");
  
  QString bestWine;
  int bestMajor = 0;
  int bestMinor = 0;
  
  // Check wine paths first (these are usually newer versions)
  for (const QString &path : winePaths) {
    if (QFile::exists(path)) {
      QString version = getWineVersion(path);
      if (!version.isEmpty()) {
        int major, minor;
        if (parseWineVersion(version, major, minor)) {
          s.str(std::string(""));
          s<<"Found wine at "<<path.toUtf8().constData()<<" version "<<version.toUtf8().constData()<<"\n";
          ltr_int_log_message(s.str().c_str());
          
          // Check if version is compatible (9.0+ for TrackIR 5.5.2 installer)
          if (isVersionGreaterOrEqual(major, minor, 9, 0)) {
            // Prefer wine-staging over regular wine
            if (path.contains(QStringLiteral("wine-staging")) || path.contains(QStringLiteral("staging"))) {
              if (isVersionGreaterOrEqual(major, minor, bestMajor, bestMinor)) {
                bestWine = path;
                bestMajor = major;
                bestMinor = minor;
              }
            } else if (bestWine.isEmpty() || !bestWine.contains(QStringLiteral("wine-staging"))) {
              // Only use regular wine if no wine-staging is found
              if (isVersionGreaterOrEqual(major, minor, bestMajor, bestMinor)) {
                bestWine = path;
                bestMajor = major;
                bestMinor = minor;
              }
            }
          }
        }
      }
    }
  }
  
  // Check wine commands if no compatible path found
  if (bestWine.isEmpty()) {
    for (const QString &cmd : wineCommands) {
      QString version = getWineVersion(cmd);
      if (!version.isEmpty()) {
        int major, minor;
        if (parseWineVersion(version, major, minor)) {
          s.str(std::string(""));
          s<<"Found wine command "<<cmd.toUtf8().constData()<<" version "<<version.toUtf8().constData()<<"\n";
          ltr_int_log_message(s.str().c_str());
          
          // Check if version is compatible (9.0+ for TrackIR 5.5.2 installer)
          if (isVersionGreaterOrEqual(major, minor, 9, 0)) {
            // Prefer wine-staging over regular wine
            if (cmd.contains(QStringLiteral("wine-staging")) || cmd.contains(QStringLiteral("staging"))) {
              if (isVersionGreaterOrEqual(major, minor, bestMajor, bestMinor)) {
                bestWine = cmd;
                bestMajor = major;
                bestMinor = minor;
              }
            } else if (bestWine.isEmpty() || !bestWine.contains(QStringLiteral("wine-staging"))) {
              // Only use regular wine if no wine-staging is found
              if (isVersionGreaterOrEqual(major, minor, bestMajor, bestMinor)) {
                bestWine = cmd;
                bestMajor = major;
                bestMinor = minor;
              }
            }
          }
        }
      }
    }
  }
  
  if (!bestWine.isEmpty()) {
    s.str(std::string(""));
    s<<"Selected best wine version: "<<bestWine.toUtf8().constData()<<" (v"<<bestMajor<<"."<<bestMinor<<")\n";
    ltr_int_log_message(s.str().c_str());
  } else {
    s.str(std::string(""));
    s<<"No compatible wine version found (requires 9.0+ for TrackIR 5.5.2 installer)\n";
    ltr_int_log_message(s.str().c_str());
  }
  
  return bestWine;
}

// NEW: Get wine version string
QString WineLauncher::getWineVersion(const QString &winePath)
{
  QProcess wineVersion;
  wineVersion.start(winePath, QStringList() << QStringLiteral("--version"));
  
  if (wineVersion.waitForFinished(5000)) {
    QString output = QString::fromUtf8(wineVersion.readAllStandardOutput());
    QString error = QString::fromUtf8(wineVersion.readAllStandardError());
    
    if (wineVersion.exitCode() == 0) {
      // Extract version from output (e.g., "wine-9.0.1" or "wine-9.0")
      QRegularExpression versionRegex(QStringLiteral(R"(wine-(\d+\.\d+(?:\.\d+)?))"));
      QRegularExpressionMatch match = versionRegex.match(output);
      if (match.hasMatch()) {
        return match.captured(1);
      }
    }
  }
  
  return QString();
}

// NEW: Get winetricks version
QString WineLauncher::getWinetricksVersion()
{
  QStringList winetricksPaths = {QStringLiteral("winetricks"), QStringLiteral("/usr/local/bin/winetricks"), QStringLiteral("/opt/winetricks/bin/winetricks")};
  
  for (const QString &path : winetricksPaths) {
    QProcess winetricksVersion;
    winetricksVersion.start(path, QStringList() << QStringLiteral("--version"));
    
    if (winetricksVersion.waitForFinished(5000)) {
      QString output = QString::fromUtf8(winetricksVersion.readAllStandardOutput());
      
      if (winetricksVersion.exitCode() == 0) {
        // Extract version from output (e.g., "20240105-next" or "20240105")
        QRegularExpression versionRegex(QStringLiteral(R"((\d{8}(?:-next)?))"));
        QRegularExpressionMatch match = versionRegex.match(output);
        if (match.hasMatch()) {
          return match.captured(1);
        }
      }
    }
  }
  
  return QString();
}

// NEW: Check if wine version is compatible (9.0+)
bool WineLauncher::isWineVersionCompatible(const QString &winePath)
{
  QString version = getWineVersion(winePath);
  if (version.isEmpty()) {
    return false;
  }
  
  int major, minor;
  if (parseWineVersion(version, major, minor)) {
    return isVersionGreaterOrEqual(major, minor, 9, 0);
  }
  
  return false;
}

// NEW: Check if winetricks is compatible (20240105-next+)
bool WineLauncher::isWinetricksCompatible()
{
  QString version = getWinetricksVersion();
  if (version.isEmpty()) {
    return false;
  }
  
  int year, month, day;
  if (parseWinetricksVersion(version, year, month, day)) {
    // Check if version is 20240105 or newer
    return isDateGreaterOrEqual(year, month, day, 2024, 1, 5);
  }
  
  return false;
}

// NEW: Parse wine version string (e.g., "9.0.1" -> major=9, minor=0)
bool WineLauncher::parseWineVersion(const QString &versionString, int &major, int &minor)
{
  QRegularExpression versionRegex(QStringLiteral(R"((\d+)\.(\d+)(?:\.\d+)?)"));
  QRegularExpressionMatch match = versionRegex.match(versionString);
  
  if (match.hasMatch()) {
    major = match.captured(1).toInt();
    minor = match.captured(2).toInt();
    return true;
  }
  
  return false;
}

// NEW: Parse winetricks version string (e.g., "20240105-next" -> year=2024, month=1, day=5)
bool WineLauncher::parseWinetricksVersion(const QString &versionString, int &year, int &month, int &day)
{
  QRegularExpression dateRegex(QStringLiteral(R"((\d{4})(\d{2})(\d{2})(?:-next)?)"));
  QRegularExpressionMatch match = dateRegex.match(versionString);
  
  if (match.hasMatch()) {
    year = match.captured(1).toInt();
    month = match.captured(2).toInt();
    day = match.captured(3).toInt();
    return true;
  }
  
  return false;
}

// NEW: Compare version numbers
bool WineLauncher::isVersionGreaterOrEqual(int major1, int minor1, int major2, int minor2)
{
  if (major1 > major2) return true;
  if (major1 < major2) return false;
  return minor1 >= minor2;
}

// NEW: Compare dates
bool WineLauncher::isDateGreaterOrEqual(int year1, int month1, int day1, int year2, int month2, int day2)
{
  if (year1 > year2) return true;
  if (year1 < year2) return false;
  if (month1 > month2) return true;
  if (month1 < month2) return false;
  return day1 >= day2;
}

void WineLauncher::run(const QString &tgt)
{
  envSet(QString::fromUtf8("WINEARCH"), QString::fromUtf8("win64"));
  wine.setProcessEnvironment(env);
  QString cmd(winePath);
  QStringList args(tgt);
  std::ostringstream s;
  s<<"Launching wine command: '" << cmd.toUtf8().constData() << " " << tgt.toUtf8().constData() << "'\n";
  ltr_int_log_message(s.str().c_str());
  wine.setProcessChannelMode(QProcess::MergedChannels);
  wine.start(cmd, args);
}

void WineLauncher::run(const QString &tgt, const QStringList &params)
{
  envSet(QString::fromUtf8("WINEARCH"), QString::fromUtf8("win64"));
  wine.setProcessEnvironment(env);
  QString cmd(winePath);
  QStringList args;
  args << tgt << params;
  std::ostringstream s;
  s<<"Launching wine command: '"<< cmd.toUtf8().constData()
   << " " << tgt.toUtf8().constData() << " " << params.join(QStringLiteral(" ")).toUtf8().constData() << "'\n";
  ltr_int_log_message(s.str().c_str());
  wine.setProcessChannelMode(QProcess::MergedChannels);
  wine.start(cmd, args);
}

void WineLauncher::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
  QString status;
  switch(exitStatus){
    case QProcess::NormalExit:
      status = QString::fromUtf8("Normal exit");
      break;
    case QProcess::CrashExit:
      status = QString::fromUtf8("Crashed");
      break;
    default:
      status = QString::fromUtf8("Unknown exit status");
      break;
  }
  ltr_int_log_message("Wine finished with exitcode %d (%s).", exitCode, status.toUtf8().constData());
  QString msg(QString::fromUtf8(wine.readAllStandardOutput().constData()));
  std::ostringstream s;
  s<<msg.toUtf8().constData()<<"\n";
  ltr_int_log_message(s.str().c_str());
  if(exitCode == 0 ){
    emit finished(true);
  }else{
    emit finished(false);
  }
}

const QString errorStr(QProcess::ProcessError error)
{
  QString reason;
  switch(error){
    case QProcess::FailedToStart:
      reason = QString::fromUtf8("Failed To Start");
      break;
    case QProcess::Crashed:
      reason = QString::fromUtf8("Crashed");
      break;
    case QProcess::Timedout:
      reason = QString::fromUtf8("Timedout");
      break;
    case QProcess::WriteError:
      reason = QString::fromUtf8("Write Error");
      break;
    case QProcess::ReadError:
      reason = QString::fromUtf8("Read Error");
      break;
    default:
      reason = QString::fromUtf8("Unknown Error");
      break;
  }
  return reason;
}

void WineLauncher::error(QProcess::ProcessError error)
{
  QString msg(QString::fromUtf8(wine.readAllStandardOutput().constData()));
  QString reason = errorStr(error);
  ltr_int_log_message("Error launching wine(%s)!", reason.toUtf8().constData());
  std::ostringstream s;
  s<<msg.toUtf8().constData()<<"\n";
  ltr_int_log_message(s.str().c_str());
  emit finished(false);
}

bool WineLauncher::wineAvailable()
{
  return available;
}


bool WineLauncher::check()
{
  envSet(QString::fromUtf8("WINEARCH"), QString::fromUtf8("win64"));
  wine.setProcessEnvironment(env);
  QString cmd(winePath);
  QStringList args(QString::fromUtf8("--version"));
  std::ostringstream s;
  s<<"Launching wine command: '" << cmd.toUtf8().constData() << " --version'\n";
  ltr_int_log_message(s.str().c_str());
  wine.setProcessChannelMode(QProcess::MergedChannels);
  wine.start(cmd, args);
  
  while(!wine.waitForFinished()){
    if(wine.error() != QProcess::Timedout){
      std::ostringstream s;
      s<<"Process error: "<<errorStr(wine.error()).toUtf8().constData()<<"\n";
      ltr_int_log_message(s.str().c_str());
      return false;
    }
  }
  if(wine.exitCode() == 0){
    return true;
  }
  return false;
}


/*
void PluginInstall::installWinePlugin()
{
  gui.pushButton_2->setEnabled(false);
  QString prefix = QFileDialog::getExistingDirectory(NULL, QString("Select Wine Prefix..."),
                     QDir::homePath()+"/.wine", QFileDialog::ShowDirsOnly);
  QString installerPath = PREF.getDataPath("linuxtrack-wine.exe");

  QString program = "/bin/bash";
  QString arg = QString("WINEPREFIX=") + prefix + " wine " + installerPath;
  QStringList args;
  args << "-c" << arg;
  inst->start(program, args);
}

void Extractor::extractFirmware(QString file)
{
  QString cmd("bash -c \"wine ");
  cmd += QString("%1\"").arg(file);
  progress(QString("Initializing wine and running installer %1").arg(file));
  wine->start(cmd);
}
*/

