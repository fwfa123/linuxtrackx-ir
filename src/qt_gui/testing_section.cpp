#include "testing_section.h"
#include "ltr_gui.h"
#include "plugin_install.h"
#include "steam_integration.h"
#include "lutris_integration.h"
#include "tracker.h"
#include "ltr_state.h"
#include <linuxtrack.h>
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

TestingSection::TestingSection(QObject *parent)
    : QObject(parent)
    , testingGroupBox(nullptr)
    , testerExeRadioButton(nullptr)
    , ftTesterRadioButton(nullptr)
    , platformComboBox(nullptr)
    , loadGamesButton(nullptr)
    , gameComboBox(nullptr)
    , runTesterButton(nullptr)
    , currentPlatform(QString::fromUtf8(""))
    , currentGame(QString::fromUtf8(""))
    , currentTesterType(QString::fromUtf8("Tester.exe"))
    , currentGames()
    , steamIntegration(nullptr)
    , lutrisIntegration(nullptr)
    , trackingStarted(false)
{
}

TestingSection::~TestingSection()
{
}

void TestingSection::setupUI(Ui::LinuxtrackMainForm &ui)
{
    // Connect UI elements to member variables
    testingGroupBox = ui.TestingGroupBox;
    testerExeRadioButton = ui.TesterExeRadioButton;
    ftTesterRadioButton = ui.FTTesterRadioButton;
    platformComboBox = ui.PlatformComboBox;
    loadGamesButton = ui.LoadGamesButton;
    gameComboBox = ui.GameComboBox;
    runTesterButton = ui.RunTesterButton;
    
    // Initialize integration objects
    steamIntegration = new SteamIntegration(this);
    lutrisIntegration = new LutrisIntegration(this);
    
    // Set initial state
    if (gameComboBox) {
        gameComboBox->setEnabled(false);
    }
    if (runTesterButton) {
        runTesterButton->setEnabled(false);
    }

    // Listen to tracker state changes to reset guard when tracking stops
    QObject::connect(&STATE, SIGNAL(stateChanged(linuxtrack_state_type)),
                     this, SLOT(onTrackerStateChanged(linuxtrack_state_type)));
}

void TestingSection::onTrackerStateChanged(linuxtrack_state_type current_state)
{
    if (current_state == STOPPED) {
        trackingStarted = false;
    }
}

void TestingSection::startTracking()
{
    // Start tracking immediately when testing workflow begins (once)
    if (trackingStarted) {
        return;
    }
    qDebug() << "Starting tracking for testing workflow";

    // Reuse existing start logic used elsewhere in the GUI
    static QString sec(QString::fromUtf8("Default"));
    TRACKER.start(sec);
    trackingStarted = true;

    // Inform the user once that tracking has been automatically started for testing.
    QMessageBox::information(nullptr, QString::fromUtf8("Tracking Started"),
                             QString::fromUtf8("Head tracking has been automatically started for testing.\n\n"
                                               "Use the tracking window to pause, recenter, or stop tracking as needed."));

    // Notify GUI to sync tracking window UI timers
    emit testingWorkflowStarted();
}

void TestingSection::onTesterSelectionChanged()
{
    if (testerExeRadioButton && testerExeRadioButton->isChecked()) {
        currentTesterType = QString::fromUtf8("Tester.exe");
    } else if (ftTesterRadioButton && ftTesterRadioButton->isChecked()) {
        currentTesterType = QString::fromUtf8("FT_Tester");
    }
    
    // Start tracking when tester selection changes
    startTracking();
    
    qDebug() << "Tester selection changed to:" << currentTesterType;
}

void TestingSection::onPlatformSelectionChanged()
{
    if (platformComboBox) {
        currentPlatform = platformComboBox->currentText();
        qDebug() << "Platform selection changed to:" << currentPlatform;
        
        // Clear games list when platform changes
        if (gameComboBox) {
            gameComboBox->clear();
            gameComboBox->setEnabled(false);
        }
        if (runTesterButton) {
            runTesterButton->setEnabled(false);
        }

        // Begin testing workflow: ensure tracking is started
        startTracking();
    }
}

void TestingSection::onLoadGamesClicked()
{
    if (platformComboBox) {
        // Beginning testing workflow via Load Games should start tracking
        startTracking();
        QString platform = platformComboBox->currentText();
        loadGamesForPlatform(platform);
    }
}

