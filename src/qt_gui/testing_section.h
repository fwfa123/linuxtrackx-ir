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
#include "ltr_gui.h"
#include "steam_integration.h"
#include "lutris_integration.h"

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

    // Testing functionality
    void startTracking();
    void loadGamesForPlatform(const QString &platform);
    void runSelectedTester();
    bool checkTesterInPrefix(const QString &prefixPath, const QString &testerType);
    void offerWineBridgeInstallation(const QString &prefixPath);

public slots:
    void onTesterSelectionChanged();
    void onPlatformSelectionChanged();
    void onLoadGamesClicked();
    void onRunTesterClicked();

private:
    QStringList getSteamGames();
    QStringList getLutrisGames();
    QStringList getCustomPrefixGames();
    QString getPrefixForGame(const QString &gameName, const QString &platform);
    QString findTesterInPrefix(const QString &prefixPath, const QString &testerType);
    void executeTester(const QString &testerPath, const QString &prefixPath, const QString &platform);
    void showMissingTesterDialog(const QString &prefixPath);
    
    // Helper methods for game identification
    QString getCurrentGameId();
    QString getCurrentGameSlug();

    QString currentPlatform;
    QString currentGame;
    QString currentTesterType;
    QStringList currentGames;
    
    // Integration objects
    SteamIntegration *steamIntegration;
    LutrisIntegration *lutrisIntegration;

    // Guard to avoid repeated tracking starts
    bool trackingStarted;
};

#endif // TESTING_SECTION_H 