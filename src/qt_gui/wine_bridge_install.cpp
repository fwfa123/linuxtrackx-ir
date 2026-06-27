#include "wine_bridge_install.h"
#include "wine_executable.h"
#include "installer_paths.h"

#include "../utils.h"

#include <QByteArrayView>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QAbstractButton>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QStandardPaths>
#include <QWidget>

namespace WineBridgeInstall {

bool prefixIsWin64(const QString &prefixPath)
{
    QFile userReg(prefixPath + QStringLiteral("/user.reg"));
    if (!userReg.open(QIODevice::ReadOnly))
        return false;
    return userReg.readAll().contains(QByteArrayView("#arch=win64"));
}

QString installDirectoryForPrefix(const QString &prefixPath)
{
    if (prefixIsWin64(prefixPath))
        return prefixPath + QStringLiteral("/drive_c/Program Files (x86)/Linuxtrack");
    return prefixPath + QStringLiteral("/drive_c/Program Files/Linuxtrack");
}

QString registryPathForPrefix(const QString &prefixPath)
{
    if (prefixIsWin64(prefixPath))
        return QStringLiteral("C:\\Program Files (x86)\\Linuxtrack\\");
    return QStringLiteral("C:\\Program Files\\Linuxtrack\\");
}

static QString firmwareBasePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
        + QStringLiteral("/.config/linuxtrack/tir_firmware");
}

QStringList findExistingInstallDirs(const QString &prefixPath)
{
    QStringList found;
    const QStringList candidates = {prefixPath + QStringLiteral("/drive_c/Program Files/Linuxtrack"),
                                    prefixPath
                                    + QStringLiteral("/drive_c/Program Files (x86)/Linuxtrack")};
    for (const QString &dirPath : candidates) {
        const QDir dir(dirPath);
        if (!dir.exists())
            continue;
        const bool hasMarker =
            QFileInfo::exists(dir.filePath(QStringLiteral("NPClient.dll")))
            || QFileInfo::exists(dir.filePath(QStringLiteral("uninstall.exe")));
        if (hasMarker)
            found.append(dir.absolutePath());
    }
    return found;
}

static bool wineRegDeleteKey(const QString &winePath, const QString &prefixPath,
                             const QProcessEnvironment &extraEnv, const QString &hiveKey)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QStringLiteral("WINEPREFIX"), prefixPath);
    for (const QString &key : extraEnv.keys())
        env.insert(key, extraEnv.value(key));

    QProcess process;
    process.setProcessEnvironment(env);
    process.start(winePath,
                  {QStringLiteral("reg"), QStringLiteral("delete"), hiveKey, QStringLiteral("/f")});
    if (!process.waitForFinished(30000)) {
        ltr_int_log_message("wineRegDeleteKey timeout key=%s\n", hiveKey.toUtf8().constData());
        return false;
    }
    if (process.exitCode() != 0) {
        ltr_int_log_message("wineRegDeleteKey key=%s exit=%d stderr=%s (may be absent)\n",
                            hiveKey.toUtf8().constData(), process.exitCode(),
                            process.readAllStandardError().constData());
    }
    return true;
}

static bool isManagedFirmwareSymlink(const QString &path)
{
    QFileInfo info(path);
    if (!info.isSymLink())
        return false;

    QFileInfo targetInfo(info.symLinkTarget());
    QString canonical = targetInfo.canonicalFilePath();
    if (canonical.isEmpty())
        canonical = targetInfo.absoluteFilePath();

    const QFileInfo baseInfo(firmwareBasePath());
    QString baseCanonical = baseInfo.canonicalFilePath();
    if (baseCanonical.isEmpty())
        baseCanonical = baseInfo.absoluteFilePath();
    if (baseCanonical.isEmpty())
        return false;

    return canonical == baseCanonical || canonical.startsWith(baseCanonical + QLatin1Char('/'));
}

