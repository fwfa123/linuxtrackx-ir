# 🧪 LinuxTrack Qt5 Cross-Platform Testing Guide

**Project**: LinuxTrack Qt4 to Qt5 Migration Testing  
**Version**: v0.99.22  
**Target**: Multi-distribution Linux testing  
**Current Status**: Ready for Phase 8 Testing & Validation

---

## 📋 **TESTING OVERVIEW**

This guide provides systematic testing procedures for LinuxTrack Qt5 across different Linux distributions. Complete all sections for thorough validation.

### **🎯 Testing Objectives:**
1. **Installation Verification**: Confirm clean installation across distributions
2. **Functionality Testing**: Verify all applications work correctly
3. **Desktop Integration**: Test system menu and icon integration
4. **Device Compatibility**: Verify hardware detection and tracking
5. **Help System**: Confirm Qt5 help system functions properly
6. **Cross-Platform**: Document distribution-specific issues

### **📊 Test Matrix:**
- **Primary Target**: Kubuntu 25.10 (Qt5 focus)
- **Secondary Targets**: Ubuntu, Fedora, openSUSE, Arch, etc.
- **Architecture**: x86_64 (primary), i386 (if available)

---

## 🔧 **PRE-TESTING SETUP**

### **Step 1: System Preparation**

#### **For Ubuntu/Debian-based Systems (Kubuntu 25.10):**
```bash
# Update system
sudo apt update && sudo apt upgrade -y

# Install build dependencies
sudo apt install -y build-essential autoconf automake libtool pkg-config
sudo apt install -y qtbase5-dev qttools5-dev qttools5-dev-tools
sudo apt install -y libqt5help5 qt5-assistant
sudo apt install -y libopencv-dev libusb-1.0-0-dev

# Optional: TrackIR support
sudo apt install -y libusb-dev

# Optional: Wine bridge (if testing Windows compatibility)
sudo apt install -y wine-development libwine-development-dev

# Verify Qt5 tools
which qmake-qt5 && which qhelpgenerator-qt5
```

#### **For RPM-based Systems (Fedora/openSUSE):**
```bash
# Fedora
sudo dnf groupinstall -y "Development Tools"
sudo dnf install -y autoconf automake libtool pkgconfig
sudo dnf install -y qt5-qtbase-devel qt5-qttools-devel
sudo dnf install -y opencv-devel libusb1-devel

# openSUSE
sudo zypper install -y pattern-devel-basis
sudo zypper install -y autoconf automake libtool pkg-config
sudo zypper install -y libqt5-qtbase-devel libqt5-qttools-devel
sudo zypper install -y opencv-devel libusb-1_0-devel
```

#### **For Arch-based Systems:**
```bash
sudo pacman -S base-devel autoconf automake libtool pkgconf
sudo pacman -S qt5-base qt5-tools qt5-doc
sudo pacman -S opencv libusb
```

### **Step 2: Source Code Preparation**
```bash
# Clone or copy the LinuxTrack source
cd /tmp
cp -r /path/to/linuxtrack-clean-june14 ./linuxtrack-test
cd linuxtrack-test

# Verify branch
git status
# Should show: On branch installation-system-completion

# Clean any previous builds
make clean 2>/dev/null || true
rm -f config.log config.status
```

---

## 🔨 **PHASE 1: BUILD TESTING**

### **Step 1: Configuration Test**
```bash
# Test configure script
./configure --prefix=/opt/linuxtrack

# Verify configuration
echo "Configuration Results:"
echo "- Prefix: $(grep '^prefix=' config.status)"
echo "- Qt5 Support: $(grep -i 'qt.*yes' config.log | head -3)"
echo "- OpenCV Support: $(grep -i 'opencv.*yes' config.log)"
echo "- TrackIR Support: $(grep -i 'trackir.*yes' config.log)"
```

