## Why

The current master-side OLED display uses QMK's character-based text renderer, which limits layout to a fixed 5-character-wide grid and produces a generic, low-fidelity look. Replacing it with pixel-accurate bitmap rendering allows the display to match a custom-designed layout with purpose-built glyphs for digits and modifier indicators.

## What Changes

- Replace the text-based `oledkit_render_info_user()` implementation in `keymaps/default/keymap.c` with a pixel-addressed bitmap renderer
- Add a `PROGMEM` bitmap data array for the base display image (`display_base_master.png`)
- Add `PROGMEM` bitmap arrays for digit glyphs (0–9, 15×9 px each) and modifier/lock indicator glyphs (16×16 px each: Caps Lock, Num Lock, Shift, Control, Super, Alt)
- Implement centered digit rendering for layer number (bounding box 0,13–31,27) and WPM (bounding box 0,43–31,57), with 2 px spacing between digits and truncation at 3 digits
- Render modifier/lock glyphs into fixed bounding boxes in the lower portion of the screen (y 78–127), conditionally shown based on state
- Remove the old character-row approach; no other existing functionality is changed

## Capabilities

### New Capabilities

- `master-oled-bitmap-display`: Pixel-accurate OLED renderer for the master half, compositing a base image with digit glyphs (layer, WPM) and modifier/lock indicator glyphs

### Modified Capabilities

_(none — this is a new rendering path replacing an internal implementation detail, not a behavioral contract change)_

## Impact

- **Files changed**: `keyboards/keyball64/keymaps/default/keymap.c` (primary), potentially a new `oled_bitmap.h` / `oled_bitmap.c` within the keymap or keyboard lib directory
- **QMK APIs used**: `oled_write_raw_P`, `oled_set_cursor` / `oled_set_pixel` (pixel-level), `PROGMEM` bitmap arrays
- **No firmware feature flag changes** — OLED is already enabled
- **No slave-side changes** — only the master render function is replaced
