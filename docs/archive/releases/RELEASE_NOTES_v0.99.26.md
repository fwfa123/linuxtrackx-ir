# LinuxTrack X-IR Release Notes v0.99.26

**Release Date:** January 15, 2025  
**Version:** v0.99.26  
**Codename:** "Help System Overhaul"

## 🎯 Overview

This release represents a major documentation and help system overhaul for LinuxTrack X-IR, featuring comprehensive gaming integration documentation, improved user experience, and a complete migration to a modern wiki-based documentation system.

## ✨ New Features

### 🎮 Enhanced Gaming Integration
- **New Gaming Tab Documentation**: Complete guide for gaming platform integration
- **TrackIR Permission Setup**: Streamlined permission installation process
- **AntiMicroX Integration**: Detailed setup for head tracking with non-TrackIR games
- **Multi-Platform Support**: Steam Proton, Lutris, and custom Wine prefix support

### 📚 Comprehensive Documentation System
- **GitHub Wiki Migration**: Complete conversion from HTML help to Markdown wiki
- **Visual Documentation**: Enhanced with screenshots and step-by-step guides
- **Improved Navigation**: Sidebar navigation and better content organization
- **Search-Friendly**: Optimized for GitHub's search functionality

## 🔧 Improvements

### Help System Enhancements
- **New Help Files**:
  - `gaming_tab.htm` - Complete gaming integration guide
  - `trackir_permissions.htm` - TrackIR permission setup process
- **Updated Existing Help**: Enhanced all existing help files with better content
- **Image Integration**: All screenshots properly organized and linked

### Documentation Structure
- **Wiki Organization**: Logical content structure with clear sections
- **Image Management**: Centralized image directory with proper references
- **Link Management**: All internal links updated for wiki format
- **Table Formatting**: Complex tables properly converted to Markdown

### Developer Tools
- **Conversion Scripts**: Automated HTML to Markdown conversion tools
- **Image Handlers**: Automated image processing and path management
- **Link Fixers**: Automated link reference updates
- **Build Integration**: Help system integrated into build process

## 🎯 Gaming Platform Support

### Steam (Proton)
- Automatic Steam installation detection
- Proton version compatibility
- Seamless Wine Bridge installation
- Game library integration

### Lutris
- Lutris installation detection
- Wine prefix management
- Game-specific configuration
- Profile management

### Custom Wine Prefixes
- Manual Wine prefix selection
- Custom installation paths
- Advanced configuration options
- Debugging support

### X-Plane Plugin
- Native Linux support
- Direct plugin installation
- No Wine dependency
- Optimized performance

## 🛠️ Technical Improvements

### Build System
- **Qt Help Integration**: Automated help file generation
- **Version Management**: Proper version tagging and tracking
- **Cross-Platform**: Enhanced macOS and Linux support
- **Dependencies**: Updated project dependencies

### Code Quality
- **Documentation**: Comprehensive inline documentation
- **Error Handling**: Improved error messages and user feedback
- **Performance**: Optimized help system loading
- **Maintainability**: Better code organization

## 📋 User Experience Enhancements

### Installation Process
- **Streamlined Setup**: Simplified initial configuration
- **Permission Management**: Automated udev rule installation
- **Group Management**: Automatic user group assignment
- **Visual Feedback**: Clear status indicators and progress

### Gaming Integration
- **One-Click Setup**: Simplified platform installation
- **Testing Tools**: Built-in game testing functionality
- **Configuration Wizards**: Step-by-step setup guides
- **Troubleshooting**: Comprehensive troubleshooting guides

### Documentation Access
- **Context-Sensitive Help**: Help integrated into GUI
- **Online Wiki**: Accessible documentation online
- **Search Functionality**: Easy content discovery
- **Visual Guides**: Screenshot-rich documentation

## 🔧 Configuration Options

### Linuxtrack Server
- **Output Formats**:
  - `uinput-abs (Antimicrox)` - For AntiMicroX integration
  - `uinput-rel` - Relative input for mouse emulation
  - `joystick` - Joystick input format
  - `freetrack` - FreeTrack protocol support

### Gaming Platforms
- **Steam Integration**: Automatic Proton detection
- **Lutris Support**: Wine prefix management
- **Custom Prefixes**: Manual configuration
- **X-Plane Plugin**: Native Linux support

### Testing Tools
- **Tester Selection**: TrackIR vs FreeTrack testers
- **Platform Testing**: Multi-platform game testing
- **Game Filtering**: Easy game discovery
- **Real-time Feedback**: Live testing results

## 🐛 Bug Fixes

- Fixed help system loading issues
- Resolved image path references
- Corrected internal link navigation
- Fixed table formatting in help files
- Resolved permission dialog issues
- Fixed Wine Bridge installation paths

## 📚 Documentation Updates

### New Documentation
- **Gaming Tab Guide**: Complete gaming integration documentation
- **TrackIR Permissions**: Step-by-step permission setup
- **AntiMicroX Integration**: Detailed setup and configuration
- **Platform-Specific Guides**: Steam, Lutris, and custom prefix setup

