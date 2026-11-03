# src deslop report

Date: 2026-11-03. Scope: `src/`. This is an audit, not a cleanup. Nothing in `src/` was changed for this pass.

Follow-up, not yet started: [SRC_DESLOP_CLEANUP_PLAN.md](SRC_DESLOP_CLEANUP_PLAN.md).

## How this was read

Pattern search across `src/`, then a close read of the largest and newest files: the Wine NPClient, the MFC42 extractor, library path loading, TrackIR prefs, Steam/Lutris integration, the Wine launcher, and the GUI launch paths for `ltr_pipe` / `ltr_udp`.

Generated lexer/parser files (`pref_flex.*`, `pref_bison.*`) were left out. Original project TODOs and comments that explain a real constraint (X11 grab order, protocol layout, USB timing) were left out.

Recent curve-editor work (`spline.c`, `axis.c`, `scurve.cpp`, `scview.cpp`, `pose_crosshair_*`), `mingw_axes_prefs.c`, `keyb_x11.cpp`, `ltr_udp.c`, and the master/slave server sources did not show this layer. Old commented-out `printf`s in the server are leftover debug from the original tree, not a new coat of narration.

Severity:

- **High** — duplicated logic that will drift, or a comment that describes a different behavior than the code.
- **Medium** — real duplication or debug noise, behavior is otherwise local.
- **Low** — comments only.

## High

### `src/wine_bridge/client/NPClient_main.c` — daemon start pasted twice

`NP_RegisterProgramProfileID` contains the same startup sequence twice: once when gamedata has a name (about 1977–2098) and again for an unknown game id (about 2133–2253). Each copy probes `/tmp/linuxtrack.sock` with `system("test -S ...")`, walks `/opt/bin`, `/usr/local/bin`, `/usr/bin`, and `PATH` for `ltr_server1`, runs `pgrep` inside that loop, backgrounds the binary with `system("%s > /dev/null 2>&1 &")`, sleeps 3 seconds, then repeats the walk for `ltr_gui` with a 5 second sleep.

A backgrounded shell command returns success when the shell managed to launch the job. A missing binary still takes the sleep, then fails the later `pgrep` check. The two copies already differ: the known-game path retries `linuxtrack_init(gd.name)`, the unknown-id path retries `linuxtrack_init("Default")` even after it stored `g_profile_name` as Operation Flashpoint or ArmA.

Pull this into one helper that takes the profile name. Check that the executable exists before starting it, and do not sleep on a failed start.

### `src/wine_bridge/client/NPClient_main.c` — startup that logs failure and continues

`NP_StartDataTransmission` (about 2363–2470), on the non-MinGW path, initializes, waits, then keeps going when the state is still bad. The log lines say `continuing in best-effort mode`. It then calls wakeup, request-frames, and notification-on, and logs again if the state is still bad.

After that it "warms up" by calling `linuxtrack_get_tracking_state()` five times with `Sleep(50)`, then `Sleep(500)`. The comments tie this to a crash at offset `0xA0` and say the library is not ready even when the state is `RUNNING`.

`NP_GetData` (about 1724–1791) adds another forced wait of up to 1 second after start, then repeats the state checks. Comments around the pose call say a crash there is in the library, not this file.

The MinGW path in the same function (2336–2360) returns on a missing `FT_SharedMem` mapping and does not do the warmup sleeps. The two builds of one export no longer share a failure policy.

The extra socket ping (2421–2441, and the matching block in `NP_StopDataTransmission` at 2512–2529) opens `/tmp/ltr_m_sock` by hand and sends raw command `3` on start and `2` on stop. The MinGW path uses `LTR_CMD_WAKEUP` and `LTR_CMD_PAUSE`. Collapse the socket block into the existing `send_command_to_master` path and use the named commands.

### `src/qt_gui/extractor.cpp` — SHA256 failure claims to try the next source

`Mfc42uWinetricksExtractor` download loop (about 1399–1508) says it tries each source until one works. On a hash mismatch both the wget copy (1445) and the curl copy (1480) log `SHA256 verification failed, trying next source...` and then `return false`. Later sources are never tried. The wget and curl blocks also duplicate the hash check and the extract-method `if/else`. `continue` is the behavior the log already describes. One download-and-verify helper should own both tools.

The same function dumps `DEBUG: parts[i]=...` into the user-visible progress log (1407–1428).

### `src/qt_gui/extractor.cpp` — MFC42 search written twice, with duplicate paths

After `winetricks mfc42` (1030–1122) the code runs `find` for `*.dll`, `*mfc*`, and `*vc*`, then `ls` of the prefix and of `drive_c`, and prints all of it through `progress()`. `searchPaths` lists `system32/mfc42u.dll` and `syswow64/mfc42u.dll` twice (1072–1073 and 1078–1079). After `vcrun6` (1153–1206) the same finds and the same walks run again, introduced by `Search again with the same comprehensive paths`.

One function should search the prefix and copy the DLLs. The directory dumps belong behind a debug flag, not in the progress log. `extractFromVS6SP6` (1526 onward) is also labeled Step 1 / Step 2 / Step 3 in comments that restate the `cabextract` and copy calls. Low priority next to the duplicated search.

### `src/linuxtrack.c` — stderr debug on every symbol load

`linuxtrack_load_functions` (278–295) writes a `fprintf(stderr, "DEBUG: ...")` line for every symbol, then writes the same text through `linuxtrack_log`. `get_install_prefix_from_lib_path` and `build_dynamic_search_paths` do the same for every candidate. There are 60 `fprintf(stderr, "DEBUG:` sites in this file.