static bool removeBridgeFiles(const QString &installDir)
{
    const QStringList firmwareNames = {QStringLiteral("TIRViews.dll"), QStringLiteral("mfc42u.dll"),
                                       QStringLiteral("mfc42.dll")};
    for (const QString &name : firmwareNames) {
        const QString path = QDir(installDir).filePath(name);
        if (!QFileInfo::exists(path))
            continue;
        if (isManagedFirmwareSymlink(path)) {
            if (!QFile::remove(path)) {
                ltr_int_log_message("removeBridgeFiles: could not unlink firmware symlink %s\n",
                                    path.toUtf8().constData());
            } else {
                ltr_int_log_message("removeBridgeFiles: unlinked firmware symlink %s\n",
                                    path.toUtf8().constData());
            }
        } else {
            ltr_int_log_message("removeBridgeFiles: skipping non-managed file %s\n",
                                path.toUtf8().constData());
        }
    }

    static const char *const bridgeFiles[] = {
        "NPClient.dll",
        "NPClient64.dll",
        "NPClient.dll.backup",
        "NPClient64.dll.backup",
        "FreeTrackClient.dll.backup",
        "check_data.exe",
        "Controller.exe",
        "Tester.exe",
        "Tester64.exe",
        "TrackIR.exe",
        "FreeTrackClient.dll",
        "FreeTrackTester.exe",
        "linuxtrack.ico",
        "uninstall.exe",
    };
    for (const char *name : bridgeFiles) {
        const QString path = QDir(installDir).filePath(QString::fromUtf8(name));
        if (QFileInfo::exists(path) && !QFile::remove(path)) {
            ltr_int_log_message("removeBridgeFiles: could not delete %s\n", path.toUtf8().constData());
        }
    }

    QDir dir(installDir);
    if (dir.exists() && dir.isEmpty()) {
        if (!dir.rmdir(dir.absolutePath())) {
            ltr_int_log_message("removeBridgeFiles: could not rmdir %s\n",
                                installDir.toUtf8().constData());
            return false;
        }
        ltr_int_log_message("removeBridgeFiles: removed empty dir %s\n", installDir.toUtf8().constData());
    }
    return true;
}

static void removeStartMenuShortcuts(const QString &prefixPath)
{
    const QDir usersDir(prefixPath + QStringLiteral("/drive_c/users"));
    if (!usersDir.exists())
        return;

    static const QStringList menuSuffixes = {
        QStringLiteral("/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Linuxtrack"),
        QStringLiteral("/Application Data/Microsoft/Windows/Start Menu/Programs/Linuxtrack"),
    };

    const QStringList userEntries = usersDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &user : userEntries) {
        for (const QString &suffix : menuSuffixes) {
            const QString menuPath = usersDir.filePath(user + suffix);
            QDir menuDir(menuPath);
            if (!menuDir.exists())
                continue;
            const QStringList files = menuDir.entryList(QDir::Files);
            for (const QString &fileName : files)
                QFile::remove(menuDir.filePath(fileName));
            if (menuDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot).isEmpty())
                menuDir.rmdir(menuDir.absolutePath());
        }
    }
}

static bool removeBridgeRegistry(const QString &winePath, const QString &prefixPath,
                                 const QProcessEnvironment &extraEnv)
{
    static const QStringList registryKeys = {
        QStringLiteral("HKLM\\SOFTWARE\\Linuxtrack"),
        QStringLiteral("HKCU\\Software\\NaturalPoint\\NATURALPOINT\\NPClient Location"),
        QStringLiteral("HKCU\\Software\\Freetrack\\FreetrackClient"),
        QStringLiteral("HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Linuxtrack"),
    };

    for (const QString &key : registryKeys)
        wineRegDeleteKey(winePath, prefixPath, extraEnv, key);
    return true;
}

bool removeFromPrefix(const QString &prefixPath, const QString &wineBinaryPath,
                      const QProcessEnvironment &extraEnv, const QStringList &installDirs,
                      QString *lastError)
{
    const QString resolvedWine = resolveWineExecutable(wineBinaryPath);
    if (resolvedWine.isEmpty()) {
        if (lastError)
            *lastError = QString::fromUtf8("Wine binary not found or not executable: ") + wineBinaryPath;
        return false;
    }

    removeStartMenuShortcuts(prefixPath);

    for (const QString &installDir : installDirs) {
        if (!removeBridgeFiles(installDir)) {
            if (lastError)
                *lastError = QString::fromUtf8("Could not fully remove install directory: ") + installDir;
            return false;
        }
    }

    removeBridgeRegistry(resolvedWine, prefixPath, extraEnv);

    ltr_int_log_message("WineBridgeInstall: removed existing install from prefix %s\n",
                        prefixPath.toUtf8().constData());
    return true;
}

