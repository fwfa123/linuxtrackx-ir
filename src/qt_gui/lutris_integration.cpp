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
#include "installer_paths.h"
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QProcessEnvironment>

LutrisIntegration::LutrisIntegration(QObject *parent)
    : QObject(parent)
    , useCustomPaths(false)
{
    loadCustomPaths();
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

    // Priority 1: Check for custom paths from QSettings
    if (useCustomPaths && !customDatabasePath.isEmpty() && !customConfigPath.isEmpty()) {
        databasePath = customDatabasePath;
        configPath = customConfigPath;
        ltr_int_log_message("LutrisIntegration::initializePaths() - Using custom paths from settings\n");
        ltr_int_log_message("LutrisIntegration::initializePaths() - Custom database path: %s\n", databasePath.toUtf8().constData());
        ltr_int_log_message("LutrisIntegration::initializePaths() - Custom config path: %s\n", configPath.toUtf8().constData());
        return true;
    }

    // Priority 2: Check for environment variable LUTRIS_INSTALL_DIR
    QString lutrisRoot = QString::fromUtf8(qgetenv("LUTRIS_INSTALL_DIR"));
    if (!lutrisRoot.isEmpty() && !lutrisRoot.endsWith(QLatin1Char('/'))) {
        lutrisRoot += QLatin1Char('/');
    }

    if (!lutrisRoot.isEmpty()) {
        databasePath = lutrisRoot + QString::fromUtf8("pga.db");
        configPath = lutrisRoot + QString::fromUtf8("games/");
        ltr_int_log_message("LutrisIntegration::initializePaths() - Using environment variable LUTRIS_INSTALL_DIR: %s\n", lutrisRoot.toUtf8().constData());
        ltr_int_log_message("LutrisIntegration::initializePaths() - Database path: %s\n", databasePath.toUtf8().constData());
        ltr_int_log_message("LutrisIntegration::initializePaths() - Config path: %s\n", configPath.toUtf8().constData());
        return true;
    }

    // Priority 3: Fall back to default home-based paths with smart detection
    databasePath = homeDir + QString::fromUtf8("/.local/share/lutris/pga.db");
    
    // Detect config path: Check for Lutris 5.19+ structure first, then fall back to older structure
    QString configPathNew = homeDir + QString::fromUtf8("/.local/share/lutris/games/");
    QString configPathOld = homeDir + QString::fromUtf8("/.config/lutris/games/");
    
    QDir configDirNew(configPathNew);
    QDir configDirOld(configPathOld);
    
    bool newPathExists = configDirNew.exists();
    bool oldPathExists = configDirOld.exists();
    
    ltr_int_log_message("LutrisIntegration::initializePaths() - Checking config path locations:\n");
    ltr_int_log_message("  Lutris 5.19+ path: %s (exists=%s)\n", 
                       configPathNew.toUtf8().constData(), 
                       newPathExists ? "yes" : "no");
    ltr_int_log_message("  Older path: %s (exists=%s)\n", 
                       configPathOld.toUtf8().constData(), 
                       oldPathExists ? "yes" : "no");
    
    // Validate paths: check if they contain actual game config files
    bool newPathValid = newPathExists && hasGameConfigs(configPathNew);
    bool oldPathValid = oldPathExists && hasGameConfigs(configPathOld);
    
    ltr_int_log_message("  Lutris 5.19+ path validation: valid=%s\n", newPathValid ? "yes" : "no");
    ltr_int_log_message("  Older path validation: valid=%s\n", oldPathValid ? "yes" : "no");
    
    // Choose path: prioritize newer structure if it exists and is valid,
    // otherwise use older structure if it exists and is valid,
    // otherwise default to newer structure (for new installations)
    if (newPathValid) {
        configPath = configPathNew;
        ltr_int_log_message("LutrisIntegration::initializePaths() - Selected Lutris 5.19+ config path (validated)\n");
    } else if (oldPathValid) {
        configPath = configPathOld;
        ltr_int_log_message("LutrisIntegration::initializePaths() - Selected older config path (validated)\n");
    } else if (newPathExists) {
        configPath = configPathNew;
        ltr_int_log_message("LutrisIntegration::initializePaths() - Selected Lutris 5.19+ config path (exists but no .yml files yet)\n");
    } else if (oldPathExists) {
        configPath = configPathOld;
        ltr_int_log_message("LutrisIntegration::initializePaths() - Selected older config path (exists but no .yml files yet)\n");
    } else {
        // Neither exists - default to newer structure for new installations
        configPath = configPathNew;
        ltr_int_log_message("LutrisIntegration::initializePaths() - Neither path exists, defaulting to Lutris 5.19+ structure\n");
    }
    
    // Debug logging to help identify path issues
    ltr_int_log_message("LutrisIntegration::initializePaths() - Final paths:\n");
    ltr_int_log_message("  Home directory: %s\n", homeDir.toUtf8().constData());
    ltr_int_log_message("  Database path: %s\n", databasePath.toUtf8().constData());
    ltr_int_log_message("  Config path: %s\n", configPath.toUtf8().constData());

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
    // Check if Lutris binary exists in the current PATH
    const QString lutrisExe = QStandardPaths::findExecutable(QStringLiteral("lutris"));
    if (lutrisExe.isEmpty()) {
        // Binary not found in PATH, try Flatpak fallback
        if (detectLutrisFlatpak()) {
            ltr_int_log_message("Lutris detected via Flatpak fallback.\n");
            return true;
        }
        ltr_int_log_message("Lutris executable not found in PATH.\n");
        return false;            // truly *not* installed
    }
    ltr_int_log_message("Found Lutris binary at: %s\n", lutrisExe.toUtf8().constData());

    // Check per-user data – but don't fail if it's missing
    const QFileInfo dbFile(databasePath);
    bool dbExists = dbFile.exists();

    QDir cfgDir(configPath);
    bool cfgExists = cfgDir.exists();

    ltr_int_log_message("Database file %s (exists=%s)\n",
                        databasePath.toUtf8().constData(),
                        dbExists ? "yes" : "no");
    ltr_int_log_message("Config dir   %s (exists=%s)\n",
                        configPath.toUtf8().constData(),
                        cfgExists ? "yes" : "no");

    // If the binary is there, we consider Lutris *installed*,
    // even if the user hasn't run it yet.
    if (!dbExists && !cfgExists) {
        ltr_int_log_message("Lutris binary present but per-user data missing – "
                            "likely not yet launched.\n");
        // Optionally create empty config dir so subsequent calls see something:
        // QDir().mkpath(configPath);
        return true;           // still "installed"
    }

    // If DB or cfg exists, everything is fine
    ltr_int_log_message("Lutris installation detected.\n");
    return true;
}