### **Step 2: Build Test**
```bash
# Build with timing
echo "Starting build at $(date)"
time make -j$(nproc)
echo "Build completed at $(date)"

# Check for critical build results
echo -e "\n=== BUILD VERIFICATION ==="
echo "Main applications built:"
find . -name "ltr_gui" -executable 2>/dev/null | head -1
find . -name "mickey" -executable 2>/dev/null | head -1

echo -e "\nHelp files generated:"
find . -name "help.qch" 2>/dev/null
find . -name "help.qhc" 2>/dev/null

echo -e "\nCore libraries built:"
find . -name "liblinuxtrack*" 2>/dev/null | head -3
```

### **Step 3: Build Results Documentation**
```bash
# Create build report
cat > build_report_$(lsb_release -si)_$(lsb_release -sr).txt << EOF
=== LinuxTrack Qt5 Build Report ===
Date: $(date)
Distribution: $(lsb_release -d)
Kernel: $(uname -r)
Architecture: $(uname -m)

Build Status: $(echo $?)
Configuration: SUCCESS
Build Time: [Record from above]

Critical Components:
- ltr_gui: $(find . -name "ltr_gui" -executable >/dev/null && echo "BUILT" || echo "MISSING")
- mickey: $(find . -name "mickey" -executable >/dev/null && echo "BUILT" || echo "MISSING")
- Help System: $(find . -name "help.qch" >/dev/null && echo "BUILT" || echo "MISSING")
- Core Libraries: $(find . -name "liblinuxtrack*" >/dev/null && echo "BUILT" || echo "MISSING")

Issues Found:
[Document any build warnings or errors here]
EOF
```

---

## 📦 **PHASE 2: INSTALLATION TESTING**

### **Step 1: Test Installation**
```bash
# Test installation to temporary directory first
sudo rm -rf /tmp/linuxtrack-install-test
make install DESTDIR=/tmp/linuxtrack-install-test

echo -e "\n=== INSTALLATION VERIFICATION ==="
echo "Main applications installed:"
ls -la /tmp/linuxtrack-install-test/opt/linuxtrack/bin/ | grep -E "(ltr_gui|mickey)"

echo -e "\nLibraries installed:"
ls -la /tmp/linuxtrack-install-test/opt/linuxtrack/lib/linuxtrack/ | head -5

echo -e "\nDesktop integration files:"
ls -la /tmp/linuxtrack-install-test/usr/share/applications/*linuxtrack* 2>/dev/null || echo "No desktop files found"

echo -e "\nudev rules installed:"
ls -la /tmp/linuxtrack-install-test/lib/udev/rules.d/*TIR* 2>/dev/null || echo "No udev rules found"
```

### **Step 2: Actual System Installation**
```bash
# WARNING: This modifies your system!
echo "About to install LinuxTrack Qt5 to system..."
echo "Press Ctrl+C to cancel, or Enter to continue..."
read

# Perform actual installation
sudo make install

echo -e "\n=== SYSTEM INSTALLATION VERIFICATION ==="
echo "Installed applications:"
ls -la /opt/linuxtrack/bin/ | grep -E "(ltr_gui|mickey)"

echo -e "\nDesktop integration:"
ls -la /usr/share/applications/*linuxtrack* 2>/dev/null

echo -e "\nudev rules:"
ls -la /lib/udev/rules.d/*TIR* 2>/dev/null

# Reload desktop database
sudo update-desktop-database /usr/share/applications 2>/dev/null || true
sudo gtk-update-icon-cache -f -t /usr/share/icons/hicolor 2>/dev/null || true
```

---

## 🖥️ **PHASE 3: DESKTOP INTEGRATION TESTING**

### **Step 1: Menu Integration Test**
```bash
echo "=== DESKTOP INTEGRATION TEST ==="
echo "1. Open your application menu (K-menu in Kubuntu)"
echo "2. Search for 'LinuxTrack'"
echo "3. Verify these applications appear:"
echo "   - LinuxTrack (main application)"
echo "   - LinuxTrack Wii Server (if applicable)"
echo ""
echo "4. Check application categories:"
echo "   - Should appear in Games > Settings or similar"
echo ""
echo "5. Test icon display:"
echo "   - Icons should display correctly in menu"
echo "   - No missing icon placeholders"
```

