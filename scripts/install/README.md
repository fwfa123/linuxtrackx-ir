# 📦 Installation Scripts Directory

This directory contains all installation and setup scripts for LinuxTrack X-IR, organized by purpose and distribution.

---

## 🚀 Quick Start

### **Arch Linux / Manjaro Users**
```bash
# One-command installation (no Wine development tools required)
./scripts/install/install_arch_prebuilt.sh
```

### **Other Distributions**
```bash
# Standard installation with Wine development tools
./scripts/install/linuxtrack_install.sh
```

---

## 📋 Scripts Overview

### **Distribution-Specific Installers**

#### **Arch Linux / Manjaro**
- **[install_arch_prebuilt.sh](install_arch_prebuilt.sh)** - Complete Arch Linux installation using prebuilt Wine bridge components
  - **Purpose**: Install LinuxTrack without Wine development tools
  - **Status**: Production Ready
  - **Features**: Automated dependency installation, package download, build system modification

#### **General Installation**
- **[linuxtrack_install.sh](linuxtrack_install.sh)** - General LinuxTrack installation script
  - **Purpose**: Standard installation with Wine development tools
  - **Status**: Production Ready
  - **Features**: Dependency detection, build automation

### **Wine Bridge Components**

#### **Package Creation**
- **[create_wine_bridge_package.sh](create_wine_bridge_package.sh)** - Create prebuilt Wine bridge package
  - **Purpose**: Build and package Wine bridge components for distribution
  - **Status**: Production Ready
  - **Features**: Component building, package creation, metadata generation

#### **Build System Modification**
- **[modify_build_for_prebuilt.sh](modify_build_for_prebuilt.sh)** - Modify build system for prebuilt components
  - **Purpose**: Enable build system to use prebuilt Wine bridge components
  - **Status**: Production Ready
  - **Features**: Makefile modification, graceful fallback, component detection

### **NSIS Installation**

#### **Arch Linux NSIS**
- **[install_nsis_arch.sh](install_nsis_arch.sh)** - Install NSIS on Arch Linux
  - **Purpose**: Install NSIS with multiple fallback methods
  - **Status**: Production Ready
  - **Features**: AUR package installation, manual download, verification

#### **Documentation**
- **[README_NSIS_ARCH.md](README_NSIS_ARCH.md)** - NSIS installation guide for Arch Linux
  - **Purpose**: Document NSIS installation process
  - **Status**: Production Ready
  - **Content**: Installation methods, troubleshooting, verification

### **Winetricks Setup**
- **[install_winetricks.sh](install_winetricks.sh)** - Install Winetricks and required components
  - **Purpose**: Setup Winetricks for Wine compatibility
  - **Status**: Production Ready
  - **Features**: Winetricks installation, MFC42 setup, verification

### **Post-Installation**
- **[post_install_dialog.sh](post_install_dialog.sh)** - Post-installation setup dialog
  - **Purpose**: Guide users through post-installation configuration
  - **Status**: Production Ready
  - **Features**: Interactive setup, group management, verification

### **Verification and Health Checks**
- **[verify_installation.sh](verify_installation.sh)** - Comprehensive installation verification
  - **Purpose**: Verify complete LinuxTrack installation
  - **Status**: Production Ready
  - **Features**: Component verification, dependency checking, health assessment

- **[linuxtrack_health_check.sh](linuxtrack_health_check.sh)** - System health verification
  - **Purpose**: Check system health and LinuxTrack status
  - **Status**: Production Ready
  - **Features**: Health assessment, problem detection, repair suggestions

- **[linuxtrack_quick_recovery.sh](linuxtrack_quick_recovery.sh)** - Quick system recovery
  - **Purpose**: Recover from common installation issues
  - **Status**: Production Ready
  - **Features**: Automatic recovery, problem resolution, system repair

### **Setup and Configuration**
- **[setup.sh](setup.sh)** - General setup and configuration
  - **Purpose**: Setup LinuxTrack environment and configuration
  - **Status**: Production Ready
  - **Features**: Environment setup, configuration management, initialization

