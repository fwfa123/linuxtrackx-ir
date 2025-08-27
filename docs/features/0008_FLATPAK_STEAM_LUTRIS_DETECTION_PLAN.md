# 0008 Flatpak Steam/Lutris Detection Plan

## Context

The Linuxtrack GUI provides installation buttons for Steam (Proton) and Lutris platforms on the Gaming tab. Currently, the detection logic only checks for native package installations in hardcoded paths, causing false negatives when users install Steam or Lutris via Flatpak. Users receive error messages stating these applications are not installed when they actually exist as Flatpak packages.

## Current Detection Logic

### Steam Detection
Located in `SteamIntegration::initializePaths()` (`steam_integration.cpp`):
- Checks paths: `~/.steam/steam/`, `~/.local/share/Steam/`, `/usr/share/steam/`
- Validates by checking existence of `steam` binary in each path
- Sets `steamPath`, `steamAppsPath`, `compatDataPath` if found

### Lutris Detection
Located in `LutrisIntegration::isLutrisInstalled()` (`lutris_integration.cpp`):
- Checks database path: `~/.local/share/lutris/pga.db`
- Checks config path: `~/.config/lutris/games/`
- Validates by checking existence of both paths

### Integration Points
- Gaming tab buttons call `PluginInstall::installSteamProtonBridge()` and `PluginInstall::installLutrisWineBridge()`
- Testing section calls `SteamIntegration::isSteamInstalled()` and `LutrisIntegration::isLutrisInstalled()`
- Both methods return false for Flatpak installations, triggering error dialogs

## Proposed Solution

### Flatpak Detection Algorithm

#### Steam Flatpak Detection
1. Check if Flatpak is installed using `flatpak --version`
2. Query installed Flatpak apps: `flatpak list --app --columns=application`
3. Look for `com.valvesoftware.Steam` in the results
4. If found, get installation details: `flatpak info com.valvesoftware.Steam`
5. Extract Steam data directory from Flatpak info (typically `~/.var/app/com.valvesoftware.Steam/data/Steam`)
6. Validate Steam structure exists in Flatpak data directory
7. Set Steam paths to Flatpak data directory structure

#### Lutris Flatpak Detection
1. Check if Flatpak is installed using `flatpak --version`
2. Query installed Flatpak apps: `flatpak list --app --columns=application`
3. Look for `net.lutris.Lutris` in the results
4. If found, get installation details: `flatpak info net.lutris.Lutris`
5. Extract Lutris data directory from Flatpak info (typically `~/.var/app/net.lutris.Lutris/data/lutris`)
6. Validate Lutris database and config exist in Flatpak data directory
7. Set Lutris paths to Flatpak data directory structure

### Implementation Steps

#### Phase 1: Core Detection Functions
1. Create `FlatpakDetector` class (`flatpak_detector.h`, `flatpak_detector.cpp`)
2. Implement `FlatpakDetector::isFlatpakInstalled()` - check `flatpak --version`
3. Implement `FlatpakDetector::listInstalledApps()` - run `flatpak list --app`
4. Implement `FlatpakDetector::getAppInfo(const QString &appId)` - run `flatpak info <appId>`

#### Phase 2: Steam Flatpak Integration
1. Extend `SteamIntegration::initializePaths()` to call Flatpak detection as fallback
2. Add `SteamIntegration::detectSteamFlatpak()` method
3. Map Flatpak Steam data directory structure to expected Steam paths:
   - Flatpak Steam data: `~/.var/app/com.valvesoftware.Steam/data/Steam`
   - Map to: `steamPath = <flatpak-data>/Steam`, `steamAppsPath = <flatpak-data>/Steam/steamapps/`

#### Phase 3: Lutris Flatpak Integration
1. Extend `LutrisIntegration::initializePaths()` to call Flatpak detection as fallback
2. Add `LutrisIntegration::detectLutrisFlatpak()` method
3. Map Flatpak Lutris data directory structure to expected Lutris paths:
   - Flatpak Lutris data: `~/.var/app/net.lutris.Lutris/data/lutris`
   - Map to: `databasePath = <flatpak-data>/pga.db`, `configPath = <flatpak-data>/games/`

#### Phase 4: Error Handling and Logging
1. Add detailed logging for Flatpak detection attempts
2. Update error messages to mention Flatpak support
3. Add fallback messaging when Flatpak detection fails

