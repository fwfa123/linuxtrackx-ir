#include "wine_bridge_install.h"
#include "installer_paths.h"

#include "../utils.h"

#include <QByteArrayView>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QStandardPaths>

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
    const QString firmwareBase =
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
        + QStringLiteral("/.config/linuxtrack/tir_firmware/");
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
                     QString *debugInfo)
{
    return installToPrefix(prefixPath, wineBinaryPath, QProcessEnvironment(), lastError, debugInfo);
}

static bool isRequiredBridgeSource(const char *source, bool win64Prefix)
{
    if (qstrcmp(source, "NPClient.dll") == 0)
        return true;
    return win64Prefix && qstrcmp(source, "NPClient64.dll") == 0;
}

bool installToPrefix(const QString &prefixPath, const QString &wineBinaryPath,
                     const QProcessEnvironment &extraEnv, QString *lastError, QString *debugInfo)
{
    auto appendDebug = [&](const QString &line) {
        if (debugInfo)
            *debugInfo += line;
    };

    const QFileInfo wineInfo(wineBinaryPath);
    if (!wineInfo.exists() || !wineInfo.isExecutable()) {
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
    if (!wineRegAddString(wineBinaryPath, prefixPath, extraEnv,
                          QStringLiteral("HKLM\\SOFTWARE\\Linuxtrack"), QStringLiteral("Install_dir"),
                          installDirWin)) {
        if (lastError)
            *lastError = QString::fromUtf8("Failed to write HKLM\\SOFTWARE\\Linuxtrack registry keys.");
        return false;
    }
    if (!wineRegAddString(wineBinaryPath, prefixPath, extraEnv,
                          QStringLiteral("HKCU\\Software\\NaturalPoint\\NATURALPOINT\\NPClient Location"),
                          QStringLiteral("Path"), regPath)) {
        if (lastError)
            *lastError =
                QString::fromUtf8("Failed to write NaturalPoint NPClient Location registry key.");
        return false;
    }
    if (!wineRegAddString(wineBinaryPath, prefixPath, extraEnv,
                          QStringLiteral("HKCU\\Software\\Freetrack\\FreetrackClient"),
                          QStringLiteral("Path"), regPath)) {
        ltr_int_log_message("WineBridgeInstall: Freetrack registry optional write failed\n");
    }

    applyFirmwareLinks(installDir);
    runCheckDataOptional(wineBinaryPath, prefixPath, extraEnv, installDir);

    appendDebug(QString::fromUtf8("Installed Wine bridge to: ") + installDir + QString::fromUtf8("\n"));
    appendDebug(QString::fromUtf8("Registry path: ") + regPath + QString::fromUtf8("\n"));
    ltr_int_log_message("WineBridgeInstall: success dir=%s reg=%s\n", installDir.toUtf8().constData(),
                        regPath.toUtf8().constData());
    return true;
}

} // namespace WineBridgeInstall
