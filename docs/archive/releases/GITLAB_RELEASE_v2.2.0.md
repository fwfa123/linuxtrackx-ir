# LinuxTrack X-IR v2.2.0

**Release date:** 2026-08-01  
**Tag:** `v2.2.0` on `main`  
**Previous release:** [v2.1.0](https://gitlab.com/fwfa123/linuxtrackx-ir/-/releases/v2.1.0)

## Overview

**v2.2.0 is a minor feature release** on `main`. It adds **OpenTrack / FreeTrack UDP** output (`ltr_udp`) and **controller / HOTAS global hotkeys** for Pause/Resume and Recenter in the Tracking window. No breaking changes to the Wine bridge or install flow from v2.1.0.

![Tracking window — Global Hotkeys with controller binds](https://gitlab.com/fwfa123/linuxtrackx-ir/-/raw/main/src/qt_gui/help/TrackingHotkeys.png)

## Highlights

### OpenTrack / FreeTrack UDP Bridge

- New **`ltr_udp`** tool and **Advanced** tab controls send head tracking over UDP in OpenTrack or FreeTrack format (default `127.0.0.1:4242`).
- Useful for titles such as X4 Foundations that wait for an OpenTrack connection.
- Resolves [#62](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/62) (MR !12). Credits: Mike Becker (uap-universe), based on StarTuz’s `ltr_udp` work.

### Controller / HOTAS global hotkeys

![Set HotKey — press a key or controller button](https://gitlab.com/fwfa123/linuxtrackx-ir/-/raw/main/src/qt_gui/help/HotkeyAssign.png)

- Bind **Pause/Resume** and **Recenter** to a keyboard key or joystick/HOTAS button via host **evdev** (no AntimicroX).
- Short display labels (for example `PS4 A`) with full device name in the tooltip.
- Bindings stay saved when the stick is unplugged and reopen automatically when it returns.
- Resolves [#63](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/63).

## Fixed and improved (since v2.1.0)

- Exact joystick device name matching so DualShock sibling nodes (Touchpad / Motion Sensors) are not opened by mistake.
- Missing-controller dialog only after Assign (not cold start or Clear).

## Upgrade from v2.1.0

- **AppImage users:** download **`LinuxTrack-X-IR-2.2.0-x86_64.AppImage`** from the GitHub release; `chmod +x` and run. No Wine bridge reinstall required.
- **Source installs:** `git fetch && git checkout v2.2.0`, rebuild from a clean `build/` directory, `sudo cmake --install .`.
- **Hotkeys:** existing keyboard binds remain compatible; controller binds are additive.

## Quick start (AppImage)

```bash
chmod +x LinuxTrack-X-IR-2.2.0-x86_64.AppImage
./LinuxTrack-X-IR-2.2.0-x86_64.AppImage
```

## Build from source

```bash
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
git checkout v2.2.0
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j"$(nproc)"
sudo cmake --install .
```

Distribution packages and build levels: [`docs/readme/`](https://gitlab.com/fwfa123/linuxtrackx-ir/-/tree/v2.2.0/docs/readme).

## Links

- [CHANGELOG.md](https://gitlab.com/fwfa123/linuxtrackx-ir/-/blob/v2.2.0/CHANGELOG.md)
- [GitHub mirror release + AppImage download](https://github.com/fwfa123/linuxtrackx-ir/releases/tag/v2.2.0)
- [Wiki — Device / Tracking setup](https://gitlab.com/fwfa123/linuxtrackx-ir/-/wikis/dev_setup)
- [Wiki — Advanced tab (UDP)](https://gitlab.com/fwfa123/linuxtrackx-ir/-/wikis/advanced_tab)
