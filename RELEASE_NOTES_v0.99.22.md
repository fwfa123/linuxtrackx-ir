# LinuxTrack X-IR v0.99.22 Release Notes

## Release Date: August 6, 2024

### 🎉 Major Release: Gaming Platform Integration & Wine Bridge Enhancement

This release introduces significant improvements to gaming platform integration, enhanced Wine bridge functionality, and automatic tracking features.

## 🚀 New Features

### Gaming Platform Integration
- **Automatic Tracking Start**: Gaming tab installation buttons now automatically start tracking when games are launched
- **Enhanced Lutris Integration**: Improved Wine detection for Lutris with better fallback logic
- **Steam/Proton Support**: Enhanced compatibility with Steam and Proton gaming environments
- **Gaming Tab UI**: New user interface for managing gaming platform integrations

### Wine Bridge Enhancements
- **GUI Installer Display Fix**: Wine bridge installer now properly displays GUI instead of running silently
- **Interactive Installation**: Added user feedback during Wine bridge installation process
- **Improved Wine Detection**: Better detection of various Wine installations (Default wine, winehq-staging, etc.)
- **Enhanced Debugging**: Comprehensive debugging output for Wine path resolution

### Automatic Tracking Features
- **Smart Tracking Start**: Automatic tracking initiation when supported games are detected
- **Platform Detection**: Intelligent detection of gaming platforms and Wine prefixes
- **User Feedback**: Clear feedback about tracking status and Wine bridge availability

## 🔧 Technical Improvements

### AppImage Enhancements
- **Complete Self-Containment**: All dependencies bundled (175 libraries)
- **Qt5 Complete Bundling**: Full Qt5 library and plugin support
- **Theme Integration**: Support for multiple desktop environments (XFCE, GNOME, KDE, etc.)
- **Help System**: Complete help system with SQLite database
- **Wine Bridge Components**: Wine bridge installer included in AppImage

### Build System
- **Phase 4 AppImage Builder**: Enhanced AppImage creation with Wine bridge optimization
- **Dependency Management**: Improved library bundling and isolation
- **Cross-Distribution Support**: Better compatibility across different Linux distributions

## 📦 Included Components

### Core Applications
- **ltr_gui**: Main GUI application with enhanced gaming integration
- **mickey**: Head tracking application
- **osc_server**: OSC communication server
- **wii_server**: Wiimote support server
- **ltr_pipe**: Data processing pipeline

### Gaming Support
- **Wine Bridge Installer**: Windows game compatibility
- **X-Plane Plugins**: 32-bit and 64-bit X-Plane support
- **Flight Simulator Support**: Enhanced compatibility with flight simulators

### Documentation & Help
- **Complete Help System**: SQLite-based help database
- **User Documentation**: Updated guides and tutorials
- **Installation Instructions**: Enhanced setup documentation

## 🐛 Bug Fixes

- Fixed Wine bridge installer GUI display issues
- Improved Wine detection for various distributions
- Enhanced error handling for missing Wine runners
- Fixed theme integration issues across desktop environments
- Resolved help system database access problems

## 📋 System Requirements

### Minimum Requirements
- **OS**: Linux (x86_64)
- **Memory**: 2GB RAM
- **Storage**: 100MB free space
- **Display**: X11 or Wayland with X11 compatibility

### Recommended Requirements
- **OS**: Modern Linux distribution (Ubuntu 20.04+, Fedora 33+, Arch Linux)
- **Memory**: 4GB RAM
- **Storage**: 500MB free space
- **Wine**: For Windows game compatibility

## 🚀 Installation

### AppImage (Recommended)
```bash
# Download the AppImage
wget https://github.com/fwfa123/linuxtrackx-ir/releases/download/v0.99.22/LinuxTrack-X-IR-0.99.22-x86_64.AppImage

# Make executable
chmod +x LinuxTrack-X-IR-0.99.22-x86_64.AppImage

# Run
./LinuxTrack-X-IR-0.99.22-x86_64.AppImage
```

### Source Build
```bash
git clone https://github.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
git checkout v0.99.22
./autogen.sh
./configure --prefix=/usr
make -j$(nproc)
sudo make install
```

## 🎮 Gaming Setup

### Wine Bridge Installation
1. Launch LinuxTrack X-IR
2. Go to the "Misc" tab
3. Click "Install Wine Bridge" button
4. Follow the installation prompts

### Automatic Tracking
1. Install Wine bridge (see above)
2. Launch your Windows game
3. LinuxTrack will automatically detect and start tracking
4. Use the gaming tab to manage tracking settings

## 📝 Changelog

### Version 0.99.22
- **120daf9**: Update version references to 0.99.22 in documentation and scripts
- **a3ca821**: Version bump to 0.99.22 - Gaming Platform Integration & Automatic Tracking
- **d1c3156**: Add automatic tracking start to gaming tab installation buttons
- **5a5b4ff**: Fix Wine bridge installer GUI display issue
- **b64965d**: Improve Wine detection for Lutris integration

## 🔗 Links

- **GitHub Repository**: https://github.com/fwfa123/linuxtrackx-ir
- **Issues**: https://github.com/fwfa123/linuxtrackx-ir/issues
- **Documentation**: https://github.com/fwfa123/linuxtrackx-ir/tree/main/docs

## 🙏 Acknowledgments

Thank you to all contributors and users who have helped test and improve LinuxTrack X-IR. Your feedback and contributions make this project better with each release.

---

**LinuxTrack X-IR v0.99.22** - Bringing head tracking to Linux gaming since 2009
