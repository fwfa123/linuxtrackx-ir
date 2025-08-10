# Feature 0002 – Installation Flow: No Auto-Start/Auto-Launch

Date: 2025-08-10
Branch: `feature/testing-section-gaming-tab`

## Goal
Make installation flows non-intrusive. Remove automatic `TRACKER.start()` and any implicit tester launches from Wine Bridge install buttons in the Gaming tab (`gridLayout_3`).

## Rationale
- Installation shouldn’t change tracking state or launch testers without explicit user intent.
- Testing is available via the Testing section (Run Tester) when the user wants it.

## Scope
- Buttons in `gridLayout_3` in `Gaming` tab:
  - `SteamProtonButton`, `LutrisButton`, `CustomPrefixButton`, future “Other Platform”, “Wine Prefix”.
- `PluginInstall` install flows.

## Changes
- Removed auto `TRACKER.start()` and info dialogs from:
  - `PluginInstall::installLinuxtrackWine()` call sites for custom/Steam/Lutris paths.
  - `LinuxtrackGui::on_OtherPlatformButton_pressed()` and `on_WinePrefixButton_pressed()` placeholders.
- Kept existing Testing section behavior (auto-start guarded when entering testing workflow).

## Definition of Done
- Pressing any install button does not start tracking nor launch testers.
- Testing section still auto-starts tracking once per workflow (guarded), as previously designed.

## Follow-ups (Optional)
- Extract `TesterLauncher` helper to further isolate launching logic from UI.

