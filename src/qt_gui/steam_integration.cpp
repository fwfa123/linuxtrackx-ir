#include "steam_integration.h"
#include <QDebug>
#include <QProcess>
// #include <QMessageBox> // Not needed for CLI test
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDirIterator>
#include "../utils.h"

SteamIntegration::SteamIntegration(QObject *parent)
    : QObject(parent)
{
    initializePaths();
    parseLibraryFolders();
}

SteamIntegration::~SteamIntegration()
{
}

bool SteamIntegration::initializePaths()
{
    QString homeDir = getHomeDirectory();
    
    // CRITICAL: Verify that the home directory is not a hardcoded build-time path
    QString currentUser = QString::fromUtf8(qgetenv("USER"));
    QString expectedHome = QString::fromUtf8("/home/") + currentUser;
    
    ltr_int_log_message("SteamIntegration::initializePaths() - Current user: %s\n", currentUser.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::initializePaths() - Expected home: %s\n", expectedHome.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::initializePaths() - Actual home: %s\n", homeDir.toUtf8().constData());
    
    // If the home directory looks like a hardcoded build-time path, try to fix it
    QStringList homeParts = homeDir.split(QString::fromUtf8("/"));
    if (homeParts.size() >= 3 && homeParts[1] == QString::fromUtf8("home") && 
        homeParts[2] != currentUser && !currentUser.isEmpty()) {
        ltr_int_log_message("SteamIntegration::initializePaths() - WARNING: Detected hardcoded build-time path!\n");
        ltr_int_log_message("SteamIntegration::initializePaths() - Attempting to fix by using current user's home\n");
        homeDir = expectedHome;
    }
    
    // Try to find Steam installation
    QStringList steamPaths = {
        homeDir + QStringLiteral("/.steam/steam/"),
        homeDir + QStringLiteral("/.local/share/Steam/"),
        QStringLiteral("/usr/share/steam/")
    };
    
    for (const QString& path : steamPaths) {
        if (QDir(path).exists() && QFile(path + QStringLiteral("steam")).exists()) {
            steamPath = path;
            steamAppsPath = path + QStringLiteral("steamapps/");
            compatDataPath = path + QStringLiteral("steamapps/compatdata/");
            break;
        }
    }
    
    // Debug logging
    ltr_int_log_message("SteamIntegration::initializePaths() - Final home directory: %s\n", homeDir.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::initializePaths() - Steam path: %s\n", steamPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::initializePaths() - Steam apps path: %s\n", steamAppsPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::initializePaths() - Compat data path: %s\n", compatDataPath.toUtf8().constData());
    
    return !steamPath.isEmpty();
}

QString SteamIntegration::getHomeDirectory()
{
    // CRITICAL: Use environment variables first to avoid build-time path capture
    QString homeDir = QString::fromUtf8(qgetenv("HOME"));
    
    // Debug logging
    ltr_int_log_message("SteamIntegration::getHomeDirectory() - HOME env var: %s\n", homeDir.toUtf8().constData());
    
    // Fallback to QStandardPaths only if environment variable is empty
    if (homeDir.isEmpty()) {
        homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        ltr_int_log_message("SteamIntegration::getHomeDirectory() - QStandardPaths fallback: %s\n", homeDir.toUtf8().constData());
    }
    
    // Additional fallback using USER environment variable
    if (homeDir.isEmpty()) {
        QString user = QString::fromUtf8(qgetenv("USER"));
        if (!user.isEmpty()) {
            homeDir = QString::fromUtf8("/home/") + user;
            ltr_int_log_message("SteamIntegration::getHomeDirectory() - Using /home/$USER: %s\n", homeDir.toUtf8().constData());
        }
    }
    
    return homeDir;
}

bool SteamIntegration::isSteamInstalled()
{
    return !steamPath.isEmpty() && QDir(steamPath).exists();
}

QString SteamIntegration::getSteamPath()
{
    return steamPath;
}

QString SteamIntegration::getSteamAppsPath()
{
    return steamAppsPath;
}

QString SteamIntegration::getCompatDataPath()
{
    return compatDataPath;
}

bool SteamIntegration::parseLibraryFolders()
{
    if (steamPath.isEmpty()) {
        setLastError(QStringLiteral("Steam path not found"));
        return false;
    }
    
    QString libraryFoldersPath = steamPath + QStringLiteral("steamapps/libraryfolders.vdf");
    QFile file(libraryFoldersPath);
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setLastError(QStringLiteral("Could not open libraryfolders.vdf: ") + file.errorString());
        return false;
    }
    
    libraryPaths.clear();
    gameToLibraryMap.clear();
    
    // Add default library
    libraryPaths.append(steamAppsPath);
    
    QTextStream in(&file);
    QString line;
    bool inLibrary = false;
    QString currentLibraryPath;
    QString currentLibraryId;
    
    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        
        // Check if we're entering a library section (only numeric library IDs)
        if (QRegularExpression(QStringLiteral("\"\\d+\"")).match(line).hasMatch()) {
            QString libraryId = line.remove(QRegularExpression(QStringLiteral("[\"{}]"))).trimmed();
            bool ok;
            libraryId.toInt(&ok);
            if (ok) {
                inLibrary = true;
                currentLibraryId = libraryId;
                currentLibraryPath.clear();
                ltr_int_log_message("SteamIntegration::parseLibraryFolders() - Entering library section: %s\n", currentLibraryId.toUtf8().constData());
            }
        }
        // Check if we're exiting a library section
        else if (line == QStringLiteral("}") && inLibrary) {
            inLibrary = false;
            if (!currentLibraryPath.isEmpty()) {
                libraryPaths.append(currentLibraryPath + QStringLiteral("/steamapps/"));
                ltr_int_log_message("SteamIntegration::parseLibraryFolders() - Found library: %s\n", currentLibraryPath.toUtf8().constData());
            }
        }
        // Parse library path
        else if (inLibrary && line.startsWith(QStringLiteral("\"path\""))) {
            currentLibraryPath = extractVDFValue(line);
            ltr_int_log_message("SteamIntegration::parseLibraryFolders() - Raw line: '%s'\n", line.toUtf8().constData());
            ltr_int_log_message("SteamIntegration::parseLibraryFolders() - Library path: %s\n", currentLibraryPath.toUtf8().constData());
        }
        // Parse apps in this library
        else if (inLibrary && line.startsWith(QStringLiteral("\"apps\""))) {
            // Apps section starts, we'll parse game IDs in the next lines
            ltr_int_log_message("SteamIntegration::parseLibraryFolders() - Entering apps section\n");
        }
        // Skip game ID parsing from VDF - we'll scan for manifest files instead
    }
    
    file.close();
    
    ltr_int_log_message("SteamIntegration::parseLibraryFolders() - Found %d libraries\n", libraryPaths.size());
    
    // Now scan each library for manifest files to find games
    scanLibrariesForGames();
    
    ltr_int_log_message("SteamIntegration::parseLibraryFolders() - Found %d games\n", gameToLibraryMap.size());
    
    return true;
}

