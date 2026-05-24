# Installation Scripts Directory

This directory now contains only helpers that still apply to the current CMake + Qt6 + MinGW native Wine bridge.

For normal source builds, use the distro guides under [`docs/readme/`](../../docs/readme/) or the Arch helper:

```bash
./scripts/build_arch_linux.sh
```

## Current Helpers

- [`install_wine_bridge.sh`](install_wine_bridge.sh): Copy Wine bridge PE files into a Wine/Proton prefix (`wine reg` + firmware symlinks). Same flow as the GUI native install.
- [`install_winetricks.sh`](install_winetricks.sh): Optional runtime helper for installing a recent `winetricks`.
- [`mfc42_alternative_installers.sh`](mfc42_alternative_installers.sh): MFC42 fallback installer used by the GUI path.
- [`install_mfc42_alternative.sh`](install_mfc42_alternative.sh): Thin wrapper around the MFC42 fallback methods.
- [`mfc42_python_installer.py`](mfc42_python_installer.py): Python fallback used by the MFC42 wrapper.
- [`linuxtrack_quick_recovery.sh`](linuxtrack_quick_recovery.sh): CMake-oriented recovery helper for local source-tree repair.

## Related Docs

- [`docs/readme/arch-linux.md`](../../docs/readme/arch-linux.md)
- [`docs/readme/debian-ubuntu.md`](../../docs/readme/debian-ubuntu.md)
- [`docs/readme/fedora-rhel.md`](../../docs/readme/fedora-rhel.md)
- [`docs/readme/troubleshooting.md`](../../docs/readme/troubleshooting.md)
- [`docs/guides/WINE_BRIDGE_MODERN.md`](../../docs/guides/WINE_BRIDGE_MODERN.md)

## Archived Legacy Helpers

Older install-folder scripts that targeted the previous autotools, prebuilt bridge, Qt5, or winegcc-era workflows were moved to [`docs/archive/scripts/install/`](../../docs/archive/scripts/install/).

That archive includes the old NSIS helpers (`install_nsis_arch.sh`), prebuilt Wine bridge package scripts, the deprecated Arch prebuilt installer, the autotools-style `linuxtrack_install.sh`, stale setup/post-install scripts, and the Qt5-era health/verification helpers.
