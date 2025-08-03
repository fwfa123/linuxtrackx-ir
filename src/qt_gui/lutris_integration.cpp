#include "lutris_integration.h"
#include <QDebug>
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include "../utils.h"

LutrisIntegration::LutrisIntegration(QObject *parent)
    : QObject(parent)
{
    initializePaths();
}

LutrisIntegration::~LutrisIntegration()
{
    closeLutrisDatabase();
}

bool LutrisIntegration::initializePaths()
{
    QString homeDir = getHomeDirectory();
    
    // CRITICAL: Verify that the home directory is not a hardcoded build-time path
    QString currentUser = QString::fromUtf8(qgetenv("USER"));
    QString expectedHome = QString::fromUtf8("/home/") + currentUser;
    
    ltr_int_log_message("LutrisIntegration::initializePaths() - Current user: %s\n", currentUser.toUtf8().constData());
    ltr_int_log_message("LutrisIntegration::initializePaths() - Expected home: %s\n", expectedHome.toUtf8().constData());
    ltr_int_log_message("LutrisIntegration::initializePaths() - Actual home: %s\n", homeDir.toUtf8().constData());
    
    // If the home directory looks like a hardcoded build-time path, try to fix it
    // Check if the home directory contains a username that doesn't match the current user
    QStringList homeParts = homeDir.split(QString::fromUtf8("/"));
    if (homeParts.size() >= 3 && homeParts[1] == QString::fromUtf8("home") && 
        homeParts[2] != currentUser && !currentUser.isEmpty()) {
        ltr_int_log_message("LutrisIntegration::initializePaths() - WARNING: Detected hardcoded build-time path!\n");
        ltr_int_log_message("LutrisIntegration::initializePaths() - Attempting to fix by using current user's home\n");
        homeDir = expectedHome;
    }
    
    databasePath = homeDir + QString::fromUtf8("/.local/share/lutris/pga.db");
    configPath = homeDir + QString::fromUtf8("/.config/lutris/games/");
    
    // Debug logging to help identify path issues
    ltr_int_log_message("LutrisIntegration::initializePaths() - Final home directory: %s\n", homeDir.toUtf8().constData());
    ltr_int_log_message("LutrisIntegration::initializePaths() - Database path: %s\n", databasePath.toUtf8().constData());
    ltr_int_log_message("LutrisIntegration::initializePaths() - Config path: %s\n", configPath.toUtf8().constData());
    
    return true;
}

QString LutrisIntegration::getHomeDirectory()
{
    // CRITICAL: Use environment variables first to avoid build-time path capture
    QString homeDir = QString::fromUtf8(qgetenv("HOME"));
    
    // Debug logging
    ltr_int_log_message("LutrisIntegration::getHomeDirectory() - HOME env var: %s\n", homeDir.toUtf8().constData());
    
    // Fallback to QStandardPaths only if environment variable is empty
    if (homeDir.isEmpty()) {
        homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        ltr_int_log_message("LutrisIntegration::getHomeDirectory() - QStandardPaths fallback: %s\n", homeDir.toUtf8().constData());
    }
    
    // Additional fallback using USER environment variable
    if (homeDir.isEmpty()) {
        QString user = QString::fromUtf8(qgetenv("USER"));
        if (!user.isEmpty()) {
            homeDir = QString::fromUtf8("/home/") + user;
            ltr_int_log_message("LutrisIntegration::getHomeDirectory() - Using /home/$USER: %s\n", homeDir.toUtf8().constData());
        }
    }
    
    // Final fallback - try to get from passwd
    if (homeDir.isEmpty()) {
        // Try to get home directory from passwd file
        QFile passwdFile(QString::fromUtf8("/etc/passwd"));
        if (passwdFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&passwdFile);
            QString user = QString::fromUtf8(qgetenv("USER"));
            if (!user.isEmpty()) {
                while (!in.atEnd()) {
                    QString line = in.readLine();
                    QStringList parts = line.split(QString::fromUtf8(":"));
                    if (parts.size() >= 6 && parts[0] == user) {
                        homeDir = parts[5];
                        ltr_int_log_message("LutrisIntegration::getHomeDirectory() - Found in passwd: %s\n", homeDir.toUtf8().constData());
                        break;
                    }
                }
            }
            passwdFile.close();
        }
    }
    
    // If all else fails, use a reasonable default
    if (homeDir.isEmpty()) {
        homeDir = QString::fromUtf8("/home/unknown");
        ltr_int_log_message("LutrisIntegration::getHomeDirectory() - Using fallback: %s\n", homeDir.toUtf8().constData());
    }
    
    return homeDir;
}

