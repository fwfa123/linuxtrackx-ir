## 0003 – Gaming Tab UI Improvements: Follow-up Tasks

### Scope
Finalize localization, polish Testing UX, and add QA coverage for the new Gaming tab workflows.

### Tasks
- Localization sweep (high priority)
  - Convert remaining user-facing strings to `tr()` in the Qt GUI codebase (beyond the already updated files).
  - Verify `ltr.ui` additions (accessible names, tooltips, labels) are routed through translations.
  - Generate/update `.ts` files and recompile `.qm` resources.

- Testing section UX refinements (medium)
  - During async loads, disable `PlatformComboBox`, `GameComboBox`, and `RunTesterButton`, re-enable on completion.
  - Ensure cancellation/re-entry is handled cleanly if the platform changes mid-load.

- Advanced menu and legacy controls cleanup (low)
  - Confirm there are no remaining references to removed `OtherPlatformButton` and `WinePrefixButton` in code or docs.

- Resource verification (low)
  - Validate Steam/Lutris/Wine icons render correctly on different distros/themes.

- QA matrix (high)
  - Prereq gating: Verify enable/disable across all combinations of TIR firmware/MFC42 presence.
  - Install flows: Steam/Lutris/Custom Prefix happy paths and error dialogs.
  - Testing flows: Auto-load triggers, filter behavior, persistence (last platform/game), and tester launch per platform.

### Deliverables
- All user-visible strings translated via `tr()` and present in `.ts` catalogs.
- Smooth Testing UX with appropriate disabling/enabling during async operations.
- Updated docs/screenshots if UI text changed.

### Notes
- Keep operations non-blocking and on the main thread for UI work; use signals/slots for async completion.
- Maintain the non-autostart installer UX.


