# 🚀 Implementation Phase 1 Plan

**Branch**: `gaming-tab-and-lutris-install`  
**Phase**: 1 - Tab Structure Changes  
**Date**: January 2025  
**Status**: READY TO IMPLEMENT

---

## 📋 Phase 1 Implementation Overview

### Goals
1. **Rename Misc Tab to System**
2. **Add Gaming Tab**
3. **Split TIR/MFC140 functionality**
4. **Move existing wine bridge button**

### Success Criteria
- [ ] Misc tab renamed to "System"
- [ ] New Gaming tab added with proper layout
- [ ] TIR/MFC140 functionality separated from wine bridge installation
- [ ] Existing wine bridge button moved to Gaming tab
- [ ] All changes maintain backward compatibility

---

## 🔧 Implementation Steps

### Step 1: Rename Misc Tab to System

#### 1.1 Update UI File
**File**: `src/qt_gui/ltr.ui`
**Change**: Update tab title from "Misc." to "System"

```xml
<!-- Current -->
<widget class="QWidget" name="tab_4">
  <attribute name="title">
   <string>Misc.</string>
  </attribute>

<!-- New -->
<widget class="QWidget" name="tab_4">
  <attribute name="title">
   <string>System</string>
  </attribute>
```

#### 1.2 Update Code References
**Files to Check**:
- `src/qt_gui/ltr_gui.cpp` - Any references to Misc tab
- `src/qt_gui/ltr_gui.h` - Documentation updates
- `src/qt_gui/plugin_install.cpp` - Button references

### Step 2: Add Gaming Tab

#### 2.1 Add Gaming Tab to UI
**File**: `src/qt_gui/ltr.ui`
**Location**: Add before System tab (tab_4), making System the last tab

```xml
<widget class="QWidget" name="tab_5">
  <attribute name="title">
   <string>Gaming</string>
  </attribute>
  <layout class="QVBoxLayout" name="gamingLayout">
    <!-- Top: Full-width TIR/MFC140 button -->
    <item>
      <widget class="QPushButton" name="InstallTirMfcButton">
        <property name="text">
         <string>Install Linuxtrack-Wine support (TIR Firmware + MFC140)</string>
        </property>
      </widget>
    </item>
    
    <!-- Grid Row 1: Platform buttons -->
    <item>
      <layout class="QHBoxLayout" name="platformButtonsLayout">
        <item>
          <widget class="QPushButton" name="SteamProtonButton">
            <property name="text">
             <string>Steam (Proton)</string>
            </property>
          </widget>
        </item>
        <item>
          <widget class="QPushButton" name="LutrisButton">
            <property name="text">
             <string>Lutris</string>
            </property>
          </widget>
        </item>
        <item>
          <widget class="QPushButton" name="OtherPlatformButton">
            <property name="text">
             <string>Other Platform</string>
            </property>
          </widget>
        </item>
      </layout>
    </item>
    
    <!-- Grid Row 2: Utility buttons -->
    <item>
      <layout class="QHBoxLayout" name="utilityButtonsLayout">
        <item>
          <widget class="QPushButton" name="CustomPrefixButton">
            <property name="text">
             <string>Custom Prefix</string>
            </property>
          </widget>
        </item>
        <item>
          <widget class="QPushButton" name="BatchInstallButton">
            <property name="text">
             <string>Batch Install</string>
            </property>
          </widget>
        </item>
        <item>
          <widget class="QPushButton" name="WinePrefixButton">
            <property name="text">
             <string>Wine Prefix</string>
            </property>
          </widget>
        </item>
      </layout>
    </item>
    
    <!-- TrackIR Support buttons -->
    <item>
      <widget class="QPushButton" name="LaunchMickeyButton">
        <property name="text">
         <string>Launch Mickey (TrackIR)</string>
        </property>
      </widget>
    </item>
    <item>
      <widget class="QPushButton" name="LaunchLtrPipeButton">
        <property name="text">
         <string>Launch ltr_pipe for Antimicrox</string>
        </property>
      </widget>
    </item>
  </layout>
</widget>
```