QString LutrisIntegration::getLutrisDatabasePath()
{
    return databasePath;
}

QString LutrisIntegration::getLutrisConfigPath()
{
    return configPath;
}

bool LutrisIntegration::isLutrisInstalled()
{
    // Debug logging to show what paths are being checked
    ltr_int_log_message("LutrisIntegration::isLutrisInstalled() - Checking database path: %s\n", databasePath.toUtf8().constData());
    ltr_int_log_message("LutrisIntegration::isLutrisInstalled() - Checking config path: %s\n", configPath.toUtf8().constData());
    
    // Check if Lutris database exists
    QFileInfo dbFile(databasePath);
    if (!dbFile.exists()) {
        // Provide more helpful error message with suggestions
        QString currentUser = QString::fromUtf8(qgetenv("USER"));
        QString expectedPath = QString::fromUtf8("/home/") + currentUser + QString::fromUtf8("/.local/share/lutris/pga.db");
        
        lastError = QString::fromUtf8("Lutris database not found at: ") + databasePath + QString::fromUtf8("\n\n");
        lastError += QString::fromUtf8("Expected location: ") + expectedPath + QString::fromUtf8("\n\n");
        lastError += QString::fromUtf8("This could mean:\n");
        lastError += QString::fromUtf8("1. Lutris is not installed\n");
        lastError += QString::fromUtf8("2. Lutris database is in a different location\n");
        lastError += QString::fromUtf8("3. The path was hardcoded during AppImage build\n\n");
        lastError += QString::fromUtf8("To install Lutris:\n");
        lastError += QString::fromUtf8("  Ubuntu/Debian: sudo apt install lutris\n");
        lastError += QString::fromUtf8("  Fedora: sudo dnf install lutris\n");
        lastError += QString::fromUtf8("  Arch: sudo pacman -S lutris");
        
        ltr_int_log_message("LutrisIntegration::isLutrisInstalled() - Database file does not exist\n");
        return false;
    }
    
    // Check if config directory exists
    QDir configDir(configPath);
    if (!configDir.exists()) {
        lastError = QString::fromUtf8("Lutris config directory not found at: ") + configPath + QString::fromUtf8("\n\n");
        lastError += QString::fromUtf8("This usually means Lutris is not properly installed or configured.");
        ltr_int_log_message("LutrisIntegration::isLutrisInstalled() - Config directory does not exist\n");
        return false;
    }
    
    ltr_int_log_message("LutrisIntegration::isLutrisInstalled() - Lutris installation found\n");
    return true;
}

bool LutrisIntegration::openLutrisDatabase()
{
    if (!isLutrisInstalled()) {
        return false;
    }
    
    lutrisDb = QSqlDatabase::addDatabase(QString::fromUtf8("QSQLITE"), QString::fromUtf8("LutrisConnection"));
    lutrisDb.setDatabaseName(databasePath);
    
    if (!lutrisDb.open()) {
        lastError = QString::fromUtf8("Failed to open Lutris database: ") + lutrisDb.lastError().text();
        return false;
    }
    
    return true;
}

void LutrisIntegration::closeLutrisDatabase()
{
    if (lutrisDb.isOpen()) {
        lutrisDb.close();
    }
    QSqlDatabase::removeDatabase(QString::fromUtf8("LutrisConnection"));
}