enum class ExistingInstallPromptResult {
    ProceedWithInstall,
    RemoveThenInstall,
    UninstallOnlySuccess,
    Cancelled,
    RemovalFailed,
};

static ExistingInstallPromptResult promptExistingInstall(QWidget *promptParent,
                                                         const QString &prefixPath,
                                                         const QString &wineBinaryPath,
                                                         const QProcessEnvironment &extraEnv,
                                                         QString *lastError)
{
    const QStringList existing = findExistingInstallDirs(prefixPath);
    if (existing.isEmpty())
        return ExistingInstallPromptResult::ProceedWithInstall;
    if (!promptParent)
        return ExistingInstallPromptResult::ProceedWithInstall;

    QString message = QObject::tr(
        "An existing Linuxtrack Wine bridge installation was found in this prefix:\n\n");
    for (const QString &dir : existing)
        message += QStringLiteral("  • ") + dir + QStringLiteral("\n");
    message += QObject::tr(
        "\nWhat would you like to do? Firmware files in ~/.config/linuxtrack/tir_firmware "
        "are not deleted.");

    QMessageBox box(QMessageBox::Question, QObject::tr("Existing Linuxtrack Installation"), message,
                    QMessageBox::NoButton, promptParent);
    QPushButton *removeAndInstallBtn =
        box.addButton(QObject::tr("Remove and install"), QMessageBox::YesRole);
    QPushButton *installOverBtn =
        box.addButton(QObject::tr("Install over existing"), QMessageBox::NoRole);
    QPushButton *uninstallOnlyBtn = box.addButton(QObject::tr("Uninstall only"), QMessageBox::ActionRole);
    QAbstractButton *cancelBtn = box.addButton(QMessageBox::Cancel);
    box.setDefaultButton(installOverBtn);
    box.exec();

    QAbstractButton *clicked = box.clickedButton();
    if (!clicked || clicked == cancelBtn)
        return ExistingInstallPromptResult::Cancelled;
    if (clicked == installOverBtn)
        return ExistingInstallPromptResult::ProceedWithInstall;
    if (clicked == uninstallOnlyBtn) {
        if (removeFromPrefix(prefixPath, wineBinaryPath, extraEnv, existing, lastError))
            return ExistingInstallPromptResult::UninstallOnlySuccess;
        return ExistingInstallPromptResult::RemovalFailed;
    }
    if (clicked == removeAndInstallBtn) {
        if (removeFromPrefix(prefixPath, wineBinaryPath, extraEnv, existing, lastError))
            return ExistingInstallPromptResult::RemoveThenInstall;
        return ExistingInstallPromptResult::RemovalFailed;
    }
    return ExistingInstallPromptResult::Cancelled;
}

static bool wineRegAddString(const QString &winePath, const QString &prefixPath,
                             const QProcessEnvironment &extraEnv, const QString &hiveKey,
                             const QString &valueName, const QString &valueData)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QStringLiteral("WINEPREFIX"), prefixPath);
    for (const QString &key : extraEnv.keys())
        env.insert(key, extraEnv.value(key));

    QProcess process;
    process.setProcessEnvironment(env);
    process.start(winePath,
                  {QStringLiteral("reg"), QStringLiteral("add"), hiveKey,
                   QStringLiteral("/v"), valueName, QStringLiteral("/t"), QStringLiteral("REG_SZ"),
                   QStringLiteral("/d"), valueData, QStringLiteral("/f")});
    if (!process.waitForFinished(30000) || process.exitCode() != 0) {
        ltr_int_log_message("wineRegAddString failed key=%s name=%s exit=%d stderr=%s\n",
                            hiveKey.toUtf8().constData(), valueName.toUtf8().constData(),
                            process.exitCode(), process.readAllStandardError().constData());
        return false;
    }
    return true;
}

