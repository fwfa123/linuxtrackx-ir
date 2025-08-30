#ifndef TESTING_SECTION_H
#define TESTING_SECTION_H

#include <QObject>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QStringList>
#include <QProgressBar>
#include <QtConcurrent>
#include <QFutureWatcher>
#include "ltr_gui.h"
#include "steam_integration.h"
#include "lutris_integration.h"
#include "tester_launcher.h"

// WineArchitecture is defined in tester_launcher.h

class TestingSection : public QObject
{
    Q_OBJECT

public:
    explicit TestingSection(QObject *parent = nullptr);
    ~TestingSection();

    // Initialize UI connections
    void setupUI(Ui::LinuxtrackMainForm &ui);

    // UI Components
    QGroupBox *testingGroupBox;
    QRadioButton *testerExeRadioButton;
    QRadioButton *ftTesterRadioButton;
    QComboBox *platformComboBox;
    QPushButton *loadGamesButton;
    QComboBox *gameComboBox;
    QPushButton *runTesterButton;
    QLabel *testingStatusLabel;
    QLineEdit *gameFilterEdit;
    QProgressBar *testingProgressBar;

    // Testing functionality
    void startTracking();
    void loadGamesForPlatform(const QString &platform);
    void runSelectedTester();
    bool checkTesterInPrefix(const QString &prefixPath, const QString &testerType);
    void offerWineBridgeInstallation(const QString &prefixPath);

signals:
    void testingWorkflowStarted();

public slots:
    void onTesterSelectionChanged();
    void onPlatformSelectionChanged();
    void onLoadGamesClicked();
    void onRunTesterClicked();
    void onTrackerStateChanged(linuxtrack_state_type current_state);
    void onGameSelectionChanged();
    void onFilterTextChanged(const QString &text);
    
    // Platform helpers (avoid string-based logic tied to translations)
    QString getCurrentPlatformKey() const;
    static QString platformLabelToKey(const QString &label);

private:
    QStringList getSteamGames();
    QStringList getLutrisGames();
    QStringList getCustomPrefixGames();
    QString getPrefixForGame(const QString &gameName, const QString &platform);
    QString findTesterInPrefix(const QString &prefixPath, const QString &testerType);
    void executeTester(const QString &testerPath, const QString &prefixPath, const QString &platform, WineArchitecture arch);
    void showMissingTesterDialog(const QString &prefixPath);
    
    // New architecture detection functions
    WineArchitecture detectWinePrefixArchitecture(const QString &prefixPath);
    QString selectAppropriateTester(const QString &prefixPath, WineArchitecture arch, const QString &preferredTester);
    bool validateTesterCompatibility(const QString &testerPath, const QString &prefixPath, WineArchitecture arch);
    
    // Helper method for fallback tester search
    QString findAnyTesterInPrefix(const QString &prefixPath);
    
    // Helper methods for game identification
    QString getCurrentGameId();
    QString getCurrentGameSlug();

    QString currentPlatform;
    QString currentGame;
    QString currentTesterType;
    QStringList currentGames;
    
    // Selected custom Wine prefix path for the "Custom Prefix" platform
    QString customPrefixPath;
    
    // Integration objects
    SteamIntegration *steamIntegration;
    LutrisIntegration *lutrisIntegration;

    // Guard to avoid repeated tracking starts
    bool trackingStarted;

    // Async loading support
    QFutureWatcher<QStringList> gamesLoadWatcher;
};

#endif // TESTING_SECTION_H 