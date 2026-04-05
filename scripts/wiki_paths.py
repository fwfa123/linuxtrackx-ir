"""
Shared defaults for help→wiki conversion scripts.

Converted Markdown and images are written to a local wiki repository checkout,
not under docs/wiki/ in the main tree. Override with LINUXTRACK_WIKI_ROOT or
--wiki-root.
"""

from __future__ import annotations

import os
from pathlib import Path


def repo_root() -> Path:
    """linuxtrackx-ir repository root (parent of scripts/)."""
    return Path(__file__).resolve().parent.parent


def default_wiki_root() -> Path:
    """Sibling clone: ../linuxtrackx-ir.wiki next to the main repo."""
    env = os.environ.get("LINUXTRACK_WIKI_ROOT")
    if env:
        return Path(env).expanduser().resolve()
    return repo_root().parent / "linuxtrackx-ir.wiki"


def default_help_dir() -> Path:
    return repo_root() / "src" / "qt_gui" / "help"
