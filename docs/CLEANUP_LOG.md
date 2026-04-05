# Repository cleanup log

Major cleanups that removed or relocated tracked content. For full file-level history, use `git log` and tags noted below.

---

## 2026 — aggressive cleanup (`cleanup/aggressive-2026`)

**Safety:** tag `pre-cleanup-2026-aggressive` points at the tree *before* this work; optional full-tree archive was `git archive` at cleanup start.

**Summary**

| Area | Action |
|------|--------|
| Repo root | Removed mis-committed `configure`/`libtool`/`test.c`/log-style files |
| Root drafts | Moved `GITHUB_*` / `GITLAB_*` / `RELEASE_*` / `CHANGELOG_DRAFT_*`, fork/legacy text into `docs/archive/releases/` and `docs/archive/root-legacy/` |
| `docs/progress/` | Removed (process artifacts; use issues / changelog) |
| `docs/wiki/` | Removed in-repo mirror (PNGs + markdown); kept `docs/wiki/README.md` stub; wiki tooling targets external wiki clones |
| `docs/testing/` | Removed huge logs `KUBUNTU_25_TESTING.md`, `TESTING_INSTRUCTIONS.md` |
| `docs/guides/` | Moved `LEGACY_*.md` → `docs/archive/guides/` |
| `docs/technical/` | Moved stale plans (e.g. help CI, desktop integration, MFC42 progress) → `docs/archive/technical/` |
| Docs navigation | Updated `DOCUMENTATION_INDEX.md`, `guides/README.md`, Qt6-focused notes; `cleanup_repo.sh` comment points at `docs/features/` for policy |

**Not committed here:** local-only plans under `docs/features/` (e.g. review/plan markdowns) left untracked per maintainer preference.

---

## August 31, 2025 — script and doc pruning

**Backup:** `backup-before-cleanup` branch + zip (if still retained locally).

**Approximate impact (at the time):** scripts 53→38; docs 123→79; ~113 files removed; large line deletion count across the tree.

**Removed (by category)**

- **Build / Wine scripts:** legacy `build_32bit_libs`, old wine-bridge helpers, `update_configure_arch`, obsolete `scripts/dev/*` Qt5 helpers, temporary `fix_*` scripts, redundant `hardware_test` / `wine_test`-style scripts, old AppImage helper scripts at repo root.
- **Docs:** entire `docs/wine-legacy/`; completed `docs/features/0001–0012_*` plans; session/progress summaries under `docs/progress/` (later removed entirely in 2026); obsolete technical analyses (TrackIR/MFC140 variants); AI continuation prompt files; duplicate/outdated Wine technical files; old root-level `RELEASE_NOTES_*` drafts; `docs/guides/WINE_BRIDGE_INSTALLATION_GUIDE.md` (superseded); AppImage v2 log dumps under `scripts/appimage/v2/logs/`.
- **Should-not-be-tracked:** generated/qmake-era paths under `src/` (`.pro`, `pathconfig.h`, etc.) and stray checksum/AppImage artifacts as listed in history.

---

## Recovery

```bash
# Example: restore a path from before 2026 aggressive cleanup
git show pre-cleanup-2026-aggressive:<path>

# Example: 2025 backup branch
git checkout backup-before-cleanup -- <path>
```

Use `git log --follow -- <path>` to locate the last commit that contained a file.

---

## What to keep current

Prefer **[`DOCUMENTATION_INDEX.md`](DOCUMENTATION_INDEX.md)** and **[`README.md`](../README.md)** for navigation. This log is **historical**; it is not automatically updated on every deletion—append a short subsection when you run the next large cleanup.