#### 2.2 Update GUI Class
**File**: `src/qt_gui/ltr_gui.cpp`
**Changes**: Add button connections for new Gaming tab

```cpp
// Add to LinuxtrackGui constructor
QObject::connect(ui.InstallTirMfcButton, SIGNAL(pressed()), this, SLOT(on_InstallTirMfcButton_pressed()));
QObject::connect(ui.SteamProtonButton, SIGNAL(pressed()), this, SLOT(on_SteamProtonButton_pressed()));
QObject::connect(ui.LutrisButton, SIGNAL(pressed()), this, SLOT(on_LutrisButton_pressed()));
QObject::connect(ui.OtherPlatformButton, SIGNAL(pressed()), this, SLOT(on_OtherPlatformButton_pressed()));
QObject::connect(ui.CustomPrefixButton, SIGNAL(pressed()), this, SLOT(on_CustomPrefixButton_pressed()));
QObject::connect(ui.BatchInstallButton, SIGNAL(pressed()), this, SLOT(on_BatchInstallButton_pressed()));
QObject::connect(ui.WinePrefixButton, SIGNAL(pressed()), this, SLOT(on_WinePrefixButton_pressed()));
QObject::connect(ui.LaunchLtrPipeButton, SIGNAL(pressed()), this, SLOT(on_LaunchLtrPipeButton_pressed()));
```

#### 2.3 Add Header Declarations
**File**: `src/qt_gui/ltr_gui.h`
**Changes**: Add slot declarations

```cpp
// Add to private slots section
void on_InstallTirMfcButton_pressed();
void on_SteamProtonButton_pressed();
void on_LutrisButton_pressed();
void on_OtherPlatformButton_pressed();
void on_CustomPrefixButton_pressed();
void on_BatchInstallButton_pressed();
void on_WinePrefixButton_pressed();
void on_LaunchLtrPipeButton_pressed();
```

### Step 3: Split TIR/MFC140 Functionality

#### 3.1 Modify PluginInstall Class
**File**: `src/qt_gui/plugin_install.cpp`
**Changes**: Split `installWinePlugin()` into separate functions

```cpp
// Add new method for TIR/MFC140 only
void PluginInstall::installTirFirmwareAndMfc140()
{
    // Check if all required firmware files already exist
    if(isTirFirmwareInstalled() && isMfc140uInstalled()){
        QMessageBox::information(getParentWidget(), QString::fromUtf8("Already Installed"),
            QString::fromUtf8("TIR Firmware and MFC140 libraries are already installed."));
        return;
    }
    
    // Otherwise, proceed with the normal extraction flow
    if(!isTirFirmwareInstalled()){
        state = TIR_FW;
        tirFirmwareInstall();
    } else if(!isMfc140uInstalled()){
        // Install MFC140 (modern approach only)
        state = MFC;
        mfc140uInstall();
    } else {
        // Both already installed
        QMessageBox::information(getParentWidget(), QString::fromUtf8("Already Installed"),
            QString::fromUtf8("TIR Firmware and MFC140 libraries are already installed."));
    }
}

// Add new method for wine bridge installation only
void PluginInstall::installWineBridgeToPrefix(const QString& prefix)
{
    // Check prerequisites
    if(!isTirFirmwareInstalled() || !isMfc140uInstalled()) {
        QMessageBox::warning(getParentWidget(), QString::fromUtf8("Prerequisites Required"),
            QString::fromUtf8("Please install TIR Firmware and MFC140 libraries first using the button above."));
        return;
    }
    
    // Proceed with wine bridge installation
    installLinuxtrackWine();
}

// Helper method to get parent widget
QWidget* PluginInstall::getParentWidget()
{
    QWidget *parentWidget = nullptr;
    QObject *obj = parent();
    while (obj && !parentWidget) {
        parentWidget = qobject_cast<QWidget*>(obj);
        if (parentWidget && parentWidget->isWindow()) {
            break; // Found the main window
        }
        obj = obj->parent();
    }
    if (!parentWidget) {
        parentWidget = qobject_cast<QWidget*>(parent()); // Fallback
    }
    return parentWidget;
}
```

