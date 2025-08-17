## Review: 0006_TESTER_GAME_ID_AUTOFILL_PLAN

### Summary
- Auto-fill is implemented in both testers with the specified precedence: `LTR_GAME_ID` → Steam mapping (`SteamAppId`/`SteamGameId` via local file) → `LTR_GAME_NAME` lookup → default `2307`.
- Shared helpers exist in `src/wine_bridge/client/rest.c` for both name and Steam AppID lookups; GUI sets `LTR_GAME_NAME` when launching testers.
- Minimal logging is present (stdout plus a fallback log file under `~/.config/linuxtrack/tester_autofill.log` or inside `WINEPREFIX`).

### What matches the plan
- Env override respected: `LTR_GAME_ID` is parsed and used on init in both `src/wine_bridge/tester/main.cpp` and `src/wine_bridge/ft_tester/main.cpp`.
- Steam Proton hints implemented: testers read `SteamAppId`/`SteamGameId` and consult `~/.config/linuxtrack/tir_firmware/steam_to_trackir_id.txt` via `game_data_find_id_by_steam_appid`.
- `LTR_GAME_NAME` lookup implemented via `game_data_find_id_by_name`.
- Fallback to `2307` maintained when nothing resolves.
- Optional GUI integration: `src/qt_gui/testing_section.cpp` sets `LTR_GAME_NAME` for Steam, Lutris, and Custom Prefix execution paths.

### Implementation notes
- `game_data_find_id_by_name` rules align with the plan: exact match (case-insensitive), then contains-match with longest query length preference, then a fuzzy longest common substring score with lowest-ID tie-break. Parsing reads `~/.config/linuxtrack/tir_firmware/gamedata.txt` and supports encrypted entries.
- Logging: testers print which source was used and also append to a rotating path via `append_log` with multiple fallbacks (HOME → WINEPREFIX → executable directory).
- `ft_tester` uses control `IDC_NUM` to display the ID; `Tester.exe` uses `IDC_APPID`.

### Gaps and risks
- Case-folding is ASCII-only; non-ASCII/UTF-8 titles may not match as expected.
- `strcasecmp` is used; ensure portability on MinGW/Windows builds (guard with `_stricmp` if needed).
- In `ft_tester`, once the timer runs, `IDC_NUM` is updated with FreeTrack `dataID`, which can visually override the initial auto-filled ID; this is a UX nuance rather than a functional blocker.

### Recommendations
- Consider UTF-8–aware case-insensitive comparison (or document ASCII-only matching) in `rest.c`.
- Add a small portability guard for `strcasecmp` on Windows toolchains.
- Optionally unify the logging helper between testers to avoid duplication.

### Code references
```140:175:src/wine_bridge/tester/main.cpp
case WM_INITDIALOG: {
  int initial_id = 2307;
  char *env_id = getenv("LTR_GAME_ID");
  if (env_id && *env_id) { int v = atoi(env_id); if (v > 0) { initial_id = v; printf("INFO: Using LTR_GAME_ID=%d\n", v); append_log("Using LTR_GAME_ID=%d", v); } }
  else {
    char *steam_appid = getenv("SteamAppId"); if (!(steam_appid && *steam_appid)) { steam_appid = getenv("SteamGameId"); }
    int mapped_id = -1;
    if (steam_appid && *steam_appid && game_data_find_id_by_steam_appid(steam_appid, &mapped_id) && mapped_id > 0) {
      initial_id = mapped_id; printf("INFO: Using Steam mapping SteamAppId=%s => TrackIR ID=%d\n", steam_appid, mapped_id);
      append_log("Using Steam mapping SteamAppId=%s => TrackIR ID=%d", steam_appid, mapped_id);
    } else {
      char *env_name = getenv("LTR_GAME_NAME"); if (env_name && *env_name) { int found_id = -1;
        if (game_data_find_id_by_name(env_name, &found_id) && found_id > 0) {
          initial_id = found_id; printf("INFO: Using LTR_GAME_NAME match '%s' => TrackIR ID=%d\n", env_name, found_id);
          append_log("Using LTR_GAME_NAME match '%s' => TrackIR ID=%d", env_name, found_id);
        }
      }
    }
  }
  if (initial_id == 2307) { printf("INFO: Auto-fill not found; using default TrackIR ID=2307\n"); append_log("Auto-fill not found; using default TrackIR ID=2307"); }
  SetDlgItemInt(hwndDlg, IDC_APPID, (UINT)initial_id, TRUE);
}
```

```247:281:src/wine_bridge/ft_tester/main.cpp
case WM_INITDIALOG:
  SetDlgItemText(hwndDlg, IDC_TITLE, "Default");
  {
    int initial_id = 2307; char *env_id = getenv("LTR_GAME_ID");
    if (env_id && *env_id) { int v = atoi(env_id); if (v > 0) { initial_id = v; printf("INFO: Using LTR_GAME_ID=%d\n", v); append_log("Using LTR_GAME_ID=%d", v); } }
    else {
      char *steam_appid = getenv("SteamAppId"); if (!(steam_appid && *steam_appid)) { steam_appid = getenv("SteamGameId"); }
      int mapped_id = -1;
      if (steam_appid && *steam_appid && game_data_find_id_by_steam_appid(steam_appid, &mapped_id) && mapped_id > 0) {
        initial_id = mapped_id; printf("INFO: Using Steam mapping SteamAppId=%s => TrackIR ID=%d\n", steam_appid, mapped_id);
        append_log("Using Steam mapping SteamAppId=%s => TrackIR ID=%d", steam_appid, mapped_id);
      } else {
        char *env_name = getenv("LTR_GAME_NAME"); if (env_name && *env_name) { int found_id = -1;
          if (game_data_find_id_by_name(env_name, &found_id) && found_id > 0) {
            initial_id = found_id; printf("INFO: Using LTR_GAME_NAME match '%s' => TrackIR ID=%d\n", env_name, found_id);
            append_log("Using LTR_GAME_NAME match '%s' => TrackIR ID=%d", env_name, found_id);
          }
        }
      }
    }
    if (initial_id == 2307) { printf("INFO: Auto-fill not found; using default TrackIR ID=2307\n"); append_log("Auto-fill not found; using default TrackIR ID=2307"); }
    SetDlgItemInt(hwndDlg, IDC_NUM, (UINT)initial_id, TRUE);
  }
```

```226:296:src/wine_bridge/client/rest.c
bool game_data_find_id_by_name(const char *name, int *out_id) { /* exact → contains → fuzzy with lowest-ID tie-break */ }
bool game_data_find_id_by_steam_appid(const char *steam_appid, int *out_id) { /* parse steam_to_trackir_id.txt */ }
```

```641:645:src/qt_gui/testing_section.cpp
// Provide game name hint to tester for auto-ID lookup
if (!currentGame.isEmpty()) { env.insert(QString::fromUtf8("LTR_GAME_NAME"), currentGame); }
```

### Verdict
Plan is implemented with correct precedence, shared helpers, and basic logging. Recommend minor portability and UTF-8 handling improvements; otherwise, behavior matches the feature brief and targeted files.

