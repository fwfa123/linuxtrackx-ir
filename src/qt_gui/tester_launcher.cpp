#include "tester_launcher.h"
#include "steam_integration.h"
#include "lutris_integration.h"
#include <QDir>
#include <QFileInfo>
#include <QStringList>

QString TesterLauncher::getPrefixForGame(const QString &gameName,
                                         const QString &platform,
                                         SteamIntegration *steamIntegration,
                                         LutrisIntegration *lutrisIntegration)
{
    if (platform == QString::fromUtf8("Steam")) {
        if (!steamIntegration) { return QString(); }
        QList<SteamGame> steamGames = steamIntegration->getSteamGames();
        for (const SteamGame &game : steamGames) {
            if (game.game_name == gameName && game.is_installed) {
                return steamIntegration->findProtonPrefix(game.game_id);
            }
        }
        return QString();
    }

    if (platform == QString::fromUtf8("Lutris")) {
        if (!lutrisIntegration) { return QString(); }
        QList<LutrisGame> lutrisGames = lutrisIntegration->getLutrisGames();
        for (const LutrisGame &game : lutrisGames) {
            if (game.game_name == gameName) {
                return game.wine_prefix;
            }
        }
        return QString();
    }

    // Custom Prefix resolution is handled by caller (TestingSection)
    return QString();
}

QString TesterLauncher::findTesterInPrefix(const QString &prefixPath,
                                           const QString &testerType)
{
    QDir prefixDir(prefixPath);
    if (!prefixDir.exists()) {
        return QString();
    }

    QStringList possibleNames;
    if (testerType == QString::fromUtf8("Tester.exe")) {
        possibleNames << QString::fromUtf8("Tester.exe") << QString::fromUtf8("Tester64.exe");
    } else if (testerType == QString::fromUtf8("FT_Tester")) {
        possibleNames << QString::fromUtf8("FT_Tester.exe") << QString::fromUtf8("FreeTrackTester.exe");
    }

    for (const QString &testerName : possibleNames) {
        QString testerPath = prefixDir.filePath(testerName);
        QFileInfo testerFile(testerPath);
        if (testerFile.exists() && testerFile.isFile()) {
            return testerPath;
        }
    }

    QStringList searchDirs = {
        QString::fromUtf8("drive_c/windows"),
        QString::fromUtf8("drive_c/Program Files"),
        QString::fromUtf8("drive_c/Program Files/Linuxtrack"),
        QString::fromUtf8("drive_c/Program Files (x86)"),
        QString::fromUtf8("drive_c/Program Files (x86)/Linuxtrack")
    };
    for (const QString &searchDir : searchDirs) {
        QDir dir = prefixDir;
        dir.cd(searchDir);
        if (dir.exists()) {
            for (const QString &testerName : possibleNames) {
                QString testerPath = dir.filePath(testerName);
                QFileInfo testerFile(testerPath);
                if (testerFile.exists() && testerFile.isFile()) {
                    return testerPath;
                }
            }
        }
    }

    return QString();
}