QStringList SteamIntegration::getLibraryPaths()
{
    return libraryPaths;
}

void SteamIntegration::scanLibrariesForGames()
{
    gameToLibraryMap.clear();
    
    for (const QString& libraryPath : libraryPaths) {
        QDir libraryDir(libraryPath);
        if (!libraryDir.exists()) {
            ltr_int_log_message("SteamIntegration::scanLibrariesForGames() - Library path does not exist: %s\n", libraryPath.toUtf8().constData());
            continue;
        }
        
        // Look for appmanifest_*.acf files
        QStringList filters;
        filters << QStringLiteral("appmanifest_*.acf");
        QFileInfoList manifestFiles = libraryDir.entryInfoList(filters, QDir::Files);
        
        ltr_int_log_message("SteamIntegration::scanLibrariesForGames() - Found %d manifest files in %s\n", 
            manifestFiles.size(), libraryPath.toUtf8().constData());
        
        for (const QFileInfo& manifestFile : manifestFiles) {
            QString fileName = manifestFile.fileName();
            // Extract game ID from filename: appmanifest_123456.acf -> 123456
            QString gameId = fileName.mid(12, fileName.length() - 16); // Remove "appmanifest_" and ".acf"
            
            bool ok;
            int gameIdInt = gameId.toInt(&ok);
            if (ok && gameIdInt > 0) {
                gameToLibraryMap[gameId] = libraryPath;
                ltr_int_log_message("SteamIntegration::scanLibrariesForGames() - Found game %s in library: %s\n", 
                    gameId.toUtf8().constData(), libraryPath.toUtf8().constData());
            }
        }
    }
}

