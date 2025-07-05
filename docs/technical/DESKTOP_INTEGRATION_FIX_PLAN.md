# LinuxTrack Desktop Integration and Library Loading Fix Plan

## **Problem Summary**

### **Current State (Working)**
- ✅ **Build and install works**: Application functions correctly
- ✅ **Library loading works**: No `libwc.so.0` errors when installed normally
- ❌ **Desktop integration broken**: Applications don't appear in system menu

### **After Manual Fix (Broken)**
- ✅ **Desktop integration works**: Applications appear in system menu
- ❌ **Library loading broken**: `libwc.so.0` errors when launched from menu

## **Root Cause Analysis**

The issue is **path relativity**:

1. **Normal Installation**: 
   - Binaries installed to `/opt/bin/` (or whatever prefix)
   - Libraries installed to `/opt/lib/linuxtrack/`
   - When launched from terminal or direct path, relative paths work correctly

2. **After Manual Desktop Fix**:
   - Desktop files moved to `/usr/share/applications/`
   - When launched from system menu, the **working directory changes**
   - Relative path `../lib/linuxtrack/` now resolves incorrectly

## **Implementation Plan**

### **Phase 1: Fix Desktop Installation (Without Breaking Library Loading)**
**Status**: ✅ **COMPLETED**

**Objective**: Install desktop files to `/usr/share/applications/` while keeping current working library path resolution

**Files to Modify**:
- [x] `src/Makefile.am` - Change desktop installation directory
- [x] `src/linuxtrack.desktop` - Fix Exec path
- [x] `src/mickey.desktop` - Fix Exec path  
- [x] `src/linuxtrack-wii.desktop` - Fix Exec path
- [x] `src/linuxtrack-wine.desktop` - Fix Exec path

**Changes Needed**:
- [x] Change `desktopdir = $(datadir)/applications` to `desktopdir = /usr/share/applications`
- [x] Update Exec paths to use absolute paths that work with any prefix
- [x] Ensure Icon paths are correct

**Changes Made**:
1. **src/Makefile.am**: Changed `desktopdir = $(datadir)/applications` to `desktopdir = /usr/share/applications`
2. **Desktop Files**: Updated Exec paths from hardcoded `/opt/linuxtrack/bin/` to `$(bindir)/` for autotools substitution

**Validation**:
- [x] Build and install completes successfully
- [x] Desktop files appear in `/usr/share/applications/`
- [x] Applications appear in system application menu
- [x] **Library loading still works** (no regression)
- [x] Libraries registered in system library cache
- [x] Application launches successfully from command line

---

### **Phase 2: Test and Confirm Library Loading Issue**
**Status**: ⏳ **PENDING**

**Objective**: Confirm that the library loading issue reproduces after Phase 1

**Test Steps**:
- [ ] Install with Phase 1 changes
- [ ] Verify applications appear in system menu
- [ ] Launch applications from system menu
- [ ] Check for `libwc.so.0` errors in log files
- [ ] Document exact error messages and conditions

**Expected Result**: Library loading errors when launched from menu, but not from terminal

---

### **Phase 3: Fix Library Path Resolution**
**Status**: ⏳ **PENDING**

**Objective**: Make library path resolution work regardless of working directory

**Files to Modify**:
- [ ] `src/Makefile.am` - Update library path configuration
- [ ] `src/utils.c` - Improve library path resolution logic (if needed)

**Options to Consider**:
- [ ] Use absolute paths instead of relative paths
- [ ] Use `$ORIGIN` rpath to make paths relative to binary location
- [ ] Add fallback path resolution logic
- [ ] Use `dladdr()` to find library location dynamically

**Changes Needed**:
- [ ] Update `LINUXFLAGS` in Makefile.am
- [ ] Update `LINUXLDFLAGS` for proper rpath
- [ ] Test library path resolution logic

**Validation**:
- [ ] Applications launch correctly from system menu
- [ ] No library loading errors in any context
- [ ] Applications still work when launched from terminal

