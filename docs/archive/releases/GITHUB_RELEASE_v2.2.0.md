## Minor release — v2.2.0

> **Issues & support:** [gitlab.com/fwfa123/linuxtrackx-ir](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues) (primary). This GitHub repo is a mirror—**do not use bare `#NN` issue numbers** here; they may link to the wrong upstream project. Use [GitLab issues](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues) instead.

**Canonical release notes:** [GitLab v2.2.0](https://gitlab.com/fwfa123/linuxtrackx-ir/-/releases/v2.2.0)

### Overview

**v2.2.0** adds **OpenTrack / FreeTrack UDP** output and **controller / HOTAS global hotkeys** for Pause/Resume and Recenter. No Wine bridge breaking changes from [v2.1.0](https://gitlab.com/fwfa123/linuxtrackx-ir/-/releases/v2.1.0).

![Tracking window — Global Hotkeys](https://raw.githubusercontent.com/fwfa123/linuxtrackx-ir/main/src/qt_gui/help/TrackingHotkeys.png)

### Highlights

**OpenTrack / FreeTrack UDP**
- `ltr_udp` + Advanced tab controls; default `127.0.0.1:4242`.
- [GitLab issue 62](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/62) / MR !12 (Mike Becker / StarTuz lineage).

**Controller / HOTAS hotkeys**

![Set HotKey dialog](https://raw.githubusercontent.com/fwfa123/linuxtrackx-ir/main/src/qt_gui/help/HotkeyAssign.png)

- Bind Pause/Resume and Recenter to keyboard or joystick via host evdev.
- Short labels + tooltips; bindings survive unplug/replug.
- [GitLab issue 63](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/63).

### Fixed (since v2.1.0)

- Exact joystick device match (avoid DualShock Touchpad/Motion Sensors).
- Missing-controller dialog only after Assign (not Clear / cold start).

### Upgrade from v2.1.0

- Replace the AppImage with **`LinuxTrack-X-IR-2.2.0-x86_64.AppImage`** below — no Wine bridge reinstall.
- Source: `git checkout v2.2.0`, clean `build/`, rebuild, `sudo cmake --install .`.

### Quick start

```bash
chmod +x LinuxTrack-X-IR-2.2.0-x86_64.AppImage
./LinuxTrack-X-IR-2.2.0-x86_64.AppImage
```
