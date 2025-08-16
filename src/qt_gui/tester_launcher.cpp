#include "tester_launcher.h"
#include "steam_integration.h"
#include "lutris_integration.h"
#include <QDir>
#include <QFileInfo>
#include <QStringList>
#include <QProcess>
#include <QProcessEnvironment>
#include <QTextStream>
#include <QDebug>

QString TesterLauncher::getPrefixForGame(const QString &gameName,
                                         const QString &platform,
                                         SteamIntegration *steamIntegration,
                                         LutrisIntegration *lutrisIntegration)
{
    // Normalize to stable key for platform handling
    const QString platformKey = QString(platform).trimmed().toLower();
    if (platformKey == QStringLiteral("steam")) {
        if (!steamIntegration) { return QString(); }
        QList<SteamGame> steamGames = steamIntegration->getSteamGames();
        for (const SteamGame &game : steamGames) {
            if (game.game_name == gameName && game.is_installed) {
                return steamIntegration->findProtonPrefix(game.game_id);
            }
        }
        return QString();
    }

    if (platformKey == QStringLiteral("lutris")) {
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

WineArchitecture TesterLauncher::detectWinePrefixArchitecture(const QString &prefixPath)
{
    QDir prefixDir(prefixPath);
    if (!prefixDir.exists()) {
        qDebug() << "Prefix directory does not exist:" << prefixPath;
        return WineArchitecture::UNKNOWN;
    }
    
    // Method 1: Check for explicit WINEARCH environment variable in prefix context
    // This would be set if the prefix was created with a specific architecture
    
    // Method 2: Directory structure analysis
    QDir syswow64Dir(prefixPath + QString::fromUtf8("/drive_c/windows/syswow64"));
    QDir system32Dir(prefixPath + QString::fromUtf8("/drive_c/windows/system32"));
    
    if (syswow64Dir.exists() && system32Dir.exists()) {
        // Check if syswow64 has more DLLs than system32 (indicates 64-bit prefix)
        QStringList syswow64Files = syswow64Dir.entryList(QStringList() << QString::fromUtf8("*.dll"), QDir::Files);
        QStringList system32Files = system32Dir.entryList(QStringList() << QString::fromUtf8("*.dll"), QDir::Files);
        
        if (syswow64Files.size() > system32Files.size() * 0.8) {
            qDebug() << "Detected 64-bit prefix based on syswow64 DLL count";
            return WineArchitecture::WIN64;
        }
    }
    
    // Method 3: Check for 64-bit specific directories
    if (syswow64Dir.exists()) {
        qDebug() << "Detected 64-bit prefix based on syswow64 directory";
        return WineArchitecture::WIN64;
    }
    
    // Method 4: Check for 32-bit specific indicators
    if (system32Dir.exists() && !syswow64Dir.exists()) {
        qDebug() << "Detected 32-bit prefix based on system32 only";
        return WineArchitecture::WIN32;
    }
    
    // Method 5: Wine command detection
    QProcess wineProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QString::fromUtf8("WINEPREFIX"), prefixPath);
    wineProcess.setProcessEnvironment(env);
    
    wineProcess.start(QString::fromUtf8("wine"), QStringList() << QString::fromUtf8("--version"));
    
    if (wineProcess.waitForFinished(5000)) {
        QString output = QString::fromUtf8(wineProcess.readAllStandardOutput());
        QString error = QString::fromUtf8(wineProcess.readAllStandardError());
        QString combined = output + error;
        
        qDebug() << "Wine version output:" << combined;
        
        // Look for architecture indicators in wine output
        if (combined.contains(QString::fromUtf8("x86_64"), Qt::CaseInsensitive) ||
            combined.contains(QString::fromUtf8("amd64"), Qt::CaseInsensitive)) {
            qDebug() << "Detected 64-bit wine from version output";
            return WineArchitecture::WIN64;
        } else if (combined.contains(QString::fromUtf8("i386"), Qt::CaseInsensitive) ||
                   combined.contains(QString::fromUtf8("i686"), Qt::CaseInsensitive)) {
            qDebug() << "Detected 32-bit wine from version output";
            return WineArchitecture::WIN32;
        }
    } else {
        qDebug() << "Wine version detection timed out or failed";
    }
    
    // Method 6: Registry analysis (fallback)
    QString systemRegPath = prefixPath + QString::fromUtf8("/system.reg");
    QFile systemRegFile(systemRegPath);
    
    if (systemRegFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&systemRegFile);
        QString content = in.readAll();
        systemRegFile.close();
        
        // Look for architecture indicators in registry
        if (content.contains(QString::fromUtf8("x86_64"), Qt::CaseInsensitive) ||
            content.contains(QString::fromUtf8("amd64"), Qt::CaseInsensitive)) {
            qDebug() << "Detected 64-bit prefix from registry";
            return WineArchitecture::WIN64;
        } else if (content.contains(QString::fromUtf8("i386"), Qt::CaseInsensitive) ||
                   content.contains(QString::fromUtf8("i686"), Qt::CaseInsensitive)) {
            qDebug() << "Detected 32-bit prefix from registry";
            return WineArchitecture::WIN32;
        }
    }
    
    qDebug() << "Could not determine prefix architecture, defaulting to UNKNOWN";
    return WineArchitecture::UNKNOWN;
}

