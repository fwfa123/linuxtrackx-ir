# src deslop cleanup plan

Date: 2026-11-03. Status: not started. This is the plan for a later pass. No `src/` changes have been made for it.

Findings: [SRC_DESLOP_REPORT.md](SRC_DESLOP_REPORT.md).

Clean up the deslop findings in `src` by collapsing duplicated blocks and removing debug noise, while keeping startup timing, return codes, and the success path of Steam/Lutris and MFC42 installs the same.

No automated tests cover these paths (`src/tests/` does not exercise the GUI, NPClient, or the extractor). Verification is a rebuild plus a short manual checklist.

## Work items

1. Dedup NPClient daemon start; send named master commands; keep waits and retry profile strings.
2. Share MFC42 download/hash/search helpers; continue the source loop on hash mismatch.
3. Drop `linuxtrack.c` stderr `DEBUG` prints and the empty 32-bit branch.
4. Share home lookup; rewrite to `/home/$USER` only when the current path is missing.
5. Dedup wine pick, pipe/udp launch, and dock/undock; delete unused methods; strip narrating comments.
6. Rebuild the GUI, `liblinuxtrack`, and NPClient; smoke-check prefs and Steam/Lutris paths on a normal `HOME`.

## What stays the same

These look like slop and are also load-bearing. Do not change them in this pass:

- `NP_StartDataTransmission` still returns 0 after a bad state on the non-MinGW path, and still does the five `get_tracking_state` calls, the `Sleep(50)` loop, the `Sleep(500)`, and the up-to-1-second wait in `NP_GetData`. Those waits were added for a crash at offset `0xA0`.
- Unknown-game registration still retries `linuxtrack_init("Default")`. Known-game registration still retries `gd.name`. The helper takes the profile string so the two call sites keep those exact arguments.
- Sleeps of 3s (`ltr_server1`) and 5s (`ltr_gui`) still run after a start attempt that the shell accepted. Absolute paths that are not executable are skipped before `system()`, so a missing `/opt/bin/ltr_server1` no longer burns 3 seconds. A bare `ltr_server1` / `ltr_gui` name still goes through `system()` plus `pgrep`, so PATH lookup is unchanged.
- Steam and Lutris still use `HOME` when that directory exists. The rewrite to `/home/$USER` runs only when the current home path does not exist and `/home/$USER` does. A normal home, sudo into an existing home, and a custom existing home stay put. The old hardcoded-path case (a home that is not on disk) still gets rewritten.
- Lutris-only fallbacks (`/etc/passwd`, then `"/home/unknown"`) stay in Lutris. They are not copied into Steam.
- `installToLutrisPrefix` keeps its runner-path and `winehq-staging` branches. Do not fold that function into `resolveWineBinaryPath`. Only drop the `printf("DEBUG:")` lines.
- TrackIR prefs keep the deferred `initializeUI` queued call, the parent check, and the early return when `TirThreshold` / `TirMaxBlob` / `TirMinBlob` are null. Per-widget checks for the other controls stay.

## Changes

### 1. NPClient registration and master commands

In [src/wine_bridge/client/NPClient_main.c](../../src/wine_bridge/client/NPClient_main.c), `NP_RegisterProgramProfileID` has the same daemon walk at about 1977–2098 and 2133–2253. `send_command_to_master` (line 1257) already connects and sends, and `LTR_CMD_WAKEUP` is 3 and `LTR_CMD_PAUSE` is 2.

- Add a static helper that takes the profile name and performs the socket probe, path walks, sleeps, and `linuxtrack_init` retry. Both branches call it.
- Replace the hand-rolled `/tmp/ltr_m_sock` blocks in start (2421–2441) and stop (2512–2529) with `send_command_to_master(LTR_CMD_WAKEUP, 0)` and `send_command_to_master(LTR_CMD_PAUSE, 0)`. Same command, one connection instead of connect-then-send.
- Strip the narrating comments around `linuxtrack_get_pose`. Leave the wait and the state checks.

### 2. MFC42 extractor

In [src/qt_gui/extractor.cpp](../../src/qt_gui/extractor.cpp):

- `downloadVCRedist` (1367): wget and curl each copy the hash check and the extract-method chain. On mismatch both log `trying next source` and `return false` (1446, 1480), which leaves the source loop. One helper should download, hash, and extract. Hash failure deletes the file and continues the source loop. A wget download whose extraction fails still falls through to curl, as it does now.
- `on_DownloadButton_pressed` (1813) is a single chosen source, not a source list. Use the same helper. A hash failure on wget tries curl for that same URL instead of returning. If both fail, the existing failure UI runs. Drop the `DEBUG: parts[i]` progress lines in both functions.
- `tryWinetricksInstall` (1030–1206): one private search/copy helper, called after `mfc42` and again after `vcrun6`. Remove the duplicated `system32/mfc42u.dll` and `syswow64/mfc42u.dll` entries (1078–1079). Drop the `find` / `ls` dumps from the progress log. Keep the success and "not found" lines.
- Delete the Step 1/2/3 comments in `extractFromVS6SP6`. Leave the `cabextract` calls.