static void applyFirmwareLinks(const QString &installDir)
{
    const QString firmwareBase = firmwareBasePath() + QLatin1Char('/');
    const QStringList linkNames = {QStringLiteral("TIRViews.dll"), QStringLiteral("mfc42u.dll"),
                                   QStringLiteral("mfc42.dll")};
    for (const QString &name : linkNames) {
        const QString sourcePath = firmwareBase + name;
        const QString destPath = installDir + QStringLiteral("/") + name;
        QFileInfo sourceInfo(sourcePath);
        if (!sourceInfo.exists()) {
            ltr_int_log_message("applyFirmwareLinks: missing source %s\n",
                                sourcePath.toUtf8().constData());
            continue;
        }
        QFileInfo destInfo(destPath);
        if (destInfo.exists() || destInfo.isSymLink()) {
            if (!QFile::remove(destPath)) {
                ltr_int_log_message("applyFirmwareLinks: could not remove %s\n",
                                    destPath.toUtf8().constData());
                continue;
            }
        }
        if (!QFile::link(sourcePath, destPath)) {
            ltr_int_log_message("applyFirmwareLinks: link failed %s -> %s\n",
                                destPath.toUtf8().constData(), sourcePath.toUtf8().constData());
            continue;
        }
        ltr_int_log_message("applyFirmwareLinks: %s -> %s\n", destPath.toUtf8().constData(),
                            sourcePath.toUtf8().constData());
    }
}

static void runCheckDataOptional(const QString &winePath, const QString &prefixPath,
                                 const QProcessEnvironment &extraEnv, const QString &installDir)
{
    const QString checkData = QDir(installDir).filePath(QStringLiteral("check_data.exe"));
    if (!QFileInfo::exists(checkData))
        return;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QStringLiteral("WINEPREFIX"), prefixPath);
    for (const QString &key : extraEnv.keys())
        env.insert(key, extraEnv.value(key));

    QProcess process;
    process.setProcessEnvironment(env);
    process.start(winePath, {checkData});
    if (!process.waitForFinished(10000))
        process.kill();
    ltr_int_log_message("check_data.exe finished exit=%d\n", process.exitCode());
}

bool installToPrefix(const QString &prefixPath, const QString &wineBinaryPath, QString *lastError,
                     QString *debugInfo, QWidget *promptParent, InstallOutcome *outcome)
{
    return installToPrefix(prefixPath, wineBinaryPath, QProcessEnvironment(), lastError, debugInfo,
                           promptParent, outcome);
}

static bool isRequiredBridgeSource(const char *source, bool win64Prefix)
{
    if (qstrcmp(source, "NPClient.dll") == 0)
        return true;
    return win64Prefix && qstrcmp(source, "NPClient64.dll") == 0;
}

