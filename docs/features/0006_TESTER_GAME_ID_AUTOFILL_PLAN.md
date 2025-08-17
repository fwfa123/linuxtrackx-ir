## Tester Game ID Auto-fill from gamedata.txt

### Brief
When the Wine bridge testers (`Tester.exe` 32-bit, `Tester64.exe` 64-bit) and `ft_tester` are launched inside a Wine prefix, they currently default the Application ID field to `2307`. Instead, they should attempt to auto-detect the correct LinuxTrack Application ID by matching the running game's name to an entry in `~/.config/linuxtrack/tir_firmware/gamedata.txt`, and pre-fill the UI field accordingly. Keep manual override possible.

### Targeted Files and Functions
- `src/wine_bridge/tester/main.cpp`
  - WM_INITDIALOG: currently `SetDlgItemInt(hwndDlg, IDC_APPID, 2307, true);`
  - `IDSTART` handler: reads `IDC_APPID`, falls back to `2307`
- `src/wine_bridge/ft_tester/main.cpp`
  - WM_INITDIALOG / dialog init: no current auto-fill logic; add same behavior
- `src/wine_bridge/client/rest.c`
  - `game_data_get_desc(int id, game_desc_t *gd)`: parses `gamedata.txt`
  - Add new helper: `bool game_data_find_id_by_name(const char *name, int *out_id)` (exact + fuzzy match)
- Optional small utility (if preferred to avoid duplication):
  - New file `src/wine_bridge/client/gamedata_lookup.{h,c}` with `game_data_find_id_by_name`

### Detection Algorithm (in testers on startup)
1) Try explicit override via environment:
   - If `LTR_GAME_ID` is set and parseable as integer, use it to pre-fill `IDC_APPID`.
2) Try semantic match via environment-provided game name:
   - If `LTR_GAME_NAME` is set, call `game_data_find_id_by_name(LTR_GAME_NAME, &id)`. If found, pre-fill.
3) Steam Proton hints (best-effort, no hard dependency):
   - If `SteamAppId` or `SteamGameId` is set, optionally consult a lightweight mapping file if present:
     - `~/.config/linuxtrack/tir_firmware/steam_to_trackir_id.txt` format: `<steam_appid>=<ltr_id>` per line.
     - If a mapping exists, use it. Otherwise skip (no network or Steam APIs).
4) Fallback to default:
   - If none of the above resolved an ID, leave the existing default `2307` behavior.

Notes:
- Do not introduce network or heavyweight dependencies; only rely on env vars and local files.
- Keep the UI editable; user can still change the ID and click Start.

### Matching Rules in `game_data_find_id_by_name`
1) Exact name match (case-insensitive) against the quoted name in `gamedata.txt`.
2) Prefix/suffix-insensitive contains match (case-insensitive), preferring the longest match.
3) If multiple candidates, choose the one with the longest common subsequence length; tie-break by lowest ID.
4) Return `false` if no reasonable match is found.

### UI Behavior Changes (testers)
- On dialog init (WM_INITDIALOG):
  - Attempt auto-detection following the algorithm above and set `IDC_APPID` if found.
  - If not found, keep `2307`.
- On Start (IDSTART):
  - Maintain existing logic: read `IDC_APPID`; if parse fails, fallback remains `2307`.

### Optional Integration (improves accuracy, but not required)
- When launching testers from the Qt GUI (`TestingSection::executeTester`), set helpful environment variables:
  - `LTR_GAME_ID` (if the game’s TrackIR ID is known in GUI layer)
  - `LTR_GAME_NAME` (selected game name from Steam/Lutris UI)
  - This allows testers to resolve ID even without Steam/Lutris-specific parsing.

### Implementation Steps
1) Implement `game_data_find_id_by_name` alongside `game_data_get_desc` (either in `rest.c` or new `gamedata_lookup.c`).
2) Update `src/wine_bridge/tester/main.cpp` WM_INITDIALOG to:
   - Check `LTR_GAME_ID` → set `IDC_APPID`
   - Else check `LTR_GAME_NAME` → call `game_data_find_id_by_name` and set `IDC_APPID` on success
   - Else check optional `steam_to_trackir_id.txt` using `SteamAppId`/`SteamGameId`
   - Else keep `2307`
3) Mirror the same logic in `src/wine_bridge/ft_tester/main.cpp`.
4) Minimal logging to stdout for visibility (what source was used, or fallback triggered).

### Edge Cases
- Missing `gamedata.txt`: testers proceed with default (2307) and log a message.
- Ambiguous name matches: pick best per rules; log candidate count and chosen ID.
- Non-ASCII names: use UTF-8 compare; ensure parsing buffer sizes are sufficient.


