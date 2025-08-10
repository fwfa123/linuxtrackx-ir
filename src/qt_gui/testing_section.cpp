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
#include <QSettings>
#include <QFileDialog>
#include "tester_launcher.h"
#include <QLineEdit>
#include <algorithm>

// Map user-facing platform labels to stable keys; keep logic decoupled from translation
static QString toPlatformKey(const QString &label)
{
    const QString norm = label.trimmed().toLower();
    if (norm.startsWith(QStringLiteral("steam"))) return QStringLiteral("steam");
    if (norm.startsWith(QStringLiteral("lutris"))) return QStringLiteral("lutris");
    if (norm.contains(QStringLiteral("custom"))) return QStringLiteral("custom");
    return QStringLiteral("");
}

TestingSection::TestingSection(QObject *parent)
    : QObject(parent)
    , testingGroupBox(nullptr)
    , testerExeRadioButton(nullptr)
    , ftTesterRadioButton(nullptr)
    , platformComboBox(nullptr)
    , loadGamesButton(nullptr)
    , gameComboBox(nullptr)
    , runTesterButton(nullptr)
    , testingStatusLabel(nullptr)
    , gameFilterEdit(nullptr)
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
    testingStatusLabel = ui.TestingStatusLabel;
    gameFilterEdit = ui.GameFilterEdit;
    testingProgressBar = ui.TestingProgressBar;
    
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

    // Persist last selections
    QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
    settings.beginGroup(QStringLiteral("TestingSection"));
    const QString lastPlatform = settings.value(QStringLiteral("last_platform"), QStringLiteral("Steam")).toString();
    const QString lastGame = settings.value(QStringLiteral("last_game_%1").arg(lastPlatform), QString()).toString();
    settings.endGroup();

    if (platformComboBox) {
        int idx = platformComboBox->findText(lastPlatform);
        if (idx >= 0) platformComboBox->setCurrentIndex(idx);
    }
    currentPlatform = platformComboBox ? platformComboBox->currentText() : QString();

    if (testingStatusLabel) {
        testingStatusLabel->clear();
    }

    // Listen to tracker state changes to reset guard when tracking stops
    QObject::connect(&STATE, SIGNAL(stateChanged(linuxtrack_state_type)),
                     this, SLOT(onTrackerStateChanged(linuxtrack_state_type)));

    // Filter handler
    if (gameFilterEdit) {
        QObject::connect(gameFilterEdit, &QLineEdit::textChanged,
                         this, &TestingSection::onFilterTextChanged);
    }

    // Async loader completion
    QObject::connect(&gamesLoadWatcher, &QFutureWatcher<QStringList>::finished, [this]() {
        const QStringList loaded = gamesLoadWatcher.result();
        currentGames = loaded;
        // Populate with filter applied
        onFilterTextChanged(gameFilterEdit ? gameFilterEdit->text() : QString());
        // Finalize status/progress only here
        if (testingStatusLabel) {
            if (!currentGames.isEmpty()) {
                testingStatusLabel->setText(QObject::tr("Loaded %1 games").arg(currentGames.size()));
            } else {
                testingStatusLabel->setText(QObject::tr("No games found"));
            }
        }
        if (testingProgressBar) testingProgressBar->setVisible(false);
        // Restore last game selection if present
        QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
        settings.beginGroup(QStringLiteral("TestingSection"));
        const QString lastGame = settings.value(QStringLiteral("last_game_%1").arg(currentPlatform), QString()).toString();
        settings.endGroup();
        if (!lastGame.isEmpty() && gameComboBox) {
            int idx = gameComboBox->findText(lastGame);
            if (idx >= 0) {
                gameComboBox->setCurrentIndex(idx);
                if (runTesterButton) runTesterButton->setEnabled(true);
            }
        }
    });
}
QString TestingSection::getCurrentPlatformKey() const
{
    return toPlatformKey(currentPlatform);
}

