#ifndef WINE_EXECUTABLE_H
#define WINE_EXECUTABLE_H

#include <QString>

/** Resolve a Wine binary name or path to an absolute executable path. */
QString resolveWineExecutable(const QString &winePathOrName);

/** Run `wine --version` and return the version string (e.g. "11.0.1"), or empty on failure. */
QString queryWineVersionString(const QString &resolvedWinePath);

#endif