void TestingSection::loadGamesForPlatform(const QString &platform)
{
    currentPlatform = platform;
    currentGames.clear();
    
    if (platform == QString::fromUtf8("Steam")) {
        currentGames = getSteamGames();
    } else if (platform == QString::fromUtf8("Lutris")) {
        currentGames = getLutrisGames();
    } else if (platform == QString::fromUtf8("Custom Prefix")) {
        currentGames = getCustomPrefixGames();
    }
    
    // Populate game combo box
    if (gameComboBox) {
        gameComboBox->clear();
        gameComboBox->addItems(currentGames);
        gameComboBox->setEnabled(!currentGames.isEmpty());
    }
    
    qDebug() << "Loaded" << currentGames.size() << "games for platform:" << platform;
}

QStringList TestingSection::getSteamGames()
{
    QStringList games;
    
    if (!steamIntegration) {
        qDebug() << "Steam integration not initialized";
        return games;
    }
    
    if (!steamIntegration->isSteamInstalled()) {
        qDebug() << "Steam not installed";
        return games;
    }
    
    // Get list of Steam games
    QList<SteamGame> steamGames = steamIntegration->getSteamGames();
    
    for (const SteamGame &game : steamGames) {
        if (game.is_installed) {
            games << game.game_name;
        }
    }
    
    qDebug() << "Found" << games.size() << "Steam games";
    return games;
}

QStringList TestingSection::getLutrisGames()
{
    QStringList games;
    
    if (!lutrisIntegration) {
        qDebug() << "Lutris integration not initialized";
        return games;
    }
    
    if (!lutrisIntegration->isLutrisInstalled()) {
        qDebug() << "Lutris not installed";
        return games;
    }
    
    // Get list of Lutris games
    QList<LutrisGame> lutrisGames = lutrisIntegration->getLutrisGames();
    
    for (const LutrisGame &game : lutrisGames) {
        games << game.game_name;
    }
    
    qDebug() << "Found" << games.size() << "Lutris games";
    return games;
}

QStringList TestingSection::getCustomPrefixGames()
{
    // TODO: Implement custom prefix game discovery
    QStringList games;
    
    // Placeholder implementation
    // In real implementation, this would allow user to browse
    // and select custom wine prefixes
    
    return games;
}

void TestingSection::onRunTesterClicked()
{
    // Ensure tracking is running before launching testers
    startTracking();
    if (gameComboBox && !gameComboBox->currentText().isEmpty()) {
        currentGame = gameComboBox->currentText();
        runSelectedTester();
    }
}

void TestingSection::runSelectedTester()
{
    if (currentGame.isEmpty() || currentPlatform.isEmpty()) {
        QMessageBox::warning(nullptr, QString::fromUtf8("Missing Information"), 
                           QString::fromUtf8("Please select both a game and platform before running tester."));
        return;
    }
    
    // Get prefix for selected game
    QString prefixPath = getPrefixForGame(currentGame, currentPlatform);
    if (prefixPath.isEmpty()) {
        QMessageBox::warning(nullptr, QString::fromUtf8("Prefix Not Found"), 
                           QString::fromUtf8("Could not find wine prefix for selected game."));
        return;
    }
    
    // Check if tester exists in prefix
    if (!checkTesterInPrefix(prefixPath, currentTesterType)) {
        showMissingTesterDialog(prefixPath);
        return;
    }
    
    // Find tester executable
    QString testerPath = findTesterInPrefix(prefixPath, currentTesterType);
    if (testerPath.isEmpty()) {
        QMessageBox::warning(nullptr, QString::fromUtf8("Tester Not Found"), 
                           QString::fromUtf8("Could not find tester executable in wine prefix."));
        return;
    }
    
    // Execute tester
    executeTester(testerPath, prefixPath, currentPlatform);
}