---

### **Phase 4: Add System Integration Hooks**
**Status**: ⏳ **PENDING**

**Objective**: Ensure proper system integration after installation

**Files to Modify**:
- [ ] `src/Makefile.am` - Add post-install hooks

**Changes Needed**:
- [ ] Add desktop database update: `update-desktop-database /usr/share/applications`
- [ ] Add icon cache update: `gtk-update-icon-cache -f -t /usr/share/icons/hicolor`
- [ ] Add icon installation to system directories
- [ ] Add error handling for non-writable system directories

**Validation**:
- [ ] Applications appear in menu immediately after installation
- [ ] Icons display correctly
- [ ] No errors during installation process

---

### **Phase 5: Comprehensive Testing**
**Status**: ⏳ **PENDING**

**Objective**: Verify complete solution works across different scenarios

**Test Scenarios**:
- [ ] **Clean Installation**: Fresh build and install
- [ ] **Different Prefixes**: Test with `--prefix=/usr/local` and `--prefix=/opt`
- [ ] **Different Distributions**: Test on Debian, Ubuntu, Arch
- [ ] **User vs System Installation**: Test both scenarios
- [ ] **Uninstall/Reinstall**: Verify clean uninstall and reinstall

**Validation Checklist**:
- [ ] Applications appear in system menu
- [ ] Applications launch and function correctly
- [ ] No library loading errors
- [ ] Icons display properly
- [ ] Desktop database updates correctly
- [ ] No regressions in existing functionality

---

## **Technical Details**

### **Current Library Path Configuration**
```makefile
# In src/Makefile.am
LINUXFLAGS = '-DLIB_PATH="../lib/linuxtrack/"'
LINUXLDLFAGS = '-Wl,-rpath,$$ORIGIN/../lib/linuxtrack'
```

### **Library Path Resolution Logic**
```c
// In src/utils.c
char *ltr_int_get_lib_path(const char *libname)
{
  char *app_path = ltr_int_my_strdup(LIB_PATH);
  char *lib_path1 = ltr_int_my_strcat(app_path, libname);
  char *lib_path = ltr_int_my_strcat(lib_path1, LIB_SUFFIX);
  // ...
}
```

### **Desktop File Structure**
```ini
# Example: src/linuxtrack.desktop
[Desktop Entry]
Version=1.0
Name=LinuxTrack
Comment=Head tracking configuration and setup
Exec=/opt/linuxtrack/bin/ltr_gui  # ← This needs to be fixed
Icon=linuxtrack
Terminal=false
Type=Application
Categories=Game;Settings;HardwareSettings;
```

---

## **Progress Tracking**

### **Completed Tasks**
- [x] Problem analysis and root cause identification
- [x] Implementation plan creation
- [x] Technical investigation of current configuration

### **Current Phase**
**Phase 1**: Fix Desktop Installation (Without Breaking Library Loading)

### **Next Steps**
1. Modify `src/Makefile.am` to install desktop files to `/usr/share/applications/`
2. Update desktop file Exec paths to use absolute paths
3. Test installation and verify desktop integration works
4. Confirm library loading still works (no regression)

---

## **Notes and Considerations**

### **Distribution Compatibility**
- Solution must work on Debian, Ubuntu, and Arch Linux
- Handle cases where `/usr/share/applications/` is not writable
- Provide fallback mechanisms for different installation scenarios

### **Backward Compatibility**
- Existing installations should continue to work
- No breaking changes to current functionality
- Graceful handling of missing system directories

### **Error Handling**
- Graceful fallbacks if system directories are not writable
- Clear error messages for troubleshooting
- Logging of installation and runtime issues

---

## **References**

- [Freedesktop Desktop Entry Specification](https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html)
- [Linux Library Path Resolution](https://man7.org/linux/man-pages/man8/ld.so.8.html)
- [Autotools Installation Directory Variables](https://www.gnu.org/software/automake/manual/html_node/Installation-Directory-Variables.html) 