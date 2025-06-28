# LinuxTrack Documentation Index

## 📚 Complete Documentation Guide

This index provides comprehensive navigation for all LinuxTrack Qt5 modernization documentation, troubleshooting guides, and maintenance resources.

---

## 🚀 Getting Started

### New Users
1. **[README.md](README.md)** - Main project overview and quick start guide
2. **[SETUP_GUIDE_QT5_SUCCESS.md](SETUP_GUIDE_QT5_SUCCESS.md)** - Detailed setup instructions with proven success steps
3. **[TESTING_INSTRUCTIONS.md](TESTING_INSTRUCTIONS.md)** - Comprehensive testing procedures

### Existing Users (After System Changes)
1. **[QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)** - ⚠️ **CRITICAL** - System upgrade resolution
2. **[QUICK_TROUBLESHOOTING_QT5.md](QUICK_TROUBLESHOOTING_QT5.md)** - Quick fixes for common issues
3. **[linuxtrack_health_check.sh](linuxtrack_health_check.sh)** - Automated system health verification

---

## 🛠️ Troubleshooting Guides

### By Issue Type

#### System Upgrade Issues
- **[QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)** - Complete Qt5/Qt6 conflict resolution
- **[QUICK_TROUBLESHOOTING_QT5.md](QUICK_TROUBLESHOOTING_QT5.md)** - Quick fixes including Qt version conflicts

#### Hardware Issues
- **[TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)** - Comprehensive TrackIR hardware troubleshooting
- **Hardware Detection**: See sections in main troubleshooting guides

#### Display/Compatibility Issues
- **[WAYLAND_COMPATIBILITY_ISSUE.md](WAYLAND_COMPATIBILITY_ISSUE.md)** - Wayland display server compatibility
- **[run_qt5_gui_x11.sh](run_qt5_gui_x11.sh)** - X11 compatibility launcher

#### Build/Library Issues
- **[QUICK_TROUBLESHOOTING_QT5.md](QUICK_TROUBLESHOOTING_QT5.md)** - Library loading and build errors
- **[QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)** - Build system problems

### By Severity

#### Critical Issues (System Broken)
1. **[linuxtrack_quick_recovery.sh](linuxtrack_quick_recovery.sh)** - Automated recovery script
2. **[QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)** - System upgrade conflicts
3. **[TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)** - Complex permission issues

#### Warning Issues (Functional but Problematic)
1. **[QUICK_TROUBLESHOOTING_QT5.md](QUICK_TROUBLESHOOTING_QT5.md)** - Common warning fixes
2. **[WAYLAND_COMPATIBILITY_ISSUE.md](WAYLAND_COMPATIBILITY_ISSUE.md)** - Display compatibility
3. **[linuxtrack_health_check.sh](linuxtrack_health_check.sh)** - Health monitoring

#### Informational Issues (Cosmetic)
- See "Known Issues" sections in main guides

---

## 🔧 Maintenance and Monitoring

### Regular Maintenance
- **[SYSTEM_MAINTENANCE_GUIDE.md](SYSTEM_MAINTENANCE_GUIDE.md)** - Comprehensive maintenance procedures
- **[linuxtrack_health_check.sh](linuxtrack_health_check.sh)** - Automated health monitoring
- **[linuxtrack_quick_recovery.sh](linuxtrack_quick_recovery.sh)** - Quick recovery automation

### System Updates
- **[QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)** - Pre/post upgrade procedures
- **[SYSTEM_MAINTENANCE_GUIDE.md](SYSTEM_MAINTENANCE_GUIDE.md)** - Update preparation and recovery

### Performance Optimization
- **[SYSTEM_MAINTENANCE_GUIDE.md](SYSTEM_MAINTENANCE_GUIDE.md)** - Performance tuning section
- **[SETUP_GUIDE_QT5_SUCCESS.md](SETUP_GUIDE_QT5_SUCCESS.md)** - Optimal configuration

---

## 📊 Status and Progress Documentation

### Current Status
- **[SUCCESS_SUMMARY.md](SUCCESS_SUMMARY.md)** - Overall project success status
- **[QT5_MODERNIZATION_STATUS.md](QT5_MODERNIZATION_STATUS.md)** - Qt5 modernization completion status
- **[WINE_BRIDGE_SUCCESS.md](WINE_BRIDGE_SUCCESS.md)** - Wine integration status

### Development Progress
- **[MODERNIZATION_PLAN.md](MODERNIZATION_PLAN.md)** - Original modernization plan
- **[QT5_MODERNIZATION_STATUS.md](QT5_MODERNIZATION_STATUS.md)** - Implementation status
- **[ChangeLog](ChangeLog)** - Version history

---

## 🚨 Emergency Procedures

### Quick Recovery (Immediate)
1. **Run automated recovery**: `./linuxtrack_quick_recovery.sh`
2. **Check status**: `./linuxtrack_health_check.sh`
3. **If still broken**: See critical troubleshooting guides

### System Upgrade Recovery
1. **[QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)** - Complete upgrade resolution
2. **Emergency commands**: See "Emergency Recovery" sections
3. **Manual recovery**: See step-by-step procedures

### Hardware Issues
1. **[TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)** - Hardware troubleshooting
2. **Basic diagnostics**: `lsusb | grep 131d` for TrackIR detection
3. **Permission fixes**: See udev rules sections

---

## 📋 Documentation by Audience

