# 🍷 Wine Version Selection Implementation

**Date**: January 2025  
**Purpose**: Automatic selection of the newest compatible wine and winetricks versions for LinuxTrack extractors  
**Status**: **IMPLEMENTATION COMPLETE**

---

## 📋 Overview

This implementation adds automatic wine version selection to the LinuxTrack GUI, ensuring that the extractors always use the newest available compatible wine and winetricks versions. This addresses the issue where users had to manually configure wine versions for optimal TrackIR firmware extraction.

### **Key Requirements**
- **Wine**: Version 9.0 or higher (wine-staging preferred)
- **Winetricks**: Version 20240105-next or higher
- **Automatic Detection**: Finds and selects the best available versions
- **Fallback Logic**: Graceful degradation when optimal versions aren't available

---

## 🎯 Implementation Details

### **1. Enhanced WineLauncher Class**

#### **New Methods Added**
```cpp
// Wine version selection
QString selectBestWineVersion();
bool isWineVersionCompatible(const QString &winePath);
QString getWineVersion(const QString &winePath);
void setWinePath(const QString &path);

// Winetricks compatibility
bool isWinetricksCompatible();
QString getWinetricksVersion();

// Version parsing helpers
bool parseWineVersion(const QString &versionString, int &major, int &minor);
bool parseWinetricksVersion(const QString &versionString, int &year, int &month, int &day);
bool isVersionGreaterOrEqual(int major1, int minor1, int major2, int minor2);
bool isDateGreaterOrEqual(int year1, int month1, int day1, int year2, int month2, int day2);
```

#### **Wine Version Detection Logic**
The system searches for wine versions in this priority order:

1. **Wine Staging Paths** (Preferred):
   - `/opt/wine-staging/bin/wine`
   - `/usr/local/bin/wine-staging`

2. **Regular Wine Paths**:
   - `/opt/wine/bin/wine`
   - `/usr/local/bin/wine`

3. **System Commands**:
   - `wine-staging`
   - `wine-development`
   - `wine`

#### **Version Compatibility Check**
- **Minimum Requirement**: Wine 9.0+
- **Preference**: Wine Staging over regular Wine
- **Selection**: Newest compatible version found

### **2. Enhanced PluginInstall Class**

#### **Updated installWinePlugin() Method**
```cpp
void PluginInstall::installWinePlugin()
{
    // NEW: Check wine and winetricks compatibility before proceeding
    if (!inst->isWineVersionCompatible(inst->selectBestWineVersion())) {
        // Show error dialog with installation instructions
        return;
    }
    
    if (!inst->isWinetricksCompatible()) {
        // Show error dialog with winetricks update instructions
        return;
    }
    
    // Proceed with normal installation flow...
}
```

#### **Compatibility Verification**
- **Wine Check**: Verifies wine 9.0+ is available
- **Winetricks Check**: Verifies winetricks 20240105-next+ is available
- **User Guidance**: Provides specific installation instructions for incompatible versions

### **3. Enhanced Extractor Classes**

#### **Updated Mfc42uExtractor**
- Uses the selected best wine version for wine prefix initialization
- Improved error handling and user feedback
- Better winetricks integration

#### **Updated TirFwExtractor**
- Uses the selected best wine version for firmware extraction
- Enhanced compatibility with newer wine versions

---

## 🔧 Technical Implementation

### **Version Parsing**

#### **Wine Version Parsing**
```cpp
// Input: "wine-9.0.1" -> major=9, minor=0
bool WineLauncher::parseWineVersion(const QString &versionString, int &major, int &minor)
{
    QRegularExpression versionRegex(R"((\d+)\.(\d+)(?:\.\d+)?)");
    QRegularExpressionMatch match = versionRegex.match(versionString);
    
    if (match.hasMatch()) {
        major = match.captured(1).toInt();
        minor = match.captured(2).toInt();
        return true;
    }
    return false;
}
```

