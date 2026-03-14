# LinuxTrack X-IR v0.99.30 - Lutris, AppImage & UI Improvements

## Major changes

### Lutris / Flatpak / Wine
- Lutris Flatpak wine binary resolution and Proton via `files/bin/wine` (Issue #43)
- Flatseal doc for library path; Fedora 43 wine-wow64 paths and Wine lib probing

### UI / Gaming tab
- New Advanced tab (Linuxtrack Server moved from Gaming tab, with warning)
- Gaming tab: "Install Wine Bridge" label, updated help; layout and help image updates

### Tracking
- Restore Linux PS3Eye (1415:2000) in device selector
- Fix freeze when re-starting tracking after Stop (#47); fix smoothing save states (#44)

## Fixes

- AppImage GUI unable to find `ltr_pipe` executable
- SIGPIPE crash when tester program exits
- Issue #41: udev path detection, graceful Start failure, socket close on connect fail
- udev rules path: use `/etc/udev/rules.d/` for local admin rules

## Downloads

- **AppImage**: `LinuxTrack-X-IR-0.99.30-x86_64.AppImage`
- **Source**: Available in releases

## Full release notes

See [GITLAB_RELEASE_v0.99.30.md](GITLAB_RELEASE_v0.99.30.md) for complete details.