`build_dynamic_search_paths` (392–394) has an empty branch:

```c
if(strstr(libname, "32") != NULL){
  // Already looking for 32-bit library, paths are already added above
} else {
```

Drop the stderr traces and keep `linuxtrack_log`. Invert the 32-bit test so the empty arm goes away.

### `src/qt_gui/tir_prefs.cpp` — Qt6 commentary and cout traces

Constructor, `SetupUI`, `Activate`, and `initializeUI` (61–304) are wrapped in `Qt6 RADICAL FIX` paragraphs that restate the next call, plus `std::cout` lines (`Scheduling UI initialization...`, `All critical UI widgets validated...`). `Activate` also still has `printf("Type: %d\n", tirType)` at line 155.

`initializeUI` returns if `TirThreshold`, `TirMaxBlob`, or `TirMinBlob` is null (185), then guards every later `setValue` again (`Set all UI values with null checks`, 206–214). The second layer does not add a check the first one missed for those three widgets.

### `src/qt_gui/steam_integration.cpp` and `src/qt_gui/lutris_integration.cpp` — home directory rewritten from `USER`

Both `initializePaths` implementations (steam 32–47, lutris 37–53) share a block labeled `CRITICAL: Verify that the home directory is not a hardcoded build-time path`. If `HOME` is `/home/<someone>` and that name differs from `USER`, both replace it with `/home/$USER`. That fires for any home that is not `/home/$USER` (sudo, a custom home, a container), not only for a path baked in at compile time.

Lutris `getHomeDirectory` then continues through `/etc/passwd` and a last-resort `"/home/unknown"`. Steam and Lutris should share one home lookup, and a mismatched username should be logged rather than rewritten.

`installToLutrisPrefix` (about 1028–1134) resolves `~`, `$HOME`, absolute paths, and `/bin/wine` again, with `printf("DEBUG: ...")` and the comment `Resolve provided wineVersion into an executable path robustly`. `resolveWineBinaryPath` (212–243) already does this. Only the non-absolute branch calls the helper.

## Medium

### `src/qt_gui/wine_launcher.cpp` — repeated version pick, unused helpers

`selectBestWineVersion` (104–194) picks the newest staging-vs-regular version once for filesystem paths and again for the `wine` / `wine-staging` commands. Almost every helper added in this file is bannered `// NEW:`.

`isWineVersionCompatible` and `isWinetricksCompatible` are declared in `wine_launcher.h` and defined here, and nothing calls them.

### `src/qt_gui/ltr_gui.cpp` — `ltr_pipe` and `ltr_udp` launch copied

Starting `ltr_pipe` (1076–1100) and `ltr_udp` (1269–1294) is the same block: find the lib dir, note that `QProcess::startDetached()` cannot take a `QProcessEnvironment`, build `env LD_LIBRARY_PATH=...`, and if the lib path is empty launch anyway because it `might work if library is in standard system paths`. One helper that takes the program and the argument list would hold the comment once.

### `src/qt_gui/ltr_show.cpp` — dock and undock

`dockToMainWindow` (372–390) and `undockFromMainWindow` (392–407) walk parents, search top-level widgets for `LinuxtrackGui`, and `invokeMethod`. The only difference is the slot name. The dock copy opens with `Try to find the main window (LinuxtrackGui) robustly`.

`showEvent` (689–700) ignores a failed `setHotKey` in an empty body. The same three-line comment is pasted for tracking toggle and recenter.

### `src/qt_gui/trackir_permission_dialog.cpp` — unused installer

`installUdevRules()` (177–240) is declared and defined and never called. It writes only the TrackIR `131d` rule and ignores `chmod` / `udevadm` failures. The installer the dialog actually runs is `installUdevRulesAndGroups` (about 296–367). UI setup comments in the same file (`Checkbox for don't show again`, `Create the udev rules content`) only name the next widget.

### `src/qt_gui/testing_section.cpp` — line-by-line UI comments

`setupUI` and the slots (about 75–309) narrate each statement: `Connect UI elements to member variables`, `Set initial state`, `Populate with filter applied`, `Start tracking when tester selection changes`. `platformLabelToKey` (160–163) only forwards to `toPlatformKey`.

## Low

### `src/qt_gui/profile_selector.cpp`

New comments sit on the old style: `Set the current index first`, `Qt6.5: Add null check`, and `Qt6 RADICAL FIX` on the destructor. The timer lambda checks `if (this && ui.Profiles)`. `this` is not null inside a member lambda. `"Default"` and `""` go through `QString::fromUtf8` even though they are ASCII.

### `src/qt_gui/steam_integration.cpp` Proton path comments

Around 673–769 the path extraction is real. The notes `Construct the path more robustly` and `This is a best-effort fallback when path extraction fails` do not add a constraint the code does not already show.

## Suggested cleanup order

1. NPClient registration: one daemon-start helper, stop sleeping on a missing binary, and make the unknown-id retry use the profile it just stored.
2. NPClient start/stop: one failure policy, drop the warmup sleeps, send named master commands.
3. Extractor: `continue` on SHA256 mismatch, one search helper, drop progress-log `DEBUG:` dumps and the duplicated `searchPaths` entries.
4. `linuxtrack.c`: drop `fprintf(stderr, "DEBUG:` and keep the log.
5. Steam and Lutris: one home lookup that does not rewrite `HOME`.
6. Delete or call `installUdevRules`, `isWineVersionCompatible`, and `isWinetricksCompatible`.
7. Comment-only pass on `tir_prefs.cpp`, `ltr_show.cpp`, `testing_section.cpp`, and `profile_selector.cpp` after the behavior items above.
