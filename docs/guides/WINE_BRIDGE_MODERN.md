# Modern Wine Bridge Guide (WOW64 + MinGW)

This guide describes the current LinuxTrack Wine bridge architecture and build/runtime requirements.

## Baseline Requirements

- Wine 11.0+ (or current Proton / Wine Staging equivalent)
- MinGW-w64 cross-compilers:
  - `i686-w64-mingw32-gcc`
  - `x86_64-w64-mingw32-gcc`
  - corresponding `windres` tools

NSIS is **not** required as of v2.0.0. The bridge builds native PE outputs (`.dll` / `.exe`) with MinGW only.

## Produced Artifacts

After `cmake --install`, payloads live under:

- `/opt/lib/linuxtrack/wine_bridge/` (or your `CMAKE_INSTALL_PREFIX`)

Key files:

- `NPClient.dll`, `NPClient64.dll`
- `check_data.exe`, `Controller.exe`, `Tester.exe`, `Tester64.exe`
- `FreeTrackClient.dll`, `ftc.exe` (installed as `FreeTrackTester.exe` in prefixes)
- `TrackIR.exe`

There is no `linuxtrack-wine.exe` NSIS installer in v2.0.0.

## Build Steps

Use an out-of-tree build directory:

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j"$(nproc)"
sudo cmake --install .
```

CMake reports `Wine bridge: enabled (MinGW toolchain)` when MinGW is found.

## Installing into a Wine/Proton prefix

All GUI entry points use the same **native install**:

1. Copy files from `lib/linuxtrack/wine_bridge/` into `Program Files/Linuxtrack` or `Program Files (x86)/Linuxtrack` (WOW64 prefixes).
2. Write registry keys with `wine reg` (`NaturalPoint`, `Freetrack`, `HKLM\SOFTWARE\Linuxtrack`). NaturalPoint and `Install_dir` must succeed; a Freetrack registry failure is logged only (some games need that key removed on disk instead — see `docs/GAME_WORKAROUNDS.md`).
3. Symlink firmware DLLs from `~/.config/linuxtrack/tir_firmware/`.

**GUI:** Gaming tab → Install Wine Bridge (Lutris, Steam Proton, or custom prefix).

**CLI:**

```bash
./scripts/install/install_wine_bridge.sh /path/to/prefix /path/to/wine
```

**Uninstall:** Delete the `Linuxtrack` folder under `Program Files` or `Program Files (x86)` and remove registry keys under `NaturalPoint`, `Freetrack`, and `HKLM\SOFTWARE\Linuxtrack`.

## Runtime Model

- Windows-facing bridge binaries are PE built via MinGW.
- IPC remains AF_UNIX socket based.
- `check_data.exe` validates firmware data (run automatically after GUI install; failures are logged only).

## Quick Verification

```bash
cmake -S . -B build-check
cmake --build build-check -j"$(nproc)"
file build-check/src/wine_bridge/client/NPClient.dll
ls /opt/lib/linuxtrack/wine_bridge/NPClient.dll   # after install
```

Expected `file` output should report PE32/PE32+ Windows binaries.