### 3. Library load debug

In [src/linuxtrack.c](../../src/linuxtrack.c), remove the `fprintf(stderr, "DEBUG:` lines (60 sites) and keep the matching `linuxtrack_log` calls. Invert the empty `strstr(libname, "32")` branch at 392–394 so the 32-bit extra path is the only arm. Do not change the path list or the mandatory-symbol failure return.

### 4. Home directory

Steam ([src/qt_gui/steam_integration.cpp](../../src/qt_gui/steam_integration.cpp) 32–47 and 109–132) and Lutris ([src/qt_gui/lutris_integration.cpp](../../src/qt_gui/lutris_integration.cpp) 37–53 and 148–196) copy the same lookup and the same rewrite.

- Add `InstallerPaths::homeDirectory()` in [src/qt_gui/installer_paths.h](../../src/qt_gui/installer_paths.h) and the matching `.cpp`. Order: `HOME`, then `QStandardPaths::HomeLocation`, then `/home/$USER` only when `HOME` is empty.
- Both `initializePaths` methods call one rewrite helper: if the path is under `/home/<name>`, `<name>` is not `USER`, the path does not exist, and `/home/$USER` does exist, use `/home/$USER`. Otherwise keep the path. Log the rewrite when it happens.
- Lutris `getHomeDirectory` calls the shared lookup, then keeps its passwd and `"/home/unknown"` fallbacks only when the result is still empty.

### 5. Smaller GUI duplications and dead code

- [src/qt_gui/wine_launcher.cpp](../../src/qt_gui/wine_launcher.cpp): one "consider this wine candidate" function used by the path loop and the command loop inside `selectBestWineVersion`. Keep path-first, and keep the command loop only when no path won. Delete unused `isWineVersionCompatible` and `isWinetricksCompatible` (declared in the header, defined at 269 and 285, no callers). Remove the `// NEW:` banners. Update the two sample call sites in [WINE_VERSION_SELECTION_IMPLEMENTATION.md](WINE_VERSION_SELECTION_IMPLEMENTATION.md) so the doc no longer shows those calls.
- [src/qt_gui/ltr_gui.cpp](../../src/qt_gui/ltr_gui.cpp): one helper next to `findLinuxtrackLibPath` for the `env LD_LIBRARY_PATH=...` `startDetached` used by `ltr_pipe` (1076–1100) and `ltr_udp` (1269–1294). Empty lib path still launches the binary with its own arguments.
- [src/qt_gui/ltr_show.cpp](../../src/qt_gui/ltr_show.cpp): one parent-walk / top-level search used by `dockToMainWindow` and `undockFromMainWindow`. Slot names stay `dockTrackingWindow` and `undockTrackingWindow`. One comment for the two empty `setHotKey` failure bodies.
- [src/qt_gui/trackir_permission_dialog.cpp](../../src/qt_gui/trackir_permission_dialog.cpp): delete unused `installUdevRules()` (177) and its header declaration. Leave `installUdevRulesAndGroups` as the installer the dialog runs.
- [src/qt_gui/tir_prefs.cpp](../../src/qt_gui/tir_prefs.cpp): remove `Qt6 RADICAL FIX` paragraphs, `std::cout` traces, and `printf("Type: %d\n", tirType)`. After the early return, drop the second null check on the three widgets already tested. Keep the other `setValue` guards.
- Comment-only: [src/qt_gui/testing_section.cpp](../../src/qt_gui/testing_section.cpp) and [src/qt_gui/profile_selector.cpp](../../src/qt_gui/profile_selector.cpp). Leave `platformLabelToKey` and the `QString::fromUtf8` ASCII calls in place. The `if (this && ui.Profiles)` check can lose the `this` test.

### 6. Report

When the pass is done, add a short status note at the top of [SRC_DESLOP_REPORT.md](SRC_DESLOP_REPORT.md) listing what was cleaned and which behaviors were intentionally left (warmup sleeps, best-effort return 0, unknown-id `"Default"` retry).

## Regression check

Build the targets that include the edits: the GUI (`ltr_gui`), `liblinuxtrack` (from `linuxtrack.c`), and the MinGW `NPClient` target if the cross compiler is present.

Then, on a normal `HOME` that matches this user:

- GUI starts, TrackIR device prefs still show threshold and blob values.
- Steam and Lutris still resolve the usual `~/.steam` and `~/.local/share/lutris` locations. This is the path the rewrite does not touch.
- Wine version selection still prefers an existing `/opt/wine-staging/bin/wine` or `/opt/wine/bin/wine` at 9.0 or newer.

There is no game or Wine prefix in this pass, so NPClient and MFC42 download are reviewed by reading the helper call sites against the old branches, not by running a title.
