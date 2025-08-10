#include "installer_paths.h"
#include "ltr_gui_prefs.h"
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>

namespace InstallerPaths {

static QString tryPath(const QString &path)
{
  QFileInfo f(path);
  return (f.exists() && f.isFile()) ? f.absoluteFilePath() : QString();
}

QString resolveWineBridgeInstallerPath()
{
  // 1) Prefer packaged data path via PrefProxy (handles AppImage prefix too)
  //    We keep both with and without wine/ subdir for robustness
  QString p1 = PrefProxy::getDataPath(QString::fromUtf8("wine/linuxtrack-wine.exe"));
  QString found = tryPath(p1);
  if (!found.isEmpty()) return found;

  QString p2 = PrefProxy::getDataPath(QString::fromUtf8("linuxtrack-wine.exe"));
  found = tryPath(p2);
  if (!found.isEmpty()) return found;

  // 2) Canonical system locations
  QStringList systemCandidates;
  systemCandidates << QString::fromUtf8("/usr/local/share/linuxtrack/wine/linuxtrack-wine.exe")
                   << QString::fromUtf8("/usr/share/linuxtrack/wine/linuxtrack-wine.exe")
                   << QString::fromUtf8("/usr/local/share/linuxtrack/linuxtrack-wine.exe")
                   << QString::fromUtf8("/usr/share/linuxtrack/linuxtrack-wine.exe");
  for (const QString &c : systemCandidates) {
    found = tryPath(c);
    if (!found.isEmpty()) return found;
  }

  // 3) AppImage-embedded path (relative to application dir)
  const QString appDir = QCoreApplication::applicationDirPath();
  QString appImageCandidate = QDir(appDir).filePath(QString::fromUtf8("../share/linuxtrack/linuxtrack-wine.exe"));
  found = tryPath(appImageCandidate);
  if (!found.isEmpty()) return found;

  // Not found
  return QString();
}

} // namespace InstallerPaths


