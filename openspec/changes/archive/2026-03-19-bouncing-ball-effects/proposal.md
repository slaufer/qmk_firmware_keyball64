## Why

The bouncing ball on the slave OLED is visually simple; adding a motion trail that grows with typing speed makes the animation more dynamic and gives users a satisfying visual indicator of their WPM.

## What Changes

- Add `draw_bitmap_masked()` to `oled_bitmap.c/h` — composites a bitmap using a PROGMEM mask, writing only pixels where the mask is ON.
- Apply `glyph_ball_8x8_mask.png` to the existing ball glyph render, replacing the plain `draw_bitmap` call.
- Track a circular history of the last 3 ball positions in `oled_bitmap.c`; extend the `BALL_POS_SYNC` RPC payload to include all 4 positions (current + 3 history) plus a WPM-derived trail count.
- Render 0–3 trail segments behind the ball using `glyph_ball_trail_8x8.png` composited with randomly-selected masks from `glyph_ball_trail{1,2,3}_8x8_mask{N}.png` (4 variants each), chosen via `random8()`.
- Add new PROGMEM bitmap arrays to `oled_bitmap.c` for the trail glyph and its 12 mask variants (3 sets × 4 masks).
- Update `png_to_c.py` to include all new PNG assets.

## Capabilities

### New Capabilities

- `bouncing-ball-trail`: WPM-gated trail effect behind the bouncing ball — tracks history positions, selects trail length from WPM tier (0/1/2/3 segments), renders each segment with the appropriate masked trail glyph using a random variant.

### Modified Capabilities

- `bouncing-ball`: Ball glyph is now rendered with `draw_bitmap_masked()` using `glyph_ball_8x8_mask.png`; sync payload expands to carry 3 history positions alongside the current position.

## Impact

- `keyboards/keyball64/lib/oled_bitmap/oled_bitmap.c` — new bitmaps, `draw_bitmap_masked()`, trail history state, updated render logic.
- `keyboards/keyball64/lib/oled_bitmap/oled_bitmap.h` — new `draw_bitmap_masked()` declaration; updated get/set API to expose history.
- `keyboards/keyball64/keymaps/via/keymap.c` — expanded `BALL_POS_SYNC` payload (8 bytes: 4 × (x,y) positions); update `housekeeping_task_user` to send history.
- `keyboards/keyball64/keymaps/default/keymap.c` — same sync changes as `via`.
- `/home/slaufer/repos/holykeebs/keyball_display/png_to_c.py` — add 13 new entries (1 trail glyph + 12 masks).