QString TesterLauncher::findAppropriateTester(const QString &prefixPath, WineArchitecture arch)
{
    QStringList searchDirs = {
        QString::fromUtf8(""),
        QString::fromUtf8("drive_c/windows"),
        QString::fromUtf8("drive_c/Program Files"),
        QString::fromUtf8("drive_c/Program Files/Linuxtrack"),
        QString::fromUtf8("drive_c/Program Files (x86)"),
        QString::fromUtf8("drive_c/Program Files (x86)/Linuxtrack")
    };
    
    // Determine which testers to look for based on architecture
    QStringList testerNames;
    if (arch == WineArchitecture::WIN64) {
        testerNames << QString::fromUtf8("Tester64.exe") << QString::fromUtf8("Tester.exe");
    } else if (arch == WineArchitecture::WIN32) {
        testerNames << QString::fromUtf8("Tester.exe") << QString::fromUtf8("Tester64.exe");
    } else {
        // For unknown architecture, try both in order of preference
        testerNames << QString::fromUtf8("Tester.exe") << QString::fromUtf8("Tester64.exe");
    }
    
    QDir prefixDir(prefixPath);
    if (!prefixDir.exists()) {
        return QString();
    }
    
    // Search for testers in the specified directories
    for (const QString &searchDir : searchDirs) {
        QDir searchDirObj = prefixDir;
        if (!searchDir.isEmpty()) {
            if (!searchDirObj.cd(searchDir)) {
                continue;
            }
        }
        
        for (const QString &testerName : testerNames) {
            QString testerPath = searchDirObj.filePath(testerName);
            QFileInfo testerFile(testerPath);
            // Do not require execute bit for Windows executables; Wine can launch them
            if (testerFile.exists() && testerFile.isFile()) {
                qDebug() << "Found appropriate tester:" << testerPath << "for architecture:" << (int)arch;
                return testerPath;
            }
        }
    }
    
    qDebug() << "No appropriate tester found for architecture:" << (int)arch;
    return QString();
}

bool TesterLauncher::isTesterCompatible(const QString &testerPath, const QString &prefixPath, WineArchitecture arch)
{
    QFileInfo testerFile(testerPath);
    if (!testerFile.exists() || !testerFile.isFile() || !testerFile.isExecutable()) {
        return false;
    }
    
    // Basic compatibility check based on tester name and architecture
    QString testerName = testerFile.fileName();
    
    if (arch == WineArchitecture::WIN64) {
        // For 64-bit prefixes, prefer Tester64.exe but accept Tester.exe
        return testerName.compare(QString::fromUtf8("Tester64.exe"), Qt::CaseInsensitive) == 0 ||
               testerName.compare(QString::fromUtf8("Tester.exe"), Qt::CaseInsensitive) == 0;
    } else if (arch == WineArchitecture::WIN32) {
        // For 32-bit prefixes, prefer Tester.exe but accept Tester64.exe
        return testerName.compare(QString::fromUtf8("Tester.exe"), Qt::CaseInsensitive) == 0 ||
               testerName.compare(QString::fromUtf8("Tester64.exe"), Qt::CaseInsensitive) == 0;
    } else {
        // For unknown architecture, accept both
        return testerName.compare(QString::fromUtf8("Tester.exe"), Qt::CaseInsensitive) == 0 ||
               testerName.compare(QString::fromUtf8("Tester64.exe"), Qt::CaseInsensitive) == 0;
    }
}


