# Help documentation plan: 6DOF pan tuning & translation alignment

## Goals

1. Help users migrating from **OpenTrack** find **neck displacement** and **relative translation** equivalents.
2. Document **3-point Cap** tuning (two sliders, no `Head-X`) as clearly as Clip.
3. Separate **pan tuning** (spurious translation while rotating) from **translation/rotation alignment** (deliberate lean while looking off-axis).
4. Fix **stale** references to pan tuning in the old Detailed Axes dialog.
5. Cross-link **Model Setup**, **Tracking Setup**, and **System** help.

## Deliverables

| Item | Action |
|------|--------|
| `src/qt_gui/help/sixdof_tuning.htm` | **New** hub page: glossary, Cap/Clip slider table, alignment guide, OpenTrack mapping, UDP note |
| `src/qt_gui/help/model_setup.htm` | Cap-specific subsection, link to hub, `Head-Y`/`Head-Z` defaults note |
| `src/qt_gui/help/misc.htm` | Expand alignment; link to hub; when games care about translation |
| `src/qt_gui/help/axes_setup.htm` | Pose maps **Lean** vs **Look**; link to hub |
| `src/qt_gui/help/news.htm` | Clarify v2.1 pan tuning was in old dialog; v3+ on Model Setup only |
| `src/qt_gui/help/welcome.htm` | One-line link under setup flow |
| `src/qt_gui/help_view.cpp` | TOC entry **6DOF tuning** after Model Setup |
| `src/qt_gui/ltr_gui.qhp` | Matching TOC section |

## Out of scope (future)

- **Cap finetuning screenshot:** placeholder in `sixdof_tuning.htm`; bundle `src/qt_gui/help/CapFinetuning.png` when a capture is available and swap the placeholder table for a standard help `<img>` (see `model_setup.htm` / `Model.png`).
- Per-game alignment recommendations.
- Translating new strings in `ltr_gui_*.ts`.

## Verification

- Open Help → confirm new page in tree and links resolve.
- Grep built `help/` copy in build dir if packaging help separately.

## Status

Implemented 2026-11-20: all deliverables above applied in tree.