#### **Winetricks Version Parsing**
```cpp
// Input: "20240105-next" -> year=2024, month=1, day=5
bool WineLauncher::parseWinetricksVersion(const QString &versionString, int &year, int &month, int &day)
{
    QRegularExpression dateRegex(R"((\d{4})(\d{2})(\d{2})(?:-next)?)");
    QRegularExpressionMatch match = dateRegex.match(versionString);
    
    if (match.hasMatch()) {
        year = match.captured(1).toInt();
        month = match.captured(2).toInt();
        day = match.captured(3).toInt();
        return true;
    }
    return false;
}
```

### **Version Comparison**

#### **Wine Version Comparison**
```cpp
bool WineLauncher::isVersionGreaterOrEqual(int major1, int minor1, int major2, int minor2)
{
    if (major1 > major2) return true;
    if (major1 < major2) return false;
    return minor1 >= minor2;
}
```

#### **Date Comparison**
```cpp
bool WineLauncher::isDateGreaterOrEqual(int year1, int month1, int day1, int year2, int month2, int day2)
{
    if (year1 > year2) return true;
    if (year1 < year2) return false;
    if (month1 > month2) return true;
    if (month1 < month2) return false;
    return day1 >= day2;
}
```

---

## 🚀 Usage Instructions

### **For Users**

#### **Automatic Operation**
1. **Launch LinuxTrack GUI**
2. **Navigate to Misc tab**
3. **Click "Install Linuxtrack-Wine support..."**
4. **System automatically:**
   - Detects available wine versions
   - Selects the best compatible version
   - Verifies winetricks compatibility
   - Proceeds with installation or shows guidance

#### **Manual Testing**
```bash
# Test wine version selection logic
cd linuxtrackx-ir
./scripts/test_wine_selection.sh
```

### **For Developers**

#### **Building with Wine Selection**
```bash
# Standard build process (wine selection is automatic)
autoreconf -fiv
./configure --prefix=/opt
make -j$(nproc)
sudo make install
```

#### **Testing Wine Selection**
```bash
# Run the test script to verify detection
./scripts/test_wine_selection.sh

# Expected output for compatible system:
# ✓ Best wine version: /opt/wine-staging/bin/wine (v9.0.1)
# ✓ Best winetricks version: /usr/local/bin/winetricks (v20240105-next)
# 🎉 EXCELLENT: Both wine and winetricks are compatible
```

---

## 🔍 Troubleshooting

### **Common Issues**

#### **1. Wine Version Too Old**
**Symptoms**: Error dialog "Wine Version Incompatible"

**Solutions**:
```bash
# Ubuntu/Debian/MX
sudo apt install wine-staging
sudo update-alternatives --config wine

# Fedora
sudo dnf install wine-staging

# Arch
sudo pacman -S wine-staging
```

#### **2. Winetricks Version Too Old**
**Symptoms**: Error dialog "Winetricks Version Incompatible"

**Solutions**:
```bash
# Method 1: Install latest version
wget https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks
chmod +x winetricks
sudo mv winetricks /usr/local/bin/

# Method 2: Update package
sudo apt update && sudo apt install winetricks
```

#### **3. No Compatible Wine Found**
**Symptoms**: "No compatible wine version found"

**Solutions**:
```bash
# Check available wine versions
./scripts/wine_check.sh

# Install wine-staging if not available
sudo apt install wine-staging  # Ubuntu/Debian/MX
sudo dnf install wine-staging  # Fedora
sudo pacman -S wine-staging    # Arch
```

### **Debug Information**

#### **Enable Debug Output**
The wine selection process logs detailed information to the LinuxTrack log:

```bash
# View log output
ltr_gui 2>&1 | grep -i wine
```

#### **Test Script Output**
```bash
# Run comprehensive test
./scripts/test_wine_selection.sh

# Look for these key indicators:
# ✓ Compatible version (9.0+)
# ✓ Best wine version: [path] (v[major].[minor])
# 🎉 EXCELLENT: Both wine and winetricks are compatible
```

