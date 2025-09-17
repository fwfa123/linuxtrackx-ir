# LinuxTrack X-IR v0.99.25 Release Notes

**Release Date**: September 17, 2025  
**Version**: 0.99.25  
**Codename**: "Arch Linux Wine32 Alternative Sources"

---

## 🎯 **Major Features**

### **Alternative MFC42.dll Sources for Arch Linux Users**
- **Problem Solved**: Users on Arch Linux with only wow64 (no wine32) can now install MFC42.dll libraries
- **New Sources**: Added VS6SP6.EXE and VC6RedistSetup_deu.exe as alternative download sources
- **Smart Extraction**: Automatic method selection (cabextract vs Wine installer) based on source type
- **SHA256 Verification**: All downloads verified against provided checksums for security
- **Fallback Support**: Multiple extraction methods ensure compatibility across different systems

### **Enhanced Wine Integration & Detection**
- **Lutris Wine Path Fix**: Resolved critical path doubling issue in Lutris Wine integration
- **Smart Path Resolution**: Automatic detection of absolute vs relative Wine paths
- **Enhanced YAML Parsing**: Better extraction of wine_path from installer tasks and game configurations
- **Version Extraction**: Improved Wine version detection from full executable paths
- **Runner Pattern Support**: Added support for various Wine runner patterns (lutris-GE-Proton8-15-x86_64, wine-ge-8-26-x86_64, etc.)

---

## 🔧 **Technical Improvements**

### **Build System Enhancements**
- **Automatic Installation**: `sudo make install` now handles all post-installation steps automatically
- **Library Path Configuration**: Automatic ldconfig execution with verification and retry logic
- **Icon & Desktop Integration**: Automatic installation and cache updates for desktop files
- **Wine Bridge Cleanup**: Removed version numbers from Wine bridge application titles for professional appearance

### **User Experience Improvements**
- **One-Command Installation**: Eliminated manual post-installation steps
- **Comprehensive Verification**: Enhanced post-installation verification script with clear success/failure feedback
- **Better Error Handling**: Improved error messages and troubleshooting information
- **Professional UI**: Clean application titles without confusing version numbers

### **Cross-Distribution Compatibility**
- **Arch Linux Support**: Enhanced support for Arch Linux users without wine32
- **Fedora/RHEL Improvements**: Better Wine integration guidance and package mappings
- **Debian Build Analysis**: Comprehensive documentation for Debian build issues and solutions

---

## 🐛 **Bug Fixes**

### **Critical Fixes**
- **MFC42.dll Symlink Creation**: Fixed missing symlink creation for mfc42.dll in winebridge installer
- **Lutris Wine Path Doubling**: Resolved path concatenation issue causing invalid Wine paths
- **Wine Version Detection**: Fixed Wine version detection and path resolution issues
- **NPTest Architecture**: Fixed NPTest dialog titles to show correct architecture (NPTest32/NPTest64)

### **Installation Fixes**
- **Automatic Library Loading**: Fixed library loading issues with automatic ldconfig execution
- **Desktop Integration**: Resolved icon and desktop file installation issues
- **Cache Updates**: Fixed automatic cache updates for desktop files

---

## 📋 **Detailed Changes**

### **New Files Added**
- `docs/features/0001_PLAN.md` - MFC42 alternative sources feature plan
- `docs/features/0001_REVIEW.md` - MFC42 implementation review
- `docs/DEBIAN_BUILD_ANALYSIS_AND_SOLUTIONS.md` - Comprehensive Debian build analysis
- `docs/CLEANUP_LOG.md` - Repository cleanup documentation

### **Enhanced Files**
- `src/qt_gui/extractor.cpp` - Added alternative MFC42 extraction methods
- `src/qt_gui/sources_mfc42.txt` - Added new download sources with SHA256 verification
- `src/wine_bridge/client/check_data.c` - Added mfc42.dll symlink creation
- `src/qt_gui/lutris_integration.cpp` - Enhanced Wine path resolution and YAML parsing
- `src/Makefile.am` - Added automatic installation hooks
- `scripts/post_install.sh` - Transformed to verification script

### **Configuration Updates**
- `configure.ac` - Improved Wine64 detection and build system configuration
- Wine bridge resource files - Removed version numbers from application titles
- AppImage configuration - Updated for v0.99.25

---

## 🚀 **Installation Instructions**

### **For New Users**
```bash
# Clone and build
git clone https://github.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
./configure
make
sudo make install
```

### **For Arch Linux Users (without wine32)**
1. Install required packages: `sudo pacman -S wine-staging cabextract`
2. Follow standard installation process
3. Use "Download: VS6SP6.EXE" or "Download: VC6RedistSetup_deu.exe" options in MFC42 installer
4. The system will automatically handle extraction and installation

### **For Existing Users**
- No special upgrade steps required
- Existing installations will continue to work
- New MFC42 sources will be available in the installer

---

## 🔍 **Testing & Verification**

### **MFC42 Installation Testing**
- ✅ VS6SP6.EXE extraction via cabextract method
- ✅ VC6RedistSetup_deu.exe extraction via Wine installer method
- ✅ SHA256 verification of downloaded files
- ✅ Automatic symlink creation for both mfc42.dll and mfc42u.dll

### **Wine Integration Testing**
- ✅ Lutris Wine path resolution
- ✅ Wine version detection from various runner patterns
- ✅ YAML parsing of Lutris game configurations
- ✅ NPTest architecture identification

### **Build System Testing**
- ✅ Automatic installation process
- ✅ Library path configuration
- ✅ Desktop file integration
- ✅ Cross-distribution compatibility

---

## 📚 **Documentation Updates**

- **Installation Guide**: Updated with automatic installation process
- **Arch Linux Guide**: Enhanced with Wine32 alternative solutions
- **Troubleshooting**: Added comprehensive troubleshooting section
- **Feature Documentation**: Added detailed feature plans and reviews
- **Build Analysis**: Comprehensive Debian build analysis and solutions

---

## 🎉 **What's New for Users**

### **Arch Linux Users**
- **No More Wine32 Dependency**: Install MFC42.dll without requiring wine32 package
- **Multiple Download Sources**: Choose from VS6SP6.EXE or VC6RedistSetup_deu.exe
- **Automatic Extraction**: System automatically chooses the right extraction method
- **Security Verification**: All downloads verified with SHA256 checksums

### **All Users**
- **Simplified Installation**: One-command installation with automatic post-installation steps
- **Professional Appearance**: Clean Wine bridge application titles
- **Better Error Messages**: Clear feedback and troubleshooting information
- **Enhanced Lutris Support**: Improved Wine integration with Lutris games

---

## 🔮 **Future Development**

- **Additional MFC42 Sources**: More alternative sources for different distributions
- **Enhanced Wine Integration**: Further improvements to Wine detection and integration
- **Cross-Distribution Testing**: Expanded testing across more Linux distributions
- **User Experience**: Continued improvements to installation and setup process

---

## 📞 **Support & Feedback**

- **GitHub Issues**: Report bugs and request features at [GitHub Issues](https://github.com/fwfa123/linuxtrackx-ir/issues)
- **Documentation**: Comprehensive guides available in `docs/` directory
- **Troubleshooting**: Check `docs/troubleshooting/` for common issues and solutions

---

## 🙏 **Acknowledgments**

Special thanks to the LinuxTrack community for testing and feedback, particularly Arch Linux users who helped identify the Wine32 dependency issues that led to the MFC42 alternative sources feature.

---

**Full changelog available in [CHANGELOG.md](CHANGELOG.md)**