QString TestingSection::getPrefixForGame(const QString &gameName, const QString &platform)
{
    if (platform == QString::fromUtf8("Steam")) {
        if (!steamIntegration) {
            qDebug() << "Steam integration not initialized";
            return QString::fromUtf8("");
        }
        
        // Find the Steam game by name and get its prefix
        QList<SteamGame> steamGames = steamIntegration->getSteamGames();
        for (const SteamGame &game : steamGames) {
            if (game.game_name == gameName && game.is_installed) {
                QString prefixPath = steamIntegration->findProtonPrefix(game.game_id);
                qDebug() << "Found Steam prefix for" << gameName << ":" << prefixPath;
                return prefixPath;
            }
        }
        
        qDebug() << "Steam game not found:" << gameName;
        return QString::fromUtf8("");
        
    } else if (platform == QString::fromUtf8("Lutris")) {
        if (!lutrisIntegration) {
            qDebug() << "Lutris integration not initialized";
            return QString::fromUtf8("");
        }
        
        // Find the Lutris game by name and get its prefix
        QList<LutrisGame> lutrisGames = lutrisIntegration->getLutrisGames();
        for (const LutrisGame &game : lutrisGames) {
            if (game.game_name == gameName) {
                qDebug() << "Found Lutris prefix for" << gameName << ":" << game.wine_prefix;
                return game.wine_prefix;
            }
        }
        
        qDebug() << "Lutris game not found:" << gameName;
        return QString::fromUtf8("");
        
    } else if (platform == QString::fromUtf8("Custom Prefix")) {
        // TODO: Implement custom prefix selection
        // For now, return empty string
        return QString::fromUtf8("");
    }
    
    return QString::fromUtf8("");
}

bool TestingSection::checkTesterInPrefix(const QString &prefixPath, const QString &testerType)
{
    QString testerPath = findTesterInPrefix(prefixPath, testerType);
    return !testerPath.isEmpty();
}

QString TestingSection::findTesterInPrefix(const QString &prefixPath, const QString &testerType)
{
    QDir prefixDir(prefixPath);
    if (!prefixDir.exists()) {
        return QString::fromUtf8("");
    }
    
    QStringList possibleNames;
    if (testerType == QString::fromUtf8("Tester.exe")) {
        possibleNames << QString::fromUtf8("Tester.exe") << QString::fromUtf8("Tester64.exe");
    } else if (testerType == QString::fromUtf8("FT_Tester")) {
        possibleNames << QString::fromUtf8("FT_Tester.exe") << QString::fromUtf8("FreeTrackTester.exe");
    }
    
    // Search in prefix root directory (where NSIS installer may place testers)
    for (const QString &testerName : possibleNames) {
        QString testerPath = prefixDir.filePath(testerName);
        QFileInfo testerFile(testerPath);
        // In Wine prefixes, Windows executables typically lack the Linux executable bit.
        // Treat existence of the file as sufficient.
        if (testerFile.exists() && testerFile.isFile()) {
            return testerPath;
        }
    }
    
    // Search in common Windows program directories
    QStringList searchDirs = {QString::fromUtf8("drive_c/windows"), QString::fromUtf8("drive_c/Program Files"), QString::fromUtf8("drive_c/Program Files (x86)"), QString::fromUtf8("drive_c/Program Files (x86)/Linuxtrack")};
    for (const QString &searchDir : searchDirs) {
        QDir dir = prefixDir;
        dir.cd(searchDir);
        if (dir.exists()) {
            for (const QString &testerName : possibleNames) {
                QString testerPath = dir.filePath(testerName);
                QFileInfo testerFile(testerPath);
                // Do not require the executable bit for .exe files in Wine prefixes
                if (testerFile.exists() && testerFile.isFile()) {
                    qDebug() << "Found tester at:" << testerPath;
                    return testerPath;
                }
            }
        }
    }
    
    return QString::fromUtf8("");
}

void TestingSection::showMissingTesterDialog(const QString &prefixPath)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        nullptr, 
        QString::fromUtf8("Tester Not Found"), 
        QString::fromUtf8("Tester executable not found in wine prefix:\n%1\n\nWould you like to install Wine Bridge now?").arg(prefixPath),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        offerWineBridgeInstallation(prefixPath);
    }
}

void TestingSection::offerWineBridgeInstallation(const QString &prefixPath)
{
    Q_UNUSED(prefixPath);
    qDebug() << "Offering Wine Bridge installation for platform:" << currentPlatform;

    // Acquire PluginInstall from the main GUI and delegate
    LinuxtrackGui *mainGui = qobject_cast<LinuxtrackGui*>(parent());
    if (!mainGui) {
        qDebug() << "TestingSection: unable to access main GUI for installation delegation";
        return;
    }
    PluginInstall *pluginInstall = mainGui->getPluginInstall();
    if (!pluginInstall) {
        qDebug() << "TestingSection: PluginInstall not available";
        return;
    }

    if (currentPlatform == QString::fromUtf8("Steam")) {
        pluginInstall->installSteamProtonBridge();
    } else if (currentPlatform == QString::fromUtf8("Lutris")) {
        pluginInstall->installLutrisWineBridge();
    } else if (currentPlatform == QString::fromUtf8("Custom Prefix")) {
        pluginInstall->installWineBridgeToCustomPrefix();
    } else {
        qDebug() << "TestingSection: Unknown platform for installation:" << currentPlatform;
    }
}

