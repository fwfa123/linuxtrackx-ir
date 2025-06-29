# 🐧 LinuxTrack Qt5 Testing - Kubuntu 25.10 Quick Start

**Target**: Kubuntu 25.10  
**Phase**: Quick validation testing  
**Time Required**: ~30-45 minutes

---

## ⚡ **QUICK START - Kubuntu 25.10**

### **Step 1: System Preparation (5 minutes)**
```bash
# Update system and install dependencies
sudo apt update && sudo apt upgrade -y

# Install Qt5 and build tools
sudo apt install -y build-essential autoconf automake libtool pkg-config
sudo apt install -y qtbase5-dev qttools5-dev qttools5-dev-tools
sudo apt install -y libqt5help5 qt5-assistant libopencv-dev libusb-1.0-0-dev

# Verify Qt5 installation
qmake-qt5 --version && qhelpgenerator-qt5 --help | head -2
```

### **Step 2: Source Setup (2 minutes)**
```bash
# Copy source to test location
cd /tmp
cp -r /mnt/Local_Git/git-repos/linuxtrack-clean-june14 ./linuxtrack-kubuntu-test
cd linuxtrack-kubuntu-test

# Clean previous builds
make clean 2>/dev/null || true
```

### **Step 3: Build Test (5-10 minutes)**
```bash
# Configure
./configure --prefix=/opt/linuxtrack

# Build (time it)
echo "Build started: $(date)"
time make -j$(nproc)
echo "Build finished: $(date)"

# Quick verification
echo "=== BUILD CHECK ==="
ls -la src/qt_gui/ltr_gui 2>/dev/null && echo "✅ ltr_gui built" || echo "❌ ltr_gui missing"
ls -la src/mickey/mickey 2>/dev/null && echo "✅ mickey built" || echo "❌ mickey missing"
ls -la src/qt_gui/help.qch 2>/dev/null && echo "✅ help files built" || echo "❌ help files missing"
```

### **Step 4: Test Installation (5 minutes)**
```bash
# Test install to temp directory
sudo rm -rf /tmp/kubuntu-install-test
make install DESTDIR=/tmp/kubuntu-install-test

echo "=== INSTALL CHECK ==="
ls -la /tmp/kubuntu-install-test/opt/linuxtrack/bin/ | grep -E "(ltr_gui|mickey)"
ls -la /tmp/kubuntu-install-test/usr/share/applications/*linux* 2>/dev/null
```

### **Step 5: System Installation (5 minutes)**
```bash
# Install to system (CAREFUL - modifies your system!)
echo "Installing to system in 5 seconds... Press Ctrl+C to cancel"
sleep 5
sudo make install

# Refresh desktop
sudo update-desktop-database
sudo gtk-update-icon-cache -f -t /usr/share/icons/hicolor

echo "✅ Installation complete!"
```

### **Step 6: Functionality Test (10 minutes)**

#### **Test 1: Desktop Integration**
```bash
echo "=== DESKTOP INTEGRATION TEST ==="
echo "1. Open K-Menu (Application Launcher)"
echo "2. Search for 'LinuxTrack'"
echo "3. You should see:"
echo "   - LinuxTrack (main app)"
echo "   - LinuxTrack Wii Server"
echo ""
echo "Result: [PASS/FAIL] - Applications visible in menu: ____"
```

#### **Test 2: Application Launch**
```bash
echo "=== APPLICATION LAUNCH TEST ==="

# Test ltr_gui
echo "Testing ltr_gui launch..."
timeout 10s /opt/linuxtrack/bin/ltr_gui --version
if [ $? -eq 0 ]; then
    echo "✅ ltr_gui version check passed"
else
    echo "❌ ltr_gui version check failed"
fi

# Test mickey  
echo "Testing mickey launch..."
timeout 10s /opt/linuxtrack/bin/mickey --version 2>/dev/null
if [ $? -eq 0 ]; then
    echo "✅ mickey version check passed"
else
    echo "❌ mickey version check failed or no --version option"
fi

echo ""
echo "Manual Test Required:"
echo "1. Launch from menu: LinuxTrack"
echo "2. Application should start without errors"
echo "3. Press F1 to test help system"
echo "4. Help window should open with Qt5 help content"
echo ""
echo "Result: [PASS/FAIL] - GUI applications work: ____"
```

#### **Test 3: Help System**
```bash
echo "=== HELP SYSTEM TEST ==="
echo "Help files present:"
ls -la /opt/linuxtrack/share/linuxtrack/help.qch 2>/dev/null && echo "✅ help.qch found" || echo "❌ help.qch missing"
ls -la /opt/linuxtrack/share/linuxtrack/help.qhc 2>/dev/null && echo "✅ help.qhc found" || echo "❌ help.qhc missing"

echo ""
echo "Manual Test Required:"
echo "1. Open LinuxTrack application"
echo "2. Press F1 or go to Help menu"
echo "3. Qt5 help window should open"
echo "4. Navigate through help topics"
echo ""
echo "Result: [PASS/FAIL] - Help system works: ____"
```

---

## 📋 **KUBUNTU 25.10 TEST REPORT**

Copy and fill out this report:

```
=== LinuxTrack Qt5 - Kubuntu 25.10 Test Report ===
Date: $(date)
Tester: [Your Name]
System: Kubuntu 25.10
Qt Version: $(qmake-qt5 -query QT_VERSION)
Kernel: $(uname -r)

BUILD PHASE:
- Configuration: [SUCCESS/FAILED]
- Build Time: [X minutes]
- Build Result: [SUCCESS/FAILED]
- Critical Components: [ALL BUILT/MISSING: list]

INSTALLATION PHASE:
- Test Install: [SUCCESS/FAILED]
- System Install: [SUCCESS/FAILED]
- Desktop Integration: [SUCCESS/FAILED]

FUNCTIONALITY PHASE:
- Menu Integration: [PASS/FAIL]
- Application Launch: [PASS/FAIL]
- Help System: [PASS/FAIL]
- Overall Functionality: [PASS/FAIL]

ISSUES FOUND:
1. [List any issues]
2. [Describe problems]

OVERALL ASSESSMENT:
Status: [PASS/FAIL/PARTIAL]
Ready for Kubuntu: [YES/NO]
Recommended: [YES/NO/WITH FIXES]

NEXT STEPS:
[What should be done next]
```

---

## 🚀 **After Testing**

### **If All Tests Pass:**
1. Document success in main progress tracker
2. Test on additional Ubuntu-based distributions  
3. Move to testing other distribution families

### **If Issues Found:**
1. Document specific problems
2. Check if issues are Kubuntu-specific
3. Report back for fixes before testing other distros

### **Quick Commands for Different Results:**

#### **Complete Success:**
```bash
echo "✅ Kubuntu 25.10: FULL SUCCESS - LinuxTrack Qt5 ready for production!"
```

#### **Partial Success:**
```bash
echo "⚠️  Kubuntu 25.10: PARTIAL SUCCESS - Core functionality works, minor issues found"
```

#### **Major Issues:**
```bash
echo "❌ Kubuntu 25.10: ISSUES FOUND - Needs fixes before proceeding"
```

---

**Start testing now! This should take about 30-45 minutes total.** 🚀

**Next distributions to test:** Ubuntu 24.04 LTS, Linux Mint, then Fedora/openSUSE 