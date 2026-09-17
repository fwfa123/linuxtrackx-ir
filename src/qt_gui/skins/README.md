# Linuxtrack X-IR GUI skins

Place custom skins in this folder. Each skin is a subdirectory:

```
~/.config/linuxtrack/skins/<skin-name>/
  skin.qss          (required) — Qt Style Sheet
  skin.ini          (optional) — @token color map
```

Then open **System → Appearance**, choose the skin, click **Preview**, then **Apply**.

**Open Skins Folder** creates this directory and refreshes:

- `README.md` (this guide)
- Demo skins: `example/`, `f35/`, `xwing/` (official demos — copy them before editing)

## Quick start

1. Copy a demo folder to a new name, e.g. `cp -a example my-skin`.
2. Edit `my-skin/skin.qss` and optionally `skin.ini` (both include NOTES comments).
3. In the GUI, select **my-skin**, click **Preview**, then **Apply**.
4. Use **Cancel** to restore the last applied skin.

Do not customize `example/`, `f35/`, or `xwing/` in place — **Open Skins Folder** overwrites those demos from the app bundle.

## Built-in choices

| Combo label | Folder id | Effect |
|-------------|-----------|--------|
| System (native) | *(none)* | Platform widget style, no stylesheet (default for new installs) |
| Default | `default` | Sparse Fusion + palette-aware accent/warning tweaks |
| Example (Apache cockpit) | `example` | Dark olive / amber attack-heli HUD demo |
| Example (F-35 glass) | `f35` | Charcoal / cyan modern glass-cockpit demo |
| Example (X-wing cockpit) | `xwing` | Gunmetal / amber scopes / Rebel-red targeting demo |

A user folder with the same id as a bundled skin overrides the bundled copy (except the three demos above, which are refreshed from the bundle when you open this folder).

Non-native QSS skins switch the app to the **Fusion** style for consistent styling; **System (native)** restores the platform style.

## `skin.ini` tokens (optional)

```ini
[colors]
primary_bg = #1e1e2e
primary_fg = #cdd6f4
accent     = #89b4fa
warning    = #f38ba8
muted      = #a6adc8
```

Demo skins use richer token sets (see each folder’s `skin.ini`). Values replace `@token` in `skin.qss` before apply. Longer names win over shorter ones (`@primary_bg` before `@primary`).

- Use `palette(highlight)`, `palette(base)`, `palette(text)` in QSS when you want OS light/dark themes to keep working (**Default** does this).
- Use fixed `@token` hex colors for an intentionally fixed look (Apache / F-35 / X-wing demos).

## Safe QSS selectors

Sparse rules work best:

- Global types: `QToolTip`, `QPushButton`, `QTabWidget::pane`, `QGroupBox`
- Object names: `QLabel#GamingPrereqHintLabel`, `QLabel#AdvancedWarningLabel`, `QLabel#label_system_info_title`
- Prefer not painting every widget unless you are shipping a full fixed theme

## Tips

- Preference key: `~/.config/linuxtrack/ltr_gui.conf` → `appearance/skin`
- Keep a backup before experimenting
- Qt Style Sheets: https://doc.qt.io/qt-6/stylesheet-reference.html
- In-app help: System tab help (**Appearance** section)
