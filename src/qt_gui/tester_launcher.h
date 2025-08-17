#ifndef TESTER_LAUNCHER_H
#define TESTER_LAUNCHER_H

#include <QString>

class SteamIntegration;
class LutrisIntegration;

// Wine prefix architecture enumeration
enum class WineArchitecture {
    UNKNOWN,
    WIN32,
    WIN64
};

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
    
    // Detect wine prefix architecture
    static WineArchitecture detectWinePrefixArchitecture(const QString &prefixPath);
    
    // Find appropriate tester based on architecture
    static QString findAppropriateTester(const QString &prefixPath, WineArchitecture arch);
    
    // Check if a specific tester is compatible with a prefix
    static bool isTesterCompatible(const QString &testerPath, const QString &prefixPath, WineArchitecture arch);
};

#endif // TESTER_LAUNCHER_H


