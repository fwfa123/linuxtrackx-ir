#ifndef FLATPAK_DETECTOR_H
#define FLATPAK_DETECTOR_H

#include <QString>
#include <QStringList>
#include <QObject>

/**
 * @brief Class for detecting and managing Flatpak installations
 *
 * This class provides functionality to detect Flatpak installations,
 * list installed applications, and get information about specific apps.
 */
class FlatpakDetector : public QObject
{
    Q_OBJECT

    // Timeout constants for Flatpak command execution (in milliseconds)
    static constexpr int FLATPAK_VERSION_TIMEOUT_MS = 5000;    // 5 seconds for version check
    static constexpr int FLATPAK_COMMAND_TIMEOUT_MS = 10000;   // 10 seconds for other commands

public:
    FlatpakDetector(QObject *parent = nullptr);
    ~FlatpakDetector();

    /**
     * @brief Check if Flatpak is installed on the system
     * @return true if flatpak command is available, false otherwise
     */
    bool isFlatpakInstalled();

    /**
     * @brief Check if a specific Flatpak application is installed
     * @param appId The Flatpak application ID (e.g., "com.valvesoftware.Steam")
     * @return true if the application is installed, false otherwise
     */
    bool isAppInstalled(const QString &appId);

    /**
     * @brief Get a list of all installed Flatpak applications
     * @return QStringList of application IDs
     */
    QStringList listInstalledApps();

    /**
     * @brief Get detailed information about a Flatpak application
     * @param appId The Flatpak application ID
     * @return QString containing application information
     */
    QString getAppInfo(const QString &appId);

    /**
     * @brief Get the data directory for a Flatpak application
     * @param appId The Flatpak application ID
     * @return QString path to the application's data directory
     */
    QString getAppDataPath(const QString &appId);

    /**
     * @brief Get the last error message
     * @return QString containing the last error message
     */
    QString getLastError() const;

private:
    QString lastError;
    void setLastError(const QString &error);
    // Cached state to reduce repeated flatpak invocations during a session
    QStringList cachedApps;
    bool isAppListCached = false;
    bool flatpakChecked = false;
    bool flatpakInstalled = false;
};

#endif // FLATPAK_DETECTOR_H