### End Users
- **[README.md](README.md)** - Main overview
- **[QUICK_TROUBLESHOOTING_QT5.md](QUICK_TROUBLESHOOTING_QT5.md)** - Quick fixes
- **[run_qt5_gui.sh](run_qt5_gui.sh)** - Application launcher
- **[linuxtrack_health_check.sh](linuxtrack_health_check.sh)** - Health check

### System Administrators
- **[SYSTEM_MAINTENANCE_GUIDE.md](SYSTEM_MAINTENANCE_GUIDE.md)** - Maintenance procedures
- **[QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)** - Upgrade management
- **[TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)** - Permission management

### Developers
- **[MODERNIZATION_PLAN.md](MODERNIZATION_PLAN.md)** - Technical architecture
- **[QT5_MODERNIZATION_STATUS.md](QT5_MODERNIZATION_STATUS.md)** - Implementation details
- **[docs/](docs/)** - Technical documentation
- **[src/](src/)** - Source code

### Testers
- **[TESTING_INSTRUCTIONS.md](TESTING_INSTRUCTIONS.md)** - Comprehensive testing procedures
- **[linuxtrack_health_check.sh](linuxtrack_health_check.sh)** - Automated testing
- **[SUCCESS_SUMMARY.md](SUCCESS_SUMMARY.md)** - Expected results

---

## 🔍 Search Guide

### By Keywords

#### "Qt5" or "Qt6"
- **[QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)**
- **[QT5_MODERNIZATION_STATUS.md](QT5_MODERNIZATION_STATUS.md)**
- **[QUICK_TROUBLESHOOTING_QT5.md](QUICK_TROUBLESHOOTING_QT5.md)**

#### "TrackIR" or "Hardware"
- **[TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)**
- **[SUCCESS_SUMMARY.md](SUCCESS_SUMMARY.md)**
- **[SETUP_GUIDE_QT5_SUCCESS.md](SETUP_GUIDE_QT5_SUCCESS.md)**

#### "System Upgrade" or "Broken"
- **[QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)**
- **[linuxtrack_quick_recovery.sh](linuxtrack_quick_recovery.sh)**
- **[SYSTEM_MAINTENANCE_GUIDE.md](SYSTEM_MAINTENANCE_GUIDE.md)**

#### "Permission" or "Access Denied"
- **[TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)**
- **[SETUP_GUIDE_QT5_SUCCESS.md](SETUP_GUIDE_QT5_SUCCESS.md)**

#### "Library" or "Loading"
- **[QUICK_TROUBLESHOOTING_QT5.md](QUICK_TROUBLESHOOTING_QT5.md)**
- **[QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)**

#### "Wayland" or "Display"
- **[WAYLAND_COMPATIBILITY_ISSUE.md](WAYLAND_COMPATIBILITY_ISSUE.md)**
- **[run_qt5_gui_x11.sh](run_qt5_gui_x11.sh)**

---

## 🎯 Quick Reference

### Most Common Issues
1. **System upgrade broke LinuxTrack** → [QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md](QT5_QT6_SYSTEM_UPGRADE_TROUBLESHOOTING.md)
2. **TrackIR permission errors** → [TRACKIR_PERMISSION_TROUBLESHOOTING.md](TRACKIR_PERMISSION_TROUBLESHOOTING.md)
3. **Application won't launch** → [QUICK_TROUBLESHOOTING_QT5.md](QUICK_TROUBLESHOOTING_QT5.md)
4. **Library loading errors** → [QUICK_TROUBLESHOOTING_QT5.md](QUICK_TROUBLESHOOTING_QT5.md)
5. **Display issues on Wayland** → [WAYLAND_COMPATIBILITY_ISSUE.md](WAYLAND_COMPATIBILITY_ISSUE.md)

### Emergency Commands
```bash
# Quick health check
./linuxtrack_health_check.sh

# Automatic recovery
./linuxtrack_quick_recovery.sh

# Manual Qt5 rebuild
cd src/qt_gui && qmake-qt5 ltr_gui.pro && make -j$(nproc)

# Check TrackIR hardware
lsusb | grep 131d

# Launch application
./run_qt5_gui.sh
```

### Key File Locations
- **Executable**: `src/qt_gui/ltr_gui`
- **Libraries**: `src/.libs/lib*.so*`
- **Configuration**: `~/.config/linuxtrack/linuxtrack1.conf`
- **udev Rules**: `/lib/udev/rules.d/99-TIR.rules`
- **Launch Script**: `./run_qt5_gui.sh`

---

## 📞 Support Resources

### Self-Help (Recommended)
1. **Health Check**: `./linuxtrack_health_check.sh`
2. **Quick Recovery**: `./linuxtrack_quick_recovery.sh`
3. **Troubleshooting Guides**: See issue-specific guides above

### Documentation History
- **Created**: June 2025 during Qt5 modernization
- **Last Major Update**: System upgrade troubleshooting addition
- **Status**: Comprehensive and actively maintained

### Contributing to Documentation
- **Location**: All .md files in project root
- **Style**: Markdown with emoji indicators
- **Testing**: Verify all commands and procedures before documenting

---

**Documentation Status**: COMPREHENSIVE ✅  
**Coverage**: ALL MAJOR ISSUES DOCUMENTED ✅  
**Automation**: HEALTH CHECK AND RECOVERY SCRIPTS ✅  
**Maintenance**: SYSTEM MAINTENANCE GUIDE ✅

This documentation index provides complete navigation for all LinuxTrack resources, from basic setup to advanced troubleshooting and system maintenance. 