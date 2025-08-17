## Gaming Tab UI improvements plan

### Context
The current Gaming tab (see attached screenshot) consists of:
- A header button: "Install Linuxtrack-Wine support (TIR Firmware + MFC42)".
- Installer buttons laid out in a grid: "Steam (Proton)", "Custom Prefix", "Lutris", "Other Platform", "Install Xplane plugin...", "Wine Prefix".
- A Testing section containing:
  - Tester selection: "Tester.exe (TrackIR)" and "FT_Tester (FreeTrack)" radio buttons.
  - Platform combo box (defaults to "Steam"), a "Load Games" button, a disabled "Game" combo box and a disabled "Run Tester" button.
  - Two utility buttons: "Launch Mickey (TrackIR)" and "Launch ltr_pipe for Antimicrox".

Goal: Improve information hierarchy, consistency with install flows, discoverability, and reduce unnecessary clicks while preserving the non-autostart interactive installer UX.

### Relevant files and primary functions/classes
- UI layout
  - `src/qt_gui/ltr.ui` (QTabWidget `LtrTab`, tab "Gaming", layout `gamingTabLayout`, grid `gridLayout_3`, testing group `TestingGroupBox`)
- UI controller
  - `src/qt_gui/ltr_gui.cpp` / `src/qt_gui/ltr_gui.h` (slots: `on_SteamProtonButton_pressed`, `on_LutrisButton_pressed`, `on_CustomPrefixButton_pressed`, testing-section signal wiring)
  - `src/qt_gui/testing_section.cpp` / `src/qt_gui/testing_section.h` (testing workflow, game loading, run tester enabling state)
- Install orchestration
  - `src/qt_gui/plugin_install.cpp` / `src/qt_gui/plugin_install.h` (methods: `installTirFirmwareAndMfc42`, `installSteamProtonBridge`, `installLutrisWineBridge`, `installWineBridgeToCustomPrefix`)
  - `src/qt_gui/steam_integration.cpp` / `src/qt_gui/steam_integration.h`
  - `src/qt_gui/lutris_integration.cpp` / `src/qt_gui/lutris_integration.h`
- Shared helpers
  - `src/qt_gui/installer_paths.*`, `src/qt_gui/utils.*`, `src/qt_gui/tracker.*`

### Proposed changes (no code; structure and behavior only)

1) Clarify prerequisites with status indicators
- Replace the single header button "Install Linuxtrack-Wine support (TIR Firmware + MFC42)" with a "Prerequisites" group that shows two lines with status icons/text:
  - TrackIR firmware: [Installed/Not installed] with an action button "Install/Repair" (calls `installTirFirmwareAndMfc42()` entry that chains into MFC42 as needed).
  - MFC42 libraries: [Installed/Not installed] with an action button "Install/Repair".
- Disable all installer buttons when prerequisites are not met. Re-enable dynamically when both are satisfied.
- Implementation hooks:
  - Query via `PluginInstall::isTirFirmwareInstalled()` and `PluginInstall::isMfc42uInstalled()` at tab show and after each install completes.
  - Expose small helpers in `PluginInstall` to re-check and emit a signal (e.g., `prereqStatusChanged(bool ready)`), consumed by `LinuxtrackGui` to enable/disable installer section.

2) Group install targets with consistent design and copy
- Convert the installer grid into a titled group "Install Wine Bridge to..." with three primary buttons initially:
  - Steam (Proton)
  - Lutris
  - Custom Prefix
- Move "Other Platform" and "Wine Prefix" into an "Advanced..." sub-menu/button to reduce visual noise (optional: disabled until Phase 2).
- Keep "Install Xplane plugin..." as a separate small button under a new group "Flight Sim Plugins" for clarity.
- Ensure each button opens the same pre-launch information dialog: "Starting Interactive Installation" and then launches; on success/failure, show completion/critical dialogs (already aligned for Steam/Lutris; Custom Prefix uses existing `installLinuxtrackWine`).

3) Improve Testing section workflow
- Default Platform based on the last used value (persist in `QSettings`).
- Auto-load games when Platform changes; remove the separate "Load Games" button. If loading is slow, show a small busy indicator and keep the button as a fallback/retry.
- Enable the "Game" combo box when data is present; enable "Run Tester" after a valid selection is made.
- Add a filter box (optional Phase 2) to search games when the list is large.
- Display a one-line status area below Testing for quick feedback (e.g., counts, last error snippet).
- Persist last selected game per platform in `QSettings` to speed up repeated testing.

4) Visual/UX polish
- Align button widths and spacing across rows and columns for visual rhythm.
- Add standard icons where available (Steam, Lutris) via `ltr_rc.qrc`.
- Provide tooltips for each control with concise descriptions of what will happen next.
- Ensure keyboard navigation order is logical; set tab order in `ltr.ui`.
- Use consistent capitalization: titles in Title Case, button labels in Sentence case.

