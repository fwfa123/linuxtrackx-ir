### LinuxTrack AppImage Builder Rework (New Approach Plan)

**Context**
- The current AppImage builder `scripts/appimage/build_appimage_phase4.sh` produces a working AppImage but has “a lot of issues.” We will take a new approach while retaining what works. This plan reviews what the current script does and does not cover for the user, then proposes a structured redesign.

### Scope and Goals
- Replace the monolithic Phase 4 script with a modular, configuration-driven builder that:
  - Minimizes manual library copying; relies on linuxdeploy/linuxdeploy-qt where possible
  - Ensures deterministic, verifiable builds (reproducibility checks, rpath validation)
  - Separates Wine bridge packaging from core AppImage assembly
  - Provides clear user-facing options (flags), logs, and failure modes
  - Keeps Wayland/X11 behavior sane while defaulting to XCB for compatibility (overrideable)

### Current Script Review (What it DOES)
- File: `scripts/appimage/build_appimage_phase4.sh`
  - Verifies AppImage tools (`appimagetool`, `linuxdeploy`, `linuxdeploy-plugin-qt`)
  - Creates `AppDir_phase4` structure and an extensive `AppRun` with heavy Qt isolation and X11 forcing
  - Generates desktop file and installs icons; copies udev rules (`src/99-TIR.rules`, `src/99-Mickey.rules`)
  - Copies help files from `src/qt_gui` and `src/mickey` into `usr/share/linuxtrack/help/*`
  - Builds the project (autoreconf, configure, make) and installs into AppDir
  - Builds Wine bridge with distro-specific wine library search (Arch, Debian/Ubuntu, Fedora, openSUSE) and installs project outputs into AppDir
  - Copies Wine bridge installer `linuxtrack-wine.exe` into `AppDir/wine_bridge` and `usr/share/linuxtrack`
  - Creates an AppImage-internal installer script `wine_bridge/scripts/install_wine_bridge.sh`
  - Bundles dependencies via:
    - `linuxdeploy` and `linuxdeploy-qt`
    - Manual copying of many libs (Qt5Core/Gui/Widgets/...; OpenCV core/imgproc/objdetect; GLib/GObject; ICU; compression; SSL; text rendering; SELinux; OpenGL; X11; XCB family)
    - Explicit plugin directories: platforms, imageformats, iconengines, styles; ensures SQLite driver `libqsqlite.so`
    - Optional 32-bit `liblinuxtrack.so.0` into `usr/lib32/linuxtrack` for 32-bit Wine testers
    - `patchelf` rpath adjustments for binaries and libraries
  - Provides optimization utilities (strip, prune plugins, doc/man pruning); aggressive lib dedup disabled in main
  - Creates AppImage with `appimagetool` and performs a verification pass (extract and check for Qt/OpenCV presence, wine_bridge tree, counts, quick run `--help`)

### Current Script Gaps (What it DOES NOT cover well)
- Hard-coded paths and versions (e.g., ICU `.so.72/.71/.70/.69`, OpenCV `.406`) risk portability and break on other distros
- Duplicated and conflicting Qt env exports; always-on debug flags for help/Qt plugins in production runs
- Manual library copying duplicates linuxdeploy responsibilities; risks bundling blacklisted or host-incompatible libs
- Wayland disabled globally; lacks graceful feature detection and selective fallback (xcb-only may be over-constraining)
- Limited architecture/multi-arch strategy; 32-bit runtime is best-effort and sourced from host paths
- No recipe/config; parameters (version, app id, paths, flags) are inlined; limited override mechanism
- No CI-friendly entry points or clear idempotence; cleanup/build/install steps are interwoven
- No metainfo/appstream, update information, or signing; no optional zsync
- Optimization step is either too aggressive (disabled) or manual; no measured, repeatable size policy
- Validation is minimal: no full `ldd` leakage audit vs an allowlist, no plugin enumeration checks beyond presence, no reproducibility checks

### Proposed New Approach
1) Introduce a small framework in `scripts/appimage/v2/`:
   - `config.sh`: Centralize `APP_NAME`, `APP_ID`, `VERSION`, paths, toggles (e.g., `FORCE_XCB`, `WITH_WINE_BRIDGE`, `WITH_HELP`)
   - `prepare.sh`: Clean/build/install project into `AppDir/` (idempotent, no bundling)
   - `bundle.sh`: Run `linuxdeploy` and `linuxdeploy-qt` with explicit entries and plugins; minimal manual copies guarded by detection (SQLite driver)
   - `wine_bridge.sh`: Build and stage Wine bridge strictly from project build outputs; do not pull host libs ad hoc
   - `optimize.sh`: Size policy (strip, docs/man pruning) with conservative rules and metrics
   - `validate.sh`: Perform `ldd` audits, plugin checks, Qt/sqlite presence, no external leakage beyond an allowlist
   - `package.sh`: Produce AppImage (+ optional zsync), embed/update metainfo
   - `common.sh`: Logging, error handling, traps, helpers (require_cmd, copy_if_exists, rpath_set)

2) Minimize manual library copying
   - Prefer `linuxdeploy` autodetection; pass `-e usr/bin/ltr_gui -l usr/lib/linuxtrack/*.so` as needed
   - Use `linuxdeploy-plugin-qt` to gather Qt libs/plugins; only manually ensure `libqsqlite.so` when missing
   - Maintain an allowlist/denylist to avoid bundling glibc, kernel, GPU drivers, and other blacklisted libs

3) AppRun simplification
   - Keep a minimal `AppRun` that sets the smallest viable Qt env
   - Default to `xcb`; allow `FORCE_QPA=minimal` or `FORCE_XCB=1` via env/flag
   - Remove production-unfriendly debug envs; reserve them for a `--debug` run mode

