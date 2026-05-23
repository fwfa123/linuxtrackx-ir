#ifndef LUTRIS_INTEGRATION_H
#define LUTRIS_INTEGRATION_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QSettings>
#include "flatpak_detector.h"
#include "wine_bridge_install.h"

class QWidget;

class LutrisGame {
public:
    int install_id;
    QString game_slug;
    QString runner_type;
    QString install_path;
    int config_id;
    QString wine_prefix;
    QString wine_version;
    QString game_name;
    QString config_path;
    
    LutrisGame() : install_id(0), config_id(0) {}
};

class LutrisIntegration : public QObject
{
    Q_OBJECT

public:
    explicit LutrisIntegration(QObject *parent = nullptr);
    ~LutrisIntegration();
    
    // Main functionality
    bool isLutrisInstalled();
    bool isLutrisInstalledFlatpak();
    QStringList getAvailableGames();
    QList<LutrisGame> getLutrisGames();
    bool installToLutrisGame(const QString &gameSlug);
    bool installToLutrisGame(const LutrisGame &game);
    bool installToLutrisPrefix(const QString &prefixPath, const QString &wineVersion = QString());
    // Fast path: set the currently selected game's identifiers so we can avoid full scans
    void setSelectedLutrisGameConfig(const QString &gameSlug, const QString &configPath);
    
    // Database operations
    bool openLutrisDatabase();
    void closeLutrisDatabase();
    QList<LutrisGame> queryLutrisGames();
    
    // Configuration parsing
    bool parseLutrisConfig(const QString &configPath, LutrisGame &game);
    QString findWinePrefix(const QString &configPath);
    QString findWineVersion(const QString &configPath);
    
    // Utility methods
    QString getLutrisDatabasePath();
    QString getLutrisConfigPath();
    /** Resolve Lutris runner version (e.g. "GE-Proton9-27") to full path to wine binary. Uses Flatpak runners path when applicable. Returns empty if not found.
     *  Issue #43: Same logic on Qt5 (main) and Qt6 (qt6-migration); supports Lutris Flatpak Wine/Proton paths. */
    QString resolveWineBinaryPath(const QString &wineVersion);
    QStringList getWinePrefixes();
    bool isValidWinePrefix(const QString &prefixPath);
    bool hasGameConfigs(const QString &directoryPath);  // Check if directory contains .yml game config files

    // Custom path configuration
    void setCustomPaths(const QString &dbPath, const QString &cfgPath);
    void loadCustomPaths();
    void saveCustomPaths();
    bool hasCustomPaths() const;
    void clearCustomPaths();

    // Error handling
    QString getLastError() const { return lastError; }
    void clearError() { lastError.clear(); }
    QString getDebugInfo() const { return debugInfo; }

    void setInstallPromptParent(QWidget *parent) { installPromptParent = parent; }
    WineBridgeInstall::InstallOutcome getLastInstallOutcome() const { return lastInstallOutcome; }

public:
    // Path initialization (public for external re-initialization)
    bool initializePaths();

private:
    QSqlDatabase lutrisDb;
    QString lastError;
    QString debugInfo;
    QString databasePath;
    QString configPath;
    /** When non-empty (Flatpak Lutris), base path for runners e.g. .../data/lutris; runners/wine/ and runners/proton/ live under it. */
    QString lutrisRunnersBasePath;
    /** Guard: only try on-demand Flatpak detection in resolveWineBinaryPath once per session. */
    bool flatpakResolveAttempted = false;
    // Custom path configuration
    bool useCustomPaths;
    QString customDatabasePath;
    QString customConfigPath;
    // Cache for fast-path installs
    bool hasSelectedGame = false;
    QString selectedGameSlug;
    QString selectedConfigPath;
    LutrisGame selectedParsedGame;
    QMap<QString, LutrisGame> configParseCache; // key: configPath
    QWidget *installPromptParent = nullptr;
    WineBridgeInstall::InstallOutcome lastInstallOutcome = WineBridgeInstall::InstallOutcome::Failed;
    QString getHomeDirectory();
    bool detectLutrisFlatpak();
    void setupFlatpakLutrisPaths();
    bool createWineEnvironment(const LutrisGame &game);
    bool runWineBridgeInstaller(const QString &prefixPath, const QString &winePath);
};

#endif // LUTRIS_INTEGRATION_H 