QList<LutrisGame> LutrisIntegration::queryLutrisGames()
{
    QList<LutrisGame> games;
    
    if (!lutrisDb.isOpen()) {
        if (!openLutrisDatabase()) {
            return games;
        }
    }
    
    // First, let's see what runners are available
    QSqlQuery runnerQuery(lutrisDb);
    runnerQuery.prepare(QString::fromUtf8("SELECT DISTINCT runner FROM games"));
    if (runnerQuery.exec()) {
        QStringList runners;
        while (runnerQuery.next()) {
            runners.append(runnerQuery.value(0).toString());
        }
        debugInfo = QString::fromUtf8("Available runners: ") + runners.join(QString::fromUtf8(", ")) + QString::fromUtf8("\n");
    }
    
    // Now query for wine games
    QSqlQuery query(lutrisDb);
    query.prepare(QString::fromUtf8("SELECT id, slug, runner, directory, configpath, name FROM games WHERE runner = 'wine'"));
    
    if (!query.exec()) {
        lastError = QString::fromUtf8("Failed to query Lutris games: ") + query.lastError().text();
        return games;
    }
    
    int totalGames = 0;
    int validGames = 0;
    
    while (query.next()) {
        totalGames++;
        LutrisGame game;
        game.install_id = query.value(0).toInt();
        game.game_slug = query.value(1).toString();
        game.runner_type = query.value(2).toString();
        game.install_path = query.value(3).toString();
        game.config_path = query.value(4).toString();
        game.game_name = query.value(5).toString();
        
        // Parse configuration file to get wine prefix and version
        if (!game.config_path.isEmpty()) {
            // Debug: Show the full config path being used
            if (totalGames <= 5) {
                debugInfo += QString::fromUtf8("  Full config path: ") + getLutrisConfigPath() + QString::fromUtf8("/") + game.config_path + QString::fromUtf8(".yml\n");
            }
            parseLutrisConfig(game.config_path, game);
        }
        
        // Debug: Show first few games' prefix info
        if (totalGames <= 5) {
            debugInfo += QString::fromUtf8("Game ") + QString::number(totalGames) + QString::fromUtf8(": ") + game.game_name + QString::fromUtf8("\n");
            debugInfo += QString::fromUtf8("  Config path: ") + game.config_path + QString::fromUtf8("\n");
            debugInfo += QString::fromUtf8("  Wine prefix: ") + game.wine_prefix + QString::fromUtf8("\n");
            debugInfo += QString::fromUtf8("  Wine version: ") + game.wine_version + QString::fromUtf8("\n");
            
            // Detailed validation debugging
            if (!game.wine_prefix.isEmpty()) {
                QDir prefixDir(game.wine_prefix);
                QDir driveCDir(game.wine_prefix + QString::fromUtf8("/drive_c"));
                QDir windowsDir(game.wine_prefix + QString::fromUtf8("/drive_c/windows"));
                
                debugInfo += QString::fromUtf8("  Prefix exists: ") + (prefixDir.exists() ? QString::fromUtf8("Yes") : QString::fromUtf8("No")) + QString::fromUtf8("\n");
                debugInfo += QString::fromUtf8("  drive_c exists: ") + (driveCDir.exists() ? QString::fromUtf8("Yes") : QString::fromUtf8("No")) + QString::fromUtf8("\n");
                debugInfo += QString::fromUtf8("  windows exists: ") + (windowsDir.exists() ? QString::fromUtf8("Yes") : QString::fromUtf8("No")) + QString::fromUtf8("\n");
            }
            debugInfo += QString::fromUtf8("  Is valid: ") + (isValidWinePrefix(game.wine_prefix) ? QString::fromUtf8("Yes") : QString::fromUtf8("No")) + QString::fromUtf8("\n");
        }
        
        // Only add games with valid wine prefixes
        if (!game.wine_prefix.isEmpty() && isValidWinePrefix(game.wine_prefix)) {
            games.append(game);
            validGames++;
        }
    }
    
    debugInfo += QString::fromUtf8("Total wine games found: ") + QString::number(totalGames) + QString::fromUtf8("\n");
    debugInfo += QString::fromUtf8("Valid wine prefixes: ") + QString::number(validGames) + QString::fromUtf8("\n");
    
    return games;
}

QStringList LutrisIntegration::getAvailableGames()
{
    QStringList gameNames;
    QList<LutrisGame> games = queryLutrisGames();
    
    for (const LutrisGame &game : games) {
        if (!game.game_name.isEmpty()) {
            gameNames.append(game.game_name);
        } else {
            gameNames.append(game.game_slug);
        }
    }
    
    // Sort alphabetically
    gameNames.sort();
    
    return gameNames;
}

QList<LutrisGame> LutrisIntegration::getLutrisGames()
{
    QList<LutrisGame> games = queryLutrisGames();
    
    // Sort games alphabetically by name
    std::sort(games.begin(), games.end(), [](const LutrisGame &a, const LutrisGame &b) {
        QString nameA = a.game_name.isEmpty() ? a.game_slug : a.game_name;
        QString nameB = b.game_name.isEmpty() ? b.game_slug : b.game_name;
        return nameA.compare(nameB, Qt::CaseInsensitive) < 0;
    });
    
    return games;
}

