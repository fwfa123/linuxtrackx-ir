# LinuxTrack user guides

This directory holds focused supplemental guides. **Current releases use Qt6 and CMake** (see the main [README.md](../../README.md)). **Per-distro install and troubleshooting** live under [`docs/readme/`](../readme/) (Debian/Ubuntu, Fedora/RHEL, Arch, Flatpak, migration notes).

## Guides in this folder

| File | Purpose |
|------|---------|
| [`WINE_BRIDGE_MODERN.md`](WINE_BRIDGE_MODERN.md) | Modern MinGW wine bridge (no NSIS); build and install into 32-bit or 64-bit prefixes |

## Cross-references

- **[`../readme/debian-ubuntu.md`](../readme/debian-ubuntu.md)** — Debian/Ubuntu packages and Qt6 build
- **[`../readme/fedora-rhel.md`](../readme/fedora-rhel.md)** — Fedora/RHEL
- **[`../readme/arch-linux.md`](../readme/arch-linux.md)** — Arch Linux
- **[`../readme/troubleshooting.md`](../readme/troubleshooting.md)** — Common issues |
- **[`../AppImage_WineBridge.md`](../AppImage_WineBridge.md)** — AppImage + Wine bridge notes |
- **[`../WINE_SUPPORT_MODERN.md`](../WINE_SUPPORT_MODERN.md)** — Wine support overview |

## Quick commands

```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j"$(nproc)"
sudo cmake --install .
ltr_gui
```

Use your install’s `ltr_gui` launcher, or `scripts/test/run_gui.sh` to launch from the source tree. For Wayland/X11 issues, see **[`../troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md`](../troubleshooting/WAYLAND_COMPATIBILITY_ISSUE.md)**.
