## 1. Bitmap Data — Convert PNGs to C Arrays

- [x] 1.1 Write a script (or use an existing tool) to convert each PNG in `keyball_display/` to a row-major `uint8_t` PROGMEM array, 2 bytes per row, MSB = leftmost pixel
- [x] 1.2 Convert `display_base_master.png` (32×128) → `uint8_t` array, 4 bytes per row × 128 rows = 512 bytes
- [x] 1.3 Convert digit glyphs 0–9 (`glyph_N_15x9.png`, actual dimensions 9×15) → ten `uint8_t[30]` PROGMEM arrays (2 bytes × 15 rows each)
- [x] 1.4 Convert indicator glyphs (capslock, numlock, shift, control, super, alt, `*_16x16.png`) → six `uint8_t[32]` PROGMEM arrays (2 bytes × 16 rows each)

## 2. Rendering Infrastructure

- [x] 2.1 Add `draw_bitmap(x0, y0, data, w, h)` helper in `keymap.c` that iterates row-major bytes via `pgm_read_byte` and calls `oled_write_pixel`
- [x] 2.2 Add `draw_number(value, box_x, box_y)` helper that extracts up to 3 digits (mod 1000), computes centered x_start (`box_x + (32 - total_w) / 2` where `total_w = n * 9 + (n-1) * 2`), and calls `draw_bitmap` for each digit glyph (9×15) with 2px gaps

## 3. Replace Master OLED Renderer

- [x] 3.1 In `oledkit_render_info_user()`, replace all existing `oled_write_P` calls with a call to render the base image via `draw_bitmap` at x=0, y=0
- [x] 3.2 Call `draw_number` for layer number with bounding box (0, 13, 32, 15) — passes current `get_highest_layer(layer_state)`
- [x] 3.3 Call `draw_number` for WPM with bounding box (0, 43, 32, 15) — passes `get_current_wpm()`
- [x] 3.4 Conditionally call `draw_bitmap` for Caps Lock glyph at (0, 78) when `host_keyboard_led_state().caps_lock` is true
- [x] 3.5 Conditionally call `draw_bitmap` for Num Lock glyph at (16, 78) when `host_keyboard_led_state().num_lock` is true
- [x] 3.6 Read `get_mods() | get_oneshot_mods()` and conditionally render Shift glyph at (0, 96) when any Shift bit is set
- [x] 3.7 Conditionally render Control glyph at (16, 96) when any Control bit is set
- [x] 3.8 Conditionally render Super glyph at (0, 112) when any GUI bit is set
- [x] 3.9 Conditionally render Alt glyph at (16, 112) when any Alt bit is set

## 4. Verification

- [x] 4.1 Compile with `qmk compile -kb keyball64 -km default` and confirm no errors or warnings
- [x] 4.2 Flash firmware and visually confirm base image appears on master OLED
- [x] 4.3 Verify layer number glyph renders in the correct bounding box and updates when layers change
- [x] 4.4 Verify WPM glyph renders in the correct bounding box and updates with typing activity
- [x] 4.5 Verify Caps Lock and Num Lock glyphs appear/disappear correctly
- [x] 4.6 Verify Shift, Control, Super, and Alt glyphs appear when the respective modifiers are held and clear when released
