# 0014 — Curve Editor Enhancements & Translation Alignment UX

## Context

The sensitivity curves setup dialog (`ScpForm`) and its per-axis curve widget (`SCurve`) already implement the full parameter math for non-linear axis mapping (deadzone, left/right curvature, multiplier/sensitivity, limits, filter). However, several usability gaps exist:

- The translation/rotation alignment toggle is buried in `ltr.ui` with an inverted label ("Disable translation/rotation alignment")
- The curve plot (`SCView`) has no zoom or pan support — users cannot inspect fine regions of the curve
- No way to directly edit control points on the graph, enter X/Y coordinates, or save/load curve templates

This plan covers (a) and (b) as immediate work, plus a forward-looking section for the remaining items.

---

## Phase 1 — Relabel Translation/Rotation Alignment Toggle

### Goal
Make the existing orientation-aligned translation feature discoverable by relabeling it with positive framing.

### Files to change

| File | Change |
|------|--------|
| `src/qt_gui/ltr.ui` (line ~792) | Rename widget `TransRotDisable` → `AlignTranslationWithOrientation`. Update `<string>` text from `"Disable translation/rotation alignment "` to `"Align translations with virtual orientation"`. |
| `src/qt_gui/ltr_gui.cpp` (lines 248, 553–560) | Rename all references: `ui.TransRotDisable` → `ui.AlignTranslationWithOrientation`, slot name `on_TransRotDisable_stateChanged` → `on_AlignTranslationWithOrientation_stateChanged`. Keep the inverted logic (`!enabled`) since the checkbox now reads positively. |
| `src/qt_gui/ltr_gui.h` (line 62) | Update slot declaration: `void on_AlignTranslationWithOrientation_stateChanged(int state);` |

### Notes
- The underlying C function `ltr_int_do_tr_align()` / `ltr_int_set_tr_align()` in `pref_global.c` and `ltlib_int.c` is unchanged — only the UI label and variable names shift.
- This feature already exists in the core: when enabled, `ltr_int_align_translation(pitch, yaw, roll, displacement, rotated)` (in `tracking.c:388`) rotates raw translation vectors by the current virtual orientation before applying per-axis curves.

---

## Phase 2 — Scroll-Wheel Zoom on SCView Plot

### Goal
Allow users to zoom into any region of the sensitivity curve plot using the mouse scroll wheel, with reset via middle-click or double-click.

### Files to change

| File | Change |
|------|--------|
| `src/qt_gui/scview.h` | Add: `#include <QWheelEvent>`, `#include <QMouseEvent>`; private members `float m_zoomXMin, m_zoomXMax, m_zoomYMin, m_zoomYMax; bool m_manualZoom; void wheelEvent(QWheelEvent*) override; void mousePressEvent(QMouseEvent*) override;` |
| `src/qt_gui/scview.cpp` | 1. Constructor: initialize zoom bounds to auto range (`m_manualZoom = false`). 2. Implement `wheelEvent()`: on scroll, adjust `m_zoomXMin/m_zoomXMax` (horizontal) or `m_zoomYMin/m_zoomYMax` (vertical) by a factor of ~1.15 centered on cursor position in plot coordinates. Set `m_manualZoom = true`. 3. Implement `mousePressEvent()`: middle-click resets zoom to auto; double-click also resets. 4. In `paintEvent()` and `drawGrid()`, use `m_zoomXMin/m_zoomXMax` for the X-axis tick range instead of computing from limits when `m_manualZoom` is true. |

### Algorithm — wheel event zoom

1. Convert mouse position to plot coordinates:
   - `float xNorm = (event->position().x() - plot.centerX) / plot.halfW;`
   - `float yNorm = (plot.bottom - event->position().y()) / plot.height * max_f;`
2. Determine scroll direction from `event->angleDelta()`: positive = zoom in, negative = zoom out.
3. Compute zoom factor: `float f = 1.0f + (delta / 120.0f) * 0.15f;`
4. Adjust bounds centered on cursor position:
   - X-axis: `m_zoomXMin = xCenter - (xCenter - m_zoomXMin) * f;` then similarly for max.
   - Y-axis: same logic applied to `yNorm`.
5. Clamp minimum range to prevent inversion (e.g., min span of 0.1 units).
6. Call `update()` to repaint.

### Algorithm — reset zoom

- Middle-click (`Qt::MiddleButton`) or double-click → set `m_manualZoom = false`, invalidate cached bounds, call `redraw()`.

### Notes
- The existing `niceStep()` function in `scview.cpp` already computes tick spacing; it will automatically adapt to the new zoomed range.
- No changes needed to `spline()` or `tracker.h` — zoom is purely a display concern.

---

## Phase 3 — Forward Plan (Not Yet Implemented)