QString SteamIntegration::findGameLibrary(const QString &gameId)
{
    if (gameToLibraryMap.contains(gameId)) {
        return gameToLibraryMap[gameId];
    }
    
    // Fallback: search all libraries for the game
    for (const QString& libraryPath : libraryPaths) {
        QString manifestPath = libraryPath + QStringLiteral("appmanifest_") + gameId + QStringLiteral(".acf");
        if (QFile::exists(manifestPath)) {
            return libraryPath;
        }
    }
    
    return QString();
}

QString SteamIntegration::findProtonPrefix(const QString &gameId)
{
    // Search ALL Steam libraries for the Proton prefix (like Protontricks does)
    // The game and its Proton prefix can be in different libraries
    QStringList candidates;
    
    for (const QString& libraryPath : libraryPaths) {
        // compatdata is inside steamapps directory, not at library root
        QString compatDataPath = libraryPath + QStringLiteral("compatdata/");
        QString prefixPath = compatDataPath + gameId + QStringLiteral("/pfx");
        
        ltr_int_log_message("SteamIntegration::findProtonPrefix() - Checking library: %s\n", 
            libraryPath.toUtf8().constData());
        ltr_int_log_message("SteamIntegration::findProtonPrefix() - Prefix path: %s\n", 
            prefixPath.toUtf8().constData());
        
        // Check if the directory exists
        QDir prefixDir(prefixPath);
        if (prefixDir.exists()) {
            ltr_int_log_message("SteamIntegration::findProtonPrefix() - Found prefix in library: %s\n", 
                libraryPath.toUtf8().constData());
            candidates.append(prefixPath);
        }
    }
    
    if (candidates.isEmpty()) {
        ltr_int_log_message("SteamIntegration::findProtonPrefix() - No Proton prefix found for game %s\n", 
            gameId.toUtf8().constData());
        return QString();
    }
    
    // If multiple candidates found, return the first one (could be enhanced with modification time)
    ltr_int_log_message("SteamIntegration::findProtonPrefix() - Found %d prefix candidates, using: %s\n", 
        candidates.size(), candidates.first().toUtf8().constData());
    return candidates.first();
}

QString SteamIntegration::findProtonPrefixInLibrary(const QString &gameId, const QString &libraryPath)
{
    // This function now just searches the specific library (for backward compatibility)
    // compatdata is inside steamapps directory, not at library root
    QString compatDataPath = libraryPath + QStringLiteral("compatdata/");
    QString prefixPath = compatDataPath + gameId + QStringLiteral("/pfx");
    
    ltr_int_log_message("SteamIntegration::findProtonPrefixInLibrary() - Game: %s, Library: %s\n", 
        gameId.toUtf8().constData(), libraryPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::findProtonPrefixInLibrary() - Compat data path: %s\n", 
        compatDataPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::findProtonPrefixInLibrary() - Prefix path: %s\n", 
        prefixPath.toUtf8().constData());
    
    // Check if the directory exists
    QDir prefixDir(prefixPath);
    bool exists = prefixDir.exists();
    ltr_int_log_message("SteamIntegration::findProtonPrefixInLibrary() - Directory exists: %s\n", 
        exists ? "YES" : "NO");
    
    if (exists) {
        ltr_int_log_message("SteamIntegration::findProtonPrefixInLibrary() - Prefix found!\n");
        return prefixPath;
    }
    
    ltr_int_log_message("SteamIntegration::findProtonPrefixInLibrary() - Prefix not found\n");
    return QString();
}