---

## 📊 Compatibility Matrix

### **Supported Wine Versions**
| Version | Status | Notes |
|---------|--------|-------|
| Wine 9.0+ | ✅ **Fully Supported** | Recommended for best compatibility |
| Wine 8.0-8.x | ❌ **Not Supported** | Too old for extractor requirements |
| Wine 7.0-7.x | ❌ **Not Supported** | Too old for extractor requirements |

### **Supported Winetricks Versions**
| Version | Status | Notes |
|---------|--------|-------|
| 20240105-next+ | ✅ **Fully Supported** | Recommended for best compatibility |
| 20240105 | ✅ **Supported** | Minimum required version |
| 20231201-20231231 | ❌ **Not Supported** | Too old for extractor requirements |

### **Distribution Support**
| Distribution | Wine Staging | Regular Wine | Winetricks | Status |
|--------------|--------------|--------------|------------|---------|
| **Ubuntu 22.04+** | ✅ Available | ✅ Available | ✅ Available | **FULL SUPPORT** |
| **Debian 12+** | ✅ Available | ✅ Available | ✅ Available | **FULL SUPPORT** |
| **MX Linux 23+** | ✅ Available | ✅ Available | ✅ Available | **FULL SUPPORT** |
| **Fedora 38+** | ✅ Available | ✅ Available | ✅ Available | **FULL SUPPORT** |
| **Arch Linux** | ✅ Available | ✅ Available | ✅ Available | **FULL SUPPORT** |
| **Manjaro** | ✅ Available | ✅ Available | ✅ Available | **FULL SUPPORT** |

---

## 🎯 Benefits

### **For Users**
- ✅ **Automatic Optimization**: No manual wine configuration required
- ✅ **Better Compatibility**: Always uses the best available wine version
- ✅ **Clear Guidance**: Specific instructions when versions are incompatible
- ✅ **Reliable Extraction**: Improved success rate for TrackIR firmware extraction

### **For Developers**
- ✅ **Robust Detection**: Comprehensive wine version detection logic
- ✅ **Extensible Design**: Easy to add support for new wine versions
- ✅ **Comprehensive Testing**: Test script validates detection logic
- ✅ **Clear Documentation**: Detailed implementation and usage guides

---

## 🔮 Future Enhancements

### **Planned Improvements**
1. **Proton Integration**: Support for Steam Proton wine versions
2. **Custom Wine Paths**: User-configurable wine installation paths
3. **Version Notifications**: Alert users when newer wine versions are available
4. **Automatic Updates**: Integration with system package managers for updates

### **Potential Extensions**
1. **Wine Prefix Management**: Automatic wine prefix optimization
2. **Game-Specific Profiles**: Different wine versions for different games
3. **Performance Monitoring**: Track wine performance and suggest optimizations

---

## 📝 Implementation Files

### **Modified Files**
- `src/qt_gui/wine_launcher.h` - Added wine version selection methods
- `src/qt_gui/wine_launcher.cpp` - Implemented wine version detection logic
- `src/qt_gui/plugin_install.cpp` - Added compatibility checks
- `src/qt_gui/extractor.cpp` - Updated to use selected wine versions

### **New Files**
- `scripts/test_wine_selection.sh` - Test script for wine version detection
- `docs/technical/WINE_VERSION_SELECTION_IMPLEMENTATION.md` - This documentation

### **Testing**
- **Test Script**: `./scripts/test_wine_selection.sh`
- **Manual Testing**: Use LinuxTrack GUI "Install Linuxtrack-Wine support..." button
- **Log Analysis**: Check LinuxTrack logs for wine selection messages

---

**Implementation Status**: ✅ **COMPLETE**  
**Testing Status**: ✅ **READY FOR TESTING**  
**Documentation Status**: ✅ **COMPLETE** 