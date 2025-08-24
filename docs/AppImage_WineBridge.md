# AppImage Wine Bridge Guide (32-bit and 64-bit)

This guide explains how the LinuxTrack X-IR AppImage bundles both 64-bit and 32-bit runtimes and how to use them with Wine prefixes.

## What’s included
- 64-bit runtime libraries under `usr/lib/linuxtrack/`
- 32-bit runtime library under `usr/lib/i386-linux-gnu/linuxtrack/`:
  - `liblinuxtrack.so.0` -> `liblinuxtrack.so.0.0.0`
- AppRun sets `LD_LIBRARY_PATH` to include both paths
- Wine Bridge scripts set `LINUXTRACK_LIBS` to the bundled 32-bit lib when present

## Use with your Wine prefix
- Standard install of Wine Bridge from AppImage:
  - In the AppImage contents: `wine_bridge/scripts/install_wine_bridge.sh` will set `LINUXTRACK_LIBS` to the bundled 32-bit lib if found
- Manually run a 32-bit app with bundled lib:
```bash
/path/to/LinuxTrack-X-IR-*.AppImage --appimage-extract
cd squashfs-root/wine_bridge/scripts
./run_with_ltr32.sh "C:\\Program Files\\Linuxtrack\\Tester.exe"
```

## Environment variable (manual)
If you prefer manual control:
```bash
export LINUXTRACK_LIBS="/path/to/squashfs-root/usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0"
WINEPREFIX=/path/to/your/32bit/prefix wine "C:\\Program Files\\Linuxtrack\\Tester.exe"
```

## Verifying the loader
Check the latest loader log to confirm dlopen success:
```bash
ls -t /tmp/linuxtrack.log* 2>/dev/null | head -n1 | xargs -r cat
# Look for: "Loaded OK." and ensure no ELFCLASS64 or Not found messages
```

## Notes
- The 32-bit runtime is bundled on a best-effort basis during AppImage build. If the local toolchain lacks 32-bit support, the AppImage will still work for 64-bit; 32-bit support would then require system installation of the 32-bit runtime.
- When available, the AppImage automatically exposes the bundled 32-bit lib to Wine installers via `LINUXTRACK_LIBS`.


