# 🎮 Lutris Integration Research Findings

**Branch**: `gaming-tab-and-lutris-install`  
**Date**: January 2025  
**Sources**: [Lutris Forums](https://forums.lutris.net/)  
**Status**: RESEARCH COMPLETED - IMPLEMENTATION COMPLETED ✅

---

## 📋 Key Findings from Lutris Forums

### 1. Lutris Wine Prefix Structure and Access

**Source**: [Run in Prefix From Command Line](https://forums.lutris.net/t/run-in-prefix-from-command-line/6045/7)

#### Lutris Database and Configuration
- **Database Location**: `~/.local/share/lutris/pga.db` (SQLite database)
- **Configuration Location**: `~/.config/lutris/games/` (YAML configuration files)
- **Database Contains**: Install ID, game slug, runner type, install path, config ID
- **Configuration Files**: Individual game configurations in YAML format

#### Wine Prefix Access Methods
```bash
# Basic wine prefix access
WINEPREFIX=/path/to/your/prefix wine /path/to/your/exe

# Using Lutris wine version
~/.local/share/lutris/runners/wine/wine-version-of-choice/bin/wine
```

### 2. Complete Wine Environment Setup

**Source**: [SOLVED "Run EXE inside wine prefix" command line](https://forums.lutris.net/t/solved-run-exe-inside-wine-prefix-command-line/7850)

#### Full Environment Variables Required
```bash
#!/bin/sh

export WINEPREFIX=/home/ytawo/Games/warcraft3
export WINEESYNC=1
export WINE=/home/ytawo/.local/share/lutris/runners/wine/lutris-4.20-x86_64/bin/wine
export DXVK_STATE_CACHE_PATH=/home/ytawo/Games/warcraft3
export __GL_SHADER_DISK_CACHE_PATH=/home/ytawo/Games/warcraft3
export SDL_VIDEO_FULLSCREEN_DISPLAY=off
export WINEARCH=win64
export WINE_LARGE_ADDRESS_AWARE=1
export game_name='Warcraft 3'
export WINEDLLOVERRIDES='api-ms-win-crt-private-l1-1-0,ucrtbase=n,b;d3d10,d3d10_1,d3d10core,d3d11,dxgi=n;d3d12,nvapi,nvapi64='

GAME_LOCATION='/home/ytawo/Games/warcraft3/drive_c/Program Files/Warcraft III/x86_64'
GAME_EXECUTABLE='Warcraft III.exe'

cd "$GAME_LOCATION"
$WINE "$GAME_EXECUTABLE" "$@"
```

#### Key Environment Variables Identified
- **WINEPREFIX**: Path to wine prefix
- **WINEESYNC**: Esync support
- **WINE**: Path to specific wine version
- **DXVK_STATE_CACHE_PATH**: DXVK cache location
- **__GL_SHADER_DISK_CACHE_PATH**: Shader cache location
- **WINEARCH**: Wine architecture (win64)
- **WINE_LARGE_ADDRESS_AWARE**: Large address support
- **WINEDLLOVERRIDES**: DLL override settings

### 3. Lutris Command Line Integration

**Source**: [Run in Prefix From Command Line](https://forums.lutris.net/t/run-in-prefix-from-command-line/6045/3)

#### Desktop Shortcut Method
- Lutris exports desktop shortcuts with unique numerical identifiers
- Commands use format: `lutris rungame/[game-id]`
- Game ID corresponds to installation order in Lutris

#### Database Query Method
- Use SQLite database `~/.local/share/lutris/pga.db`
- Query for game information including install ID, config ID, and paths
- Match with configuration files in `~/.config/lutris/games/`

---

## 🎯 Implementation Strategy Based on Research

### 1. Lutris Detection and Configuration Parsing

#### Database Access Method
```cpp
// Lutris database structure
struct LutrisGame {
    int install_id;
    QString game_slug;
    QString runner_type;
    QString install_path;
    int config_id;
    QString wine_prefix;
    QString wine_version;
};

// Parse Lutris database
QList<LutrisGame> parseLutrisDatabase() {
    // Access ~/.local/share/lutris/pga.db
    // Query for wine-based games
    // Extract wine prefix information
}
```

#### Configuration File Parsing
```cpp
// Parse YAML configuration files
struct LutrisConfig {
    QString wine_prefix;
    QString wine_version;
    QString executable_path;
    QMap<QString, QString> environment_variables;
};

LutrisConfig parseLutrisConfig(const QString& config_file) {
    // Parse ~/.config/lutris/games/[game-id].yml
    // Extract wine prefix and environment settings
}
```

### 2. Wine Bridge Installation Process

#### Environment Setup for Lutris Prefixes
```cpp
bool setupLutrisWineEnvironment(const QString& wine_prefix, const QString& wine_version) {
    // Set required environment variables
    setenv("WINEPREFIX", wine_prefix.toUtf8().constData(), 1);
    setenv("WINEARCH", "win64", 1);
    setenv("WINEESYNC", "1", 1);
    
    // Set wine version path
    QString wine_path = QString("~/.local/share/lutris/runners/wine/%1/bin/wine").arg(wine_version);
    setenv("WINE", wine_path.toUtf8().constData(), 1);
    
    // Set cache paths
    setenv("DXVK_STATE_CACHE_PATH", wine_prefix.toUtf8().constData(), 1);
    setenv("__GL_SHADER_DISK_CACHE_PATH", wine_prefix.toUtf8().constData(), 1);
    
    return true;
}
```

#### Installation Process
```cpp
bool installWineBridgeToLutrisPrefix(const QString& wine_prefix, const QString& wine_version) {
    // 1. Setup Lutris wine environment
    setupLutrisWineEnvironment(wine_prefix, wine_version);
    
    // 2. Run wine bridge installer
    QString installer_path = getWineBridgeInstallerPath();
    QStringList args;
    args << installer_path << "/S"; // Silent installation
    
    // 3. Execute installation
    return executeWineBridgeInstallation(args);
}
```

### 3. User Interface Design

#### Lutris Wine Prefix Selection
```cpp
class LutrisPrefixSelector : public QDialog {
private:
    QListWidget* prefix_list;
    QList<LutrisGame> available_games;
    
public:
    QString selectLutrisWinePrefix(QWidget* parent) {
        // 1. Parse Lutris database
        available_games = parseLutrisDatabase();
        
        // 2. Filter wine-based games
        QList<LutrisGame> wine_games = filterWineGames(available_games);
        
        // 3. Display selection dialog
        return showPrefixSelectionDialog(wine_games, parent);
    }
};
```

#### Integration with Existing WineLauncher
```cpp
class LutrisWineLauncher : public WineLauncher {
private:
    QString lutris_wine_version;
    QString lutris_wine_prefix;
    
public:
    bool setupLutrisEnvironment(const QString& game_id) {
        // 1. Query Lutris database for game
        LutrisGame game = getLutrisGame(game_id);
        
        // 2. Parse game configuration
        LutrisConfig config = parseLutrisConfig(game.config_id);
        
        // 3. Setup environment variables
        return setupLutrisWineEnvironment(config.wine_prefix, config.wine_version);
    }
};
```

---

## 🔧 Technical Implementation Plan

### Phase 1: Lutris Integration Foundation

#### 1.1 Database and Configuration Access
**Files to Create:**
- `src/qt_gui/lutris_database.cpp` - SQLite database access
- `src/qt_gui/lutris_database.h` - Database interface
- `src/qt_gui/lutris_config.cpp` - YAML configuration parsing
- `src/qt_gui/lutris_config.h` - Configuration interface

**Implementation:**
```cpp
// Database access
class LutrisDatabase {
public:
    QList<LutrisGame> getWineGames();
    LutrisGame getGameById(int install_id);
    QString getWinePrefix(int install_id);
};

// Configuration parsing
class LutrisConfigParser {
public:
    LutrisConfig parseGameConfig(const QString& config_file);
    QStringList getAvailableWineVersions();
    QString getWinePrefixPath(const QString& config_file);
};
```

#### 1.2 Environment Management
**Files to Modify:**
- `src/qt_gui/wine_launcher.cpp` - Extend for Lutris support
- `src/qt_gui/wine_launcher.h` - Add Lutris methods

**Implementation:**
```cpp
// Extend WineLauncher for Lutris
class WineLauncher {
public:
    bool setupLutrisEnvironment(const QString& wine_prefix, const QString& wine_version);
    bool runInLutrisPrefix(const QString& wine_prefix, const QString& executable);
    QStringList getLutrisWineVersions();
};
```

### Phase 2: GUI Integration

#### 2.1 Lutris Prefix Selection Dialog
**Files to Create:**
- `src/qt_gui/lutris_prefix_dialog.cpp` - Prefix selection UI
- `src/qt_gui/lutris_prefix_dialog.h` - Dialog interface
- `src/qt_gui/lutris_prefix_dialog.ui` - UI layout

**Features:**
- List available Lutris wine prefixes
- Show game names and wine versions
- Allow user selection
- Display prefix information

#### 2.2 Wine/Steam Tab Integration
**Files to Modify:**
- `src/qt_gui/ltr.ui` - Add new tab
- `src/qt_gui/ltr_gui.cpp` - Add button handlers
- `src/qt_gui/plugin_install.cpp` - Add Lutris installation

**Implementation:**
```cpp
// Add to PluginInstall class
void PluginInstall::installLutrisWineBridge() {
    // 1. Show Lutris prefix selection dialog
    QString selected_prefix = showLutrisPrefixDialog();
    
    if (!selected_prefix.isEmpty()) {
        // 2. Setup Lutris environment
        setupLutrisEnvironment(selected_prefix);
        
        // 3. Install wine bridge
        installWineBridgeToPrefix(selected_prefix);
    }
}
```

### Phase 3: Installation Process

#### 3.1 Lutris-Specific Installation Script
**Files to Create:**
- `scripts/install/install_lutris_wine_bridge.sh` - Lutris installation script

**Features:**
- Parse Lutris database and configuration
- Setup proper environment variables
- Install wine bridge to selected prefix
- Verify installation success

#### 3.2 Error Handling and Validation
**Implementation:**
```cpp
class LutrisInstaller {
public:
    bool validateLutrisInstallation();
    bool validateWinePrefix(const QString& prefix);
    bool installWineBridge(const QString& prefix);
    bool verifyInstallation(const QString& prefix);
    void handleInstallationError(const QString& error);
};
```

---

## 📊 Research Impact on Implementation

### ✅ **Confirmed Technical Approaches**

1. **Database Access**: SQLite database provides reliable game information
2. **Configuration Parsing**: YAML files contain all necessary wine settings
3. **Environment Setup**: Complete environment variable list identified
4. **Wine Version Management**: Lutris wine runners are accessible
5. **Prefix Structure**: Wine prefixes follow standard structure

### 🎯 **Implementation Advantages**

1. **Reliable Detection**: Database provides accurate game and prefix information
2. **Complete Environment**: All required environment variables identified
3. **Standard Structure**: Lutris follows predictable directory structure
4. **Extensible Design**: Can easily add support for other wine managers
5. **User-Friendly**: Can show game names instead of technical paths

### ⚠️ **Potential Challenges**

1. **Database Schema**: May need to handle different Lutris versions
2. **Configuration Format**: YAML parsing complexity
3. **Environment Variables**: Many variables to manage
4. **Wine Version Compatibility**: Different wine versions may behave differently
5. **Error Handling**: Complex failure scenarios to handle

---

## 🚀 Next Implementation Steps

### Immediate Actions (This Week)

1. **Create Lutris Database Access**
   - Implement SQLite database reading
   - Parse game information
   - Extract wine prefix paths

2. **Implement Configuration Parsing**
   - Parse YAML configuration files
   - Extract wine environment settings
   - Handle different configuration formats

3. **Test Lutris Environment Setup**
   - Implement environment variable setup
   - Test with actual Lutris installations
   - Verify wine bridge compatibility

### Week 2 Actions

1. **Create GUI Components**
   - Implement prefix selection dialog
   - Add to Wine/Steam tab
   - Integrate with existing installation flow

2. **Implement Installation Process**
   - Create Lutris-specific installation script
   - Add error handling and validation
   - Test installation process

### Week 3 Actions

1. **Integration and Testing**
   - Integrate all components
   - Test with real Lutris installations
   - Handle edge cases and errors

2. **Documentation and Polish**
   - Update documentation
   - Add user guides
   - Final testing and validation

---

## 📝 Key Insights from Research

### 1. **Lutris Structure is Well-Defined**
- Database provides reliable game information
- Configuration files contain all necessary settings
- Wine prefixes follow standard structure

### 2. **Environment Setup is Critical**
- Many environment variables required
- Wine version selection is important
- Cache paths need proper configuration

### 3. **Integration is Feasible**
- Clear access methods identified
- Standard wine prefix structure
- Reliable detection methods available

### 4. **User Experience Can Be Excellent**
- Can show game names instead of technical paths
- Can leverage existing Lutris configuration
- Can provide clear feedback and error handling

---

---

## ✅ **IMPLEMENTATION COMPLETED** - January 29, 2025

### **🎯 Final Implementation Results**
- ✅ **Wine Version Parsing**: Fixed YAML structure parsing to correctly detect `wine.version`
- ✅ **Lutris Wine Path**: Uses correct path `/home/mario/.local/share/lutris/runners/wine/[version]/bin/wine`
- ✅ **Game Detection**: Successfully queries Lutris database and lists games alphabetically
- ✅ **NSIS Installer**: Maintains interactive functionality for user installation
- ✅ **Environment Setup**: Proper Wine environment variables for Lutris prefixes
- ✅ **Debug Logging**: Comprehensive logging for troubleshooting and verification

### **🔧 Key Technical Achievement**
The main breakthrough was fixing the Wine version parsing from simple key-value search to proper YAML structure parsing that:
- Tracks indentation levels to understand nested structure
- Enters `wine:` section when found
- Looks for `version:` keys within the wine section
- Handles nested YAML structure correctly

### **📊 Test Results**
- ✅ Correctly detects Wine versions like `lutris-GE-Proton8-15-x86_64`
- ✅ Constructs proper Wine path with Lutris runners
- ✅ Uses Lutris-specific Wine instead of system Wine
- ✅ NSIS installer remains interactive
- ✅ Game detection and alphabetical listing working

---

*Last Updated: January 29, 2025*  
*Status: IMPLEMENTATION COMPLETED ✅*  
*Sources: [Lutris Forums](https://forums.lutris.net/)* 