### Files to Modify

#### Core Files
- **New**: `src/qt_gui/flatpak_detector.h` - Flatpak detection interface
- **New**: `src/qt_gui/flatpak_detector.cpp` - Flatpak detection implementation

#### Existing Files
- `src/qt_gui/steam_integration.h` - Add Flatpak detection method declaration
- `src/qt_gui/steam_integration.cpp` - Extend `initializePaths()` with Flatpak fallback
- `src/qt_gui/lutris_integration.h` - Add Flatpak detection method declaration
- `src/qt_gui/lutris_integration.cpp` - Extend `initializePaths()` with Flatpak fallback

#### Build System
- `src/qt_gui/ltr_gui.pro.in` - Add new Flatpak detector source files

### Algorithms

#### Flatpak App Detection
```cpp
bool FlatpakDetector::isAppInstalled(const QString &appId)
{
    // 1. Check if flatpak command exists
    if (!isFlatpakInstalled()) return false;
    
    // 2. Run flatpak list command
    QProcess process;
    process.start("flatpak", QStringList() << "list" << "--app" << "--columns=application");
    process.waitForFinished();
    
    // 3. Parse output for appId
    QString output = QString::fromUtf8(process.readAllStandardOutput());
    QStringList lines = output.split('\n');
    return lines.contains(appId);
}
```

#### Steam Flatpak Path Mapping
```cpp
QString SteamIntegration::getSteamFlatpakDataPath()
{
    // Typical Flatpak Steam data location
    QString homeDir = getHomeDirectory();
    return homeDir + "/.var/app/com.valvesoftware.Steam/data/Steam";
}

void SteamIntegration::setupFlatpakSteamPaths()
{
    QString flatpakSteamPath = getSteamFlatpakDataPath();
    if (QDir(flatpakSteamPath).exists()) {
        steamPath = flatpakSteamPath;
        steamAppsPath = flatpakSteamPath + "/steamapps/";
        compatDataPath = flatpakSteamPath + "/steamapps/compatdata/";
    }
}
```

#### Lutris Flatpak Path Mapping
```cpp
QString LutrisIntegration::getLutrisFlatpakDataPath()
{
    // Typical Flatpak Lutris data location
    QString homeDir = getHomeDirectory();
    return homeDir + "/.var/app/net.lutris.Lutris/data/lutris";
}

void LutrisIntegration::setupFlatpakLutrisPaths()
{
    QString flatpakLutrisPath = getLutrisFlatpakDataPath();
    if (QDir(flatpakLutrisPath).exists()) {
        databasePath = flatpakLutrisPath + "/pga.db";
        configPath = flatpakLutrisPath + "/games/";
    }
}
```

### Dependencies

#### External Dependencies
- `flatpak` command-line tool (checked at runtime)
- Qt5 QProcess for running external commands

#### Internal Dependencies
- Existing `SteamIntegration` and `LutrisIntegration` classes
- `utils.h` for logging functions (`ltr_int_log_message`)

### Testing Considerations

#### Test Cases
1. Native Steam/Lutris installation (existing functionality)
2. Flatpak Steam installation only
3. Flatpak Lutris installation only
4. Both native and Flatpak installations present
5. Flatpak installed but Steam/Lutris data directories missing
6. Flatpak not installed on system

#### Edge Cases
- Flatpak Steam with custom data directory
- Multiple Flatpak installations
- Flatpak sandboxing restrictions
- Permission issues accessing Flatpak data directories

### Migration Strategy

#### Backward Compatibility
- Existing native installation detection remains unchanged
- Flatpak detection is added as fallback only
- No breaking changes to existing functionality

#### Rollback Plan
- Flatpak detection can be disabled by commenting out Flatpak fallback calls
- Original error messages preserved if Flatpak detection fails
- No permanent changes to user data or system state

### Success Criteria

#### Functional
- Flatpak Steam installations are detected and usable
- Flatpak Lutris installations are detected and usable
- Game library scanning works for Flatpak installations
- Wine bridge installation works for Flatpak games

#### Non-Functional
- No performance impact on systems without Flatpak
- Minimal startup time increase (< 100ms)
- Clear error messages when Flatpak detection fails
- Comprehensive logging for troubleshooting
