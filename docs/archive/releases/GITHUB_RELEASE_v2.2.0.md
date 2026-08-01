## Minor release — v2.2.0

> **Issues & support:** [gitlab.com/fwfa123/linuxtrackx-ir](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues) (primary). This GitHub repo is a mirror—**do not use bare `#NN` issue numbers** here; they may link to the wrong upstream project. Use [GitLab issues](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues) instead.

**Canonical release notes:** [GitLab v2.2.0](https://gitlab.com/fwfa123/linuxtrackx-ir/-/releases/v2.2.0)

### Overview

**v2.2.0** is a minor feature release after [v2.1.0](https://gitlab.com/fwfa123/linuxtrackx-ir/-/releases/v2.1.0). It adds **OpenTrack / FreeTrack UDP** output (`ltr_udp`) and **controller / HOTAS global hotkeys** for Pause/Resume and Recenter in the Tracking window. No Wine bridge breaking changes.

![Tracking window — Global Hotkeys with controller binds](https://raw.githubusercontent.com/fwfa123/linuxtrackx-ir/main/src/qt_gui/help/TrackingHotkeys.png)

### Highlights

**OpenTrack / FreeTrack UDP Bridge**
- New **`ltr_udp`** tool and **Advanced** tab controls send head tracking over UDP in OpenTrack or FreeTrack format (default `127.0.0.1:4242`).
- Useful for titles such as X4 Foundations that wait for an OpenTrack connection.
- Tracked as [GitLab issue 62](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/62) / [MR !12](https://gitlab.com/fwfa123/linuxtrackx-ir/-/merge_requests/12).

**Credits — OpenTrack UDP:** Thanks to **Mike Becker** ([uap-universe](https://gitlab.com/uap-universe)) for contributing the OpenTrack / FreeTrack UDP integration, based on **StarTuz**’s [linuxtrack-Qt6-Wayland](https://github.com/StarTuz/linuxtrack-Qt6-Wayland) `ltr_udp` work.

**Controller / HOTAS global hotkeys**

![Set HotKey — press a key or controller button](https://raw.githubusercontent.com/fwfa123/linuxtrackx-ir/main/src/qt_gui/help/HotkeyAssign.png)

- Bind **Pause/Resume** and **Recenter** to a keyboard key or a joystick/HOTAS button via host **evdev** (no AntimicroX required).
- Short display labels (for example `PS4 A`) with the full device name in the tooltip.
- Bindings stay saved when the stick is unplugged and reopen automatically when it returns.
- Your user typically needs membership in the `input` group to read controller devices.
- Tracked as [GitLab issue 63](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/63).

**AppImage**
- **`LinuxTrack-X-IR-2.2.0-x86_64.AppImage`** attached to this release (Level 7 bundle: TrackIR, Wine bridge, X-Plane, webcam, OSC, face track, Wiimote, `ltr_udp`).
- FUSE3-friendly type2 runtime from the v2.0.1 line — no host `libfuse2` on modern distros.

### Fixed (since v2.1.0)

- Exact joystick device name matching so DualShock sibling nodes (Touchpad / Motion Sensors) are not opened by mistake.
- Missing-controller dialog only after Assign — not on cold start or when clearing hotkeys.
- `ltr_udp` CLI/GUI argv alignment and status labeling polish before merge.

### Docs and wiki

- In-app help and GitHub/GitLab wikis updated for v2.2.0 (What’s new, Tracking window hotkeys screenshots, Advanced / Gaming notes).

### Upgrade from v2.1.0

- Replace the AppImage with **`LinuxTrack-X-IR-2.2.0-x86_64.AppImage`** below — no Wine bridge reinstall.
- Source: `git checkout v2.2.0`, clean `build/`, rebuild, `sudo cmake --install .`.
- Existing keyboard hotkeys remain compatible; controller binds are additive.

### Quick start

```bash
chmod +x LinuxTrack-X-IR-2.2.0-x86_64.AppImage
./LinuxTrack-X-IR-2.2.0-x86_64.AppImage
```

### Build from source

```bash
git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
git checkout v2.2.0
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j"$(nproc)"
sudo cmake --install .
```

See [`docs/readme/`](https://gitlab.com/fwfa123/linuxtrackx-ir/-/tree/v2.2.0/docs/readme) on GitLab.

### Links

- **GitLab release (canonical):** https://gitlab.com/fwfa123/linuxtrackx-ir/-/releases/v2.2.0
- **CHANGELOG:** https://gitlab.com/fwfa123/linuxtrackx-ir/-/blob/v2.2.0/CHANGELOG.md
- **Wiki — Device / Tracking setup:** https://gitlab.com/fwfa123/linuxtrackx-ir/-/wikis/dev_setup
- **Wiki — Advanced tab (UDP):** https://gitlab.com/fwfa123/linuxtrackx-ir/-/wikis/advanced_tab
- **GitLab issue 62 (OpenTrack UDP):** https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/62
- **GitLab issue 63 (HOTAS hotkeys):** https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/63
