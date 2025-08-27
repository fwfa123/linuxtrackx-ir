#include "flatpak_detector.h"
#include <QProcess>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include "../utils.h"

// Qt 5 compatibility - use QStringLiteral for string literals
#define FLATPAK_CMD QStringLiteral("flatpak")
#define VERSION_ARG QStringLiteral("--version")
#define LIST_ARG QStringLiteral("list")
#define APP_ARG QStringLiteral("--app")
#define COLUMNS_ARG QStringLiteral("--columns=application")
#define INFO_ARG QStringLiteral("info")
#define VAR_APP_PATH QStringLiteral("/.var/app/")
#define DATA_PATH QStringLiteral("/data")
#define NEWLINE QStringLiteral("\n")

FlatpakDetector::FlatpakDetector(QObject *parent)
    : QObject(parent)
{
}

FlatpakDetector::~FlatpakDetector()
{
}

bool FlatpakDetector::isFlatpakInstalled()
{
    if (flatpakChecked) {
        return flatpakInstalled;
    }

    QProcess process;
    process.start(FLATPAK_CMD, QStringList() << VERSION_ARG);
    bool finished = process.waitForFinished(FLATPAK_VERSION_TIMEOUT_MS);

    if (!finished) {
        process.kill();
        setLastError(QStringLiteral("Timeout checking flatpak version"));
        ltr_int_log_message("FlatpakDetector::isFlatpakInstalled() - Timeout while checking flatpak version\n");
        flatpakChecked = true;
        flatpakInstalled = false;
        return false;
    }

    if (process.exitCode() == 0) {
        QString output = QString::fromUtf8(process.readAllStandardOutput()).trimmed();
        ltr_int_log_message("FlatpakDetector::isFlatpakInstalled() - Flatpak version: %s\n",
                          output.toUtf8().constData());
        flatpakChecked = true;
        flatpakInstalled = true;
        return true;
    } else {
        QString errorOutput = QString::fromUtf8(process.readAllStandardError()).trimmed();
        ltr_int_log_message("FlatpakDetector::isFlatpakInstalled() - Flatpak not found or failed: %s\n",
                          errorOutput.toUtf8().constData());
        flatpakChecked = true;
        flatpakInstalled = false;
        return false;
    }
}

bool FlatpakDetector::isAppInstalled(const QString &appId)
{
    if (!isFlatpakInstalled()) {
        setLastError(QStringLiteral("Flatpak is not installed"));
        return false;
    }

    // Use cached list if available
    QStringList lines;
    if (isAppListCached) {
        lines = cachedApps;
    } else {
        QProcess process;
        process.start(FLATPAK_CMD, QStringList() << LIST_ARG << APP_ARG << COLUMNS_ARG);
        bool finished = process.waitForFinished(FLATPAK_COMMAND_TIMEOUT_MS);
        if (!finished) {
            process.kill();
            setLastError(QStringLiteral("Timeout listing Flatpak apps"));
            return false;
        }

        if (process.exitCode() != 0) {
            QString errorOutput = QString::fromUtf8(process.readAllStandardError()).trimmed();
            setLastError(QStringLiteral("Failed to list Flatpak apps: %1").arg(errorOutput));
            return false;
        }

        QString output = QString::fromUtf8(process.readAllStandardOutput());
        lines = output.split(QStringLiteral("\n"));
        lines.removeAll(QString()); // Remove empty strings
        // Cache for future calls
        cachedApps = lines;
        isAppListCached = true;
    }

    ltr_int_log_message("FlatpakDetector::isAppInstalled() - Checking for app: %s\n",
                      appId.toUtf8().constData());
    ltr_int_log_message("FlatpakDetector::isAppInstalled() - Found %d installed apps\n", lines.size());

    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        if (trimmedLine == appId) {
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

    if (isAppListCached) {
        return cachedApps;
    }

    QProcess process;
    process.start(FLATPAK_CMD, QStringList() << LIST_ARG << APP_ARG << COLUMNS_ARG);
    bool finished = process.waitForFinished(FLATPAK_COMMAND_TIMEOUT_MS);
    if (!finished) {
        process.kill();
        setLastError(QStringLiteral("Timeout listing Flatpak apps"));
        return apps;
    }

    if (process.exitCode() != 0) {
        QString errorOutput = QString::fromUtf8(process.readAllStandardError()).trimmed();
        setLastError(QStringLiteral("Failed to list Flatpak apps: %1").arg(errorOutput));
        return apps;
    }

    QString output = QString::fromUtf8(process.readAllStandardOutput());
    apps = output.split(QStringLiteral("\n"));
    apps.removeAll(QString()); // Remove empty strings

    // Trim whitespace from each app ID
    for (int i = 0; i < apps.size(); ++i) {
        apps[i] = apps[i].trimmed();
    }

    // Remove empty strings
    apps.removeAll(QString());

    // Cache
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

    QProcess process;
    process.start(FLATPAK_CMD, QStringList() << INFO_ARG << appId);
    bool finished = process.waitForFinished(FLATPAK_COMMAND_TIMEOUT_MS);

    if (!finished) {
        process.kill();
        setLastError(QStringLiteral("Timeout getting Flatpak app info for %1").arg(appId));
        return QString();
    }

    if (process.exitCode() != 0) {
        QString errorOutput = QString::fromUtf8(process.readAllStandardError()).trimmed();
        setLastError(QStringLiteral("Failed to get app info for %1: %2").arg(appId, errorOutput));
        return QString();
    }

    QString output = QString::fromUtf8(process.readAllStandardOutput());
    ltr_int_log_message("FlatpakDetector::getAppInfo() - Info for %s:\n%s\n",
                      appId.toUtf8().constData(), output.toUtf8().constData());

    return output;
}

QString FlatpakDetector::getAppDataPath(const QString &appId)
{
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    // Typical Flatpak data directory structure
    QString dataPath = homeDir + VAR_APP_PATH + appId + DATA_PATH;

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
