## Review: Feature 0001 - Alternative MFC42 sources and symlink fix

### Summary
The implementation largely follows the plan in `docs/features/0001_PLAN.md`:
- Added missing `mfc42.dll` symlink creation in `src/wine_bridge/client/check_data.c` alongside `mfc42u.dll` with appropriate messaging.
- Extended `src/qt_gui/sources_mfc42.txt` to include VS6SP6 and VC6RedistSetup with method metadata.
- Implemented extractor changes in `src/qt_gui/extractor.{h,cpp}`: new methods `extractFromVS6SP6`, `extractFromVC6RedistSetup`, metadata parsing in `downloadVCRedist`, and UI updates. Direct DLL download is stubbed with warnings (acceptable for now).

### Correctness Checks
- `check_data.c` now creates symlinks for both `mfc42u.dll` and `mfc42.dll` from `~/.config/linuxtrack/tir_firmware/` to the wine app dir, with error messages mentioning both. This meets Phase 1.
- `sources_mfc42.txt` contains entries:
  - VS6SP6 with SHA256 and method `extract_vcredist`
  - VC6RedistSetup_deu with SHA256 and method `wine_installer`
  - Legacy sources retained. This meets Phase 2.
- Extractor (`Mfc42uWinetricksExtractor`):
  - Parses `sources_mfc42.txt` lines split by `|` and selects method accordingly.
  - Implements VS6SP6 two-step `cabextract` flow and copies both `mfc42.dll` and `mfc42u.dll` to `~/.config/linuxtrack/tir_firmware/`.
  - Implements VC6RedistSetup via Wine `/T` extraction and falls back to existing wine flow.
  - Maintains winetricks-first strategy with fallbacks. This aligns with Phase 3 and Phase 4 messaging.

### Issues and Risks
1) UI/Copy destination consistency
   - Multiple code paths copy only `mfc42u.dll` and not `mfc42.dll` (e.g., wine success paths around lines ~689, ~1519, ~1027), while VS6SP6 copies both. Plan requires copying both in all success paths.

2) VC6RedistSetup extraction completeness
   - `extractFromVC6RedistSetup()` starts Wine with `/T` but defers to `wineFinished()`, which currently copies only `mfc42u.dll`. It should ensure both DLLs are copied when available.

3) Direct DLL download
   - `downloadDirectDLL()` is intentionally unimplemented; acceptable with warnings, but the sources file lists commented direct links. Consider hiding this method unless explicitly enabled.

4) Installer browse label
   - `on_BrowseInstaller_pressed()` mentions “Visual C++ 2015-2022 Redistributable” and checks for `vc_redist` filename, which is inconsistent with VC6 (VS6SP6/VC6RedistSetup). Update prompt and validation to VC6.

5) Path handling and overwrite behavior
   - `QFile::copy` will fail if a destination exists; consider removing existing files before copy or using `QFile::remove(dstPath)` to ensure idempotency.

6) Cleanup and temp dirs
   - VS6SP6 path cleans up temp dir; other flows that create temp prefixes/dirs should ensure cleanup when done (where safe).

7) Messaging consistency
   - Ensure progress dialogs explicitly state which method is active and fallbacks taken. Most messages exist; minor polishing suggested.

### Recommendations
- Update all success copy paths to place both `mfc42.dll` and `mfc42u.dll` into `~/.config/linuxtrack/tir_firmware/` when found (search/copy lists should include both names consistently).
- Extend `wineFinished()` and `cabextractFinished()` post-extraction copy logic to attempt both DLLs.
- Fix `on_BrowseInstaller_pressed()` label and validation to point to VC6 redistributables (accept VS6SP6 and VC6RedistSetup filenames) rather than `vc_redist`.
- Before copying, remove existing destination files to avoid silent failures: if `QFile::exists(dst)`, call `QFile::remove(dst)`.
- Optionally guard the “direct DLL” method behind a compile-time flag or advanced toggle until fully implemented with checksum verification.

### Conclusion
Implementation is close to the plan and functionally adds critical fallbacks and the `mfc42.dll` symlink. Address the noted inconsistencies so both DLLs are consistently copied across all extraction paths and adjust the browse flow to VC6 installers.
