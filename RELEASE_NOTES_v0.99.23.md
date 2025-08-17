# LinuxTrack X-IR v0.99.23 Release Notes

## Highlights
- AppImage builder v2: modular, self-contained pipeline under `scripts/appimage/v2/`
- Help system fix: bundled Qt SQLite driver and plugin paths; Help works without host Qt
- Gaming tab improvements: installers via Automake, Wii desktop Exec fix, UI/UX polish
- Tester autofill and Wine prefix architecture detection refinements

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

## Acknowledgements
Thanks to contributors across recent feature branches and documentation updates.
