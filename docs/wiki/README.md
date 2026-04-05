# Wiki content lives outside this repository

The full wiki (Markdown and images) is maintained in separate wiki checkouts, not under `docs/wiki/` in the main tree.

## Hosted wikis

- **GitLab** (project wiki): [linuxtrackx-ir wikis](https://gitlab.com/fwfa123/linuxtrackx-ir/-/wikis/home)
- **GitHub** (companion wiki): [linuxtrackx-ir wiki](https://github.com/fwfa123/linuxtrackx-ir/wiki)

## Local clones (for editing)

Typical layout next to this repo:

```bash
# GitHub-style wiki repo (used by scripts/convert_help_to_wiki.py by default)
git clone https://github.com/fwfa123/linuxtrackx-ir.wiki.git
```

If you also use a GitLab wiki checkout, clone it separately (for example a `linuxtrackx-ir-gitlab.wiki` remote you maintain) and edit there.

## Help → wiki export

HTML help under `src/qt_gui/help/` can be converted into Markdown in your **wiki working copy** (not into this directory). From the main repo:

```bash
export LINUXTRACK_WIKI_ROOT=/path/to/linuxtrackx-ir.wiki   # optional; default: ../linuxtrackx-ir.wiki
python3 scripts/convert_help_to_wiki.py --help
```

See `scripts/wiki_paths.py` for path defaults.
