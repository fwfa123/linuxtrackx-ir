#ifndef WINE_BRIDGE_INSTALL_H
#define WINE_BRIDGE_INSTALL_H

#include <QString>
#include <QStringList>
#include <QProcessEnvironment>

class QWidget;

namespace WineBridgeInstall {

enum class InstallOutcome {
    Installed,
    Cancelled,
    UninstalledOnly,
    Failed,
};

bool prefixIsWin64(const QString &prefixPath);

/** Host paths under prefix where an existing bridge install was detected. */
QStringList findExistingInstallDirs(const QString &prefixPath);

/** Remove bridge files and registry keys for the given install directories. */
bool removeFromPrefix(const QString &prefixPath, const QString &wineBinaryPath,
                      const QProcessEnvironment &extraEnv, const QStringList &installDirs,
                      QString *lastError);

/** Copy wine_bridge payload into prefix, write registry, link firmware. */
bool installToPrefix(const QString &prefixPath, const QString &wineBinaryPath, QString *lastError,
                     QString *debugInfo = nullptr, QWidget *promptParent = nullptr,
                     InstallOutcome *outcome = nullptr);

/** Same as installToPrefix with extra env vars for wine reg (e.g. Steam Proton). */
bool installToPrefix(const QString &prefixPath, const QString &wineBinaryPath,
                     const QProcessEnvironment &extraEnv, QString *lastError,
                     QString *debugInfo = nullptr, QWidget *promptParent = nullptr,
                     InstallOutcome *outcome = nullptr);

QString installDirectoryForPrefix(const QString &prefixPath);
QString registryPathForPrefix(const QString &prefixPath);

} // namespace WineBridgeInstall

#endif
