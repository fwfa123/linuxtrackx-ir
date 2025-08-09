# Feature 0001 – Testing Section for Gaming Tab: Code Review

Date: 2025-08-09
Branch: `feature/testing-section-gaming-tab`
Reviewer: automated

---

## Summary
The Testing Section feature is largely scaffolded and integrated into the Gaming tab. UI elements and most interaction flows are present, Steam/Lutris discovery adapters exist, and tester launching paths are implemented for Steam (Proton), Lutris, and a Custom Prefix. Several items remain TODO or need corrective refinements to make the workflow robust and consistent across platforms.

---

## Alignment with Plan (0001_TESTING_SECTION_PLAN.md)
- UI (ltr.ui, ltr_gui.*)
  - Added `TestingGroupBox` with tester selection, platform selection, game list, and Run button. Connected signals to `LinuxtrackGui` and then to `TestingSection`.
  - Status: Implemented.

- Core logic (testing_section.*)
  - Tester selection, platform selection, Load Games, Run Tester flows are present. Uses `SteamIntegration`/`LutrisIntegration` for discovery and prefix lookup.
  - `startTracking()` exists but is a TODO. The plan requires automatic tracking on entering/testing.
  - Missing “install-if-missing” action: `offerWineBridgeInstallation()` is a placeholder; should call into existing Wine Bridge install flows.
  - Status: Partially implemented.

- Steam integration (steam_integration.*)
  - Library detection, manifest scanning, prefix detection, Proton version/path mapping implemented with detailed logging.
  - `runWineBridgeInstallerWithProton()` implemented; installer path currently hardcoded to `/usr/local/share/linuxtrack/linuxtrack-wine.exe` (see Issue S2 below).
  - Status: Implemented with caveats.

- Lutris integration (lutris_integration.*)
  - Uses SQLite (`QSqlDatabase`) to query games; parses YAML configs to find prefix/version and provides installer invocation via Wine.
  - Status: Implemented, but requires Qt SQL module in the build file (see Build B1).

- New files planned but not present
  - `tester_launcher.{h,cpp}` were planned in the document but are not present; equivalent logic was placed in `TestingSection::executeTester`.
  - Status: Document–code mismatch (see Doc D1).

---

## Issues and Risks

### Functional
1. T1 – Tester existence check uses executable bit
   - In `TestingSection::findTesterInPrefix()`, detection requires `QFileInfo::isExecutable()` on Windows `.exe` files inside Wine prefixes. These files typically do not have the executable bit set in the Linux filesystem. Result: false negatives.
   - Impact: Users get “Tester Not Found” even when present.
   - Fix: Treat existence as sufficient; don’t require `isExecutable()` for `.exe`.

2. T2 – Tester location specifics (Linuxtrack installers)
   - The tester binaries (`Tester.exe`, `FreeTrackTester.exe`) are Linuxtrack-provided tools installed by the Wine Bridge NSIS installers into the prefix under `$PROGRAMFILES\Linuxtrack` (default). They are not NaturalPoint/TrackIR vendor apps.
   - Impact: Search should prioritize `drive_c/Program Files/Linuxtrack` and `drive_c/Program Files (x86)/Linuxtrack` within the selected Wine prefix (and the user-selected InstallDir if customized), rather than NaturalPoint vendor paths.
   - Fix: Update `findTesterInPrefix()` to probe these Linuxtrack directories first; keep a minimal fallback over common Windows dirs if needed.

3. T3 – startTracking() is a TODO
   - Required by plan: tracking should start automatically at the beginning of testing workflow.
   - Fix: Reuse logic already used in `LinuxtrackGui` (e.g., `TRACKER.start("Default")` + messages) or factor to a helper and call from `TestingSection::startTracking()`.

4. T4 – Wine Bridge install integration is placeholder
   - `offerWineBridgeInstallation()` just shows a message.
   - Fix: Delegate to `PluginInstall` paths: Steam → `installSteamProtonBridge()`, Lutris → `installLutrisWineBridge()`, Custom Prefix → `installWineBridgeToCustomPrefix()`.

5. T5 – Steam/Lutris: path handling edge cases
   - Steam: `executeTester()` builds Linux path to `Tester.exe` and launches via `proton run`. This generally works if paths are correct. Ensure the path is the full Linux path under `<prefix>/drive_c/...` (current code searches both root and Windows dirs; OK once T1/T2 fixed).
   - Lutris: Forces `WINEARCH=win64` by default. Some testers are 32-bit only.
   - Fix: Detect tester bitness from filename (e.g., `Tester64.exe`) or from prefix and set `WINEARCH` accordingly, or let Wine default.