### 3A — Direct Control Point Editing on Graph

**Gap:** SCurve currently exposes only sliders/spinboxes for parameters (`SCFactor`, `SCLeftCurv`, `SCRightCurv`, `SCDeadZone`). Users cannot click/drag points directly on the curve in `SCView`.

**Approach:**
- Extend `SCView` with hit-testing: detect when the cursor is near a control point (deadzone start, curvature peak at ~1.0 output) and enter drag mode.
- On drag, convert pixel position back to spline parameter space using the inverse of `valueToPixelX()` / `outputToPixelY()`.
- Map the new X/Y coordinate to the underlying spline parameters (`splines_def` in `spline.h`) via the Bernstein polynomial relationship:
  - Left control point X: `x1 = deadzone + (1 - deadzone) * (1 - l_curvature)` → solve for `l_curvature`.
  - Right control point X: same with `r_curvature`.
- Emit `TRACKER.axisChange(axis, AXIS_LCURV/AXIS_RCURV, new_val)` to update the curve in real time.
- Requires adding `mouseMoveEvent()` and `hoverEnterEvent()` to `SCView`, plus a selected-point state machine.

**Files affected:** `scview.h/cpp`, `spline.c` (for inverse mapping), `tracker.cpp` (via existing `axisChange`).

### 3B — X/Y Coordinate Entry Boxes for Control Points

**Gap:** No numeric entry fields for individual control point coordinates on the graph.

**Approach:**
- Add two `QDoubleSpinBox` widgets per side (left/right) in `scurve.ui`: "X Position" and "Y Multiplier".
- Wire value changes to compute the corresponding spline parameter (`l_curvature`/`r_curvature`) from the X/Y coordinate, then call `TRACKER.axisChange()`.
- When symmetry is enabled, both sides share values; when disabled, they are independent.
- Keep existing curvature sliders as the primary control; spinboxes become precision alternatives.

**Files affected:** `scurve.ui` (add 4 spinboxes), `scurve.cpp` (new slot handlers + sync with sliders).

### 3C — Curve Template System

**Gap:** No mechanism to save a curve configuration as a reusable template or apply stock templates to an axis. The user referenced these stock curves: "One:One", "Deadzone", "Smooth", "Fast", "Slow".

**Approach:**
- Define a `CurveTemplate` struct (in a new header, e.g., `curve_template.h`) containing all 6 per-axis parameters: `{ deadzone, l_curvature, r_curvature, multiplier, l_limit, r_limit }`.
- Add a `CurveTemplateStore` singleton that loads/saves templates from the config file under a `[Curves]` section using `PREF` (from `ltr_gui_prefs.h`). Each template stored as: `TemplateName-deadzone=...`, etc.
- In `scp_form.ui`, add a "Templates" dropdown + "+" / "-" buttons above or beside the AxisList. The "+" button saves the current axis parameters as a new named template; "-" removes it.
- Apply a template to the active axis by reading its stored values and calling `TRACKER.axisChange()` for each parameter, then syncing the SCurve UI via `axisChanged()`.

**Stock curve presets (suggested names avoiding close mimicry):**

| Suggested Name | deadzone | l_curv | r_curv | multiplier | Notes |
|---|---|---|---|---|---|
| Linear | 0.0 | 0.5 | 0.5 | 1.0 | 1:1 mapping, no curvature |
| Neutral | 0.15 | 0.5 | 0.5 | 3.0 | Moderate deadzone, moderate speed |
| Gentle | 0.05 | 0.4 | 0.4 | 2.0 | Slight deadzone, slower response |
| Aggressive | 0.02 | 0.6 | 0.6 | 8.0 | Minimal deadzone, fast response |
| Damped | 0.3 | 0.3 | 0.3 | 4.0 | Large deadzone, slows near center |

**Files affected:** New `curve_template.h/cpp`, `scp_form.ui` (template UI), `scp_form.cpp` (apply/save handlers), config persistence via `PREF`.

---

## Summary of All Changes by File

| File | Phase |
|------|-------|
| `src/qt_gui/ltr.ui` — widget rename + label text | 1 |
| `src/qt_gui/ltr_gui.cpp` — variable/slot name updates | 1 |
| `src/qt_gui/ltr_gui.h` — slot declaration update | 1 |
| `src/qt_gui/scview.h` — wheel/mouse event overrides, zoom state | 2 |
| `src/qt_gui/scview.cpp` — `wheelEvent()`, `mousePressEvent()`, zoom-aware `paintEvent()`/`drawGrid()` | 2 |
| *(future)* `curve_template.h/cpp` — template struct + store | 3C |
| *(future)* `scurve.ui` — X/Y spinboxes, template UI | 3B, 3C |
| *(future)* `scp_form.cpp` — template apply/save handlers | 3C |
