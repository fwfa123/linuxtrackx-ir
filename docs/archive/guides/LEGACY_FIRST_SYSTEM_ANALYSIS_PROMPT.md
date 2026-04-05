# LinuxTrack X-IR First System Analysis Prompt

## 🎯 Investigation Context

**Background:** We're investigating why 32-bit Tester.exe works on your first MX-Linux system but fails to connect to TrackIR on the second MX-Linux system, despite both systems having the same LinuxTrack build.

**Current Status on Second System:**
- ✅ Wine prefix architecture: Fixed (32-bit)
- ✅ Build system: Cleaned up multiple Wine installations
- ✅ Tester.exe execution: Runs without errors
- ❌ Connectivity: Still doesn't connect to TrackIR
- ❓ Hypothesis: Wine version compatibility differences

## 📋 Required Analysis Tasks

### 1. System Information Gathering

**Run these commands and record the outputs:**

```bash
# Basic system information
uname -a
lsb_release -a
cat /etc/os-release

# Current working directory and user
pwd
whoami
```

### 2. Wine Configuration Analysis

**Critical for comparison with second system:**

```bash
# Wine version (compare with second system's Wine staging 9.21)
wine --version
which wine

# Wine prefix architecture (should be 32-bit if working)
grep "#arch" ~/.wine/system.reg

# Wine prefix contents
ls -la ~/.wine/drive_c/Program\ Files/Linuxtrack/
ls -la ~/.wine/drive_c/Program\ Files\ \(x86\)/Linuxtrack/ 2>/dev/null || echo "No (x86) directory"

# Wine DLL registration
wine reg query "HKEY_CURRENT_USER\\Software\\NaturalPoint"
```

### 3. LinuxTrack Installation Analysis

```bash
# LinuxTrack version and installation
ltr_gui --version
which ltr_gui
dpkg -l | grep linuxtrack

# LinuxTrack build information
find /usr -name "*linuxtrack*" -type f 2>/dev/null | head -10

# Wine bridge components
ls -la /usr/share/linuxtrack/
file /usr/share/linuxtrack/linuxtrack-wine.exe
```

### 4. Connectivity Testing

**Test both 32-bit and 64-bit versions:**

```bash
# Start LinuxTrack GUI first
ltr_gui &

# Then test connectivity (run these in separate terminals)
export WINEPREFIX=~/.wine
wine "/home/$USER/.wine/drive_c/Program Files/Linuxtrack/Tester.exe"

# Test 64-bit version
wine "/home/$USER/.wine/drive_c/Program Files/Linuxtrack/Tester64.exe"

# Monitor logs while testing
tail -f /tmp/linuxtrack*.log
```

### 5. Package and Dependency Analysis

```bash
# Wine-related packages
dpkg -l | grep -E "(wine|libwine)" | grep -v "^rc"

# Development packages
dpkg -l | grep -E "(mingw|gcc.*multilib|lib.*dev.*i386)" | head -10

# TrackIR and USB packages
dpkg -l | grep -E "(libusb|opencv|v4l)" | head -10
```

### 6. Hardware and Kernel Analysis

```bash
# USB devices (look for TrackIR)
lsusb | grep -i trackir || lsusb | grep "131d:0159"

# Kernel modules
lsmod | grep -E "(usb|video)" | head -10

# Kernel version
uname -r
```

### 7. Process and Communication Analysis

```bash
# Check for LinuxTrack processes
ps aux | grep -E "(ltr_|linuxtrack)" | grep -v grep

# Socket communication
ls -la /tmp/ltr_* 2>/dev/null || echo "No LinuxTrack sockets found"

# Test socket if it exists
netstat -ln | grep ltr || ss -ln | grep ltr
```

## 🎯 Key Comparison Points

**Compare these with second system:**

1. **Wine Version:** What version is working on first system?
2. **Prefix Architecture:** Is it 32-bit or 64-bit?
3. **Build Process:** How was LinuxTrack built on first system?
4. **Connectivity Results:** Do both Tester.exe versions work?
5. **Package Differences:** What packages differ between systems?

## 📝 Analysis Instructions

**For each command:**
1. Run the command
2. Copy the **exact output**
3. Note any error messages
4. Test connectivity **before and after** running each test

**Important Notes:**
- Test connectivity with LinuxTrack GUI running
- Monitor `/tmp/linuxtrack*.log` files during testing
- Note any differences from second system behavior
- Include screenshots if GUI behavior differs

## 🔍 Expected Findings

**Working First System Should Show:**
- Wine version (likely different from staging 9.21)
- 32-bit Wine prefix (`#arch=win32`)
- Successful Tester.exe connectivity
- LinuxTrack socket communication working
- Proper Wine DLL registration

## 📋 Data Collection Template

**Please organize your findings as:**

```
=== FIRST MX-LINUX SYSTEM ANALYSIS ===

1. SYSTEM INFO:
[Output from uname, lsb_release, etc.]

2. WINE CONFIGURATION:
[Wine version, prefix architecture, etc.]

3. LINUXTRACK INSTALLATION:
[Version, installation path, components]

4. CONNECTIVITY TESTING:
[Results for both Tester.exe versions]

5. PACKAGE ANALYSIS:
[Wine and development packages]

6. HARDWARE/KERNEL:
[USB devices, kernel modules]

7. PROCESSES/COMMUNICATION:
[Running processes, socket status]

=== COMPARISON WITH SECOND SYSTEM ===
[Differences and potential causes]
```

## 🚀 Next Steps

After gathering this information:
1. Compare Wine versions between systems
2. Identify package differences
3. Test if second system can use first system's Wine version
4. Investigate socket communication issues
5. Check for TrackIR firmware differences

## 📊 FIRST SYSTEM TEST RESULTS

**Please paste your test results here following the template below:**

```
=== FIRST MX-LINUX SYSTEM ANALYSIS ===

1. SYSTEM INFO:
[Paste uname, lsb_release, os-release output here]

2. WINE CONFIGURATION:
[Paste wine version, which wine, prefix architecture, etc.]

3. LINUXTRACK INSTALLATION:
[Paste version, installation details, components]

4. CONNECTIVITY TESTING:
[Paste Tester.exe connectivity results and logs]

5. PACKAGE ANALYSIS:
[Paste Wine and development package lists]

6. HARDWARE/KERNEL:
[Paste USB devices, kernel modules, kernel version]

7. PROCESSES/COMMUNICATION:
[Paste LinuxTrack processes and socket status]

=== COMPARISON WITH SECOND SYSTEM ===
[Analysis of differences and potential causes]
```

---

**Reference:** See `32BIT_WINE_ARCHITECTURE_INVESTIGATION_REPORT.md` for current second system status.
