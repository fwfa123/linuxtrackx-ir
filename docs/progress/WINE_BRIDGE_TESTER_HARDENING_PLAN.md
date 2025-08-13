### Wine Bridge Tester Hardening Plan

- **Scope**: Stabilize and clean up the `tester` binary under `src/wine_bridge/tester`.
- **Goal**: Correct UI data presentation, fix resource handling, make initialization/shutdown robust, and reduce duplication.
- **Status**: Planned
- **Owners**: TBD
- **Last updated**: 2025-08-13

#### Task checklist

- [x] Fix float display in UI
  - Files/functions: `src/wine_bridge/tester/main.cpp` → `TimerProcedure`
  - Edits: use `snprintf` + `SetDlgItemText` for `pitch`, `roll`, `yaw`, `tx`, `ty`, `tz` (and `padding[]` if needed)
  - Acceptance: values show with decimals (e.g., 12.34), no truncation

- [x] Correct `GetDlgItemInt` usage
  - Files/functions: `main.cpp` → `DialogProc` `IDSTART` branch
  - Edits: `BOOL ok = FALSE; UINT num = GetDlgItemInt(..., &ok, FALSE);` (no casts); default to 2307 when `!ok`
  - Acceptance: valid AppID read; invalid input falls back to 2307

- [x] Handle timer lifecycle and errors
  - Files/functions: `main.cpp` → `DialogProc` `IDSTART` branch
  - Edits: kill existing timer before starting; check `SetTimer` return; on fail, log and don’t proceed
  - Acceptance: repeated Start works without multiple timers; timer ID nonzero; failures reported

- [x] Clean shutdown on dialog close
  - Files/functions: `main.cpp` → `DialogProc` `WM_CLOSE`
  - Edits: kill timer if set; call `npifc_close()`; then `EndDialog`
  - Acceptance: no timer callbacks after close; no further NPClient calls

- [x] Free `client_path()` result
  - Files/functions: `src/wine_bridge/tester/npifc.c` → `npifc_init`
  - Edits: `free(client)` after `LoadLibrary` attempt
  - Acceptance: no leaked `asprintf` buffer

- [x] Properly release NPClient DLL and null function pointers
  - Files/functions: `npifc.c` → `npifc_close`
  - Edits: call `FreeLibrary(npclient); npclient = NULL;` set all `NP_*` function pointers to `NULL`; `initialized = false`
  - Acceptance: multiple init/close cycles work without crashes; no dangling calls after close

- [x] Robust registry error handling
  - Files/functions: `npifc.c` → `client_path`
  - Edits: check `LONG` return values from `RegOpenKeyEx` and `RegQueryValueEx`; print clear errors; handle zero-length buffer; ensure `RegCloseKey` on all paths
  - Acceptance: on missing key/value, function returns `NULL` and caller handles gracefully

- [x] Only update UI on successful data fetch
  - Files/functions: `main.cpp` → `TimerProcedure`; `npifc.c` → `npifc_getdata`
  - Edits: in timer, call `npifc_getdata(&td)` and only update controls if return indicates success
  - Acceptance: UI doesn’t flicker garbage when NPClient fails

- [x] De-duplicate `tir_data_t` definition
  - Files/functions: `src/wine_bridge/tester/npifc.h`, `src/wine_bridge/client/NPClient_dll.h`
  - Edits: remove duplicate struct from `tester/npifc.h`; include the canonical header (or centralize in shared header)
  - Acceptance: single source of truth; project builds

- [ ] Optional: show NP signatures/version in UI
  - Files/functions: `npifc.c` (`NP_GetSignature`), `main.cpp`
  - Edits: display DLL/app signatures and/or version in the dialog or log
  - Acceptance: signatures visible for debugging

#### Runtime portability improvements

- [x] Make linuxtrack library discovery robust across distros
  - Files/functions: `src/linuxtrack.c` → `linuxtrack_get_prefix`, `linuxtrack_find_library`
  - Edits: add `USERPROFILE` fallback for Wine; add absolute fallbacks to `/usr/local/lib/linuxtrack` and `/usr/lib/linuxtrack` (both arch dirs)
  - Acceptance: NPClient init works without setting `LINUXTRACK_LIBS` env var on common distros

#### Build and test

- Build the tester target via the existing `Makefile` in `src/wine_bridge/tester` and ensure it succeeds.
- Runtime checks:
  - Start with default `2307`; verify profile registration and periodic UI updates
  - Enter invalid AppID; verify graceful fallback or error
  - Press Start repeatedly; verify only one timer is active
  - Close dialog; no further logs or callbacks
  - Start/Close cycle multiple times; no crashes or leaks


