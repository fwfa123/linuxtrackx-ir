# LinuxTrack Build Guide: Arch Linux

## Quick Package Installation

### Core Dependencies (All Levels)
```bash
sudo pacman -S base-devel cmake pkg-config
sudo pacman -S libusb bison flex
sudo pacman -S qt6-base qt6-tools qt6-5compat
sudo pacman -S mxml mesa glu  # mxml: Arch package name
# zlib: if you get "zlib and zlib-ng-compat are in conflict", answer N and do NOT remove zlib-ng-compat.
# zlib-ng-compat provides libz; CMake will find it. Only install zlib if neither is present.
sudo pacman -S zlib   # omit if zlib-ng-compat is already installed (CachyOS, etc.)
```

### Wine Support (Level 2+)
```bash
# Enable multilib repository (edit /etc/pacman.conf: uncomment [multilib] and Include)
sudo pacman -Syu

# Wine: use official 'wine' (wine-stable is in AUR if you prefer)
sudo pacman -S wine wine-mono wine-gecko
# 32-bit support (multilib)
sudo pacman -S lib32-glibc lib32-gcc-libs
# If available: lib32-wine (or lib32-wine-stable) for 32-bit wine bridge
sudo pacman -S lib32-wine 2>/dev/null || true
# MFC42 and Wine bridge installer
sudo pacman -S winetricks cabextract wget
./scripts/install/install_nsis_arch.sh
```

**IMPORTANT**: Enable multilib in `/etc/pacman.conf`. The error "WINEARCH is set to 'win32' but this is not supported in wow64 mode" means multilib is not enabled or 32-bit Wine packages are missing.

### Webcam Support (Level 3+)
```bash
sudo pacman -S libv4l v4l-utils opencv
```

### AppImage / packaging build (maintainers)
Install the **Webcam Support (Level 3+)** packages on the build host so the AppImage includes `libwc`, PS3 Eye (`libp3e`), and OpenCV-linked facetrack drivers where applicable. End users running the released AppImage do not need system OpenCV if the image was bundled correctly.

### OSC Support (Level 4+)
```bash
sudo pacman -S liblo   # official [extra], not AUR
```

### Wiimote Support (Level 5+)
```bash
# From AUR (use yay or paru)
yay -S cwiid
```

### X-Plane Support (Level 6)
Download the X-Plane SDK from [developer.x-plane.com/sdk/plugin-sdk-downloads](https://developer.x-plane.com/sdk/plugin-sdk-downloads/), extract to `/opt/xplane-sdk/` so that `CHeaders` exists at `/opt/xplane-sdk/CHeaders`. Do not use a direct scripted download from that page (it serves HTML).
```bash
sudo mkdir -p /opt/xplane-sdk
# extract your XPSDK*.tar.gz so that /opt/xplane-sdk/CHeaders exists
```

## Build Commands

### Level 1: TrackIR Only
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 2: TrackIR + Wine (Most Common)
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 3: TrackIR + Wine + Webcam
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 4: TrackIR + Wine + Webcam + OSC
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 5: TrackIR + Wine + Webcam + OSC + Wiimote
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 6: Complete Build with X-Plane
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_WIIMOTE=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders
cmake --build . -j$(nproc)
sudo cmake --install .
```

## First Launch (Wayland is default on modern Arch)

On **Wayland**, if the GUI does not appear, run:
```bash
QT_QPA_PLATFORM=xcb ltr_gui
```
Otherwise:
```bash
ltr_gui
```

## Verification
```bash
ldconfig -p | grep linuxtrack
ltr_server1 --help
ltr_gui
ls /opt/lib/linuxtrack/wine_bridge/   # if Level 2+ built
```

## Troubleshooting

### Wine Issues (Most Common)

| Problem | Solution |
|---------|----------|
| `wine: WINEARCH is set to 'win32' but this is not supported in wow64 mode` | Enable multilib in `/etc/pacman.conf` and install `lib32-glibc` `lib32-gcc-libs`; if `lib32-wine` exists, install it |
| `wine-staging` conflicts | Remove conflicting packages: `sudo pacman -R wine-staging wine-gecko wine-mono winetricks` |
| Missing 32-bit headers | Install `lib32-glibc` and `lib32-gcc-libs` (multilib). The `build_32bit_libs.sh` script has been removed; lib32-mxml and lib32-liblo are not required for the default CMake wine bridge build |
| 32-bit/64-bit compilation conflicts | Use explicit 64-bit flags: `CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64"` |
| `zlib` and `zlib-ng-compat` in conflict (CachyOS etc.) | Answer **N** (do not remove zlib-ng-compat). Omit `zlib`; zlib-ng-compat provides `libz`. Run `pkg-config --exists zlib` to confirm. |
| Qt6 CMake config not found | Install Qt6 development packages: `sudo pacman -S qt6-base qt6-tools` |

### Common Issues

| Problem | Solution |
|---------|----------|
| `Couldn't load library 'libwc.so.0'` | Update library cache: `sudo ldconfig` |
| GUI not displaying on Wayland | **Wayland is the default on modern Arch.** Use: `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | Add to groups: `sudo usermod -a -G plugdev,input $USER` |
| Application not in launcher | Desktop integration should work with `/opt` prefix. If issues persist, run: `sudo update-desktop-database /opt/share/applications` |

### 32-bit and Wine bridge

For the **default CMake build**, you need 32-bit **headers** (for building), not lib32-mxml or lib32-liblo. Install:
```bash
sudo pacman -S lib32-glibc lib32-gcc-libs
```
Ensure multilib is enabled in `/etc/pacman.conf`. The `build_32bit_libs.sh` script has been removed. If you see "skipping incompatible" for other libraries, see [ARCH_LINUX_32BIT_LIBRARIES.md](../ARCH_LINUX_32BIT_LIBRARIES.md) for optional manual builds.

### Qt6 Tools PATH (Rare)
If Qt6 tools aren't in PATH, add them:
```bash
export PATH="/usr/lib/qt6/bin:$PATH"
```

## Advanced: Custom Build Combinations

Combine cmake flags for your specific needs:

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_OSC=ON
```

See the main README for all available CMake options.
