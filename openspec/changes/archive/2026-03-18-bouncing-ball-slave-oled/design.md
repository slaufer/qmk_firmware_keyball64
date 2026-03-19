## Context

The keyball64 split keyboard has a 32×128 pixel OLED on each half. The slave OLED renders via `oled_task_user()` in `oled_bitmap.c`, which currently draws a blank background and four modifier key glyphs in the bottom 32 rows (y 96–127). The top 96 rows (y 0–95) are unused on the slave.

The existing `draw_bitmap()` function handles pixel-accurate glyph rendering. Key press events flow through QMK's `process_record_user()` callback in each keymap.

## Goals / Non-Goals

**Goals:**
- Render an 8×8 ball bitmap that bounces within x 0–31, y 0–95 on the slave OLED
- Advance the ball by 2px diagonally on every key press (any key, any side)
- Correctly reverse direction on boundary contact, including simultaneous corner collisions
- Keep all ball logic self-contained inside `oled_bitmap.c` behind a single call: `oled_ball_on_key_press()`

**Non-Goals:**
- Animation driven by time (timer-based movement) — key-press-driven only
- Any interaction with master OLED
- Modifying the existing modifier glyph rendering

## Decisions

### Ball bitmap stored as PROGMEM byte array

The ball graphic from `keyball_display/glyph_ball_8x8.png` will be converted to an 8×8 PROGMEM byte array (8 bytes, 1 byte/row, MSB = leftmost pixel) using the existing `keyball_display/png_to_c.py` script. The entry `('glyph_ball_8x8.png', 'bmp_ball')` must be added to that script's `files` list before running it. The resulting array is stored directly in `oled_bitmap.c`, consistent with all other glyphs.

**Alternatives considered:** Manual byte-by-byte conversion — rejected; `png_to_c.py` already exists for exactly this purpose.

### Ball state as static variables in oled_bitmap.c

`ball_x`, `ball_y`, `ball_dx`, `ball_dy` are `static` module-level variables. No struct or new header type needed. Initial state: position (0, 0), direction (+2, +2).

**Alternatives considered:** Storing state in keymap — rejected; keeps ball logic cohesive and avoids polluting keymap files with display concerns.

### Public API: `oled_ball_on_key_press()`

A single function declared in `oled_bitmap.h` and defined in `oled_bitmap.c`. Keymaps call this from `process_record_user()` on `record->event.pressed`. The function updates ball position/direction; `oled_task_user()` renders the current position on every frame.

**Alternatives considered:** Advancing the ball inside `oled_task_user()` directly — rejected; the task runs on a timer, not key events.

### Collision using ball top-left position against clamped bounds

The bounding box is x 0–31, y 0–95. The ball is 8×8. Valid top-left positions are x ∈ [0, 23], y ∈ [0, 87] (i.e., max = bound_max − ball_size + 1 = 32 − 8 = 24 exclusive, so max inclusive = 23; similarly 96 − 8 = 88 exclusive → 87 inclusive).

Collision check after applying delta:
- If new_x < 0 or new_x > 23 → reverse `dx`, clamp x
- If new_y < 0 or new_y > 87 → reverse `dy`, clamp y

Both axes checked independently so corner collisions work correctly.

## Risks / Trade-offs

- **[Risk] Ball rendering flickers** → The full slave base image is redrawn every frame; the ball is drawn on top each frame. No double-buffering exists. Acceptable — existing modifier glyphs already work this way.
- **[Risk] `oled_ball_on_key_press()` called on master half** → Function is a no-op unless `!is_keyboard_master()`, or simply always safe to call (state updates are cheap). We'll guard with `#if defined(OLED_ENABLE)` to match the rest of the file.
- **[Risk] Key repeat sends many events quickly** → Ball advances once per physical press event (`record->event.pressed == true` only), so held keys produce one advance, not continuous movement.

## Migration Plan

No schema or data migration required. Changes are additive:
1. Add ball bitmap and state to `oled_bitmap.c`
2. Add `oled_ball_on_key_press()` declaration to `oled_bitmap.h`
3. Call `oled_ball_on_key_press()` in both keymap files' `process_record_user()`

Rollback: revert the three file changes.