### **Documentation**
- **[ARCH_LINUX_INSTALL_GUIDE.md](ARCH_LINUX_INSTALL_GUIDE.md)** - Arch Linux installation guide
  - **Purpose**: Comprehensive Arch Linux installation documentation
  - **Status**: Production Ready
  - **Content**: Step-by-step instructions, troubleshooting, verification

---

## 🎯 Usage by Distribution

### **Arch Linux / Manjaro**
```bash
# Recommended: Prebuilt installation (no Wine development tools)
./scripts/install/install_arch_prebuilt.sh

# Alternative: Standard installation (requires Wine development tools)
./scripts/install/linuxtrack_install.sh
```

### **Ubuntu / Debian / MX Linux**
```bash
# Standard installation
./scripts/install/linuxtrack_install.sh

# Or manual installation
sudo apt install wine-devel
autoreconf -fiv
./configure --prefix=/opt
make -j$(nproc)
sudo make install
```

### **Fedora / RHEL / CentOS**
```bash
# Standard installation
./scripts/install/linuxtrack_install.sh

# Or manual installation
sudo dnf install wine-devel
autoreconf -fiv
./configure --prefix=/opt
make -j$(nproc)
sudo make install
```

---

## 🔧 Script Development

### **Creating New Scripts**
1. **Follow naming convention**: `purpose_distribution.sh` or `purpose_general.sh`
2. **Include header**: Script purpose, date, status
3. **Add error handling**: Use `set -e` and proper error checking
4. **Include documentation**: Add comments and usage examples
5. **Test thoroughly**: Test on target distributions

### **Script Categories**
- **Installation**: Complete installation procedures
- **Setup**: Environment and configuration setup
- **Verification**: Health checks and verification
- **Recovery**: Problem resolution and recovery
- **Documentation**: Guides and documentation

### **Status Levels**
- **Development**: Under development, not ready for use
- **Testing**: Ready for testing, may have issues
- **Production Ready**: Tested and ready for production use
- **Deprecated**: No longer maintained, use alternatives

---

## 📚 Related Documentation

### **Installation Guides**
- **[Wine Bridge Installation Guide](../../docs/guides/WINE_BRIDGE_INSTALLATION_GUIDE.md)** - Complete Wine bridge installation guide
- **[Main README](../../README.md)** - Project overview and quick start
- **[Arch Linux Guide](ARCH_LINUX_INSTALL_GUIDE.md)** - Arch Linux specific installation

### **Technical Documentation**
- **[Wine Bridge Packaging Plan](../../docs/technical/WINE_BRIDGE_PACKAGING_PLAN.md)** - Technical implementation details
- **[Troubleshooting Guide](../../docs/troubleshooting/TROUBLESHOOTING.md)** - Problem resolution
- **[Testing Guide](../../docs/testing/README.md)** - Testing procedures

### **Scripts in Other Directories**
- **[wine_check.sh](../wine_check.sh)** - Wine compatibility checker
- **[Health check scripts](../debug/)** - Additional health check scripts
- **[Build scripts](../build/)** - Build automation scripts

---

## 🎯 Quick Reference

### **Most Common Commands**
```bash
# Arch Linux installation
./scripts/install/install_arch_prebuilt.sh

# Health check
./scripts/install/linuxtrack_health_check.sh

# Quick recovery
./scripts/install/linuxtrack_quick_recovery.sh

# Installation verification
./scripts/install/verify_installation.sh
```

### **Troubleshooting**
```bash
# Check system health
./scripts/install/linuxtrack_health_check.sh

# Recover from issues
./scripts/install/linuxtrack_quick_recovery.sh

# Verify installation
./scripts/install/verify_installation.sh
```

---

**For detailed installation instructions, see the [Wine Bridge Installation Guide](../../docs/guides/WINE_BRIDGE_INSTALLATION_GUIDE.md).** 