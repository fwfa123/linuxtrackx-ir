#include "steam_integration.h"
#include <QDebug>
#include <QProcess>
#include <QMessageBox>
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
        
        // Check if we're entering a library section
        if (QRegularExpression(QStringLiteral("\"\\d+\"")).match(line).hasMatch()) {
            inLibrary = true;
            currentLibraryId = line.remove(QRegularExpression(QStringLiteral("[\"{}]")));
            currentLibraryPath.clear();
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
            ltr_int_log_message("SteamIntegration::parseLibraryFolders() - Library path: %s\n", currentLibraryPath.toUtf8().constData());
        }
        // Parse apps in this library
        else if (inLibrary && line.startsWith(QStringLiteral("\"apps\""))) {
            // Apps section starts, we'll parse game IDs in the next lines
        }
        else if (inLibrary && line.contains(QStringLiteral("\"")) && !line.startsWith(QStringLiteral("\"path\"")) && !line.startsWith(QStringLiteral("\"apps\""))) {
            // This is a game ID line in the apps section
            QString gameId = line.remove(QRegularExpression(QStringLiteral("[\"{}]"))).trimmed();
            if (!gameId.isEmpty() && !currentLibraryPath.isEmpty()) {
                gameToLibraryMap[gameId] = currentLibraryPath + QStringLiteral("/steamapps/");
                ltr_int_log_message("SteamIntegration::parseLibraryFolders() - Game %s in library: %s\n", 
                    gameId.toUtf8().constData(), currentLibraryPath.toUtf8().constData());
            }
        }
    }
    
    file.close();
    
    ltr_int_log_message("SteamIntegration::parseLibraryFolders() - Found %d libraries\n", libraryPaths.size());
    ltr_int_log_message("SteamIntegration::parseLibraryFolders() - Found %d games\n", gameToLibraryMap.size());
    
    return true;
}

QStringList SteamIntegration::getLibraryPaths()
{
    return libraryPaths;
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
    QString libraryPath = findGameLibrary(gameId);
    if (!libraryPath.isEmpty()) {
        return findProtonPrefixInLibrary(gameId, libraryPath);
    }
    
    return QString();
}

QString SteamIntegration::findProtonPrefixInLibrary(const QString &gameId, const QString &libraryPath)
{
    // Construct compatdata path relative to library
    QString libraryRoot = libraryPath;
    libraryRoot.remove(QStringLiteral("/steamapps/"));
    QString compatDataPath = libraryRoot + QStringLiteral("/compatdata/");
    QString prefixPath = compatDataPath + gameId + QStringLiteral("/pfx");
    
    ltr_int_log_message("SteamIntegration::findProtonPrefixInLibrary() - Game: %s, Library: %s\n", 
        gameId.toUtf8().constData(), libraryPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::findProtonPrefixInLibrary() - Compat data path: %s\n", 
        compatDataPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::findProtonPrefixInLibrary() - Prefix path: %s\n", 
        prefixPath.toUtf8().constData());
    
    if (QDir(prefixPath).exists()) {
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
            
            // Find Proton prefix
            if (!game.game_id.isEmpty()) {
                game.prefix_path = findProtonPrefixInLibrary(game.game_id, libraryPath);
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
    // Extract value from VDF format: "key" "value"
    int firstQuote = line.indexOf(QStringLiteral("\""), 1);
    int secondQuote = line.indexOf(QStringLiteral("\""), firstQuote + 1);
    
    if (firstQuote != -1 && secondQuote != -1) {
        return line.mid(firstQuote + 1, secondQuote - firstQuote - 1);
    }
    
    return QString();
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
    // For now, return a default version
    // TODO: Implement proper Proton version detection
    return QStringLiteral("Proton 8.0");
}

QString SteamIntegration::getProtonPath(const QString &protonVersion)
{
    QString steamPath = getSteamPath();
    QString protonPath = steamPath + QStringLiteral("steamapps/common/") + protonVersion;
    
    if (QDir(protonPath).exists()) {
        return protonPath;
    }
    
    return QString();
}

bool SteamIntegration::runWineBridgeInstallerWithProton(
    const QString &prefixPath, 
    const QString &protonPath, 
    const QProcessEnvironment &env)
{
    QProcess process;
    process.setProcessEnvironment(env);
    
    // Change to prefix directory
    process.setWorkingDirectory(prefixPath);
    
    // Get wine bridge installer path
    QString installerPath = getWineBridgeInstallerPath();
    if (installerPath.isEmpty()) {
        setLastError(QStringLiteral("Could not find wine bridge installer"));
        return false;
    }
    
    // Build command: proton run [installer_path]
    QStringList arguments;
    arguments << QStringLiteral("run") << installerPath;
    
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - Proton path: %s\n", 
        protonPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - Prefix path: %s\n", 
        prefixPath.toUtf8().constData());
    ltr_int_log_message("SteamIntegration::runWineBridgeInstallerWithProton() - Installer path: %s\n", 
        installerPath.toUtf8().constData());
    
    process.start(protonPath + QStringLiteral("/proton"), arguments);
    
    if (!process.waitForStarted()) {
        setLastError(QStringLiteral("Failed to start Proton process: ") + process.errorString());
        return false;
    }
    
    return true;
}

QString SteamIntegration::getWineBridgeInstallerPath()
{
    // TODO: Implement proper wine bridge installer path detection
    // For now, return a placeholder
    return QStringLiteral("/usr/share/linuxtrack/wine_bridge/LinuxtrackWineBridge.exe");
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
    
    return info;
}

 