6. T6 – Custom Prefix discovery is TODO
   - Plan calls for manual prefix selection and validation; code returns empty list/path.
   - Fix: Allow directory chooser for “Custom Prefix” and validate structure before launching.

### Build/Packaging
1. B1 – Missing Qt SQL module in build file
   - `lutris_integration.cpp` uses `QSqlDatabase/QSqlQuery` but `ltr_gui.pro.in` does not include `QT += sql`.
   - Impact: Link failures or unresolved symbols for Qt SQL.
   - Fix: Add `QT += sql` to `ltr_gui.pro.in`.

2. B2 – Installer path inconsistency
   - Steam integration uses `/usr/local/share/linuxtrack/linuxtrack-wine.exe`, while Lutris integration searches under `/usr/local/share/linuxtrack/wine/linuxtrack-wine.exe` (and others).
   - Impact: One of the flows will fail to find the installer depending on packaging.
   - Fix: Centralize installer path resolution and use the same helper in both integrations; prefer probing multiple canonical locations. Ideally expose via a shared helper.

### Robustness/UX
1. U1 – Error handling/messages
   - Good coverage of message boxes; consider indicating which platform path failed (e.g., missing Proton path vs. missing tester vs. invalid prefix) and suggest remediation inline.

2. U2 – Verbose logging defaults
   - Steam integration logs are very verbose. Consider gating with an env var or debug flag to reduce noise in normal runs.

3. U3 – Platform defaults and HOME fallbacks
   - Steam/Lutris integrations try to correct hardcoded paths; one Steam fallback uses `~/...` (see S1).
   - Fix: Remove user-specific fallbacks and rely on environment + probing.

### Documentation/Plan
1. D1 – Planned files differ from implementation
   - `tester_launcher.*` were planned but logic was implemented inside `TestingSection`. Either create the launcher class or update the plan to reflect consolidated design.

### Security
- S1 – Avoid user-specific hardcoded paths
  - `SteamIntegration::getProtonPath()` has a default fallback to `~/...`; this should be removed.

- S2 – Centralize installer path detection
  - Hardcoded strings risk drift across packaging.

---

## Suggested Fixes (Conservative)
- TestingSection
  - Change tester detection to check existence only; prioritize `drive_c/Program Files/Linuxtrack` and `drive_c/Program Files (x86)/Linuxtrack` within the Wine prefix (and consider user-customized InstallDir if needed), with minimal generic fallbacks.
  - Implement `startTracking()` by reusing existing `TRACKER.start("Default")` invocation used elsewhere in GUI.
  - Implement `offerWineBridgeInstallation()` to call into `PluginInstall` depending on `currentPlatform`.
  - For Lutris launch, set `WINEARCH` based on tester name or omit to let Wine decide.
  - Add a simple directory picker for Custom Prefix to set `prefixPath` and validate it with existing helpers.

- Build
  - Add `QT += sql` to `src/qt_gui/ltr_gui.pro.in`.

- Steam/Lutris integrations
  - Remove `~/...` fallback.
  - Share a single helper for Wine installer path lookup; probe both `/usr/local/share/linuxtrack/wine/linuxtrack-wine.exe` and `/usr/share/linuxtrack/wine/linuxtrack-wine.exe` (and the AppImage bundle path if applicable).
  - Consider consolidating verbose logs under a debug flag.

- Documentation
  - Update `0001_TESTING_SECTION_PLAN.md` to reflect no separate `tester_launcher.*`, or introduce the class and move launching logic there.

---

## Action Items Checklist
- [ ] Add `QT += sql` to `ltr_gui.pro.in`.
- [x] TestingSection: do not require `isExecutable()` for `.exe`.
- [x] TestingSection: search `Program Files/Program Files (x86)/Linuxtrack` inside the prefix for testers (and honor custom InstallDir if needed).
- [x] TestingSection: implement `startTracking()` using existing tracking start logic.
- [x] TestingSection: implement `offerWineBridgeInstallation()` and delegate to `PluginInstall`.
- [ ] TestingSection: implement Custom Prefix selection (directory chooser + validation).
- [x] Lutris launch: avoid forcing `WINEARCH=win64` blindly.
- [x] SteamIntegration: remove `~` fallback; centralize installer path lookup.
- [x] LutrisIntegration: use centralized installer path helper; unify with SteamIntegration.
- [ ] Optionally extract a `TesterLauncher` helper to match the plan and reduce responsibility in `TestingSection`.

---

## Notes
- The feature is close to end-to-end for Steam/Lutris when prerequisites exist. The remaining items focus on correctness (file detection), build linkage (Qt SQL), and missing integration points (installer delegation, custom prefix).
