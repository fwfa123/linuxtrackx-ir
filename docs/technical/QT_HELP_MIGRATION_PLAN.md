## Qt Help bundling: migrate old working behavior to v2 AppImage pipeline

### Summary
The old AppImage script bundled prebuilt Qt Help artifacts and a Qt5 runtime (Qt5Help, Qt5Sql, SQLite driver), and Help worked. The new v2 pipeline regenerates `.qhc/.qch` with the host `qhelpgenerator` (often Qt6), which is incompatible with the bundled Qt5 runtime, causing Help failures. We will: (1) stop unconditional regeneration, (2) enforce Qt major match for regeneration, (3) keep bundling of Qt5Help/Qt5Sql/libqsqlite, and (4) ensure `QT_HELP_PATH` points to a writable user dir at runtime.

### Affected files
- `scripts/appimage/v2/prepare.sh`: gate help regeneration; enforce Qt major version match; keep validation.
- `scripts/appimage/v2/bundle.sh`: keep copying `.qhc/.qch`, ensure `libQt5Help.so.*`, `libQt5Sql.so.*`, and `libqsqlite.so` in plugin paths; retain linuxdeploy hook for `QT_HELP_PATH`.
- `scripts/appimage/v2/common.sh`: AppRun already sets `QT_HELP_PATH` to a user-writable dir and exports Qt plugin paths. Keep dual-path logic (`usr/plugins` and `usr/lib/qt5/plugins`).
- `scripts/appimage/v2/validate.sh`: verify `.qhc/.qch` are valid SQLite and contain `ContentsTable` and `FileDataTable`; warn/error on mismatch.

### Implementation details
1) Gate help regeneration
- Add `REGENERATE_HELP` (default `0`). Only regenerate if set or if artifacts are missing.
- Prefer existing repo artifacts: `src/qt_gui/help.qhc`, `src/qt_gui/help.qch`, `src/mickey/help.*`.

2) Enforce Qt major match when regenerating
- Detect `QT_VERSION` via `qmake -query QT_VERSION` and `QHELP_VERSION` via `qhelpgenerator --version`.
- Extract majors. If `QT_MAJOR != QHELP_MAJOR`, abort with guidance to install a matching Qt5 `qhelpgenerator` or skip regeneration.

3) Bundle runtime requirements (already present, keep/verify)
- Ensure presence of `libQt5Help.so.*`, `libQt5Sql.so.*` in `AppDir`.
- Ensure `libqsqlite.so*` exists in both `usr/plugins/sqldrivers` and `usr/lib/qt5/plugins/sqldrivers`.
- Keep linuxdeploy Qt hook patch that copies help files to `~/.local/share/linuxtrack/help/...` and exports `QT_HELP_PATH`.

4) Validation tightening
- For each `.qhc/.qch` in `usr/share/linuxtrack/help/{ltr_gui,mickey}/`, use `sqlite3` to confirm database opens and tables include `ContentsTable` and `FileDataTable`.
- Optionally record generator major version to compare against bundled Qt major and warn on mismatch.

### Rationale
Qt Help databases are SQLite-based and version-sensitive. A Qt6-generated `.qhc/.qch` can be rejected by a Qt5 `QHelpEngine` with errors like “Cannot unregister index tables.” Ensuring generation with a Qt5 toolchain or reusing known-good artifacts avoids this.

### Rollout plan
- Phase 1: Disable help regeneration by default, bundle existing artifacts, verify Help works in AppImage.
- Phase 2: Allow regeneration when a matching Qt5 toolchain is detected; document how to install Qt5 `qhelpgenerator` on the build host.
- Phase 3: Optional containerized Qt5 build for reproducibility.

### Notes for maintainers
- If Help still fails, run AppImage with `QT_DEBUG_PLUGINS=1` and check for `QSQLITE` in drivers. Ensure `QT_PLUGIN_PATH` and `QT_SQL_DRIVER_PATH` include the correct plugin directories.
- Source locations for artifacts: `src/qt_gui/help.qhc`, `src/qt_gui/help.qch`, `src/mickey/help.qhc`, `src/mickey/help.qch`, plus `src/qt_gui/help/` content.