bool LutrisIntegration::parseLutrisConfig(const QString &configPath, LutrisGame &game)
{
    QString fullPath = getLutrisConfigPath() + QString::fromUtf8("/") + configPath + QString::fromUtf8(".yml");
    
    QFile file(fullPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        lastError = QString::fromUtf8("Failed to open Lutris config file: ") + fullPath;
        return false;
    }
    
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    
    // Debug: Show config content for debugging
    debugInfo += QString::fromUtf8("  Config file exists: Yes\n");
    debugInfo += QString::fromUtf8("  Config content (first 500 chars): ") + content.left(500) + QString::fromUtf8("\n");
    ltr_int_log_message("Config content (first 500 chars): %s\n", content.left(500).toUtf8().constData());
    
    // Extract wine prefix - try multiple patterns
    QRegularExpression prefixRegex1(QString::fromUtf8("\\s*prefix:\\s*['\"]([^'\"]+)['\"]"));
    QRegularExpression prefixRegex2(QString::fromUtf8("\\s*prefix:\\s*([^\\s]+)"));
    QRegularExpression prefixRegex3(QString::fromUtf8("\\s*wine_prefix:\\s*['\"]([^'\"]+)['\"]"));
    QRegularExpression prefixRegex4(QString::fromUtf8("\\s*wine_prefix:\\s*([^\\s]+)"));
    
    QRegularExpressionMatch prefixMatch = prefixRegex1.match(content);
    if (!prefixMatch.hasMatch()) {
        prefixMatch = prefixRegex2.match(content);
    }
    if (!prefixMatch.hasMatch()) {
        prefixMatch = prefixRegex3.match(content);
    }
    if (!prefixMatch.hasMatch()) {
        prefixMatch = prefixRegex4.match(content);
    }
    
    if (prefixMatch.hasMatch()) {
        game.wine_prefix = prefixMatch.captured(1);
        // Expand environment variables if present
        if (game.wine_prefix.startsWith(QString::fromUtf8("$HOME"))) {
            game.wine_prefix.replace(QString::fromUtf8("$HOME"), getHomeDirectory());
        }
    }
    
    // Extract wine version - parse YAML structure properly
    QStringList lines = content.split(QString::fromUtf8("\n"));
    QString wineVersion;
    bool inWineSection = false;
    int wineIndent = 0;
    
    ltr_int_log_message("Searching for wine version in %d lines\n", lines.size());
    
    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        
        // Skip empty lines
        if (trimmedLine.isEmpty()) {
            continue;
        }
        
        // Calculate indentation level
        int indent = 0;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == QChar::fromLatin1(' ')) {
                indent++;
            } else {
                break;
            }
        }
        
        // Check if we're entering the wine section
        if (trimmedLine == QString::fromUtf8("wine:")) {
            inWineSection = true;
            wineIndent = indent;
            ltr_int_log_message("Entered wine section at indent level %d\n", indent);
            continue;
        }
        
        // If we're in the wine section, look for version
        if (inWineSection && indent > wineIndent) {
            if (trimmedLine.startsWith(QString::fromUtf8("version:"))) {
                wineVersion = trimmedLine.mid(8).trimmed();
                ltr_int_log_message("Found wine.version: '%s'\n", wineVersion.toUtf8().constData());
                // Remove quotes if present
                if (wineVersion.startsWith(QString::fromUtf8("\"")) && wineVersion.endsWith(QString::fromUtf8("\""))) {
                    wineVersion = wineVersion.mid(1, wineVersion.length() - 2);
                }
                break;
            }
        }
        
        // Exit wine section if we encounter a section at the same or lower indent level
        if (inWineSection && indent <= wineIndent && !trimmedLine.isEmpty()) {
            inWineSection = false;
            ltr_int_log_message("Exited wine section\n");
        }
    }
    
    if (!wineVersion.isEmpty()) {
        game.wine_version = wineVersion;
        // Expand $HOME if present
        if (game.wine_version.contains(QString::fromUtf8("$HOME"))) {
            game.wine_version.replace(QString::fromUtf8("$HOME"), getHomeDirectory());
        }
        ltr_int_log_message("Found Wine version: %s\n", game.wine_version.toUtf8().constData());
    } else {
        ltr_int_log_message("No Wine version found in config\n");
    }
    
    return true;
}

QString LutrisIntegration::findWinePrefix(const QString &configPath)
{
    LutrisGame game;
    if (parseLutrisConfig(configPath, game)) {
        return game.wine_prefix;
    }
    return QString();
}

