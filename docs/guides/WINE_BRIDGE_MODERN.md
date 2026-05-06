# Modern Wine Bridge Guide (WOW64 + MinGW)

This guide describes the current LinuxTrack Wine bridge architecture and build/runtime requirements.

## Baseline Requirements

- Wine 11.0+ (or current Proton / Wine Staging equivalent)
- MinGW-w64 cross-compilers:
  - `i686-w64-mingw32-gcc`
  - `x86_64-w64-mingw32-gcc`
  - corresponding `windres` tools
- NSIS (`makensis`)

The bridge now builds native PE outputs (`.dll` / `.exe`) instead of legacy winegcc `.dll.so` / `.exe.so`.

## Produced Artifacts

Expected outputs in `build/.../src/wine_bridge/`:

- `client/NPClient.dll`
- `client/NPClient64.dll`
- `client/check_data.exe`
- `controller/Controller.exe`
- `tester/Tester.exe`
- `tester/Tester64.exe`
- `ft_client/FreeTrackClient.dll`
- `ft_tester/ftc.exe`
- `views/TrackIR.exe`
- `linuxtrack-wine.exe` (NSIS installer)

## Build Steps

Use an out-of-tree build directory:

```bash
mkdir -p build-wow64 && cd build-wow64
cmake .. -DCMAKE_INSTALL_PREFIX=/opt
cmake --build . -j"$(nproc)"
sudo cmake --install .
```

## Runtime Model

- Windows-facing bridge binaries are built as PE via MinGW.
- IPC remains AF_UNIX socket based.
- `check_data.exe` validates firmware data and signatures.
- Firmware symlink creation is handled by the native Linux side in `LutrisIntegration::runWineBridgeInstaller()`.

## Quick Verification

```bash
cmake -S . -B build-wow64-check
cmake --build build-wow64-check -j"$(nproc)"
file build-wow64-check/src/wine_bridge/client/NPClient.dll
ls build-wow64-check/src/wine_bridge/linuxtrack-wine.exe
```

Expected `file` output should report PE32/PE32+ Windows binaries.

