#include "installer_paths.h"
#include "ltr_gui_prefs.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

namespace InstallerPaths {

QString resolveWineBridgePayloadDir()
{
  QStringList candidates;

  // AppImage / portable layout (prefer bundled payload over host installs)
  const QString appImageRoot = qEnvironmentVariable("APPDIR");
  if (!appImageRoot.isEmpty()) {
    candidates << appImageRoot + QStringLiteral("/usr/lib/linuxtrack/wine_bridge")
               << appImageRoot + QStringLiteral("/wine_bridge/payload");
  }
  const QString appBinDir = QCoreApplication::applicationDirPath();
  if (!appBinDir.isEmpty()) {
    candidates << appBinDir + QStringLiteral("/../lib/linuxtrack/wine_bridge")
               << appBinDir + QStringLiteral("/../../wine_bridge/payload");
  }

  candidates << PrefProxy::getDataPath(QString::fromUtf8("wine_bridge"))
             << QString::fromUtf8("/opt/lib/linuxtrack/wine_bridge")
             << QString::fromUtf8("/usr/local/lib/linuxtrack/wine_bridge")
             << QString::fromUtf8("/usr/lib/linuxtrack/wine_bridge");

  for (const QString &c : candidates) {
    if (c.isEmpty())
      continue;
    QDir d(c);
    if (d.exists() && QFileInfo(d.filePath(QString::fromUtf8("NPClient.dll"))).isFile())
      return d.absolutePath();
  }
  return QString();
}

} // namespace InstallerPaths
