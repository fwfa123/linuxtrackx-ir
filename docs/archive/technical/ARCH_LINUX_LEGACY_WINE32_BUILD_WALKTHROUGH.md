# Arch Linux Legacy Wine32 Build Walkthrough

This page preserves the older Arch/CachyOS guidance for the former Wine Unix library / winegcc-style bridge build path. It is archived because the active branch now builds the Wine bridge as real MinGW PE artifacts (`NPClient.dll`, `NPClient64.dll`, tester executables, and installer payloads). The active Arch instructions live in [`../../readme/arch-linux.md`](../../readme/arch-linux.md).

## Why This Was Needed

Arch moved to a WOW64-style Wine packaging model. The older bridge build depended on classic 32-bit Wine Unix library paths such as:

- `/usr/lib32/wine/i386-unix`
- `/usr/lib32/wine`
- `/usr/lib/wine/x86_64-unix`

When those paths were missing or mismatched, CMake/Wine bridge builds could fail even when 64-bit Wine prefixes worked. Users on CachyOS and rolling Arch derivatives often had to install alternate Wine packages before configuring the project.

## Former Practical Tiers

1. **Tier A: Official repos**

   Enable `[multilib]` in `/etc/pacman.conf`, then install:

   ```bash
   sudo pacman -Syu
   sudo pacman -S wine wine-mono wine-gecko lib32-glibc lib32-gcc-libs
   sudo pacman -S lib32-wine 2>/dev/null || true
   ```

2. **Tier B: AUR `wine32`**

   This was reported working on CachyOS for users who needed the classic 32-bit tree under `/usr/lib32/wine/...`.

   ```bash
   paru -S wine32
   ```

   During install, `paru` / `pacman` could ask for a `lib32-jack` provider:

   - `lib32-pipewire-jack` fit most PipeWire desktops.
   - `lib32-jack2` fit classic JACK setups.

   If a terminal showed a patch under `less`, pressing `q` resumed the install. Running with `PAGER=cat` avoided that pager.

3. **Tier C: AUR `wine-stable` + `wine-stable-mono`**

   This was a heavier workaround and could replace repo `wine` / `wine-mono`, affecting other games.

## Former First-Time Walkthrough Notes

The older Arch walkthrough recommended installing `wine32` before the first CMake configure so the bridge build would not fail and force a repeated configure/build.

Former checkpoint table:

| Checkpoint | Pass/Fail | Notes |
|------------|-----------|-------|
| `[multilib]` enabled | | |
| `zlib` / `zlib-ng-compat` conflict | | What you chose |
| `ls /usr/lib32/wine/i386-unix` exists before configure | | e.g. after AUR `wine32` |
| `./scripts/build_arch_linux.sh` full run | | Where it stopped |
| MinGW toolchain warning/error vs build failure | | |
| `cmake` Wine paths auto vs `-DWINE_*` | | |
| `sudo cmake --install .` | | |
| `ltr_gui` with/without `QT_QPA_PLATFORM=xcb` | | |
| Wine bridge from local build | | Error snippet if any |
| Wine bridge via AppImage installer only | | |

## Former Explicit CMake Path Workaround

Older instructions sometimes used explicit Wine Unix library paths:

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/opt -DENABLE_LTR_32LIB_ON_X64=ON \
  -DWINE_LIBS_PATH=/usr/lib32/wine/i386-unix \
  -DWINE64_LIBS_PATH=/usr/lib/wine/x86_64-unix
```

That path workaround is no longer part of the active MinGW PE bridge build guidance.
