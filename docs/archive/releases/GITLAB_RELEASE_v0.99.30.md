# LinuxTrack X-IR v0.99.30 Release

**Previous Version:** v0.99.29  
**Release Date:** March 14, 2026  
**Codename:** "Lutris, AppImage & UI Improvements"

## Overview

LinuxTrack X-IR v0.99.30 brings Lutris Flatpak and Wine path resolution improvements, AppImage and runtime fixes, UI and Gaming tab updates, and documentation updates. This release resolves issues #41, #43, #44, and #47.

## Downloads

### AppImage (Recommended)

📦 **Download AppImage**: [Available on GitHub Releases](https://github.com/fwfa123/linuxtrackx-ir/releases/tag/v0.99.30)

The AppImage is available on GitHub and can be downloaded directly:
- `LinuxTrack-X-IR-0.99.30-x86_64.AppImage`

Make it executable and run:
```bash
chmod +x LinuxTrack-X-IR-0.99.30-x86_64.AppImage
./LinuxTrack-X-IR-0.99.30-x86_64.AppImage
```

## Major Changes

### Lutris / Flatpak / Wine

- **Lutris Flatpak**: Wine binary resolution and Proton via `files/bin/wine` (Issue #43)
- **Documentation**: Flatseal doc for library path; Fedora 43 wine-wow64 paths and Wine lib probing (cmake + readme)

### UI / Gaming Tab

- **Advanced tab**: Linuxtrack Server moved from Gaming tab with warning
- **Gaming tab**: "Install Wine Bridge" label, updated help; spacer, font, geometry tweaks; updated help images

### Tracking / Devices

- **PS3Eye**: Restore Linux PS3Eye (1415:2000) in device selector
- **Tracking**: Fix freeze when re-starting tracking after Stop (Issue #47); fix smoothing save states (Issue #44)

## Detailed Changelog

### Fixed

- AppImage GUI unable to find `ltr_pipe` executable
- SIGPIPE crash when tester program exits
- Issue #41: udev path detection, graceful Start failure, socket close on connect fail
- udev rules path: use `/etc/udev/rules.d/` for local admin rules

### Changed

- Troubleshooting and README docs: udev rules paths, quick issues section

**Resolves:** GitHub Issues #41, #43, #44, #47

## Testing

This release has been tested with:
- Qt5 builds (main branch)
- Qt6 builds (qt6-migration branch)

## Migration Notes

- **Lutris Flatpak users**: Wine/Proton path resolution improved; see troubleshooting docs if you use Flatseal.
- **AppImage users**: `ltr_pipe` is now found correctly; update if you had workarounds.
- **udev**: If you use custom udev rules, ensure they are in `/etc/udev/rules.d/` for local admin use.

---

**Full Changelog**: See [CHANGELOG.md](CHANGELOG.md) for complete change history.
