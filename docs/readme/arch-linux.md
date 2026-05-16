# LinuxTrack Build Guide: Arch Linux

This guide applies to **Arch Linux** and derivatives (**CachyOS**, EndeavourOS, Manjaro, etc.). **CMake + Qt6** is the only supported build system.

## Choose your path

### End users (recommended): AppImage

For day-to-day use, prefer the **official AppImage** (e.g. v1.0.2+). It bundles the GUI and dependencies users need on rolling distros.

- **Wine bridge:** If a **source build** on your machine fails to produce a working `linuxtrack-wine.exe` (common with **Wine WOW64** on Arch), you can still install the bridge using the **installer shipped with the AppImage** while running **`ltr_gui` from a local `/opt` install** if you wish. Several users reported this workflow on [GitLab #38](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/38).
- Releases: see the project README / GitLab releases.

### Build from source

**Script vs manual:** [`scripts/build_arch_linux.sh`](../../scripts/build_arch_linux.sh) is the **usual** from-source path: it installs dependencies, sets Arch-friendly Wine paths, and configures roughly **README Level 2** (TrackIR + Wine bridge). Use the **package lists and CMake commands** below instead if you want full control, extra `-DENABLE_*` options (X-Plane, webcam, OSC, etc.), or to avoid the script (for example, no AUR helper / yay bootstrap). On **CachyOS** and similar, read [First-time build walkthrough](#first-time-build-walkthrough) — you may want AUR **wine32** before the first configure so the Wine bridge does not fail on the first try.

Use this when you need a full **`/opt` install**, packaging work, or the latest `main` commits. Either follow the **package and CMake** sections below or run the helper script:

```bash
./scripts/build_arch_linux.sh
```

If the script’s Wine verification step is noisy on your system, use split steps (see [Automated build script](#automated-build-script)).

---

## First-time build walkthrough

For **CachyOS**, **Arch**, and similar after **`git clone`** when you want a **from-source** `/opt` install. (If you only need the app working, start with the **AppImage** in [Choose your path](#choose-your-path) — less friction than Arch Wine.)

### What counts as success

- **`ltr_gui` runs** (on Wayland often: `QT_QPA_PLATFORM=xcb ltr_gui`).
- **TrackIR** works for your setup.
- **Wine bridge** (Windows games): on WOW64-heavy systems this is the fragile part; see [Wine, WOW64, and the 32-bit bridge](#wine-wow64-and-the-32-bit-bridge) and [GitLab #38](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/38).

### Steps

1. **System prep:** Enable **`[multilib]`** in `/etc/pacman.conf` (uncomment the `Include` line under `[multilib]`). Run `sudo pacman -Syu`. Check libz with **`pacman -Q zlib zlib-ng-compat`** and **`pkg-config --exists zlib`** (not **`which`** — these are libraries, not shell commands; [package notes](#package-names-that-trip-people-up)). If **`pacman -S zlib`** conflicts with **`zlib-ng-compat`**, you already have the compat stack — answer **N** and skip `zlib`. If **neither** package is installed, add one: usually `sudo pacman -S zlib`, or `zlib-ng-compat` if your image standardizes on it.

2. **Wine bridge (required for this from-source path):** This walkthrough and **`build_arch_linux.sh`** target **Level 2+** — the **Wine bridge is built on your machine** from this tree (not shipped prebuilt). The build still produces **both 32-bit and 64-bit** Wine-bridge pieces when `winegcc` / `wineg++`, NSIS, and the right Wine **Unix** libraries are available (see [Wine, WOW64, and the 32-bit bridge](#wine-wow64-and-the-32-bit-bridge)). *Only* a deliberate **Level 1** configure (no Wine flags) skips the bridge. On **CachyOS** / rolling Arch WOW64, install AUR **[wine32](https://aur.archlinux.org/packages/wine32)** *before* the first **`cmake`** so `/usr/lib32/wine/i386-unix` exists — otherwise the 32-bit side often fails. You need **yay**, **paru**, or similar (`./scripts/build_arch_linux.sh` can install **yay** on first run — [Automated build script](#automated-build-script)). Example: `yay -S wine32`. If you already configured without it and the bridge step failed, install **wine32** (or Tier C **wine-stable** + **wine-stable-mono**), **remove the `build/` directory**, and configure again.

3. **Scripted build (default ≈ README Level 2):** From the repo root:
   ```bash
   ./scripts/build_arch_linux.sh --help
   ./scripts/build_arch_linux.sh
   ```
   Use split steps if needed: `--deps-only`, `--wine32-only`, `--configure-only`, `--build-only`, `--install-only` ([Automated build script](#automated-build-script)). **`--configure-only`** runs **NSIS**, **liblo**, and **Wine** preflight so CMake can enable the **Wine plugin** and **OSC** (older script revisions skipped those on split runs — see [Troubleshooting](#wine-issues-common)).

4. **If the Wine bridge still fails:** Use explicit CMake paths (see [Level 2](#level-2-trackir--wine-most-common)) or the **AppImage bridge** while keeping a local `ltr_gui` ([Choose your path](#choose-your-path)).

5. **First launch:** `ltr_gui` or `QT_QPA_PLATFORM=xcb ltr_gui`; see [Camera view / 3D preview quirks](#camera-view--3d-preview-quirks).

**`build_arch_linux.sh` and AUR `wine32`:** Repo **`wine`** and AUR **`wine32`** **conflict**. If **`paru`** asks to remove **`wine32`** to install **`wine`**, answer **`N`**. The script now **skips** repo **`wine`** when **`wine32`** is already installed; if an older script revision already failed mid-run, update the repo script and re-run from **`./scripts/build_arch_linux.sh --wine32-only`** or **`--configure-only`** as needed.

### Faster builds (Octopi, parallel compile)

- **Octopi:** Use it to install **all official-repo** dependencies in one go (with **multilib** enabled). **AUR `wine32`** still has to be built with **`makepkg`** — use Octopi’s **AUR** integration if your build includes it; otherwise install **`wine32`** once with **paru** / **yay** (or let **`build_arch_linux.sh`** bootstrap **yay**).
- **Parallel jobs:** Before long AUR or CMake builds, run `export MAKEFLAGS="-j$(nproc)"` (or set the same **`MAKEFLAGS`** in **`/etc/makepkg.conf`** so Octopi/`makepkg` use all cores).
- **Pacman:** In **`/etc/pacman.conf`**, raise **`ParallelDownloads`** (e.g. `10`) for quicker repo installs.
- **Avoid redoing work:** Install **wine32** *before* the first **`cmake`** so you don’t repeat a full failed configure/build ([step 2](#first-time-build-walkthrough)).

### Checkpoint table (for issues or doc improvements)

Copy and fill when reporting problems:

| Checkpoint | Pass/Fail | Notes |
|------------|-----------|-------|
| `[multilib]` enabled | | |
| `zlib` / `zlib-ng-compat` conflict | | What you chose |
| `ls /usr/lib32/wine/i386-unix` exists before configure | | e.g. after AUR **wine32** |
| `./scripts/build_arch_linux.sh` full run | | Where it stopped |
| `winegcc` warning only vs build failure | | |
| `cmake` Wine paths auto vs `-DWINE_*` | | |
| `sudo cmake --install .` | | |
| `ltr_gui` (with/without `QT_QPA_PLATFORM=xcb`) | | |
| Wine bridge from **local** build | | Error snippet if any |
| Wine bridge via **AppImage** installer only | | |

---

## Package names that trip people up

These mismatches caused real confusion on [GitLab #38](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/38):

- **`mxml`** — Arch package is **`mxml`**, not `libmxml`.
- **`wine-stable` / `lib32-wine-stable`** — **Not** in the official repos. They are **AUR** packages (or absent under those exact names). Do not expect `sudo pacman -S wine-stable` to work.
- **`zlib` vs `zlib-ng-compat`** — On CachyOS and similar, **do not** remove `zlib-ng-compat` to install `zlib`. Prefer skipping `zlib` if `zlib-ng-compat` already provides `libz` (see Core Dependencies below).
- **`which zlib` / `which zlib-ng-compat`** — These are **package names**, not programs on your `PATH`. Use **`pacman -Q zlib zlib-ng-compat`** to see what is installed, and **`pkg-config --exists zlib`** to confirm the build can find libz. If **`sudo pacman -S zlib`** reports a **conflict** with `zlib-ng-compat`, that means **`zlib-ng-compat` is already installed** — answer **N** (keep it) and **do not** install `zlib`.
- **`liblo`** — Official **`[extra]`** repository: `sudo pacman -S liblo` (not AUR).

---

## Wine, WOW64, and the 32-bit bridge

Arch moved to a **WOW64**-style Wine packaging ([Arch announcement](https://archlinux.org/news/transition-to-the-new-wow64-wine-and-wine-staging/)). Stock **`wine`** may not expose the classic **32-bit Unix Wine libraries** under paths our CMake logic expects, which breaks **building** or **installing** the Wine bridge from a **local** build even when **64-bit** prefixes work.

CMake detects 32-bit Wine Unix libs via `cmake/FindWineLibs.cmake`, notably:

- `/usr/lib32/wine/i386-unix` (typical with AUR **wine32**)
- `/usr/lib32/wine`
- 64-bit: `/usr/lib/wine/x86_64-unix`

**Practical tiers** (try in order):

1. **Tier A — Official repos:** Enable **`[multilib]`** in `/etc/pacman.conf`. Install `wine`, `wine-mono`, `wine-gecko`, `lib32-glibc`, `lib32-gcc-libs`. Optionally install **`lib32-wine`** if your mirror lists it (`pacman -Ss lib32-wine`).
2. **Tier B — AUR `wine32`:** Reported working on CachyOS for users who need the classic 32-bit tree under `/usr/lib32/wine/...` ([AUR: wine32](https://aur.archlinux.org/packages/wine32)). Matches the paths `FindWineLibs.cmake` checks first on Arch. While installing **`wine32`**, **paru** / **pacman** may ask for a provider for **`lib32-jack`**: **`lib32-pipewire-jack`** (option **2**) fits most **PipeWire** desktops (typical CachyOS); **`lib32-jack2`** (option **1**) if you use classic JACK. To skip the prompt: `sudo pacman -S lib32-pipewire-jack` or `lib32-jack2` first, then `paru -S wine32`. You may later see **`pipewire-jack`** conflicting with **`jack`** — on a **PipeWire** setup, removing **`jack`** (**`y`**) is normal; choose **`N`** only if you rely on standalone **JACK2** for pro audio. **CachyOS** may also offer the same name from **`cachyos-extra-v3`** vs **`extra`** (e.g. **`lib32-rust-libs`**) — either is usually fine; **default / repo (1)** is typical on CachyOS. If the terminal seems **stuck** showing a patch and **`Paging with less`**, press **`q`** to exit the pager and continue; or run with **`PAGER=cat`** to avoid **`less`** during **`paru`**.
3. **Tier C — AUR `wine-stable` + `wine-stable-mono`:** Maintainer-tested on CachyOS for a full build. **Tradeoff:** this often means **replacing** repo `wine` / `wine-mono` with older AUR builds, which can affect other games. See discussion on [GitLab #38](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/38).

**CachyOS / rolling Arch:** If you are building the **Wine bridge** from source, doing **Tier B** (`wine32`) **before** the first `cmake` run avoids a common first-time failure (stock WOW64 layout vs `FindWineLibs.cmake`). See [First-time build walkthrough](#first-time-build-walkthrough).

**Runtime / prefixes / MFC42:** See [WINE_SUPPORT_MODERN.md](../WINE_SUPPORT_MODERN.md) and [src/wine_bridge/WINE_SETUP.md](../../src/wine_bridge/WINE_SETUP.md). In the GUI installer, if the default MFC42 path fails, try the **alternate option** in the dropdown (as noted in #38). **`winetricks`** is still commonly used for `mfc42` in game prefixes; Steam/Proton tips live in [CROSS_DISTRIBUTION_WINE_BRIDGE.md](../troubleshooting/CROSS_DISTRIBUTION_WINE_BRIDGE.md).

---

## Quick Package Installation

### Core Dependencies (All Levels)
```bash
sudo pacman -S base-devel cmake pkg-config
sudo pacman -S libusb bison flex
sudo pacman -S qt6-base qt6-tools qt6-5compat
sudo pacman -S mxml mesa glu  # package name is mxml, not libmxml
# zlib: if you get "zlib and zlib-ng-compat are in conflict", answer N and do NOT remove zlib-ng-compat.
# zlib-ng-compat provides libz; CMake will find it. Only install zlib if neither is present.
sudo pacman -S zlib   # omit if zlib-ng-compat is already installed (CachyOS, etc.)
```

### Wine Support (Level 2+)
```bash
# Enable multilib repository (edit /etc/pacman.conf: uncomment [multilib] and Include)
sudo pacman -Syu

# Wine: official repos
sudo pacman -S wine wine-mono wine-gecko
# 32-bit support (multilib)
sudo pacman -S lib32-glibc lib32-gcc-libs
# Optional if available on your mirror:
sudo pacman -S lib32-wine 2>/dev/null || true
# If bridge still does not build/install, add Tier B or C from "Wine, WOW64" above (AUR).

# MFC42 and Wine bridge installer
sudo pacman -S winetricks cabextract wget
./scripts/install/install_nsis_arch.sh
```

**IMPORTANT:** Enable multilib in `/etc/pacman.conf`. The error `WINEARCH is set to 'win32' but this is not supported in wow64 mode` usually means missing multilib packages, missing 32-bit Wine support for the prefix, or needing an AUR Wine stack — see [Wine, WOW64, and the 32-bit bridge](#wine-wow64-and-the-32-bit-bridge).

### X-Plane Support (Level 3+)
```bash
# Download X-Plane SDK from: https://developer.x-plane.com/sdk/plugin-sdk-downloads/
# Extract to: /opt/xplane-sdk/
sudo mkdir -p /opt/xplane-sdk
sudo tar -xzf XPSDK*.tar.gz -C /opt/xplane-sdk/
```

### Webcam Support (Level 4+)
```bash
sudo pacman -S libv4l v4l-utils
```

### AppImage / packaging build (maintainers)
The v2 `prepare.sh` enables `-DENABLE_WEBCAM=ON`, `-DENABLE_OSC=ON`, `-DENABLE_XPLANE=ON`, and `-DENABLE_FACE_TRACKER=OFF` (same as CMake default; aligns with README **Level 5**). Install **Webcam Support (Level 4+)** on the build host so `libwc` and PS3 Eye (`libp3e`) build. To ship OpenCV facetrack inside the AppImage, add `-DENABLE_FACE_TRACKER=ON` and install **Face tracking (Level 6+)** packages on the builder. End users of the released AppImage do not need OpenCV on the system.

### OSC Support (Level 5+)
```bash
sudo pacman -S liblo   # official [extra], not AUR
```

### Face tracking (Level 6+)
Requires Level 4 (webcam). Install OpenCV and pass `-DENABLE_FACE_TRACKER=ON` (default is **OFF**).
```bash
sudo pacman -S opencv
```

### Wiimote Support (Level 7+)
Optional; **cwiid** on AUR is unmaintained and often fails (python2 / old deps — [GitLab #8](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/8)). The **`build_arch_linux.sh`** helper does **not** install Wiimote unless you pass **`--with-wiimote`**.
```bash
# Manual AUR install if you need Wiimote (may fail on current Arch)
yay -S cwiid
# Alternative: yay -S cwiid-git

pkg-config --exists cwiid && pkg-config --modversion cwiid || echo "cwiid.pc not found; check PKG_CONFIG_PATH."
```

---

## Automated build script

[`scripts/build_arch_linux.sh`](../../scripts/build_arch_linux.sh) installs dependencies, Wine (multilib), **liblo** (OSC), checks X-Plane SDK path, NSIS, configures CMake with Arch-friendly `WINE_*` paths, builds, and installs to `/opt`. **Wiimote (AUR cwiid) is not installed** unless you pass **`--with-wiimote`** ([GitLab #8](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/8)).

**CMake vs packages:** The script’s `cmake ..` line matches roughly **README Level 2** (TrackIR + Wine + 32-bit lib + bridge): it does **not** pass `-DENABLE_WEBCAM=ON`, `-DENABLE_XPLANE=ON`, etc. Defaults leave those **OFF**. The script still installs **opencv**, **v4l-utils**, and **liblo** so you can re-run CMake with higher-level flags without reinstalling packages.

**AUR helper:** If neither **yay** nor **paru** is installed, the script clones and builds **yay** from the AUR (needs **network**, uses **`sudo pacman`**, and implies the usual AUR trust model). Install **paru**/**yay** yourself first if you prefer.

**Split runs** (if a verification step fails or you prefer control — see [GitLab #37](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues/37)):

```bash
./scripts/build_arch_linux.sh --deps-only
./scripts/build_arch_linux.sh --deps-only --with-wiimote   # deps + optional Wiimote attempt
./scripts/build_arch_linux.sh --wine32-only    # Wine + verification only
./scripts/build_arch_linux.sh --configure-only
./scripts/build_arch_linux.sh --build-only
./scripts/build_arch_linux.sh --install-only   # script runs sudo where needed for install
./scripts/build_arch_linux.sh --with-wiimote   # full run + try AUR cwiid
```

Use `--help` for all flags.

---

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

If `FindWineLibs` does not detect your layout, set paths explicitly (directories must exist on disk):

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON \
  -DWINE_LIBS_PATH=/usr/lib32/wine/i386-unix \
  -DWINE64_LIBS_PATH=/usr/lib/wine/x86_64-unix
```

Inspect candidates with `ls /usr/lib32/wine/i386-unix` and `ls /usr/lib/wine/x86_64-unix`.

### Level 3: TrackIR + Wine + X-Plane
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 4: TrackIR + Wine + X-Plane + Webcam
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 5: TrackIR + Wine + X-Plane + Webcam + OSC
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 6: + Face tracking
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

### Level 7: + Wiimote
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders -DENABLE_WEBCAM=ON -DENABLE_OSC=ON -DENABLE_FACE_TRACKER=ON -DENABLE_WIIMOTE=ON
cmake --build . -j$(nproc)
sudo cmake --install .
```

---

## First launch (Wayland is default on modern Arch)

On **Wayland**, if the GUI does not appear, run:
```bash
QT_QPA_PLATFORM=xcb ltr_gui
```
Otherwise:
```bash
ltr_gui
```

### Camera view / 3D preview quirks

Some users on **source builds** report weak camera preview or a broken 3D view while **in-game tracking still works**. Try `QT_QPA_PLATFORM=xcb`, update GPU/Mesa drivers, and in **Misc** settings try disabling heavy **3D** preview options. This is environment-dependent; there is no single guaranteed fix.

---

## Verification
```bash
ldconfig -p | grep linuxtrack
ltr_server1 --help
ltr_gui
ls /opt/lib/linuxtrack/wine_bridge/   # built bridge pieces (Level 2+)
ls /opt/share/linuxtrack/wine/linuxtrack-wine.exe 2>/dev/null || true  # NSIS installer when built
```

---

## Troubleshooting

### Wine bridge: `c000007b` or installer fails with AppImage-built bridge

The NSIS step runs **`check_data.exe`** built against **Wine Unix libraries**. An AppImage produced on another distro may embed a bridge that fails on Arch paths. **Fix:** build and install on Arch (see Level 2 and explicit `-DWINE_LIBS_PATH` / `-DWINE64_LIBS_PATH` above) so `linuxtrack-wine.exe` matches your system, or use the **AppImage’s** bridge installer as in [Choose your path](#choose-your-path). Details were consolidated from older troubleshooting notes now in [docs/archive/technical/](../archive/technical/).

### Wine issues (common)

| Problem | Solution |
|---------|----------|
| `wine: WINEARCH is set to 'win32' but this is not supported in wow64 mode` | Enable multilib; install `lib32-glibc` `lib32-gcc-libs`; add AUR **wine32** or **wine-stable** stack if needed — see [Wine, WOW64, and the 32-bit bridge](#wine-wow64-and-the-32-bit-bridge) |
| `wine-staging` conflicts | Remove conflicting packages: `sudo pacman -R wine-staging wine-gecko wine-mono winetricks` (adjust to your mix) |
| Missing 32-bit headers | Install `lib32-glibc` and `lib32-gcc-libs` (multilib). Default CMake bridge build does **not** require `lib32-mxml` / `lib32-liblo` |
| 32-bit/64-bit compilation conflicts | Use explicit 64-bit flags: `CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64"` |
| `zlib` and `zlib-ng-compat` in conflict (CachyOS etc.) | Answer **N** (do not remove zlib-ng-compat). Omit `zlib`; run `pkg-config --exists zlib` to confirm |
| Qt6 CMake config not found | `sudo pacman -S qt6-base qt6-tools` |
| CMake finds wrong Wine paths | Set `-DWINE_LIBS_PATH` and `-DWINE64_LIBS_PATH` to existing directories (see Level 2) |
| **`Wine plugin: disabled (winegcc/wineg++/makensis not found)`** | CMake requires **all three** on `PATH`. Install **`makensis`** (`./scripts/install/install_nsis_arch.sh` or AUR **nsis**). Install **Wine** / **wine32** so **`winegcc`** and **`wineg++`** exist. If you used **`--configure-only`** without a prior full run, run **`./scripts/build_arch_linux.sh --configure-only` again** with an updated script (it preflights NSIS/Wine/liblo) or install those manually, **`rm -rf build`**, reconfigure. |
| **`liblo` not found** / OSC disabled | `sudo pacman -S liblo` then `pkg-config --exists liblo`. Split **`--configure-only`** previously skipped this; current **`build_arch_linux.sh`** runs **`install_osc_support`** before configure. |
| **Webcam disabled** but V4L found | Default **`ENABLE_WEBCAM=OFF`**. Add **`-DENABLE_WEBCAM=ON`** to **`cmake`** (see Level 4) if you need webcam support. |

### Common issues

| Problem | Solution |
|---------|----------|
| `Couldn't load library 'libwc.so.0'` | `sudo ldconfig` |
| GUI not displaying on Wayland | `QT_QPA_PLATFORM=xcb ltr_gui` |
| Permission denied on device | `sudo usermod -a -G plugdev,input $USER` (re-login) |
| Application not in launcher | `sudo update-desktop-database /opt/share/applications` |

### 32-bit libraries (advanced)

For optional manual **lib32** dependency builds, see [ARCH_LINUX_32BIT_LIBRARIES.md](../ARCH_LINUX_32BIT_LIBRARIES.md).

### Qt6 tools PATH (rare)
```bash
export PATH="/usr/lib/qt6/bin:$PATH"
```

---

## Advanced: Custom Build Combinations

```bash
# Example: TrackIR + X-Plane without Wine/Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_XPLANE=ON -DXPLANE_SDK_PATH=/opt/xplane-sdk/CHeaders

# Example: Wine + OSC without Webcam
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON -DENABLE_OSC=ON
```

See the main README for all CMake options.

---

## Historical / archived notes

Older Arch–Wine investigations (autotools-era wording, duplicate guides) live under **[docs/archive/technical/](../archive/technical/)**. **Current** instructions are only this file plus the main [README.md](../../README.md).
