# Arch Linux Installation Guide for LinuxTrack X-IR

## Overview

This guide provides step-by-step instructions for installing LinuxTrack X-IR on Arch Linux and its derivatives (Manjaro, EndeavourOS, etc.) using **CMake** and **Qt6**. The project uses CMake only (autotools has been removed).

**See also:** [docs/readme/arch-linux.md](../../docs/readme/arch-linux.md) (primary) and [docs/guides/ARCH_LINUX_INSTALL_GUIDE.md](../../docs/guides/ARCH_LINUX_INSTALL_GUIDE.md) for more detail.

## Package name notes

- **`libmxml`** → **`mxml`** (Arch package name)
- **`liblo`** → **`liblo`** (official [extra], not AUR)
- **`nsis`** → AUR or `./scripts/install/install_nsis_arch.sh`
- **Qt:** **Qt6** only (`qt6-base`, `qt6-tools`, `qt6-5compat`). Do not use Qt5.

## Method 1: Level 1 (TrackIR-only, CMake + Qt6)

```bash
sudo pacman -Syu
sudo pacman -S --needed base-devel cmake pkg-config
sudo pacman -S --needed libusb bison flex
sudo pacman -S --needed qt6-base qt6-tools qt6-5compat
sudo pacman -S --needed mxml mesa glu sqlite
# zlib: on CachyOS etc. if "zlib vs zlib-ng-compat" conflict, answer N and skip. Else: sudo pacman -S zlib

git clone https://gitlab.com/fwfa123/linuxtrackx-ir.git
cd linuxtrackx-ir
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)
sudo cmake --install .
```

## Method 2: Level 2 (+ Wine, NSIS)

Enable **multilib** in `/etc/pacman.conf`, then:

```bash
sudo pacman -Syu
sudo pacman -S --needed base-devel cmake pkg-config libusb zlib bison flex
sudo pacman -S --needed qt6-base qt6-tools qt6-5compat mxml mesa glu sqlite
sudo pacman -S --needed wine wine-mono wine-gecko
sudo pacman -S --needed lib32-glibc lib32-gcc-libs
sudo pacman -S --needed winetricks cabextract wget
./scripts/install/install_nsis_arch.sh

cd linuxtrackx-ir
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

## First launch (Wayland is default on modern Arch)

If the GUI does not show on Wayland:
```bash
QT_QPA_PLATFORM=xcb ltr_gui
```
Otherwise: `ltr_gui`

## Verification

```bash
ldconfig -p | grep linuxtrack
ltr_gui
ltr_server1 --help
makensis /VERSION   # if Level 2
pkg-config --exists mxml
pkg-config --exists libusb-1.0
```

## Troubleshooting

- **`target not found: libmxml`** → Use `mxml`.
- **`target not found: nsis`** → `./scripts/install/install_nsis_arch.sh` or AUR.
- **32-bit / Wine** → `lib32-glibc`, `lib32-gcc-libs`; multilib enabled. The `build_32bit_libs.sh` script has been removed.
- **GUI on Wayland** → `QT_QPA_PLATFORM=xcb ltr_gui`.

## Additional resources

- [Arch Linux readme](../../docs/readme/arch-linux.md)
- [NSIS script](install_nsis_arch.sh)
- [NSIS README](README_NSIS_ARCH.md)
