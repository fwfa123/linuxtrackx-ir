#include "flatpak_detector.h"
#include <QProcess>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include "../utils.h"

#define VERSION_ARG QStringLiteral("--version")
#define LIST_ARG QStringLiteral("list")
#define APP_ARG QStringLiteral("--app")
#define COLUMNS_ARG QStringLiteral("--columns=application")
#define INFO_ARG QStringLiteral("info")
#define VAR_APP_PATH QStringLiteral("/.var/app/")
#define DATA_PATH QStringLiteral("/data")

FlatpakDetector::FlatpakDetector(QObject *parent)
    : QObject(parent)
{
}

FlatpakDetector::~FlatpakDetector()
{
}

QProcessEnvironment FlatpakDetector::hostEnvironmentForSystemBinary()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    // AppImage sets LD_LIBRARY_PATH for bundled libs; host tools like flatpak must not inherit it.
    env.remove(QStringLiteral("LD_LIBRARY_PATH"));
    env.remove(QStringLiteral("LD_PRELOAD"));
    env.remove(QStringLiteral("LD_AUDIT"));
    env.remove(QStringLiteral("QT_PLUGIN_PATH"));
    env.remove(QStringLiteral("QT_QPA_PLATFORM_PLUGIN_PATH"));
    return env;
}

QString FlatpakDetector::flatpakExecutable()
{
    return QStandardPaths::findExecutable(QStringLiteral("flatpak"));
}

bool FlatpakDetector::flatpakInstallTreeExists()
{
    const QString home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return QDir(home + QStringLiteral("/.local/share/flatpak")).exists()
        || QDir(QStringLiteral("/var/lib/flatpak")).exists();
}

bool FlatpakDetector::isAppInstalledByFilesystem(const QString &appId) const
{
    const QString home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    const QStringList candidates = {
        home + QStringLiteral("/.local/share/flatpak/app/") + appId,
        QStringLiteral("/var/lib/flatpak/app/") + appId,
    };

    for (const QString &path : candidates) {
        if (QDir(path).exists()) {
            ltr_int_log_message(
                "FlatpakDetector::isAppInstalledByFilesystem() - Found %s at %s\n",
                appId.toUtf8().constData(), path.toUtf8().constData());
            return true;
        }
    }
    return false;
}

bool FlatpakDetector::startFlatpakProcess(QProcess &process, const QStringList &args, int timeoutMs)
{
    const QString flatpakBin = flatpakExecutable();
    if (flatpakBin.isEmpty()) {
        setLastError(QStringLiteral("flatpak executable not found in PATH"));
        return false;
    }

    process.setProcessEnvironment(hostEnvironmentForSystemBinary());
    process.start(flatpakBin, args);
    const bool finished = process.waitForFinished(timeoutMs);
    if (!finished) {
        process.kill();
        return false;
    }
    return true;
}

bool FlatpakDetector::isFlatpakInstalled()
{
    if (flatpakChecked) {
        return flatpakInstalled;
    }

    flatpakChecked = true;
    flatpakCliWorks = false;

    const QString flatpakBin = flatpakExecutable();
    if (flatpakBin.isEmpty()) {
        setLastError(QStringLiteral("flatpak executable not found in PATH"));
        ltr_int_log_message("FlatpakDetector::isFlatpakInstalled() - flatpak not in PATH\n");
        flatpakInstalled = false;
        return false;
    }

    QProcess process;
    if (!startFlatpakProcess(process, QStringList() << VERSION_ARG, FLATPAK_VERSION_TIMEOUT_MS)) {
        setLastError(QStringLiteral("Timeout checking flatpak version"));
        ltr_int_log_message("FlatpakDetector::isFlatpakInstalled() - Timeout while checking flatpak version\n");
    } else if (process.exitCode() == 0) {
        const QString output = QString::fromUtf8(process.readAllStandardOutput()).trimmed();
        ltr_int_log_message("FlatpakDetector::isFlatpakInstalled() - Flatpak version: %s\n",
                            output.toUtf8().constData());
        flatpakCliWorks = true;
        flatpakInstalled = true;
        return true;
    } else {
        const QString errorOutput = QString::fromUtf8(process.readAllStandardError()).trimmed();
        ltr_int_log_message("FlatpakDetector::isFlatpakInstalled() - Flatpak CLI failed: %s\n",
                            errorOutput.toUtf8().constData());
    }

    // CLI failed (e.g. AppImage LD_LIBRARY_PATH breaks host flatpak) — trust install tree if present.
    if (flatpakInstallTreeExists()) {
        ltr_int_log_message(
            "FlatpakDetector::isFlatpakInstalled() - Flatpak CLI unavailable; using filesystem install tree\n");
        flatpakInstalled = true;
        return true;
    }

    setLastError(QStringLiteral("Flatpak is not installed or flatpak command failed"));
    flatpakInstalled = false;
    return false;
}

