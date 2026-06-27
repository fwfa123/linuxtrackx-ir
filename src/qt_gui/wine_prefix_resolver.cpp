#include "wine_prefix_resolver.h"
#include "wine_executable.h"

#include "../utils.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QSet>

namespace {

bool appendCandidate(QList<WineCandidate> *out, QSet<QString> *seen, const QString &rawPath,
                     const QString &source)
{
    if (!out || !seen || rawPath.isEmpty())
        return false;

    const QString resolved = resolveWineExecutable(rawPath);
    if (resolved.isEmpty() || seen->contains(resolved))
        return false;

    seen->insert(resolved);
    WineCandidate candidate;
    candidate.path = resolved;
    candidate.source = source;
    candidate.version = queryWineVersionString(resolved);
    out->append(candidate);
    ltr_int_log_message("detectWineFromPrefix: candidate %s from %s (v%s)\n",
                        resolved.toUtf8().constData(), source.toUtf8().constData(),
                        candidate.version.toUtf8().constData());
    return true;
}

QStringList searchDirectoriesForPrefix(const QString &prefixPath)
{
    QStringList dirs;
    const QFileInfo prefixInfo(prefixPath);
    QDir prefixDir = prefixInfo.absoluteDir();
    dirs << prefixInfo.absoluteFilePath();
    if (prefixDir.cdUp())
        dirs << prefixDir.absolutePath();
    if (prefixDir.cdUp())
        dirs << prefixDir.absolutePath();
    return dirs;
}

void scanEnvAssignments(const QString &content, const QString &baseDir, QList<WineCandidate> *out,
                        QSet<QString> *seen)
{
    static const QRegularExpression envRegex(
        QStringLiteral(R"((?:^|\s)(?:export\s+)?(?:WINE|WINELOADER|WINESERVER)=["']?([^"'\s#]+))"),
        QRegularExpression::MultilineOption);

    QRegularExpressionMatchIterator it = envRegex.globalMatch(content);
    while (it.hasNext()) {
        const QRegularExpressionMatch match = it.next();
        QString value = match.captured(1).trimmed();
        if (value.endsWith(QLatin1Char('/')))
            value.chop(1);
        if (!value.endsWith(QStringLiteral("/wine")) && !value.endsWith(QStringLiteral("/wine64")))
            continue;

        if (!QDir::isAbsolutePath(value))
            value = QDir(baseDir).absoluteFilePath(value);

        appendCandidate(out, seen, value, QStringLiteral("launch script environment"));
    }
}

void scanExecPaths(const QString &content, const QString &baseDir, QList<WineCandidate> *out,
                   QSet<QString> *seen)
{
    static const QRegularExpression execPathRegex(
        QStringLiteral("(?:^|\\s)(?:exec\\s+)?(/[^\\s\"'#]+/(?:wine64|wine))"));
    static const QRegularExpression quotedPathRegex(
        QStringLiteral("[\"'](/[^\\s\"']+/(?:wine64|wine))[\"']"));

    for (const QRegularExpression &regex : {execPathRegex, quotedPathRegex}) {
        QRegularExpressionMatchIterator it = regex.globalMatch(content);
        while (it.hasNext()) {
            const QRegularExpressionMatch match = it.next();
            QString value = match.captured(1).trimmed();
            if (!QDir::isAbsolutePath(value))
                value = QDir(baseDir).absoluteFilePath(value);
            appendCandidate(out, seen, value, QStringLiteral("launch script"));
        }
    }
}

void scanScriptFile(const QString &filePath, QList<WineCandidate> *out, QSet<QString> *seen)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    const QString content = QString::fromUtf8(file.readAll());
    const QFileInfo fileInfo(filePath);
    scanEnvAssignments(content, fileInfo.absolutePath(), out, seen);
    scanExecPaths(content, fileInfo.absolutePath(), out, seen);
}

void scanDesktopFile(const QString &filePath, QList<WineCandidate> *out, QSet<QString> *seen)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    const QString content = QString::fromUtf8(file.readAll());
    const QFileInfo fileInfo(filePath);
    scanExecPaths(content, fileInfo.absolutePath(), out, seen);
}

void scanSiblingInstalls(const QString &prefixPath, QList<WineCandidate> *out, QSet<QString> *seen)
{
    const QFileInfo prefixInfo(prefixPath);
    QDir parentDir = prefixInfo.absoluteDir();
    if (!parentDir.exists())
        return;

    const QString parentPath = parentDir.absolutePath();
    appendCandidate(out, seen, parentPath + QStringLiteral("/wine/bin/wine"),
                    QStringLiteral("sibling Wine install"));

    const QStringList wineDirNames = parentDir.entryList({QStringLiteral("wine-*")}, QDir::Dirs);
    for (const QString &wineDirName : wineDirNames) {
        appendCandidate(out, seen, parentPath + QLatin1Char('/') + wineDirName + QStringLiteral("/bin/wine"),
                        QStringLiteral("sibling Wine install"));
    }

    const QStringList protonDirNames = parentDir.entryList({QStringLiteral("proton*")}, QDir::Dirs);
    for (const QString &protonDirName : protonDirNames) {
        appendCandidate(out, seen,
                        parentPath + QLatin1Char('/') + protonDirName + QStringLiteral("/files/bin/wine"),
                        QStringLiteral("sibling Wine install"));
    }

    QDir runnersWineDir(parentPath + QStringLiteral("/runners/wine"));
    if (runnersWineDir.exists()) {
        const QStringList runnerNames = runnersWineDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &runnerName : runnerNames) {
            appendCandidate(out, seen, runnersWineDir.filePath(runnerName + QStringLiteral("/bin/wine")),
                            QStringLiteral("sibling Wine install"));
        }
    }
}

} // namespace

bool isValidWinePrefixPath(const QString &prefixPath)
{
    if (prefixPath.isEmpty())
        return false;

    const QDir prefixDir(prefixPath);
    if (!prefixDir.exists())
        return false;

    const QDir driveCDir(prefixPath + QStringLiteral("/drive_c"));
    if (!driveCDir.exists())
        return false;

    const QDir windowsDir(prefixPath + QStringLiteral("/drive_c/windows"));
    return windowsDir.exists();
}

QList<WineCandidate> detectWineFromPrefix(const QString &prefixPath)
{
    QList<WineCandidate> candidates;
    QSet<QString> seen;

    const QStringList searchDirs = searchDirectoriesForPrefix(prefixPath);
    for (const QString &dirPath : searchDirs) {
        QDir dir(dirPath);
        if (!dir.exists())
            continue;

        const QStringList scriptFiles =
            dir.entryList({QStringLiteral("*.sh")}, QDir::Files | QDir::Readable);
        for (const QString &scriptName : scriptFiles)
            scanScriptFile(dir.filePath(scriptName), &candidates, &seen);

        const QStringList desktopFiles =
            dir.entryList({QStringLiteral("*.desktop")}, QDir::Files | QDir::Readable);
        for (const QString &desktopName : desktopFiles)
            scanDesktopFile(dir.filePath(desktopName), &candidates, &seen);
    }

    scanSiblingInstalls(prefixPath, &candidates, &seen);
    return candidates;
}
