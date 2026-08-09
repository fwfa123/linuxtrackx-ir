# Linuxtrack X-IR GUI skins

Place custom skins in this folder. Each skin is a subdirectory:

```
~/.config/linuxtrack/skins/<skin-name>/
  skin.qss          (required)
  skin.ini          (optional color tokens)
```

Then open **System → Appearance**, choose the skin, click **Preview**, then **Apply**.

This file (and the `example/` starter skin) are created when you use **Open Skins Folder**.

## Quick start

1. Copy `example/` (Apache), `f35/` (glass), or `xwing/` (Rebel fighter) to a new name, e.g. `my-skin/`.
2. Edit `my-skin/skin.qss` (and optionally `skin.ini`). Files include NOTES comments.
3. In the GUI, select **my-skin**, click **Preview**, then **Apply**.

## Built-in choices

| Skin in the combo | Effect |
|-------------------|--------|
| System (native) | Platform widget style, no stylesheet (default for new installs) |
| Default | Sparse Fusion + palette-aware accent/warning tweaks |
| Example (Apache) | Demo attack-heli HUD (dark olive / amber) — copy `example/` |
| Example (F-35) | Demo glass cockpit (charcoal / cyan) — copy `f35/` |
| Example (X-wing) | Demo starfighter panel (gunmetal / Rebel red / amber) — copy `xwing/` |

A user folder named `default`, `example`, `f35`, or `xwing` overrides the matching bundled skin.

## `skin.ini` tokens (optional)

```ini
[colors]
primary_bg = #1e1e2e
primary_fg = #cdd6f4
accent     = #89b4fa
warning    = #f38ba8
muted      = #a6adc8
```

Values replace `@token` placeholders in `skin.qss` before apply. Longer names win
over shorter ones (`@primary_bg` before `@primary`).

Prefer Qt `palette(...)` in QSS when you want OS light/dark themes to keep working.
Use fixed hex tokens only for an intentionally fixed look.

## Safe QSS selectors

Sparse rules on top of Fusion + the system palette work best:

- `QToolTip`, `QPushButton:default`, `QTabWidget::pane`
- Object names such as `QLabel#GamingPrereqHintLabel`
- `palette(highlight)`, `palette(base)`, `palette(text)` instead of hard-coded colors where possible

## Tips

- **Cancel** restores the last **Apply**’d skin.
- Preference is stored in `~/.config/linuxtrack/ltr_gui.conf` under `appearance/skin`.
- Keep a backup before experimenting.
- Qt Style Sheets reference: https://doc.qt.io/qt-6/stylesheet-reference.html