QList<SteamGame> SteamIntegration::getSteamGames()
{
    QList<SteamGame> allGames;
    
    for (const QString& libraryPath : libraryPaths) {
        QList<SteamGame> libraryGames = scanLibraryForGames(libraryPath);
        allGames.append(libraryGames);
    }
    
    // Sort games alphabetically by name
    std::sort(allGames.begin(), allGames.end(), [](const SteamGame &a, const SteamGame &b) {
        QString nameA = a.game_name.isEmpty() ? a.game_id : a.game_name;
        QString nameB = b.game_name.isEmpty() ? b.game_id : b.game_name;
        return nameA.compare(nameB, Qt::CaseInsensitive) < 0;
    });
    
    ltr_int_log_message("SteamIntegration::getSteamGames() - Found %d total games\n", allGames.size());
    return allGames;
}

QList<SteamGame> SteamIntegration::scanLibraryForGames(const QString &libraryPath)
{
    QList<SteamGame> games;
    QDir libraryDir(libraryPath);
    
    if (!libraryDir.exists()) {
        ltr_int_log_message("SteamIntegration::scanLibraryForGames() - Library path does not exist: %s\n", 
            libraryPath.toUtf8().constData());
        return games;
    }
    
    // Find all appmanifest_*.acf files
    QStringList filters;
    filters << QStringLiteral("appmanifest_*.acf");
    QStringList manifestFiles = libraryDir.entryList(filters, QDir::Files);
    
    ltr_int_log_message("SteamIntegration::scanLibraryForGames() - Found %d manifest files in %s\n", 
        manifestFiles.size(), libraryPath.toUtf8().constData());
    
    for (const QString& manifestFile : manifestFiles) {
        QString manifestPath = libraryPath + manifestFile;
        SteamGame game;
        
        if (parseAppManifest(manifestPath, game)) {
            // Set library path
            game.library_path = libraryPath;
            
            // Find Proton prefix (search all libraries)
            if (!game.game_id.isEmpty()) {
                game.prefix_path = findProtonPrefix(game.game_id);
            }
            
            games.append(game);
            
            ltr_int_log_message("SteamIntegration::scanLibraryForGames() - Game: %s (%s) - Prefix: %s\n", 
                game.game_name.toUtf8().constData(), 
                game.game_id.toUtf8().constData(),
                game.prefix_path.isEmpty() ? "None" : game.prefix_path.toUtf8().constData());
        }
    }
    
    return games;
}

bool SteamIntegration::parseAppManifest(const QString &manifestPath, SteamGame &game)
{
    QFile file(manifestPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setLastError(QStringLiteral("Could not open manifest file: ") + file.errorString());
        return false;
    }
    
    QTextStream in(&file);
    QString line;
    bool inAppState = false;
    
    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        
        if (line == QStringLiteral("\"AppState\"")) {
            inAppState = true;
        } else if (line == QStringLiteral("}") && inAppState) {
            break;
        } else if (inAppState) {
            if (line.startsWith(QStringLiteral("\"appid\""))) {
                game.game_id = extractVDFValue(line);
            } else if (line.startsWith(QStringLiteral("\"name\""))) {
                game.game_name = extractVDFValue(line);
            } else if (line.startsWith(QStringLiteral("\"StateFlags\""))) {
                int stateFlags = extractVDFValue(line).toInt();
                game.is_installed = (stateFlags & 4) != 0; // 4 = installed
            } else if (line.startsWith(QStringLiteral("\"installdir\""))) {
                game.app_manifest_path = manifestPath;
            }
        }
    }
    
    file.close();
    return !game.game_id.isEmpty() && game.is_installed;
}

QString SteamIntegration::extractVDFValue(const QString &line)
{
    // For VDF format like: "key" "value"
    // Find all quotes in the line
    QList<int> quotePositions;
    for (int i = 0; i < line.length(); i++) {
        if (line.at(i) == QLatin1Char('\"')) {
            quotePositions.append(i);
        }
    }
    
    // We need at least 4 quotes: "key" "value"
    if (quotePositions.size() < 4) {
        return QString();
    }
    
    // The value is between the 3rd and 4th quotes
    int startQuote = quotePositions[2];
    int endQuote = quotePositions[3];
    QString value = line.mid(startQuote + 1, endQuote - startQuote - 1);
    
    return value;
}

QStringList SteamIntegration::getAvailableGames()
{
    QList<SteamGame> games = getSteamGames();
    QStringList gameNames;
    
    for (const SteamGame& game : games) {
        if (!game.game_name.isEmpty()) {
            gameNames.append(game.game_name);
        }
    }
    
    return gameNames;
}