QString LutrisIntegration::findWineVersion(const QString &configPath)
{
    LutrisGame game;
    if (parseLutrisConfig(configPath, game)) {
        return game.wine_version;
    }
    return QString();
}

QStringList LutrisIntegration::getWinePrefixes()
{
    QStringList prefixes;
    QList<LutrisGame> games = queryLutrisGames();
    
    for (const LutrisGame &game : games) {
        if (!game.wine_prefix.isEmpty() && isValidWinePrefix(game.wine_prefix)) {
            prefixes.append(game.wine_prefix);
        }
    }
    
    return prefixes;
}

bool LutrisIntegration::isValidWinePrefix(const QString &prefixPath)
{
    if (prefixPath.isEmpty()) {
        return false;
    }
    
    QDir prefixDir(prefixPath);
    if (!prefixDir.exists()) {
        return false;
    }
    
    // Check for typical wine prefix structure
    QDir driveCDir(prefixPath + QString::fromUtf8("/drive_c"));
    if (!driveCDir.exists()) {
        return false;
    }
    
    // Additional check for Windows directory
    QDir windowsDir(prefixPath + QString::fromUtf8("/drive_c/windows"));
    return windowsDir.exists();
}

bool LutrisIntegration::installToLutrisGame(const QString &gameSlug)
{
    QList<LutrisGame> games = queryLutrisGames();
    
    for (const LutrisGame &game : games) {
        if (game.game_slug == gameSlug) {
            return installToLutrisPrefix(game.wine_prefix, game.wine_version);
        }
    }
    
    lastError = QString::fromUtf8("Game not found: ") + gameSlug;
    return false;
}

bool LutrisIntegration::installToLutrisPrefix(const QString &prefixPath, const QString &wineVersion)
{
    // Build debug info
    debugInfo = QString::fromUtf8("Prefix path: ") + prefixPath + QString::fromUtf8("\n");
    debugInfo += QString::fromUtf8("Wine version: ") + wineVersion + QString::fromUtf8("\n");
    
    QDir prefixDir(prefixPath);
    debugInfo += QString::fromUtf8("Prefix directory exists: ") + (prefixDir.exists() ? QString::fromUtf8("Yes") : QString::fromUtf8("No")) + QString::fromUtf8("\n");
    
    QDir driveCDir(prefixPath + QString::fromUtf8("/drive_c"));
    debugInfo += QString::fromUtf8("drive_c exists: ") + (driveCDir.exists() ? QString::fromUtf8("Yes") : QString::fromUtf8("No")) + QString::fromUtf8("\n");
    
    QDir windowsDir(prefixPath + QString::fromUtf8("/drive_c/windows"));
    debugInfo += QString::fromUtf8("windows directory exists: ") + (windowsDir.exists() ? QString::fromUtf8("Yes") : QString::fromUtf8("No"));
    
    if (!isValidWinePrefix(prefixPath)) {
        lastError = QString::fromUtf8("Invalid wine prefix: ") + prefixPath + QString::fromUtf8("\n\nDebug info:\n") + debugInfo;
        return false;
    }
    
    // Find wine executable
    QString winePath;
    if (!wineVersion.isEmpty()) {
        QString homeDir = getHomeDirectory();
        
        // Handle special cases for "Default wine" and "winehq-staging"
        QString actualWineVersion = wineVersion;
        if (wineVersion == QString::fromUtf8("winehq-staging") || 
            wineVersion == QString::fromUtf8("Default wine") ||
            wineVersion == QString::fromUtf8("wine-staging")) {
            debugInfo += QString::fromUtf8("Detected 'Default wine' or 'winehq-staging', checking for system Wine\n");
            ltr_int_log_message("Detected 'Default wine' or 'winehq-staging', checking for system Wine\n");
        }
        
        winePath = homeDir + QString::fromUtf8("/.local/share/lutris/runners/wine/") + actualWineVersion + QString::fromUtf8("/bin/wine");
        
        QFileInfo wineFile(winePath);
        if (!wineFile.exists()) {
            debugInfo += QString::fromUtf8("Lutris Wine version not found at: ") + winePath + QString::fromUtf8("\n");
            ltr_int_log_message("Lutris Wine version not found at: %s\n", winePath.toUtf8().constData());
            
            // Try to find alternative Wine installations
            QStringList alternativePaths;
            alternativePaths << QString::fromUtf8("/usr/bin/wine");
            alternativePaths << QString::fromUtf8("/usr/local/bin/wine");
            alternativePaths << QString::fromUtf8("/opt/wine-staging/bin/wine");
            alternativePaths << QString::fromUtf8("/opt/wine-stable/bin/wine");
            
            bool foundAlternative = false;
            for (const QString &altPath : alternativePaths) {
                QFileInfo altWineFile(altPath);
                if (altWineFile.exists() && altWineFile.isExecutable()) {
                    winePath = altPath;
                    debugInfo += QString::fromUtf8("Found alternative Wine at: ") + winePath + QString::fromUtf8("\n");
                    ltr_int_log_message("Found alternative Wine at: %s\n", winePath.toUtf8().constData());
                    foundAlternative = true;
                    break;
                }
            }
            
            if (!foundAlternative) {
                winePath = QString::fromUtf8("wine"); // Fallback to system wine
                debugInfo += QString::fromUtf8("Falling back to system wine\n");
                ltr_int_log_message("Falling back to system wine\n");
            }
        } else {
            debugInfo += QString::fromUtf8("Using Lutris Wine version: ") + winePath + QString::fromUtf8("\n");
            ltr_int_log_message("Using Lutris Wine version: %s\n", winePath.toUtf8().constData());
        }
    } else {
        winePath = QString::fromUtf8("wine"); // Use system wine
        debugInfo += QString::fromUtf8("No Wine version specified, using system wine\n");
        ltr_int_log_message("No Wine version specified, using system wine\n");
    }
    
    debugInfo += QString::fromUtf8("Final Wine path: ") + winePath + QString::fromUtf8("\n");
    ltr_int_log_message("Final Wine path: %s\n", winePath.toUtf8().constData());
    
    return runWineBridgeInstaller(prefixPath, winePath);
}