5) Accessibility and localization readiness
- Set accessible names/labels in `ltr.ui` for screen readers.
- Move all new user-visible strings through Qt translation (tr()).

### Implementation details by file

- `src/qt_gui/ltr.ui`
  - Replace the top header button with a `QGroupBox` "Prerequisites" containing two rows (firmware, MFC42) with status labels and action buttons.
  - Replace `gridLayout_3` with a vertical layout with a `QGroupBox` "Install Wine Bridge to..." and a horizontal row of buttons: Steam (Proton), Lutris, Custom Prefix. Add an "Advanced..." `QToolButton` with a popup menu for Other Platform/Wine Prefix.
  - In Testing group, remove the standalone "Load Games" button (or hide by default) and add a small `QLabel` for status messages; add optional `QLineEdit` filter above the Game combo.
  - Normalize button minimum widths and add stretch spacers to keep alignment.

- `src/qt_gui/ltr_gui.cpp` / `.h`
  - On tab show (in constructor after `setupUi` or in `on_LtrTab_currentChanged` when Gaming is selected), query prerequisites via `PluginInstall` and update the UI state.
  - Connect the new prerequisite action buttons to `pi->installTirFirmwareAndMfc42()` (or specific sub-actions if split). After completion, re-check and update status.
  - Wire Steam/Lutris/Custom buttons to existing slot calls (no behavior change beyond enable/disable gating).
  - For Testing section: listen to `PlatformComboBox::currentTextChanged` to trigger async `Load Games` call (delegate to `TestingSection`). Enable/disable the Game combo and Run button based on data/selection. Persist and restore last platform and game.

- `src/qt_gui/plugin_install.cpp` / `.h`
  - Expose lightweight getters `bool areGamingPrereqsReady() const;` or reuse existing checks where appropriate.
  - Emit a signal `void prereqStatusChanged(bool ready);` when TIR/MFC42 installation changes status, so the Gaming tab can update immediately.
  - Keep the unified dialog copy for "Starting Interactive Installation" and the final completion/failure dialogs across platforms.

- `src/qt_gui/testing_section.cpp` / `.h`
  - Add `loadGamesForPlatform(const QString &platform)` which routes to Steam/Lutris integration to populate the internal game model.
  - Add optional `setFilter(const QString &text)` to filter the model client-side.
  - Emit `gamesLoaded(int count)` and `loadFailed(const QString &error)` so `ltr_gui` can toggle UI and show status.
  - Store/retrieve last platform and game in `QSettings`.

- `src/qt_gui/steam_integration.*` and `src/qt_gui/lutris_integration.*`
  - Provide a common return shape for game entries used by the Testing section (id, name, prefix path). If creating a shared type is too heavy, expose a simplified `QStringList getDisplayNames()` and a mapping helper.
  - Ensure existing methods return sufficient debug info for the Testing status line.

- `src/qt_gui/ltr_rc.qrc`
  - Add Steam and Lutris icons if available; reference in `ltr.ui`.

### Step-by-step behavior algorithms

- Prerequisite gating
  1. On Gaming tab activation, call `isTirFirmwareInstalled()` and `isMfc42uInstalled()`.
  2. If either is false, show status "Not installed" and disable installer buttons and Testing group controls.
  3. When the user clicks an Install/Repair action, run the existing flows. On completion, re-evaluate and update UI.

- Installer button press (Steam/Lutris/Custom)
  1. Confirm prerequisites are satisfied (defensive check).
  2. Show informational dialog "Starting Interactive Installation".
  3. Launch installer (existing platform-specific code). Await completion where supported; show completion/failure dialog consistently.

- Testing workflow
  1. On platform change, attempt auto-load of games. Show "Loading..." in the status label and disable Game/Run.
  2. Populate Game combo on success; enable it. Auto-select last-used game if present. Enable "Run Tester" when a valid selection exists.
  3. On failure, show a concise error in the status label and keep controls disabled.
  4. Optional: support text filter to reduce visible game list; selection drives the enabled state of Run.

### Phasing (can be done incrementally)
- Phase 1 (UI structure + gating): Implemented.
- Phase 2 (Testing UX): Implemented (auto-load, status label, persistence, optional filter).
- Phase 3 (Polish): Implemented (icons, tooltips, accessibility, tr()).
- Phase 4 (Advanced): Advanced menu implemented; batch install and busy indicators deferred.

### Notes
- Keep the non-autostart design intact: the pre-launch dialog remains informational and the NSIS window is user-driven; completion/failure dialog appears after we can observe process termination.
- Avoid blocking the main UI thread during long operations; if we introduce async loaders, use signals/slots and update the UI safely.


