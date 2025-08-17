# LinuxTrack X-IR v0.99.23 Release Notes

## Highlights
- AppImage builder v2: modular, self-contained pipeline under `scripts/appimage/v2/`
- Help system fix: bundled Qt SQLite driver and plugin paths; Help works without host Qt
- Gaming tab improvements: installers via Automake, Wii desktop Exec fix, UI/UX polish
- Tester autofill and Wine prefix architecture detection refinements
 - Project migration: repository moved to GitLab; file issues and feature requests on GitLab

## Feature summary (0001–0007)
- 0001: Testing Section for Gaming Tab – core workflow, Steam/Lutris/Custom prefix support
- 0002: Installation Flow – no auto-start/auto-launch in install buttons; testing remains explicit
- 0003: Gaming Tab UI Improvements – prerequisites gating, grouped installers, workflow polish
- 0004: Antimicrox Integration Plan – ltr_pipe control (uinput-abs), device naming, UI hooks
- 0005: Automatic Wine Prefix Architecture Detection – selects 32/64-bit tester and sets WINEARCH
- 0006: Tester Game ID Auto-fill – prefill from gamedata.txt, env hints (LTR_GAME_NAME/ID)
- 0007: AppImage Builder Rework (v2) – modular pipeline, Qt bundling, validation, packaging

## What’s New
- AppImage v2 pipeline: prepare → bundle → wine_bridge → optimize → validate → package
  - Bundles Qt libs/plugins, XCB platform, SQLite driver
  - AppRun sets QT_PLUGIN_PATH and QT_SQL_DRIVER_PATH for reliable QtHelp
  - RPATH harmonization for usr/lib and usr/lib/linuxtrack
  - Validation: ldd audit, Qt plugin checks
- Wine bridge: staged into AppDir; installer included in AppImage

## Improvements from feature/gaming-tab-ui-improvements
- Install .desktop launchers via Automake; update desktop DB
- Wii desktop Exec corrected to wii_server
- Auto-generate and install Qt help (qhc/qch) via qmake; persists across distclean
- Tester autofill rules synchronized and improved (numeric+token matching)
- Automatic Wine prefix architecture detection; set WINEARCH per platform
- Antimicrox uinput-abs control working; robust LogView
- QtConcurrent added to project; UI polish and i18n sweep

## Build and Run
```bash
# Build AppImage with v2 pipeline
bash scripts/appimage/v2/ci_build.sh

# Run
chmod +x LinuxTrack-X-IR-0.99.23-x86_64.AppImage
./LinuxTrack-X-IR-0.99.23-x86_64.AppImage
```

## Known Notes
- AppImage relies on host glibc/toolchain and GPU/X11 drivers (normal for AppImages)
- For broader distro compatibility, consider building on an older base or bundling libstdc++/libgcc_s

### Upgrade notice (required actions)
- Reinstall TrackIR firmware and MFC42 runtime (Prerequisites → Install/Repair)
- Reinstall the Wine Bridge in every Wine/Proton prefix where it was previously installed:
  - Steam (Proton): use the Steam installer button
  - Lutris: use the Lutris installer button
  - Custom Prefix: choose your WINEPREFIX via the Custom Prefix installer
These steps ensure the updated testers and runtime files are deployed in each prefix.

## Acknowledgements
Thanks to contributors across recent feature branches and documentation updates.

---

Repository migration notice: The project has moved to GitLab. Please use GitLab for issues and feature requests: https://gitlab.com/fwfa123/linuxtrackx-ir