### Updated Documentation
- **Interface Guide**: Enhanced with new features
- **Model Setup**: Improved setup instructions
- **System Requirements**: Updated prerequisites
- **Troubleshooting**: Expanded troubleshooting section

### Wiki Migration
- **Complete HTML to Markdown conversion**
- **Image optimization and organization**
- **Link reference updates**
- **Table formatting improvements**
- **Navigation structure enhancement**

## 🚀 Installation Instructions

### Prerequisites
- Linux distribution with Qt 5+ support
- Wine (for Windows game support)
- AntiMicroX (for non-TrackIR games)
- TrackIR device (optional)

### Installation Steps
1. **Download**: Get the latest release from GitHub/GitLab
2. **Extract**: Extract the archive to your preferred location
3. **Build**: Run `./configure && make -j$(nproc)`
4. **Install**: Run `sudo make install`
5. **Configure**: Launch GUI and follow setup wizard
6. **Gaming Setup**: Use Gaming tab for platform integration

### Post-Installation
1. **Permissions**: Install TrackIR permissions if using TrackIR
2. **Gaming Platforms**: Set up Steam, Lutris, or custom prefixes
3. **Testing**: Use built-in testing tools to verify setup
4. **Documentation**: Refer to wiki for detailed guides

## 🔄 Migration from Previous Versions

### Help System Migration
- **Automatic**: Help system automatically updates
- **Backup**: Previous help files preserved
- **Compatibility**: Maintains backward compatibility
- **Performance**: Improved loading times

### Configuration Migration
- **Settings**: User settings automatically migrated
- **Profiles**: Existing profiles preserved
- **Customizations**: User customizations maintained
- **Data**: No data loss during upgrade

## 🎯 Known Issues

### Current Limitations
- **Wine Dependencies**: Some games require specific Wine versions
- **Permission Requirements**: TrackIR requires udev rule installation
- **Platform Detection**: Some custom setups may require manual configuration
- **Performance**: Complex games may require performance tuning

### Workarounds
- **Manual Installation**: Use custom prefix option for complex setups
- **Permission Issues**: Re-run permission setup if needed
- **Performance**: Adjust tracking sensitivity for optimal performance
- **Compatibility**: Check game-specific documentation

## 🔮 Future Roadmap

### Planned Features
- **Enhanced Game Detection**: Improved automatic game detection
- **Profile Management**: Advanced profile sharing and management
- **Cloud Integration**: Optional cloud profile synchronization
- **Mobile App**: Companion mobile application

### Documentation Plans
- **Video Tutorials**: Step-by-step video guides
- **Community Wiki**: User-contributed content
- **API Documentation**: Developer API documentation
- **Plugin System**: Third-party plugin support

## 👥 Contributing

### How to Contribute
1. **Fork**: Fork the repository on GitHub/GitLab
2. **Branch**: Create a feature branch
3. **Develop**: Make your changes
4. **Test**: Test thoroughly
5. **Submit**: Create a pull request

### Documentation Contributions
- **Wiki Updates**: Edit wiki pages directly
- **Help Files**: Update HTML help files
- **Screenshots**: Provide updated screenshots
- **Translations**: Help with translations

## 📞 Support

### Getting Help
- **Wiki**: Check the comprehensive wiki documentation
- **Issues**: Report issues on GitHub/GitLab
- **Community**: Join the community discussions
- **Email**: Contact the development team

### Resources
- **GitHub Repository**: https://github.com/linuxtrackx-ir/linuxtrackx-ir
- **GitLab Repository**: https://gitlab.com/linuxtrackx-ir/linuxtrackx-ir
- **Wiki Documentation**: https://github.com/linuxtrackx-ir/linuxtrackx-ir/wiki
- **Issue Tracker**: Use GitHub/GitLab issues

## 📄 Changelog

### v0.99.26 (2025-01-15)
- **NEW**: Gaming tab documentation and help system
- **NEW**: TrackIR permission setup guide
- **NEW**: AntiMicroX integration documentation
- **NEW**: Complete GitHub wiki migration
- **NEW**: Automated conversion scripts
- **IMPROVED**: Help system performance and organization
- **IMPROVED**: Documentation structure and navigation
- **IMPROVED**: Image management and references
- **FIXED**: Help system loading issues
- **FIXED**: Image path references
- **FIXED**: Internal link navigation
- **FIXED**: Table formatting in help files

### Previous Versions
- See git history for complete changelog
- All previous features and fixes preserved
- Backward compatibility maintained

## 🏆 Acknowledgments

### Contributors
- **Development Team**: Core development and maintenance
- **Community**: Bug reports, feature requests, and testing
- **Documentation**: Wiki contributors and help system updates
- **Testing**: Beta testers and quality assurance

### Special Thanks
- **Qt Framework**: For the excellent GUI framework
- **Wine Project**: For Windows compatibility layer
- **AntiMicroX**: For input mapping capabilities
- **Open Source Community**: For inspiration and support

---

**LinuxTrack X-IR v0.99.26** - Bringing professional head tracking to Linux gaming!

*For the latest updates and community discussions, visit our GitHub and GitLab repositories.*
