#ifndef WINE_PREFIX_RESOLVER_H
#define WINE_PREFIX_RESOLVER_H

#include <QString>
#include <QList>

struct WineCandidate {
    QString path;
    QString source;
    QString version;
};

/** Return validated Wine binaries inferred from scripts and sibling installs near a prefix. */
QList<WineCandidate> detectWineFromPrefix(const QString &prefixPath);

/** True when prefixPath looks like a Wine WINEPREFIX root. */
bool isValidWinePrefixPath(const QString &prefixPath);

#endif
