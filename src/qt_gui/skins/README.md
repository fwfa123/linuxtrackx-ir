# Linuxtrack X-IR GUI skins

Place custom skins in this folder. Each skin is a subdirectory:

```
~/.config/linuxtrack/skins/<skin-name>/
  skin.qss          (required)
  skin.ini          (optional color tokens)
```

Then open **System → Appearance**, choose the skin, click **Preview**, then **Apply**.

This file is created automatically when you use **Open Skins Folder**. Full authoring notes are below.

## Built-in choices

| Skin in the combo | Effect |
|-------------------|--------|
| System (native) | Platform widget style, no stylesheet (default for new installs) |
| Default | Sparse Fusion + palette-aware accent/warning tweaks |

A user folder named `default` overrides the bundled Default skin.

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
