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
    QStringList getAvailableGames();
    QList<LutrisGame> getLutrisGames();
    bool installToLutrisGame(const QString &gameSlug);
    bool installToLutrisPrefix(const QString &prefixPath, const QString &wineVersion = QString());
    
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
    QStringList getWinePrefixes();
    bool isValidWinePrefix(const QString &prefixPath);
    
    // Error handling
    QString getLastError() const { return lastError; }
    void clearError() { lastError.clear(); }
    QString getDebugInfo() const { return debugInfo; }

private:
    QSqlDatabase lutrisDb;
    QString lastError;
    QString debugInfo;
    QString databasePath;
    QString configPath;
    
    // Helper methods
    bool initializePaths();
    QString getHomeDirectory();
    bool createWineEnvironment(const LutrisGame &game);
    bool runWineBridgeInstaller(const QString &prefixPath, const QString &winePath);
};

#endif // LUTRIS_INTEGRATION_H 