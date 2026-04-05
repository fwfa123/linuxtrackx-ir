# LinuxTrack X-IR v0.99.27 Release

**Release Date:** [DATE TO BE FILLED]  
**Previous Version:** v0.99.26

## Overview

LinuxTrack X-IR v0.99.27 includes significant fixes and improvements focusing on build system reliability, cross-distribution compatibility, and user experience enhancements. This release addresses critical issues with 32-bit library support, Wine bridge functionality, X-Plane plugin compatibility, and improves the minimal build experience across multiple Linux distributions.

## Major Changes

### Build System & Compatibility
- **Enhanced Cross-Distribution Support**: Improved compatibility with Fedora 42, Debian 12, Arch Linux, and other distributions
- **Minimal Build Philosophy**: Properly implemented basic build philosophy with optional components gracefully disabling instead of failing
- **DESTDIR Support**: Full support for staging installations for packaging systems
- **Glibc Compatibility**: Fixed compatibility issues with glibc 2.38+ functions

### Critical Bug Fixes
- **32-bit Wine Bridge**: Fixed 32-bit liblinuxtrack library build and installation issues
- **Wine Bridge Tracking Data**: Restored slave process spawning in client mode, fixing tracking data communication
- **X-Plane Plugin**: Resolved undefined symbol errors and library path lookup issues
- **System Tab Display**: Fixed memory parsing and build components display showing actual values

### User Experience Improvements
- **TrackIR Setup**: Eliminated redundant password prompts in permission dialog
- **Qt5 Build Instructions**: Comprehensive documentation for Qt5 tools PATH fixes across distributions
- **Error Handling**: Improved error messages and installation verification

## Detailed Changelog

[SELECTED ITEMS FROM CHANGELOG_DRAFT_v0.99.27.md WILL BE INSERTED HERE]

### Fixed
<!-- Items from the draft changelog marked for inclusion will go here -->

### Added
<!-- Items from the draft changelog marked for inclusion will go here -->

### Changed/Improved
<!-- Items from the draft changelog marked for inclusion will go here -->

## Installation

### AppImage (Recommended)
Download the AppImage from the releases page:
- `LinuxTrack-X-IR-0.99.27-x86_64.AppImage`

Make it executable and run:
```bash
chmod +x LinuxTrack-X-IR-0.99.27-x86_64.AppImage
./LinuxTrack-X-IR-0.99.27-x86_64.AppImage
```

### Source Build
For source builds, refer to the updated README.md and BUILD_OPTIONS.md for comprehensive build instructions.

**Important Notes:**
- Qt5 tools must be in PATH for builds (see README for distribution-specific fixes)
- Minimal builds now properly disable optional components
- Use `--disable-ldconfig` for distributions that handle library paths automatically

## Migration from v0.99.26

- No breaking changes
- Existing configurations remain compatible
- Users experiencing 32-bit Wine bridge issues should upgrade
- X-Plane plugin users should reinstall the plugin after upgrading


## Thank You

We would like to express our sincere gratitude to:

- **All users** who have been testing and reporting issues on GitLab - your feedback is invaluable in improving LinuxTrack X-IR
- **István Váradi** for the code changes and helpful commits that have significantly improved the project

Your contributions make LinuxTrack X-IR better with every release!

## Downloads

- **AppImage**: [Link to be added after build]
- **Source Code**: Available on GitLab

---

**Full Changelog**: See `CHANGELOG.md` for complete history.

