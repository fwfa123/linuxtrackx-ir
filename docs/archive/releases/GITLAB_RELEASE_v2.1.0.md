# LinuxTrack X-IR v2.1.0

**Release date:** 2026-06-27  
**Tag:** `v2.1.0` on `main`  
**Previous release:** [v2.0.1](https://gitlab.com/fwfa123/linuxtrackx-ir/-/releases/v2.0.1)

## Overview

**v2.1.0 is a minor feature release** on `main`. It delivers a redesigned **Detailed Axes Setup** dialog with live pose maps and axis-baseline restore/save on the Tracking Setup tab. No breaking changes to the Wine bridge or install flow from v2.0.1.

## Highlights

- **Pose maps** — Look (yaw/pitch), Lean (TX/TZ), and Raise/Roll maps with mapper-style visualization: teal guide rings, live red dead-zone region, green position dot; Measured vs Filtered source selector.
- **Detailed Axes graph** — Resizable response curve, coordinate grid, legend strip, dark-mode overlay, pan tuning sync.
- **Restore axis defaults** / **Save as axis defaults** on Tracking Setup — separate `AxisBaseline` rollback from the Default profile used when new games are detected.
- **Build docs** — multilib packages (`gcc-multilib`, `libc6-dev-i386`) for 32-bit `linuxtrack32` on Debian/Ubuntu.

## Fixed

- Pose map dead-zone overlay uses normalized slider fraction (0–1), not physical degrees/mm.
- Wine bridge path resolution for custom Wine prefixes.

## Upgrade from v2.0.1

- **Source installs:** `git fetch && git checkout v2.1.0`, rebuild from a clean `build/` directory, `sudo cmake --install .`.
- **AppImage users:** replace with **`LinuxTrack-X-IR-2.1.0-x86_64.AppImage`** when published on the GitHub mirror release.
- **Wine bridge:** no reinstall required unless you rebuild from source.

## AppImage

**`LinuxTrack-X-IR-2.1.0-x86_64.AppImage`** will be attached to the [GitHub mirror release](https://github.com/fwfa123/linuxtrackx-ir/releases/tag/v2.1.0) when the maintainer build completes.

```bash
git checkout v2.1.0
./scripts/appimage/docker_build.sh
```

## Build from source

```bash
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
git checkout v2.1.0
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j"$(nproc)"
sudo cmake --install .
```

Distribution packages: [`docs/readme/`](https://gitlab.com/fwfa123/linuxtrackx-ir/-/tree/v2.1.0/docs/readme).

## Links

- [CHANGELOG.md](https://gitlab.com/fwfa123/linuxtrackx-ir/-/blob/v2.1.0/CHANGELOG.md)
- [GitHub mirror release](https://github.com/fwfa123/linuxtrackx-ir/releases/tag/v2.1.0)