bool LutrisIntegration::openLutrisDatabase()
{
    if (!isLutrisInstalled()) {
        return false;
    }
    
    lutrisDb = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("LutrisConnection"));
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
    QSqlDatabase::removeDatabase(QStringLiteral("LutrisConnection"));
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
    runnerQuery.prepare(QStringLiteral("SELECT DISTINCT runner FROM games"));
    if (runnerQuery.exec()) {
        QStringList runners;
        while (runnerQuery.next()) {
            runners.append(runnerQuery.value(0).toString());
        }
        debugInfo = QString::fromUtf8("Available runners: ") + runners.join(QString::fromUtf8(", ")) + QString::fromUtf8("\n");
    }
    
    // Now query for wine games
    QSqlQuery query(lutrisDb);
    query.prepare(QStringLiteral("SELECT id, slug, runner, directory, configpath, name FROM games WHERE runner = 'wine'"));
    
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
    
    // Extract wine version / wine executable - parse YAML structure properly
    QStringList lines = content.split(QString::fromUtf8("\n"));
    QString wineSectionVersion;
    QString topLevelVersion;
    QString wineExecutablePath;
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
        if (trimmedLine == QStringLiteral("wine:")) {
            inWineSection = true;
            wineIndent = indent;
            ltr_int_log_message("Entered wine section at indent level %d\n", indent);
            continue;
        }
        
        // If we're in the wine section, look for version or executable/path
        if (inWineSection && indent > wineIndent) {
            if (trimmedLine.startsWith(QStringLiteral("version:"))) {
                wineSectionVersion = trimmedLine.mid(8).trimmed();
                ltr_int_log_message("Found wine.version: '%s'\n", wineSectionVersion.toUtf8().constData());
                // Remove quotes if present
                if (wineSectionVersion.startsWith(QStringLiteral("\"")) && wineSectionVersion.endsWith(QStringLiteral("\""))) {
                    wineSectionVersion = wineSectionVersion.mid(1, wineSectionVersion.length() - 2);
                }
            } else if (trimmedLine.startsWith(QStringLiteral("executable:")) || trimmedLine.startsWith(QStringLiteral("path:"))) {
                QString exeValue = trimmedLine.mid(trimmedLine.indexOf(QLatin1Char(':')) + 1).trimmed();
                if (exeValue.startsWith(QStringLiteral("\"")) && exeValue.endsWith(QStringLiteral("\""))) {
                    exeValue = exeValue.mid(1, exeValue.length() - 2);
                }
                wineExecutablePath = exeValue;
                ltr_int_log_message("Found wine.executable/path: '%s'\n", wineExecutablePath.toUtf8().constData());
            }
        }
        
        // Exit wine section if we encounter a section at the same or lower indent level
        if (inWineSection && indent <= wineIndent && !trimmedLine.isEmpty()) {
            inWineSection = false;
            ltr_int_log_message("Exited wine section\n");
        }
    }
    
    // If wine section didn't yield anything, check for top-level keys
    if (wineExecutablePath.isEmpty()) {
        // Look for top-level wine_path: <abs_path>
        QRegularExpression winePathRe(QString::fromUtf8("^\\s*wine_path\\s*:\\s*['\"]?([^'\"\n]+)['\"]?"), QRegularExpression::MultilineOption);
        QRegularExpressionMatch m = winePathRe.match(content);
        if (m.hasMatch()) {
            wineExecutablePath = m.captured(1).trimmed();
            ltr_int_log_message("Found top-level wine_path: '%s'\n", wineExecutablePath.toUtf8().constData());
        }
    }

    // Look for wine_path in installer tasks (for games that specify wine during installation)
    if (wineExecutablePath.isEmpty()) {
        // Try quoted version first
        QRegularExpression quotedInstallerRe(QString::fromUtf8("\\s*wine_path\\s*:\\s*['\"]([^'\"\\n]+)['\"]"));
        QRegularExpressionMatch quotedMatch = quotedInstallerRe.match(content);
        if (quotedMatch.hasMatch()) {
            wineExecutablePath = quotedMatch.captured(1).trimmed();
            ltr_int_log_message("Found installer wine_path (quoted): %s\n", wineExecutablePath.toUtf8().constData());
        } else {
            // Try unquoted version
            QRegularExpression unquotedInstallerRe(QString::fromUtf8("\\s*wine_path\\s*:\\s*([^\\s\\n]+)"));
            QRegularExpressionMatch unquotedMatch = unquotedInstallerRe.match(content);
            if (unquotedMatch.hasMatch()) {
                wineExecutablePath = unquotedMatch.captured(1).trimmed();
                ltr_int_log_message("Found installer wine_path (unquoted): %s\n", wineExecutablePath.toUtf8().constData());
            }
        }
    }

    if (wineSectionVersion.isEmpty()) {
        // Look for top-level version: <string>
        for (const QString &line : lines) {
            int indent = 0;
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == QChar::fromLatin1(' ')) {
                    indent++;
                } else {
                    break;
                }
            }
            QString trimmedLine = line.trimmed();
            if (indent == 0 && trimmedLine.startsWith(QStringLiteral("version:"))) {
                QString v = trimmedLine.mid(8).trimmed();
                if (v.startsWith(QStringLiteral("\"")) && v.endsWith(QStringLiteral("\""))) {
                    v = v.mid(1, v.length() - 2);
                }
                topLevelVersion = v;
                ltr_int_log_message("Found top-level version: '%s'\n", topLevelVersion.toUtf8().constData());
                break;
            }
        }
    }

    // Selection precedence - prioritize runtime configuration over installer configuration:
    // 1) wine-section version (runtime configuration)
    // 2) top-level version (if it looks like a Wine runner id)
    // 3) wine executable path (installer or other paths)

    // Updated regex to match more patterns: lutris-*, wine-*, lutris-GE-*, wine-ge-*, etc.
    QRegularExpression plausibleRunner(QString::fromUtf8("^(lutris|wine)(-[a-zA-Z0-9]+)?-.+"));

    // Priority 1: Wine section version (runtime configuration)
    if (!wineSectionVersion.isEmpty()) {
        game.wine_version = wineSectionVersion;
        if (game.wine_version.contains(QString::fromUtf8("$HOME"))) {
            game.wine_version.replace(QString::fromUtf8("$HOME"), getHomeDirectory());
        }
        ltr_int_log_message("Using Wine version (wine section): %s\n", game.wine_version.toUtf8().constData());
    }
    // Priority 2: Top-level version (if it looks like a wine runner)
    else if (!topLevelVersion.isEmpty() && plausibleRunner.match(topLevelVersion).hasMatch()) {
        game.wine_version = topLevelVersion;
        if (game.wine_version.contains(QString::fromUtf8("$HOME"))) {
            game.wine_version.replace(QString::fromUtf8("$HOME"), getHomeDirectory());
        }
        ltr_int_log_message("Using Wine version (top-level): %s\n", game.wine_version.toUtf8().constData());
    }
    // Priority 3: Extract from wine executable path (installer or other paths)
    else if (!wineExecutablePath.isEmpty()) {
        // Expand environment variables
        if (wineExecutablePath.contains(QString::fromUtf8("$HOME"))) {
            wineExecutablePath.replace(QString::fromUtf8("$HOME"), getHomeDirectory());
        }
        if (wineExecutablePath.startsWith(QStringLiteral("~"))) {
            wineExecutablePath.replace(0, 1, getHomeDirectory());
        }

        // If this is a full path to wine binary, extract the version name
        if (wineExecutablePath.contains(QString::fromUtf8("/bin/wine"))) {
            // Extract version from path like: /home/user/.local/share/lutris/runners/wine/wine-ge-8-26-x86_64/bin/wine
            QString pathWithoutBin = wineExecutablePath.left(wineExecutablePath.lastIndexOf(QString::fromUtf8("/bin/wine")));
            QString versionFromPath = pathWithoutBin.mid(pathWithoutBin.lastIndexOf(QString::fromUtf8("/")) + 1);
            if (!versionFromPath.isEmpty() && versionFromPath != QString::fromUtf8("wine")) {
                game.wine_version = versionFromPath;
                ltr_int_log_message("Extracted version from wine path: %s\n", game.wine_version.toUtf8().constData());
            } else {
                game.wine_version = wineExecutablePath;
                ltr_int_log_message("Using Wine executable path (couldn't extract version): %s\n", game.wine_version.toUtf8().constData());
            }
        } else {
            game.wine_version = wineExecutablePath;
            ltr_int_log_message("Using Wine executable path: %s\n", game.wine_version.toUtf8().constData());
        }
    } else {
        ltr_int_log_message("No Wine version or path found in config\n");
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

void LutrisIntegration::setSelectedLutrisGameConfig(const QString &gameSlug, const QString &configPath)
{
    selectedGameSlug = gameSlug;
    selectedConfigPath = configPath;
    hasSelectedGame = !selectedGameSlug.isEmpty() && !selectedConfigPath.isEmpty();
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

bool LutrisIntegration::hasGameConfigs(const QString &directoryPath)
{
    if (directoryPath.isEmpty()) {
        return false;
    }
    
    QDir configDir(directoryPath);
    if (!configDir.exists()) {
        return false;
    }
    
    // Check if directory contains .yml files (game configs)
    QFileInfoList entries = configDir.entryInfoList(QDir::Files);
    for (const QFileInfo &entry : entries) {
        if (entry.suffix().toLower() == QString::fromUtf8("yml")) {
            return true;
        }
    }
    
    return false;
}

bool LutrisIntegration::installToLutrisGame(const QString &gameSlug)
{
    // Fast path: if the selected game is set and matches the slug, reuse it
    if (hasSelectedGame && selectedGameSlug == gameSlug) {
        // Ensure we have parsed details for the selected config
        if (selectedParsedGame.game_slug == selectedGameSlug && !selectedParsedGame.wine_prefix.isEmpty()) {
            return installToLutrisPrefix(selectedParsedGame.wine_prefix, selectedParsedGame.wine_version);
        }
        // Parse from cache or file
        LutrisGame parsed;
        if (configParseCache.contains(selectedConfigPath)) {
            parsed = configParseCache.value(selectedConfigPath);
        } else {
            if (!parseLutrisConfig(selectedConfigPath, parsed)) {
                lastError = QString::fromUtf8("Failed to parse selected game's config: ") + selectedConfigPath;
                return false;
            }
            parsed.game_slug = selectedGameSlug;
            configParseCache.insert(selectedConfigPath, parsed);
        }
        selectedParsedGame = parsed;
        return installToLutrisPrefix(parsed.wine_prefix, parsed.wine_version);
    }

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
    printf("DEBUG: installToLutrisPrefix called with wineVersion='%s'\n", wineVersion.toUtf8().constData());
    ltr_int_log_message("DEBUG: installToLutrisPrefix called with wineVersion='%s'\n", wineVersion.toUtf8().constData());
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
        
        // Resolve provided wineVersion into an executable path robustly
        QString trimmedVersion = actualWineVersion.trimmed();
        if (trimmedVersion.startsWith(QStringLiteral("~"))) {
            trimmedVersion.replace(0, 1, homeDir);
        }
        if (trimmedVersion.contains(QString::fromUtf8("$HOME"))) {
            trimmedVersion.replace(QString::fromUtf8("$HOME"), homeDir);
        }

        // Determine resolution branch explicitly
        const bool hasBinWine = trimmedVersion.contains(QStringLiteral("/bin/wine"));
        const bool looksAbsolute = QDir::isAbsolutePath(trimmedVersion) || trimmedVersion.startsWith(QLatin1Char('/'));
        const bool looksLikeLutrisRunnerPath = trimmedVersion.contains(QStringLiteral("/.local/share/lutris/runners/wine/"));

        ltr_int_log_message("wineVersion resolution: trimmed='%s', hasBinWine=%s, isAbsolute=%s, looksRunnerPath=%s\n",
                            trimmedVersion.toUtf8().constData(),
                            hasBinWine ? "true" : "false",
                            looksAbsolute ? "true" : "false",
                            looksLikeLutrisRunnerPath ? "true" : "false");

        ltr_int_log_message("DEBUG: homeDir='%s', trimmedVersion='%s'\n", homeDir.toUtf8().constData(), trimmedVersion.toUtf8().constData());

        QFileInfo directInfo(trimmedVersion);
        if (looksAbsolute || looksLikeLutrisRunnerPath) {
            ltr_int_log_message("DEBUG: Taking absolute/runner path branch\n");
            if (directInfo.isDir()) {
                winePath = QDir(trimmedVersion).filePath(QStringLiteral("bin/wine"));
            } else {
                winePath = trimmedVersion;
            }
            debugInfo += QString::fromUtf8("Using absolute/direct Wine path: ") + winePath + QString::fromUtf8("\n");
            ltr_int_log_message("Using absolute/direct Wine path: %s\n", winePath.toUtf8().constData());
        } else if (hasBinWine) {
            ltr_int_log_message("DEBUG: Taking hasBinWine branch\n");
            winePath = trimmedVersion;
            debugInfo += QString::fromUtf8("Using provided Wine path containing /bin/wine: ") + winePath + QString::fromUtf8("\n");
            ltr_int_log_message("Using provided Wine path containing /bin/wine: %s\n", winePath.toUtf8().constData());
        } else {
            ltr_int_log_message("DEBUG: Taking constructed path branch\n");
            // Only construct if it's not already an absolute path
            if (!QDir::isAbsolutePath(trimmedVersion)) {
                winePath = homeDir + QString::fromUtf8("/.local/share/lutris/runners/wine/") + trimmedVersion + QString::fromUtf8("/bin/wine");
                ltr_int_log_message("Constructed Wine path from runner version: %s\n", winePath.toUtf8().constData());
            } else {
                winePath = trimmedVersion;
                ltr_int_log_message("Using absolute Wine path as-is: %s\n", winePath.toUtf8().constData());
            }
        }
        
        QFileInfo wineFile(winePath);
        if (!wineFile.exists()) {
            debugInfo += QString::fromUtf8("Lutris Wine version not found at: ") + winePath + QString::fromUtf8("\n");
            ltr_int_log_message("Lutris Wine version not found at: %s\n", winePath.toUtf8().constData());
            
            // Try to find alternative Wine installations (prioritize /opt as default install location)
            QStringList alternativePaths;
            alternativePaths << QString::fromUtf8("/opt/wine-staging/bin/wine");
            alternativePaths << QString::fromUtf8("/opt/wine-stable/bin/wine");
            alternativePaths << QString::fromUtf8("/opt/wine/bin/wine");
            alternativePaths << QString::fromUtf8("/usr/local/bin/wine");
            alternativePaths << QString::fromUtf8("/usr/bin/wine");
            
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
    // Centralized resolution of linuxtrack-wine.exe
    QString installerPath = InstallerPaths::resolveWineBridgeInstallerPath();
    if (installerPath.isEmpty()) {
        lastError = QString::fromUtf8("Linuxtrack Wine installer not found in expected locations.");
        debugInfo += lastError + QString::fromUtf8("\n");
        ltr_int_log_message("Wine installer not found in any expected location\n");
        return false;
    }
    debugInfo += QString::fromUtf8("Found Wine installer at: ") + installerPath + QString::fromUtf8("\n");
    ltr_int_log_message("Found Wine installer at: %s\n", installerPath.toUtf8().constData());

    // Prepare environment
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QStringLiteral("WINEPREFIX"), prefixPath);

    // Log what we're about to do
    debugInfo += QString::fromUtf8("Starting Wine installer with:\n");
    debugInfo += QString::fromUtf8("  Wine executable: ") + winePath + QString::fromUtf8("\n");
    debugInfo += QString::fromUtf8("  Installer path: ") + installerPath + QString::fromUtf8("\n");
    debugInfo += QString::fromUtf8("  Working directory: ") + prefixPath + QString::fromUtf8("\n");

    ltr_int_log_message("Starting Wine installer with:\n");
    ltr_int_log_message("  Wine executable: %s\n", winePath.toUtf8().constData());
    ltr_int_log_message("  Installer path: %s\n", installerPath.toUtf8().constData());
    ltr_int_log_message("  Working directory: %s\n", prefixPath.toUtf8().constData());
    ltr_int_log_message("  WINEPREFIX: %s\n", prefixPath.toUtf8().constData());

    // Launch via QProcess so we can observe completion (align with Steam flow)
    QProcess process;
    process.setProcessEnvironment(env);
    process.setWorkingDirectory(prefixPath);

    QStringList arguments;
    arguments << installerPath;

    process.start(winePath, arguments);
    if (!process.waitForStarted()) {
        QString errorMsg = QString::fromUtf8("Failed to start Wine process: ") + process.errorString();
        ltr_int_log_message("LutrisIntegration::runWineBridgeInstaller() - %s\n", errorMsg.toUtf8().constData());
        lastError = errorMsg;
        return false;
    }

    ltr_int_log_message("Wine process started successfully\n");

    // Wait for completion (120s like Proton path). NSIS is interactive; user should complete within this window.
    if (!process.waitForFinished(120000)) { // 120 seconds
        process.kill();
        QString errorMsg = QString::fromUtf8("Wine process timed out after 120 seconds");
        ltr_int_log_message("LutrisIntegration::runWineBridgeInstaller() - %s\n", errorMsg.toUtf8().constData());
        lastError = errorMsg;
        return false;
    }

    int exitCode = process.exitCode();
    ltr_int_log_message("LutrisIntegration::runWineBridgeInstaller() - Process finished with exit code: %d\n", exitCode);

    if (exitCode != 0) {
        QString errorOutput = QString::fromUtf8(process.readAllStandardError());
        QString standardOutput = QString::fromUtf8(process.readAllStandardOutput());
        ltr_int_log_message("LutrisIntegration::runWineBridgeInstaller() - Standard output: %s\n", standardOutput.toUtf8().constData());
        ltr_int_log_message("LutrisIntegration::runWineBridgeInstaller() - Error output: %s\n", errorOutput.toUtf8().constData());
        lastError = QString::fromUtf8("Wine process failed with exit code ") + QString::number(exitCode);
        if (!errorOutput.isEmpty()) {
            lastError += QString::fromUtf8(": ") + errorOutput;
        }
        return false;
    }

    ltr_int_log_message("LutrisIntegration::runWineBridgeInstaller() - Wine Bridge installer completed successfully\n");
    return true;
}

// Custom path configuration methods
void LutrisIntegration::setCustomPaths(const QString &dbPath, const QString &cfgPath)
{
    customDatabasePath = dbPath;
    customConfigPath = cfgPath;
    useCustomPaths = (!dbPath.isEmpty() && !cfgPath.isEmpty());
    ltr_int_log_message("LutrisIntegration::setCustomPaths() - Custom paths set: DB=%s, CFG=%s\n",
                       dbPath.toUtf8().constData(), cfgPath.toUtf8().constData());
}

void LutrisIntegration::loadCustomPaths()
{
    QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
    settings.beginGroup(QStringLiteral("LutrisIntegration"));

    customDatabasePath = settings.value(QStringLiteral("customDatabasePath")).toString();
    customConfigPath = settings.value(QStringLiteral("customConfigPath")).toString();
    useCustomPaths = settings.value(QStringLiteral("useCustomPaths"), false).toBool();

    settings.endGroup();

    ltr_int_log_message("LutrisIntegration::loadCustomPaths() - Loaded custom paths: DB=%s, CFG=%s, useCustom=%s\n",
                       customDatabasePath.toUtf8().constData(),
                       customConfigPath.toUtf8().constData(),
                       useCustomPaths ? "true" : "false");
}

void LutrisIntegration::saveCustomPaths()
{
    QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
    settings.beginGroup(QStringLiteral("LutrisIntegration"));

    settings.setValue(QStringLiteral("customDatabasePath"), customDatabasePath);
    settings.setValue(QStringLiteral("customConfigPath"), customConfigPath);
    settings.setValue(QStringLiteral("useCustomPaths"), useCustomPaths);

    settings.endGroup();

    ltr_int_log_message("LutrisIntegration::saveCustomPaths() - Saved custom paths: DB=%s, CFG=%s, useCustom=%s\n",
                       customDatabasePath.toUtf8().constData(),
                       customConfigPath.toUtf8().constData(),
                       useCustomPaths ? "true" : "false");
}

bool LutrisIntegration::hasCustomPaths() const
{
    return useCustomPaths && !customDatabasePath.isEmpty() && !customConfigPath.isEmpty();
}

void LutrisIntegration::clearCustomPaths()
{
    useCustomPaths = false;
    customDatabasePath.clear();
    customConfigPath.clear();

    // Remove from settings
    QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
    settings.beginGroup(QStringLiteral("LutrisIntegration"));
    settings.remove(QStringLiteral("customDatabasePath"));
    settings.remove(QStringLiteral("customConfigPath"));
    settings.remove(QStringLiteral("useCustomPaths"));
    settings.endGroup();

    ltr_int_log_message("LutrisIntegration::clearCustomPaths() - Custom paths cleared and settings removed\n");
}

// Flatpak detection methods
bool LutrisIntegration::isLutrisInstalledFlatpak()
{
    FlatpakDetector detector;
    return detector.isAppInstalled(QStringLiteral("net.lutris.Lutris"));
}

bool LutrisIntegration::detectLutrisFlatpak()
{
    FlatpakDetector detector;

    // Check if Lutris is installed as a Flatpak app
    if (!detector.isAppInstalled(QStringLiteral("net.lutris.Lutris"))) {
        ltr_int_log_message("LutrisIntegration::detectLutrisFlatpak() - Lutris Flatpak app not found\n");
        return false;
    }

    // Get the Lutris data directory for Flatpak
    QString flatpakLutrisPath = detector.getAppDataPath(QStringLiteral("net.lutris.Lutris")) + QStringLiteral("/lutris");

    ltr_int_log_message("LutrisIntegration::detectLutrisFlatpak() - Checking Flatpak Lutris path: %s\n",
                      flatpakLutrisPath.toUtf8().constData());

    // Check if the Lutris directory structure exists in Flatpak data
    if (QDir(flatpakLutrisPath).exists()) {
        ltr_int_log_message("LutrisIntegration::detectLutrisFlatpak() - Flatpak Lutris directory found\n");
        setupFlatpakLutrisPaths();
        return true;
    } else {
        ltr_int_log_message("LutrisIntegration::detectLutrisFlatpak() - Flatpak Lutris directory not found: %s\n",
                          flatpakLutrisPath.toUtf8().constData());
        return false;
    }
}

void LutrisIntegration::setupFlatpakLutrisPaths()
{
    FlatpakDetector detector;
    QString flatpakDataPath = detector.getAppDataPath(QStringLiteral("net.lutris.Lutris"));

    // Set up paths for Flatpak Lutris installation
    databasePath = flatpakDataPath + QStringLiteral("/lutris/pga.db");
    configPath = flatpakDataPath + QStringLiteral("/lutris/games/");

    ltr_int_log_message("LutrisIntegration::setupFlatpakLutrisPaths() - Flatpak Lutris paths configured:\n");
    ltr_int_log_message("  Database path: %s\n", databasePath.toUtf8().constData());
    ltr_int_log_message("  Config path: %s\n", configPath.toUtf8().constData());
}