bool SteamIntegration::installToSteamGame(const QString &gameId)
{
    // Find the Proton prefix
    QString prefixPath = findProtonPrefix(gameId);
    if (prefixPath.isEmpty()) {
        setLastError(QStringLiteral("No Proton prefix found for game: ") + gameId);
        return false;
    }
    
    // Get Proton version
    QString protonVersion = findProtonVersion(gameId);
    QString protonPath = getProtonPath(protonVersion);
    
    if (protonPath.isEmpty()) {
        setLastError(QStringLiteral("Could not find Proton installation for version: ") + protonVersion);
        return false;
    }
    
    // Set up environment variables
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QStringLiteral("WINEPREFIX"), prefixPath);
    env.insert(QStringLiteral("STEAM_COMPAT_DATA_PATH"), getCompatDataPath() + gameId);
    env.insert(QStringLiteral("STEAM_COMPAT_CLIENT_INSTALL_PATH"), steamPath);
    
    // Launch NSIS installer with Proton
    return runWineBridgeInstallerWithProton(prefixPath, protonPath, env);
}

QString SteamIntegration::findProtonVersion(const QString &gameId)
{
    // Find the Proton prefix for this game
    QString prefixPath = findProtonPrefix(gameId);
    if (prefixPath.isEmpty()) {
        ltr_int_log_message("SteamIntegration::findProtonVersion() - No prefix found for game %s\n", 
            gameId.toUtf8().constData());
        return QString();
    }
    
    // Read the config_info file to get the Proton version
    QString configInfoPath = prefixPath;
    configInfoPath.remove(QStringLiteral("/pfx"));
    configInfoPath += QStringLiteral("/config_info");
    
    QFile configFile(configInfoPath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ltr_int_log_message("SteamIntegration::findProtonVersion() - Could not open config_info: %s\n", 
            configInfoPath.toUtf8().constData());
        return QString();
    }
    
    QTextStream in(&configFile);
    QString line = in.readLine(); // First line contains the version
    configFile.close();
    
    if (line.isEmpty()) {
        ltr_int_log_message("SteamIntegration::findProtonVersion() - Empty version in config_info\n");
        return QString();
    }
    
    // Parse the version to determine the Proton installation name
    // Version format: "10.0-200" -> "Proton - Experimental"
    // Version format: "8.0-5" -> "Proton 8.0"
    QString protonName;
    ltr_int_log_message("SteamIntegration::findProtonVersion() - Raw version line: '%s'\n", 
        line.toUtf8().constData());
    
    if (line.startsWith(QStringLiteral("10.0-"))) {
        protonName = QStringLiteral("Proton - Experimental");
        ltr_int_log_message("SteamIntegration::findProtonVersion() - Mapped to Proton - Experimental\n");
    } else if (line.startsWith(QStringLiteral("9.0-"))) {
        // Check if Proton 9.0 (Beta) exists, otherwise fall back to path extraction
        bool foundBeta = false;
        for (const QString& libraryPath : libraryPaths) {
            QString libraryRoot = libraryPath;
            libraryRoot.remove(QStringLiteral("/steamapps/"));
            QString betaPath = libraryRoot + QStringLiteral("/steamapps/common/Proton 9.0 (Beta)");
            if (QDir(betaPath).exists()) {
                protonName = QStringLiteral("Proton 9.0 (Beta)");
                foundBeta = true;
                ltr_int_log_message("SteamIntegration::findProtonVersion() - Found Proton 9.0 (Beta)\n");
                break;
            }
        }
        if (!foundBeta) {
            protonName = QStringLiteral("Proton 9.0");
            ltr_int_log_message("SteamIntegration::findProtonVersion() - Mapped to Proton 9.0 (Beta not found)\n");
        }
    } else if (line.startsWith(QStringLiteral("8.0-"))) {
        protonName = QStringLiteral("Proton 8.0");
        ltr_int_log_message("SteamIntegration::findProtonVersion() - Mapped to Proton 8.0\n");
    } else if (line.startsWith(QStringLiteral("7.0-"))) {
        protonName = QStringLiteral("Proton 7.0");
        ltr_int_log_message("SteamIntegration::findProtonVersion() - Mapped to Proton 7.0\n");
    } else {
        ltr_int_log_message("SteamIntegration::findProtonVersion() - Version not recognized, trying path extraction\n");
        // Try to find the actual Proton installation by checking the config_info
        // The second line contains the Proton files path
        configFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QStringList lines = QString::fromUtf8(configFile.readAll()).split(QStringLiteral("\n"));
        configFile.close();
        
        if (lines.size() > 1) {
            QString protonPath = lines[1];
            ltr_int_log_message("SteamIntegration::findProtonVersion() - Proton path from config: %s\n", 
                protonPath.toUtf8().constData());
            // Extract the Proton name from the path
            // Example: /media/Fast/Steam_Linux/steamapps/common/Proton - Experimental/files/
            QRegularExpression regex(QStringLiteral(".*/common/([^/]+)/files/"));
            QRegularExpressionMatch match = regex.match(protonPath);
            if (match.hasMatch()) {
                protonName = match.captured(1);
                ltr_int_log_message("SteamIntegration::findProtonVersion() - Extracted Proton name: %s\n", 
                    protonName.toUtf8().constData());
            } else {
                ltr_int_log_message("SteamIntegration::findProtonVersion() - Failed to extract Proton name from path\n");
            }
        } else {
            ltr_int_log_message("SteamIntegration::findProtonVersion() - No second line in config_info\n");
        }
    }
    
    ltr_int_log_message("SteamIntegration::findProtonVersion() - Game %s uses %s (version %s)\n", 
        gameId.toUtf8().constData(), protonName.toUtf8().constData(), line.toUtf8().constData());
    
    return protonName;
}

