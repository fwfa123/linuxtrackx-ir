#ifndef WINE_BRIDGE_INSTALL_H
#define WINE_BRIDGE_INSTALL_H

#include <QString>
#include <QProcessEnvironment>

namespace WineBridgeInstall {

bool prefixIsWin64(const QString &prefixPath);

/** Copy wine_bridge payload into prefix, write registry, link firmware. */
bool installToPrefix(const QString &prefixPath, const QString &wineBinaryPath, QString *lastError,
                     QString *debugInfo = nullptr);

/** Same as installToPrefix with extra env vars for wine reg (e.g. Steam Proton). */
bool installToPrefix(const QString &prefixPath, const QString &wineBinaryPath,
                     const QProcessEnvironment &extraEnv, QString *lastError,
                     QString *debugInfo = nullptr);

QString installDirectoryForPrefix(const QString &prefixPath);
QString registryPathForPrefix(const QString &prefixPath);

} // namespace WineBridgeInstall

#endif
