#ifndef STEAM_INTEGRATION_H
#define STEAM_INTEGRATION_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QProcess>
#include "flatpak_detector.h"

class SteamGame {
public:
    QString game_id;
    QString game_name;
    QString proton_version;
    QString prefix_path;
    QString library_path;
    bool is_installed;
    QString app_manifest_path;
    
    SteamGame() : is_installed(false) {}
};

class SteamIntegration : public QObject
{
    Q_OBJECT

public:
    explicit SteamIntegration(QObject *parent = nullptr);
    ~SteamIntegration();
    
    // Main functionality
    bool isSteamInstalled();
    QString getSteamPath();
    bool isSteamInstalledFlatpak();
    QStringList getAvailableGames();
    QList<SteamGame> getSteamGames();
    bool installToSteamGame(const QString &gameId);
    bool installToSteamPrefix(const QString &prefixPath, const QString &protonVersion = QString());
    
    // Steam library operations
    bool parseLibraryFolders();
    QStringList getLibraryPaths();
    QString findGameLibrary(const QString &gameId);
    void scanLibrariesForGames();
    QList<SteamGame> scanLibraryForGames(const QString &libraryPath);
    
    // Game information parsing
    bool parseAppManifest(const QString &manifestPath, SteamGame &game);
    QString findProtonPrefix(const QString &gameId);
    QString findProtonPrefixInLibrary(const QString &gameId, const QString &libraryPath);
    QString findProtonVersion(const QString &gameId);
    
    // Utility methods
    QString getSteamAppsPath();
    QString getCompatDataPath();
    QStringList getProtonVersions();
    bool isValidProtonPrefix(const QString &prefixPath);
    QString getProtonPath(const QString &protonVersion);
    
    // Error handling
    QString getLastError() const;
    void clearError() { lastError.clear(); }
    QString getDebugInfo() const;
    void setLastError(const QString &error);

private:
    QString lastError;
    QString debugInfo;
    QString steamPath;
    QString steamAppsPath;
    QString compatDataPath;
    QStringList libraryPaths;
    QMap<QString, QString> gameToLibraryMap; // gameId -> libraryPath
    QString selectedPrefixLibrary; // Store which library was used for prefix lookup
    
    // Helper methods
    bool initializePaths();
    QString getHomeDirectory();
    bool detectSteamFlatpak();
    void setupFlatpakSteamPaths();
    bool createProtonEnvironment(const SteamGame &game);
    bool runWineBridgeInstaller(const QString &prefixPath, const QString &winePath);
    bool runWineBridgeInstallerWithProton(const QString &prefixPath, const QString &protonPath, const QProcessEnvironment &env);
    QStringList parseVDFFile(const QString &filePath);
    QString extractVDFValue(const QString &line);
    QString getGameNameFromAppManifest(const QString &manifestPath);
    bool isGameUsingProton(const QString &gameId);
};

#endif // STEAM_INTEGRATION_H 