QString SteamIntegration::getProtonPath(const QString &protonVersion)
{
    // Search all Steam libraries for the Proton installation
    for (const QString& libraryPath : libraryPaths) {
        QString libraryRoot = libraryPath;
        libraryRoot.remove(QStringLiteral("/steamapps/"));
        QString protonPath = libraryRoot + QStringLiteral("/steamapps/common/") + protonVersion;
        
        ltr_int_log_message("SteamIntegration::getProtonPath() - Checking: %s\n", 
            protonPath.toUtf8().constData());
        
        if (QDir(protonPath).exists()) {
            ltr_int_log_message("SteamIntegration::getProtonPath() - Found Proton at: %s\n", 
                protonPath.toUtf8().constData());
            return protonPath;
        }
    }
    
    ltr_int_log_message("SteamIntegration::getProtonPath() - Proton %s not found in any library\n", 
        protonVersion.toUtf8().constData());
    return QString();
}

bool SteamIntegration::runWineBridgeInstallerWithProton(
    const QString &prefixPath, 
    const QString &protonPath, 
    const QProcessEnvironment &env)
{
    QProcess process;
    process.setProcessEnvironment(env);
    
    // Set working directory to the Proton prefix root (like Wstart example)
    process.setWorkingDirectory(prefixPath);
    
    // Get wine bridge installer path
    QString installerPath = getWineBridgeInstallerPath();
    if (installerPath.isEmpty()) {
        setLastError(QStringLiteral("Could not find wine bridge installer"));
        return false;
    }
    
    // Set environment variables for Proton (following Wstart example)
    QProcessEnvironment processEnv = QProcessEnvironment::systemEnvironment();
    // STEAM_COMPAT_DATA_PATH should point to the compatdata/[GAME_ID] directory (without /pfx)
    QString compatDataPath = prefixPath;
    compatDataPath.chop(4); // Remove "/pfx" from the end
    processEnv.insert(QStringLiteral("STEAM_COMPAT_DATA_PATH"), compatDataPath);
    processEnv.insert(QStringLiteral("WINEPREFIX"), prefixPath);
    processEnv.insert(QStringLiteral("STEAM_COMPAT_CLIENT_INSTALL_PATH"), steamPath);
    process.setProcessEnvironment(processEnv);
    
    // Build command: proton run [installer_path] (following Wstart example)
    QStringList arguments;
    arguments << QStringLiteral("run") << installerPath;
    
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - Proton path: %s\n", 
        protonPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - Prefix path: %s\n", 
        prefixPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - Working directory: %s\n", 
        prefixPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - Installer path: %s\n", 
        installerPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - STEAM_COMPAT_DATA_PATH: %s\n", 
        compatDataPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - WINEPREFIX: %s\n", 
        prefixPath.toUtf8().constData());
    
    process.start(protonPath + QStringLiteral("/proton"), arguments);
    
    if (!process.waitForStarted()) {
        setLastError(QStringLiteral("Failed to start Proton process: ") + process.errorString());
        return false;
    }
    
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - Proton process started successfully\n");
    
    // Wait for the process to finish (with timeout)
    if (!process.waitForFinished(30000)) { // 30 second timeout
        setLastError(QStringLiteral("Proton process timed out after 30 seconds"));
        process.kill();
        return false;
    }
    
    int exitCode = process.exitCode();
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - Proton process finished with exit code: %d\n", exitCode);
    
    if (exitCode != 0) {
        QString errorOutput = QString::fromUtf8(process.readAllStandardError());
        setLastError(QStringLiteral("Proton process failed with exit code ") + QString::number(exitCode) + 
                    QStringLiteral(": ") + errorOutput);
        return false;
    }
    
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - Wine Bridge installer completed successfully\n");
    return true;
}