bool installToPrefix(const QString &prefixPath, const QString &wineBinaryPath,
                     const QProcessEnvironment &extraEnv, QString *lastError, QString *debugInfo,
                     QWidget *promptParent, InstallOutcome *outcome)
{
    auto setOutcome = [&](InstallOutcome value) {
        if (outcome)
            *outcome = value;
    };
    auto appendDebug = [&](const QString &line) {
        if (debugInfo)
            *debugInfo += line;
    };

    setOutcome(InstallOutcome::Failed);

    const QString resolvedWine = resolveWineExecutable(wineBinaryPath);
    if (resolvedWine.isEmpty()) {
        if (lastError)
            *lastError = QString::fromUtf8("Wine binary not found or not executable: ") + wineBinaryPath;
        return false;
    }

    const QString payloadDir = InstallerPaths::resolveWineBridgePayloadDir();
    if (payloadDir.isEmpty()) {
        if (lastError)
            *lastError =
                QString::fromUtf8("Linuxtrack Wine bridge files not found (NPClient.dll payload directory).");
        return false;
    }

    switch (promptExistingInstall(promptParent, prefixPath, resolvedWine, extraEnv, lastError)) {
    case ExistingInstallPromptResult::ProceedWithInstall:
    case ExistingInstallPromptResult::RemoveThenInstall:
        break;
    case ExistingInstallPromptResult::UninstallOnlySuccess:
        setOutcome(InstallOutcome::UninstalledOnly);
        return true;
    case ExistingInstallPromptResult::Cancelled:
        setOutcome(InstallOutcome::Cancelled);
        return false;
    case ExistingInstallPromptResult::RemovalFailed:
        setOutcome(InstallOutcome::Failed);
        return false;
    }

    const bool win64Prefix = prefixIsWin64(prefixPath);
    const QString installDir = installDirectoryForPrefix(prefixPath);
    const QString regPath = registryPathForPrefix(prefixPath);

    if (!QDir().mkpath(installDir)) {
        if (lastError)
            *lastError = QString::fromUtf8("Could not create install directory: ") + installDir;
        return false;
    }

    struct BridgeFileMap {
        const char *source;
        const char *dest;
    };
    static const BridgeFileMap files[] = {
        {"NPClient.dll", "NPClient.dll"},         {"NPClient64.dll", "NPClient64.dll"},
        {"check_data.exe", "check_data.exe"},     {"Controller.exe", "Controller.exe"},
        {"Tester.exe", "Tester.exe"},             {"Tester64.exe", "Tester64.exe"},
        {"TrackIR.exe", "TrackIR.exe"},           {"FreeTrackClient.dll", "FreeTrackClient.dll"},
        {"ftc.exe", "FreeTrackTester.exe"},
    };

    for (const BridgeFileMap &entry : files) {
        const QString src = QDir(payloadDir).filePath(QString::fromUtf8(entry.source));
        const QString dst = QDir(installDir).filePath(QString::fromUtf8(entry.dest));
        if (!QFileInfo::exists(src)) {
            if (isRequiredBridgeSource(entry.source, win64Prefix)) {
                if (lastError)
                    *lastError = QString::fromUtf8("Required Wine bridge file missing in payload: ")
                                  + src;
                return false;
            }
            ltr_int_log_message("WineBridgeInstall: skipping optional missing payload %s\n",
                                src.toUtf8().constData());
            continue;
        }
        if (QFileInfo::exists(dst) && !QFile::remove(dst)) {
            if (lastError)
                *lastError = QString::fromUtf8("Could not replace existing file: ") + dst;
            return false;
        }
        if (!QFile::copy(src, dst)) {
            if (lastError)
                *lastError = QString::fromUtf8("Failed to copy ") + src + QString::fromUtf8(" to ") + dst;
            return false;
        }
        ltr_int_log_message("WineBridgeInstall: copied %s\n", dst.toUtf8().constData());
    }

    const QString npClientDll = QDir(installDir).filePath(QStringLiteral("NPClient.dll"));
    if (!QFileInfo::exists(npClientDll)) {
        if (lastError)
            *lastError = QString::fromUtf8("NPClient.dll was not installed into the prefix.");
        return false;
    }
    if (win64Prefix) {
        const QString npClient64 = QDir(installDir).filePath(QStringLiteral("NPClient64.dll"));
        if (!QFileInfo::exists(npClient64)) {
            if (lastError)
                *lastError =
                    QString::fromUtf8("NPClient64.dll was not installed (required for WOW64 prefixes).");
            return false;
        }
    }

    const QString installDirWin = regPath.endsWith(QLatin1Char('\\')) ? regPath.chopped(1) : regPath;
    if (!wineRegAddString(resolvedWine, prefixPath, extraEnv,
                          QStringLiteral("HKLM\\SOFTWARE\\Linuxtrack"), QStringLiteral("Install_dir"),
                          installDirWin)) {
        if (lastError)
            *lastError = QString::fromUtf8("Failed to write HKLM\\SOFTWARE\\Linuxtrack registry keys.");
        return false;
    }
    if (!wineRegAddString(resolvedWine, prefixPath, extraEnv,
                          QStringLiteral("HKCU\\Software\\NaturalPoint\\NATURALPOINT\\NPClient Location"),
                          QStringLiteral("Path"), regPath)) {
        if (lastError)
            *lastError =
                QString::fromUtf8("Failed to write NaturalPoint NPClient Location registry key.");
        return false;
    }
    if (!wineRegAddString(resolvedWine, prefixPath, extraEnv,
                          QStringLiteral("HKCU\\Software\\Freetrack\\FreetrackClient"),
                          QStringLiteral("Path"), regPath)) {
        ltr_int_log_message("WineBridgeInstall: Freetrack registry optional write failed\n");
    }

    applyFirmwareLinks(installDir);
    runCheckDataOptional(resolvedWine, prefixPath, extraEnv, installDir);

    appendDebug(QString::fromUtf8("Installed Wine bridge to: ") + installDir + QString::fromUtf8("\n"));
    appendDebug(QString::fromUtf8("Registry path: ") + regPath + QString::fromUtf8("\n"));
    ltr_int_log_message("WineBridgeInstall: success dir=%s reg=%s\n", installDir.toUtf8().constData(),
                        regPath.toUtf8().constData());
    setOutcome(InstallOutcome::Installed);
    return true;
}

} // namespace WineBridgeInstall
