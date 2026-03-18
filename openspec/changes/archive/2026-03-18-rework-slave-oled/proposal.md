## Why

The slave OLED currently uses character-cell text rendering inherited from the original design. The master OLED was already reworked to use pixel-accurate bitmap compositing; this change applies the same approach to the slave half, giving it a consistent implementation and a clean baseline for future content.

## What Changes

- Add a `slave-oled-bitmap-display` rendering path to `oled_bitmap.c` that renders a base display image and composites modifier glyphs (Shift, Control, Super, Alt) at the same bounding boxes used by the master, using `is_keyboard_left()` for side-aware modifier detection
- The slave base image (`display_base_slave.png`) is intentionally blank; only modifier indicator glyphs are composited onto it
- Remove the old text-based slave rendering (WPM row, held-keys row, text modifier rows)
- Refresh the digit glyph byte arrays in `oled_bitmap.c` from their updated source PNGs (`glyph_0_15x9.png` – `glyph_9_15x9.png`)

## Capabilities

### New Capabilities

- `slave-oled-bitmap-display`: Pixel-accurate bitmap rendering for the slave OLED — base image always rendered, modifier indicator glyphs composited at bounding boxes (0,96)–(31,127), side-aware via `is_keyboard_left()`

### Modified Capabilities

- `oled-stats-layout`: Slave half layout changes — text-based WPM, held-keys, and modifier rows are removed; slave now uses bitmap compositing instead
- `keyball64-oled`: Secondary half requirement changes — WPM and held-key display are removed from the secondary half; modifier indicators now use bitmap glyphs rather than inverted text rows

## Impact

- `keyboards/keyball64/lib/oled_bitmap/oled_bitmap.c`: New slave render path, updated digit glyph data
- `openspec/specs/slave-oled-bitmap-display/spec.md`: New spec (created)
- `openspec/specs/oled-stats-layout/spec.md`: Delta — slave half requirements updated
- `openspec/specs/keyball64-oled/spec.md`: Delta — secondary half requirements updated