QString SteamIntegration::getWineBridgeInstallerPath()
{
    // TODO: Implement proper wine bridge installer path detection
    // For now, return a placeholder
    return QStringLiteral("/usr/local/share/linuxtrack/linuxtrack-wine.exe");
}

QString SteamIntegration::getLastError() const
{
    return lastError;
}

void SteamIntegration::setLastError(const QString &error)
{
    lastError = error;
    ltr_int_log_message("SteamIntegration::setLastError() - %s\n", error.toUtf8().constData());
}

QString SteamIntegration::getDebugInfo() const
{
    QString info;
    info += QStringLiteral("Steam Path: ") + steamPath + QStringLiteral("\n");
    info += QStringLiteral("Steam Apps Path: ") + steamAppsPath + QStringLiteral("\n");
    info += QStringLiteral("Compat Data Path: ") + compatDataPath + QStringLiteral("\n");
    info += QStringLiteral("Library Paths:\n");
    
    for (const QString& path : libraryPaths) {
        info += QStringLiteral("  - ") + path + QStringLiteral("\n");
    }
    
    info += QStringLiteral("Game to Library Map:\n");
    for (auto it = gameToLibraryMap.begin(); it != gameToLibraryMap.end(); ++it) {
        info += QStringLiteral("  ") + it.key() + QStringLiteral(" -> ") + it.value() + QStringLiteral("\n");
    }
    
    // Add detailed Proton prefix detection info
    info += QStringLiteral("\nProton Prefix Detection Details:\n");
    for (auto it = gameToLibraryMap.begin(); it != gameToLibraryMap.end(); ++it) {
        QString gameId = it.key();
        QString libraryPath = it.value();
        
        // Construct the same paths as findProtonPrefix (updated)
        QString compatDataPath = libraryPath + QStringLiteral("compatdata/");
        QString prefixPath = compatDataPath + gameId + QStringLiteral("/pfx");
        
        QDir prefixDir(prefixPath);
        bool exists = prefixDir.exists();
        
        info += QStringLiteral("  Game ") + gameId + QStringLiteral(":\n");
        info += QStringLiteral("    Library: ") + libraryPath + QStringLiteral("\n");
        info += QStringLiteral("    Compat Data: ") + compatDataPath + QStringLiteral("\n");
        info += QStringLiteral("    Prefix Path: ") + prefixPath + QStringLiteral("\n");
        info += QStringLiteral("    Exists: ") + (exists ? QStringLiteral("YES") : QStringLiteral("NO")) + QStringLiteral("\n");
        
        // Also check if the compatdata directory itself exists
        QDir compatDir(compatDataPath);
        bool compatExists = compatDir.exists();
        info += QStringLiteral("    Compat Data Dir Exists: ") + (compatExists ? QStringLiteral("YES") : QStringLiteral("NO")) + QStringLiteral("\n");
    }
    
    return info;
}

 