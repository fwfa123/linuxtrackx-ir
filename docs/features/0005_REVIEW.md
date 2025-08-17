## 0005 Review: Automatic Wine Prefix Architecture Detection for Tester Execution

### Summary
- **Overall**: Core flow implemented and integrated. Automatic detection is wired into `TestingSection::runSelectedTester()`, tester selection honors detected architecture, and `WINEARCH` is set per platform execution path.
- **Blocking issues**: Duplicate `WineArchitecture` enum declarations and a header/implementation signature mismatch will prevent compilation. A few logic and robustness gaps should be addressed.

### Plan vs Implementation
- **New functions**
  - `TestingSection::detectWinePrefixArchitecture()` implemented as a thin wrapper calling centralized logic.
  - `TestingSection::selectAppropriateTester()` implemented with architecture-first search and fallback.
  - `TestingSection::validateTesterCompatibility()` implemented (basic filename-based check).
- **Modified functions**
  - `TestingSection::runSelectedTester()` updated to detect architecture, choose tester, validate, and execute.
  - `TestingSection::executeTester()` updated to set `WINEARCH` for Steam/Lutris/Custom paths.
  - `TestingSection::checkTesterInPrefix()` delegates to centralized lookup.
- **Centralization**
  - `TesterLauncher` adds: prefix resolution, architecture detection, tester search, and compatibility helper.

### Critical/Bug-level Findings
- **Conflicting enum declarations (compile-time error)**
  - `WineArchitecture` is declared in both `src/qt_gui/testing_section.h` and `src/qt_gui/tester_launcher.h` (global scope). Including both in `testing_section.cpp` will cause a redefinition error.
  - Fix: Keep the enum in one header (prefer `tester_launcher.h`) and include it. Remove the duplicate from `testing_section.h` and update includes accordingly.

- **Header/implementation mismatch (compile-time error)**
  - `TestingSection::executeTester` signature in header: `void executeTester(const QString &testerPath, const QString &prefixPath, const QString &platform);`
  - Implementation: `void executeTester(const QString &testerPath, const QString &prefixPath, const QString &platform, WineArchitecture arch)` and call sites pass `arch`.
  - Fix: Update the declaration in `testing_section.h` to include the `WineArchitecture` parameter.

### Functional/Logic Issues
- **`.exe` executability check is too strict (can cause false negatives)**
  - `TesterLauncher::findAppropriateTester()` (and `TestingSection::selectAppropriateTester()` fallback) require `QFileInfo::isExecutable()` for `.exe` files. On Linux these often lack the execute bit but still run via Wine.
  - Fix: Check only `exists()` and `isFile()` for `.exe` discovery, consistent with `TesterLauncher::findTesterInPrefix()`.

- **Platform label coupling in prefix resolution**
  - `TesterLauncher::getPrefixForGame()` matches exact strings `"Steam"`/`"Lutris"`, while `TestingSection` generally works with platform keys via `platformLabelToKey()`.
  - Risk: Localization or label changes will break resolution. Fix by passing a stable key or enum to the helper, or normalize the passed label first.

- **Steam compat data path derivation is brittle**
  - Chops `"/pfx"` by string length to derive `STEAM_COMPAT_DATA_PATH`. If the suffix varies, this breaks.
  - Fix: Use `QDir compat(prefixPath); compat.cdUp();` to move up one directory reliably.

- **Architecture detection heuristics**
  - DLL count threshold (`syswow64Files > system32Files * 0.8`) is unnecessary given the simpler `syswow64` presence check that follows; it adds I/O overhead.
  - Consider reordering: first check `syswow64` existence, then fall back to other methods.

- **Validation is minimal**
  - `validateTesterCompatibility()` only inspects filename vs architecture. It doesn’t attempt a dry-run or metadata check. Acceptable as a first pass, but note limitation.

### Subtle Interface/Data Alignment Checks
- **Enum sharing**: Once deduplicated, ensure both `TestingSection` and `TesterLauncher` include the same enum header and use the same type in public APIs.
- **String vs key usage**: Normalize the platform parameter to a key before calling helpers to avoid locale-sensitive behavior.

### Style/Consistency
- Centralization in `TesterLauncher` is good and matches the plan.
- Logging via `qDebug()` is consistent. Consider gating or promoting critical messages to user-visible UI when appropriate.
- Parameter naming and QString usage are consistent with the codebase.

### Over-engineering / Size
- Files remain large but changes are localized. No immediate refactor required; consider extracting Steam/Lutris execution blocks into helpers later for readability.

### Recommended Edits
1) Deduplicate `WineArchitecture`:
   - Remove the enum from `src/qt_gui/testing_section.h` and include `tester_launcher.h` where needed.
2) Fix `executeTester` signature:
   - Update declaration in `src/qt_gui/testing_section.h` to: `void executeTester(const QString &testerPath, const QString &prefixPath, const QString &platform, WineArchitecture arch);`
3) Relax `.exe` checks:
   - In `src/qt_gui/tester_launcher.cpp::findAppropriateTester()` and `src/qt_gui/testing_section.cpp::selectAppropriateTester()` fallback, drop `isExecutable()` for `.exe` files.
4) Normalize platform label usage:
   - Pass a stable platform key or enum to `TesterLauncher::getPrefixForGame()` and update comparisons accordingly.
5) Make Proton compat path robust:
   - Replace `compatDataPath.chop(4);` with a `QDir`-based parent directory resolution.
6) Optionally simplify detection:
   - Prefer `syswow64` presence check first; keep registry and `wine --version` as fallbacks.

### Test Coverage Suggestions
- Prefix detection:
  - 32-bit prefix with only `system32`.
  - 64-bit prefix with `syswow64` present.
  - Corrupted/missing `system.reg`.
  - `wine --version` timeout path.
- Tester discovery:
  - Test presence of `.exe` without execute bit.
  - Both testers present; verify preference order per arch.
- Platform execution:
  - Steam Proton: verify `STEAM_COMPAT_DATA_PATH` derived via parent directory logic.
  - Lutris/system Wine: verify `WINEARCH` is set to `win32`/`win64` per detection, and omitted for `UNKNOWN`.


