# Linuxtrack X-IR GUI skins

Custom skins let you restyle `ltr_gui` with Qt Style Sheets (QSS).

## Built-in choices

| Skin | Effect |
|------|--------|
| `native` | Platform widget style, no stylesheet (default for new installs) |
| `default` | Sparse Fusion + palette-aware accent/warning tweaks |

## Install a custom skin

1. Create a folder under `~/.config/linuxtrack/skins/<skin-name>/`.
2. Add `skin.qss` (required) and optionally `skin.ini` (color tokens).
3. Open **System → Appearance**, pick the skin, click **Preview**, then **Apply**.

A user folder with the same name as a bundled skin overrides the bundled copy.

## `skin.ini` tokens

Optional companion file with a `[colors]` section. Values replace `@token`
placeholders in `skin.qss` before the stylesheet is applied.

```ini
[colors]
primary_bg = #1e1e2e
primary_fg = #cdd6f4
accent     = #89b4fa
warning    = #f38ba8
muted      = #a6adc8
```

Use `@accent`, `@warning`, etc. in your QSS. Longer token names are substituted
before shorter ones (so `@primary_bg` wins over `@primary` if both exist).

The bundled `default` skin uses Qt `palette(...)` instead of fixed hex colors so
OS light/dark themes keep working. Prefer that approach unless you are shipping
an intentionally fixed look.

## Safe QSS selectors

Prefer sparse rules on top of Fusion + the system `QPalette`:

- Global widget types: `QToolTip`, `QPushButton:default`, `QTabWidget::pane`
- Object names for one-off widgets: `QLabel#GamingPrereqHintLabel`
- Prefer `palette(highlight)`, `palette(base)`, `palette(text)` over hex
- Avoid painting every widget; leave most chrome to the palette

## Tips

- Click **Cancel** to restore the last **Apply**’d skin.
- **Open Skins Folder** creates `~/.config/linuxtrack/skins/` if needed.
- Keep a backup of working skins before experimenting.
- Preference is stored in `~/.config/linuxtrack/ltr_gui.conf` under `appearance/skin`.