4) Wine bridge handling
   - Build wine bridge as part of project install; stage only built artifacts from `DESTDIR`
   - Provide `usr/share/linuxtrack/linuxtrack-wine.exe` and an installer wrapper `usr/bin/ltr_wine_install` that forwards to `wine_bridge/scripts/install_wine_bridge.sh`
   - Optional 32-bit support: rely on project-produced 32-bit artifacts; if unavailable, skip bundling and surface a clear message

5) Deterministic builds and validation
   - Set `SOURCE_DATE_EPOCH` (use latest git commit timestamp when available)
   - Rpath: set only once, uniformly, and validate all binaries/libs with `chrpath/patchelf` checks
   - Full leakage audit: `ldd` all ELF files and fail if any dependency resolves outside `AppDir` except an explicit allowlist (e.g., libGL drivers)
   - Plugin audit: enumerate `usr/lib/qt5/plugins` expected subsets (platforms/imageformats/sqldrivers) and assert `libqsqlite.so` present

6) Packaging polish
   - Generate `usr/share/metainfo/${APP_ID}.metainfo.xml`
   - Optional update info and zsync artifact
   - Embed icons at multiple sizes and validate desktop file keys

7) CI entrypoint
   - `scripts/appimage/v2/ci_build.sh` runs the full pipeline non-interactively with caches disabled

### Algorithms / Step-by-step
- Build/Install (prepare.sh)
  1. `require_cmd autoreconf qmake make` etc.
  2. `make distclean || true`; `autoreconf -fiv`; `./configure --prefix=/usr`
  3. `make -j$(nproc)`; `make install DESTDIR="$APPDIR"`
  4. Verify critical binaries exist: `usr/bin/ltr_gui`, libraries under `usr/lib*/linuxtrack`

- Bundling (bundle.sh)
  1. Run `linuxdeploy --appdir "$APPDIR" -e usr/bin/ltr_gui --icon-file ... --desktop-file ...`
  2. Run `linuxdeploy-plugin-qt` on `usr/bin/ltr_gui`
  3. If `usr/lib/qt5/plugins/sqldrivers/libqsqlite.so` missing, search in common locations; copy first match
  4. Apply uniform rpath: binaries `$ORIGIN/../lib:$ORIGIN/../lib/linuxtrack`; libraries `$ORIGIN`
  5. Do not manually copy host-versioned libs unless explicitly required and version-independent

- Wine bridge (wine_bridge.sh)
  1. If `WITH_WINE_BRIDGE=1`, copy `linuxtrack-wine.exe` from `$APPDIR/usr/share/linuxtrack/wine` (installed by project) to `AppDir/wine_bridge`
  2. Ensure `wine_bridge/scripts/install_wine_bridge.sh` exists and is executable
  3. Surface clear messages if wine not present at runtime (handled by installer wrapper)

- Optimize (optimize.sh)
  1. Strip binaries/libs (`--strip-unneeded`) where safe
  2. Prune docs/man/help assets per allowlist; keep essential help and man pages only
  3. Limit Qt plugins to essential subsets (keep xcb, minimal, essential imageformats, sqldrivers)

- Validate (validate.sh)
  1. `ldd` audit: fail if any ELF resolves to outside `AppDir` except allowlist
  2. Ensure `libQt5Sql.so.5` and `libqsqlite.so` present; ensure `platforms/libqxcb.so` present
  3. Enumerate Qt plugins and log counts; verify desktop file and icon presence
  4. Optionally run `--appimage-extract-and-run --help` smoke test

### Files to Add/Change
- Add (new):
  - `scripts/appimage/v2/config.sh`
  - `scripts/appimage/v2/common.sh`
  - `scripts/appimage/v2/prepare.sh`
  - `scripts/appimage/v2/bundle.sh`
  - `scripts/appimage/v2/wine_bridge.sh`
  - `scripts/appimage/v2/optimize.sh`
  - `scripts/appimage/v2/validate.sh`
  - `scripts/appimage/v2/package.sh`
  - `scripts/appimage/v2/ci_build.sh`
- Change (existing):
  - `scripts/appimage/build_appimage_phase4.sh`: keep for reference; deprecate with notice, or refactor to thin wrapper around v2 entrypoint
  - Optionally adjust project install targets to ensure `linuxtrack-wine.exe` is consistently installed under `usr/share/linuxtrack/wine` during `make install`

### Risks and Mitigations
- Plugin or lib regressions due to reduced manual copying
  - Mitigate with strict validation and a targeted allowlist
- Wayland/X11 differences across desktops
  - Provide user-toggle and runtime detection; default to xcb for safety, not hard-disable Wayland
- 32-bit support variability
  - Clearly mark as optional; avoid pulling host libs; document requirements

### Rollout Phases
- Phase A: Introduce v2 scripts alongside existing script; parity build and side-by-side verification
- Phase B: Enable v2 in CI, produce release artifacts; keep old script as fallback
- Phase C: Remove deprecated script once v2 is stable across primary distros

### User-Facing Coverage Summary
- What current script covers: building and installing the project; generating `AppRun`, desktop/icon assets, udev rules; bundling Qt and many system libs; ensuring SQLite plugin; packaging and basic verification; Wine bridge installer shipping and a helper script
- What it does not cover for the user: portable, recipe-based configuration; robust Wayland handling; deterministic builds; a clean validation story (ldd allowlist, plugin audits); CI-ready modular workflow; metainfo/update info; predictable 32-bit strategy; minimal, production-safe environment configuration


