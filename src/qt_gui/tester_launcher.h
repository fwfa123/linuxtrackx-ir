#ifndef TESTER_LAUNCHER_H
#define TESTER_LAUNCHER_H

#include <QString>

class SteamIntegration;
class LutrisIntegration;

// Lightweight helper to centralize tester-related lookup logic.
// Conservative: only resolution helpers for now; launching remains in TestingSection.
class TesterLauncher {
public:
    // Resolve Wine prefix for a given game and platform.
    // Returns empty string when not found.
    static QString getPrefixForGame(const QString &gameName,
                                    const QString &platform,
                                    SteamIntegration *steamIntegration,
                                    LutrisIntegration *lutrisIntegration);

    // Find tester executable inside a prefix.
    // testerType: "Tester.exe" or "FT_Tester". Returns absolute path or empty.
    static QString findTesterInPrefix(const QString &prefixPath,
                                      const QString &testerType);
};

#endif // TESTER_LAUNCHER_H


