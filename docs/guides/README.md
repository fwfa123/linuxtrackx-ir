# LinuxTrack user guides

This directory holds focused supplemental guides. **Current releases use Qt6 and CMake** (see the main [README.md](../../README.md)). **Per-distro install and troubleshooting** live under [`docs/readme/`](../readme/) (Debian/Ubuntu, Fedora/RHEL, Arch, Flatpak, migration notes).

## Guides in this folder

| File | Purpose |
|------|---------|
| [`WINE_BRIDGE_MODERN.md`](WINE_BRIDGE_MODERN.md) | Modern WOW64 + MinGW wine bridge (no NSIS); build and install |

## Cross-references

- **[`../readme/debian-ubuntu.md`](../readme/debian-ubuntu.md)** — Debian/Ubuntu packages and Qt6 build
- **[`../readme/fedora-rhel.md`](../readme/fedora-rhel.md)** — Fedora/RHEL
- **[`../readme/arch-linux.md`](../readme/arch-linux.md)** — Arch Linux
- **[`../readme/troubleshooting.md`](../readme/troubleshooting.md)** — Common issues |
- **[`../AppImage_WineBridge.md`](../AppImage_WineBridge.md)** — AppImage + Wine bridge notes |
- **[`../WINE_SUPPORT_MODERN.md`](../WINE_SUPPORT_MODERN.md)** — Wine support overview |

## Legacy snapshots

Older MX Linux and Arch redirect guides, along with `LEGACY_*` snapshots, were moved to **[`../archive/guides/`](../archive/guides/)** for historical reference only.

## Quick commands

```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j"$(nproc)"
sudo cmake --install .
ltr_gui
```

Use your install’s `ltr_gui` launcher or helper scripts under **`scripts/test/`** (e.g. `run_qt5_gui.sh` — **legacy filename**, built against Qt6) to start the GUI. For Wayland/X11 issues, see **[`../troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md`](../troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md)**.
