# Linuxtrack X-IR GUI skins

Custom skins let you restyle `ltr_gui` with Qt Style Sheets (QSS).

## Install a skin

1. Create a folder under `~/.config/linuxtrack/skins/<skin-name>/`.
2. Add `skin.qss` (required) and optionally `skin.ini` (color tokens).
3. Open **System → Appearance**, pick the skin, click **Preview**, then **Apply**.

Bundled skins (for example `default`) are shipped inside the application and can be overridden by a user folder with the same name.

## `skin.ini` tokens

Optional companion file with a `[colors]` section. Values replace `@token` placeholders in `skin.qss` before the stylesheet is applied.

```ini
[colors]
primary_bg = #1e1e2e
primary_fg = #cdd6f4
accent     = #89b4fa
warning    = #f38ba8
muted      = #a6adc8
```

Supported tokens in the default skin:

| Token | Typical use |
|-------|-------------|
| `@primary_bg` | Tooltip / panel backgrounds |
| `@primary_fg` | Primary text on those panels |
| `@accent` | Highlights, default-button border, section titles |
| `@warning` | Warning / prerequisite hint labels |
| `@muted` | Secondary / diagnostic text (available for custom QSS) |

Longer token names are substituted before shorter ones (so `@primary_bg` wins over `@primary` if both exist).

## Safe QSS selectors

Prefer sparse rules on top of Fusion + the system `QPalette`:

- Global widget types: `QToolTip`, `QPushButton:default`, `QTabWidget::pane`
- Object names for one-off widgets: `QLabel#GamingPrereqHintLabel`
- Avoid hardcoding every widget color; leave most chrome to the palette so dark/light OS themes still work

## Tips

- Click **Cancel** to restore the last **Apply**’d skin.
- Keep a backup of working skins before experimenting.
- If a skin fails to load, the UI falls back to an empty stylesheet and the saved preference is unchanged until you successfully **Apply** again.