#### 3.2 Update Header File
**File**: `src/qt_gui/plugin_install.h`
**Changes**: Add new method declarations

```cpp
// Add to public slots section
void installTirFirmwareAndMfc140();
void installWineBridgeToPrefix(const QString& prefix);

// Add to private section
QWidget* getParentWidget();
```

### Step 4: Move Existing Wine Bridge Button

#### 4.1 Remove from System Tab
**File**: `src/qt_gui/ltr.ui`
**Change**: Remove `LinuxtrackWineButton` from System tab

#### 4.2 Add to Gaming Tab
**File**: `src/qt_gui/ltr.ui`
**Change**: Add wine bridge button to Gaming tab (Custom Prefix button)

```xml
<widget class="QPushButton" name="CustomPrefixButton">
  <property name="text">
   <string>Custom Prefix</string>
  </property>
</widget>
```

#### 4.3 Update Button Connections
**File**: `src/qt_gui/plugin_install.cpp`
**Change**: Update button connection

```cpp
// Change from LinuxtrackWineButton to CustomPrefixButton
QObject::connect(gui.CustomPrefixButton, SIGNAL(pressed()),
  this, SLOT(installWineBridgeToCustomPrefix()));
```

---

## 🧪 Testing Plan

### Test Cases

#### 1. Tab Structure Tests
- [ ] **Test 1**: Verify Misc tab renamed to System
- [ ] **Test 2**: Verify Gaming tab appears and is functional
- [ ] **Test 3**: Verify all existing buttons still work
- [ ] **Test 4**: Verify tab switching works correctly

#### 2. TIR/MFC140 Split Tests
- [ ] **Test 1**: Verify TIR/MFC140 button works independently
- [ ] **Test 2**: Verify wine bridge installation requires prerequisites
- [ ] **Test 3**: Verify error messages are clear and helpful
- [ ] **Test 4**: Verify existing workflow still works

#### 3. Button Functionality Tests
- [ ] **Test 1**: Verify Custom Prefix button works (moved wine bridge)
- [ ] **Test 2**: Verify Launch Mickey button works
- [ ] **Test 3**: Verify Launch ltr_pipe button works
- [ ] **Test 4**: Verify placeholder buttons don't crash

### Build and Test Commands

```bash
# Build the project
autoreconf -fiv
./configure --prefix=/opt --enable-debug
make -j$(nproc)

# Test the GUI
./src/qt_gui/ltr_gui

# Run specific tests
make check
```

---

## 📝 Implementation Notes

### Backward Compatibility
- **Existing Workflow**: Current wine bridge installation still works
- **Button Functionality**: All existing buttons maintain their functionality
- **Error Handling**: Same error handling patterns preserved
- **User Experience**: No breaking changes to existing users

### Error Handling
- **Prerequisites Check**: Clear messages when TIR/MFC140 not installed
- **Button States**: Disable buttons when prerequisites not met
- **User Feedback**: Informative messages for all scenarios
- **Graceful Degradation**: Fallback options for failures

### Future Considerations
- **Platform Buttons**: Placeholder implementations for future development
- **Batch Install**: Framework ready for future implementation
- **Other Platform**: Extensible design for new platforms
- **Configuration**: Easy to add new button configurations

---

## 🚀 Next Steps After Phase 1

### Phase 2 Preparation
1. **Lutris Integration**: Implement Lutris button functionality
2. **Steam Proton**: Research and implement Steam Proton integration
3. **Advanced Features**: Add batch install and other platform support

### Documentation Updates
1. **User Guide**: Update documentation for new Gaming tab
2. **Developer Guide**: Document new button implementation patterns
3. **Testing Guide**: Add testing procedures for new functionality

---

*Last Updated: January 2025*  
*Next Review: [Date + 1 week]* 