bool FlatpakDetector::isAppInstalled(const QString &appId)
{
    if (isAppInstalledByFilesystem(appId)) {
        return true;
    }

    if (!isFlatpakInstalled()) {
        setLastError(QStringLiteral("Flatpak is not installed"));
        return false;
    }

    if (!flatpakCliWorks) {
        ltr_int_log_message(
            "FlatpakDetector::isAppInstalled() - Flatpak CLI unavailable, app %s not found on filesystem\n",
            appId.toUtf8().constData());
        return false;
    }

    QStringList lines;
    if (isAppListCached) {
        lines = cachedApps;
    } else {
        QProcess process;
        if (!startFlatpakProcess(process,
                                 QStringList() << LIST_ARG << APP_ARG << COLUMNS_ARG,
                                 FLATPAK_COMMAND_TIMEOUT_MS)) {
            setLastError(QStringLiteral("Timeout listing Flatpak apps"));
            return false;
        }

        if (process.exitCode() != 0) {
            const QString errorOutput = QString::fromUtf8(process.readAllStandardError()).trimmed();
            setLastError(QStringLiteral("Failed to list Flatpak apps: %1").arg(errorOutput));
            return false;
        }

        const QString output = QString::fromUtf8(process.readAllStandardOutput());
        lines = output.split(QLatin1Char('\n'));
        lines.removeAll(QString());
        cachedApps = lines;
        isAppListCached = true;
    }

    ltr_int_log_message("FlatpakDetector::isAppInstalled() - Checking for app: %s\n",
                        appId.toUtf8().constData());
    ltr_int_log_message("FlatpakDetector::isAppInstalled() - Found %d installed apps\n", lines.size());

    for (const QString &line : lines) {
        if (line.trimmed() == appId) {
            ltr_int_log_message("FlatpakDetector::isAppInstalled() - Found app: %s\n",
                                appId.toUtf8().constData());
            return true;
        }
    }

    ltr_int_log_message("FlatpakDetector::isAppInstalled() - App not found: %s\n",
                        appId.toUtf8().constData());
    return false;
}

QStringList FlatpakDetector::listInstalledApps()
{
    QStringList apps;

    if (!isFlatpakInstalled()) {
        setLastError(QStringLiteral("Flatpak is not installed"));
        return apps;
    }

    if (!flatpakCliWorks) {
        setLastError(QStringLiteral("Cannot list Flatpak apps: flatpak command failed"));
        return apps;
    }

    if (isAppListCached) {
        return cachedApps;
    }

    QProcess process;
    if (!startFlatpakProcess(process,
                             QStringList() << LIST_ARG << APP_ARG << COLUMNS_ARG,
                             FLATPAK_COMMAND_TIMEOUT_MS)) {
        setLastError(QStringLiteral("Timeout listing Flatpak apps"));
        return apps;
    }

    if (process.exitCode() != 0) {
        const QString errorOutput = QString::fromUtf8(process.readAllStandardError()).trimmed();
        setLastError(QStringLiteral("Failed to list Flatpak apps: %1").arg(errorOutput));
        return apps;
    }

    const QString output = QString::fromUtf8(process.readAllStandardOutput());
    apps = output.split(QLatin1Char('\n'));
    apps.removeAll(QString());

    for (int i = 0; i < apps.size(); ++i) {
        apps[i] = apps[i].trimmed();
    }
    apps.removeAll(QString());

    cachedApps = apps;
    isAppListCached = true;

    ltr_int_log_message("FlatpakDetector::listInstalledApps() - Found %d apps\n", apps.size());
    return apps;
}

QString FlatpakDetector::getAppInfo(const QString &appId)
{
    if (!isFlatpakInstalled()) {
        setLastError(QStringLiteral("Flatpak is not installed"));
        return QString();
    }

    if (!flatpakCliWorks) {
        setLastError(QStringLiteral("Cannot get Flatpak app info: flatpak command failed"));
        return QString();
    }

    QProcess process;
    if (!startFlatpakProcess(process, QStringList() << INFO_ARG << appId, FLATPAK_COMMAND_TIMEOUT_MS)) {
        setLastError(QStringLiteral("Timeout getting Flatpak app info for %1").arg(appId));
        return QString();
    }

    if (process.exitCode() != 0) {
        const QString errorOutput = QString::fromUtf8(process.readAllStandardError()).trimmed();
        setLastError(QStringLiteral("Failed to get app info for %1: %2").arg(appId, errorOutput));
        return QString();
    }

    const QString output = QString::fromUtf8(process.readAllStandardOutput());
    ltr_int_log_message("FlatpakDetector::getAppInfo() - Info for %s:\n%s\n",
                        appId.toUtf8().constData(), output.toUtf8().constData());

    return output;
}

QString FlatpakDetector::getAppDataPath(const QString &appId)
{
    const QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    const QString dataPath = homeDir + VAR_APP_PATH + appId + DATA_PATH;

    ltr_int_log_message("FlatpakDetector::getAppDataPath() - Data path for %s: %s\n",
                        appId.toUtf8().constData(), dataPath.toUtf8().constData());

    return dataPath;
}

QString FlatpakDetector::getLastError() const
{
    return lastError;
}

void FlatpakDetector::setLastError(const QString &error)
{
    lastError = error;
    ltr_int_log_message("FlatpakDetector::setLastError() - %s\n", error.toUtf8().constData());
}
