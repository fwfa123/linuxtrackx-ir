#include "installer_paths.h"
#include "ltr_gui_prefs.h"
#include <QFileInfo>
#include <QDir>

namespace InstallerPaths {

QString resolveWineBridgePayloadDir()
{
  QStringList candidates;
  candidates << QString::fromUtf8("/opt/lib/linuxtrack/wine_bridge")
             << QString::fromUtf8("/usr/local/lib/linuxtrack/wine_bridge")
             << QString::fromUtf8("/usr/lib/linuxtrack/wine_bridge")
             << PrefProxy::getDataPath(QString::fromUtf8("wine_bridge"));
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
