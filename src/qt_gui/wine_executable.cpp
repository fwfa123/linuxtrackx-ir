#include "wine_executable.h"

#include "../utils.h"

#include <QFileInfo>
#include <QProcess>
#include <QRegularExpression>
#include <QStandardPaths>

QString resolveWineExecutable(const QString &winePathOrName)
{
    if (winePathOrName.isEmpty())
        return QString();

    const QFileInfo directInfo(winePathOrName);
    if (directInfo.isAbsolute() && directInfo.exists() && directInfo.isExecutable())
        return directInfo.canonicalFilePath();

    const QString found = QStandardPaths::findExecutable(winePathOrName);
    if (found.isEmpty())
        return QString();

    const QFileInfo foundInfo(found);
    if (!foundInfo.exists() || !foundInfo.isExecutable())
        return QString();

    return foundInfo.canonicalFilePath();
}

QString queryWineVersionString(const QString &resolvedWinePath)
{
    const QString winePath = resolveWineExecutable(resolvedWinePath);
    if (winePath.isEmpty())
        return QString();

    QProcess process;
    process.start(winePath, {QStringLiteral("--version")});
    if (!process.waitForFinished(5000) || process.exitCode() != 0)
        return QString();

    const QString output = QString::fromUtf8(process.readAllStandardOutput());
    const QRegularExpression versionRegex(QStringLiteral(R"(wine-(\d+\.\d+(?:\.\d+)?))"));
    const QRegularExpressionMatch match = versionRegex.match(output);
    if (match.hasMatch())
        return match.captured(1);

    return QString();
}