### **Step 2: Launch from Menu Test**
```bash
echo "=== MENU LAUNCH TEST ==="
echo "1. Launch LinuxTrack from application menu"
echo "2. Note startup time and any error messages"
echo "3. If application doesn't start, check terminal:"

# Test command-line launch
echo "Testing command-line launch:"
/opt/linuxtrack/bin/ltr_gui --version 2>&1 || echo "Version check failed"
```

---

## 🚀 **PHASE 4: APPLICATION FUNCTIONALITY TESTING**

### **Step 1: LinuxTrack GUI (ltr_gui) Testing**
```bash
echo "=== LTR_GUI FUNCTIONALITY TEST ==="
echo "Manual testing required:"
echo ""
echo "1. APPLICATION STARTUP:"
echo "   - Launch: /opt/linuxtrack/bin/ltr_gui"
echo "   - Should start without errors"
echo "   - GUI should display properly"
echo ""
echo "2. HELP SYSTEM TEST:"
echo "   - Press F1 or go to Help menu"
echo "   - Qt5 help window should open"
echo "   - Help content should display correctly"
echo "   - Images and formatting should work"
echo ""
echo "3. CONFIGURATION INTERFACE:"
echo "   - Test basic settings navigation"
echo "   - Verify device detection works"
echo "   - Check that GUI responds properly"
echo ""
echo "4. RESOURCE LOADING:"
echo "   - Check for missing graphics errors"
echo "   - Verify all UI elements display correctly"
```

### **Step 2: Mickey Application Testing**
```bash
echo "=== MICKEY APPLICATION TEST ==="
echo "Manual testing required:"
echo ""
echo "1. APPLICATION STARTUP:"
echo "   - Launch: /opt/linuxtrack/bin/mickey"
echo "   - Should start without errors"
echo ""
echo "2. HELP SYSTEM TEST:"
echo "   - Test Mickey's help system"
echo "   - Verify Qt5 help integration"
echo ""
echo "3. FUNCTIONALITY:"
echo "   - Test basic Mickey operations"
echo "   - Verify companion app features work"
```

### **Step 3: Device Detection Testing**
```bash
echo "=== DEVICE DETECTION TEST ==="
echo "If you have tracking hardware:"
echo ""
echo "1. TRACKIR DEVICES:"
echo "   - Connect TrackIR device"
echo "   - Check: lsusb | grep -i track"
echo "   - Verify device recognition in LinuxTrack"
echo ""
echo "2. WEBCAM TESTING:"
echo "   - Test webcam detection"
echo "   - Verify OpenCV face tracking"
echo ""
echo "3. OTHER DEVICES:"
echo "   - Test any other supported hardware"
echo "   - Document detection results"
```

---

## 📋 **PHASE 5: RESULTS DOCUMENTATION**

### **Create Test Report**
```bash
# Create comprehensive test report
cat > linuxtrack_test_report_$(date +%Y%m%d_%H%M).txt << EOF
=== LinuxTrack Qt5 Cross-Platform Test Report ===
Test Date: $(date)
Tester: [Your Name]
Distribution: $(lsb_release -d)
Kernel: $(uname -r)
Architecture: $(uname -m)
Qt Version: $(qmake-qt5 -query QT_VERSION 2>/dev/null || echo "Unknown")

=== BUILD PHASE ===
Build Status: [SUCCESS/FAILED]
Build Time: [X minutes]
Issues: [None/List issues]

=== INSTALLATION PHASE ===
Installation Status: [SUCCESS/FAILED]
Files Installed Correctly: [YES/NO]
Desktop Integration: [SUCCESS/FAILED]
udev Rules: [INSTALLED/NOT INSTALLED]

=== FUNCTIONALITY PHASE ===
ltr_gui Launch: [SUCCESS/FAILED]
ltr_gui Help System: [WORKING/BROKEN]
ltr_gui Interface: [FUNCTIONAL/ISSUES]

mickey Launch: [SUCCESS/FAILED]
mickey Help System: [WORKING/BROKEN]
mickey Interface: [FUNCTIONAL/ISSUES]

=== DEVICE TESTING ===
Device Detection: [TESTED/NOT TESTED]
TrackIR Support: [WORKING/NOT TESTED]
Webcam Support: [WORKING/NOT TESTED]

=== DESKTOP INTEGRATION ===
Menu Entries: [VISIBLE/MISSING]
Icon Display: [CORRECT/BROKEN]
Launch from Menu: [WORKING/FAILED]

=== OVERALL ASSESSMENT ===
Overall Status: [PASS/FAIL/PARTIAL]
Ready for Production: [YES/NO/WITH FIXES]

=== ISSUES FOUND ===
1. [List any issues]
2. [Describe problems]
3. [Note missing features]

=== RECOMMENDATIONS ===
1. [Suggested fixes]
2. [Improvements needed]
3. [Distribution-specific notes]
EOF

echo "Test report created: linuxtrack_test_report_$(date +%Y%m%d_%H%M).txt"
```