QString TestingSection::getCurrentGameId()
{
    if (currentPlatform != QString::fromUtf8("Steam")) {
        return QString::fromUtf8("");
    }
    
    // Get the selected game name from the combo box
    QString selectedGameName = gameComboBox->currentText();
    if (selectedGameName.isEmpty()) {
        return QString::fromUtf8("");
    }
    
    // Find the game ID by matching the game name
    QList<SteamGame> games = steamIntegration->getSteamGames();
    for (const SteamGame &game : games) {
        if (game.game_name == selectedGameName) {
            return game.game_id;
        }
    }
    
    return QString::fromUtf8("");
}

QString TestingSection::getCurrentGameSlug()
{
    if (currentPlatform != QString::fromUtf8("Lutris")) {
        return QString::fromUtf8("");
    }
    
    // Get the selected game name from the combo box
    QString selectedGameName = gameComboBox->currentText();
    if (selectedGameName.isEmpty()) {
        return QString::fromUtf8("");
    }
    
    // Find the game slug by matching the game name
    QList<LutrisGame> games = lutrisIntegration->getLutrisGames();
    for (const LutrisGame &game : games) {
        if (game.game_name == selectedGameName) {
            return game.game_slug;
        }
    }
    
    return QString::fromUtf8("");
}

void TestingSection::executeTester(const QString &testerPath, const QString &prefixPath, const QString &platform)
{
    if (platform == QString::fromUtf8("Steam")) {
        // Use existing Steam integration logic
        QString gameId = getCurrentGameId();
        if (!gameId.isEmpty()) {
            // Get the Proton path using existing Steam integration
            QString protonVersion = steamIntegration->findProtonVersion(gameId);
            QString protonPath = steamIntegration->getProtonPath(protonVersion);
            
            if (!protonPath.isEmpty()) {
                QString protonBinaryPath = protonPath + QString::fromUtf8("/proton");
                QFileInfo protonBinary(protonBinaryPath);
                
                if (protonBinary.exists() && protonBinary.isExecutable()) {
                    QProcess process;
                    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
                    
                    // Set up environment variables exactly like the existing Steam integration
                    QString compatDataPath = prefixPath;
                    compatDataPath.chop(4); // Remove "/pfx" from the end
                    env.insert(QString::fromUtf8("STEAM_COMPAT_DATA_PATH"), compatDataPath);
                    env.insert(QString::fromUtf8("WINEPREFIX"), prefixPath);
                    env.insert(QString::fromUtf8("STEAM_COMPAT_CLIENT_INSTALL_PATH"), steamIntegration->getSteamPath());
                    
                    process.setProcessEnvironment(env);
                    process.setWorkingDirectory(prefixPath);
                    
                    QStringList arguments;
                    arguments << QString::fromUtf8("run") << testerPath;
                    
                    qDebug() << "Launching tester with Proton:" << protonBinaryPath;
                    qDebug() << "Arguments:" << arguments;
                    qDebug() << "Working directory:" << prefixPath;
                    
                    process.start(protonBinaryPath, arguments);
                    
                    if (process.waitForStarted(5000)) {
                        qDebug() << "Tester launched successfully:" << testerPath;
                        qDebug() << "Process ID:" << process.processId();
                        QMessageBox::information(nullptr, QString::fromUtf8("Tester Launched"), 
                                               QString::fromUtf8("Tester has been launched successfully through Steam Proton."));
                    } else {
                        qDebug() << "Failed to launch tester:" << process.errorString();
                        QMessageBox::warning(nullptr, QString::fromUtf8("Launch Failed"), 
                                           QString::fromUtf8("Failed to launch tester: %1").arg(process.errorString()));
                    }
                } else {
                    qDebug() << "Proton binary not found or not executable:" << protonBinaryPath;
                    QMessageBox::warning(nullptr, QString::fromUtf8("Proton Not Found"), 
                                       QString::fromUtf8("Proton binary not found: %1").arg(protonBinaryPath));
                }
            } else {
                qDebug() << "Proton path not found for version:" << protonVersion;
                QMessageBox::warning(nullptr, QString::fromUtf8("Proton Not Found"), 
                                   QString::fromUtf8("Proton installation not found for version: %1").arg(protonVersion));
            }
        } else {
            qDebug() << "No game ID available for Steam platform";
            QMessageBox::warning(nullptr, QString::fromUtf8("Game Not Selected"), 
                               QString::fromUtf8("Please select a Steam game first."));
        }
    } else if (platform == QString::fromUtf8("Lutris")) {
        // Use existing Lutris integration logic
        QString gameSlug = getCurrentGameSlug();
        if (!gameSlug.isEmpty()) {
            QList<LutrisGame> games = lutrisIntegration->getLutrisGames();
            QString wineVersion;
            
            // Find the wine version for the current game
            for (const LutrisGame &game : games) {
                if (game.game_slug == gameSlug) {
                    wineVersion = game.wine_version;
                    break;
                }
            }
            
            if (!wineVersion.isEmpty()) {
                QString homeDir = QDir::homePath();
                QString winePath = QString::fromUtf8("%1/.local/share/lutris/runners/wine/%2/bin/wine").arg(homeDir, wineVersion);
                QFileInfo wineBinary(winePath);
                
                if (wineBinary.exists() && wineBinary.isExecutable()) {
                    QProcess process;
                    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
                    
                    // Set up environment variables for Lutris
                    env.insert(QString::fromUtf8("WINEPREFIX"), prefixPath);
                    env.insert(QString::fromUtf8("WINEARCH"), QString::fromUtf8("win64"));
                    env.insert(QString::fromUtf8("WINEESYNC"), QString::fromUtf8("1"));
                    
                    process.setProcessEnvironment(env);
                    process.setWorkingDirectory(prefixPath);
                    
                    QStringList arguments;
                    arguments << testerPath;
                    
                    qDebug() << "Launching tester with Lutris Wine:" << winePath;
                    qDebug() << "Wine version:" << wineVersion;
                    qDebug() << "Game slug:" << gameSlug;
                    
                    process.start(winePath, arguments);
                    
                    if (process.waitForStarted(5000)) {
                        qDebug() << "Tester launched successfully:" << testerPath;
                        qDebug() << "Process ID:" << process.processId();
                        QMessageBox::information(nullptr, QString::fromUtf8("Tester Launched"), 
                                               QString::fromUtf8("Tester has been launched successfully through Lutris Wine."));
                    } else {
                        qDebug() << "Failed to launch tester:" << process.errorString();
                        QMessageBox::warning(nullptr, QString::fromUtf8("Launch Failed"), 
                                           QString::fromUtf8("Failed to launch tester: %1").arg(process.errorString()));
                    }
                } else {
                    qDebug() << "Lutris wine binary not found or not executable:" << winePath;
                    QMessageBox::warning(nullptr, QString::fromUtf8("Lutris Wine Not Found"), 
                                       QString::fromUtf8("Lutris wine binary not found: %1").arg(winePath));
                }
            } else {
                qDebug() << "Wine version not found for game:" << gameSlug;
                QMessageBox::warning(nullptr, QString::fromUtf8("Wine Version Not Found"), 
                                   QString::fromUtf8("Wine version not found for game: %1").arg(gameSlug));
            }
        } else {
            qDebug() << "No game slug available for Lutris platform";
            QMessageBox::warning(nullptr, QString::fromUtf8("Game Not Selected"), 
                               QString::fromUtf8("Please select a Lutris game first."));
        }
    } else {
        // Use system wine for Custom Prefix
        QProcess process;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        
        env.insert(QString::fromUtf8("WINEPREFIX"), prefixPath);
        process.setProcessEnvironment(env);
        process.setWorkingDirectory(prefixPath);
        
        QStringList arguments;
        arguments << testerPath;
        
        qDebug() << "Launching tester with system Wine";
        
        process.start(QString::fromUtf8("wine"), arguments);
        
        if (process.waitForStarted(5000)) {
            qDebug() << "Tester launched successfully:" << testerPath;
            qDebug() << "Process ID:" << process.processId();
            QMessageBox::information(nullptr, QString::fromUtf8("Tester Launched"), 
                                   QString::fromUtf8("Tester has been launched successfully through system Wine."));
        } else {
            qDebug() << "Failed to launch tester:" << process.errorString();
            QMessageBox::warning(nullptr, QString::fromUtf8("Launch Failed"), 
                               QString::fromUtf8("Failed to launch tester: %1").arg(process.errorString()));
        }
    }
} 