bool LutrisIntegration::runWineBridgeInstaller(const QString &prefixPath, const QString &winePath)
{
    // Check if linuxtrack-wine.exe exists
    QString installerPath = QString::fromUtf8("/usr/local/share/linuxtrack/wine/linuxtrack-wine.exe");
    QFileInfo installerFile(installerPath);
    if (!installerFile.exists()) {
        lastError = QString::fromUtf8("Linuxtrack Wine installer not found at: ") + installerPath;
        return false;
    }
    
    // Add debug info about the Wine path being used
    debugInfo += QString::fromUtf8("Starting Wine installer with:\n");
    debugInfo += QString::fromUtf8("  Wine executable: ") + winePath + QString::fromUtf8("\n");
    debugInfo += QString::fromUtf8("  Installer path: ") + installerPath + QString::fromUtf8("\n");
    debugInfo += QString::fromUtf8("  Working directory: ") + prefixPath + QString::fromUtf8("\n");
    
    ltr_int_log_message("Starting Wine installer with:\n");
    ltr_int_log_message("  Wine executable: %s\n", winePath.toUtf8().constData());
    ltr_int_log_message("  Installer path: %s\n", installerPath.toUtf8().constData());
    ltr_int_log_message("  Working directory: %s\n", prefixPath.toUtf8().constData());
    
    // Set up environment variables
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QString::fromUtf8("WINEPREFIX"), prefixPath);
    env.insert(QString::fromUtf8("WINEARCH"), QString::fromUtf8("win64"));
    env.insert(QString::fromUtf8("WINEESYNC"), QString::fromUtf8("1"));
    env.insert(QString::fromUtf8("DXVK_STATE_CACHE_PATH"), prefixPath);
    env.insert(QString::fromUtf8("__GL_SHADER_DISK_CACHE_PATH"), prefixPath);
    
    // Create and run the wine process
    QProcess *process = new QProcess(this);
    process->setProcessEnvironment(env);
    process->setWorkingDirectory(prefixPath);
    
    QStringList arguments;
    arguments << installerPath;
    
    // Start the process but don't wait for completion since NSIS is interactive
    process->start(winePath, arguments);
    
    if (!process->waitForStarted()) {
        lastError = QString::fromUtf8("Failed to start wine process: ") + process->errorString();
        delete process;
        return false;
    }
    
    // For NSIS installers, we assume success if the process started successfully
    // The user will interact with the NSIS installer directly
    // We don't wait for completion as it's interactive
    // Don't delete the process - let it run independently
    process->setParent(nullptr); // Detach from parent so it can run independently
    return true;
} 