---

## 🔧 **TROUBLESHOOTING GUIDE**

### **Common Build Issues:**

#### **Qt5 Not Found:**
```bash
# Ubuntu/Debian
sudo apt install qtbase5-dev qttools5-dev

# Fedora
sudo dnf install qt5-qtbase-devel qt5-qttools-devel

# Verify Qt5
qmake-qt5 --version
```

#### **Missing Dependencies:**
```bash
# Check config.log for specific missing packages
grep -i "not found\|missing" config.log

# Common fixes:
sudo apt install libopencv-dev libusb-1.0-0-dev  # Ubuntu
sudo dnf install opencv-devel libusb1-devel      # Fedora
```

#### **Help System Generation Fails:**
```bash
# Verify qhelpgenerator-qt5 exists
which qhelpgenerator-qt5

# Manual help generation test
cd src/qt_gui
qhelpgenerator-qt5 ltr_gui.qhp -o help.qch
```

### **Runtime Issues:**

#### **Application Won't Start:**
```bash
# Check library dependencies
ldd /opt/linuxtrack/bin/ltr_gui

# Check for missing Qt5 plugins
export QT_DEBUG_PLUGINS=1
/opt/linuxtrack/bin/ltr_gui
```

#### **Desktop Integration Problems:**
```bash
# Manually update desktop database
sudo update-desktop-database
sudo gtk-update-icon-cache -f -t /usr/share/icons/hicolor
```

---

## 📊 **MULTI-DISTRIBUTION TESTING MATRIX**

### **Kubuntu 25.10 Testing Checklist:**
- [ ] Build Success
- [ ] Installation Success  
- [ ] Desktop Integration
- [ ] ltr_gui Functionality
- [ ] mickey Functionality
- [ ] Help System
- [ ] Device Detection
- [ ] Overall Assessment

### **Additional Distributions to Test:**
- [ ] Ubuntu 24.04 LTS
- [ ] Fedora 40/41
- [ ] openSUSE Leap/Tumbleweed
- [ ] Arch Linux
- [ ] Debian 12
- [ ] Linux Mint

### **Test Results Summary:**
```bash
# Create summary after testing multiple distributions
cat > cross_platform_summary.txt << EOF
=== LinuxTrack Qt5 Cross-Platform Summary ===

Distributions Tested: [Number]
Successful Builds: [Number]
Successful Installations: [Number]
Fully Functional: [Number]

Best Compatibility: [Distribution name]
Issues Found: [Summary]
Recommended Target: [Distribution recommendation]
EOF
```

---

## 🎯 **NEXT STEPS AFTER TESTING**

### **If Testing Succeeds:**
1. Update `INSTALLATION_PROGRESS_TRACKER.md` to 100% complete
2. Document successful distributions
3. Create installation packages for major distros
4. Prepare release documentation

### **If Issues Found:**
1. Document specific problems in detail
2. Create bug reports with reproduction steps
3. Prioritize fixes based on severity
4. Retest after fixes applied

### **Distribution-Specific Notes:**
- Document any special requirements per distribution
- Note version-specific compatibility issues
- Create distribution-specific installation guides

---

**Start your testing with Kubuntu 25.10 and work through each phase systematically. Good luck!** 🚀 