QString TestingSection::platformLabelToKey(const QString &label)
{
    return toPlatformKey(label);
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
    QMessageBox::information(nullptr, tr("Tracking Started"),
                             tr("Head tracking has been automatically started for testing.\n\n"
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

        // Auto-load games for the new platform
        if (testingStatusLabel) testingStatusLabel->setText(QObject::tr("Loading..."));
        if (testingProgressBar) testingProgressBar->setVisible(true);
        loadGamesForPlatform(currentPlatform);

        // Persist last platform (store label for UX; logic uses stable key helpers)
        QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
        settings.beginGroup(QStringLiteral("TestingSection"));
        settings.setValue(QStringLiteral("last_platform"), currentPlatform);
        settings.endGroup();
    }
}

void TestingSection::onLoadGamesClicked()
{
    if (platformComboBox) {
        // Beginning testing workflow via Load Games should start tracking
        startTracking();
        QString platform = platformComboBox->currentText();
        if (testingStatusLabel) testingStatusLabel->setText(QObject::tr("Loading..."));
        if (testingProgressBar) testingProgressBar->setVisible(true);
        loadGamesForPlatform(platform);
    }
}

void TestingSection::loadGamesForPlatform(const QString &platform)
{
    currentPlatform = platform;
    currentGames.clear();
    
    const QString platformKey = getCurrentPlatformKey();
    if (platformKey == QStringLiteral("custom")) {
        // Run custom prefix selection synchronously on the UI thread
        const QStringList customGames = getCustomPrefixGames();
        currentGames = customGames;
        onFilterTextChanged(gameFilterEdit ? gameFilterEdit->text() : QString());
        if (testingStatusLabel) {
            testingStatusLabel->setText(customGames.isEmpty() ? QObject::tr("No games found")
                                                              : QObject::tr("Loaded %1 games").arg(customGames.size()));
        }
        if (testingProgressBar) testingProgressBar->setVisible(false);
        return;
    }

    // Thread off Steam/Lutris discovery only
    auto task = QtConcurrent::run([this, platformKey]() -> QStringList {
        if (platformKey == QStringLiteral("steam")) {
            return getSteamGames();
        } else if (platformKey == QStringLiteral("lutris")) {
            return getLutrisGames();
        }
        return QStringList();
    });
    gamesLoadWatcher.setFuture(task);
}

void TestingSection::onFilterTextChanged(const QString &text)
{
    Q_UNUSED(text);
    // Re-populate list from currentGames with filter
    if (!gameComboBox) return;
    gameComboBox->blockSignals(true);
    gameComboBox->clear();
    QStringList toShow = currentGames;
    const QString f = gameFilterEdit ? gameFilterEdit->text().trimmed() : QString();
    if (!f.isEmpty()) {
        toShow.erase(std::remove_if(toShow.begin(), toShow.end(), [&](const QString &name){
            return !name.contains(f, Qt::CaseInsensitive);
        }), toShow.end());
    }
    gameComboBox->addItems(toShow);
    gameComboBox->setEnabled(!toShow.isEmpty());
    gameComboBox->blockSignals(false);
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
    QStringList games;
    // Let user choose a Wine prefix directory and validate minimal structure
    const QString dialogTitle = tr("Select Wine Prefix");
    const QString startDir = customPrefixPath.isEmpty() ? QDir::homePath() : customPrefixPath;
    const QString selectedDir = QFileDialog::getExistingDirectory(nullptr, dialogTitle, startDir);

    if (selectedDir.isEmpty()) {
        // User cancelled; return empty game list
        return games;
    }

    QDir prefixDir(selectedDir);
    QDir checkDir(prefixDir);
    bool isValidPrefix = checkDir.cd(QString::fromUtf8("drive_c")) && checkDir.cd(QString::fromUtf8("windows"));

    if (!isValidPrefix) {
        QMessageBox::warning(nullptr,
                             tr("Invalid Prefix"),
                             tr("The selected directory does not look like a Wine prefix (missing drive_c/windows)."));
        return games;
    }

    customPrefixPath = QDir(selectedDir).absolutePath();

    // Present a single logical entry for the chosen prefix
    games << QString::fromUtf8("Custom Prefix");
    return games;
}

void TestingSection::onRunTesterClicked()
{
    // Ensure tracking is running before launching testers
    startTracking();
    if (gameComboBox && !gameComboBox->currentText().isEmpty()) {
        currentGame = gameComboBox->currentText();
        // Persist last game for current platform
        QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
        settings.beginGroup(QStringLiteral("TestingSection"));
        settings.setValue(QStringLiteral("last_game_%1").arg(currentPlatform), currentGame);
        settings.endGroup();
        runSelectedTester();
    }
}

void TestingSection::onGameSelectionChanged()
{
    if (!gameComboBox) return;
    const bool hasSelection = !gameComboBox->currentText().isEmpty();
    if (runTesterButton) {
        runTesterButton->setEnabled(hasSelection);
    }
}

void TestingSection::runSelectedTester()
{
    if (currentGame.isEmpty() || currentPlatform.isEmpty()) {
        QMessageBox::warning(nullptr, tr("Missing Information"), 
                             tr("Please select both a game and platform before running tester."));
        return;
    }
    
    // Get prefix for selected game
    QString prefixPath = getPrefixForGame(currentGame, currentPlatform);
    if (prefixPath.isEmpty()) {
        QMessageBox::warning(nullptr, tr("Prefix Not Found"), 
                             tr("Could not find wine prefix for selected game."));
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
        QMessageBox::warning(nullptr, tr("Tester Not Found"), 
                             tr("Could not find tester executable in wine prefix."));
        return;
    }
    
    // Execute tester
    executeTester(testerPath, prefixPath, currentPlatform);
}

QString TestingSection::getPrefixForGame(const QString &gameName, const QString &platform)
{
    const QString platformKey = platformLabelToKey(platform);
    if (platformKey == QStringLiteral("steam") || platformKey == QStringLiteral("lutris")) {
        QString prefixPath = TesterLauncher::getPrefixForGame(gameName, platform, steamIntegration, lutrisIntegration);
        if (!prefixPath.isEmpty()) {
            qDebug() << "Found" << platform << "prefix for" << gameName << ":" << prefixPath;
            return prefixPath;
        }
        qDebug() << platform << "game not found:" << gameName;
        return QString::fromUtf8("");
    } else if (platformKey == QStringLiteral("custom")) {
        if (customPrefixPath.isEmpty()) {
            // Try to acquire it now
            const QStringList chosen = getCustomPrefixGames();
            Q_UNUSED(chosen);
        }
        return customPrefixPath;
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
    return TesterLauncher::findTesterInPrefix(prefixPath, testerType);
}

void TestingSection::showMissingTesterDialog(const QString &prefixPath)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        nullptr, 
        tr("Tester Not Found"), 
        tr("Tester executable not found in wine prefix:\n%1\n\nWould you like to install Wine Bridge now?").arg(prefixPath),
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

    const QString platformKey = getCurrentPlatformKey();
    if (platformKey == QStringLiteral("steam")) {
        pluginInstall->installSteamProtonBridge();
    } else if (platformKey == QStringLiteral("lutris")) {
        pluginInstall->installLutrisWineBridge();
    } else if (platformKey == QStringLiteral("custom")) {
        pluginInstall->installWineBridgeToCustomPrefix();
    } else {
        qDebug() << "TestingSection: Unknown platform for installation:" << currentPlatform;
    }
}

QString TestingSection::getCurrentGameId()
{
    if (platformLabelToKey(currentPlatform) != QStringLiteral("steam")) {
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
    if (platformLabelToKey(currentPlatform) != QStringLiteral("lutris")) {
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
    const QString platformKey = platformLabelToKey(platform);
    if (platformKey == QStringLiteral("steam")) {
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
                        QMessageBox::information(nullptr, tr("Tester Launched"), 
                                                 tr("Tester has been launched successfully through Steam Proton."));
                    } else {
                        qDebug() << "Failed to launch tester:" << process.errorString();
                        QMessageBox::warning(nullptr, tr("Launch Failed"), 
                                             tr("Failed to launch tester: %1").arg(process.errorString()));
                    }
                } else {
                    qDebug() << "Proton binary not found or not executable:" << protonBinaryPath;
                    QMessageBox::warning(nullptr, tr("Proton Not Found"), 
                                         tr("Proton binary not found: %1").arg(protonBinaryPath));
                }
            } else {
                qDebug() << "Proton path not found for version:" << protonVersion;
                QMessageBox::warning(nullptr, tr("Proton Not Found"), 
                                     tr("Proton installation not found for version: %1").arg(protonVersion));
            }
        } else {
            qDebug() << "No game ID available for Steam platform";
            QMessageBox::warning(nullptr, tr("Game Not Selected"), 
                                 tr("Please select a Steam game first."));
        }
    } else if (platformKey == QStringLiteral("lutris")) {
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
                    // Only force win64 when launching the 64-bit tester explicitly
                    {
                        QFileInfo testerInfo(testerPath);
                        const QString testerName = testerInfo.fileName();
                        if (testerName.compare(QString::fromUtf8("Tester64.exe"), Qt::CaseInsensitive) == 0) {
                            env.insert(QString::fromUtf8("WINEARCH"), QString::fromUtf8("win64"));
                        }
                    }
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
                    QMessageBox::information(nullptr, tr("Tester Launched"), 
                                             tr("Tester has been launched successfully through Lutris Wine."));
                    } else {
                        qDebug() << "Failed to launch tester:" << process.errorString();
                        QMessageBox::warning(nullptr, tr("Launch Failed"), 
                                             tr("Failed to launch tester: %1").arg(process.errorString()));
                    }
                } else {
                    qDebug() << "Lutris wine binary not found or not executable:" << winePath;
                    QMessageBox::warning(nullptr, tr("Lutris Wine Not Found"), 
                                         tr("Lutris wine binary not found: %1").arg(winePath));
                }
            } else {
                qDebug() << "Wine version not found for game:" << gameSlug;
                QMessageBox::warning(nullptr, tr("Wine Version Not Found"), 
                                     tr("Wine version not found for game: %1").arg(gameSlug));
            }
        } else {
            qDebug() << "No game slug available for Lutris platform";
            QMessageBox::warning(nullptr, tr("Game Not Selected"), 
                                 tr("Please select a Lutris game first."));
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
        QMessageBox::information(nullptr, tr("Tester Launched"), 
                                 tr("Tester has been launched successfully through system Wine."));
        } else {
            qDebug() << "Failed to launch tester:" << process.errorString();
            QMessageBox::warning(nullptr, tr("Launch Failed"), 
                                 tr("Failed to launch tester: %1").arg(process.errorString()));
        }
    }
} 