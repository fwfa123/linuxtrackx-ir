## Winetricks detection and execution robustness for Fedora

Brief: On a fresh Fedora install, the extractor reports that it cannot find `winetricks` even though it is present at `/usr/bin/winetricks` (user confirmed via `which winetricks` → `/usr/bin/winetricks`). Expand detection and invocation logic so the MFC42 flow reliably finds and runs winetricks on Fedora (and similar environments, including sandboxed cases).

### Scope and targets
- Files to update
  - `src/qt_gui/extractor.cpp`
    - `Mfc42uWinetricksExtractor::isWinetricksAvailable()`
    - `Mfc42uWinetricksExtractor::tryWinetricksInstall()`
  - `src/qt_gui/extractor.h`
    - Declarations for new helper(s) and member(s)
- Optional copy adjustments
  - `src/qt_gui/plugin_install.cpp` (messages that suggest how to proceed if winetricks is not found)

### Detection and invocation design
1) Replace boolean-only availability with a path-resolving locator
   - Add `QString locateWinetricks()` that returns the resolved program path or an empty string if not found.
   - Search order:
     - `QStandardPaths::findExecutable("winetricks")` using current `PATH`.
     - Try absolute fallbacks commonly present on Fedora and other distros:
       - `/usr/bin/winetricks`
       - `/usr/local/bin/winetricks`
       - `/bin/winetricks`
     - If running under a Flatpak/sandboxed environment (detect via `QProcessEnvironment` keys like `FLATPAK_ID`, `container`, or presence of `/app`), attempt host lookup:
       - Probe: run `flatpak-spawn --host which winetricks` and parse result.
       - If found, record program=`flatpak-spawn` and prefix args=`{"--host", "/full/path/to/winetricks"}` for later execution.

2) Cache the resolution
   - Add members to `Mfc42uWinetricksExtractor`:
     - `QString winetricksProgram_` (actual program to execute; either `winetricks`, an absolute path like `/usr/bin/winetricks`, or `flatpak-spawn`).
     - `QStringList winetricksPrefixArgs_` (arguments that must precede our own arguments; empty in the normal case; `{"--host", "/usr/bin/winetricks"}` in the Flatpak-host case).
   - Populate these once (e.g., first call to `locateWinetricks()`), and reuse thereafter.

3) Update `isWinetricksAvailable()`
   - Use `locateWinetricks()`; if empty, return false.
   - Verify functionality by running the resolved program with `winetricksPrefixArgs_ + {"--version"}` and a longer timeout (e.g., 10s).
   - Consider `QProcess::exitStatus()` and `exitCode()`; only accept `NormalExit` with `exitCode == 0`.
   - Log the discovered path and the environment `PATH` used for diagnostics.

4) Update `tryWinetricksInstall()` to use the resolved program
   - Build the argument list as: `QStringList args = winetricksPrefixArgs_; args << "mfc42";`
   - Ensure `QProcessEnvironment env` has a robust `PATH` that includes typical system locations even for GUI launches that inherit a minimal environment:
     - Merge current `PATH` with `"/usr/bin:/usr/local/bin:/bin:/usr/sbin:/sbin"` if missing.
   - Keep existing `WINEPREFIX` and `WINEARCH=win32` setup intact.
   - Increase the initial `--version` and main `waitForFinished` timeouts modestly (e.g., 10s for version; keep or slightly extend the install timeout as needed) and retain progress logging.
   - Log the exact command being executed (program + args) for debugging.

5) User feedback adjustments (optional but recommended)
   - If detection fails, the UI should mention Fedora explicitly, e.g.:
     - "Winetricks was not found. On Fedora, it is typically installed at `/usr/bin/winetricks`. If launching the GUI from a desktop icon, the `PATH` may be limited. Try launching from a terminal, or install via: `sudo dnf install winetricks`."
   - If detection succeeds via `flatpak-spawn --host`, show a note that host winetricks is being used.

### Step-by-step algorithm (summary)
1. On dialog open or before use, call `locateWinetricks()`.
2. If not found, show guidance (Fedora-focused message); skip to alternative installers.
3. If found, validate via `--version` with extended timeout and detailed logging.
4. For installation, run the resolved program with prefix args (if any) plus `mfc42` in the prepared `WINEPREFIX` and `WINEARCH` environment.
5. Continue with existing DLL discovery and copy logic.

### Notes and constraints
- Use `QStandardPaths::findExecutable` for primary discovery; avoid shell-specific assumptions.
- Do not rely solely on `PATH` when resolving `winetricks`; try absolute fallbacks.
- Keep existing progress and debug output; add explicit logs for resolved winetricks location and environment.

### Test matrix
- Fedora (Nobara/Fedora 42): `which winetricks` → `/usr/bin/winetricks` (per user), verify detection and `--version` success; run full mfc42 install.
- Environment with minimal `PATH` lacking `/usr/bin`: verify absolute path fallback succeeds.
- Flatpak/sandboxed run: verify `flatpak-spawn --host which winetricks` path resolution and execution work.
- Negative case: no winetricks installed; ensure helpful guidance and fallback to alternative installer path remains functional.

### Verbatim requirements from user
- "In this Fedora OS it is stating that it cannot find winetricks. Can we expand the logic so that it is detected and used correctly in Fedora also?"
- `which winetricks` prints `/usr/bin/winetricks` on the user’s system.


